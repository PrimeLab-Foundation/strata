#include "python_convert.h"
#include "python_document.h"
#include "python_types.h"
#include "strata/json/json_document.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_parser_inline.hpp"
#include "strata/json/json_sax_handler.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/util/simd_string.hpp"

#include <cstring>
#include <string>
#include <vector>

#if defined(__ARM_NEON) || defined(__aarch64__)
#include <arm_neon.h>
#endif

// Forward-declare CPython internal function for hash-aware dict insertion.
// This function is exported in libpython (PyAPI_FUNC) but the header guard
// requires Py_BUILD_CORE. Forward-declaring lets us link without that define.
// Available since CPython 3.6+. Falls back to PyDict_SetItem if unavailable.
extern "C" {
int _PyDict_SetItem_KnownHash(PyObject* mp, PyObject* key, PyObject* item, Py_hash_t hash);
}

namespace {

// Cached key entry: interned PyObject* + pre-computed hash.
// Storing the hash alongside the key avoids recomputing it during
// _PyDict_SetItem_KnownHash, saving ~10-15% of dict insertion cost.
struct CachedKeyEntry {
    PyObject* key;
    Py_hash_t hash;
};

// Cursor-predicted key cache with flat storage.
//
// Key insight: for JSON arrays of same-schema objects (the dominant pattern),
// keys repeat in the same order for every record.  A cursor that tracks
// position in the key sequence gives O(1) amortized lookup:
//
//   First record:  O(k²) total — k keys, each scanned linearly (cold cache)
//   All others:    O(k) total — each lookup is a single comparison at cursor
//
// For 500 records × 21 keys: ~10,500 lookups, ~10,500 comparisons (vs ~105,000
// with unordered_map or linear scan).  The cursor also gives better cache
// locality than std::unordered_map's chained buckets.
class KeyCache {
  public:
    KeyCache() {
        entries_.reserve(32);
        // Initialize hash table slots to empty
        std::memset(hash_slots_, 0xFF, sizeof(hash_slots_));
    }

    /// Look up or create a cached key entry.
    ///
    /// Returns a **borrowed** CachedKeyEntry — the caller must NOT Py_DECREF
    /// the key.  The cache owns the reference; _PyDict_SetItem_KnownHash
    /// will INCREF internally when inserting into a dict.
    CachedKeyEntry get(std::string_view key) {
        const size_t n = entries_.size();

        // Fast path: cursor prediction — O(1) for repeated key patterns.
        // After the first record, the cursor cycles through entries in the
        // same order that keys appear.  For same-schema objects this means
        // every lookup is a single memcmp hit.
        if (n > 0) {
            auto& e = entries_[cursor_];
            if (e.key_len == static_cast<uint16_t>(key.size()) &&
                std::memcmp(e.key_data, key.data(), key.size()) == 0) {
                // Borrowed ref — no INCREF needed.
                cursor_ = (cursor_ + 1 < n) ? cursor_ + 1 : 0;
                return e.cached;
            }

            // Prediction miss: use hash table for O(1) lookup.
            // FNV-1a hash of key bytes, masked to table size.
            uint32_t h = 2166136261u;
            for (size_t ki = 0; ki < key.size(); ++ki)
                h = (h ^ static_cast<unsigned char>(key.data()[ki])) * 16777619u;
            uint32_t slot = h & kHashMask;

            // Linear probing (table is always < 50% full for n ≤ 64)
            for (int probe = 0; probe < kHashSlots; ++probe) {
                uint8_t idx = hash_slots_[slot];
                if (idx == 0xFF)
                    break; // empty slot → key not in cache
                if (idx < n) {
                    auto& s = entries_[idx];
                    if (s.key_len == static_cast<uint16_t>(key.size()) &&
                        std::memcmp(s.key_data, key.data(), key.size()) == 0) {
                        cursor_ = (idx + 1 < n) ? idx + 1 : 0;
                        return s.cached;
                    }
                }
                slot = (slot + 1) & kHashMask;
            }
        }

        // Not found: intern the key and add to cache.
        PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
        if (!py_key)
            return {nullptr, 0};

        PyUnicode_InternInPlace(&py_key);
        Py_hash_t hash = PyObject_Hash(py_key);

        CachedKeyEntry cached{py_key, hash};
        size_t new_idx = entries_.size();

        // Cap entries to avoid hash table overload (75% load factor).
        // Beyond this, we still return the correct key but don't cache it.
        if (new_idx >= kMaxEntries) {
            // Return uncached entry — caller still gets a valid key+hash,
            // but we must transfer ownership: the caller will use it once
            // and we won't track it. Actually, we need to own it for cleanup.
            // Just grow the entries vector but skip hash table insertion.
            entries_.push_back({std::string(key), nullptr, 0, cached});
            auto& back = entries_.back();
            back.key_data = back.key_storage.data();
            back.key_len = static_cast<uint16_t>(key.size());
            return cached;
        }

        entries_.push_back({std::string(key), nullptr, 0, cached});
        auto& back = entries_.back();
        back.key_data = back.key_storage.data();
        back.key_len = static_cast<uint16_t>(key.size());

        // Insert into hash table
        uint32_t h = 2166136261u;
        for (size_t ki = 0; ki < key.size(); ++ki)
            h = (h ^ static_cast<unsigned char>(key.data()[ki])) * 16777619u;
        uint32_t slot = h & kHashMask;
        while (hash_slots_[slot] != 0xFF)
            slot = (slot + 1) & kHashMask;
        hash_slots_[slot] = static_cast<uint8_t>(new_idx);

        return cached;
    }

    ~KeyCache() {
        for (auto& e : entries_) {
            Py_DECREF(e.cached.key);
        }
    }

    struct Entry {
        std::string key_storage; // owning storage for key bytes
        const char* key_data;    // points into key_storage (for fast access)
        uint16_t key_len;        // cached length for quick comparison
        CachedKeyEntry cached;   // {interned PyObject*, pre-computed hash}
    };

    // Public access for speculative key matching in the parser.
    const std::vector<Entry>& entries() const { return entries_; }
    size_t cursor() const { return cursor_; }
    void advance_cursor() { cursor_ = (cursor_ + 1 < entries_.size()) ? cursor_ + 1 : 0; }

  private:
    static constexpr int kHashSlots = 128; // must be power of 2
    static constexpr uint32_t kHashMask = kHashSlots - 1;
    static constexpr size_t kMaxEntries = 96; // 75% load — stop hash inserts beyond this

    std::vector<Entry> entries_;
    size_t cursor_ = 0;
    uint8_t hash_slots_[kHashSlots]; // index into entries_, 0xFF = empty
};

// Small integer cache: avoid PyLong_FromLongLong overhead for 0..256.
// CPython internally caches -5..256, but the function call + range check
// costs ~10 cycles each time. Our inline check + direct array lookup
// eliminates that overhead on the hot path.
static constexpr int kSmallIntMax = 256;
static PyObject* g_small_int_cache[kSmallIntMax + 1] = {};
static bool g_small_int_cache_ready = false;

static void ensure_small_int_cache() {
    if (g_small_int_cache_ready)
        return;
    for (int i = 0; i <= kSmallIntMax; ++i) {
        g_small_int_cache[i] = PyLong_FromLong(i);
        // These are permanent references — never decref'd.
    }
    g_small_int_cache_ready = true;
}

class PythonObjectBuilder : public strata::JsonSaxHandler {
  public:
    explicit PythonObjectBuilder() {
        stack_.reserve(32);
        keys_.reserve(32);
        array_items_.reserve(256);
        array_starts_.reserve(16);
        depth_sizes_.reserve(8);
        policy_ = strata::get_duplicate_key_policy();
        ensure_small_int_cache();
    }

    ~PythonObjectBuilder() {
        if (root_) {
            Py_DECREF(root_);
        }
        for (auto obj : stack_) {
            if (obj)
                Py_DECREF(obj);
        }
        for (auto* p : array_items_) {
            Py_DECREF(p);
        }
    }

    bool on_null() override {
        Py_INCREF(Py_None);
        return push_value(Py_None);
    }

    bool on_bool(bool v) override {
        PyObject* obj = v ? Py_True : Py_False;
        Py_INCREF(obj);
        return push_value(obj);
    }

    bool on_int(int64_t v) override {
        // Fast path: small integers (0..256) are pre-cached — avoids
        // PyLong_FromLongLong function call overhead on the hot path.
        if (static_cast<uint64_t>(v) <= static_cast<uint64_t>(kSmallIntMax)) {
            PyObject* cached = g_small_int_cache[v];
            Py_INCREF(cached);
            return push_value(cached);
        }
        return push_value(PyLong_FromLongLong(v));
    }

    bool on_uint(uint64_t v) override {
        if (v <= static_cast<uint64_t>(kSmallIntMax)) {
            PyObject* cached = g_small_int_cache[v];
            Py_INCREF(cached);
            return push_value(cached);
        }
        return push_value(PyLong_FromUnsignedLongLong(v));
    }

    bool on_double(double v) override { return push_value(PyFloat_FromDouble(v)); }

    bool on_string(std::string_view v) override {
        // Fast path for ASCII strings: skip CPython's internal codepoint scan
        // by constructing compact-ASCII directly. Most JSON string values are
        // pure ASCII (no \uXXXX escapes decoding to non-ASCII).
        const size_t slen = v.size();
        const char* sdata = v.data();

        // Short strings (≤16 bytes, the common case): scalar OR-accumulation
        // is cheaper than NEON setup for these sizes.
        if (LIKELY(slen <= 16)) {
            unsigned char high = 0;
            for (size_t si = 0; si < slen; ++si)
                high |= static_cast<unsigned char>(sdata[si]);
            if (LIKELY(high < 0x80)) {
                PyObject* s = PyUnicode_New(static_cast<Py_ssize_t>(slen), 127);
                if (!s)
                    return false;
                memcpy(PyUnicode_1BYTE_DATA(s), sdata, slen);
                return push_value(s);
            }
            return push_value(PyUnicode_DecodeUTF8(sdata, static_cast<Py_ssize_t>(slen), NULL));
        }

        // Longer strings: SIMD ASCII check
        bool is_ascii = true;
        size_t si = 0;
#if defined(__ARM_NEON) || defined(__aarch64__)
        for (; si + 16 <= slen; si += 16) {
            uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(sdata + si));
            if (vmaxvq_u8(chunk) >= 0x80) {
                is_ascii = false;
                break;
            }
        }
#endif
        if (is_ascii) {
            for (; si < slen; ++si) {
                if (static_cast<unsigned char>(sdata[si]) >= 0x80) {
                    is_ascii = false;
                    break;
                }
            }
        }

        if (LIKELY(is_ascii)) {
            PyObject* s = PyUnicode_New(static_cast<Py_ssize_t>(slen), 127);
            if (!s)
                return false;
            memcpy(PyUnicode_1BYTE_DATA(s), sdata, slen);
            return push_value(s);
        }
        return push_value(PyUnicode_DecodeUTF8(sdata, static_cast<Py_ssize_t>(slen), NULL));
    }

    bool on_start_object(size_t /*size_hint*/) override {
        // Predict dict size from previous object at the same nesting depth.
        // For arrays of same-schema objects (the dominant JSON pattern),
        // every object has the same key count, so this gives a perfect prediction.
        // Eliminates dict resize cascades: e.g. for 21 keys, avoids 8→16→32.
        size_t depth = stack_.size();
        PyObject* dict;
        if (depth < depth_sizes_.size() && depth_sizes_[depth] > 0) {
            dict = _PyDict_NewPresized(static_cast<Py_ssize_t>(depth_sizes_[depth]));
        } else {
            dict = PyDict_New();
        }
        if (!dict)
            return false;
        stack_.push_back(dict);
        return true;
    }

    bool on_key(std::string_view v) override {
        CachedKeyEntry entry = cache_.get(v);
        if (!entry.key)
            return false;
        keys_.push_back(entry);
        return true;
    }

    /// Speculative key match: called by the parser before SIMD string scanning.
    ///
    /// Compares the raw JSON bytes (after the opening quote) against the
    /// cursor-predicted key in the cache.  If the bytes match and the next
    /// byte is a closing quote, we skip the full parse_string() path:
    /// - No SIMD escape scan (find_next_escape_simd)
    /// - No cache lookup (already at the cursor)
    /// - No PyUnicode_FromStringAndSize (reuse cached PyObject)
    ///
    /// @param data  Bytes after the opening '"'.
    /// @param remaining  Available bytes after the opening '"'.
    /// @return Number of bytes consumed (key_len + 1 for closing quote),
    ///         or 0 if prediction failed (fall back to full parsing).
    size_t try_match_key(const char* data, size_t remaining) {
        const auto& entries = cache_.entries();
        const size_t n = entries.size();
        if (n == 0)
            return 0;

        const auto& e = entries[cache_.cursor()];
        const size_t key_len = e.key_len;

        // Need key_len bytes + closing quote
        if (key_len + 1 > remaining)
            return 0;

        // Compare key bytes
        if (std::memcmp(data, e.key_data, key_len) != 0)
            return 0;

        // Check closing quote
        if (data[key_len] != '"')
            return 0;

        // Match! Push the cached key (borrowed ref) and advance cursor.
        keys_.push_back(e.cached);
        cache_.advance_cursor();
        return key_len + 1; // consumed: key bytes + closing quote
    }

    bool on_end_object() override {
        if (stack_.empty())
            return false;
        PyObject* dict = stack_.back();
        // Record the object size for depth-based prediction.
        size_t depth = stack_.size() - 1;
        Py_ssize_t sz = PyDict_GET_SIZE(dict);
        if (depth >= depth_sizes_.size()) {
            depth_sizes_.resize(depth + 1, 0);
        }
        depth_sizes_[depth] = static_cast<size_t>(sz);
        stack_.pop_back();
        return push_value(dict);
    }

    // Array building: collect items in a single flat C++ vector, tracked by start
    // indices.  At on_end_array, build PyList_New(n) + PyList_SET_ITEM (steals ref,
    // no INCREF/DECREF).  Uses one flat vector to avoid per-array allocation overhead.
    bool on_start_array(size_t /*size_hint*/) override {
        stack_.push_back(nullptr); // nullptr sentinel = "building an array"
        array_starts_.push_back(array_items_.size());
        return true;
    }

    bool on_end_array() override {
        if (stack_.empty() || stack_.back() != nullptr)
            return false;
        stack_.pop_back();

        size_t start = array_starts_.back();
        array_starts_.pop_back();
        size_t count = array_items_.size() - start;

        PyObject* list = PyList_New(static_cast<Py_ssize_t>(count));
        if (!list) {
            for (size_t i = start; i < array_items_.size(); ++i) {
                Py_DECREF(array_items_[i]);
            }
            array_items_.resize(start);
            return false;
        }
        for (size_t i = 0; i < count; ++i) {
            PyList_SET_ITEM(list, static_cast<Py_ssize_t>(i), array_items_[start + i]);
        }
        array_items_.resize(start);
        return push_value(list);
    }

    PyObject* take_root() {
        PyObject* res = root_;
        root_ = nullptr;
        return res;
    }

    // Reset for reuse across multiple parse calls (e.g. NDJSON batches).
    // Clears parse state but keeps KeyCache and depth_sizes_ populated
    // for cross-line key reuse and dict pre-sizing prediction.
    void reset() {
        if (root_) {
            Py_DECREF(root_);
            root_ = nullptr;
        }
        for (auto obj : stack_) {
            if (obj)
                Py_DECREF(obj);
        }
        stack_.clear();
        // keys_ contains borrowed refs from KeyCache — no DECREF needed.
        keys_.clear();
        for (auto* p : array_items_) {
            Py_DECREF(p);
        }
        array_items_.clear();
        array_starts_.clear();
        policy_ = strata::get_duplicate_key_policy();
    }

  private:
    bool push_value(PyObject* val) {
        if (!val)
            return false;

        if (stack_.empty()) {
            if (root_) {
                Py_DECREF(root_);
            }
            root_ = val;
            return true;
        }

        PyObject* top = stack_.back();
        if (top == nullptr) {
            // Building an array — collect into C++ vector (no Python API calls).
            array_items_.push_back(val);
            return true;
        }

        // Must be a dict.
        if (keys_.empty()) {
            Py_DECREF(val);
            return false;
        }
        CachedKeyEntry key_entry = keys_.back();
        keys_.pop_back();

        // Keys are borrowed from the KeyCache — no INCREF/DECREF needed.
        // _PyDict_SetItem_KnownHash and PyDict_SetDefault INCREF the key
        // internally, so the dict holds its own reference.

        // Hot path: LastWins (default policy) with pre-computed hash.
        if (policy_ == strata::DuplicateKeyPolicy::LastWins) {
            if (_PyDict_SetItem_KnownHash(top, key_entry.key, val, key_entry.hash) < 0) {
                Py_DECREF(val);
                return false;
            }
            Py_DECREF(val);
            return true;
        }

        if (policy_ == strata::DuplicateKeyPolicy::FirstWins) {
            PyObject* result = PyDict_SetDefault(top, key_entry.key, val);
            if (!result) {
                Py_DECREF(val);
                return false;
            }
            Py_DECREF(val);
            return true;
        }

        // Warn / Error: need to detect duplicate first
        if (PyDict_Contains(top, key_entry.key)) {
            if (policy_ == strata::DuplicateKeyPolicy::Warn) {
                PyObject* key_repr = PyObject_Repr(key_entry.key);
                const char* key_str = PyUnicode_AsUTF8(key_repr);
                std::string msg = "Duplicate key encountered: ";
                msg += key_str;
                PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
                Py_XDECREF(key_repr);
                Py_DECREF(val);
                return true;
            }
            Py_DECREF(val);
            return false;
        }

        if (_PyDict_SetItem_KnownHash(top, key_entry.key, val, key_entry.hash) < 0) {
            Py_DECREF(val);
            return false;
        }
        Py_DECREF(val);
        return true;
    }

    PyObject* root_ = nullptr;
    std::vector<PyObject*> stack_;
    std::vector<CachedKeyEntry> keys_;   // borrowed key refs + pre-computed hash
    std::vector<PyObject*> array_items_; // flat storage for all in-flight array items
    std::vector<size_t> array_starts_;   // stack of start indices into array_items_
    std::vector<size_t> depth_sizes_;    // predicted object size at each nesting depth
    strata::DuplicateKeyPolicy policy_;
    KeyCache cache_;
};

} // namespace

static void emit_duplicate_key_warnings() {
    auto warnings = strata::consume_parse_warnings();
    for (const auto& msg : warnings) {
        PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
    }
}

static PyObject* json_value_to_python_internal(const strata::JsonValue& val, KeyCache& cache) {
    if (val.is_null()) {
        Py_RETURN_NONE;
    }

    if (val.is_bool()) {
        PyObject* result = val.as_bool() ? Py_True : Py_False;
        Py_INCREF(result);
        return result;
    }

    if (val.is_number()) {
        return PyFloat_FromDouble(val.as_number());
    }

    if (val.is_string()) {
        const std::string& s = val.as_string();
        return PyUnicode_FromStringAndSize(s.c_str(), s.size());
    }

    if (val.is_array()) {
        const auto& arr = val.as_array();
        PyObject* list = PyList_New(arr.size());
        if (!list)
            return NULL;
        for (size_t i = 0; i < arr.size(); ++i) {
            PyObject* item = json_value_to_python_internal(arr[i], cache);
            if (!item) {
                Py_DECREF(list);
                return NULL;
            }
            PyList_SET_ITEM(list, i, item);
        }
        return list;
    }

    if (val.is_object()) {
        const auto& obj = val.as_object();
        PyObject* dict = _PyDict_NewPresized(static_cast<Py_ssize_t>(obj.size()));
        if (!dict)
            return NULL;

        for (const auto& [key, value] : obj) {
            // Borrowed ref from cache — no DECREF needed.
            CachedKeyEntry entry = cache.get(key);
            if (!entry.key) {
                Py_DECREF(dict);
                return NULL;
            }

            PyObject* py_val = json_value_to_python_internal(value, cache);
            if (!py_val) {
                Py_DECREF(dict);
                return NULL;
            }

            int rc = _PyDict_SetItem_KnownHash(dict, entry.key, py_val, entry.hash);
            Py_DECREF(py_val);

            if (rc < 0) {
                Py_DECREF(dict);
                return NULL;
            }
        }
        return dict;
    }

    Py_RETURN_NONE;
}

// Convert JsonValue to PyObject
PyObject* json_value_to_python(const strata::JsonValue& val) {
    KeyCache cache;
    return json_value_to_python_internal(val, cache);
}

// Helper: parse JSON text directly to a Python object via SAX (no intermediate C++ DOM).
// Uses parse_sax_inline<PythonObjectBuilder> for devirtualised, inlineable parsing.
// validate_utf8=false is safe when the caller creates PyUnicode objects, which validate inline.
PyObject* parse_json_to_python(std::string_view text, bool validate_utf8) {
    PythonObjectBuilder builder;
    auto status = strata::parse_sax_inline(text, builder, validate_utf8);
    if (status != strata::Status::Ok) {
        return nullptr; // caller must set the Python exception
    }
    return builder.take_root();
}

// Reusable-builder variant: caller provides a PythonObjectBuilder that persists across
// calls (e.g. inside an NDJSON batch loop). The builder is reset() before each parse so
// that the KeyCache is reused across lines (avoids recreating the same Python key strings
// 5000 times in a typical NDJSON workload).
PyObject* parse_json_to_python_reuse(std::string_view text, bool validate_utf8,
                                     PythonObjectBuilder& builder) {
    builder.reset();
    auto status = strata::parse_sax_inline(text, builder, validate_utf8);
    if (status != strata::Status::Ok) {
        return nullptr;
    }
    return builder.take_root();
}

// NDJSON batch functions: reuse a single PythonObjectBuilder (and its KeyCache) across all
// lines in the batch, avoiding per-line vector/map construction and Python key re-creation.

// Direct NDJSON parsing: bypass NdjsonStream overhead for the all-at-once case.
// Uses SIMD newline counting for pre-allocation, then parses lines directly
// from the raw buffer without per-line whitespace checks.
PyObject* parse_ndjson_direct(const char* data, size_t len, int skip_errors) {
    PyGcPause gc_pause;
    PythonObjectBuilder builder;

    // SIMD-count newlines for precise pre-allocation
    size_t line_count = strata::util::count_newlines_simd(data, len) + 1;

    std::vector<PyObject*> items;
    items.reserve(line_count);

    size_t pos = 0;
    while (pos < len) {
        // Find next newline using SIMD
        size_t nl = strata::util::find_newline_simd(data, len, pos);
        size_t line_end = nl;

        // Handle \r\n
        if (line_end > pos && data[line_end - 1] == '\r')
            --line_end;

        size_t line_len = line_end - pos;

        // Skip empty/whitespace lines cheaply: check first char
        if (line_len == 0 || static_cast<unsigned char>(data[pos]) <= ' ') {
            // Only do full whitespace check if first char is whitespace
            if (line_len == 0 || strata::util::is_whitespace_only_simd(data + pos, line_len)) {
                pos = (nl < len) ? nl + 1 : len;
                continue;
            }
        }

        std::string_view line(data + pos, line_len);
        PyObject* item = parse_json_to_python_reuse(line, /*validate_utf8=*/false, builder);
        if (!item) {
            if (PyErr_Occurred())
                PyErr_Clear();
            if (skip_errors) {
                pos = (nl < len) ? nl + 1 : len;
                continue;
            }
            break;
        }
        items.push_back(item);
        pos = (nl < len) ? nl + 1 : len;
    }

    // Build the Python list in one allocation with SET_ITEM (no extra INCREF)
    PyObject* result_list = PyList_New(static_cast<Py_ssize_t>(items.size()));
    if (!result_list) {
        for (auto* obj : items)
            Py_DECREF(obj);
        return NULL;
    }
    for (size_t i = 0; i < items.size(); ++i) {
        PyList_SET_ITEM(result_list, static_cast<Py_ssize_t>(i), items[i]); // steals ref
    }
    return result_list;
}

PyObject* parse_ndjson_all_to_python(strata::NdjsonStream& stream, int skip_errors) {
    PyGcPause gc_pause;
    PythonObjectBuilder builder;

    // Collect parsed items into a temporary vector, then build the list once.
    // This avoids repeated PyList_Append resizing and extra INCREF/DECREF.
    std::vector<PyObject*> items;
    items.reserve(256);

    while (stream.has_next()) {
        std::string_view line = stream.read_raw_line();
        if (line.empty())
            break;

        PyObject* item = parse_json_to_python_reuse(line, /*validate_utf8=*/false, builder);
        if (!item) {
            if (PyErr_Occurred())
                PyErr_Clear();
            stream.record_error();
            if (skip_errors)
                continue;
            // On error without skip: stop parsing, return what we have so far
            break;
        }
        items.push_back(item);
    }

    // Build the Python list in one allocation with SET_ITEM (no extra INCREF)
    PyObject* result_list = PyList_New(static_cast<Py_ssize_t>(items.size()));
    if (!result_list) {
        for (auto* obj : items)
            Py_DECREF(obj);
        return NULL;
    }
    for (size_t i = 0; i < items.size(); ++i) {
        PyList_SET_ITEM(result_list, static_cast<Py_ssize_t>(i), items[i]); // steals ref
    }
    return result_list;
}

PyObject* parse_ndjson_batch_to_python(strata::NdjsonStream& stream, Py_ssize_t batch_size,
                                       int skip_errors) {
    PyObject* result_list = PyList_New(0);
    if (!result_list)
        return NULL;

    PyGcPause gc_pause;
    PythonObjectBuilder builder;

    Py_ssize_t count = 0;
    while (count < batch_size && stream.has_next()) {
        std::string_view line = stream.read_raw_line();
        if (line.empty())
            break;

        PyObject* item = parse_json_to_python_reuse(line, /*validate_utf8=*/false, builder);
        if (!item) {
            if (PyErr_Occurred())
                PyErr_Clear();
            stream.record_error();
            if (skip_errors)
                continue;
            break;
        }

        if (PyList_Append(result_list, item) < 0) {
            Py_DECREF(item);
            Py_DECREF(result_list);
            return NULL;
        }
        Py_DECREF(item);
        count++;
    }

    return result_list;
}

// Python parse_ndjson() — parse all NDJSON lines into a list in one C++ call.
// Uses direct buffer parsing (bypass NdjsonStream) for maximum throughput.
PyObject* strata_parse_ndjson(PyObject* self, PyObject* args) {
    const char* data;
    Py_ssize_t len;
    int skip_errors = 0;

    // Fast path: single str argument, no skip_errors
    if (PyTuple_GET_SIZE(args) >= 1) {
        PyObject* arg = PyTuple_GET_ITEM(args, 0);
        if (PyUnicode_Check(arg)) {
            Py_ssize_t py_len;
            data = PyUnicode_AsUTF8AndSize(arg, &py_len);
            if (!data)
                return NULL;
            len = py_len;
        } else if (PyBytes_Check(arg)) {
            data = PyBytes_AS_STRING(arg);
            len = PyBytes_GET_SIZE(arg);
        } else {
            PyErr_SetString(PyExc_TypeError, "parse_ndjson() argument must be str or bytes");
            return NULL;
        }

        if (PyTuple_GET_SIZE(args) >= 2) {
            PyObject* se = PyTuple_GET_ITEM(args, 1);
            skip_errors = PyObject_IsTrue(se);
        }

        STRATA_CPP_TRY
        return parse_ndjson_direct(data, static_cast<size_t>(len), skip_errors);
        STRATA_CPP_CATCH
    }

    if (!PyArg_ParseTuple(args, "s#|p", &data, &len, &skip_errors)) {
        return NULL;
    }

    STRATA_CPP_TRY
    return parse_ndjson_direct(data, static_cast<size_t>(len), skip_errors);
    STRATA_CPP_CATCH
}

// Forward declarations for iterator support
extern PyObject* create_list_iterator(PyObject* list);
extern PyObject* create_dict_iterator(PyObject* dict);

// Forward declaration for cursor creation
extern PyObject* strata_parse_json_file(PyObject* self, PyObject* args);

// Python loads() function with return_type and iterator kwargs
PyObject* strata_loads(PyObject* self, PyObject* args, PyObject* kwargs) {
    const char* data;
    Py_ssize_t len;

    // Fast path: no kwargs → skip keyword parsing, strcmp, etc.
    // This is the hot path for NDJSON per-line parsing (2000+ calls).
    if (kwargs == NULL || PyDict_GET_SIZE(kwargs) == 0) {
        // Direct tuple access — avoids PyArg_ParseTuple format string parsing.
        if (PyTuple_GET_SIZE(args) != 1) {
            PyErr_SetString(PyExc_TypeError, "loads() takes exactly 1 argument");
            return NULL;
        }
        PyObject* arg = PyTuple_GET_ITEM(args, 0);
        Py_ssize_t py_len;
        if (PyUnicode_Check(arg)) {
            data = PyUnicode_AsUTF8AndSize(arg, &py_len);
            if (!data)
                return NULL;
            len = py_len;
        } else if (PyBytes_Check(arg)) {
            data = PyBytes_AS_STRING(arg);
            len = PyBytes_GET_SIZE(arg);
        } else {
            PyErr_SetString(PyExc_TypeError, "loads() argument must be str or bytes");
            return NULL;
        }

        STRATA_CPP_TRY

        PyGcPause gc_pause;
        static thread_local PythonObjectBuilder* tl_builder = new PythonObjectBuilder();
        PyObject* result = parse_json_to_python_reuse(std::string_view(data, len),
                                                      /*validate_utf8=*/false, *tl_builder);
        if (!result) {
            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            }
            return NULL;
        }
        return result;

        STRATA_CPP_CATCH
    }

    const char* return_type = "dict";
    int iterator = 0;

    static const char* kwlist[] = {"source", "return_type", "iterator", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s#|sp", const_cast<char**>(kwlist), &data, &len,
                                     &return_type, &iterator)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Validate return_type
    bool as_cursor = false;
    if (strcmp(return_type, "dict") == 0) {
        as_cursor = false;
    } else if (strcmp(return_type, "cursor") == 0) {
        as_cursor = true;
    } else {
        PyErr_Format(PyExc_ValueError, "return_type must be 'dict' or 'cursor', got '%s'",
                     return_type);
        return NULL;
    }

    if (as_cursor) {
        // Parse to C++ document and return (doc, cursor) tuple
        auto doc_result = strata::JsonDocument::from_string(std::string_view(data, len));
        if (!doc_result.ok()) {
            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            }
            return NULL;
        }

        strata::JsonDocument doc = std::move(doc_result.value);
        auto cursor = doc.root();
        PyObject* py_doc = create_py_json_document(std::move(doc));
        if (!py_doc)
            return NULL;
        PyObject* py_cursor = create_py_json_cursor(std::move(cursor), py_doc);
        if (!py_cursor) {
            Py_DECREF(py_doc);
            return NULL;
        }

        PyObject* result = PyTuple_New(2);
        if (!result) {
            Py_DECREF(py_cursor);
            Py_DECREF(py_doc);
            return NULL;
        }
        PyTuple_SET_ITEM(result, 0, py_doc);
        PyTuple_SET_ITEM(result, 1, py_cursor);
        return result;
    }

    // Standard SAX parse path — reuse a thread-local builder so the KeyCache
    // persists across calls (e.g. NDJSON per-line parsing benchmarks).
    // Use a leaked raw pointer to avoid destructor running after Python shutdown.
    PyGcPause gc_pause;
    static thread_local PythonObjectBuilder* tl_builder = new PythonObjectBuilder();

    PyObject* result = parse_json_to_python_reuse(std::string_view(data, len),
                                                  /*validate_utf8=*/false, *tl_builder);

    if (!result) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        }
        return NULL;
    }

    if (iterator) {
        if (PyDict_Check(result)) {
            PyObject* it = create_dict_iterator(result);
            Py_DECREF(result);
            return it;
        } else if (PyList_Check(result)) {
            PyObject* it = create_list_iterator(result);
            Py_DECREF(result);
            return it;
        }
    }

    return result;

    STRATA_CPP_CATCH
}

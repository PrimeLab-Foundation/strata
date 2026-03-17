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
        for (int i = 0; i < kMaxSchemas; ++i) {
            schemas_[i].num_keys = 0;
            schemas_[i].cursor_pos = 0;
        }
    }

    /// Fast FNV-1a hash with 8-byte-at-a-time processing.
    static inline uint32_t fnv1a(const char* data, size_t len) {
        uint32_t h = 2166136261u;
        // Process 8 bytes at a time for keys > 8 bytes
        const char* end = data + len;
        while (data + 8 <= end) {
            uint64_t chunk;
            std::memcpy(&chunk, data, 8);
            // Mix all 8 bytes into hash
            h = (h ^ static_cast<uint32_t>(chunk)) * 16777619u;
            h = (h ^ static_cast<uint32_t>(chunk >> 32)) * 16777619u;
            data += 8;
        }
        // Process remaining bytes
        while (data < end) {
            h = (h ^ static_cast<unsigned char>(*data)) * 16777619u;
            ++data;
        }
        return h;
    }

    /// Look up or create a cached key entry.
    ///
    /// Returns a **borrowed** CachedKeyEntry — the caller must NOT Py_DECREF
    /// the key.  The cache owns the reference; _PyDict_SetItem_KnownHash
    /// will INCREF internally when inserting into a dict.
    ///
    /// After a successful call, last_matched_index() returns the entry index
    /// of the matched/inserted key (for external schema recording).
    CachedKeyEntry get(std::string_view key) {
        const size_t n = entries_.size();

        if (n == 0) {
            // Empty cache — first key ever
            CachedKeyEntry result = insert_new(key, fnv1a(key.data(), key.size()));
            last_matched_idx_ = static_cast<uint16_t>(entries_.size() - 1);
            return result;
        }

        // Fast path 1: cursor prediction — O(1).
        // cursor_ is always the single prediction source for both speculative
        // matching and normal get(). Schema tracking keeps it aligned.
        {
            auto& e = entries_[cursor_];
            if (e.key_len == static_cast<uint16_t>(key.size()) &&
                std::memcmp(e.key_data, key.data(), key.size()) == 0) {
                last_matched_idx_ = static_cast<uint16_t>(cursor_);
                // For multi-schema data, advance schema cursor and set cursor_
                // to next schema key. For settled data, simple advance.
                if (UNLIKELY(!settled_) && active_schema_ >= 0) {
                    auto& schema = schemas_[active_schema_];
                    if (schema.cursor_pos < schema.num_keys &&
                        schema.key_indices[schema.cursor_pos] == static_cast<uint16_t>(cursor_)) {
                        uint16_t matched_pos = schema.cursor_pos;
                        schema.cursor_pos++;
                        if (schema.cursor_pos < schema.num_keys) {
                            cursor_ = schema.key_indices[schema.cursor_pos];
                        } else {
                            cursor_ = (cursor_ + 1 < n) ? cursor_ + 1 : 0;
                        }
                        // Advance shared keys in other schemas.
                        if (num_schemas_ > 1 && matched_pos < 2) {
                            uint16_t midx = schema.key_indices[matched_pos];
                            for (int j = 0; j < num_schemas_; ++j) {
                                if (j != active_schema_ && schemas_[j].cursor_pos == matched_pos &&
                                    schemas_[j].num_keys > matched_pos &&
                                    schemas_[j].key_indices[matched_pos] == midx) {
                                    schemas_[j].cursor_pos = matched_pos + 1;
                                }
                            }
                        }
                    } else {
                        cursor_ = (cursor_ + 1 < n) ? cursor_ + 1 : 0;
                    }
                } else {
                    cursor_ = (cursor_ + 1 < n) ? cursor_ + 1 : 0;
                }
                return e.cached;
            }
        }

        // Fast path 2: try other schemas' predictions (for schema switches).
        // Only triggered on cursor miss. Cost: up to kMaxSchemas memcmp.
        if (!settled_) {
            for (int i = 0; i < num_schemas_; ++i) {
                auto& schema = schemas_[i];
                if (schema.cursor_pos < schema.num_keys) {
                    uint16_t idx = schema.key_indices[schema.cursor_pos];
                    auto& e = entries_[idx];
                    if (e.key_len == static_cast<uint16_t>(key.size()) &&
                        std::memcmp(e.key_data, key.data(), key.size()) == 0) {
                        active_schema_ = i;
                        schema.cursor_pos++;
                        last_matched_idx_ = idx;
                        // Set cursor_ to next schema key.
                        if (schema.cursor_pos < schema.num_keys) {
                            cursor_ = schema.key_indices[schema.cursor_pos];
                        } else {
                            cursor_ = (idx + 1 < n) ? idx + 1 : 0;
                        }
                        return e.cached;
                    }
                }
            }
        }

        // Hash table fallback.
        uint32_t h = fnv1a(key.data(), key.size());
        uint32_t slot = h & kHashMask;

        // Linear probing (table is < 75% full)
        for (int probe = 0; probe < 16; ++probe) {
            uint16_t idx = hash_slots_[slot];
            if (idx == 0xFFFF)
                break; // empty slot → key not in cache
            if (idx < n) {
                auto& s = entries_[idx];
                if (s.key_len == static_cast<uint16_t>(key.size()) &&
                    std::memcmp(s.key_data, key.data(), key.size()) == 0) {
                    cursor_ = (idx + 1 < n) ? idx + 1 : 0;
                    last_matched_idx_ = idx;
                    return s.cached;
                }
            }
            slot = (slot + 1) & kHashMask;
        }

        // Cache miss: insert new key
        CachedKeyEntry result = insert_new(key, h);
        if (entries_.size() > n) {
            last_matched_idx_ = static_cast<uint16_t>(entries_.size() - 1);
        } else {
            // Found in overflow scan
            last_matched_idx_ =
                static_cast<uint16_t>(cursor_ > 0 ? cursor_ - 1 : entries_.size() - 1);
        }
        return result;
    }

    /// Index of the entry matched/inserted by the last get() call.
    uint16_t last_matched_index() const { return last_matched_idx_; }

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

    /// Get the predicted entry index for speculative matching.
    /// Returns cursor_ directly — zero overhead on the hot path.
    /// Schema tracking updates cursor_ at object boundaries and on misses.
    size_t predicted_entry_index() const { return cursor_; }

    /// Advance the cursor after a successful speculative match.
    void advance_after_match(size_t matched_idx) {
        const size_t n = entries_.size();
        last_matched_idx_ = static_cast<uint16_t>(matched_idx);
        // For multi-schema data, advance schema cursor and set cursor_
        // to next schema key. For settled data, simple advance.
        if (UNLIKELY(!settled_) && active_schema_ >= 0) {
            auto& schema = schemas_[active_schema_];
            if (schema.cursor_pos < schema.num_keys &&
                schema.key_indices[schema.cursor_pos] == static_cast<uint16_t>(matched_idx)) {
                uint16_t matched_pos = schema.cursor_pos;
                schema.cursor_pos++;
                if (schema.cursor_pos < schema.num_keys) {
                    cursor_ = schema.key_indices[schema.cursor_pos];
                } else {
                    cursor_ = (matched_idx + 1 < n) ? matched_idx + 1 : 0;
                }
                // Advance shared keys in other schemas (only during discovery).
                // Once multi_settled, schemas are known — sync is wasted work.
                if (num_schemas_ > 1 && matched_pos < 2 && !multi_settled_) {
                    uint16_t midx = schema.key_indices[matched_pos];
                    for (int j = 0; j < num_schemas_; ++j) {
                        if (j != active_schema_ && schemas_[j].cursor_pos == matched_pos &&
                            schemas_[j].num_keys > matched_pos &&
                            schemas_[j].key_indices[matched_pos] == midx) {
                            schemas_[j].cursor_pos = matched_pos + 1;
                        }
                    }
                }
                return;
            }
        }
        cursor_ = (matched_idx + 1 < n) ? matched_idx + 1 : 0;
    }

    /// Number of known schemas for multi-schema fallback in speculative matching.
    int num_schemas() const { return num_schemas_; }

    /// Try alternative schema predictions when primary cursor misses.
    /// Returns the entry index if a match is found, or SIZE_MAX on miss.
    /// On match, switches active schema and advances cursor.
    size_t try_alternative_schemas(const char* data, size_t remaining) {
        if (settled_ || num_schemas_ <= 1)
            return SIZE_MAX;
        const size_t n = entries_.size();
        for (int s = 0; s < num_schemas_; ++s) {
            auto& schema = schemas_[s];
            if (schema.cursor_pos >= schema.num_keys)
                continue;
            size_t idx = schema.key_indices[schema.cursor_pos];
            if (idx == cursor_)
                continue; // already checked by primary prediction
            auto& e = entries_[idx];
            size_t key_len = e.key_len;
            if (key_len + 1 > remaining)
                continue;
            if (std::memcmp(data, e.key_data, key_len) != 0)
                continue;
            if (data[key_len] != '"')
                continue;
            // Match! Switch to this schema.
            active_schema_ = s;
            last_matched_idx_ = static_cast<uint16_t>(idx);
            schema.cursor_pos++;
            if (schema.cursor_pos < schema.num_keys) {
                cursor_ = schema.key_indices[schema.cursor_pos];
            } else {
                cursor_ = (idx + 1 < n) ? idx + 1 : 0;
            }
            return idx;
        }
        return SIZE_MAX;
    }

    /// Whether schema tracking has settled (single schema confirmed).
    /// When true, notify_object_start/end are effectively no-ops and
    /// schema tracking in PythonObjectBuilder can be skipped entirely.
    bool settled() const { return settled_; }

    /// Whether multi-schema tracking has stabilized.
    /// When true, schema recording in the handler can be skipped.
    bool multi_settled() const { return multi_settled_; }

    /// Called at the start of each JSON object (array-element objects only).
    /// For settled single-schema data: no-op (cursor_ wraps naturally).
    /// For multi-schema data: resets schema cursors and aligns cursor_.
    void notify_object_start() {
        if (LIKELY(settled_))
            return;

        // Reset all schema cursors to their start position.
        for (int i = 0; i < num_schemas_; ++i) {
            schemas_[i].cursor_pos = 0;
        }
        // Point cursor_ at the active schema's first key.
        if (active_schema_ >= 0 && schemas_[active_schema_].num_keys > 0) {
            cursor_ = schemas_[active_schema_].key_indices[0];
        }
    }

    /// Called at the end of each JSON object (array-element objects only).
    /// @param keys  Array of entry indices for each top-level key in order.
    /// @param count Number of keys.
    void notify_object_end(const uint16_t* keys, uint16_t count) {
        if (LIKELY(settled_ || multi_settled_))
            return;
        if (count == 0)
            return;

        // Check if this key sequence matches the active schema
        if (active_schema_ >= 0) {
            auto& schema = schemas_[active_schema_];
            if (schema.num_keys == count &&
                std::memcmp(schema.key_indices, keys, count * sizeof(uint16_t)) == 0) {
                // Same schema matched. After 2 consecutive matches with a
                // single schema, mark as settled (zero overhead for remaining
                // 498+ objects in a typical benchmark).
                if (num_schemas_ == 1) {
                    settle_count_++;
                    if (settle_count_ >= 2) {
                        settled_ = true;
                    }
                } else {
                    // Multi-schema: count total matches across all schemas
                    settle_count_++;
                    if (settle_count_ >= 6) {
                        // All schemas seen enough times — stop verifying
                        multi_settled_ = true;
                    }
                }
                return;
            }
        }

        // Check if this key sequence matches any known schema
        for (int i = 0; i < num_schemas_; ++i) {
            auto& schema = schemas_[i];
            if (schema.num_keys == count &&
                std::memcmp(schema.key_indices, keys, count * sizeof(uint16_t)) == 0) {
                active_schema_ = i;
                if (num_schemas_ > 1) {
                    settle_count_++; // Count towards multi-settle
                    if (settle_count_ >= 6) {
                        multi_settled_ = true;
                    }
                } else {
                    settle_count_ = 0;
                }
                return;
            }
        }

        // New schema — register it
        if (num_schemas_ < kMaxSchemas && count <= kMaxSchemaKeys) {
            auto& schema = schemas_[num_schemas_];
            std::memcpy(schema.key_indices, keys, count * sizeof(uint16_t));
            schema.num_keys = count;
            schema.cursor_pos = 0;
            active_schema_ = num_schemas_;
            num_schemas_++;
            settle_count_ = 0;
        }
    }

  private:
    /// Insert a new key into the cache. Called when key is not found.
    /// @param h  Pre-computed FNV-1a hash (avoids double-hashing).
    CachedKeyEntry insert_new(std::string_view key, uint32_t h) {
        size_t new_idx = entries_.size();

        // If hash table is full, check overflow entries (index >= kMaxEntries)
        // before creating a new PyObject. Without this, keys beyond the hash
        // table capacity would be re-created on every lookup, causing O(n²)
        // growth of entries_ across records.
        if (new_idx >= kMaxEntries) {
            for (size_t i = kMaxEntries; i < new_idx; ++i) {
                auto& s = entries_[i];
                if (s.key_len == static_cast<uint16_t>(key.size()) &&
                    std::memcmp(s.key_data, key.data(), key.size()) == 0) {
                    cursor_ = (i + 1 < new_idx) ? i + 1 : 0;
                    return s.cached;
                }
            }
            // Truly new key beyond hash table capacity. Add to entries_
            // for ownership tracking but don't insert into hash table.
            PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
            if (!py_key)
                return {nullptr, 0};
            PyUnicode_InternInPlace(&py_key);
            Py_hash_t py_hash = PyObject_Hash(py_key);
            CachedKeyEntry cached{py_key, py_hash};
            entries_.push_back({std::string(key), nullptr, 0, cached});
            auto& back = entries_.back();
            back.key_data = back.key_storage.data();
            back.key_len = static_cast<uint16_t>(key.size());
            return cached;
        }

        PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
        if (!py_key)
            return {nullptr, 0};

        PyUnicode_InternInPlace(&py_key);
        Py_hash_t py_hash = PyObject_Hash(py_key);

        CachedKeyEntry cached{py_key, py_hash};
        entries_.push_back({std::string(key), nullptr, 0, cached});
        auto& back = entries_.back();
        back.key_data = back.key_storage.data();
        back.key_len = static_cast<uint16_t>(key.size());

        // Insert into hash table — reuse pre-computed hash h
        uint32_t slot = h & kHashMask;
        while (hash_slots_[slot] != 0xFFFF)
            slot = (slot + 1) & kHashMask;
        hash_slots_[slot] = static_cast<uint16_t>(new_idx);

        return cached;
    }

    static constexpr int kHashSlots = 256; // power of 2, larger table = less probing
    static constexpr uint32_t kHashMask = kHashSlots - 1;
    static constexpr size_t kMaxEntries = 192; // 75% of 256

    // Multi-schema cursor tracking.
    // Each schema stores its own key index sequence so the cursor can predict
    // keys correctly even when JSON objects alternate between different schemas.
    static constexpr int kMaxSchemas = 4;
    static constexpr int kMaxSchemaKeys = 64;

    struct SchemaInfo {
        uint16_t key_indices[kMaxSchemaKeys]; // entry indices for each key in order
        uint16_t num_keys = 0;
        uint16_t cursor_pos = 0; // current position in key_indices for prediction
    };

    std::vector<Entry> entries_;
    size_t cursor_ = 0;
    uint16_t hash_slots_[kHashSlots]; // index into entries_, 0xFFFF = empty

    // Schema tracking state
    SchemaInfo schemas_[kMaxSchemas];
    int num_schemas_ = 0;
    int active_schema_ = -1;
    int settle_count_ = 0; // consecutive same-schema matches (single) or total matches (multi)
    bool settled_ = false; // true after single schema confirmed (zero-overhead mode)
    bool multi_settled_ = false; // true after multi-schema patterns stabilized

    // Index of the entry matched/inserted by the last get() call.
    uint16_t last_matched_idx_ = 0;
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
        // Only track schemas for objects that are direct children of arrays.
        // This is where schema variation occurs (e.g., mixed benchmark has
        // flat/nested/wide_arrays records alternating in the same array).
        // Inner objects (dict values, deeply nested) are consistent per-schema
        // and handled well by the legacy cursor.
        // Once settled (single or multi), skip schema recording overhead.
        // notify_object_start is still needed for multi-schema to reset cursors.
        bool in_array = !stack_.empty() && stack_.back() == nullptr;
        if (UNLIKELY(in_array && !schema_tracking_active_ && !cache_.settled())) {
            cache_.notify_object_start();
            // Only record schema keys during discovery phase (before multi_settled).
            // Once multi_settled, schemas are known — recording is wasted work.
            if (!cache_.multi_settled()) {
                schema_tracking_active_ = true;
                schema_tracking_depth_ = stack_.size();
                schema_recording_count_ = 0;
            }
        }

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

        // Record key index for schema tracking (only for top-level keys of
        // tracked array-element objects, not inner/nested object keys).
        if (schema_tracking_active_ && stack_.size() == schema_tracking_depth_ + 1) {
            if (schema_recording_count_ < kMaxSchemaKeys) {
                schema_recording_[schema_recording_count_++] = cache_.last_matched_index();
            }
        }
        return true;
    }

    /// Speculative key match: called by the parser before SIMD string scanning.
    ///
    /// Compares the raw JSON bytes (after the opening quote) against the
    /// predicted key from the active schema (or legacy cursor). If the bytes
    /// match and the next byte is a closing quote, we skip the full
    /// parse_string() path:
    /// - No SIMD escape scan (find_next_escape_simd)
    /// - No cache lookup (already at the predicted position)
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

        // Use schema-aware prediction: tries active schema first,
        // then falls back to legacy cursor position.
        size_t pred_idx = cache_.predicted_entry_index();
        const auto& e = entries[pred_idx];
        const size_t key_len = e.key_len;

        // Need key_len bytes + closing quote
        if (key_len + 1 <= remaining && std::memcmp(data, e.key_data, key_len) == 0 &&
            data[key_len] == '"') {
            // Primary prediction match! Push the cached key and advance cursor.
            keys_.push_back(e.cached);
            cache_.advance_after_match(pred_idx);

            // Record key index for schema tracking (speculative match bypasses on_key).
            if (schema_tracking_active_ && stack_.size() == schema_tracking_depth_ + 1) {
                if (schema_recording_count_ < kMaxSchemaKeys) {
                    schema_recording_[schema_recording_count_++] = static_cast<uint16_t>(pred_idx);
                }
            }
            return key_len + 1; // consumed: key bytes + closing quote
        }

        // Primary prediction missed. For multi-schema data, try other schemas'
        // cursor positions before falling back to expensive full string parsing.
        if (cache_.num_schemas() > 1) {
            size_t alt_idx = cache_.try_alternative_schemas(data, remaining);
            if (alt_idx != SIZE_MAX) {
                const auto& alt_e = entries[alt_idx];
                keys_.push_back(alt_e.cached);
                // Record key index for schema tracking
                if (schema_tracking_active_ && stack_.size() == schema_tracking_depth_ + 1) {
                    if (schema_recording_count_ < kMaxSchemaKeys) {
                        schema_recording_[schema_recording_count_++] =
                            static_cast<uint16_t>(alt_idx);
                    }
                }
                return alt_e.key_len + 1;
            }
        }

        return 0;
    }

    bool on_end_object() override {
        if (stack_.empty())
            return false;
        PyObject* dict = stack_.back();

        // Notify key cache of object end only for tracked array-element objects.
        size_t depth = stack_.size() - 1;
        if (schema_tracking_active_ && depth == schema_tracking_depth_) {
            cache_.notify_object_end(schema_recording_, schema_recording_count_);
            schema_tracking_active_ = false;
        }

        // Record the object size for depth-based prediction.
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
        schema_tracking_active_ = false;
        schema_tracking_depth_ = 0;
        schema_recording_count_ = 0;
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

    // Schema tracking: only active for objects that are direct children of arrays.
    bool schema_tracking_active_ = false;
    size_t schema_tracking_depth_ = 0;
    static constexpr int kMaxSchemaKeys = 64;
    uint16_t schema_recording_[kMaxSchemaKeys];
    uint16_t schema_recording_count_ = 0;
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

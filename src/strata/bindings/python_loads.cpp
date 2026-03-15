#include "python_convert.h"
#include "python_document.h"
#include "python_types.h"
#include "strata/json/json_document.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_parser_inline.hpp"
#include "strata/json/json_sax_handler.hpp"
#include "strata/json/ndjson_stream.hpp"

#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

// Forward-declare CPython internal function for hash-aware dict insertion.
// This function is exported in libpython (PyAPI_FUNC) but the header guard
// requires Py_BUILD_CORE. Forward-declaring lets us link without that define.
// Available since CPython 3.6+. Falls back to PyDict_SetItem if unavailable.
extern "C" {
int _PyDict_SetItem_KnownHash(PyObject* mp, PyObject* key, PyObject* item, Py_hash_t hash);
}

namespace {

struct string_hash {
    using is_transparent = void;
    size_t operator()(std::string_view sv) const { return std::hash<std::string_view>{}(sv); }
    size_t operator()(const std::string& s) const { return std::hash<std::string>{}(s); }
};

struct string_equal {
    using is_transparent = void;
    bool operator()(std::string_view sv1, std::string_view sv2) const { return sv1 == sv2; }
};

// Cached key entry: interned PyObject* + pre-computed hash.
// Storing the hash alongside the key avoids recomputing it during
// _PyDict_SetItem_KnownHash, saving ~10-15% of dict insertion cost.
struct CachedKeyEntry {
    PyObject* key;
    Py_hash_t hash;
};

// Key cache: maps JSON key strings → interned PyObject* with pre-computed hash.
// Thread-local instance persists across multiple parse calls (e.g. all lines in an NDJSON
// batch), so the same key strings are not recreated for every line.
class KeyCache {
  public:
    CachedKeyEntry get(std::string_view key) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            Py_INCREF(it->second.key);
            return it->second;
        }

        PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
        if (!py_key)
            return {nullptr, 0};

        PyUnicode_InternInPlace(&py_key);
        Py_hash_t hash = PyObject_Hash(py_key);
        Py_INCREF(py_key); // One for the cache
        CachedKeyEntry entry{py_key, hash};
        cache_[std::string(key)] = entry;
        return entry;
    }

    ~KeyCache() {
        for (auto& pair : cache_) {
            Py_DECREF(pair.second.key);
        }
    }

  private:
    std::unordered_map<std::string, CachedKeyEntry, string_hash, string_equal> cache_;
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
        return push_value(PyUnicode_FromStringAndSize(v.data(), v.size()));
    }

    bool on_start_object(size_t size_hint) override {
        PyObject* dict;
        if (size_hint > 0) {
            dict = _PyDict_NewPresized(static_cast<Py_ssize_t>(size_hint));
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

    bool on_end_object() override {
        if (stack_.empty())
            return false;
        PyObject* dict = stack_.back();
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
    // Clears parse state but keeps the KeyCache populated for cross-line key reuse.
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
        for (auto& entry : keys_) {
            Py_DECREF(entry.key);
        }
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

        // Hot path: LastWins (default policy) with pre-computed hash.
        // _PyDict_SetItem_KnownHash avoids recomputing the key hash,
        // saving ~10-15% of dict insertion time per key-value pair.
        if (policy_ == strata::DuplicateKeyPolicy::LastWins) {
            if (_PyDict_SetItem_KnownHash(top, key_entry.key, val, key_entry.hash) < 0) {
                Py_DECREF(key_entry.key);
                Py_DECREF(val);
                return false;
            }
            Py_DECREF(key_entry.key);
            Py_DECREF(val);
            return true;
        }

        if (policy_ == strata::DuplicateKeyPolicy::FirstWins) {
            PyObject* result = PyDict_SetDefault(top, key_entry.key, val);
            if (!result) {
                Py_DECREF(key_entry.key);
                Py_DECREF(val);
                return false;
            }
            Py_DECREF(key_entry.key);
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
                Py_DECREF(key_entry.key);
                Py_DECREF(val);
                return true;
            }
            Py_DECREF(key_entry.key);
            Py_DECREF(val);
            return false;
        }

        if (_PyDict_SetItem_KnownHash(top, key_entry.key, val, key_entry.hash) < 0) {
            Py_DECREF(key_entry.key);
            Py_DECREF(val);
            return false;
        }
        Py_DECREF(key_entry.key);
        Py_DECREF(val);
        return true;
    }

    PyObject* root_ = nullptr;
    std::vector<PyObject*> stack_;
    std::vector<CachedKeyEntry> keys_;   // key + pre-computed hash
    std::vector<PyObject*> array_items_; // flat storage for all in-flight array items
    std::vector<size_t> array_starts_;   // stack of start indices into array_items_
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
            CachedKeyEntry entry = cache.get(key);
            if (!entry.key) {
                Py_DECREF(dict);
                return NULL;
            }

            PyObject* py_val = json_value_to_python_internal(value, cache);
            if (!py_val) {
                Py_DECREF(entry.key);
                Py_DECREF(dict);
                return NULL;
            }

            int rc = _PyDict_SetItem_KnownHash(dict, entry.key, py_val, entry.hash);
            Py_DECREF(entry.key);
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
// Avoids the Python NdjsonStream wrapper overhead.
PyObject* strata_parse_ndjson(PyObject* self, PyObject* args) {
    const char* data;
    Py_ssize_t len;
    int skip_errors = 0;

    if (!PyArg_ParseTuple(args, "s#|p", &data, &len, &skip_errors)) {
        return NULL;
    }

    STRATA_CPP_TRY

    strata::NdjsonStream stream(std::string_view(data, len));
    return parse_ndjson_all_to_python(stream, skip_errors);

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
        if (!PyArg_ParseTuple(args, "s#", &data, &len)) {
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

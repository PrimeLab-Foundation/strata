#include "python_convert.h"
#include "python_types.h"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_sax_handler.hpp"

#include <string>
#include <unordered_map>
#include <vector>

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

// Key cache: maps JSON key strings → interned PyObject*
// Thread-local instance persists across multiple parse calls (e.g. all lines in an NDJSON
// batch), so the same key strings are not recreated for every line.
class KeyCache {
  public:
    PyObject* get(std::string_view key) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            Py_INCREF(it->second);
            return it->second;
        }

        PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
        if (py_key) {
            PyUnicode_InternInPlace(&py_key);
            Py_INCREF(py_key); // One for the cache
            cache_[std::string(key)] = py_key;
        }
        return py_key;
    }

    ~KeyCache() {
        for (auto& pair : cache_) {
            Py_DECREF(pair.second);
        }
    }

  private:
    std::unordered_map<std::string, PyObject*, string_hash, string_equal> cache_;
};

// Thread-local key cache shared across all PythonObjectBuilder instances on the same thread.
// This allows NDJSON batch parsing to reuse the same interned key strings across lines.
static thread_local KeyCache g_key_cache;

class PythonObjectBuilder : public strata::JsonSaxHandler {
  public:
    explicit PythonObjectBuilder() {
        stack_.reserve(32);
        keys_.reserve(32);
        // Cache policy once per document (thread-local read is cheap but avoidable in hot path)
        policy_ = strata::get_duplicate_key_policy();
    }

    ~PythonObjectBuilder() {
        if (root_) {
            Py_DECREF(root_);
        }
        for (auto obj : stack_) {
            Py_DECREF(obj);
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

    bool on_int(int64_t v) override { return push_value(PyLong_FromLongLong(v)); }

    bool on_uint(uint64_t v) override { return push_value(PyLong_FromUnsignedLongLong(v)); }

    bool on_double(double v) override { return push_value(PyFloat_FromDouble(v)); }

    bool on_string(std::string_view v) override {
        return push_value(PyUnicode_FromStringAndSize(v.data(), v.size()));
    }

    bool on_start_object(size_t) override {
        PyObject* dict = PyDict_New();
        if (!dict)
            return false;
        stack_.push_back(dict);
        return true;
    }

    bool on_key(std::string_view v) override {
        PyObject* key = g_key_cache.get(v);
        if (!key)
            return false;
        keys_.push_back(key);
        return true;
    }

    bool on_end_object() override {
        if (stack_.empty())
            return false;
        PyObject* dict = stack_.back();
        stack_.pop_back();
        return push_value(dict);
    }

    bool on_start_array(size_t) override {
        PyObject* list = PyList_New(0);
        if (!list)
            return false;
        stack_.push_back(list);
        return true;
    }

    bool on_end_array() override {
        if (stack_.empty())
            return false;
        PyObject* list = stack_.back();
        stack_.pop_back();
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
            Py_DECREF(obj);
        }
        stack_.clear();
        for (auto obj : keys_) {
            Py_DECREF(obj);
        }
        keys_.clear();
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
        if (PyList_Check(top)) {
            if (PyList_Append(top, val) < 0) {
                Py_DECREF(val);
                return false;
            }
            Py_DECREF(val);
            return true;
        } else if (PyDict_Check(top)) {
            if (keys_.empty()) {
                Py_DECREF(val);
                return false;
            }
            PyObject* key = keys_.back();
            keys_.pop_back();

            // Optimized duplicate key handling based on cached policy
            if (policy_ == strata::DuplicateKeyPolicy::LastWins) {
                // Always overwrite – single hash lookup
                if (PyDict_SetItem(top, key, val) < 0) {
                    Py_DECREF(key);
                    Py_DECREF(val);
                    return false;
                }
                Py_DECREF(key);
                Py_DECREF(val);
                return true;
            }

            if (policy_ == strata::DuplicateKeyPolicy::FirstWins) {
                // PyDict_SetDefault: inserts key=val only when key absent; single hash op
                PyObject* result = PyDict_SetDefault(top, key, val);
                if (!result) {
                    Py_DECREF(key);
                    Py_DECREF(val);
                    return false;
                }
                // Whether key was inserted or already present, release our refs
                Py_DECREF(key);
                Py_DECREF(val);
                return true;
            }

            // Warn / Error: need to detect duplicate first
            if (PyDict_Contains(top, key)) {
                if (policy_ == strata::DuplicateKeyPolicy::Warn) {
                    PyObject* key_repr = PyObject_Repr(key);
                    const char* key_str = PyUnicode_AsUTF8(key_repr);
                    std::string msg = "Duplicate key encountered: ";
                    msg += key_str;
                    PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
                    Py_XDECREF(key_repr);
                    Py_DECREF(key);
                    Py_DECREF(val);
                    return true;
                }
                // Error policy
                Py_DECREF(key);
                Py_DECREF(val);
                return false;
            }

            if (PyDict_SetItem(top, key, val) < 0) {
                Py_DECREF(key);
                Py_DECREF(val);
                return false;
            }
            Py_DECREF(key);
            Py_DECREF(val);
            return true;
        }
        Py_DECREF(val);
        return false;
    }

    PyObject* root_ = nullptr;
    std::vector<PyObject*> stack_;
    std::vector<PyObject*> keys_;
    strata::DuplicateKeyPolicy policy_;
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
        PyObject* dict = PyDict_New();
        if (!dict)
            return NULL;

        for (const auto& [key, value] : obj) {
            PyObject* py_key = cache.get(key);
            if (!py_key) {
                Py_DECREF(dict);
                return NULL;
            }

            PyObject* py_val = json_value_to_python_internal(value, cache);
            if (!py_val) {
                Py_DECREF(py_key);
                Py_DECREF(dict);
                return NULL;
            }

            int rc = PyDict_SetItem(dict, py_key, py_val);
            Py_DECREF(py_key);
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
    return json_value_to_python_internal(val, g_key_cache);
}

// Helper: parse JSON text directly to a Python object via SAX (no intermediate C++ DOM).
// validate_utf8=false is safe when the caller creates PyUnicode objects, which validate inline.
PyObject* parse_json_to_python(std::string_view text, bool validate_utf8) {
    PythonObjectBuilder builder;
    auto status = strata::parse_sax(text, builder, validate_utf8);
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
    auto status = strata::parse_sax(text, builder, validate_utf8);
    if (status != strata::Status::Ok) {
        return nullptr;
    }
    return builder.take_root();
}

// Python loads() function
PyObject* strata_loads(PyObject* self, PyObject* args) {
    const char* data;
    Py_ssize_t len;

    // Parse arguments
    if (!PyArg_ParseTuple(args, "s#", &data, &len)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Suspend GC during bulk object creation: avoids collection pauses while building
    // the Python object tree from JSON.
    PyGcPause gc_pause;

    // Skip upfront UTF-8 validation: PyUnicode_FromStringAndSize (called inside
    // PythonObjectBuilder::on_string) validates UTF-8 inline. Non-string JSON bytes
    // are ASCII and are inherently valid UTF-8.
    PyObject* result = parse_json_to_python(std::string_view(data, len), /*validate_utf8=*/false);

    if (!result) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        }
        return NULL;
    }

    return result;

    STRATA_CPP_CATCH
}

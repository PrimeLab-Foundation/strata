#include "python_convert.h"
#include "python_types.h"
#include "strata/json/json_parse.hpp"

#include <string>
#include <unordered_map>
#include <vector>

static void emit_duplicate_key_warnings() {
    auto warnings = strata::consume_parse_warnings();
    for (const auto& msg : warnings) {
        PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
    }
}

// Simple key cache to avoid creating many Python strings for the same key
class KeyCache {
  public:
    PyObject* get(const std::string& key) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            Py_INCREF(it->second);
            return it->second;
        }

        PyObject* py_key = PyUnicode_FromStringAndSize(key.c_str(), key.size());
        if (py_key) {
            PyUnicode_InternInPlace(&py_key);
            Py_INCREF(py_key); // One for the cache
            cache_[key] = py_key;
        }
        return py_key;
    }

    ~KeyCache() {
        for (auto& pair : cache_) {
            Py_DECREF(pair.second);
        }
    }

  private:
    std::unordered_map<std::string, PyObject*> cache_;
};

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
    KeyCache cache;
    return json_value_to_python_internal(val, cache);
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

    // Parse JSON
    auto result = strata::parse_json(std::string_view(data, len));

    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        return NULL;
    }

    emit_duplicate_key_warnings();

    // Convert to Python
    PyGcPause gc_pause;
    return json_value_to_python(result.value);

    STRATA_CPP_CATCH
}

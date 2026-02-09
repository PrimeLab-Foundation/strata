#include "python_convert.h"
#include "python_object_builder.h"
#include "python_types.h"
#include "strata/json/json_parse.hpp"

#include <climits>
#include <string>
#include <unordered_map>
#include <vector>

// Thread-local arena for zero-allocation parsing
thread_local strata::util::Arena g_parse_arena;
thread_local strata::bindings::KeyCache g_key_cache;
thread_local strata::util::Arena g_parse_builder_arena(4 * 1024);
thread_local strata::bindings::PythonObjectBuilder g_parse_builder(&g_parse_builder_arena,
                                                                  g_key_cache);

namespace {
using strata::bindings::KeyCache;

struct BuilderResetGuard {
    strata::bindings::PythonObjectBuilder& builder;
    explicit BuilderResetGuard(strata::bindings::PythonObjectBuilder& builder_ref)
        : builder(builder_ref) {}
    ~BuilderResetGuard() { builder.reset(); }
    BuilderResetGuard(const BuilderResetGuard&) = delete;
    BuilderResetGuard& operator=(const BuilderResetGuard&) = delete;
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

    if (val.is_int()) {
        int64_t v = val.as_int();
        if (v >= LONG_MIN && v <= LONG_MAX) {
            return PyLong_FromLong(static_cast<long>(v));
        }
        return PyLong_FromLongLong(v);
    }

    if (val.is_double()) {
        return PyFloat_FromDouble(val.as_double());
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
    strata::util::Arena arena;
    KeyCache cache(&arena);
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

    // Reset thread-local arena for reuse
    g_parse_arena.reset();
    g_key_cache.reset(&g_parse_arena);
    g_parse_builder.reset();
    BuilderResetGuard builder_guard(g_parse_builder);

    // Use fast path: Direct-to-Python via SAX
    constexpr size_t kGcPauseMinSize = 64 * 1024;
    const size_t size = static_cast<size_t>(len);
    auto parse = [&]() {
        return strata::parse_sax(std::string_view(data, size), g_parse_builder);
    };
    strata::Status status = strata::Status::ParseError;
    if (size >= kGcPauseMinSize) {
        ::PyGcPause gc_pause;
        status = parse();
    } else {
        status = parse();
    }

    if (status != strata::Status::Ok) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        }
        return NULL;
    }

    return g_parse_builder.take_root();

    STRATA_CPP_CATCH
}

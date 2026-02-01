#include "python_convert.h"
#include "python_document.h"
#include "python_types.h"
#include "strata/search/jsonpath.hpp"
#include "strata/util/fast_parse.hpp"

#include <cstdlib>
#include <limits>
#include <string>
#include <vector>

static void emit_duplicate_key_warnings() {
    auto warnings = strata::consume_parse_warnings();
    for (const auto& msg : warnings) {
        PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
    }
}

extern int strata_get_cycle_policy();

static PyObject* make_cycle_memo() { return PyDict_New(); }

static bool memo_contains(PyObject* memo, PyObject* obj, bool& ok) {
    PyObject* key = PyLong_FromVoidPtr(obj);
    if (!key) {
        ok = false;
        return false;
    }
    int rc = PyDict_Contains(memo, key);
    Py_DECREF(key);
    if (rc < 0) {
        ok = false;
        return false;
    }
    return rc == 1;
}

static bool memo_add(PyObject* memo, PyObject* obj) {
    PyObject* key = PyLong_FromVoidPtr(obj);
    if (!key)
        return false;
    int rc = PyDict_SetItem(memo, key, Py_True);
    Py_DECREF(key);
    return rc == 0;
}

static void memo_remove(PyObject* memo, PyObject* obj) {
    PyObject* key = PyLong_FromVoidPtr(obj);
    if (!key) {
        PyErr_Clear();
        return;
    }
    if (PyDict_DelItem(memo, key) < 0) {
        PyErr_Clear();
    }
    Py_DECREF(key);
}

static bool is_container(PyObject* obj) {
    return PyDict_Check(obj) || PyList_Check(obj) || PyTuple_Check(obj);
}

static strata::JsonValue pyobject_to_json_value(PyObject* obj, PyObject* memo, bool& ok) {
    if (!ok) {
        return strata::JsonValue();
    }

    if (obj == Py_None) {
        return strata::JsonValue();
    }

    if (obj == Py_True) {
        return strata::JsonValue(strata::JsonValue::Variant(true));
    }
    if (obj == Py_False) {
        return strata::JsonValue(strata::JsonValue::Variant(false));
    }

    if (PyUnicode_Check(obj)) {
        Py_ssize_t len = 0;
        const char* data = PyUnicode_AsUTF8AndSize(obj, &len);
        if (!data) {
            ok = false;
            return strata::JsonValue();
        }
        return strata::JsonValue(strata::JsonValue::Variant(std::string(data, len)));
    }

    if (PyLong_Check(obj)) {
        int overflow = 0;
        long long int_val = PyLong_AsLongLongAndOverflow(obj, &overflow);
        if (overflow == 0 && !PyErr_Occurred()) {
            return strata::JsonValue(strata::JsonValue::Variant(static_cast<double>(int_val)));
        }
        PyErr_Clear();

        PyObject* str_obj = PyObject_Str(obj);
        if (!str_obj) {
            ok = false;
            return strata::JsonValue();
        }
        Py_ssize_t len = 0;
        const char* data = PyUnicode_AsUTF8AndSize(str_obj, &len);
        if (!data) {
            Py_DECREF(str_obj);
            ok = false;
            return strata::JsonValue();
        }
        double value = 0.0;
        size_t consumed = 0;
        bool parsed =
            strata::util::parse_double_fast(data, static_cast<size_t>(len), value, consumed);
        Py_DECREF(str_obj);
        if (!parsed || consumed != static_cast<size_t>(len)) {
            PyErr_SetString(PyExc_ValueError, "Failed to convert large integer");
            ok = false;
            return strata::JsonValue();
        }
        return strata::JsonValue(strata::JsonValue::Variant(value));
    }

    if (PyFloat_Check(obj)) {
        return strata::JsonValue(strata::JsonValue::Variant(PyFloat_AS_DOUBLE(obj)));
    }

    if (is_container(obj)) {
        if (memo_contains(memo, obj, ok)) {
            int policy = strata_get_cycle_policy();
            if (policy == 1) {
                PyErr_SetString(PyExc_ValueError, "Cycle detected during JSONPath conversion");
                ok = false;
                return strata::JsonValue();
            }
            if (policy == 0) {
                if (PyErr_WarnEx(PyExc_RuntimeWarning, "Cycle detected during JSONPath conversion",
                                 1) < 0) {
                    ok = false;
                    return strata::JsonValue();
                }
            }
            return strata::JsonValue();
        }

        if (!memo_add(memo, obj)) {
            ok = false;
            return strata::JsonValue();
        }

        if (PyDict_Check(obj)) {
            strata::JsonValue::Object out;
            PyObject* key = nullptr;
            PyObject* value = nullptr;
            Py_ssize_t pos = 0;
            while (PyDict_Next(obj, &pos, &key, &value)) {
                if (!PyUnicode_Check(key)) {
                    PyErr_SetString(PyExc_TypeError, "Dict keys must be strings");
                    ok = false;
                    break;
                }
                Py_ssize_t klen = 0;
                const char* kdata = PyUnicode_AsUTF8AndSize(key, &klen);
                if (!kdata) {
                    ok = false;
                    break;
                }
                std::string kstr(kdata, klen);
                strata::JsonValue v = pyobject_to_json_value(value, memo, ok);
                if (!ok) {
                    break;
                }
                out.emplace(std::move(kstr), std::move(v));
            }
            memo_remove(memo, obj);
            return ok ? strata::JsonValue(strata::JsonValue::Variant(std::move(out)))
                      : strata::JsonValue();
        }

        strata::JsonValue::Array arr;
        Py_ssize_t size = PySequence_Size(obj);
        if (size < 0) {
            ok = false;
            memo_remove(memo, obj);
            return strata::JsonValue();
        }
        arr.reserve(static_cast<size_t>(size));
        for (Py_ssize_t i = 0; i < size; ++i) {
            PyObject* item = PySequence_GetItem(obj, i);
            if (!item) {
                ok = false;
                break;
            }
            strata::JsonValue v = pyobject_to_json_value(item, memo, ok);
            Py_DECREF(item);
            if (!ok) {
                break;
            }
            arr.push_back(std::move(v));
        }
        memo_remove(memo, obj);
        return ok ? strata::JsonValue(strata::JsonValue::Variant(std::move(arr)))
                  : strata::JsonValue();
    }

    PyErr_SetString(PyExc_TypeError, "Unsupported type for JSONPath search");
    ok = false;
    return strata::JsonValue();
}

//=============================================================================
// CompiledPath Type
//=============================================================================

typedef struct {
    PyObject_HEAD strata::CompiledPath* path;
} PyCompiledPath;

static void PyCompiledPath_dealloc(PyCompiledPath* self) {
    delete self->path;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyCompiledPath_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyCompiledPath* self = (PyCompiledPath*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->path = nullptr;
    }
    return (PyObject*)self;
}

// Forward declarations
static PyObject* PyCompiledPath_from_string(PyObject* cls, PyObject* args);
static PyObject* PyCompiledPath_execute(PyCompiledPath* self, PyObject* args);

// Method table
static PyMethodDef PyCompiledPath_methods[] = {
    {"from_string", (PyCFunction)PyCompiledPath_from_string, METH_VARARGS | METH_CLASS,
     "Create CompiledPath from JSONPath string"},
    {"execute", (PyCFunction)PyCompiledPath_execute, METH_VARARGS, "Execute path on cursor"},
    {NULL, NULL, 0, NULL}};

// Type object
static PyTypeObject PyCompiledPathType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.CompiledPath",
    .tp_basicsize = sizeof(PyCompiledPath),
    .tp_dealloc = (destructor)PyCompiledPath_dealloc,
    .tp_new = PyCompiledPath_new,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Compiled JSONPath Query",
    .tp_methods = PyCompiledPath_methods,
};

//=============================================================================
// Method Implementations
//=============================================================================

static PyObject* PyCompiledPath_from_string(PyObject* cls, PyObject* args) {
    const char* path_str;

    if (!PyArg_ParseTuple(args, "s", &path_str)) {
        return NULL;
    }

    STRATA_CPP_TRY

    auto result = strata::compile_jsonpath(path_str);
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
        return NULL;
    }

    PyCompiledPath* self = (PyCompiledPath*)PyType_GenericAlloc(&PyCompiledPathType, 0);
    if (!self)
        return NULL;

    self->path = new strata::CompiledPath(std::move(result.value));

    return (PyObject*)self;

    STRATA_CPP_CATCH
}

static PyObject* PyCompiledPath_execute(PyCompiledPath* self, PyObject* args) {
    PyObject* cursor_obj;

    if (!PyArg_ParseTuple(args, "O", &cursor_obj)) {
        return NULL;
    }

    // Accept either C extension JsonCursor or Python JsonCursor wrapper (has _cursor)
    PyObject* cursor_to_use = cursor_obj;
    PyObject* borrowed = NULL;
    if (!is_py_json_cursor(cursor_obj) && PyObject_HasAttrString(cursor_obj, "_cursor")) {
        borrowed = PyObject_GetAttrString(cursor_obj, "_cursor");
        if (borrowed && is_py_json_cursor(borrowed)) {
            cursor_to_use = borrowed;
        } else if (borrowed) {
            Py_DECREF(borrowed);
            borrowed = NULL;
        }
    }

    if (!is_py_json_cursor(cursor_to_use)) {
        Py_XDECREF(borrowed);
        PyErr_SetString(PyExc_TypeError, "Expected JsonCursor object");
        return NULL;
    }

    strata::JsonCursor* cursor_ptr = get_py_json_cursor(cursor_to_use);
    Py_XDECREF(borrowed);
    borrowed = NULL;
    if (!cursor_ptr) {
        PyErr_SetString(PyExc_TypeError, "Invalid JsonCursor");
        return NULL;
    }

    STRATA_CPP_TRY

    auto result_values = strata::eval_jsonpath(*cursor_ptr, *self->path);
    return json_value_list_to_python(result_values);

    STRATA_CPP_CATCH
}

//=============================================================================
// search() and compile_path() Functions
//=============================================================================

PyObject* strata_compile_path(PyObject* self, PyObject* args) {
    const char* path_str;

    if (!PyArg_ParseTuple(args, "s", &path_str)) {
        return NULL;
    }

    STRATA_CPP_TRY

    auto result = strata::compile_jsonpath(path_str);
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
        return NULL;
    }

    PyCompiledPath* self_obj = (PyCompiledPath*)PyType_GenericAlloc(&PyCompiledPathType, 0);
    if (!self_obj)
        return NULL;

    self_obj->path = new strata::CompiledPath(std::move(result.value));

    return (PyObject*)self_obj;

    STRATA_CPP_CATCH
}

PyObject* strata_search(PyObject* self, PyObject* args) {
    PyObject* data_obj;
    PyObject* path_obj;

    if (!PyArg_ParseTuple(args, "OO", &data_obj, &path_obj)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Compile the path (if it's a string)
    strata::CompiledPath compiled_path;

    if (PyUnicode_Check(path_obj)) {
        // String path - compile it
        const char* path_str = PyUnicode_AsUTF8(path_obj);
        if (!path_str)
            return NULL;

        auto compile_result = strata::compile_jsonpath(path_str);
        if (!compile_result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
            return NULL;
        }
        compiled_path = std::move(compile_result.value);
    } else if (Py_TYPE(path_obj) == &PyCompiledPathType) {
        // Pre-compiled path - use it directly
        PyCompiledPath* compiled_obj = (PyCompiledPath*)path_obj;
        compiled_path = *compiled_obj->path;
    } else {
        PyErr_SetString(PyExc_TypeError, "path must be a string or CompiledPath");
        return NULL;
    }

    // Handle JsonDocument/JsonCursor inputs (parsed once, no reparse)
    if (is_py_json_document(data_obj)) {
        auto* doc = get_py_json_document(data_obj);
        if (!doc) {
            PyErr_SetString(PyExc_TypeError, "Invalid JsonDocument");
            return NULL;
        }

        strata::JsonCursor cursor(doc->root());
        auto result_values = strata::eval_jsonpath(cursor, compiled_path);
        return json_value_list_to_python(result_values);
    }

    if (is_py_json_cursor(data_obj)) {
        auto* cursor_ptr = get_py_json_cursor(data_obj);
        if (!cursor_ptr) {
            PyErr_SetString(PyExc_TypeError, "Invalid JsonCursor");
            return NULL;
        }

        auto result_values = strata::eval_jsonpath(*cursor_ptr, compiled_path);
        return json_value_list_to_python(result_values);
    }

    // Handle string input (JSON text)
    if (PyUnicode_Check(data_obj)) {
        Py_ssize_t json_len;
        const char* json_data = PyUnicode_AsUTF8AndSize(data_obj, &json_len);
        if (!json_data)
            return NULL;

        // Parse JSON text
        auto parse_result = strata::parse_json(std::string_view(json_data, json_len));
        if (!parse_result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return NULL;
        }

        emit_duplicate_key_warnings();

        // Execute query
        strata::JsonCursor cursor(&parse_result.value);
        auto result_values = strata::eval_jsonpath(cursor, compiled_path);
        return json_value_list_to_python(result_values);
    }

    if (PyBytes_Check(data_obj)) {
        char* json_data = nullptr;
        Py_ssize_t json_len = 0;
        if (PyBytes_AsStringAndSize(data_obj, &json_data, &json_len) < 0) {
            return NULL;
        }
        auto parse_result = strata::parse_json(std::string_view(json_data, json_len));
        if (!parse_result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return NULL;
        }

        emit_duplicate_key_warnings();

        strata::JsonCursor cursor(&parse_result.value);
        auto result_values = strata::eval_jsonpath(cursor, compiled_path);
        return json_value_list_to_python(result_values);
    }

    // Handle Python object (dict/list/scalar) - convert directly
    PyObject* memo = make_cycle_memo();
    if (!memo) {
        return NULL;
    }
    bool ok = true;
    strata::JsonValue root = pyobject_to_json_value(data_obj, memo, ok);
    Py_DECREF(memo);
    if (!ok || PyErr_Occurred()) {
        return NULL;
    }

    strata::JsonCursor cursor(&root);
    auto result_values = strata::eval_jsonpath(cursor, compiled_path);
    return json_value_list_to_python(result_values);

    STRATA_CPP_CATCH
}

//=============================================================================
// Module Registration
//=============================================================================

int register_jsonpath_types(PyObject* module) {
    // Register CompiledPath type
    if (PyType_Ready(&PyCompiledPathType) < 0) {
        return -1;
    }
    Py_INCREF(&PyCompiledPathType);
    if (PyModule_AddObject(module, "CompiledPath", (PyObject*)&PyCompiledPathType) < 0) {
        Py_DECREF(&PyCompiledPathType);
        return -1;
    }

    return 0;
}

// Methods are added to main module via python_module.cpp
// No need for get_jsonpath_methods() here

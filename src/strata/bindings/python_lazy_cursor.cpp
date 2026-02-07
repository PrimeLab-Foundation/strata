#include "python_types.h"
#include "strata/json/json_lazy_cursor.hpp"
#include "strata/json/json_parse.hpp"

#include <string>

// ============================================================================
// LazyCursor Python Type
// ============================================================================

struct LazyCursorObject {
    PyObject_HEAD
    strata::LazyJsonCursor* cursor;
    PyObject* input_owner;  // Keep reference to input string to ensure lifetime
};

static void LazyCursor_dealloc(LazyCursorObject* self) {
    delete self->cursor;
    Py_XDECREF(self->input_owner);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* LazyCursor_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    LazyCursorObject* self = (LazyCursorObject*)type->tp_alloc(type, 0);
    if (self != nullptr) {
        self->cursor = nullptr;
        self->input_owner = nullptr;
    }
    return (PyObject*)self;
}

static int LazyCursor_init(LazyCursorObject* self, PyObject* args, PyObject* kwds) {
    static const char* kwlist[] = {"json_input", nullptr};
    PyObject* input_obj = nullptr;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", const_cast<char**>(kwlist), &input_obj)) {
        return -1;
    }

    const char* data = nullptr;
    Py_ssize_t len = 0;

    if (PyBytes_Check(input_obj)) {
        data = PyBytes_AS_STRING(input_obj);
        len = PyBytes_GET_SIZE(input_obj);
    } else if (PyUnicode_Check(input_obj)) {
        data = PyUnicode_AsUTF8AndSize(input_obj, &len);
        if (data == nullptr) {
            return -1;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "json_input must be str or bytes");
        return -1;
    }

    STRATA_CPP_TRY
    self->cursor = new strata::LazyJsonCursor(std::string_view(data, static_cast<size_t>(len)));
    Py_INCREF(input_obj);
    self->input_owner = input_obj;
    return 0;
    STRATA_CPP_CATCH
}

// Type detection methods
static PyObject* LazyCursor_is_null(LazyCursorObject* self, PyObject* Py_UNUSED(ignored)) {
    if (self->cursor->is_null()) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* LazyCursor_is_bool(LazyCursorObject* self, PyObject* Py_UNUSED(ignored)) {
    if (self->cursor->is_bool()) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* LazyCursor_is_number(LazyCursorObject* self, PyObject* Py_UNUSED(ignored)) {
    if (self->cursor->is_number()) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* LazyCursor_is_string(LazyCursorObject* self, PyObject* Py_UNUSED(ignored)) {
    if (self->cursor->is_string()) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* LazyCursor_is_array(LazyCursorObject* self, PyObject* Py_UNUSED(ignored)) {
    if (self->cursor->is_array()) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* LazyCursor_is_object(LazyCursorObject* self, PyObject* Py_UNUSED(ignored)) {
    if (self->cursor->is_object()) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

// Helper to create a new LazyCursor from an existing cursor
static PyObject* create_lazy_cursor_from(
    strata::LazyJsonCursor&& cursor,
    PyObject* input_owner,
    PyTypeObject* type
) {
    LazyCursorObject* new_obj = (LazyCursorObject*)type->tp_alloc(type, 0);
    if (new_obj == nullptr) {
        return nullptr;
    }

    new_obj->cursor = new strata::LazyJsonCursor(std::move(cursor));
    Py_INCREF(input_owner);
    new_obj->input_owner = input_owner;

    return (PyObject*)new_obj;
}

// Navigation: get_field
static PyObject* LazyCursor_get_field(LazyCursorObject* self, PyObject* args) {
    const char* key = nullptr;
    Py_ssize_t key_len = 0;

    if (!PyArg_ParseTuple(args, "s#", &key, &key_len)) {
        return nullptr;
    }

    STRATA_CPP_TRY
    auto result = self->cursor->get_field(std::string_view(key, static_cast<size_t>(key_len)));

    if (!result.ok()) {
        if (result.status == strata::Status::KeyNotFound) {
            PyErr_SetString(PyExc_KeyError, key);
        } else if (result.status == strata::Status::TypeMismatch) {
            PyErr_SetString(PyExc_TypeError, "Not an object");
        } else {
            PyErr_SetString(PyExc_ValueError, "Parse error");
        }
        return nullptr;
    }

    return create_lazy_cursor_from(std::move(result.value), self->input_owner, Py_TYPE(self));
    STRATA_CPP_CATCH
}

// Navigation: get_at
static PyObject* LazyCursor_get_at(LazyCursorObject* self, PyObject* args) {
    Py_ssize_t index = 0;

    if (!PyArg_ParseTuple(args, "n", &index)) {
        return nullptr;
    }

    if (index < 0) {
        PyErr_SetString(PyExc_IndexError, "Negative indices not supported");
        return nullptr;
    }

    STRATA_CPP_TRY
    auto result = self->cursor->get_at(static_cast<size_t>(index));

    if (!result.ok()) {
        if (result.status == strata::Status::IndexOutOfBounds) {
            PyErr_SetString(PyExc_IndexError, "Index out of bounds");
        } else if (result.status == strata::Status::TypeMismatch) {
            PyErr_SetString(PyExc_TypeError, "Not an array");
        } else {
            PyErr_SetString(PyExc_ValueError, "Parse error");
        }
        return nullptr;
    }

    return create_lazy_cursor_from(std::move(result.value), self->input_owner, Py_TYPE(self));
    STRATA_CPP_CATCH
}

// __getitem__ support (for both keys and indices)
static PyObject* LazyCursor_getitem(LazyCursorObject* self, PyObject* key) {
    if (PyUnicode_Check(key)) {
        PyObject* args = PyTuple_Pack(1, key);
        if (args == nullptr) return nullptr;
        PyObject* result = LazyCursor_get_field(self, args);
        Py_DECREF(args);
        return result;
    } else if (PyLong_Check(key)) {
        Py_ssize_t index = PyLong_AsSsize_t(key);
        if (index == -1 && PyErr_Occurred()) {
            return nullptr;
        }
        PyObject* args = Py_BuildValue("(n)", index);
        if (args == nullptr) return nullptr;
        PyObject* result = LazyCursor_get_at(self, args);
        Py_DECREF(args);
        return result;
    } else {
        PyErr_SetString(PyExc_TypeError, "Key must be str or int");
        return nullptr;
    }
}

// Materialization: value()
static PyObject* LazyCursor_value(LazyCursorObject* self, PyObject* Py_UNUSED(ignored));

// Forward declaration for recursive conversion
static PyObject* json_value_to_python(const strata::JsonValue& value);

static PyObject* json_value_to_python(const strata::JsonValue& value) {
    if (value.is_null()) {
        Py_RETURN_NONE;
    }
    if (value.is_bool()) {
        if (value.as_bool()) {
            Py_RETURN_TRUE;
        }
        Py_RETURN_FALSE;
    }
    if (value.is_number()) {
        return PyFloat_FromDouble(value.as_number());
    }
    if (value.is_string()) {
        const std::string& s = value.as_string();
        return PyUnicode_FromStringAndSize(s.data(), static_cast<Py_ssize_t>(s.size()));
    }
    if (value.is_array()) {
        const auto& arr = value.as_array();
        PyObject* list = PyList_New(static_cast<Py_ssize_t>(arr.size()));
        if (list == nullptr) return nullptr;
        for (size_t i = 0; i < arr.size(); ++i) {
            PyObject* item = json_value_to_python(arr[i]);
            if (item == nullptr) {
                Py_DECREF(list);
                return nullptr;
            }
            PyList_SET_ITEM(list, static_cast<Py_ssize_t>(i), item);
        }
        return list;
    }
    if (value.is_object()) {
        const auto& obj = value.as_object();
        PyObject* dict = PyDict_New();
        if (dict == nullptr) return nullptr;
        for (const auto& [k, v] : obj) {
            PyObject* py_key = PyUnicode_FromStringAndSize(k.data(), static_cast<Py_ssize_t>(k.size()));
            if (py_key == nullptr) {
                Py_DECREF(dict);
                return nullptr;
            }
            PyObject* py_val = json_value_to_python(v);
            if (py_val == nullptr) {
                Py_DECREF(py_key);
                Py_DECREF(dict);
                return nullptr;
            }
            int rc = PyDict_SetItem(dict, py_key, py_val);
            Py_DECREF(py_key);
            Py_DECREF(py_val);
            if (rc < 0) {
                Py_DECREF(dict);
                return nullptr;
            }
        }
        return dict;
    }
    Py_RETURN_NONE;
}

static PyObject* LazyCursor_value(LazyCursorObject* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CPP_TRY
    // Fast paths for primitives
    if (self->cursor->is_null()) {
        Py_RETURN_NONE;
    }
    if (self->cursor->is_bool()) {
        auto result = self->cursor->materialize_bool();
        if (!result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Failed to materialize bool");
            return nullptr;
        }
        if (result.value) {
            Py_RETURN_TRUE;
        }
        Py_RETURN_FALSE;
    }
    if (self->cursor->is_number()) {
        auto result = self->cursor->materialize_number();
        if (!result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Failed to materialize number");
            return nullptr;
        }
        return PyFloat_FromDouble(result.value);
    }
    if (self->cursor->is_string()) {
        auto result = self->cursor->materialize_string();
        if (!result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Failed to materialize string");
            return nullptr;
        }
        return PyUnicode_FromStringAndSize(result.value.data(),
                                           static_cast<Py_ssize_t>(result.value.size()));
    }

    // For containers, use full materialization
    auto result = self->cursor->materialize();
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Failed to materialize value");
        return nullptr;
    }

    return json_value_to_python(result.value);
    STRATA_CPP_CATCH
}

// Keys iterator for objects
static PyObject* LazyCursor_keys(LazyCursorObject* self, PyObject* Py_UNUSED(ignored)) {
    if (!self->cursor->is_object()) {
        PyErr_SetString(PyExc_TypeError, "Not an object");
        return nullptr;
    }

    STRATA_CPP_TRY
    auto iter_result = self->cursor->iter_fields();
    if (!iter_result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Failed to iterate fields");
        return nullptr;
    }

    PyObject* list = PyList_New(0);
    if (list == nullptr) return nullptr;

    auto iter = iter_result.value;
    while (!iter.at_end()) {
        auto field = *iter;
        std::string key = field.key();
        PyObject* py_key = PyUnicode_FromStringAndSize(key.data(),
                                                       static_cast<Py_ssize_t>(key.size()));
        if (py_key == nullptr) {
            Py_DECREF(list);
            return nullptr;
        }
        if (PyList_Append(list, py_key) < 0) {
            Py_DECREF(py_key);
            Py_DECREF(list);
            return nullptr;
        }
        Py_DECREF(py_key);
        ++iter;
    }

    // Return iterator over the list
    PyObject* iter_obj = PyObject_GetIter(list);
    Py_DECREF(list);
    return iter_obj;
    STRATA_CPP_CATCH
}

// __len__ support
static Py_ssize_t LazyCursor_length(LazyCursorObject* self) {
    if (self->cursor->is_array()) {
        auto result = self->cursor->array_size();
        if (result.ok()) {
            return static_cast<Py_ssize_t>(result.value);
        }
    } else if (self->cursor->is_object()) {
        auto result = self->cursor->object_size();
        if (result.ok()) {
            return static_cast<Py_ssize_t>(result.value);
        }
    }
    return -1;
}

// Method definitions
static PyMethodDef LazyCursor_methods[] = {
    {"is_null", (PyCFunction)LazyCursor_is_null, METH_NOARGS,
     "Check if current value is null"},
    {"is_bool", (PyCFunction)LazyCursor_is_bool, METH_NOARGS,
     "Check if current value is a boolean"},
    {"is_number", (PyCFunction)LazyCursor_is_number, METH_NOARGS,
     "Check if current value is a number"},
    {"is_string", (PyCFunction)LazyCursor_is_string, METH_NOARGS,
     "Check if current value is a string"},
    {"is_array", (PyCFunction)LazyCursor_is_array, METH_NOARGS,
     "Check if current value is an array"},
    {"is_object", (PyCFunction)LazyCursor_is_object, METH_NOARGS,
     "Check if current value is an object"},
    {"get_field", (PyCFunction)LazyCursor_get_field, METH_VARARGS,
     "Navigate to object field by key"},
    {"get_at", (PyCFunction)LazyCursor_get_at, METH_VARARGS,
     "Navigate to array element by index"},
    {"value", (PyCFunction)LazyCursor_value, METH_NOARGS,
     "Materialize current value to Python object"},
    {"keys", (PyCFunction)LazyCursor_keys, METH_NOARGS,
     "Iterate over object keys (without materializing values)"},
    {nullptr, nullptr, 0, nullptr}
};

// Mapping protocol for __getitem__
static PyMappingMethods LazyCursor_as_mapping = {
    (lenfunc)LazyCursor_length,      // mp_length
    (binaryfunc)LazyCursor_getitem,  // mp_subscript
    nullptr                          // mp_ass_subscript
};

// Type definition
static PyTypeObject LazyCursorType = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "_strata.LazyCursor",              // tp_name
    sizeof(LazyCursorObject),          // tp_basicsize
    0,                                 // tp_itemsize
    (destructor)LazyCursor_dealloc,    // tp_dealloc
    0,                                 // tp_vectorcall_offset
    nullptr,                           // tp_getattr
    nullptr,                           // tp_setattr
    nullptr,                           // tp_as_async
    nullptr,                           // tp_repr
    nullptr,                           // tp_as_number
    nullptr,                           // tp_as_sequence
    &LazyCursor_as_mapping,            // tp_as_mapping
    nullptr,                           // tp_hash
    nullptr,                           // tp_call
    nullptr,                           // tp_str
    nullptr,                           // tp_getattro
    nullptr,                           // tp_setattro
    nullptr,                           // tp_as_buffer
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // tp_flags
    "Lazy cursor for on-demand JSON parsing",  // tp_doc
    nullptr,                           // tp_traverse
    nullptr,                           // tp_clear
    nullptr,                           // tp_richcompare
    0,                                 // tp_weaklistoffset
    nullptr,                           // tp_iter
    nullptr,                           // tp_iternext
    LazyCursor_methods,                // tp_methods
    nullptr,                           // tp_members
    nullptr,                           // tp_getset
    nullptr,                           // tp_base
    nullptr,                           // tp_dict
    nullptr,                           // tp_descr_get
    nullptr,                           // tp_descr_set
    0,                                 // tp_dictoffset
    (initproc)LazyCursor_init,         // tp_init
    nullptr,                           // tp_alloc
    LazyCursor_new,                    // tp_new
};

// ============================================================================
// Module-level functions
// ============================================================================

// lazy() function - create a LazyCursor from JSON input
PyObject* strata_lazy(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* input_obj = nullptr;

    if (!PyArg_ParseTuple(args, "O", &input_obj)) {
        return nullptr;
    }

    // Create new LazyCursor
    PyObject* cursor_args = PyTuple_Pack(1, input_obj);
    if (cursor_args == nullptr) return nullptr;

    PyObject* cursor = PyObject_Call((PyObject*)&LazyCursorType, cursor_args, nullptr);
    Py_DECREF(cursor_args);

    return cursor;
}

// ============================================================================
// Registration
// ============================================================================

int register_lazy_cursor_types(PyObject* module) {
    if (PyType_Ready(&LazyCursorType) < 0) {
        return -1;
    }

    Py_INCREF(&LazyCursorType);
    if (PyModule_AddObject(module, "LazyCursor", (PyObject*)&LazyCursorType) < 0) {
        Py_DECREF(&LazyCursorType);
        return -1;
    }

    return 0;
}

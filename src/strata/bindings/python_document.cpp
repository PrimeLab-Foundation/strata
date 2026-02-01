#include "python_document.h"

#include "python_convert.h"
#include "python_types.h"
#include "strata/json/json_cursor.hpp"
#include "strata/json/json_document.hpp"

// Run a block inside STRATA_CPP_TRY/STRATA_CPP_CATCH for cursor getters and type checks.
#define STRATA_CURSOR_TRY_RETURN_BLOCK(block)                                                      \
    STRATA_CPP_TRY                                                                                 \
    block STRATA_CPP_CATCH

//=============================================================================
// Type Structures
//=============================================================================

typedef struct {
    PyObject_HEAD strata::JsonDocument* doc;
} PyJsonDocument;

typedef struct {
    PyObject_HEAD strata::JsonCursor* cursor;
    PyObject* doc_ref; // Keep document alive
} PyJsonCursor;

// Forward declarations of all methods
static void PyJsonDocument_dealloc(PyJsonDocument* self);
static PyObject* PyJsonDocument_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static PyObject* PyJsonDocument_from_string(PyObject* cls, PyObject* args);
static PyObject* PyJsonDocument_root(PyJsonDocument* self, PyObject* Py_UNUSED(ignored));

static void PyJsonCursor_dealloc(PyJsonCursor* self);
static PyObject* PyJsonCursor_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static PyObject* PyJsonCursor_to_python(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_field(PyJsonCursor* self, PyObject* args);
static PyObject* PyJsonCursor_at(PyJsonCursor* self, PyObject* args);
static PyObject* PyJsonCursor_get_str(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_get_int(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_get_float(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_get_bool(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_is_object(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_is_array(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_is_null(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_is_bool(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_is_number(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyJsonCursor_is_string(PyJsonCursor* self, PyObject* Py_UNUSED(ignored));

// Method tables
static PyMethodDef PyJsonDocument_methods[] = {
    {"from_string", (PyCFunction)PyJsonDocument_from_string, METH_VARARGS | METH_CLASS,
     "Create JsonDocument from JSON string"},
    {"root", (PyCFunction)PyJsonDocument_root, METH_NOARGS, "Get cursor at root"},
    {NULL, NULL, 0, NULL}};

static PyMethodDef PyJsonCursor_methods[] = {
    {"to_python", (PyCFunction)PyJsonCursor_to_python, METH_NOARGS, "Convert to Python object"},
    {"field", (PyCFunction)PyJsonCursor_field, METH_VARARGS, "Get field by name"},
    {"at", (PyCFunction)PyJsonCursor_at, METH_VARARGS, "Get array element by index"},
    {"get_str", (PyCFunction)PyJsonCursor_get_str, METH_NOARGS, "Get string value"},
    {"get_int", (PyCFunction)PyJsonCursor_get_int, METH_NOARGS, "Get integer value"},
    {"get_float", (PyCFunction)PyJsonCursor_get_float, METH_NOARGS, "Get float value"},
    {"get_bool", (PyCFunction)PyJsonCursor_get_bool, METH_NOARGS, "Get boolean value"},
    {"is_object", (PyCFunction)PyJsonCursor_is_object, METH_NOARGS, "Check if value is object"},
    {"is_array", (PyCFunction)PyJsonCursor_is_array, METH_NOARGS, "Check if value is array"},
    {"is_null", (PyCFunction)PyJsonCursor_is_null, METH_NOARGS, "Check if value is null"},
    {"is_bool", (PyCFunction)PyJsonCursor_is_bool, METH_NOARGS, "Check if value is bool"},
    {"is_number", (PyCFunction)PyJsonCursor_is_number, METH_NOARGS, "Check if value is number"},
    {"is_string", (PyCFunction)PyJsonCursor_is_string, METH_NOARGS, "Check if value is string"},
    {NULL, NULL, 0, NULL}};

// Type objects
static PyTypeObject PyJsonDocumentType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.JsonDocument",
    .tp_basicsize = sizeof(PyJsonDocument),
    .tp_dealloc = (destructor)PyJsonDocument_dealloc,
    .tp_new = PyJsonDocument_new,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "JSON Document",
    .tp_methods = PyJsonDocument_methods,
};

PyTypeObject PyJsonCursorType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.JsonCursor",
    .tp_basicsize = sizeof(PyJsonCursor),
    .tp_dealloc = (destructor)PyJsonCursor_dealloc,
    .tp_new = PyJsonCursor_new,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "JSON Cursor",
    .tp_methods = PyJsonCursor_methods,
};

//=============================================================================
// Type helpers (exported for other bindings)
//=============================================================================

bool is_py_json_document(PyObject* obj) { return obj && Py_TYPE(obj) == &PyJsonDocumentType; }

bool is_py_json_cursor(PyObject* obj) { return obj && Py_TYPE(obj) == &PyJsonCursorType; }

strata::JsonDocument* get_py_json_document(PyObject* obj) {
    if (!is_py_json_document(obj)) {
        return nullptr;
    }
    return ((PyJsonDocument*)obj)->doc;
}

strata::JsonCursor* get_py_json_cursor(PyObject* obj) {
    if (!is_py_json_cursor(obj)) {
        return nullptr;
    }
    return ((PyJsonCursor*)obj)->cursor;
}

//=============================================================================
// JsonDocument Method Implementations
//=============================================================================

static void PyJsonDocument_dealloc(PyJsonDocument* self) {
    delete self->doc;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyJsonDocument_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyJsonDocument* self = (PyJsonDocument*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->doc = nullptr;
    }
    return (PyObject*)self;
}

static PyObject* PyJsonDocument_from_string(PyObject* cls, PyObject* args) {
    const char* text;
    Py_ssize_t len;

    if (!PyArg_ParseTuple(args, "s#", &text, &len)) {
        return NULL;
    }

    STRATA_CPP_TRY

    auto result = strata::parse_json(std::string_view(text, len));
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        return NULL;
    }

    PyJsonDocument* self = (PyJsonDocument*)PyJsonDocument_new(&PyJsonDocumentType, NULL, NULL);
    if (!self)
        return NULL;

    self->doc = new strata::JsonDocument(std::move(result.value));

    return (PyObject*)self;

    STRATA_CPP_CATCH
}

static PyObject* PyJsonDocument_root(PyJsonDocument* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CPP_TRY

    PyJsonCursor* cursor = (PyJsonCursor*)PyJsonCursor_new(&PyJsonCursorType, NULL, NULL);
    if (!cursor)
        return NULL;

    cursor->cursor = new strata::JsonCursor(self->doc->root());
    cursor->doc_ref = (PyObject*)self;
    Py_INCREF(cursor->doc_ref);

    return (PyObject*)cursor;

    STRATA_CPP_CATCH
}

//=============================================================================
// JsonCursor Method Implementations
//=============================================================================

static void PyJsonCursor_dealloc(PyJsonCursor* self) {
    delete self->cursor;
    Py_XDECREF(self->doc_ref);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyJsonCursor_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyJsonCursor* self = (PyJsonCursor*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cursor = nullptr;
        self->doc_ref = nullptr;
    }
    return (PyObject*)self;
}

static PyObject* PyJsonCursor_to_python(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CPP_TRY
    const strata::JsonValue* val = self->cursor->raw();
    if (!val) {
        Py_RETURN_NONE;
    }
    return json_value_to_python(*val);
    STRATA_CPP_CATCH
}

static PyObject* PyJsonCursor_field(PyJsonCursor* self, PyObject* args) {
    const char* field;
    if (!PyArg_ParseTuple(args, "s", &field)) {
        return NULL;
    }

    STRATA_CPP_TRY

    strata::JsonCursor result = self->cursor->field(field);
    if (!result.raw()) {
        Py_RETURN_NONE;
    }

    PyJsonCursor* new_cursor = (PyJsonCursor*)PyJsonCursor_new(&PyJsonCursorType, NULL, NULL);
    if (!new_cursor)
        return NULL;

    new_cursor->cursor = new strata::JsonCursor(std::move(result));
    new_cursor->doc_ref = self->doc_ref;
    Py_XINCREF(new_cursor->doc_ref);

    return (PyObject*)new_cursor;

    STRATA_CPP_CATCH
}

static PyObject* PyJsonCursor_at(PyJsonCursor* self, PyObject* args) {
    Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "n", &index)) {
        return NULL;
    }

    STRATA_CPP_TRY

    strata::JsonCursor result = self->cursor->at(index);
    if (!result.raw()) {
        Py_RETURN_NONE;
    }

    PyJsonCursor* new_cursor = (PyJsonCursor*)PyJsonCursor_new(&PyJsonCursorType, NULL, NULL);
    if (!new_cursor)
        return NULL;

    new_cursor->cursor = new strata::JsonCursor(std::move(result));
    new_cursor->doc_ref = self->doc_ref;
    Py_XINCREF(new_cursor->doc_ref);

    return (PyObject*)new_cursor;

    STRATA_CPP_CATCH
}

static PyObject* PyJsonCursor_get_str(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(
        std::string result = self->cursor->get_str();
        return PyUnicode_FromStringAndSize(result.data(), result.size());)
}

static PyObject* PyJsonCursor_get_int(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(return PyLong_FromLongLong(self->cursor->get_int());)
}

static PyObject* PyJsonCursor_get_float(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(return PyFloat_FromDouble(self->cursor->get_float());)
}

static PyObject* PyJsonCursor_get_bool(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(
        return PyBool_FromLong(self->cursor->get_bool_or_throw() ? 1 : 0);)
}

static PyObject* PyJsonCursor_is_object(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(return PyBool_FromLong(self->cursor->is_object() ? 1 : 0);)
}

static PyObject* PyJsonCursor_is_array(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(return PyBool_FromLong(self->cursor->is_array() ? 1 : 0);)
}

static PyObject* PyJsonCursor_is_null(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(return PyBool_FromLong(self->cursor->is_null() ? 1 : 0);)
}

static PyObject* PyJsonCursor_is_bool(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(return PyBool_FromLong(self->cursor->is_bool() ? 1 : 0);)
}

static PyObject* PyJsonCursor_is_number(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(return PyBool_FromLong(self->cursor->is_number() ? 1 : 0);)
}

static PyObject* PyJsonCursor_is_string(PyJsonCursor* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CURSOR_TRY_RETURN_BLOCK(return PyBool_FromLong(self->cursor->is_string() ? 1 : 0);)
}

//=============================================================================
// Helper Functions for Other Modules
//=============================================================================

PyObject* create_py_json_document(strata::JsonDocument&& doc) {
    PyJsonDocument* py_doc = (PyJsonDocument*)PyType_GenericAlloc(&PyJsonDocumentType, 0);
    if (!py_doc)
        return NULL;

    py_doc->doc = new strata::JsonDocument(std::move(doc));
    return (PyObject*)py_doc;
}

PyObject* create_py_json_cursor(strata::JsonCursor&& cursor, PyObject* doc_ref) {
    PyJsonCursor* py_cursor = (PyJsonCursor*)PyType_GenericAlloc(&PyJsonCursorType, 0);
    if (!py_cursor)
        return NULL;

    py_cursor->cursor = new strata::JsonCursor(std::move(cursor));
    py_cursor->doc_ref = doc_ref;
    Py_XINCREF(py_cursor->doc_ref);

    return (PyObject*)py_cursor;
}

//=============================================================================
// Module Registration
//=============================================================================

int register_document_types(PyObject* module) {
    // Prepare JsonDocument type
    if (PyType_Ready(&PyJsonDocumentType) < 0) {
        return -1;
    }
    Py_INCREF(&PyJsonDocumentType);
    if (PyModule_AddObject(module, "JsonDocument", (PyObject*)&PyJsonDocumentType) < 0) {
        Py_DECREF(&PyJsonDocumentType);
        return -1;
    }

    // Prepare JsonCursor type
    if (PyType_Ready(&PyJsonCursorType) < 0) {
        return -1;
    }
    Py_INCREF(&PyJsonCursorType);
    if (PyModule_AddObject(module, "JsonCursor", (PyObject*)&PyJsonCursorType) < 0) {
        Py_DECREF(&PyJsonCursorType);
        return -1;
    }

    return 0;
}

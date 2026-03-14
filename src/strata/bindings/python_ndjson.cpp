#include "python_convert.h"
#include "python_types.h"
#include "strata/json/ndjson_stream.hpp"

// Run a block inside STRATA_CPP_TRY/STRATA_CPP_CATCH for one-liner stream methods.
#define STRATA_NDJSON_TRY_RETURN_BLOCK(block)                                                      \
    STRATA_CPP_TRY                                                                                 \
    block STRATA_CPP_CATCH

//=============================================================================
// NdjsonStream Type
//=============================================================================

typedef struct {
    PyObject_HEAD strata::NdjsonStream* stream;
} PyNdjsonStream;

static void PyNdjsonStream_dealloc(PyNdjsonStream* self) {
    delete self->stream;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyNdjsonStream_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyNdjsonStream* self = (PyNdjsonStream*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->stream = nullptr;
    }
    return (PyObject*)self;
}

// Forward declarations
static PyObject* PyNdjsonStream_from_string(PyObject* cls, PyObject* args);
static PyObject* PyNdjsonStream_has_next(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyNdjsonStream_next_line(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyNdjsonStream_parse_all(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored));
static PyObject* PyNdjsonStream_next_batch(PyNdjsonStream* self, PyObject* args);
static PyObject* PyNdjsonStream_error_count(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored));

// Method table
static PyMethodDef PyNdjsonStream_methods[] = {
    {"from_string", (PyCFunction)PyNdjsonStream_from_string, METH_VARARGS | METH_CLASS,
     "Create NdjsonStream from string"},
    {"has_next", (PyCFunction)PyNdjsonStream_has_next, METH_NOARGS,
     "Check if there are more lines"},
    {"next_line", (PyCFunction)PyNdjsonStream_next_line, METH_NOARGS, "Parse next line"},
    {"parse_all", (PyCFunction)PyNdjsonStream_parse_all, METH_VARARGS, "Parse all lines"},
    {"next_batch", (PyCFunction)PyNdjsonStream_next_batch, METH_VARARGS,
     "Parse next batch of lines"},
    {"error_count", (PyCFunction)PyNdjsonStream_error_count, METH_NOARGS,
     "Get number of errors encountered"},
    {NULL, NULL, 0, NULL}};

// Type object
static PyTypeObject PyNdjsonStreamType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.NdjsonStream",
    .tp_basicsize = sizeof(PyNdjsonStream),
    .tp_dealloc = (destructor)PyNdjsonStream_dealloc,
    .tp_new = PyNdjsonStream_new,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "NDJSON Stream",
    .tp_methods = PyNdjsonStream_methods,
};

//=============================================================================
// Method Implementations
//=============================================================================

static PyObject* PyNdjsonStream_from_string(PyObject* cls, PyObject* args) {
    const char* text;
    Py_ssize_t len;

    if (!PyArg_ParseTuple(args, "s#", &text, &len)) {
        return NULL;
    }

    STRATA_CPP_TRY

    PyNdjsonStream* self = (PyNdjsonStream*)PyType_GenericAlloc(&PyNdjsonStreamType, 0);
    if (!self)
        return NULL;

    self->stream = new strata::NdjsonStream(std::string_view(text, len));

    return (PyObject*)self;

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonStream_has_next(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_NDJSON_TRY_RETURN_BLOCK(return PyBool_FromLong(self->stream->has_next() ? 1 : 0);)
}

// next_line() uses the SAX path: parse directly to Python objects without C++ DOM.
static PyObject* PyNdjsonStream_next_line(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CPP_TRY

    std::string_view line = self->stream->read_raw_line();
    if (line.empty()) {
        // end of stream (read_raw_line returns empty only when no data remains)
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    // Parse directly to Python via SAX – no intermediate C++ DOM
    PyObject* result = parse_json_to_python(line, /*validate_utf8=*/false);
    if (!result) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON in NDJSON line");
        }
        return NULL;
    }
    return result;

    STRATA_CPP_CATCH
}

// parse_all() reuses a single PythonObjectBuilder (and its KeyCache) across all lines
// to avoid per-line allocations and Python key re-creation.
static PyObject* PyNdjsonStream_parse_all(PyNdjsonStream* self, PyObject* args) {
    int skip_errors = 1; // Default: skip errors

    if (!PyArg_ParseTuple(args, "|p", &skip_errors)) {
        return NULL;
    }

    STRATA_CPP_TRY

    return parse_ndjson_all_to_python(*self->stream, skip_errors);

    STRATA_CPP_CATCH
}

// next_batch() reuses a single PythonObjectBuilder (and its KeyCache) across all lines
// in the batch to avoid per-line allocations and Python key re-creation.
static PyObject* PyNdjsonStream_next_batch(PyNdjsonStream* self, PyObject* args) {
    Py_ssize_t batch_size = 100;
    int skip_errors = 1;

    if (!PyArg_ParseTuple(args, "|np", &batch_size, &skip_errors)) {
        return NULL;
    }

    STRATA_CPP_TRY

    return parse_ndjson_batch_to_python(*self->stream, batch_size, skip_errors);

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonStream_error_count(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_NDJSON_TRY_RETURN_BLOCK(return PyLong_FromSize_t(self->stream->error_count());)
}

//=============================================================================
// Module Registration
//=============================================================================

int register_ndjson_types(PyObject* module) {
    if (PyType_Ready(&PyNdjsonStreamType) < 0) {
        return -1;
    }
    Py_INCREF(&PyNdjsonStreamType);
    if (PyModule_AddObject(module, "NdjsonStream", (PyObject*)&PyNdjsonStreamType) < 0) {
        Py_DECREF(&PyNdjsonStreamType);
        return -1;
    }

    return 0;
}

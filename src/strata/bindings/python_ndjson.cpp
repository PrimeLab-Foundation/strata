#include "python_convert.h"
#include "python_object_builder.h"
#include "python_types.h"
#include "strata/json/ndjson_stream.hpp"
#include "strata/json/parallel_ndjson.hpp"
#include "strata/util/arena_allocator.hpp"

#include <string>
#include <vector>

// Run a block inside STRATA_CPP_TRY/STRATA_CPP_CATCH for one-liner stream methods.
#define STRATA_NDJSON_TRY_RETURN_BLOCK(block)                                                      \
    STRATA_CPP_TRY                                                                                 \
    block STRATA_CPP_CATCH

//=============================================================================
// NdjsonStream Type
//=============================================================================

struct NdjsonPythonContext {
    strata::util::Arena key_arena;
    strata::bindings::KeyCache key_cache;
    strata::util::Arena builder_arena;
    strata::bindings::PythonObjectBuilder builder;

    NdjsonPythonContext()
        : key_cache(&key_arena), builder_arena(4 * 1024), builder(&builder_arena, key_cache) {}
};

typedef struct {
    PyObject_HEAD strata::NdjsonStream* stream;
    NdjsonPythonContext* context;
} PyNdjsonStream;

static void PyNdjsonStream_dealloc(PyNdjsonStream* self) {
    delete self->context;
    delete self->stream;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyNdjsonStream_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyNdjsonStream* self = (PyNdjsonStream*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->stream = nullptr;
        self->context = nullptr;
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

// Parallel NDJSON function (standalone, not a method on NdjsonStream)
static PyObject* parallel_parse_ndjson(PyObject* self, PyObject* args, PyObject* kwargs);

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

// Module-level parallel_parse_ndjson function - exposed as ndjson_parallel_parse_all
static PyMethodDef parallel_ndjson_methods[] = {
    {"ndjson_parallel_parse_all", (PyCFunction)parallel_parse_ndjson,
     METH_VARARGS | METH_KEYWORDS,
     "Parse NDJSON data in parallel.\n\n"
     "Args:\n"
     "    data (str): NDJSON string data\n"
     "    skip_errors (bool): Skip malformed lines (default: True)\n"
     "    num_threads (int): Number of threads (0 = auto-detect, default: 0)\n"
     "    min_chunk_size (int): Minimum bytes per chunk (0 = default)\n\n"
     "Returns:\n"
     "    list: Parsed JSON values in order\n"},
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
    self->context = new NdjsonPythonContext();

    return (PyObject*)self;

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonStream_has_next(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_NDJSON_TRY_RETURN_BLOCK(return PyBool_FromLong(self->stream->has_next() ? 1 : 0);)
}

static PyObject* PyNdjsonStream_next_line(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_CPP_TRY

    NdjsonPythonContext* context = self->context;
    context->builder.reset();
    auto status = self->stream->next_sax(context->builder);

    if (status != strata::Status::Ok) {
        context->builder.reset();
        if (status == strata::Status::KeyNotFound) {
            PyErr_SetNone(PyExc_StopIteration);
        } else if (status == strata::Status::ParseError) {
            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError, "Invalid JSON in NDJSON line");
            }
        } else {
            PyErr_SetString(PyExc_RuntimeError, "NDJSON parsing error");
        }
        return NULL;
    }

    PyObject* result = context->builder.take_root();
    if (!result) {
        context->builder.reset();
        PyErr_SetString(PyExc_RuntimeError, "NDJSON parse produced no result");
        return NULL;
    }
    return result;

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonStream_parse_all(PyNdjsonStream* self, PyObject* args) {
    int skip_errors = 1; // Default: skip errors

    if (!PyArg_ParseTuple(args, "|p", &skip_errors)) {
        return NULL;
    }

    STRATA_CPP_TRY
    std::vector<PyObject*> items;
    items.reserve(256);

    NdjsonPythonContext* context = self->context;
    self->stream->validate_utf8_once();
    PyGcPause gc_pause;

    while (true) {
        context->builder.reset();
        auto status = self->stream->next_sax(context->builder);
        if (status == strata::Status::Ok) {
            PyObject* obj = context->builder.take_root();
            if (!obj) {
                context->builder.reset();
                for (auto* item : items) {
                    Py_DECREF(item);
                }
                PyErr_SetString(PyExc_RuntimeError, "NDJSON parse produced no result");
                return NULL;
            }
            items.push_back(obj);
            continue;
        }

        context->builder.reset();
        if (status == strata::Status::KeyNotFound) {
            break;
        }

        if (PyErr_Occurred()) {
            for (auto* item : items) {
                Py_DECREF(item);
            }
            return NULL;
        }

        if (!skip_errors) {
            break;
        }
    }

    PyObject* list = PyList_New(items.size());
    if (!list) {
        for (auto* item : items) {
            Py_DECREF(item);
        }
        return NULL;
    }
    for (size_t i = 0; i < items.size(); ++i) {
        PyList_SET_ITEM(list, i, items[i]);
    }
    return list;

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonStream_next_batch(PyNdjsonStream* self, PyObject* args) {
    Py_ssize_t batch_size = 100;
    int skip_errors = 1;

    if (!PyArg_ParseTuple(args, "|np", &batch_size, &skip_errors)) {
        return NULL;
    }

    STRATA_CPP_TRY
    if (batch_size < 0) {
        PyErr_SetString(PyExc_ValueError, "batch_size must be non-negative");
        return NULL;
    }

    std::vector<PyObject*> items;
    items.reserve(static_cast<size_t>(batch_size));

    NdjsonPythonContext* context = self->context;
    self->stream->validate_utf8_once();
    PyGcPause gc_pause;

    while (items.size() < static_cast<size_t>(batch_size)) {
        context->builder.reset();
        auto status = self->stream->next_sax(context->builder);
        if (status == strata::Status::Ok) {
            PyObject* obj = context->builder.take_root();
            if (!obj) {
                context->builder.reset();
                for (auto* item : items) {
                    Py_DECREF(item);
                }
                PyErr_SetString(PyExc_RuntimeError, "NDJSON parse produced no result");
                return NULL;
            }
            items.push_back(obj);
            continue;
        }

        context->builder.reset();
        if (status == strata::Status::KeyNotFound) {
            break;
        }

        if (PyErr_Occurred()) {
            for (auto* item : items) {
                Py_DECREF(item);
            }
            return NULL;
        }

        if (!skip_errors) {
            break;
        }
    }

    PyObject* list = PyList_New(items.size());
    if (!list) {
        for (auto* item : items) {
            Py_DECREF(item);
        }
        return NULL;
    }
    for (size_t i = 0; i < items.size(); ++i) {
        PyList_SET_ITEM(list, i, items[i]);
    }
    return list;

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonStream_error_count(PyNdjsonStream* self, PyObject* Py_UNUSED(ignored)) {
    STRATA_NDJSON_TRY_RETURN_BLOCK(return PyLong_FromSize_t(self->stream->error_count());)
}

//=============================================================================
// Parallel NDJSON Implementation
//=============================================================================

static PyObject* parallel_parse_ndjson(PyObject* self, PyObject* args, PyObject* kwargs) {
    (void)self;  // Unused
    const char* data;
    Py_ssize_t data_len;
    int skip_errors = 1;
    int num_threads = 0;
    Py_ssize_t min_chunk_size = 0;

    static const char* kwlist[] = {"data", "skip_errors", "num_threads", "min_chunk_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s#|pin", const_cast<char**>(kwlist), &data,
                                     &data_len, &skip_errors, &num_threads, &min_chunk_size)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Configure parallel parsing
    strata::ParallelNdjsonConfig config;
    config.skip_errors = skip_errors != 0;
    config.num_threads = num_threads > 0 ? static_cast<size_t>(num_threads) : 0;
    if (min_chunk_size < 0) {
        PyErr_SetString(PyExc_ValueError, "min_chunk_size must be non-negative");
        return NULL;
    }
    if (min_chunk_size > 0) {
        config.min_chunk_size = static_cast<size_t>(min_chunk_size);
    }

    // Create parallel stream
    strata::ParallelNdjsonStream stream(std::string_view(data, data_len), config);

    // Parse - GIL is held since C++ exceptions need to be converted to Python exceptions
    // The parallel parsing is still multi-threaded in C++, just the main thread waits with GIL
    std::vector<strata::JsonValue> cpp_results = stream.parse_all_parallel();

    // Convert to Python list (uses json_value_list_to_python from python_convert.h)
    return json_value_list_to_python(cpp_results);

    STRATA_CPP_CATCH
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

    // Add parallel NDJSON functions to module
    for (PyMethodDef* method = parallel_ndjson_methods; method->ml_name != NULL; ++method) {
        PyObject* func = PyCFunction_New(method, NULL);
        if (!func) {
            return -1;
        }
        if (PyModule_AddObject(module, method->ml_name, func) < 0) {
            Py_DECREF(func);
            return -1;
        }
    }

    return 0;
}

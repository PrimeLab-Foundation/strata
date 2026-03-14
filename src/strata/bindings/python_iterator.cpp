#include "python_convert.h"
#include "python_types.h"
#include "strata/json/ndjson_stream.hpp"

#include <cstring>
#include <fstream>
#include <string>

//=============================================================================
// StrataListIterator — iterates over a Python list, yielding one element at a time
//=============================================================================

typedef struct {
    PyObject_HEAD PyObject* source; // owned ref to the list
    Py_ssize_t index;
    Py_ssize_t size;
} StrataListIterator;

static void StrataListIterator_dealloc(StrataListIterator* self) {
    Py_XDECREF(self->source);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* StrataListIterator_iter(PyObject* self) {
    Py_INCREF(self);
    return self;
}

static PyObject* StrataListIterator_next(StrataListIterator* self) {
    if (self->index >= self->size) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
    PyObject* item = PyList_GET_ITEM(self->source, self->index);
    self->index++;
    Py_INCREF(item);
    return item;
}

static PyTypeObject StrataListIteratorType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.ListIterator",
    .tp_basicsize = sizeof(StrataListIterator),
    .tp_dealloc = (destructor)StrataListIterator_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Iterator over a list",
    .tp_iter = StrataListIterator_iter,
    .tp_iternext = (iternextfunc)StrataListIterator_next,
};

//=============================================================================
// StrataDictIterator — iterates over a Python dict, yielding (key, value) tuples
//=============================================================================

typedef struct {
    PyObject_HEAD PyObject* source; // owned ref to the dict
    Py_ssize_t pos;                 // iteration position for PyDict_Next
} StrataDictIterator;

static void StrataDictIterator_dealloc(StrataDictIterator* self) {
    Py_XDECREF(self->source);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* StrataDictIterator_iter(PyObject* self) {
    Py_INCREF(self);
    return self;
}

static PyObject* StrataDictIterator_next(StrataDictIterator* self) {
    PyObject* key = NULL;
    PyObject* value = NULL;
    if (!PyDict_Next(self->source, &self->pos, &key, &value)) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
    // Return (key, value) tuple
    PyObject* tuple = PyTuple_New(2);
    if (!tuple)
        return NULL;
    Py_INCREF(key);
    Py_INCREF(value);
    PyTuple_SET_ITEM(tuple, 0, key);
    PyTuple_SET_ITEM(tuple, 1, value);
    return tuple;
}

static PyTypeObject StrataDictIteratorType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.DictIterator",
    .tp_basicsize = sizeof(StrataDictIterator),
    .tp_dealloc = (destructor)StrataDictIterator_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Iterator over a dict yielding (key, value) tuples",
    .tp_iter = StrataDictIterator_iter,
    .tp_iternext = (iternextfunc)StrataDictIterator_next,
};

//=============================================================================
// StrataNdjsonFileIterator — reads NDJSON file line by line, yielding parsed dicts
//=============================================================================

typedef struct {
    PyObject_HEAD std::string* file_content; // owned
    strata::NdjsonStream* stream;            // owned
} StrataNdjsonFileIterator;

static void StrataNdjsonFileIterator_dealloc(StrataNdjsonFileIterator* self) {
    delete self->stream;
    delete self->file_content;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* StrataNdjsonFileIterator_iter(PyObject* self) {
    Py_INCREF(self);
    return self;
}

static PyObject* StrataNdjsonFileIterator_next(StrataNdjsonFileIterator* self) {
    STRATA_CPP_TRY

    while (self->stream->has_next()) {
        std::string_view line = self->stream->read_raw_line();
        if (line.empty())
            continue;

        PyObject* item = parse_json_to_python(line, /*validate_utf8=*/false);
        if (!item) {
            if (PyErr_Occurred())
                PyErr_Clear();
            self->stream->record_error();
            continue;
        }
        return item;
    }

    PyErr_SetNone(PyExc_StopIteration);
    return NULL;

    STRATA_CPP_CATCH
}

static PyTypeObject StrataNdjsonFileIteratorType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.NdjsonFileIterator",
    .tp_basicsize = sizeof(StrataNdjsonFileIterator),
    .tp_dealloc = (destructor)StrataNdjsonFileIterator_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Streaming iterator over NDJSON file lines",
    .tp_iter = StrataNdjsonFileIterator_iter,
    .tp_iternext = (iternextfunc)StrataNdjsonFileIterator_next,
};

//=============================================================================
// Factory functions (called from other modules)
//=============================================================================

PyObject* create_list_iterator(PyObject* list) {
    StrataListIterator* it = (StrataListIterator*)PyType_GenericAlloc(&StrataListIteratorType, 0);
    if (!it)
        return NULL;
    Py_INCREF(list);
    it->source = list;
    it->index = 0;
    it->size = PyList_GET_SIZE(list);
    return (PyObject*)it;
}

PyObject* create_dict_iterator(PyObject* dict) {
    StrataDictIterator* it = (StrataDictIterator*)PyType_GenericAlloc(&StrataDictIteratorType, 0);
    if (!it)
        return NULL;
    Py_INCREF(dict);
    it->source = dict;
    it->pos = 0;
    return (PyObject*)it;
}

PyObject* create_ndjson_file_iterator(const char* filepath) {
    STRATA_CPP_TRY

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        PyErr_Format(PyExc_FileNotFoundError, "Cannot open file: %s", filepath);
        return NULL;
    }
    auto size = file.tellg();
    file.seekg(0);

    auto* content = new std::string(static_cast<size_t>(size), '\0');
    file.read(content->data(), size);
    file.close();

    auto* stream = new strata::NdjsonStream(*content);

    StrataNdjsonFileIterator* it =
        (StrataNdjsonFileIterator*)PyType_GenericAlloc(&StrataNdjsonFileIteratorType, 0);
    if (!it) {
        delete stream;
        delete content;
        return NULL;
    }
    it->file_content = content;
    it->stream = stream;
    return (PyObject*)it;

    STRATA_CPP_CATCH
}

//=============================================================================
// Module Registration
//=============================================================================

int register_iterator_type(PyObject* module) {
    if (PyType_Ready(&StrataListIteratorType) < 0)
        return -1;
    if (PyType_Ready(&StrataDictIteratorType) < 0)
        return -1;
    if (PyType_Ready(&StrataNdjsonFileIteratorType) < 0)
        return -1;
    return 0;
}

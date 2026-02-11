#ifndef STRATA_PYTHON_DOCUMENT_H
#define STRATA_PYTHON_DOCUMENT_H

#include "strata/json/json_cursor.hpp"
#include "strata/json/json_document.hpp"

#include <Python.h>

// Helper functions to create Python wrappers for C++ types
// These are used by other Python binding modules (mmap, search, etc.)

PyObject* create_py_json_document(strata::JsonDocument&& doc);
PyObject* create_py_json_cursor(strata::JsonCursor&& cursor, PyObject* doc_ref);

// Type checks and extraction for JsonDocument/JsonCursor
bool is_py_json_document(PyObject* obj);
bool is_py_json_cursor(PyObject* obj);
strata::JsonDocument* get_py_json_document(PyObject* obj);
strata::JsonCursor* get_py_json_cursor(PyObject* obj);

#endif // STRATA_PYTHON_DOCUMENT_H

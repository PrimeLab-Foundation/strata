#include "python_document.h"
#include "python_types.h"
#include "strata/json/json_mmap.hpp"

#include <string>
#include <vector>

static void emit_duplicate_key_warnings() {
    auto warnings = strata::consume_parse_warnings();
    for (const auto& msg : warnings) {
        PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
    }
}

//=============================================================================
// parse_json_file Function
//=============================================================================

PyObject* strata_parse_json_file(PyObject* self, PyObject* args) {
    const char* filepath;

    if (!PyArg_ParseTuple(args, "s", &filepath)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Parse file with mmap
    auto result = strata::parse_json_file(filepath);
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Failed to parse JSON file");
        return NULL;
    }

    emit_duplicate_key_warnings();

    // Get the document
    strata::JsonDocument doc = std::move(result.value);

    // Get root cursor BEFORE moving doc
    strata::JsonCursor root_cursor = doc.root();

    // Create Python wrappers using helper functions
    PyObject* py_doc = create_py_json_document(std::move(doc));
    if (!py_doc)
        return NULL;

    PyObject* py_cursor = create_py_json_cursor(std::move(root_cursor), py_doc);
    if (!py_cursor) {
        Py_DECREF(py_doc);
        return NULL;
    }

    // Return tuple (document, cursor)
    PyObject* tuple = PyTuple_Pack(2, py_doc, py_cursor);
    Py_DECREF(py_doc);
    Py_DECREF(py_cursor);

    return tuple;

    STRATA_CPP_CATCH
}

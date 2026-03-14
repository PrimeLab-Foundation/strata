#include "python_types.h"
#include "strata/json/json_parse.hpp"

#include <string>

// Forward declarations
extern PyObject* strata_dumps(PyObject* self, PyObject* obj);
extern PyObject* strata_dumps_bytes(PyObject* self, PyObject* obj);
extern PyObject* strata_loads(PyObject* self, PyObject* args);
extern PyObject* strata_parse_ndjson(PyObject* self, PyObject* args);
extern PyObject* strata_parse_json_file(PyObject* self, PyObject* args);
extern PyObject* strata_compile_path(PyObject* self, PyObject* args);
extern PyObject* strata_search(PyObject* self, PyObject* args);
extern PyObject* strata_set_cycle_policy(PyObject* self, PyObject* args);
extern int register_document_types(PyObject* module);
extern int register_ndjson_types(PyObject* module);
extern int register_jsonpath_types(PyObject* module);

static PyObject* strata_set_duplicate_key_policy(PyObject* self, PyObject* args) {
    const char* policy = nullptr;
    if (!PyArg_ParseTuple(args, "s", &policy)) {
        return NULL;
    }

    std::string p(policy ? policy : "");
    if (p == "first") {
        strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::FirstWins);
    } else if (p == "last") {
        strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::LastWins);
    } else if (p == "error") {
        strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::Error);
    } else if (p == "warn") {
        strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::Warn);
    } else {
        PyErr_SetString(PyExc_ValueError,
                        "unknown duplicate key policy (expected first|last|error|warn)");
        return NULL;
    }

    Py_RETURN_NONE;
}

// Method definitions
static PyMethodDef strata_methods[] = {
    {"dumps", (PyCFunction)strata_dumps, METH_O,
     "dumps(obj) -> str\n\nSerialize Python object to JSON string."},
    {"dumps_bytes", (PyCFunction)strata_dumps_bytes, METH_O,
     "dumps_bytes(obj) -> bytes\n\nSerialize Python object to JSON bytes."},
    {"loads", strata_loads, METH_VARARGS,
     "loads(s) -> object\n\nParse JSON string to Python object."},
    {"parse_ndjson", strata_parse_ndjson, METH_VARARGS,
     "parse_ndjson(s, skip_errors=False) -> list\n\n"
     "Parse all NDJSON lines into a list."},
    {"parse_json_file", strata_parse_json_file, METH_VARARGS,
     "parse_json_file(filepath) -> (JsonDocument, JsonCursor)\n\n"
     "Parse JSON file using memory-mapped I/O."},
    {"compile_path", strata_compile_path, METH_VARARGS,
     "compile_path(path) -> CompiledPath\n\nCompile a JSONPath expression."},
    {"search", strata_search, METH_VARARGS,
     "search(data, path) -> list\n\nSearch JSON data using JSONPath."},
    {"set_duplicate_key_policy", strata_set_duplicate_key_policy, METH_VARARGS,
     "set_duplicate_key_policy(policy)\n\n"
     "Policy: first (default), last, error, warn."},
    {"set_cycle_policy", strata_set_cycle_policy, METH_VARARGS,
     "set_cycle_policy(policy)\n\n"
     "Policy: warn (default), error, ignore."},
    {NULL, NULL, 0, NULL} // Sentinel
};

// Module definition
static struct PyModuleDef strata_module = {
    PyModuleDef_HEAD_INIT,
    "_strata",                     // Module name
    "Fast JSON library with SIMD", // Module docstring
    -1,                            // Module state (-1 = global)
    strata_methods                 // Method table
};

// Module initialization
PyMODINIT_FUNC PyInit__strata(void) {
    PyObject* module = PyModule_Create(&strata_module);
    if (module == NULL) {
        return NULL;
    }

    // Register document types (JsonDocument, JsonCursor)
    if (register_document_types(module) < 0) {
        Py_DECREF(module);
        return NULL;
    }

    // Register NDJSON types (NdjsonStream)
    if (register_ndjson_types(module) < 0) {
        Py_DECREF(module);
        return NULL;
    }

    // Register JSONPath types (CompiledPath)
    if (register_jsonpath_types(module) < 0) {
        Py_DECREF(module);
        return NULL;
    }

    // Add version
    if (PyModule_AddStringConstant(module, "__version__", "0.2.0") < 0) {
        Py_DECREF(module);
        return NULL;
    }

    return module;
}

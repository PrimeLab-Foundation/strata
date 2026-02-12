#include "python_types.h"
#include "strata/json/json_parse.hpp"

#include <string>

// Forward declarations
extern PyObject* strata_dumps(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_dumps_bytes(PyObject* self, PyObject* obj);
extern PyObject* strata_loads(PyObject* self, PyObject* source);
extern PyObject* strata_load(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_dump(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_loads_tape(PyObject* self, PyObject* args);
extern PyObject* strata_parse_json_file(PyObject* self, PyObject* args);
extern PyObject* strata_parse_json(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_compile_path(PyObject* self, PyObject* args);
extern PyObject* strata_search(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_search_ndjson(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_query(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_set_dumps_type_order(PyObject* self, PyObject* args);
extern PyObject* strata_set_cycle_policy(PyObject* self, PyObject* args);
extern int register_document_types(PyObject* module);
extern int register_ndjson_types(PyObject* module);
extern int register_search_types(PyObject* module);
extern int register_lazy_cursor_types(PyObject* module);
extern PyObject* strata_lazy(PyObject* self, PyObject* args);

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
    {"loads", (PyCFunction)strata_loads, METH_O,
     "loads(source) -> object\n\nParse JSON string to Python object."},
    {"dumps", (PyCFunction)strata_dumps, METH_VARARGS | METH_KEYWORDS,
     "dumps(obj, *, return_type='str') -> str|bytes|bytearray\n\n"
     "Serialize Python object to JSON."},
    {"load", (PyCFunction)strata_load, METH_VARARGS | METH_KEYWORDS,
     "load(source, **kwargs) -> object\n\n"
     "Load JSON/NDJSON from a path or file-like object."},
    {"dump", (PyCFunction)strata_dump, METH_VARARGS | METH_KEYWORDS,
     "dump(target, obj, **kwargs) -> int\n\n"
     "Write JSON/NDJSON to a path or file-like object."},
    {"search", (PyCFunction)strata_search, METH_VARARGS | METH_KEYWORDS,
     "search(data, path, *, strata_mode=None, ndjson=None, skip_errors=False, on_error=None,\n"
     "       parallel=None, num_threads=0, limit=0, lazy=False) -> list\n\n"
     "Search JSON/NDJSON data using JSONPath.\n"
     "strata_mode: 'dict', 'string', 'cursor', or 'file' to force input handling.\n"
     "ndjson/skip_errors/on_error control NDJSON handling when auto-detected or forced."},
    {"query", (PyCFunction)strata_query, METH_VARARGS | METH_KEYWORDS,
     "query(data, path) -> list\n\nSearch dict/list data using JSONPath."},
    {"compile_path", strata_compile_path, METH_VARARGS,
     "compile_path(path) -> CompiledPath\n\nCompile a JSONPath expression."},
    {"set_duplicate_key_policy", strata_set_duplicate_key_policy, METH_VARARGS,
     "set_duplicate_key_policy(policy)\n\n"
     "Policy: first (default), last, error, warn."},
    {"set_dumps_type_order", strata_set_dumps_type_order, METH_VARARGS,
     "set_dumps_type_order(policy)\n\n"
     "Policy: strings_first (default), ints_first."},
    {"set_cycle_policy", strata_set_cycle_policy, METH_VARARGS,
     "set_cycle_policy(policy)\n\n"
     "Policy: warn (default), error, ignore."},
    {"loads_tape", strata_loads_tape, METH_VARARGS,
     "loads_tape(s) -> object\n\nParse JSON string via tape format to Python object.\n\n"
     "Uses token tape as intermediate representation. Useful for benchmarking\n"
     "tape-based parsing performance."},
    {"parse_json_file", strata_parse_json_file, METH_VARARGS,
     "parse_json_file(filepath) -> (JsonDocument, JsonCursor)\n\n"
     "Parse JSON file using memory-mapped I/O."},
    {"parse_json", (PyCFunction)strata_parse_json, METH_VARARGS | METH_KEYWORDS,
     "parse_json(text) -> JsonCursor\n\nParse JSON text into a cursor."},
    {"search_ndjson", (PyCFunction)strata_search_ndjson, METH_VARARGS | METH_KEYWORDS,
     "search_ndjson(data, path, *, skip_errors=False, on_error=None) -> list\n\n"
     "Search NDJSON data line-by-line using JSONPath."},
    {"dumps_bytes", (PyCFunction)strata_dumps_bytes, METH_O,
     "dumps_bytes(obj) -> bytes\n\nSerialize Python object to JSON bytes."},
    {"lazy", strata_lazy, METH_VARARGS,
     "lazy(json_input) -> LazyCursor\n\n"
     "Create a lazy cursor for on-demand JSON parsing.\n"
     "Parses only the minimum bytes needed for each operation."},
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
    if (register_search_types(module) < 0) {
        Py_DECREF(module);
        return NULL;
    }

    // Register LazyCursor types
    if (register_lazy_cursor_types(module) < 0) {
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

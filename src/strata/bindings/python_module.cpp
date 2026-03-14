#include "python_convert.h"
#include "python_types.h"
#include "strata/json/json_mmap.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/ndjson_stream.hpp"

#include <cstring>
#include <fstream>
#include <string>
#include <unordered_map>

// Forward declarations
extern PyObject* strata_dumps(PyObject* self, PyObject* obj);
extern PyObject* strata_dumps_bytes(PyObject* self, PyObject* obj);
extern PyObject* strata_loads(PyObject* self, PyObject* args);
extern PyObject* strata_parse_ndjson(PyObject* self, PyObject* args);
extern PyObject* strata_parse_json_file(PyObject* self, PyObject* args);
extern PyObject* strata_compile_path(PyObject* self, PyObject* args);
extern PyObject* strata_search(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_set_cycle_policy(PyObject* self, PyObject* args);
extern bool set_cycle_policy_from_string(const char* policy, std::string& error);
extern int register_document_types(PyObject* module);
extern int register_ndjson_types(PyObject* module);
extern int register_jsonpath_types(PyObject* module);

//=============================================================================
// Config Registry
//=============================================================================

struct ConfigEntry {
    PyObject* value; // owned ref
};

static std::unordered_map<std::string, ConfigEntry>& get_config_map() {
    static std::unordered_map<std::string, ConfigEntry> config_map;
    return config_map;
}

// Internal getter for C++ code — returns borrowed ref or nullptr
PyObject* strata_config_get_internal(const char* key) {
    auto& map = get_config_map();
    auto it = map.find(key);
    if (it != map.end())
        return it->second.value;
    return nullptr;
}

// Internal: check if mem_eff is enabled
bool strata_config_mem_eff() {
    PyObject* val = strata_config_get_internal("mem_eff");
    return val && PyObject_IsTrue(val);
}

static void config_set_internal(const std::string& key, PyObject* value) {
    auto& map = get_config_map();
    auto it = map.find(key);
    if (it != map.end()) {
        Py_DECREF(it->second.value);
    }
    Py_INCREF(value);
    map[key] = {value};

    // Sync with existing policy setters
    if (key == "duplicate_key_policy" && PyUnicode_Check(value)) {
        const char* p = PyUnicode_AsUTF8(value);
        if (p) {
            std::string ps(p);
            if (ps == "first")
                strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::FirstWins);
            else if (ps == "last")
                strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::LastWins);
            else if (ps == "error")
                strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::Error);
            else if (ps == "warn")
                strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::Warn);
        }
    } else if (key == "cycle_policy" && PyUnicode_Check(value)) {
        const char* p = PyUnicode_AsUTF8(value);
        if (p) {
            std::string error;
            set_cycle_policy_from_string(p, error);
        }
    }
}

static PyObject* strata_config_set(PyObject* self, PyObject* args) {
    const char* key;
    PyObject* value;
    if (!PyArg_ParseTuple(args, "sO", &key, &value))
        return NULL;

    std::string k(key);

    // Validate known keys
    if (k == "mem_eff") {
        if (!PyBool_Check(value)) {
            PyErr_SetString(PyExc_TypeError, "mem_eff must be a bool");
            return NULL;
        }
    } else if (k == "duplicate_key_policy") {
        if (!PyUnicode_Check(value)) {
            PyErr_SetString(PyExc_TypeError, "duplicate_key_policy must be a string");
            return NULL;
        }
        const char* p = PyUnicode_AsUTF8(value);
        if (!p || (strcmp(p, "first") != 0 && strcmp(p, "last") != 0 && strcmp(p, "error") != 0 &&
                   strcmp(p, "warn") != 0)) {
            PyErr_SetString(PyExc_ValueError, "duplicate_key_policy must be first|last|error|warn");
            return NULL;
        }
    } else if (k == "cycle_policy") {
        if (!PyUnicode_Check(value)) {
            PyErr_SetString(PyExc_TypeError, "cycle_policy must be a string");
            return NULL;
        }
        const char* p = PyUnicode_AsUTF8(value);
        if (!p || (strcmp(p, "warn") != 0 && strcmp(p, "error") != 0 && strcmp(p, "ignore") != 0)) {
            PyErr_SetString(PyExc_ValueError, "cycle_policy must be warn|error|ignore");
            return NULL;
        }
    } else {
        PyErr_Format(PyExc_KeyError, "unknown config key: %s", key);
        return NULL;
    }

    config_set_internal(k, value);
    Py_RETURN_NONE;
}

static PyObject* strata_config_get(PyObject* self, PyObject* args) {
    const char* key;
    if (!PyArg_ParseTuple(args, "s", &key))
        return NULL;

    PyObject* val = strata_config_get_internal(key);
    if (!val) {
        Py_RETURN_NONE;
    }
    Py_INCREF(val);
    return val;
}

static PyObject* strata_config_list(PyObject* self, PyObject* args) {
    auto& map = get_config_map();
    PyObject* dict = PyDict_New();
    if (!dict)
        return NULL;
    for (const auto& [k, entry] : map) {
        PyObject* key = PyUnicode_FromStringAndSize(k.data(), k.size());
        if (!key) {
            Py_DECREF(dict);
            return NULL;
        }
        if (PyDict_SetItem(dict, key, entry.value) < 0) {
            Py_DECREF(key);
            Py_DECREF(dict);
            return NULL;
        }
        Py_DECREF(key);
    }
    return dict;
}

//=============================================================================
// Legacy policy setters (delegate to config)
//=============================================================================

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

    // Sync config map
    PyObject* val = PyUnicode_FromString(policy);
    if (val) {
        auto& map = get_config_map();
        auto it = map.find("duplicate_key_policy");
        if (it != map.end())
            Py_DECREF(it->second.value);
        map["duplicate_key_policy"] = {val};
    }

    Py_RETURN_NONE;
}

//=============================================================================
// load() and dump() — file-based I/O
//=============================================================================

static PyObject* strata_load(PyObject* self, PyObject* args) {
    const char* filepath;
    if (!PyArg_ParseTuple(args, "s", &filepath))
        return NULL;

    STRATA_CPP_TRY

    size_t len = strlen(filepath);

    // Check for NDJSON/JSONL
    bool is_ndjson = (len > 7 && strcmp(filepath + len - 7, ".ndjson") == 0) ||
                     (len > 6 && strcmp(filepath + len - 6, ".jsonl") == 0);

    if (is_ndjson) {
        // Read file contents
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            PyErr_Format(PyExc_FileNotFoundError, "Cannot open file: %s", filepath);
            return NULL;
        }
        auto size = file.tellg();
        file.seekg(0);
        std::string content(static_cast<size_t>(size), '\0');
        file.read(content.data(), size);
        file.close();

        strata::NdjsonStream stream(content);
        return parse_ndjson_all_to_python(stream, 0);
    }

    // JSON: use mmap
    auto result = strata::parse_json_file(filepath);
    if (!result.ok()) {
        PyErr_Format(PyExc_ValueError, "Failed to parse JSON file: %s", filepath);
        return NULL;
    }

    auto warnings = strata::consume_parse_warnings();
    for (const auto& msg : warnings) {
        PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
    }

    return json_value_to_python(result.value.root_value());

    STRATA_CPP_CATCH
}

extern PyObject* strata_dumps(PyObject* self, PyObject* obj);

static PyObject* strata_dump(PyObject* self, PyObject* args) {
    PyObject* obj;
    const char* filepath;
    if (!PyArg_ParseTuple(args, "Os", &obj, &filepath))
        return NULL;

    STRATA_CPP_TRY

    // Serialize to string
    PyObject* json_str = strata_dumps(self, obj);
    if (!json_str)
        return NULL;

    Py_ssize_t len;
    const char* data = PyUnicode_AsUTF8AndSize(json_str, &len);
    if (!data) {
        Py_DECREF(json_str);
        return NULL;
    }

    // Write to file
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        Py_DECREF(json_str);
        PyErr_Format(PyExc_IOError, "Cannot open file for writing: %s", filepath);
        return NULL;
    }
    file.write(data, len);
    file.write("\n", 1);
    file.close();

    Py_DECREF(json_str);
    Py_RETURN_NONE;

    STRATA_CPP_CATCH
}

// Method definitions
static PyMethodDef strata_methods[] = {
    {"dumps", (PyCFunction)strata_dumps, METH_O,
     "dumps(obj) -> str\n\nSerialize Python object to JSON string."},
    {"dumps_bytes", (PyCFunction)strata_dumps_bytes, METH_O,
     "dumps_bytes(obj) -> bytes\n\nSerialize Python object to JSON bytes."},
    {"loads", strata_loads, METH_VARARGS,
     "loads(s) -> object\n\nParse JSON string to Python object."},
    {"load", strata_load, METH_VARARGS,
     "load(filepath) -> object\n\nLoad JSON/NDJSON/JSONL file to Python object."},
    {"dump", strata_dump, METH_VARARGS,
     "dump(obj, filepath)\n\nSerialize Python object to JSON file."},
    {"parse_ndjson", strata_parse_ndjson, METH_VARARGS,
     "parse_ndjson(s, skip_errors=False) -> list\n\n"
     "Parse all NDJSON lines into a list."},
    {"parse_json_file", strata_parse_json_file, METH_VARARGS,
     "parse_json_file(filepath) -> (JsonDocument, JsonCursor)\n\n"
     "Parse JSON file using memory-mapped I/O."},
    {"compile_path", strata_compile_path, METH_VARARGS,
     "compile_path(path) -> CompiledPath\n\nCompile a JSONPath expression."},
    {"search", (PyCFunction)strata_search, METH_VARARGS | METH_KEYWORDS,
     "search(data, path, *, mem_eff=None) -> list\n\nSearch JSON data using JSONPath."},
    {"set_duplicate_key_policy", strata_set_duplicate_key_policy, METH_VARARGS,
     "set_duplicate_key_policy(policy)\n\n"
     "Policy: first (default), last, error, warn."},
    {"set_cycle_policy", strata_set_cycle_policy, METH_VARARGS,
     "set_cycle_policy(policy)\n\n"
     "Policy: warn (default), error, ignore."},
    {"config_set", strata_config_set, METH_VARARGS,
     "config_set(key, value)\n\nSet a config value."},
    {"config_get", strata_config_get, METH_VARARGS,
     "config_get(key) -> value\n\nGet a config value."},
    {"config_list", strata_config_list, METH_NOARGS,
     "config_list() -> dict\n\nList all config keys and values."},
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

    // Initialize config defaults
    {
        auto& map = get_config_map();
        if (map.empty()) {
            Py_INCREF(Py_False);
            map["mem_eff"] = {Py_False};
            PyObject* dup = PyUnicode_FromString("first");
            if (dup)
                map["duplicate_key_policy"] = {dup};
            PyObject* cyc = PyUnicode_FromString("warn");
            if (cyc)
                map["cycle_policy"] = {cyc};
        }
    }

    return module;
}

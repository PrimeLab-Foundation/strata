#include "python_convert.h"
#include "python_types.h"
#include "strata/json/json_mmap.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/util/output_buffer.hpp"

#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>

// POSIX I/O for fast file read/write (avoids iostream overhead)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// Forward declarations
extern PyObject* strata_dumps(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_dumps_internal(PyObject* obj);
extern bool strata_serialize_to_buffer(PyObject* obj, const char** out_data, size_t* out_size);
// Thread-local serialize buffer from python_dumps.cpp — used by dump() to append newline
extern thread_local strata::util::OutputBuffer g_serialize_buffer;
extern PyObject* strata_loads(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_parse_json_file(PyObject* self, PyObject* args);
extern PyObject* strata_compile_path(PyObject* self, PyObject* args);
extern PyObject* strata_search(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* strata_query(PyObject* self, PyObject* args, PyObject* kwargs);
extern bool set_cycle_policy_from_string(const char* policy, std::string& error);
extern int register_document_types(PyObject* module);
extern int register_ndjson_types(PyObject* module);
extern int register_jsonpath_types(PyObject* module);
extern int register_iterator_type(PyObject* module);

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
    if (k == "duplicate_key_policy") {
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
// load() and dump() — file-based I/O
//=============================================================================

// Forward declarations for iterator support
extern PyObject* create_list_iterator(PyObject* list);
extern PyObject* create_dict_iterator(PyObject* dict);
extern PyObject* create_ndjson_file_iterator(const char* filepath);

static PyObject* strata_load(PyObject* self, PyObject* args, PyObject* kwargs) {
    const char* filepath;
    const char* return_type = "dict";
    int iterator = 0;

    static const char* kwlist[] = {"filepath", "return_type", "iterator", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|sp", const_cast<char**>(kwlist), &filepath,
                                     &return_type, &iterator))
        return NULL;

    STRATA_CPP_TRY

    size_t len = strlen(filepath);

    // Check for NDJSON/JSONL
    bool is_ndjson = (len > 7 && strcmp(filepath + len - 7, ".ndjson") == 0) ||
                     (len > 6 && strcmp(filepath + len - 6, ".jsonl") == 0);

    // Validate return_type
    bool as_cursor = false;
    if (strcmp(return_type, "dict") == 0) {
        as_cursor = false;
    } else if (strcmp(return_type, "cursor") == 0) {
        as_cursor = true;
    } else {
        PyErr_Format(PyExc_ValueError, "return_type must be 'dict' or 'cursor', got '%s'",
                     return_type);
        return NULL;
    }

    if (is_ndjson) {
        if (as_cursor) {
            PyErr_SetString(PyExc_ValueError,
                            "return_type='cursor' not supported for NDJSON files");
            return NULL;
        }

        if (iterator) {
            return create_ndjson_file_iterator(filepath);
        }

        // Read file via POSIX read (avoids ifstream and mmap overhead)
        int fd = open(filepath, O_RDONLY);
        if (fd < 0) {
            PyErr_Format(PyExc_FileNotFoundError, "Cannot open file: %s", filepath);
            return NULL;
        }
        struct stat sb;
        if (fstat(fd, &sb) < 0) {
            close(fd);
            PyErr_Format(PyExc_IOError, "Cannot stat file: %s", filepath);
            return NULL;
        }
        size_t file_size = static_cast<size_t>(sb.st_size);
        if (file_size == 0) {
            close(fd);
            return PyList_New(0);
        }

        static thread_local char* ndjson_buf = nullptr;
        static thread_local size_t ndjson_buf_cap = 0;
        if (ndjson_buf_cap < file_size) {
            char* new_buf = static_cast<char*>(std::realloc(ndjson_buf, file_size));
            if (!new_buf) {
                close(fd);
                return PyErr_NoMemory();
            }
            ndjson_buf = new_buf;
            ndjson_buf_cap = file_size;
        }
        ssize_t bytes_read = read(fd, ndjson_buf, file_size);
        close(fd);
        if (bytes_read < 0 || static_cast<size_t>(bytes_read) != file_size) {
            PyErr_Format(PyExc_IOError, "Failed to read file: %s", filepath);
            return NULL;
        }

        return parse_ndjson_direct(ndjson_buf, file_size, 0);
    }

    // JSON file
    if (as_cursor) {
        // return_type='cursor': use mmap, return (doc, cursor) tuple
        return strata_parse_json_file(self, args);
    }

    // JSON dict: POSIX read + fast SAX-based parse (single pass, no C++ DOM)
    {
        int fd = open(filepath, O_RDONLY);
        if (fd < 0) {
            PyErr_Format(PyExc_FileNotFoundError, "Cannot open file: %s", filepath);
            return NULL;
        }
        struct stat sb;
        if (fstat(fd, &sb) < 0) {
            close(fd);
            PyErr_Format(PyExc_IOError, "Cannot stat file: %s", filepath);
            return NULL;
        }
        size_t file_size = static_cast<size_t>(sb.st_size);
        if (file_size == 0) {
            close(fd);
            PyErr_SetString(PyExc_ValueError, "Empty file");
            return NULL;
        }

        // Thread-local raw buffer avoids per-call alloc and zero-initialization
        static thread_local char* file_buf = nullptr;
        static thread_local size_t file_buf_cap = 0;
        if (file_buf_cap < file_size) {
            char* new_buf = static_cast<char*>(std::realloc(file_buf, file_size));
            if (!new_buf) {
                close(fd);
                return PyErr_NoMemory();
            }
            file_buf = new_buf;
            file_buf_cap = file_size;
        }
        ssize_t bytes_read = read(fd, file_buf, file_size);
        close(fd);
        if (bytes_read < 0 || static_cast<size_t>(bytes_read) != file_size) {
            PyErr_Format(PyExc_IOError, "Failed to read file: %s", filepath);
            return NULL;
        }

        PyObject* py_result;
        {
            PyGcPause gc_pause;
            py_result = parse_json_to_python(std::string_view(file_buf, file_size),
                                             /*validate_utf8=*/false);
        }

        if (!py_result)
            return NULL;

        if (iterator) {
            if (PyDict_Check(py_result)) {
                PyObject* it = create_dict_iterator(py_result);
                Py_DECREF(py_result);
                return it;
            } else if (PyList_Check(py_result)) {
                PyObject* it = create_list_iterator(py_result);
                Py_DECREF(py_result);
                return it;
            }
        }

        return py_result;
    }

    STRATA_CPP_CATCH
}

static PyObject* strata_dump(PyObject* self, PyObject* args) {
    PyObject* obj;
    const char* filepath;
    if (!PyArg_ParseTuple(args, "Os", &obj, &filepath))
        return NULL;

    STRATA_CPP_TRY

    // Serialize directly to buffer (no PyUnicode intermediate)
    const char* data;
    size_t len;
    if (!strata_serialize_to_buffer(obj, &data, &len))
        return NULL;

    // Write buffer to file using POSIX I/O (avoids iostream overhead)
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        PyErr_Format(PyExc_IOError, "Cannot open file for writing: %s", filepath);
        return NULL;
    }

    // Single write call for data + newline by appending newline to buffer
    // (buffer is thread-local and will be cleared on next use anyway)
    g_serialize_buffer.push_back('\n');

    const char* buf = g_serialize_buffer.data();
    size_t total = g_serialize_buffer.size();
    while (total > 0) {
        ssize_t written = write(fd, buf, total);
        if (written < 0) {
            close(fd);
            PyErr_Format(PyExc_IOError, "Failed to write to file: %s", filepath);
            return NULL;
        }
        buf += written;
        total -= static_cast<size_t>(written);
    }
    close(fd);

    Py_RETURN_NONE;

    STRATA_CPP_CATCH
}

// Method definitions
static PyMethodDef strata_methods[] = {
    {"dumps", (PyCFunction)strata_dumps, METH_VARARGS | METH_KEYWORDS,
     "dumps(obj, *, return_type='str') -> str|bytes\n\n"
     "Serialize Python object to JSON string or bytes."},
    {"loads", (PyCFunction)strata_loads, METH_VARARGS | METH_KEYWORDS,
     "loads(source, *, return_type='dict', iterator=False) -> object\n\n"
     "Parse JSON string to Python object."},
    {"load", (PyCFunction)strata_load, METH_VARARGS | METH_KEYWORDS,
     "load(filepath, *, return_type='dict', iterator=False) -> object\n\n"
     "Load JSON/NDJSON/JSONL file."},
    {"dump", strata_dump, METH_VARARGS,
     "dump(obj, filepath)\n\nSerialize Python object to JSON file."},
    {"compile_path", strata_compile_path, METH_VARARGS,
     "compile_path(path) -> CompiledPath\n\nCompile a JSONPath expression."},
    {"search", (PyCFunction)strata_search, METH_VARARGS | METH_KEYWORDS,
     "search(filepath, path, *, iterator=False) -> list\n\n"
     "Search JSON file using JSONPath."},
    {"query", (PyCFunction)strata_query, METH_VARARGS | METH_KEYWORDS,
     "query(data, path, *, iterator=False) -> list\n\n"
     "Query dict/list using JSONPath."},
    {"parse_ndjson", strata_parse_ndjson, METH_VARARGS,
     "parse_ndjson(text, skip_errors=False) -> list\n\n"
     "Parse NDJSON/JSONL text into a list of objects in one C++ call."},
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

    // Register iterator type
    if (register_iterator_type(module) < 0) {
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

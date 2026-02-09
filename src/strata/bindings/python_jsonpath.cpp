#include "python_convert.h"
#include "python_document.h"
#include "python_ndjson.h"
#include "python_types.h"
#include "strata/json/json_parse.hpp"
#include "strata/search/jsonpath.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <vector>

extern PyObject* strata_dumps_bytes(PyObject* self, PyObject* obj);

static void emit_duplicate_key_warnings() {
    auto warnings = strata::consume_parse_warnings();
    for (const auto& msg : warnings) {
        PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
    }
}

namespace {

enum class NdjsonErrorMode {
    Skip,
    Warn,
    Error,
};

enum class StrataSearchMode {
    Auto,
    Dict,
    String,
    Cursor,
};

bool parse_strata_mode(PyObject* mode_obj, StrataSearchMode* mode) {
    if (!mode) {
        PyErr_SetString(PyExc_RuntimeError, "invalid strata_mode output");
        return false;
    }
    if (mode_obj == Py_None) {
        *mode = StrataSearchMode::Auto;
        return true;
    }
    if (!PyUnicode_Check(mode_obj)) {
        PyErr_SetString(PyExc_TypeError, "strata_mode must be 'dict', 'string', or 'cursor'");
        return false;
    }
    const char* raw = PyUnicode_AsUTF8(mode_obj);
    if (!raw) {
        return false;
    }
    std::string value(raw);
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (value == "dict") {
        *mode = StrataSearchMode::Dict;
        return true;
    }
    if (value == "string") {
        *mode = StrataSearchMode::String;
        return true;
    }
    if (value == "cursor") {
        *mode = StrataSearchMode::Cursor;
        return true;
    }
    PyErr_SetString(PyExc_ValueError, "strata_mode must be 'dict', 'string', or 'cursor'");
    return false;
}

bool parse_ndjson_error_mode(int skip_errors, PyObject* on_error_obj, NdjsonErrorMode* mode) {
    if (!mode) {
        PyErr_SetString(PyExc_RuntimeError, "invalid error mode output");
        return false;
    }
    if (on_error_obj == Py_None) {
        *mode = skip_errors ? NdjsonErrorMode::Skip : NdjsonErrorMode::Error;
        return true;
    }
    if (!PyUnicode_Check(on_error_obj)) {
        PyErr_SetString(PyExc_TypeError, "on_error must be 'skip', 'warn', or 'error'");
        return false;
    }
    const char* raw = PyUnicode_AsUTF8(on_error_obj);
    if (!raw) {
        return false;
    }
    std::string value(raw);
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (value == "skip") {
        *mode = NdjsonErrorMode::Skip;
        return true;
    }
    if (value == "warn") {
        *mode = NdjsonErrorMode::Warn;
        return true;
    }
    if (value == "error") {
        *mode = NdjsonErrorMode::Error;
        return true;
    }
    PyErr_SetString(PyExc_ValueError, "on_error must be 'skip', 'warn', or 'error'");
    return false;
}

inline bool is_json_whitespace(unsigned char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

bool is_whitespace_only(std::string_view line) {
    for (unsigned char c : line) {
        if (!is_json_whitespace(c)) {
            return false;
        }
    }
    return true;
}

std::string_view trim_line_endings(std::string_view line) {
    if (!line.empty() && line.back() == '\n') {
        line.remove_suffix(1);
    }
    if (!line.empty() && line.back() == '\r') {
        line.remove_suffix(1);
    }
    return line;
}

bool append_ndjson_match(PyObject* results, size_t line_no,
                         const std::vector<strata::JsonValue>& matches) {
    PyObject* matches_list = json_value_list_to_python(matches);
    if (!matches_list) {
        return false;
    }
    PyObject* entry = PyDict_New();
    if (!entry) {
        Py_DECREF(matches_list);
        return false;
    }
    PyObject* line_obj = PyLong_FromSize_t(line_no);
    if (!line_obj) {
        Py_DECREF(matches_list);
        Py_DECREF(entry);
        return false;
    }
    if (PyDict_SetItemString(entry, "line", line_obj) < 0 ||
        PyDict_SetItemString(entry, "matches", matches_list) < 0) {
        Py_DECREF(line_obj);
        Py_DECREF(matches_list);
        Py_DECREF(entry);
        return false;
    }
    Py_DECREF(line_obj);
    Py_DECREF(matches_list);
    if (PyList_Append(results, entry) < 0) {
        Py_DECREF(entry);
        return false;
    }
    Py_DECREF(entry);
    return true;
}

PyObject* search_from_json_buffer(const char* data, Py_ssize_t len,
                                  const strata::CompiledPath& compiled_path) {
    auto parse_result = strata::parse_json(std::string_view(data, static_cast<size_t>(len)));
    if (!parse_result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        return NULL;
    }

    emit_duplicate_key_warnings();

    strata::JsonCursor cursor(&parse_result.value);
    auto result_values = strata::eval_jsonpath(cursor, compiled_path);
    return json_value_list_to_python(result_values);
}

PyObject* search_cursor_mode(PyObject* data_obj, const strata::CompiledPath& compiled_path) {
    if (is_py_json_document(data_obj)) {
        auto* doc = get_py_json_document(data_obj);
        if (!doc) {
            PyErr_SetString(PyExc_TypeError, "Invalid JsonDocument");
            return NULL;
        }

        strata::JsonCursor cursor(doc->root());
        auto result_values = strata::eval_jsonpath(cursor, compiled_path);
        return json_value_list_to_python(result_values);
    }

    if (is_py_json_cursor(data_obj)) {
        auto* cursor_ptr = get_py_json_cursor(data_obj);
        if (!cursor_ptr) {
            PyErr_SetString(PyExc_TypeError, "Invalid JsonCursor");
            return NULL;
        }

        auto result_values = strata::eval_jsonpath(*cursor_ptr, compiled_path);
        return json_value_list_to_python(result_values);
    }

    if (is_py_ndjson_cursor(data_obj)) {
        auto* cursor_data = get_py_ndjson_cursor(data_obj);
        if (!cursor_data) {
            PyErr_SetString(PyExc_TypeError, "Invalid NdjsonCursor");
            return NULL;
        }
        const auto& values = cursor_data->values;
        const auto& line_numbers = cursor_data->line_numbers;
        if (values.size() != line_numbers.size()) {
            PyErr_SetString(PyExc_RuntimeError, "NdjsonCursor data is inconsistent");
            return NULL;
        }

        PyObject* results = PyList_New(0);
        if (!results) {
            return NULL;
        }

        for (size_t i = 0; i < values.size(); ++i) {
            strata::JsonCursor cursor(&values[i]);
            auto matches = strata::eval_jsonpath(cursor, compiled_path);
            if (matches.empty()) {
                continue;
            }
            if (!append_ndjson_match(results, line_numbers[i], matches)) {
                Py_DECREF(results);
                return NULL;
            }
        }
        return results;
    }

    PyErr_SetString(PyExc_TypeError, "cursor mode expects JsonDocument, JsonCursor, or NdjsonCursor");
    return NULL;
}

bool process_ndjson_line(std::string_view line, size_t line_no,
                         const strata::CompiledPath& compiled_path, NdjsonErrorMode mode,
                         PyObject* results, strata::ParseSaxOptions& options,
                         strata::ParseSaxContext& parse_context) {
    if (line.empty() || is_whitespace_only(line)) {
        return true;
    }
    auto parse_result = strata::parse_json(line, options, &parse_context);
    if (!parse_result.ok()) {
        if (mode == NdjsonErrorMode::Skip) {
            return true;
        }
        std::string message = "Invalid JSON on line " + std::to_string(line_no);
        if (mode == NdjsonErrorMode::Warn) {
            if (PyErr_WarnEx(PyExc_RuntimeWarning, message.c_str(), 1) < 0) {
                return false;
            }
            return true;
        }
        PyErr_SetString(PyExc_ValueError, message.c_str());
        return false;
    }

    emit_duplicate_key_warnings();

    strata::JsonCursor cursor(&parse_result.value);
    auto matches = strata::eval_jsonpath(cursor, compiled_path);
    if (matches.empty()) {
        return true;
    }
    return append_ndjson_match(results, line_no, matches);
}

} // namespace

//=============================================================================
// CompiledPath Type
//=============================================================================

typedef struct {
    PyObject_HEAD strata::CompiledPath* path;
} PyCompiledPath;

static void PyCompiledPath_dealloc(PyCompiledPath* self) {
    delete self->path;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyCompiledPath_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyCompiledPath* self = (PyCompiledPath*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->path = nullptr;
    }
    return (PyObject*)self;
}

// Forward declarations
static PyObject* PyCompiledPath_from_string(PyObject* cls, PyObject* args);
static PyObject* PyCompiledPath_execute(PyCompiledPath* self, PyObject* args);

// Method table
static PyMethodDef PyCompiledPath_methods[] = {
    {"from_string", (PyCFunction)PyCompiledPath_from_string, METH_VARARGS | METH_CLASS,
     "Create CompiledPath from JSONPath string"},
    {"execute", (PyCFunction)PyCompiledPath_execute, METH_VARARGS, "Execute path on cursor"},
    {NULL, NULL, 0, NULL}};

// Type object
static PyTypeObject PyCompiledPathType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.CompiledPath",
    .tp_basicsize = sizeof(PyCompiledPath),
    .tp_dealloc = (destructor)PyCompiledPath_dealloc,
    .tp_new = PyCompiledPath_new,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Compiled JSONPath Query",
    .tp_methods = PyCompiledPath_methods,
};

//=============================================================================
// Method Implementations
//=============================================================================

static PyObject* PyCompiledPath_from_string(PyObject* cls, PyObject* args) {
    const char* path_str;

    if (!PyArg_ParseTuple(args, "s", &path_str)) {
        return NULL;
    }

    STRATA_CPP_TRY

    auto result = strata::compile_jsonpath(path_str);
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
        return NULL;
    }

    PyCompiledPath* self = (PyCompiledPath*)PyType_GenericAlloc(&PyCompiledPathType, 0);
    if (!self)
        return NULL;

    self->path = new strata::CompiledPath(std::move(result.value));

    return (PyObject*)self;

    STRATA_CPP_CATCH
}

static PyObject* PyCompiledPath_execute(PyCompiledPath* self, PyObject* args) {
    PyObject* cursor_obj;

    if (!PyArg_ParseTuple(args, "O", &cursor_obj)) {
        return NULL;
    }

    // Accept either C extension JsonCursor or Python JsonCursor wrapper (has _cursor)
    PyObject* cursor_to_use = cursor_obj;
    PyObject* borrowed = NULL;
    if (!is_py_json_cursor(cursor_obj) && PyObject_HasAttrString(cursor_obj, "_cursor")) {
        borrowed = PyObject_GetAttrString(cursor_obj, "_cursor");
        if (borrowed && is_py_json_cursor(borrowed)) {
            cursor_to_use = borrowed;
        } else if (borrowed) {
            Py_DECREF(borrowed);
            borrowed = NULL;
        }
    }

    if (!is_py_json_cursor(cursor_to_use)) {
        Py_XDECREF(borrowed);
        PyErr_SetString(PyExc_TypeError, "Expected JsonCursor object");
        return NULL;
    }

    strata::JsonCursor* cursor_ptr = get_py_json_cursor(cursor_to_use);
    Py_XDECREF(borrowed);
    borrowed = NULL;
    if (!cursor_ptr) {
        PyErr_SetString(PyExc_TypeError, "Invalid JsonCursor");
        return NULL;
    }

    STRATA_CPP_TRY

    auto result_values = strata::eval_jsonpath(*cursor_ptr, *self->path);
    return json_value_list_to_python(result_values);

    STRATA_CPP_CATCH
}

//=============================================================================
// search() and compile_path() Functions
//=============================================================================

PyObject* strata_compile_path(PyObject* self, PyObject* args) {
    const char* path_str;

    if (!PyArg_ParseTuple(args, "s", &path_str)) {
        return NULL;
    }

    STRATA_CPP_TRY

    auto result = strata::compile_jsonpath(path_str);
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
        return NULL;
    }

    PyCompiledPath* self_obj = (PyCompiledPath*)PyType_GenericAlloc(&PyCompiledPathType, 0);
    if (!self_obj)
        return NULL;

    self_obj->path = new strata::CompiledPath(std::move(result.value));

    return (PyObject*)self_obj;

    STRATA_CPP_CATCH
}

PyObject* strata_search(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* data_obj;
    PyObject* path_obj;
    PyObject* mode_obj = Py_None;

    static const char* kwlist[] = {"data", "path", "strata_mode", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|O", const_cast<char**>(kwlist), &data_obj,
                                     &path_obj, &mode_obj)) {
        return NULL;
    }

    STRATA_CPP_TRY

    StrataSearchMode mode;
    if (!parse_strata_mode(mode_obj, &mode)) {
        return NULL;
    }

    // Compile the path (if it's a string)
    strata::CompiledPath compiled_path;

    if (PyUnicode_Check(path_obj)) {
        // String path - compile it
        const char* path_str = PyUnicode_AsUTF8(path_obj);
        if (!path_str)
            return NULL;

        auto compile_result = strata::compile_jsonpath(path_str);
        if (!compile_result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
            return NULL;
        }
        compiled_path = std::move(compile_result.value);
    } else if (Py_TYPE(path_obj) == &PyCompiledPathType) {
        // Pre-compiled path - use it directly
        PyCompiledPath* compiled_obj = (PyCompiledPath*)path_obj;
        compiled_path = *compiled_obj->path;
    } else {
        PyErr_SetString(PyExc_TypeError, "path must be a string or CompiledPath");
        return NULL;
    }

    if (mode == StrataSearchMode::Dict) {
        PyObject* json_bytes = strata_dumps_bytes(nullptr, data_obj);
        if (!json_bytes) {
            return NULL;
        }
        char* json_data = nullptr;
        Py_ssize_t json_len = 0;
        if (PyBytes_AsStringAndSize(json_bytes, &json_data, &json_len) < 0) {
            Py_DECREF(json_bytes);
            return NULL;
        }
        PyObject* result = search_from_json_buffer(json_data, json_len, compiled_path);
        Py_DECREF(json_bytes);
        return result;
    }

    if (mode == StrataSearchMode::String) {
        if (PyUnicode_Check(data_obj)) {
            Py_ssize_t json_len;
            const char* json_data = PyUnicode_AsUTF8AndSize(data_obj, &json_len);
            if (!json_data) {
                return NULL;
            }
            return search_from_json_buffer(json_data, json_len, compiled_path);
        }
        if (PyBytes_Check(data_obj)) {
            char* json_data = nullptr;
            Py_ssize_t json_len = 0;
            if (PyBytes_AsStringAndSize(data_obj, &json_data, &json_len) < 0) {
                return NULL;
            }
            return search_from_json_buffer(json_data, json_len, compiled_path);
        }
        PyErr_SetString(PyExc_TypeError, "string mode expects JSON text (str or bytes)");
        return NULL;
    }

    if (mode == StrataSearchMode::Cursor) {
        return search_cursor_mode(data_obj, compiled_path);
    }

    // Handle JsonDocument/JsonCursor inputs (parsed once, no reparse)
    if (is_py_json_document(data_obj)) {
        return search_cursor_mode(data_obj, compiled_path);
    }

    if (is_py_json_cursor(data_obj)) {
        return search_cursor_mode(data_obj, compiled_path);
    }

    if (is_py_ndjson_cursor(data_obj)) {
        return search_cursor_mode(data_obj, compiled_path);
    }

    // Handle string input (JSON text)
    if (PyUnicode_Check(data_obj)) {
        Py_ssize_t json_len;
        const char* json_data = PyUnicode_AsUTF8AndSize(data_obj, &json_len);
        if (!json_data) {
            return NULL;
        }
        return search_from_json_buffer(json_data, json_len, compiled_path);
    }

    if (PyBytes_Check(data_obj)) {
        char* json_data = nullptr;
        Py_ssize_t json_len = 0;
        if (PyBytes_AsStringAndSize(data_obj, &json_data, &json_len) < 0) {
            return NULL;
        }
        return search_from_json_buffer(json_data, json_len, compiled_path);
    }

    PyErr_SetString(PyExc_TypeError,
                    "search() expects JSON text (str/bytes), dict mode data, or a "
                    "JsonDocument/JsonCursor/NdjsonCursor");
    return NULL;

    STRATA_CPP_CATCH
}

PyObject* strata_search_ndjson(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* data_obj;
    PyObject* path_obj;
    int skip_errors = 0;
    PyObject* on_error_obj = Py_None;

    static const char* kwlist[] = {"data", "path", "skip_errors", "on_error", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|pO", const_cast<char**>(kwlist), &data_obj,
                                     &path_obj, &skip_errors, &on_error_obj)) {
        return NULL;
    }

    STRATA_CPP_TRY

    NdjsonErrorMode mode;
    if (!parse_ndjson_error_mode(skip_errors, on_error_obj, &mode)) {
        return NULL;
    }

    strata::CompiledPath compiled_path;
    if (PyUnicode_Check(path_obj)) {
        const char* path_str = PyUnicode_AsUTF8(path_obj);
        if (!path_str) {
            return NULL;
        }
        auto compile_result = strata::compile_jsonpath(path_str);
        if (!compile_result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
            return NULL;
        }
        compiled_path = std::move(compile_result.value);
    } else if (Py_TYPE(path_obj) == &PyCompiledPathType) {
        PyCompiledPath* compiled_obj = (PyCompiledPath*)path_obj;
        compiled_path = *compiled_obj->path;
    } else {
        PyErr_SetString(PyExc_TypeError, "path must be a string or CompiledPath");
        return NULL;
    }

    PyObject* results = PyList_New(0);
    if (!results) {
        return NULL;
    }

    strata::ParseSaxOptions options;
    strata::ParseSaxContext parse_context;

    PyObject* pathlike = PyOS_FSPath(data_obj);
    if (pathlike) {
        const char* filepath = nullptr;
        if (PyUnicode_Check(pathlike)) {
            filepath = PyUnicode_AsUTF8(pathlike);
        } else if (PyBytes_Check(pathlike)) {
            filepath = PyBytes_AsString(pathlike);
        } else {
            PyErr_SetString(PyExc_TypeError, "path must be str or bytes");
        }
        Py_DECREF(pathlike);
        if (!filepath) {
            Py_DECREF(results);
            return NULL;
        }

        std::ifstream file(filepath, std::ios::binary);
        if (!file) {
            Py_DECREF(results);
            PyErr_SetString(PyExc_OSError, "Failed to open NDJSON file");
            return NULL;
        }

        std::string line;
        size_t line_no = 0;
        while (std::getline(file, line)) {
            line_no++;
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            std::string_view view(line);
            if (!process_ndjson_line(view, line_no, compiled_path, mode, results, options,
                                     parse_context)) {
                Py_DECREF(results);
                return NULL;
            }
        }
        if (file.bad()) {
            Py_DECREF(results);
            PyErr_SetString(PyExc_OSError, "Error reading NDJSON file");
            return NULL;
        }
        return results;
    }
    PyErr_Clear();

    PyObject* iter = PyObject_GetIter(data_obj);
    if (iter) {
        size_t line_no = 0;
        PyObject* line_obj;
        while ((line_obj = PyIter_Next(iter))) {
            line_no++;
            std::string_view view;
            if (PyBytes_Check(line_obj)) {
                char* data = nullptr;
                Py_ssize_t len = 0;
                if (PyBytes_AsStringAndSize(line_obj, &data, &len) < 0) {
                    Py_DECREF(line_obj);
                    Py_DECREF(iter);
                    Py_DECREF(results);
                    return NULL;
                }
                view = std::string_view(data, static_cast<size_t>(len));
            } else if (PyUnicode_Check(line_obj)) {
                Py_ssize_t len = 0;
                const char* data = PyUnicode_AsUTF8AndSize(line_obj, &len);
                if (!data) {
                    Py_DECREF(line_obj);
                    Py_DECREF(iter);
                    Py_DECREF(results);
                    return NULL;
                }
                view = std::string_view(data, static_cast<size_t>(len));
            } else {
                Py_DECREF(line_obj);
                Py_DECREF(iter);
                Py_DECREF(results);
                PyErr_SetString(PyExc_TypeError, "NDJSON lines must be str or bytes");
                return NULL;
            }
            view = trim_line_endings(view);
            bool ok = process_ndjson_line(view, line_no, compiled_path, mode, results, options,
                                          parse_context);
            Py_DECREF(line_obj);
            if (!ok) {
                Py_DECREF(iter);
                Py_DECREF(results);
                return NULL;
            }
        }
        Py_DECREF(iter);
        if (PyErr_Occurred()) {
            Py_DECREF(results);
            return NULL;
        }
        return results;
    }
    PyErr_Clear();

    if (!PyObject_HasAttrString(data_obj, "readline")) {
        Py_DECREF(results);
        PyErr_SetString(PyExc_TypeError, "search_ndjson expects a path or file-like object");
        return NULL;
    }

    size_t line_no = 0;
    while (true) {
        PyObject* line_obj = PyObject_CallMethod(data_obj, "readline", NULL);
        if (!line_obj) {
            Py_DECREF(results);
            return NULL;
        }

        bool is_empty = false;
        std::string_view view;
        if (PyBytes_Check(line_obj)) {
            char* data = nullptr;
            Py_ssize_t len = 0;
            if (PyBytes_AsStringAndSize(line_obj, &data, &len) < 0) {
                Py_DECREF(line_obj);
                Py_DECREF(results);
                return NULL;
            }
            if (len == 0) {
                is_empty = true;
            } else {
                view = std::string_view(data, static_cast<size_t>(len));
            }
        } else if (PyUnicode_Check(line_obj)) {
            Py_ssize_t len = 0;
            const char* data = PyUnicode_AsUTF8AndSize(line_obj, &len);
            if (!data) {
                Py_DECREF(line_obj);
                Py_DECREF(results);
                return NULL;
            }
            if (len == 0) {
                is_empty = true;
            } else {
                view = std::string_view(data, static_cast<size_t>(len));
            }
        } else {
            Py_DECREF(line_obj);
            Py_DECREF(results);
            PyErr_SetString(PyExc_TypeError, "NDJSON lines must be str or bytes");
            return NULL;
        }

        if (is_empty) {
            Py_DECREF(line_obj);
            break;
        }

        line_no++;
        view = trim_line_endings(view);
        bool ok = process_ndjson_line(view, line_no, compiled_path, mode, results, options,
                                      parse_context);
        Py_DECREF(line_obj);
        if (!ok) {
            Py_DECREF(results);
            return NULL;
        }
    }

    return results;

    STRATA_CPP_CATCH
}

//=============================================================================
// Module Registration
//=============================================================================

int register_jsonpath_types(PyObject* module) {
    // Register CompiledPath type
    if (PyType_Ready(&PyCompiledPathType) < 0) {
        return -1;
    }
    Py_INCREF(&PyCompiledPathType);
    if (PyModule_AddObject(module, "CompiledPath", (PyObject*)&PyCompiledPathType) < 0) {
        Py_DECREF(&PyCompiledPathType);
        return -1;
    }

    return 0;
}

// Methods are added to main module via python_module.cpp
// No need for get_jsonpath_methods() here

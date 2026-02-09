#include "python_convert.h"
#include "python_ndjson.h"
#include "python_object_builder.h"
#include "python_types.h"
#include "strata/json/json_parse.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/json/parallel_ndjson.hpp"
#include "strata/util/arena_allocator.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

// Run a block inside STRATA_CPP_TRY/STRATA_CPP_CATCH for one-liner stream methods.
#define STRATA_NDJSON_TRY_RETURN_BLOCK(block)                                                      \
    STRATA_CPP_TRY                                                                                 \
    block STRATA_CPP_CATCH

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

bool process_ndjson_cursor_line(std::string_view line, size_t line_no, NdjsonErrorMode mode,
                                strata::bindings::NdjsonCursorData& cursor_data,
                                strata::ParseSaxOptions& options,
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

    cursor_data.line_numbers.push_back(line_no);
    cursor_data.values.push_back(std::move(parse_result.value));
    return true;
}

} // namespace

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
// NdjsonCursor Type
//=============================================================================

typedef struct {
    PyObject_HEAD strata::bindings::NdjsonCursorData* cursor;
} PyNdjsonCursor;

static void PyNdjsonCursor_dealloc(PyNdjsonCursor* self) {
    delete self->cursor;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyNdjsonCursor_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyNdjsonCursor* self = (PyNdjsonCursor*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cursor = nullptr;
    }
    return (PyObject*)self;
}

// Forward declarations
static PyObject* PyNdjsonCursor_from_string(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* PyNdjsonCursor_from_file(PyObject* cls, PyObject* args, PyObject* kwargs);

// Method table
static PyMethodDef PyNdjsonCursor_methods[] = {
    {"from_string", (PyCFunction)PyNdjsonCursor_from_string,
     METH_VARARGS | METH_KEYWORDS | METH_CLASS, "Create NdjsonCursor from string"},
    {"from_file", (PyCFunction)PyNdjsonCursor_from_file,
     METH_VARARGS | METH_KEYWORDS | METH_CLASS, "Create NdjsonCursor from file"},
    {NULL, NULL, 0, NULL}};

// Type object
static PyTypeObject PyNdjsonCursorType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.NdjsonCursor",
    .tp_basicsize = sizeof(PyNdjsonCursor),
    .tp_dealloc = (destructor)PyNdjsonCursor_dealloc,
    .tp_new = PyNdjsonCursor_new,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "NDJSON Cursor",
    .tp_methods = PyNdjsonCursor_methods,
};

//=============================================================================
// NdjsonCursor Method Implementations
//=============================================================================

static PyObject* PyNdjsonCursor_from_string(PyObject* cls, PyObject* args, PyObject* kwargs) {
    (void)cls;  // Unused
    PyObject* data_obj;
    int skip_errors = 0;
    PyObject* on_error_obj = Py_None;

    static const char* kwlist[] = {"data", "skip_errors", "on_error", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|pO", const_cast<char**>(kwlist), &data_obj,
                                     &skip_errors, &on_error_obj)) {
        return NULL;
    }

    NdjsonErrorMode mode;
    if (!parse_ndjson_error_mode(skip_errors, on_error_obj, &mode)) {
        return NULL;
    }

    const char* data = nullptr;
    Py_ssize_t len = 0;
    if (PyBytes_Check(data_obj)) {
        char* bytes = nullptr;
        if (PyBytes_AsStringAndSize(data_obj, &bytes, &len) < 0) {
            return NULL;
        }
        data = bytes;
    } else if (PyUnicode_Check(data_obj)) {
        data = PyUnicode_AsUTF8AndSize(data_obj, &len);
        if (!data) {
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "data must be str or bytes");
        return NULL;
    }

    STRATA_CPP_TRY

    auto* cursor_data = new strata::bindings::NdjsonCursorData();
    strata::ParseSaxOptions options;
    strata::ParseSaxContext parse_context;

    std::string_view text(data, static_cast<size_t>(len));
    size_t line_no = 0;
    size_t pos = 0;
    while (pos < text.size()) {
        size_t next = text.find('\n', pos);
        if (next == std::string_view::npos) {
            next = text.size();
        }
        line_no++;
        std::string_view line(text.data() + pos, next - pos);
        line = trim_line_endings(line);
        if (!process_ndjson_cursor_line(line, line_no, mode, *cursor_data, options,
                                        parse_context)) {
            delete cursor_data;
            return NULL;
        }
        if (next == text.size()) {
            break;
        }
        pos = next + 1;
    }

    PyNdjsonCursor* self_obj = (PyNdjsonCursor*)PyType_GenericAlloc(&PyNdjsonCursorType, 0);
    if (!self_obj) {
        delete cursor_data;
        return NULL;
    }
    self_obj->cursor = cursor_data;
    return (PyObject*)self_obj;

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonCursor_from_file(PyObject* cls, PyObject* args, PyObject* kwargs) {
    (void)cls;  // Unused
    PyObject* path_obj;
    int skip_errors = 0;
    PyObject* on_error_obj = Py_None;

    static const char* kwlist[] = {"filepath", "skip_errors", "on_error", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|pO", const_cast<char**>(kwlist), &path_obj,
                                     &skip_errors, &on_error_obj)) {
        return NULL;
    }

    NdjsonErrorMode mode;
    if (!parse_ndjson_error_mode(skip_errors, on_error_obj, &mode)) {
        return NULL;
    }

    PyObject* pathlike = PyOS_FSPath(path_obj);
    if (!pathlike) {
        return NULL;
    }
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
        return NULL;
    }

    STRATA_CPP_TRY

    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        PyErr_SetString(PyExc_OSError, "Failed to open NDJSON file");
        return NULL;
    }

    auto* cursor_data = new strata::bindings::NdjsonCursorData();
    strata::ParseSaxOptions options;
    strata::ParseSaxContext parse_context;

    std::string line;
    size_t line_no = 0;
    while (std::getline(file, line)) {
        line_no++;
        std::string_view view(line);
        view = trim_line_endings(view);
        if (!process_ndjson_cursor_line(view, line_no, mode, *cursor_data, options,
                                        parse_context)) {
            delete cursor_data;
            return NULL;
        }
    }
    if (file.bad()) {
        delete cursor_data;
        PyErr_SetString(PyExc_OSError, "Error reading NDJSON file");
        return NULL;
    }

    PyNdjsonCursor* self_obj = (PyNdjsonCursor*)PyType_GenericAlloc(&PyNdjsonCursorType, 0);
    if (!self_obj) {
        delete cursor_data;
        return NULL;
    }
    self_obj->cursor = cursor_data;
    return (PyObject*)self_obj;

    STRATA_CPP_CATCH
}

//=============================================================================
// NdjsonCursor Type helpers (exported for other bindings)
//=============================================================================

bool is_py_ndjson_cursor(PyObject* obj) { return obj && Py_TYPE(obj) == &PyNdjsonCursorType; }

strata::bindings::NdjsonCursorData* get_py_ndjson_cursor(PyObject* obj) {
    if (!is_py_ndjson_cursor(obj)) {
        return nullptr;
    }
    return ((PyNdjsonCursor*)obj)->cursor;
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
    if (PyType_Ready(&PyNdjsonCursorType) < 0) {
        return -1;
    }
    Py_INCREF(&PyNdjsonStreamType);
    if (PyModule_AddObject(module, "NdjsonStream", (PyObject*)&PyNdjsonStreamType) < 0) {
        Py_DECREF(&PyNdjsonStreamType);
        return -1;
    }
    Py_INCREF(&PyNdjsonCursorType);
    if (PyModule_AddObject(module, "NdjsonCursor", (PyObject*)&PyNdjsonCursorType) < 0) {
        Py_DECREF(&PyNdjsonCursorType);
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

#include "python_convert.h"
#include "python_ndjson.h"
#include "python_object_builder.h"
#include "python_types.h"
#include "strata/json/json_parse.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/json/parallel_ndjson.hpp"
#include "strata/util/arena_allocator.hpp"
#include "strata/util/simd_string.hpp"

#include <algorithm>
#include <cerrno>
#include <cctype>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <string_view>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
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

using strata::bindings::NdjsonCursorErrorMode;

bool parse_ndjson_error_mode(int skip_errors, PyObject* on_error_obj,
                             NdjsonCursorErrorMode* mode) {
    if (!mode) {
        PyErr_SetString(PyExc_RuntimeError, "invalid error mode output");
        return false;
    }
    if (on_error_obj == Py_None) {
        *mode = skip_errors ? NdjsonCursorErrorMode::Skip : NdjsonCursorErrorMode::Error;
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
        *mode = NdjsonCursorErrorMode::Skip;
        return true;
    }
    if (value == "warn") {
        *mode = NdjsonCursorErrorMode::Warn;
        return true;
    }
    if (value == "error") {
        *mode = NdjsonCursorErrorMode::Error;
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

bool process_ndjson_cursor_line(std::string_view line, size_t line_no, NdjsonCursorErrorMode mode,
                                strata::bindings::NdjsonCursorData& cursor_data,
                                strata::ParseSaxOptions& options,
                                strata::ParseSaxContext& parse_context) {
    if (line.empty() || is_whitespace_only(line)) {
        return true;
    }
    auto parse_result = strata::parse_json(line, options, &parse_context);
    if (!parse_result.ok()) {
        if (mode == NdjsonCursorErrorMode::Skip) {
            return true;
        }
        std::string message = "Invalid JSON on line " + std::to_string(line_no);
        if (mode == NdjsonCursorErrorMode::Warn) {
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

constexpr size_t kParallelMinSize = 2 * 1024 * 1024;
constexpr size_t kParallelSmallLineThreshold = 4 * 1024;
constexpr size_t kParallelSmallLineChunkSize = 2 * 1024 * 1024;

size_t count_newlines(const char* data, Py_ssize_t len) {
    if (!data || len <= 0) {
        return 0;
    }
    size_t count = 0;
    for (Py_ssize_t i = 0; i < len; ++i) {
        if (data[i] == '\n') {
            ++count;
        }
    }
    return count;
}

struct MappedRegion {
    const char* data = nullptr;
    size_t size = 0;

    ~MappedRegion() {
        if (data && size > 0) {
            munmap(const_cast<char*>(data), size);
        }
    }
};

static std::shared_ptr<void> map_file_readonly(const char* filepath, const char** data_out,
                                               size_t* size_out,
                                               std::string* error_message) {
    if (data_out) {
        *data_out = nullptr;
    }
    if (size_out) {
        *size_out = 0;
    }

    int fd = ::open(filepath, O_RDONLY);
    if (fd < 0) {
        if (error_message) {
            *error_message = std::string("Failed to open NDJSON file: ") + std::strerror(errno);
        }
        return {};
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        ::close(fd);
        if (error_message) {
            *error_message = std::string("Failed to stat NDJSON file: ") + std::strerror(errno);
        }
        return {};
    }

    size_t size = static_cast<size_t>(st.st_size);
    if (size_out) {
        *size_out = size;
    }
    if (size == 0) {
        ::close(fd);
        if (data_out) {
            *data_out = nullptr;
        }
        return {};
    }

    void* addr = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
    ::close(fd);
    if (addr == MAP_FAILED) {
        if (error_message) {
            *error_message = std::string("Failed to mmap NDJSON file: ") + std::strerror(errno);
        }
        return {};
    }

    const char* data = static_cast<const char*>(addr);
    if (data_out) {
        *data_out = data;
    }

    auto region = std::make_shared<MappedRegion>();
    region->data = data;
    region->size = size;
    return std::shared_ptr<void>(region);
}

} // namespace

strata::bindings::NdjsonCursorData* create_lazy_ndjson_cursor_data(
    const char* filepath,
    strata::bindings::NdjsonCursorErrorMode mode,
    std::string* error_message) {
    if (!filepath) {
        if (error_message) {
            *error_message = "Invalid NDJSON file path";
        }
        return nullptr;
    }

    const char* data_ptr = nullptr;
    size_t data_size = 0;
    std::shared_ptr<void> mapped = map_file_readonly(filepath, &data_ptr, &data_size,
                                                     error_message);
    if (!mapped && ((data_size > 0) || (error_message && !error_message->empty()))) {
        return nullptr;
    }

    auto* cursor_data = new strata::bindings::NdjsonCursorData();
    cursor_data->lazy = true;
    cursor_data->error_mode = mode;
    cursor_data->data_ptr = data_ptr;
    cursor_data->data_size = data_size;
    cursor_data->mapped = std::move(mapped);
    cursor_data->parsed_lines = 0;

    std::string_view data_view(data_ptr ? data_ptr : "", data_size);
    cursor_data->line_offsets = strata::collect_line_offsets(data_view);

    cursor_data->data_line_offsets.clear();
    cursor_data->data_line_numbers.clear();
    cursor_data->data_line_offsets.reserve(cursor_data->line_offsets.size());
    cursor_data->data_line_numbers.reserve(cursor_data->line_offsets.size());

    for (size_t i = 0; i < cursor_data->line_offsets.size(); ++i) {
        size_t start = cursor_data->line_offsets[i];
        size_t end = (i + 1 < cursor_data->line_offsets.size())
                         ? cursor_data->line_offsets[i + 1]
                         : data_size;
        if (end < start) {
            continue;
        }
        std::string_view line(data_view.data() + start, end - start);
        line = trim_line_endings(line);
        if (line.empty()) {
            continue;
        }
        if (strata::util::is_whitespace_only_simd(line.data(), line.size())) {
            continue;
        }
        cursor_data->data_line_offsets.push_back(start);
        cursor_data->data_line_numbers.push_back(i + 1);
    }

    return cursor_data;
}

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

namespace {

PyObject* parse_ndjson_sequential(const char* data, Py_ssize_t len, bool skip_errors) {
    strata::NdjsonStream stream(std::string_view(data, static_cast<size_t>(len)));
    NdjsonPythonContext context;
    std::vector<PyObject*> items;
    items.reserve(256);

    stream.validate_utf8_once();
    PyGcPause gc_pause;

    while (true) {
        context.builder.reset();
        auto status = stream.next_sax(context.builder);
        if (status == strata::Status::Ok) {
            PyObject* obj = context.builder.take_root();
            if (!obj) {
                context.builder.reset();
                for (auto* item : items) {
                    Py_DECREF(item);
                }
                PyErr_SetString(PyExc_RuntimeError, "NDJSON parse produced no result");
                return NULL;
            }
            items.push_back(obj);
            continue;
        }

        context.builder.reset();
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
}

} // namespace

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
static PyObject* strata_parse_ndjson(PyObject* self, PyObject* args, PyObject* kwargs);
static PyObject* strata_iter_ndjson(PyObject* self, PyObject* args, PyObject* kwargs);

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

// Module-level NDJSON functions
static PyMethodDef ndjson_module_methods[] = {
    {"parse_ndjson", (PyCFunction)strata_parse_ndjson, METH_VARARGS | METH_KEYWORDS,
     "parse_ndjson(data, *, skip_errors=False, parallel=None, num_threads=0) -> list\n\n"
     "Parse NDJSON data into a list of Python objects."},
    {"iter_ndjson", (PyCFunction)strata_iter_ndjson, METH_VARARGS | METH_KEYWORDS,
     "iter_ndjson(data, *, skip_errors=False, batch_size=1024) -> iterator\n\n"
     "Iterate over NDJSON lines as Python objects."},
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
// NdjsonIterator Type
//=============================================================================

typedef struct {
    PyObject_HEAD strata::NdjsonStream* stream;
    NdjsonPythonContext* context;
    PyObject* data_ref;
    int skip_errors;
    Py_ssize_t batch_size;
    PyObject* batch_list;
    Py_ssize_t batch_index;
    int pending_error;
    int finished;
} PyNdjsonIterator;

static void PyNdjsonIterator_dealloc(PyNdjsonIterator* self) {
    delete self->context;
    delete self->stream;
    Py_XDECREF(self->data_ref);
    Py_XDECREF(self->batch_list);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyNdjsonIterator_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyNdjsonIterator* self = (PyNdjsonIterator*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->stream = nullptr;
        self->context = nullptr;
        self->data_ref = nullptr;
        self->skip_errors = 0;
        self->batch_size = 0;
        self->batch_list = nullptr;
        self->batch_index = 0;
        self->pending_error = 0;
        self->finished = 0;
    }
    return (PyObject*)self;
}

static PyObject* PyNdjsonIterator_iter(PyObject* self) {
    Py_INCREF(self);
    return self;
}

static PyObject* ndjson_iterator_next_batch(PyNdjsonIterator* self, int* hit_error) {
    if (hit_error) {
        *hit_error = 0;
    }

    std::vector<PyObject*> items;
    items.reserve(static_cast<size_t>(self->batch_size));

    NdjsonPythonContext* context = self->context;
    self->stream->validate_utf8_once();
    PyGcPause gc_pause;

    bool saw_error = false;

    while (items.size() < static_cast<size_t>(self->batch_size)) {
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
            self->finished = 1;
            break;
        }

        if (PyErr_Occurred()) {
            for (auto* item : items) {
                Py_DECREF(item);
            }
            return NULL;
        }

        if (!self->skip_errors) {
            saw_error = true;
            self->finished = 1;
            break;
        }
    }

    if (hit_error) {
        *hit_error = saw_error ? 1 : 0;
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
}

static PyObject* PyNdjsonIterator_iternext(PyNdjsonIterator* self) {
    if (self->batch_list && self->batch_index < PyList_GET_SIZE(self->batch_list)) {
        PyObject* item = PyList_GET_ITEM(self->batch_list, self->batch_index++);
        Py_INCREF(item);
        return item;
    }

    Py_CLEAR(self->batch_list);
    self->batch_index = 0;

    if (self->pending_error) {
        self->pending_error = 0;
        self->finished = 1;
        PyErr_SetString(PyExc_ValueError, "Invalid JSON in NDJSON line");
        return NULL;
    }

    if (self->finished) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    int hit_error = 0;
    PyObject* batch = ndjson_iterator_next_batch(self, &hit_error);
    if (!batch) {
        return NULL;
    }

    self->batch_list = batch;
    self->batch_index = 0;
    self->pending_error = hit_error;

    if (PyList_GET_SIZE(self->batch_list) == 0) {
        if (self->pending_error) {
            self->pending_error = 0;
            self->finished = 1;
            PyErr_SetString(PyExc_ValueError, "Invalid JSON in NDJSON line");
            return NULL;
        }
        if (self->finished) {
            PyErr_SetNone(PyExc_StopIteration);
            return NULL;
        }
    }

    PyObject* item = PyList_GET_ITEM(self->batch_list, self->batch_index++);
    Py_INCREF(item);
    return item;
}

static PyTypeObject PyNdjsonIteratorType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.NdjsonIterator",
    .tp_basicsize = sizeof(PyNdjsonIterator),
    .tp_dealloc = (destructor)PyNdjsonIterator_dealloc,
    .tp_new = PyNdjsonIterator_new,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "NDJSON Iterator",
    .tp_iter = PyNdjsonIterator_iter,
    .tp_iternext = (iternextfunc)PyNdjsonIterator_iternext,
};

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
static PyObject* PyNdjsonCursor_from_file_lazy(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* PyNdjsonCursor_stats(PyNdjsonCursor* self, PyObject* args);

// Method table
static PyMethodDef PyNdjsonCursor_methods[] = {
    {"from_string", (PyCFunction)PyNdjsonCursor_from_string,
     METH_VARARGS | METH_KEYWORDS | METH_CLASS, "Create NdjsonCursor from string"},
    {"from_file", (PyCFunction)PyNdjsonCursor_from_file,
     METH_VARARGS | METH_KEYWORDS | METH_CLASS, "Create NdjsonCursor from file"},
    {"from_file_lazy", (PyCFunction)PyNdjsonCursor_from_file_lazy,
     METH_VARARGS | METH_KEYWORDS | METH_CLASS, "Create lazy NdjsonCursor from file"},
    {"stats", (PyCFunction)PyNdjsonCursor_stats, METH_NOARGS, "NdjsonCursor stats"},
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

PyObject* create_py_ndjson_cursor(strata::bindings::NdjsonCursorData* cursor_data) {
    if (!cursor_data) {
        return NULL;
    }
    PyNdjsonCursor* self_obj = (PyNdjsonCursor*)PyType_GenericAlloc(&PyNdjsonCursorType, 0);
    if (!self_obj) {
        delete cursor_data;
        return NULL;
    }
    self_obj->cursor = cursor_data;
    return (PyObject*)self_obj;
}

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

    NdjsonCursorErrorMode mode;
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
    cursor_data->lazy = false;
    cursor_data->error_mode = mode;
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
    cursor_data->parsed_lines = cursor_data->values.size();
    return create_py_ndjson_cursor(cursor_data);

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

    NdjsonCursorErrorMode mode;
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
    cursor_data->lazy = false;
    cursor_data->error_mode = mode;
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

    cursor_data->parsed_lines = cursor_data->values.size();
    return create_py_ndjson_cursor(cursor_data);

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonCursor_from_file_lazy(PyObject* cls, PyObject* args, PyObject* kwargs) {
    (void)cls;  // Unused
    PyObject* path_obj;
    int skip_errors = 0;
    PyObject* on_error_obj = Py_None;

    static const char* kwlist[] = {"filepath", "skip_errors", "on_error", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|pO", const_cast<char**>(kwlist), &path_obj,
                                     &skip_errors, &on_error_obj)) {
        return NULL;
    }

    NdjsonCursorErrorMode mode;
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

    std::string error_message;
    auto* cursor_data = create_lazy_ndjson_cursor_data(filepath, mode, &error_message);
    if (!cursor_data) {
        if (error_message.empty()) {
            PyErr_SetString(PyExc_OSError, "Failed to open NDJSON file");
        } else {
            PyErr_SetString(PyExc_OSError, error_message.c_str());
        }
        return NULL;
    }

    return create_py_ndjson_cursor(cursor_data);

    STRATA_CPP_CATCH
}

static PyObject* PyNdjsonCursor_stats(PyNdjsonCursor* self, PyObject* args) {
    (void)args;
    if (!self || !self->cursor) {
        PyErr_SetString(PyExc_RuntimeError, "Invalid NdjsonCursor");
        return NULL;
    }
    auto* cursor = self->cursor;
    size_t line_count = cursor->lazy ? cursor->data_line_offsets.size() : cursor->values.size();
    size_t cached_lines = cursor->values.size();
    size_t parsed_lines = cursor->parsed_lines;

    PyObject* result = PyDict_New();
    if (!result) {
        return NULL;
    }

    PyObject* lazy_obj = PyBool_FromLong(cursor->lazy ? 1 : 0);
    PyObject* line_count_obj = PyLong_FromSize_t(line_count);
    PyObject* cached_obj = PyLong_FromSize_t(cached_lines);
    PyObject* parsed_obj = PyLong_FromSize_t(parsed_lines);

    if (!lazy_obj || !line_count_obj || !cached_obj || !parsed_obj) {
        Py_XDECREF(lazy_obj);
        Py_XDECREF(line_count_obj);
        Py_XDECREF(cached_obj);
        Py_XDECREF(parsed_obj);
        Py_DECREF(result);
        return NULL;
    }

    PyDict_SetItemString(result, "lazy", lazy_obj);
    PyDict_SetItemString(result, "line_count", line_count_obj);
    PyDict_SetItemString(result, "cached_lines", cached_obj);
    PyDict_SetItemString(result, "parsed_lines", parsed_obj);

    Py_DECREF(lazy_obj);
    Py_DECREF(line_count_obj);
    Py_DECREF(cached_obj);
    Py_DECREF(parsed_obj);

    return result;
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
// Public NDJSON API
//=============================================================================

static PyObject* strata_parse_ndjson(PyObject* self, PyObject* args, PyObject* kwargs) {
    (void)self;  // Unused
    PyObject* data_obj;
    int skip_errors = 0;
    PyObject* parallel_obj = Py_None;
    int num_threads = 0;

    static const char* kwlist[] = {"data", "skip_errors", "parallel", "num_threads", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|pOi", const_cast<char**>(kwlist), &data_obj,
                                     &skip_errors, &parallel_obj, &num_threads)) {
        return NULL;
    }

    PyObject* unicode_obj = nullptr;
    const char* data = nullptr;
    Py_ssize_t data_len = 0;
    size_t data_chars = 0;

    if (PyUnicode_Check(data_obj)) {
        data = PyUnicode_AsUTF8AndSize(data_obj, &data_len);
        if (!data) {
            return NULL;
        }
        data_chars = static_cast<size_t>(PyUnicode_GetLength(data_obj));
    } else if (PyBytes_Check(data_obj)) {
        char* bytes = nullptr;
        Py_ssize_t len = 0;
        if (PyBytes_AsStringAndSize(data_obj, &bytes, &len) < 0) {
            return NULL;
        }
        unicode_obj = PyUnicode_DecodeUTF8(bytes, len, "strict");
        if (!unicode_obj) {
            return NULL;
        }
        data = PyUnicode_AsUTF8AndSize(unicode_obj, &data_len);
        if (!data) {
            Py_DECREF(unicode_obj);
            return NULL;
        }
        data_chars = static_cast<size_t>(PyUnicode_GetLength(unicode_obj));
    } else {
        PyErr_SetString(PyExc_TypeError, "data must be str or bytes");
        return NULL;
    }

    STRATA_CPP_TRY

    bool parallel_set = false;
    bool parallel = false;
    if (parallel_obj != Py_None) {
        int truth = PyObject_IsTrue(parallel_obj);
        if (truth < 0) {
            Py_XDECREF(unicode_obj);
            return NULL;
        }
        parallel_set = true;
        parallel = truth != 0;
    }

    bool use_parallel = false;
    size_t min_chunk_size = 0;
    if (parallel_set) {
        use_parallel = parallel;
    } else {
        size_t data_size = data_chars;
        if (data_size >= kParallelMinSize) {
            use_parallel = true;
            size_t newline_count = count_newlines(data, data_len);
            size_t line_count = newline_count;
            if (data_size > 0 && data_len > 0 && data[data_len - 1] != '\n') {
                line_count += 1;
            } else if (line_count == 0) {
                line_count = 1;
            }
            if (line_count > 0) {
                double avg_line_size =
                    static_cast<double>(data_size) / static_cast<double>(line_count);
                if (avg_line_size < static_cast<double>(kParallelSmallLineThreshold) &&
                    data_size >= kParallelSmallLineChunkSize * 2) {
                    min_chunk_size = kParallelSmallLineChunkSize;
                }
            }
        }
    }

    PyObject* result = nullptr;
    if (use_parallel) {
        if (num_threads < 0) {
            PyErr_SetString(PyExc_ValueError, "num_threads must be non-negative");
            Py_XDECREF(unicode_obj);
            return NULL;
        }
        strata::ParallelNdjsonConfig config;
        config.skip_errors = skip_errors != 0;
        config.num_threads = num_threads > 0 ? static_cast<size_t>(num_threads) : 0;
        if (min_chunk_size > 0) {
            config.min_chunk_size = min_chunk_size;
        }
        strata::ParallelNdjsonStream stream(std::string_view(data, static_cast<size_t>(data_len)),
                                            config);
        std::vector<strata::JsonValue> cpp_results = stream.parse_all_parallel();
        result = json_value_list_to_python(cpp_results);
    } else {
        result = parse_ndjson_sequential(data, data_len, skip_errors != 0);
    }

    Py_XDECREF(unicode_obj);
    return result;

    STRATA_CPP_CATCH
}

static PyObject* strata_iter_ndjson(PyObject* self, PyObject* args, PyObject* kwargs) {
    (void)self;  // Unused
    PyObject* data_obj;
    int skip_errors = 0;
    Py_ssize_t batch_size = 1024;

    static const char* kwlist[] = {"data", "skip_errors", "batch_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|pn", const_cast<char**>(kwlist), &data_obj,
                                     &skip_errors, &batch_size)) {
        return NULL;
    }

    if (batch_size <= 0) {
        PyErr_SetString(PyExc_ValueError, "batch_size must be positive");
        return NULL;
    }

    PyObject* data_ref = nullptr;
    const char* data = nullptr;
    Py_ssize_t data_len = 0;

    if (PyUnicode_Check(data_obj)) {
        data = PyUnicode_AsUTF8AndSize(data_obj, &data_len);
        if (!data) {
            return NULL;
        }
        data_ref = data_obj;
        Py_INCREF(data_ref);
    } else if (PyBytes_Check(data_obj)) {
        char* bytes = nullptr;
        Py_ssize_t len = 0;
        if (PyBytes_AsStringAndSize(data_obj, &bytes, &len) < 0) {
            return NULL;
        }
        PyObject* unicode_obj = PyUnicode_DecodeUTF8(bytes, len, "strict");
        if (!unicode_obj) {
            return NULL;
        }
        data = PyUnicode_AsUTF8AndSize(unicode_obj, &data_len);
        if (!data) {
            Py_DECREF(unicode_obj);
            return NULL;
        }
        data_ref = unicode_obj;
    } else {
        PyErr_SetString(PyExc_TypeError, "data must be str or bytes");
        return NULL;
    }

    STRATA_CPP_TRY

    PyNdjsonIterator* iter =
        (PyNdjsonIterator*)PyType_GenericAlloc(&PyNdjsonIteratorType, 0);
    if (!iter) {
        Py_XDECREF(data_ref);
        return NULL;
    }

    iter->stream = new strata::NdjsonStream(std::string_view(data, static_cast<size_t>(data_len)));
    iter->context = new NdjsonPythonContext();
    iter->data_ref = data_ref;
    iter->skip_errors = skip_errors != 0;
    iter->batch_size = batch_size;
    iter->batch_list = nullptr;
    iter->batch_index = 0;
    iter->pending_error = 0;
    iter->finished = 0;

    return (PyObject*)iter;

    STRATA_CPP_CATCH
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
    if (PyType_Ready(&PyNdjsonIteratorType) < 0) {
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
    Py_INCREF(&PyNdjsonIteratorType);
    if (PyModule_AddObject(module, "NdjsonIterator", (PyObject*)&PyNdjsonIteratorType) < 0) {
        Py_DECREF(&PyNdjsonIteratorType);
        return -1;
    }

    // Add NDJSON functions to module
    for (PyMethodDef* method = ndjson_module_methods; method->ml_name != NULL; ++method) {
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

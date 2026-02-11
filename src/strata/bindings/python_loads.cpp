#include "python_convert.h"
#include "python_object_builder.h"
#include "python_types.h"
#include "strata/json/ndjson_stream.hpp"
#include "strata/json/parallel_ndjson.hpp"
#include "strata/json/json_parse.hpp"

#include <algorithm>
#include <climits>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

// Thread-local arena for zero-allocation parsing
thread_local strata::util::Arena g_parse_arena;
thread_local strata::bindings::KeyCache g_key_cache;
thread_local strata::util::Arena g_parse_builder_arena(4 * 1024);
thread_local strata::bindings::PythonObjectBuilder g_parse_builder(&g_parse_builder_arena,
                                                                  g_key_cache);

namespace {
using strata::bindings::KeyCache;

struct BuilderResetGuard {
    strata::bindings::PythonObjectBuilder& builder;
    explicit BuilderResetGuard(strata::bindings::PythonObjectBuilder& builder_ref)
        : builder(builder_ref) {}
    ~BuilderResetGuard() { builder.reset(); }
    BuilderResetGuard(const BuilderResetGuard&) = delete;
    BuilderResetGuard& operator=(const BuilderResetGuard&) = delete;
};
} // namespace

static void emit_duplicate_key_warnings() {
    auto warnings = strata::consume_parse_warnings();
    for (const auto& msg : warnings) {
        PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
    }
}

namespace {
constexpr size_t kParallelMinSize = 2 * 1024 * 1024;
constexpr size_t kParallelSmallLineThreshold = 4 * 1024;
constexpr size_t kParallelSmallLineChunkSize = 2 * 1024 * 1024;

inline bool is_json_whitespace(unsigned char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

bool looks_like_json_text(const char* data, Py_ssize_t len) {
    if (!data || len <= 0) {
        return true;
    }
    Py_ssize_t i = 0;
    while (i < len && is_json_whitespace(static_cast<unsigned char>(data[i]))) {
        ++i;
    }
    if (i >= len) {
        return true;
    }
    unsigned char c = static_cast<unsigned char>(data[i]);
    if (c == '{' || c == '[' || c == '"' || c == '-' || c == 't' || c == 'f' || c == 'n') {
        return true;
    }
    return c >= '0' && c <= '9';
}

bool ends_with_ndjson(const char* data, Py_ssize_t len) {
    static const char suffix[] = ".ndjson";
    constexpr Py_ssize_t suffix_len = static_cast<Py_ssize_t>(sizeof(suffix) - 1);
    if (!data || len < suffix_len) {
        return false;
    }
    return std::equal(suffix, suffix + suffix_len, data + (len - suffix_len));
}

bool is_ndjson_name(PyObject* obj) {
    if (PyUnicode_Check(obj)) {
        Py_ssize_t len = 0;
        const char* text = PyUnicode_AsUTF8AndSize(obj, &len);
        if (!text) {
            return false;
        }
        return ends_with_ndjson(text, len);
    }
    if (PyBytes_Check(obj)) {
        char* data = nullptr;
        Py_ssize_t len = 0;
        if (PyBytes_AsStringAndSize(obj, &data, &len) < 0) {
            return false;
        }
        return ends_with_ndjson(data, len);
    }
    PyObject* pathlike = PyOS_FSPath(obj);
    if (pathlike) {
        bool result = is_ndjson_name(pathlike);
        Py_DECREF(pathlike);
        return result;
    }
    PyErr_Clear();
    return false;
}

bool detect_ndjson_name_attr(PyObject* obj) {
    PyObject* name_obj = PyObject_GetAttrString(obj, "name");
    if (!name_obj) {
        PyErr_Clear();
        return false;
    }
    bool result = is_ndjson_name(name_obj);
    Py_DECREF(name_obj);
    return result;
}

bool read_file_bytes(const char* filepath, std::string* out) {
    if (!out) {
        PyErr_SetString(PyExc_RuntimeError, "invalid output buffer");
        return false;
    }
    std::FILE* file = std::fopen(filepath, "rb");
    if (!file) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, filepath);
        return false;
    }
    if (std::fseek(file, 0, SEEK_END) != 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, filepath);
        std::fclose(file);
        return false;
    }
    long size = std::ftell(file);
    if (size < 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, filepath);
        std::fclose(file);
        return false;
    }
    if (std::fseek(file, 0, SEEK_SET) != 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, filepath);
        std::fclose(file);
        return false;
    }
    out->assign(static_cast<size_t>(size), '\0');
    if (size > 0) {
        size_t read_count = std::fread(out->data(), 1, static_cast<size_t>(size), file);
        if (read_count != static_cast<size_t>(size)) {
            PyErr_SetFromErrnoWithFilename(PyExc_OSError, filepath);
            std::fclose(file);
            return false;
        }
    }
    std::fclose(file);
    return true;
}

size_t count_lines(const char* data, Py_ssize_t len) {
    if (!data || len <= 0) {
        return 0;
    }
    size_t count = 0;
    for (Py_ssize_t i = 0; i < len; ++i) {
        if (data[i] == '\n') {
            ++count;
        }
    }
    if (len > 0 && data[len - 1] != '\n') {
        ++count;
    }
    return count;
}

PyObject* parse_ndjson_text(const char* data, Py_ssize_t len, bool skip_errors,
                            PyObject* parallel_obj, int num_threads) {
    bool parallel_set = false;
    bool parallel = false;
    if (parallel_obj != Py_None) {
        int truth = PyObject_IsTrue(parallel_obj);
        if (truth < 0) {
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
        size_t data_size = static_cast<size_t>(len);
        if (data_size >= kParallelMinSize) {
            use_parallel = true;
            size_t line_count = count_lines(data, len);
            if (line_count > 0) {
                size_t avg_line = data_size / line_count;
                if (avg_line < kParallelSmallLineThreshold &&
                    data_size >= kParallelSmallLineChunkSize * 2) {
                    min_chunk_size = kParallelSmallLineChunkSize;
                }
            }
        }
    }

    if (use_parallel) {
        if (num_threads < 0) {
            PyErr_SetString(PyExc_ValueError, "num_threads must be non-negative");
            return NULL;
        }
        strata::ParallelNdjsonConfig config;
        config.skip_errors = skip_errors;
        config.num_threads = num_threads > 0 ? static_cast<size_t>(num_threads) : 0;
        if (min_chunk_size > 0) {
            config.min_chunk_size = min_chunk_size;
        }
        strata::ParallelNdjsonStream stream(std::string_view(data, static_cast<size_t>(len)),
                                            config);
        std::vector<strata::JsonValue> results = stream.parse_all_parallel();
        emit_duplicate_key_warnings();
        return json_value_list_to_python(results);
    }

    strata::NdjsonStream stream(std::string_view(data, static_cast<size_t>(len)));
    std::vector<strata::JsonValue> results = stream.parse_all_fast(skip_errors);
    emit_duplicate_key_warnings();
    return json_value_list_to_python(results);
}
} // namespace

static PyObject* json_value_to_python_internal(const strata::JsonValue& val, KeyCache& cache) {
    if (val.is_null()) {
        Py_RETURN_NONE;
    }

    if (val.is_bool()) {
        PyObject* result = val.as_bool() ? Py_True : Py_False;
        Py_INCREF(result);
        return result;
    }

    if (val.is_int()) {
        int64_t v = val.as_int();
        if (v >= LONG_MIN && v <= LONG_MAX) {
            return PyLong_FromLong(static_cast<long>(v));
        }
        return PyLong_FromLongLong(v);
    }

    if (val.is_double()) {
        return PyFloat_FromDouble(val.as_double());
    }

    if (val.is_string()) {
        const std::string& s = val.as_string();
        return PyUnicode_FromStringAndSize(s.c_str(), s.size());
    }

    if (val.is_array()) {
        const auto& arr = val.as_array();
        PyObject* list = PyList_New(arr.size());
        if (!list)
            return NULL;
        for (size_t i = 0; i < arr.size(); ++i) {
            PyObject* item = json_value_to_python_internal(arr[i], cache);
            if (!item) {
                Py_DECREF(list);
                return NULL;
            }
            PyList_SET_ITEM(list, i, item);
        }
        return list;
    }

    if (val.is_object()) {
        const auto& obj = val.as_object();
        PyObject* dict = PyDict_New();
        if (!dict)
            return NULL;

        for (const auto& [key, value] : obj) {
            PyObject* py_key = cache.get(key);
            if (!py_key) {
                Py_DECREF(dict);
                return NULL;
            }

            PyObject* py_val = json_value_to_python_internal(value, cache);
            if (!py_val) {
                Py_DECREF(py_key);
                Py_DECREF(dict);
                return NULL;
            }

            int rc = PyDict_SetItem(dict, py_key, py_val);
            Py_DECREF(py_key);
            Py_DECREF(py_val);

            if (rc < 0) {
                Py_DECREF(dict);
                return NULL;
            }
        }
        return dict;
    }

    Py_RETURN_NONE;
}

// Convert JsonValue to PyObject
PyObject* json_value_to_python(const strata::JsonValue& val) {
    strata::util::Arena arena;
    KeyCache cache(&arena);
    return json_value_to_python_internal(val, cache);
}

static PyObject* parse_json_buffer(const char* data, Py_ssize_t len) {
    // Reset thread-local arena for reuse
    g_parse_arena.reset();
    g_key_cache.reset(&g_parse_arena);
    g_parse_builder.reset();
    BuilderResetGuard builder_guard(g_parse_builder);

    // Use fast path: Direct-to-Python via SAX
    constexpr size_t kGcPauseMinSize = 64 * 1024;
    const size_t size = static_cast<size_t>(len);
    auto parse = [&]() {
        return strata::parse_sax(std::string_view(data, size), g_parse_builder);
    };
    strata::Status status = strata::Status::ParseError;
    if (size >= kGcPauseMinSize) {
        ::PyGcPause gc_pause;
        status = parse();
    } else {
        status = parse();
    }

    if (status != strata::Status::Ok) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        }
        return NULL;
    }

    return g_parse_builder.take_root();
}

// Python loads() function
PyObject* strata_loads(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* source = nullptr;

    static const char* kwlist[] = {"source", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", const_cast<char**>(kwlist), &source)) {
        return NULL;
    }

    STRATA_CPP_TRY

    if (PyUnicode_Check(source)) {
        Py_ssize_t len = 0;
        const char* data = PyUnicode_AsUTF8AndSize(source, &len);
        if (!data) {
            return NULL;
        }
        return parse_json_buffer(data, len);
    }

    if (PyBytes_Check(source)) {
        char* data = nullptr;
        Py_ssize_t len = 0;
        if (PyBytes_AsStringAndSize(source, &data, &len) < 0) {
            return NULL;
        }
        return parse_json_buffer(data, len);
    }

    if (PyByteArray_Check(source) || PyMemoryView_Check(source)) {
        PyObject* bytes_obj = PyBytes_FromObject(source);
        if (!bytes_obj) {
            return NULL;
        }
        char* data = nullptr;
        Py_ssize_t len = 0;
        if (PyBytes_AsStringAndSize(bytes_obj, &data, &len) < 0) {
            Py_DECREF(bytes_obj);
            return NULL;
        }
        PyObject* result = parse_json_buffer(data, len);
        Py_DECREF(bytes_obj);
        return result;
    }

    PyErr_SetString(PyExc_TypeError, "loads() expects str, bytes, bytearray, or memoryview");
    return NULL;

    STRATA_CPP_CATCH
}

// Python load() function
PyObject* strata_load(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* source = nullptr;
    PyObject* ndjson_obj = Py_None;
    int skip_errors = 0;
    PyObject* parallel_obj = Py_None;
    int num_threads = 0;

    static const char* kwlist[] = {"source", "ndjson", "skip_errors", "parallel", "num_threads",
                                   NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|OpOi", const_cast<char**>(kwlist), &source,
                                     &ndjson_obj, &skip_errors, &parallel_obj, &num_threads)) {
        return NULL;
    }

    STRATA_CPP_TRY

    bool ndjson_flag_set = false;
    bool ndjson_flag = false;
    if (ndjson_obj != Py_None) {
        int truth = PyObject_IsTrue(ndjson_obj);
        if (truth < 0) {
            return NULL;
        }
        ndjson_flag_set = true;
        ndjson_flag = truth != 0;
    }

    const char* text_data = nullptr;
    Py_ssize_t text_len = 0;
    bool has_text = false;
    bool looks_like_json = false;
    if (PyUnicode_Check(source)) {
        text_data = PyUnicode_AsUTF8AndSize(source, &text_len);
        if (!text_data) {
            return NULL;
        }
        has_text = true;
        looks_like_json = looks_like_json_text(text_data, text_len);
    } else if (PyBytes_Check(source)) {
        char* data = nullptr;
        if (PyBytes_AsStringAndSize(source, &data, &text_len) < 0) {
            return NULL;
        }
        text_data = data;
        has_text = true;
        looks_like_json = looks_like_json_text(text_data, text_len);
    }

    if (has_text && looks_like_json) {
        if (ndjson_flag_set && ndjson_flag) {
            return parse_ndjson_text(text_data, text_len, skip_errors != 0, parallel_obj,
                                     num_threads);
        }
        return parse_json_buffer(text_data, text_len);
    }

    PyObject* pathlike = PyOS_FSPath(source);
    if (pathlike) {
        bool use_ndjson = false;
        if (ndjson_flag_set) {
            use_ndjson = ndjson_flag;
        } else {
            use_ndjson = is_ndjson_name(pathlike);
            if (PyErr_Occurred()) {
                Py_DECREF(pathlike);
                return NULL;
            }
        }

        const char* filepath = nullptr;
        if (PyUnicode_Check(pathlike)) {
            filepath = PyUnicode_AsUTF8(pathlike);
        } else if (PyBytes_Check(pathlike)) {
            filepath = PyBytes_AsString(pathlike);
        } else {
            Py_DECREF(pathlike);
            PyErr_SetString(PyExc_TypeError, "path must be str or bytes");
            return NULL;
        }
        if (!filepath) {
            Py_DECREF(pathlike);
            return NULL;
        }

        std::string payload;
        if (!read_file_bytes(filepath, &payload)) {
            Py_DECREF(pathlike);
            return NULL;
        }
        Py_DECREF(pathlike);

        if (use_ndjson) {
            return parse_ndjson_text(payload.data(),
                                     static_cast<Py_ssize_t>(payload.size()),
                                     skip_errors != 0, parallel_obj, num_threads);
        }
        return parse_json_buffer(payload.data(), static_cast<Py_ssize_t>(payload.size()));
    }
    PyErr_Clear();

    int has_read = PyObject_HasAttrString(source, "read");
    if (has_read < 0) {
        return NULL;
    }
    if (has_read) {
        PyObject* payload_obj = PyObject_CallMethod(source, "read", NULL);
        if (!payload_obj) {
            return NULL;
        }

        bool use_ndjson = false;
        if (ndjson_flag_set) {
            use_ndjson = ndjson_flag;
        } else {
            use_ndjson = detect_ndjson_name_attr(source);
            if (PyErr_Occurred()) {
                Py_DECREF(payload_obj);
                return NULL;
            }
        }

        PyObject* result = NULL;
        if (PyUnicode_Check(payload_obj)) {
            Py_ssize_t len = 0;
            const char* data = PyUnicode_AsUTF8AndSize(payload_obj, &len);
            if (data) {
                result = use_ndjson
                             ? parse_ndjson_text(data, len, skip_errors != 0, parallel_obj,
                                                 num_threads)
                             : parse_json_buffer(data, len);
            }
        } else if (PyBytes_Check(payload_obj)) {
            char* data = nullptr;
            Py_ssize_t len = 0;
            if (PyBytes_AsStringAndSize(payload_obj, &data, &len) == 0) {
                result = use_ndjson
                             ? parse_ndjson_text(data, len, skip_errors != 0, parallel_obj,
                                                 num_threads)
                             : parse_json_buffer(data, len);
            }
        } else if (PyByteArray_Check(payload_obj) || PyMemoryView_Check(payload_obj)) {
            PyObject* bytes_obj = PyBytes_FromObject(payload_obj);
            if (bytes_obj) {
                char* data = nullptr;
                Py_ssize_t len = 0;
                if (PyBytes_AsStringAndSize(bytes_obj, &data, &len) == 0) {
                    result = use_ndjson
                                 ? parse_ndjson_text(data, len, skip_errors != 0, parallel_obj,
                                                     num_threads)
                                 : parse_json_buffer(data, len);
                }
                Py_DECREF(bytes_obj);
            }
        } else {
            PyErr_SetString(PyExc_TypeError,
                            "file-like object must return str or bytes from read()");
        }

        Py_DECREF(payload_obj);
        return result;
    }

    if (has_text) {
        if (ndjson_flag_set && ndjson_flag) {
            return parse_ndjson_text(text_data, text_len, skip_errors != 0, parallel_obj,
                                     num_threads);
        }
        return parse_json_buffer(text_data, text_len);
    }

    PyErr_SetString(PyExc_TypeError, "load() expects a path, file-like object, or JSON text");
    return NULL;

    STRATA_CPP_CATCH
}

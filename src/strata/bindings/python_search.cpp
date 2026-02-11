#include "python_convert.h"
#include "python_document.h"
#include "python_ndjson.h"
#include "python_types.h"
#include "strata/json/json_mmap.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/json/parallel_ndjson.hpp"
#include "strata/search/search.hpp"
#include "strata/search/search_ndjson_fused.hpp"
#include "strata/util/simd_string.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
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
    File,
};

struct SimpleFieldExtractionSpec {
    bool enabled = false;
    strata::SimpleFieldMode mode = strata::SimpleFieldMode::RootField;
    std::string_view field;
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
        PyErr_SetString(PyExc_TypeError,
                        "strata_mode must be 'dict', 'string', 'cursor', or 'file'");
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
    if (value == "file") {
        *mode = StrataSearchMode::File;
        return true;
    }
    PyErr_SetString(PyExc_ValueError, "strata_mode must be 'dict', 'string', 'cursor', or 'file'");
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

bool fused_ndjson_enabled() {
    const char* env = std::getenv("STRATA_DISABLE_FUSED_NDJSON");
    if (!env || *env == '\0') {
        return true;
    }
    std::string value(env);
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (value == "0" || value == "false" || value == "no") {
        return true;
    }
    return false;
}

SimpleFieldExtractionSpec get_simple_field_spec(const strata::CompiledPath& path) {
    SimpleFieldExtractionSpec spec;
    const auto& steps = path.steps();
    if (steps.size() == 2 && steps[0].op == strata::PathOp::Root &&
        steps[1].op == strata::PathOp::Field) {
        spec.enabled = true;
        spec.mode = strata::SimpleFieldMode::RootField;
        spec.field = steps[1].field;
        return spec;
    }
    if (steps.size() == 3 && steps[0].op == strata::PathOp::Root &&
        steps[1].op == strata::PathOp::Wildcard &&
        steps[2].op == strata::PathOp::Field) {
        spec.enabled = true;
        spec.mode = strata::SimpleFieldMode::RootWildcardField;
        spec.field = steps[2].field;
        return spec;
    }
    return spec;
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

constexpr size_t kParallelMinSize = 2 * 1024 * 1024;
constexpr size_t kParallelSmallLineThreshold = 4 * 1024;
constexpr size_t kParallelSmallLineChunkSize = 2 * 1024 * 1024;

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
    auto result_values = strata::eval_search_path(cursor, compiled_path);
    return json_value_list_to_python(result_values);
}

PyObject* search_dict_mode(PyObject* data_obj, const strata::CompiledPath& compiled_path) {
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

PyObject* search_from_file_like(PyObject* data_obj, const strata::CompiledPath& compiled_path) {
    PyObject* payload = PyObject_CallMethod(data_obj, "read", NULL);
    if (!payload) {
        return NULL;
    }

    PyObject* result = NULL;
    if (PyUnicode_Check(payload)) {
        Py_ssize_t json_len = 0;
        const char* json_data = PyUnicode_AsUTF8AndSize(payload, &json_len);
        if (json_data) {
            result = search_from_json_buffer(json_data, json_len, compiled_path);
        }
    } else if (PyBytes_Check(payload)) {
        char* json_data = nullptr;
        Py_ssize_t json_len = 0;
        if (PyBytes_AsStringAndSize(payload, &json_data, &json_len) == 0) {
            result = search_from_json_buffer(json_data, json_len, compiled_path);
        }
    } else if (PyByteArray_Check(payload) || PyMemoryView_Check(payload)) {
        PyObject* bytes_obj = PyBytes_FromObject(payload);
        if (bytes_obj) {
            char* json_data = nullptr;
            Py_ssize_t json_len = 0;
            if (PyBytes_AsStringAndSize(bytes_obj, &json_data, &json_len) == 0) {
                result = search_from_json_buffer(json_data, json_len, compiled_path);
            }
            Py_DECREF(bytes_obj);
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "file-like object must return str or bytes from read()");
    }

    Py_DECREF(payload);
    return result;
}

PyObject* search_cursor_mode(PyObject* data_obj, const strata::CompiledPath& compiled_path) {
    PyObject* cursor_obj = data_obj;
    PyObject* borrowed = NULL;
    if (!is_py_json_cursor(data_obj) && !is_py_json_document(data_obj) && !is_py_ndjson_cursor(data_obj)) {
        int has_cursor = PyObject_HasAttrString(data_obj, "_cursor");
        if (has_cursor < 0) {
            return NULL;
        }
        if (has_cursor) {
            borrowed = PyObject_GetAttrString(data_obj, "_cursor");
            if (borrowed && is_py_json_cursor(borrowed)) {
                cursor_obj = borrowed;
            } else {
                Py_XDECREF(borrowed);
                borrowed = NULL;
            }
        }
    }

    if (is_py_json_document(data_obj)) {
        auto* doc = get_py_json_document(data_obj);
        if (!doc) {
            Py_XDECREF(borrowed);
            PyErr_SetString(PyExc_TypeError, "Invalid JsonDocument");
            return NULL;
        }

        strata::JsonCursor cursor(doc->root());
        auto result_values = strata::eval_search_path(cursor, compiled_path);
        Py_XDECREF(borrowed);
        return json_value_list_to_python(result_values);
    }

    if (is_py_json_cursor(cursor_obj)) {
        auto* cursor_ptr = get_py_json_cursor(cursor_obj);
        if (!cursor_ptr) {
            Py_XDECREF(borrowed);
            PyErr_SetString(PyExc_TypeError, "Invalid JsonCursor");
            return NULL;
        }

        auto result_values = strata::eval_search_path(*cursor_ptr, compiled_path);
        Py_XDECREF(borrowed);
        return json_value_list_to_python(result_values);
    }

    if (is_py_ndjson_cursor(data_obj)) {
        auto* cursor_data = get_py_ndjson_cursor(data_obj);
        if (!cursor_data) {
            Py_XDECREF(borrowed);
            PyErr_SetString(PyExc_TypeError, "Invalid NdjsonCursor");
            return NULL;
        }
        const auto& values = cursor_data->values;
        const auto& line_numbers = cursor_data->line_numbers;
        if (values.size() != line_numbers.size()) {
            Py_XDECREF(borrowed);
            PyErr_SetString(PyExc_RuntimeError, "NdjsonCursor data is inconsistent");
            return NULL;
        }

        PyObject* results = PyList_New(0);
        if (!results) {
            Py_XDECREF(borrowed);
            return NULL;
        }

        for (size_t i = 0; i < values.size(); ++i) {
            strata::JsonCursor cursor(&values[i]);
            auto matches = strata::eval_search_path(cursor, compiled_path);
            if (matches.empty()) {
                continue;
            }
            if (!append_ndjson_match(results, line_numbers[i], matches)) {
                Py_DECREF(results);
                Py_XDECREF(borrowed);
                return NULL;
            }
        }
        Py_XDECREF(borrowed);
        return results;
    }

    Py_XDECREF(borrowed);
    PyErr_SetString(PyExc_TypeError, "cursor mode expects JsonDocument, JsonCursor, or NdjsonCursor");
    return NULL;
}

PyObject* search_file_pathlike(PyObject* pathlike,
                               const strata::CompiledPath& compiled_path) {
    const char* filepath = nullptr;
    if (PyUnicode_Check(pathlike)) {
        filepath = PyUnicode_AsUTF8(pathlike);
    } else if (PyBytes_Check(pathlike)) {
        filepath = PyBytes_AsString(pathlike);
    } else {
        PyErr_SetString(PyExc_TypeError, "path must be str or bytes");
        return NULL;
    }
    if (!filepath) {
        return NULL;
    }

    auto result = strata::parse_json_file(filepath);
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Failed to parse JSON file");
        return NULL;
    }

    emit_duplicate_key_warnings();

    strata::JsonCursor cursor(result.value.root());
    auto result_values = strata::eval_search_path(cursor, compiled_path);
    return json_value_list_to_python(result_values);
}

bool process_ndjson_line(std::string_view line, size_t line_no,
                         const strata::CompiledPath& compiled_path,
                         const SimpleFieldExtractionSpec* simple_spec, NdjsonErrorMode mode,
                         PyObject* results, strata::ParseSaxOptions& options,
                         strata::ParseSaxContext& parse_context,
                         std::vector<strata::JsonValue>& fused_matches) {
    if (line.empty() || is_whitespace_only(line)) {
        return true;
    }

    if (simple_spec && simple_spec->enabled) {
        fused_matches.clear();
        auto status = strata::extract_simple_field_matches(line, simple_spec->field,
                                                           simple_spec->mode, fused_matches,
                                                           options, &parse_context);
        if (status != strata::Status::Ok) {
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

        if (fused_matches.empty()) {
            return true;
        }
        return append_ndjson_match(results, line_no, fused_matches);
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
    auto matches = strata::eval_search_path(cursor, compiled_path);
    if (matches.empty()) {
        return true;
    }
    return append_ndjson_match(results, line_no, matches);
}

bool process_ndjson_text(const char* data, Py_ssize_t len, const strata::CompiledPath& compiled_path,
                         const SimpleFieldExtractionSpec* simple_spec, NdjsonErrorMode mode,
                         PyObject* results, strata::ParseSaxOptions& options,
                         strata::ParseSaxContext& parse_context,
                         std::vector<strata::JsonValue>& fused_matches) {
    if (!data || len <= 0) {
        return true;
    }
    Py_ssize_t start = 0;
    size_t line_no = 0;
    while (start < len) {
        Py_ssize_t end = start;
        while (end < len && data[end] != '\n') {
            ++end;
        }
        std::string_view line(data + start, static_cast<size_t>(end - start));
        line_no++;
        if (!line.empty() && line.back() == '\r') {
            line.remove_suffix(1);
        }
        if (!process_ndjson_line(line, line_no, compiled_path, simple_spec, mode, results,
                                 options, parse_context, fused_matches)) {
            return false;
        }
        start = end + 1;
    }
    return true;
}

bool process_ndjson_iterable(PyObject* data_obj, const strata::CompiledPath& compiled_path,
                             const SimpleFieldExtractionSpec* simple_spec, NdjsonErrorMode mode,
                             PyObject* results, strata::ParseSaxOptions& options,
                             strata::ParseSaxContext& parse_context,
                             std::vector<strata::JsonValue>& fused_matches) {
    PyObject* iter = PyObject_GetIter(data_obj);
    if (!iter) {
        return false;
    }
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
                return false;
            }
            view = std::string_view(data, static_cast<size_t>(len));
        } else if (PyUnicode_Check(line_obj)) {
            Py_ssize_t len = 0;
            const char* data = PyUnicode_AsUTF8AndSize(line_obj, &len);
            if (!data) {
                Py_DECREF(line_obj);
                Py_DECREF(iter);
                return false;
            }
            view = std::string_view(data, static_cast<size_t>(len));
        } else {
            Py_DECREF(line_obj);
            Py_DECREF(iter);
            PyErr_SetString(PyExc_TypeError, "NDJSON lines must be str or bytes");
            return false;
        }
        view = trim_line_endings(view);
        bool ok = process_ndjson_line(view, line_no, compiled_path, simple_spec, mode, results,
                                      options, parse_context, fused_matches);
        Py_DECREF(line_obj);
        if (!ok) {
            Py_DECREF(iter);
            return false;
        }
    }
    Py_DECREF(iter);
    if (PyErr_Occurred()) {
        return false;
    }
    return true;
}

PyObject* search_ndjson_data(PyObject* data_obj, const strata::CompiledPath& compiled_path,
                             NdjsonErrorMode mode, bool treat_string_as_text,
                             bool parallel_set, bool parallel, int num_threads) {
    PyObject* results = PyList_New(0);
    if (!results) {
        return NULL;
    }

    strata::ParseSaxOptions options;
    strata::ParseSaxContext parse_context;
    std::vector<strata::JsonValue> fused_matches;

    SimpleFieldExtractionSpec simple_spec;
    bool use_fused = fused_ndjson_enabled() && compiled_path.is_simple_field_extraction();
    if (use_fused) {
        simple_spec = get_simple_field_spec(compiled_path);
        if (!simple_spec.enabled) {
            use_fused = false;
        }
    }
    const SimpleFieldExtractionSpec* simple_spec_ptr = use_fused ? &simple_spec : nullptr;

    if (treat_string_as_text) {
        if (PyUnicode_Check(data_obj) || PyBytes_Check(data_obj) || PyByteArray_Check(data_obj) ||
            PyMemoryView_Check(data_obj)) {
            PyObject* bytes_obj = data_obj;
            PyObject* temp_bytes = NULL;
            const char* data = nullptr;
            Py_ssize_t len = 0;
            if (PyUnicode_Check(data_obj)) {
                data = PyUnicode_AsUTF8AndSize(data_obj, &len);
                if (!data) {
                    Py_DECREF(results);
                    return NULL;
                }
            } else {
                if (!PyBytes_Check(data_obj)) {
                    temp_bytes = PyBytes_FromObject(data_obj);
                    if (!temp_bytes) {
                        Py_DECREF(results);
                        return NULL;
                    }
                    bytes_obj = temp_bytes;
                }
                if (PyBytes_AsStringAndSize(bytes_obj, const_cast<char**>(&data), &len) < 0) {
                    Py_XDECREF(temp_bytes);
                    Py_DECREF(results);
                    return NULL;
                }
            }

            bool use_parallel = false;
            size_t min_chunk_size = 0;
            if (parallel_set) {
                use_parallel = parallel;
            } else {
                size_t data_size = static_cast<size_t>(len);
                if (data_size >= kParallelMinSize) {
                    use_parallel = true;
                    size_t newline_count =
                        strata::util::count_newlines_simd(data, static_cast<size_t>(len));
                    size_t line_count = newline_count;
                    if (data_size > 0 && len > 0 && data[len - 1] != '\n') {
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

            if (use_parallel) {
                if (num_threads < 0) {
                    PyErr_SetString(PyExc_ValueError, "num_threads must be non-negative");
                    Py_XDECREF(temp_bytes);
                    Py_DECREF(results);
                    return NULL;
                }
                strata::ParallelNdjsonConfig config;
                config.skip_errors = (mode != NdjsonErrorMode::Error);
                config.num_threads = num_threads > 0 ? static_cast<size_t>(num_threads) : 0;
                if (min_chunk_size > 0) {
                    config.min_chunk_size = min_chunk_size;
                }
                strata::ParallelNdjsonStream stream(
                    std::string_view(data, static_cast<size_t>(len)), config);
                if (mode == NdjsonErrorMode::Warn || mode == NdjsonErrorMode::Error) {
                    auto search_result = stream.search_all_parallel_with_errors(compiled_path);
                    if (mode == NdjsonErrorMode::Error && !search_result.errors.empty()) {
                        std::string message =
                            "Invalid JSON on line " + std::to_string(search_result.errors[0].first);
                        PyErr_SetString(PyExc_ValueError, message.c_str());
                        Py_XDECREF(temp_bytes);
                        Py_DECREF(results);
                        return NULL;
                    }
                    if (mode == NdjsonErrorMode::Warn) {
                        for (const auto& err : search_result.errors) {
                            std::string message =
                                "Invalid JSON on line " + std::to_string(err.first);
                            if (PyErr_WarnEx(PyExc_RuntimeWarning, message.c_str(), 1) < 0) {
                                Py_XDECREF(temp_bytes);
                                Py_DECREF(results);
                                return NULL;
                            }
                        }
                    }
                    for (const auto& entry : search_result.matches) {
                        if (!append_ndjson_match(results, entry.line, entry.matches)) {
                            Py_XDECREF(temp_bytes);
                            Py_DECREF(results);
                            return NULL;
                        }
                    }
                } else {
                    auto matches = stream.search_all_parallel(compiled_path);
                    for (const auto& entry : matches) {
                        if (!append_ndjson_match(results, entry.line, entry.matches)) {
                            Py_XDECREF(temp_bytes);
                            Py_DECREF(results);
                            return NULL;
                        }
                    }
                }
                Py_XDECREF(temp_bytes);
                return results;
            }

            bool ok = process_ndjson_text(data, len, compiled_path, simple_spec_ptr, mode, results,
                                          options, parse_context, fused_matches);
            Py_XDECREF(temp_bytes);
            if (!ok) {
                Py_DECREF(results);
                return NULL;
            }
            return results;
        }
    }

    PyObject* pathlike = PyOS_FSPath(data_obj);
    if (pathlike) {
        const char* filepath = nullptr;
        if (PyUnicode_Check(pathlike)) {
            filepath = PyUnicode_AsUTF8(pathlike);
        } else if (PyBytes_Check(pathlike)) {
            filepath = PyBytes_AsString(pathlike);
        } else {
            Py_DECREF(pathlike);
            Py_DECREF(results);
            PyErr_SetString(PyExc_TypeError, "path must be str or bytes");
            return NULL;
        }
        if (!filepath) {
            Py_DECREF(pathlike);
            Py_DECREF(results);
            return NULL;
        }

        bool use_parallel = false;
        if (parallel_set) {
            use_parallel = parallel;
        } else {
            std::ifstream size_file(filepath, std::ios::binary | std::ios::ate);
            if (!size_file) {
                Py_DECREF(pathlike);
                Py_DECREF(results);
                PyErr_SetString(PyExc_OSError, "Failed to open NDJSON file");
                return NULL;
            }
            std::streamsize size = size_file.tellg();
            if (size >= 0 && static_cast<size_t>(size) >= kParallelMinSize) {
                use_parallel = true;
            }
        }

        if (use_parallel) {
            if (num_threads < 0) {
                Py_DECREF(pathlike);
                Py_DECREF(results);
                PyErr_SetString(PyExc_ValueError, "num_threads must be non-negative");
                return NULL;
            }
            std::ifstream file(filepath, std::ios::binary | std::ios::ate);
            if (!file) {
                Py_DECREF(pathlike);
                Py_DECREF(results);
                PyErr_SetString(PyExc_OSError, "Failed to open NDJSON file");
                return NULL;
            }
            std::streamsize size = file.tellg();
            if (size < 0) {
                Py_DECREF(pathlike);
                Py_DECREF(results);
                PyErr_SetString(PyExc_OSError, "Failed to read NDJSON file size");
                return NULL;
            }
            file.seekg(0, std::ios::beg);
            std::string data(static_cast<size_t>(size), '\0');
            if (!file.read(&data[0], size)) {
                Py_DECREF(pathlike);
                Py_DECREF(results);
                PyErr_SetString(PyExc_OSError, "Failed to read NDJSON file");
                return NULL;
            }

            size_t data_size = data.size();
            size_t min_chunk_size = 0;
            size_t newline_count = strata::util::count_newlines_simd(data.data(), data_size);
            size_t line_count = newline_count;
            if (data_size > 0 && data[data_size - 1] != '\n') {
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

            strata::ParallelNdjsonConfig config;
            config.skip_errors = (mode != NdjsonErrorMode::Error);
            config.num_threads = num_threads > 0 ? static_cast<size_t>(num_threads) : 0;
            if (min_chunk_size > 0) {
                config.min_chunk_size = min_chunk_size;
            }
            strata::ParallelNdjsonStream stream(std::string_view(data), config);
            if (mode == NdjsonErrorMode::Warn || mode == NdjsonErrorMode::Error) {
                auto search_result = stream.search_all_parallel_with_errors(compiled_path);
                if (mode == NdjsonErrorMode::Error && !search_result.errors.empty()) {
                    std::string message =
                        "Invalid JSON on line " + std::to_string(search_result.errors[0].first);
                    PyErr_SetString(PyExc_ValueError, message.c_str());
                    Py_DECREF(pathlike);
                    Py_DECREF(results);
                    return NULL;
                }
                if (mode == NdjsonErrorMode::Warn) {
                    for (const auto& err : search_result.errors) {
                        std::string message =
                            "Invalid JSON on line " + std::to_string(err.first);
                        if (PyErr_WarnEx(PyExc_RuntimeWarning, message.c_str(), 1) < 0) {
                            Py_DECREF(pathlike);
                            Py_DECREF(results);
                            return NULL;
                        }
                    }
                }
                for (const auto& entry : search_result.matches) {
                    if (!append_ndjson_match(results, entry.line, entry.matches)) {
                        Py_DECREF(pathlike);
                        Py_DECREF(results);
                        return NULL;
                    }
                }
            } else {
                auto matches = stream.search_all_parallel(compiled_path);
                for (const auto& entry : matches) {
                    if (!append_ndjson_match(results, entry.line, entry.matches)) {
                        Py_DECREF(pathlike);
                        Py_DECREF(results);
                        return NULL;
                    }
                }
            }
            Py_DECREF(pathlike);
            return results;
        }

        std::ifstream file(filepath, std::ios::binary);
        if (!file) {
            Py_DECREF(pathlike);
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
            if (!process_ndjson_line(view, line_no, compiled_path, simple_spec_ptr, mode, results,
                                     options, parse_context, fused_matches)) {
                Py_DECREF(pathlike);
                Py_DECREF(results);
                return NULL;
            }
        }
        if (file.bad()) {
            Py_DECREF(pathlike);
            Py_DECREF(results);
            PyErr_SetString(PyExc_OSError, "Error reading NDJSON file");
            return NULL;
        }
        Py_DECREF(pathlike);
        return results;
    }
    PyErr_Clear();

    if (!process_ndjson_iterable(data_obj, compiled_path, simple_spec_ptr, mode, results, options,
                                 parse_context, fused_matches)) {
        Py_DECREF(results);
        return NULL;
    }
    return results;
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

    auto result = strata::compile_search_path(path_str);
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

    auto result_values = strata::eval_search_path(*cursor_ptr, *self->path);
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

    auto result = strata::compile_search_path(path_str);
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
    PyObject* ndjson_obj = Py_None;
    int skip_errors = 0;
    PyObject* on_error_obj = Py_None;
    PyObject* parallel_obj = Py_None;
    int num_threads = 0;

    static const char* kwlist[] = {"data",      "path",       "strata_mode", "ndjson",
                                   "skip_errors", "on_error", "parallel",    "num_threads",
                                   NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|OOpOOi", const_cast<char**>(kwlist),
                                     &data_obj, &path_obj, &mode_obj, &ndjson_obj, &skip_errors,
                                     &on_error_obj, &parallel_obj, &num_threads)) {
        return NULL;
    }

    STRATA_CPP_TRY

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
    if (num_threads < 0) {
        PyErr_SetString(PyExc_ValueError, "num_threads must be non-negative");
        return NULL;
    }

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

        auto compile_result = strata::compile_search_path(path_str);
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

    if (ndjson_flag_set && !ndjson_flag && (skip_errors || on_error_obj != Py_None)) {
        PyErr_SetString(PyExc_TypeError, "ndjson=False cannot be used with skip_errors/on_error");
        return NULL;
    }

    if (mode != StrataSearchMode::Auto) {
        if ((ndjson_flag_set && ndjson_flag) || skip_errors || on_error_obj != Py_None) {
            PyErr_SetString(PyExc_TypeError, "ndjson options are only supported in auto mode");
            return NULL;
        }
    }

    if (mode == StrataSearchMode::Dict) {
        return search_dict_mode(data_obj, compiled_path);
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

    if (mode == StrataSearchMode::File) {
        PyObject* pathlike = PyOS_FSPath(data_obj);
        if (!pathlike) {
            return NULL;
        }
        PyObject* result = search_file_pathlike(pathlike, compiled_path);
        Py_DECREF(pathlike);
        return result;
    }

    bool has_text = false;
    bool looks_like_json = false;
    if (PyUnicode_Check(data_obj)) {
        Py_ssize_t text_len = 0;
        const char* text = PyUnicode_AsUTF8AndSize(data_obj, &text_len);
        if (!text) {
            return NULL;
        }
        has_text = true;
        looks_like_json = looks_like_json_text(text, text_len);
    } else if (PyBytes_Check(data_obj)) {
        char* text = nullptr;
        Py_ssize_t text_len = 0;
        if (PyBytes_AsStringAndSize(data_obj, &text, &text_len) < 0) {
            return NULL;
        }
        has_text = true;
        looks_like_json = looks_like_json_text(text, text_len);
    }

    if (PyDict_Check(data_obj) || PyList_Check(data_obj)) {
        if ((ndjson_flag_set && ndjson_flag) || skip_errors || on_error_obj != Py_None) {
            PyErr_SetString(PyExc_TypeError, "ndjson options are only supported for NDJSON search");
            return NULL;
        }
        return search_dict_mode(data_obj, compiled_path);
    }

    int has_cursor_attr = PyObject_HasAttrString(data_obj, "_cursor");
    if (has_cursor_attr < 0) {
        return NULL;
    }
    if (is_py_json_document(data_obj) || is_py_json_cursor(data_obj) || is_py_ndjson_cursor(data_obj) ||
        has_cursor_attr) {
        return search_cursor_mode(data_obj, compiled_path);
    }

    bool ndjson_requested = false;
    if (ndjson_flag_set) {
        ndjson_requested = ndjson_flag;
    } else if (skip_errors || on_error_obj != Py_None) {
        ndjson_requested = true;
    }

    if (!ndjson_flag_set && !ndjson_requested) {
        bool ndjson_by_name = detect_ndjson_name_attr(data_obj);
        if (PyErr_Occurred()) {
            return NULL;
        }
        bool ndjson_by_extension = ndjson_by_name;
        if (!ndjson_by_extension) {
            bool check_pathlike = !has_text || !looks_like_json;
            if (check_pathlike) {
                PyObject* pathlike = PyOS_FSPath(data_obj);
                if (pathlike) {
                    ndjson_by_extension = is_ndjson_name(pathlike);
                    Py_DECREF(pathlike);
                    if (PyErr_Occurred()) {
                        return NULL;
                    }
                } else {
                    PyErr_Clear();
                }
            }
        }
        ndjson_requested = ndjson_by_extension;
    }

    if (ndjson_requested) {
        NdjsonErrorMode ndjson_mode;
        if (!parse_ndjson_error_mode(skip_errors, on_error_obj, &ndjson_mode)) {
            return NULL;
        }
        bool treat_string_as_text = has_text && looks_like_json;
        return search_ndjson_data(data_obj, compiled_path, ndjson_mode, treat_string_as_text,
                                  parallel_set, parallel, num_threads);
    }

    if (skip_errors || on_error_obj != Py_None) {
        PyErr_SetString(PyExc_TypeError, "on_error is only supported for NDJSON search");
        return NULL;
    }

    if (!has_text) {
        PyObject* pathlike = PyOS_FSPath(data_obj);
        if (pathlike) {
            PyObject* result = search_file_pathlike(pathlike, compiled_path);
            Py_DECREF(pathlike);
            return result;
        }
        PyErr_Clear();
    }

    bool check_pathlike = has_text && !looks_like_json;
    if (check_pathlike) {
        PyObject* pathlike = PyOS_FSPath(data_obj);
        if (pathlike) {
            PyObject* result = search_file_pathlike(pathlike, compiled_path);
            Py_DECREF(pathlike);
            return result;
        }
        PyErr_Clear();
    }

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

    int has_read = PyObject_HasAttrString(data_obj, "read");
    if (has_read < 0) {
        return NULL;
    }
    if (has_read) {
        return search_from_file_like(data_obj, compiled_path);
    }

    PyErr_SetString(PyExc_TypeError,
                    "search() expects JSON text (str/bytes), dict/list data, a "
                    "JsonDocument/JsonCursor/NdjsonCursor, or a file-like object");
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
        auto compile_result = strata::compile_search_path(path_str);
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
    std::vector<strata::JsonValue> fused_matches;

    SimpleFieldExtractionSpec simple_spec;
    bool use_fused = fused_ndjson_enabled() && compiled_path.is_simple_field_extraction();
    if (use_fused) {
        simple_spec = get_simple_field_spec(compiled_path);
        if (!simple_spec.enabled) {
            use_fused = false;
        }
    }
    const SimpleFieldExtractionSpec* simple_spec_ptr = use_fused ? &simple_spec : nullptr;

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
            if (!process_ndjson_line(view, line_no, compiled_path, simple_spec_ptr, mode, results,
                                     options, parse_context, fused_matches)) {
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
            bool ok = process_ndjson_line(view, line_no, compiled_path, simple_spec_ptr, mode,
                                          results, options, parse_context, fused_matches);
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
        bool ok = process_ndjson_line(view, line_no, compiled_path, simple_spec_ptr, mode, results,
                                      options, parse_context, fused_matches);
        Py_DECREF(line_obj);
        if (!ok) {
            Py_DECREF(results);
            return NULL;
        }
    }

    return results;

    STRATA_CPP_CATCH
}

PyObject* strata_query(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* data_obj;
    PyObject* path_obj;

    static const char* kwlist[] = {"data", "path", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO", const_cast<char**>(kwlist), &data_obj,
                                     &path_obj)) {
        return NULL;
    }

    if (!PyDict_Check(data_obj) && !PyList_Check(data_obj)) {
        PyErr_SetString(PyExc_TypeError, "query() expects a dict or list");
        return NULL;
    }

    STRATA_CPP_TRY

    strata::CompiledPath compiled_path;
    if (PyUnicode_Check(path_obj)) {
        const char* path_str = PyUnicode_AsUTF8(path_obj);
        if (!path_str) {
            return NULL;
        }
        auto compile_result = strata::compile_search_path(path_str);
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

    return search_dict_mode(data_obj, compiled_path);

    STRATA_CPP_CATCH
}

//=============================================================================
// Module Registration
//=============================================================================

int register_search_types(PyObject* module) {
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
// No need for get_search_methods() here

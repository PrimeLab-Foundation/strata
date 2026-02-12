#ifndef STRATA_PYTHON_NDJSON_H
#define STRATA_PYTHON_NDJSON_H

#include "strata/json/json_core.hpp"

#include <Python.h>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace strata {
namespace bindings {

enum class NdjsonCursorErrorMode : uint8_t {
    Skip = 0,
    Warn = 1,
    Error = 2,
};

struct NdjsonCursorData {
    std::vector<strata::JsonValue> values;
    std::vector<size_t> line_numbers;
    bool lazy = false;
    NdjsonCursorErrorMode error_mode = NdjsonCursorErrorMode::Error;
    std::vector<size_t> line_offsets;
    std::vector<size_t> data_line_offsets;
    std::vector<size_t> data_line_numbers;
    const char* data_ptr = nullptr;
    size_t data_size = 0;
    std::shared_ptr<void> mapped;
    size_t parsed_lines = 0;
};

} // namespace bindings
} // namespace strata

bool is_py_ndjson_cursor(PyObject* obj);
strata::bindings::NdjsonCursorData* get_py_ndjson_cursor(PyObject* obj);

strata::bindings::NdjsonCursorData* create_lazy_ndjson_cursor_data(
    const char* filepath,
    strata::bindings::NdjsonCursorErrorMode mode,
    std::string* error_message);
PyObject* create_py_ndjson_cursor(strata::bindings::NdjsonCursorData* cursor_data);

#endif // STRATA_PYTHON_NDJSON_H

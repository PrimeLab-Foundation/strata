#ifndef STRATA_PYTHON_NDJSON_H
#define STRATA_PYTHON_NDJSON_H

#include "strata/json/json_core.hpp"

#include <Python.h>
#include <cstddef>
#include <vector>

namespace strata {
namespace bindings {

struct NdjsonCursorData {
    std::vector<strata::JsonValue> values;
    std::vector<size_t> line_numbers;
};

} // namespace bindings
} // namespace strata

bool is_py_ndjson_cursor(PyObject* obj);
strata::bindings::NdjsonCursorData* get_py_ndjson_cursor(PyObject* obj);

#endif // STRATA_PYTHON_NDJSON_H

#ifndef STRATA_PYTHON_CONVERT_H
#define STRATA_PYTHON_CONVERT_H

#define PY_SSIZE_T_CLEAN
#include "python_types.h"
#include "strata/json/json_core.hpp"
#include "strata/json/json_cursor.hpp"

#include <Python.h>
#include <vector>

/** Convert JsonValue to PyObject (defined in python_loads.cpp; used by document, search, ndjson).
 */
PyObject* json_value_to_python(const strata::JsonValue& val);

/** Optimized list conversion (defined in python_loads.cpp).
 * Reuses KeyCache and Arena across all items for significantly better performance.
 */
PyObject* json_value_list_to_python(const std::vector<strata::JsonValue>& values);

/** Convert vector of JsonCursor to Python list.
 * Avoids double materialization and reuses KeyCache.
 */
PyObject* json_cursor_list_to_python(const std::vector<strata::JsonCursor>& cursors);

#endif // STRATA_PYTHON_CONVERT_H

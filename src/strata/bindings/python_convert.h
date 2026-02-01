#ifndef STRATA_PYTHON_CONVERT_H
#define STRATA_PYTHON_CONVERT_H

#define PY_SSIZE_T_CLEAN
#include "python_types.h"
#include "strata/json/json_core.hpp"

#include <Python.h>

/** Convert JsonValue to PyObject (defined in python_loads.cpp; used by document, jsonpath, ndjson).
 */
PyObject* json_value_to_python(const strata::JsonValue& val);

/** Convert vector of JsonValue to Python list. Inline so callers (jsonpath, ndjson, loads) get
 * inlining without LTO. */
inline PyObject* json_value_list_to_python(const std::vector<strata::JsonValue>& values) {
    PyGcPause gc_pause;
    PyObject* list = PyList_New(values.size());
    if (!list)
        return NULL;
    for (size_t i = 0; i < values.size(); ++i) {
        PyObject* item = json_value_to_python(values[i]);
        if (!item) {
            Py_DECREF(list);
            return NULL;
        }
        PyList_SET_ITEM(list, i, item);
    }
    return list;
}

#endif // STRATA_PYTHON_CONVERT_H

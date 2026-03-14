#ifndef STRATA_PYTHON_CONVERT_H
#define STRATA_PYTHON_CONVERT_H

#define PY_SSIZE_T_CLEAN
#include "python_types.h"
#include "strata/json/json_core.hpp"

#include <Python.h>

namespace strata {
class NdjsonStream;
}

/** Convert JsonValue to PyObject (defined in python_loads.cpp; used by document, jsonpath, ndjson).
 */
PyObject* json_value_to_python(const strata::JsonValue& val);

#ifdef __cplusplus
#include <string_view>
/** Parse JSON text directly to a Python object via SAX (no intermediate C++ DOM).
 *  Defined in python_loads.cpp. validate_utf8=false skips the SIMD pre-scan; safe
 *  when PyUnicode creation will validate string content anyway.
 */
PyObject* parse_json_to_python(std::string_view text, bool validate_utf8 = true);

/** Parse all remaining lines of an NdjsonStream into a Python list, reusing a single
 *  PythonObjectBuilder (and its KeyCache) across lines for reduced allocations.
 *  GC is suspended for the duration. Defined in python_loads.cpp.
 */
PyObject* parse_ndjson_all_to_python(strata::NdjsonStream& stream, int skip_errors);

/** Parse up to batch_size lines from an NdjsonStream into a Python list, reusing a
 *  single PythonObjectBuilder (and its KeyCache) across lines. GC is suspended.
 *  Defined in python_loads.cpp.
 */
PyObject* parse_ndjson_batch_to_python(strata::NdjsonStream& stream, Py_ssize_t batch_size,
                                       int skip_errors);
/** Python-exposed parse_ndjson(text, skip_errors=False) -> list.
 *  Defined in python_loads.cpp. */
PyObject* strata_parse_ndjson(PyObject* self, PyObject* args);
#endif

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

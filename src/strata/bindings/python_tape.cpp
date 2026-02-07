/**
 * python_tape.cpp - Python object builder from tape format
 *
 * This file provides TapeToPythonBuilder, which reads from a JsonTape
 * and builds Python objects directly. This enables efficient tape-based
 * parsing for Python when repeated access patterns are needed.
 */

#include "python_convert.h"
#include "python_types.h"
#include "strata/json/json_tape.hpp"
#include "strata/json/json_parse.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace strata {
namespace bindings {

/**
 * Builds Python objects from a JsonTape.
 *
 * This enables two-phase parsing:
 * 1. Parse JSON to tape (fast, minimal allocation)
 * 2. Build Python objects from tape (on-demand)
 *
 * The tape can be reused for multiple Python object builds or queries.
 */
class TapeToPythonBuilder {
  public:
    explicit TapeToPythonBuilder(const JsonTape& tape) : tape_(tape), it_(tape.begin()) {}

    PyObject* build() {
        if (tape_.empty()) {
            PyErr_SetString(PyExc_ValueError, "Cannot build from empty tape");
            return nullptr;
        }

        PyObject* result = build_value();
        if (result && it_.token() != TapeToken::RootEnd) {
            Py_DECREF(result);
            PyErr_SetString(PyExc_ValueError, "Extra data after JSON value");
            return nullptr;
        }
        return result;
    }

  private:
    PyObject* build_value() {
        TapeToken tok = it_.token();

        switch (tok) {
            case TapeToken::Null:
                ++it_;
                Py_RETURN_NONE;

            case TapeToken::True:
                ++it_;
                Py_RETURN_TRUE;

            case TapeToken::False:
                ++it_;
                Py_RETURN_FALSE;

            case TapeToken::Int64: {
                int64_t v = it_.as_int64();
                ++it_;
                // Use PyLong for better precision than float
                if (v >= LONG_MIN && v <= LONG_MAX) {
                    return PyLong_FromLong(static_cast<long>(v));
                }
                return PyLong_FromLongLong(v);
            }

            case TapeToken::Uint64: {
                uint64_t v = it_.as_uint64();
                ++it_;
                if (v <= static_cast<uint64_t>(LONG_MAX)) {
                    return PyLong_FromLong(static_cast<long>(v));
                }
                return PyLong_FromUnsignedLongLong(v);
            }

            case TapeToken::Double: {
                double v = it_.as_double();
                ++it_;
                return PyFloat_FromDouble(v);
            }

            case TapeToken::String: {
                std::string_view sv = it_.as_string();
                ++it_;
                return PyUnicode_FromStringAndSize(sv.data(), sv.size());
            }

            case TapeToken::StartObject:
                return build_object();

            case TapeToken::StartArray:
                return build_array();

            case TapeToken::Key:
            case TapeToken::EndObject:
            case TapeToken::EndArray:
            case TapeToken::RootEnd:
                PyErr_SetString(PyExc_ValueError, "Unexpected token in tape");
                return nullptr;
        }

        PyErr_SetString(PyExc_ValueError, "Unknown token in tape");
        return nullptr;
    }

    PyObject* build_object() {
        size_t count = it_.container_size();
        ++it_;  // Consume StartObject

        PyObject* dict = PyDict_New();
        if (!dict) return nullptr;

        for (size_t i = 0; i < count; ++i) {
            // Expect Key token
            if (it_.token() != TapeToken::Key) {
                Py_DECREF(dict);
                PyErr_SetString(PyExc_ValueError, "Expected key token in object");
                return nullptr;
            }
            std::string_view key = it_.as_string();
            ++it_;  // Consume Key

            // Build key string
            PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
            if (!py_key) {
                Py_DECREF(dict);
                return nullptr;
            }

            // Build value
            PyObject* py_val = build_value();
            if (!py_val) {
                Py_DECREF(py_key);
                Py_DECREF(dict);
                return nullptr;
            }

            // Add to dict
            int rc = PyDict_SetItem(dict, py_key, py_val);
            Py_DECREF(py_key);
            Py_DECREF(py_val);

            if (rc < 0) {
                Py_DECREF(dict);
                return nullptr;
            }
        }

        // Expect EndObject token
        if (it_.token() != TapeToken::EndObject) {
            Py_DECREF(dict);
            PyErr_SetString(PyExc_ValueError, "Expected end of object token");
            return nullptr;
        }
        ++it_;  // Consume EndObject

        return dict;
    }

    PyObject* build_array() {
        size_t count = it_.container_size();
        ++it_;  // Consume StartArray

        PyObject* list = PyList_New(count);
        if (!list) return nullptr;

        for (size_t i = 0; i < count; ++i) {
            PyObject* item = build_value();
            if (!item) {
                Py_DECREF(list);
                return nullptr;
            }
            PyList_SET_ITEM(list, i, item);  // Steals reference
        }

        // Expect EndArray token
        if (it_.token() != TapeToken::EndArray) {
            Py_DECREF(list);
            PyErr_SetString(PyExc_ValueError, "Expected end of array token");
            return nullptr;
        }
        ++it_;  // Consume EndArray

        return list;
    }

    const JsonTape& tape_;
    JsonTape::Iterator it_;
};

}  // namespace bindings
}  // namespace strata

// Build Python object from tape
PyObject* tape_to_python(const strata::JsonTape& tape) {
    strata::bindings::TapeToPythonBuilder builder(tape);
    return builder.build();
}

// Python loads_tape() function - parses via tape for demonstration
PyObject* strata_loads_tape(PyObject* self, PyObject* args) {
    const char* data;
    Py_ssize_t len;

    // Parse arguments
    if (!PyArg_ParseTuple(args, "s#", &data, &len)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Parse to tape
    auto tape_result = strata::parse_to_tape(std::string_view(data, len));
    if (tape_result.status != strata::Status::Ok) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        return NULL;
    }

    // Build Python object from tape
    return tape_to_python(tape_result.value);

    STRATA_CPP_CATCH
}

#include "python_types.h"
#include "strata/util/dragonbox.hpp"
#include "strata/util/output_buffer.hpp"
#include "strata/util/simd_string.hpp"

#include <charconv>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// Thread-local buffer for zero-allocation serialization
thread_local strata::util::OutputBuffer g_serialize_buffer;

enum class CyclePolicy { Warn, Error, Ignore };
static CyclePolicy g_cycle_policy = CyclePolicy::Ignore;

bool set_cycle_policy_from_string(const char* policy, std::string& error) {
    if (policy == nullptr) {
        error = "cycle policy must be a string";
        return false;
    }
    if (strcmp(policy, "warn") == 0) {
        g_cycle_policy = CyclePolicy::Warn;
        return true;
    }
    if (strcmp(policy, "error") == 0) {
        g_cycle_policy = CyclePolicy::Error;
        return true;
    }
    if (strcmp(policy, "ignore") == 0) {
        g_cycle_policy = CyclePolicy::Ignore;
        return true;
    }
    error = "unknown cycle policy (expected warn|error|ignore)";
    return false;
}

int strata_get_cycle_policy() { return static_cast<int>(g_cycle_policy); }

template <typename Buffer>
static inline void append_literal(Buffer& out, const char* data, size_t len) {
    out.append(data, len);
}

template <typename Buffer> static inline bool append_int64(Buffer& out, int64_t value) {
    static const char kDigitPairs[] = "00010203040506070809"
                                      "10111213141516171819"
                                      "20212223242526272829"
                                      "30313233343536373839"
                                      "40414243444546474849"
                                      "50515253545556575859"
                                      "60616263646566676869"
                                      "70717273747576777879"
                                      "80818283848586878889"
                                      "90919293949596979899";

    // Reserve 8 bytes for the fast path (max 7 chars: -999999).
    // Large ints reserve separately.
    out.reserve(out.size() + 8);

    if (LIKELY(value >= -999999 && value <= 999999)) {
        uint32_t v;
        if (value < 0) {
            out.unsafe_push_back('-');
            v = static_cast<uint32_t>(-value);
        } else {
            v = static_cast<uint32_t>(value);
        }

        if (v < 10) {
            out.unsafe_push_back(static_cast<char>('0' + v));
            return true;
        }
        if (v < 100) {
            out.unsafe_append(kDigitPairs + v * 2, 2);
            return true;
        }

        // 3-6 digit numbers: decompose into pairs
        if (v < 10000) {
            uint32_t high = v / 100;
            uint32_t low = v - high * 100;
            if (high < 10) {
                out.unsafe_push_back(static_cast<char>('0' + high));
            } else {
                out.unsafe_append(kDigitPairs + high * 2, 2);
            }
            out.unsafe_append(kDigitPairs + low * 2, 2);
        } else {
            // 5-6 digit numbers
            uint32_t top = v / 10000;
            uint32_t rem = v - top * 10000;
            uint32_t mid = rem / 100;
            uint32_t low = rem - mid * 100;
            if (top < 10) {
                out.unsafe_push_back(static_cast<char>('0' + top));
            } else {
                out.unsafe_append(kDigitPairs + top * 2, 2);
            }
            out.unsafe_append(kDigitPairs + mid * 2, 2);
            out.unsafe_append(kDigitPairs + low * 2, 2);
        }
        return true;
    }

    // Large ints: need up to 21 bytes (sign + 20 digits)
    out.reserve(out.size() + 21);
    char* start = out.data() + out.size();
    auto result = std::to_chars(start, start + 21, value);
    if (UNLIKELY(result.ec != std::errc())) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to format integer");
        return false;
    }
    out.unsafe_advance(static_cast<size_t>(result.ptr - start));
    return true;
}

template <typename Buffer> static inline bool append_py_long(Buffer& out, PyObject* obj) {
#ifdef PyUnstable_Long_IsCompact
    if (PyUnstable_Long_IsCompact(reinterpret_cast<PyLongObject*>(obj))) {
        Py_ssize_t val = PyUnstable_Long_CompactValue(reinterpret_cast<PyLongObject*>(obj));
        return append_int64(out, static_cast<int64_t>(val));
    }
#endif

    int overflow = 0;
    int64_t val = PyLong_AsLongLongAndOverflow(obj, &overflow);
    if (overflow == 0) {
        return append_int64(out, val);
    }

    PyObject* str_obj = PyObject_Str(obj);
    if (!str_obj) {
        return false;
    }
    Py_ssize_t len = 0;
    const char* data = PyUnicode_AsUTF8AndSize(str_obj, &len);
    if (!data) {
        Py_DECREF(str_obj);
        return false;
    }
    out.append(data, static_cast<size_t>(len));
    Py_DECREF(str_obj);
    return true;
}

template <typename Buffer> static inline bool append_double(Buffer& out, double value) {
    if (UNLIKELY(std::isnan(value) || std::isinf(value))) {
        out.append("null", 4);
        return true;
    }

    // Fast path: integer-valued doubles (very common in JSON data).
    // Avoid std::to_chars entirely — reuse the fast integer formatter.
    // Exclude -0.0 (signbit set, but ival==0 would lose the sign).
    if (value >= -999999999999999.0 && value <= 999999999999999.0) {
        int64_t ival = static_cast<int64_t>(value);
        if (static_cast<double>(ival) == value && LIKELY(ival != 0 || !std::signbit(value))) {
            append_int64(out, ival);
            out.append(".0", 2);
            return true;
        }
    }

    // Write directly into output buffer to avoid extra memcpy
    out.reserve(out.size() + 32);
    int len = strata::util::dragonbox_d2s(value, out.data() + out.size());
    out.unsafe_advance(static_cast<size_t>(len));
    return true;
}

template <typename Buffer> static inline bool append_string(PyObject* obj, Buffer& out) {
    if (PyUnicode_IS_COMPACT_ASCII(obj)) {
        Py_ssize_t len = PyUnicode_GET_LENGTH(obj);
        const char* data = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(obj));
        // Single-pass: escape_json_string_simd now handles clean strings efficiently
        // (finds first escape position; if none, does unsafe copy — no redundant scan).
        strata::util::escape_json_string_simd(data, static_cast<size_t>(len), out);
        return true;
    }

    Py_ssize_t len = 0;
    const char* data = PyUnicode_AsUTF8AndSize(obj, &len);
    if (!data) {
        return false;
    }

    strata::util::escape_json_string_simd(data, static_cast<size_t>(len), out);
    return true;
}

// Cached per-dumps() call to avoid repeated TLS lookup of Py_GetRecursionLimit().
static thread_local int g_max_depth = 1000;

static inline int get_max_serialize_depth() { return g_max_depth; }

// Cycle detection: thread-local vector of container object pointers.
// Only used when cycle_policy != Ignore.
thread_local std::vector<PyObject*> g_seen_stack;

// Check for cycle + push if not found. Returns true if cycle detected.
template <typename Buffer>
static inline bool check_and_push_cycle(PyObject* obj, Buffer& out, bool& handled_ok) {
    for (auto* seen : g_seen_stack) {
        if (seen == obj) {
            switch (g_cycle_policy) {
            case CyclePolicy::Warn:
                PyErr_WarnEx(PyExc_RuntimeWarning,
                             "Circular reference detected during serialization", 1);
                out.append("null", 4);
                handled_ok = true;
                return true;
            case CyclePolicy::Error:
                PyErr_SetString(PyExc_ValueError,
                                "Circular reference detected during serialization");
                handled_ok = false;
                return true;
            case CyclePolicy::Ignore:
                out.append("null", 4);
                handled_ok = true;
                return true;
            }
        }
    }
    g_seen_stack.push_back(obj);
    return false;
}

static inline void pop_seen() { g_seen_stack.pop_back(); }

// Estimate per-item size by sampling the first element of a list/dict value.
static inline size_t estimate_item_size(PyObject* item) {
    if (PyDict_CheckExact(item)) {
        Py_ssize_t dsz = PyDict_GET_SIZE(item);
        size_t est = static_cast<size_t>(dsz) * 48 + 2;
        // Peek one level deeper for nested lists
        if (dsz <= 8) {
            Py_ssize_t pos = 0;
            PyObject* k = nullptr;
            PyObject* v = nullptr;
            while (PyDict_Next(item, &pos, &k, &v)) {
                if (PyList_CheckExact(v)) {
                    est += static_cast<size_t>(PyList_GET_SIZE(v)) * 64;
                } else if (PyDict_CheckExact(v)) {
                    est += static_cast<size_t>(PyDict_GET_SIZE(v)) * 48;
                }
            }
        }
        return est;
    }
    if (PyList_CheckExact(item)) {
        return static_cast<size_t>(PyList_GET_SIZE(item)) * 64 + 2;
    }
    return 48;
}

static inline size_t estimate_size(PyObject* obj) {
    if (PyDict_CheckExact(obj)) {
        Py_ssize_t sz = PyDict_GET_SIZE(obj);
        size_t est = static_cast<size_t>(sz) * 48 + 2;
        // For small dicts (common top-level wrapper), peek at values to catch
        // large nested collections — avoids many buffer reallocations.
        if (sz <= 4) {
            Py_ssize_t pos = 0;
            PyObject* k = nullptr;
            PyObject* v = nullptr;
            while (PyDict_Next(obj, &pos, &k, &v)) {
                if (PyList_CheckExact(v)) {
                    Py_ssize_t list_sz = PyList_GET_SIZE(v);
                    // Sample first element for per-item estimate
                    size_t per_item = 128;
                    if (list_sz > 0) {
                        per_item = estimate_item_size(PyList_GET_ITEM(v, 0));
                        if (per_item < 128)
                            per_item = 128;
                    }
                    est += static_cast<size_t>(list_sz) * per_item;
                } else if (PyDict_CheckExact(v)) {
                    est += static_cast<size_t>(PyDict_GET_SIZE(v)) * 48;
                }
            }
        }
        return est;
    }
    if (PyList_CheckExact(obj)) {
        Py_ssize_t sz = PyList_GET_SIZE(obj);
        size_t per_item = 128;
        if (sz > 0) {
            per_item = estimate_item_size(PyList_GET_ITEM(obj, 0));
            if (per_item < 128)
                per_item = 128;
        }
        return static_cast<size_t>(sz) * per_item + 2;
    }
    if (PyTuple_Check(obj)) {
        return static_cast<size_t>(PyTuple_GET_SIZE(obj)) * 128 + 2;
    }
    if (PyUnicode_Check(obj)) {
        Py_ssize_t len = 0;
        const char* data = PyUnicode_AsUTF8AndSize(obj, &len);
        if (data && len > 0) {
            return static_cast<size_t>(len) + 2;
        }
    }
    return 256;
}

// Recursive serializer: uses the hardware call stack instead of an explicit frame stack.
// This reduces per-element overhead (no vector push/pop, no frame type dispatch) at the
// cost of ~128 bytes of stack per nesting level (bounded by sys.getrecursionlimit()).
template <typename Buffer>
static inline bool serialize_value(PyObject* val, Buffer& out, int depth);

// Write a dict key (compact ASCII fast path). Returns false on error.
template <typename Buffer>
static inline bool write_dict_key(PyObject* key, Buffer& out, bool first) {
    if (LIKELY(PyUnicode_IS_COMPACT_ASCII(key))) {
        const Py_ssize_t klen = PyUnicode_GET_LENGTH(key);
        const char* kdata = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(key));
        const size_t need = static_cast<size_t>(klen) + (first ? 3 : 4);
        out.reserve(out.size() + need);
        if (!first) {
            out.unsafe_push_back(',');
        }
        // Inline scalar escape check — keys are almost always short clean ASCII,
        // so a lookup-table loop is faster than SIMD function call overhead.
        bool key_clean = true;
        for (Py_ssize_t ki = 0; ki < klen; ++ki) {
            unsigned char c = static_cast<unsigned char>(kdata[ki]);
            if (UNLIKELY(c < 0x20 || c == '"' || c == '\\')) {
                key_clean = false;
                break;
            }
        }
        if (LIKELY(key_clean)) {
            out.unsafe_push_back('"');
            out.unsafe_append(kdata, static_cast<size_t>(klen));
            out.unsafe_push_back('"');
            out.unsafe_push_back(':');
        } else {
            strata::util::escape_json_string_simd(kdata, static_cast<size_t>(klen), out);
            out.push_back(':');
        }
        return true;
    }
    if (!first) {
        out.push_back(',');
    }
    if (!PyUnicode_Check(key)) {
        PyErr_SetString(PyExc_TypeError, "Dict keys must be strings");
        return false;
    }
    if (!append_string(key, out)) {
        return false;
    }
    out.push_back(':');
    return true;
}

template <typename Buffer>
static inline bool serialize_dict(PyObject* dict, Buffer& out, int depth) {
    bool tracking = UNLIKELY(g_cycle_policy != CyclePolicy::Ignore);
    if (tracking) {
        bool ok;
        if (check_and_push_cycle(dict, out, ok))
            return ok;
    }
    Py_ssize_t sz = PyDict_GET_SIZE(dict);
    if (sz == 0) {
        out.append("{}", 2);
        if (tracking)
            pop_seen();
        return true;
    }
    out.push_back('{');
    Py_ssize_t pos = 0;
    PyObject* key = nullptr;
    PyObject* value = nullptr;
    bool first = true;
    while (PyDict_Next(dict, &pos, &key, &value)) {
        if (!write_dict_key(key, out, first)) {
            if (tracking)
                pop_seen();
            return false;
        }
        first = false;
        PyTypeObject* vt = Py_TYPE(value);
        if (LIKELY(vt == &PyUnicode_Type)) {
            if (!append_string(value, out)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else if (LIKELY(vt == &PyLong_Type)) {
            if (!append_py_long(out, value)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else if (vt == &PyFloat_Type) {
            if (!append_double(out, PyFloat_AS_DOUBLE(value))) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else {
            if (!serialize_value(value, out, depth)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        }
    }
    out.push_back('}');
    if (tracking)
        pop_seen();
    return true;
}

template <typename Buffer>
static inline bool serialize_list(PyObject* list, Buffer& out, int depth) {
    bool tracking = UNLIKELY(g_cycle_policy != CyclePolicy::Ignore);
    if (tracking) {
        bool ok;
        if (check_and_push_cycle(list, out, ok))
            return ok;
    }
    Py_ssize_t sz = PyList_GET_SIZE(list);
    if (sz == 0) {
        out.append("[]", 2);
        if (tracking)
            pop_seen();
        return true;
    }
    out.push_back('[');
    // First item (no comma)
    {
        PyObject* item = PyList_GET_ITEM(list, 0);
        PyTypeObject* vt = Py_TYPE(item);
        if (LIKELY(vt == &PyUnicode_Type)) {
            if (!append_string(item, out)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else if (LIKELY(vt == &PyLong_Type)) {
            if (!append_py_long(out, item)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else if (vt == &PyFloat_Type) {
            if (!append_double(out, PyFloat_AS_DOUBLE(item))) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else if (vt == &PyDict_Type) {
            if (UNLIKELY(depth >= get_max_serialize_depth())) {
                PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
                if (tracking)
                    pop_seen();
                return false;
            }
            if (!serialize_dict(item, out, depth + 1)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else {
            if (!serialize_value(item, out, depth)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        }
    }
    // Remaining items (with comma prefix)
    for (Py_ssize_t i = 1; i < sz; ++i) {
        out.push_back(',');
        PyObject* item = PyList_GET_ITEM(list, i);
        PyTypeObject* vt = Py_TYPE(item);
        if (LIKELY(vt == &PyUnicode_Type)) {
            if (!append_string(item, out)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else if (LIKELY(vt == &PyLong_Type)) {
            if (!append_py_long(out, item)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else if (vt == &PyFloat_Type) {
            if (!append_double(out, PyFloat_AS_DOUBLE(item))) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else if (vt == &PyDict_Type) {
            if (UNLIKELY(depth >= get_max_serialize_depth())) {
                PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
                if (tracking)
                    pop_seen();
                return false;
            }
            if (!serialize_dict(item, out, depth + 1)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        } else {
            if (!serialize_value(item, out, depth)) {
                if (tracking)
                    pop_seen();
                return false;
            }
        }
    }
    out.push_back(']');
    if (tracking)
        pop_seen();
    return true;
}

template <typename Buffer>
static inline bool serialize_tuple(PyObject* tup, Buffer& out, int depth) {
    bool tracking = UNLIKELY(g_cycle_policy != CyclePolicy::Ignore);
    if (tracking) {
        bool ok;
        if (check_and_push_cycle(tup, out, ok))
            return ok;
    }
    Py_ssize_t sz = PyTuple_GET_SIZE(tup);
    if (sz == 0) {
        out.append("[]", 2);
        if (tracking)
            pop_seen();
        return true;
    }
    out.push_back('[');
    for (Py_ssize_t i = 0; i < sz; ++i) {
        if (i > 0) {
            out.push_back(',');
        }
        if (!serialize_value(PyTuple_GET_ITEM(tup, i), out, depth)) {
            if (tracking)
                pop_seen();
            return false;
        }
    }
    out.push_back(']');
    if (tracking)
        pop_seen();
    return true;
}

template <typename Buffer>
static inline bool serialize_value(PyObject* val, Buffer& out, int depth) {
    PyTypeObject* vt = Py_TYPE(val);
    if (LIKELY(vt == &PyUnicode_Type)) {
        return append_string(val, out);
    }
    if (LIKELY(vt == &PyLong_Type)) {
        return append_py_long(out, val);
    }
    if (vt == &PyDict_Type) {
        if (UNLIKELY(depth >= get_max_serialize_depth())) {
            PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
            return false;
        }
        return serialize_dict(val, out, depth + 1);
    }
    if (vt == &PyList_Type) {
        if (UNLIKELY(depth >= get_max_serialize_depth())) {
            PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
            return false;
        }
        return serialize_list(val, out, depth + 1);
    }
    if (vt == &PyFloat_Type) {
        return append_double(out, PyFloat_AS_DOUBLE(val));
    }
    if (UNLIKELY(val == Py_None)) {
        out.append("null", 4);
        return true;
    }
    if (UNLIKELY(vt == &PyBool_Type)) {
        out.append(val == Py_True ? "true" : "false", val == Py_True ? 4 : 5);
        return true;
    }
    if (PyTuple_Check(val)) {
        if (UNLIKELY(depth >= get_max_serialize_depth())) {
            PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
            return false;
        }
        return serialize_tuple(val, out, depth + 1);
    }
    if (PyUnicode_Check(val)) {
        return append_string(val, out);
    }
    PyErr_SetString(PyExc_TypeError, "Object of unsupported type cannot be serialized to JSON");
    return false;
}

// Python dumps() function
PyObject* strata_dumps(PyObject* self, PyObject* obj) {
    STRATA_CPP_TRY

    PyGcPause gc_pause;
    g_max_depth = Py_GetRecursionLimit();
    g_seen_stack.clear();
    g_serialize_buffer.clear();
    g_serialize_buffer.reserve(estimate_size(obj));

    if (!serialize_value(obj, g_serialize_buffer, 0)) {
        return NULL;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }

    return PyUnicode_FromStringAndSize(g_serialize_buffer.data(), g_serialize_buffer.size());

    STRATA_CPP_CATCH
}

// Python dumps_bytes() function
PyObject* strata_dumps_bytes(PyObject* self, PyObject* obj) {
    STRATA_CPP_TRY

    PyGcPause gc_pause;
    g_max_depth = Py_GetRecursionLimit();
    g_seen_stack.clear();
    g_serialize_buffer.clear();
    g_serialize_buffer.reserve(estimate_size(obj));

    if (!serialize_value(obj, g_serialize_buffer, 0)) {
        return NULL;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }

    return PyBytes_FromStringAndSize(g_serialize_buffer.data(), g_serialize_buffer.size());

    STRATA_CPP_CATCH
}

PyObject* strata_set_cycle_policy(PyObject* self, PyObject* args) {
    const char* policy = nullptr;
    if (!PyArg_ParseTuple(args, "s", &policy)) {
        return NULL;
    }

    std::string error;
    if (!set_cycle_policy_from_string(policy, error)) {
        PyErr_SetString(PyExc_ValueError, error.c_str());
        return NULL;
    }

    Py_RETURN_NONE;
}

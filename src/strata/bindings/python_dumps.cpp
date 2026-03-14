#include "python_types.h"
#include "strata/util/arena_allocator.hpp"
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

// Thread-local buffers for zero-allocation serialization
thread_local strata::util::OutputBuffer g_serialize_buffer;
thread_local strata::util::Arena g_serialize_arena;

enum class CyclePolicy { Warn, Error, Ignore };
static CyclePolicy g_cycle_policy = CyclePolicy::Warn;

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

    if (LIKELY(value >= -9999 && value <= 9999)) {
        uint32_t v = static_cast<uint32_t>(value);
        if (value < 0) {
            out.push_back('-');
            v = static_cast<uint32_t>(-value);
        }

        if (v < 10) {
            out.push_back(static_cast<char>('0' + v));
            return true;
        }
        if (v < 100) {
            out.append(kDigitPairs + v * 2, 2);
            return true;
        }

        uint32_t high = v / 100;
        uint32_t low = v - high * 100;
        if (high < 10) {
            out.push_back(static_cast<char>('0' + high));
        } else {
            out.append(kDigitPairs + high * 2, 2);
        }
        out.append(kDigitPairs + low * 2, 2);
        return true;
    }

    char buf[32];
    auto result = std::to_chars(buf, buf + sizeof(buf), value);
    if (result.ec != std::errc()) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to format integer");
        return false;
    }
    out.append(buf, static_cast<size_t>(result.ptr - buf));
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

    char buf[32];
    int len = strata::util::dragonbox_d2s(value, buf);
    out.append(buf, static_cast<size_t>(len));
    return true;
}

template <typename Buffer> static inline bool append_string(PyObject* obj, Buffer& out) {
    if (PyUnicode_IS_COMPACT_ASCII(obj)) {
        Py_ssize_t len = PyUnicode_GET_LENGTH(obj);
        const char* data = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(obj));
        if (strata::util::try_copy_clean_string(data, static_cast<size_t>(len), out)) {
            return true;
        }
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

struct Frame {
    enum class Type { Dict, List, Tuple };
    Type type;
    PyObject* obj;
    Py_ssize_t index;
    Py_ssize_t size;
    Py_ssize_t pos;
    bool first;
};

template <typename Buffer> static bool serialize_iterative(PyObject* root, Buffer& out);

static inline size_t estimate_size(PyObject* obj) {
    if (PyDict_CheckExact(obj)) {
        return static_cast<size_t>(PyDict_GET_SIZE(obj)) * 48 + 2;
    }
    if (PyList_CheckExact(obj)) {
        return static_cast<size_t>(PyList_GET_SIZE(obj)) * 32 + 2;
    }
    if (PyTuple_Check(obj)) {
        return static_cast<size_t>(PyTuple_GET_SIZE(obj)) * 32 + 2;
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

template <typename Buffer> static bool serialize_iterative(PyObject* root, Buffer& out) {
    using FrameAllocator = strata::util::ArenaAllocator<Frame>;
    std::vector<Frame, FrameAllocator> stack{FrameAllocator(&g_serialize_arena)};
    stack.reserve(64);

    // Push a container (dict/list/tuple) onto the frame stack with cycle detection.
    // Non-container, non-string types set a TypeError. Returns false only on error.
    auto push_container = [&](PyObject* val) -> bool {
        for (const auto& f : stack) {
            if (UNLIKELY(f.obj == val)) {
                switch (g_cycle_policy) {
                case CyclePolicy::Warn:
                    PyErr_WarnEx(PyExc_RuntimeWarning, "Cycle detected during JSON serialization",
                                 1);
                    [[fallthrough]];
                case CyclePolicy::Ignore:
                    out.append("null", 4);
                    return true;
                case CyclePolicy::Error:
                    PyErr_SetString(PyExc_ValueError, "Cycle detected during JSON serialization");
                    return false;
                }
            }
        }
        if (LIKELY(PyDict_CheckExact(val))) {
            Py_ssize_t sz = PyDict_GET_SIZE(val);
            if (sz == 0) {
                out.append("{}", 2);
            } else {
                out.push_back('{');
                stack.push_back({Frame::Type::Dict, val, 0, sz, 0, true});
            }
        } else if (LIKELY(PyList_CheckExact(val))) {
            Py_ssize_t sz = PyList_GET_SIZE(val);
            if (sz == 0) {
                out.append("[]", 2);
            } else {
                out.push_back('[');
                stack.push_back({Frame::Type::List, val, 0, sz, 0, true});
            }
        } else if (PyTuple_Check(val)) {
            Py_ssize_t sz = PyTuple_GET_SIZE(val);
            if (sz == 0) {
                out.append("[]", 2);
            } else {
                out.push_back('[');
                stack.push_back({Frame::Type::Tuple, val, 0, sz, 0, true});
            }
        } else if (PyUnicode_Check(val)) {
            return append_string(val, out);
        } else {
            PyErr_SetString(PyExc_TypeError,
                            "Object of unsupported type cannot be serialized to JSON");
            return false;
        }
        return true;
    };

    // Write a single value to out. Primitives are written inline; containers push a frame.
    // Type-check order: str > int > dict > list > float >> None/bool/other.
    // Dict and list are placed before float/None/bool since they appear as values frequently.
    auto write_value = [&](PyObject* val) -> bool {
        PyTypeObject* vt = Py_TYPE(val);
        if (LIKELY(vt == &PyUnicode_Type)) {
            return append_string(val, out);
        }
        if (LIKELY(vt == &PyLong_Type)) {
            return append_py_long(out, val);
        }
        if (vt == &PyDict_Type) {
            return push_container(val);
        }
        if (vt == &PyList_Type) {
            return push_container(val);
        }
        if (vt == &PyFloat_Type) {
            append_double(out, PyFloat_AS_DOUBLE(val));
            return true;
        }
        if (UNLIKELY(val == Py_None)) {
            out.append("null", 4);
            return true;
        }
        if (UNLIKELY(vt == &PyBool_Type)) {
            out.append(val == Py_True ? "true" : "false", val == Py_True ? 4 : 5);
            return true;
        }
        return push_container(val); // tuple/str-subclass/unsupported
    };

    // Serialize root (which may itself be a primitive).
    if (!write_value(root)) {
        return false;
    }

    while (!stack.empty()) {
        Frame& frame = stack.back();

        if (frame.type == Frame::Type::Dict) {
            PyObject* key = nullptr;
            PyObject* value = nullptr;
            if (PyDict_Next(frame.obj, &frame.pos, &key, &value)) {
                // Fast path for compact ASCII keys (interned keys are almost always ASCII).
                if (LIKELY(PyUnicode_IS_COMPACT_ASCII(key))) {
                    const Py_ssize_t klen = PyUnicode_GET_LENGTH(key);
                    const char* kdata = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(key));
                    const size_t need = static_cast<size_t>(klen) + (frame.first ? 3 : 4);
                    out.reserve(out.size() + need);
                    if (!frame.first) {
                        out.unsafe_push_back(',');
                    }
                    frame.first = false;
                    if (LIKELY(
                            !strata::util::string_needs_escape(kdata, static_cast<size_t>(klen)))) {
                        out.unsafe_push_back('"');
                        out.unsafe_append(kdata, static_cast<size_t>(klen));
                        out.unsafe_push_back('"');
                        out.unsafe_push_back(':');
                    } else {
                        strata::util::escape_json_string_simd(kdata, static_cast<size_t>(klen),
                                                              out);
                        out.push_back(':');
                    }
                } else {
                    if (!frame.first) {
                        out.push_back(',');
                    }
                    frame.first = false;
                    if (!PyUnicode_Check(key)) {
                        PyErr_SetString(PyExc_TypeError, "Dict keys must be strings");
                        return false;
                    }
                    if (!append_string(key, out)) {
                        return false;
                    }
                    out.push_back(':');
                }
                if (!write_value(value)) {
                    return false;
                }
            } else {
                out.push_back('}');
                stack.pop_back();
            }
        } else {
            // List or Tuple
            if (frame.index < frame.size) {
                if (!frame.first) {
                    out.push_back(',');
                }
                frame.first = false;
                PyObject* item = (frame.type == Frame::Type::List)
                                     ? PyList_GET_ITEM(frame.obj, frame.index)
                                     : PyTuple_GET_ITEM(frame.obj, frame.index);
                frame.index += 1;
                if (!write_value(item)) {
                    return false;
                }
            } else {
                out.push_back(']');
                stack.pop_back();
            }
        }
    }

    return true;
}

// Python dumps() function
PyObject* strata_dumps(PyObject* self, PyObject* obj) {
    STRATA_CPP_TRY

    g_serialize_arena.reset();
    g_serialize_buffer.clear();
    g_serialize_buffer.reserve(estimate_size(obj));

    if (!serialize_iterative(obj, g_serialize_buffer)) {
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

    g_serialize_arena.reset();
    g_serialize_buffer.clear();
    g_serialize_buffer.reserve(estimate_size(obj));

    if (!serialize_iterative(obj, g_serialize_buffer)) {
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

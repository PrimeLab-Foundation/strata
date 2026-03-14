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

static inline bool is_container(PyObject* obj) {
    return PyDict_CheckExact(obj) || PyList_CheckExact(obj) || PyTuple_Check(obj);
}

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
#if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L
    auto result = std::to_chars(buf, buf + sizeof(buf), value);
    if (result.ec != std::errc()) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to format integer");
        return false;
    }
    out.append(buf, static_cast<size_t>(result.ptr - buf));
#else
    int n = std::snprintf(buf, sizeof(buf), "%" PRId64, value);
    if (n < 0 || static_cast<size_t>(n) >= sizeof(buf)) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to format integer");
        return false;
    }
    out.append(buf, static_cast<size_t>(n));
#endif
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

template <typename Buffer>
static inline bool serialize_primitive(PyObject* obj, Buffer& out, bool& handled) {
    // Order by frequency: str ~35%, int ~33%, float ~30%, then rare types.
    if (PyUnicode_CheckExact(obj)) {
        handled = true;
        return append_string(obj, out);
    }
    PyTypeObject* type = Py_TYPE(obj);
    if (type == &PyLong_Type) {
        handled = true;
        return append_py_long(out, obj);
    }
    if (type == &PyFloat_Type) {
        handled = true;
        return append_double(out, PyFloat_AS_DOUBLE(obj));
    }
    if (obj == Py_None) {
        append_literal(out, "null", 4);
        handled = true;
        return true;
    }
    if (type == &PyBool_Type) {
        // Py_True/Py_False: bool is a subtype of int, but &PyLong_Type won't match it.
        append_literal(out, obj == Py_True ? "true" : "false", obj == Py_True ? 4 : 5);
        handled = true;
        return true;
    }
    // Non-ASCII unicode subtype (e.g. str subclasses): fall through to append_string
    if (PyUnicode_Check(obj)) {
        handled = true;
        return append_string(obj, out);
    }

    handled = false;
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

template <typename Buffer>
static bool serialize_iterative_with_memo(PyObject* root, Buffer& out, PyObject* memo);

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

static PyObject* dumps_bytes_direct(PyObject* obj, size_t estimate) {
    if (estimate == 0 || estimate > static_cast<size_t>(PY_SSIZE_T_MAX)) {
        return nullptr;
    }

    PyObject* bytes = PyBytes_FromStringAndSize(nullptr, static_cast<Py_ssize_t>(estimate));
    if (!bytes) {
        return nullptr;
    }

    char* buffer = PyBytes_AS_STRING(bytes);
    strata::util::FixedOutputBuffer direct_buffer(buffer, estimate);
    direct_buffer.clear();

    if (!serialize_iterative(obj, direct_buffer) || PyErr_Occurred()) {
        Py_DECREF(bytes);
        return nullptr;
    }

    if (direct_buffer.overflowed()) {
        Py_DECREF(bytes);
        return nullptr;
    }

    Py_ssize_t actual = static_cast<Py_ssize_t>(direct_buffer.size());
    if (actual < 0) {
        Py_DECREF(bytes);
        return nullptr;
    }
    if (actual != static_cast<Py_ssize_t>(estimate)) {
        if (_PyBytes_Resize(&bytes, actual) < 0) {
            Py_DECREF(bytes);
            return nullptr;
        }
    }
    return bytes;
}

template <typename Buffer> static bool serialize_iterative(PyObject* root, Buffer& out) {
    using FrameAllocator = strata::util::ArenaAllocator<Frame>;
    std::vector<Frame, FrameAllocator> stack{FrameAllocator(&g_serialize_arena)};
    stack.reserve(64);
    PyObject* current = root;

    while (true) {
        if (current) {
            if (current == Py_None) {
                append_literal(out, "null", 4);
                current = nullptr;
                continue;
            }

            // Fast identity checks for boolean singletons (before type dispatch)
            if (UNLIKELY(current == Py_True)) {
                append_literal(out, "true", 4);
                current = nullptr;
                continue;
            }
            if (UNLIKELY(current == Py_False)) {
                append_literal(out, "false", 5);
                current = nullptr;
                continue;
            }

            bool container = is_container(current);
            if (container) {
                bool cycle = false;
                for (const auto& f : stack) {
                    if (f.obj == current) {
                        cycle = true;
                        break;
                    }
                }
                if (cycle) {
                    switch (g_cycle_policy) {
                    case CyclePolicy::Warn:
                        PyErr_WarnEx(PyExc_RuntimeWarning,
                                     "Cycle detected during JSON serialization", 1);
                        append_literal(out, "null", 4);
                        current = nullptr;
                        continue;
                    case CyclePolicy::Ignore:
                        append_literal(out, "null", 4);
                        current = nullptr;
                        continue;
                    case CyclePolicy::Error:
                        PyErr_SetString(PyExc_ValueError,
                                        "Cycle detected during JSON serialization");
                        return false;
                    }
                }
            }

            if (LIKELY(PyDict_CheckExact(current))) {
                Py_ssize_t size = PyDict_GET_SIZE(current);
                if (size == 0) {
                    append_literal(out, "{}", 2);
                    current = nullptr;
                    continue;
                }

                out.push_back('{');
                stack.push_back(Frame{Frame::Type::Dict, current, 0, size, 0, true});
                current = nullptr;
                continue;
            }

            if (LIKELY(PyList_CheckExact(current))) {
                Py_ssize_t size = PyList_GET_SIZE(current);
                if (size == 0) {
                    append_literal(out, "[]", 2);
                    current = nullptr;
                    continue;
                }

                out.push_back('[');
                stack.push_back(Frame{Frame::Type::List, current, 0, size, 0, true});
                current = nullptr;
                continue;
            }

            if (PyTuple_Check(current)) {
                Py_ssize_t size = PyTuple_GET_SIZE(current);
                if (size == 0) {
                    append_literal(out, "[]", 2);
                    current = nullptr;
                    continue;
                }

                out.push_back('[');
                stack.push_back(Frame{Frame::Type::Tuple, current, 0, size, 0, true});
                current = nullptr;
                continue;
            }

            if (LIKELY(PyUnicode_Check(current))) {
                if (!append_string(current, out)) {
                    return false;
                }
                current = nullptr;
                continue;
            }

            PyTypeObject* type = Py_TYPE(current);

            if (LIKELY(type == &PyLong_Type)) {
                if (!append_py_long(out, current)) {
                    return false;
                }
                current = nullptr;
                continue;
            }

            if (LIKELY(type == &PyFloat_Type)) {
                double val = PyFloat_AS_DOUBLE(current);
                if (!append_double(out, val)) {
                    return false;
                }
                current = nullptr;
                continue;
            }

            if (UNLIKELY(type == &PyBool_Type)) {
                if (current == Py_True) {
                    append_literal(out, "true", 4);
                } else {
                    append_literal(out, "false", 5);
                }
                current = nullptr;
                continue;
            }

            PyErr_SetString(PyExc_TypeError,
                            "Object of unsupported type cannot be serialized to JSON");
            return false;
        }

        if (stack.empty()) {
            break;
        }

        Frame& frame = stack.back();
        if (frame.type == Frame::Type::Dict) {
            PyObject* key = nullptr;
            PyObject* value = nullptr;
            if (PyDict_Next(frame.obj, &frame.pos, &key, &value)) {
                // Fast path for compact ASCII keys (interned dict keys are almost always ASCII).
                if (LIKELY(PyUnicode_IS_COMPACT_ASCII(key))) {
                    const Py_ssize_t klen = PyUnicode_GET_LENGTH(key);
                    const char* kdata = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(key));
                    // Reserve for: [,]"key": = klen + 3 or 4 bytes
                    const size_t need = static_cast<size_t>(klen) + (frame.first ? 3 : 4);
                    out.reserve(out.size() + need);
                    if (!frame.first) {
                        out.unsafe_push_back(',');
                    }
                    frame.first = false;
                    // Check for escapes; fall back if needed (rare for interned keys)
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
                bool handled = false;
                if (!serialize_primitive(value, out, handled)) {
                    return false;
                }
                if (handled) {
                    continue;
                }
                current = value;
            } else {
                out.push_back('}');
                stack.pop_back();
            }
            continue;
        }

        if (frame.index < frame.size) {
            if (!frame.first) {
                out.push_back(',');
            }
            frame.first = false;

            PyObject* item = (frame.type == Frame::Type::List)
                                 ? PyList_GET_ITEM(frame.obj, frame.index)
                                 : PyTuple_GET_ITEM(frame.obj, frame.index);
            frame.index += 1;
            bool handled = false;
            if (!serialize_primitive(item, out, handled)) {
                return false;
            }
            if (handled) {
                continue;
            }
            current = item;
            continue;
        }

        out.push_back(']');
        stack.pop_back();
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

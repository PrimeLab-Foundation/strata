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
#include <unordered_set>
#include <vector>

// Thread-local buffers for zero-allocation serialization
thread_local strata::util::OutputBuffer g_serialize_buffer;
thread_local strata::util::Arena g_serialize_arena;

enum class CyclePolicy { Warn, Error, Ignore, NoCheck };
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
    if (strcmp(policy, "nocheck") == 0) {
        g_cycle_policy = CyclePolicy::NoCheck;
        return true;
    }
    error = "unknown cycle policy (expected warn|error|ignore|nocheck)";
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
        // Fast path for compact ASCII strings (most common case)
        // Use single-pass escape_or_copy to avoid double-scan
        Py_ssize_t len = PyUnicode_GET_LENGTH(obj);
        const char* data = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(obj));
        strata::util::escape_or_copy_string_simd(data, static_cast<size_t>(len), out);
        return true;
    }

    Py_ssize_t len = 0;
    const char* data = PyUnicode_AsUTF8AndSize(obj, &len);
    if (!data) {
        return false;
    }

    // Non-ASCII strings: use single-pass escape_or_copy
    strata::util::escape_or_copy_string_simd(data, static_cast<size_t>(len), out);
    return true;
}

template <typename Buffer>
static inline bool serialize_primitive(PyObject* obj, Buffer& out, bool& handled) {
    if (obj == Py_None) {
        append_literal(out, "null", 4);
        handled = true;
        return true;
    }
    if (obj == Py_True) {
        append_literal(out, "true", 4);
        handled = true;
        return true;
    }
    if (obj == Py_False) {
        append_literal(out, "false", 5);
        handled = true;
        return true;
    }
    if (PyUnicode_Check(obj)) {
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

// Estimation constants for pre-sizing output buffer
static constexpr size_t kMinEstimate = 1024;  // minimum buffer size (larger to reduce reallocs)

// Lightweight size estimate - O(1) based on top-level container size
// Avoids expensive recursive traversal of Python objects
static inline size_t estimate_size(PyObject* obj) {
    // Simple heuristic: top-level container size * multiplier
    // This is much faster than recursive estimation
    if (PyDict_CheckExact(obj)) {
        // Dicts: estimate based on number of keys
        // Each entry is ~50-100 bytes on average (key + value + separators)
        Py_ssize_t size = PyDict_GET_SIZE(obj);
        return static_cast<size_t>(size) * 96 + kMinEstimate;
    }
    if (PyList_CheckExact(obj)) {
        // Lists: estimate based on number of items
        // Each item is ~30-60 bytes on average
        Py_ssize_t size = PyList_GET_SIZE(obj);
        return static_cast<size_t>(size) * 64 + kMinEstimate;
    }
    if (PyTuple_Check(obj)) {
        Py_ssize_t size = PyTuple_GET_SIZE(obj);
        return static_cast<size_t>(size) * 64 + kMinEstimate;
    }
    // Primitives and strings
    if (PyUnicode_Check(obj)) {
        return static_cast<size_t>(PyUnicode_GET_LENGTH(obj)) + 16;  // +quotes+margin
    }
    return kMinEstimate;
}

// Direct serialization into a PyUnicode compact-ASCII object.
// Avoids the OutputBuffer → PyUnicode_FromStringAndSize copy+decode path.
// Returns nullptr (without setting an error) on overflow so the caller can
// fall back to the dynamic-buffer path.
static PyObject* dumps_str_direct(PyObject* obj, size_t estimate) {
    if (estimate == 0 || estimate > static_cast<size_t>(PY_SSIZE_T_MAX)) {
        return nullptr;
    }

    // Allocate a compact ASCII Unicode object (maxchar=127).
    // PyUnicode_New gives us a writable buffer via PyUnicode_1BYTE_DATA.
    PyObject* unicode = PyUnicode_New(static_cast<Py_ssize_t>(estimate), 127);
    if (!unicode) {
        return nullptr;
    }

    char* buffer = reinterpret_cast<char*>(PyUnicode_1BYTE_DATA(unicode));
    strata::util::FixedOutputBuffer direct_buffer(buffer, estimate);
    direct_buffer.clear();

    if (!serialize_iterative(obj, direct_buffer) || PyErr_Occurred()) {
        Py_DECREF(unicode);
        return nullptr;
    }

    if (direct_buffer.overflowed()) {
        Py_DECREF(unicode);
        return nullptr;
    }

    size_t actual = direct_buffer.size();

    // Check whether the output is purely ASCII.  If any byte >= 0x80 exists
    // (because the input contained non-ASCII strings that were passed through
    // as raw UTF-8), the compact-ASCII object we allocated is the wrong kind.
    // Fall back to PyUnicode_DecodeUTF8 for correctness.
    if (!strata::util::is_ascii_only_simd(buffer, actual)) {
        PyObject* result = PyUnicode_DecodeUTF8(buffer, static_cast<Py_ssize_t>(actual), nullptr);
        Py_DECREF(unicode);
        return result;
    }

    Py_ssize_t actual_ssize = static_cast<Py_ssize_t>(actual);
    if (actual_ssize < 0) {
        Py_DECREF(unicode);
        return nullptr;
    }

    if (actual == estimate) {
        // Perfect fit — nothing to do.
        return unicode;
    }

    // Actual size differs from estimate — we must create a correctly-sized
    // object because PyUnicode internals store the length and there is no
    // public resize API.
    PyObject* trimmed = PyUnicode_New(actual_ssize, 127);
    if (!trimmed) {
        Py_DECREF(unicode);
        return nullptr;
    }
    std::memcpy(PyUnicode_1BYTE_DATA(trimmed), buffer, actual);
    Py_DECREF(unicode);
    return trimmed;
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

    // O(1) cycle detection using hash set instead of O(n) linear scan
    // Skip entirely when policy is NoCheck for maximum performance
    const bool check_cycles = (g_cycle_policy != CyclePolicy::NoCheck);
    std::unordered_set<PyObject*> seen;
    if (check_cycles) {
        seen.reserve(64);
    }

    PyObject* current = root;

    while (true) {
        if (current) {
            // Fast path for primitives first (avoid is_container check for most common types)
            // Strings are most common in JSON, then integers
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

            if (current == Py_None) {
                append_literal(out, "null", 4);
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

            // Now check containers (need cycle detection)
            bool container = is_container(current);
            if (container && check_cycles) {
                // O(1) cycle detection using hash set
                bool cycle = seen.count(current) > 0;
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
                    case CyclePolicy::NoCheck:
                        // Should never reach here
                        break;
                    }
                }
                seen.insert(current);  // Add to seen set for O(1) cycle detection
            }

            if (LIKELY(PyDict_CheckExact(current))) {
                Py_ssize_t size = PyDict_GET_SIZE(current);
                if (size == 0) {
                    if (check_cycles) seen.erase(current);  // Remove empty containers from seen
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
                    if (check_cycles) seen.erase(current);  // Remove empty containers from seen
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
                    if (check_cycles) seen.erase(current);  // Remove empty containers from seen
                    append_literal(out, "[]", 2);
                    current = nullptr;
                    continue;
                }

                out.push_back('[');
                stack.push_back(Frame{Frame::Type::Tuple, current, 0, size, 0, true});
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
                if (check_cycles) seen.erase(frame.obj);  // Remove from seen set when popping
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
        if (check_cycles) seen.erase(frame.obj);  // Remove from seen set when popping
        stack.pop_back();
    }

    return true;
}

// Python dumps() function
PyObject* strata_dumps(PyObject* self, PyObject* obj) {
    STRATA_CPP_TRY

    g_serialize_arena.reset();
    size_t estimate = estimate_size(obj);

    // Fast path: serialize directly into a PyUnicode compact-ASCII buffer.
    // Avoids the intermediate OutputBuffer and the PyUnicode_FromStringAndSize
    // UTF-8 decode + copy overhead.
    if (PyObject* direct = dumps_str_direct(obj, estimate)) {
        return direct;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }

    // Fallback: estimate was too small (overflow) — use the dynamic buffer.
    g_serialize_arena.reset();
    g_serialize_buffer.clear();
    g_serialize_buffer.reserve(estimate);

    if (!serialize_iterative(obj, g_serialize_buffer)) {
        return NULL;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }

    const char* data = g_serialize_buffer.data();
    size_t len = g_serialize_buffer.size();

    // Fast path for ASCII-only output (common case): allocate a compact
    // ASCII PyUnicode and memcpy.  Avoids the full UTF-8 decode that
    // PyUnicode_FromStringAndSize performs.
    if (strata::util::is_ascii_only_simd(data, len)) {
        Py_ssize_t slen = static_cast<Py_ssize_t>(len);
        PyObject* unicode = PyUnicode_New(slen, 127);
        if (!unicode) {
            return NULL;
        }
        std::memcpy(PyUnicode_1BYTE_DATA(unicode), data, len);
        return unicode;
    }

    return PyUnicode_DecodeUTF8(data, static_cast<Py_ssize_t>(len), nullptr);

    STRATA_CPP_CATCH
}

// Python dumps_bytes() function
PyObject* strata_dumps_bytes(PyObject* self, PyObject* obj) {
    STRATA_CPP_TRY

    g_serialize_arena.reset();
    size_t estimate = estimate_size(obj);
    if (PyObject* direct = dumps_bytes_direct(obj, estimate)) {
        return direct;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }

    g_serialize_arena.reset();
    g_serialize_buffer.clear();
    g_serialize_buffer.reserve(estimate);

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

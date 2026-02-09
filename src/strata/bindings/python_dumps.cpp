#include "python_types.h"
#include "strata/util/arena_allocator.hpp"
#include "strata/util/dragonbox.hpp"
#include "strata/util/output_buffer.hpp"
#include "strata/util/simd_string.hpp"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// Thread-local buffers for zero-allocation serialization
thread_local strata::util::OutputBuffer g_serialize_buffer;
thread_local strata::util::Arena g_serialize_arena;
thread_local size_t g_last_dumps_size = 0;

enum class CyclePolicy { Warn, Error, Ignore, NoCheck };
static CyclePolicy g_cycle_policy = CyclePolicy::Warn;

enum class DumpsTypeOrder { StringsFirst, IntsFirst };
#if defined(STRATA_DUMPS_INTS_FIRST)
static DumpsTypeOrder g_dumps_type_order = DumpsTypeOrder::IntsFirst;
#else
static DumpsTypeOrder g_dumps_type_order = DumpsTypeOrder::StringsFirst;
#endif

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

bool set_dumps_type_order_from_string(const char* order, std::string& error) {
    if (order == nullptr) {
        error = "dumps type order must be a string";
        return false;
    }
    if (strcmp(order, "strings_first") == 0) {
        g_dumps_type_order = DumpsTypeOrder::StringsFirst;
        return true;
    }
    if (strcmp(order, "ints_first") == 0) {
        g_dumps_type_order = DumpsTypeOrder::IntsFirst;
        return true;
    }
    error = "unknown dumps type order (expected strings_first|ints_first)";
    return false;
}

int strata_get_cycle_policy() { return static_cast<int>(g_cycle_policy); }

static inline bool is_container(PyObject* obj) {
    return PyDict_CheckExact(obj) || PyList_CheckExact(obj) || PyTuple_Check(obj);
}

// Stack-allocated small buffer for cycle detection with hash-set spillover.
// Linear scan over a fixed array is faster than unordered_set for typical
// JSON nesting depths (< 32).  Falls back to a heap-allocated hash set
// only when depth exceeds kInlineCapacity.
class SmallCycleSet {
    static constexpr size_t kInlineCapacity = 32;
    PyObject* inline_buf_[kInlineCapacity];
    size_t inline_size_ = 0;

    // Spillover set — only allocated when inline buffer overflows.
    // We use a sorted vector for the spillover since insertions beyond depth
    // 32 are rare and the set stays small.
    PyObject** spill_ = nullptr;
    size_t spill_size_ = 0;
    size_t spill_cap_ = 0;

  public:
    SmallCycleSet() = default;
    ~SmallCycleSet() { std::free(spill_); }

    // Non-copyable
    SmallCycleSet(const SmallCycleSet&) = delete;
    SmallCycleSet& operator=(const SmallCycleSet&) = delete;

    bool contains(PyObject* ptr) const {
        // Linear scan over inline buffer (branch-free on modern CPUs for small N)
        for (size_t i = 0; i < inline_size_; ++i) {
            if (inline_buf_[i] == ptr) return true;
        }
        // Scan spillover (rare path)
        for (size_t i = 0; i < spill_size_; ++i) {
            if (spill_[i] == ptr) return true;
        }
        return false;
    }

    void insert(PyObject* ptr) {
        if (LIKELY(inline_size_ < kInlineCapacity)) {
            inline_buf_[inline_size_++] = ptr;
            return;
        }
        // Spillover path
        if (spill_size_ == spill_cap_) {
            size_t new_cap = spill_cap_ == 0 ? 16 : spill_cap_ * 2;
            auto* p = static_cast<PyObject**>(std::realloc(spill_, new_cap * sizeof(PyObject*)));
            if (!p) return;  // OOM — skip (cycle detection is best-effort)
            spill_ = p;
            spill_cap_ = new_cap;
        }
        spill_[spill_size_++] = ptr;
    }

    void erase(PyObject* ptr) {
        // Check inline buffer (search from end — LIFO pattern for nesting)
        for (size_t i = inline_size_; i-- > 0;) {
            if (inline_buf_[i] == ptr) {
                // Swap with last element for O(1) removal
                inline_buf_[i] = inline_buf_[--inline_size_];
                return;
            }
        }
        // Check spillover
        for (size_t i = spill_size_; i-- > 0;) {
            if (spill_[i] == ptr) {
                spill_[i] = spill_[--spill_size_];
                return;
            }
        }
    }
};

template <typename Buffer>
static inline void append_literal(Buffer& out, const char* data, size_t len) {
    out.append(data, len);
}

template <typename Buffer> static inline bool append_int64(Buffer& out, int64_t value) {
    static constexpr char kDigitPairs[] = "00010203040506070809"
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
    char* ptr = buf + sizeof(buf);
    uint64_t v = 0;
    if (value < 0) {
        v = static_cast<uint64_t>(-(value + 1)) + 1;
    } else {
        v = static_cast<uint64_t>(value);
    }

    while (v >= 100) {
        uint32_t idx = static_cast<uint32_t>(v % 100);
        v /= 100;
        ptr -= 2;
        std::memcpy(ptr, kDigitPairs + idx * 2, 2);
    }

    if (v < 10) {
        *--ptr = static_cast<char>('0' + static_cast<int>(v));
    } else {
        ptr -= 2;
        std::memcpy(ptr, kDigitPairs + v * 2, 2);
    }

    if (value < 0) {
        *--ptr = '-';
    }

    out.append(ptr, static_cast<size_t>(buf + sizeof(buf) - ptr));
    return true;
}

template <typename Buffer> static inline bool append_py_long(Buffer& out, PyObject* obj) {
#ifdef PyUnstable_Long_IsCompact
    auto* long_obj = reinterpret_cast<PyLongObject*>(obj);
    if (LIKELY(PyUnstable_Long_IsCompact(long_obj))) {
        Py_ssize_t val = PyUnstable_Long_CompactValue(long_obj);
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
    Py_ssize_t len = 0;
    const char* data = nullptr;
    if (PyUnicode_IS_COMPACT_ASCII(obj)) {
        // Fast path for compact ASCII strings (most common case)
        len = PyUnicode_GET_LENGTH(obj);
        data = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(obj));
    } else {
        data = PyUnicode_AsUTF8AndSize(obj, &len);
        if (!data) {
            return false;
        }
    }

    size_t size = static_cast<size_t>(len);
    if (size <= 16) {
        uint32_t needs_escape = 0;
        for (size_t i = 0; i < size; ++i) {
            unsigned char c = static_cast<unsigned char>(data[i]);
            needs_escape |= static_cast<uint32_t>((c < 0x20) | (c == '"') | (c == '\\'));
        }
        if (needs_escape == 0) {
            out.push_back('"');
            out.append(data, size);
            out.push_back('"');
            return true;
        }
    }

    strata::util::escape_or_copy_string_simd(data, size, out);
    return true;
}

template <typename Buffer>
static inline bool serialize_primitive(PyObject* obj, Buffer& out, bool& handled, bool ints_first) {
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
    PyTypeObject* type = nullptr;
    if (ints_first) {
        type = Py_TYPE(obj);
        if (type == &PyLong_Type) {
            handled = true;
            return append_py_long(out, obj);
        }
        if (PyUnicode_Check(obj)) {
            handled = true;
            return append_string(obj, out);
        }
    } else {
        if (PyUnicode_Check(obj)) {
            handled = true;
            return append_string(obj, out);
        }
        type = Py_TYPE(obj);
        if (type == &PyLong_Type) {
            handled = true;
            return append_py_long(out, obj);
        }
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

static inline size_t structural_budget(Frame::Type type, Py_ssize_t size) {
    if (size <= 0) {
        return 2;
    }
    if (type == Frame::Type::Dict) {
        return static_cast<size_t>(size) * 2 + 1;
    }
    return static_cast<size_t>(size) + 1;
}

// Estimation constants for pre-sizing output buffer
static constexpr size_t kMinEstimate = 1024;
static constexpr size_t kMaxEstimate = static_cast<size_t>(PY_SSIZE_T_MAX);
static constexpr size_t kDictEntryEstimate = 96;
static constexpr size_t kListEntryEstimate = 64;
static constexpr size_t kContainerOverhead = 2;
static constexpr size_t kStringOverhead = 16;
static constexpr int kMaxEstimateDepth = 2;

static inline size_t clamp_add(size_t a, size_t b, size_t cap) {
    if (a > cap - b) {
        return cap;
    }
    return a + b;
}

static inline size_t clamp_mul(size_t a, size_t b, size_t cap) {
    if (a == 0 || b == 0) {
        return 0;
    }
    if (a > cap / b) {
        return cap;
    }
    return a * b;
}

static inline size_t apply_growth_factor(size_t estimate) {
    return clamp_add(estimate, estimate / 2, kMaxEstimate);
}

static inline size_t estimate_value_size(PyObject* obj, int depth);

static inline size_t estimate_sequence_size(PyObject* obj, Py_ssize_t size, int depth, bool is_tuple) {
    if (size <= 0) {
        return kContainerOverhead;
    }
    size_t estimate =
        clamp_add(clamp_mul(static_cast<size_t>(size), kListEntryEstimate, kMaxEstimate), kContainerOverhead,
                  kMaxEstimate);
    if (depth < kMaxEstimateDepth) {
        PyObject* first = is_tuple ? PyTuple_GET_ITEM(obj, 0) : PyList_GET_ITEM(obj, 0);
        if (first && PyDict_CheckExact(first)) {
            size_t dict_est = estimate_value_size(first, depth + 1);
            size_t scaled =
                clamp_add(clamp_mul(static_cast<size_t>(size), dict_est, kMaxEstimate), kContainerOverhead,
                          kMaxEstimate);
            if (scaled > estimate) {
                estimate = scaled;
            }
        }
    }
    return estimate;
}

static inline size_t estimate_dict_size(PyObject* obj, int depth) {
    Py_ssize_t size = PyDict_GET_SIZE(obj);
    if (size <= 0) {
        return kContainerOverhead;
    }
    size_t per_entry = kDictEntryEstimate;
    if (depth < kMaxEstimateDepth) {
        PyObject* key = nullptr;
        PyObject* value = nullptr;
        Py_ssize_t pos = 0;
        if (PyDict_Next(obj, &pos, &key, &value) && value != nullptr) {
            if (PyList_CheckExact(value) || PyTuple_Check(value)) {
                size_t list_est = estimate_value_size(value, depth + 1);
                size_t adjusted = clamp_add(list_est, kStringOverhead, kMaxEstimate);
                if (adjusted > per_entry) {
                    per_entry = adjusted;
                }
            }
        }
    }
    return clamp_add(clamp_mul(static_cast<size_t>(size), per_entry, kMaxEstimate), kContainerOverhead,
                     kMaxEstimate);
}

static inline size_t estimate_value_size(PyObject* obj, int depth) {
    if (PyDict_CheckExact(obj)) {
        return estimate_dict_size(obj, depth);
    }
    if (PyList_CheckExact(obj)) {
        return estimate_sequence_size(obj, PyList_GET_SIZE(obj), depth, false);
    }
    if (PyTuple_Check(obj)) {
        return estimate_sequence_size(obj, PyTuple_GET_SIZE(obj), depth, true);
    }
    if (PyUnicode_Check(obj)) {
        return clamp_add(static_cast<size_t>(PyUnicode_GET_LENGTH(obj)), kStringOverhead, kMaxEstimate);
    }
    return kStringOverhead;
}

// Depth-limited size estimate with one-element sampling.
static inline size_t estimate_size(PyObject* obj) {
    size_t estimate = estimate_value_size(obj, 0);
    if (g_last_dumps_size > estimate) {
        estimate = g_last_dumps_size;
    }
    if (estimate < kMinEstimate) {
        estimate = kMinEstimate;
    }
    if (estimate > kMaxEstimate) {
        estimate = kMaxEstimate;
    }
    return apply_growth_factor(estimate);
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
        if (result) {
            g_last_dumps_size = actual;
        }
        return result;
    }

    Py_ssize_t actual_ssize = static_cast<Py_ssize_t>(actual);
    if (actual_ssize < 0) {
        Py_DECREF(unicode);
        return nullptr;
    }

    if (actual != estimate) {
        // Shrink in-place when possible to avoid a second allocation+copy.
        if (PyUnicode_Resize(&unicode, actual_ssize) < 0) {
            Py_DECREF(unicode);
            return nullptr;
        }
    }

    g_last_dumps_size = actual;
    return unicode;
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
    g_last_dumps_size = static_cast<size_t>(actual);
    return bytes;
}

template <typename Buffer> static bool serialize_iterative(PyObject* root, Buffer& out) {
    using FrameAllocator = strata::util::ArenaAllocator<Frame>;
    std::vector<Frame, FrameAllocator> stack{FrameAllocator(&g_serialize_arena)};
    stack.reserve(64);

    // Lightweight cycle detection using stack-allocated small buffer.
    // Linear scan over 32-element array is faster than unordered_set for
    // typical nesting depths.  Falls back to heap allocation only for
    // pathologically deep structures.  Skipped entirely for NoCheck.
    const bool check_cycles = (g_cycle_policy != CyclePolicy::NoCheck);
    SmallCycleSet seen;
    const bool ints_first = (g_dumps_type_order == DumpsTypeOrder::IntsFirst);

    PyObject* current = root;

    while (true) {
        if (current) {
            // Fast path for primitives first (avoid is_container check for most common types)
            // String vs int order can be configured to match workload profiles.
            PyTypeObject* type = nullptr;
            if (ints_first) {
                type = Py_TYPE(current);
                if (LIKELY(type == &PyLong_Type)) {
                    if (!append_py_long(out, current)) {
                        return false;
                    }
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
            } else {
                if (LIKELY(PyUnicode_Check(current))) {
                    if (!append_string(current, out)) {
                        return false;
                    }
                    current = nullptr;
                    continue;
                }
                type = Py_TYPE(current);
                if (LIKELY(type == &PyLong_Type)) {
                    if (!append_py_long(out, current)) {
                        return false;
                    }
                    current = nullptr;
                    continue;
                }
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
                bool cycle = seen.contains(current);
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
                seen.insert(current);
            }

            if (LIKELY(PyDict_CheckExact(current))) {
                Py_ssize_t size = PyDict_GET_SIZE(current);
                out.ensure_extra(structural_budget(Frame::Type::Dict, size));
                if (size == 0) {
                    if (check_cycles) seen.erase(current);
                    out.push_back_unchecked('{');
                    out.push_back_unchecked('}');
                    current = nullptr;
                    continue;
                }

                out.push_back_unchecked('{');
                stack.push_back(Frame{Frame::Type::Dict, current, 0, size, 0, true});
                current = nullptr;
                continue;
            }

            if (LIKELY(PyList_CheckExact(current))) {
                Py_ssize_t size = PyList_GET_SIZE(current);
                out.ensure_extra(structural_budget(Frame::Type::List, size));
                if (size == 0) {
                    if (check_cycles) seen.erase(current);
                    out.push_back_unchecked('[');
                    out.push_back_unchecked(']');
                    current = nullptr;
                    continue;
                }

                out.push_back_unchecked('[');
                stack.push_back(Frame{Frame::Type::List, current, 0, size, 0, true});
                current = nullptr;
                continue;
            }

            if (PyTuple_Check(current)) {
                Py_ssize_t size = PyTuple_GET_SIZE(current);
                out.ensure_extra(structural_budget(Frame::Type::Tuple, size));
                if (size == 0) {
                    if (check_cycles) seen.erase(current);
                    out.push_back_unchecked('[');
                    out.push_back_unchecked(']');
                    current = nullptr;
                    continue;
                }

                out.push_back_unchecked('[');
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
                    out.push_back_unchecked(',');
                }
                frame.first = false;

                if (!PyUnicode_Check(key)) {
                    PyErr_SetString(PyExc_TypeError, "Dict keys must be strings");
                    return false;
                }
                if (!append_string(key, out)) {
                    return false;
                }
                out.push_back_unchecked(':');
                bool handled = false;
                if (!serialize_primitive(value, out, handled, ints_first)) {
                    return false;
                }
                if (handled) {
                    continue;
                }
                current = value;
            } else {
                out.push_back_unchecked('}');
                if (check_cycles) seen.erase(frame.obj);
                stack.pop_back();
            }
            continue;
        }

        if (frame.index < frame.size) {
            if (!frame.first) {
                out.push_back_unchecked(',');
            }
            frame.first = false;

            PyObject* item = (frame.type == Frame::Type::List)
                                 ? PyList_GET_ITEM(frame.obj, frame.index)
                                 : PyTuple_GET_ITEM(frame.obj, frame.index);
            frame.index += 1;
            bool handled = false;
            if (!serialize_primitive(item, out, handled, ints_first)) {
                return false;
            }
            if (handled) {
                continue;
            }
            current = item;
            continue;
        }

        out.push_back_unchecked(']');
        if (check_cycles) seen.erase(frame.obj);
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
    g_last_dumps_size = len;

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

    g_last_dumps_size = g_serialize_buffer.size();
    return PyBytes_FromStringAndSize(g_serialize_buffer.data(), g_serialize_buffer.size());

    STRATA_CPP_CATCH
}

PyObject* strata_set_dumps_type_order(PyObject* self, PyObject* args) {
    const char* order = nullptr;
    if (!PyArg_ParseTuple(args, "s", &order)) {
        return NULL;
    }

    std::string error;
    if (!set_dumps_type_order_from_string(order, error)) {
        PyErr_SetString(PyExc_ValueError, error.c_str());
        return NULL;
    }

    Py_RETURN_NONE;
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

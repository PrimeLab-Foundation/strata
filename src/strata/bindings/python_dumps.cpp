#include "python_types.h"
#include "strata/util/arena_allocator.hpp"
#include "strata/util/dragonbox.hpp"
#include "strata/util/output_buffer.hpp"
#include "strata/util/simd_string.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
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

PyObject* strata_dumps_bytes(PyObject* self, PyObject* obj);

namespace {
enum class ReturnType { Str, Bytes, ByteArray };

bool parse_return_type(PyObject* obj, ReturnType* out, ReturnType default_type) {
    if (!out) {
        PyErr_SetString(PyExc_RuntimeError, "invalid return_type output");
        return false;
    }
    if (obj == Py_None || obj == nullptr) {
        *out = default_type;
        return true;
    }
    if (PyType_Check(obj)) {
        if (obj == (PyObject*)&PyUnicode_Type) {
            *out = ReturnType::Str;
            return true;
        }
        if (obj == (PyObject*)&PyBytes_Type) {
            *out = ReturnType::Bytes;
            return true;
        }
        if (obj == (PyObject*)&PyByteArray_Type) {
            *out = ReturnType::ByteArray;
            return true;
        }
        PyErr_SetString(PyExc_TypeError, "return_type must be 'str', 'bytes', or 'bytearray'");
        return false;
    }
    if (PyUnicode_Check(obj)) {
        const char* value = PyUnicode_AsUTF8(obj);
        if (!value) {
            return false;
        }
        if (strcmp(value, "str") == 0) {
            *out = ReturnType::Str;
            return true;
        }
        if (strcmp(value, "bytes") == 0) {
            *out = ReturnType::Bytes;
            return true;
        }
        if (strcmp(value, "bytearray") == 0) {
            *out = ReturnType::ByteArray;
            return true;
        }
    }
    PyErr_SetString(PyExc_TypeError, "return_type must be 'str', 'bytes', or 'bytearray'");
    return false;
}

bool is_text_io(PyObject* obj) {
    static PyObject* text_io_base = nullptr;
    if (!text_io_base) {
        PyObject* io_module = PyImport_ImportModule("io");
        if (!io_module) {
            PyErr_Clear();
            return false;
        }
        text_io_base = PyObject_GetAttrString(io_module, "TextIOBase");
        Py_DECREF(io_module);
        if (!text_io_base) {
            PyErr_Clear();
            return false;
        }
    }
    int result = PyObject_IsInstance(obj, text_io_base);
    if (result < 0) {
        PyErr_Clear();
        return false;
    }
    return result == 1;
}

bool ends_with_ndjson(const char* data, Py_ssize_t len) {
    static const char suffix[] = ".ndjson";
    constexpr Py_ssize_t suffix_len = static_cast<Py_ssize_t>(sizeof(suffix) - 1);
    if (!data || len < suffix_len) {
        return false;
    }
    return std::equal(suffix, suffix + suffix_len, data + (len - suffix_len));
}

bool is_ndjson_name(PyObject* obj) {
    if (PyUnicode_Check(obj)) {
        Py_ssize_t len = 0;
        const char* text = PyUnicode_AsUTF8AndSize(obj, &len);
        if (!text) {
            return false;
        }
        return ends_with_ndjson(text, len);
    }
    if (PyBytes_Check(obj)) {
        char* data = nullptr;
        Py_ssize_t len = 0;
        if (PyBytes_AsStringAndSize(obj, &data, &len) < 0) {
            return false;
        }
        return ends_with_ndjson(data, len);
    }
    PyObject* pathlike = PyOS_FSPath(obj);
    if (pathlike) {
        bool result = is_ndjson_name(pathlike);
        Py_DECREF(pathlike);
        return result;
    }
    PyErr_Clear();
    return false;
}

bool detect_ndjson_name_attr(PyObject* obj) {
    PyObject* name_obj = PyObject_GetAttrString(obj, "name");
    if (!name_obj) {
        PyErr_Clear();
        return false;
    }
    bool result = is_ndjson_name(name_obj);
    Py_DECREF(name_obj);
    return result;
}
} // namespace

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

static inline bool is_unicode_type(PyTypeObject* type, unsigned long flags) {
    return type == &PyUnicode_Type || (flags & Py_TPFLAGS_UNICODE_SUBCLASS);
}

static inline bool is_tuple_type(PyTypeObject* type, unsigned long flags) {
    return type == &PyTuple_Type || (flags & Py_TPFLAGS_TUPLE_SUBCLASS);
}

static inline bool is_container_fast(PyTypeObject* type, unsigned long flags) {
    return type == &PyDict_Type || type == &PyList_Type || is_tuple_type(type, flags);
}

static constexpr int kSmallIntMin = -5;
static constexpr int kSmallIntMax = 256;
static constexpr size_t kSmallIntCount =
    static_cast<size_t>(kSmallIntMax - kSmallIntMin + 1);

struct SmallIntTable {
    std::array<std::array<char, 4>, kSmallIntCount> data{};
    std::array<uint8_t, kSmallIntCount> len{};

    SmallIntTable() {
        for (int value = kSmallIntMin; value <= kSmallIntMax; ++value) {
            char buf[8];
            int written = std::snprintf(buf, sizeof(buf), "%d", value);
            if (written <= 0) {
                continue;
            }
            size_t idx = static_cast<size_t>(value - kSmallIntMin);
            len[idx] = static_cast<uint8_t>(written);
            std::memcpy(data[idx].data(), buf, static_cast<size_t>(written));
        }
    }
};

static const SmallIntTable kSmallIntTable;

template <typename Buffer> static inline bool append_small_int(Buffer& out, int64_t value) {
    if (value < kSmallIntMin || value > kSmallIntMax) {
        return false;
    }
    size_t idx = static_cast<size_t>(value - kSmallIntMin);
    out.append(kSmallIntTable.data[idx].data(), kSmallIntTable.len[idx]);
    return true;
}

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
        int64_t value = static_cast<int64_t>(val);
        if (append_small_int(out, value)) {
            return true;
        }
        return append_int64(out, value);
    }
#endif

    int overflow = 0;
    int64_t val = PyLong_AsLongLongAndOverflow(obj, &overflow);
    if (overflow == 0) {
        if (append_small_int(out, val)) {
            return true;
        }
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

enum class PrimitiveResult { Handled, IsContainer, Error };

template <typename Buffer>
static inline PrimitiveResult serialize_primitive(PyObject* obj, Buffer& out, bool ints_first) {
    if (obj == Py_None) {
        append_literal(out, "null", 4);
        return PrimitiveResult::Handled;
    }
    if (obj == Py_True) {
        append_literal(out, "true", 4);
        return PrimitiveResult::Handled;
    }
    if (obj == Py_False) {
        append_literal(out, "false", 5);
        return PrimitiveResult::Handled;
    }
    PyTypeObject* type = Py_TYPE(obj);
    unsigned long flags = type->tp_flags;
    if (ints_first) {
        if (type == &PyLong_Type) {
            return append_py_long(out, obj) ? PrimitiveResult::Handled : PrimitiveResult::Error;
        }
        if (is_unicode_type(type, flags)) {
            return append_string(obj, out) ? PrimitiveResult::Handled : PrimitiveResult::Error;
        }
    } else {
        if (is_unicode_type(type, flags)) {
            return append_string(obj, out) ? PrimitiveResult::Handled : PrimitiveResult::Error;
        }
        if (type == &PyLong_Type) {
            return append_py_long(out, obj) ? PrimitiveResult::Handled : PrimitiveResult::Error;
        }
    }
    if (type == &PyFloat_Type) {
        return append_double(out, PyFloat_AS_DOUBLE(obj)) ? PrimitiveResult::Handled
                                                          : PrimitiveResult::Error;
    }

    if (is_container_fast(type, flags)) {
        return PrimitiveResult::IsContainer;
    }

    PyErr_SetString(PyExc_TypeError,
                    "Object of unsupported type cannot be serialized to JSON");
    return PrimitiveResult::Error;
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
static constexpr Py_ssize_t kEstimateSampleLimit = 8;

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

struct EstimateCache {
    std::array<std::unordered_map<PyObject*, size_t>, kMaxEstimateDepth + 1> by_depth;

    bool lookup(PyObject* obj, int depth, size_t& out) {
        if (depth > kMaxEstimateDepth) {
            return false;
        }
        auto& map = by_depth[depth];
        auto it = map.find(obj);
        if (it == map.end()) {
            return false;
        }
        out = it->second;
        return true;
    }

    void store(PyObject* obj, int depth, size_t value) {
        if (depth > kMaxEstimateDepth) {
            return;
        }
        by_depth[depth].emplace(obj, value);
    }
};

static inline size_t estimate_unicode_bytes(PyObject* obj) {
    size_t len = static_cast<size_t>(PyUnicode_GET_LENGTH(obj));
    if (PyUnicode_IS_COMPACT_ASCII(obj)) {
        return len;
    }
    Py_UCS4 max_char = PyUnicode_MAX_CHAR_VALUE(obj);
    size_t bytes_per = 1;
    if (max_char <= 0x7F) {
        bytes_per = 1;
    } else if (max_char <= 0x7FF) {
        bytes_per = 2;
    } else if (max_char <= 0xFFFF) {
        bytes_per = 3;
    } else {
        bytes_per = 4;
    }
    return clamp_mul(len, bytes_per, kMaxEstimate);
}

static inline size_t estimate_value_size(PyObject* obj, int depth, EstimateCache& cache);

static inline size_t estimate_sequence_size(PyObject* obj,
                                            Py_ssize_t size,
                                            int depth,
                                            bool is_tuple,
                                            EstimateCache& cache) {
    if (size <= 0) {
        return kContainerOverhead;
    }
    size_t per_entry = kListEntryEstimate;
    if (depth < kMaxEstimateDepth) {
        PyObject* first = is_tuple ? PyTuple_GET_ITEM(obj, 0) : PyList_GET_ITEM(obj, 0);
        if (first) {
            size_t first_est = estimate_value_size(first, depth + 1, cache);
            if (first_est > per_entry) {
                per_entry = first_est;
            }
        }
    }
    per_entry = clamp_add(per_entry, 1, kMaxEstimate);
    return clamp_add(clamp_mul(static_cast<size_t>(size), per_entry, kMaxEstimate), kContainerOverhead,
                     kMaxEstimate);
}

static inline size_t estimate_dict_size(PyObject* obj, int depth, EstimateCache& cache) {
    Py_ssize_t size = PyDict_GET_SIZE(obj);
    if (size <= 0) {
        return kContainerOverhead;
    }
    size_t per_entry = kDictEntryEstimate;
    if (depth < kMaxEstimateDepth) {
        PyObject* key = nullptr;
        PyObject* value = nullptr;
        Py_ssize_t pos = 0;
        size_t key_total = 0;
        size_t value_total = 0;
        Py_ssize_t samples = 0;
        while (samples < kEstimateSampleLimit && PyDict_Next(obj, &pos, &key, &value)) {
            if (key != nullptr) {
                PyTypeObject* key_type = Py_TYPE(key);
                unsigned long key_flags = key_type->tp_flags;
                if (is_unicode_type(key_type, key_flags)) {
                    size_t key_len = clamp_add(estimate_unicode_bytes(key), kStringOverhead, kMaxEstimate);
                    key_total = clamp_add(key_total, key_len, kMaxEstimate);
                } else {
                    key_total = clamp_add(key_total, kStringOverhead, kMaxEstimate);
                }
            }
            if (value != nullptr) {
                size_t value_est = estimate_value_size(value, depth + 1, cache);
                value_total = clamp_add(value_total, value_est, kMaxEstimate);
            }
            ++samples;
        }
        if (samples > 0) {
            size_t avg_key = key_total / static_cast<size_t>(samples);
            size_t avg_val = value_total / static_cast<size_t>(samples);
            per_entry = clamp_add(avg_key, avg_val, kMaxEstimate);
            per_entry = clamp_add(per_entry, 2, kMaxEstimate);
            if (per_entry < kDictEntryEstimate) {
                per_entry = kDictEntryEstimate;
            }
        }
    }
    return clamp_add(clamp_mul(static_cast<size_t>(size), per_entry, kMaxEstimate), kContainerOverhead,
                     kMaxEstimate);
}

static inline size_t estimate_value_size(PyObject* obj, int depth, EstimateCache& cache) {
    PyTypeObject* type = Py_TYPE(obj);
    unsigned long flags = type->tp_flags;
    size_t cached = 0;
    if (type == &PyDict_Type) {
        if (cache.lookup(obj, depth, cached)) {
            return cached;
        }
        size_t estimate = estimate_dict_size(obj, depth, cache);
        cache.store(obj, depth, estimate);
        return estimate;
    }
    if (type == &PyList_Type) {
        if (cache.lookup(obj, depth, cached)) {
            return cached;
        }
        size_t estimate =
            estimate_sequence_size(obj, PyList_GET_SIZE(obj), depth, false, cache);
        cache.store(obj, depth, estimate);
        return estimate;
    }
    if (is_tuple_type(type, flags)) {
        if (cache.lookup(obj, depth, cached)) {
            return cached;
        }
        size_t estimate =
            estimate_sequence_size(obj, PyTuple_GET_SIZE(obj), depth, true, cache);
        cache.store(obj, depth, estimate);
        return estimate;
    }
    if (is_unicode_type(type, flags)) {
        return clamp_add(estimate_unicode_bytes(obj), kStringOverhead, kMaxEstimate);
    }
    return kStringOverhead;
}

// Depth-limited size estimate with one-element sampling.
static inline size_t estimate_size(PyObject* obj) {
    EstimateCache cache;
    size_t estimate = estimate_value_size(obj, 0, cache);
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

    // Cycle detection scans open frames back-to-front and is skipped for shallow depths.
    const bool check_cycles = (g_cycle_policy != CyclePolicy::NoCheck);
    constexpr size_t kCycleCheckDepth = 1;
    size_t nesting_depth = 0;
    const bool ints_first = (g_dumps_type_order == DumpsTypeOrder::IntsFirst);

    PyObject* current = root;
    bool current_is_container = false;

    auto has_cycle = [&stack](PyObject* ptr) -> bool {
        for (size_t i = stack.size(); i-- > 0;) {
            if (stack[i].obj == ptr) {
                return true;
            }
        }
        return false;
    };

    while (true) {
        if (current) {
            PyTypeObject* type = Py_TYPE(current);
            unsigned long flags = type->tp_flags;
            if (!current_is_container) {
                // Fast path for primitives first (avoid is_container check for most common types)
                // String vs int order can be configured to match workload profiles.
                if (ints_first) {
                    if (LIKELY(type == &PyLong_Type)) {
                        if (!append_py_long(out, current)) {
                            return false;
                        }
                        current = nullptr;
                        continue;
                    }
                    if (LIKELY(type == &PyUnicode_Type)) {
                        if (!append_string(current, out)) {
                            return false;
                        }
                        current = nullptr;
                        continue;
                    }
                    if (UNLIKELY(flags & Py_TPFLAGS_UNICODE_SUBCLASS)) {
                        if (!append_string(current, out)) {
                            return false;
                        }
                        current = nullptr;
                        continue;
                    }
                } else {
                    if (LIKELY(type == &PyUnicode_Type)) {
                        if (!append_string(current, out)) {
                            return false;
                        }
                        current = nullptr;
                        continue;
                    }
                    if (UNLIKELY(flags & Py_TPFLAGS_UNICODE_SUBCLASS)) {
                        if (!append_string(current, out)) {
                            return false;
                        }
                        current = nullptr;
                        continue;
                    }
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

                if (current == Py_True) {
                    append_literal(out, "true", 4);
                    current = nullptr;
                    continue;
                }

                if (current == Py_False) {
                    append_literal(out, "false", 5);
                    current = nullptr;
                    continue;
                }
            }

            // Now check containers (need cycle detection)
            bool container = current_is_container;
            if (!container) {
                container = is_container_fast(type, flags);
            }
            current_is_container = false;
            if (container && check_cycles && nesting_depth >= kCycleCheckDepth) {
                if (has_cycle(current)) {
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
            }

            if (LIKELY(type == &PyDict_Type)) {
                Py_ssize_t size = PyDict_GET_SIZE(current);
                out.ensure_extra(structural_budget(Frame::Type::Dict, size));
                if (size == 0) {
                    out.push_back_unchecked('{');
                    out.push_back_unchecked('}');
                    current = nullptr;
                    continue;
                }

                out.push_back_unchecked('{');
                stack.push_back(Frame{Frame::Type::Dict, current, 0, size, 0, true});
                ++nesting_depth;
                current = nullptr;
                continue;
            }

            if (LIKELY(type == &PyList_Type)) {
                Py_ssize_t size = PyList_GET_SIZE(current);
                out.ensure_extra(structural_budget(Frame::Type::List, size));
                if (size == 0) {
                    out.push_back_unchecked('[');
                    out.push_back_unchecked(']');
                    current = nullptr;
                    continue;
                }

                out.push_back_unchecked('[');
                stack.push_back(Frame{Frame::Type::List, current, 0, size, 0, true});
                ++nesting_depth;
                current = nullptr;
                continue;
            }

            if (is_tuple_type(type, flags)) {
                Py_ssize_t size = PyTuple_GET_SIZE(current);
                out.ensure_extra(structural_budget(Frame::Type::Tuple, size));
                if (size == 0) {
                    out.push_back_unchecked('[');
                    out.push_back_unchecked(']');
                    current = nullptr;
                    continue;
                }

                out.push_back_unchecked('[');
                stack.push_back(Frame{Frame::Type::Tuple, current, 0, size, 0, true});
                ++nesting_depth;
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

                PyTypeObject* key_type = Py_TYPE(key);
                if (!is_unicode_type(key_type, key_type->tp_flags)) {
                    PyErr_SetString(PyExc_TypeError, "Dict keys must be strings");
                    return false;
                }
                if (!append_string(key, out)) {
                    return false;
                }
                out.push_back_unchecked(':');
                PrimitiveResult result = serialize_primitive(value, out, ints_first);
                if (result == PrimitiveResult::Handled) {
                    continue;
                }
                if (result == PrimitiveResult::Error) {
                    return false;
                }
                current = value;
                current_is_container = true;
            } else {
                out.push_back_unchecked('}');
                stack.pop_back();
                --nesting_depth;
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
            PrimitiveResult result = serialize_primitive(item, out, ints_first);
            if (result == PrimitiveResult::Handled) {
                continue;
            }
            if (result == PrimitiveResult::Error) {
                return false;
            }
            current = item;
            current_is_container = true;
            continue;
        }

        out.push_back_unchecked(']');
        stack.pop_back();
        --nesting_depth;
    }

    return true;
}

static PyObject* dumps_str_impl(PyObject* obj) {
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

// Python dumps() function
PyObject* strata_dumps(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* obj = nullptr;
    PyObject* return_type_obj = Py_None;

    static const char* kwlist[] = {"obj", "return_type", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|O", const_cast<char**>(kwlist), &obj,
                                     &return_type_obj)) {
        return NULL;
    }

    STRATA_CPP_TRY

    ReturnType return_type = ReturnType::Str;
    if (!parse_return_type(return_type_obj, &return_type, ReturnType::Str)) {
        return NULL;
    }

    if (return_type == ReturnType::Str) {
        return dumps_str_impl(obj);
    }
    if (return_type == ReturnType::Bytes) {
        return strata_dumps_bytes(self, obj);
    }
    PyObject* bytes_obj = strata_dumps_bytes(self, obj);
    if (!bytes_obj) {
        return NULL;
    }
    char* data = nullptr;
    Py_ssize_t len = 0;
    if (PyBytes_AsStringAndSize(bytes_obj, &data, &len) < 0) {
        Py_DECREF(bytes_obj);
        return NULL;
    }
    PyObject* bytearray_obj = PyByteArray_FromStringAndSize(data, len);
    Py_DECREF(bytes_obj);
    return bytearray_obj;

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

namespace {
PyObject* serialize_record(PyObject* value, ReturnType return_type) {
    if (return_type == ReturnType::Str) {
        return dumps_str_impl(value);
    }
    if (return_type == ReturnType::Bytes) {
        return strata_dumps_bytes(nullptr, value);
    }
    PyObject* bytes_obj = strata_dumps_bytes(nullptr, value);
    if (!bytes_obj) {
        return NULL;
    }
    char* data = nullptr;
    Py_ssize_t len = 0;
    if (PyBytes_AsStringAndSize(bytes_obj, &data, &len) < 0) {
        Py_DECREF(bytes_obj);
        return NULL;
    }
    PyObject* bytearray_obj = PyByteArray_FromStringAndSize(data, len);
    Py_DECREF(bytes_obj);
    return bytearray_obj;
}

bool write_payload_file(std::FILE* file, PyObject* payload, ReturnType return_type) {
    const char* data = nullptr;
    Py_ssize_t len = 0;
    if (return_type == ReturnType::Str) {
        data = PyUnicode_AsUTF8AndSize(payload, &len);
        if (!data) {
            return false;
        }
    } else if (return_type == ReturnType::Bytes) {
        char* bytes = nullptr;
        if (PyBytes_AsStringAndSize(payload, &bytes, &len) < 0) {
            return false;
        }
        data = bytes;
    } else {
        data = PyByteArray_AsString(payload);
        if (!data) {
            return false;
        }
        len = PyByteArray_Size(payload);
    }

    if (len > 0) {
        size_t written = std::fwrite(data, 1, static_cast<size_t>(len), file);
        if (written != static_cast<size_t>(len)) {
            PyErr_SetFromErrno(PyExc_OSError);
            return false;
        }
    }
    return true;
}

bool write_newline_file(std::FILE* file) {
    const char newline = '\n';
    size_t written = std::fwrite(&newline, 1, 1, file);
    if (written != 1) {
        PyErr_SetFromErrno(PyExc_OSError);
        return false;
    }
    return true;
}

bool write_payload_filelike(PyObject* target, PyObject* payload) {
    PyObject* result = PyObject_CallMethod(target, "write", "O", payload);
    if (!result) {
        return false;
    }
    Py_DECREF(result);
    return true;
}
} // namespace

// Python dump() function
PyObject* strata_dump(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* target = nullptr;
    PyObject* obj = nullptr;
    PyObject* ndjson_obj = Py_None;
    PyObject* return_type_obj = Py_None;

    static const char* kwlist[] = {"target", "obj", "ndjson", "return_type", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|OO", const_cast<char**>(kwlist), &target,
                                     &obj, &ndjson_obj, &return_type_obj)) {
        return NULL;
    }

    STRATA_CPP_TRY

    bool ndjson_flag_set = false;
    bool ndjson_flag = false;
    if (ndjson_obj != Py_None) {
        int truth = PyObject_IsTrue(ndjson_obj);
        if (truth < 0) {
            return NULL;
        }
        ndjson_flag_set = true;
        ndjson_flag = truth != 0;
    }

    PyObject* pathlike = PyOS_FSPath(target);
    bool target_is_path = pathlike != nullptr;
    if (!target_is_path) {
        PyErr_Clear();
    }

    bool use_ndjson = false;
    if (ndjson_flag_set) {
        use_ndjson = ndjson_flag;
    } else if (target_is_path) {
        use_ndjson = is_ndjson_name(pathlike);
        if (PyErr_Occurred()) {
            Py_DECREF(pathlike);
            return NULL;
        }
    } else {
        use_ndjson = detect_ndjson_name_attr(target);
        if (PyErr_Occurred()) {
            return NULL;
        }
    }

    ReturnType default_type = ReturnType::Bytes;
    if (!target_is_path && is_text_io(target)) {
        default_type = ReturnType::Str;
    }
    ReturnType return_type = ReturnType::Str;
    if (!parse_return_type(return_type_obj, &return_type, default_type)) {
        Py_XDECREF(pathlike);
        return NULL;
    }

    if (!target_is_path && return_type != ReturnType::Str && is_text_io(target)) {
        Py_XDECREF(pathlike);
        PyErr_SetString(PyExc_TypeError,
                        "file-like object expects str; use return_type='str'");
        return NULL;
    }

    size_t line_count = 0;

    if (target_is_path) {
        const char* filepath = nullptr;
        if (PyUnicode_Check(pathlike)) {
            filepath = PyUnicode_AsUTF8(pathlike);
        } else if (PyBytes_Check(pathlike)) {
            filepath = PyBytes_AsString(pathlike);
        } else {
            Py_DECREF(pathlike);
            PyErr_SetString(PyExc_TypeError, "path must be str or bytes");
            return NULL;
        }
        if (!filepath) {
            Py_DECREF(pathlike);
            return NULL;
        }

        const char* mode = (return_type == ReturnType::Str) ? "w" : "wb";
        std::FILE* file = std::fopen(filepath, mode);
        if (!file) {
            Py_DECREF(pathlike);
            PyErr_SetFromErrnoWithFilename(PyExc_OSError, filepath);
            return NULL;
        }

        if (use_ndjson) {
            if (PyDict_Check(obj)) {
                PyObject* payload = serialize_record(obj, return_type);
                if (!payload) {
                    std::fclose(file);
                    Py_DECREF(pathlike);
                    return NULL;
                }
                if (!write_payload_file(file, payload, return_type) || !write_newline_file(file)) {
                    Py_DECREF(payload);
                    std::fclose(file);
                    Py_DECREF(pathlike);
                    return NULL;
                }
                Py_DECREF(payload);
                line_count = 1;
            } else if (PyUnicode_Check(obj) || PyBytes_Check(obj) || PyByteArray_Check(obj)) {
                std::fclose(file);
                Py_DECREF(pathlike);
                PyErr_SetString(PyExc_TypeError,
                                "ndjson dump expects JSON-serializable objects, not raw text");
                return NULL;
            } else {
                PyObject* iter = PyObject_GetIter(obj);
                if (!iter) {
                    if (!PyErr_ExceptionMatches(PyExc_TypeError)) {
                        std::fclose(file);
                        Py_DECREF(pathlike);
                        return NULL;
                    }
                    PyErr_Clear();
                    PyObject* payload = serialize_record(obj, return_type);
                    if (!payload) {
                        std::fclose(file);
                        Py_DECREF(pathlike);
                        return NULL;
                    }
                    if (!write_payload_file(file, payload, return_type) ||
                        !write_newline_file(file)) {
                        Py_DECREF(payload);
                        std::fclose(file);
                        Py_DECREF(pathlike);
                        return NULL;
                    }
                    Py_DECREF(payload);
                    line_count = 1;
                } else {
                    PyObject* item;
                    while ((item = PyIter_Next(iter))) {
                        PyObject* payload = serialize_record(item, return_type);
                        Py_DECREF(item);
                        if (!payload) {
                            Py_DECREF(iter);
                            std::fclose(file);
                            Py_DECREF(pathlike);
                            return NULL;
                        }
                        if (!write_payload_file(file, payload, return_type) ||
                            !write_newline_file(file)) {
                            Py_DECREF(payload);
                            Py_DECREF(iter);
                            std::fclose(file);
                            Py_DECREF(pathlike);
                            return NULL;
                        }
                        Py_DECREF(payload);
                        line_count++;
                    }
                    Py_DECREF(iter);
                    if (PyErr_Occurred()) {
                        std::fclose(file);
                        Py_DECREF(pathlike);
                        return NULL;
                    }
                }
            }
        } else {
            PyObject* payload = serialize_record(obj, return_type);
            if (!payload) {
                std::fclose(file);
                Py_DECREF(pathlike);
                return NULL;
            }
            if (!write_payload_file(file, payload, return_type)) {
                Py_DECREF(payload);
                std::fclose(file);
                Py_DECREF(pathlike);
                return NULL;
            }
            Py_DECREF(payload);
            line_count = 1;
        }

        std::fclose(file);
        Py_DECREF(pathlike);
        return PyLong_FromSize_t(line_count);
    }

    int has_write = PyObject_HasAttrString(target, "write");
    if (has_write < 0) {
        return NULL;
    }
    if (!has_write) {
        PyErr_SetString(PyExc_TypeError, "dump() expects a path or file-like object");
        return NULL;
    }

    PyObject* newline_obj = nullptr;
    if (use_ndjson) {
        if (return_type == ReturnType::Str) {
            newline_obj = PyUnicode_FromString("\n");
        } else if (return_type == ReturnType::Bytes) {
            newline_obj = PyBytes_FromString("\n");
        } else {
            newline_obj = PyByteArray_FromStringAndSize("\n", 1);
        }
        if (!newline_obj) {
            return NULL;
        }
    }

    if (use_ndjson) {
        if (PyDict_Check(obj)) {
            PyObject* payload = serialize_record(obj, return_type);
            if (!payload) {
                Py_XDECREF(newline_obj);
                return NULL;
            }
            if (!write_payload_filelike(target, payload) ||
                !write_payload_filelike(target, newline_obj)) {
                Py_DECREF(payload);
                Py_XDECREF(newline_obj);
                return NULL;
            }
            Py_DECREF(payload);
            line_count = 1;
        } else if (PyUnicode_Check(obj) || PyBytes_Check(obj) || PyByteArray_Check(obj)) {
            Py_XDECREF(newline_obj);
            PyErr_SetString(PyExc_TypeError,
                            "ndjson dump expects JSON-serializable objects, not raw text");
            return NULL;
        } else {
            PyObject* iter = PyObject_GetIter(obj);
            if (!iter) {
                if (!PyErr_ExceptionMatches(PyExc_TypeError)) {
                    Py_XDECREF(newline_obj);
                    return NULL;
                }
                PyErr_Clear();
                PyObject* payload = serialize_record(obj, return_type);
                if (!payload) {
                    Py_XDECREF(newline_obj);
                    return NULL;
                }
                if (!write_payload_filelike(target, payload) ||
                    !write_payload_filelike(target, newline_obj)) {
                    Py_DECREF(payload);
                    Py_XDECREF(newline_obj);
                    return NULL;
                }
                Py_DECREF(payload);
                line_count = 1;
            } else {
                PyObject* item;
                while ((item = PyIter_Next(iter))) {
                    PyObject* payload = serialize_record(item, return_type);
                    Py_DECREF(item);
                    if (!payload) {
                        Py_DECREF(iter);
                        Py_XDECREF(newline_obj);
                        return NULL;
                    }
                    if (!write_payload_filelike(target, payload) ||
                        !write_payload_filelike(target, newline_obj)) {
                        Py_DECREF(payload);
                        Py_DECREF(iter);
                        Py_XDECREF(newline_obj);
                        return NULL;
                    }
                    Py_DECREF(payload);
                    line_count++;
                }
                Py_DECREF(iter);
                if (PyErr_Occurred()) {
                    Py_XDECREF(newline_obj);
                    return NULL;
                }
            }
        }
    } else {
        PyObject* payload = serialize_record(obj, return_type);
        if (!payload) {
            Py_XDECREF(newline_obj);
            return NULL;
        }
        if (!write_payload_filelike(target, payload)) {
            Py_DECREF(payload);
            Py_XDECREF(newline_obj);
            return NULL;
        }
        Py_DECREF(payload);
        line_count = 1;
    }

    Py_XDECREF(newline_obj);
    return PyLong_FromSize_t(line_count);

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

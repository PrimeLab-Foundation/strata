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

#if defined(__ARM_NEON) || defined(__aarch64__)
#include <arm_neon.h>
#define STRATA_SERIALIZE_HAS_NEON 1
#elif defined(__SSE2__)
#include <emmintrin.h>
#endif

// Thread-local buffer for zero-allocation serialization
thread_local strata::util::OutputBuffer g_serialize_buffer;

// Create a PyUnicode string from a buffer.  When the content is pure ASCII
// (the common case for JSON output), we skip CPython's internal codepoint
// scan by constructing the compact-ASCII representation directly.
static inline PyObject* buffer_to_pyunicode(const char* data, size_t len) {
    // Quick ASCII check: scan for any byte with high bit set.
    bool is_ascii = true;
    size_t i = 0;
#if defined(STRATA_SERIALIZE_HAS_NEON)
    for (; i + 16 <= len; i += 16) {
        uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(data + i));
        // Any byte >= 0x80 means non-ASCII
        if (vmaxvq_u8(chunk) >= 0x80) {
            is_ascii = false;
            break;
        }
    }
#elif defined(__SSE2__)
    for (; i + 16 <= len; i += 16) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + i));
        if (_mm_movemask_epi8(chunk) != 0) {
            is_ascii = false;
            break;
        }
    }
#endif
    if (is_ascii) {
        for (; i < len; ++i) {
            if (static_cast<unsigned char>(data[i]) >= 0x80) {
                is_ascii = false;
                break;
            }
        }
    }

    if (is_ascii) {
        Py_ssize_t slen = static_cast<Py_ssize_t>(len);
        PyObject* result = PyUnicode_New(slen, 127);
        if (!result)
            return NULL;
        memcpy(PyUnicode_1BYTE_DATA(result), data, len);
        return result;
    }
    // Non-ASCII path: full UTF-8 decode
    return PyUnicode_DecodeUTF8(data, static_cast<Py_ssize_t>(len), NULL);
}

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

    // Reserve 12 bytes for the fast path (max 11 chars: -2147483648).
    out.reserve(out.size() + 12);

    // Fast path covers full int32 range (±2,147,483,647) using digit-pair
    // decomposition. This handles virtually all JSON integers without
    // falling through to std::to_chars.
    if (LIKELY(value >= -2147483647LL && value <= 2147483647LL)) {
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

        // 3-4 digit numbers
        if (v < 10000) {
            uint32_t high = v / 100;
            uint32_t low = v - high * 100;
            if (high < 10) {
                out.unsafe_push_back(static_cast<char>('0' + high));
            } else {
                out.unsafe_append(kDigitPairs + high * 2, 2);
            }
            out.unsafe_append(kDigitPairs + low * 2, 2);
            return true;
        }

        // 5-6 digit numbers
        if (v < 1000000) {
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
            return true;
        }

        // 7-10 digit numbers (covers full int32 range)
        // Decompose: v = top * 10000_0000 + mid_hi * 10000 + mid_lo * 100 + low
        if (v < 100000000) {
            // 7-8 digits
            uint32_t hi4 = v / 10000;
            uint32_t lo4 = v - hi4 * 10000;
            uint32_t p1 = hi4 / 100;
            uint32_t p2 = hi4 - p1 * 100;
            uint32_t p3 = lo4 / 100;
            uint32_t p4 = lo4 - p3 * 100;
            if (p1 < 10) {
                out.unsafe_push_back(static_cast<char>('0' + p1));
            } else {
                out.unsafe_append(kDigitPairs + p1 * 2, 2);
            }
            out.unsafe_append(kDigitPairs + p2 * 2, 2);
            out.unsafe_append(kDigitPairs + p3 * 2, 2);
            out.unsafe_append(kDigitPairs + p4 * 2, 2);
        } else {
            // 9-10 digits
            uint32_t hi = v / 100000000;
            uint32_t lo8 = v - hi * 100000000;
            uint32_t hi4 = lo8 / 10000;
            uint32_t lo4 = lo8 - hi4 * 10000;
            uint32_t p1 = hi4 / 100;
            uint32_t p2 = hi4 - p1 * 100;
            uint32_t p3 = lo4 / 100;
            uint32_t p4 = lo4 - p3 * 100;
            if (hi < 10) {
                out.unsafe_push_back(static_cast<char>('0' + hi));
            } else {
                out.unsafe_append(kDigitPairs + hi * 2, 2);
            }
            out.unsafe_append(kDigitPairs + p1 * 2, 2);
            out.unsafe_append(kDigitPairs + p2 * 2, 2);
            out.unsafe_append(kDigitPairs + p3 * 2, 2);
            out.unsafe_append(kDigitPairs + p4 * 2, 2);
        }
        return true;
    }

    // Large ints (beyond int32 range): need up to 21 bytes (sign + 20 digits)
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
    if (value >= -999999999999999.0 && value <= 999999999999999.0) {
        int64_t ival = static_cast<int64_t>(value);
        if (static_cast<double>(ival) == value && LIKELY(ival != 0 || !std::signbit(value))) {
            append_int64(out, ival);
            out.append(".0", 2);
            return true;
        }
    }

    // Use Ryu algorithm for non-integer floats
    out.reserve(out.size() + 36);
    char* start = out.data() + out.size();
    char* p = start;

    if (value < 0) {
        *p++ = '-';
        value = -value;
    }

    int len = strata::util::dragonbox_d2s(value, p);
    out.unsafe_advance(static_cast<size_t>((p - start) + len));
    return true;
}

template <typename Buffer> static inline bool append_string(PyObject* obj, Buffer& out) {
    if (LIKELY(PyUnicode_IS_COMPACT_ASCII(obj))) {
        const Py_ssize_t len = PyUnicode_GET_LENGTH(obj);
        const char* data = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(obj));
        const size_t ulen = static_cast<size_t>(len);

        // Reserve space for quotes + data (worst case: no escapes)
        out.reserve(out.size() + ulen + 2);

        // Single-pass: write directly to buffer memory, only advance size_ once at end.
        // If escapes found, size_ is unchanged so partial writes are harmless.
        char* dest = out.data() + out.size();
        char* const dest_start = dest;
        *dest++ = '"';

        size_t i = 0;

#ifdef STRATA_SERIALIZE_HAS_NEON
        // Short string fast path: check 16 bytes at once even for strings < 16 chars.
        // We pad-read up to 16 bytes (safe because compact ASCII strings are null-terminated
        // and part of a PyObject allocation that's always >= 16 bytes after the data).
        if (ulen <= 16 && ulen > 0) {
            uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(data));
            const uint8x16_t thr = vdupq_n_u8(0x20);
            const uint8x16_t qv = vdupq_n_u8('"');
            const uint8x16_t bv = vdupq_n_u8('\\');
            uint8x16_t esc =
                vorrq_u8(vcltq_u8(chunk, thr), vorrq_u8(vceqq_u8(chunk, qv), vceqq_u8(chunk, bv)));
            // Mask out bytes beyond the string length
            // Create mask: 0xFF for positions < ulen, 0x00 for positions >= ulen
            static const uint8_t mask_data[32] __attribute__((aligned(16))) = {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            uint8x16_t mask = vld1q_u8(mask_data + (16 - ulen));
            esc = vandq_u8(esc, mask);
            if (LIKELY(vmaxvq_u8(esc) == 0)) {
                std::memcpy(dest, data, ulen);
                dest += ulen;
                *dest++ = '"';
                out.unsafe_advance(static_cast<size_t>(dest - dest_start));
                return true;
            }
            // Has escapes — fall through to full handler
            strata::util::escape_json_string_simd(data, ulen, out);
            return true;
        }

        {
            const uint8x16_t thr = vdupq_n_u8(0x20);
            const uint8x16_t qv = vdupq_n_u8('"');
            const uint8x16_t bv = vdupq_n_u8('\\');

            for (; i + 16 <= ulen; i += 16) {
                uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(data + i));
                uint8x16_t esc = vorrq_u8(vcltq_u8(chunk, thr),
                                          vorrq_u8(vceqq_u8(chunk, qv), vceqq_u8(chunk, bv)));
                if (UNLIKELY(vmaxvq_u8(esc) != 0))
                    goto has_escapes;
                vst1q_u8(reinterpret_cast<uint8_t*>(dest), chunk);
                dest += 16;
            }
        }
#endif
        // Scalar tail
        for (; i < ulen; ++i) {
            unsigned char c = static_cast<unsigned char>(data[i]);
            if (UNLIKELY(c < 0x20 || c == '"' || c == '\\'))
                goto has_escapes;
            *dest++ = data[i];
        }

        *dest++ = '"';
        out.unsafe_advance(static_cast<size_t>(dest - dest_start));
        return true;

    has_escapes:
        // Fall back to full escape handler — size_ is unchanged
        strata::util::escape_json_string_simd(data, ulen, out);
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

// Write a dict key using direct pointer writes to avoid size_ dependency chain.
// Combines comma+quote+key+quote+colon into a single pointer advancement.
template <typename Buffer>
static inline bool write_dict_key(PyObject* key, Buffer& out, bool first) {
    if (LIKELY(PyUnicode_IS_COMPACT_ASCII(key))) {
        const Py_ssize_t klen = PyUnicode_GET_LENGTH(key);
        const char* kdata = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(key));
        const size_t need = static_cast<size_t>(klen) + (first ? 3 : 4);
        out.reserve(out.size() + need);

        // Direct pointer writes — avoids 4-5 separate size_ updates
        char* p = out.data() + out.size();
        if (!first) {
            *p++ = ',';
        }

        // NEON escape check for keys (most keys are < 16 chars, no escapes)
        bool key_clean = true;
#ifdef STRATA_SERIALIZE_HAS_NEON
        if (LIKELY(klen <= 16 && klen > 0)) {
            uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(kdata));
            const uint8x16_t thr = vdupq_n_u8(0x20);
            const uint8x16_t qv = vdupq_n_u8('"');
            const uint8x16_t bv = vdupq_n_u8('\\');
            uint8x16_t esc =
                vorrq_u8(vcltq_u8(chunk, thr), vorrq_u8(vceqq_u8(chunk, qv), vceqq_u8(chunk, bv)));
            static const uint8_t mask_data[32] __attribute__((aligned(16))) = {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            uint8x16_t mask = vld1q_u8(mask_data + (16 - klen));
            esc = vandq_u8(esc, mask);
            key_clean = (vmaxvq_u8(esc) == 0);
        } else
#endif
        {
            for (Py_ssize_t ki = 0; ki < klen; ++ki) {
                unsigned char c = static_cast<unsigned char>(kdata[ki]);
                if (UNLIKELY(c < 0x20 || c == '"' || c == '\\')) {
                    key_clean = false;
                    break;
                }
            }
        }

        if (LIKELY(key_clean)) {
            *p++ = '"';
            std::memcpy(p, kdata, static_cast<size_t>(klen));
            p += klen;
            *p++ = '"';
            *p++ = ':';
            out.unsafe_advance(static_cast<size_t>(p - out.data()) - out.size());
        } else {
            // Advance past comma we already wrote
            out.unsafe_advance(static_cast<size_t>(p - out.data()) - out.size());
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

// Template-parameterized serializers: Tracking=true compiles in cycle detection,
// Tracking=false eliminates all cycle-checking branches at compile time.
// This gives ~5-10% speedup on the common (no-tracking) path.

template <bool Tracking, typename Buffer>
static inline bool serialize_dict_t(PyObject* dict, Buffer& out, int depth);
template <bool Tracking, typename Buffer>
static inline bool serialize_list_t(PyObject* list, Buffer& out, int depth);

template <bool Tracking, typename Buffer>
static inline bool serialize_item_t(PyObject* item, Buffer& out, int depth) {
    PyTypeObject* vt = Py_TYPE(item);
    if (LIKELY(vt == &PyUnicode_Type)) {
        return append_string(item, out);
    }
    if (LIKELY(vt == &PyLong_Type)) {
        return append_py_long(out, item);
    }
    if (vt == &PyFloat_Type) {
        return append_double(out, PyFloat_AS_DOUBLE(item));
    }
    if (vt == &PyDict_Type) {
        if (UNLIKELY(depth >= get_max_serialize_depth())) {
            PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
            return false;
        }
        return serialize_dict_t<Tracking>(item, out, depth + 1);
    }
    if (vt == &PyList_Type) {
        if (UNLIKELY(depth >= get_max_serialize_depth())) {
            PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
            return false;
        }
        return serialize_list_t<Tracking>(item, out, depth + 1);
    }
    if (UNLIKELY(item == Py_None)) {
        out.append("null", 4);
        return true;
    }
    if (UNLIKELY(vt == &PyBool_Type)) {
        out.append(item == Py_True ? "true" : "false", item == Py_True ? 4 : 5);
        return true;
    }
    // Fallback for tuples and subtypes
    return serialize_value(item, out, depth);
}

template <bool Tracking, typename Buffer>
static inline bool serialize_dict_t(PyObject* dict, Buffer& out, int depth) {
    if constexpr (Tracking) {
        bool ok;
        if (check_and_push_cycle(dict, out, ok))
            return ok;
    }
    Py_ssize_t sz = PyDict_GET_SIZE(dict);
    if (sz == 0) {
        out.append("{}", 2);
        if constexpr (Tracking)
            pop_seen();
        return true;
    }
    Py_ssize_t pos = 0;
    PyObject* key = nullptr;
    PyObject* value = nullptr;

    // First key-value pair: write "{" before key (no comma)
    PyDict_Next(dict, &pos, &key, &value);
    out.push_back('{');
    if (!write_dict_key(key, out, true)) {
        if constexpr (Tracking)
            pop_seen();
        return false;
    }
    if (!serialize_item_t<Tracking>(value, out, depth)) {
        if constexpr (Tracking)
            pop_seen();
        return false;
    }

    // Remaining key-value pairs: always with comma
    while (PyDict_Next(dict, &pos, &key, &value)) {
        if (!write_dict_key(key, out, false)) {
            if constexpr (Tracking)
                pop_seen();
            return false;
        }
        if (!serialize_item_t<Tracking>(value, out, depth)) {
            if constexpr (Tracking)
                pop_seen();
            return false;
        }
    }
    out.push_back('}');
    if constexpr (Tracking)
        pop_seen();
    return true;
}

// Batch optimization for lists of same-schema dicts.
// Pre-serializes dict keys once and reuses them for all elements,
// eliminating per-dict key escape checking and reducing buffer writes.
// Python 3.7+ guarantees dict insertion order, so if all dicts were
// created with the same code, key order is identical.
static constexpr int kMaxBatchKeys = 24;

template <bool Tracking, typename Buffer>
static inline bool try_batch_list_of_dicts(PyObject* list, Buffer& out, int depth) {
    const Py_ssize_t sz = PyList_GET_SIZE(list);
    if (sz < 4)
        return false; // not enough elements to benefit

    // Check first element
    PyObject* first = PyList_GET_ITEM(list, 0);
    if (!PyDict_CheckExact(first))
        return false;
    const Py_ssize_t nkeys = PyDict_GET_SIZE(first);
    if (nkeys == 0 || nkeys > kMaxBatchKeys)
        return false;

    // Pre-serialize keys from first element: ,"keyname":
    // Stack-allocated for zero heap overhead.
    struct PreKey {
        const char* data; // points into keybuf
        uint16_t len;
    };
    PreKey prekeys[kMaxBatchKeys];
    char keybuf[kMaxBatchKeys * 80]; // generous space
    char* kp = keybuf;

    Py_ssize_t pos = 0;
    PyObject* key = nullptr;
    PyObject* val = nullptr;
    int ki = 0;

    while (PyDict_Next(first, &pos, &key, &val)) {
        if (!PyUnicode_IS_COMPACT_ASCII(key))
            return false;
        const char* kdata = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(key));
        Py_ssize_t klen = PyUnicode_GET_LENGTH(key);

        // Check key is escape-free
        for (Py_ssize_t i = 0; i < klen; ++i) {
            unsigned char c = static_cast<unsigned char>(kdata[i]);
            if (c < 0x20 || c == '"' || c == '\\')
                return false;
        }

        // Check buffer space
        size_t need = static_cast<size_t>(klen) + 4; // ,"key":
        if (static_cast<size_t>(kp - keybuf) + need > sizeof(keybuf))
            return false;

        prekeys[ki].data = kp;
        if (ki > 0)
            *kp++ = ',';
        *kp++ = '"';
        std::memcpy(kp, kdata, static_cast<size_t>(klen));
        kp += klen;
        *kp++ = '"';
        *kp++ = ':';
        prekeys[ki].len = static_cast<uint16_t>(kp - prekeys[ki].data);
        ++ki;
    }

    // Quick verify: spot-check a few elements for same key count
    // (full key order verification happens implicitly during serialization)
    for (Py_ssize_t i = 1; i < sz && i < 4; ++i) {
        PyObject* elem = PyList_GET_ITEM(list, i);
        if (!PyDict_CheckExact(elem) || PyDict_GET_SIZE(elem) != nkeys)
            return false;
    }

    // Serialize using pre-computed keys
    out.push_back('[');

    for (Py_ssize_t i = 0; i < sz; ++i) {
        if (i > 0)
            out.push_back(',');
        PyObject* dict = PyList_GET_ITEM(list, i);

        // Verify this dict has the expected key count (fast bail for heterogeneous lists)
        if (UNLIKELY(!PyDict_CheckExact(dict) || PyDict_GET_SIZE(dict) != nkeys)) {
            // Schema mismatch — can't use batch. But we've already written partial output.
            // Write this and remaining elements normally.
            out.push_back('{');
            Py_ssize_t dpos = 0;
            PyObject* dk = nullptr;
            PyObject* dv = nullptr;
            bool dfirst = true;
            while (PyDict_Next(dict, &dpos, &dk, &dv)) {
                if (!write_dict_key(dk, out, dfirst))
                    return false;
                dfirst = false;
                if (!serialize_item_t<Tracking>(dv, out, depth + 1))
                    return false;
            }
            out.push_back('}');
            for (Py_ssize_t j = i + 1; j < sz; ++j) {
                out.push_back(',');
                if (!serialize_item_t<Tracking>(PyList_GET_ITEM(list, j), out, depth))
                    return false;
            }
            out.push_back(']');
            return true;
        }

        out.push_back('{');
        Py_ssize_t dpos = 0;
        PyObject* dk = nullptr;
        PyObject* dv = nullptr;
        int k = 0;
        while (PyDict_Next(dict, &dpos, &dk, &dv)) {
            // Write pre-computed key (no escape check, single append)
            out.append(prekeys[k].data, prekeys[k].len);
            // Inline top-3 type checks to avoid serialize_item_t function call
            PyTypeObject* vt = Py_TYPE(dv);
            if (LIKELY(vt == &PyUnicode_Type)) {
                if (!append_string(dv, out))
                    return false;
            } else if (LIKELY(vt == &PyLong_Type)) {
                if (!append_py_long(out, dv))
                    return false;
            } else if (vt == &PyFloat_Type) {
                if (!append_double(out, PyFloat_AS_DOUBLE(dv)))
                    return false;
            } else if (UNLIKELY(dv == Py_None)) {
                out.append("null", 4);
            } else if (UNLIKELY(vt == &PyBool_Type)) {
                out.append(dv == Py_True ? "true" : "false", dv == Py_True ? 4 : 5);
            } else {
                if (!serialize_item_t<Tracking>(dv, out, depth + 1))
                    return false;
            }
            ++k;
        }
        out.push_back('}');
    }

    out.push_back(']');
    return true;
}

// Fast path for homogeneous int arrays: skips per-element type dispatch.
// Checks first min(sz, 8) elements to verify all are PyLong_Type, then
// serializes with a tight loop calling append_py_long() directly.
template <typename Buffer>
static inline bool serialize_int_array_fast(PyObject* list, Buffer& out, Py_ssize_t sz) {
    const Py_ssize_t check = sz < 8 ? sz : 8;
    for (Py_ssize_t i = 0; i < check; ++i) {
        if (Py_TYPE(PyList_GET_ITEM(list, i)) != &PyLong_Type)
            return false;
    }

    // Pre-reserve: each int needs at most 20 chars + comma
    out.reserve(out.size() + static_cast<size_t>(sz) * 21 + 2);
    out.push_back('[');

    if (!append_py_long(out, PyList_GET_ITEM(list, 0)))
        return false;

    for (Py_ssize_t i = 1; i < sz; ++i) {
        PyObject* item = PyList_GET_ITEM(list, i);
        if (UNLIKELY(Py_TYPE(item) != &PyLong_Type)) {
            for (Py_ssize_t j = i; j < sz; ++j) {
                out.push_back(',');
                if (!serialize_item_t<false>(PyList_GET_ITEM(list, j), out, 0))
                    return false;
            }
            out.push_back(']');
            return true;
        }
        out.push_back(',');
        if (!append_py_long(out, item))
            return false;
    }

    out.push_back(']');
    return true;
}

// Fast path for homogeneous float arrays: skips per-element type dispatch.
// Checks first min(sz, 8) elements to verify all are PyFloat_Type, then
// serializes with a tight loop calling append_double() directly.
template <typename Buffer>
static inline bool serialize_float_array_fast(PyObject* list, Buffer& out, Py_ssize_t sz) {
    // Verify homogeneity by sampling first min(sz, 8) elements
    const Py_ssize_t check = sz < 8 ? sz : 8;
    for (Py_ssize_t i = 0; i < check; ++i) {
        if (Py_TYPE(PyList_GET_ITEM(list, i)) != &PyFloat_Type)
            return false;
    }

    // Pre-reserve: each float needs at most 25 chars + comma
    out.reserve(out.size() + static_cast<size_t>(sz) * 26 + 2);
    out.push_back('[');

    if (!append_double(out, PyFloat_AS_DOUBLE(PyList_GET_ITEM(list, 0))))
        return false;

    for (Py_ssize_t i = 1; i < sz; ++i) {
        PyObject* item = PyList_GET_ITEM(list, i);
        // Late bail if a non-float sneaks in after the sampled prefix
        if (UNLIKELY(Py_TYPE(item) != &PyFloat_Type)) {
            // Write remaining elements through the generic path
            for (Py_ssize_t j = i; j < sz; ++j) {
                out.push_back(',');
                if (!serialize_item_t<false>(PyList_GET_ITEM(list, j), out, 0))
                    return false;
            }
            out.push_back(']');
            return true;
        }
        out.push_back(',');
        if (!append_double(out, PyFloat_AS_DOUBLE(item)))
            return false;
    }

    out.push_back(']');
    return true;
}

// Fast path for homogeneous string arrays: skips per-element type dispatch.
// Checks first min(sz, 8) elements to verify all are PyUnicode_Type, then
// serializes with a tight loop calling append_string() directly.
template <typename Buffer>
static inline bool serialize_string_array_fast(PyObject* list, Buffer& out, Py_ssize_t sz) {
    const Py_ssize_t check = sz < 8 ? sz : 8;
    for (Py_ssize_t i = 0; i < check; ++i) {
        if (Py_TYPE(PyList_GET_ITEM(list, i)) != &PyUnicode_Type)
            return false;
    }

    out.push_back('[');
    if (!append_string(PyList_GET_ITEM(list, 0), out))
        return false;

    for (Py_ssize_t i = 1; i < sz; ++i) {
        PyObject* item = PyList_GET_ITEM(list, i);
        if (UNLIKELY(Py_TYPE(item) != &PyUnicode_Type)) {
            for (Py_ssize_t j = i; j < sz; ++j) {
                out.push_back(',');
                if (!serialize_item_t<false>(PyList_GET_ITEM(list, j), out, 0))
                    return false;
            }
            out.push_back(']');
            return true;
        }
        out.push_back(',');
        if (!append_string(item, out))
            return false;
    }

    out.push_back(']');
    return true;
}

template <bool Tracking, typename Buffer>
static inline bool serialize_list_t(PyObject* list, Buffer& out, int depth) {
    if constexpr (Tracking) {
        bool ok;
        if (check_and_push_cycle(list, out, ok))
            return ok;
    }
    Py_ssize_t sz = PyList_GET_SIZE(list);
    if (sz == 0) {
        out.append("[]", 2);
        if constexpr (Tracking)
            pop_seen();
        return true;
    }

    // Try fast paths for homogeneous arrays (skip per-element type dispatch)
    if (!Tracking && sz >= 4) {
        PyTypeObject* first_type = Py_TYPE(PyList_GET_ITEM(list, 0));
        if (first_type == &PyLong_Type) {
            if (serialize_int_array_fast(list, out, sz))
                return true;
        } else if (first_type == &PyFloat_Type) {
            if (serialize_float_array_fast(list, out, sz))
                return true;
        } else if (first_type == &PyUnicode_Type) {
            if (serialize_string_array_fast(list, out, sz))
                return true;
        }
    }

    // Try batch optimization for lists of same-schema dicts
    if (!Tracking && sz >= 4 && PyDict_CheckExact(PyList_GET_ITEM(list, 0))) {
        if (UNLIKELY(depth >= get_max_serialize_depth())) {
            PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
            if constexpr (Tracking)
                pop_seen();
            return false;
        }
        if (try_batch_list_of_dicts<Tracking>(list, out, depth)) {
            if constexpr (Tracking)
                pop_seen();
            return true;
        }
    }

    out.push_back('[');

    if (!serialize_item_t<Tracking>(PyList_GET_ITEM(list, 0), out, depth)) {
        if constexpr (Tracking)
            pop_seen();
        return false;
    }

    for (Py_ssize_t i = 1; i < sz; ++i) {
        out.push_back(',');
        if (!serialize_item_t<Tracking>(PyList_GET_ITEM(list, i), out, depth)) {
            if constexpr (Tracking)
                pop_seen();
            return false;
        }
    }

    out.push_back(']');
    if constexpr (Tracking)
        pop_seen();
    return true;
}

// Non-template wrappers: dispatch based on runtime cycle policy
template <typename Buffer>
static inline bool serialize_dict(PyObject* dict, Buffer& out, int depth) {
    if (UNLIKELY(g_cycle_policy != CyclePolicy::Ignore))
        return serialize_dict_t<true>(dict, out, depth);
    return serialize_dict_t<false>(dict, out, depth);
}

template <typename Buffer>
static inline bool serialize_list(PyObject* list, Buffer& out, int depth) {
    if (UNLIKELY(g_cycle_policy != CyclePolicy::Ignore))
        return serialize_list_t<true>(list, out, depth);
    return serialize_list_t<false>(list, out, depth);
}

template <typename Buffer>
static inline bool serialize_tuple(PyObject* tup, Buffer& out, int depth) {
    Py_ssize_t sz = PyTuple_GET_SIZE(tup);
    if (sz == 0) {
        out.append("[]", 2);
        return true;
    }
    out.push_back('[');
    for (Py_ssize_t i = 0; i < sz; ++i) {
        if (i > 0) {
            out.push_back(',');
        }
        if (!serialize_value(PyTuple_GET_ITEM(tup, i), out, depth)) {
            return false;
        }
    }
    out.push_back(']');
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

// Unified dumps() with return_type kwarg: "str" (default) or "bytes"
PyObject* strata_dumps(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* obj;

    // Fast path: no kwargs → skip PyArg_ParseTupleAndKeywords overhead.
    // Direct tuple access avoids format string parsing entirely.
    if (kwargs == NULL || PyDict_GET_SIZE(kwargs) == 0) {
        if (PyTuple_GET_SIZE(args) != 1) {
            PyErr_SetString(PyExc_TypeError, "dumps() takes exactly 1 argument");
            return NULL;
        }
        obj = PyTuple_GET_ITEM(args, 0);

        STRATA_CPP_TRY

        PyGcPause gc_pause;
        g_max_depth = Py_GetRecursionLimit();
        g_seen_stack.clear();
        g_serialize_buffer.clear();

        size_t est = estimate_size(obj);
        if (g_serialize_buffer.capacity() < est) {
            g_serialize_buffer.reserve(est);
        }

        bool ok;
        if (LIKELY(g_cycle_policy == CyclePolicy::Ignore)) {
            PyTypeObject* vt = Py_TYPE(obj);
            if (vt == &PyDict_Type)
                ok = serialize_dict_t<false>(obj, g_serialize_buffer, 1);
            else if (vt == &PyList_Type)
                ok = serialize_list_t<false>(obj, g_serialize_buffer, 1);
            else
                ok = serialize_value(obj, g_serialize_buffer, 0);
        } else {
            ok = serialize_value(obj, g_serialize_buffer, 0);
        }

        if (!ok)
            return NULL;
        if (PyErr_Occurred())
            return NULL;

        return buffer_to_pyunicode(g_serialize_buffer.data(), g_serialize_buffer.size());

        STRATA_CPP_CATCH
    }

    // Slow path: parse kwargs for return_type
    const char* return_type = "str";
    static const char* kwlist[] = {"obj", "return_type", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|s", const_cast<char**>(kwlist), &obj,
                                     &return_type)) {
        return NULL;
    }

    bool as_bytes;
    if (strcmp(return_type, "str") == 0) {
        as_bytes = false;
    } else if (strcmp(return_type, "bytes") == 0) {
        as_bytes = true;
    } else {
        PyErr_Format(PyExc_ValueError, "return_type must be 'str' or 'bytes', got '%s'",
                     return_type);
        return NULL;
    }

    STRATA_CPP_TRY

    PyGcPause gc_pause;
    g_max_depth = Py_GetRecursionLimit();
    g_seen_stack.clear();
    g_serialize_buffer.clear();

    size_t est = estimate_size(obj);
    if (g_serialize_buffer.capacity() < est) {
        g_serialize_buffer.reserve(est);
    }

    bool ok;
    if (LIKELY(g_cycle_policy == CyclePolicy::Ignore)) {
        PyTypeObject* vt = Py_TYPE(obj);
        if (vt == &PyDict_Type)
            ok = serialize_dict_t<false>(obj, g_serialize_buffer, 1);
        else if (vt == &PyList_Type)
            ok = serialize_list_t<false>(obj, g_serialize_buffer, 1);
        else
            ok = serialize_value(obj, g_serialize_buffer, 0);
    } else {
        ok = serialize_value(obj, g_serialize_buffer, 0);
    }

    if (!ok) {
        return NULL;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }

    if (as_bytes) {
        return PyBytes_FromStringAndSize(g_serialize_buffer.data(), g_serialize_buffer.size());
    }
    return buffer_to_pyunicode(g_serialize_buffer.data(), g_serialize_buffer.size());

    STRATA_CPP_CATCH
}

// Internal: serialize to string (used by dump())
PyObject* strata_dumps_internal(PyObject* obj) {
    PyGcPause gc_pause;
    g_max_depth = Py_GetRecursionLimit();
    g_seen_stack.clear();
    g_serialize_buffer.clear();
    size_t est = estimate_size(obj);
    if (g_serialize_buffer.capacity() < est)
        g_serialize_buffer.reserve(est);

    if (!serialize_value(obj, g_serialize_buffer, 0)) {
        return NULL;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }

    return buffer_to_pyunicode(g_serialize_buffer.data(), g_serialize_buffer.size());
}

// Internal: serialize to raw buffer, return (data, size) pair — avoids PyUnicode allocation
bool strata_serialize_to_buffer(PyObject* obj, const char** out_data, size_t* out_size) {
    PyGcPause gc_pause;
    g_max_depth = Py_GetRecursionLimit();
    g_seen_stack.clear();
    g_serialize_buffer.clear();
    size_t est = estimate_size(obj);
    if (g_serialize_buffer.capacity() < est)
        g_serialize_buffer.reserve(est);

    bool ok;
    if (LIKELY(g_cycle_policy == CyclePolicy::Ignore)) {
        PyTypeObject* vt = Py_TYPE(obj);
        if (vt == &PyDict_Type)
            ok = serialize_dict_t<false>(obj, g_serialize_buffer, 1);
        else if (vt == &PyList_Type)
            ok = serialize_list_t<false>(obj, g_serialize_buffer, 1);
        else
            ok = serialize_value(obj, g_serialize_buffer, 0);
    } else {
        ok = serialize_value(obj, g_serialize_buffer, 0);
    }

    if (!ok) {
        return false;
    }
    if (PyErr_Occurred()) {
        return false;
    }

    *out_data = g_serialize_buffer.data();
    *out_size = g_serialize_buffer.size();
    return true;
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

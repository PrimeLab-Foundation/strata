#pragma once

/**
 * @file python_dumps_output.h
 * @brief The serializer's output machinery: staging, schema cache, raw dicts.
 *
 * Split from python_dumps.cpp for the ~800-line rule; included only there.
 * Three pieces live here:
 *
 *  - StagedOutput: an 8KB stage flushed into either a std::string (the str
 *    path) or a growing PyBytes (the bytes path -- one document copy, not
 *    two).
 *  - SchemaCacheLease: the per-thread prepared-key cache with owned key
 *    references and per-slot value-kind predictions.
 *  - rawdict: the runtime-proved direct walk over a dict's entry array.
 */

#include "python_types.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>
#include <string>
#include <vector>

namespace strata::bindings {

/**
 * A staging buffer between the serializer and the output string.
 *
 * `ensure(n)` guarantees `n` writable bytes at `cursor()`; everything the
 * serializer emits is then raw stores and memcpys, and the output string is
 * touched once per stageful. Reservations must fit the stage — every caller
 * here reserves a small constant — while `write_spanning` takes any size and
 * routes oversized payloads straight to the string.
 */
class StagedOutput {
  public:
    explicit StagedOutput(std::string& sink) : str_sink_(&sink) {}

    /// Bytes mode: stagefuls land directly in a growing PyBytes, so the
    /// output is copied once (stage -> bytes) instead of twice (stage ->
    /// string -> bytes). The two whole-document copies were 12% of a
    /// users.json dump. Failure is remembered and reported at take_bytes().
    StagedOutput() = default;

    ~StagedOutput() { Py_XDECREF(bytes_); }

    [[nodiscard]] bool init_bytes(size_t initial) {
        bytes_ = PyBytes_FromStringAndSize(nullptr, static_cast<Py_ssize_t>(initial));
        bytes_capacity_ = initial;
        return bytes_ != nullptr;
    }

    /// Finish bytes mode: exact-size the object and hand it over.
    [[nodiscard]] PyObject* take_bytes() {
        flush();
        if (failed_ || bytes_ == nullptr)
            return PyErr_NoMemory();
        if (_PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(bytes_length_)) != 0)
            return nullptr; // bytes_ already cleared by the resize
        PyObject* result = bytes_;
        bytes_ = nullptr;
        return result;
    }

    StagedOutput(const StagedOutput&) = delete;
    StagedOutput& operator=(const StagedOutput&) = delete;

    /// Make room for @p bytes of upcoming raw writes. @p bytes ≤ stage size.
    void ensure(size_t bytes) {
        if (used_ + bytes > sizeof(stage_))
            flush();
    }

    [[nodiscard]] char* cursor() noexcept { return stage_ + used_; }
    void advance(size_t bytes) noexcept { used_ += bytes; }

    /// Take back @p bytes just written to the stage (never past a flush).
    void rewind(size_t bytes) noexcept { used_ -= bytes; }

    /// One byte, after a covering ensure().
    void put(char c) noexcept { stage_[used_++] = c; }

    /// @p len bytes, after a covering ensure().
    void write(const char* data, size_t len) noexcept {
        std::memcpy(stage_ + used_, data, len);
        used_ += len;
    }

    /// Any length; spills to the sink when the stage cannot hold it.
    void write_spanning(const char* data, size_t len) {
        if (used_ + len <= sizeof(stage_)) {
            std::memcpy(stage_ + used_, data, len);
            used_ += len;
            return;
        }
        flush();
        if (len >= sizeof(stage_) / 2) {
            sink_write(data, len);
            return;
        }
        std::memcpy(stage_, data, len);
        used_ = len;
    }

    /// A string sink for the rare escaped-string path, flushed first. In
    /// bytes mode this is a per-thread scratch the caller then spans in.
    [[nodiscard]] std::string& direct_sink() {
        flush();
        if (str_sink_ != nullptr)
            return *str_sink_;
        static thread_local std::string scratch;
        scratch.clear();
        return scratch;
    }

    /// Bytes mode only: true when direct_sink() handed out the scratch that
    /// still has to be written through the stage.
    [[nodiscard]] bool sink_is_scratch() const noexcept { return str_sink_ == nullptr; }

    void flush() {
        if (used_ != 0) {
            sink_write(stage_, used_);
            used_ = 0;
        }
    }

  private:
    void sink_write(const char* data, size_t len) {
        if (str_sink_ != nullptr) {
            str_sink_->append(data, len);
            return;
        }
        if (failed_)
            return;
        if (bytes_length_ + len > bytes_capacity_) {
            size_t grown = bytes_capacity_ * 2;
            while (grown < bytes_length_ + len)
                grown *= 2;
            if (_PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(grown)) != 0) {
                // bytes_ is cleared on failure; remember and report later so
                // the writers stay branch-free.
                PyErr_Clear();
                failed_ = true;
                return;
            }
            bytes_capacity_ = grown;
        }
        std::memcpy(PyBytes_AS_STRING(bytes_) + bytes_length_, data, len);
        bytes_length_ += len;
    }

    std::string* str_sink_ = nullptr;
    PyObject* bytes_ = nullptr;
    size_t bytes_length_ = 0;
    size_t bytes_capacity_ = 0;
    bool failed_ = false;
    size_t used_ = 0;
    char stage_[8192];
};

/**
 * The per-thread schema cache, shared across dumps() calls.
 *
 * Sharing is what lets repeated serialization of same-shaped payloads skip
 * key preparation entirely after the first call. The keys are *owned*
 * references: identity comparison is only sound while the objects live, and
 * a borrowed pointer could be freed between calls and reincarnated as a
 * different key. The busy flag covers re-entrancy -- a cycle warning can run
 * arbitrary Python, which can call dumps() again mid-walk; the nested call
 * pays for a private, empty cache instead.
 */
class SchemaCacheLease {
  public:
    /// What one schema slot's value was last time; verified before use.
    enum class ValueKind : uint8_t { Unknown, Str, Float, Int, Bool, None, List, Dict };

    /// Prepared `"key":` bytes for one object shape at one depth.
    struct Schema {
        std::vector<PyObject*> keys;      ///< owned references
        std::string blob;                 ///< the prepared bytes, back to back
        std::vector<uint32_t> offsets{0}; ///< blob boundaries, keys.size() + 1
        uint8_t kinds[24] = {};           ///< ValueKind per slot, adaptively learned
        bool prepared = false;
        uint32_t divergences = 0;
        /// A depth whose objects keep changing shape is not record-shaped;
        /// it stops remembering rather than paying incref churn per object.
        bool retired = false;

        void remember(PyObject* const* other, Py_ssize_t count) {
            if (++divergences > 16) {
                for (PyObject* key : keys)
                    Py_DECREF(key);
                keys.clear();
                retired = true;
                prepared = false;
                return;
            }
            for (PyObject* key : keys)
                Py_DECREF(key);
            keys.assign(other, other + count);
            for (PyObject* key : keys)
                Py_INCREF(key);
            prepared = false;
        }

        [[nodiscard]] bool matches(PyObject* const* other, Py_ssize_t count) const noexcept {
            if (static_cast<Py_ssize_t>(keys.size()) != count)
                return false;
            for (Py_ssize_t index = 0; index < count; ++index) {
                // Identity, not equality: strata interns the keys it parses
                // and CPython interns identifier-like literals, so same-schema
                // records share key objects. A miss costs a rebuild, never a
                // wrong answer.
                if (keys[static_cast<size_t>(index)] != other[index])
                    return false;
            }
            return true;
        }
    };

    SchemaCacheLease() {
        if (!busy_ && shared() != nullptr) {
            busy_ = true;
            slots_ = shared();
            owns_flag_ = true;
        } else {
            slots_ = &fallback_;
        }
    }

    ~SchemaCacheLease() {
        if (owns_flag_)
            busy_ = false;
    }

    SchemaCacheLease(const SchemaCacheLease&) = delete;
    SchemaCacheLease& operator=(const SchemaCacheLease&) = delete;

    [[nodiscard]] std::vector<Schema>& slots() noexcept { return *slots_; }

  private:
    [[nodiscard]] static std::vector<Schema>* shared() {
        // Deliberately leaked: a destructor after interpreter shutdown could
        // not legally Py_DECREF the owned keys anyway.
        static thread_local std::vector<Schema>* instance =
            new (std::nothrow) std::vector<Schema>();
        return instance;
    }

    static thread_local bool busy_;

    std::vector<Schema>* slots_;
    std::vector<Schema> fallback_;
    bool owns_flag_ = false;
};

inline thread_local bool SchemaCacheLease::busy_ = false;

#if PY_VERSION_HEX >= 0x030B0000 && PY_VERSION_HEX < 0x030F0000
#define STRATA_RAW_DICT_WALK 1
/**
 * Direct iteration over a dict's entry array.
 *
 * `PyDict_Next` re-validates and re-dispatches per call — profiled at 8% of a
 * record-heavy dump. Combined unicode-key tables (every dict strata builds,
 * and every dict literal) store `{key, value}` entries contiguously; walking
 * them is a pointer loop.
 *
 * The layout is CPython-internal, so it is mirrored minimally (the fields up
 * to the entry array, stable across 3.11–3.14), version-gated, and — the
 * load-bearing part — **proved at runtime**: the first use walks probe dicts
 * both ways and compares. Any mismatch, on any future build, disables the
 * raw walk for the process and every caller silently keeps `PyDict_Next`.
 * Split-table dicts (instance attribute dicts) and non-unicode tables always
 * take the fallback.
 */
namespace rawdict {

struct KeysPrefix {
    Py_ssize_t dk_refcnt;
    uint8_t dk_log2_size;
    uint8_t dk_log2_index_bytes;
    uint8_t dk_kind;
    uint32_t dk_version;
    Py_ssize_t dk_usable;
    Py_ssize_t dk_nentries;
    char dk_indices[1]; // the index table; entries follow it
};

struct UnicodeEntry {
    PyObject* me_key;
    PyObject* me_value;
};

constexpr uint8_t kKindUnicode = 1; // DICT_KEYS_UNICODE

/// The entry array of @p dict, or nullptr when this dict cannot be walked raw.
[[nodiscard]] inline const UnicodeEntry* entry_array(PyObject* dict, Py_ssize_t* entry_count) {
    auto* impl = reinterpret_cast<PyDictObject*>(dict);
    if (impl->ma_values != nullptr)
        return nullptr; // split table: values live elsewhere
    auto* keys = reinterpret_cast<const KeysPrefix*>(impl->ma_keys);
    if (keys->dk_kind != kKindUnicode)
        return nullptr;
    *entry_count = keys->dk_nentries;
    const char* base = reinterpret_cast<const char*>(keys) + offsetof(KeysPrefix, dk_indices);
    return reinterpret_cast<const UnicodeEntry*>(
        base + (static_cast<size_t>(1) << keys->dk_log2_index_bytes));
}

/// One-time layout proof: walk probe dicts raw and via PyDict_Next, compare.
[[nodiscard]] inline bool probe_layout() {
    PyRef small(PyDict_New());
    PyRef big(PyDict_New());
    if (!small || !big)
        return false;
    char name[16];
    for (int index = 0; index < 40; ++index) {
        std::snprintf(name, sizeof(name), "k%d", index);
        PyRef value(PyLong_FromLong(index));
        if (!value ||
            PyDict_SetItemString(index < 4 ? small.get() : big.get(), name, value.get()) != 0)
            return false;
    }
    if (PyDict_DelItemString(small.get(), "k1") != 0) // leave a hole
        return false;

    for (PyObject* dict : {small.get(), big.get()}) {
        Py_ssize_t entry_count = 0;
        const UnicodeEntry* entries = entry_array(dict, &entry_count);
        if (entries == nullptr)
            return false;
        Py_ssize_t position = 0;
        PyObject* expected_key = nullptr;
        PyObject* expected_value = nullptr;
        Py_ssize_t walked = 0;
        for (Py_ssize_t index = 0; index < entry_count; ++index) {
            if (entries[index].me_value == nullptr)
                continue; // deleted slot
            if (!PyDict_Next(dict, &position, &expected_key, &expected_value))
                return false;
            if (entries[index].me_key != expected_key || entries[index].me_value != expected_value)
                return false;
            ++walked;
        }
        if (PyDict_Next(dict, &position, &expected_key, &expected_value))
            return false; // raw walk ended early
        if (walked != PyDict_GET_SIZE(dict))
            return false;
    }
    return true;
}

[[nodiscard]] inline bool available() {
    static const bool proved = probe_layout();
    return proved;
}

} // namespace rawdict
#endif // STRATA_RAW_DICT_WALK


} // namespace strata::bindings

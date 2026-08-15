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
 *    references.
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
    explicit StagedOutput(std::string& sink)
        : str_sink_(&sink), base_(stage_), capacity_(sizeof(stage_)) {}

    /**
     * Bytes mode: the serializer writes *directly into the PyBytes buffer* —
     * generated once, copied never. Str mode stages through the stack and
     * flushes into the string.
     *
     * Both modes share one branch-free hot path: `base_`/`capacity_` always
     * describe the current write region, so put/write/cursor are a load and
     * a store with no mode test. (A per-write `direct_` select was measured
     * as a 6–8% whole-serializer regression — the mode check lives only in
     * the cold overflow path.)
     */
    StagedOutput() = default;

    ~StagedOutput() { Py_XDECREF(bytes_); }

    [[nodiscard]] bool init_bytes(size_t initial) {
        if (initial < 64)
            initial = 64;
        bytes_ = PyBytes_FromStringAndSize(nullptr, static_cast<Py_ssize_t>(initial));
        if (bytes_ == nullptr)
            return false;
        base_ = PyBytes_AS_STRING(bytes_);
        capacity_ = initial;
        used_ = 0;
        return true;
    }

    /// Finish bytes mode: exact-size the object and hand it over.
    [[nodiscard]] PyObject* take_bytes() {
        if (failed_ || bytes_ == nullptr)
            return PyErr_NoMemory();
        if (_PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(used_)) != 0)
            return nullptr; // bytes_ already cleared by the resize
        PyObject* result = bytes_;
        bytes_ = nullptr;
        return result;
    }

    StagedOutput(const StagedOutput&) = delete;
    StagedOutput& operator=(const StagedOutput&) = delete;

    /// Make room for @p bytes of upcoming raw writes. Str mode: @p bytes must
    /// fit the stage (every caller asks for a small constant). Bytes mode:
    /// any size.
    void ensure(size_t bytes) {
        if (used_ + bytes > capacity_)
            overflow(bytes);
    }

    [[nodiscard]] char* cursor() noexcept { return base_ + used_; }
    void advance(size_t bytes) noexcept { used_ += bytes; }

    /// Take back @p bytes just written (never past an overflow).
    void rewind(size_t bytes) noexcept { used_ -= bytes; }

    /// One byte, after a covering ensure().
    void put(char c) noexcept { base_[used_++] = c; }

    /// @p len bytes, after a covering ensure().
    void write(const char* data, size_t len) noexcept {
        std::memcpy(base_ + used_, data, len);
        used_ += len;
    }

    /// Any length, no prior ensure needed.
    void write_spanning(const char* data, size_t len) {
        if (used_ + len <= capacity_) {
            std::memcpy(base_ + used_, data, len);
            used_ += len;
            return;
        }
        if (str_sink_ != nullptr) {
            flush();
            if (len >= sizeof(stage_) / 2) {
                str_sink_->append(data, len);
                return;
            }
            std::memcpy(stage_, data, len);
            used_ = len;
            return;
        }
        overflow(len);
        std::memcpy(base_ + used_, data, len);
        used_ += len;
    }

    /// A string sink for the rare escaped-string path, flushed first. In
    /// bytes mode this is a per-thread scratch the caller then spans in.
    [[nodiscard]] std::string& direct_sink() {
        if (str_sink_ != nullptr) {
            flush();
            return *str_sink_;
        }
        static thread_local std::string scratch;
        scratch.clear();
        return scratch;
    }

    /// Bytes mode only: true when direct_sink() handed out the scratch that
    /// still has to be written through write_spanning.
    [[nodiscard]] bool sink_is_scratch() const noexcept { return str_sink_ == nullptr; }

    void flush() {
        if (str_sink_ != nullptr && used_ != 0) {
            str_sink_->append(stage_, used_);
            used_ = 0;
        }
    }

  private:
    /// The cold path: str mode flushes the stage, bytes mode grows the object.
    void overflow(size_t needed) {
        if (str_sink_ != nullptr) {
            flush();
            return;
        }
        if (failed_) {
            used_ = 0; // absorb writes safely in the scratch region
            return;
        }
        size_t grown = capacity_ * 2;
        while (grown < used_ + needed)
            grown *= 2;
        if (_PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(grown)) != 0) {
            PyErr_Clear();
            failed_ = true;
            // bytes_ was cleared by the failed resize; write into the stage
            // as scratch so raw stores stay in bounds.
            base_ = stage_;
            capacity_ = sizeof(stage_);
            used_ = 0;
            return;
        }
        base_ = PyBytes_AS_STRING(bytes_);
        capacity_ = grown;
    }

    std::string* str_sink_ = nullptr;
    PyObject* bytes_ = nullptr;
    char* base_ = nullptr;
    size_t capacity_ = 0;
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
    /// Prepared `"key":` bytes for one object shape at one depth.
    struct Schema {
        std::vector<PyObject*> keys;      ///< owned references
        std::string blob;                 ///< the prepared bytes, back to back
        std::vector<uint32_t> offsets{0}; ///< blob boundaries, keys.size() + 1
        bool prepared = false;

        void remember(PyObject* const* other, Py_ssize_t count) {
            for (PyObject* key : keys)
                Py_DECREF(key);
            keys.assign(other, other + count);
            for (PyObject* key : keys)
                Py_INCREF(key);
            prepared = false;
        }

        /// Keys past the first, by identity. `select` has already matched the
        /// count and the first key from its inline arrays; only a plausible
        /// hit pays for touching this vector's heap storage. Identity, not
        /// equality: strata interns the keys it parses and CPython interns
        /// identifier-like literals, so same-schema records share key
        /// objects. A miss costs a rebuild, never a wrong answer.
        [[nodiscard]] bool matches_tail(PyObject* const* other, Py_ssize_t count) const noexcept {
            for (Py_ssize_t index = 1; index < count; ++index) {
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

    /**
     * One depth's schema set: four ways, scanned in place.
     *
     * A single slot per depth has a 0% hit rate on documents that rotate a
     * few record shapes -- mixed.json cycles four schemas round-robin and
     * missed every single object. Four ways cover that; the ways are *not*
     * reordered on a hit, because round-robin access is LRU's worst case:
     * move-to-front put every hit at the deepest way and paid a full
     * three-schema shuffle per object (a measured 45 ns regression before
     * this was flattened). A scan of up to four count-compares is near-free;
     * replacement just walks a round-robin cursor.
     *
     * `select` returns the matching way's index, or `kMiss` after
     * remembering the shape in the next victim way.
     */
    struct DepthSchemas {
        static constexpr size_t kWays = 4;
        static constexpr size_t kMiss = kWays;

        /// Scanned in place of the ways' own storage: each way's key count
        /// and first key, inline. A rejected way costs two loads from this
        /// cache line instead of a pointer chase into its `keys` vector --
        /// the scan is the per-object cost on shape-rotating documents, and
        /// with four ways it ran once per record on mixed.json.
        Py_ssize_t counts[kWays] = {-1, -1, -1, -1};
        PyObject* first_keys[kWays] = {nullptr, nullptr, nullptr, nullptr};
        Schema ways[kWays];
        uint8_t victim = 0;
        uint32_t misses = 0;
        /// True churn -- shapes that never repeat -- retires the depth so it
        /// stops paying remember()'s reference traffic.
        bool retired = false;

        [[nodiscard]] size_t select(PyObject* const* keys, Py_ssize_t count) {
            if (retired)
                return kMiss;
            PyObject* const first = keys[0]; // callers guarantee count >= 1
            for (size_t way = 0; way < kWays; ++way) {
                if (counts[way] == count && first_keys[way] == first &&
                    ways[way].matches_tail(keys, count))
                    return way;
            }
            if (++misses > 64) {
                for (Schema& schema : ways) {
                    for (PyObject* key : schema.keys)
                        Py_DECREF(key);
                    schema.keys.clear();
                    schema.prepared = false;
                }
                for (size_t way = 0; way < kWays; ++way) {
                    counts[way] = -1;
                    first_keys[way] = nullptr;
                }
                retired = true;
                return kMiss;
            }
            ways[victim].remember(keys, count);
            counts[victim] = count;
            first_keys[victim] = first;
            victim = static_cast<uint8_t>((victim + 1) % kWays);
            return kMiss;
        }
    };

    [[nodiscard]] std::vector<DepthSchemas>& slots() noexcept { return *slots_; }

  private:
    [[nodiscard]] static std::vector<DepthSchemas>* shared() {
        // Deliberately leaked: a destructor after interpreter shutdown could
        // not legally Py_DECREF the owned keys anyway.
        static thread_local std::vector<DepthSchemas>* instance =
            new (std::nothrow) std::vector<DepthSchemas>();
        return instance;
    }

    static thread_local bool busy_;

    std::vector<DepthSchemas>* slots_;
    std::vector<DepthSchemas> fallback_;
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

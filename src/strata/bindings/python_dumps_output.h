#pragma once

/**
 * @file python_dumps_output.h
 * @brief The serializer's output machinery: staging, schema cache, raw dicts.
 *
 * Split from python_dumps.cpp for the ~800-line rule; included only there.
 * Three pieces live here:
 *
 *  - StagedOutput: an 8KB stage flushed into a std::string (the str path),
 *    or a PyBytes written directly, sized to the previous document, with
 *    the stage holding the tail that its last reservation could not fit
 *    (the bytes path -- one document copy at most, usually none).
 *  - SchemaCacheLease: the per-thread lease of everything a dumps() call
 *    keeps per nesting level -- the prepared-key cache with owned key
 *    references, and the staged key/value rows the dict writers emit from.
 *  - rawdict: the runtime-proved direct walk over a dict's entry array.
 */

#include "python_types.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <new>
#include <string>
#include <type_traits>
#include <vector>

// Cold-path annotation: keeps rare paths out of the serializer's hot text.
// The benchmark condition interleaves five engines per round; on x86's small
// L1I every byte of hot footprint refaults per call, and these functions run
// once per schema, once per cycle, or never.
//
// Defined here rather than in python_dumps.cpp because both files need them
// and there must be exactly one definition: a second spelling whose `#else`
// arm expands to nothing would hand an out-of-lining decision the codegen
// argument depends on back to the optimizer, silently and per compiler.
#if defined(__clang__) || defined(__GNUC__)
#define STRATA_COLD_FN __attribute__((noinline, cold))
#define STRATA_NOINLINE_HOT __attribute__((noinline))
#elif defined(_MSC_VER)
#define STRATA_COLD_FN __declspec(noinline)
#define STRATA_NOINLINE_HOT __declspec(noinline)
#else
#define STRATA_COLD_FN
#define STRATA_NOINLINE_HOT
#endif

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
    /// The widest single reservation any writer makes (python_dumps.cpp
    /// guards its string paths with it). An exact repeat never grows its
    /// block only while the stage can hold the tail one such reservation
    /// leaves behind plus the next one — pinned below.
    static constexpr size_t kMaxReservation = 4096;
    static constexpr size_t kStageBytes = 8192;
    static_assert(2 * kMaxReservation <= kStageBytes,
                  "the stage must hold two of the widest reservation, or exact repeats resize");

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
     *
     * The block is sized to the previous document exactly (see
     * dumps_to_python), so on a repeated document the *last* reservation —
     * every writer reserves a small constant before it writes — no longer
     * fits the block although the content does. That reservation, and
     * everything after it, goes to the stage; take_bytes() copies the tail
     * into the block when the content fits, and only a document that really
     * outgrew the block pays a resize. The alternative, a block with
     * headroom shrunk in place, was measured to cost 13 µs per call on the
     * Windows runners and on macOS against 2 µs for an exact-fit block
     * (docs/decisions.md 2026-09-03).
     */
    StagedOutput() = default;

    ~StagedOutput() { Py_XDECREF(bytes_); }

    [[nodiscard]] bool init_bytes(size_t initial) {
        if (initial == 0)
            initial = 1; // never the shared empty singleton, which cannot be resized
        bytes_ = PyBytes_FromStringAndSize(nullptr, static_cast<Py_ssize_t>(initial));
        if (bytes_ == nullptr)
            return false;
        base_ = PyBytes_AS_STRING(bytes_);
        capacity_ = initial;
        block_capacity_ = initial;
        block_used_ = 0;
        staged_ = false;
        used_ = 0;
        return true;
    }

    /// Finish bytes mode: land the staged tail, exact-size the object, hand
    /// it over. A block that already has the document's size is not resized.
    [[nodiscard]] PyObject* take_bytes() {
        if (failed_ || bytes_ == nullptr)
            return PyErr_NoMemory();
        size_t total = used_;
        if (staged_) {
            total = block_used_ + used_;
            if (total > block_capacity_) {
                if (_PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(total)) != 0)
                    return nullptr; // bytes_ already cleared by the resize
                block_capacity_ = total;
            }
            std::memcpy(PyBytes_AS_STRING(bytes_) + block_used_, stage_, used_);
            staged_ = false;
        }
        if (total != block_capacity_ &&
            _PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(total)) != 0)
            return nullptr;
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
            flush_str();
            if (len >= sizeof(stage_) / 2) {
                str_sink_->append(data, len);
                return;
            }
            std::memcpy(stage_, data, len);
            used_ = len;
            return;
        }
        overflow(len);
        if (used_ + len > capacity_)
            return; // a failed grow: absorb the write, take_bytes() reports
        std::memcpy(base_ + used_, data, len);
        used_ += len;
    }

    /// A string sink for the rare escaped-string path, flushed first. In
    /// bytes mode this is a per-thread scratch the caller then spans in.
    [[nodiscard]] std::string& direct_sink() {
        if (str_sink_ != nullptr) {
            flush_str();
            return *str_sink_;
        }
        static thread_local std::string scratch;
        scratch.clear();
        return scratch;
    }

    /// Bytes mode only: true when direct_sink() handed out the scratch that
    /// still has to be written through write_spanning.
    [[nodiscard]] bool sink_is_scratch() const noexcept { return str_sink_ == nullptr; }

    /// Str mode only: land the stage in the string. In bytes mode the stage
    /// holds live document bytes that only take_bytes() may move.
    void flush_str() {
        if (str_sink_ != nullptr && used_ != 0) {
            str_sink_->append(stage_, used_);
            used_ = 0;
        }
    }

  private:
    /// The cold path: str mode flushes the stage; bytes mode stages the
    /// reservation that did not fit the block, and grows the block only
    /// once the stage is full too (or for a reservation wider than it).
    void overflow(size_t needed) {
        if (str_sink_ != nullptr) {
            flush_str();
            return;
        }
        if (failed_) {
            used_ = 0; // absorb writes safely in the scratch region
            return;
        }
        if (!staged_) {
            if (needed <= sizeof(stage_)) {
                block_used_ = used_;
                base_ = stage_;
                capacity_ = sizeof(stage_);
                used_ = 0;
                staged_ = true;
                return;
            }
            grow_block(used_ + needed);
            return;
        }
        // The stage is full: the document outgrew the block. Grow it, move
        // the stage in, and resume direct writes.
        const size_t total = block_used_ + used_;
        const size_t staged = used_;
        if (!grow_block(total + needed))
            return; // the stage is scratch now; nothing to move
        std::memcpy(base_ + block_used_, stage_, staged);
        used_ = total;
        staged_ = false;
    }

    /// Grow the block by half until it holds @p minimum (the block starts
    /// at the previous document's exact size, so doubling would spike a
    /// document that grew by a few kilobytes to twice its footprint); on
    /// failure the stage becomes scratch so raw stores stay in bounds and
    /// take_bytes() reports.
    bool grow_block(size_t minimum) {
        size_t grown = block_capacity_ + block_capacity_ / 2 + 1;
        while (grown < minimum)
            grown += grown / 2 + 1;
        if (_PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(grown)) != 0) {
            PyErr_Clear();
            failed_ = true;
            // bytes_ was cleared by the failed resize.
            base_ = stage_;
            capacity_ = sizeof(stage_);
            used_ = 0;
            staged_ = false;
            return false;
        }
        base_ = PyBytes_AS_STRING(bytes_);
        capacity_ = grown;
        block_capacity_ = grown;
        return true;
    }

    std::string* str_sink_ = nullptr;
    PyObject* bytes_ = nullptr;
    char* base_ = nullptr;
    size_t capacity_ = 0;
    size_t block_capacity_ = 0; ///< bytes mode: the block's size, whichever region is live
    size_t block_used_ = 0;     ///< bytes mode: bytes committed to the block before staging
    bool staged_ = false;       ///< bytes mode: the live region is the stage
    bool failed_ = false;
    size_t used_ = 0;
    char stage_[kStageBytes];
};

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
 * A *general*-kind table stores 24-byte `{hash, key, value}` entries instead,
 * and strata's own parser makes those: `_PyDict_NewPresized` (python_builder.h
 * `new_mapping`) allocates general-kind on 3.11–3.14 whatever the key types,
 * so every record wider than five keys that `loads` builds used to be refused
 * here — a `loads` → `dumps` round trip served by `PyDict_Next` and the
 * general writer while the same data parsed by stdlib `json` took the fused
 * one. Such a table is **compacted** into the `{key, value}` view above,
 * once, out of line, on the branch that already refused it; the two hot loops
 * then read one shape and there is one definition of each. A general table
 * never becomes unicode again (CPython clamps `dictresize`'s `unicode`
 * parameter on a general source), so refusing it is permanent, not transient.
 *
 * The layout is CPython-internal, so it is mirrored minimally (the fields up
 * to the entry array, stable across 3.11–3.14 and identical for both kinds),
 * version-gated, and — the load-bearing part — **proved at runtime**: the
 * first use walks probe dicts of both kinds both ways and compares, keys,
 * values and (general only) hashes. Any mismatch, on any future build,
 * disables that kind's walk for the process and every caller silently keeps
 * `PyDict_Next`; the two proofs are independent bits, so one layout changing
 * does not cost the other its fast path. Split-table dicts (instance
 * attribute dicts) and any other kind always take the fallback.
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

/// The `{key, value}` pair a unicode table stores natively, and the shape a
/// general table is compacted into (`PyDictUnicodeEntry`).
struct Entry {
    PyObject* me_key;
    PyObject* me_value;
};

constexpr uint8_t kKindUnicode = 1; // DICT_KEYS_UNICODE
constexpr uint8_t kKindGeneral = 0; // DICT_KEYS_GENERAL

/// One entry of a general-kind table (`PyDictKeyEntry`): the hash first, so
/// the pair is 24 bytes on LP64/LLP64 and the key sits at +8, not +0.
/// Declared with the real member types rather than a hard-coded width, so
/// every ABI is right by construction.
struct GeneralEntry {
    Py_hash_t me_hash;
    PyObject* me_key;
    PyObject* me_value;
};

/// Compaction scratch width: one more than the widest schema row, so the
/// `too_many` boundary is still detected *inside* `write_mapping`'s
/// collection loop. With only the row's width the loop could not break on
/// its own and the `count != PyDict_GET_SIZE` reset would send the dict back
/// through a second walk. (It does not save the uncached `PyDict_Next` walk
/// a >24-key dict takes either way -- that one is unavoidable for any
/// non-cacheable dict, of either kind.)
constexpr Py_ssize_t kScratchEntries = 25;

/// The entry array's first byte. Shared by both kinds: the index table always
/// starts at `dk_indices` and its *total* size is `1 << dk_log2_index_bytes`
/// (CPython `_DK_ENTRIES`), whatever the entry width behind it.
[[nodiscard]] inline const char* entry_base(const KeysPrefix* keys) noexcept {
    return reinterpret_cast<const char*>(keys) + offsetof(KeysPrefix, dk_indices) +
           (static_cast<size_t>(1) << keys->dk_log2_index_bytes);
}

/// The entry array of @p dict, or nullptr when this dict cannot be walked raw.
[[nodiscard]] inline const Entry* entry_array(PyObject* dict, Py_ssize_t* entry_count) {
    auto* impl = reinterpret_cast<PyDictObject*>(dict);
    if (impl->ma_values != nullptr)
        return nullptr; // split table: values live elsewhere
    auto* keys = reinterpret_cast<const KeysPrefix*>(impl->ma_keys);
    if (keys->dk_kind != kKindUnicode)
        return nullptr;
    *entry_count = keys->dk_nentries;
    return reinterpret_cast<const Entry*>(entry_base(keys));
}

/// The general entry array of @p dict, with `dk_nentries` published, or
/// nullptr when this dict is not a walkable general table.
///
/// The two refusals are in this order on purpose. `ma_values != nullptr`
/// comes first because a *split* table's kind is `DICT_KEYS_SPLIT` and its
/// entries' `me_value` fields are meaningless -- the values live in
/// `ma_values`. And the kind test is `== kKindGeneral` exactly, never
/// `!= kKindUnicode`: the negated form would start walking split tables (and
/// any kind a future CPython adds) as if they were general.
[[nodiscard]] inline const GeneralEntry* general_entries(PyObject* dict,
                                                         Py_ssize_t* nentries) noexcept {
    auto* impl = reinterpret_cast<PyDictObject*>(dict);
    if (impl->ma_values != nullptr)
        return nullptr;
    auto* keys = reinterpret_cast<const KeysPrefix*>(impl->ma_keys);
    if (keys->dk_kind != kKindGeneral)
        return nullptr;
    *nentries = keys->dk_nentries;
    return reinterpret_cast<const GeneralEntry*>(entry_base(keys));
}

/// Copy a hole-free general table narrower than the scratch into @p out.
/// Returns the number of entries copied, or -1 when this dict is not such a
/// table. The triage -- `ma_values`, the kind, the width, then
/// `dk_nentries == ma_used` -- runs before a single 24-byte entry is read, so
/// a holed or over-wide general dict is refused at the same three loads it
/// costs today, and the scan is bounded by the dict's *size*, never by
/// `dk_nentries` (which a dict that once held a million keys still carries).
[[nodiscard]] inline Py_ssize_t compact_general_exact_unchecked(PyObject* dict,
                                                                Entry* out) noexcept {
    Py_ssize_t nentries = 0;
    const GeneralEntry* source = general_entries(dict, &nentries);
    if (source == nullptr)
        return -1;
    const Py_ssize_t size = PyDict_GET_SIZE(dict);
    // `dk_nentries == ma_used` is exactly "no deleted slot": a combined table
    // never stores a NULL value for a live key, so every slot below nentries
    // is occupied.
    if (size <= 0 || size >= kScratchEntries || nentries != size)
        return -1;
    for (Py_ssize_t index = 0; index < size; ++index) {
        out[index].me_key = source[index].me_key;
        out[index].me_value = source[index].me_value;
    }
    return size;
}

/// Copy the live entries of a general table into @p out, holes removed,
/// stopping once the scratch is full. Returns the number copied, or -1 when
/// this dict is not a walkable general table. The cap is what bounds the scan
/// on a sparse table; `write_mapping`'s own walk is `dk_nentries`-long either
/// way, so nothing here is longer than the fallback it replaces.
[[nodiscard]] inline Py_ssize_t compact_general_holes_unchecked(PyObject* dict,
                                                                Entry* out) noexcept {
    Py_ssize_t nentries = 0;
    const GeneralEntry* source = general_entries(dict, &nentries);
    if (source == nullptr)
        return -1;
    Py_ssize_t live = 0;
    for (Py_ssize_t index = 0; index < nentries && live < kScratchEntries; ++index) {
        PyObject* const value = source[index].me_value;
        if (value == nullptr)
            continue; // deleted slot
        out[live].me_key = source[index].me_key;
        out[live].me_value = value;
        ++live;
    }
    return live;
}

/// A `str` subclass instance, built through the public type constructor.
///
/// A key that is not `PyUnicode_CheckExact` converts a table to
/// DICT_KEYS_GENERAL, and the conversion never reverses -- which is how the
/// probe reaches the production shape (a general table whose live keys are
/// all exact `str`) without calling `_PyDict_NewPresized` and so without
/// resting the proof on one internal API's current behaviour.
[[nodiscard]] inline PyObject* probe_subclass_key() {
    PyRef bases(PyTuple_Pack(1, reinterpret_cast<PyObject*>(&PyUnicode_Type)));
    PyRef namespace_dict(PyDict_New());
    if (!bases || !namespace_dict)
        return nullptr;
    PyRef type(PyObject_CallFunction(reinterpret_cast<PyObject*>(&PyType_Type), "sOO",
                                     "_strata_probe_str", bases.get(), namespace_dict.get()));
    if (!type)
        return nullptr;
    return PyObject_CallFunction(type.get(), "s", "probe_subclass_key");
}

/// Insert `"<prefix><index>": index` for @p index in [0, count) into @p dict.
[[nodiscard]] inline bool probe_fill_str(PyObject* dict, const char* prefix, int count) {
    char name[32];
    for (int index = 0; index < count; ++index) {
        std::snprintf(name, sizeof(name), "%s%d", prefix, index);
        PyRef value(PyLong_FromLong(index));
        if (!value || PyDict_SetItemString(dict, name, value.get()) != 0)
            return false;
    }
    return true;
}

/// Insert `index: index` for @p index in [0, count) into @p dict. Integer
/// keys are the cheapest way to a wide general table -- and the first one is
/// what makes the table general at all.
[[nodiscard]] inline bool probe_fill_int(PyObject* dict, int count) {
    for (int index = 0; index < count; ++index) {
        PyRef key(PyLong_FromLong(index));
        PyRef value(PyLong_FromLong(index));
        if (!key || !value || PyDict_SetItem(dict, key.get(), value.get()) != 0)
            return false;
    }
    return true;
}

/// Walk @p dict's unicode entry array in lock step with `PyDict_Next`.
[[nodiscard]] inline bool probe_unicode_dict(PyObject* dict, bool wide) {
    Py_ssize_t entry_count = 0;
    const Entry* entries = entry_array(dict, &entry_count);
    if (entries == nullptr)
        return false;
    if (wide) {
        // The index table is wider than one byte per slot here, so
        // `1 << dk_log2_index_bytes` and `DK_SIZE` differ and `entry_base`'s
        // scaling rule is load-bearing rather than incidental. A witness that
        // never reaches this regime does not prove the accessor where it is
        // live (docs/context/convention.md, "Portable by construction").
        auto* keys =
            reinterpret_cast<const KeysPrefix*>(reinterpret_cast<PyDictObject*>(dict)->ma_keys);
        if (keys->dk_log2_index_bytes == keys->dk_log2_size)
            return false;
    }
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
    return walked == PyDict_GET_SIZE(dict);
}

/// Walk @p dict's *general* entry array in lock step with `PyDict_Next`, then
/// check both compactions against the same oracle.
///
/// `me_hash` is compared too, because it is the field the unicode layout has
/// no analogue for and the one a reordering *inside* the entry struct would
/// move. It discriminates best on the two narrow witnesses, whose keys are
/// long strings with unrelated hashes; the wide witness's integer keys hash
/// to themselves, so there the check is weaker than the pointer comparisons
/// beside it -- stated rather than claimed away.
[[nodiscard]] inline bool probe_general_dict(PyObject* dict, Entry* scratch, bool wide,
                                             bool holed) {
    auto* keys =
        reinterpret_cast<const KeysPrefix*>(reinterpret_cast<PyDictObject*>(dict)->ma_keys);
    if (keys->dk_kind != kKindGeneral)
        return false;
    if (wide && keys->dk_log2_index_bytes == keys->dk_log2_size)
        return false; // see probe_unicode_dict: the index-width rule must be live
    Py_ssize_t nentries = 0;
    const GeneralEntry* entries = general_entries(dict, &nentries);
    if (entries == nullptr)
        return false;
    const Py_ssize_t size = PyDict_GET_SIZE(dict);
    if (holed != (nentries > size))
        return false; // the hole the witness claims must actually be there

    Py_ssize_t position = 0;
    PyObject* expected_key = nullptr;
    PyObject* expected_value = nullptr;
    Py_ssize_t walked = 0;
    for (Py_ssize_t index = 0; index < nentries; ++index) {
        if (entries[index].me_value == nullptr)
            continue; // deleted slot
        if (!PyDict_Next(dict, &position, &expected_key, &expected_value))
            return false;
        if (entries[index].me_key != expected_key || entries[index].me_value != expected_value)
            return false;
        const Py_hash_t hash = PyObject_Hash(expected_key);
        if (hash == -1 && PyErr_Occurred())
            return false;
        if (entries[index].me_hash != hash)
            return false;
        ++walked;
    }
    if (PyDict_Next(dict, &position, &expected_key, &expected_value))
        return false; // raw walk ended early
    if (walked != size)
        return false;

    // The hole-tolerant compaction: same pairs, same order, capped.
    const Py_ssize_t live = compact_general_holes_unchecked(dict, scratch);
    if (live != (size < kScratchEntries ? size : kScratchEntries))
        return false;
    position = 0;
    for (Py_ssize_t index = 0; index < live; ++index) {
        if (!PyDict_Next(dict, &position, &expected_key, &expected_value))
            return false;
        if (scratch[index].me_key != expected_key || scratch[index].me_value != expected_value)
            return false;
    }
    // The exact compaction accepts this dict only when it has no hole and
    // fits the scratch, and then agrees pair for pair.
    const Py_ssize_t exact = compact_general_exact_unchecked(dict, scratch);
    const bool exact_expected = !holed && size > 0 && size < kScratchEntries;
    if (exact_expected != (exact == size))
        return false;
    if (exact < 0)
        return true;
    position = 0;
    for (Py_ssize_t index = 0; index < exact; ++index) {
        if (!PyDict_Next(dict, &position, &expected_key, &expected_value))
            return false;
        if (scratch[index].me_key != expected_key || scratch[index].me_value != expected_value)
            return false;
    }
    return true;
}

enum : unsigned {
    kProvedUnicode = 1u, ///< a unicode table's entry array reads correctly
    kProvedGeneral = 2u, ///< a general table's does, and both compactions agree
};

/// One-time layout proof: walk probe dicts raw and via PyDict_Next, compare.
///
/// The two halves are independent bits so that a future CPython changing one
/// layout leaves the other's fast path alone; each half's failure only turns
/// its own walk off, and both resolve together at import
/// (`prepare_dumps_runtime`) so no walk can be the first to allocate a dict.
[[nodiscard]] inline unsigned probe_unicode_layout() {
    PyRef small(PyDict_New());
    PyRef big(PyDict_New());
    PyRef wide(PyDict_New());
    if (!small || !big || !wide)
        return 0;
    if (!probe_fill_str(small.get(), "k", 4) || !probe_fill_str(big.get(), "k", 36))
        return 0;
    if (PyDict_DelItemString(small.get(), "k1") != 0) // leave a hole
        return 0;
    // 200 keys land `dk_log2_size` at 9, so the index table is 2 bytes per
    // slot and `1 << dk_log2_index_bytes` is not `DK_SIZE`.
    if (!probe_fill_str(wide.get(), "wide_unicode_probe_key_", 200))
        return 0;
    if (!probe_unicode_dict(small.get(), false) || !probe_unicode_dict(big.get(), false) ||
        !probe_unicode_dict(wide.get(), true))
        return 0;
    return kProvedUnicode;
}

[[nodiscard]] inline unsigned probe_general_layout() {
    Entry scratch[kScratchEntries];
    // (1) A narrow table with exact `str` keys and a real hole. The subclass
    // key is what makes the table general; it is removed again so the live
    // keys are the production shape, and the hole is punched *after* the last
    // resize, because a resize compacts holes away.
    PyRef holed(PyDict_New());
    PyRef dense(PyDict_New());
    PyRef subclass(probe_subclass_key());
    PyRef one(PyLong_FromLong(1));
    if (!holed || !dense || !subclass || !one)
        return 0;
    if (PyDict_SetItem(holed.get(), subclass.get(), one.get()) != 0)
        return 0;
    if (!probe_fill_str(holed.get(), "general_probe_key_", 8))
        return 0;
    if (PyDict_DelItem(holed.get(), subclass.get()) != 0)
        return 0;
    if (PyDict_SetItemString(holed.get(), "general_probe_hole", one.get()) != 0 ||
        PyDict_DelItemString(holed.get(), "general_probe_hole") != 0)
        return 0;

    // (2) A hole-free table of the same shape, so the exact compaction is
    // proved on an accepting input as well as on a refusing one.
    if (PyDict_SetItem(dense.get(), subclass.get(), one.get()) != 0)
        return 0;
    if (!probe_fill_str(dense.get(), "dense_probe_key_", 7))
        return 0;

    // (3) A wide table, for the index-width rule.
    PyRef wide(PyDict_New());
    if (!wide || !probe_fill_int(wide.get(), 200))
        return 0;

    if (!probe_general_dict(holed.get(), scratch, false, true) ||
        !probe_general_dict(dense.get(), scratch, false, false) ||
        !probe_general_dict(wide.get(), scratch, true, false))
        return 0;
    return kProvedGeneral;
}

/// Both proofs, as a bitmask. Leaves no exception set: a refusal is a
/// disabled fast path, never an error the caller has to notice.
[[nodiscard]] inline unsigned probe_layout() {
    unsigned proved = probe_unicode_layout();
    if (proved != 0)
        proved |= probe_general_layout();
    if (PyErr_Occurred())
        PyErr_Clear();
    return proved;
}

/// The one guarded static. Two accessors read it, so a general-layout refusal
/// cannot disable the unicode walk and vice versa.
[[nodiscard]] inline unsigned proved_layouts() {
    static const unsigned proved = probe_layout();
    return proved;
}

[[nodiscard]] inline bool available() { return (proved_layouts() & kProvedUnicode) != 0; }

[[nodiscard]] inline bool general_available() { return (proved_layouts() & kProvedGeneral) != 0; }

/// The gated, out-of-line forms the writers reach. `noinline` plus `cold`
/// together are what keep the compaction out of the hot writers' text and out
/// of their fallthrough, whatever `-fprofile-use` learns about how often a
/// general table arrives -- the test suite trains this path warm.
[[nodiscard]] inline STRATA_COLD_FN Py_ssize_t compact_general_exact(PyObject* dict,
                                                                     Entry* out) noexcept {
    if (!general_available())
        return -1;
    return compact_general_exact_unchecked(dict, out);
}

[[nodiscard]] inline STRATA_COLD_FN Py_ssize_t compact_general_holes(PyObject* dict,
                                                                     Entry* out) noexcept {
    if (!general_available())
        return -1;
    return compact_general_holes_unchecked(dict, out);
}

/// The fused writer's accessor: a unicode table's own entry array, or a
/// general table compacted into @p scratch.
///
/// The unicode path is `entry_array` verbatim -- the same three loads and the
/// same two branches -- so no instruction is added where it matters: the edge
/// that already existed gains a target, and that target is a `cold` call, so
/// no profile can grow it or place its body in the fallthrough. Verified by
/// disassembly on both ISAs, not assumed: the fused verification loop and
/// `write_mapping`'s collection loop come out identical instruction for
/// instruction, and both writers keep their frame size and their count of
/// frame-relative accesses (docs/decisions.md, 2026-09-12).
///
/// `entry_count` is written through, never address-escaped to the cold
/// callee: the compaction returns its count instead of taking an out
/// parameter, so the caller's variable stays a promotable SSA value.
[[nodiscard]] inline const Entry* fused_entry_array(PyObject* dict, Py_ssize_t* entry_count,
                                                    Entry* scratch) {
    const Entry* entries = entry_array(dict, entry_count);
    if (entries != nullptr)
        return entries;
    const Py_ssize_t live = compact_general_exact(dict, scratch);
    if (live < 0)
        return nullptr;
    *entry_count = live;
    return scratch;
}

/// `write_mapping`'s accessor: the same, with holes compacted away. Its
/// caller's loop bound is then the live count rather than `dk_nentries`, and
/// the loop's own `me_value == nullptr` skip simply never fires.
[[nodiscard]] inline const Entry* mapping_entry_array(PyObject* dict, Py_ssize_t* entry_count,
                                                      Entry* scratch) {
    const Entry* entries = entry_array(dict, entry_count);
    if (entries != nullptr)
        return entries;
    const Py_ssize_t live = compact_general_holes(dict, scratch);
    if (live < 0)
        return nullptr;
    *entry_count = live;
    return scratch;
}

} // namespace rawdict
#endif // STRATA_RAW_DICT_WALK

/**
 * The per-thread state a dumps() call leases: prepared keys and staged rows.
 *
 * Sharing is what lets repeated serialization of same-shaped payloads skip
 * key preparation entirely after the first call. The keys are *owned*
 * references: identity comparison is only sound while the objects live, and
 * a borrowed pointer could be freed between calls and reincarnated as a
 * different key. The busy flag covers re-entrancy -- a cycle warning can run
 * arbitrary Python, which can call dumps() again mid-walk; the nested call
 * pays for a private, empty state instead, so neither the outer walk's
 * schemas nor its staged rows are touched.
 */
class SchemaCacheLease {
  public:
    /// Keys per schema slot row; object width past this takes the plain walk.
    static constexpr size_t kSchemaSlots = 24;
    /// Bytes per inline `"key":` slot; a wider span falls back to heap spans.
    static constexpr size_t kSlotBytes = 16;
    /// Nesting levels with a schema slot and a staged row of their own;
    /// deeper objects take the plain walk (python_dumps.cpp kMaxCachedDepth).
    static constexpr size_t kMaxDepth = 64;

    /// Prepared `"key":` bytes for one object shape at one depth.
    ///
    /// The prepared bytes live *inline*, one fixed 16-byte slot per key, so
    /// the emit loop's whole read stream is this contiguous struct. The
    /// benchmark harness runs a full gc.collect() before every timed call,
    /// and the collector's traversal re-warms every dict's own internals —
    /// the memory a direct-walk serializer reads — while evicting side
    /// structures; with blob/offsets as separate heap allocations the emit
    /// path paid three cold hops per record under exactly that condition
    /// (parity isolated, behind under the harness, on every x86 leg).
    /// Spans wider than a slot keep the heap blob as the fallback.
    struct Schema {
        std::vector<PyObject*> keys;                ///< owned references
        std::string blob;                           ///< fallback bytes for wide spans only
        std::vector<uint32_t> offsets{0};           ///< fallback boundaries, keys.size() + 1
        uint8_t spans[kSchemaSlots] = {};           ///< inline `"key":` length per slot
        char slots[kSchemaSlots * kSlotBytes] = {}; ///< the inline bytes
        PyObject* key_row[kSchemaSlots] = {};       ///< borrowed copies for the verify scan
        bool prepared = false;
        bool wide = false; ///< some span exceeded a slot: emit from the blob

        /// Move-only, deliberately. `keys` holds *owned* references, so the
        /// one relocation this type sees -- `State::schemas` growing when a
        /// deeper level is first written -- must transfer them. A copy would
        /// duplicate the raw pointers into a second slot and hand
        /// `release_keys` two releases for one reference; deleting it makes
        /// that the compiler's rule rather than a comment's. No destructor is
        /// declared here on purpose: one would suppress the implicit moves and
        /// silently turn that relocation back into a copy.
        ///
        /// The moves are defaulted *without* an explicit `noexcept`: written
        /// out, the specification would be believed rather than computed
        /// (C++20 P1286R2), and a future throwing member would call
        /// `std::terminate` mid-relocation instead of failing the
        /// `is_nothrow_move_constructible_v` assertion below. Deduced, that
        /// assertion is a real guard.
        Schema() = default;
        Schema(Schema&&) = default;
        Schema& operator=(Schema&&) = default;
        Schema(const Schema&) = delete;
        Schema& operator=(const Schema&) = delete;

        void remember(PyObject* const* other, Py_ssize_t count) {
            // The one step that can allocate, taken before anything is
            // mutated: a throw here leaves the way exactly as it was, rather
            // than with its old references already dropped and a vector the
            // next remember() would release a second time. `assign` cannot
            // then allocate, so the rest of this runs to completion.
            keys.reserve(static_cast<size_t>(count));
            for (PyObject* key : keys)
                Py_DECREF(key);
            keys.assign(other, other + count);
            for (PyObject* key : keys)
                Py_INCREF(key);
            // Borrowed duplicates of the owned vector, inline: the per-record
            // verify scan reads this row instead of chasing the vector's heap
            // storage (cold after the harness's per-call gc.collect()).
            for (Py_ssize_t index = 0; index < count; ++index)
                key_row[static_cast<size_t>(index)] = other[index];
            prepared = false;
            wide = false;
        }

        /// Drop the remembered shape: release the owned keys and blank the
        /// borrowed row, leaving nothing an identity compare could hit. The
        /// prepared bytes are not cleared -- `prepared` is false, and
        /// `build_schema` resets `blob` and `offsets` before it writes them.
        void forget() {
            prepared = false;
            wide = false;
            for (PyObject* key : keys)
                Py_DECREF(key);
            keys.clear();
            for (PyObject*& remembered : key_row)
                remembered = nullptr;
        }

        /// Keys past the first, by identity. `select` has already matched the
        /// count and the first key from its inline arrays; only a plausible
        /// hit pays for touching this vector's heap storage. Identity, not
        /// equality: strata interns the keys it parses and CPython interns
        /// identifier-like literals, so same-schema records share key
        /// objects. A miss costs a rebuild, never a wrong answer.
        [[nodiscard]] bool matches_tail(PyObject* const* other, Py_ssize_t count) const noexcept {
            for (Py_ssize_t index = 1; index < count; ++index) {
                if (key_row[static_cast<size_t>(index)] != other[index])
                    return false;
            }
            return true;
        }
    };

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

        /// Take one way out of service: it stops matching *before* its owned
        /// keys are released, so no select() can reach a slot whose shape is
        /// half gone. `counts` back at -1 is what un-matches it -- callers
        /// guarantee count >= 1, so -1 can never be asked for.
        void invalidate(size_t way) {
            counts[way] = -1;
            first_keys[way] = nullptr;
            ways[way].forget();
        }

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
                for (size_t way = 0; way < kWays; ++way)
                    invalidate(way);
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

    /**
     * The staged key and value rows of one dict nesting level.
     *
     * The dict writers read a record's keys and values once -- a walk that
     * runs no user code -- and then emit from that row, so the record that
     * reaches the output is the one the serializer read. The row is *leased*
     * rather than declared as a local array, because a local array in
     * write_mapping or write_record_fused is a local array in every function
     * they are inlined into: under `-fprofile-use` the whole dict writer
     * folds into the per-value dispatcher, and a 192-byte row there becomes a
     * 448-byte frame plus a stack-protector canary on `write` and
     * `write_sequence` -- paid by every value of every document, arrays of
     * scalars included (build/evidence/E26-P2/BUILDS.md). Leasing it keeps
     * the hot walk free of stack arrays under any inlining decision the
     * profile makes.
     *
     * One row per nesting level is exactly what the recursion can have live
     * at once: `map_depth_` is unique along the walk's path, so a level's row
     * belongs to the single write_mapping / write_record_fused frame at that
     * level, and a sibling record reuses it only after that frame -- and the
     * RowLock registered over it -- has gone.
     */
    struct StagedRow {
        PyObject* keys[kSchemaSlots];
        PyObject* values[kSchemaSlots];
    };

    /// One registration of a staged row on the serializer's row list.
    ///
    /// Leased for the second half of the same reason the rows are: an
    /// intrusive list node *is* an address-taken local, and a function with
    /// one gets a stack-protector canary under `-fstack-protector-strong`
    /// (the Linux legs' default) whatever its frame contains. Holding the
    /// node here leaves the frame guard two pointers the optimizer keeps in
    /// registers. Two nodes per level is what the recursion can have live:
    /// write_mapping_body registers its values row and, on the branch that
    /// still reads staged keys, its keys row; the fused writer registers one.
    struct RowNode {
        RowNode* next;
        PyObject* const* row;
        uint8_t first;
        uint8_t last;
        bool latched;
    };

    /// Everything one dumps() call leases: a schema set and a staged row per
    /// nesting level. Heap-resident and never moved, so a row pointer stays
    /// valid for the whole walk (`schemas` may reallocate; `rows` may not).
    struct State {
        std::vector<DepthSchemas> schemas;
        /// Indexed by dict nesting level. Index `kMaxDepth` is the scratch
        /// every deeper level stages into and *none of them reads*:
        /// write_mapping_body hands a level that deep to
        /// write_mapping_uncached, which walks the dict itself, before it
        /// touches a row. Levels never overlap in a row of their own, so
        /// only the levels past the cache share.
        StagedRow rows[kMaxDepth + 1];
        /// Two registration nodes per level, addressed rather than pushed:
        /// only levels inside the schema cache ever register one.
        RowNode locks[2 * (kMaxDepth + 1)];
#if defined(STRATA_RAW_DICT_WALK)
        /// Compaction scratch for general-kind tables: one per lease, not one
        /// per level. A compacted array is fully consumed -- into the staged
        /// row by the fused writer, into the keys/values row by
        /// write_mapping -- before any step that can recurse or run user
        /// code, which is the same point the raw `entries` pointer already
        /// stopped being readable (python_dumps.cpp, the fused writer's
        /// "never touches `entries` again"). So no two levels hold one, and
        /// the copy is in fact safer than the pointer: user code that frees
        /// the table cannot reach it.
        ///
        /// Leased for the reason StagedRow is: a stack array here is a stack
        /// array in every function the profile inlines its holder into, and
        /// it drags a stack-protector canary.
        rawdict::Entry general[rawdict::kScratchEntries];
#endif
    };

    SchemaCacheLease() {
        if (!busy_) {
            State* const state = shared();
            if (state != nullptr) {
                busy_ = true;
                state_ = state;
                owns_flag_ = true;
                return;
            }
        }
        // Re-entrant, or the thread's own state could not be allocated: a
        // private state, so the outer walk keeps its schemas and its rows.
        // A failed allocation is reported by ok(), not swallowed.
        fallback_.reset(new (std::nothrow) State);
        state_ = fallback_.get();
    }

    /// Ends the lease. The private state dies with the call, and its schemas
    /// own the keys they remembered, so they are released here -- the shared
    /// state's are not, because it outlives every call by design (see
    /// shared()).
    ///
    /// Safe on the exceptional exit too, and this is the last local of
    /// dumps_to_python to be destroyed, so every Frame, RowLock and the
    /// Serializer itself have already gone: nothing borrows these keys any
    /// more. The release cannot run user code or disturb a pending exception
    /// either -- only exact `str` keys are ever remembered (python_dumps.cpp
    /// `exact_keys`), and a `str` has neither `__del__` nor weakrefs.
    ~SchemaCacheLease() {
        if (fallback_)
            release_keys(*fallback_);
        if (owns_flag_)
            busy_ = false;
    }

    SchemaCacheLease(const SchemaCacheLease&) = delete;
    SchemaCacheLease& operator=(const SchemaCacheLease&) = delete;

    /// False when even the private state could not be allocated.
    [[nodiscard]] bool ok() const noexcept { return state_ != nullptr; }

    [[nodiscard]] State& state() noexcept { return *state_; }

  private:
    /// Release every owned key a private state remembered, way by way.
    ///
    /// `invalidate` un-matches the way before it releases, so the state is
    /// left consistent rather than half torn down -- it is about to be freed,
    /// but a slot that still matched a shape whose keys are gone is exactly
    /// the hazard E26-FIX2 closed, and there is no second rule here.
    static void release_keys(State& state) noexcept {
        for (DepthSchemas& depth : state.schemas) {
            for (size_t way = 0; way < DepthSchemas::kWays; ++way)
                depth.invalidate(way);
        }
    }

    [[nodiscard]] static State* shared() {
        // Deliberately leaked: a destructor after interpreter shutdown could
        // not legally Py_DECREF the owned keys anyway. That is the whole of
        // the shutdown policy -- the *private* state a re-entrant lease takes
        // is released instead, but inside the call that made it, with the
        // interpreter running (see ~SchemaCacheLease). Default-initialized,
        // not value-initialized: the staged rows are scratch that is always
        // written before it is read, and zeroing 25 KB per thread would buy
        // nothing.
        static thread_local State* instance = new (std::nothrow) State;
        return instance;
    }

    static thread_local bool busy_;

    std::unique_ptr<State> fallback_;
    State* state_ = nullptr;
    bool owns_flag_ = false;
};

inline thread_local bool SchemaCacheLease::busy_ = false;

// The keys a schema slot remembers are owned references, released one by one
// when a private lease ends. The only relocation such a slot ever sees is
// `State::schemas` growing as a deeper level is first written, and it has to
// move: a copy would leave two slots holding the same raw pointers, and the
// release would run twice on one reference.
static_assert(std::is_nothrow_move_constructible_v<SchemaCacheLease::DepthSchemas>,
              "schema slots must relocate by move, never by copy");
static_assert(!std::is_copy_constructible_v<SchemaCacheLease::DepthSchemas>,
              "schema slots own key references and must not be copyable");

} // namespace strata::bindings

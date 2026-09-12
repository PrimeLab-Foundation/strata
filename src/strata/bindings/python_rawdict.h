#pragma once

/**
 * @file python_rawdict.h
 * @brief The runtime-proved direct walk over a dict's entry array.
 *
 * Split out of python_dumps_output.h for the ~800-line rule, and
 * self-contained: it needs `PyRef` and nothing else of the serializer. Both
 * dict writers in python_dumps.cpp read a record through it, and the layout
 * proof that gates it resolves once at import, in `prepare_dumps_runtime`.
 *
 * Everything here is either inlined into a writer's hot path (`entry_array`,
 * the two `*_available` bits) or runs once per process / on an already-refused
 * edge — and everything in the second group carries `STRATA_COLD_FN`. That is
 * not decoration: on ELF, `cold` is what puts a body in `.text.unlikely.`, and
 * the probe chain below is ~3.3 KB of import-only code that would otherwise be
 * emitted inside the serializer's hot cluster on the Linux legs.
 */

#include "python_types.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace strata::bindings {

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
///
/// `_unchecked` because the layout bit is the caller's: the serializer's cold
/// wrapper tests `general_available()` first, and the probe below calls this
/// *before* that bit exists -- running it against the `PyDict_Next` oracle is
/// what establishes it.
[[nodiscard]] inline STRATA_COLD_FN Py_ssize_t
compact_general_exact_unchecked(PyObject* dict, Entry* out) noexcept {
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
[[nodiscard]] inline STRATA_COLD_FN Py_ssize_t
compact_general_holes_unchecked(PyObject* dict, Entry* out) noexcept {
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
[[nodiscard]] inline STRATA_COLD_FN PyObject* probe_subclass_key() {
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
[[nodiscard]] inline STRATA_COLD_FN bool probe_fill_str(PyObject* dict, const char* prefix,
                                                        int count) {
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
[[nodiscard]] inline STRATA_COLD_FN bool probe_fill_int(PyObject* dict, int count) {
    for (int index = 0; index < count; ++index) {
        PyRef key(PyLong_FromLong(index));
        PyRef value(PyLong_FromLong(index));
        if (!key || !value || PyDict_SetItem(dict, key.get(), value.get()) != 0)
            return false;
    }
    return true;
}

/// Walk @p dict's unicode entry array in lock step with `PyDict_Next`.
[[nodiscard]] inline STRATA_COLD_FN bool probe_unicode_dict(PyObject* dict, bool wide) {
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
[[nodiscard]] inline STRATA_COLD_FN bool probe_general_dict(PyObject* dict, Entry* scratch,
                                                            bool wide, bool holed) {
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
[[nodiscard]] inline STRATA_COLD_FN unsigned probe_unicode_layout() {
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

[[nodiscard]] inline STRATA_COLD_FN unsigned probe_general_layout() {
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
[[nodiscard]] inline STRATA_COLD_FN unsigned probe_layout() {
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

// How a writer reaches the general half. There is deliberately no accessor
// here that takes the scratch as an argument: a `scratch` parameter has to be
// materialized before the call, and the compiler schedules that load onto the
// *unicode* path, where it is dead (measured: one load in `write_record_fused`
// on both ISAs, two in `write_mapping` on x86-64, since `this` had to be
// reloaded from the frame to reach it). Instead each writer keeps
// `entry_array` inline exactly as it was, and its `nullptr` edge calls a cold
// member that finds the lease's scratch off `this` -- already live in a
// register in both writers. The unicode path then loads nothing it did not
// load before the general half existed, on either ISA, and its per-key loops
// come back from the compiler unchanged -- verbatim on arm64, and on x86-64
// modulo frame-slot renumbering, with `write_mapping`'s collection loop one
// memory operand better. Per-ISA counts and the method: E26-P23 in
// docs/performance/experiment-ledger.md (docs/decisions.md, 2026-09-12).

} // namespace rawdict
#endif // STRATA_RAW_DICT_WALK

} // namespace strata::bindings

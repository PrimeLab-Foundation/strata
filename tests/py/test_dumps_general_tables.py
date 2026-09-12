"""Dumps contract: general-kind keys tables serialize like unicode ones.

CPython 3.11-3.14 gives a dict one of two keys-table layouts. A table built
only from exact ``str`` keys is DICT_KEYS_UNICODE, 16-byte ``{key, value}``
entries; a table that ever held any other key -- and every table
``_PyDict_NewPresized`` returns, which is what strata's own parser uses for
records wider than five keys (``python_builder.h`` ``new_mapping``) -- is
DICT_KEYS_GENERAL, 24-byte ``{hash, key, value}`` entries. The serializer
reads both: a general table is compacted into the unicode shape once, out of
line, and the two hot walks see one layout
(``python_rawdict.h`` ``rawdict``).

Every assertion here is a *parity* assertion, because that is the whole
contract: a general table's bytes are its unicode twin's bytes and stdlib
``json``'s bytes. The one exception is the cycle-placement case, which the
stdlib oracle cannot express -- there the unicode twin is the oracle, and the
change these tests guard is that a general record now reaches the fused
record writer and so inherits its documented placement (docs/decisions.md
2026-09-12, api.md § Config ``cycle_policy``).

There is no C++ twin for this file: the code under test is in
``src/strata/bindings/``, which includes ``Python.h``, and ``tests/cpp/`` is
core-only by the core-purity rule. The parity requirement is met by this
file and its byte-identical mirror in ``tests/py/``.
"""

import ctypes
import gc
import json
import sys
import sysconfig
import threading
import warnings

import pytest

import strata

# ---------------------------------------------------------------------------
# Building a general-kind table from pure Python
# ---------------------------------------------------------------------------
#
# A non-``str`` key converts a table to DICT_KEYS_GENERAL and the conversion
# never reverses: CPython clamps ``dictresize``'s ``unicode`` parameter when
# the source table is general, so growing, copying, ``{**d}``, ``update`` and
# delete-then-reinsert all keep it. That is what lets these helpers reach the
# layout on every supported version without ctypes and without depending on
# strata's own presize.


def general(pairs):
    """A dict whose keys table is DICT_KEYS_GENERAL, holding only the pairs."""
    document = {0: None}
    document.pop(0)
    for key, value in pairs:
        document[key] = value
    return document


def general_holed(pairs):
    """The same, with a deleted slot that survives to the serializer.

    The delete has to come *after* the last insertion: a resize compacts
    holes away, so punching one first and then filling to width leaves no
    hole at all. ``test_the_general_layout_is_actually_exercised`` checks
    that this really does leave ``dk_nentries > len(d)``.
    """
    document = general(pairs)
    document["__strata_test_hole__"] = None
    del document["__strata_test_hole__"]
    return document


def unicode_twin(pairs):
    """The same content in a DICT_KEYS_UNICODE table."""
    return dict(pairs)


def items(width, first=0):
    return [(f"field_{index}", index + first) for index in range(width)]


# ---------------------------------------------------------------------------
# Shared oracles and harness
# ---------------------------------------------------------------------------


def _compact(obj):
    return json.dumps(obj, separators=(",", ":"), ensure_ascii=False).encode()


def _both_modes(doc):
    out = strata.dumps(doc, return_type="bytes")
    assert out == _compact(doc)
    assert strata.dumps(doc).encode() == out
    return out


def _matches_its_unicode_twin(general_doc, unicode_doc):
    """The parity the whole change is about, in both return types."""
    assert _both_modes(general_doc) == _both_modes(unicode_doc)


def _on_a_fresh_thread(work):
    """Run ``work`` on a new thread and re-raise whatever it raised.

    The prepared-key cache is thread-local and a depth retires permanently
    after 64 misses, so a test that means to exercise the fused record writer
    has to run on a thread the rest of the suite has not retired
    (test_dumps_contract.py says the same at more length).
    """
    raised = []

    def run():
        try:
            work()
        except BaseException as error:  # noqa: BLE001 -- re-raised on the test thread
            raised.append(error)

    thread = threading.Thread(target=run)
    thread.start()
    thread.join()
    if raised:
        raise raised[0]


# ---------------------------------------------------------------------------
# Reading the layout, for the coverage canary only
# ---------------------------------------------------------------------------
#
# Offsets are computed, never hard-coded: under ``Py_GIL_DISABLED``
# ``PyObject_HEAD`` is 32 bytes rather than 16, so ``ma_keys`` moves. That
# build has no CI leg, so the canary skips it outright instead of reading
# memory at a guessed address.

_WORD = ctypes.sizeof(ctypes.c_ssize_t)
_FREE_THREADED = bool(sysconfig.get_config_var("Py_GIL_DISABLED"))
# PyObject_HEAD, then ma_used and the version/watcher tag, then ma_keys.
_MA_KEYS_OFFSET = 2 * _WORD + 2 * _WORD
_LAYOUT_READABLE = (
    sys.implementation.name == "cpython"
    and (3, 11) <= sys.version_info < (3, 15)
    and not _FREE_THREADED
    and _WORD == 8
)
_KIND_GENERAL = 0
_KIND_UNICODE = 1


def _keys_table(document):
    return ctypes.c_void_p.from_address(id(document) + _MA_KEYS_OFFSET).value


def _dk_kind(document):
    return ctypes.c_uint8.from_address(_keys_table(document) + 10).value


def _dk_nentries(document):
    return ctypes.c_ssize_t.from_address(_keys_table(document) + 24).value


def _prefix_is_plausible():
    """Check the mirrored prefix against fresh witnesses before trusting it.

    ``_MA_KEYS_OFFSET`` is computed from the documented field order, but a
    future 3.x inside the version window could still move a field ahead of
    ``ma_keys`` -- and then everything below dereferences whatever integer
    happens to sit there. So each kind gets a witness whose answers are known
    in advance: the pointer has to be a non-null aligned address, the kind one
    CPython defines, and ``dk_nentries`` at least the dict's size. An
    arbitrary tag word passes none of those. This is the test's own guard, not
    the serializer's -- the C++ side reads ``ma_keys`` through the real struct
    from the CPython headers and never computes an offset.
    """
    for witness in ({"a": 1, "b": 2, "c": 3}, general(items(8))):
        address = _keys_table(witness)
        if not address or address % _WORD:
            return False
        if ctypes.c_uint8.from_address(address + 10).value not in (0, 1, 2):
            return False
        if ctypes.c_ssize_t.from_address(address + 24).value < len(witness):
            return False
    return True


_PREFIX_PLAUSIBLE = _LAYOUT_READABLE and _prefix_is_plausible()
_NO_LAYOUT = "the keys-table layout is only mirrored for CPython 3.11-3.14 with the GIL"
_MOVED_PREFIX = "the mirrored PyDictObject prefix moved -- re-audit _MA_KEYS_OFFSET"


def _require_layout():
    """Skip, loudly and distinguishably, when the canary cannot read."""
    if not _LAYOUT_READABLE:
        pytest.skip(_NO_LAYOUT)
    if not _PREFIX_PLAUSIBLE:
        pytest.skip(_MOVED_PREFIX)


# ---------------------------------------------------------------------------
# 1. A general table dumps like its unicode twin, at every width
# ---------------------------------------------------------------------------


def test_general_tables_dump_like_their_unicode_twins():
    """The layouts are an implementation detail: same content, same bytes.

    Widths 1..26 straddle both boundaries that matter -- CPython's presize
    no-op boundary at five members, and ``kMaxSchemaKeys`` at 24.
    """

    def work():
        for width in range(1, 27):
            pairs = items(width)
            _matches_its_unicode_twin(general(pairs), unicode_twin(pairs))
            # And as records of an array, which is the shape the fused record
            # writer serves.
            _matches_its_unicode_twin(
                [general(items(width, first)) for first in range(6)],
                [unicode_twin(items(width, first)) for first in range(6)],
            )

    _on_a_fresh_thread(work)


# ---------------------------------------------------------------------------
# 2. The defect's own contract: a strata round trip equals a json one
# ---------------------------------------------------------------------------


def _round_trip_widths(warm):
    for width in range(1, 27):
        records = [{f"field_{index}": index + row for index in range(width)} for row in range(4)]
        text = json.dumps(records)
        by_json = json.loads(text)
        by_strata = strata.loads(text)
        assert by_strata == by_json
        if warm:
            for _ in range(3):
                strata.dumps(by_strata)
        assert strata.dumps(by_strata) == strata.dumps(by_json) == _compact(by_json).decode()
        assert (
            strata.dumps(by_strata, return_type="bytes")
            == strata.dumps(by_json, return_type="bytes")
            == _compact(by_json)
        )
        # bytes input reaches the same builder
        assert strata.dumps(strata.loads(text.encode())) == _compact(by_json).decode()


@pytest.mark.parametrize("warm", [False, True])
def test_strata_parsed_records_dump_like_json_parsed_ones(warm):
    """api.md § dumps: the output is a function of the value, not of how it
    was built. ``strata.loads`` presizes records above five members, which is
    what gives them a general keys table; before the compaction they took a
    different writer and paid for it."""
    _on_a_fresh_thread(lambda: _round_trip_widths(warm))


def test_a_narrow_record_after_a_wide_one_at_the_same_depth():
    """The builder's size hint is per depth and survives across ``loads``
    calls on a leased builder, so one wide document poisons later narrow ones
    at that depth (they are presized, hence general). Their bytes must not
    notice."""

    def work():
        wide = json.dumps([{f"field_{index}": index for index in range(9)} for _ in range(3)])
        strata.loads(wide)
        narrow_text = json.dumps([{"a": 1, "b": 2} for _ in range(3)])
        parsed = strata.loads(narrow_text)
        assert parsed == json.loads(narrow_text)
        assert strata.dumps(parsed) == _compact(json.loads(narrow_text)).decode()
        assert strata.dumps(parsed, return_type="bytes") == _compact(json.loads(narrow_text))

    _on_a_fresh_thread(work)


# ---------------------------------------------------------------------------
# 3. Holes, refills and tables grown past their usable size
# ---------------------------------------------------------------------------


def test_general_tables_with_holes_and_refills():
    """The matrix of test_fused_verification.py, re-run over general tables:
    a deleted slot must be skipped and a reinserted key must land at the end.
    A holed general table is refused by the fused writer exactly as a holed
    unicode one is -- the compaction only accepts a table whose
    ``dk_nentries`` equals its size."""

    def work():
        records = []
        twins = []
        for width in (1, 4, 24):
            keys = [f"field_{index}" for index in range(width)]
            for deleted in range(width):
                full = [(key, None) for key in keys]
                for _ in range(8):
                    records.append(general(full))
                    twins.append(unicode_twin(full))
                hole = general(full)
                hole_twin = unicode_twin(full)
                del hole[keys[deleted]]
                del hole_twin[keys[deleted]]
                reinserted = dict(hole)
                reinserted_twin = dict(hole_twin)
                reinserted[keys[deleted]] = [deleted, True]
                reinserted_twin[keys[deleted]] = [deleted, True]
                records.extend([hole, reinserted, general(full)])
                twins.extend([hole_twin, reinserted_twin, unicode_twin(full)])
        for _ in range(3):
            _matches_its_unicode_twin(records, twins)

    _on_a_fresh_thread(work)


def test_general_tables_holed_after_their_last_resize():
    """``general_holed`` punches the hole after the table stopped growing, so
    ``dk_nentries`` really does exceed the size -- the case a hole punched
    before the fill would not reach, because the resize compacts it away."""

    def work():
        for width in (1, 6, 11, 24, 30):
            pairs = items(width)
            _matches_its_unicode_twin(general_holed(pairs), unicode_twin(pairs))
            _matches_its_unicode_twin(
                [general_holed(items(width, first)) for first in range(6)],
                [unicode_twin(items(width, first)) for first in range(6)],
            )

    _on_a_fresh_thread(work)


def test_general_tables_grown_past_their_usable_size():
    """A presized table grown past ``dk_usable`` resizes and stays general.
    Nothing pinned that before."""

    def work():
        grown = []
        twins = []
        for row in range(6):
            document = general(items(6, row))
            twin = unicode_twin(items(6, row))
            for index in range(6, 30):
                document[f"field_{index}"] = index + row
                twin[f"field_{index}"] = index + row
            grown.append(document)
            twins.append(twin)
        _matches_its_unicode_twin(grown, twins)

    _on_a_fresh_thread(work)


# ---------------------------------------------------------------------------
# 4. Past the schema row
# ---------------------------------------------------------------------------


def test_general_tables_past_the_schema_width():
    """``kMaxSchemaKeys`` is 24 and the compaction scratch holds 25, so the
    truncation boundary is detected in the collection loop rather than by a
    second walk. 24/25/26/40 keys must all come out right."""

    def work():
        for width in (23, 24, 25, 26, 40):
            pairs = items(width)
            _matches_its_unicode_twin(general(pairs), unicode_twin(pairs))
            _matches_its_unicode_twin(general_holed(pairs), unicode_twin(pairs))
            _matches_its_unicode_twin(
                [general(items(width, first)) for first in range(4)],
                [unicode_twin(items(width, first)) for first in range(4)],
            )
        # Mixed widths at one depth, so the cache is asked about both sides of
        # the boundary in one document.
        widths = (24, 25, 24, 25, 26, 24)
        _matches_its_unicode_twin(
            [general(items(width)) for width in widths],
            [unicode_twin(items(width)) for width in widths],
        )

    _on_a_fresh_thread(work)


# ---------------------------------------------------------------------------
# 5. Non-str keys
# ---------------------------------------------------------------------------


def test_a_general_table_with_a_non_str_key_raises_the_same_typeerror():
    """api.md § dumps: 'dict keys must be `str` (else `TypeError`)'. The
    offending key and the message are the walk-order ones, whichever layout
    the table has."""
    for factory in (general, general_holed):
        document = factory([("a", 1)])
        document[2] = 3
        with pytest.raises(TypeError, match="keys must be str, not int"):
            strata.dumps(document)
        with pytest.raises(TypeError, match="keys must be str, not int"):
            strata.dumps(document, return_type="bytes")
    # Wide enough to have been a fused-writer candidate had the key been str.
    wide = general(items(11))
    wide[(1, 2)] = None
    with pytest.raises(TypeError, match="keys must be str, not tuple"):
        strata.dumps(wide)


# ---------------------------------------------------------------------------
# 6. Nesting: the single compaction scratch is shared across levels
# ---------------------------------------------------------------------------


def test_general_records_nested_under_general_records():
    """One compaction scratch is leased per call, not per nesting level. It
    is safe because both walks are finished with the compacted array before
    anything can recurse -- and a lifetime bug there shows up as wrong bytes
    only on documents like these."""

    def leaf(width, seed):
        return general(items(width, seed))

    def level(depth, width, seed, factory):
        if depth == 0:
            return factory(items(width, seed))
        inner = factory(items(width, seed))
        inner[f"field_{width}"] = level(depth - 1, width, seed + 1, factory)
        inner[f"field_{width + 1}"] = [
            level(depth - 1, width, seed + step, factory) for step in range(3)
        ]
        return inner

    def work():
        for width in (3, 11, 23, 24, 25):
            for depth in (1, 2, 3):
                _matches_its_unicode_twin(
                    [level(depth, width, seed, general) for seed in range(4)],
                    [level(depth, width, seed, unicode_twin) for seed in range(4)],
                )
        # General leaves under unicode parents and the other way round, so the
        # scratch is entered and left at alternating levels.
        mixed = [
            {"id": index, "inner": leaf(11, index), "rows": [leaf(7, index), leaf(25, index)]}
            for index in range(6)
        ]
        _both_modes(mixed)
        _both_modes([general([("id", index), ("rows", [{"a": index}] * 3)]) for index in range(6)])

    _on_a_fresh_thread(work)


# ---------------------------------------------------------------------------
# 7. Cycles reached through a general record
# ---------------------------------------------------------------------------

_WIDE_KEYS = tuple(f"k{index}" for index in range(24))


def _cycle_documents(factory):
    """(label, build, warm) triples over containers of one keys-table kind."""

    def self_reference():
        document = factory([("name", "root"), ("self", None)])
        document["self"] = document
        return document

    def two_levels():
        document = factory([("a", 1), ("b", None)])
        document["b"] = factory([("a", 2), ("b", document)])
        return document

    def wide():
        document = factory([(key, 1) for key in _WIDE_KEYS[:-1]] + [(_WIDE_KEYS[-1], None)])
        document[_WIDE_KEYS[-1]] = document
        return document

    return [
        (
            "self",
            self_reference,
            factory([("name", "root"), ("self", factory([("name", "x"), ("self", 1)]))]),
        ),
        (
            "two_levels",
            two_levels,
            factory(
                [
                    ("a", 1),
                    ("b", factory([("a", 1), ("b", factory([("a", 1), ("b", 0)]))])),
                ],
            ),
        ),
        (
            "wide",
            wide,
            factory(
                [(key, 1) for key in _WIDE_KEYS[:-1]]
                + [
                    (
                        _WIDE_KEYS[-1],
                        factory(
                            [(key, 1) for key in _WIDE_KEYS[:-1]] + [(_WIDE_KEYS[-1], 0)],
                        ),
                    ),
                ],
            ),
        ),
    ]


def _with_cycle_policy(policy, call):
    previous = strata.config.get("cycle_policy")
    strata.config.set("cycle_policy", policy)
    try:
        _on_a_fresh_thread(call)
    finally:
        strata.config.set("cycle_policy", previous)


@pytest.mark.parametrize("mode", ["str", "bytes"])
@pytest.mark.parametrize("warmed", [False, True])
@pytest.mark.parametrize("policy", ["warn", "error", "ignore"])
def test_cycles_through_general_records_match_their_unicode_twins(policy, warmed, mode):
    """api.md § Config, ``cycle_policy``, and its placement caveat. A general
    record now reaches the fused record writer, so its placeholder lands where
    a unicode record's does -- which is what this pins, since stdlib ``json``
    cannot express the caveat at all."""

    def dump(value):
        if mode == "bytes":
            return strata.dumps(value, return_type="bytes").decode()
        return strata.dumps(value)

    def run():
        general_cases = _cycle_documents(general)
        unicode_cases = _cycle_documents(unicode_twin)
        for (label, build, warm), (_, twin_build, twin_warm) in zip(
            general_cases,
            unicode_cases,
            strict=True,
        ):
            if warmed:
                for _ in range(3):
                    dump(warm)
                    dump(twin_warm)
            if policy == "error":
                with pytest.raises(ValueError, match="Circular reference detected"):
                    dump(build())
                with pytest.raises(ValueError, match="Circular reference detected"):
                    dump(twin_build())
                continue
            with warnings.catch_warnings(record=True) as caught:
                warnings.simplefilter("always")
                produced = dump(build())
                expected = dump(twin_build())
            assert produced == expected, label
            messages = [str(item.message) for item in caught]
            if policy == "warn":
                assert messages == ["Circular reference detected"] * 2, label
            else:
                assert messages == [], label

    _with_cycle_policy(policy, run)


# ---------------------------------------------------------------------------
# 8. str-subclass keys keep out of the cache, on either layout
# ---------------------------------------------------------------------------


class _Key(str):
    __slots__ = ()


def test_str_subclass_keys_on_a_general_table():
    """A ``str`` subclass key keeps a dict out of the schema cache (its keys
    are owned there, and releasing a subclass can run user code) and off the
    fused writer. Its layout is general either way -- a non-exact key makes it
    so -- and the bytes are the plain walk's."""
    records = []
    for index in range(6):
        document = general(items(4, index))
        document[_Key("subclass")] = index
        records.append(document)
    _both_modes(records)
    wide = general(items(23))
    wide[_Key("subclass")] = 1
    _both_modes(wide)
    _both_modes([wide, wide])


# ---------------------------------------------------------------------------
# 9. Mutation from inside the walk, on a general table
# ---------------------------------------------------------------------------


def test_mutating_a_general_record_mid_emit_keeps_the_row_read_on_entry():
    """api.md § dumps, "Mutation during serialization": a dict of at most 24
    exact-``str`` keys, below 64 levels of dict nesting, "is emitted as the
    row read on entry". The clause now covers general tables too -- they reach
    the same two writers -- and nothing executed it on that layout:
    test_dumps_reentrancy.py, its review twin and test_dumps_cycles_fused.py
    build every dict from a literal, which is always DICT_KEYS_UNICODE.

    The mutation runs at the sanctioned user-code step that needs neither a
    warnings filter nor an interpreter setting: ``__str__`` of an ``int``
    subclass beyond int64. It fires in the *middle* of the row, clears the
    record and refills it with 40 other keys, then collects -- so the keys the
    writer borrowed and the values it has not written yet are all orphaned
    while the row is staged, which is what the row lock exists for. The bytes
    must be the row read on entry, and the same for both layouts.
    """

    class _Mutating(int):
        victim = None

        def __str__(self):
            self.victim.clear()
            for index in range(40):
                self.victim[f"late_{index}"] = index
            gc.collect()
            return int.__str__(self)

    big = 2**70

    def emitted(factory, width, mode):
        # One record per call: the mutation is destructive, so a second dump
        # of the same object would be measuring the refilled dict.
        # Warm the depth first, so this shape reaches the fused record writer.
        strata.dumps([factory(items(width)) for _ in range(8)])
        record = factory(items(width))
        trigger = _Mutating(big)
        trigger.victim = record
        record[f"field_{width // 2}"] = trigger
        if mode == "bytes":
            return strata.dumps([record], return_type="bytes").decode()
        return strata.dumps([record])

    def run():
        for width in (4, 11, 23):
            row = [
                f'"field_{index}":{big if index == width // 2 else index}' for index in range(width)
            ]
            expected = "[{" + ",".join(row) + "}]"
            for mode in ("str", "bytes"):
                assert emitted(general, width, mode) == expected
                assert emitted(unicode_twin, width, mode) == expected

    _on_a_fresh_thread(run)


# ---------------------------------------------------------------------------
# 10. The coverage canary
# ---------------------------------------------------------------------------


def test_the_general_layout_is_actually_exercised():
    """A maintenance signal, deliberately separate from the parity tests: if
    CPython stops giving these shapes a general table, the corpus above stops
    covering the compaction and would rot green. Reading the layout is what
    tells us, and the offsets are computed rather than assumed -- and
    validated against known witnesses before anything is dereferenced."""
    _require_layout()
    assert _dk_kind(general(items(8))) == _KIND_GENERAL
    assert _dk_kind(unicode_twin(items(8))) == _KIND_UNICODE
    assert _dk_kind(json.loads('{"a":1,"b":2,"c":3,"d":4,"e":5,"f":6}')) == _KIND_UNICODE

    holed = general_holed(items(8))
    assert _dk_kind(holed) == _KIND_GENERAL
    assert _dk_nentries(holed) > len(holed), "the hole has to survive the last resize"

    subclassed = {"plain": 1}
    subclassed[_Key("subclass")] = 2
    assert _dk_kind(subclassed) == _KIND_GENERAL

    def parsed_records_are_general():
        text = json.dumps([{f"field_{index}": index for index in range(9)} for _ in range(4)])
        records = strata.loads(text)
        # The builder teaches a depth its size at the first record's close, so
        # record 0 is unsized and the rest are presized -- hence general.
        assert _dk_kind(records[0]) == _KIND_UNICODE
        assert [_dk_kind(record) for record in records[1:]] == [_KIND_GENERAL] * 3

    _on_a_fresh_thread(parsed_records_are_general)


# ---------------------------------------------------------------------------
# 11. The producer side: what makes these tables general in the first place
# ---------------------------------------------------------------------------


def test_the_builder_presizes_records_above_five_members():
    """``python_builder.h`` ``new_mapping``: above five members a record is
    built with ``_PyDict_NewPresized``, which is worth 5-7% on ``loads flat``
    and is what gives the record a general keys table. Pinned so that
    "fixing" the layout by dropping the presize is a visible change, not a
    silent one."""

    def work():
        narrow = strata.loads(json.dumps([{f"f{index}": index for index in range(5)}] * 4))
        assert [_dk_kind(record) for record in narrow] == [_KIND_UNICODE] * 4
        wide = strata.loads(json.dumps([{f"f{index}": index for index in range(6)}] * 4))
        assert _dk_kind(wide[0]) == _KIND_UNICODE
        assert [_dk_kind(record) for record in wide[1:]] == [_KIND_GENERAL] * 3

    _require_layout()
    _on_a_fresh_thread(work)


def test_the_size_hint_is_per_depth_and_survives_across_calls():
    """The hints live beside the KeyCache on the leased builder and are not
    cleared by ``reset()``, so they carry from one ``loads`` call to the next
    on a thread -- per depth, which is why a wide document at depth 1 changes
    the layout of a later two-key document at depth 1 and leaves depth 2
    alone. Values are unaffected either way."""

    def work():
        strata.loads(json.dumps([{f"f{index}": index for index in range(9)} for _ in range(3)]))
        text = json.dumps([{"a": 1, "b": 2}, {"a": 3, "b": 4}])
        parsed = strata.loads(text)
        assert parsed == json.loads(text)
        if _PREFIX_PLAUSIBLE:
            assert _dk_kind(parsed[0]) == _KIND_GENERAL
        nested = strata.loads('[{"x":{"y":1}}]')
        assert nested == [{"x": {"y": 1}}]

    _on_a_fresh_thread(work)

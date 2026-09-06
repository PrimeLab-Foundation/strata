"""Contract tests for `strata.dumps` when a callback mutates what it is writing.

`dumps` runs user code at two documented points: the cycle placeholder's
warning under the default `cycle_policy="warn"` (api.md § Config), and
`PyObject_Str` on a value that is not an exact `int` — an `int` subclass's
`__str__`, which api.md's big-int clause reaches. That code can mutate the very
container being written, and every pointer the serializer holds into that
container is borrowed.

What these tests pin (python_dumps.cpp, "Re-entrancy" in the file header):

* **Nothing reads freed memory.** Before this file existed, clearing the list
  from an `int` subclass's `__str__` was a SIGSEGV and clearing a dict emitted
  values out of the freed table.
* **A list is followed live**, element by element, as stdlib json's encoder
  follows it: a callback that shrinks the list ends the array there, one that
  appends has the appended elements written, one that replaces has the
  replacements written.
* **A dict emits the row the serializer read** on entry — the same row from the
  general path and from the fused record writer, so the two stay byte-identical
  (docs/architecture/fused_record_writer.md). Dicts too wide for that row, and
  dicts with `str` subclass keys, take the plain `PyDict_Next` walk, which
  follows the dict live.
* **Output is always valid JSON**, and identical in `str` and `bytes` mode.
"""

import gc
import json
import sys
import threading
import warnings

import pytest

import strata

BIG = 2**70
BIG_TEXT = str(BIG)
MODES = ("str", "bytes")


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------


def _compact(obj):
    return json.dumps(obj, separators=(",", ":"))


def _dump(obj, mode):
    out = strata.dumps(obj, return_type=mode)
    assert isinstance(out, bytes if mode == "bytes" else str)
    return out.decode() if mode == "bytes" else out


def _on_a_fresh_thread(call):
    """Run `call` on a new thread and re-raise whatever it raised.

    The schema cache is thread-local and a depth retires after 64 misses,
    never to un-retire (E26-AUDIT §3): under the canonical run order
    `tests/py/`'s random corpus retires the shallow depths on the main thread,
    and `write_record_fused` then bails to `write_mapping` for the rest of the
    process. A fused-writer test that runs there proves nothing.
    """
    box = {}

    def run():
        try:
            box["value"] = call()
        except BaseException as error:  # noqa: BLE001 - re-raised below
            box["error"] = error

    thread = threading.Thread(target=run)
    thread.start()
    thread.join()
    if "error" in box:
        raise box["error"]
    return box["value"]


class _Once:
    """Fires an action the first time it is asked, and only then.

    The collection afterwards is load-bearing, not hygiene: freeing a list's
    item array or a dict's entry table leaves the bytes readable, so a stale
    pointer often *looks* right. Walking and reusing that memory is what turns
    the defect from a silent wrong answer into a visible one — the audit's
    reproducers collect for the same reason.
    """

    def __init__(self, action):
        self.action = action
        self.fired = 0

    def fire(self):
        if self.fired == 0:
            self.fired = 1
            self.action()
            gc.collect()


def _big_trigger(once):
    """A value whose serialization runs user code: an int subclass past int64."""

    class Big(int):
        def __str__(self):
            once.fire()
            return int.__str__(self)

    return Big(BIG)


def _self_cycle():
    """A list that contains itself, so writing it reaches the cycle policy."""
    cyclic = []
    cyclic.append(cyclic)
    return cyclic


def _pads(count, prefix="v"):
    return [{"pad": f"{prefix}{index}"} for index in range(count)]


@pytest.fixture
def cycle_policy():
    """Set `cycle_policy` and restore whatever the session had."""
    previous = strata.config.get("cycle_policy")
    yield strata.config.set
    strata.config.set("cycle_policy", previous)


# ---------------------------------------------------------------------------
# The two audit reproducers, as regression tests
# (build/evidence/E26-AUDIT/repro/iso2.py and iso3.py)
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_clearing_the_list_from_an_int_subclass_str_does_not_crash(mode):
    """E26-AUDIT §4 / iso2.py: SIGSEGV before this fix.

    `write_sequence` held `PySequence_Fast_ITEMS` across the element loop while
    `write_int` ran `__str__`; clearing the list freed that array.
    """
    doc = [None, *_pads(64)]
    once = _Once(doc.clear)
    doc[0] = _big_trigger(once)

    out = _dump(doc, mode)

    assert once.fired == 1
    # The list is empty by the time the loop looks again, so the array ends.
    assert out == f"[{BIG_TEXT}]"
    assert json.loads(out) == [BIG]


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("action", ("clear", "grow", "delrest"))
def test_mutating_the_dict_from_an_int_subclass_str_emits_the_row(mode, action):
    """E26-AUDIT §4 / iso3.py: emitted values out of a cleared dict before.

    The record now carries the row the serializer read, whatever the callback
    did to the dict afterwards.
    """
    doc = {"big": None}
    doc.update({f"k{index}": {"pad": f"v{index}"} for index in range(10)})

    def mutate():
        if action == "clear":
            doc.clear()
        elif action == "grow":
            for index in range(500):
                doc[f"extra{index}"] = {"z": index}
        else:
            for key in [key for key in doc if key != "big"]:
                del doc[key]

    once = _Once(mutate)
    doc["big"] = _big_trigger(once)
    expected = _compact({**doc, "big": BIG})

    out = _dump(doc, mode)

    assert once.fired == 1
    assert out == expected


# ---------------------------------------------------------------------------
# A list is followed live, like stdlib json
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_shrinking_list_ends_where_it_shrank(mode):
    doc = [None, {"a": 1}, {"a": 2}, {"a": 3}, {"a": 4}]

    def mutate():
        del doc[3:]

    doc[0] = _big_trigger(_Once(mutate))

    assert _dump(doc, mode) == f'[{BIG_TEXT},{{"a":1}},{{"a":2}}]'


@pytest.mark.parametrize("mode", MODES)
def test_a_growing_list_writes_what_was_appended(mode):
    """The elements a callback appends are written, as stdlib json writes them.

    Before the fix the size was snapshotted, so they were silently dropped —
    deliberately left unpinned by E26-AUDIT §4 because fixing the hazard was
    expected to change it.
    """
    doc = [None, {"a": 1}]

    def mutate():
        doc.extend({"added": index} for index in range(3))

    doc[0] = _big_trigger(_Once(mutate))

    expected = f'[{BIG_TEXT},{{"a":1}},{{"added":0}},{{"added":1}},{{"added":2}}]'
    assert _dump(doc, mode) == expected


@pytest.mark.parametrize("mode", MODES)
def test_a_replaced_element_is_written_as_replaced(mode):
    doc = [None, {"a": 1}, {"a": 2}]

    def mutate():
        for index in range(1, len(doc)):
            doc[index] = {"replaced": index}

    doc[0] = _big_trigger(_Once(mutate))

    assert _dump(doc, mode) == f'[{BIG_TEXT},{{"replaced":1}},{{"replaced":2}}]'


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("index", range(5))
def test_a_list_cleared_at_every_element_index(mode, index):
    """The differential: the trigger walks every position of the same list."""
    doc = [{"a": position} for position in range(5)]
    once = _Once(doc.clear)
    doc[index] = _big_trigger(once)

    out = _dump(doc, mode)

    head = [_compact({"a": position}) for position in range(index)]
    body = ",".join([*head, BIG_TEXT])
    assert out == f"[{body}]"
    json.loads(out)


@pytest.mark.parametrize("mode", MODES)
def test_a_tuple_is_immune(mode):
    """A tuple cannot be mutated, so its elements are read once and stay put."""
    outer = []
    once = _Once(outer.clear)
    inner = (_big_trigger(once), {"a": 1}, {"a": 2})
    outer.extend([inner, {"b": 1}])

    assert _dump(outer, mode) == f'[[{BIG_TEXT},{{"a":1}},{{"a":2}}]]'


# ---------------------------------------------------------------------------
# A dict emits the row it read — general path and fused record writer alike
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("position", range(4))
def test_a_dict_cleared_at_every_key_position(mode, position):
    """The differential over dicts: same record, trigger at each key."""
    keys = ["k0", "k1", "k2", "k3"]
    doc = {key: {"v": key} for key in keys}
    once = _Once(doc.clear)
    doc[keys[position]] = _big_trigger(once)
    expected = _compact({**doc, keys[position]: BIG})

    out = _dump(doc, mode)

    assert once.fired == 1
    assert out == expected


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("action", ("clear", "grow", "replace"))
def test_the_fused_record_writer_survives_a_mutated_record(mode, action):
    """The array-of-records path, on a thread whose schema cache is not retired.

    `grow` is the shape that crashed a fresh process: 500 insertions resize the
    dict, and `write_record_fused` was reading its entry table across the
    callback.
    """

    def build():
        record = {"a": 8, "b": None, "c": "x"}

        def mutate():
            if action == "clear":
                record.clear()
            elif action == "grow":
                for index in range(500):
                    record[f"extra{index}"] = index
            else:
                for key in list(record):
                    record[key] = "replaced"

        once = _Once(mutate)
        record["b"] = _big_trigger(once)
        doc = [{"a": index, "b": {"n": index}, "c": "x"} for index in range(8)]
        doc.append(record)
        out = _dump(doc, mode)
        assert once.fired == 1
        return out

    out = _on_a_fresh_thread(build)

    expected = [{"a": index, "b": {"n": index}, "c": "x"} for index in range(8)]
    expected.append({"a": 8, "b": BIG, "c": "x"})
    assert out == _compact(expected)


@pytest.mark.parametrize("mode", MODES)
def test_the_fused_writer_and_the_general_path_agree_under_mutation(mode):
    """Two identical documents, one emitted fused and one not, byte for byte.

    A retired thread routes every record through `write_mapping`; a fresh one
    routes the tail of the list through `write_record_fused`. The row both
    emit is the row they read, so the bytes match.
    """

    def build():
        record = {"a": 8, "b": None, "c": "x"}

        def mutate():
            for index in range(200):
                record[f"extra{index}"] = index

        record["b"] = [_big_trigger(_Once(mutate))]
        doc = [{"a": index, "b": [index], "c": "x"} for index in range(8)]
        doc.append(record)
        return _dump(doc, mode)

    def retire_then_build():
        # 200 one-off shapes at depth 1 push `select` past its 64-miss
        # retirement, so the same document is emitted by write_mapping.
        for index in range(200):
            strata.dumps([{f"one_off_{index}": index}])
        return build()

    assert _on_a_fresh_thread(build) == _on_a_fresh_thread(retire_then_build)


@pytest.mark.parametrize("mode", MODES)
def test_a_wide_dict_follows_the_dict(mode):
    """Past `kMaxSchemaKeys` the plain `PyDict_Next` walk runs, which
    re-validates against the dict on every step instead of staging a row."""
    doc = {f"k{index:02d}": index for index in range(30)}
    once = _Once(doc.clear)
    doc["k00"] = _big_trigger(once)

    out = _dump(doc, mode)

    assert once.fired == 1
    assert out == f'{{"k00":{BIG_TEXT}}}'
    json.loads(out)


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_dict_mutated_through_its_parent(mode):
    """The callback empties the record two levels up, not the one being walked."""
    inner = {"deep": None}
    doc = {"outer": {"inner": inner}, "tail": {"t": 1}}
    inner["deep"] = _big_trigger(_Once(doc.clear))

    out = _dump(doc, mode)

    assert out == _compact({"outer": {"inner": {"deep": BIG}}, "tail": {"t": 1}})


# ---------------------------------------------------------------------------
# The cycle warning as the trigger, under every policy
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("action", ("clear", "deltail", "grow"))
def test_a_cycle_warning_handler_may_mutate_the_list(mode, action, cycle_policy):
    """E26-AUDIT §4 / iso.py: SIGSEGV on all three actions before this fix."""
    cycle_policy("cycle_policy", "warn")
    doc = [1, 2, _self_cycle(), *_pads(64, "p")]

    def mutate():
        if action == "clear":
            del doc[3:]
        elif action == "deltail":
            del doc[4:]
        else:
            doc.extend(_pads(200))

    once = _Once(mutate)
    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = lambda *args, **kwargs: once.fire()
        out = _dump(doc, mode)

    assert once.fired == 1
    parsed = json.loads(out)
    # The cyclic element is a list holding itself, so it writes as `[null]`.
    assert parsed[:3] == [1, 2, [None]]
    if action == "clear":
        assert parsed == [1, 2, [None]]
    elif action == "deltail":
        assert parsed == [1, 2, [None], {"pad": "p0"}]
    else:
        assert len(parsed) == 3 + 64 + 200


@pytest.mark.parametrize("mode", MODES)
def test_a_cycle_warning_handler_may_mutate_the_dict(mode, cycle_policy):
    cycle_policy("cycle_policy", "warn")
    doc = {"a": _self_cycle(), "b": {"x": 1}, "c": "text"}
    once = _Once(doc.clear)

    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = lambda *args, **kwargs: once.fire()
        out = _dump(doc, mode)

    assert once.fired == 1
    assert out == _compact({"a": [None], "b": {"x": 1}, "c": "text"})


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("policy", ("warn", "error", "ignore"))
def test_every_cycle_policy_with_a_mutating_handler(mode, policy, cycle_policy):
    """`error` refuses before any handler runs; `ignore` runs none; `warn` runs
    one and the walk still finishes on a live container."""
    cycle_policy("cycle_policy", policy)
    doc = [_self_cycle(), {"a": 1}, {"a": 2}]
    once = _Once(doc.clear)

    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = lambda *args, **kwargs: once.fire()
        if policy == "error":
            with pytest.raises(ValueError, match="Circular reference detected"):
                _dump(doc, mode)
            assert once.fired == 0
            return
        out = _dump(doc, mode)

    if policy == "warn":
        assert once.fired == 1
        assert out == "[[null]]"
    else:
        assert once.fired == 0
        assert out == '[[null],{"a":1},{"a":2}]'


@pytest.mark.parametrize("mode", MODES)
def test_a_warning_filter_that_raises_stops_the_dump(mode, cycle_policy):
    """Reviewer checklist #8: a failing `PyErr_WarnEx` stops serialization —
    still true with the container owned and the row locked."""
    cycle_policy("cycle_policy", "warn")
    doc = [_self_cycle(), {"a": 1}]

    with warnings.catch_warnings():
        warnings.simplefilter("error")
        with pytest.raises(RuntimeWarning, match="Circular reference detected"):
            _dump(doc, mode)


# ---------------------------------------------------------------------------
# Ownership: the callback may drop the last reference to what is being walked
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_dropping_the_last_reference_to_the_open_list(mode):
    """The list being walked is reachable only from its parent; the callback
    empties the parent. The walk owns it, so it finishes."""
    parent = [[None, {"a": 1}, {"a": 2}], {"b": 1}]
    once = _Once(parent.clear)
    parent[0][0] = _big_trigger(once)

    out = _dump(parent, mode)

    assert once.fired == 1
    assert out == f'[[{BIG_TEXT},{{"a":1}},{{"a":2}}]]'


@pytest.mark.parametrize("mode", MODES)
def test_dropping_the_last_reference_to_the_open_dict(mode):
    parent = [{"t": None, "u": {"a": 1}}, {"b": 1}]
    once = _Once(parent.clear)
    parent[0]["t"] = _big_trigger(once)

    out = _dump(parent, mode)

    assert once.fired == 1
    assert out == _compact([{"t": BIG, "u": {"a": 1}}])


@pytest.mark.parametrize("mode", MODES)
def test_a_sibling_container_is_untouched(mode):
    """Mutating a container the walk is not inside changes what is written
    from that container on, and nothing else."""
    sibling = [{"s": 1}, {"s": 2}]
    doc = [[_big_trigger(_Once(sibling.clear))], sibling, {"tail": 1}]

    assert _dump(doc, mode) == f'[[{BIG_TEXT}],[],{{"tail":1}}]'


# ---------------------------------------------------------------------------
# Paths that cannot run user code at all
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_scalar_run_is_never_interrupted(mode):
    """The scalar runs handle exact types only, so nothing in them calls back:
    the run writes its whole span and the general loop takes over at the
    subclass that ends it."""
    doc = [*range(40), None, {"a": 1}]
    once = _Once(doc.clear)
    doc[40] = _big_trigger(once)

    digits = ",".join(str(index) for index in range(40))
    assert _dump(doc, mode) == f"[{digits},{BIG_TEXT}]"


@pytest.mark.parametrize("mode", MODES)
def test_an_exact_big_int_runs_no_user_code(mode):
    """`PyObject_Str` on an exact int cannot reach Python, so a list holding
    one is never re-entered — the bytes are the plain ones."""
    doc = [BIG, -BIG, 10**100, {"a": 1}]

    assert _dump(doc, mode) == _compact(doc)


@pytest.mark.parametrize("mode", MODES)
def test_a_dict_subclass_value_is_never_stringified(mode):
    """A dict subclass is serialized as an object, never through `__str__` or
    `__repr__`: that route to user code does not exist."""
    calls = []

    class Mapping(dict):
        def __str__(self):  # pragma: no cover - must never be called
            calls.append("str")
            return "{}"

        def __repr__(self):  # pragma: no cover - must never be called
            calls.append("repr")
            return "{}"

    doc = [Mapping({"a": 1}), Mapping({"a": 2})]

    assert _dump(doc, mode) == '[{"a":1},{"a":2}]'
    assert calls == []


@pytest.mark.parametrize("mode", MODES)
def test_a_str_subclass_value_runs_no_user_code(mode):
    """`str` subclasses are written through the same escaper, not `__str__`."""
    calls = []

    class Text(str):
        def __str__(self):  # pragma: no cover - must never be called
            calls.append("str")
            return "x"

    doc = [Text('a"b'), {"k": Text("ü")}]

    assert _dump(doc, mode) == '["a\\"b",{"k":"ü"}]'
    assert calls == []


# ---------------------------------------------------------------------------
# The schema cache owns its keys, so it only remembers exact `str`
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_str_subclass_key_serializes_like_its_text(mode):
    class Key(str):
        pass

    doc = [{Key("a"): 1, Key("b"): [2]} for _ in range(5)]

    assert _dump(doc, mode) == _compact([{"a": 1, "b": [2]}] * 5)


@pytest.mark.parametrize("mode", MODES)
def test_a_str_subclass_key_with_a_finalizer_cannot_reach_a_staged_row(mode):
    """A cached key is an owned reference, and releasing a `str` subclass runs
    `__del__`. Keeping such keys out of the cache is what stops that from
    firing while another record's row is staged; this walks the shape that
    would trigger it — many one-off subclass-keyed records, so the cache would
    have been evicting one on nearly every record."""
    finalized = []
    victims = []

    class Key(str):
        def __del__(self):
            finalized.append(1)
            for victim in victims:
                victim.clear()

    docs = [{Key(f"k{index}"): {"v": index}} for index in range(64)]
    victims.append(docs[-1])

    out = _dump(docs, mode)

    assert out == _compact([{f"k{index}": {"v": index}} for index in range(64)])
    victims.clear()
    docs.clear()


# ---------------------------------------------------------------------------
# The rest of the contract still holds around the guards
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_dumps_from_a_mutating_callback(mode):
    """The nested call has its own cycle stack, its own buffer and its own
    schema cache; the outer call's row survives it."""
    inner = []
    doc = {"t": None, "u": {"a": 1}}

    def mutate():
        doc.clear()
        inner.append(strata.dumps({"n": [1, 2]}))

    doc["t"] = _big_trigger(_Once(mutate))

    out = _dump(doc, mode)

    assert inner == ['{"n":[1,2]}']
    assert out == _compact({"t": BIG, "u": {"a": 1}})


@pytest.mark.parametrize("mode", MODES)
def test_mutation_at_the_depth_boundary(mode):
    """The framed sequence loop (`write_sequence_body`) re-reads too."""
    previous = sys.getrecursionlimit()
    try:
        sys.setrecursionlimit(60)
        deepest = [None, {"a": 1}]
        once = _Once(deepest.clear)
        deepest[0] = _big_trigger(once)
        doc = deepest
        for _ in range(55):
            doc = [doc]
        out = _dump(doc, mode)
    finally:
        sys.setrecursionlimit(previous)

    assert once.fired == 1
    assert out == "[" * 55 + f"[{BIG_TEXT}]" + "]" * 55


@pytest.mark.parametrize("mode", MODES)
def test_a_cycle_found_after_a_mutation_still_reports(mode, cycle_policy):
    """Ownership does not hide a cycle: the container the walk holds is still
    the one `open_` is scanned for."""
    cycle_policy("cycle_policy", "error")
    doc = [None, {"a": 1}]

    def mutate():
        doc.append(doc)

    doc[0] = _big_trigger(_Once(mutate))

    with pytest.raises(ValueError, match="Circular reference detected"):
        _dump(doc, mode)


@pytest.mark.parametrize("mode", MODES)
def test_the_walk_leaves_nothing_open_after_a_mutating_dump(mode, cycle_policy):
    """Reviewer checklist #3: after the call the objects that were open
    serialize normally, so no frame outlived the walk."""
    cycle_policy("cycle_policy", "error")
    inner = {"a": 1}
    doc = [None, inner]
    doc[0] = _big_trigger(_Once(doc.clear))

    _dump(doc, mode)

    assert _dump(doc, mode) == "[]"
    assert _dump(inner, mode) == '{"a":1}'
    assert _dump([inner, inner], mode) == '[{"a":1},{"a":1}]'


@pytest.mark.parametrize("mode", MODES)
def test_concurrent_mutating_dumps_on_several_threads(mode):
    """Per-thread stage, size hint and schema cache: the guards are per call."""
    results = {}

    def worker(name):
        doc = [None, *[{"pad": name} for _ in range(8)]]
        doc[0] = _big_trigger(_Once(doc.clear))
        results[name] = _dump(doc, mode)

    threads = [threading.Thread(target=worker, args=(f"t{index}",)) for index in range(4)]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()

    assert results == {f"t{index}": f"[{BIG_TEXT}]" for index in range(4)}

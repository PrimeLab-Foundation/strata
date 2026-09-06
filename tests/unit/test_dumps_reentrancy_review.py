"""Re-entrancy cases the independent review added (E26-FIX1-REVIEW).

These are the cases `tests/unit/test_dumps_reentrancy.py` does not cover.  Each
one fails, crashes, or reads freed memory on the unfixed serializer; the
eviction case below aborts it under AddressSanitizer.

Vehicles.  The serializer runs user code at four steps (api.md, "Mutation
during serialization"): a cycle placeholder's ``PyErr_WarnEx`` under
``cycle_policy="warn"``; ``write_int``'s ``PyObject_Str`` on an ``int``
subclass past int64; the same call on an exact ``int`` whose decimal
conversion CPython 3.12+ delegates to ``_pylong``; and its own releases of
what that code orphaned.  The subclass vehicle needs a value past int64 -- the
compact-int fast path is taken before exactness is consulted, so a small
subclass value never reaches ``__str__``.
"""

import gc
import json
import sys
import weakref

import pytest

import strata

BIG = 1 << 70  # past int64, so write_int falls to PyObject_Str
MODES = ("str", "bytes")


def _text(document, mode):
    out = strata.dumps(document, return_type=mode)
    return out.decode() if mode == "bytes" else out


class _Once:
    """Runs `action` on the first call, then collects.

    The collection is what makes a freed read visible: a freed item array still
    reads correctly until something reuses it.
    """

    def __init__(self, action):
        self._action = action
        self.fired = False

    def fire(self):
        if self.fired:
            return
        self.fired = True
        self._action()
        gc.collect()


def _vehicle(action):
    """An int subclass past int64 whose __str__ fires `action` once."""
    once = _Once(action)

    class Trigger(int):
        def __str__(self):
            once.fire()
            return int.__str__(self)

    return Trigger(BIG), once


# --- the schema cache's evicted keys -------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_an_evicted_schema_key_cannot_reach_a_staged_row(mode):
    """`Schema::remember` releases the keys of the way it evicts.  A `str`
    subclass key's release runs `__del__` -- inside `write_mapping_body`, after
    the current record's keys and values are staged and before either is
    emitted.  Keeping subclass keys out of the cache is what closes that; on
    the unfixed serializer this is a heap-use-after-free in `remember`."""
    victim = {}

    def refill():
        victim.clear()
        for index in range(5):
            victim["".join(["vk", str(index), "x" * 40])] = "".join(
                ["vv", str(index), "y" * 40],
            )

    def shape(tag, width):
        class Key(str):
            def __del__(self):
                victim.clear()
                gc.collect()

        keys = [Key("".join(["f", str(tag)]))]
        keys += [f"k{index}" for index in range(width - 1)]
        return {key: index for index, key in enumerate(keys)}

    # fill every way at this depth with subclass-keyed shapes, then drop every
    # reference but the cache's
    for tag in range(4):
        strata.dumps([shape(tag, 2 + tag)], return_type=mode)
    gc.collect()

    refill()
    expected = json.dumps(victim, separators=(",", ":"))
    # a fifth shape at the same depth: select() misses and evicts a way
    assert _text([victim], mode) == f"[{expected}]"
    assert len(victim) == 5


@pytest.mark.parametrize("mode", MODES)
def test_a_str_subclass_key_makes_the_dict_follow_the_dict(mode):
    """Subclass keys take the plain `PyDict_Next` walk, which follows the live
    dict -- so a callback that empties it ends the object there.  Exact-str
    narrow dicts emit the row as read instead; both are safe, and this pins
    which rule applies."""

    class Key(str):
        pass

    document = {Key("a"): 1, Key("b"): None, Key("c"): 3}
    trigger, once = _vehicle(document.clear)
    document[Key("b")] = trigger

    assert _text(document, mode) == f'{{"a":1,"b":{BIG}}}'
    assert once.fired


# --- routes to user code that must not exist -----------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_list_subclass_is_walked_by_layout_not_by_protocol(mode):
    """`__len__` and `__iter__` are never consulted, so a subclass cannot lie
    the walk past the end of its own storage."""

    class Sequence(list):
        def __len__(self):
            raise AssertionError("__len__ must never be reached")

        def __iter__(self):
            raise AssertionError("__iter__ must never be reached")

    document = Sequence([{"a": 1}, None, {"b": 2}])
    trigger, once = _vehicle(document.clear)
    document[1] = trigger

    assert _text(document, mode) == f'[{{"a":1}},{BIG}]'
    assert once.fired


@pytest.mark.parametrize("mode", MODES)
def test_a_dict_subclass_is_walked_by_layout_not_by_protocol(mode):
    class Mapping(dict):
        def keys(self):
            raise AssertionError("keys() must never be reached")

        def items(self):
            raise AssertionError("items() must never be reached")

        def __str__(self):
            raise AssertionError("tp_str must never be reached")

    inner = Mapping({"a": 1, "b": None})
    trigger, once = _vehicle(inner.clear)
    inner["b"] = trigger

    assert _text({"outer": inner}, mode) == f'{{"outer":{{"a":1,"b":{BIG}}}}}'
    assert once.fired


# --- ownership: what the callback is allowed to drop ---------------------


@pytest.mark.parametrize("mode", MODES)
def test_clearing_every_ancestor_at_once(mode):
    """Each container owns itself for the window user code can run beneath it,
    so emptying the whole chain from the leaf leaves every open container
    alive; the walk then unwinds through containers that are now empty."""
    leaf = {"c": None}
    middle = [leaf, {"pad": 1}]
    top = {"a": middle, "b": [4, 5, 6]}
    root = [top, {"tail": 1}]

    def empty_everything():
        middle.clear()
        top.clear()
        root.clear()
        leaf.clear()

    trigger, once = _vehicle(empty_everything)
    leaf["c"] = trigger

    assert _text(root, mode) == f'[{{"a":[{{"c":{BIG}}}],"b":[4,5,6]}}]'
    assert once.fired


@pytest.mark.parametrize("mode", MODES)
def test_a_weakref_callback_fired_under_the_walk(mode):
    """A weakref callback is user code reached without a `__del__`: dropping
    the referent inside the vehicle runs it while the walk is open."""
    sibling = [{"v": index} for index in range(4)]
    document = [sibling, {"a": None}, {"tail": 1}]

    class Cell:
        pass

    holder = {"cell": Cell()}
    reference = weakref.ref(holder["cell"], lambda _ref: sibling.clear())

    def drop():
        holder.clear()
        gc.collect()
        assert reference() is None

    trigger, once = _vehicle(drop)
    document[1]["a"] = trigger

    # the sibling was already emitted and the outer list is untouched, so the
    # only requirement is that the freed sibling is not read again
    expected = f'[[{{"v":0}},{{"v":1}},{{"v":2}},{{"v":3}}],{{"a":{BIG}}},{{"tail":1}}]'
    assert _text(document, mode) == expected
    assert once.fired
    assert sibling == []


# --- the contract against stdlib json ------------------------------------


@pytest.mark.parametrize("action", ["clear", "grow", "shrink", "replace"])
@pytest.mark.parametrize("position", [0, 1, 3, 5])
def test_a_mutated_list_matches_stdlib_json(action, position):
    """Lists are followed live, element by element, exactly as stdlib json's
    encoder does.  stdlib renders ints through `int.__repr__` and so never
    calls an int subclass's `__str__`; it gets a `default=` hook instead, which
    is user code it does run at the same point in the same loop."""
    width = 6
    marker = str(BIG)

    def mutate(target):
        if action == "clear":
            target.clear()
        elif action == "grow":
            target.extend(["g0", "g1"])
        elif action == "shrink":
            del target[position + 1 :]
        elif action == "replace":
            for index in range(len(target)):
                if index != position:
                    target[index] = "R"

    strata_document = [{"a": index} for index in range(width)]
    trigger, _once = _vehicle(lambda: mutate(strata_document))
    strata_document[position] = trigger

    class Unserializable:
        pass

    json_document = [{"a": index} for index in range(width)]
    json_document[position] = Unserializable()
    fired = []

    def default(_object):
        if not fired:
            fired.append(True)
            mutate(json_document)
        return marker

    expected = json.dumps(json_document, separators=(",", ":"), default=default)
    assert strata.dumps(strata_document) == expected.replace(f'"{marker}"', marker)


def test_a_mutated_narrow_dict_emits_the_row_as_read():
    """The documented divergence: a dict of at most 24 exact `str` keys emits
    the row the serializer read on entry, because the general and the fused
    emitter must produce identical bytes and only the snapshot is producible by
    both.  stdlib json follows the dict live and stops."""
    document = {f"k{index}": {"a": index} for index in range(6)}
    trigger, once = _vehicle(document.clear)
    document["k3"] = trigger

    assert strata.dumps(document) == (
        f'{{"k0":{{"a":0}},"k1":{{"a":1}},"k2":{{"a":2}},"k3":{BIG},"k4":{{"a":4}},"k5":{{"a":5}}}}'
    )
    assert once.fired


def test_a_mutated_wide_dict_follows_the_dict():
    """Past `kMaxSchemaKeys` the walk is `PyDict_Next`, which re-validates
    against the dict on every call -- so a wide dict follows the live dict,
    like stdlib json and unlike its narrow sibling above."""
    document = {f"k{index:02d}": index for index in range(40)}
    trigger, once = _vehicle(document.clear)
    document["k10"] = trigger

    prefix = ",".join(f'"k{index:02d}":{index}' for index in range(10))
    assert strata.dumps(document) == f'{{{prefix},"k10":{BIG}}}'
    assert once.fired


# --- reference balance on every failing exit -----------------------------


def test_no_reference_drift_on_any_failing_exit():
    """The deferred push, the row lock and the frame all release on the error
    returns, not only on the success return."""

    class Unserializable:
        pass

    def unserializable():
        inner = [{"a": 1}, {"b": 2}]
        return {"x": inner, "y": [Unserializable()]}, TypeError

    def bad_key():
        inner = [{"a": 1}]
        return {"x": inner, "y": {1: 2}}, TypeError

    def too_deep():
        document = [{"a": 1}]
        for _ in range(sys.getrecursionlimit() + 5):
            document = [document]
        return document, ValueError

    def cyclic():
        document = {"x": [{"a": 1}]}
        document["self"] = document
        return document, ValueError

    previous = strata.config.get("cycle_policy")
    strata.config.set("cycle_policy", "error")
    try:
        for make in (unserializable, bad_key, too_deep, cyclic):
            document, expected = make()
            watched = list(document.values()) if isinstance(document, dict) else [document]
            gc.collect()
            before = [sys.getrefcount(item) for item in watched]
            for mode in MODES:
                with pytest.raises(expected):
                    strata.dumps(document, return_type=mode)
            gc.collect()
            after = [sys.getrefcount(item) for item in watched]
            assert after == before, make.__name__
    finally:
        strata.config.set("cycle_policy", previous)

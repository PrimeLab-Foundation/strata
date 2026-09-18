"""Contract mirrors for the `dumps`/`dump` unsupported-type hook (`default=`).

One named test per clause of docs/context/api.md's `default` rows and of the
error table in docs/architecture/dumps_default_hook.md; each cites the clause
it pins. The design record is the contract, api.md is the published form of
it, and nothing here asserts an implementation detail the two do not state.
"""

from __future__ import annotations

import gc
import json
import sys
import warnings
from pathlib import Path

import pytest

import strata


class Unsupported:
    """A type `dumps` has no writer for -- the object every test starts from."""

    def __init__(self, payload=None):
        self.payload = payload


class Marker:
    """Distinguishable from `Unsupported`, for the `default() returned` row."""


# --- signature -------------------------------------------------------------


def test_dumps_default_is_keyword_only():
    """api.md: `dumps(obj, *, return_type="str", default=None)` -- keyword-only."""
    with pytest.raises(TypeError):
        strata.dumps([], "str")  # positional return_type was never allowed either
    with pytest.raises(TypeError):
        strata.dumps(Unsupported(), lambda _o: 1)


def test_dump_default_is_keyword_only(tmp_path: Path):
    """api.md: `dump(obj, path, *, split_by=None, default=None)`."""
    with pytest.raises(TypeError):
        strata.dump([], str(tmp_path / "a.json"), None, lambda _o: 1)


def test_default_none_is_the_documented_absence():
    """api.md: `default` is `None` or a callable of one argument."""
    assert strata.dumps({"a": 1}, default=None) == '{"a":1}'


# --- error table row 1: not None and not callable --------------------------


@pytest.mark.parametrize("value", [1, "f", [], {}, object(), 0.5, True])
def test_default_not_callable_is_typeerror(value):
    """Record error table row 1: `TypeError("default must be callable, not %s")`."""
    with pytest.raises(TypeError) as excinfo:
        strata.dumps({"a": 1}, default=value)
    assert str(excinfo.value) == f"default must be callable, not {type(value).__name__}"


def test_default_not_callable_is_raised_before_any_byte(tmp_path: Path):
    """Record error table row 1: raised at the fastcall boundary, nothing written."""
    target = tmp_path / "untouched.json"
    with pytest.raises(TypeError) as excinfo:
        strata.dump([{"a": 1}], str(target), default=7)
    assert str(excinfo.value) == "default must be callable, not int"
    assert not target.exists()


def test_default_not_callable_checked_even_for_a_serializable_tree():
    """Row 1 is a boundary check: it does not wait for an unsupported object."""
    with pytest.raises(TypeError, match="^default must be callable, not str$"):
        strata.dumps([1, 2, 3], default="not callable")


# --- error table row 2: unsupported type, default is None ------------------


def test_unsupported_without_default_keeps_its_message():
    """Record error table row 2: the message today's callers test against."""
    with pytest.raises(TypeError) as excinfo:
        strata.dumps(Unsupported())
    assert str(excinfo.value) == "Object of type Unsupported is not JSON serializable"


def test_unsupported_with_default_none_keeps_its_message():
    """Row 2: an explicit `default=None` is the same as no hook at all."""
    with pytest.raises(TypeError) as excinfo:
        strata.dumps({"k": Unsupported()}, default=None)
    assert str(excinfo.value) == "Object of type Unsupported is not JSON serializable"


# --- the hook's value is serialized in place -------------------------------


def test_hook_replaces_the_object():
    """api.md: the return value is serialized in the unsupported object's place."""
    assert strata.dumps(Unsupported(), default=lambda _o: "x") == '"x"'


def test_hook_receives_the_object_itself():
    """api.md: a callable of one argument -- the unsupported object."""
    seen = []
    obj = Unsupported()
    strata.dumps({"k": obj}, default=lambda o: seen.append(o) or 0)
    assert len(seen) == 1
    assert seen[0] is obj


def test_hook_applies_at_every_position():
    """api.md: called wherever `dumps` would otherwise raise."""
    obj = Unsupported()
    tree = {"a": obj, "b": [obj, {"c": obj}], "d": (obj,)}
    assert json.loads(strata.dumps(tree, default=lambda _o: "R")) == {
        "a": "R",
        "b": ["R", {"c": "R"}],
        "d": ["R"],
    }


def test_hook_called_once_per_occurrence():
    """api.md: one call per object reached, not per distinct object."""
    calls = []
    obj = Unsupported()
    strata.dumps([obj, obj, obj], default=lambda o: calls.append(o) or 1)
    assert len(calls) == 3


def test_hook_may_return_a_container():
    """Record, Depth: the returned value re-enters the walk's ordinary path."""
    out = strata.dumps(Unsupported(), default=lambda _o: {"k": [1, 2, {"n": None}]})
    assert json.loads(out) == {"k": [1, 2, {"n": None}]}


def test_hook_return_is_used_in_bytes_mode():
    """api.md: `return_type` is orthogonal to `default`."""
    assert strata.dumps(Unsupported(), default=lambda _o: 5, return_type="bytes") == b"5"


# --- error table row 3: the callable raises --------------------------------


def test_hook_exception_propagates_unchanged():
    """Record error table row 3: same type, same args, no wrapping."""
    sentinel = ValueError("boom", 42)

    def hook(_object):
        raise sentinel

    with pytest.raises(ValueError) as excinfo:
        strata.dumps({"k": Unsupported()}, default=hook)
    assert excinfo.value is sentinel
    assert excinfo.value.args == ("boom", 42)


def test_hook_exception_context_is_not_fabricated():
    """Row 3: no chaining, no `__context__` fabrication."""

    def hook(_object):
        raise RuntimeError("inner")

    with pytest.raises(RuntimeError) as excinfo:
        strata.dumps([Unsupported()], default=hook)
    assert excinfo.value.__context__ is None
    assert excinfo.value.__cause__ is None
    assert excinfo.value.__suppress_context__ is False


@pytest.mark.parametrize("exc", [KeyboardInterrupt, MemoryError, SystemExit])
def test_hook_may_raise_base_exceptions(exc):
    """Row 3: `KeyboardInterrupt`, `MemoryError`, `SystemExit` included."""

    def hook(_object):
        raise exc()

    with pytest.raises(exc):
        strata.dumps({"k": Unsupported()}, default=hook)


def test_hook_exception_writes_nothing_to_a_dump(tmp_path: Path):
    """Row 3: nothing is written to the destination of a `dump`."""
    target = tmp_path / "out.json"
    target.write_text("PREVIOUS", encoding="utf-8")

    def hook(_object):
        raise ValueError("no")

    with pytest.raises(ValueError):
        strata.dump([{"k": Unsupported()}], str(target), default=hook)
    assert target.read_text(encoding="utf-8") == "PREVIOUS"


# --- error table row 4: the callable returns an unsupported type -----------


def test_hook_returning_unsupported_raises_the_distinct_message():
    """Record error table row 4 / api.md chain bound: distinct message."""
    with pytest.raises(TypeError) as excinfo:
        strata.dumps(Unsupported(), default=lambda _o: Marker())
    assert str(excinfo.value) == (
        "default() returned an object of type Marker that is not JSON serializable"
    )


def test_hook_returning_the_same_object_does_not_recurse():
    """Chain bound 1: `default=lambda o: o` raises instead of looping.

    stdlib `json` re-enters `default` on the returned object until its cycle
    marker stops it and orjson caps the chain at 254; strata raises on the
    first unsupported return (api.md, the documented divergence).
    """
    with pytest.raises(TypeError) as excinfo:
        strata.dumps(Unsupported(), default=lambda o: o)
    assert str(excinfo.value) == (
        "default() returned an object of type Unsupported that is not JSON serializable"
    )


def test_hook_not_invoked_twice_on_an_unsupported_return():
    """Row 4: the callable is **not** invoked a second time."""
    calls = []

    def hook(o):
        calls.append(o)
        return Marker()

    with pytest.raises(TypeError):
        strata.dumps(Unsupported(), default=hook)
    assert len(calls) == 1


def test_chain_bound_is_one_call_per_object_not_per_walk():
    """Chain bound 1 bounds the chain on one object, not the walk.

    An unsupported object *nested inside* what the hook returned is a
    different object at an ordinary walk position (record, Depth: "the
    returned value re-enters `write_value`, so a returned container takes the
    ordinary `write_sequence`/`write_mapping` path"), so it gets its own
    single call. What the bound forbids is re-offering the returned object
    itself, which the test above pins.
    """
    calls = []

    def hook(o):
        calls.append(o)
        if len(calls) == 1:
            return {"inner": Unsupported()}
        return "leaf"

    assert json.loads(strata.dumps(Unsupported(), default=hook)) == {"inner": "leaf"}
    assert len(calls) == 2


# --- error table row 5: the callable returns None --------------------------


def test_hook_returning_none_is_json_null():
    """Record error table row 5: `None` is a supported value, not a sentinel."""
    assert strata.dumps(Unsupported(), default=lambda _o: None) == "null"
    assert strata.dumps({"k": Unsupported()}, default=lambda _o: None) == '{"k":null}'


# --- error table row 6: a lone surrogate from the hook ---------------------


def test_hook_returning_a_lone_surrogate_raises_unicodeencodeerror():
    """Record error table row 6: unchanged `UnicodeEncodeError`."""
    with pytest.raises(UnicodeEncodeError):
        strata.dumps(Unsupported(), default=lambda _o: "\ud800")


def test_hook_returning_a_lone_surrogate_key_raises_unicodeencodeerror():
    """Row 6: the rule is the str's, wherever the hook put it."""
    with pytest.raises(UnicodeEncodeError):
        strata.dumps(Unsupported(), default=lambda _o: {"\ud800": 1})


# --- error table row 7: keys are excluded ----------------------------------


def test_default_never_applies_to_a_non_str_key():
    """Record error table row 7: `default` never applies to keys."""
    calls = []
    with pytest.raises(TypeError) as excinfo:
        strata.dumps({1: "v"}, default=lambda o: calls.append(o) or "k")
    assert str(excinfo.value) == "keys must be str, not int"
    assert calls == []


def test_default_never_applies_to_an_unsupported_key_object():
    """Row 7: an unsupported *key* is still the key message, hook untouched."""
    calls = []
    with pytest.raises(TypeError) as excinfo:
        strata.dumps({Unsupported(): 1}, default=lambda o: calls.append(o) or "k")
    assert str(excinfo.value) == "keys must be str, not Unsupported"
    assert calls == []


def test_default_never_applies_to_keys_of_a_wide_record():
    """Row 7 holds past the fused record writer's 24-key bound."""
    record = {f"k{i}": i for i in range(30)}
    record[7] = "not a str key"
    calls = []
    with pytest.raises(TypeError) as excinfo:
        strata.dumps(record, default=lambda o: calls.append(o) or "k")
    assert str(excinfo.value) == "keys must be str, not int"
    assert calls == []


def test_default_never_applies_to_keys_of_a_returned_record():
    """Row 7 holds inside what the hook itself returned."""
    calls = []

    def hook(o):
        calls.append(o)
        return {2: "v"}

    with pytest.raises(TypeError) as excinfo:
        strata.dumps(Unsupported(), default=hook)
    assert str(excinfo.value) == "keys must be str, not int"
    assert len(calls) == 1


# --- error table row 8: split values are excluded --------------------------


def test_default_never_applies_to_a_split_value(tmp_path: Path):
    """Record error table row 8: grouping happens before serialization."""
    calls = []
    with pytest.raises((ValueError, TypeError)):
        strata.dump(
            [{"g": Unsupported(), "v": 1}],
            str(tmp_path / "folder"),
            split_by="g",
            default=lambda o: calls.append(o) or "grouped",
        )
    assert calls == []


def test_default_applies_to_folder_dump_values(tmp_path: Path):
    """Row 8's other half: values inside the records still reach the hook."""
    folder = tmp_path / "folder"
    strata.dump(
        [{"g": "a", "v": Unsupported()}],
        str(folder),
        split_by="g",
        default=lambda _o: "hooked",
    )
    assert json.loads((folder / "a.json").read_text(encoding="utf-8")) == [
        {"g": "a", "v": "hooked"}
    ]


# --- depth, cycles and the caps --------------------------------------------


def _nest(levels: int, leaf):
    """`levels` nested lists with `leaf` at the bottom, built iteratively."""
    root = []
    node = root
    for _ in range(levels - 1):
        child = []
        node.append(child)
        node = child
    node.append(leaf)
    return root


def test_hook_at_the_boundary_opens_no_frame():
    """Record, Depth: the hook is invoked at the current `open_count_`.

    At the last admissible depth a scalar from the hook lands exactly as a
    directly-written scalar does -- the hook itself is inside no new frame.
    """
    levels = sys.getrecursionlimit()
    deep = _nest(levels, Unsupported())
    assert strata.dumps(deep, default=lambda _o: 1).endswith("[1]" + "]" * (levels - 1))


def test_hook_result_container_takes_the_ordinary_depth_check():
    """Depth: a returned container's own push raises exactly where a direct
    child of the same parent would."""
    levels = sys.getrecursionlimit()
    deep = _nest(levels, Unsupported())
    with pytest.raises(ValueError, match="Maximum serialization depth exceeded"):
        strata.dumps(deep, default=lambda _o: [1])
    # One level shallower the same returned list fits, which is what makes the
    # boundary the cap's and not the hook's.
    shallower = _nest(levels - 1, Unsupported())
    assert strata.dumps(shallower, default=lambda _o: [1]).endswith("[[1]]" + "]" * (levels - 2))


def _cycle_policy(value):
    previous = strata.config.get("cycle_policy")
    strata.config.set("cycle_policy", value)
    return previous


def test_hook_returning_an_open_container_warns_under_warn_policy():
    """Record, Cycles: identical to that container appearing directly."""
    previous = _cycle_policy("warn")
    try:
        outer = []
        outer.append(Unsupported())
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            out = strata.dumps(outer, default=lambda _o: outer)
        assert "null" in out
        assert any(issubclass(w.category, RuntimeWarning) for w in caught)
    finally:
        _cycle_policy(previous)


def test_hook_returning_an_open_container_raises_under_error_policy():
    """Cycles: `"error"` raises `ValueError` for a returned open container."""
    previous = _cycle_policy("error")
    try:
        outer = []
        outer.append(Unsupported())
        with pytest.raises(ValueError, match="Circular reference detected"):
            strata.dumps(outer, default=lambda _o: outer)
    finally:
        _cycle_policy(previous)


def test_hook_returning_an_open_container_is_silent_under_ignore_policy():
    """Cycles: `"ignore"` emits `null` silently."""
    previous = _cycle_policy("ignore")
    try:
        outer = []
        outer.append(Unsupported())
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            out = strata.dumps(outer, default=lambda _o: outer)
        assert "null" in out
        assert not caught
    finally:
        _cycle_policy(previous)


# --- re-entrancy: the hook is the fifth user-code step ---------------------


def test_hook_may_call_dumps_re_entrantly():
    """Record, SchemaCacheLease: a nested `dumps` through the hook itself."""
    out = strata.dumps(
        {"k": Unsupported({"inner": [1, 2, 3]})},
        default=lambda o: json.loads(strata.dumps(o.payload)),
    )
    assert json.loads(out) == {"k": {"inner": [1, 2, 3]}}


def test_nested_dumps_through_the_hook_leaks_no_key_reference():
    """M12 criterion 2 / E26-FIX2b: re-pin T1's destructor release.

    The nested call leases the *fallback* schema state, which is the state
    whose remembered keys used to outlive it (python_dumps_output.h:560-562,
    ledger T1). Driving it through the hook makes that lease ordinary rather
    than rare, so the refcount of a key the nested call remembers must read
    zero drift over repeated calls.
    """
    key = "reentrant_key_" + "x" * 8
    record = {key: 1, "b": 2, "c": 3}

    def hook(_object):
        return json.loads(strata.dumps(record))

    payload = {"outer": Unsupported()}
    # Warm every one-shot cost (lease allocation, schema build, interning).
    for _ in range(3):
        strata.dumps(payload, default=hook)
    gc.collect()
    before = sys.getrefcount(key)
    for _ in range(100):
        strata.dumps(payload, default=hook)
    gc.collect()
    assert sys.getrefcount(key) - before == 0


def test_nested_dumps_through_the_hook_leaks_no_object_reference():
    """Criterion 2: the hook's own argument and result read zero drift too."""
    obj = Unsupported({"a": 1})
    gc.collect()
    before = sys.getrefcount(obj)
    for _ in range(100):
        strata.dumps([obj], default=lambda o: strata.dumps(o.payload))
    gc.collect()
    assert sys.getrefcount(obj) - before == 0


def test_hook_that_shrinks_the_list_being_written():
    """Record, user_steps_: the loop follows the live list (E26-FIX1/FIX2)."""
    live = [1, Unsupported(), 3, 4, 5]

    def hook(_object):
        del live[2:]
        return "cut"

    assert json.loads(strata.dumps(live, default=hook)) == [1, "cut"]


def test_hook_that_grows_the_list_being_written():
    """user_steps_: an appended element is written, as for every other step."""
    live = [Unsupported()]
    appended = []

    def hook(_object):
        if not appended:
            appended.append(True)
            live.append("added")
        return "hooked"

    assert json.loads(strata.dumps(live, default=hook)) == ["hooked", "added"]


def test_hook_that_clears_the_dict_being_written():
    """user_steps_: a mutated dict is still emitted safely (api.md rules)."""
    live = {"a": Unsupported(), "b": 2, "c": 3}

    def hook(_object):
        live.clear()
        return "hooked"

    # The row read on entry is what a <= 24-key record emits (api.md,
    # "Mutation during serialization"); the test pins that it is valid JSON
    # with no crash, not a particular survivor set.
    parsed = json.loads(strata.dumps(live, default=hook))
    assert parsed["a"] == "hooked"
    assert set(parsed) <= {"a", "b", "c"}


def test_hook_result_release_can_run_a_finalizer():
    """Record: releasing the hook's result is a consequent user-code step."""
    fired = []

    class Orphan:
        def __del__(self):
            fired.append(True)

    def hook(_object):
        Orphan()  # dropped inside the hook
        return "ok"

    assert strata.dumps([Unsupported()], default=hook) == '["ok"]'
    gc.collect()
    assert fired


# --- default=None byte identity over the generated corpus ------------------


def _corpus():
    """Shapes the canonical datasets are built from, plus the edges."""
    return [
        {},
        [],
        "",
        0,
        -1,
        1.5,
        True,
        False,
        None,
        [1, 2, 3],
        {"a": 1, "b": [1, 2], "c": {"d": None}},
        [{"id": i, "name": f"n{i}", "ok": i % 2 == 0} for i in range(50)],
        {f"k{i}": i for i in range(30)},
        {"nested": {"deep": {"deeper": [{"x": 1.25e300}, {"y": -0.0}]}}},
        ["é中文", "tab\there", 'quote"d', "back\\slash"],
        [10**30, -(10**30), 2**63, -(2**63) - 1],
        {"unicode key é": "value", "plain": ["a", "b"]},
        [[[[[["deep"]]]]]],
        {"mixed": [1, "two", 3.0, None, True, {"k": [1]}]},
        strata.loads(
            '[{"a":1,"b":2,"c":3,"d":4,"e":5,"f":6},{"a":7,"b":8,"c":9,"d":10,"e":11,"f":12}]'
        ),
    ]


@pytest.mark.parametrize("value", _corpus())
def test_default_none_is_byte_identical_to_no_default(value):
    """M12 criterion 4: `default=None` is byte-identical to a no-`default` call."""
    assert strata.dumps(value, default=None) == strata.dumps(value)
    assert strata.dumps(value, default=None, return_type="bytes") == strata.dumps(
        value, return_type="bytes"
    )


@pytest.mark.parametrize("value", _corpus())
def test_an_installed_hook_changes_no_supported_byte(value):
    """Criterion 4's other half: a hook nothing reaches changes no output."""
    assert strata.dumps(value, default=lambda _o: "unreachable") == strata.dumps(value)


@pytest.mark.parametrize("value", _corpus())
def test_dump_default_none_is_byte_identical(value, tmp_path: Path):
    """Criterion 4 through the file entry point."""
    plain = tmp_path / "plain.json"
    hooked = tmp_path / "hooked.json"
    strata.dump(value, str(plain))
    strata.dump(value, str(hooked), default=None)
    assert plain.read_bytes() == hooked.read_bytes()


# --- stdlib oracle ---------------------------------------------------------


def _json_safe_hooks():
    return [
        lambda o: "str",
        lambda o: 12345,
        lambda o: None,
        lambda o: [1, 2, 3],
        lambda o: {"type": type(o).__name__},
        lambda o: {"payload": getattr(o, "payload", None)},
    ]


@pytest.mark.parametrize("hook", _json_safe_hooks())
def test_matches_stdlib_json_with_the_same_hook(hook):
    """api.md: stdlib/orjson semantics for the values `default` produces."""
    tree = {
        "a": Unsupported(),
        "b": [Unsupported({"x": 1}), 1, "s"],
        "c": {"d": Unsupported(None)},
    }
    assert json.loads(strata.dumps(tree, default=hook)) == json.loads(
        json.dumps(tree, default=hook)
    )

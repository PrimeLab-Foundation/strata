"""Contract tests for the *latched* half of the serializer's re-entrancy rule.

`tests/unit/test_dumps_reentrancy.py` pins what a mutating callback may
observe. This file pins the narrower thing that makes those answers reachable
at all: the serializer borrows, and takes its strong references only when it is
about to run user code — so a reference taken for a callback has to outlive the
callback, all the way to the end of the walk of the container it protects
(python_dumps.cpp, "Ownership, latched" in the file header).

Each test here fails on an implementation that takes the references, runs the
user code and drops them again, or that latches only the container the code
runs directly beneath, or that forgets to re-latch a container opened after an
earlier event. They pass on any implementation that owns every container for
the whole walk, which is why they are contract tests rather than a description
of the current mechanism.

The fourth user-code step gets its own section: a `Py_DECREF` of the
serializer's own can fire a `__del__` or a weakref callback, and that code sees
the same walk the other three steps do.
"""

import gc
import json
import weakref

import pytest

import strata

BIG = 2**70
BIG_TEXT = str(BIG)
MODES = ("str", "bytes")


def _compact(obj):
    return json.dumps(obj, separators=(",", ":"))


def _dump(obj, mode):
    out = strata.dumps(obj, return_type=mode)
    assert isinstance(out, bytes if mode == "bytes" else str)
    return out.decode() if mode == "bytes" else out


class _Trigger(int):
    """An `int` subclass past int64: `write_int` renders it through `__str__`.

    `action` runs once, on the first value serialized; the collection after it
    is what turns a stale pointer from "usually reads right" into a crash.
    """

    action = None
    fired = 0

    def __str__(self):
        if _Trigger.fired == 0 and _Trigger.action is not None:
            _Trigger.fired = 1
            _Trigger.action()
            gc.collect()
        return int.__str__(self)


@pytest.fixture(autouse=True)
def _reset_trigger():
    _Trigger.action = None
    _Trigger.fired = 0
    yield
    _Trigger.action = None
    _Trigger.fired = 0


@pytest.fixture
def cycle_policy():
    previous = strata.config.get("cycle_policy")
    yield strata.config.set
    strata.config.set("cycle_policy", previous)


# ---------------------------------------------------------------------------
# The reference has to outlive the callback, not merely cover it
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("depth", [1, 2, 3, 4, 5])
@pytest.mark.parametrize("mode", MODES)
def test_the_open_list_survives_past_the_callback_at_every_depth(depth, mode):
    """The callback drops the last reference to every list on the walk, and
    the trigger is the *first* of several elements — so every one of those
    lists is read again after the callback returns.

    An implementation that released its references when `__str__` returned
    reads a freed `ob_item` on the very next element.
    """
    innermost = [_Trigger(BIG), 1, 2]
    node = innermost
    for _ in range(depth):
        node = [node, {"tail": 1}]
    root = node

    _Trigger.action = root.clear

    out = _dump(root, mode)

    assert _Trigger.fired == 1
    # Every level ends where the clear left it: the root is empty, and each
    # level below it was already entered and finishes its own elements.
    expected = f"[{BIG_TEXT},1,2]"
    for _ in range(depth - 1):
        expected = f'[{expected},{{"tail":1}}]'
    assert out == (f"[{expected}]" if depth else expected)


@pytest.mark.parametrize("mode", MODES)
def test_the_open_dict_row_survives_past_the_callback(mode):
    """The callback empties the dict whose row is mid-emit; the entries after
    the trigger still have to be written, from the row as read."""
    record = {"a": {"n": 0}, "big": None, "c": {"n": 2}, "d": {"n": 3}}
    record["big"] = _Trigger(BIG)
    holder = [record]
    _Trigger.action = record.clear

    out = _dump(holder, mode)

    assert _Trigger.fired == 1
    assert out == _compact([{"a": {"n": 0}, "big": BIG, "c": {"n": 2}, "d": {"n": 3}}])


@pytest.mark.parametrize("mode", MODES)
def test_an_unprepared_schema_keeps_its_keys_past_the_callback(mode):
    """A one-off shape misses the schema cache and emits its keys from the
    staged row, so the keys after the trigger are borrowed across it too."""
    record = {"k0": {"n": 0}, "k1": None, "k2": 2, "k3": "three"}
    record["k1"] = _Trigger(BIG)
    _Trigger.action = record.clear

    out = _dump(record, mode)

    assert _Trigger.fired == 1
    assert out == _compact({"k0": {"n": 0}, "k1": BIG, "k2": 2, "k3": "three"})


# ---------------------------------------------------------------------------
# A second event, after the first has been unwound
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_container_opened_after_an_earlier_event_is_protected_too(mode):
    """Two triggers in sibling branches. The first fires while the second
    branch is not open yet; when the walk reaches that branch and fires the
    second, the containers opened since the first must be protected again."""
    first_leaf = [None, 1]
    second_leaf = [None, 2]
    root = [[first_leaf], [second_leaf]]

    calls = []

    class Twice(int):
        def __str__(self):
            calls.append(len(calls))
            root.clear() if len(calls) == 1 else second_leaf.append(9)
            gc.collect()
            return int.__str__(self)

    first_leaf[0] = Twice(BIG)
    second_leaf[0] = Twice(BIG)

    out = _dump(root, mode)

    # The root is cleared by the first trigger, so the walk of the first branch
    # finishes and the loop then sees an empty root. Only one trigger runs.
    assert calls == [0]
    assert out == f"[[[{BIG_TEXT},1]]]"


@pytest.mark.parametrize("mode", MODES)
def test_a_second_event_deeper_than_the_first(mode):
    """The second trigger runs below containers opened after the first, and
    drops the last reference to all of them at once."""
    deep = [None, {"z": 1}]
    branch = [1, 2, deep, {"after": 1}]
    root = [[None, 1], branch]

    order = []

    class Two(int):
        index = -1

        def __str__(self):
            order.append(self.index)
            if len(order) == 2:
                root.clear()
                gc.collect()
            return int.__str__(self)

    root[0][0] = Two(BIG)
    root[0][0].index = 0
    deep[0] = Two(BIG)
    deep[0].index = 1

    out = _dump(root, mode)

    assert order == [0, 1]
    # The clear happens inside `deep`; `deep` and `branch` are still walked to
    # their ends, and the root's loop then finds nothing left.
    assert out == f'[[{BIG_TEXT},1],[1,2,[{BIG_TEXT},{{"z":1}}],{{"after":1}}]]'


# ---------------------------------------------------------------------------
# The fourth step: user code fired by the serializer's own release
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_del_fired_by_the_serializers_own_release_sees_a_live_walk(mode):
    """The callback removes a value from the record, leaving the serializer
    holding the last reference to it. Releasing that reference at the end of
    the record runs `__del__`, which clears the array two levels up — an
    array the walk returns to."""
    deaths = []

    class Doomed(dict):
        def __del__(self):
            deaths.append(1)
            root.clear()

    record = {"a": [None], "doomed": Doomed(x=1), "tail": 2}
    record["a"][0] = _Trigger(BIG)
    root = [record, {"never": 1}]
    _Trigger.action = lambda: record.pop("doomed")

    out = _dump(root, mode)

    assert _Trigger.fired == 1
    assert deaths == [1]
    assert out == _compact([{"a": [BIG], "doomed": {"x": 1}, "tail": 2}])


@pytest.mark.parametrize("mode", MODES)
def test_a_weakref_callback_fired_by_the_serializers_own_release(mode):
    """Same shape, with a weakref callback instead of `__del__`."""
    fired = []

    class Holder(dict):
        __slots__ = ("__weakref__",)

    victim = Holder(x=1)
    record = {"a": [None], "victim": victim, "tail": 2}
    record["a"][0] = _Trigger(BIG)
    root = [record, {"never": 1}]

    def on_death(_ref):
        fired.append(1)
        root.clear()

    reference = weakref.ref(victim, on_death)

    def drop():
        record.pop("victim")

    _Trigger.action = drop
    del victim

    out = _dump(root, mode)

    assert _Trigger.fired == 1
    assert fired == [1]
    assert reference() is None
    assert out == _compact([{"a": [BIG], "victim": {"x": 1}, "tail": 2}])


@pytest.mark.parametrize("mode", MODES)
def test_a_str_subclass_returned_from_str_may_mutate_when_released(mode):
    """`write_int` releases the text `__str__` returned. A `str` subclass can
    run `__del__` there, after the digits are already in the output."""
    deaths = []

    class Text(str):
        def __del__(self):
            deaths.append(1)
            root.clear()

    class Big(int):
        def __str__(self):
            return Text(int.__str__(self))

    root = [[Big(BIG), 1], {"tail": 1}]

    out = _dump(root, mode)

    assert deaths == [1]
    assert out == f"[[{BIG_TEXT},1]]"


# ---------------------------------------------------------------------------
# The cycle warning is the other latch site
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_the_cycle_warning_handler_may_drop_the_whole_walk(mode, cycle_policy, recwarn):
    """The placeholder's warning is user code reached without any `__str__`:
    it must protect the containers on the walk the same way, including the
    repeated container itself, which the finding frame deliberately does not
    push."""
    cycle_policy("cycle_policy", "warn")
    inner = [1, 2]
    inner.append(inner)
    inner.append({"after": 1})
    root = [inner, {"never": 1}]

    def clear_everything(*_args, **_kwargs):
        root.clear()
        inner_snapshot()

    def inner_snapshot():
        gc.collect()

    import warnings

    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = clear_everything
        out = _dump(root, mode)

    # The cycle emits null; the element after it is still written, and the
    # root's loop then finds nothing left.
    assert out == '[[1,2,null,{"after":1}]]'


@pytest.mark.parametrize("mode", MODES)
def test_an_exact_big_int_runs_no_user_code(mode):
    """The exact-`int` half of `write_int` reaches `long_to_decimal_string`,
    which is why it takes no references. Pinned by output, not by mechanism:
    exact big ints keep serializing exactly."""
    values = [2**70, -(2**70), 2**200, 10**30, -(10**30)]
    document = {"v": values, "nested": [{"n": value} for value in values]}

    assert _dump(document, mode) == _compact(document)

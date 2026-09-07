"""Contract tests for the serializer's user-code step counter.

`write_sequence` no longer re-derives a list's `ob_item`/`ob_size` after every
element; it re-derives them after every element that *ran user code*, which
`Serializer::user_steps_` counts (python_dumps.cpp, "Freshness" in the file
header). This file pins the equivalence: at each of the four steps the header
enumerates, a list mutated by that step is followed live exactly as stdlib
json's encoder follows it, and a dict is still emitted as the row read on
entry.

Each test fails on an implementation whose counter misses its step — the list
would be walked from a stale `ob_item`/`ob_size`, which is a read of freed
memory when the mutation reallocated (the defect 79fa3df fixed), and a wrong
answer when it did not. They pass on any implementation that re-reads after
every element, which is why they are contract tests and not a description of
the mechanism.

The steps, in the order the header lists them:

  1. a cycle placeholder's warning under `cycle_policy="warn"`;
  2. an `int` subclass's `__str__` beyond int64;
  3. an **exact** `int` beyond int64 whose decimal conversion CPython 3.12+
     delegates to the `_pylong` Python module;
  4. the serializer's own release of a reference it took at one of those —
     a `__del__` firing out of `Frame`, `DeferredOpen` or `RowLock`.
"""

import gc
import json
import sys
import warnings

import pytest

import strata

BIG = 2**70
BIG_TEXT = str(BIG)
# Comfortably past the ~10 000 digits at which CPython 3.12+ hands the decimal
# conversion to the `_pylong` *Python* module.
HUGE_DIGITS = 20000
MODES = ("str", "bytes")


def _compact(obj):
    return json.dumps(obj, separators=(",", ":"))


def _dump(obj, mode):
    out = strata.dumps(obj, return_type=mode)
    assert isinstance(out, bytes if mode == "bytes" else str)
    return out.decode() if mode == "bytes" else out


class _Once:
    """Fire an action the first time it is asked, then collect.

    The collection is load-bearing: a freed item array stays readable, so a
    stale pointer usually *looks* right until the memory is reused.
    """

    def __init__(self, action):
        self.action = action
        self.fired = 0

    def fire(self):
        if self.fired == 0:
            self.fired = 1
            self.action()
            gc.collect()


@pytest.fixture
def cycle_policy():
    previous = strata.config.get("cycle_policy")
    yield strata.config.set
    strata.config.set("cycle_policy", previous)


@pytest.fixture
def int_max_str_digits():
    previous = sys.get_int_max_str_digits()
    sys.set_int_max_str_digits(0)  # 0 disables the limit
    yield
    sys.set_int_max_str_digits(previous)


@pytest.fixture
def profiler():
    """Install a profile hook for the duration of one test, always removing it."""
    yield sys.setprofile
    sys.setprofile(None)


# ---------------------------------------------------------------------------
# Step 1 — a cycle warning's handler
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_growing_list_is_followed_after_a_cycle_warning(mode, cycle_policy):
    """The warning handler appends; the loop must see the longer list.

    The counter moves in `latch()`, which runs immediately before the warning.
    """
    cycle_policy("cycle_policy", "warn")
    cyclic = []
    cyclic.append(cyclic)
    root = [cyclic, "a"]

    once = _Once(lambda: root.extend(["b", "c"]))

    def showwarning(*args, **kwargs):
        once.fire()

    # PyErr_WarnEx deduplicates through __warningregistry__; the filter reset
    # keeps the warning firing on every run, not only when pytest's warnings
    # plugin has cleared the registry between items.
    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = showwarning
        out = _dump(root, mode)

    assert once.fired == 1
    # The cyclic element emits `[null]`; then the loop re-derives the bounds
    # and walks the two appended elements.
    assert out == '[[null],"a","b","c"]'


@pytest.mark.parametrize("mode", MODES)
def test_a_shrinking_list_ends_where_the_cycle_warning_left_it(mode, cycle_policy):
    """A handler that empties the list ends the loop there, as stdlib json does."""
    cycle_policy("cycle_policy", "warn")
    cyclic = []
    cyclic.append(cyclic)
    root = [cyclic, "a", "b"]

    once = _Once(root.clear)

    # PyErr_WarnEx deduplicates through __warningregistry__; the filter reset
    # keeps the warning firing on every run, not only when pytest's warnings
    # plugin has cleared the registry between items.
    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = lambda *args, **kwargs: once.fire()
        out = _dump(root, mode)

    assert once.fired == 1
    assert out == "[[null]]"


# ---------------------------------------------------------------------------
# Step 2 — an `int` subclass's `__str__`
# ---------------------------------------------------------------------------


def _big_subclass(once):
    class Big(int):
        def __str__(self):
            once.fire()
            return int.__str__(self)

    return Big(BIG)


@pytest.mark.parametrize("mode", MODES)
def test_a_growing_list_is_followed_after_an_int_subclass_str(mode):
    root = ["head"]
    once = _Once(lambda: root.extend(["x", "y"]))
    root.append(_big_subclass(once))

    out = _dump(root, mode)

    assert once.fired == 1
    assert out == f'["head",{BIG_TEXT},"x","y"]'


@pytest.mark.parametrize("mode", MODES)
def test_a_shrinking_list_is_followed_after_an_int_subclass_str(mode):
    root = ["head", None, None, None]
    once = _Once(lambda: root.__setitem__(slice(2, None), []))
    root[1] = _big_subclass(once)

    out = _dump(root, mode)

    assert once.fired == 1
    assert out == f'["head",{BIG_TEXT}]'


@pytest.mark.parametrize("mode", MODES)
def test_a_reallocating_append_run_is_followed_after_an_int_subclass_str(mode):
    """Enough appends to force `ob_item` to move, not merely `ob_size` to grow.

    A counter that missed this step would walk the freed array.
    """
    root = ["head"]
    once = _Once(lambda: root.extend(f"e{index}" for index in range(64)))
    root.append(_big_subclass(once))

    out = _dump(root, mode)

    assert once.fired == 1
    expected = ["head", BIG] + [f"e{index}" for index in range(64)]
    assert out == _compact(expected).replace(str(BIG), BIG_TEXT)


# ---------------------------------------------------------------------------
# Step 3 — an exact `int` through the `_pylong` module (CPython 3.12+)
# ---------------------------------------------------------------------------


@pytest.mark.skipif(
    sys.version_info < (3, 12),
    reason="the decimal conversion is delegated to _pylong only on CPython 3.12+",
)
@pytest.mark.parametrize("mode", MODES)
def test_a_list_mutated_from_inside_pylong_is_followed(mode, int_max_str_digits, profiler):
    """The step that has no `__str__` to hook: `_pylong` runs *bytecode*.

    A profile hook fires on entry to every `_pylong` frame — the import of the
    module included, which is the window that once freed the int itself
    (E26-FIX1 v4). Mutating there is user code running at step 3 exactly.
    """
    huge = 10**HUGE_DIGITS
    root = ["head", huge]
    once = _Once(lambda: root.extend(["x", "y"]))

    def hook(frame, event, arg):
        if event == "call" and frame.f_code.co_filename.endswith("_pylong.py"):
            once.fire()

    profiler(hook)
    out = _dump(root, mode)
    profiler(None)

    assert once.fired == 1
    assert out == f'["head",{huge},"x","y"]'


@pytest.mark.skipif(
    sys.version_info < (3, 12),
    reason="the decimal conversion is delegated to _pylong only on CPython 3.12+",
)
@pytest.mark.parametrize("mode", MODES)
def test_a_dict_mutated_from_inside_pylong_emits_the_row(mode, int_max_str_digits, profiler):
    """A record of at most 24 exact-`str` keys emits the row read on entry.

    api.md's mutation contract: lists are followed live, that dict is not.
    """
    huge = 10**HUGE_DIGITS
    record = {"a": 1, "b": huge, "c": "tail"}
    root = [record]
    once = _Once(record.clear)

    def hook(frame, event, arg):
        if event == "call" and frame.f_code.co_filename.endswith("_pylong.py"):
            once.fire()

    profiler(hook)
    out = _dump(root, mode)
    profiler(None)

    assert once.fired == 1
    assert out == f'[{{"a":1,"b":{huge},"c":"tail"}}]'


# ---------------------------------------------------------------------------
# Step 4 — a `__del__` fired by the serializer's own release
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_list_mutated_from_a_del_fired_by_a_release_is_followed(mode):
    """The finalizer runs when `DeferredOpen` releases what `latch()` owned.

    Step 2 drops the last outside reference to the inner list; the serializer
    is holding it, so the `__del__` fires from `close_container` — after the
    element has been written and before the outer loop looks at its bounds.
    """
    root = []
    fired = []

    class Watched(list):
        def __del__(self):
            fired.append(1)
            root.append("tail")

    once = _Once(lambda: root.__setitem__(0, "gone"))
    root.extend([Watched([_big_subclass(once)]), "a"])

    out = _dump(root, mode)

    assert once.fired == 1
    assert fired == [1]
    # element 0 is the inner list, written before the swap took effect; the
    # release then appends, and the re-derived bounds carry the loop to it.
    assert out == f'[[{BIG_TEXT}],"a","tail"]'


@pytest.mark.parametrize("mode", MODES)
def test_a_shrinking_list_from_a_del_fired_by_a_release(mode):
    root = []
    fired = []

    class Watched(list):
        def __del__(self):
            fired.append(1)
            del root[1:]

    once = _Once(lambda: root.__setitem__(0, "gone"))
    root.extend([Watched([_big_subclass(once)]), "a", "b", "c"])

    out = _dump(root, mode)

    assert once.fired == 1
    assert fired == [1]
    assert out == f'[[{BIG_TEXT}]]'


# ---------------------------------------------------------------------------
# The counter is per-walk state, not per-loop: nesting and re-entrancy
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_every_enclosing_list_re_derives_its_bounds(mode):
    """A step deep in the tree must un-freshen *every* loop above it.

    The counter is one monotone number on the walk, so an inner step moves it
    for the outer loops too. An implementation with a per-loop flag cleared by
    the innermost reader passes the inner assertion and fails the outer one.
    """
    inner = []
    middle = [inner, "m1"]
    outer = [middle, "o1"]

    def mutate():
        inner.append("i-new")  # already entered; not observed by its own loop
        middle.append("m2")
        outer.append("o2")

    once = _Once(mutate)
    inner.append(_big_subclass(once))

    out = _dump(outer, mode)

    assert once.fired == 1
    assert out == f'[[[{BIG_TEXT},"i-new"],"m1","m2"],"o1","o2"]'


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_dumps_inside_a_step_does_not_disturb_the_outer_loop(mode):
    """The nested call leases its own state; the outer loop keeps its bounds."""
    inner_result = {}
    root = ["head"]

    def mutate():
        inner_result["text"] = strata.dumps({"nested": [1, 2, 3]})
        root.append("tail")

    once = _Once(mutate)
    root.append(_big_subclass(once))

    out = _dump(root, mode)

    assert once.fired == 1
    assert inner_result["text"] == '{"nested":[1,2,3]}'
    assert out == f'["head",{BIG_TEXT},"tail"]'


@pytest.mark.parametrize("mode", MODES)
def test_a_tuple_is_unmoved_by_a_step(mode):
    """A tuple cannot change, so the cold block re-derives the same bounds."""
    root = ("head", _big_subclass(_Once(lambda: None)), "tail")
    out = _dump(root, mode)
    assert out == f'["head",{BIG_TEXT},"tail"]'


# ---------------------------------------------------------------------------
# The counter does not fire when nothing ran: ordinary documents are unmoved
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_documents_that_run_nothing_serialize_exactly_as_stdlib_json(mode):
    """The bounds stay hoisted for these, so this is the no-step control."""
    documents = [
        [1, 2, 3, 4],
        [{"id": index, "name": f"n{index}", "ok": index % 2 == 0} for index in range(8)],
        {"users": [{"id": 1, "tags": ["a", "b"]}, {"id": 2, "tags": []}]},
        [[[1]], [[2]], []],
        [2**62, -(2**62), 1.5, None, True, False, "x"],
        list(range(200)),
    ]
    for document in documents:
        assert _dump(document, mode) == _compact(document)

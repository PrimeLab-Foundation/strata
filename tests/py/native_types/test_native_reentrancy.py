"""Native conversions that run user code, and the walk that survives them.

docs/architecture/native_types.md, "Re-entrancy" and "Frames, cycles and
depth": every conversion but a pure leaf is a user-code step, latched before it
runs, so a property, a `utcoffset()`, an Enum's `value` or a Decimal's
`__str__` that mutates the containers being written -- or a collection whose
`gc.callbacks` do -- leaves the walk reading only memory it owns, and the
writers' rules of api.md "Mutation during serialization" still hold: lists are
followed live, a dict of at most 24 exact-`str` keys is emitted as the row read
on entry, a wider dict is followed live. Each mutation is followed by
`gc.collect()` so a freed item array or entry table is reused, not left
readable.
"""

import dataclasses
import datetime as dt
import enum
import gc
import pathlib
import subprocess
import sys
import textwrap
import threading
import warnings
from decimal import Decimal

import pytest

import strata

MODES = ("str", "bytes")


def text(out):
    return out.decode() if isinstance(out, bytes) else out


def on_a_fresh_thread(body):
    box = {}

    def run():
        try:
            box["value"] = body()
        except BaseException as error:  # re-raised on the calling thread
            box["error"] = error

    thread = threading.Thread(target=run)
    thread.start()
    thread.join()
    if "error" in box:
        raise box["error"]
    return box["value"]


def in_a_fresh_interpreter(code, *args):
    """Runs `code` (argv: package root, *args) in a child interpreter; returns its stdout lines.

    For documents that recurse to the depth limit: how much C stack that takes
    is the build's own, and an overflow ends the child, never the suite.
    """
    # The package root goes in by argument: the build gate's fresh build is
    # named by no environment variable.
    package_root = str(pathlib.Path(strata.__file__).resolve().parent.parent)
    prelude = "import sys\nsys.path.insert(0, sys.argv[1])\n"
    result = subprocess.run(
        [sys.executable, "-c", prelude + textwrap.dedent(code), package_root, *args],
        capture_output=True,
        text=True,
        timeout=120,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    return result.stdout.splitlines()


class Once:
    """Runs `action` (then a collection) on its first call; returns `result` every time."""

    def __init__(self, action, result):
        self.action = action
        self.result = result
        self.calls = 0

    def __call__(self):
        self.calls += 1
        if self.calls == 1:
            self.action()
            gc.collect()
        return self.result


# Four conversions that run user code, each written as `"x"` or `1.5`.


def via_property(action):
    @dataclasses.dataclass
    class Lazy:
        field: object = None

    once = Once(action, "x")
    Lazy.field = property(lambda self: once())
    return Lazy.__new__(Lazy), '{"field":"x"}'


def via_utcoffset(action):
    once = Once(action, dt.timedelta(0))

    class Zone(dt.tzinfo):
        def utcoffset(self, when):
            return once()

        def dst(self, when):
            return None

    return dt.datetime(2026, 1, 1, tzinfo=Zone()), '"2026-01-01T00:00:00+00:00"'


def via_value(action):
    once = Once(action, "x")

    class Member(enum.Enum):
        A = 1

        @property
        def value(self):
            return once()

    return Member.A, '"x"'


def via_str(action):
    once = Once(action, "1.5")

    class Money(Decimal):
        def __str__(self):
            return once()

    return Money("1"), "1.5"


CONVERSIONS = [via_property, via_utcoffset, via_value, via_str]
CONVERSION_IDS = ["property", "utcoffset", "value", "__str__"]


@pytest.mark.parametrize("make", CONVERSIONS, ids=CONVERSION_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_clearing_the_enclosing_list_ends_it_after_the_native(mode, make):
    container = [1, None, 2, 3]
    native, written = make(container.clear)
    container[1] = native
    assert text(strata.dumps(container, return_type=mode)) == f"[1,{written}]"


@pytest.mark.parametrize("make", CONVERSIONS, ids=CONVERSION_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_growing_the_enclosing_list_writes_what_was_appended(mode, make):
    container = [1, None]
    native, written = make(lambda: container.extend([{"n": 2}, [3]]))
    container[1] = native
    assert text(strata.dumps(container, return_type=mode)) == f'[1,{written},{{"n":2}},[3]]'


@pytest.mark.parametrize("make", CONVERSIONS, ids=CONVERSION_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_clearing_the_enclosing_record_emits_the_row_read_on_entry(mode, make):
    record = {"a": 1, "b": None, "c": [2], "d": "x"}
    native, written = make(record.clear)
    record["b"] = native
    expected = f'{{"a":1,"b":{written},"c":[2],"d":"x"}}'
    assert text(strata.dumps(record, return_type=mode)) == expected
    assert record == {}


@pytest.mark.parametrize("make", CONVERSIONS, ids=CONVERSION_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_clearing_a_wide_dict_follows_the_dict(mode, make):
    wide = {f"k{index:02d}": index for index in range(30)}
    native, written = make(wide.clear)
    wide["k05"] = native
    expected = ",".join(f'"k{index:02d}":{index}' for index in range(5))
    assert text(strata.dumps(wide, return_type=mode)) == f'{{{expected},"k05":{written}}}'


@pytest.mark.parametrize("make", CONVERSIONS, ids=CONVERSION_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_clearing_every_enclosing_container_at_once(mode, make):
    inner = [1, None, 2]
    record = {"a": inner}
    outer = [record, "tail", {"z": 1}]

    def empty_all():
        inner.clear()
        record.clear()
        outer.clear()

    native, written = make(empty_all)
    inner[1] = native
    assert text(strata.dumps(outer, return_type=mode)) == f'[{{"a":[1,{written}]}}]'


@pytest.mark.parametrize("make", CONVERSIONS, ids=CONVERSION_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_records_of_one_shape_survive_a_conversion_that_clears_them(mode, make):
    # Past eight records the thread's schema cache holds their shape and the
    # fused record writer takes them.
    def body():
        rows = [{"id": index, "v": index, "w": "x"} for index in range(12)]
        native, written = make(lambda: [row.clear() for row in rows])
        rows[10]["v"] = native
        # Row 10 is emitted as read on entry; row 11 is read after the clear.
        expected = [f'{{"id":{i},"v":{written if i == 10 else i},"w":"x"}}' for i in range(11)]
        expected.append("{}")
        return text(strata.dumps(rows, return_type=mode)), "[" + ",".join(expected) + "]"

    for out, expected in (body(), on_a_fresh_thread(body)):
        assert out == expected


# ---------------------------------------------------------------------------
# Collections: `gc.callbacks` running inside a conversion.
# ---------------------------------------------------------------------------


@pytest.fixture
def gc_callback():
    added = []

    def install(callback):
        gc.callbacks.append(callback)
        added.append(callback)

    yield install
    for callback in added:
        gc.callbacks.remove(callback)


@pytest.mark.parametrize("make", CONVERSIONS, ids=CONVERSION_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_a_gc_callback_during_a_conversion_may_clear_the_containers(mode, make, gc_callback):
    inner = [1, None, 2]
    record = {"a": inner, "b": 3}
    outer = [record, {"z": 1}]
    armed = []

    def callback(phase, info):
        if phase == "start" and armed:
            armed.clear()
            inner.clear()
            record.clear()
            outer.clear()

    gc_callback(callback)
    # The conversion's own collection (Once) runs the callback.
    native, written = make(lambda: armed.append(True))
    inner[1] = native
    assert text(strata.dumps(outer, return_type=mode)) == f'[{{"a":[1,{written}],"b":3}}]'


def test_type_resolution_under_constant_collection_is_safe():
    # A fresh interpreter: the first native object resolves the type table
    # inside the walk, with a collection -- and a callback that empties the
    # document -- possible at every allocation.
    code = textwrap.dedent(
        """
        import sys
        sys.path.insert(0, sys.argv[1])
        import dataclasses, datetime, decimal, enum, gc, json, uuid
        import strata

        class Color(enum.Enum):
            RED = "red"

        @dataclasses.dataclass
        class Row:
            a: object
            b: object

        doc = [
            [datetime.date(2026, 1, index % 28 + 1), uuid.UUID(int=index),
             decimal.Decimal(index) / 3, Color.RED, Row({index}, frozenset({"x"}))]
            for index in range(200)
        ]
        victims = list(doc)

        def callback(phase, info):
            if phase == "start" and victims:
                victims.pop().clear()

        gc.callbacks.append(callback)
        gc.set_threshold(1)
        for _ in range(3):
            json.loads(strata.dumps(doc))
            json.loads(strata.dumps(doc, return_type="bytes").decode())
        print("ok")
        """
    )
    # The package root goes in by argument: the build gate's fresh build is
    # named by no environment variable.
    package_root = str(pathlib.Path(strata.__file__).resolve().parent.parent)
    result = subprocess.run(
        [sys.executable, "-c", code, package_root],
        capture_output=True,
        text=True,
        timeout=120,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    assert result.stdout.strip() == "ok"


# ---------------------------------------------------------------------------
# A set mutated while it is written.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_set_grown_by_a_field_read_raises_its_runtime_error(mode):
    tags = {"a", "b", "c"}

    @dataclasses.dataclass(eq=False)
    class Tagger:
        name: str

    Tagger.name = property(lambda self: tags.add(f"new-{len(tags)}") or "t")
    tags.add(Tagger.__new__(Tagger))
    with pytest.raises(RuntimeError, match="^Set changed size during iteration$"):
        strata.dumps({"doc": [1, {"tags": tags}]}, return_type=mode)
    # The serializer is left usable.
    assert text(strata.dumps([{"a": 1}], return_type=mode)) == '[{"a":1}]'


@pytest.mark.parametrize("mode", MODES)
def test_a_set_emptied_by_its_own_element_raises_its_runtime_error(mode):
    tags = {1, 2, 3}

    class Clearer(enum.Enum):
        A = 1

        @property
        def value(self):
            tags.clear()
            gc.collect()
            return 0

    tags.add(Clearer.A)
    with pytest.raises(RuntimeError, match="changed size during iteration"):
        strata.dumps([tags], return_type=mode)


# ---------------------------------------------------------------------------
# Cycles through a dataclass, and depth.
# ---------------------------------------------------------------------------


@dataclasses.dataclass
class Node:
    name: str
    children: list = dataclasses.field(default_factory=list)


def _cyclic_tree():
    root = Node("root")
    child = Node("child", [root])
    root.children.append(child)
    return {"tree": root, "rows": [{"id": 1}]}


EXPECTED_CYCLE = (
    '{"tree":{"name":"root","children":[{"name":"child","children":[null]}]},"rows":[{"id":1}]}'
)


@pytest.fixture
def cycle_policy():
    saved = strata.config.get("cycle_policy")
    yield lambda policy: strata.config.set("cycle_policy", policy)
    strata.config.set("cycle_policy", saved)


@pytest.mark.parametrize("mode", MODES)
def test_a_dataclass_cycle_under_warn(mode, cycle_policy):
    cycle_policy("warn")
    with pytest.warns(RuntimeWarning, match="Circular reference detected") as caught:
        out = strata.dumps(_cyclic_tree(), return_type=mode)
    assert text(out) == EXPECTED_CYCLE
    assert len(caught) == 1


@pytest.mark.parametrize("mode", MODES)
def test_a_dataclass_cycle_under_error(mode, cycle_policy):
    cycle_policy("error")
    with pytest.raises(ValueError, match="^Circular reference detected$"):
        strata.dumps(_cyclic_tree(), return_type=mode)


@pytest.mark.parametrize("mode", MODES)
def test_a_dataclass_cycle_under_ignore(mode, cycle_policy):
    cycle_policy("ignore")
    with warnings.catch_warnings():
        warnings.simplefilter("error")
        assert text(strata.dumps(_cyclic_tree(), return_type=mode)) == EXPECTED_CYCLE


@pytest.mark.parametrize("mode", MODES)
def test_a_dataclass_cycle_in_dumps_with_default_follows_the_policy(mode, cycle_policy):
    cycle_policy("error")
    with pytest.raises(ValueError, match="^Circular reference detected$"):
        strata.dumps_with_default(_cyclic_tree(), str, return_type=mode)
    cycle_policy("ignore")
    assert text(strata.dumps_with_default(_cyclic_tree(), str, return_type=mode)) == (
        EXPECTED_CYCLE
    )


@pytest.mark.parametrize("mode", MODES)
def test_an_enum_chain_past_the_limit_raises_inside_a_document(mode):
    # The chain recurses to the depth limit, so it runs in a child interpreter.
    lines = in_a_fresh_interpreter(
        """
        import enum
        import strata

        class Loop(enum.Enum):
            SELF = 1

        mode = sys.argv[2]
        member = Loop.SELF
        member._value_ = member
        for call in (
            lambda: strata.dumps({"rows": [{"id": 1, "state": member}]}, return_type=mode),
            lambda: strata.dumps_with_default([member], str, return_type=mode),
        ):
            try:
                call()
            except ValueError as error:
                print(error)
        member._value_ = 1
        out = strata.dumps({"state": member}, return_type=mode)
        print(out.decode() if isinstance(out, bytes) else out)
        """,
        mode,
    )
    assert lines == ["Maximum serialization depth exceeded"] * 2 + ['{"state":1}']


@pytest.mark.parametrize("mode", MODES)
def test_dataclasses_nested_to_the_depth_limit_on_a_thread(mode):
    # A fresh thread gets the platform's default stack; the chain recurses to
    # the depth limit on it, so it runs in a child interpreter.
    lines = in_a_fresh_interpreter(
        """
        import dataclasses, threading
        import strata

        @dataclasses.dataclass
        class Link:
            next: object = None

        def chain(levels):
            node = None
            for _ in range(levels):
                node = Link(node)
            return node

        mode, limit, out = sys.argv[2], sys.getrecursionlimit(), []

        def run():
            written = strata.dumps(chain(limit), return_type=mode)
            written = written.decode() if isinstance(written, bytes) else written
            out.append(written == '{"next":' * limit + "null" + "}" * limit)
            try:
                strata.dumps(chain(limit + 1), return_type=mode)
            except ValueError as error:
                out.append(error)

        thread = threading.Thread(target=run)
        thread.start()
        thread.join()
        for line in out:
            print(line)
        """,
        mode,
    )
    assert lines == ["True", "Maximum serialization depth exceeded"]


def test_a_dataclass_type_left_without_an_owner_while_its_fields_are_read_is_held():
    # Review P1: the field-name lookup holds type(obj) across dataclasses.fields()
    # and the field reads. Here a field's `name` read moves the instance to
    # another class and collects, which frees the dataclass type unless the
    # serializer holds it (make test-py-asan reports the read otherwise).
    state = {}

    class Other:
        pass

    class Trap(dataclasses.Field):
        __slots__ = ()

        @property
        def name(self):
            instance = state.pop("instance", None)
            if instance is not None:
                instance.__class__ = Other
                gc.collect()
            return dataclasses.Field.name.__get__(self)

    def build():
        @dataclasses.dataclass
        class Doomed:
            x: int = 1

        Doomed.__dataclass_fields__["x"].__class__ = Trap
        return Doomed()

    instance = build()
    state["instance"] = instance
    assert strata.dumps(instance) == '{"x":1}'
    assert type(instance) is Other and "instance" not in state

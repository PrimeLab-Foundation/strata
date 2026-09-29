"""Integration tests for `strata.dumps_with_default(obj, default, *, return_type="str")`.

docs/architecture/dumps_with_default.md (api.md, dumps_with_default): the stdlib
oracle `json.loads(dumps_with_default(o, f)) == json.loads(json.dumps(o,
default=f))` over a generated corpus of JSON-safe callables; NDJSON lines and
the documented file composition loading back through strata; a callable that
mutates the container being written, calls either entry point again, or runs
a collection or finalizer, including the fixed use-after-free of a collection
forced at the hook image's cycle-policy read. The clause-by-clause contract
lives in `tests/unit/test_dumps_with_default.py` and `..._state.py`; its
integration mirror, on realistic documents, in
`test_dumps_with_default_contract.py`.

Test composition rule: corpus-wide checks read the output through
`json.loads` against the stdlib, never against `strata.dumps`.
"""

import base64
import datetime
import enum
import fractions
import gc
import ipaddress
import json
import pathlib
import random
import sys
import threading
import warnings
import weakref

import pytest

import strata
from strata import _dumps_hook

MODES = ("str", "bytes")

SEED = 20260927


class Opaque:
    """An unsupported type carrying a tag the callables below convert to."""

    __slots__ = ("__weakref__", "tag")

    def __init__(self, tag=None):
        self.tag = tag


def text(out):
    return out.decode() if isinstance(out, bytes) else out


def compact(obj):
    return json.dumps(obj, separators=(",", ":"), ensure_ascii=False)


def on_a_fresh_thread(body):
    """Run `body` on a new thread (an empty schema cache) and re-raise its error."""
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


# ---------------------------------------------------------------------------
# A JSON-safe callable and the generated corpus for the stdlib oracle. Every
# type here is unsupported by `dumps`: the native types `dumps` now writes
# itself (docs/architecture/native_types.md) never reach a callable, and their
# oracle corpus lives in tests/py/native_types/.
# ---------------------------------------------------------------------------


class Level(enum.IntEnum):
    LOW = 1
    HIGH = 3


class Bag:
    """An unsupported container: the callable returns its items, sorted."""

    def __init__(self, items):
        self.items = list(items)


class Record:
    """An unsupported record: the callable returns a dict of unsupported children."""

    def __init__(self, name, span, tags, where, ids):
        self.name = name
        self.span = span
        self.tags = tags
        self.where = where
        self.ids = ids


class Node:
    """Not a dataclass: the callable returns a dict whose children are Nodes again."""

    def __init__(self, name, children=()):
        self.name = name
        self.children = list(children)


def json_safe(obj):
    """A `default` whose every return is a JSON type (so stdlib never chains)."""
    if isinstance(obj, datetime.timedelta):
        return obj.total_seconds()
    if isinstance(obj, (fractions.Fraction, ipaddress.IPv4Address)):
        return str(obj)
    if isinstance(obj, range):
        return list(obj)
    if isinstance(obj, Bag):
        return sorted(obj.items)
    if isinstance(obj, Record):
        return {
            "name": obj.name,
            "span": obj.span,
            "tags": obj.tags,
            "where": obj.where,
            "ids": obj.ids,
        }
    if isinstance(obj, complex):
        return [obj.real, obj.imag]
    if isinstance(obj, bytes):
        return base64.b64encode(obj).decode("ascii")
    if isinstance(obj, pathlib.PurePath):
        return obj.as_posix()
    if isinstance(obj, Node):
        return {"name": obj.name, "children": obj.children}
    if isinstance(obj, Opaque):
        return obj.tag
    raise TypeError(f"no conversion for {type(obj).__name__}")


def via_nested_hooked(obj):
    """A callable that serializes the object with a nested `dumps_with_default`."""
    return strata.dumps_with_default(obj, json_safe)


def _unsupported(rng):
    roll = rng.randrange(13)
    # Built eagerly, one of each, so the draw order stays fixed per call.
    values = [
        datetime.timedelta(seconds=rng.randrange(10**6)),
        fractions.Fraction(rng.randrange(-(10**6), 10**6), rng.randrange(1, 1000)),
        ipaddress.IPv4Address(rng.getrandbits(32)),
        pathlib.PurePosixPath(f"p/{rng.randrange(99)}"),
        range(rng.randrange(5)),
        Opaque(rng.choice(["red", "blue"])),
        Opaque(rng.choice([(1, 2), None])),
        rng.choice(list(Level)),
        Bag({rng.randrange(100) for _ in range(rng.randrange(5))}),
        Bag(f"s{rng.randrange(9)}" for _ in range(rng.randrange(4))),
        complex(rng.randrange(9), -rng.randrange(9)),
        rng.randbytes(rng.randrange(12)),
        Record(
            name=f"e{rng.randrange(99)}",
            span=datetime.timedelta(minutes=rng.randrange(600)),
            tags=Bag({"a", "b"}),
            where=complex(rng.random(), -1.5),
            ids=[Opaque(rng.getrandbits(64)), pathlib.PurePosixPath("a/b")],
        ),
    ]
    if rng.random() < 0.05:
        return Node("root", [Node("a", [Node("a1")]), Node("b")])
    return values[roll]


_SCALARS = [
    None,
    True,
    False,
    0,
    -7,
    2**63,
    10**30,
    0.5,
    -0.0,
    1e300,
    "",
    "text",
    "é 你 \U0001f600",
]


def _random_value(rng, depth=0):
    roll = rng.random()
    if depth > 3 or roll < 0.3:
        return rng.choice(_SCALARS)
    if roll < 0.5:
        return _unsupported(rng)
    if roll < 0.65:
        return [_random_value(rng, depth + 1) for _ in range(rng.randint(0, 5))]
    if roll < 0.75:
        width = rng.randint(1, 6)
        return [
            {f"f{index}": _random_value(rng, depth + 2) for index in range(width)}
            for _ in range(rng.randint(1, 8))
        ]
    return {f"k{index}": _random_value(rng, depth + 1) for index in range(rng.randint(0, 7))}


def _corpus(count):
    rng = random.Random(SEED)
    return [_random_value(rng) for _ in range(count)]


CORPUS = _corpus(250)

HOOKS = [("json-safe", json_safe), ("nested-dumps_with_default", via_nested_hooked)]


@pytest.mark.parametrize("hook", [h for _, h in HOOKS], ids=[n for n, _ in HOOKS])
@pytest.mark.parametrize("mode", MODES)
def test_the_stdlib_oracle_agrees_over_the_corpus(mode, hook):
    for document in CORPUS:
        expected = json.loads(json.dumps(document, default=hook))
        assert json.loads(strata.dumps_with_default(document, hook, return_type=mode)) == expected


@pytest.mark.parametrize("mode", MODES)
def test_the_stdlib_oracle_agrees_on_a_fresh_thread(mode):
    def body():
        return [
            strata.dumps_with_default(document, default=json_safe, return_type=mode)
            for document in CORPUS
        ]

    for document, out in zip(CORPUS, on_a_fresh_thread(body), strict=True):
        assert json.loads(out) == json.loads(json.dumps(document, default=json_safe))


def test_the_callable_is_called_where_the_stdlib_calls_it():
    ours, theirs = [], []

    def recorder(calls):
        def hook(obj):
            calls.append(type(obj))
            return json_safe(obj)

        return hook

    for document in CORPUS:
        strata.dumps_with_default(document, recorder(ours))
        json.dumps(document, default=recorder(theirs))
    assert ours == theirs
    kinds = set(ours)
    for kind in (datetime.timedelta, fractions.Fraction, Opaque, Bag, Record, Node):
        assert kind in kinds
    # An IntEnum is an `int`: written directly, never handed to the callable.
    assert Level not in kinds


# ---------------------------------------------------------------------------
# Output read back through strata: NDJSON lines, and the documented file
# composition that stands in for a hooked `dump` (design record, "What does not
# get a hook").
# ---------------------------------------------------------------------------


def _records(count):
    rng = random.Random(SEED + 1)
    return [
        {
            "region": rng.choice(["eu", "us", "apac"]),
            "id": index,
            "at": datetime.timedelta(hours=index),
            "amount": fractions.Fraction(index, 4),
            "tags": Bag({rng.randrange(5) for _ in range(3)}),
            "color": Opaque(rng.choice(["red", "blue"])),
        }
        for index in range(count)
    ]


def test_ndjson_lines_written_with_the_callable_load_back(tmp_path):
    records = _records(30)
    target = tmp_path / "records.ndjson"
    lines = [strata.dumps_with_default(record, json_safe) for record in records]
    target.write_text("\n".join(lines) + "\n", encoding="utf-8")
    expected = [json.loads(json.dumps(record, default=json_safe)) for record in records]
    assert strata.load(target) == expected
    assert list(strata.load(target, iterator=True)) == expected


def test_the_documented_file_composition_loads_back(tmp_path):
    for index, document in enumerate(CORPUS[:20]):
        target = tmp_path / f"doc{index}.json"
        target.write_bytes(
            strata.dumps_with_default(document, json_safe, return_type="bytes") + b"\n"
        )
        assert strata.load(target) == json.loads(json.dumps(document, default=json_safe))


# ---------------------------------------------------------------------------
# The callable mutates the container being written (api.md "Mutation during
# serialization", the writers' rules unchanged in the hook image: lists are
# followed live; a dict of at most 24 exact-`str` keys is emitted as the row
# read on entry; wider dicts are followed live).
# ---------------------------------------------------------------------------


class Once:
    """A callable that runs `action` on its first call, then returns `result`."""

    def __init__(self, action, result="x"):
        self.action = action
        self.result = result
        self.calls = 0

    def __call__(self, obj):
        self.calls += 1
        if self.calls == 1:
            self.action()
            # Freed item arrays and entry tables often still *read* right; a
            # collection reuses the memory so a stale pointer shows.
            gc.collect()
        return self.result


@pytest.mark.parametrize("mode", MODES)
def test_a_callable_that_grows_the_list_writes_what_it_appended(mode):
    doc = [Opaque(), 1]
    # A thousand appends reallocate the item array under the walk.
    hook = Once(lambda: doc.extend(range(1000)))
    out = text(strata.dumps_with_default(doc, hook, return_type=mode))
    assert out == compact(["x", 1, *range(1000)])
    assert hook.calls == 1


@pytest.mark.parametrize("mode", MODES)
def test_a_callable_that_shrinks_the_list_ends_it_there(mode):
    doc = [{"a": 0}, Opaque(), {"a": 2}, {"a": 3}, {"a": 4}]

    def shrink():
        del doc[2:]

    out = text(strata.dumps_with_default(doc, Once(shrink), return_type=mode))
    assert out == '[{"a":0},"x"]'


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("index", range(5))
def test_a_callable_that_clears_the_list_at_every_index(mode, index):
    doc = [{"a": position} for position in range(5)]
    doc[index] = Opaque()
    out = text(strata.dumps_with_default(doc, Once(doc.clear), return_type=mode))
    assert out == compact([*({"a": position} for position in range(index)), "x"])


@pytest.mark.parametrize("mode", MODES)
def test_a_callable_that_replaces_later_elements_writes_the_replacements(mode):
    doc = [Opaque(), 1, 2]

    def replace():
        for position in range(1, len(doc)):
            doc[position] = {"replaced": position}

    out = text(strata.dumps_with_default(doc, Once(replace), return_type=mode))
    assert out == '["x",{"replaced":1},{"replaced":2}]'


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("position", range(4))
@pytest.mark.parametrize("action", ("clear", "grow"))
def test_a_callable_that_mutates_the_dict_emits_the_row_it_read(mode, position, action):
    keys = ["k0", "k1", "k2", "k3"]
    doc = {key: {"v": key} for key in keys}
    doc[keys[position]] = Opaque()
    expected = compact({**doc, keys[position]: "x"})

    def mutate():
        if action == "clear":
            doc.clear()
        else:
            for index in range(500):
                doc[f"extra{index}"] = index

    hook = Once(mutate)
    out = text(strata.dumps_with_default(doc, hook, return_type=mode))
    assert out == expected
    assert hook.calls == 1


@pytest.mark.parametrize("mode", MODES)
def test_a_callable_that_clears_a_wide_dict_follows_the_dict(mode):
    doc = {f"k{index:02d}": index for index in range(30)}
    doc["k00"] = Opaque()
    hook = Once(doc.clear)
    out = text(strata.dumps_with_default(doc, hook, return_type=mode))
    assert out == '{"k00":"x"}'
    assert hook.calls == 1


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("action", ("clear", "grow", "replace"))
def test_the_fused_record_writer_survives_a_callable_that_mutates_the_record(mode, action):
    def body():
        record = {"a": 8, "b": Opaque(), "c": "x"}

        def mutate():
            if action == "clear":
                record.clear()
            elif action == "grow":
                for index in range(500):
                    record[f"extra{index}"] = index
            else:
                for key in list(record):
                    record[key] = "replaced"

        doc = [{"a": index, "b": {"n": index}, "c": "x"} for index in range(8)]
        doc.append(record)
        hook = Once(mutate, result={"n": "hooked"})
        out = text(strata.dumps_with_default(doc, hook, return_type=mode))
        return out, hook.calls

    out, calls = on_a_fresh_thread(body)
    expected = [{"a": index, "b": {"n": index}, "c": "x"} for index in range(8)]
    expected.append({"a": 8, "b": {"n": "hooked"}, "c": "x"})
    assert out == compact(expected)
    assert calls == 1


# ---------------------------------------------------------------------------
# The callable calls either entry point re-entrantly: `dumps` leases
# `_strata`'s state, a nested `dumps_with_default` the hook image's fallback.
# ---------------------------------------------------------------------------


class Payload:
    def __init__(self, body):
        self.body = body


def _payload_doc():
    return [
        {"id": index, "p": Payload({"n": index, "tags": ["a", str(index)]})} for index in range(12)
    ]


@pytest.mark.parametrize("mode", MODES)
def test_a_callable_that_calls_dumps_embeds_the_nested_document(mode):
    def nested(obj):
        return strata.dumps(obj.body)

    def body():
        doc = _payload_doc()
        return text(strata.dumps_with_default(doc, nested, return_type=mode)), doc

    for out, doc in (on_a_fresh_thread(body), body()):
        assert out == json.dumps(doc, default=nested, separators=(",", ":"), ensure_ascii=False)


@pytest.mark.parametrize("mode", MODES)
def test_a_callable_that_calls_dumps_with_default_embeds_the_nested_document(mode):
    def nested(obj):
        return strata.dumps_with_default(obj.body, json_safe)

    def body():
        doc = [
            {"id": index, "p": Payload({"at": datetime.timedelta(days=index + 1)})}
            for index in range(12)
        ]
        return text(strata.dumps_with_default(doc, nested, return_type=mode)), doc

    for out, doc in (on_a_fresh_thread(body), body()):
        assert out == json.dumps(doc, default=nested, separators=(",", ":"), ensure_ascii=False)


@pytest.mark.parametrize("mode", MODES)
def test_a_callable_that_recurses_through_dumps_with_default_builds_the_tree(mode):
    def expand(obj):
        if isinstance(obj, Node):
            children = json.loads(strata.dumps_with_default(obj.children, expand, return_type=mode))
            return {"name": obj.name, "children": children}
        return json_safe(obj)

    tree = Node("root", [Node("a", [Node("a1"), Node("a2", [Node("deep")])]), Node("b")])
    out = strata.dumps_with_default([tree, {"t": tree}], expand, return_type=mode)
    expected = json.loads(json.dumps([tree, {"t": tree}], default=json_safe))
    assert json.loads(out) == expected


@pytest.mark.parametrize("mode", MODES)
def test_a_callable_that_alternates_both_entry_points(mode):
    # Each Node's name goes through `dumps`, its children through a nested
    # `dumps_with_default`: the two images' leases interleave on one thread.
    def expand(obj):
        name = json.loads(strata.dumps({"name": obj.name}, return_type=mode))["name"]
        children = json.loads(strata.dumps_with_default(obj.children, expand, return_type=mode))
        return {"name": name, "children": children}

    def body():
        tree = Node("root", [Node("a", [Node("a1")]), Node("b", [Node("b1"), Node("b2")])])
        document = [{"id": index, "tree": tree} for index in range(6)]
        out = strata.dumps_with_default(document, expand, return_type=mode)
        return json.loads(out), json.loads(json.dumps(document, default=json_safe))

    for got, expected in (on_a_fresh_thread(body), body()):
        assert got == expected
    # Both serializers are left usable on this thread.
    assert strata.dumps([{"a": 1}, {"a": 2}]) == '[{"a":1},{"a":2}]'
    assert strata.dumps_with_default([{"a": Opaque(1)}], json_safe) == '[{"a":1}]'


# ---------------------------------------------------------------------------
# A collection or finalizer runs inside a successful `dumps_with_default`
# (the design record: "a collection possible inside a successful call").
# ---------------------------------------------------------------------------


class Finalized:
    """Cyclic garbage whose finalizer runs `action` when the collector frees it."""

    def __init__(self, action, fired):
        self.action = action
        self.fired = fired
        self.cycle = self

    def __del__(self):
        self.fired.append(True)
        self.action()


@pytest.mark.parametrize("mode", MODES)
def test_a_collection_inside_the_callable_may_mutate_the_list(mode):
    doc = [Opaque(), 1]
    fired = []

    def hook(obj):
        Finalized(lambda: doc.append("from-del"), fired)
        gc.collect()
        return "x"

    out = text(strata.dumps_with_default(doc, hook, return_type=mode))
    assert fired == [True]
    assert out == '["x",1,"from-del"]'


@pytest.mark.parametrize("mode", MODES)
def test_a_collection_inside_the_callable_may_clear_the_dict(mode):
    doc = {"a": Opaque(), "b": {"n": 1}, "c": [2]}
    fired = []

    def hook(obj):
        Finalized(doc.clear, fired)
        gc.collect()
        return "x"

    out = text(strata.dumps_with_default(doc, hook, return_type=mode))
    assert fired == [True]
    assert out == '{"a":"x","b":{"n":1},"c":[2]}'


class Released(list):
    """A returned list whose release runs `action`: its finalizer fires when
    the serializer drops the reference the callable returned."""

    def __del__(self):
        self.action()


@pytest.mark.parametrize("mode", MODES)
def test_releasing_the_returned_value_may_shrink_the_list(mode):
    doc = [Opaque(), 1, 2]
    released = []

    def hook(obj):
        value = Released([7])
        value.action = lambda: (released.append(True), doc.clear())
        return value

    out = text(strata.dumps_with_default(doc, hook, return_type=mode))
    assert released == [True]
    assert out == "[[7]]"


@pytest.mark.parametrize("mode", MODES)
def test_releasing_the_hooked_object_may_fire_a_weakref_callback(mode):
    item = Opaque()
    doc = [item, 1]
    callbacks = []
    watch = weakref.ref(item, lambda ref: (callbacks.append(True), doc.append("from-callback")))
    del item

    def hook(obj):
        doc[0] = None  # the serializer now holds the object's last reference
        return "x"

    out = text(strata.dumps_with_default(doc, hook, return_type=mode))
    assert watch() is None
    assert callbacks == [True]
    assert out == '["x",1,"from-callback"]'


# ---------------------------------------------------------------------------
# Regression, a fixed use-after-free: a collection at the cycle-policy read.
# At a cycle the hook image asks `_strata.config_get` for `cycle_policy`. That
# builtin is METH_VARARGS, so the call allocates an argument tuple the
# collector tracks, and CPython 3.10/3.11 collect inline on such an allocation:
# `gc.callbacks` run inside the walk, right there. A callback that cleared the
# dict being written freed the strings of the row the walk still borrowed, and
# their memory, reused with another payload, showed up in the output. The walk
# now latches its row before the read. CPython 3.12+ only schedule a collection
# on allocation and run it at the next bytecode boundary, so no callback can
# land at the read there and the test skips.
# ---------------------------------------------------------------------------

_ROW_PAYLOAD = "x" * 63


def _write_under_a_forced_collection(document, default):
    # The collector is re-enabled here, so its first tracked allocation after
    # this line is the one inside the native call.
    gc.enable()
    return _dumps_hook.dumps_with_default(document, default)


@pytest.mark.parametrize("policy", ("ignore", "warn"))
def test_a_collection_at_the_cycle_policy_read_cannot_free_the_row_being_written(policy):
    state = {"armed": False, "tail": False, "inline": False}
    reused = []

    def clear_at_the_policy_read(phase, info):
        if phase != "start" or not state["armed"]:
            return
        caller = sys._getframe(1)
        # Inline, at the read: the walk's own frame is the caller, and the
        # tail value (written after the cycle) has not been reached yet.
        if caller.f_code is not _write_under_a_forced_collection.__code__ or state["tail"]:
            return
        state["armed"] = False
        state["inline"] = True
        document.clear()
        # Same-size strings with a different payload, to reuse what was freed.
        reused.extend("".join(["Q" * 63, str(index % 10)]) for index in range(64))

    def tail(obj):
        state["tail"] = True
        return "end"

    # The cycle comes first, so the policy is read before `default` ever runs;
    # the strings are owned by the dict alone.
    document = {}
    document["self"] = document
    for index in range(8):
        document[f"z{index}"] = "".join([_ROW_PAYLOAD, str(index)])
    document["tail"] = Opaque()
    expected = compact(
        {
            "self": None,
            **{f"z{index}": _ROW_PAYLOAD + str(index) for index in range(8)},
            "tail": "end",
        },
    )

    saved_policy = strata.config.get("cycle_policy")
    saved_threshold = gc.get_threshold()
    was_enabled = gc.isenabled()
    with warnings.catch_warnings(record=True) as caught:
        warnings.simplefilter("always")
        try:
            strata.config.set("cycle_policy", policy)
            gc.callbacks.append(clear_at_the_policy_read)
            gc.collect()
            gc.disable()
            gc.set_threshold(1)
            # Drain the 1-tuple free list, so the argument tuple is a real
            # tracked allocation and not a recycled one.
            hold = [(index,) for index in range(6000)]
            state["armed"] = True
            out = _write_under_a_forced_collection(document, tail)
            state["armed"] = False
            del hold
        finally:
            state["armed"] = False
            gc.set_threshold(*saved_threshold)
            if clear_at_the_policy_read in gc.callbacks:
                gc.callbacks.remove(clear_at_the_policy_read)
            if was_enabled:
                gc.enable()
            else:
                gc.disable()
            strata.config.set("cycle_policy", saved_policy)
    if not state["inline"]:
        pytest.skip("the collector cannot be forced inside this call on this version")
    assert document == {}
    assert len(reused) == 64
    assert "Q" not in out
    assert out == expected
    warned = [str(w.message) for w in caught if issubclass(w.category, RuntimeWarning)]
    assert warned == (["Circular reference detected"] if policy == "warn" else [])

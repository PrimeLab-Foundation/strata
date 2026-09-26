"""Integration tests for the `dumps`/`dump` unsupported-type hook (`default=`).

docs/architecture/dumps_default_hook.md § Test placement: `dump` and folder
`dump` with a hook, NDJSON, a hook that mutates the container being written or
calls `dumps` again, finalizers the hook's allocations fire, and the stdlib
oracle `json.loads(strata.dumps(o, default=f)) == json.loads(json.dumps(o,
default=f))` over a generated corpus of JSON-safe hooks. The clause-by-clause
contract lives in `tests/unit/test_dumps_default_hook.py`.
"""

import base64
import dataclasses
import datetime
import decimal
import enum
import gc
import json
import os
import pathlib
import random
import re
import threading
import uuid
import weakref

import pytest

import strata

MODES = ("str", "bytes")

SEED = 20260926

#: An mtime far from "now", so a rewrite of the file cannot keep it by accident.
OLD_MTIME_NS = 1_000_000_000 * 10**9


class Opaque:
    """An unsupported type carrying a tag the hooks below convert to."""

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
# JSON-safe hook and the generated corpus for the stdlib oracle
# ---------------------------------------------------------------------------


class Color(enum.Enum):
    RED = "red"
    BLUE = "blue"


class Pair(enum.Enum):
    ONE = (1, 2)
    NONE = None


class Level(enum.IntEnum):
    LOW = 1
    HIGH = 3


@dataclasses.dataclass
class Point:
    x: float
    y: float


@dataclasses.dataclass
class Event:
    name: str
    at: datetime.datetime
    tags: set
    where: Point
    ids: list


class Node:
    """Not a dataclass: the hook returns a dict whose children are Nodes again."""

    def __init__(self, name, children=()):
        self.name = name
        self.children = list(children)


def json_safe(obj):
    """A `default` whose every return is a JSON type (so stdlib never chains)."""
    if isinstance(obj, (datetime.datetime, datetime.date, datetime.time)):
        return obj.isoformat()
    if isinstance(obj, uuid.UUID):
        return str(obj)
    if isinstance(obj, decimal.Decimal):
        return str(obj)
    if isinstance(obj, enum.Enum):
        return obj.value
    if dataclasses.is_dataclass(obj) and not isinstance(obj, type):
        return dataclasses.asdict(obj)
    if isinstance(obj, (set, frozenset)):
        return sorted(obj)
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


def via_nested_strata(obj):
    """A hook that serializes the object with a nested, hooked `strata.dumps`."""
    return strata.dumps(obj, default=json_safe)


def _unsupported(rng):
    roll = rng.randrange(13)
    base = datetime.datetime(2026, 9, 26, 12, 30, 15, 123456)
    # Built eagerly, one of each, so the draw order stays fixed per call.
    values = [
        base + datetime.timedelta(seconds=rng.randrange(10**6)),
        (base + datetime.timedelta(days=rng.randrange(1000))).date(),
        datetime.time(rng.randrange(24), rng.randrange(60)),
        uuid.UUID(int=rng.getrandbits(128)),
        decimal.Decimal(rng.randrange(-(10**9), 10**9)) / 1000,
        rng.choice(list(Color)),
        rng.choice(list(Pair)),
        rng.choice(list(Level)),
        {rng.randrange(100) for _ in range(rng.randrange(5))},
        frozenset(f"s{rng.randrange(9)}" for _ in range(rng.randrange(4))),
        complex(rng.randrange(9), -rng.randrange(9)),
        rng.randbytes(rng.randrange(12)),
        Event(
            name=f"e{rng.randrange(99)}",
            at=base,
            tags={"a", "b"},
            where=Point(rng.random(), -1.5),
            ids=[uuid.UUID(int=rng.getrandbits(128)), pathlib.PurePosixPath("a/b")],
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

HOOKS = [("json-safe", json_safe), ("nested-strata", via_nested_strata)]


@pytest.mark.parametrize("hook", [h for _, h in HOOKS], ids=[n for n, _ in HOOKS])
@pytest.mark.parametrize("mode", MODES)
def test_the_stdlib_oracle_agrees_over_the_corpus(mode, hook):
    for document in CORPUS:
        expected = json.loads(json.dumps(document, default=hook))
        assert json.loads(strata.dumps(document, return_type=mode, default=hook)) == expected


@pytest.mark.parametrize("mode", MODES)
def test_the_stdlib_oracle_agrees_on_a_fresh_thread(mode):
    def body():
        return [strata.dumps(document, return_type=mode, default=json_safe) for document in CORPUS]

    for document, out in zip(CORPUS, on_a_fresh_thread(body), strict=True):
        assert json.loads(out) == json.loads(json.dumps(document, default=json_safe))


def test_the_corpus_reaches_the_hook():
    calls = []

    def counting(obj):
        calls.append(type(obj))
        return json_safe(obj)

    for document in CORPUS:
        strata.dumps(document, default=counting)
    kinds = set(calls)
    for kind in (datetime.datetime, uuid.UUID, decimal.Decimal, Color, set, Event, Node):
        assert kind in kinds
    # An IntEnum is an `int`: written directly, never handed to the hook.
    assert Level not in kinds


# ---------------------------------------------------------------------------
# `dump` and folder `dump` with a hook
# ---------------------------------------------------------------------------


def test_dump_writes_what_dumps_returns(tmp_path):
    for index, document in enumerate(CORPUS[:60]):
        target = tmp_path / f"doc{index}.json"
        strata.dump(document, target, default=json_safe)
        written = target.read_bytes()
        assert written == strata.dumps(document, return_type="bytes", default=json_safe) + b"\n"
        assert strata.load(target) == json.loads(json.dumps(document, default=json_safe))


def _records(count):
    rng = random.Random(SEED + 1)
    return [
        {
            "region": rng.choice(["eu", "us", "apac"]),
            "team": rng.choice(["core", "web"]),
            "id": index,
            "at": datetime.datetime(2026, 1, 1) + datetime.timedelta(hours=index),
            "amount": decimal.Decimal(index) / 4,
            "tags": {rng.randrange(5) for _ in range(3)},
            "color": rng.choice(list(Color)),
        }
        for index in range(count)
    ]


def test_folder_dump_with_one_split_key(tmp_path):
    records = _records(40)
    strata.dump(records, tmp_path, split_by="region", default=json_safe)
    plain = [json.loads(json.dumps(record, default=json_safe)) for record in records]
    regions = sorted({record["region"] for record in plain})
    for region in regions:
        group = [record for record in plain if record["region"] == region]
        assert json.loads((tmp_path / f"{region}.json").read_text()) == group
    assert strata.load(tmp_path) == [
        record for region in regions for record in plain if record["region"] == region
    ]


def test_folder_dump_with_two_split_keys(tmp_path):
    records = _records(40)
    strata.dump(records, tmp_path, split_by=["region", "team"], default=json_safe)
    plain = [json.loads(json.dumps(record, default=json_safe)) for record in records]
    paths = sorted({f"{record['region']}/{record['team']}.json" for record in plain})
    expected = []
    for path in paths:
        group = [r for r in plain if f"{r['region']}/{r['team']}.json" == path]
        assert json.loads((tmp_path / path).read_text()) == group
        expected.extend(group)
    assert strata.load(tmp_path) == expected


def test_ndjson_lines_written_with_the_hook_load_back(tmp_path):
    records = _records(30)
    target = tmp_path / "records.ndjson"
    lines = [strata.dumps(record, default=json_safe) for record in records]
    target.write_text("\n".join(lines) + "\n", encoding="utf-8")
    expected = [json.loads(json.dumps(record, default=json_safe)) for record in records]
    assert strata.load(target) == expected
    assert list(strata.load(target, iterator=True)) == expected


# ---------------------------------------------------------------------------
# A failing hook leaves the destination as it was
# ---------------------------------------------------------------------------


def _raises(obj):
    raise LookupError("hook failed")


def _returns_unsupported(obj):
    return {1, 2}


def _returns_surrogate(obj):
    return "\ud800"


FAILURES = [
    ("raises", _raises, LookupError),
    ("returns-unsupported", _returns_unsupported, TypeError),
    ("returns-lone-surrogate", _returns_surrogate, UnicodeEncodeError),
]


def _preexisting(path):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(b"original\n")
    os.utime(path, ns=(OLD_MTIME_NS, OLD_MTIME_NS))
    return path


@pytest.mark.parametrize(
    ("hook", "error"), [(h, e) for _, h, e in FAILURES], ids=[n for n, _, _ in FAILURES]
)
def test_a_failing_hook_leaves_an_existing_file_untouched(tmp_path, hook, error):
    target = _preexisting(tmp_path / "out.json")
    with pytest.raises(error):
        strata.dump({"a": [1, 2], "b": Opaque()}, target, default=hook)
    assert target.read_bytes() == b"original\n"
    assert target.stat().st_mtime_ns == OLD_MTIME_NS


def test_the_hooks_exception_reaches_the_caller_of_dump_unchanged(tmp_path):
    target = _preexisting(tmp_path / "out.json")
    error = KeyboardInterrupt("stop")

    def hook(obj):
        raise error

    with pytest.raises(KeyboardInterrupt) as info:
        strata.dump([Opaque()], target, default=hook)
    assert info.value is error
    assert info.value.__context__ is None
    assert target.read_bytes() == b"original\n"


@pytest.mark.parametrize(
    ("hook", "error"), [(h, e) for _, h, e in FAILURES], ids=[n for n, _, _ in FAILURES]
)
def test_a_failing_group_leaves_its_file_and_keeps_the_groups_before_it(tmp_path, hook, error):
    # api.md § Folder mode: "a group whose serialization fails is not written,
    # and the groups written before it stay written".
    stale = _preexisting(tmp_path / "b.json")
    records = [{"g": "a", "v": "fine"}, {"g": "b", "v": Opaque()}, {"g": "a", "v": 2}]
    with pytest.raises(error):
        strata.dump(records, tmp_path, split_by="g", default=hook)
    assert (tmp_path / "a.json").read_text() == '[{"g":"a","v":"fine"},{"g":"a","v":2}]\n'
    assert stale.read_bytes() == b"original\n"
    assert stale.stat().st_mtime_ns == OLD_MTIME_NS


# ---------------------------------------------------------------------------
# Error-table rows 1, 4, 7 and 8 through `dump` (docs/architecture/
# dumps_default_hook.md § Error contract): the exact message, the number of
# hook calls the row allows, and a destination left as it was.
# ---------------------------------------------------------------------------


def exact(message):
    return f"^{re.escape(message)}$"


class Counting:
    """A `default` callable that records every object it is called with."""

    def __init__(self, result="hooked"):
        self.seen = []
        self.result = result

    def __call__(self, obj):
        self.seen.append(obj)
        return self.result


def _assert_untouched(path):
    assert path.read_bytes() == b"original\n"
    assert path.stat().st_mtime_ns == OLD_MTIME_NS


NOT_CALLABLE = [(5, "int"), ("f", "str"), ([], "list"), ({}, "dict"), (Opaque(), "Opaque")]


@pytest.mark.parametrize(("value", "name"), NOT_CALLABLE, ids=[n for _, n in NOT_CALLABLE])
def test_a_non_callable_default_creates_no_file_and_keeps_an_existing_one(tmp_path, value, name):
    # api.md § Unsupported-type hook: "`default` neither `None` nor callable ⇒
    # TypeError("default must be callable, not %s") (the type name), raised
    # before any byte is produced or any file or directory is touched."
    message = exact(f"default must be callable, not {name}")
    fresh = tmp_path / "fresh.json"
    with pytest.raises(TypeError, match=message):
        strata.dump({"a": 1}, fresh, default=value)
    assert not fresh.exists()
    existing = _preexisting(tmp_path / "existing.json")
    with pytest.raises(TypeError, match=message):
        strata.dump([Opaque()], existing, default=value)
    _assert_untouched(existing)


@pytest.mark.parametrize(("value", "name"), NOT_CALLABLE, ids=[n for _, n in NOT_CALLABLE])
def test_a_non_callable_default_creates_no_directory(tmp_path, value, name):
    # api.md § Unsupported-type hook: "`default` neither `None` nor callable ⇒
    # TypeError("default must be callable, not %s") (the type name), raised
    # before any byte is produced or any file or directory is touched."
    message = exact(f"default must be callable, not {name}")
    records = [{"k": "a", "j": "x", "v": 1}, {"k": "b", "j": "y", "v": Opaque()}]
    one = tmp_path / "one"
    with pytest.raises(TypeError, match=message):
        strata.dump(records, one, split_by="k", default=value)
    assert not one.exists()
    two = tmp_path / "two"
    with pytest.raises(TypeError, match=message):
        strata.dump(records, two, split_by=["k", "j"], default=value)
    assert not two.exists()
    stale = _preexisting(tmp_path / "existing" / "a.json")
    with pytest.raises(TypeError, match=message):
        strata.dump(records, stale.parent, split_by="k", default=value)
    assert [path.name for path in stale.parent.iterdir()] == ["a.json"]
    _assert_untouched(stale)


UNSUPPORTED_RETURNS = [(Opaque(), "Opaque"), ({1, 2}, "set"), (b"x", "bytes"), (1j, "complex")]


@pytest.mark.parametrize(
    ("value", "name"), UNSUPPORTED_RETURNS, ids=[n for _, n in UNSUPPORTED_RETURNS]
)
def test_an_unsupported_return_writes_no_file(tmp_path, value, name):
    # api.md § Unsupported-type hook, "Chain bound 1": "The callable returns an
    # unsupported object ⇒ TypeError("default() returned an object of type %s
    # that is not JSON serializable"), and the callable is not called on its
    # own return."
    message = exact(f"default() returned an object of type {name} that is not JSON serializable")
    first = Opaque()
    fresh = tmp_path / "fresh.json"
    hook = Counting(value)
    with pytest.raises(TypeError, match=message):
        strata.dump({"a": [1, 2], "b": first, "c": Opaque()}, fresh, default=hook)
    assert hook.seen == [first]
    assert not fresh.exists()
    existing = _preexisting(tmp_path / "existing.json")
    hook = Counting(value)
    with pytest.raises(TypeError, match=message):
        strata.dump([1, first, Opaque()], existing, default=hook)
    assert hook.seen == [first]
    _assert_untouched(existing)


@pytest.mark.parametrize(
    ("value", "name"), UNSUPPORTED_RETURNS, ids=[n for _, n in UNSUPPORTED_RETURNS]
)
def test_an_unsupported_return_writes_no_group_file(tmp_path, value, name):
    # api.md § Unsupported-type hook, "Chain bound 1": "The callable returns an
    # unsupported object ⇒ TypeError("default() returned an object of type %s
    # that is not JSON serializable"), and the callable is not called on its
    # own return."
    message = exact(f"default() returned an object of type {name} that is not JSON serializable")
    first = Opaque()
    records = [{"g": "a", "v": first}, {"g": "a", "v": Opaque()}]
    fresh = tmp_path / "fresh"
    hook = Counting(value)
    with pytest.raises(TypeError, match=message):
        strata.dump(records, fresh, split_by="g", default=hook)
    assert hook.seen == [first]
    assert not (fresh / "a.json").exists()
    stale = _preexisting(tmp_path / "existing" / "a.json")
    hook = Counting(value)
    with pytest.raises(TypeError, match=message):
        strata.dump(records, stale.parent, split_by="g", default=hook)
    assert hook.seen == [first]
    _assert_untouched(stale)


BAD_KEYS = [(1, "int"), (None, "NoneType"), ((1, 2), "tuple"), (2.5, "float"), (Opaque(), "Opaque")]


def _key_documents(key):
    records = [{"a": index, "b": index} for index in range(8)]
    records.append({"a": 8, key: Opaque()})
    return {"small": {key: Opaque()}, "mixed": {"a": 1, key: Opaque()}, "records": records}


@pytest.mark.parametrize(("key", "name"), BAD_KEYS, ids=[n for _, n in BAD_KEYS])
def test_a_non_str_key_through_dump_never_calls_the_hook(tmp_path, key, name):
    # api.md § Unsupported-type hook: "Keys are excluded. A non-`str` dict key
    # raises the unchanged TypeError("keys must be str, not %s"); `default` is
    # never called for a key."
    message = exact(f"keys must be str, not {name}")

    def body():
        calls = {}
        for shape, document in _key_documents(key).items():
            target = _preexisting(tmp_path / f"{shape}.json")
            hook = Counting()
            with pytest.raises(TypeError, match=message):
                strata.dump(document, target, default=hook)
            _assert_untouched(target)
            calls[shape] = len(hook.seen)
        return calls

    # Once on a fresh thread (the fused record writer is live there) and once
    # on this one, whatever state earlier tests left it in.
    expected = {"small": 0, "mixed": 0, "records": 0}
    assert on_a_fresh_thread(body) == expected
    assert body() == expected


BAD_SPLIT_VALUES = [(Opaque(), "Opaque"), (1.5, "float"), (None, "NoneType"), ([1], "list")]


@pytest.mark.parametrize(("value", "name"), BAD_SPLIT_VALUES, ids=[n for _, n in BAD_SPLIT_VALUES])
def test_a_bad_split_value_creates_no_directory_and_never_calls_the_hook(tmp_path, value, name):
    # api.md § Unsupported-type hook: "`split_by` values are excluded.
    # `dump(..., split_by=...)` groups records before serializing anything, so a
    # split value that is not `str`/`int`/`bool` raises the unchanged
    # TypeError("split_by values must be str, int or bool, not %s"); `default`
    # applies only to the records' serialization, per group file."
    message = exact(f"split_by values must be str, int or bool, not {name}")
    hook = Counting()
    records = [{"k": "a", "v": Opaque()}, {"k": value, "v": Opaque()}]
    one = tmp_path / "one"
    with pytest.raises(TypeError, match=message):
        strata.dump(records, one, split_by="k", default=hook)
    assert not one.exists()
    two = tmp_path / "two"
    with pytest.raises(TypeError, match=message):
        strata.dump([{"k": "a", "j": value, "v": Opaque()}], two, split_by=["k", "j"], default=hook)
    assert not two.exists()
    stale = _preexisting(tmp_path / "existing" / "a.json")
    with pytest.raises(TypeError, match=message):
        strata.dump(records, stale.parent, split_by="k", default=hook)
    assert [path.name for path in stale.parent.iterdir()] == ["a.json"]
    _assert_untouched(stale)
    assert hook.seen == []


# ---------------------------------------------------------------------------
# The hook mutates the container being written (api.md "Mutation during
# serialization": lists are followed live; a dict of at most 24 exact-`str`
# keys is emitted as the row read on entry; wider dicts are followed live).
# ---------------------------------------------------------------------------


class Once:
    """A hook that runs `action` on its first call, then returns `result`."""

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
def test_a_hook_that_grows_the_list_writes_what_it_appended(mode):
    doc = [Opaque(), 1]
    # A thousand appends reallocate the item array under the walk.
    hook = Once(lambda: doc.extend(range(1000)))
    out = text(strata.dumps(doc, return_type=mode, default=hook))
    assert out == compact(["x", 1, *range(1000)])
    assert hook.calls == 1


@pytest.mark.parametrize("mode", MODES)
def test_a_hook_that_shrinks_the_list_ends_it_there(mode):
    doc = [{"a": 0}, Opaque(), {"a": 2}, {"a": 3}, {"a": 4}]

    def shrink():
        del doc[2:]

    out = text(strata.dumps(doc, return_type=mode, default=Once(shrink)))
    assert out == '[{"a":0},"x"]'


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("index", range(5))
def test_a_hook_that_clears_the_list_at_every_index(mode, index):
    doc = [{"a": position} for position in range(5)]
    doc[index] = Opaque()
    out = text(strata.dumps(doc, return_type=mode, default=Once(doc.clear)))
    assert out == compact([*({"a": position} for position in range(index)), "x"])


@pytest.mark.parametrize("mode", MODES)
def test_a_hook_that_replaces_later_elements_writes_the_replacements(mode):
    doc = [Opaque(), 1, 2]

    def replace():
        for position in range(1, len(doc)):
            doc[position] = {"replaced": position}

    out = text(strata.dumps(doc, return_type=mode, default=Once(replace)))
    assert out == '["x",{"replaced":1},{"replaced":2}]'


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("position", range(4))
@pytest.mark.parametrize("action", ("clear", "grow"))
def test_a_hook_that_mutates_the_dict_emits_the_row_it_read(mode, position, action):
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
    out = text(strata.dumps(doc, return_type=mode, default=hook))
    assert out == expected
    assert hook.calls == 1


@pytest.mark.parametrize("mode", MODES)
def test_a_hook_that_clears_a_wide_dict_follows_the_dict(mode):
    doc = {f"k{index:02d}": index for index in range(30)}
    doc["k00"] = Opaque()
    hook = Once(doc.clear)
    out = text(strata.dumps(doc, return_type=mode, default=hook))
    assert out == '{"k00":"x"}'
    assert hook.calls == 1


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("action", ("clear", "grow", "replace"))
def test_the_fused_record_writer_survives_a_hook_that_mutates_the_record(mode, action):
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
        out = text(strata.dumps(doc, return_type=mode, default=hook))
        return out, hook.calls

    out, calls = on_a_fresh_thread(body)
    expected = [{"a": index, "b": {"n": index}, "c": "x"} for index in range(8)]
    expected.append({"a": 8, "b": {"n": "hooked"}, "c": "x"})
    assert out == compact(expected)
    assert calls == 1


# ---------------------------------------------------------------------------
# The hook calls `dumps` re-entrantly
# ---------------------------------------------------------------------------


class Payload:
    def __init__(self, body):
        self.body = body


@pytest.mark.parametrize("mode", MODES)
def test_a_hook_that_calls_dumps_embeds_the_nested_document(mode):
    def nested(obj):
        return strata.dumps(obj.body, default=json_safe)

    def body():
        doc = [
            {"id": index, "p": Payload({"at": datetime.date(2026, 9, index + 1)})}
            for index in range(12)
        ]
        return text(strata.dumps(doc, return_type=mode, default=nested)), doc

    for out, doc in (on_a_fresh_thread(body), body()):
        assert out == json.dumps(doc, default=nested, separators=(",", ":"), ensure_ascii=False)


@pytest.mark.parametrize("mode", MODES)
def test_a_hook_that_recurses_through_dumps_builds_the_tree(mode):
    def expand(obj):
        if isinstance(obj, Node):
            children = json.loads(strata.dumps(obj.children, return_type=mode, default=expand))
            return {"name": obj.name, "children": children}
        return json_safe(obj)

    tree = Node("root", [Node("a", [Node("a1"), Node("a2", [Node("deep")])]), Node("b")])
    out = strata.dumps([tree, {"t": tree}], return_type=mode, default=expand)
    expected = json.loads(json.dumps([tree, {"t": tree}], default=json_safe))
    assert json.loads(out) == expected


# ---------------------------------------------------------------------------
# A collection or finalizer runs inside a successful, hooked `dumps`
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
def test_a_collection_inside_the_hook_may_mutate_the_list(mode):
    doc = [Opaque(), 1]
    fired = []

    def hook(obj):
        Finalized(lambda: doc.append("from-del"), fired)
        gc.collect()
        return "x"

    out = text(strata.dumps(doc, return_type=mode, default=hook))
    assert fired == [True]
    assert out == '["x",1,"from-del"]'


@pytest.mark.parametrize("mode", MODES)
def test_a_collection_inside_the_hook_may_clear_the_dict(mode):
    doc = {"a": Opaque(), "b": {"n": 1}, "c": [2]}
    fired = []

    def hook(obj):
        Finalized(doc.clear, fired)
        gc.collect()
        return "x"

    out = text(strata.dumps(doc, return_type=mode, default=hook))
    assert fired == [True]
    assert out == '{"a":"x","b":{"n":1},"c":[2]}'


class Released(list):
    """A returned list whose release runs `action`: its finalizer fires when
    the serializer drops the reference the hook returned."""

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

    out = text(strata.dumps(doc, return_type=mode, default=hook))
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

    out = text(strata.dumps(doc, return_type=mode, default=hook))
    assert watch() is None
    assert callbacks == [True]
    assert out == '["x",1,"from-callback"]'

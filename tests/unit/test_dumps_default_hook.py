"""Contract tests for the `dumps`/`dump` unsupported-type hook (`default=`).

One named test per clause of docs/context/api.md § "Unsupported-type hook
(`default=`)" and per row of the error table in
docs/architecture/dumps_default_hook.md, each citing the clause it pins. The
integration side (files, folders, mutation from the hook, the stdlib oracle)
lives in `tests/py/test_dumps_default_hook.py`.

The schema cache is thread-local, so every test whose outcome depends on cache
state runs its body on a fresh thread.
"""

import itertools
import json
import math
import random
import re
import sys
import threading
import warnings

import pytest

import strata
from strata import _strata

MODES = ("str", "bytes")

CYCLE_POLICIES = ("warn", "error", "ignore")

SEED = 20260926


class Opaque:
    """An unsupported type: `dumps` has no writer for it."""

    __slots__ = ("__weakref__", "tag")

    def __init__(self, tag=None):
        self.tag = tag


class Counting:
    """A `default` callable that records every object it is called with."""

    def __init__(self, convert=None):
        self.seen = []
        self.convert = convert

    def __call__(self, obj):
        self.seen.append(obj)
        if self.convert is None:
            return f"hooked-{len(self.seen)}"
        return self.convert(obj)

    @property
    def calls(self):
        return len(self.seen)


def text(out):
    """`dumps` output as `str`, whichever return type produced it."""
    return out.decode() if isinstance(out, bytes) else out


def exact(message):
    return f"^{re.escape(message)}$"


_counter = itertools.count()


def fresh_key(tag):
    """A unique, exact, non-interned `str`: the test holds its only references."""
    return "".join(["default-hook-", tag, "-", str(next(_counter))])


def in_fresh_cache(body):
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
# Error table, row 1: "`default` neither `None` nor callable ⇒
# TypeError("default must be callable, not %s") (the type name), raised before
# any byte is produced or any file or directory is touched."
# ---------------------------------------------------------------------------

NOT_CALLABLE = [(5, "int"), ("f", "str"), ([], "list"), ({}, "dict"), (Opaque(), "Opaque")]


@pytest.mark.parametrize(("value", "name"), NOT_CALLABLE, ids=[n for _, n in NOT_CALLABLE])
@pytest.mark.parametrize("mode", MODES)
def test_a_default_that_is_not_callable_is_a_type_error(mode, value, name):
    message = exact(f"default must be callable, not {name}")
    # Refused at the boundary: even a fully supported document is not written.
    with pytest.raises(TypeError, match=message):
        strata.dumps({"a": 1}, return_type=mode, default=value)
    with pytest.raises(TypeError, match=message):
        strata.dumps([Opaque()], return_type=mode, default=value)


@pytest.mark.parametrize(("value", "name"), NOT_CALLABLE, ids=[n for _, n in NOT_CALLABLE])
def test_a_default_that_is_not_callable_touches_no_file(tmp_path, value, name):
    target = tmp_path / "out.json"
    with pytest.raises(TypeError, match=exact(f"default must be callable, not {name}")):
        strata.dump({"a": 1}, target, default=value)
    assert not target.exists()


@pytest.mark.parametrize(("value", "name"), NOT_CALLABLE, ids=[n for _, n in NOT_CALLABLE])
def test_a_default_that_is_not_callable_touches_no_directory(tmp_path, value, name):
    target = tmp_path / "groups"
    with pytest.raises(TypeError, match=exact(f"default must be callable, not {name}")):
        strata.dump([{"k": "a", "v": 1}], target, split_by="k", default=value)
    assert not target.exists()


def test_a_default_that_is_not_callable_wins_over_a_directory_without_split_by(tmp_path):
    # "before ... any file or directory is touched": the boundary check runs
    # ahead of the directory-target ValueError.
    with pytest.raises(TypeError, match=exact("default must be callable, not int")):
        strata.dump({"a": 1}, tmp_path, default=5)


# ---------------------------------------------------------------------------
# Error table, row 2: "An unsupported object with no `default` ⇒ the unchanged
# TypeError("Object of type %s is not JSON serializable")."
# ---------------------------------------------------------------------------

UNSUPPORTED = [
    (Opaque(), "Opaque"),
    ({1, 2}, "set"),
    (frozenset(), "frozenset"),
    (b"x", "bytes"),
    (bytearray(b"x"), "bytearray"),
    (object(), "object"),
    (1j, "complex"),
    (range(3), "range"),
]


@pytest.mark.parametrize(("value", "name"), UNSUPPORTED, ids=[n for _, n in UNSUPPORTED])
@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("spelling", ("absent", "none"))
def test_an_unsupported_object_without_default_keeps_its_message(mode, value, name, spelling):
    kwargs = {} if spelling == "absent" else {"default": None}
    message = exact(f"Object of type {name} is not JSON serializable")
    for document in (value, [value], {"a": value}, [{"a": [value]}]):
        with pytest.raises(TypeError, match=message):
            strata.dumps(document, return_type=mode, **kwargs)


# ---------------------------------------------------------------------------
# Error table, row 3: "The callable raises ⇒ that exception propagates
# unchanged: same object, same type and args, no wrapping or chaining
# (`KeyboardInterrupt`, `MemoryError` and `SystemExit` included); a `dump`
# writes nothing to its destination."
# ---------------------------------------------------------------------------


def _raised():
    return [
        ValueError("boom", 42),
        TypeError("Object of type Opaque is not JSON serializable"),
        RuntimeError(),
        KeyboardInterrupt("stop"),
        SystemExit(3),
        MemoryError("oom"),
    ]


RAISED_IDS = [
    "ValueError",
    "TypeError",
    "RuntimeError",
    "KeyboardInterrupt",
    "SystemExit",
    "MemoryError",
]


def _raising(error):
    def hook(obj):
        raise error

    return hook


@pytest.mark.parametrize("index", range(len(RAISED_IDS)), ids=RAISED_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_an_exception_from_the_callable_propagates_unchanged(mode, index):
    error = _raised()[index]
    args = error.args
    for document in (Opaque(), [1, Opaque()], {"a": {"b": [Opaque()]}}):
        with pytest.raises(type(error)) as info:
            strata.dumps(document, return_type=mode, default=_raising(error))
        assert info.value is error
        assert type(info.value) is type(error)
        assert info.value.args == args
        assert info.value.__context__ is None
        assert info.value.__cause__ is None
        assert info.value.__suppress_context__ is False
    # The thread's serializer is left usable.
    assert text(strata.dumps({"a": [1]}, return_type=mode)) == '{"a":[1]}'


@pytest.mark.parametrize("index", range(len(RAISED_IDS)), ids=RAISED_IDS)
def test_an_exception_from_the_callable_writes_no_file(tmp_path, index):
    error = _raised()[index]
    target = tmp_path / "out.json"
    with pytest.raises(type(error)) as info:
        strata.dump({"a": Opaque()}, target, default=_raising(error))
    assert info.value is error
    assert info.value.__context__ is None
    assert not target.exists()


def test_an_exception_from_the_callable_stops_the_walk_at_once():
    hook = Counting(convert=_raising(ValueError("first")))
    with pytest.raises(ValueError, match="^first$"):
        strata.dumps([Opaque(), Opaque(), Opaque()], default=hook)
    assert hook.calls == 1


# ---------------------------------------------------------------------------
# Error table, row 4 / api.md "Chain bound 1": "The callable returns an
# unsupported object ⇒ TypeError("default() returned an object of type %s that
# is not JSON serializable"), and the callable is not called on its own return."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(("value", "name"), UNSUPPORTED, ids=[n for _, n in UNSUPPORTED])
@pytest.mark.parametrize("mode", MODES)
def test_an_unsupported_return_is_refused_with_its_own_message(mode, value, name):
    message = exact(f"default() returned an object of type {name} that is not JSON serializable")
    for document in (Opaque(), [Opaque()], {"a": Opaque()}, [{"a": [1, Opaque()]}]):
        hook = Counting(convert=lambda obj: value)
        with pytest.raises(TypeError, match=message):
            strata.dumps(document, return_type=mode, default=hook)
        assert hook.calls == 1


@pytest.mark.parametrize("mode", MODES)
def test_the_chain_bound_is_one_for_an_identity_default(mode):
    # stdlib `json` re-enters `default=lambda o: o` until its circular marker
    # stops it; strata refuses the first unsupported return.
    hook = Counting(convert=lambda obj: obj)
    item = Opaque()
    message = exact("default() returned an object of type Opaque that is not JSON serializable")
    with pytest.raises(TypeError, match=message):
        strata.dumps([item], return_type=mode, default=hook)
    assert hook.seen == [item]

    calls = []

    def identity(obj):
        calls.append(obj)
        return obj

    with pytest.raises(TypeError, match=message):
        strata.dumps({"a": item}, return_type=mode, default=identity)
    assert calls == [item]


@pytest.mark.parametrize("mode", MODES)
def test_the_callable_is_called_once_per_unsupported_position(mode):
    # "called ... once per such object": each position gets one call, in
    # document order, and a supported value never reaches the callable.
    items = [Opaque(index) for index in range(4)]
    hook = Counting(convert=lambda obj: obj.tag)
    document = {"a": items[0], "b": [1, items[1], {"c": items[2]}], "d": "x", "e": items[3]}
    out = strata.dumps(document, return_type=mode, default=hook)
    assert json.loads(out) == {"a": 0, "b": [1, 1, {"c": 2}], "d": "x", "e": 3}
    assert hook.seen == items

    repeated = Opaque(7)
    hook = Counting(convert=lambda obj: obj.tag)
    out = strata.dumps([repeated, repeated, repeated], return_type=mode, default=hook)
    assert text(out) == "[7,7,7]"
    assert hook.seen == [repeated, repeated, repeated]


@pytest.mark.parametrize("mode", MODES)
def test_unsupported_objects_inside_a_returned_container_get_their_own_call(mode):
    # "Unsupported objects *nested inside* a returned container are ordinary
    # positions and get their own call."
    outer, inner = Opaque("outer"), Opaque("inner")

    def convert(obj):
        return [inner, {"k": inner}] if obj is outer else obj.tag

    hook = Counting(convert=convert)
    out = strata.dumps([outer], return_type=mode, default=hook)
    assert text(out) == '[["inner",{"k":"inner"}]]'
    assert hook.seen == [outer, inner, inner]


# ---------------------------------------------------------------------------
# Error table, row 5: "The callable returns `None` ⇒ `null` (`None` is a value,
# not a 'cannot handle' sentinel)."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_none_return_is_null(mode):
    hook = Counting(convert=lambda obj: None)
    assert text(strata.dumps(Opaque(), return_type=mode, default=hook)) == "null"
    assert text(strata.dumps([Opaque(), 1], return_type=mode, default=hook)) == "[null,1]"
    assert text(strata.dumps({"a": Opaque()}, return_type=mode, default=hook)) == '{"a":null}'
    assert hook.calls == 3


# ---------------------------------------------------------------------------
# Error table, row 6: "a returned `str` with no UTF-8 encoding ⇒
# UnicodeEncodeError, as for any other `str`."
# ---------------------------------------------------------------------------

LONE_SURROGATES = ["\ud800", "ab\udfff", "x\ud83d"]


@pytest.mark.parametrize("value", LONE_SURROGATES)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_lone_surrogate_is_a_unicode_encode_error(mode, value):
    with pytest.raises(UnicodeEncodeError) as direct:
        strata.dumps([value], return_type=mode)
    with pytest.raises(UnicodeEncodeError) as hooked:
        strata.dumps([Opaque()], return_type=mode, default=lambda obj: value)
    assert str(hooked.value) == str(direct.value)
    assert hooked.value.object == direct.value.object


def test_a_returned_lone_surrogate_writes_no_file(tmp_path):
    target = tmp_path / "out.json"
    with pytest.raises(UnicodeEncodeError):
        strata.dump([Opaque()], target, default=lambda obj: "\ud800")
    assert not target.exists()


# ---------------------------------------------------------------------------
# api.md: "Keys are excluded. A non-`str` dict key raises the unchanged
# TypeError("keys must be str, not %s"); `default` is never called for a key."
# ---------------------------------------------------------------------------

BAD_KEYS = [(1, "int"), (None, "NoneType"), ((1, 2), "tuple"), (2.5, "float"), (Opaque(), "Opaque")]


def _key_documents(key):
    small = {key: 1}
    mixed = {"a": 1, key: 2}
    wide = {f"k{index:02d}": index for index in range(30)}
    wide[key] = 30
    records = [{"a": index, "b": index} for index in range(8)]
    records.append({"a": 8, key: 9})
    return {"small": small, "mixed": mixed, "wide": wide, "records": records}


@pytest.mark.parametrize(("key", "name"), BAD_KEYS, ids=[n for _, n in BAD_KEYS])
@pytest.mark.parametrize("mode", MODES)
def test_the_callable_is_never_called_for_a_key(mode, key, name):
    message = exact(f"keys must be str, not {name}")

    def body():
        seen = {}
        for shape, document in _key_documents(key).items():
            hook = Counting()
            with pytest.raises(TypeError, match=message):
                strata.dumps(document, return_type=mode, default=hook)
            seen[shape] = hook.calls
        return seen

    # Once on a fresh thread (the fused record writer is live there) and once
    # on this one, whatever state earlier tests left it in.
    expected = {"small": 0, "mixed": 0, "wide": 0, "records": 0}
    assert in_fresh_cache(body) == expected
    assert body() == expected


@pytest.mark.parametrize("mode", MODES)
def test_a_key_is_refused_even_where_a_value_was_hooked(mode):
    hook = Counting()
    document = {"a": Opaque(), Opaque(): 1}
    with pytest.raises(TypeError, match=exact("keys must be str, not Opaque")):
        strata.dumps(document, return_type=mode, default=hook)
    assert hook.calls <= 1
    assert all(type(obj) is Opaque for obj in hook.seen)
    assert all(obj is document["a"] for obj in hook.seen)


# ---------------------------------------------------------------------------
# api.md: "`split_by` values are excluded. `dump(..., split_by=...)` groups
# records before serializing anything, so a split value that is not
# `str`/`int`/`bool` raises the unchanged TypeError("split_by values must be
# str, int or bool, not %s"); `default` applies only to the records'
# serialization, per group file."
# ---------------------------------------------------------------------------

BAD_SPLIT_VALUES = [(Opaque(), "Opaque"), (1.5, "float"), (None, "NoneType"), ([1], "list")]


@pytest.mark.parametrize(("value", "name"), BAD_SPLIT_VALUES, ids=[n for _, n in BAD_SPLIT_VALUES])
def test_the_callable_is_never_called_for_a_split_value(tmp_path, value, name):
    hook = Counting()
    records = [{"k": "a", "v": Opaque()}, {"k": value, "v": Opaque()}]
    message = exact(f"split_by values must be str, int or bool, not {name}")
    with pytest.raises(TypeError, match=message):
        strata.dump(records, tmp_path / "one", split_by="k", default=hook)
    with pytest.raises(TypeError, match=message):
        strata.dump(
            [{"k": "a", "j": value, "v": Opaque()}],
            tmp_path / "two",
            split_by=["k", "j"],
            default=hook,
        )
    assert hook.calls == 0


def test_the_callable_applies_to_the_records_of_each_group(tmp_path):
    hook = Counting(convert=lambda obj: obj.tag)
    records = [
        {"k": "a", "v": Opaque("a0")},
        {"k": "b", "v": Opaque("b0")},
        {"k": "a", "v": [Opaque("a1")]},
    ]
    strata.dump(records, tmp_path, split_by="k", default=hook)
    assert hook.calls == 3
    assert (tmp_path / "a.json").read_text() == '[{"k":"a","v":"a0"},{"k":"a","v":["a1"]}]\n'
    assert (tmp_path / "b.json").read_text() == '[{"k":"b","v":"b0"}]\n'


# ---------------------------------------------------------------------------
# api.md § Folder mode: "A directory target without `split_by` ⇒ ValueError —
# whatever failed first: the document is serialized before the target is
# opened, so a `default` callable has already run, and an exception it raised
# is replaced by this ValueError."
# ---------------------------------------------------------------------------


def test_a_directory_target_without_split_by_is_a_value_error_after_the_hook(tmp_path):
    hook = Counting()
    with pytest.raises(ValueError, match=exact("a directory target requires split_by")):
        strata.dump([Opaque()], tmp_path, default=hook)
    assert hook.calls == 1


def test_a_directory_target_replaces_the_callables_exception(tmp_path):
    hook = Counting(convert=_raising(LookupError("from the hook")))
    with pytest.raises(ValueError, match=exact("a directory target requires split_by")):
        strata.dump([Opaque()], tmp_path, default=hook)
    assert hook.calls == 1
    assert sorted(path.name for path in tmp_path.iterdir()) == []


# ---------------------------------------------------------------------------
# api.md: "its return value is serialized in that object's place". Supported
# subclasses are supported values; anything outside the type set is refused
# (Chain bound 1), whatever it subclasses.
# ---------------------------------------------------------------------------


class MyStr(str):
    pass


class MyInt(int):
    pass


class MyFloat(float):
    pass


class MyList(list):
    pass


class MyTuple(tuple):
    __slots__ = ()


class MyDict(dict):
    pass


class MySet(set):
    pass


class MyBytes(bytes):
    pass


SUBCLASS_RETURNS = [
    ("str", lambda: MyStr("text")),
    ("int", lambda: MyInt(-5)),
    ("big-int", lambda: MyInt(1 << 80)),
    ("float", lambda: MyFloat(2.5)),
    ("bool", lambda: True),
    ("list", lambda: MyList([1, "a", None])),
    ("tuple", lambda: MyTuple((1, 2.5))),
    ("dict", lambda: MyDict({"k": 1, "j": [2]})),
]


@pytest.mark.parametrize(
    "make", [m for _, m in SUBCLASS_RETURNS], ids=[n for n, _ in SUBCLASS_RETURNS]
)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_supported_subclass_is_written_like_a_direct_value(mode, make):
    for build in (lambda v: v, lambda v: [v, 1], lambda v: {"a": v}):
        direct = strata.dumps(build(make()), return_type=mode)
        hook = Counting(convert=lambda obj: make())
        hooked = strata.dumps(build(Opaque()), return_type=mode, default=hook)
        assert hooked == direct
        assert hook.calls == 1


REFUSED_SUBCLASSES = [
    ("set", lambda: MySet({1}), "MySet"),
    ("bytes", lambda: MyBytes(b"x"), "MyBytes"),
    ("object", Opaque, "Opaque"),
]


@pytest.mark.parametrize(
    ("make", "name"),
    [(m, n) for _, m, n in REFUSED_SUBCLASSES],
    ids=[i for i, _, _ in REFUSED_SUBCLASSES],
)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_unsupported_subclass_is_refused(mode, make, name):
    hook = Counting(convert=lambda obj: make())
    message = exact(f"default() returned an object of type {name} that is not JSON serializable")
    with pytest.raises(TypeError, match=message):
        strata.dumps([Opaque()], return_type=mode, default=hook)
    assert hook.calls == 1


# ---------------------------------------------------------------------------
# api.md: "a returned container one level past the limit raises 'Maximum
# serialization depth exceeded'" — the same boundary as a direct child at that
# position. The hook is invoked inside no new frame.
# ---------------------------------------------------------------------------


def _python_stack_depth():
    depth = 0
    frame = sys._getframe()
    while frame is not None:
        depth += 1
        frame = frame.f_back
    return depth


def _nest(levels, leaf):
    """`levels` nested lists with `leaf` as the innermost list's one element."""
    node = [leaf]
    for _ in range(levels - 1):
        node = [node]
    return node


RETURNED_CONTAINERS = [("list", lambda: []), ("dict", lambda: {}), ("record", lambda: {"a": [1]})]


@pytest.mark.parametrize(
    "make", [m for _, m in RETURNED_CONTAINERS], ids=[n for n, _ in RETURNED_CONTAINERS]
)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_container_meets_the_depth_limit_of_a_direct_child(mode, make):
    saved = sys.getrecursionlimit()
    limit = max(300, _python_stack_depth() + 120)
    extra = 2 if make() == {"a": [1]} else 1  # containers the returned value opens
    try:
        sys.setrecursionlimit(limit)
        # The returned value's innermost container lands at `limit`: accepted.
        levels = limit - extra
        direct = strata.dumps(_nest(levels, make()), return_type=mode)
        hooked = strata.dumps(_nest(levels, Opaque()), return_type=mode, default=lambda o: make())
        assert hooked == direct
        # One level deeper it lands at `limit + 1`: refused, exactly as direct.
        message = "^Maximum serialization depth exceeded$"
        with pytest.raises(ValueError, match=message):
            strata.dumps(_nest(levels + 1, make()), return_type=mode)
        hook = Counting(convert=lambda obj: make())
        with pytest.raises(ValueError, match=message):
            strata.dumps(_nest(levels + 1, Opaque()), return_type=mode, default=hook)
        assert hook.calls == 1
    finally:
        sys.setrecursionlimit(saved)


@pytest.mark.parametrize("mode", MODES)
def test_the_callable_opens_no_frame_at_the_limit(mode):
    # At exactly `limit` open containers a returned scalar still serializes:
    # the unsupported object is consulted, never pushed as a container.
    saved = sys.getrecursionlimit()
    limit = max(300, _python_stack_depth() + 120)
    try:
        sys.setrecursionlimit(limit)
        direct = strata.dumps(_nest(limit, 1), return_type=mode)
        hooked = strata.dumps(_nest(limit, Opaque()), return_type=mode, default=lambda o: 1)
        assert hooked == direct
        assert text(hooked).count("[") == limit
    finally:
        sys.setrecursionlimit(saved)


# ---------------------------------------------------------------------------
# api.md § Unsupported-type hook: "a returned container that is already open
# is a cycle under the active `cycle_policy`" — output and warning identical to
# the container appearing directly at that position. Checked on a fresh
# thread, cold and after warming the same shapes, so the schema-cache state
# the placement caveat depends on is exercised too.
#
# One placement differs, by decision (docs/decisions.md, 2026-09-26): the
# callable's return is written through `Serializer::write`, the value path,
# whose record branch probes the open-container stack, so a returned open dict
# is reported where it is returned — on time — even at a list-element position
# where a directly-reached, warmed repeated dict lands one container late (the
# 2026-09-11/12 caveat belongs to the array element loop only). The policy
# behaviour there still matches the direct cycle.
# ---------------------------------------------------------------------------


def _list_in_itself(hooked):
    doc = []
    doc.append(Opaque() if hooked else doc)
    return doc, (lambda obj: doc) if hooked else None


def _dict_in_itself(hooked):
    doc = {"a": 1, "b": None}
    doc["b"] = Opaque() if hooked else doc
    return doc, (lambda obj: doc) if hooked else None


def _list_as_dict_value(hooked):
    doc = [{"x": 1, "y": None}]
    doc[0]["y"] = Opaque() if hooked else doc
    return doc, (lambda obj: doc) if hooked else None


def _dict_as_list_element(hooked):
    doc = {"a": 1, "kids": [None]}
    doc["kids"][0] = Opaque() if hooked else doc
    return doc, (lambda obj: doc) if hooked else None


#: The on-time placement of `_dict_as_list_element`'s returned open dict.
ON_TIME_LIST_ELEMENT = '{"a":1,"kids":[null]}'

# (label, builder, an acyclic document of the same shapes at every depth the
# cycle revisits, serialized first when the case is warmed)
CYCLE_SHAPES = [
    ("list-in-itself", _list_in_itself, [[[]]]),
    ("dict-in-itself", _dict_in_itself, {"a": 1, "b": {"a": 1, "b": {"a": 1, "b": 0}}}),
    ("list-as-dict-value", _list_as_dict_value, [{"x": 1, "y": [{"x": 1, "y": []}]}]),
    (
        "dict-as-list-element",
        _dict_as_list_element,
        {"a": 1, "kids": [{"a": 1, "kids": [{"a": 1, "kids": []}]}]},
    ),
]


def _cycle_outcomes(build, warm, hooked, mode):
    """Two calls on a fresh thread after an optional warmup: outputs and warnings."""

    def body():
        doc, hook = build(hooked)
        for _ in range(3 if warm is not None else 0):
            strata.dumps(warm, return_type=mode)
        outcomes = []
        for _ in range(2):
            with warnings.catch_warnings(record=True) as caught:
                warnings.simplefilter("always")
                try:
                    if hook is None:
                        out = strata.dumps(doc, return_type=mode)
                    else:
                        out = strata.dumps(doc, return_type=mode, default=hook)
                    result = ("ok", text(out))
                except ValueError as error:
                    result = ("ValueError", str(error))
            caught_list = [(w.category, str(w.message)) for w in caught]
            outcomes.append((result, caught_list))
        return outcomes

    return in_fresh_cache(body)


@pytest.mark.parametrize(
    ("build", "warm_doc"),
    [(b, w) for _, b, w in CYCLE_SHAPES],
    ids=[n for n, _, _ in CYCLE_SHAPES],
)
@pytest.mark.parametrize("warmed", (False, True), ids=("cold", "warmed"))
@pytest.mark.parametrize("policy", CYCLE_POLICIES)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_open_container_is_the_direct_cycle(mode, policy, warmed, build, warm_doc):
    warm = warm_doc if warmed else None
    saved = strata.config.get("cycle_policy")
    try:
        strata.config.set("cycle_policy", policy)
        direct = _cycle_outcomes(build, warm, hooked=False, mode=mode)
        hooked = _cycle_outcomes(build, warm, hooked=True, mode=mode)
    finally:
        strata.config.set("cycle_policy", saved)
    for outcomes in (direct, hooked):
        for (kind, _), caught in outcomes:
            if policy == "error":
                assert kind == "ValueError"
                assert caught == []
            elif policy == "warn":
                assert kind == "ok"
                assert [category for category, _ in caught] == [RuntimeWarning]
            else:
                assert kind == "ok"
                assert caught == []
    late_caveat_case = warmed and build is _dict_as_list_element and policy != "error"
    if not late_caveat_case:
        assert hooked == direct
        return
    # The decided divergence: the hooked output is the on-time placement, and
    # the warning each call raises is the direct cycle's.
    for ((_, hooked_out), hooked_caught), (_, direct_caught) in zip(hooked, direct, strict=True):
        assert hooked_out == ON_TIME_LIST_ELEMENT
        assert hooked_caught == direct_caught


@pytest.mark.parametrize("policy", CYCLE_POLICIES)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_open_list_under_each_policy(mode, policy):
    # The plain reading of the clause, pinned as literals.
    saved = strata.config.get("cycle_policy")
    try:
        strata.config.set("cycle_policy", policy)
        doc = [1, Opaque()]
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            if policy == "error":
                with pytest.raises(ValueError):
                    strata.dumps(doc, return_type=mode, default=lambda obj: doc)
            else:
                out = strata.dumps(doc, return_type=mode, default=lambda obj: doc)
                assert text(out) == "[1,null]"
        expected = [RuntimeWarning] if policy == "warn" else []
        assert [w.category for w in caught] == expected
    finally:
        strata.config.set("cycle_policy", saved)


# ---------------------------------------------------------------------------
# api.md: "`default=None` is the same call as no `default` at all, byte for
# byte" — over a seeded generated corpus, both return types, through the facade
# and the native module; and a callable that is never reached changes nothing.
# ---------------------------------------------------------------------------

_SCALARS = [
    None,
    True,
    False,
    0,
    -1,
    2**31,
    2**63 - 1,
    -(2**63),
    2**63,
    10**40,
    MyInt(3),
    0.0,
    -0.0,
    0.1,
    -2.5,
    1e300,
    5e-324,
    math.nan,
    math.inf,
    -math.inf,
    MyFloat(1.25),
    "",
    "plain",
    'quote " backslash \\ newline \n tab \t',
    "café 你好 \U0001f600",
    "\x00\x01\x1f\x7f",
    MyStr("sub"),
]


def _random_value(rng, depth=0):
    roll = rng.random()
    if depth > 3 or roll < 0.35:
        return rng.choice(_SCALARS)
    if roll < 0.5:
        return [_random_value(rng, depth + 1) for _ in range(rng.randint(0, 5))]
    if roll < 0.6:
        return tuple(_random_value(rng, depth + 1) for _ in range(rng.randint(0, 4)))
    if roll < 0.7:
        # Records of one shape: the fused record writer's input.
        width = rng.randint(1, 8)
        return [
            {f"f{index}": _random_value(rng, depth + 2) for index in range(width)}
            for _ in range(rng.randint(1, 10))
        ]
    if roll < 0.75:
        return {f"w{index:02d}": rng.choice(_SCALARS) for index in range(rng.randint(25, 30))}
    return {f"key{i}": _random_value(rng, depth + 1) for i in range(rng.randint(0, 6))}


def _corpus(count):
    rng = random.Random(SEED)
    return [_random_value(rng) for _ in range(count)]


CORPUS = _corpus(300)


@pytest.mark.parametrize("mode", MODES)
def test_default_none_is_byte_identical_to_no_default(mode):
    never = Counting()
    for document in CORPUS:
        plain = strata.dumps(document, return_type=mode)
        assert strata.dumps(document, return_type=mode, default=None) == plain
        assert _strata.dumps(document, return_type=mode, default=None) == plain
        assert strata.dumps(document, return_type=mode, default=never) == plain
    assert never.calls == 0


@pytest.mark.parametrize("mode", MODES)
def test_default_none_is_byte_identical_on_a_fresh_thread(mode):
    def body():
        return [
            (
                strata.dumps(document, return_type=mode),
                strata.dumps(document, return_type=mode, default=None),
            )
            for document in CORPUS
        ]

    for plain, spelled in in_fresh_cache(body):
        assert spelled == plain


def test_default_none_writes_the_same_file(tmp_path):
    for index, document in enumerate(CORPUS[:40]):
        plain, spelled = tmp_path / f"plain{index}.json", tmp_path / f"none{index}.json"
        native = tmp_path / f"native{index}.json"
        strata.dump(document, plain)
        strata.dump(document, spelled, default=None)
        _strata.dump(document, str(native), default=None)
        assert spelled.read_bytes() == plain.read_bytes()
        assert native.read_bytes() == plain.read_bytes()


# ---------------------------------------------------------------------------
# api.md "Mutation during serialization" / design record § Re-entrancy: a hook
# that calls `dumps` makes the nested lease ordinary, so E26-FIX2b's refcount
# pin is re-run with the nested call driven *through the hook itself*.
# ---------------------------------------------------------------------------


def _key_delta(document, keys, mode, hook, calls=100):
    """Refcount growth of `keys` over `calls` hooked serializations, after a warmup."""
    strata.dumps(document, return_type=mode, default=hook)
    before = [sys.getrefcount(key) for key in keys]
    for _ in range(calls):
        strata.dumps(document, return_type=mode, default=hook)
    after = [sys.getrefcount(key) for key in keys]
    return [now - was for now, was in zip(after, before, strict=True)]


def _nested_hook(inner, mode, **kwargs):
    def hook(obj):
        return text(strata.dumps(inner, return_type=mode, **kwargs))

    return hook


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_dumps_through_the_hook_releases_its_keys(mode):
    def body():
        inner_key, outer_key = fresh_key("inner"), fresh_key("outer")
        inner = {inner_key: 1}
        document = {outer_key: Opaque()}
        hook = _nested_hook(inner, mode)
        out = strata.dumps(document, return_type=mode, default=hook)
        assert json.loads(out) == {outer_key: json.dumps({inner_key: 1}, separators=(",", ":"))}
        return _key_delta(document, [inner_key, outer_key], mode, hook)

    assert in_fresh_cache(body) == [0, 0]


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_dumps_through_the_hook_releases_a_wide_record(mode):
    def body():
        keys = [fresh_key(f"wide{index}") for index in range(24)]
        inner = [{key: index for index, key in enumerate(keys)} for _ in range(4)]
        document = [Opaque(), {"pad": 1}]
        return _key_delta(document, keys, mode, _nested_hook(inner, mode))

    assert in_fresh_cache(body) == [0] * 24


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_hooked_dumps_through_the_hook_releases_its_keys(mode):
    # The nested call carries a hook of its own, which calls `dumps` again.
    def body():
        deep_key, mid_key = fresh_key("deep"), fresh_key("mid")
        deep = {deep_key: [1, 2]}
        middle = {mid_key: Opaque()}
        mid_hook = _nested_hook(deep, mode)
        document = [Opaque(), Opaque()]
        return _key_delta(
            document, [deep_key, mid_key], mode, _nested_hook(middle, mode, default=mid_hook)
        )

    assert in_fresh_cache(body) == [0, 0]


@pytest.mark.parametrize("mode", MODES)
def test_a_failing_nested_dumps_through_the_hook_releases_its_keys(mode):
    def body():
        key = fresh_key("failing")
        inner = {key: 1, "bad": Opaque()}
        document = [Opaque()]
        hook = _nested_hook(inner, mode)
        message = exact("Object of type Opaque is not JSON serializable")
        with pytest.raises(TypeError, match=message):
            strata.dumps(document, return_type=mode, default=hook)
        before = sys.getrefcount(key)
        for _ in range(50):
            with pytest.raises(TypeError, match=message):
                strata.dumps(document, return_type=mode, default=hook)
        return sys.getrefcount(key) - before

    assert in_fresh_cache(body) == 0


# ---------------------------------------------------------------------------
# The native keyword loop: `return_type` keeps the first compare, `default` the
# second (docs/decisions.md, 2026-09-26). Called on the native function, since
# the facade's own signature refuses an unknown keyword before it gets here.
# ---------------------------------------------------------------------------


def test_the_native_keyword_loop_keeps_every_refusal_it_had():
    # api.md "Unsupported-type hook": `default=None` is the same call as none.
    assert _strata.dumps([1], default=None, return_type="bytes") == b"[1]"
    with pytest.raises(TypeError, match=exact("return_type must be str, not int")):
        _strata.dumps([1], default=str, return_type=5)
    unexpected = exact("dumps() got an unexpected keyword argument 'indent'")
    with pytest.raises(TypeError, match=unexpected):
        _strata.dumps([1], default=str, indent=2)

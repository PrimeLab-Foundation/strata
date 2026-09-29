"""Contract tests for `strata.dumps_with_default(obj, default, *, return_type="str")`.

One named test per row of the error table in
docs/architecture/dumps_with_default.md § Public contract (api.md,
dumps_with_default), each citing the row it pins, plus the signature clause
above the table. Shared state across the two images (the cycle policy read from
`_strata`, the returned-open-container shapes, the E26-FIX2b refcount re-pin)
lives in `test_dumps_with_default_state.py`; the stdlib oracle over a corpus of
unsupported types, mutating and re-entrant callables and finalizers live in
`tests/py/test_dumps_with_default.py`.

Test composition rule (the record's last paragraph): the byte-identity oracle
against `strata.dumps` runs on a small fixed set of documents; corpus-wide
checks compare `json.loads(dumps_with_default(...))` with the stdlib.

The schema cache is thread-local, so every test whose outcome depends on cache
state runs its body on a fresh thread.
"""

import json
import math
import random
import re
import sys
import threading
import warnings

import pytest

import strata
from strata import _dumps_hook, _strata

MODES = ("str", "bytes")

CYCLE_POLICIES = ("warn", "error", "ignore")

#: The public facade and the native entry, which parses its own arguments.
ENTRIES = [("facade", strata.dumps_with_default), ("native", _dumps_hook.dumps_with_default)]
ENTRY_IDS = [name for name, _ in ENTRIES]
ENTRY_FNS = [entry for _, entry in ENTRIES]

SEED = 20260927


class Opaque:
    """An unsupported type: the serializer has no writer for it."""

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
    """Serializer output as `str`, whichever return type produced it."""
    return out.decode() if isinstance(out, bytes) else out


def exact(message):
    return f"^{re.escape(message)}$"


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
# Signature (api.md, dumps_with_default): "`default` is required,
# positional-or-keyword, and must be callable." Both the facade and the native
# entry are held to it; the refusal messages differ only where Python's own
# argument binding words them.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_default_is_accepted_positionally_and_by_keyword(mode, entry):
    positional, keyword = Counting(), Counting()
    document = [1, Opaque(), {"a": Opaque()}]
    by_position = entry(document, positional, return_type=mode)
    by_keyword = entry(document, default=keyword, return_type=mode)
    assert text(by_position) == text(by_keyword) == '[1,"hooked-1",{"a":"hooked-2"}]'
    assert isinstance(by_position, bytes if mode == "bytes" else str)
    assert positional.calls == keyword.calls == 2


@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
def test_a_missing_default_is_a_type_error(entry):
    message = r"^dumps_with_default\(\) missing (1 )?required (positional )?argument:? 'default'$"
    with pytest.raises(TypeError, match=message):
        entry([Opaque()])
    with pytest.raises(TypeError, match=message):
        entry([1], return_type="bytes")


@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
def test_three_positional_arguments_are_a_type_error(entry):
    # `return_type` is keyword-only.
    hook = Counting()
    message = (
        r"^dumps_with_default\(\) takes (1 or )?2 positional arguments "
        r"(but 3 were given|\(3 given\))$"
    )
    with pytest.raises(TypeError, match=message):
        entry([Opaque()], hook, "bytes")
    assert hook.calls == 0


@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
def test_default_given_twice_is_a_type_error(entry):
    first, second = Counting(), Counting()
    message = exact("dumps_with_default() got multiple values for argument 'default'")
    with pytest.raises(TypeError, match=message):
        entry([Opaque()], first, default=second)
    assert first.calls == second.calls == 0


@pytest.mark.parametrize("keyword", ("indent", "sort_keys", "obj_default"))
@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
def test_an_unknown_keyword_is_a_type_error(entry, keyword):
    hook = Counting()
    message = exact(f"dumps_with_default() got an unexpected keyword argument '{keyword}'")
    with pytest.raises(TypeError, match=message):
        entry([Opaque()], hook, **{keyword: 2})
    assert hook.calls == 0


@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
def test_a_non_str_return_type_is_a_type_error_as_in_dumps(entry):
    with pytest.raises(TypeError, match=exact("return_type must be str, not int")) as direct:
        _strata.dumps([1], return_type=5)
    hook = Counting()
    with pytest.raises(TypeError, match=exact(str(direct.value))):
        entry([Opaque()], hook, return_type=5)
    assert hook.calls == 0


# ---------------------------------------------------------------------------
# Row 1: "`default` is not callable — `None` included ⇒ TypeError("default must
# be callable, not %s"), before any byte is produced." `None` means refused,
# not absent: this entry point exists only to run a hook.
# ---------------------------------------------------------------------------

NOT_CALLABLE = [
    (None, "NoneType"),
    (5, "int"),
    ("f", "str"),
    ([], "list"),
    ({}, "dict"),
    (Opaque(), "Opaque"),
]


@pytest.mark.parametrize(("value", "name"), NOT_CALLABLE, ids=[n for _, n in NOT_CALLABLE])
@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_a_default_that_is_not_callable_is_a_type_error(mode, entry, value, name):
    message = exact(f"default must be callable, not {name}")
    # Refused at the boundary: even a fully supported document is not written.
    with pytest.raises(TypeError, match=message):
        entry({"a": 1}, value, return_type=mode)
    with pytest.raises(TypeError, match=message):
        entry([Opaque()], default=value, return_type=mode)


@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
def test_none_is_refused_not_taken_as_absent(entry):
    message = exact("default must be callable, not NoneType")
    for document in ([1], {"a": [True, None]}, "text", None):
        with pytest.raises(TypeError, match=message):
            entry(document, None)
        with pytest.raises(TypeError, match=message):
            entry(document, default=None, return_type="bytes")


@pytest.mark.parametrize(("value", "name"), NOT_CALLABLE, ids=[n for _, n in NOT_CALLABLE])
def test_the_callable_check_runs_before_the_walk(value, name):
    # "before any byte is produced": the walk's own refusals — a bad key, a
    # cycle under "error" — are never reached.
    message = exact(f"default must be callable, not {name}")
    cyclic = []
    cyclic.append(cyclic)
    strata.config.set("cycle_policy", "error")
    for document in ({1: "bad key"}, cyclic, [Opaque(), {2: 3}]):
        with pytest.raises(TypeError, match=message):
            strata.dumps_with_default(document, value)


# ---------------------------------------------------------------------------
# Row 2: "an unsupported type ⇒ `default(obj)` is called once and its return
# written in the object's place, at the object's depth, by the value path."
# ---------------------------------------------------------------------------

UNSUPPORTED = [
    (Opaque(), "Opaque"),
    (b"x", "bytes"),
    (bytearray(b"x"), "bytearray"),
    (object(), "object"),
    (1j, "complex"),
    (range(3), "range"),
]


@pytest.mark.parametrize(("value", "name"), UNSUPPORTED, ids=[n for _, n in UNSUPPORTED])
@pytest.mark.parametrize("mode", MODES)
def test_each_unsupported_type_reaches_the_callable(mode, value, name):
    for document, expected in (
        (value, '"hooked-1"'),
        ([value], '["hooked-1"]'),
        ({"a": value}, '{"a":"hooked-1"}'),
        ([{"a": [value]}], '[{"a":["hooked-1"]}]'),
    ):
        hook = Counting()
        assert text(strata.dumps_with_default(document, hook, return_type=mode)) == expected
        assert len(hook.seen) == 1
        assert hook.seen[0] is value
        assert type(hook.seen[0]).__name__ == name


@pytest.mark.parametrize("mode", MODES)
def test_the_callable_is_called_once_per_unsupported_position(mode):
    # Each position gets one call, in document order, and a supported value
    # never reaches the callable.
    items = [Opaque(index) for index in range(4)]
    hook = Counting(convert=lambda obj: obj.tag)
    document = {"a": items[0], "b": [1, items[1], {"c": items[2]}], "d": "x", "e": items[3]}
    out = strata.dumps_with_default(document, hook, return_type=mode)
    assert json.loads(out) == {"a": 0, "b": [1, 1, {"c": 2}], "d": "x", "e": 3}
    assert hook.seen == items

    repeated = Opaque(7)
    hook = Counting(convert=lambda obj: obj.tag)
    out = strata.dumps_with_default([repeated, repeated, repeated], hook, return_type=mode)
    assert text(out) == "[7,7,7]"
    assert hook.seen == [repeated, repeated, repeated]


@pytest.mark.parametrize("mode", MODES)
def test_unsupported_objects_inside_a_returned_container_get_their_own_call(mode):
    outer, inner = Opaque("outer"), Opaque("inner")

    def convert(obj):
        return [inner, {"k": inner}] if obj is outer else obj.tag

    hook = Counting(convert=convert)
    out = strata.dumps_with_default([outer], hook, return_type=mode)
    assert text(out) == '[["inner",{"k":"inner"}]]'
    assert hook.seen == [outer, inner, inner]


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
        hooked = strata.dumps_with_default(build(Opaque()), hook, return_type=mode)
        assert hooked == direct
        assert hook.calls == 1


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
    # "at the object's depth": the returned value's containers count from the
    # position, and the limit (`sys.getrecursionlimit()`) is `dumps`'s.
    saved = sys.getrecursionlimit()
    limit = max(300, _python_stack_depth() + 120)
    extra = 2 if make() == {"a": [1]} else 1  # containers the returned value opens
    try:
        sys.setrecursionlimit(limit)
        levels = limit - extra
        direct = strata.dumps(_nest(levels, make()), return_type=mode)
        hooked = strata.dumps_with_default(
            _nest(levels, Opaque()), lambda o: make(), return_type=mode
        )
        assert hooked == direct
        message = "^Maximum serialization depth exceeded$"
        with pytest.raises(ValueError, match=message):
            strata.dumps(_nest(levels + 1, make()), return_type=mode)
        hook = Counting(convert=lambda obj: make())
        with pytest.raises(ValueError, match=message):
            strata.dumps_with_default(_nest(levels + 1, Opaque()), hook, return_type=mode)
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
        hooked = strata.dumps_with_default(_nest(limit, Opaque()), lambda o: 1, return_type=mode)
        assert hooked == direct
        assert text(hooked).count("[") == limit
    finally:
        sys.setrecursionlimit(saved)


# ---------------------------------------------------------------------------
# Row 3: "the callable raises ⇒ propagates unchanged (`KeyboardInterrupt`,
# `MemoryError`, `SystemExit` included)."
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
            strata.dumps_with_default(document, _raising(error), return_type=mode)
        assert info.value is error
        assert type(info.value) is type(error)
        assert info.value.args == args
        assert info.value.__context__ is None
        assert info.value.__cause__ is None
        assert info.value.__suppress_context__ is False
    # Both images' serializers are left usable on this thread.
    assert text(strata.dumps_with_default({"a": [1]}, str, return_type=mode)) == '{"a":[1]}'
    assert text(strata.dumps({"a": [1]}, return_type=mode)) == '{"a":[1]}'


def test_an_exception_from_the_callable_stops_the_walk_at_once():
    hook = Counting(convert=_raising(ValueError("first")))
    with pytest.raises(ValueError, match="^first$"):
        strata.dumps_with_default([Opaque(), Opaque(), Opaque()], hook)
    assert hook.calls == 1


# ---------------------------------------------------------------------------
# Row 4: "the callable returns an unsupported type ⇒ TypeError("default()
# returned an object of type %s that is not JSON serializable"); the callable
# is not called on its own return (chain bound 1, by identity)."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(("value", "name"), UNSUPPORTED, ids=[n for _, n in UNSUPPORTED])
@pytest.mark.parametrize("mode", MODES)
def test_an_unsupported_return_is_refused_with_its_own_message(mode, value, name):
    message = exact(f"default() returned an object of type {name} that is not JSON serializable")
    for document in (Opaque(), [Opaque()], {"a": Opaque()}, [{"a": [1, Opaque()]}]):
        hook = Counting(convert=lambda obj: value)
        with pytest.raises(TypeError, match=message):
            strata.dumps_with_default(document, hook, return_type=mode)
        assert hook.calls == 1


@pytest.mark.parametrize("mode", MODES)
def test_the_chain_bound_is_one_for_an_identity_default(mode):
    # stdlib `json` re-enters `default=lambda o: o` until its circular marker
    # stops it; strata refuses the first unsupported return.
    hook = Counting(convert=lambda obj: obj)
    item = Opaque()
    message = exact("default() returned an object of type Opaque that is not JSON serializable")
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default([item], hook, return_type=mode)
    assert hook.seen == [item]
    hook = Counting(convert=lambda obj: obj)
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default({"a": item}, default=hook, return_type=mode)
    assert hook.seen == [item]


REFUSED_SUBCLASSES = [
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
        strata.dumps_with_default([Opaque()], hook, return_type=mode)
    assert hook.calls == 1


# ---------------------------------------------------------------------------
# Row 5: "the callable returns `None` / a lone-surrogate `str` ⇒ `null` /
# `UnicodeEncodeError`."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_none_return_is_null(mode):
    hook = Counting(convert=lambda obj: None)
    assert text(strata.dumps_with_default(Opaque(), hook, return_type=mode)) == "null"
    assert text(strata.dumps_with_default([Opaque(), 1], hook, return_type=mode)) == "[null,1]"
    out = strata.dumps_with_default({"a": Opaque()}, hook, return_type=mode)
    assert text(out) == '{"a":null}'
    assert hook.calls == 3


LONE_SURROGATES = ["\ud800", "ab\udfff", "x\ud83d"]


@pytest.mark.parametrize("value", LONE_SURROGATES)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_lone_surrogate_is_a_unicode_encode_error(mode, value):
    with pytest.raises(UnicodeEncodeError) as direct:
        strata.dumps([value], return_type=mode)
    with pytest.raises(UnicodeEncodeError) as hooked:
        strata.dumps_with_default([Opaque()], lambda obj: value, return_type=mode)
    assert str(hooked.value) == str(direct.value)
    assert hooked.value.object == direct.value.object


# ---------------------------------------------------------------------------
# Row 6: "a non-`str` dict key ⇒ the unchanged TypeError("keys must be str, not
# %s"); the callable is never called for a key."
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
                strata.dumps_with_default(document, hook, return_type=mode)
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
        strata.dumps_with_default(document, hook, return_type=mode)
    assert hook.calls <= 1
    assert all(obj is document["a"] for obj in hook.seen)


# ---------------------------------------------------------------------------
# Row 7: "a returned open container ⇒ a cycle under the active `cycle_policy`,
# reported where it was returned (the M12 ruling)." The plain reading, pinned
# as literals; the shapes, cold and warmed, against the direct cycle are in
# `test_dumps_with_default_state.py`.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("policy", CYCLE_POLICIES)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_open_container_is_a_cycle_under_the_active_policy(mode, policy):
    strata.config.set("cycle_policy", policy)
    for doc, expected in (([1, Opaque()], "[1,null]"), ({"a": Opaque()}, '{"a":null}')):
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            if policy == "error":
                with pytest.raises(ValueError, match=exact("Circular reference detected")):
                    strata.dumps_with_default(doc, lambda obj, doc=doc: doc, return_type=mode)
            else:
                out = strata.dumps_with_default(doc, lambda obj, doc=doc: doc, return_type=mode)
                assert text(out) == expected
        expected_warnings = [RuntimeWarning] if policy == "warn" else []
        assert [w.category for w in caught] == expected_warnings


# ---------------------------------------------------------------------------
# Row 8: "a document with no unsupported object ⇒ byte-identical to
# `dumps(obj)` in both return types; the callable is never called." Byte
# identity against `strata.dumps` on a small fixed set (the composition rule);
# over a seeded corpus, the stdlib oracle.
# ---------------------------------------------------------------------------

FIXED_DOCUMENTS = [
    None,
    True,
    0,
    -(2**63),
    2**64,
    10**40,
    MyInt(3),
    0.1,
    -0.0,
    5e-324,
    math.nan,
    math.inf,
    -math.inf,
    MyFloat(1.25),
    "",
    'quote " backslash \\ newline \n tab \t \x00 \x1f',
    "café 你好 \U0001f600",
    MyStr("sub"),
    [],
    {},
    (1, "a", None),
    [1, [2, [3, {}]]],
    {"a": 1, "b": [True, None], "c": {"d": "e"}, "f": (1.5,)},
    [{"id": index, "name": f"n{index}", "score": index / 4} for index in range(12)],
    [{f"f{index}": index for index in range(24)} for _ in range(3)],
    {f"w{index:02d}": index for index in range(30)},
    MyDict({"k": MyList([1, 2])}),
]


@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_no_unsupported_object_is_byte_identical_to_dumps(mode, entry):
    never = Counting()
    for document in FIXED_DOCUMENTS:
        assert entry(document, never, return_type=mode) == strata.dumps(document, return_type=mode)
    assert never.calls == 0


@pytest.mark.parametrize("mode", MODES)
def test_no_unsupported_object_is_byte_identical_on_a_fresh_thread(mode):
    never = Counting()

    def body():
        return [
            (
                strata.dumps_with_default(document, never, return_type=mode),
                strata.dumps(document, return_type=mode),
            )
            for document in FIXED_DOCUMENTS
        ]

    for hooked, plain in in_fresh_cache(body):
        assert hooked == plain
    assert never.calls == 0


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


#: JSON-native documents (no NaN/±Inf, whose spelling is a documented
#: divergence from the stdlib), so the stdlib is an exact oracle.
CORPUS = [_random_value(random.Random(SEED + index)) for index in range(300)]


@pytest.mark.parametrize("mode", MODES)
def test_no_unsupported_object_agrees_with_the_stdlib_over_a_corpus(mode):
    never = Counting()
    for document in CORPUS:
        out = strata.dumps_with_default(document, never, return_type=mode)
        assert json.loads(out) == json.loads(json.dumps(document))
    assert never.calls == 0

    def body():
        return [strata.dumps_with_default(doc, never, return_type=mode) for doc in CORPUS]

    for document, out in zip(CORPUS, in_fresh_cache(body), strict=True):
        assert json.loads(out) == json.loads(json.dumps(document))
    assert never.calls == 0


# ---------------------------------------------------------------------------
# Row 9: "an invalid `return_type` ⇒ ValueError("invalid return_type: %s"), as
# `dumps`."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("value", ("x", "STR", "", "Bytes"))
@pytest.mark.parametrize("entry", ENTRY_FNS, ids=ENTRY_IDS)
def test_an_invalid_return_type_is_a_value_error_as_in_dumps(entry, value):
    message = exact(f"invalid return_type: {value}")
    with pytest.raises(ValueError, match=message):
        strata.dumps([1], return_type=value)
    hook = Counting()
    with pytest.raises(ValueError, match=message):
        entry([Opaque()], hook, return_type=value)
    assert hook.calls == 0

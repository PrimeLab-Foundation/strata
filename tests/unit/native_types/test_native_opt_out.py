"""Contract tests for `dumps_with_default(..., native=...)`, the per-call opt-out.

api.md, dumps_with_default: "`native=False` opts out of that precedence, per
call" (docs/decisions.md, 2026-09-29, "`dumps_with_default` gains
`native=True`"). One named test per clause -- behaviour pins 1-4 of the
opt-out's specification -- each run on both entries, the facade
`strata.dumps_with_default` and the hook image's own
`strata._dumps_hook.dumps_with_default`, and in both return types. Nesting,
threads, failures and copied contexts (pins 5, 6, 8, 9) are in
tests/py/native_types/test_native_opt_out.py; the greenlet interleave (pin 7)
is in tests/integrations/test_native_opt_out_greenlet.py.
"""

import dataclasses
import datetime as dt
import enum
import json
import re
import uuid
from decimal import Decimal

import pytest

import strata
from strata import _dumps_hook

try:
    import numpy as np
except ImportError:  # numpy is not a gate dependency
    np = None

MODES = ("str", "bytes")
ENTRIES = [("facade", strata.dumps_with_default), ("native", _dumps_hook.dumps_with_default)]


class Color(enum.Enum):
    RED = "red"


class Units(enum.IntEnum):
    ONE = 1


class Label(str, enum.Enum):
    TAG = "tag"


@dataclasses.dataclass
class Point:
    x: int
    y: float


class Opaque:
    pass


# The native families of test_serializer_contract.py's NATIVES, one object
# each; test modules are not importable from one another under
# `--import-mode=importlib`, so the list is repeated, not imported.
NATIVES = [
    dt.datetime(2026, 1, 1, 1, tzinfo=dt.timezone.utc),
    dt.date(2026, 1, 1),
    dt.time(1, 2),
    uuid.UUID(int=7),
    Decimal("2.50"),
    Color.RED,
    Point(1, 2.0),
    {1, 2},
    frozenset({"a"}),
]
if np is not None:
    NATIVES += [np.int32(3), np.bool_(True), np.array([1, 2])]

NOT_BOOLS = [1, 0, None, "False", "True"]
if np is not None:
    NOT_BOOLS += [np.bool_(True), np.bool_(False)]


def text_of(out):
    return out.decode() if isinstance(out, bytes) else out


def placements(value):
    """`value` at the root, as a list element, as a dict value, and nested."""
    return [value, [value], {"a": value}, [{"a": [value]}]]


def recording(calls):
    def default(obj):
        calls.append(obj)
        return repr(obj)

    return default


def forbidden(obj):
    raise AssertionError(f"default was called for {obj!r}")


def oracle(document, default):
    """main 38eaa9f's `dumps_with_default` (M12b): stdlib `json`, compact."""
    return json.dumps(document, default=default, separators=(",", ":"), ensure_ascii=False)


def tp_name(value):
    """`Py_TYPE(value)->tp_name`, as `_strata`'s own unsupported-type error spells it."""
    try:
        strata.dumps([value])
    except TypeError as error:
        found = re.fullmatch(r"Object of type (.+) is not JSON serializable", str(error))
        assert found is not None, str(error)
        return found.group(1)
    return type(value).__name__


def same_objects(left, right):
    return len(left) == len(right) and all(a is b for a, b in zip(left, right, strict=True))


# ---------------------------------------------------------------------------
# Pin 1: `native=False` sends every native family to `default`, once, and the
# output is the stdlib oracle's.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("label", "entry"), ENTRIES)
@pytest.mark.parametrize("value", NATIVES, ids=lambda value: type(value).__name__)
def test_opt_out_passes_every_native_to_default_once_at_every_position(label, entry, mode, value):
    # api.md, dumps_with_default: under `native=False` "every native family goes
    # to `default`, once per object ... the output is main `38eaa9f`'s
    # `dumps_with_default` byte for byte" (docs/decisions.md, 2026-09-29).
    for document in placements(value):
        calls, oracle_calls = [], []
        expected = oracle(document, recording(oracle_calls))
        out = entry(document, recording(calls), return_type=mode, native=False)
        assert out == (expected.encode() if mode == "bytes" else expected)
        assert same_objects(calls, [value])
        assert same_objects(calls, oracle_calls)


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("label", "entry"), ENTRIES)
def test_opt_out_passes_every_native_of_one_document_in_document_order(label, entry, mode):
    # api.md, dumps_with_default: every native family goes to `default`, once
    # per object (docs/decisions.md, 2026-09-29) -- all families in one walk.
    document = {"items": list(NATIVES), "nested": [{"deep": NATIVES[0]}], "plain": [1, "x"]}
    calls, oracle_calls = [], []
    expected = oracle(document, recording(oracle_calls))
    out = entry(document, recording(calls), return_type=mode, native=False)
    assert text_of(out) == expected
    assert same_objects(calls, [*NATIVES, NATIVES[0]])
    assert same_objects(calls, oracle_calls)


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("label", "entry"), ENTRIES)
def test_opt_out_supports_exactly_what_dumps_supports(label, entry, mode):
    # api.md, dumps_with_default: under `native=False` "the supported set is
    # then `dumps(obj, native=False)`'s" -- an IntEnum or str-mixin Enum member
    # is an int or a str there, so `default` is not called for it.
    supported = [Units.ONE, Label.TAG, True, None, 2**70, 0.5, "é", [], {}]
    if np is not None:
        supported.append(np.float64(1.5))
    for value in supported:
        for document in placements(value):
            expected = strata.dumps(document, return_type=mode)
            assert entry(document, forbidden, return_type=mode, native=False) == expected
            assert text_of(expected) == oracle(document, forbidden)


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("label", "entry"), ENTRIES)
def test_opt_out_a_native_inside_a_returned_container_gets_its_own_call(label, entry, mode):
    # api.md, dumps_with_default: "Unsupported objects *nested inside* a
    # returned container are ordinary positions and get their own call" -- a
    # native among them, under `native=False`, is one of them.
    stamp = NATIVES[0]
    opaque = Opaque()
    calls = []

    def default(obj):
        calls.append(obj)
        return [obj.isoformat()] if obj is stamp else [stamp]

    out = entry([opaque], default, return_type=mode, native=False)
    assert text_of(out) == f'[[["{stamp.isoformat()}"]]]'
    assert same_objects(calls, [opaque, stamp])


# ---------------------------------------------------------------------------
# Pin 2: a native `default` returns under `native=False` is the chain bound.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("label", "entry"), ENTRIES)
@pytest.mark.parametrize("value", NATIVES, ids=lambda value: type(value).__name__)
def test_opt_out_a_native_default_returns_is_the_chain_bound_type_error(label, entry, mode, value):
    # api.md, dumps_with_default: under `native=False` "a native object
    # `default` returns is unsupported (the chain bound below)"; "Chain bound 1"
    # names the returned object's type, and `default` is not called on it.
    calls = []

    def default(obj):
        calls.append(obj)
        return value

    message = f"default() returned an object of type {tp_name(value)} that is not JSON serializable"
    opaque = Opaque()
    with pytest.raises(TypeError, match=f"^{re.escape(message)}$"):
        entry([1, opaque], default, return_type=mode, native=False)
    assert same_objects(calls, [opaque])


# ---------------------------------------------------------------------------
# Pin 3: `native=True` explicit is `native` omitted, byte for byte.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("label", "entry"), ENTRIES)
@pytest.mark.parametrize("value", NATIVES, ids=lambda value: type(value).__name__)
def test_native_true_explicit_is_native_omitted_and_never_calls_default(label, entry, mode, value):
    # api.md, dumps_with_default: `native=True` is the default and natives come
    # before the callable (docs/decisions.md, 2026-09-29: "`native=True`
    # (default): today's behaviour, unchanged").
    for document in placements(value):
        expected = strata.dumps(document, return_type=mode, native=True)
        assert entry(document, forbidden, return_type=mode, native=True) == expected
        assert entry(document, forbidden, return_type=mode) == expected


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("label", "entry"), ENTRIES)
def test_native_true_explicit_still_calls_default_for_a_non_native(label, entry, mode):
    # api.md, dumps_with_default: with natives first, `default` still receives
    # every object neither `dumps` nor the native table supports.
    opaque = Opaque()
    document = [*NATIVES, opaque]
    explicit_calls, omitted_calls = [], []
    explicit = entry(document, recording(explicit_calls), return_type=mode, native=True)
    omitted = entry(document, recording(omitted_calls), return_type=mode)
    assert explicit == omitted
    assert same_objects(explicit_calls, [opaque])
    assert same_objects(omitted_calls, [opaque])


# ---------------------------------------------------------------------------
# Pin 4: a `native` that is not exactly `True` or `False` is a `TypeError`.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("label", "entry"), ENTRIES)
@pytest.mark.parametrize("value", NOT_BOOLS, ids=repr)
def test_a_native_that_is_not_a_bool_is_a_type_error(label, entry, mode, value):
    # api.md, dumps_with_default: "`native` is a `bool`, tested by identity:
    # anything else raises `TypeError("native must be a bool, not %s")` before
    # any byte is produced and without calling `default`".
    message = f"^native must be a bool, not {re.escape(tp_name(value))}$"
    with pytest.raises(TypeError, match=message):
        entry([NATIVES[0], Opaque()], forbidden, return_type=mode, native=value)


@pytest.mark.parametrize(("label", "entry"), ENTRIES)
def test_the_native_type_error_comes_before_the_other_argument_checks(label, entry):
    # docs/decisions.md, 2026-09-29: the `native` type error is raised where
    # the keyword is read, before `default` is checked and before
    # `return_type`'s value is.
    message = "^native must be a bool, not int$"
    with pytest.raises(TypeError, match=message):
        entry([1], None, native=1)
    with pytest.raises(TypeError, match=message):
        entry([1], forbidden, return_type="xml", native=1)


def test_the_hook_entry_reads_native_before_it_misses_default():
    # docs/decisions.md, 2026-09-29: raised inside the keyword loop, so before
    # the hook entry's own missing-`default` check.
    with pytest.raises(TypeError, match="^native must be a bool, not NoneType$"):
        _dumps_hook.dumps_with_default([1], native=None)

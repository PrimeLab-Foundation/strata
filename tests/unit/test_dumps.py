"""Contract tests for `strata.dumps`.

Each test pins a clause of docs/context/api.md § Parse & serialize.
"""

import json
import math
import random
import sys
import warnings

import pytest

import strata

# ---------------------------------------------------------------------------
# "Compact serialization (no whitespace)"
# ---------------------------------------------------------------------------


def test_output_has_no_whitespace():
    assert strata.dumps({"a": [1, 2], "b": {}}) == '{"a":[1,2],"b":{}}'
    assert strata.dumps([]) == "[]"
    assert strata.dumps({}) == "{}"


def test_object_key_order_is_insertion_order():
    assert strata.dumps({"zebra": 1, "alpha": 2}) == '{"zebra":1,"alpha":2}'


# ---------------------------------------------------------------------------
# "Supports dict/list/tuple/str/int/float/bool/None"
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    ("value", "expected"),
    [
        (None, "null"),
        (True, "true"),
        (False, "false"),
        (0, "0"),
        (-7, "-7"),
        (2.5, "2.5"),
        (1.0, "1.0"),
        ("text", '"text"'),
        ([], "[]"),
        ((), "[]"),
        ({}, "{}"),
        ([1, "a", None], '[1,"a",null]'),
        ((1, "a", None), '[1,"a",null]'),
        ({"k": "v"}, '{"k":"v"}'),
    ],
)
def test_supported_types_serialize(value, expected):
    assert strata.dumps(value) == expected


def test_a_tuple_serializes_as_an_array():
    """Tuples are supported, and are indistinguishable from lists in JSON."""
    assert strata.dumps({"t": (1, 2)}) == strata.dumps({"t": [1, 2]})


# ---------------------------------------------------------------------------
# Scalar-run fast paths: homogeneous prefixes, and fallbacks mid-list.
# The runs re-check every element's type, so a mixed list must produce
# exactly what the general walk would.
# ---------------------------------------------------------------------------


def test_null_runs_and_their_fallbacks():
    assert strata.dumps([None] * 200) == "[" + ",".join(["null"] * 200) + "]"
    assert strata.dumps([None, None, 1, None]) == "[null,null,1,null]"
    assert strata.dumps([None, True, "x"]) == '[null,true,"x"]'


def test_float_runs_with_nonfinite_holes():
    data = [1.5, float("nan"), 2.25, float("inf"), -0.125, float("-inf")]
    assert strata.dumps(data) == "[1.5,null,2.25,null,-0.125,null]"


def test_schema_keys_wider_than_the_copy_window():
    # Prepared `"key":` spans past 16 bytes take the spanning path; repeated
    # records exercise the prepared-schema emit on both sides of that split.
    wide = "k" * 40
    records = [{wide: index, "s": "v", "m" * 17: index * 0.5} for index in range(8)]
    assert strata.dumps(records) == json.dumps(records, separators=(",", ":"))


def test_micro_decimal_gate_boundaries():
    values = [
        9.9999e-05,  # below the 1e-4 gate: scientific layout
        1e-4,
        0.100001,
        123456789.123456,
        3999999999.999999,  # just inside the 4e9 gate
        4.0e9,  # at the gate: general path
        4000000000.5,
        12345678901.25,  # above the gate
    ]
    out = strata.dumps(values)
    assert out == json.dumps(values, separators=(",", ":"))
    assert json.loads(out) == values


def _significant_digits(value: float) -> int:
    text = repr(abs(value)).split("e")[0]
    return len(text.replace(".", "").lstrip("0").rstrip("0")) or 1


def test_full_precision_floats_render_at_every_significant_width():
    """Shortest round-trip text, width by width, against the stdlib oracle.

    The digit layout writes the significand in eight-digit groups, head
    first, so the widths where the head is narrower than, exactly, and wider
    than a group (nine, sixteen and seventeen significant digits) are its
    seams. Every width from one to seventeen is exercised here, in the fixed
    layout and the scientific one, both signs, as a list, inside a dict and
    in bytes mode -- `json.dumps` is the documented oracle for float text
    (docs/context/styleguide.md).
    """
    rng = random.Random(20260905)
    values: list[float] = []
    for width in range(1, 18):
        low = 10 ** (width - 1) if width > 1 else 1
        for _ in range(40):
            digits = rng.randrange(low, 10**width)
            for exponent in (-width, -width + 2, -5, 0, 4, 17, -22):
                value = float(f"{digits}e{exponent}")
                if math.isfinite(value) and value != 0.0:
                    values.append(value)
                    values.append(-value)
    for _ in range(400):
        values.append(rng.random())  # 14-17 digits: the mixed.json float shape

    widths = {_significant_digits(value) for value in values}
    assert widths == set(range(1, 18)), sorted(widths)

    expected = json.dumps(values, separators=(",", ":"))
    assert strata.dumps(values) == expected
    assert strata.dumps(values, return_type="bytes") == expected.encode()
    record = {"floats": values}
    assert strata.dumps(record) == json.dumps(record, separators=(",", ":"))
    assert json.loads(strata.dumps(values)) == values


def test_bool_is_not_serialized_as_an_integer():
    """bool subclasses int, so order of type checks matters."""
    assert strata.dumps([True, False, 1, 0]) == "[true,false,1,0]"


# ---------------------------------------------------------------------------
# "dict keys must be `str` (else `TypeError`)"
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("key", [1, 2.5, None, True, (1, 2)])
def test_a_non_string_key_raises_type_error(key):
    with pytest.raises(TypeError, match="key"):
        strata.dumps({key: "value"})


# ---------------------------------------------------------------------------
# "Raises `TypeError` (unsupported type)"
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("value", [object(), {1, 2}, b"bytes", 1 + 2j, range(3)])
def test_an_unsupported_type_raises_type_error(value):
    with pytest.raises(TypeError, match="not JSON serializable"):
        strata.dumps(value)


def test_an_unsupported_type_nested_deep_still_raises():
    with pytest.raises(TypeError, match="not JSON serializable"):
        strata.dumps({"a": [{"b": object()}]})


# ---------------------------------------------------------------------------
# "NaN/±Inf serialize as `null`"
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("value", [math.nan, math.inf, -math.inf])
def test_non_finite_floats_serialize_as_null(value):
    assert strata.dumps(value) == "null"


def test_non_finite_floats_inside_containers_serialize_as_null():
    assert strata.dumps([1.5, math.nan, math.inf, -math.inf]) == "[1.5,null,null,null]"
    assert strata.dumps({"x": math.nan}) == '{"x":null}'


# ---------------------------------------------------------------------------
# "big ints beyond int64 are emitted via their str form"
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "value",
    [
        2**63 - 1,  # int64 max
        -(2**63),  # int64 min
        2**63,  # one past
        -(2**63) - 1,
        12345678901234567890,
        10**100,
        -(10**100),
    ],
)
def test_big_integers_keep_every_digit(value):
    assert strata.dumps(value) == str(value)
    assert strata.loads(strata.dumps(value)) == value


# ---------------------------------------------------------------------------
# 'Raises ValueError ("Maximum serialization depth exceeded" at
#  `sys.getrecursionlimit()`)'
# ---------------------------------------------------------------------------


def test_nesting_past_the_recursion_limit_raises_value_error():
    limit = sys.getrecursionlimit()
    deep = []
    node = deep
    for _ in range(limit + 10):
        child = []
        node.append(child)
        node = child

    with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
        strata.dumps(deep)


def test_nesting_within_the_limit_is_fine():
    depth = 100
    value = []
    node = value
    for _ in range(depth):
        child = []
        node.append(child)
        node = child
    assert strata.dumps(value).startswith("[[[")


# ---------------------------------------------------------------------------
# "return_type" and the shape of the result
# ---------------------------------------------------------------------------


def test_return_type_str_is_the_default():
    result = strata.dumps({"a": 1})
    assert isinstance(result, str)


def test_return_type_bytes_returns_bytes():
    result = strata.dumps({"a": 1}, return_type="bytes")
    assert isinstance(result, bytes)
    assert result == b'{"a":1}'


def test_an_unknown_return_type_raises_value_error():
    with pytest.raises(ValueError, match="return_type"):
        strata.dumps({}, return_type="nonsense")


# ---------------------------------------------------------------------------
# Text handling: escaping matches what the parser accepts back
# ---------------------------------------------------------------------------


def test_strings_are_escaped_per_rfc_8259():
    assert strata.dumps('quote"here') == r'"quote\"here"'
    assert strata.dumps("back\\slash") == r'"back\\slash"'
    assert strata.dumps("\b\f\n\r\t") == r'"\b\f\n\r\t"'
    assert strata.dumps("\x01") == r'"\u0001"'
    assert strata.dumps("a/b") == '"a/b"'  # solidus is not escaped


def test_non_ascii_is_emitted_as_utf8_not_escaped():
    assert strata.dumps("café") == '"café"'
    assert strata.dumps("\U0001f600") == '"\U0001f600"'
    assert strata.dumps("你好", return_type="bytes") == '"你好"'.encode()


def test_round_trip_through_loads():
    value = {"s": "café \n\t\"quoted\"", "n": [1, -2.5, 10**30], "b": [True, None]}
    assert strata.loads(strata.dumps(value)) == value
    assert json.loads(strata.dumps(value)) == value


# ---------------------------------------------------------------------------
# Schema-cache isolation across nesting (regression)
#
# The prepared-key cache is keyed by dict nesting level, counted explicitly.
# When it was keyed by the cycle stack's size, frame elision let an all-scalar
# child dict share its framed parent's slot: the child's cache update could
# rewrite the very blob the parent was mid-way through emitting — silent
# corruption with one way per slot, a crash once the ways rotated. These
# shapes are the distilled trigger.
# ---------------------------------------------------------------------------


def test_nested_scalar_dicts_do_not_disturb_parent_schema():
    # Parent has container values (framed); children are all-scalar dicts
    # (frameless) whose shapes repeat so their schemas prepare mid-parent.
    record = {
        "alpha": {"x": 1, "y": 2},
        "beta": {"x": 3, "y": 4},
        "gamma": {"x": 5, "y": 6},
        "tail": 7,
    }
    docs = [record] * 8
    assert strata.dumps(docs) == json.dumps(docs, separators=(",", ":"))
    assert (
        strata.dumps(docs, return_type="bytes") == json.dumps(docs, separators=(",", ":")).encode()
    )


def test_rotating_schemas_at_one_level_stay_isolated():
    # Several distinct shapes rotating at the same level, nested under a
    # framed parent — exercises way rotation while an outer emit is live.
    shapes = [
        {"kind": "a", "id": 1, "value": 2},
        {"type": "b", "payload": "x", "n": 3},
        {"label": "c", "items": [1, 2]},
        {"w": 1.5, "h": 2.5},
    ]
    doc = [{"wrap": shapes[i % 4], "seq": i} for i in range(64)]
    assert strata.dumps(doc) == json.dumps(doc, separators=(",", ":"))
    assert strata.dumps(doc, return_type="bytes") == json.dumps(doc, separators=(",", ":")).encode()


def _compact(obj):
    return json.dumps(obj, separators=(",", ":"), ensure_ascii=False).encode()


def _both_modes(doc):
    out = strata.dumps(doc, return_type="bytes")
    assert out == _compact(doc)
    assert strata.dumps(doc).encode() == out
    return out


def test_bytes_mode_block_sizing_boundaries():
    """api.md 'dumps ... return_type="bytes"': bytes mode sizes its output block
    to the previous document on the thread and stages the tail its last
    reservation cannot take; the output must not depend on that history.
    The sequence below walks the block's transitions relative to the document
    before: exact repeats (the tail staged and copied in), growth by a few
    bytes (the tail staged, then the block resized to the exact total), a
    spanning tail narrower than the stage (staged) and wider (the block grown
    directly), growth far beyond the stage, a shrink back, tiny documents
    (the block floor), scalar runs first in the document (the run writers'
    block-wide reservations), and an escaped tail (the scratch-and-span
    path)."""
    base = [{"id": index, "name": "n" * 20, "value": index * 1.5} for index in range(400)]
    for _ in range(3):
        _both_modes(base)
    for extra in (0, 1, 2, 40, 4000, 8100, 8200, 20000, 70000):
        _both_modes([*base, "x" * extra])
    _both_modes(base)
    _both_modes(base)
    for tiny in (123, "", [], {}, "ab", [1]):
        for _ in range(3):
            _both_modes(tiny)
    runs = [*range(1000, 1300), *(index * 0.25 for index in range(300)), *base]
    for _ in range(2):
        _both_modes(runs)
    escaped = [*base, 'quote"and\\backslash\n' * 300]
    for _ in range(2):
        _both_modes(escaped)
    _both_modes(base)


def test_str_mode_survives_a_nested_dumps_call():
    """api.md cycle_policy 'warn' and dumps of big ints: a nested dumps call
    from the cycle warning's handler or from an int subclass's __str__ must
    not disturb the outer call's output (str mode shares a per-thread buffer
    across calls)."""
    nested = []

    class Big(int):
        def __str__(self):
            nested.append(strata.dumps({"inner": [1, 2, 3]}))
            return int.__str__(self)

    big = Big(2**70)
    assert (
        strata.dumps([big, {"k": big}]) == "[1180591620717411303424,{\"k\":1180591620717411303424}]"
    )
    assert nested == ['{"inner":[1,2,3]}'] * 2

    cyclic = {"a": [1, 2]}
    cyclic["a"].append(cyclic)
    seen = []
    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = lambda *args, **kwargs: seen.append(strata.dumps({"w": "x" * 50}))
        out = strata.dumps(cyclic)
    assert out == '{"a":[1,2,null]}'
    assert seen == ['{"w":"' + "x" * 50 + '"}']

"""Contract tests for `strata.dumps`.

Each test pins a clause of docs/context/api.md § Parse & serialize.
"""

import json
import math
import sys

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

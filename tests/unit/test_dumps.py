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

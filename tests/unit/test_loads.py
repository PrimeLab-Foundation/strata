"""Contract tests for `strata.loads`.

Each test pins a clause of docs/context/api.md § Parse & serialize. The quoted
fragments are the contract; if one changes, api.md, this file and
docs/decisions.md move together.
"""

import json
import warnings

import pytest

import strata

# ---------------------------------------------------------------------------
# "Parse JSON text. Default returns the full Python tree
#  (dict|list|str|int|float|bool|None)"
# ---------------------------------------------------------------------------


def test_returns_the_full_python_tree():
    parsed = strata.loads('{"a": [1, 2.5, "s", true, false, null]}')
    assert parsed == {"a": [1, 2.5, "s", True, False, None]}


@pytest.mark.parametrize(
    ("text", "expected", "expected_type"),
    [
        ("null", None, type(None)),
        ("true", True, bool),
        ("false", False, bool),
        ("0", 0, int),
        ("-7", -7, int),
        ("2.5", 2.5, float),
        ('"text"', "text", str),
        ("[]", [], list),
        ("{}", {}, dict),
    ],
)
def test_every_json_type_maps_to_its_python_type(text, expected, expected_type):
    parsed = strata.loads(text)
    assert parsed == expected
    assert type(parsed) is expected_type


def test_a_number_without_a_fraction_is_an_int():
    """Integers stay integers; only a fraction or exponent makes a float."""
    assert type(strata.loads("1")) is int
    assert type(strata.loads("1.0")) is float
    assert type(strata.loads("1e2")) is float


def test_accepts_both_str_and_bytes():
    assert strata.loads('{"a":1}') == {"a": 1}
    assert strata.loads(b'{"a":1}') == {"a": 1}


# ---------------------------------------------------------------------------
# "integers parse exactly at any size (no double squashing; beyond int64 a
#  slow path builds the arbitrary-precision int - matches stdlib json; the
#  previous implementation mis-parsed 20+ digit ints, do not reproduce)"
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "literal",
    [
        "9223372036854775807",  # int64 max
        "-9223372036854775808",  # int64 min
        "9223372036854775808",  # one past int64 max
        "-9223372036854775809",  # one past int64 min
        "12345678901234567890",  # 20 digits: the previous implementation's bug
        "18446744073709551615",  # uint64 max
        "1" + "0" * 100,
        "-" + "9" * 200,
    ],
)
def test_integers_are_exact_at_any_size(literal):
    parsed = strata.loads(literal)
    assert type(parsed) is int
    assert parsed == int(literal)
    assert parsed == json.loads(literal)


def test_a_twenty_digit_integer_is_not_truncated_to_nineteen():
    """The exact defect api.md calls out: 19 digits accumulated, the rest dropped."""
    assert strata.loads("12345678901234567890") != 1234567890123456789


def test_big_integers_survive_nesting():
    parsed = strata.loads('{"big": [123456789012345678901234567890]}')
    assert parsed["big"][0] == 123456789012345678901234567890


# ---------------------------------------------------------------------------
# 'Invalid UTF-8 in `bytes` input => ValueError("Invalid JSON") - for bytes
#  the parser is the only validator'
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "payload",
    [
        b'"a\x80b"',  # continuation byte with no lead
        b'"\xc0\x80"',  # overlong NUL
        b'"\xff"',  # invalid lead byte
        b'"\xc2"',  # truncated two-byte sequence
        b'"\xed\xa0\x80"',  # UTF-8-encoded surrogate
        b'"\xf4\x90\x80\x80"',  # above U+10FFFF
    ],
)
def test_invalid_utf8_bytes_are_rejected(payload):
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.loads(payload)


def test_valid_utf8_bytes_are_accepted():
    assert strata.loads('"café"'.encode()) == "café"
    assert strata.loads('"\U0001f600"'.encode()) == "\U0001f600"


# ---------------------------------------------------------------------------
# 'Raises ValueError (invalid JSON / bad return_type), TypeError,
#  RuntimeError (internal C++ error)'
#
# and the error contract: 'Parse errors => ValueError("Invalid JSON")'
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "text",
    [
        "",
        "   ",
        "{invalid}",
        '{"a":}',
        "[1,]",
        "01",
        "1.",
        "nulll",
        "NaN",
        "Infinity",
        '"unterminated',
        "{} {}",
        "'single quoted'",
    ],
)
def test_parse_errors_raise_value_error_with_the_contract_message(text):
    with pytest.raises(ValueError, match="^Invalid JSON$") as caught:
        strata.loads(text)
    assert str(caught.value) == "Invalid JSON"


@pytest.mark.parametrize("source", [1, 1.5, None, [], {}, object()])
def test_a_non_text_source_raises_type_error(source):
    with pytest.raises(TypeError):
        strata.loads(source)


def test_an_unknown_return_type_raises_value_error():
    with pytest.raises(ValueError, match="return_type"):
        strata.loads("{}", return_type="nonsense")


# ---------------------------------------------------------------------------
# '`return_type="cursor"` returns a lazy `JsonCursor`'
# '`iterator=True`: dict root yields (key, value), list root yields elements
#  (eager parse, lazy consumption); scalar roots ignore the flag'
# ---------------------------------------------------------------------------


def test_cursor_mode_returns_a_cursor():
    cursor = strata.loads('{"a": {"b": [1, 2]}}', return_type="cursor")
    assert isinstance(cursor, strata.JsonCursor)
    assert cursor.is_object()
    assert cursor.field("a").field("b").at(1).get_int() == 2


def test_iterator_over_a_dict_root_yields_pairs():
    pairs = list(strata.loads('{"a": 1, "b": 2}', iterator=True))
    assert pairs == [("a", 1), ("b", 2)]


def test_iterator_over_a_list_root_yields_elements():
    assert list(strata.loads("[1, 2, 3]", iterator=True)) == [1, 2, 3]


@pytest.mark.parametrize(("text", "expected"), [("42", 42), ('"s"', "s"), ("null", None)])
def test_a_scalar_root_ignores_the_iterator_flag(text, expected):
    assert strata.loads(text, iterator=True) == expected


# ---------------------------------------------------------------------------
# 'RuntimeWarning under duplicate_key_policy="warn"' and the policy behaviours
# ---------------------------------------------------------------------------

DUPLICATES = '{"a": 1, "a": 2, "b": 3}'


def test_duplicate_keys_keep_the_first_by_default():
    assert strata.config.get("duplicate_key_policy") == "first"
    assert strata.loads(DUPLICATES) == {"a": 1, "b": 3}


def test_duplicate_key_policy_last_keeps_the_last():
    strata.config.set("duplicate_key_policy", "last")
    assert strata.loads(DUPLICATES) == {"a": 2, "b": 3}


def test_duplicate_key_policy_error_rejects_the_document():
    strata.config.set("duplicate_key_policy", "error")
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.loads(DUPLICATES)


def test_duplicate_key_policy_warn_emits_a_runtime_warning_and_keeps_the_first():
    strata.config.set("duplicate_key_policy", "warn")
    with pytest.warns(RuntimeWarning, match="Duplicate key"):
        parsed = strata.loads(DUPLICATES)
    assert parsed == {"a": 1, "b": 3}


def test_a_document_without_duplicates_warns_about_nothing():
    strata.config.set("duplicate_key_policy", "warn")
    with warnings.catch_warnings():
        warnings.simplefilter("error")
        assert strata.loads('{"a": 1, "b": 2}') == {"a": 1, "b": 2}


# ---------------------------------------------------------------------------
# Structure and text handling
# ---------------------------------------------------------------------------


def test_escapes_and_surrogate_pairs_decode():
    assert strata.loads(r'"tab\there"') == "tab\there"
    assert strata.loads(r'"\u0041"') == "A"
    assert strata.loads(r'"\u00e9"') == "\u00e9"
    assert strata.loads(r'"\ud83d\ude00"') == "\U0001f600"
    assert strata.loads(r'"\u0000"') == "\x00"
    assert strata.loads(r'"\b\f\n\r\t\/\\"') == "\b\f\n\r\t/\\"


def test_lone_surrogates_are_rejected():
    for text in (r'"\ud800"', r'"\udc00"', r'"\ud800A"'):
        with pytest.raises(ValueError, match="^Invalid JSON$"):
            strata.loads(text)


def test_object_key_order_is_preserved():
    parsed = strata.loads('{"zebra": 1, "alpha": 2, "middle": 3}')
    assert list(parsed) == ["zebra", "alpha", "middle"]


def test_deeply_nested_input_parses():
    depth = 100
    parsed = strata.loads("[" * depth + "1" + "]" * depth)
    for _ in range(depth):
        parsed = parsed[0]
    assert parsed == 1

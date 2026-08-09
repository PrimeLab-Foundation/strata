"""Integration tests: strata against stdlib `json` on a generated corpus.

The contract suites in `tests/unit` pin clauses one at a time. This file
exercises the same surface end to end and differently: whole values are pushed
through both libraries and compared, so a disagreement shows up even where no
clause anticipated it. stdlib `json` is the documented round-trip oracle
(docs/context/styleguide.md).

Where strata is deliberately stricter than stdlib, that is asserted here too,
so the divergences stay visible rather than being quietly tolerated.
"""

import json
import math
import random

import pytest

import strata

SEED = 20260809


def _scalars():
    return [
        None,
        True,
        False,
        0,
        1,
        -1,
        2**31,
        2**63 - 1,
        -(2**63),
        2**63,
        12345678901234567890,
        10**60,
        0.0,
        -0.0,
        0.1,
        -2.5,
        1e15,
        1e16,
        1e-4,
        1e-5,
        3.141592653589793,
        "",
        "plain",
        'quote " backslash \\ newline \n tab \t',
        "café 你好 \U0001f600",
        "\x00\x01\x1f",
    ]


def _random_value(rng, depth=0):
    roll = rng.random()
    if depth > 3 or roll < 0.4:
        return rng.choice(_scalars())
    if roll < 0.7:
        return [_random_value(rng, depth + 1) for _ in range(rng.randint(0, 5))]
    return {f"key{i}": _random_value(rng, depth + 1) for i in range(rng.randint(0, 5))}


def _corpus(count):
    rng = random.Random(SEED)
    return [_random_value(rng) for _ in range(count)]


CORPUS = _corpus(400)


@pytest.mark.parametrize("value", CORPUS, ids=range(len(CORPUS)))
def test_strata_parses_what_stdlib_produces(value):
    assert strata.loads(json.dumps(value)) == value
    assert strata.loads(json.dumps(value, ensure_ascii=False)) == value
    assert strata.loads(json.dumps(value).encode()) == value


@pytest.mark.parametrize("value", CORPUS, ids=range(len(CORPUS)))
def test_stdlib_parses_what_strata_produces(value):
    assert json.loads(strata.dumps(value)) == value
    assert json.loads(strata.dumps(value, return_type="bytes")) == value


@pytest.mark.parametrize("value", CORPUS, ids=range(len(CORPUS)))
def test_strata_round_trips_its_own_output(value):
    once = strata.dumps(value)
    assert strata.loads(once) == value
    assert strata.dumps(strata.loads(once)) == once  # stable


def test_number_types_survive_the_round_trip():
    """An int stays an int and a float stays a float, at every magnitude."""
    for value in (0, 1, -1, 2**63, 10**80):
        assert type(strata.loads(strata.dumps(value))) is int
    for value in (0.0, 1.0, -2.5, 1e300, 5e-324):
        assert type(strata.loads(strata.dumps(value))) is float


def test_float_text_matches_stdlib_exactly():
    """strata renders floats the way `repr` does, so output matches json.dumps."""
    rng = random.Random(SEED)
    values = [rng.uniform(-1e6, 1e6) for _ in range(500)]
    values += [0.0, -0.0, 0.1, 1e15, 1e16, 1e-4, 1e-5, 5e-324, 1.7976931348623157e308]
    for value in values:
        assert strata.dumps(value) == json.dumps(value)


# ---------------------------------------------------------------------------
# Rejection parity, and the documented exceptions to it
# ---------------------------------------------------------------------------

MALFORMED = [
    "",
    "{",
    "[",
    "{'a': 1}",
    '{"a": 1,}',
    "[1, 2,]",
    "01",
    "+1",
    "1.",
    ".5",
    "1e",
    "nulll",
    "tru",
    '{"a" 1}',
    '"unterminated',
    "{} {}",
    "undefined",
]


@pytest.mark.parametrize("text", MALFORMED)
def test_both_libraries_reject_malformed_documents(text):
    with pytest.raises(ValueError):
        json.loads(text)
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.loads(text)


@pytest.mark.parametrize("text", ["NaN", "Infinity", "-Infinity"])
def test_strata_rejects_the_stdlib_non_finite_extension(text):
    """stdlib accepts these; they are not JSON, so strata does not."""
    assert math.isnan(json.loads(text)) or math.isinf(json.loads(text))
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.loads(text)


def test_strata_rejects_lone_surrogates_that_stdlib_accepts():
    assert json.loads(r'"\ud800"') == "\ud800"
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.loads(r'"\ud800"')


def test_strata_rejects_a_byte_order_mark_that_stdlib_strips():
    assert json.loads(b"\xef\xbb\xbf{}") == {}
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.loads(b"\xef\xbb\xbf{}")


def test_strata_rejects_encoded_surrogates_that_stdlib_passes_through():
    """stdlib decodes bytes with the "surrogatepass" handler; strata is strict."""
    assert json.loads(b'"\xed\xa0\x80"') == "\ud800"
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.loads(b'"\xed\xa0\x80"')


# ---------------------------------------------------------------------------
# Larger documents
# ---------------------------------------------------------------------------


def test_a_realistic_document_round_trips():
    document = {
        "users": [
            {
                "id": index,
                "name": f"user-{index}",
                "score": index * 1.5,
                "active": index % 2 == 0,
                "tags": ["a", "b"] if index % 3 else [],
                "meta": None if index % 5 else {"nested": {"deep": [1, 2, 3]}},
            }
            for index in range(200)
        ],
        "total": 200,
        "cursor": None,
    }
    text = strata.dumps(document)
    assert strata.loads(text) == document
    assert json.loads(text) == document
    assert text == json.dumps(document, separators=(",", ":"))


def test_a_wide_array_round_trips():
    values = list(range(10000))
    assert strata.loads(strata.dumps(values)) == values


def test_bytes_and_str_input_agree():
    text = json.dumps({"a": [1, "é", None]})
    assert strata.loads(text) == strata.loads(text.encode())

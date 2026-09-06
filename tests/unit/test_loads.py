"""Contract tests for `strata.loads`.

Each test pins a clause of docs/context/api.md § Parse & serialize. The quoted
fragments are the contract; if one changes, api.md, this file and
docs/decisions.md move together.
"""

import gc
import json
import math
import random
import sys
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


# The parser's short-number head resolves [-]d{1..7}[.d{1..7}] without the
# full scanner; these pin its boundaries against stdlib json: seven versus
# eight digits on either side of the point, the lone-zero forms, a number in
# the last bytes of the input (where the head cannot run), and the shapes
# that must fall through to the scanner unchanged.
@pytest.mark.parametrize(
    "literal",
    [
        "1234567",
        "12345678",
        "-1234567",
        "-12345678",
        "0",
        "-0",
        "0.5",
        "-0.0",
        "0.000123",
        "1234567.1234567",
        "1234567.12345678",
        "12345678.1234567",
        "9999999.9999999",
        "1.5e3",
        "1.5E-3",
        "1e5",
        "0.6394267984578837",
        "0.84442185152504811",
        "123.4567890123456",
        "-0.0000000000000001",
        "9007199254740993.5",
        "0.30000000000000004",
        "123456789012345678",
        "1000000",
        "10.05",
        "0.0100",
    ],
)
def test_short_numbers_match_stdlib_at_the_head_boundaries(literal):
    # The head needs eight readable bytes past a run's first digit, so the
    # long-tailed wrappers are the ones that exercise it at every width; the
    # bare and short-tailed forms pin the scanner path for the same literal.
    for text in (
        literal,
        f"[{literal}]",
        f'{{"k": {literal}}}',
        f"[{literal}, 1]",
        f"[{literal}, 123456789012]",
        f'{{"k": {literal}, "padding": 0}}',
    ):
        parsed = strata.loads(text)
        expected = json.loads(text)
        assert parsed == expected
        leaf = parsed
        reference = expected
        while isinstance(leaf, (list, dict)):
            leaf = leaf[0] if isinstance(leaf, list) else leaf["k"]
            reference = reference[0] if isinstance(reference, list) else reference["k"]
        assert type(leaf) is type(reference)
        if isinstance(leaf, float):
            assert math.copysign(1.0, leaf) == math.copysign(1.0, reference)


@pytest.mark.parametrize("literal", ["01", "-01", "00", "1.", ".5", "1e", "1.5e+", "-", "0x1"])
def test_malformed_short_numbers_are_rejected_at_every_input_distance(literal):
    for text in (literal, f"[{literal}]", f"[{literal}, 12345678901]", f"[{literal}, 1]"):
        with pytest.raises(ValueError, match="Invalid JSON"):
            strata.loads(text)


# The prediction table's per-frame cursor points into a way's entries; every
# path that moves those entries while an object is open (recording keys one
# by one on a wide record's first sighting, dropping a diverged tail, way
# replacement, retiring a churning depth) must refresh it. These documents
# drive each of those paths with predictions in flight and check the result
# against stdlib json.
def _rotating_records(shapes, count):
    return [dict(shapes[index % len(shapes)], n=index) for index in range(count)]


@pytest.mark.parametrize(
    "records",
    [
        # A wide record seen twice: the way's storage grows key by key while
        # the first record is open, then predicts every key of the second.
        [{f"key_{index}": index for index in range(64)}] * 2,
        # Wider than the recorder keeps (64 keys): the tail is never recorded.
        [{f"key_{index}": index for index in range(80)}] * 3,
        # Four shapes rotating at one depth: every way is adopted in turn.
        _rotating_records([{"a": 1}, {"b": 2, "c": 3}, {"d": [1, 2]}, {"e": {"f": 4}}], 40),
        # More shapes than ways, and shapes that share a first key and
        # diverge later: replacement and tail-dropping with cursors live.
        _rotating_records(
            [
                {"id": 1, "x": 1},
                {"id": 2, "y": 2},
                {"id": 3, "x": 3, "z": 3},
                {"id": 4},
                {"q": 5},
                {"r": 6, "id": 6},
            ],
            60,
        ),
        # A depth that never repeats a shape: it retires mid-document.
        [{f"only_{index}": index, "shared": index} for index in range(40)],
        # Nested records so several depths hold cursors at once.
        [
            {"outer": {"inner": {"leaf": index, "list": [{"k": index}] * 3}}, "n": index}
            for index in range(30)
        ],
    ],
)
def test_prediction_cursors_survive_every_way_mutation(records):
    text = json.dumps(records)
    assert strata.loads(text) == json.loads(text)
    assert strata.loads(text.encode()) == json.loads(text)
    # And under the other policies, which take the duplicate-aware inserts.
    for policy in ("last", "error", "warn"):
        strata.config.set("duplicate_key_policy", policy)
        assert strata.loads(text) == json.loads(text)


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
        b'{"\xff": 1}',  # inside a key
        b'{"k": "a\\n\xff"}',  # inside a string that also carries an escape
        b'[1, \xff]',  # outside any string
        b'["ok", "caf\xc3\xa9", "\xc3"]',  # after valid non-ASCII strings
        b'[' + b'"x",' * 3000 + b'"\x80"]',  # deep inside a large document
        b'"\xe2\x82"',  # truncated three-byte sequence at the end
    ],
)
def test_invalid_utf8_bytes_are_rejected(payload):
    # api.md: for bytes the parser is the only validator. The builder catches
    # a bad sequence where it lies -- in the string that carries it -- and the
    # error is the contract's, never the codec's.
    with pytest.raises(ValueError, match="^Invalid JSON$") as caught:
        strata.loads(payload)
    assert not isinstance(caught.value, UnicodeDecodeError)


def test_invalid_utf8_in_a_file_is_rejected(tmp_path):
    path = tmp_path / "bad.json"
    path.write_bytes(b'{"k": ["fine", "\xff"]}')
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.load(path)
    path.write_bytes(b'{"k": "caf\xc3\xa9"}')
    assert strata.load(path) == {"k": "café"}


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


# ---------------------------------------------------------------------------
# Numbers: the long-fraction step of the number head
# ---------------------------------------------------------------------------


def test_long_fraction_floats_match_stdlib():
    """api.md: floats parse to the correctly rounded double.

    The number head settles fractions of 8..19 digits from up to three words
    (16..19 behind a lone zero only) and hands the rest to the full scanner;
    every width across both bounds, either sign, at every position a number
    can sit in a document -- including the near-end positions where the
    head declines for want of readable bytes -- must give stdlib's bits.
    """
    rng = random.Random(20260904)
    texts = []
    for _ in range(400):
        width = rng.randint(1, 24)
        int_part = rng.choice(["0", "0", str(rng.randint(1, 9999999)), str(rng.randint(1, 10**15))])
        digits = "".join(rng.choice("0123456789") for _ in range(width))
        if rng.random() < 0.2:
            digits = digits[:-1] + "0"
        texts.append(("-" if rng.random() < 0.5 else "") + int_part + "." + digits)
    texts += [
        "0.9007199254740993",
        "0.90071992547409930",
        "1.0000000000000002",
        "0.30000000000000004",
        "2.2250738585072014",
        "1.7976931348623157",
        "0.1000000000000000055",
        "0.00000000000000001",
        "0.1234567890123456789",
        "0.12345678901234567890123",
        "-0.00000000",
        "0.000000000000000000",
        "1234567.123456789012",
        "1234567.1234567890123",
        "0.6390313938546974",
        "-0.99999999999999999",
        "0.49999999999999999",
    ]
    for text in texts:
        forms = (
            text,
            f"[{text}]",
            f"[{text},0]",
            f'{{"k":{text}}}',
            f"[{text},0,0,0,0,0,0,0,0,0,0,0,0]",
            f"[{text}e2,0,0,0,0,0,0,0,0,0,0]",
            f"[{text}E-3,0,0,0,0,0,0,0,0,0,0]",
        )
        for doc in forms:
            expected = json.loads(doc)
            got = strata.loads(doc)
            assert got == expected, doc
            # Bit identity: the shortest round-trip text of a finite double is
            # unique, and -0.0 keeps its sign in it.
            assert json.dumps(got) == json.dumps(expected), doc
            assert strata.loads(doc.encode()) == expected, doc


def test_long_fraction_floats_reject_malformed_neighbours():
    """api.md: invalid JSON is a ValueError -- the head's declines included."""
    for doc in (
        "[0.12345678e,0,0,0,0,0,0,0,0,0,0]",
        "[0.1234567890123456.5,0,0,0,0,0,0,0]",
        "[0.12345678901234567x,0,0,0,0,0,0]",
        "[00.12345678901234567,0,0,0,0,0,0]",
        "[0.,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]",
        "[-.12345678901234567,0,0,0,0,0,0,0]",
        "[0.12345678901234567",
    ):
        with pytest.raises(ValueError, match="^Invalid JSON$"):
            strata.loads(doc)


def test_singletons_keep_their_reference_counts():
    """api.md: null/true/false parse to None/True/False.

    The builder places the interpreter's singletons without taking a
    reference on interpreters where they are immortal, and with one where
    they are not; either way the count observable from Python must be the
    same after the parsed trees are gone as before -- the invariant that
    keeps a mortal singleton alive on CPython 3.10 and 3.11.
    """
    doc = "[" + ",".join(("null", "true", "false") * 2000) + "]"

    def parse_and_check():
        # A helper frame: pytest's assertion rewriting binds its temporaries
        # to None once an assert has run, and on an interpreter where None is
        # mortal those bindings would count in the caller's measurement.
        parsed = strata.loads(doc)
        assert parsed[:3] == [None, True, False]
        assert parsed[0] is None and parsed[1] is True and parsed[2] is False
        nested = strata.loads('{"a": null, "b": [true, {"c": false}]}')
        assert nested == {"a": None, "b": [True, {"c": False}]}

    def counts():
        return (sys.getrefcount(None), sys.getrefcount(True), sys.getrefcount(False))

    parse_and_check()  # warm every lazily created structure before measuring
    before = counts()
    for _ in range(5):
        parse_and_check()
    assert counts() == before


# ---------------------------------------------------------------------------
# Arrays: the builder fills each list in place, presized from its depth
# ---------------------------------------------------------------------------


def test_arrays_of_every_size_at_one_depth_round_trip():
    """api.md: a JSON array is a list of its elements, in order.

    The builder presizes an array from the last array it closed at the same
    depth and appends past that hint, so sizes that grow, shrink and jump at
    one depth -- and empty arrays after long ones -- must all come back exact,
    with no null tail and no element lost.
    """
    sizes = [0, 1, 5, 300, 2, 0, 17, 1000, 3, 256, 257, 255, 1, 0, 64, 65, 63, 4000, 1]
    docs = ["[" + ",".join(str(k) for k in range(n)) + "]" for n in sizes]
    for doc, n in zip(docs, sizes, strict=True):
        parsed = strata.loads(doc)
        assert parsed == list(range(n)), n
        assert type(parsed) is list
    # Every size inside one document, at one depth, in one parse.
    doc = "[" + ",".join(d for d in docs) + "]"
    assert strata.loads(doc) == [list(range(n)) for n in sizes]
    # Nested: each depth keeps its own memory.
    doc = "[[[1,2,3],[4]],[[5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25]],[[]],[]]"
    assert strata.loads(doc) == json.loads(doc)
    deep = json.dumps([[list(range(n)) for n in (40, 1, 0, 300)] for _ in range(6)])
    assert strata.loads(deep) == json.loads(deep)


def test_short_arrays_after_long_ones_keep_no_tail():
    """A short array closed at a depth that last held a long one is rebuilt
    exactly: its allocation must not carry the long array's tail."""
    doc = "[" + ",".join(str(k) for k in range(2000)) + "],[1,2,3]"
    parsed = strata.loads("[" + doc + "]")
    assert parsed[1] == [1, 2, 3]
    assert sys.getsizeof(parsed[1]) <= sys.getsizeof([1, 2, 3]) + 8 * 8
    again = strata.loads("[[1,2,3]]")
    assert sys.getsizeof(again[0]) <= sys.getsizeof([1, 2, 3]) + 8 * 8


def test_arrays_abort_cleanly_after_elements():
    """api.md: invalid JSON is a ValueError -- with any partly built array
    released, leaving the interpreter's singletons' counts untouched."""
    before = (sys.getrefcount(None), sys.getrefcount(True))
    for doc in (
        "[1,2,3,",
        "[1,2,3,x]",
        "[" + ",".join(["null", "true"] * 300) + ",",
        "[[1,2],[3,4",
        "[[[[1]]],[2],",
        '{"k":[1,2,3,null,true,',
        "[" + ",".join(str(k) for k in range(500)) + "]]",
    ):
        with pytest.raises(ValueError, match="^Invalid JSON$"):
            strata.loads(doc)
    after = (sys.getrefcount(None), sys.getrefcount(True))
    assert after == before


# ---------------------------------------------------------------------------
# Strings: the builder's speculative compact-ASCII construction
# ---------------------------------------------------------------------------


def test_strings_of_every_width_and_every_high_byte_position_decode():
    """api.md: strings decode to str.

    The builder copies a string's bytes into a compact ASCII object while it
    checks them, a word at a time with an overlapping last word, and hands
    the string to the decoder the moment a high byte shows -- turning the
    speculation off for the rest of the document. Every width across the
    word boundaries, a non-ASCII character at every position, ASCII strings
    before and after a non-ASCII one in one document, escapes, and strings at
    the very end of the input must all equal stdlib's.
    """
    alphabet = "abcdefghijklmnopqrstuvwxyz0123456789"
    for width in range(0, 41):
        text = (alphabet * 2)[:width]
        doc = json.dumps(text)
        assert strata.loads(doc) == text
        assert strata.loads(doc.encode()) == text
        assert strata.loads("[" + doc + "]") == [text]
        for position in range(width):
            mixed = text[:position] + "\u00e9" + text[position + 1 :]
            doc = json.dumps(mixed, ensure_ascii=False)
            assert strata.loads(doc) == mixed, (width, position)
            assert strata.loads(doc.encode()) == mixed, (width, position)
            wide = text[:position] + "\U0001f600" + text[position + 1 :]
            doc = json.dumps(wide, ensure_ascii=False)
            assert strata.loads(doc) == wide, (width, position)
    # A document that turns the speculation off part-way keeps decoding
    # ASCII and non-ASCII strings alike, escaped or not.
    values = ["plain", "caf\u00e9", "after", "z\u00fcrich", "tail-ascii-string", "x", "", "\u00e9"]
    doc = json.dumps(values, ensure_ascii=False)
    assert strata.loads(doc) == values
    doc = json.dumps(values, ensure_ascii=True)
    assert strata.loads(doc) == values
    doc = json.dumps({"k" + str(i): v for i, v in enumerate(values)}, ensure_ascii=False)
    assert strata.loads(doc) == json.loads(doc)
    # Escapes decode through the scratch buffer, whatever the width.
    for width in range(1, 20):
        text = "a" * width + "\n" + "b" * width
        assert strata.loads(json.dumps(text)) == text


def test_discarded_speculative_strings_do_not_leak():
    """api.md: strings decode to str -- and the builder's speculation leaves nothing behind.

    The first string with a high byte in a document is allocated as a
    compact ASCII object, then discarded for the decoder (and the document's
    remaining strings skip the speculation), so the discard happens once per
    document and must release the object. The interpreter's allocated block
    count is the metric: a leaked object per parse shows as one block per
    parse, whatever the allocator's slack.
    """
    import gc
    import sys

    docs = [
        json.dumps(["caf\u00e9-" + str(i) for i in range(200)], ensure_ascii=False),
        json.dumps({"k" + str(i): "z\u00fcrich-" + str(i) for i in range(200)}, ensure_ascii=False),
        json.dumps(["\U0001f600" + "a" * (i % 30) for i in range(200)], ensure_ascii=False),
    ]
    encoded = [doc.encode() for doc in docs]
    for _ in range(50):  # warm every path the parse can take, then measure
        for doc, raw in zip(docs, encoded, strict=True):
            strata.loads(doc)
            strata.loads(raw)
    gc.collect()
    before = sys.getallocatedblocks()
    parses = 0
    for _ in range(300):
        for doc, raw in zip(docs, encoded, strict=True):
            strata.loads(doc)
            strata.loads(raw)
            parses += 2
    gc.collect()
    after = sys.getallocatedblocks()
    # One discard per parse: a leak shows as about `parses` blocks (1,800).
    assert after - before < parses // 10, (before, after, parses)


def test_shape_churn_across_inputs_keeps_results_exact_and_leaks_nothing():
    """api.md: the tree is the same whatever the process parsed before.

    The builder's key predictor learns record shapes per depth and retires a
    depth whose objects keep disagreeing; the retirement is re-armed at every
    input. Documents that thrash a depth (many shapes at one level) alternate
    with uniform ones and with documents that share a prefix of keys, across
    text and bytes input, and every result must equal stdlib's; the allocated
    block count over the cycle stays flat, so releasing and re-learning the
    prediction ways leaks nothing.
    """
    import gc
    import sys

    rng = random.Random(2026)
    shapes = [
        lambda i: {"kind": "k", "id": i, "value": f"v{i}"},
        lambda i: {"type": "t", "payload": {"a": 1, "b": 2}, "n": i},
        lambda i: {"label": "w", "items": [1, 2, 3, 4, 5], "i": i},
        lambda i: {"uuid": "0123456789abcdef" * 2, "meta": {"x": 1}, "active": True, "i": i},
    ]
    thrash = [
        {f"key{rng.randrange(40)}": i, f"other{rng.randrange(40)}": "s", "z": i} for i in range(400)
    ]
    uniform = [{"name": f"n{i}", "age": i % 90, "email": f"e{i}@x.y"} for i in range(400)]
    prefixed = [
        {"name": f"n{i}", "age": i, **({"extra": i} if i % 3 else {}), "tail": [i]}
        for i in range(400)
    ]
    rotating = [shapes[i % 4](i) for i in range(400)]
    docs = [json.dumps(d) for d in (thrash, uniform, prefixed, rotating, thrash, rotating)]
    expected = [json.loads(d) for d in docs]
    for _ in range(3):
        for doc, want in zip(docs, expected, strict=True):
            assert strata.loads(doc) == want
            assert strata.loads(doc.encode()) == want
    gc.collect()
    before = sys.getallocatedblocks()
    for _ in range(40):
        for doc, want in zip(docs, expected, strict=True):
            assert strata.loads(doc) == want
    gc.collect()
    after = sys.getallocatedblocks()
    assert after - before < 200, (before, after)


# ---------------------------------------------------------------------------
# The value cursor: an array's element loop borrows the builder's staging
# range for the length of the loop (docs/architecture/value-cursor.md). The
# clauses below are unchanged -- these pin the ownership the borrow must not
# break: every element staged through the cursor is released on every exit,
# at every depth, and a re-entrant parse never sees a half-published range.
# ---------------------------------------------------------------------------


def _aborting_documents():
    """Documents that fail at every position an array element loop can fail
    at: after an element, after a comma, inside a nested container, at each
    of several depths, and past a staging block that has been grown."""
    docs = []
    for width in (1, 2, 3, 63, 64, 65, 300, 2000):
        body = ",".join(str(k) for k in range(width))
        docs += [
            "[" + body,
            "[" + body + ",",
            "[" + body + ",]",
            "[" + body + " ",
            "[" + body + "}",
            "[" + body + ",x]",
            "[[" + body + "],",
            "[[" + body + "],[1,2",
            '{"k":[' + body + ",",
            "[" + body + ",[1,[2,[3,",
            "[" + body + ',{"a":1},',
            "[" + body + ',{"a":',
            "[" + body + ',"unterminated',
            "[" + body + ",01]",
            "[" + body + ",1.2.3]",
            "[" + body + ",tru]",
        ]
    # Deep nesting: a cursor is opened, closed and re-opened at every level,
    # and the failure lands at the innermost one.
    docs.append("[1," * 80 + "2")
    docs.append("[1," * 80 + "2" + "]" * 40)
    return docs


def test_arrays_release_every_staged_element_on_every_abort():
    """api.md: invalid JSON is a ValueError.

    Elements already stored through the borrowed range belong to the builder
    the moment the parse gives up, so an aborted parse must free exactly what
    it built -- at every width, including widths past the staging block's
    growth boundary, and at every nesting depth.
    """
    docs = _aborting_documents()
    for doc in docs:
        with pytest.raises(ValueError, match="^Invalid JSON$"):
            strata.loads(doc)
    gc.collect()
    before = sys.getallocatedblocks()
    for _ in range(5):
        for doc in docs:
            with pytest.raises(ValueError, match="^Invalid JSON$"):
                strata.loads(doc)
    gc.collect()
    assert sys.getallocatedblocks() - before < 200


def test_aborts_inside_arrays_keep_the_singletons_counts():
    """The staged singletons are released too: their counts, the one thing a
    Python-level test can read directly, must survive an abort at any depth."""
    docs = ["[" + ",".join(["null", "true", "false"] * n) + "," for n in (1, 20, 300, 2000)] + [
        "[null,[true,[false,[null,",
        '{"a":[null,true,false,',
        "[" + ",".join(["null"] * 500) + ",[true,false",
    ]
    for doc in docs:  # warm every lazily created structure before measuring
        with pytest.raises(ValueError, match="^Invalid JSON$"):
            strata.loads(doc)
    before = (sys.getrefcount(None), sys.getrefcount(True), sys.getrefcount(False))
    for _ in range(5):
        for doc in docs:
            with pytest.raises(ValueError, match="^Invalid JSON$"):
                strata.loads(doc)
    assert (sys.getrefcount(None), sys.getrefcount(True), sys.getrefcount(False)) == before


def test_nested_containers_inside_arrays_match_the_stdlib_oracle():
    """Every nested container closes the borrowed range and re-opens it after
    the nested parse; the tree must be identical to stdlib json's on shapes
    that alternate containers and scalars at many depths."""
    rng = random.Random(20260905)

    def build(depth):
        if depth == 0:
            return rng.choice([None, True, False, 0, -1, 17, 2**70, 1.5, "s", ""])
        kind = rng.random()
        if kind < 0.45:
            return [build(depth - 1) for _ in range(rng.randrange(0, 12))]
        if kind < 0.8:
            return {f"k{i}": build(depth - 1) for i in range(rng.randrange(0, 6))}
        return build(0)

    for _ in range(300):
        doc = json.dumps([build(4) for _ in range(rng.randrange(0, 20))])
        assert strata.loads(doc) == json.loads(doc)
        assert strata.loads(doc.encode()) == json.loads(doc)
    # Arrays whose elements are all containers: the range is closed and
    # re-opened on every single element.
    doc = json.dumps([[i] for i in range(3000)])
    assert strata.loads(doc) == json.loads(doc)
    doc = json.dumps([{"i": i} for i in range(3000)])
    assert strata.loads(doc) == json.loads(doc)


def test_reentrant_loads_from_a_duplicate_key_warning_builds_both_trees():
    """A warning filter runs arbitrary Python in the middle of a parse, and
    can call loads() again. The nested call leases its own builder, so the
    outer array's staging range -- published before the object that warns was
    entered -- is untouched by it, and both trees come back whole.
    """
    outer = "[1,2,3," + ",".join(str(k) for k in range(400)) + ',{"d":1,"d":2},9]'
    inner = "[" + ",".join(str(k) for k in range(500)) + "]"
    seen = []

    previous = strata.config.get("duplicate_key_policy")
    try:
        strata.config.set("duplicate_key_policy", "warn")
        with warnings.catch_warnings():
            warnings.simplefilter("always")

            def reentrant_show(*_warning):  # noqa: ANN002 - warnings.showwarning
                # Runs inside the outer parse, from the duplicate-key warning.
                seen.append(strata.loads(inner))

            warnings.showwarning = reentrant_show
            parsed = strata.loads(outer)
    finally:
        strata.config.set("duplicate_key_policy", previous)

    assert parsed[:3] == [1, 2, 3]
    assert parsed[-1] == 9
    assert parsed[-2] == {"d": 1}
    assert len(parsed) == 405
    assert seen and all(tree == list(range(500)) for tree in seen)


# ---------------------------------------------------------------------------
# "Nesting deeper than 1024 containers raises
#  ValueError('Maximum nesting depth exceeded')" -- api.md § Parse & serialize
# and § Error contract. The parser recurses, so this is the line between an
# error and a dead process; the number is pinned here on purpose, and moves
# only with api.md and strata::kMaxNestingDepth.
# ---------------------------------------------------------------------------

MAX_NESTING_DEPTH = 1024
DEPTH_MESSAGE = "^Maximum nesting depth exceeded$"


def nested_document(depth, shape="array"):
    """`depth` open containers around a scalar, as text.

    "mixed" alternates object and array so that every level is one container,
    which is what the cap counts.
    """
    if shape == "array":
        return "[" * depth + "1" + "]" * depth
    if shape == "object":
        return '{"a":' * depth + "1" + "}" * depth
    opens = ['{"a":' if level % 2 == 0 else "[" for level in range(depth)]
    closes = ["}" if level % 2 == 0 else "]" for level in range(depth)]
    return "".join(opens) + "1" + "".join(reversed(closes))


def depth_of(value):
    """Walk a parsed tree iteratively -- recursion here would be the bug."""
    depth = 0
    while isinstance(value, (list, dict)):
        depth += 1
        value = value[0] if isinstance(value, list) else next(iter(value.values()))
    return depth


@pytest.mark.parametrize("shape", ["array", "object", "mixed"])
def test_nesting_at_the_limit_parses(shape):
    parsed = strata.loads(nested_document(MAX_NESTING_DEPTH, shape))
    assert depth_of(parsed) == MAX_NESTING_DEPTH


@pytest.mark.parametrize("shape", ["array", "object", "mixed"])
def test_nesting_past_the_limit_raises_the_pinned_message(shape):
    text = nested_document(MAX_NESTING_DEPTH + 1, shape)
    with pytest.raises(ValueError, match=DEPTH_MESSAGE):
        strata.loads(text)
    # bytes input takes the same parser and the same refusal.
    with pytest.raises(ValueError, match=DEPTH_MESSAGE):
        strata.loads(text.encode())


def test_the_refusal_survives_far_past_the_limit():
    """Far past the cap the parse must still return an error rather than run
    out of C stack: the whole reason the cap exists."""
    for depth in (MAX_NESTING_DEPTH + 1, 10_000, 200_000):
        with pytest.raises(ValueError, match=DEPTH_MESSAGE):
            strata.loads(nested_document(depth))
    # Unbalanced and deep: the cap is reached before the document ends.
    with pytest.raises(ValueError, match=DEPTH_MESSAGE):
        strata.loads("[" * 100_000)


def test_cursor_mode_is_capped_by_the_same_constant():
    """return_type="cursor" builds the C++ document, a different builder on
    the same parser -- and the same refusal (api.md § Cursor)."""
    cursor = strata.loads(nested_document(MAX_NESTING_DEPTH), return_type="cursor")
    assert cursor.is_array()
    with pytest.raises(ValueError, match=DEPTH_MESSAGE):
        strata.loads(nested_document(MAX_NESTING_DEPTH + 1), return_type="cursor")
    with pytest.raises(ValueError, match=DEPTH_MESSAGE):
        strata.loads(nested_document(MAX_NESTING_DEPTH + 1).encode(), return_type="cursor")


def test_a_too_deep_document_is_not_a_generic_parse_error():
    """The message distinguishes a refusal from malformed text, so a caller
    can tell "your document is too deep" from "your document is broken"."""
    with pytest.raises(ValueError) as refused:
        strata.loads(nested_document(MAX_NESTING_DEPTH + 1))
    assert str(refused.value) == "Maximum nesting depth exceeded"
    with pytest.raises(ValueError) as malformed:
        strata.loads("[1,]")
    assert str(malformed.value) == "Invalid JSON"


def test_the_limit_is_per_document_not_cumulative():
    """A builder is reused across calls; a refused parse must leave nothing
    behind that lowers the next document's ceiling."""
    for _ in range(3):
        with pytest.raises(ValueError, match=DEPTH_MESSAGE):
            strata.loads(nested_document(MAX_NESTING_DEPTH + 1))
        assert depth_of(strata.loads(nested_document(MAX_NESTING_DEPTH))) == MAX_NESTING_DEPTH
    # Siblings each start from zero: many deep-but-legal branches in one array.
    inner = nested_document(MAX_NESTING_DEPTH - 1)
    assert len(strata.loads("[" + ",".join([inner] * 5) + "]")) == 5

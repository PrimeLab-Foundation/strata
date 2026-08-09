"""Contract tests for the streaming leg of search().

api.md: ".json uses streaming SAX search (only matches materialized) for
plain paths -- Filter/Slice paths fall back to a full parse". The law that
governs every line here is `search(f, e) == query(load(f), e)`; streaming is
an implementation of that law, so these tests pin exactly the places where a
stream could legally have drifted from load-then-query and must not:
duplicate keys, non-default policies, invalid tails, and the fallback routes.
"""

import json
import random

import pytest

import strata


@pytest.fixture
def json_file(tmp_path):
    def write(text):
        target = tmp_path / "doc.json"
        target.write_text(text, encoding="utf-8")
        return str(target)

    return write


# ---------------------------------------------------------------------------
# The law, on documents a stream is tempted to answer differently
# ---------------------------------------------------------------------------


def test_law_holds_on_randomized_documents(json_file):
    rng = random.Random(31337)
    keys = ["a", "b", "c", "id", "name", "items"]

    def gen(depth=0):
        r = rng.random()
        if depth > 4 or r < 0.3:
            return rng.choice([1, -2, 3.5, "s", "", True, False, None, 10**25])
        if r < 0.6:
            return [gen(depth + 1) for _ in range(rng.randint(0, 5))]
        return {k: gen(depth + 1) for k in rng.sample(keys, rng.randint(0, 5))}

    def gen_expr():
        parts = ["$"]
        for _ in range(rng.randint(1, 4)):
            kind = rng.randrange(3)
            if kind == 0:
                parts.append("." + rng.choice(keys))
            elif kind == 1:
                parts.append("[*]" if rng.random() < 0.5 else ".*")
            else:
                parts.append(f"[{rng.randint(0, 4)}]")
        return "".join(parts)

    checked = 0
    for _ in range(400):
        doc = gen()
        if not isinstance(doc, (dict, list)):
            continue
        path = json_file(json.dumps(doc))
        expr = gen_expr()
        assert strata.search(path, expr) == strata.query(strata.load(path), expr)
        checked += 1
    assert checked > 250


@pytest.mark.parametrize(
    ("text", "expr", "expected"),
    [
        # FirstWins: the first occurrence is the one load() kept.
        ('{"a":1,"a":2}', "$.a", [1]),
        # A wildcard yields one value per distinct key.
        ('{"a":1,"a":2}', "$.*", [1]),
        ('{"b":0,"a":1,"b":2}', "$.*", [0, 1]),
        ('[{"a":1,"a":2},{"a":3}]', "$[*].a", [1, 3]),
        ('{"a":{"x":1},"a":{"x":2}}', "$.a.x", [1]),
        # Duplicate keys INSIDE a captured match collapse via the policy too,
        # because the capture builder is the loads builder.
        ('[{"k":{"d":1,"d":2}}]', "$[*].k", [{"d": 1}]),
    ],
)
def test_duplicate_keys_match_load_semantics(json_file, text, expr, expected):
    path = json_file(text)
    assert strata.search(path, expr) == expected
    assert strata.search(path, expr) == strata.query(strata.load(path), expr)


def test_non_default_policy_falls_back_and_still_obeys_the_law(json_file):
    path = json_file('[{"a":1,"a":2},{"a":3}]')
    strata.config.set("duplicate_key_policy", "last")
    try:
        assert strata.search(path, "$[*].a") == strata.query(strata.load(path), "$[*].a") == [2, 3]
    finally:
        strata.config.set("duplicate_key_policy", "first")


# ---------------------------------------------------------------------------
# Errors: matches never survive an invalid document
# ---------------------------------------------------------------------------


def test_trailing_garbage_raises_even_after_matches(json_file):
    path = json_file('[{"a":1}] trailing')
    with pytest.raises(ValueError):
        strata.search(path, "$[*].a")


def test_truncated_document_raises(json_file):
    path = json_file('[{"a":1},{"a":')
    with pytest.raises(ValueError):
        strata.search(path, "$[*].a")


def test_empty_file_raises_like_load(json_file):
    path = json_file("")
    with pytest.raises(ValueError, match="Empty file"):
        strata.search(path, "$[*].a")


def test_missing_file_raises_file_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        strata.search(str(tmp_path / "absent.json"), "$.a")


# ---------------------------------------------------------------------------
# Fallback routes produce the same answers as the streaming route would
# ---------------------------------------------------------------------------


def test_negative_index_falls_back(json_file):
    path = json_file("[1,2,3]")
    assert strata.search(path, "$[-1]") == [3]


def test_bare_root_falls_back(json_file):
    path = json_file("[1,2,3]")
    assert strata.search(path, "$") == [[1, 2, 3]]


def test_recursive_descent_falls_back(json_file):
    path = json_file('{"a":{"a":1}}')
    assert strata.search(path, "$..a") == [{"a": 1}, 1]


def test_big_ints_survive_streaming_capture(json_file):
    path = json_file('[{"n": 123456789012345678901234567890}]')
    assert strata.search(path, "$[*].n") == [123456789012345678901234567890]


def test_unicode_and_escapes_survive_streaming_capture(json_file):
    doc = [{"kéy": "café \U0001f30d", "q": 'a"b\n'}]
    path = json_file(json.dumps(doc, ensure_ascii=False))
    assert strata.search(path, "$[0]") == doc

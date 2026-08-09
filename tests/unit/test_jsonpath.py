"""Contract tests for `strata.query`, `strata.search` and `strata.compile`.

Each test pins a clause of docs/context/api.md § JSONPath. The last section
pins the law the milestone requires: `search(f, e) == query(load(f), e)` for
every supported expression.
"""

import json

import pytest

import strata

DATA = {
    "store": {
        "book": [
            {"title": "Moby Dick", "price": 8.99, "year": 1851, "tags": ["sea", "whale"]},
            {"title": "Dune", "price": 12.50, "year": 1965, "tags": ["desert"]},
            {"title": "Ubik", "price": 5.00, "year": 1969, "tags": []},
        ],
        "bicycle": {"color": "red", "price": 19.95},
    },
    "expensive": 10,
    "nested": {"price": {"price": 1}},
}


@pytest.fixture
def document(tmp_path):
    path = tmp_path / "store.json"
    path.write_text(json.dumps(DATA), encoding="utf-8")
    return path


# ---------------------------------------------------------------------------
# Supported grammar: "$ (mandatory root) . .field . ["field"]/['field'] .
# [n] (negative ok) . [*] . .* . ..field . [start:end:step] . [?(@.field op value)]"
# ---------------------------------------------------------------------------


def test_the_root_alone_selects_the_document():
    assert strata.query(DATA, "$") == [DATA]


def test_dotted_fields():
    assert strata.query(DATA, "$.store.bicycle.color") == ["red"]
    assert strata.query(DATA, "$.expensive") == [10]


def test_quoted_fields():
    assert strata.query(DATA, "$['store']['bicycle']['color']") == ["red"]
    assert strata.query(DATA, '$["store"]["bicycle"]["color"]') == ["red"]


def test_a_quoted_field_may_contain_characters_a_dotted_one_cannot():
    data = {"a b": {"c-d": 1}}
    assert strata.query(data, "$['a b']['c-d']") == [1]


def test_indexing_including_from_the_end():
    assert strata.query(DATA, "$.store.book[0].title") == ["Moby Dick"]
    assert strata.query(DATA, "$.store.book[-1].title") == ["Ubik"]
    assert strata.query(DATA, "$.store.book[99]") == []
    assert strata.query(DATA, "$.store.book[-99]") == []


def test_wildcards():
    assert strata.query(DATA, "$.store.book[*].title") == ["Moby Dick", "Dune", "Ubik"]
    assert strata.query(DATA, "$.store.bicycle.*") == ["red", 19.95]
    assert strata.query(DATA, "$.store.book[*].year") == [1851, 1965, 1969]


def test_recursive_descent():
    assert strata.query(DATA, "$..color") == ["red"]
    assert strata.query(DATA, "$..title") == ["Moby Dick", "Dune", "Ubik"]


def test_recursive_descent_finds_a_match_nested_inside_a_match():
    """`$..price` must return the outer *and* the inner one.

    This is exactly where the previous implementation's file search and
    in-memory query disagreed; the law below now forbids that.
    """
    prices = strata.query(DATA, "$..price")
    assert 8.99 in prices and 19.95 in prices
    # "nested" holds {"price": {"price": 1}}: both the wrapper and the inner 1.
    assert {"price": 1} in prices
    assert 1 in prices


def test_slices():
    titles = strata.query(DATA, "$.store.book[0:2]")
    assert [b["title"] for b in titles] == ["Moby Dick", "Dune"]
    assert len(strata.query(DATA, "$.store.book[:]")) == 3
    assert len(strata.query(DATA, "$.store.book[1:]")) == 2
    assert len(strata.query(DATA, "$.store.book[:2]")) == 2
    assert [b["title"] for b in strata.query(DATA, "$.store.book[0:3:2]")] == ["Moby Dick", "Ubik"]


def test_a_negative_slice_step_selects_nothing():
    """Documented quirk: it parses, and the evaluator only runs forwards."""
    assert strata.query(DATA, "$.store.book[::-1]") == []


@pytest.mark.parametrize(
    ("expression", "expected_titles"),
    [
        ("$.store.book[?(@.price > 10)]", ["Dune"]),
        ("$.store.book[?(@.price >= 8.99)]", ["Moby Dick", "Dune"]),
        ("$.store.book[?(@.price < 9)]", ["Moby Dick", "Ubik"]),
        ("$.store.book[?(@.price <= 5)]", ["Ubik"]),
        ("$.store.book[?(@.year == 1965)]", ["Dune"]),
        ("$.store.book[?(@.year != 1965)]", ["Moby Dick", "Ubik"]),
        ("$.store.book[?(@.title == 'Dune')]", ["Dune"]),
        ("$.store.book[?(@.title != 'Dune')]", ["Moby Dick", "Ubik"]),
        ("$.store.book[?(@['title'] == 'Ubik')]", ["Ubik"]),
    ],
)
def test_filters(expression, expected_titles):
    assert [b["title"] for b in strata.query(DATA, expression)] == expected_titles


def test_a_filter_on_a_missing_field_matches_nothing():
    assert strata.query(DATA, "$.store.book[?(@.absent == 1)]") == []


def test_steps_compose():
    assert strata.query(DATA, "$.store.book[?(@.price > 10)].title") == ["Dune"]
    assert strata.query(DATA, "$.store.book[0].tags[*]") == ["sea", "whale"]
    assert strata.query(DATA, "$..book[*].price") == [8.99, 12.50, 5.00]


def test_each_step_applies_to_every_match_not_to_the_match_list():
    """A step after a filter runs against each matched node, one at a time.

    So `[0]` following a filter indexes into each matched *book*, which is an
    object and has no element 0 -- it does not take the first match.
    """
    assert strata.query(DATA, "$.store.book[?(@.price > 10)][0]") == []
    assert strata.query(DATA, "$.store.book[*][0]") == []
    # Indexing the array itself is what selects a single book.
    assert strata.query(DATA, "$.store.book[1].title") == ["Dune"]


# ---------------------------------------------------------------------------
# 'Invalid expressions raise `ValueError("Invalid JSONPath expression")`.
#  All invalid expressions -- including unclosed quotes -- raise ValueError
#  (the previous implementation leaked RuntimeError for unclosed quotes).'
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "expression",
    [
        "",  # empty
        "store.book",  # no root
        "$.",  # nothing after the dot
        "$..",  # nothing to descend to
        "$[",  # unclosed bracket
        "$[]",  # empty brackets
        "$['unclosed",  # unclosed quote -- the previous RuntimeError leak
        '$["unclosed',
        "$['a\\q']",  # unsupported escape
        "$.a b",  # stray text between steps
        "$[1,2]",  # unions
        "$['a','b']",
        "$..*",  # recursive wildcard
        "$..[0]",
        "$.a[?(@.x && @.y == 1)]",  # conjunctions
        "$.a[?(@.x)]",  # existence filters
        "$.a[?(@.x == )]",  # missing value
        "$.a[?(@.x > 'text')]",  # ordering on strings
        "$.a[?@.x == 1]",  # missing parentheses
        "$.a[0:1:0]",  # zero step
        "$.a[",
        "$.a]",
    ],
)
def test_invalid_expressions_raise_value_error(expression):
    with pytest.raises(ValueError, match="^Invalid JSONPath expression$"):
        strata.query(DATA, expression)


def test_compile_rejects_the_same_expressions():
    with pytest.raises(ValueError, match="^Invalid JSONPath expression$"):
        strata.compile("$['unclosed")


# ---------------------------------------------------------------------------
# "query evaluates directly on Python objects (dict/list/tuple roots only,
#  else TypeError)"; "search operates on a file ... must end .json/.ndjson/
#  .jsonl (else TypeError)"
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("data", [1, "text", None, True, 2.5, {1, 2}, object()])
def test_query_rejects_a_non_container_root(data):
    with pytest.raises(TypeError):
        strata.query(data, "$")


def test_query_accepts_list_and_tuple_roots():
    assert strata.query([{"a": 1}, {"a": 2}], "$[*].a") == [1, 2]
    assert strata.query(({"a": 1},), "$[0].a") == [1]


def test_search_rejects_an_unsupported_extension(tmp_path):
    path = tmp_path / "data.txt"
    path.write_text("{}", encoding="utf-8")
    with pytest.raises(TypeError, match="json"):
        strata.search(path, "$")


@pytest.mark.parametrize("suffix", [".json", ".ndjson", ".jsonl"])
def test_search_accepts_the_documented_extensions(tmp_path, suffix):
    path = tmp_path / f"data{suffix}"
    path.write_text('{"a": 1}', encoding="utf-8")
    assert strata.search(path, "$..a") == [1]


def test_search_reports_a_missing_file(tmp_path):
    with pytest.raises(FileNotFoundError):
        strata.search(tmp_path / "absent.json", "$")


def test_a_bad_expression_type_is_a_type_error():
    with pytest.raises(TypeError):
        strata.query(DATA, 42)


# ---------------------------------------------------------------------------
# "strata.compile(expression) -> CompiledPath" and reuse
# ---------------------------------------------------------------------------


def test_a_compiled_path_can_be_reused():
    compiled = strata.compile("$.store.book[*].title")
    assert strata.query(DATA, compiled) == ["Moby Dick", "Dune", "Ubik"]
    assert strata.query(DATA, compiled) == strata.query(DATA, "$.store.book[*].title")


def test_a_compiled_path_reprs_its_expression():
    assert "$.store" in repr(strata.compile("$.store"))


def test_a_compiled_path_works_with_search(document):
    compiled = strata.compile("$..price")
    assert strata.search(document, compiled) == strata.search(document, "$..price")


# ---------------------------------------------------------------------------
# "`CompiledPath.execute(cursor)` runs a compiled path against a cursor"
# ---------------------------------------------------------------------------


def test_execute_against_a_cursor():
    cursor = strata.loads(json.dumps(DATA), return_type="cursor")
    matches = strata.compile("$.store.book[*].title").execute(cursor)
    assert [m.get_str() for m in matches] == ["Moby Dick", "Dune", "Ubik"]


def test_execute_returns_cursors_that_keep_the_document_alive():
    import gc

    matches = strata.compile("$.store.book[0]").execute(
        strata.loads(json.dumps(DATA), return_type="cursor"),
    )
    gc.collect()
    assert matches[0].field("title").get_str() == "Moby Dick"


def test_execute_rejects_a_non_cursor():
    with pytest.raises(TypeError):
        strata.compile("$").execute({"a": 1})


# ---------------------------------------------------------------------------
# "iterator=True" -- eager evaluate, lazy consume
# ---------------------------------------------------------------------------


def test_query_iterator_yields_the_same_matches():
    assert list(strata.query(DATA, "$..title", iterator=True)) == strata.query(DATA, "$..title")


def test_search_iterator_yields_the_same_matches(document):
    assert list(strata.search(document, "$..price", iterator=True)) == strata.search(
        document,
        "$..price",
    )


# ---------------------------------------------------------------------------
# The law: search(f, e) == query(load(f), e) for every supported expression.
# ---------------------------------------------------------------------------

EXPRESSIONS = [
    "$",
    "$.expensive",
    "$.store.bicycle.color",
    "$['store']['bicycle']['price']",
    "$.store.book[0]",
    "$.store.book[-1]",
    "$.store.book[*]",
    "$.store.book[*].title",
    "$.store.book[*].tags[*]",
    "$.store.*",
    "$..price",
    "$..title",
    "$..tags",
    "$.store.book[0:2]",
    "$.store.book[1:]",
    "$.store.book[:2]",
    "$.store.book[0:3:2]",
    "$.store.book[::-1]",
    "$.store.book[?(@.price > 10)]",
    "$.store.book[?(@.year == 1965)]",
    "$.store.book[?(@.title == 'Dune')]",
    "$.store.book[?(@.title != 'Dune')].price",
    "$.absent",
    "$..absent",
]


@pytest.mark.parametrize("expression", EXPRESSIONS)
def test_search_equals_query_over_the_loaded_document(document, expression):
    assert strata.search(document, expression) == strata.query(
        strata.load(document),
        expression,
    )


@pytest.mark.parametrize("expression", EXPRESSIONS)
def test_the_law_holds_for_ndjson_too(tmp_path, expression):
    path = tmp_path / "records.ndjson"
    path.write_text(
        "".join(json.dumps(record) + "\n" for record in [DATA, {"store": {"book": []}}]),
        encoding="utf-8",
    )
    assert strata.search(path, expression) == strata.query(strata.load(path), expression)

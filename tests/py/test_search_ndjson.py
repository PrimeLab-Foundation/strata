"""
Tests for NDJSON search via strata.search().
"""

import pytest

import strata
from strata import _strata as _native


def _write_ndjson(path, lines):
    path.write_text("\n".join(lines), encoding="utf-8")


def test_search_ndjson_valid_lines(tmp_path):
    path = tmp_path / "data.ndjson"
    _write_ndjson(path, ['{"id": 1}', '{"id": 2}'])

    results = strata.search(path, "$.id")

    assert results == [
        {"line": 1, "matches": [1]},
        {"line": 2, "matches": [2]},
    ]


def test_search_ndjson_empty_lines(tmp_path):
    path = tmp_path / "blanks.ndjson"
    _write_ndjson(path, ['{"id": 1}', "", "   ", '{"id": 2}'])

    results = strata.search(path, "$.id")

    assert results == [
        {"line": 1, "matches": [1]},
        {"line": 4, "matches": [2]},
    ]


def test_search_ndjson_malformed_lines(tmp_path):
    path = tmp_path / "bad.ndjson"
    _write_ndjson(path, ['{"id": 1}', "{bad}", '{"id": 3}'])

    with pytest.raises(ValueError):
        strata.search(path, "$.id")

    results = strata.search(path, "$.id", skip_errors=True)
    assert results == [
        {"line": 1, "matches": [1]},
        {"line": 3, "matches": [3]},
    ]


def test_search_ndjson_warn_on_error(tmp_path):
    path = tmp_path / "warn.ndjson"
    _write_ndjson(path, ['{"id": 1}', "{bad}", '{"id": 3}'])

    with pytest.warns(RuntimeWarning):
        results = strata.search(path, "$.id", on_error="warn")

    assert results == [
        {"line": 1, "matches": [1]},
        {"line": 3, "matches": [3]},
    ]


def test_search_ndjson_large_file(tmp_path):
    path = tmp_path / "large.ndjson"
    lines = [f'{{"id": {i}}}' for i in range(5000)]
    _write_ndjson(path, lines)

    results = strata.search(path, "$.id")

    assert len(results) == 5000
    assert results[0] == {"line": 1, "matches": [0]}
    assert results[-1] == {"line": 5000, "matches": [4999]}


def test_search_ndjson_mixed_types(tmp_path):
    path = tmp_path / "mixed.ndjson"
    _write_ndjson(path, ['{"a": 1}', "[1, 2]", '"hi"', "42", "true"])

    results = strata.search(path, "$")

    assert results == [
        {"line": 1, "matches": [{"a": 1}]},
        {"line": 2, "matches": [[1, 2]]},
        {"line": 3, "matches": ["hi"]},
        {"line": 4, "matches": [42]},
        {"line": 5, "matches": [True]},
    ]


def test_search_ndjson_limit(tmp_path):
    path = tmp_path / "limit.ndjson"
    lines = [f'[{{"name": "user_{i}"}}]' for i in range(20)]
    _write_ndjson(path, lines)

    results = strata.search(path, "$[*].name", limit=5)

    assert len(results) == 5
    assert sum(len(entry["matches"]) for entry in results) == 5
    assert results[0] == {"line": 1, "matches": ["user_0"]}
    assert results[-1] == {"line": 5, "matches": ["user_4"]}


def test_search_ndjson_simple_field_fused_matches_full(tmp_path):
    path = tmp_path / "names.ndjson"
    lines = [
        '[{"name": "alpha"}, {"name": "beta"}]',
        '[{"name": "gamma"}]',
    ]
    _write_ndjson(path, lines)

    compiled = _native.compile_path("$[*].name")
    cursor = _native.NdjsonCursor.from_file(str(path))
    baseline = _native.search(cursor, compiled)

    text = "\n".join(lines)
    fused = strata.search(text, "$[*].name", ndjson=True)

    assert fused == baseline


def test_search_ndjson_parallel_matches_sequential():
    lines = [f'{{"id": {i}, "name": "user_{i:06d}"}}' for i in range(80000)]
    text = "\n".join(lines)
    assert len(text) > 2 * 1024 * 1024

    seq = strata.search(text, "$.id", ndjson=True, parallel=False)
    par = strata.search(text, "$.id", ndjson=True, parallel=True)

    assert par == seq


def test_search_ndjson_lazy_cursor_matches_eager(tmp_path):
    path = tmp_path / "lazy.ndjson"
    lines = [
        '{"id": 1, "name": "alpha"}',
        '{"id": 2, "name": "beta"}',
        '[{"id": 3}, {"id": 4}]',
        '{"id": 5, "nested": {"name": "gamma"}}',
    ]
    _write_ndjson(path, lines)

    eager = _native.NdjsonCursor.from_file(str(path))
    lazy = _native.NdjsonCursor.from_file_lazy(str(path))

    queries = ["$", "$.id", "$.nested.name", "$[*].id"]
    for expr in queries:
        eager_results = strata.search(eager, expr)
        lazy_results = strata.search(lazy, expr)
        assert lazy_results == eager_results


def test_search_ndjson_lazy_cursor_random_access(tmp_path):
    path = tmp_path / "random.ndjson"
    lines = [f'{{"idx": {i}}}' for i in range(1000)]
    _write_ndjson(path, lines)

    lazy = _native.NdjsonCursor.from_file_lazy(str(path))

    assert strata.search(lazy, "$[0]") == [{"line": 1, "matches": [{"idx": 0}]}]
    assert strata.search(lazy, "$[999]") == [{"line": 1000, "matches": [{"idx": 999}]}]
    assert strata.search(lazy, "$[-1]") == [{"line": 1000, "matches": [{"idx": 999}]}]


def test_search_ndjson_lazy_cursor_memory_usage(tmp_path):
    path = tmp_path / "mem.ndjson"
    lines = [f'{{"idx": {i}}}' for i in range(1000)]
    _write_ndjson(path, lines)

    eager = _native.NdjsonCursor.from_file(str(path))
    lazy = _native.NdjsonCursor.from_file_lazy(str(path))

    eager_stats = eager.stats()
    lazy_stats = lazy.stats()

    assert eager_stats["cached_lines"] == 1000
    assert lazy_stats["cached_lines"] == 0
    assert lazy_stats["line_count"] == 1000

    strata.search(lazy, "$[0]")
    after_stats = lazy.stats()
    assert after_stats["parsed_lines"] <= 1

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

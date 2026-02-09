"""
Tests for NDJSON cursor search via strata.search().
"""

import pytest

import strata
import strata.ndjson as ndjson


def _write_ndjson(path, lines):
    path.write_text("\n".join(lines), encoding="utf-8")


def test_ndjson_cursor_valid_lines(tmp_path):
    path = tmp_path / "data.ndjson"
    _write_ndjson(path, ['{"id": 1}', '{"id": 2}'])

    cursor = ndjson.parse_ndjson_file(path)
    results = strata.search(cursor, "$.id")

    assert results == [
        {"line": 1, "matches": [1]},
        {"line": 2, "matches": [2]},
    ]


def test_ndjson_cursor_empty_lines(tmp_path):
    path = tmp_path / "blanks.ndjson"
    _write_ndjson(path, ['{"id": 1}', "", "   ", '{"id": 2}'])

    cursor = ndjson.parse_ndjson_file(path)
    results = strata.search(cursor, "$.id")

    assert results == [
        {"line": 1, "matches": [1]},
        {"line": 4, "matches": [2]},
    ]


def test_ndjson_cursor_malformed_lines(tmp_path):
    path = tmp_path / "bad.ndjson"
    _write_ndjson(path, ['{"id": 1}', "{bad}", '{"id": 3}'])

    with pytest.raises(ValueError):
        ndjson.parse_ndjson_file(path)

    cursor = ndjson.parse_ndjson_file(path, skip_errors=True)
    results = strata.search(cursor, "$.id")
    assert results == [
        {"line": 1, "matches": [1]},
        {"line": 3, "matches": [3]},
    ]


def test_ndjson_cursor_warn_on_error(tmp_path):
    path = tmp_path / "warn.ndjson"
    _write_ndjson(path, ['{"id": 1}', "{bad}", '{"id": 3}'])

    with pytest.warns(RuntimeWarning):
        cursor = ndjson.parse_ndjson_file(path, on_error="warn")

    results = strata.search(cursor, "$.id")
    assert results == [
        {"line": 1, "matches": [1]},
        {"line": 3, "matches": [3]},
    ]


def test_ndjson_cursor_large_file(tmp_path):
    path = tmp_path / "large.ndjson"
    lines = [f'{{"id": {i}}}' for i in range(5000)]
    _write_ndjson(path, lines)

    cursor = ndjson.parse_ndjson_file(path)
    results = strata.search(cursor, "$.id")

    assert len(results) == 5000
    assert results[0] == {"line": 1, "matches": [0]}
    assert results[-1] == {"line": 5000, "matches": [4999]}


def test_ndjson_cursor_mixed_types(tmp_path):
    path = tmp_path / "mixed.ndjson"
    _write_ndjson(path, ['{"a": 1}', "[1, 2]", '"hi"', "42", "true"])

    cursor = ndjson.parse_ndjson_file(path)
    results = strata.search(cursor, "$")

    assert results == [
        {"line": 1, "matches": [{"a": 1}]},
        {"line": 2, "matches": [[1, 2]]},
        {"line": 3, "matches": ["hi"]},
        {"line": 4, "matches": [42]},
        {"line": 5, "matches": [True]},
    ]

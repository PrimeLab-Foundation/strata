"""
Tests for _strata.search strata_mode handling.
"""

import pytest

from strata import _strata as _native


def test_search_strata_mode_dict():
    data = {"users": [{"id": 1}, {"id": 2}]}
    results = _native.search(data, "$.users[*].id", strata_mode="dict")
    assert results == [1, 2]


def test_search_strata_mode_string():
    json_text = '{"users": [{"id": 1}, {"id": 2}]}'
    results = _native.search(json_text, "$.users[*].id", strata_mode="string")
    assert results == [1, 2]


def test_search_strata_mode_cursor():
    json_text = '{"users": [{"id": 1}, {"id": 2}]}'
    document = _native.JsonDocument.from_string(json_text)
    cursor = document.root()
    results = _native.search(cursor, "$.users[*].id", strata_mode="cursor")
    assert results == [1, 2]


def test_search_strata_mode_file(tmp_path):
    path = tmp_path / "data.json"
    path.write_text('{"a": 1}', encoding="utf-8")
    results = _native.search(str(path), "$.a", strata_mode="file")
    assert results == [1]


def test_search_auto_file_detection(tmp_path):
    path = tmp_path / "data.json"
    path.write_text('{"a": 1}', encoding="utf-8")
    results = _native.search(str(path), "$.a")
    assert results == [1]


def test_search_strata_mode_invalid():
    with pytest.raises(ValueError):
        _native.search("{}", "$", strata_mode="nope")


def test_search_strata_mode_string_type_error():
    with pytest.raises(TypeError):
        _native.search({"a": 1}, "$.a", strata_mode="string")


def test_search_strata_mode_cursor_type_error():
    with pytest.raises(TypeError):
        _native.search("{}", "$", strata_mode="cursor")

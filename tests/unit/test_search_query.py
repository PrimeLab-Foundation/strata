"""
Test search() and query() API split.

search() = file paths only (.json/.ndjson/.jsonl)
query() = dict/list only, no mem_eff
"""

import json
from pathlib import Path

import pytest

import strata


class TestQuery:
    """Test strata.query() with dict/list input."""

    def test_query_dict(self):
        data = {"a": 1, "b": 2}
        results = strata.query(data, "$.a")
        assert results == [1]

    def test_query_list(self):
        data = [1, 2, 3]
        results = strata.query(data, "$[0]")
        assert results == [1]

    def test_query_nested(self):
        data = {"users": [{"id": 1}, {"id": 2}, {"id": 3}]}
        results = strata.query(data, "$.users[*].id")
        assert results == [1, 2, 3]

    def test_query_with_compiled_path(self):
        path = strata.compile_path("$.users[*].name")
        data = {"users": [{"name": "Alice"}, {"name": "Bob"}]}
        results = strata.query(data, path)
        assert results == ["Alice", "Bob"]

    def test_query_wildcard(self):
        data = {"a": 1, "b": 2, "c": 3}
        results = strata.query(data, "$.*")
        assert set(results) == {1, 2, 3}

    def test_query_recursive_descent(self):
        data = {"a": {"price": 10}, "b": {"price": 20}}
        results = strata.query(data, "$..price")
        assert set(results) == {10, 20}

    def test_query_filter(self):
        data = [{"age": 30}, {"age": 25}, {"age": 35}]
        results = strata.query(data, "$[?(@.age > 27)]")
        assert len(results) == 2

    def test_query_string_raises_typeerror(self):
        with pytest.raises(TypeError):
            strata.query("not a dict", "$.field")

    def test_query_int_raises_typeerror(self):
        with pytest.raises(TypeError):
            strata.query(42, "$.field")

    def test_query_none_raises_typeerror(self):
        with pytest.raises(TypeError):
            strata.query(None, "$.field")

    def test_query_no_results(self):
        data = {"a": 1}
        results = strata.query(data, "$.missing")
        assert results == []

    def test_query_iterator(self):
        data = {"users": [{"id": 1}, {"id": 2}, {"id": 3}]}
        it = strata.query(data, "$.users[*].id", iterator=True)
        items = list(it)
        assert items == [1, 2, 3]

    def test_query_iterator_lazy(self):
        data = [10, 20, 30]
        it = strata.query(data, "$[*]", iterator=True)
        assert next(it) == 10
        assert next(it) == 20
        assert next(it) == 30
        with pytest.raises(StopIteration):
            next(it)


class TestSearch:
    """Test strata.search() with file path input."""

    def _write_json(self, tmp_path, data, name="test.json"):
        path = tmp_path / name
        path.write_text(json.dumps(data))
        return str(path)

    def test_search_json_file(self, tmp_path):
        filepath = self._write_json(tmp_path, {"users": [{"id": 1}, {"id": 2}]})
        results = strata.search(filepath, "$.users[*].id")
        assert results == [1, 2]

    def test_search_with_compiled_path(self, tmp_path):
        filepath = self._write_json(tmp_path, {"a": 1, "b": 2})
        path = strata.compile_path("$.a")
        results = strata.search(filepath, path)
        assert results == [1]

    def test_search_ndjson_file(self, tmp_path):
        path = tmp_path / "test.ndjson"
        lines = [json.dumps({"id": i, "name": f"user{i}"}) for i in range(3)]
        path.write_text("\n".join(lines))
        results = strata.search(str(path), "$.name")
        assert len(results) == 3

    def test_search_jsonl_file(self, tmp_path):
        path = tmp_path / "test.jsonl"
        lines = [json.dumps({"val": i}) for i in range(2)]
        path.write_text("\n".join(lines))
        results = strata.search(str(path), "$.val")
        assert len(results) == 2

    def test_search_with_path_object(self, tmp_path):
        filepath = self._write_json(tmp_path, {"x": 42})
        results = strata.search(Path(filepath), "$.x")
        assert results == [42]

    def test_search_mem_eff_true(self, tmp_path):
        filepath = self._write_json(tmp_path, {"users": [{"id": 1}, {"id": 2}]})
        results = strata.search(filepath, "$.users[*].id", mem_eff=True)
        assert results == [1, 2]

    def test_search_mem_eff_false(self, tmp_path):
        filepath = self._write_json(tmp_path, {"a": 1})
        results = strata.search(filepath, "$.a", mem_eff=False)
        assert results == [1]

    def test_search_dict_raises_typeerror(self):
        with pytest.raises(TypeError):
            strata.search({"a": 1}, "$.a")

    def test_search_list_raises_typeerror(self):
        with pytest.raises(TypeError):
            strata.search([1, 2], "$[0]")

    def test_search_nonexistent_file(self):
        with pytest.raises((FileNotFoundError, ValueError)):
            strata.search("/nonexistent/path.json", "$.a")

    def test_search_iterator(self, tmp_path):
        filepath = self._write_json(tmp_path, {"items": [1, 2, 3]})
        it = strata.search(filepath, "$.items[*]", iterator=True)
        items = list(it)
        assert items == [1, 2, 3]


class TestCompilePath:
    """Test strata.compile_path()."""

    def test_compile_valid_path(self):
        path = strata.compile_path("$.users[*].id")
        assert path is not None

    def test_compile_invalid_path(self):
        with pytest.raises(ValueError):
            strata.compile_path("invalid")

    def test_compile_reuse_across_queries(self):
        path = strata.compile_path("$[*]")
        r1 = strata.query([1, 2], path)
        r2 = strata.query([3, 4, 5], path)
        assert r1 == [1, 2]
        assert r2 == [3, 4, 5]

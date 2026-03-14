"""
Test unified API: dumps, loads, load with return_type and iterator kwargs.
"""

import json
from pathlib import Path

import pytest

import strata


class TestDumps:
    """Test strata.dumps() with return_type kwarg."""

    def test_dumps_default_returns_str(self):
        result = strata.dumps({"key": "value"})
        assert isinstance(result, str)
        assert json.loads(result) == {"key": "value"}

    def test_dumps_return_type_str(self):
        result = strata.dumps({"a": 1}, return_type="str")
        assert isinstance(result, str)
        assert json.loads(result) == {"a": 1}

    def test_dumps_return_type_bytes(self):
        result = strata.dumps({"a": 1}, return_type="bytes")
        assert isinstance(result, bytes)
        assert json.loads(result) == {"a": 1}

    def test_dumps_return_type_invalid(self):
        with pytest.raises(ValueError):
            strata.dumps({"a": 1}, return_type="invalid")

    def test_dumps_list(self):
        result = strata.dumps([1, 2, 3])
        assert json.loads(result) == [1, 2, 3]

    def test_dumps_nested(self):
        data = {"users": [{"id": 1, "name": "Alice"}, {"id": 2, "name": "Bob"}]}
        result = strata.dumps(data)
        assert json.loads(result) == data

    def test_dumps_types(self):
        data = {"str": "hello", "int": 42, "float": 3.14, "bool": True, "null": None}
        result = strata.dumps(data)
        parsed = json.loads(result)
        assert parsed["str"] == "hello"
        assert parsed["int"] == 42
        assert parsed["bool"] is True
        assert parsed["null"] is None

    def test_dumps_bytes_roundtrip(self):
        data = {"key": "value"}
        as_bytes = strata.dumps(data, return_type="bytes")
        as_str = strata.dumps(data, return_type="str")
        assert json.loads(as_bytes) == json.loads(as_str)

    def test_dumps_empty_structures(self):
        assert json.loads(strata.dumps({})) == {}
        assert json.loads(strata.dumps([])) == []


class TestLoads:
    """Test strata.loads() with return_type and iterator kwargs."""

    def test_loads_default_returns_dict(self):
        result = strata.loads('{"a": 1}')
        assert isinstance(result, dict)
        assert result == {"a": 1}

    def test_loads_return_type_dict(self):
        result = strata.loads('{"a": 1}', return_type="dict")
        assert result == {"a": 1}

    def test_loads_returns_list(self):
        result = strata.loads('[1, 2, 3]')
        assert isinstance(result, list)
        assert result == [1, 2, 3]

    def test_loads_return_type_cursor(self):
        result = strata.loads('{"a": 1, "b": 2}', return_type="cursor")
        # Should return a JsonCursor
        from strata.json_cursor import JsonCursor

        assert isinstance(result, JsonCursor)
        assert result.is_object()

    def test_loads_bytes_input(self):
        result = strata.loads(b'{"a": 1}')
        assert result == {"a": 1}

    def test_loads_complex_structure(self):
        json_text = '{"users": [{"id": 1, "name": "Alice"}, {"id": 2, "name": "Bob"}]}'
        result = strata.loads(json_text)
        assert len(result["users"]) == 2
        assert result["users"][0]["name"] == "Alice"

    def test_loads_iterator_on_dict(self):
        it = strata.loads('{"a": 1, "b": 2}', iterator=True)
        items = list(it)
        # Dict iterator yields (key, value) tuples
        assert len(items) == 2
        keys = [k for k, v in items]
        values = [v for k, v in items]
        assert "a" in keys
        assert "b" in keys
        assert 1 in values
        assert 2 in values

    def test_loads_iterator_on_array(self):
        it = strata.loads('[10, 20, 30]', iterator=True)
        items = list(it)
        assert items == [10, 20, 30]

    def test_loads_iterator_is_lazy(self):
        it = strata.loads('[1, 2, 3]', iterator=True)
        assert next(it) == 1
        assert next(it) == 2
        assert next(it) == 3
        with pytest.raises(StopIteration):
            next(it)

    def test_loads_invalid_json(self):
        with pytest.raises(ValueError):
            strata.loads("{invalid}")

    def test_loads_all_types(self):
        result = strata.loads('[1, "two", true, false, null, 3.14]')
        assert result == [1, "two", True, False, None, 3.14]


class TestLoad:
    """Test strata.load() with return_type and iterator kwargs."""

    def _write_json(self, tmp_path, data, suffix=".json"):
        path = tmp_path / f"test{suffix}"
        path.write_text(json.dumps(data))
        return str(path)

    def test_load_json_file(self, tmp_path):
        filepath = self._write_json(tmp_path, {"key": "value"})
        result = strata.load(filepath)
        assert result == {"key": "value"}

    def test_load_json_returns_dict(self, tmp_path):
        filepath = self._write_json(tmp_path, {"a": 1})
        result = strata.load(filepath, return_type="dict")
        assert isinstance(result, dict)

    def test_load_json_returns_cursor(self, tmp_path):
        filepath = self._write_json(tmp_path, {"a": 1, "b": 2})
        result = strata.load(filepath, return_type="cursor")
        from strata.json_cursor import JsonCursor

        assert isinstance(result, JsonCursor)

    def test_load_ndjson_file(self, tmp_path):
        path = tmp_path / "test.ndjson"
        lines = [json.dumps({"id": i}) for i in range(3)]
        path.write_text("\n".join(lines))
        result = strata.load(str(path))
        assert isinstance(result, list)
        assert len(result) == 3

    def test_load_jsonl_file(self, tmp_path):
        path = tmp_path / "test.jsonl"
        lines = [json.dumps({"id": i}) for i in range(2)]
        path.write_text("\n".join(lines))
        result = strata.load(str(path))
        assert isinstance(result, list)
        assert len(result) == 2

    def test_load_ndjson_iterator(self, tmp_path):
        path = tmp_path / "test.ndjson"
        lines = [json.dumps({"id": i}) for i in range(3)]
        path.write_text("\n".join(lines))
        it = strata.load(str(path), iterator=True)
        items = list(it)
        assert len(items) == 3
        assert items[0]["id"] == 0
        assert items[2]["id"] == 2

    def test_load_json_iterator_on_dict(self, tmp_path):
        filepath = self._write_json(tmp_path, {"x": 10, "y": 20})
        it = strata.load(filepath, iterator=True)
        items = list(it)
        assert len(items) == 2

    def test_load_json_iterator_on_array(self, tmp_path):
        filepath = self._write_json(tmp_path, [1, 2, 3])
        it = strata.load(filepath, iterator=True)
        items = list(it)
        assert items == [1, 2, 3]

    def test_load_nonexistent_file(self):
        with pytest.raises((FileNotFoundError, ValueError)):
            strata.load("/nonexistent/path.json")


class TestDump:
    """Test strata.dump() file output."""

    def test_dump_creates_file(self, tmp_path):
        filepath = str(tmp_path / "out.json")
        strata.dump({"a": 1}, filepath)
        content = Path(filepath).read_text()
        assert json.loads(content) == {"a": 1}

    def test_dump_roundtrip(self, tmp_path):
        data = {"users": [{"id": 1}, {"id": 2}]}
        filepath = str(tmp_path / "out.json")
        strata.dump(data, filepath)
        loaded = strata.load(filepath)
        assert loaded == data

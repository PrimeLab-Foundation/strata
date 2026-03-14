"""
Test strata.load() and strata.dump() file I/O.
"""

import json
from pathlib import Path

import pytest

import strata


class TestLoadJson:
    """Test loading JSON files."""

    def _write(self, tmp_path, data, name="test.json"):
        p = tmp_path / name
        p.write_text(json.dumps(data))
        return str(p)

    def test_load_simple_object(self, tmp_path):
        fp = self._write(tmp_path, {"key": "value"})
        assert strata.load(fp) == {"key": "value"}

    def test_load_array(self, tmp_path):
        fp = self._write(tmp_path, [1, 2, 3])
        assert strata.load(fp) == [1, 2, 3]

    def test_load_nested(self, tmp_path):
        data = {"a": {"b": [1, 2, {"c": 3}]}}
        fp = self._write(tmp_path, data)
        assert strata.load(fp) == data

    def test_load_return_type_dict(self, tmp_path):
        fp = self._write(tmp_path, {"x": 1})
        result = strata.load(fp, return_type="dict")
        assert isinstance(result, dict)

    def test_load_return_type_cursor(self, tmp_path):
        fp = self._write(tmp_path, {"x": 1})
        result = strata.load(fp, return_type="cursor")
        from strata.json_cursor import JsonCursor

        assert isinstance(result, JsonCursor)

    def test_load_iterator_dict(self, tmp_path):
        fp = self._write(tmp_path, {"a": 1, "b": 2})
        it = strata.load(fp, iterator=True)
        items = list(it)
        assert len(items) == 2

    def test_load_iterator_array(self, tmp_path):
        fp = self._write(tmp_path, [10, 20, 30])
        it = strata.load(fp, iterator=True)
        assert list(it) == [10, 20, 30]

    def test_load_nonexistent(self):
        with pytest.raises((FileNotFoundError, ValueError)):
            strata.load("/nonexistent/path.json")


class TestLoadNdjson:
    """Test loading NDJSON/JSONL files."""

    def _write_ndjson(self, tmp_path, records, ext=".ndjson"):
        p = tmp_path / f"test{ext}"
        p.write_text("\n".join(json.dumps(r) for r in records))
        return str(p)

    def test_load_ndjson(self, tmp_path):
        fp = self._write_ndjson(tmp_path, [{"id": 1}, {"id": 2}])
        result = strata.load(fp)
        assert isinstance(result, list)
        assert len(result) == 2

    def test_load_jsonl(self, tmp_path):
        fp = self._write_ndjson(tmp_path, [{"a": 1}], ext=".jsonl")
        result = strata.load(fp)
        assert isinstance(result, list)
        assert len(result) == 1

    def test_load_ndjson_iterator(self, tmp_path):
        records = [{"id": i} for i in range(5)]
        fp = self._write_ndjson(tmp_path, records)
        it = strata.load(fp, iterator=True)
        items = list(it)
        assert len(items) == 5
        assert items[0]["id"] == 0
        assert items[4]["id"] == 4

    def test_load_ndjson_iterator_lazy(self, tmp_path):
        records = [{"v": i} for i in range(3)]
        fp = self._write_ndjson(tmp_path, records)
        it = strata.load(fp, iterator=True)
        assert next(it)["v"] == 0
        assert next(it)["v"] == 1
        assert next(it)["v"] == 2
        with pytest.raises(StopIteration):
            next(it)


class TestDump:
    """Test strata.dump() file output."""

    def test_dump_object(self, tmp_path):
        fp = str(tmp_path / "out.json")
        strata.dump({"a": 1}, fp)
        assert json.loads(Path(fp).read_text()) == {"a": 1}

    def test_dump_array(self, tmp_path):
        fp = str(tmp_path / "out.json")
        strata.dump([1, 2, 3], fp)
        assert json.loads(Path(fp).read_text()) == [1, 2, 3]

    def test_dump_load_roundtrip(self, tmp_path):
        data = {"users": [{"id": 1, "name": "Alice"}, {"id": 2, "name": "Bob"}]}
        fp = str(tmp_path / "roundtrip.json")
        strata.dump(data, fp)
        loaded = strata.load(fp)
        assert loaded == data

    def test_dump_nested_types(self, tmp_path):
        data = {
            "str": "hello",
            "int": 42,
            "float": 3.14,
            "bool": True,
            "null": None,
            "list": [1, 2],
        }
        fp = str(tmp_path / "types.json")
        strata.dump(data, fp)
        loaded = json.loads(Path(fp).read_text())
        assert loaded == data

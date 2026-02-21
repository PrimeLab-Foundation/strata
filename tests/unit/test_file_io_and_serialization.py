"""
Tests covering functions missed by existing unit tests.

Covers:
- strata.query()
- strata.load() / strata.dump()
- strata.loads_parallel_json_experiment()
- strata.mmap_io.parse_json_file()
- strata.ndjson.parse_ndjson_file() / parse_ndjson_file_lazy()
- strata.serialize.loads_tape() / dumps_bytes()
"""
import json
import pytest
import strata
import strata.mmap_io as mmap_io
import strata.ndjson as ndjson
import strata.serialize as serialize


class TestQuery:
    """Test strata.query() wrapper."""

    def test_query_simple(self):
        data = {"users": [{"name": "Alice"}, {"name": "Bob"}]}
        result = strata.query(data, "$.users[0].name")
        assert result == ["Alice"]

    def test_query_wildcard(self):
        data = {"items": [1, 2, 3]}
        result = strata.query(data, "$.items[*]")
        assert result == [1, 2, 3]


class TestLoadDump:
    """Test strata.load() and strata.dump()."""

    def test_load_from_file(self, tmp_path):
        f = tmp_path / "data.json"
        f.write_text('{"key": "value"}')
        result = strata.load(str(f))
        assert result == {"key": "value"}

    def test_dump_to_file(self, tmp_path):
        f = tmp_path / "out.json"
        strata.dump(str(f), {"a": 1})
        content = f.read_text()
        assert json.loads(content) == {"a": 1}


class TestLoadsParallel:
    """Test strata.loads_parallel_json_experiment()."""

    def test_parallel_basic(self):
        data = json.dumps({"x": list(range(100))})
        result = strata.loads_parallel_json_experiment(data)
        assert result == {"x": list(range(100))}


class TestMmapIo:
    """Test strata.mmap_io.parse_json_file()."""

    def test_parse_json_file(self, tmp_path):
        f = tmp_path / "test.json"
        f.write_text('{"hello": "world"}')
        cursor = mmap_io.parse_json_file(str(f))
        # Just verify it returns a JsonCursor (document ownership is internal)
        from strata.json_cursor import JsonCursor
        assert isinstance(cursor, JsonCursor)

    def test_parse_json_file_with_path_object(self, tmp_path):
        f = tmp_path / "test.json"
        f.write_text('{"n": 42}')
        cursor = mmap_io.parse_json_file(f)
        from strata.json_cursor import JsonCursor
        assert isinstance(cursor, JsonCursor)

    def test_parse_json_file_invalid(self, tmp_path):
        f = tmp_path / "bad.json"
        f.write_text("{bad}")
        with pytest.raises(Exception):
            mmap_io.parse_json_file(str(f))


class TestNdjsonFileFunctions:
    """Test parse_ndjson_file and parse_ndjson_file_lazy."""

    def test_parse_ndjson_file(self, tmp_path):
        f = tmp_path / "data.ndjson"
        f.write_text('{"a":1}\n{"b":2}\n')
        cursor = ndjson.parse_ndjson_file(str(f))
        assert cursor is not None

    def test_parse_ndjson_file_lazy(self, tmp_path):
        f = tmp_path / "data.ndjson"
        f.write_text('{"a":1}\n{"b":2}\n')
        cursor = ndjson.parse_ndjson_file_lazy(str(f))
        assert cursor is not None


class TestSerializeExtras:
    """Test loads_tape and dumps_bytes."""

    def test_loads_tape(self):
        result = serialize.loads_tape('{"key": "value"}')
        assert result == {"key": "value"}

    def test_loads_tape_array(self):
        result = serialize.loads_tape('[1, 2, 3]')
        assert result == [1, 2, 3]

    def test_dumps_bytes(self):
        result = serialize.dumps_bytes({"a": 1})
        assert isinstance(result, bytes)
        assert json.loads(result) == {"a": 1}

    def test_dumps_bytes_null(self):
        result = serialize.dumps_bytes(None)
        assert result == b"null"

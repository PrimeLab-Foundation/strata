"""
Tests to fill coverage gaps in Python wrapper modules.

Targets:
- python/strata/__init__.py  (query, load, dump, loads_parallel_json_experiment)
- python/strata/mmap_io.py   (parse_json_file wrapper)
- python/strata/ndjson.py    (parse_ndjson_cursor, parse_ndjson_file_lazy)
- python/strata/serialize.py (loads, dumps, set_dumps_type_order)
"""

from __future__ import annotations

import json
from pathlib import Path

import pytest

import strata
import strata.ndjson as ndjson
import strata.serialize as serialize
from strata import _strata as _native
from strata.mmap_io import parse_json_file


# ---------------------------------------------------------------------------
# mmap_io.py — parse_json_file Python wrapper (lines 7-41)
# The wrapper constructs JsonCursor(cursor, document). Using the result's
# .field() crashes (document lifetime issue in C binding), but all Python
# wrapper lines execute. We test the branches safely.
# ---------------------------------------------------------------------------

class TestMmapIoWrapper:
    """Cover strata.mmap_io.parse_json_file (the Python wrapper, not _native directly)."""

    def test_parse_json_file_str_path_returns_cursor(self, tmp_path):
        """Exercises the str-path branch (no isinstance conversion)."""
        f = tmp_path / "data.json"
        f.write_text('{"x": 1}')
        cursor = parse_json_file(str(f))
        assert isinstance(cursor, _native.JsonCursor)

    def test_parse_json_file_path_object_converts(self, tmp_path):
        """Exercises the isinstance(filepath, Path) branch (line 36)."""
        f = tmp_path / "data.json"
        f.write_text('{"y": 2}')
        cursor = parse_json_file(f)          # passes a Path, triggers str(filepath)
        assert isinstance(cursor, _native.JsonCursor)

    def test_parse_json_file_nonexistent_raises(self):
        """Exercises the error path via _native.parse_json_file."""
        with pytest.raises(ValueError):
            parse_json_file("/no/such/file.json")

    def test_parse_json_file_invalid_json_raises(self, tmp_path):
        f = tmp_path / "bad.json"
        f.write_text("{invalid json}")
        with pytest.raises(ValueError):
            parse_json_file(f)


# ---------------------------------------------------------------------------
# strata/__init__.py — query (21), load (26), dump (31),
#                      loads_parallel_json_experiment (41)
# ---------------------------------------------------------------------------

class TestInitQuery:
    """Cover strata.query (line 21)."""

    def test_query_dict(self):
        result = strata.query({"a": 1, "b": 2}, "$.a")
        assert result == [1]

    def test_query_nested(self):
        data = {"users": [{"id": 1}, {"id": 2}]}
        result = strata.query(data, "$.users[*].id")
        assert sorted(result) == [1, 2]

    def test_query_no_match_returns_empty(self):
        result = strata.query({"a": 1}, "$.z")
        assert result == []


class TestInitLoad:
    """Cover strata.load (line 26)."""

    def test_load_from_str_path(self, tmp_path):
        f = tmp_path / "data.json"
        f.write_text('{"hello": "world"}')
        result = strata.load(str(f))
        assert result == {"hello": "world"}

    def test_load_from_path_object(self, tmp_path):
        f = tmp_path / "data.json"
        f.write_text('[1, 2, 3]')
        result = strata.load(f)
        assert result == [1, 2, 3]

    def test_load_array_root(self, tmp_path):
        f = tmp_path / "arr.json"
        f.write_text('[true, null, 42]')
        result = strata.load(str(f))
        assert result == [True, None, 42]


class TestInitDump:
    """Cover strata.dump (line 31)."""

    def test_dump_dict_to_str_path(self, tmp_path):
        f = tmp_path / "out.json"
        strata.dump(str(f), {"a": 1})
        assert json.loads(f.read_text()) == {"a": 1}

    def test_dump_list_to_path_object(self, tmp_path):
        f = tmp_path / "out.json"
        strata.dump(f, [1, 2, 3])
        assert json.loads(f.read_text()) == [1, 2, 3]

    def test_dump_roundtrip_load(self, tmp_path):
        f = tmp_path / "rt.json"
        obj = {"nested": {"x": [1, "two", None, True]}}
        strata.dump(str(f), obj)
        assert strata.load(str(f)) == obj


class TestInitLoadsParallelExperiment:
    """Cover strata.loads_parallel_json_experiment (line 41)."""

    def test_parallel_loads_basic(self):
        data = json.dumps([{"id": i} for i in range(20)])
        result = strata.loads_parallel_json_experiment(data)
        assert isinstance(result, list)
        assert len(result) == 20
        assert result[0] == {"id": 0}

    def test_parallel_loads_with_num_threads(self):
        data = json.dumps({"key": "value"})
        result = strata.loads_parallel_json_experiment(data, num_threads=2)
        assert result == {"key": "value"}

    def test_parallel_loads_with_min_chunk_size(self):
        data = json.dumps(list(range(10)))
        result = strata.loads_parallel_json_experiment(data, min_chunk_size=512)
        assert result == list(range(10))

    def test_parallel_loads_default_args(self):
        data = json.dumps({"hello": "world"})
        result = strata.loads_parallel_json_experiment(data)
        assert result == {"hello": "world"}


# ---------------------------------------------------------------------------
# ndjson.py — parse_ndjson_cursor (line 106) and parse_ndjson_file_lazy (line 136)
# ---------------------------------------------------------------------------

class TestNdjsonCursorWrapper:
    """Cover ndjson.parse_ndjson_cursor (line 106)."""

    def test_parse_ndjson_cursor_from_str(self):
        data = '{"id": 1}\n{"id": 2}\n'
        cursor = ndjson.parse_ndjson_cursor(data)
        results = strata.search(cursor, "$.id")
        ids = [m for r in results for m in r["matches"]]
        assert ids == [1, 2]

    def test_parse_ndjson_cursor_from_bytes(self):
        data = b'{"a": 10}\n{"a": 20}\n'
        cursor = ndjson.parse_ndjson_cursor(data)
        results = strata.search(cursor, "$.a")
        ids = [m for r in results for m in r["matches"]]
        assert ids == [10, 20]

    def test_parse_ndjson_cursor_returns_ndjson_cursor(self):
        cursor = ndjson.parse_ndjson_cursor('{"x": 1}\n')
        assert isinstance(cursor, ndjson.NdjsonCursor)

    def test_parse_ndjson_cursor_single_line(self):
        cursor = ndjson.parse_ndjson_cursor('{"only": true}')
        results = strata.search(cursor, "$.only")
        assert results[0]["matches"] == [True]


class TestNdjsonFileLazyWrapper:
    """Cover ndjson.parse_ndjson_file_lazy (line 136)."""

    def test_parse_ndjson_file_lazy_str_path(self, tmp_path):
        f = tmp_path / "data.ndjson"
        f.write_text('{"id": 1}\n{"id": 2}\n')
        cursor = ndjson.parse_ndjson_file_lazy(str(f))
        assert isinstance(cursor, ndjson.NdjsonCursor)

    def test_parse_ndjson_file_lazy_path_object(self, tmp_path):
        f = tmp_path / "data.ndjson"
        f.write_text('{"k": "v"}\n')
        cursor = ndjson.parse_ndjson_file_lazy(f)
        results = strata.search(cursor, "$.k")
        assert results[0]["matches"] == ["v"]

    def test_parse_ndjson_file_lazy_multiple_lines(self, tmp_path):
        f = tmp_path / "data.ndjson"
        lines = [json.dumps({"n": i}) for i in range(5)]
        f.write_text("\n".join(lines) + "\n")
        cursor = ndjson.parse_ndjson_file_lazy(f)
        results = strata.search(cursor, "$.n")
        all_matches = [m for r in results for m in r["matches"]]
        assert all_matches == list(range(5))


# ---------------------------------------------------------------------------
# serialize.py — loads (line 34), dumps (line 85), set_dumps_type_order (line 141)
# ---------------------------------------------------------------------------

class TestSerializeLoads:
    """Cover serialize.loads (line 34)."""

    def test_loads_dict(self):
        assert serialize.loads('{"a": 1}') == {"a": 1}

    def test_loads_list(self):
        assert serialize.loads('[1, 2, 3]') == [1, 2, 3]

    def test_loads_string(self):
        assert serialize.loads('"hello"') == "hello"

    def test_loads_bytes(self):
        assert serialize.loads(b'{"b": 2}') == {"b": 2}

    def test_loads_null(self):
        assert serialize.loads('null') is None

    def test_loads_bool(self):
        assert serialize.loads('true') is True
        assert serialize.loads('false') is False

    def test_loads_invalid_raises(self):
        with pytest.raises(ValueError):
            serialize.loads('{bad}')


class TestSerializeDumps:
    """Cover serialize.dumps (line 85)."""

    def test_dumps_dict(self):
        result = serialize.dumps({"key": "val"})
        assert isinstance(result, str)
        assert json.loads(result) == {"key": "val"}

    def test_dumps_list(self):
        assert serialize.dumps([1, 2, 3]) == "[1,2,3]"

    def test_dumps_none(self):
        assert serialize.dumps(None) == "null"

    def test_dumps_bool(self):
        assert serialize.dumps(True) == "true"
        assert serialize.dumps(False) == "false"

    def test_dumps_int(self):
        assert serialize.dumps(42) == "42"

    def test_dumps_roundtrip(self):
        obj = {"nested": {"x": [1, 2, 3]}}
        assert json.loads(serialize.dumps(obj)) == obj


class TestSerializeSetDumpsTypeOrder:
    """Cover serialize.set_dumps_type_order (line 141)."""

    def teardown_method(self):
        serialize.set_dumps_type_order("strings_first")

    def test_set_strings_first(self):
        serialize.set_dumps_type_order("strings_first")
        result = serialize.dumps({"s": "hello", "n": 42})
        assert json.loads(result) == {"s": "hello", "n": 42}

    def test_set_ints_first(self):
        serialize.set_dumps_type_order("ints_first")
        result = serialize.dumps({"s": "hello", "n": 42})
        assert json.loads(result) == {"s": "hello", "n": 42}

    def test_type_order_does_not_break_serialization(self):
        for policy in ("strings_first", "ints_first"):
            serialize.set_dumps_type_order(policy)
            obj = {"a": 1, "b": "two", "c": [3, 4]}
            assert json.loads(serialize.dumps(obj)) == obj

"""
Test memory-mapped JSON file parsing.
"""

import json

import pytest

from strata import _strata as _native
from strata.json_cursor import parse_json


def parse_json_file(path):
    path_str = str(path)
    document, cursor = _native.parse_json_file(path_str)
    return cursor


class TestParseJsonFile:
    """Test parse_json_file functionality."""

    def test_basic_file_parsing(self, tmp_path):
        """Test basic file parsing with mmap."""
        test_file = tmp_path / "test.json"
        test_data = {"name": "Alice", "age": 30}

        test_file.write_text(json.dumps(test_data))

        doc = parse_json_file(str(test_file))
        assert doc.field("name").get_str() == "Alice"
        assert doc.field("age").get_int() == 30

    def test_with_path_object(self, tmp_path):
        """Test parsing with Path object."""
        test_file = tmp_path / "test.json"
        test_data = {"value": 42}

        test_file.write_text(json.dumps(test_data))

        doc = parse_json_file(test_file)
        assert doc.field("value").get_int() == 42

    def test_large_file(self, tmp_path):
        """Test parsing large file."""
        test_file = tmp_path / "large.json"

        # Generate large dataset
        data = {"users": [{"id": i, "name": f"User{i}"} for i in range(1000)]}
        test_file.write_text(json.dumps(data))

        doc = parse_json_file(test_file)
        users = doc.field("users")

        # Check first and last
        assert users.at(0).field("id").get_int() == 0
        assert users.at(999).field("id").get_int() == 999

    def test_nested_structure(self, tmp_path):
        """Test parsing nested structures."""
        test_file = tmp_path / "nested.json"
        test_data = {"user": {"profile": {"name": "Bob", "address": {"city": "NYC"}}}}

        test_file.write_text(json.dumps(test_data))

        doc = parse_json_file(test_file)
        name = doc.field("user").field("profile").field("name").get_str()
        assert name == "Bob"

        city = doc.field("user").field("profile").field("address").field("city").get_str()
        assert city == "NYC"

    def test_array_file(self, tmp_path):
        """Test parsing array as root."""
        test_file = tmp_path / "array.json"
        test_data = [1, 2, 3, 4, 5]

        test_file.write_text(json.dumps(test_data))

        doc = parse_json_file(test_file)
        assert doc.at(0).get_int() == 1
        assert doc.at(4).get_int() == 5

    def test_unicode_file(self, tmp_path):
        """Test parsing file with Unicode."""
        test_file = tmp_path / "unicode.json"
        test_data = {"message": "Hello 世界", "emoji": "👋"}

        test_file.write_text(json.dumps(test_data, ensure_ascii=False), encoding='utf-8')

        doc = parse_json_file(test_file)
        assert "世界" in doc.field("message").get_str()
        assert doc.field("emoji").get_str() == "👋"

    def test_empty_file(self, tmp_path):
        """Test parsing empty file."""
        test_file = tmp_path / "empty.json"
        test_file.write_text("{}")

        doc = parse_json_file(test_file)
        assert doc.is_object()

    def test_nonexistent_file(self):
        """Test error handling for nonexistent file."""
        with pytest.raises(ValueError):
            parse_json_file("/nonexistent/file.json")

    def test_invalid_json_file(self, tmp_path):
        """Test error handling for invalid JSON."""
        test_file = tmp_path / "invalid.json"
        test_file.write_text("{invalid json}")

        with pytest.raises(ValueError):
            parse_json_file(str(test_file))

    def test_comparison_with_parse_json(self, tmp_path):
        """Test that mmap gives same results as parse_json."""
        test_file = tmp_path / "data.json"
        test_data = {
            "string": "value",
            "number": 123,
            "bool": True,
            "null": None,
            "array": [1, 2, 3],
            "object": {"nested": "value"},
        }

        json_str = json.dumps(test_data)
        test_file.write_text(json_str)

        # Parse via mmap
        doc_mmap = parse_json_file(test_file)

        # Parse from string
        doc_str = parse_json(json_str)

        # Compare results
        assert doc_mmap.field("string").get_str() == doc_str.field("string").get_str()
        assert doc_mmap.field("number").get_int() == doc_str.field("number").get_int()
        assert doc_mmap.field("bool").get_bool() == doc_str.field("bool").get_bool()


class TestMmapPerformance:
    """Test mmap performance characteristics."""

    def test_large_file_memory_efficiency(self, tmp_path):
        """Test that mmap doesn't load entire file into memory immediately."""
        test_file = tmp_path / "large.json"

        # Generate 10K records
        data = {"records": [{"id": i, "data": f"Record {i}"} for i in range(10000)]}
        test_file.write_text(json.dumps(data))

        # Parse with mmap
        doc = parse_json_file(test_file)

        # Access only first record (should be fast)
        first_id = doc.field("records").at(0).field("id").get_int()
        assert first_id == 0

    def test_realistic_workload(self, tmp_path):
        """Test realistic JSON workload."""
        test_file = tmp_path / "realistic.json"

        # Realistic structure
        data = {
            "metadata": {"version": "1.0", "timestamp": "2026-01-30"},
            "users": [
                {"id": i, "name": f"User{i}", "email": f"user{i}@example.com", "active": i % 2 == 0}
                for i in range(100)
            ],
        }

        test_file.write_text(json.dumps(data))

        doc = parse_json_file(test_file)

        # Query structure
        version = doc.field("metadata").field("version").get_str()
        assert version == "1.0"

        # Verify users
        users = doc.field("users")
        assert users.is_array()
        assert users.at(0).field("id").get_int() == 0
        assert users.at(99).field("id").get_int() == 99

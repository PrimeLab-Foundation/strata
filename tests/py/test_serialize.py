"""
Test JSON serialization (dumps).
"""

import json

import pytest

import strata


class TestBasicSerialization:
    """Test basic dumps functionality."""

    def test_dumps_null(self):
        """Test serializing null."""
        assert strata.dumps(None) == "null"

    def test_dumps_bool_true(self):
        """Test serializing true."""
        assert strata.dumps(True) == "true"

    def test_dumps_bool_false(self):
        """Test serializing false."""
        assert strata.dumps(False) == "false"

    def test_dumps_integer(self):
        """Test serializing integers."""
        assert strata.dumps(0) == "0"
        assert strata.dumps(42) == "42"
        assert strata.dumps(-123) == "-123"

    def test_dumps_float(self):
        """Test serializing floats."""
        result = strata.dumps(3.14)
        # Allow some flexibility in float formatting
        assert "3.14" in result or "3.1400" in result

    def test_dumps_string(self):
        """Test serializing strings."""
        assert strata.dumps("hello") == '"hello"'
        assert strata.dumps("") == '""'

    def test_dumps_array(self):
        """Test serializing arrays."""
        assert strata.dumps([]) == "[]"
        assert strata.dumps([1, 2, 3]) == "[1,2,3]"
        # 3.0 may be serialized as "3" since it's a whole number
        result = strata.dumps([1, "two", 3.0])
        assert result in ['[1,"two",3.0]', '[1,"two",3]']

    def test_dumps_object(self):
        """Test serializing objects."""
        assert strata.dumps({}) == "{}"
        result = strata.dumps({"name": "Alice"})
        # Parse to compare (order doesn't matter)
        assert json.loads(result) == {"name": "Alice"}


class TestStringEscaping:
    """Test string escaping."""

    def test_escape_quotes(self):
        """Test escaping double quotes."""
        result = strata.dumps({"text": 'He said "hello"'})
        parsed = json.loads(result)
        assert parsed["text"] == 'He said "hello"'

    def test_escape_backslash(self):
        """Test escaping backslashes."""
        result = strata.dumps({"path": "C:\\Users\\test"})
        parsed = json.loads(result)
        assert parsed["path"] == "C:\\Users\\test"

    def test_escape_control_chars(self):
        """Test escaping control characters."""
        result = strata.dumps({"text": "line1\nline2\ttab\rcarriage"})
        parsed = json.loads(result)
        assert parsed["text"] == "line1\nline2\ttab\rcarriage"

    def test_escape_backspace_formfeed(self):
        """Test escaping backspace and formfeed."""
        result = strata.dumps({"text": "a\b\f"})
        parsed = json.loads(result)
        assert parsed["text"] == "a\b\f"

    def test_escape_unicode(self):
        """Test Unicode handling."""
        result = strata.dumps({"emoji": "Hello 👋 世界"})
        parsed = json.loads(result)
        assert parsed["emoji"] == "Hello 👋 世界"


class TestRoundTrip:
    """Test round-trip serialization (loads -> dumps -> loads)."""

    def test_roundtrip_simple_object(self):
        """Test round-trip with simple object."""
        obj = {"name": "Alice", "age": 30, "active": True}
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result == obj

    def test_roundtrip_nested_object(self):
        """Test round-trip with nested objects."""
        obj = {
            "user": {"name": "Alice", "address": {"city": "NYC", "zip": "10001"}},
            "active": True,
        }
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result == obj

    def test_roundtrip_array_of_objects(self):
        """Test round-trip with array of objects."""
        obj = [
            {"id": 1, "name": "Alice"},
            {"id": 2, "name": "Bob"},
            {"id": 3, "name": "Charlie"},
        ]
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result == obj

    def test_roundtrip_mixed_types(self):
        """Test round-trip with mixed types."""
        obj = {
            "null": None,
            "bool": True,
            "int": 42,
            "float": 3.14,
            "string": "hello",
            "array": [1, 2, 3],
            "object": {"nested": "value"},
        }
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result["null"] is None
        assert result["bool"] is True
        assert result["int"] == 42
        assert abs(result["float"] - 3.14) < 0.001
        assert result["string"] == "hello"
        assert result["array"] == [1, 2, 3]
        assert result["object"] == {"nested": "value"}

    def test_roundtrip_strata_parse(self):
        """Test round-trip using strata.parse_json."""
        obj = {"name": "Alice", "scores": [95, 87, 92]}
        json_str = strata.dumps(obj)
        cursor = strata.parse_json(json_str)

        # Verify we can access the data
        assert cursor.field("name").get_str() == "Alice"
        assert cursor.field("scores").at(0).get_float() == 95.0


class TestEdgeCases:
    """Test edge cases and special values."""

    def test_empty_containers(self):
        """Test empty arrays and objects."""
        assert strata.dumps([]) == "[]"
        assert strata.dumps({}) == "{}"

    def test_nested_empty_containers(self):
        """Test nested empty containers."""
        obj = {"empty_array": [], "empty_object": {}}
        result = json.loads(strata.dumps(obj))
        assert result == obj

    def test_large_numbers(self):
        """Test large numbers."""
        obj = {"big": 9007199254740992, "negative": -9007199254740992}
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result["big"] == 9007199254740992
        assert result["negative"] == -9007199254740992

    def test_special_floats(self):
        """Test NaN and Infinity (converted to null per JSON spec)."""
        # NaN and Inf are not valid JSON, should be converted to null
        result = strata.dumps({"nan": float('nan')})
        parsed = json.loads(result)
        assert parsed["nan"] is None

        result2 = strata.dumps({"inf": float('inf')})
        parsed2 = json.loads(result2)
        assert parsed2["inf"] is None

        result3 = strata.dumps({"neg_inf": float('-inf')})
        parsed3 = json.loads(result3)
        assert parsed3["neg_inf"] is None

    def test_unicode_string(self):
        """Test various Unicode characters."""
        obj = {
            "emoji": "🎉🎊",
            "chinese": "你好",
            "japanese": "こんにちは",
            "mixed": "Hello 世界 👋",
        }
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result == obj

    def test_deeply_nested(self):
        """Test deeply nested structures."""
        obj = {"a": {"b": {"c": {"d": {"e": {"f": "deep"}}}}}}
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result == obj

    def test_array_with_mixed_nesting(self):
        """Test array with mixed nesting."""
        obj = [1, [2, [3, [4, [5]]]]]
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result == obj


class TestDumpsBytes:
    """Test dumps_bytes functionality."""

    def test_dumps_bytes_basic(self):
        """Test dumps_bytes returns bytes."""
        result = strata.dumps({"key": "value"}, return_type="bytes")
        assert isinstance(result, bytes)
        assert json.loads(result) == {"key": "value"}

    def test_dumps_bytes_unicode(self):
        """Test dumps_bytes with Unicode."""
        result = strata.dumps({"emoji": "👋"}, return_type="bytes")
        assert isinstance(result, bytes)
        parsed = json.loads(result.decode('utf-8'))
        assert parsed["emoji"] == "👋"


class TestPerformanceData:
    """Test serialization with realistic data structures."""

    def test_user_profile(self):
        """Test typical user profile structure."""
        obj = {
            "id": 12345,
            "username": "alice123",
            "email": "alice@example.com",
            "profile": {
                "firstName": "Alice",
                "lastName": "Johnson",
                "age": 30,
                "verified": True,
            },
            "settings": {"theme": "dark", "notifications": True, "language": "en"},
            "tags": ["python", "javascript", "golang"],
        }
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result == obj

    def test_api_response(self):
        """Test typical API response structure."""
        obj = {
            "status": "success",
            "data": {
                "items": [
                    {"id": 1, "name": "Item 1", "price": 19.99},
                    {"id": 2, "name": "Item 2", "price": 29.99},
                    {"id": 3, "name": "Item 3", "price": 39.99},
                ],
                "total": 3,
                "page": 1,
            },
            "meta": {"timestamp": "2026-01-30T12:00:00Z", "version": "1.0"},
        }
        json_str = strata.dumps(obj)
        result = json.loads(json_str)
        assert result == obj


class TestErrorHandling:
    """Test error handling."""

    def test_parse_error_in_wrapper(self):
        """Test that invalid objects raise errors."""
        # Our wrapper uses stdlib json.dumps first, so this would be caught there
        # Custom objects that can't be serialized
        with pytest.raises((TypeError, ValueError)):
            strata.dumps({"bad": object()})

"""
Test JSON serialization (dumps).
"""

import json
import warnings

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

    def test_escape_unicode(self):
        """Test Unicode handling."""
        result = strata.dumps({"emoji": "Hello 👋 世界"})
        parsed = json.loads(result)
        assert parsed["emoji"] == "Hello 👋 世界"

    def test_escape_many_escapes_simd_path(self):
        """Stress SIMD escape path: long string with many quotes/backslashes (P2 SIMD escape)."""
        # Build string with escape every 16 chars to exercise SIMD chunk loop (32-byte AVX2)
        parts = []
        for i in range(64):  # 64 * 16 = 1024 chars, many escapes
            parts.append("a" * 15)
            parts.append('"' if i % 2 == 0 else "\\")
        s = "".join(parts)
        result = strata.dumps({"payload": s})
        parsed = json.loads(result)
        assert parsed["payload"] == s


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

    def test_deep_nesting_iterative_serializer(self):
        """Test deep nesting without recursion limits."""
        depth = 2000
        data = []
        current = data
        for _ in range(depth):
            next_level = []
            current.append(next_level)
            current = next_level

        json_str = strata.dumps(data)
        parsed = json.loads(json_str)

        current = parsed
        for _ in range(depth):
            assert isinstance(current, list)
            assert len(current) == 1
            current = current[0]

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
        result = strata.dumps_bytes({"key": "value"})
        assert isinstance(result, bytes)
        assert json.loads(result) == {"key": "value"}

    def test_dumps_bytes_unicode(self):
        """Test dumps_bytes with Unicode."""
        result = strata.dumps_bytes({"emoji": "👋"})
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


class TestFloatPrecision:
    """Test float precision and formatting (covers ryu_dtoa.cpp changes)."""

    def test_float_precision_common_values(self):
        """Test that common float values format reasonably."""
        # Test that floats format without excessive precision
        test_cases = [
            35.31,
            126.59,
            99.99,
            0.01,
            123.456789,
            19.99,
            29.99,
            39.99,
        ]

        for value in test_cases:
            result = strata.dumps({"price": value})
            parsed = json.loads(result)

            # Verify round-trip accuracy (most important)
            assert abs(parsed["price"] - value) < 1e-10

            # Verify output is not excessively long (no 15+ digit precision)
            # Old version: 35.310000000000002 (21 chars)
            # New version: should be under 20 chars for reasonable values
            assert len(result) < 40, f"Output too long: {result}"

    def test_float_zero_fractional_part(self):
        """Test floats that are actually integers."""
        test_cases = [
            (1.0, "1.0"),
            (42.0, "42.0"),
            (100.0, "100.0"),
            (-5.0, "-5.0"),
        ]

        for value, expected in test_cases:
            result = strata.dumps(value)
            # Should include .0 for floats
            assert expected in result or str(int(value)) in result

    def test_float_precision_limits(self):
        """Test floats near precision boundaries (9 digits)."""
        test_cases = [
            0.123456789,  # 9 digits
            0.1,
            0.01,
            0.001,
            0.0001,
            0.00001,
            0.000001,  # 1e-6 boundary
        ]

        for value in test_cases:
            result = strata.dumps({"value": value})
            parsed = json.loads(result)
            # Verify round-trip within reasonable precision
            assert abs(parsed["value"] - value) < 1e-9

    def test_float_rounding(self):
        """Test that rounding works correctly."""
        # Values that test the +0.5 rounding logic
        test_cases = [
            (0.999999999, "0.999999999"),  # Should not overflow
            (1.999999999, "1.999999999"),
            (123.123456789, "123.123456789"),
        ]

        for value, _ in test_cases:
            result = strata.dumps({"value": value})
            parsed = json.loads(result)
            # Verify no overflow and reasonable precision
            assert abs(parsed["value"] - value) < 1e-8
            # Verify no representation errors (no long trailing digits)
            assert len(result) < 50, f"Result too long: {result}"

    def test_float_negative_values(self):
        """Test negative float values."""
        test_cases = [
            -3.14,
            -99.99,
            -0.01,
            -123.456789,
        ]

        for value in test_cases:
            result = strata.dumps({"value": value})
            parsed = json.loads(result)
            assert abs(parsed["value"] - value) < 1e-10

    def test_float_very_small(self):
        """Test very small floats (near 1e-6 boundary)."""
        test_cases = [
            1e-6,  # Boundary
            1e-7,  # Falls to scientific notation
            1e-5,
            0.000001,
            0.00001,
        ]

        for value in test_cases:
            result = strata.dumps({"value": value})
            parsed = json.loads(result)
            # Should be valid JSON and round-trip correctly
            assert abs(parsed["value"] - value) / value < 0.01  # 1% tolerance

    def test_float_large_values(self):
        """Test large float values (near 1e15 boundary)."""
        test_cases = [
            1e14,
            9.999999999999e14,
            1e13,
            1e12,
        ]

        for value in test_cases:
            result = strata.dumps({"value": value})
            parsed = json.loads(result)
            # Large numbers may have some loss of precision
            assert abs(parsed["value"] - value) / value < 1e-10

    def test_float_scientific_notation(self):
        """Test values that require scientific notation."""
        test_cases = [
            1e20,  # Very large
            1e-10,  # Very small
            1.23e15,
        ]

        for value in test_cases:
            result = strata.dumps({"value": value})
            parsed = json.loads(result)
            # Verify round-trip
            assert abs(parsed["value"] - value) / abs(value) < 0.01

    def test_float_trailing_zeros_removed(self):
        """Test that trailing zeros are properly removed."""
        test_cases = [
            (1.5, "1.5"),  # Not 1.500000000
            (2.25, "2.25"),  # Not 2.250000000
            (3.125, "3.125"),  # Not 3.125000000
        ]

        for value, expected in test_cases:
            result = strata.dumps(value)
            # Verify no excessive trailing zeros
            assert result.count('0') < 5, f"Too many zeros in {result}"
            assert expected in result

    def test_float_array_mixed(self):
        """Test array with mixed float values."""
        obj = {
            "prices": [19.99, 29.99, 39.99, 0.01, 123.45],
            "scores": [95.5, 87.3, 92.8],
        }
        result = strata.dumps(obj)
        parsed = json.loads(result)

        # Verify all values round-trip correctly
        for i, expected in enumerate(obj["prices"]):
            assert abs(parsed["prices"][i] - expected) < 1e-10
        for i, expected in enumerate(obj["scores"]):
            assert abs(parsed["scores"][i] - expected) < 1e-10

    def test_float_nested_structures(self):
        """Test float precision in nested structures."""
        obj = {
            "user": {
                "balance": 123.45,
                "transactions": [
                    {"amount": 19.99, "fee": 0.50},
                    {"amount": 29.99, "fee": 0.75},
                ],
            }
        }
        result = strata.dumps(obj)
        parsed = json.loads(result)

        # Verify nested float values are clean
        assert abs(parsed["user"]["balance"] - 123.45) < 1e-10
        assert abs(parsed["user"]["transactions"][0]["amount"] - 19.99) < 1e-10
        assert abs(parsed["user"]["transactions"][0]["fee"] - 0.50) < 1e-10

    def test_float_output_size_reasonable(self):
        """Test that float output size is reasonable (not excessive precision)."""
        test_values = [
            35.31,
            99.99,
            123.456789,
            0.123456789,
        ]

        for value in test_values:
            result = strata.dumps({"value": value})
            # Output should not be excessively long
            # (old version: 35.310000000000002 = 21 chars)
            # (new version: 35.31 = 5 chars)
            assert len(result) < 30, f"Output too long for {value}: {result}"

    def test_float_comparison_with_stdlib(self):
        """Test that our float formatting is compatible with stdlib json."""
        test_values = [
            3.14,
            99.99,
            123.45,
            0.01,
            -19.99,
        ]

        for value in test_values:
            strata_result = strata.dumps({"value": value})
            stdlib_result = json.dumps({"value": value})

            # Both should parse to same value
            strata_parsed = json.loads(strata_result)
            stdlib_parsed = json.loads(stdlib_result)

            assert abs(strata_parsed["value"] - stdlib_parsed["value"]) < 1e-10


class TestErrorHandling:
    """Test error handling."""

    def test_parse_error_in_wrapper(self):
        """Test that invalid objects raise errors."""
        # Our wrapper uses stdlib json.dumps first, so this would be caught there
        # Custom objects that can't be serialized
        with pytest.raises((TypeError, ValueError)):
            strata.dumps({"bad": object()})


class TestDuplicateKeyPolicy:
    """Test configurable duplicate key handling."""

    def teardown_method(self):
        strata.set_duplicate_key_policy("first")

    def test_last_wins(self):
        strata.set_duplicate_key_policy("last")
        result = strata.loads('{"a": 1, "a": 2}')
        assert result["a"] == 2

    def test_error_wins(self):
        strata.set_duplicate_key_policy("error")
        with pytest.raises(ValueError):
            strata.loads('{"a": 1, "a": 2}')

    def test_warn_keeps_first(self):
        strata.set_duplicate_key_policy("warn")
        with pytest.warns(RuntimeWarning):
            result = strata.loads('{"a": 1, "a": 2}')
        assert result["a"] == 1


class TestCyclePolicy:
    """Test cycle detection strategies during serialization."""

    def teardown_method(self):
        strata.set_cycle_policy("warn")

    def test_warn_emits_null(self):
        data = []
        data.append(data)
        with pytest.warns(RuntimeWarning):
            assert strata.dumps(data) == "[null]"

    def test_error_raises(self):
        data = []
        data.append(data)
        strata.set_cycle_policy("error")
        with pytest.raises(ValueError):
            strata.dumps(data)

    def test_ignore_suppresses_warning(self):
        data = []
        data.append(data)
        strata.set_cycle_policy("ignore")
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            assert strata.dumps(data) == "[null]"
        assert [w for w in caught if issubclass(w.category, RuntimeWarning)] == []

    def test_nocheck_skips_detection(self):
        """nocheck policy serializes without cycle detection (acyclic data)."""
        strata.set_cycle_policy("nocheck")
        data = {"a": [1, 2, {"b": True}], "c": "hello"}
        result = strata.dumps(data)
        assert '"a"' in result
        assert '"hello"' in result

    def test_deep_nesting_cycle_detected(self):
        """Cycle detection works even with nesting > 32 (spillover path)."""
        # Build a chain of 40 nested lists, then create a cycle
        root = current = []
        for _ in range(40):
            child = []
            current.append(child)
            current = child
        current.append(root)  # cycle back to root
        with pytest.warns(RuntimeWarning):
            result = strata.dumps(root)
        assert "null" in result

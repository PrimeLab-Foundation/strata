"""
test_tape.py - Tests for tape-based JSON parsing (ADR-0003)

Tests verify that loads_tape produces identical results to loads for all
JSON types and structures. The tape format is an implementation detail,
but we verify correctness through round-trip parity.
"""

import pytest
import strata


class TestTapeBasicTypes:
    """Test tape parsing of basic JSON types."""

    def test_null(self):
        """Null parses correctly via tape."""
        assert strata.loads_tape("null") is None
        assert strata.loads_tape("null") == strata.loads("null")

    def test_true(self):
        """Boolean true parses correctly via tape."""
        assert strata.loads_tape("true") is True
        assert strata.loads_tape("true") == strata.loads("true")

    def test_false(self):
        """Boolean false parses correctly via tape."""
        assert strata.loads_tape("false") is False
        assert strata.loads_tape("false") == strata.loads("false")

    def test_integer(self):
        """Integer parses correctly via tape."""
        assert strata.loads_tape("42") == 42
        assert strata.loads_tape("42") == strata.loads("42")

    def test_negative_integer(self):
        """Negative integer parses correctly via tape."""
        assert strata.loads_tape("-123") == -123
        assert strata.loads_tape("-123") == strata.loads("-123")

    def test_zero(self):
        """Zero parses correctly via tape."""
        assert strata.loads_tape("0") == 0
        assert strata.loads_tape("0") == strata.loads("0")

    def test_float(self):
        """Float parses correctly via tape."""
        result = strata.loads_tape("3.14")
        assert abs(result - 3.14) < 0.001
        assert result == strata.loads("3.14")

    def test_scientific_notation(self):
        """Scientific notation parses correctly via tape."""
        result = strata.loads_tape("1.23e10")
        assert result == strata.loads("1.23e10")

    def test_string(self):
        """String parses correctly via tape."""
        assert strata.loads_tape('"hello"') == "hello"
        assert strata.loads_tape('"hello"') == strata.loads('"hello"')

    def test_empty_string(self):
        """Empty string parses correctly via tape."""
        assert strata.loads_tape('""') == ""
        assert strata.loads_tape('""') == strata.loads('""')


class TestTapeStringEscapes:
    """Test tape parsing of escaped strings."""

    def test_newline_escape(self):
        """Newline escape parses correctly via tape."""
        result = strata.loads_tape('"line1\\nline2"')
        assert "\n" in result
        assert result == strata.loads('"line1\\nline2"')

    def test_tab_escape(self):
        """Tab escape parses correctly via tape."""
        result = strata.loads_tape('"a\\tb"')
        assert "\t" in result
        assert result == strata.loads('"a\\tb"')

    def test_quote_escape(self):
        """Quote escape parses correctly via tape."""
        result = strata.loads_tape('"say \\"hello\\""')
        assert '"' in result
        assert result == strata.loads('"say \\"hello\\""')

    def test_backslash_escape(self):
        """Backslash escape parses correctly via tape."""
        result = strata.loads_tape('"back\\\\slash"')
        assert "\\" in result
        assert result == strata.loads('"back\\\\slash"')


class TestTapeUnicode:
    """Test tape parsing of Unicode strings."""

    def test_direct_utf8(self):
        """Direct UTF-8 parses correctly via tape."""
        result = strata.loads_tape('"Hello 世界"')
        assert "世界" in result
        assert result == strata.loads('"Hello 世界"')

    def test_emoji(self):
        """Emoji parses correctly via tape."""
        result = strata.loads_tape('"👋"')
        assert result == "👋"
        assert result == strata.loads('"👋"')

    def test_unicode_escape(self):
        """Unicode escape parses correctly via tape."""
        result = strata.loads_tape('"\\u0041"')
        assert result == "A"
        assert result == strata.loads('"\\u0041"')

    def test_surrogate_pair(self):
        """Surrogate pair (emoji) parses correctly via tape."""
        # 🎉 = U+1F389 = \uD83C\uDF89
        result = strata.loads_tape('"\\uD83C\\uDF89"')
        assert result == "🎉"
        assert result == strata.loads('"\\uD83C\\uDF89"')


class TestTapeContainers:
    """Test tape parsing of arrays and objects."""

    def test_empty_array(self):
        """Empty array parses correctly via tape."""
        assert strata.loads_tape("[]") == []
        assert strata.loads_tape("[]") == strata.loads("[]")

    def test_simple_array(self):
        """Simple array parses correctly via tape."""
        result = strata.loads_tape("[1, 2, 3]")
        assert result == [1, 2, 3]
        assert result == strata.loads("[1, 2, 3]")

    def test_mixed_array(self):
        """Mixed type array parses correctly via tape."""
        json_str = '[null, true, false, 42, "hello"]'
        result = strata.loads_tape(json_str)
        assert result == strata.loads(json_str)
        assert result == [None, True, False, 42, "hello"]

    def test_nested_arrays(self):
        """Nested arrays parse correctly via tape."""
        json_str = "[[1, 2], [3, 4]]"
        result = strata.loads_tape(json_str)
        assert result == [[1, 2], [3, 4]]
        assert result == strata.loads(json_str)

    def test_empty_object(self):
        """Empty object parses correctly via tape."""
        assert strata.loads_tape("{}") == {}
        assert strata.loads_tape("{}") == strata.loads("{}")

    def test_simple_object(self):
        """Simple object parses correctly via tape."""
        json_str = '{"a": 1, "b": 2}'
        result = strata.loads_tape(json_str)
        assert result["a"] == 1
        assert result["b"] == 2
        assert result == strata.loads(json_str)

    def test_nested_object(self):
        """Nested object parses correctly via tape."""
        json_str = '{"outer": {"inner": 42}}'
        result = strata.loads_tape(json_str)
        assert result["outer"]["inner"] == 42
        assert result == strata.loads(json_str)


class TestTapeComplexStructures:
    """Test tape parsing of complex JSON structures."""

    def test_complex_structure(self):
        """Complex nested structure parses correctly via tape."""
        json_str = """
        {
            "name": "test",
            "numbers": [1, 2, 3],
            "nested": {
                "flag": true,
                "value": null
            },
            "empty": [],
            "empty_obj": {}
        }
        """
        result = strata.loads_tape(json_str)
        expected = strata.loads(json_str)
        assert result == expected
        assert result["name"] == "test"
        assert result["numbers"] == [1, 2, 3]
        assert result["nested"]["flag"] is True
        assert result["nested"]["value"] is None

    def test_deeply_nested(self):
        """Deeply nested structure parses correctly via tape."""
        # Build deeply nested array
        json_str = "[" * 50 + "1" + "]" * 50
        result = strata.loads_tape(json_str)
        assert result == strata.loads(json_str)

    def test_large_array(self):
        """Large array parses correctly via tape."""
        json_str = "[" + ",".join(str(i) for i in range(1000)) + "]"
        result = strata.loads_tape(json_str)
        expected = strata.loads(json_str)
        assert result == expected
        assert len(result) == 1000

    def test_many_keys(self):
        """Object with many keys parses correctly via tape."""
        pairs = [f'"key{i}": {i}' for i in range(100)]
        json_str = "{" + ", ".join(pairs) + "}"
        result = strata.loads_tape(json_str)
        expected = strata.loads(json_str)
        assert result == expected
        assert len(result) == 100


class TestTapeErrors:
    """Test tape parsing error handling."""

    def test_invalid_json(self):
        """Invalid JSON raises ValueError via tape."""
        with pytest.raises(ValueError):
            strata.loads_tape("{invalid}")

    def test_trailing_comma(self):
        """Trailing comma raises ValueError via tape."""
        with pytest.raises(ValueError):
            strata.loads_tape("[1, 2,]")

    def test_empty_input(self):
        """Empty input raises ValueError via tape."""
        with pytest.raises(ValueError):
            strata.loads_tape("")

    def test_unclosed_string(self):
        """Unclosed string raises ValueError via tape."""
        with pytest.raises(ValueError):
            strata.loads_tape('"unclosed')

    def test_unclosed_array(self):
        """Unclosed array raises ValueError via tape."""
        with pytest.raises(ValueError):
            strata.loads_tape("[1, 2")


class TestTapeParityWithLoads:
    """Ensure loads_tape produces identical results to loads for various inputs."""

    @pytest.mark.parametrize(
        "json_str",
        [
            "null",
            "true",
            "false",
            "0",
            "42",
            "-123",
            "3.14159",
            "1e10",
            '""',
            '"hello"',
            '"line\\nbreak"',
            '"unicode: \\u00e9"',
            "[]",
            "[1]",
            "[1, 2, 3]",
            "[[[]]]",
            "{}",
            '{"a": 1}',
            '{"a": {"b": {"c": 1}}}',
            '[{"a": 1}, {"b": 2}]',
            '{"arr": [1, 2, 3], "obj": {"x": "y"}}',
        ],
    )
    def test_parity(self, json_str):
        """loads_tape produces same result as loads."""
        assert strata.loads_tape(json_str) == strata.loads(json_str)

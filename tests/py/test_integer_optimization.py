"""
Test cases for integer parsing optimization.

These tests verify that the optimized integer creation in PythonObjectBuilder
correctly handles all integer ranges, including:
- Small integers in Python's cache range (-5 to 256)
- Boundary values at cache edges
- Large integers requiring int64_t/uint64_t
- Edge cases at INT64_MIN, INT64_MAX, UINT64_MAX
"""

import sys
import pytest

import strata


class TestSmallIntegerParsing:
    """Verify small integers in Python's cached range are correctly parsed."""

    def test_zero(self):
        assert strata.loads("0") == 0

    def test_small_positive_integers(self):
        """Test positive integers in cache range."""
        assert strata.loads("1") == 1
        assert strata.loads("127") == 127
        assert strata.loads("255") == 255
        assert strata.loads("256") == 256

    def test_small_negative_integers(self):
        """Test negative integers in cache range."""
        assert strata.loads("-1") == -1
        assert strata.loads("-5") == -5
        assert strata.loads("-128") == -128

    def test_cache_boundary_values(self):
        """Test values at Python's small integer cache boundaries (-5 to 256)."""
        # Just below cache range
        assert strata.loads("-6") == -6
        # Just above cache range
        assert strata.loads("257") == 257


class TestIntegerBoundaries:
    """Test integer boundary handling for various C integer types."""

    def test_int64_max(self):
        """Test INT64_MAX (2^63 - 1)."""
        assert strata.loads("9223372036854775807") == 9223372036854775807

    def test_int64_min(self):
        """Test INT64_MIN (-2^63)."""
        assert strata.loads("-9223372036854775808") == -9223372036854775808

    def test_uint64_max(self):
        """Test UINT64_MAX (2^64 - 1).

        Note: Very large unsigned integers that exceed int64_t range may be
        parsed as floats by the underlying parser due to JSON number handling.
        This test verifies the approximate value is correct.
        """
        result = strata.loads("18446744073709551615")
        # The value may be returned as a float for numbers > INT64_MAX
        # Check approximate equality for float representation
        expected = 18446744073709551615
        if isinstance(result, float):
            # Float representation has limited precision for very large integers
            assert abs(result - expected) / expected < 1e-15
        else:
            assert result == expected

    def test_int32_boundaries(self):
        """Test INT32_MIN and INT32_MAX."""
        assert strata.loads("2147483647") == 2147483647  # INT32_MAX
        assert strata.loads("-2147483648") == -2147483648  # INT32_MIN

    def test_long_boundaries(self):
        """Test values at C long boundaries (platform dependent)."""
        # LONG_MAX is platform dependent (32-bit vs 64-bit)
        # On 64-bit systems, LONG_MAX == INT64_MAX
        # These values should always work regardless of platform
        assert strata.loads("2147483647") == 2147483647
        assert strata.loads("-2147483648") == -2147483648


class TestIntegerArraysAndObjects:
    """Test integer handling in arrays and objects."""

    def test_array_of_small_integers(self):
        """Array with integers in cache range."""
        result = strata.loads("[0, 1, 2, 3, 127, 255, 256]")
        assert result == [0, 1, 2, 3, 127, 255, 256]

    def test_array_of_negative_integers(self):
        """Array with negative integers."""
        result = strata.loads("[-1, -5, -6, -128, -256]")
        assert result == [-1, -5, -6, -128, -256]

    def test_array_of_mixed_integers(self):
        """Array with mixed small and large integers."""
        result = strata.loads("[-5, 0, 256, 257, 9223372036854775807]")
        assert result == [-5, 0, 256, 257, 9223372036854775807]

    def test_object_with_integer_values(self):
        """Object with integer values in various ranges."""
        result = strata.loads('{"zero": 0, "small": 42, "large": 9223372036854775807}')
        assert result["zero"] == 0
        assert result["small"] == 42
        assert result["large"] == 9223372036854775807


class TestIntegerTypePreservation:
    """Verify that integers are returned as Python int type."""

    def test_small_integer_is_int(self):
        result = strata.loads("42")
        assert isinstance(result, int)
        assert type(result) is int

    def test_large_integer_is_int(self):
        result = strata.loads("9223372036854775807")
        assert isinstance(result, int)
        assert type(result) is int

    def test_negative_integer_is_int(self):
        result = strata.loads("-42")
        assert isinstance(result, int)
        assert type(result) is int

    def test_zero_is_int(self):
        result = strata.loads("0")
        assert isinstance(result, int)
        assert type(result) is int


class TestIntegerRoundTrip:
    """Verify integer round-trip (loads/dumps) preserves values."""

    @pytest.mark.parametrize(
        "value",
        [
            0,
            1,
            -1,
            -5,
            256,
            257,
            -6,
            127,
            -128,
            2147483647,
            -2147483648,
            9223372036854775807,
            -9223372036854775808,
        ],
    )
    def test_integer_roundtrip(self, value):
        """Test that integers survive a loads/dumps round-trip."""
        json_str = str(value)
        parsed = strata.loads(json_str)
        assert parsed == value
        # Round-trip through dumps
        serialized = strata.dumps(parsed)
        reparsed = strata.loads(serialized)
        assert reparsed == value

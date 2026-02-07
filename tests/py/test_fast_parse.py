"""
Test parsing performance edge cases.

Parity tests for tests/cpp/test_fast_parse.cpp
Tests integer and float parsing across various ranges and edge cases.
"""

import json
import sys

import pytest

import strata


class TestSmallIntegerParsing:
    """Test small integer parsing (0-9999)."""

    @pytest.mark.parametrize("value", list(range(10)))
    def test_single_digit(self, value):
        """Test single digit integers (0-9)."""
        assert strata.loads(str(value)) == value

    @pytest.mark.parametrize("value", [10, 11, 19, 20, 42, 50, 73, 99])
    def test_two_digit(self, value):
        """Test two digit integers (10-99)."""
        assert strata.loads(str(value)) == value

    @pytest.mark.parametrize("value", [100, 123, 256, 500, 789, 999])
    def test_three_digit(self, value):
        """Test three digit integers (100-999)."""
        assert strata.loads(str(value)) == value

    @pytest.mark.parametrize("value", [1000, 1234, 2048, 4096, 5678, 9999])
    def test_four_digit(self, value):
        """Test four digit integers (1000-9999) - SWAR path."""
        assert strata.loads(str(value)) == value


class TestMediumIntegerParsing:
    """Test medium integer parsing (5-8 digits)."""

    @pytest.mark.parametrize(
        "value",
        [
            12345,  # 5 digits
            123456,  # 6 digits
            1234567,  # 7 digits
            12345678,  # 8 digits (SWAR 8-digit path)
            11111111,
            99999999,
            50000000,
            19851231,  # Date-like
        ],
    )
    def test_medium_integers(self, value):
        """Test medium integers (5-8 digits)."""
        assert strata.loads(str(value)) == value


class TestLargeIntegerParsing:
    """Test large integer parsing (9+ digits)."""

    @pytest.mark.parametrize(
        "value",
        [
            123456789,  # 9 digits
            1234567890,  # 10 digits
            12345678901,  # 11 digits
            2147483647,  # INT32_MAX
            4294967295,  # UINT32_MAX
            9007199254740991,  # JavaScript MAX_SAFE_INTEGER
            9223372036854775807,  # INT64_MAX
        ],
    )
    def test_large_integers(self, value):
        """Test large integers (9+ digits)."""
        result = strata.loads(str(value))
        # Large numbers may be parsed as float, check approximate equality
        if isinstance(result, float):
            assert abs(result - value) / value < 1e-15
        else:
            assert result == value


class TestNegativeIntegerParsing:
    """Test negative integer parsing."""

    @pytest.mark.parametrize(
        "value",
        [
            -1,
            -9,
            -10,
            -99,
            -100,
            -999,
            -1000,
            -9999,
            -12345,
            -123456,
            -1234567,
            -12345678,
            -123456789,
            -2147483648,  # INT32_MIN
            -9223372036854775808,  # INT64_MIN
        ],
    )
    def test_negative_integers(self, value):
        """Test negative integers."""
        result = strata.loads(str(value))
        if isinstance(result, float):
            assert abs(result - value) / abs(value) < 1e-15
        else:
            assert result == value


class TestFloatParsing:
    """Test float parsing."""

    @pytest.mark.parametrize(
        "value",
        [
            0.0,
            1.0,
            -1.0,
            3.14,
            2.71828,
            123.456,
            0.123456789,
            -987.654321,
        ],
    )
    def test_common_floats(self, value):
        """Test common float values."""
        result = strata.loads(str(value))
        assert abs(result - value) < 1e-10

    @pytest.mark.parametrize(
        "value",
        [
            1.5e10,
            -1.5e10,
            1.23e-5,
            -1.23e-5,
            1e100,
            1e-100,
        ],
    )
    def test_scientific_notation(self, value):
        """Test scientific notation parsing."""
        result = strata.loads(str(value))
        assert abs(result - value) / abs(value) < 1e-10


class TestEdgeCases:
    """Test parsing edge cases."""

    def test_leading_zeros_rejected(self):
        """Leading zeros (except 0 itself) should be rejected."""
        with pytest.raises(ValueError):
            strata.loads("01")
        with pytest.raises(ValueError):
            strata.loads("00")
        with pytest.raises(ValueError):
            strata.loads("007")

    def test_zero_variants(self):
        """Test valid zero variants."""
        assert strata.loads("0") == 0
        assert strata.loads("-0") == 0
        assert strata.loads("0.0") == 0.0

    def test_plus_sign_rejected(self):
        """Plus sign prefix is not valid JSON."""
        with pytest.raises(ValueError):
            strata.loads("+1")
        with pytest.raises(ValueError):
            strata.loads("+0")

    def test_decimal_point_must_have_fraction(self):
        """Decimal point must be followed by digits."""
        with pytest.raises(ValueError):
            strata.loads("1.")
        with pytest.raises(ValueError):
            strata.loads("123.")

    def test_exponent_must_have_digits(self):
        """Exponent marker must be followed by digits."""
        with pytest.raises(ValueError):
            strata.loads("1e")
        with pytest.raises(ValueError):
            strata.loads("1E")
        with pytest.raises(ValueError):
            strata.loads("1e+")
        with pytest.raises(ValueError):
            strata.loads("1e-")

    def test_number_followed_by_non_delimiter(self):
        """Number followed by non-delimiter should fail."""
        with pytest.raises(ValueError):
            strata.loads("123abc")
        with pytest.raises(ValueError):
            strata.loads("1.5xyz")


class TestIntegerArrayParsing:
    """Test parsing arrays of integers."""

    def test_small_integer_array(self):
        """Test array of small integers."""
        values = [0, 1, 2, 3, 127, 255, 256, 1000, 9999]
        json_str = json.dumps(values)
        result = strata.loads(json_str)
        assert result == values

    def test_large_integer_array(self):
        """Test array of large integers."""
        values = [12345678, 87654321, 2147483647, 9007199254740991]
        json_str = json.dumps(values)
        result = strata.loads(json_str)
        for i, expected in enumerate(values):
            if isinstance(result[i], float):
                assert abs(result[i] - expected) / expected < 1e-15
            else:
                assert result[i] == expected

    def test_negative_integer_array(self):
        """Test array of negative integers."""
        values = [-1, -99, -999, -9999, -123456, -2147483648]
        json_str = json.dumps(values)
        result = strata.loads(json_str)
        for i, expected in enumerate(values):
            if isinstance(result[i], float):
                assert abs(result[i] - expected) / abs(expected) < 1e-15
            else:
                assert result[i] == expected


class TestMixedNumberParsing:
    """Test parsing mixed number types."""

    def test_mixed_int_float_array(self):
        """Test array with mixed integers and floats."""
        json_str = "[1, 2.5, -3, 4.0, 100, 99.99]"
        result = strata.loads(json_str)
        assert len(result) == 6
        assert result[0] == 1
        assert abs(result[1] - 2.5) < 1e-10
        assert result[2] == -3
        assert abs(result[3] - 4.0) < 1e-10
        assert result[4] == 100
        assert abs(result[5] - 99.99) < 1e-10

    def test_object_with_numbers(self):
        """Test object with various number types."""
        json_str = '{"int": 42, "float": 3.14, "neg": -123, "sci": 1.5e10}'
        result = strata.loads(json_str)
        assert result["int"] == 42
        assert abs(result["float"] - 3.14) < 1e-10
        assert result["neg"] == -123
        assert abs(result["sci"] - 1.5e10) < 1e-5


class TestRoundTrip:
    """Test round-trip parsing and serialization."""

    @pytest.mark.parametrize(
        "value",
        [
            0,
            1,
            -1,
            42,
            -42,
            1234567890,
            -1234567890,
            3.14,
            -3.14,
            0.123456789,
            1e10,
            -1e-5,
        ],
    )
    def test_number_roundtrip(self, value):
        """Test that numbers survive a loads/dumps round-trip."""
        # Start with JSON string
        json_str = json.dumps(value)
        # Parse with strata
        parsed = strata.loads(json_str)
        # Serialize with strata
        serialized = strata.dumps(parsed)
        # Parse with stdlib to compare
        reparsed = json.loads(serialized)

        if isinstance(value, float) or isinstance(parsed, float):
            if value == 0:
                assert abs(reparsed) < 1e-10
            else:
                assert abs(reparsed - value) / abs(value) < 1e-10
        else:
            if isinstance(reparsed, float):
                assert abs(reparsed - value) / abs(value) < 1e-15
            else:
                assert reparsed == value


class TestWhitespaceHandling:
    """Test handling of whitespace around numbers."""

    def test_number_with_leading_whitespace(self):
        """Test number with leading whitespace."""
        assert strata.loads("   42") == 42
        assert abs(strata.loads("  3.14") - 3.14) < 1e-10

    def test_number_with_trailing_whitespace(self):
        """Test number with trailing whitespace."""
        assert strata.loads("42   ") == 42
        assert abs(strata.loads("3.14  ") - 3.14) < 1e-10

    def test_number_with_surrounding_whitespace(self):
        """Test number with surrounding whitespace."""
        assert strata.loads("  42  ") == 42
        assert abs(strata.loads("  3.14  ") - 3.14) < 1e-10


class TestBoundaryValues:
    """Test boundary values at type limits."""

    def test_int32_boundaries(self):
        """Test INT32 boundaries."""
        assert strata.loads("2147483647") == 2147483647  # INT32_MAX
        assert strata.loads("-2147483648") == -2147483648  # INT32_MIN

    def test_uint32_boundary(self):
        """Test UINT32 boundary."""
        assert strata.loads("4294967295") == 4294967295  # UINT32_MAX

    def test_int64_boundaries(self):
        """Test INT64 boundaries."""
        result_max = strata.loads("9223372036854775807")
        expected_max = 9223372036854775807
        if isinstance(result_max, float):
            assert abs(result_max - expected_max) / expected_max < 1e-15
        else:
            assert result_max == expected_max

        result_min = strata.loads("-9223372036854775808")
        expected_min = -9223372036854775808
        if isinstance(result_min, float):
            assert abs(result_min - expected_min) / abs(expected_min) < 1e-15
        else:
            assert result_min == expected_min

    def test_max_safe_integer(self):
        """Test JavaScript MAX_SAFE_INTEGER."""
        result = strata.loads("9007199254740991")
        if isinstance(result, float):
            # This should be exactly representable as float
            assert result == 9007199254740991.0
        else:
            assert result == 9007199254740991

"""
Test float precision and formatting.

Parity tests for tests/cpp/test_float_precision.cpp
Validates that strata.dumps produces clean, round-trip correct float output.
"""

import json
import math

import pytest

import strata


class TestSpecialValues:
    """Test special float values (NaN, Inf)."""

    def test_nan_serializes_to_null(self):
        """NaN should be serialized as null (JSON spec)."""
        result = strata.dumps({"value": float("nan")})
        parsed = json.loads(result)
        assert parsed["value"] is None

    def test_inf_serializes_to_null(self):
        """Positive infinity should be serialized as null."""
        result = strata.dumps({"value": float("inf")})
        parsed = json.loads(result)
        assert parsed["value"] is None

    def test_neg_inf_serializes_to_null(self):
        """Negative infinity should be serialized as null."""
        result = strata.dumps({"value": float("-inf")})
        parsed = json.loads(result)
        assert parsed["value"] is None

    def test_zero(self):
        """Zero should format compactly."""
        result = strata.dumps(0.0)
        # Should be "0" or "0.0", both are valid
        assert result in ["0", "0.0"]


class TestCommonValues:
    """Test common float values for round-trip accuracy."""

    @pytest.mark.parametrize(
        "value",
        [
            35.31,
            99.99,
            19.99,
            29.99,
            39.99,
            0.01,
            0.001,
            126.59,
            123.456789,
        ],
    )
    def test_common_roundtrip(self, value):
        """Common float values should round-trip correctly."""
        result = strata.dumps({"value": value})
        parsed = json.loads(result)
        assert abs(parsed["value"] - value) < 1e-10

    @pytest.mark.parametrize(
        "value",
        [
            35.31,
            99.99,
            19.99,
            29.99,
            39.99,
            0.01,
            123.456789,
        ],
    )
    def test_common_values_not_too_long(self, value):
        """Common values should not have excessive precision in output."""
        result = strata.dumps({"value": value})
        # Output should be reasonably short (not 35.310000000000002 style)
        assert len(result) < 40, f"Output too long: {result}"


class TestPrecisionLimits:
    """Test floats near precision boundaries."""

    @pytest.mark.parametrize(
        "value",
        [
            0.123456789,  # 9 digits
            0.1,
            0.01,
            0.001,
            0.0001,
            0.00001,
            0.000001,  # 1e-6 boundary
        ],
    )
    def test_precision_limits(self, value):
        """Values near precision boundaries should round-trip correctly."""
        result = strata.dumps({"value": value})
        parsed = json.loads(result)
        assert abs(parsed["value"] - value) < 1e-9


class TestIntegerFloats:
    """Test floats that are actually integers."""

    @pytest.mark.parametrize(
        "value",
        [
            1.0,
            42.0,
            100.0,
            -5.0,
            0.0,
            1000.0,
        ],
    )
    def test_integer_floats(self, value):
        """Floats with zero fractional part should format cleanly."""
        result = strata.dumps(value)
        parsed = json.loads(result)
        assert parsed == value


class TestNegativeValues:
    """Test negative float values."""

    @pytest.mark.parametrize(
        "value",
        [
            -3.14,
            -99.99,
            -0.01,
            -123.456789,
            -0.0,
        ],
    )
    def test_negative_roundtrip(self, value):
        """Negative floats should round-trip correctly."""
        result = strata.dumps({"value": value})
        parsed = json.loads(result)
        assert abs(parsed["value"] - value) < 1e-10


class TestVerySmallValues:
    """Test very small float values."""

    @pytest.mark.parametrize(
        "value",
        [
            1e-6,  # Boundary
            1e-7,  # Falls to scientific notation
            1e-5,
            0.000001,
            0.00001,
        ],
    )
    def test_very_small_roundtrip(self, value):
        """Very small values should round-trip with reasonable precision."""
        result = strata.dumps({"value": value})
        parsed = json.loads(result)
        # Allow 1% tolerance for very small values
        assert abs(parsed["value"] - value) / value < 0.01


class TestLargeValues:
    """Test large float values."""

    @pytest.mark.parametrize(
        "value",
        [
            1e14,
            9.999999999999e14,
            1e13,
            1e12,
        ],
    )
    def test_large_roundtrip(self, value):
        """Large values should round-trip with reasonable precision."""
        result = strata.dumps({"value": value})
        parsed = json.loads(result)
        # Large numbers may have some precision loss
        assert abs(parsed["value"] - value) / value < 1e-10


class TestScientificNotation:
    """Test values that require scientific notation."""

    @pytest.mark.parametrize(
        "value",
        [
            1e20,  # Very large
            1e-10,  # Very small
            1.23e15,
            -1e20,
            -1e-10,
        ],
    )
    def test_scientific_roundtrip(self, value):
        """Values requiring scientific notation should round-trip."""
        result = strata.dumps({"value": value})
        parsed = json.loads(result)
        # Allow 1% tolerance for extreme values
        assert abs(parsed["value"] - value) / abs(value) < 0.01


class TestTrailingZeros:
    """Test that trailing zeros are properly handled."""

    @pytest.mark.parametrize(
        "value,expected",
        [
            (1.5, "1.5"),
            (2.25, "2.25"),
            (3.125, "3.125"),
        ],
    )
    def test_trailing_zeros_removed(self, value, expected):
        """Trailing zeros should be properly removed."""
        result = strata.dumps(value)
        # Verify no excessive trailing zeros
        assert result.count("0") < 5, f"Too many zeros in {result}"
        # The expected substring should be present
        assert expected in result or str(value) in result


class TestFloatArrays:
    """Test arrays with float values."""

    def test_float_array_mixed(self):
        """Array with mixed float values should format correctly."""
        obj = {
            "prices": [19.99, 29.99, 39.99, 0.01, 123.45],
            "scores": [95.5, 87.3, 92.8],
        }
        result = strata.dumps(obj)
        parsed = json.loads(result)

        for i, expected in enumerate(obj["prices"]):
            assert abs(parsed["prices"][i] - expected) < 1e-10
        for i, expected in enumerate(obj["scores"]):
            assert abs(parsed["scores"][i] - expected) < 1e-10


class TestFloatNestedStructures:
    """Test float precision in nested structures."""

    def test_nested_floats(self):
        """Float values in nested structures should format correctly."""
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

        assert abs(parsed["user"]["balance"] - 123.45) < 1e-10
        assert abs(parsed["user"]["transactions"][0]["amount"] - 19.99) < 1e-10
        assert abs(parsed["user"]["transactions"][0]["fee"] - 0.50) < 1e-10


class TestStdlibCompatibility:
    """Test compatibility with stdlib json module."""

    @pytest.mark.parametrize(
        "value",
        [
            3.14,
            99.99,
            123.45,
            0.01,
            -19.99,
        ],
    )
    def test_stdlib_compatibility(self, value):
        """Float formatting should be compatible with stdlib json."""
        strata_result = strata.dumps({"value": value})
        stdlib_result = json.dumps({"value": value})

        strata_parsed = json.loads(strata_result)
        stdlib_parsed = json.loads(stdlib_result)

        assert abs(strata_parsed["value"] - stdlib_parsed["value"]) < 1e-10


class TestRoundingEdgeCases:
    """Test rounding edge cases."""

    @pytest.mark.parametrize(
        "value",
        [
            0.999999999,  # Should not overflow to 1.0
            1.999999999,
            123.123456789,
        ],
    )
    def test_rounding_edge_cases(self, value):
        """Rounding edge cases should be handled correctly."""
        result = strata.dumps({"value": value})
        parsed = json.loads(result)
        # Verify reasonable precision
        assert abs(parsed["value"] - value) < 1e-8
        # Verify output is not too long
        assert len(result) < 50, f"Result too long: {result}"

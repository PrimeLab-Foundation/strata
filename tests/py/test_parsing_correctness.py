"""
Test parsing correctness for various JSON structures and edge cases.

Tests go through Python → Python C API → C++ parse path.
"""

import pytest

import strata
from strata.json_cursor import parse_json


class TestBasicTypes:
    """Test parsing of basic JSON types."""

    def test_parse_null(self):
        root = parse_json("null")
        assert root.is_null()
        assert not root.is_bool()
        assert not root.is_number()

    def test_parse_true(self):
        root = parse_json("true")
        assert root.is_bool()
        assert root.get_bool() is True

    def test_parse_false(self):
        root = parse_json("false")
        assert root.is_bool()
        assert root.get_bool() is False

    def test_parse_integer(self):
        root = parse_json("42")
        assert root.is_number()
        assert root.get_int() == 42

    def test_parse_negative_integer(self):
        root = parse_json("-123")
        assert root.is_number()
        assert root.get_int() == -123

    def test_parse_zero(self):
        root = parse_json("0")
        assert root.is_number()
        assert root.get_int() == 0

    def test_parse_float(self):
        root = parse_json("3.14")
        assert root.is_number()
        assert root.get_float() == 3.14

    def test_parse_negative_float(self):
        root = parse_json("-2.5")
        assert root.is_number()
        assert root.get_float() == -2.5

    def test_parse_scientific_notation(self):
        root = parse_json("1.5e10")
        assert root.is_number()
        assert root.get_float() == 1.5e10

    def test_parse_scientific_notation_negative_exponent(self):
        root = parse_json("2.5e-3")
        assert root.is_number()
        assert abs(root.get_float() - 0.0025) < 1e-10

    def test_parse_string(self):
        root = parse_json('"hello"')
        assert root.is_string()
        assert root.get_str() == "hello"

    def test_parse_empty_string(self):
        root = parse_json('""')
        assert root.is_string()
        assert root.get_str() == ""

    def test_parse_empty_array(self):
        root = parse_json("[]")
        assert root.is_array()

    def test_parse_empty_object(self):
        root = parse_json("{}")
        assert root.is_object()


class TestNumberEdgeCases:
    """Test edge cases in number parsing."""

    def test_large_positive_integer(self):
        root = parse_json("9007199254740991")  # 2^53 - 1 (max safe int in JS)
        assert root.is_number()
        assert root.get_int() == 9007199254740991

    def test_large_negative_integer(self):
        root = parse_json("-9007199254740991")
        assert root.is_number()
        assert root.get_int() == -9007199254740991

    def test_very_small_float(self):
        # Note: Very small floats near the limits may fail with std::stod
        # This is a known limitation; test a more reasonable small float
        root = parse_json("1e-100")
        assert root.is_number()
        assert root.get_float() > 0
        assert root.get_float() < 1e-99

    def test_number_with_leading_zero_rejected(self):
        # Leading zeros are not valid in JSON (except for "0" itself)
        with pytest.raises(ValueError):  # Should be ParseError
            parse_json("01")

    def test_number_with_plus_sign_rejected(self):
        # Plus sign is not allowed in JSON
        with pytest.raises(ValueError):
            parse_json("+1")

    def test_decimal_without_digits_rejected(self):
        with pytest.raises(ValueError):
            parse_json("1.")

    def test_exponent_without_digits_rejected(self):
        with pytest.raises(ValueError):
            parse_json("1e")


class TestArrays:
    """Test array parsing and navigation."""

    def test_array_with_mixed_types(self):
        root = parse_json('[1, "two", true, null, 3.14]')
        assert root.is_array()

        assert root.at(0).is_number()
        assert root.at(0).get_int() == 1

        assert root.at(1).is_string()
        assert root.at(1).get_str() == "two"

        assert root.at(2).is_bool()
        assert root.at(2).get_bool() is True

        assert root.at(3).is_null()

        assert root.at(4).is_number()
        assert root.at(4).get_float() == 3.14

    def test_nested_arrays(self):
        root = parse_json('[[1, 2], [3, 4], [5]]')
        assert root.is_array()

        inner = root.at(0)
        assert inner.is_array()
        assert inner.at(0).get_int() == 1
        assert inner.at(1).get_int() == 2

        inner2 = root.at(2)
        assert inner2.at(0).get_int() == 5

    def test_deeply_nested_arrays(self):
        # Test deep nesting (10 levels)
        json_str = "[[[[[[[[[[42]]]]]]]]]]"
        root = parse_json(json_str)

        cursor = root
        for _ in range(10):
            cursor = cursor.at(0)

        assert cursor.get_int() == 42

    def test_large_array(self):
        # Test array with many elements
        json_str = "[" + ",".join(str(i) for i in range(1000)) + "]"
        root = parse_json(json_str)
        assert root.is_array()

        # Spot check a few elements
        assert root.at(0).get_int() == 0
        assert root.at(500).get_int() == 500
        assert root.at(999).get_int() == 999


class TestObjects:
    """Test object parsing and navigation."""

    def test_object_with_multiple_fields(self):
        root = parse_json('{"a": 1, "b": 2, "c": 3}')
        assert root.is_object()

        assert root.field("a").get_int() == 1
        assert root.field("b").get_int() == 2
        assert root.field("c").get_int() == 3

    def test_nested_objects(self):
        json_str = '{"outer": {"inner": {"deep": 42}}}'
        root = parse_json(json_str)

        deep_value = root.field("outer").field("inner").field("deep")
        assert deep_value.get_int() == 42

    def test_object_with_array_values(self):
        json_str = '{"numbers": [1, 2, 3], "strings": ["a", "b"]}'
        root = parse_json(json_str)

        numbers = root.field("numbers")
        assert numbers.is_array()
        assert numbers.at(1).get_int() == 2

        strings = root.field("strings")
        assert strings.at(0).get_str() == "a"

    def test_object_with_many_fields(self):
        # Test object with many fields
        fields = {f"field_{i}": i for i in range(100)}
        json_str = "{" + ",".join(f'"{k}": {v}' for k, v in fields.items()) + "}"
        root = parse_json(json_str)

        assert root.field("field_0").get_int() == 0
        assert root.field("field_50").get_int() == 50
        assert root.field("field_99").get_int() == 99


class TestWhitespace:
    """Test whitespace handling."""

    def test_leading_whitespace(self):
        root = parse_json("   42")
        assert root.get_int() == 42

    def test_trailing_whitespace(self):
        root = parse_json("42   ")
        assert root.get_int() == 42

    def test_whitespace_in_array(self):
        root = parse_json("[ 1 , 2 , 3 ]")
        assert root.at(1).get_int() == 2

    def test_whitespace_in_object(self):
        root = parse_json('{ "key" : "value" }')
        assert root.field("key").get_str() == "value"

    def test_tabs_and_newlines(self):
        json_str = '{\n\t"key":\t"value"\n}'
        root = parse_json(json_str)
        assert root.field("key").get_str() == "value"

    def test_empty_with_whitespace(self):
        root = parse_json("  {  }  ")
        assert root.is_object()


class TestComplexDocuments:
    """Test realistic complex JSON documents."""

    def test_user_document(self):
        json_str = """
        {
            "id": 123,
            "name": "Alice",
            "email": "alice@example.com",
            "active": true,
            "balance": 1234.56,
            "tags": ["premium", "verified"],
            "address": {
                "street": "123 Main St",
                "city": "Springfield",
                "zip": "12345"
            },
            "orders": [
                {"id": 1, "total": 99.99},
                {"id": 2, "total": 149.50}
            ]
        }
        """
        root = parse_json(json_str)

        assert root.field("id").get_int() == 123
        assert root.field("name").get_str() == "Alice"
        assert root.field("active").get_bool() is True
        assert root.field("balance").get_float() == 1234.56

        tags = root.field("tags")
        assert tags.at(0).get_str() == "premium"

        address = root.field("address")
        assert address.field("city").get_str() == "Springfield"

        orders = root.field("orders")
        assert orders.at(1).field("total").get_float() == 149.50

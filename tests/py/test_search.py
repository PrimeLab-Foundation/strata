"""
Test JSONPath functionality.

Tests basic selectors, wildcards, and navigation.
"""

import pytest

import strata
from strata.json_cursor import parse_json


class TestBasicSelectors:
    """Test basic JSONPath selectors."""

    def test_root_only(self):
        """Test $ selector returns the whole document."""
        json_text = '{"key": "value"}'
        results = strata.search(json_text, "$")
        assert len(results) == 1
        assert results[0] == {"key": "value"}

    def test_simple_field(self):
        """Test $.field selector."""
        json_text = '{"name": "Alice", "age": 30}'
        results = strata.search(json_text, "$.name")
        assert results == ["Alice"]

    def test_nested_field(self):
        """Test $.a.b nested selector."""
        json_text = '{"user": {"name": "Alice", "age": 30}}'
        results = strata.search(json_text, "$.user.name")
        assert results == ["Alice"]

    def test_deep_nesting(self):
        """Test deeply nested paths."""
        json_text = '{"a": {"b": {"c": {"d": 42}}}}'
        results = strata.search(json_text, "$.a.b.c.d")
        assert results == [42]

    def test_missing_field(self):
        """Test accessing missing field returns empty."""
        json_text = '{"name": "Alice"}'
        results = strata.search(json_text, "$.missing")
        assert results == []

    def test_array_index(self):
        """Test $[n] array index selector."""
        json_text = '["a", "b", "c"]'
        results = strata.search(json_text, "$[0]")
        assert results == ["a"]

        results = strata.search(json_text, "$[2]")
        assert results == ["c"]

    def test_array_index_out_of_bounds(self):
        """Test out of bounds array access returns empty."""
        json_text = '["a", "b"]'
        results = strata.search(json_text, "$[10]")
        assert results == []

    def test_mixed_navigation(self):
        """Test mixed field and array navigation."""
        json_text = '{"users": [{"name": "Alice"}, {"name": "Bob"}]}'
        results = strata.search(json_text, "$.users[0].name")
        assert results == ["Alice"]

        results = strata.search(json_text, "$.users[1].name")
        assert results == ["Bob"]


class TestWildcards:
    """Test wildcard selectors."""

    def test_array_wildcard(self):
        """Test $[*] to get all array elements."""
        json_text = '[1, 2, 3, 4]'
        results = strata.search(json_text, "$[*]")
        assert results == [1, 2, 3, 4]

    def test_object_wildcard(self):
        """Test $.* to get all object values."""
        json_text = '{"a": 1, "b": 2, "c": 3}'
        results = strata.search(json_text, "$.*")
        assert set(results) == {1, 2, 3}  # Order may vary

    def test_nested_wildcard(self):
        """Test $.users[*].name to extract all names."""
        json_text = '''
        {
            "users": [
                {"name": "Alice", "age": 30},
                {"name": "Bob", "age": 25},
                {"name": "Charlie", "age": 35}
            ]
        }
        '''
        results = strata.search(json_text, "$.users[*].name")
        assert results == ["Alice", "Bob", "Charlie"]

    def test_deep_wildcard(self):
        """Test wildcards at multiple levels."""
        json_text = '''
        {
            "departments": [
                {
                    "name": "Engineering",
                    "employees": [
                        {"id": 1, "name": "Alice"},
                        {"id": 2, "name": "Bob"}
                    ]
                },
                {
                    "name": "Sales",
                    "employees": [
                        {"id": 3, "name": "Charlie"}
                    ]
                }
            ]
        }
        '''
        results = strata.search(json_text, "$.departments[*].employees[*].name")
        assert results == ["Alice", "Bob", "Charlie"]


class TestComplexQueries:
    """Test more complex JSONPath queries."""

    def test_user_ids(self):
        """Test extracting all user IDs."""
        json_text = '''
        {
            "users": [
                {"id": 1, "name": "Alice"},
                {"id": 2, "name": "Bob"},
                {"id": 3, "name": "Charlie"}
            ]
        }
        '''
        results = strata.search(json_text, "$.users[*].id")
        assert results == [1, 2, 3]

    def test_mixed_types_in_array(self):
        """Test wildcard on array with mixed types."""
        json_text = '[1, "two", true, null, 3.14]'
        results = strata.search(json_text, "$[*]")
        assert results == [1, "two", True, None, 3.14]

    def test_nested_arrays(self):
        """Test navigation through nested arrays."""
        json_text = '[[[1, 2]], [[3, 4]]]'
        results = strata.search(json_text, "$[0][0]")
        assert results == [[1, 2]]

        results = strata.search(json_text, "$[0][0][1]")
        assert results == [2]

    def test_bracket_notation_field(self):
        """Test $["field"] bracket notation for fields."""
        json_text = '{"special-key": "value", "normal": 123}'
        results = strata.search(json_text, '$["special-key"]')
        assert results == ["value"]

    def test_recursive_descent_nested_same_field(self):
        """Ensure $..field walks into the matched field itself."""
        json_text = '{"a": {"a": {"value": 1}, "value": 2}}'
        results = strata.search(json_text, "$..a")
        assert results == [{"a": {"value": 1}, "value": 2}, {"value": 1}]

    def test_large_integer_preserved(self):
        """Large integers should stay as Python ints."""
        json_text = '{"value": 9007199254740993}'
        results = strata.search(json_text, "$.value")
        assert results == [9007199254740993]
        assert isinstance(results[0], int)


class TestCompileAndReuse:
    """Test compiling paths and reusing them."""

    def test_compile_once_use_many(self):
        """Test compiling a path once and using it on multiple documents."""
        path = strata.compile_path("$.users[*].id")

        json1 = '{"users": [{"id": 1}, {"id": 2}]}'
        json2 = '{"users": [{"id": 3}, {"id": 4}, {"id": 5}]}'

        results1 = strata.search(json1, path)
        results2 = strata.search(json2, path)

        assert results1 == [1, 2]
        assert results2 == [3, 4, 5]

    def test_compile_complex_path(self):
        """Test compiling and reusing complex paths."""
        path = strata.compile_path("$.departments[*].employees[*].name")

        json_text = '''
        {
            "departments": [
                {"employees": [{"name": "Alice"}, {"name": "Bob"}]},
                {"employees": [{"name": "Charlie"}]}
            ]
        }
        '''

        results = strata.search(json_text, path)
        assert results == ["Alice", "Bob", "Charlie"]

    def test_search_with_cursor(self):
        """Test search using a pre-parsed JsonCursor."""
        path = strata.compile_path("$.users[*].id")
        json_text = '{"users": [{"id": 1}, {"id": 2}, {"id": 3}]}'

        cursor = parse_json(json_text)
        results = strata.search(cursor, path)

        assert results == [1, 2, 3]


class TestLimits:
    """Test search limit parameter."""

    def test_search_limit(self):
        json_text = (
            '{"users": ['
            + ",".join(f'{{"id": {i}}}' for i in range(25))
            + "]}"
        )

        results = strata.search(json_text, "$.users[*].id", limit=10)

        assert results == list(range(10))


class TestErrorHandling:
    """Test error handling."""

    def test_invalid_path_syntax(self):
        """Test that invalid JSONPath raises ValueError."""
        with pytest.raises(ValueError):
            strata.compile_path("invalid")

        with pytest.raises(ValueError):
            strata.compile_path(".field")  # Must start with $

    def test_invalid_json(self):
        """Test that invalid JSON raises error."""
        with pytest.raises(ValueError):
            strata.search("{invalid json}", "$.field")


class TestEdgeCases:
    """Test edge cases and boundary conditions."""

    def test_empty_object(self):
        """Test wildcard on empty object."""
        json_text = '{}'
        results = strata.search(json_text, "$.*")
        assert results == []

    def test_empty_array(self):
        """Test wildcard on empty array."""
        json_text = '[]'
        results = strata.search(json_text, "$[*]")
        assert results == []

    def test_null_value(self):
        """Test accessing null values."""
        json_text = '{"value": null}'
        results = strata.search(json_text, "$.value")
        assert results == [None]

    def test_boolean_values(self):
        """Test accessing boolean values."""
        json_text = '{"active": true, "deleted": false}'
        results = strata.search(json_text, "$.active")
        assert results == [True]

        results = strata.search(json_text, "$.deleted")
        assert results == [False]

    def test_numeric_types(self):
        """Test accessing different numeric types."""
        json_text = '{"int": 42, "float": 3.14, "sci": 1e10}'
        results = strata.search(json_text, "$.int")
        assert results == [42]

        results = strata.search(json_text, "$.float")
        assert results == [3.14]

        results = strata.search(json_text, "$.sci")
        assert results == [1e10]

"""
Test JSONPath functionality.

Tests basic selectors, wildcards, and navigation.
"""

import pytest

import strata


def _query(json_text, path):
    """Helper: parse JSON text and query with JSONPath."""
    return strata.query(strata.loads(json_text), path)


class TestBasicSelectors:
    """Test basic JSONPath selectors."""

    def test_root_only(self):
        json_text = '{"key": "value"}'
        results = _query(json_text, "$")
        assert len(results) == 1
        assert results[0] == {"key": "value"}

    def test_simple_field(self):
        json_text = '{"name": "Alice", "age": 30}'
        results = _query(json_text, "$.name")
        assert results == ["Alice"]

    def test_nested_field(self):
        json_text = '{"user": {"name": "Alice", "age": 30}}'
        results = _query(json_text, "$.user.name")
        assert results == ["Alice"]

    def test_deep_nesting(self):
        json_text = '{"a": {"b": {"c": {"d": 42}}}}'
        results = _query(json_text, "$.a.b.c.d")
        assert results == [42]

    def test_missing_field(self):
        json_text = '{"name": "Alice"}'
        results = _query(json_text, "$.missing")
        assert results == []

    def test_array_index(self):
        json_text = '["a", "b", "c"]'
        results = _query(json_text, "$[0]")
        assert results == ["a"]

        results = _query(json_text, "$[2]")
        assert results == ["c"]

    def test_array_index_out_of_bounds(self):
        json_text = '["a", "b"]'
        results = _query(json_text, "$[10]")
        assert results == []

    def test_mixed_navigation(self):
        json_text = '{"users": [{"name": "Alice"}, {"name": "Bob"}]}'
        results = _query(json_text, "$.users[0].name")
        assert results == ["Alice"]

        results = _query(json_text, "$.users[1].name")
        assert results == ["Bob"]


class TestWildcards:
    """Test wildcard selectors."""

    def test_array_wildcard(self):
        json_text = '[1, 2, 3, 4]'
        results = _query(json_text, "$[*]")
        assert results == [1, 2, 3, 4]

    def test_object_wildcard(self):
        json_text = '{"a": 1, "b": 2, "c": 3}'
        results = _query(json_text, "$.*")
        assert set(results) == {1, 2, 3}

    def test_nested_wildcard(self):
        json_text = '''
        {
            "users": [
                {"name": "Alice", "age": 30},
                {"name": "Bob", "age": 25},
                {"name": "Charlie", "age": 35}
            ]
        }
        '''
        results = _query(json_text, "$.users[*].name")
        assert results == ["Alice", "Bob", "Charlie"]

    def test_deep_wildcard(self):
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
        results = _query(json_text, "$.departments[*].employees[*].name")
        assert results == ["Alice", "Bob", "Charlie"]


class TestComplexQueries:
    """Test more complex JSONPath queries."""

    def test_user_ids(self):
        json_text = '''
        {
            "users": [
                {"id": 1, "name": "Alice"},
                {"id": 2, "name": "Bob"},
                {"id": 3, "name": "Charlie"}
            ]
        }
        '''
        results = _query(json_text, "$.users[*].id")
        assert results == [1, 2, 3]

    def test_mixed_types_in_array(self):
        json_text = '[1, "two", true, null, 3.14]'
        results = _query(json_text, "$[*]")
        assert results == [1, "two", True, None, 3.14]

    def test_nested_arrays(self):
        json_text = '[[[1, 2]], [[3, 4]]]'
        results = _query(json_text, "$[0][0]")
        assert results == [[1, 2]]

        results = _query(json_text, "$[0][0][1]")
        assert results == [2]

    def test_bracket_notation_field(self):
        json_text = '{"special-key": "value", "normal": 123}'
        results = _query(json_text, '$["special-key"]')
        assert results == ["value"]


class TestCompileAndReuse:
    """Test compiling paths and reusing them."""

    def test_compile_once_use_many(self):
        path = strata.compile_path("$.users[*].id")

        data1 = strata.loads('{"users": [{"id": 1}, {"id": 2}]}')
        data2 = strata.loads('{"users": [{"id": 3}, {"id": 4}, {"id": 5}]}')

        results1 = strata.query(data1, path)
        results2 = strata.query(data2, path)

        assert results1 == [1, 2]
        assert results2 == [3, 4, 5]

    def test_compile_complex_path(self):
        path = strata.compile_path("$.departments[*].employees[*].name")

        data = strata.loads('''
        {
            "departments": [
                {"employees": [{"name": "Alice"}, {"name": "Bob"}]},
                {"employees": [{"name": "Charlie"}]}
            ]
        }
        ''')

        results = strata.query(data, path)
        assert results == ["Alice", "Bob", "Charlie"]

    def test_compile_path_execute_on_cursor(self):
        """Test CompiledPath.execute(cursor) matches query results."""
        json_text = '{"users": [{"id": 1}, {"id": 2}, {"id": 3}]}'
        path = strata.compile_path("$.users[*].id")

        # Via query(dict, path)
        data = strata.loads(json_text)
        via_query = strata.query(data, path)
        assert via_query == [1, 2, 3]

        # Via parse_json -> cursor, then path.execute(cursor)
        cursor = strata.parse_json(json_text)
        via_execute = path.execute(cursor)
        assert via_execute == via_query

    def test_query_with_dict(self):
        """Test query using a pre-parsed dict."""
        path = strata.compile_path("$.users[*].id")
        data = {"users": [{"id": 1}, {"id": 2}, {"id": 3}]}
        results = strata.query(data, path)
        assert results == [1, 2, 3]


class TestErrorHandling:
    """Test error handling."""

    def test_invalid_path_syntax(self):
        with pytest.raises(ValueError):
            strata.compile_path("invalid")

        with pytest.raises(ValueError):
            strata.compile_path(".field")

    def test_query_with_string_raises(self):
        """Test that query with string raises TypeError."""
        with pytest.raises(TypeError):
            strata.query("{invalid json}", "$.field")

    def test_search_with_dict_raises(self):
        """Test that search with dict raises TypeError."""
        with pytest.raises(TypeError):
            strata.search({"a": 1}, "$.a")


class TestEdgeCases:
    """Test edge cases and boundary conditions."""

    def test_empty_object(self):
        results = _query('{}', "$.*")
        assert results == []

    def test_empty_array(self):
        results = _query('[]', "$[*]")
        assert results == []

    def test_null_value(self):
        results = _query('{"value": null}', "$.value")
        assert results == [None]

    def test_boolean_values(self):
        results = _query('{"active": true, "deleted": false}', "$.active")
        assert results == [True]

        results = _query('{"active": true, "deleted": false}', "$.deleted")
        assert results == [False]

    def test_numeric_types(self):
        results = _query('{"int": 42, "float": 3.14, "sci": 1e10}', "$.int")
        assert results == [42]

        results = _query('{"int": 42, "float": 3.14, "sci": 1e10}', "$.float")
        assert results == [3.14]

        results = _query('{"int": 42, "float": 3.14, "sci": 1e10}', "$.sci")
        assert results == [1e10]

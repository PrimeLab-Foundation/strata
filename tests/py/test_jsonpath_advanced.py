"""
Test advanced JSONPath features (Phase 2.2).

Tests filter predicates, recursive descent, and array slicing.
"""

import strata


def _query(json_text, path):
    """Helper: parse JSON text and query with JSONPath."""
    return strata.query(strata.loads(json_text), path)


class TestFilterPredicates:
    """Test filter predicate functionality."""

    def test_filter_numeric_greater_than(self):
        json_text = '[{"age": 30}, {"age": 25}, {"age": 35}]'
        results = _query(json_text, "$[?(@.age > 27)]")
        assert len(results) == 2
        assert results[0]["age"] == 30
        assert results[1]["age"] == 35

    def test_filter_numeric_less_than(self):
        json_text = '[{"age": 30}, {"age": 25}, {"age": 35}]'
        results = _query(json_text, "$[?(@.age < 30)]")
        assert len(results) == 1
        assert results[0]["age"] == 25

    def test_filter_numeric_equal(self):
        json_text = '[{"age": 30}, {"age": 25}, {"age": 30}]'
        results = _query(json_text, "$[?(@.age == 30)]")
        assert len(results) == 2
        assert all(r["age"] == 30 for r in results)

    def test_filter_numeric_not_equal(self):
        json_text = '[{"age": 30}, {"age": 25}, {"age": 30}]'
        results = _query(json_text, "$[?(@.age != 30)]")
        assert len(results) == 1
        assert results[0]["age"] == 25

    def test_filter_numeric_greater_equal(self):
        json_text = '[{"age": 30}, {"age": 25}, {"age": 30}]'
        results = _query(json_text, "$[?(@.age >= 30)]")
        assert len(results) == 2

    def test_filter_numeric_less_equal(self):
        json_text = '[{"age": 30}, {"age": 25}, {"age": 35}]'
        results = _query(json_text, "$[?(@.age <= 30)]")
        assert len(results) == 2

    def test_filter_string_equal(self):
        json_text = '[{"name": "Alice"}, {"name": "Bob"}, {"name": "Alice"}]'
        results = _query(json_text, '$[?(@.name == "Alice")]')
        assert len(results) == 2
        assert all(r["name"] == "Alice" for r in results)

    def test_filter_string_not_equal(self):
        json_text = '[{"name": "Alice"}, {"name": "Bob"}]'
        results = _query(json_text, '$[?(@.name != "Alice")]')
        assert len(results) == 1
        assert results[0]["name"] == "Bob"

    def test_filter_with_nested_path(self):
        json_text = '{"users": [{"name": "Alice", "age": 30}, {"name": "Bob", "age": 25}]}'
        results = _query(json_text, "$.users[?(@.age > 27)]")
        assert len(results) == 1
        assert results[0]["name"] == "Alice"

    def test_filter_then_field_access(self):
        json_text = '{"users": [{"name": "Alice", "age": 30}, {"name": "Bob", "age": 25}, {"name": "Charlie", "age": 35}]}'
        results = _query(json_text, "$.users[?(@.age > 27)].name")
        assert set(results) == {"Alice", "Charlie"}

    def test_filter_missing_field(self):
        json_text = '[{"age": 30}, {"name": "Bob"}, {"age": 25}]'
        results = _query(json_text, "$[?(@.age > 27)]")
        assert len(results) == 1
        assert results[0]["age"] == 30

    def test_filter_wrong_type(self):
        json_text = '[{"age": 30}, {"age": "old"}, {"age": 25}]'
        results = _query(json_text, "$[?(@.age > 27)]")
        assert len(results) == 1
        assert results[0]["age"] == 30


class TestRecursiveDescent:
    """Test recursive descent (..) functionality."""

    def test_recursive_simple(self):
        json_text = '{"a": {"price": 10}, "b": {"price": 20}}'
        results = _query(json_text, "$..price")
        assert set(results) == {10, 20}

    def test_recursive_nested(self):
        json_text = '{"a": {"b": {"price": 10, "c": {"price": 20}}}}'
        results = _query(json_text, "$..price")
        assert set(results) == {10, 20}

    def test_recursive_in_arrays(self):
        json_text = '{"items": [{"price": 10}, {"price": 20}]}'
        results = _query(json_text, "$..price")
        assert set(results) == {10, 20}

    def test_recursive_mixed_structure(self):
        json_text = '{"a": {"price": 10}, "b": [{"price": 20}, {"c": {"price": 30}}]}'
        results = _query(json_text, "$..price")
        assert set(results) == {10, 20, 30}

    def test_recursive_multiple_levels(self):
        json_text = '''
        {
            "store": {
                "book": [
                    {"price": 8.95},
                    {"price": 12.99}
                ],
                "bicycle": {
                    "price": 19.95
                }
            }
        }
        '''
        results = _query(json_text, "$..price")
        assert len(results) == 3
        assert set(results) == {8.95, 12.99, 19.95}

    def test_recursive_no_matches(self):
        json_text = '{"a": {"b": 1}, "c": {"d": 2}}'
        results = _query(json_text, "$..price")
        assert results == []

    def test_recursive_with_wildcards(self):
        json_text = '{"a": {"items": [{"name": "x"}]}, "b": {"items": [{"name": "y"}]}}'
        results = _query(json_text, "$..items[*].name")
        assert len(results) == 2
        assert "x" in results and "y" in results


class TestArraySlicing:
    """Test array slicing functionality."""

    def test_slice_basic(self):
        results = _query('[0, 1, 2, 3, 4, 5]', "$[1:4]")
        assert results == [1, 2, 3]

    def test_slice_with_step(self):
        results = _query('[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]', "$[1:8:2]")
        assert results == [1, 3, 5, 7]

    def test_slice_from_start(self):
        results = _query('[0, 1, 2, 3, 4]', "$[:3]")
        assert results == [0, 1, 2]

    def test_slice_to_end(self):
        results = _query('[0, 1, 2, 3, 4]', "$[2:]")
        assert results == [2, 3, 4]

    def test_slice_negative_start(self):
        results = _query('[0, 1, 2, 3, 4, 5]', "$[-3:]")
        assert results == [3, 4, 5]

    def test_slice_negative_end(self):
        results = _query('[0, 1, 2, 3, 4, 5]', "$[:-2]")
        assert results == [0, 1, 2, 3]

    def test_slice_both_negative(self):
        results = _query('[0, 1, 2, 3, 4, 5]', "$[-4:-1]")
        assert results == [2, 3, 4]

    def test_slice_step_two(self):
        results = _query('[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]', "$[::2]")
        assert results == [0, 2, 4, 6, 8]

    def test_slice_on_nested_array(self):
        results = _query('{"numbers": [0, 1, 2, 3, 4, 5]}', "$.numbers[1:4]")
        assert results == [1, 2, 3]

    def test_slice_with_subsequent_operations(self):
        results = _query(
            '[{"name": "a"}, {"name": "b"}, {"name": "c"}, {"name": "d"}]', "$[1:3].name"
        )
        assert results == ["b", "c"]

    def test_slice_empty_range(self):
        results = _query('[0, 1, 2, 3]', "$[2:2]")
        assert results == []

    def test_slice_out_of_bounds(self):
        results = _query('[0, 1, 2]', "$[1:100]")
        assert results == [1, 2]


class TestCombinedFeatures:
    """Test combining multiple advanced features."""

    def test_filter_with_recursive(self):
        json_text = '''
        {
            "departments": [
                {"name": "Eng", "employees": [{"name": "Alice", "age": 30}, {"name": "Bob", "age": 25}]},
                {"name": "Sales", "employees": [{"name": "Charlie", "age": 35}]}
            ]
        }
        '''
        results = _query(json_text, '$.departments[?(@.name == "Eng")].employees[*].name')
        assert len(results) == 2
        assert "Alice" in results and "Bob" in results

    def test_slice_with_filter(self):
        json_text = '{"items": [{"age": 20}, {"age": 30}, {"age": 40}, {"age": 50}]}'
        results = _query(json_text, "$.items[?(@.age > 25)]")
        assert len(results) >= 2
        assert all(r["age"] > 25 for r in results)

    def test_recursive_with_slice(self):
        json_text = '{"a": {"items": [1, 2, 3, 4, 5]}, "b": {"items": [6, 7, 8, 9]}}'
        results = _query(json_text, "$..items[1:3]")
        assert len(results) == 4
        assert 2 in results and 3 in results
        assert 7 in results and 8 in results


class TestEdgeCases:
    """Test edge cases and error conditions."""

    def test_filter_on_non_array(self):
        results = _query('{"age": 30}', "$[?(@.age > 25)]")
        assert results == []

    def test_slice_on_non_array(self):
        results = _query('{"key": "value"}', "$[1:3]")
        assert results == []

    def test_recursive_on_scalar(self):
        results = _query('{"price": 10}', "$..price")
        assert results == [10]

    def test_filter_decimal_comparison(self):
        json_text = '[{"price": 10.5}, {"price": 15.7}, {"price": 8.2}]'
        results = _query(json_text, "$[?(@.price > 10)]")
        assert len(results) == 2

    def test_negative_index_in_slice(self):
        results = _query('[0, 1, 2, 3, 4]', "$[-2]")
        assert results == [3]

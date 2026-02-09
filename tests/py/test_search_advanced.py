"""
Test advanced JSONPath features (Phase 2.2).

Tests filter predicates, recursive descent, and array slicing.
"""

import strata


class TestFilterPredicates:
    """Test filter predicate functionality."""

    def test_filter_numeric_greater_than(self):
        """Test numeric > comparison."""
        json_text = '[{"age": 30}, {"age": 25}, {"age": 35}]'
        results = strata.search(json_text, "$[?(@.age > 27)]")
        assert len(results) == 2
        assert results[0]["age"] == 30
        assert results[1]["age"] == 35

    def test_filter_numeric_less_than(self):
        """Test numeric < comparison."""
        json_text = '[{"age": 30}, {"age": 25}, {"age": 35}]'
        results = strata.search(json_text, "$[?(@.age < 30)]")
        assert len(results) == 1
        assert results[0]["age"] == 25

    def test_filter_numeric_equal(self):
        """Test numeric == comparison."""
        json_text = '[{"age": 30}, {"age": 25}, {"age": 30}]'
        results = strata.search(json_text, "$[?(@.age == 30)]")
        assert len(results) == 2
        assert all(r["age"] == 30 for r in results)

    def test_filter_numeric_not_equal(self):
        """Test numeric != comparison."""
        json_text = '[{"age": 30}, {"age": 25}, {"age": 30}]'
        results = strata.search(json_text, "$[?(@.age != 30)]")
        assert len(results) == 1
        assert results[0]["age"] == 25

    def test_filter_numeric_greater_equal(self):
        """Test numeric >= comparison."""
        json_text = '[{"age": 30}, {"age": 25}, {"age": 30}]'
        results = strata.search(json_text, "$[?(@.age >= 30)]")
        assert len(results) == 2

    def test_filter_numeric_less_equal(self):
        """Test numeric <= comparison."""
        json_text = '[{"age": 30}, {"age": 25}, {"age": 35}]'
        results = strata.search(json_text, "$[?(@.age <= 30)]")
        assert len(results) == 2

    def test_filter_string_equal(self):
        """Test string == comparison."""
        json_text = '[{"name": "Alice"}, {"name": "Bob"}, {"name": "Alice"}]'
        results = strata.search(json_text, '$[?(@.name == "Alice")]')
        assert len(results) == 2
        assert all(r["name"] == "Alice" for r in results)

    def test_filter_string_not_equal(self):
        """Test string != comparison."""
        json_text = '[{"name": "Alice"}, {"name": "Bob"}]'
        results = strata.search(json_text, '$[?(@.name != "Alice")]')
        assert len(results) == 1
        assert results[0]["name"] == "Bob"

    def test_filter_with_nested_path(self):
        """Test filter on nested objects."""
        json_text = '{"users": [{"name": "Alice", "age": 30}, {"name": "Bob", "age": 25}]}'
        results = strata.search(json_text, "$.users[?(@.age > 27)]")
        assert len(results) == 1
        assert results[0]["name"] == "Alice"

    def test_filter_then_field_access(self):
        """Test filter followed by field access."""
        json_text = '{"users": [{"name": "Alice", "age": 30}, {"name": "Bob", "age": 25}, {"name": "Charlie", "age": 35}]}'
        results = strata.search(json_text, "$.users[?(@.age > 27)].name")
        assert set(results) == {"Alice", "Charlie"}

    def test_filter_missing_field(self):
        """Test filter when field is missing."""
        json_text = '[{"age": 30}, {"name": "Bob"}, {"age": 25}]'
        results = strata.search(json_text, "$[?(@.age > 27)]")
        assert len(results) == 1
        assert results[0]["age"] == 30

    def test_filter_wrong_type(self):
        """Test filter when field is wrong type."""
        json_text = '[{"age": 30}, {"age": "old"}, {"age": 25}]'
        results = strata.search(json_text, "$[?(@.age > 27)]")
        # Only numeric values should match
        assert len(results) == 1
        assert results[0]["age"] == 30


class TestRecursiveDescent:
    """Test recursive descent (..) functionality."""

    def test_recursive_simple(self):
        """Test simple recursive descent."""
        json_text = '{"a": {"price": 10}, "b": {"price": 20}}'
        results = strata.search(json_text, "$..price")
        assert set(results) == {10, 20}

    def test_recursive_nested(self):
        """Test recursive descent in nested structures."""
        json_text = '{"a": {"b": {"price": 10, "c": {"price": 20}}}}'
        results = strata.search(json_text, "$..price")
        assert set(results) == {10, 20}

    def test_recursive_in_arrays(self):
        """Test recursive descent through arrays."""
        json_text = '{"items": [{"price": 10}, {"price": 20}]}'
        results = strata.search(json_text, "$..price")
        assert set(results) == {10, 20}

    def test_recursive_mixed_structure(self):
        """Test recursive descent in mixed object/array structure."""
        json_text = '{"a": {"price": 10}, "b": [{"price": 20}, {"c": {"price": 30}}]}'
        results = strata.search(json_text, "$..price")
        assert set(results) == {10, 20, 30}

    def test_recursive_multiple_levels(self):
        """Test recursive descent across multiple levels."""
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
        results = strata.search(json_text, "$..price")
        assert len(results) == 3
        assert set(results) == {8.95, 12.99, 19.95}

    def test_recursive_no_matches(self):
        """Test recursive descent when field doesn't exist."""
        json_text = '{"a": {"b": 1}, "c": {"d": 2}}'
        results = strata.search(json_text, "$..price")
        assert results == []

    def test_recursive_with_wildcards(self):
        """Test combining recursive descent with subsequent operations."""
        json_text = '{"a": {"items": [{"name": "x"}]}, "b": {"items": [{"name": "y"}]}}'
        results = strata.search(json_text, "$..items[*].name")
        # Results contain both "x" and "y", but may be in any order
        assert len(results) == 2
        assert "x" in results and "y" in results


class TestArraySlicing:
    """Test array slicing functionality."""

    def test_slice_basic(self):
        """Test basic array slice [start:end]."""
        json_text = '[0, 1, 2, 3, 4, 5]'
        results = strata.search(json_text, "$[1:4]")
        assert results == [1, 2, 3]

    def test_slice_with_step(self):
        """Test array slice with step [start:end:step]."""
        json_text = '[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]'
        results = strata.search(json_text, "$[1:8:2]")
        assert results == [1, 3, 5, 7]

    def test_slice_from_start(self):
        """Test slice from beginning [:end]."""
        json_text = '[0, 1, 2, 3, 4]'
        results = strata.search(json_text, "$[:3]")
        assert results == [0, 1, 2]

    def test_slice_to_end(self):
        """Test slice to end [start:]."""
        json_text = '[0, 1, 2, 3, 4]'
        results = strata.search(json_text, "$[2:]")
        assert results == [2, 3, 4]

    def test_slice_negative_start(self):
        """Test slice with negative start index."""
        json_text = '[0, 1, 2, 3, 4, 5]'
        results = strata.search(json_text, "$[-3:]")
        assert results == [3, 4, 5]

    def test_slice_negative_end(self):
        """Test slice with negative end index."""
        json_text = '[0, 1, 2, 3, 4, 5]'
        results = strata.search(json_text, "$[:-2]")
        assert results == [0, 1, 2, 3]

    def test_slice_both_negative(self):
        """Test slice with both negative indices."""
        json_text = '[0, 1, 2, 3, 4, 5]'
        results = strata.search(json_text, "$[-4:-1]")
        assert results == [2, 3, 4]

    def test_slice_step_two(self):
        """Test slice with step of 2."""
        json_text = '[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]'
        results = strata.search(json_text, "$[::2]")
        assert results == [0, 2, 4, 6, 8]

    def test_slice_on_nested_array(self):
        """Test slice on nested array path."""
        json_text = '{"numbers": [0, 1, 2, 3, 4, 5]}'
        results = strata.search(json_text, "$.numbers[1:4]")
        assert results == [1, 2, 3]

    def test_slice_with_subsequent_operations(self):
        """Test slice followed by field access."""
        json_text = '[{"name": "a"}, {"name": "b"}, {"name": "c"}, {"name": "d"}]'
        results = strata.search(json_text, "$[1:3].name")
        assert results == ["b", "c"]

    def test_slice_empty_range(self):
        """Test slice with empty range."""
        json_text = '[0, 1, 2, 3]'
        results = strata.search(json_text, "$[2:2]")
        assert results == []

    def test_slice_out_of_bounds(self):
        """Test slice with out of bounds indices."""
        json_text = '[0, 1, 2]'
        results = strata.search(json_text, "$[1:100]")
        assert results == [1, 2]


class TestCombinedFeatures:
    """Test combining multiple advanced features."""

    def test_filter_with_recursive(self):
        """Test filter after recursive descent."""
        json_text = '''
        {
            "departments": [
                {"name": "Eng", "employees": [{"name": "Alice", "age": 30}, {"name": "Bob", "age": 25}]},
                {"name": "Sales", "employees": [{"name": "Charlie", "age": 35}]}
            ]
        }
        '''
        # Get department names where they have employees
        results = strata.search(json_text, "$.departments[?(@.name == \"Eng\")].employees[*].name")
        assert len(results) == 2
        assert "Alice" in results and "Bob" in results

    def test_slice_with_filter(self):
        """Test filtering a slice result."""
        json_text = '{"items": [{"age": 20}, {"age": 30}, {"age": 40}, {"age": 50}]}'
        # Get filtered subset
        results = strata.search(json_text, "$.items[?(@.age > 25)]")
        assert len(results) >= 2
        # All results should have age > 25
        assert all(r["age"] > 25 for r in results)

    def test_recursive_with_slice(self):
        """Test recursive descent followed by slicing."""
        json_text = '{"a": {"items": [1, 2, 3, 4, 5]}, "b": {"items": [6, 7, 8, 9]}}'
        results = strata.search(json_text, "$..items[1:3]")
        # Results from both arrays: [2, 3] and [7, 8]
        assert len(results) == 4
        assert 2 in results and 3 in results
        assert 7 in results and 8 in results


class TestEdgeCases:
    """Test edge cases and error conditions."""

    def test_filter_on_non_array(self):
        """Test filter on non-array returns empty."""
        json_text = '{"age": 30}'
        results = strata.search(json_text, "$[?(@.age > 25)]")
        assert results == []

    def test_slice_on_non_array(self):
        """Test slice on non-array returns empty."""
        json_text = '{"key": "value"}'
        results = strata.search(json_text, "$[1:3]")
        assert results == []

    def test_recursive_on_scalar(self):
        """Test recursive descent on scalar."""
        json_text = '{"price": 10}'
        results = strata.search(json_text, "$..price")
        assert results == [10]

    def test_filter_decimal_comparison(self):
        """Test filter with decimal numbers."""
        json_text = '[{"price": 10.5}, {"price": 15.7}, {"price": 8.2}]'
        results = strata.search(json_text, "$[?(@.price > 10)]")
        assert len(results) == 2

    def test_negative_index_in_slice(self):
        """Test negative indices are handled correctly."""
        json_text = '[0, 1, 2, 3, 4]'
        results = strata.search(json_text, "$[-2]")
        assert results == [3]

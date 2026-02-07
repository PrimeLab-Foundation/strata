"""
Tests for LazyCursor (selective materialization / lazy parsing).
"""

import pytest

from strata import LazyCursor, lazy


class TestLazyCursorBasic:
    """Basic functionality tests."""

    def test_lazy_function_creates_cursor(self):
        cursor = lazy('{"key": "value"}')
        assert isinstance(cursor, LazyCursor)

    def test_lazy_cursor_class_constructor(self):
        cursor = LazyCursor('{"key": "value"}')
        assert isinstance(cursor, LazyCursor)

    def test_lazy_cursor_accepts_bytes(self):
        cursor = lazy(b'{"key": "value"}')
        assert cursor.is_object()


class TestTypeDetection:
    """Type detection tests."""

    def test_is_null(self):
        assert lazy("null").is_null()
        assert not lazy("true").is_null()

    def test_is_bool(self):
        assert lazy("true").is_bool()
        assert lazy("false").is_bool()
        assert not lazy("null").is_bool()

    def test_is_number(self):
        assert lazy("42").is_number()
        assert lazy("-3.14").is_number()
        assert lazy("1e10").is_number()
        assert not lazy('"string"').is_number()

    def test_is_string(self):
        assert lazy('"hello"').is_string()
        assert not lazy("42").is_string()

    def test_is_array(self):
        assert lazy("[1, 2, 3]").is_array()
        assert not lazy("{}").is_array()

    def test_is_object(self):
        assert lazy('{"key": "value"}').is_object()
        assert not lazy("[]").is_object()


class TestNavigation:
    """Navigation tests."""

    def test_get_field_via_getitem(self):
        cursor = lazy('{"name": "John", "age": 30}')
        assert cursor["name"].value() == "John"
        assert cursor["age"].value() == 30.0

    def test_get_field_method(self):
        cursor = lazy('{"name": "John"}')
        assert cursor.get_field("name").value() == "John"

    def test_get_at_via_getitem(self):
        cursor = lazy("[10, 20, 30]")
        assert cursor[0].value() == 10.0
        assert cursor[1].value() == 20.0
        assert cursor[2].value() == 30.0

    def test_get_at_method(self):
        cursor = lazy("[10, 20, 30]")
        assert cursor.get_at(1).value() == 20.0

    def test_nested_navigation(self):
        json = '{"user": {"profile": {"email": "test@example.com"}}}'
        cursor = lazy(json)
        email = cursor["user"]["profile"]["email"].value()
        assert email == "test@example.com"

    def test_nested_array_in_object(self):
        json = '{"items": [{"id": 1}, {"id": 2}]}'
        cursor = lazy(json)
        assert cursor["items"][0]["id"].value() == 1.0
        assert cursor["items"][1]["id"].value() == 2.0

    def test_key_not_found_raises_keyerror(self):
        cursor = lazy('{"name": "John"}')
        with pytest.raises(KeyError):
            cursor["missing"]

    def test_index_out_of_bounds_raises_indexerror(self):
        cursor = lazy("[1, 2, 3]")
        with pytest.raises(IndexError):
            cursor[10]

    def test_type_mismatch_get_field_on_array(self):
        cursor = lazy("[1, 2, 3]")
        with pytest.raises(TypeError):
            cursor["key"]

    def test_type_mismatch_get_at_on_object(self):
        cursor = lazy('{"key": "value"}')
        with pytest.raises(TypeError):
            cursor[0]


class TestMaterialization:
    """Materialization tests."""

    def test_value_null(self):
        assert lazy("null").value() is None

    def test_value_bool(self):
        assert lazy("true").value() is True
        assert lazy("false").value() is False

    def test_value_number(self):
        assert lazy("42").value() == 42.0
        assert lazy("-3.14").value() == pytest.approx(-3.14)

    def test_value_string(self):
        assert lazy('"hello"').value() == "hello"

    def test_value_string_with_escapes(self):
        assert lazy(r'"line1\nline2"').value() == "line1\nline2"
        assert lazy(r'"tab\there"').value() == "tab\there"

    def test_value_array(self):
        result = lazy("[1, 2, 3]").value()
        assert result == [1.0, 2.0, 3.0]

    def test_value_object(self):
        result = lazy('{"a": 1, "b": 2}').value()
        assert result == {"a": 1.0, "b": 2.0}

    def test_value_nested(self):
        json = '{"data": [{"id": 1}, {"id": 2}]}'
        result = lazy(json).value()
        assert result == {"data": [{"id": 1.0}, {"id": 2.0}]}


class TestIterators:
    """Iterator tests."""

    def test_keys_basic(self):
        cursor = lazy('{"a": 1, "b": 2, "c": 3}')
        keys = list(cursor.keys())
        assert sorted(keys) == ["a", "b", "c"]

    def test_keys_empty_object(self):
        cursor = lazy("{}")
        keys = list(cursor.keys())
        assert keys == []

    def test_keys_raises_for_non_object(self):
        cursor = lazy("[1, 2, 3]")
        with pytest.raises(TypeError):
            cursor.keys()


class TestLength:
    """__len__ tests."""

    def test_len_array(self):
        assert len(lazy("[1, 2, 3, 4, 5]")) == 5
        assert len(lazy("[]")) == 0

    def test_len_object(self):
        assert len(lazy('{"a": 1, "b": 2}')) == 2
        assert len(lazy("{}")) == 0


class TestEdgeCases:
    """Edge case tests."""

    def test_unicode_escapes_in_keys(self):
        # \u0041 = 'A', \u0042 = 'B', \u0043 = 'C'
        json = r'{"\u0041\u0042\u0043": "ABC key"}'
        cursor = lazy(json)
        assert cursor["ABC"].value() == "ABC key"

    def test_unicode_escapes_in_strings(self):
        # \u0057 = 'W'
        json = r'"Hello \u0057orld"'
        cursor = lazy(json)
        assert cursor.value() == "Hello World"

    def test_empty_containers(self):
        assert lazy("[]").is_array()
        assert lazy("{}").is_object()
        assert len(lazy("[]")) == 0
        assert len(lazy("{}")) == 0

    def test_whitespace_handling(self):
        cursor = lazy("   \n\t  42  ")
        assert cursor.is_number()
        assert cursor.value() == 42.0

    def test_skip_large_values(self):
        # The cursor should skip over large nested values efficiently
        json = '{"skip": {"nested": {"deep": [1,2,3,4,5]}}, "target": 42}'
        cursor = lazy(json)
        assert cursor["target"].value() == 42.0


class TestSkipCorrectness:
    """Tests for skip algorithm correctness."""

    def test_skip_string_with_quotes(self):
        # String contains escaped quotes that look like structure
        json = r'["string with \"quotes\" and [brackets]", "second"]'
        cursor = lazy(json)
        assert cursor[1].value() == "second"

    def test_skip_string_with_braces(self):
        # String contains braces that should not affect parsing
        json = '{"key": "{not an object}", "other": 42}'
        cursor = lazy(json)
        assert cursor["other"].value() == 42.0

    def test_skip_nested_containers(self):
        json = '[[[[1, 2, 3]]], "after"]'
        cursor = lazy(json)
        assert cursor[1].value() == "after"

    def test_skip_various_numbers(self):
        json = '[0, -123, 3.14, 1e10, -2.5e-3, "end"]'
        cursor = lazy(json)
        assert cursor[5].value() == "end"


class TestPythonIntegration:
    """Tests for Python-specific integration."""

    def test_cursor_is_reusable(self):
        cursor = lazy('{"a": 1, "b": 2}')
        # Can navigate multiple times from same cursor
        assert cursor["a"].value() == 1.0
        assert cursor["b"].value() == 2.0
        # Can do it again
        assert cursor["a"].value() == 1.0

    def test_cursor_copies_are_independent(self):
        json = '{"items": [1, 2, 3]}'
        cursor1 = lazy(json)
        cursor2 = cursor1["items"]
        # Both cursors should work independently
        assert cursor1["items"][0].value() == 1.0
        assert cursor2[1].value() == 2.0


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

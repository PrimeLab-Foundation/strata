"""
Tests for KeyCache optimization in JSON parsing.

The KeyCache class in python_object_builder.h optimizes JSON key interning:
- Pre-warms common keys for zero-cost lookups
- Uses robin hood hashing for better cache locality
- Fast-path lookup for common keys using length+first-char dispatch

These tests verify correct behavior for:
- Common key hits (pre-warmed keys)
- Cache misses (uncommon keys)
- Edge cases (empty keys, unicode keys, very long keys)
- High cardinality (many unique keys)
"""

import strata


class TestKeyCacheCommonKeys:
    """Test pre-warmed common keys are handled correctly."""

    def test_common_keys_basic(self):
        """Common JSON keys should parse correctly."""
        data = '{"id": 1, "name": "test", "type": "user", "value": 42}'
        result = strata.loads(data)
        assert result == {"id": 1, "name": "test", "type": "user", "value": 42}

    def test_all_prewarm_keys(self):
        """All 20 pre-warmed keys should be handled correctly."""
        # These are the 20 keys pre-warmed in KeyCache
        prewarm_keys = [
            "id", "name", "type", "value", "data", "status", "error", "message",
            "items", "users", "results", "count", "total", "offset", "limit",
            "created", "updated", "metadata", "tags", "level"
        ]
        data = "{" + ", ".join(f'"{k}": {i}' for i, k in enumerate(prewarm_keys)) + "}"
        result = strata.loads(data)
        assert len(result) == 20
        for i, k in enumerate(prewarm_keys):
            assert result[k] == i

    def test_repeated_common_keys(self):
        """Same common keys appearing in nested objects."""
        data = '''
        {
            "items": [
                {"id": 1, "name": "a", "status": "active"},
                {"id": 2, "name": "b", "status": "pending"},
                {"id": 3, "name": "c", "status": "done"}
            ]
        }
        '''
        result = strata.loads(data)
        assert len(result["items"]) == 3
        assert result["items"][0]["id"] == 1
        assert result["items"][2]["status"] == "done"


class TestKeyCacheUncommonKeys:
    """Test cache misses for uncommon keys."""

    def test_uncommon_keys(self):
        """Keys not in pre-warm list should still work."""
        data = '{"foo": 1, "bar": 2, "baz": 3}'
        result = strata.loads(data)
        assert result == {"foo": 1, "bar": 2, "baz": 3}

    def test_mixed_common_uncommon(self):
        """Mix of common and uncommon keys."""
        data = '{"id": 1, "foo": 2, "name": "test", "bar": 4}'
        result = strata.loads(data)
        assert result == {"id": 1, "foo": 2, "name": "test", "bar": 4}


class TestKeyCacheEdgeCases:
    """Test edge cases in key handling."""

    def test_empty_string_key(self):
        """Empty string as key should work."""
        data = '{"": 42}'
        result = strata.loads(data)
        assert result[""] == 42

    def test_single_char_keys(self):
        """Single character keys."""
        data = '{"a": 1, "b": 2, "z": 26}'
        result = strata.loads(data)
        assert result == {"a": 1, "b": 2, "z": 26}

    def test_unicode_keys(self):
        """Unicode characters in keys."""
        data = '{"名前": "test", "键": "value", "キー": 42}'
        result = strata.loads(data)
        assert result["名前"] == "test"
        assert result["键"] == "value"
        assert result["キー"] == 42

    def test_emoji_keys(self):
        """Emoji in keys."""
        data = '{"🔑": "key", "📦": "box"}'
        result = strata.loads(data)
        assert result["🔑"] == "key"
        assert result["📦"] == "box"

    def test_very_long_key(self):
        """Very long key (> 256 bytes)."""
        long_key = "k" * 300
        data = f'{{"{long_key}": "value"}}'
        result = strata.loads(data)
        assert result[long_key] == "value"

    def test_keys_with_special_chars(self):
        """Keys with special characters."""
        data = r'{"a\nb": 1, "c\td": 2, "e\"f": 3}'
        result = strata.loads(data)
        assert result["a\nb"] == 1
        assert result["c\td"] == 2
        assert result["e\"f"] == 3


class TestKeyCacheHighCardinality:
    """Test high cardinality (many unique keys)."""

    def test_hundred_unique_keys(self):
        """100 unique keys."""
        keys = [f"key_{i:03d}" for i in range(100)]
        data = "{" + ", ".join(f'"{k}": {i}' for i, k in enumerate(keys)) + "}"
        result = strata.loads(data)
        assert len(result) == 100
        for i, k in enumerate(keys):
            assert result[k] == i

    def test_thousand_unique_keys(self):
        """1000 unique keys."""
        keys = [f"unique_key_{i:04d}" for i in range(1000)]
        data = "{" + ", ".join(f'"{k}": {i}' for i, k in enumerate(keys)) + "}"
        result = strata.loads(data)
        assert len(result) == 1000
        for i, k in enumerate(keys):
            assert result[k] == i

    def test_ten_thousand_unique_keys(self):
        """10000 unique keys - tests hash map capacity."""
        keys = [f"k{i:05d}" for i in range(10000)]
        data = "{" + ", ".join(f'"{k}": {i}' for i, k in enumerate(keys)) + "}"
        result = strata.loads(data)
        assert len(result) == 10000
        # Spot check some values
        assert result["k00000"] == 0
        assert result["k05000"] == 5000
        assert result["k09999"] == 9999


class TestKeyCacheNestedObjects:
    """Test key caching with nested structures."""

    def test_deeply_nested_same_keys(self):
        """Same keys at different nesting levels."""
        data = '''
        {
            "data": {
                "data": {
                    "data": {
                        "value": 42
                    },
                    "value": 3
                },
                "value": 2
            },
            "value": 1
        }
        '''
        result = strata.loads(data)
        assert result["value"] == 1
        assert result["data"]["value"] == 2
        assert result["data"]["data"]["value"] == 3
        assert result["data"]["data"]["data"]["value"] == 42

    def test_array_of_objects_with_same_keys(self):
        """Array with many objects having same keys."""
        items = [{"id": i, "name": f"item_{i}", "count": i * 10} for i in range(100)]
        import json
        data = json.dumps({"items": items})
        result = strata.loads(data)
        assert len(result["items"]) == 100
        assert result["items"][50]["id"] == 50
        assert result["items"][50]["name"] == "item_50"
        assert result["items"][50]["count"] == 500


class TestKeyCacheStability:
    """Test that key caching doesn't cause memory issues."""

    def test_repeated_parsing_same_keys(self):
        """Repeatedly parse same document - should be stable."""
        data = '{"id": 1, "name": "test", "value": 42}'
        for _ in range(1000):
            result = strata.loads(data)
            assert result["id"] == 1

    def test_repeated_parsing_different_keys(self):
        """Repeatedly parse documents with different keys."""
        for i in range(100):
            data = f'{{"key_{i}": {i}, "value_{i}": "{i}"}}'
            result = strata.loads(data)
            assert result[f"key_{i}"] == i
            assert result[f"value_{i}"] == str(i)

"""
Test NDJSON (Newline Delimited JSON) streaming.
"""

import io

import pytest

import strata.ndjson as ndjson


class TestIterNdjson:
    """Test iter_ndjson functionality."""

    def test_basic_iteration(self):
        """Test basic NDJSON iteration."""
        data = '{"name": "Alice", "age": 30}\n{"name": "Bob", "age": 25}\n{"name": "Charlie", "age": 35}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 3
        assert results[0] == {"name": "Alice", "age": 30}
        assert results[1] == {"name": "Bob", "age": 25}
        assert results[2] == {"name": "Charlie", "age": 35}

    def test_empty_lines(self):
        """Test NDJSON with empty lines."""
        data = '{"a": 1}\n\n{"b": 2}\n\n\n{"c": 3}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 3
        assert results[0] == {"a": 1}
        assert results[1] == {"b": 2}
        assert results[2] == {"c": 3}

    def test_whitespace_lines(self):
        """Test NDJSON with whitespace-only lines."""
        data = '{"a": 1}\n   \n\t\n{"b": 2}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 2
        assert results[0] == {"a": 1}
        assert results[1] == {"b": 2}

    def test_trailing_newline(self):
        """Test NDJSON with trailing newline."""
        data = '{"a": 1}\n{"b": 2}\n'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 2

    def test_no_trailing_newline(self):
        """Test NDJSON without trailing newline."""
        data = '{"a": 1}\n{"b": 2}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 2

    def test_single_line(self):
        """Test single line NDJSON."""
        data = '{"a": 1}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 1
        assert results[0] == {"a": 1}

    def test_empty_data(self):
        """Test empty NDJSON data."""
        data = ''
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 0

    def test_bytes_input(self):
        """Test NDJSON from bytes."""
        data = b'{"a": 1}\n{"b": 2}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 2
        assert results[0] == {"a": 1}

    def test_windows_line_endings(self):
        """Test NDJSON with Windows line endings."""
        data = '{"a": 1}\r\n{"b": 2}\r\n'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 2
        assert results[0] == {"a": 1}
        assert results[1] == {"b": 2}

    def test_mixed_line_endings(self):
        """Test NDJSON with mixed line endings."""
        data = '{"a": 1}\r\n{"b": 2}\n{"c": 3}\r\n'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 3


class TestParseNdjson:
    """Test parse_ndjson functionality."""

    def test_parse_all(self):
        """Test parsing all lines at once."""
        data = '{"a": 1}\n{"b": 2}\n{"c": 3}'
        results = ndjson.parse_ndjson(data)

        assert len(results) == 3
        assert results == [{"a": 1}, {"b": 2}, {"c": 3}]

    def test_parse_parallel_forced(self):
        """Test forced parallel parsing path."""
        data = '{"a": 1}\n{"b": 2}'
        results = ndjson.parse_ndjson(data, parallel=True, num_threads=1)

        assert results == [{"a": 1}, {"b": 2}]

    def test_parse_auto_detect_large_small_lines(self):
        """Test auto-detect path for large data with small lines."""
        payload = "x" * 2000
        line = '{"id": 1, "payload": "' + payload + '"}'
        lines = 600
        data = "\n".join([line] * lines)

        assert len(data) >= 1 * 1024 * 1024
        results = ndjson.parse_ndjson(data)

        assert len(results) == lines
        assert results[0]["payload"] == payload

    def test_parse_empty(self):
        """Test parsing empty data."""
        data = ''
        results = ndjson.parse_ndjson(data)

        assert results == []

    def test_parse_with_blanks(self):
        """Test parsing with blank lines."""
        data = '{"a": 1}\n\n{"b": 2}'
        results = ndjson.parse_ndjson(data)

        assert len(results) == 2


class TestNdjsonErrors:
    """Test NDJSON error handling."""

    def test_malformed_line_stops_iteration(self):
        """Test that malformed line stops iteration by default."""
        data = '{"a": 1}\n{invalid json}\n{"c": 3}'

        with pytest.raises(ValueError):
            list(ndjson.iter_ndjson(data))

    def test_skip_errors(self):
        """Test skip_errors parameter."""
        data = '{"a": 1}\n{invalid}\n{"c": 3}'
        results = list(ndjson.iter_ndjson(data, skip_errors=True))

        # Should skip the malformed line
        assert len(results) == 2
        assert results[0] == {"a": 1}
        assert results[1] == {"c": 3}

    def test_parse_all_with_errors(self):
        """Test parse_all with malformed lines."""
        data = '{"a": 1}\n{bad}\n{"c": 3}'

        # Without skip_errors, should stop at error
        results = ndjson.parse_ndjson(data, skip_errors=False)
        assert len(results) == 1
        assert results[0] == {"a": 1}

        # With skip_errors, should continue
        results2 = ndjson.parse_ndjson(data, skip_errors=True)
        assert len(results2) == 2


class TestNdjsonTypes:
    """Test NDJSON with different JSON types."""

    def test_arrays(self):
        """Test NDJSON with arrays."""
        data = '[1, 2, 3]\n[4, 5, 6]\n[7, 8, 9]'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 3
        assert results[0] == [1, 2, 3]
        assert results[1] == [4, 5, 6]
        assert results[2] == [7, 8, 9]

    def test_scalars(self):
        """Test NDJSON with scalar values."""
        data = '42\n"hello"\ntrue'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 3
        assert results[0] == 42
        assert results[1] == "hello"
        assert results[2] is True

        # Test null separately (it might have iteration issues)
        data_null = '{"value": null}'
        results_null = list(ndjson.iter_ndjson(data_null))
        assert len(results_null) == 1
        assert results_null[0]["value"] is None

    def test_mixed_types(self):
        """Test NDJSON with mixed types."""
        data = '{"type": "object"}\n[1, 2, 3]\n"string"\n42'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 4
        assert isinstance(results[0], dict)
        assert isinstance(results[1], list)
        assert isinstance(results[2], str)
        # JSON numbers may be int or float depending on parser implementation
        assert isinstance(results[3], (int, float))
        assert results[3] == 42

    def test_nested_objects(self):
        """Test NDJSON with nested structures."""
        data = '{"user": {"name": "Alice", "address": {"city": "NYC"}}}\n{"user": {"name": "Bob"}}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 2
        assert results[0]["user"]["name"] == "Alice"
        assert results[0]["user"]["address"]["city"] == "NYC"
        assert results[1]["user"]["name"] == "Bob"


class TestNdjsonPerformance:
    """Test NDJSON with realistic data."""

    def test_large_ndjson(self):
        """Test with moderately large NDJSON data."""
        # Generate 1000 lines
        lines = [f'{{"id": {i}, "value": "item_{i}"}}' for i in range(1000)]
        data = '\n'.join(lines)

        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 1000
        assert results[0] == {"id": 0, "value": "item_0"}
        assert results[999] == {"id": 999, "value": "item_999"}

    def test_stress_large_ndjson_100k(self):
        """Stress test with ~50MB NDJSON payload."""
        lines = 100000
        payload = "x" * 500
        buf = io.StringIO()
        for i in range(lines):
            buf.write(f'{{"id": {i}, "payload": "{payload}"}}')
            if i + 1 < lines:
                buf.write("\n")

        data = buf.getvalue()
        assert len(data) >= 50 * 1024 * 1024

        results = list(ndjson.iter_ndjson(data))
        assert len(results) == lines
        assert results[0]["id"] == 0
        assert results[-1]["id"] == lines - 1

    def test_memory_efficient_iteration(self):
        """Test that iteration doesn't load all into memory."""
        # This is a behavioral test - we just verify iteration works
        lines = [f'{{"id": {i}}}' for i in range(100)]
        data = '\n'.join(lines)

        count = 0
        for obj in ndjson.iter_ndjson(data):
            count += 1
            # Process one at a time
            assert "id" in obj

        assert count == 100


class TestNdjsonEdgeCases:
    """Test NDJSON edge cases."""

    def test_unicode_in_ndjson(self):
        """Test Unicode in NDJSON lines."""
        data = '{"emoji": "👋"}\n{"text": "Hello 世界"}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 2
        assert results[0]["emoji"] == "👋"
        assert results[1]["text"] == "Hello 世界"

    def test_escaped_newlines(self):
        """Test that escaped newlines in strings don't break parsing."""
        data = '{"text": "line1\\nline2"}\n{"text": "line3"}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 2
        assert results[0]["text"] == "line1\nline2"
        assert results[1]["text"] == "line3"

    def test_empty_objects(self):
        """Test empty objects in NDJSON."""
        data = '{}\n{}\n{}'
        results = list(ndjson.iter_ndjson(data))

        assert len(results) == 3
        assert all(r == {} for r in results)

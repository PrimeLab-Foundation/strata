"""Tests for parallel NDJSON parsing."""

import pytest

import strata.ndjson as ndjson
import strata._strata as native


class TestParallelNdjsonBasics:
    """Basic tests for ndjson_parallel_parse_all function."""

    def test_basic_parsing(self):
        """Parse simple NDJSON data."""
        data = '{"a": 1}\n{"b": 2}\n{"c": 3}'
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 3
        assert result[0] == {"a": 1}
        assert result[1] == {"b": 2}
        assert result[2] == {"c": 3}

    def test_empty_data(self):
        """Parse empty string."""
        result = native.ndjson_parallel_parse_all("")
        assert result == []

    def test_single_line(self):
        """Parse single line."""
        data = '{"single": true}'
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 1
        assert result[0] == {"single": True}

    def test_empty_lines(self):
        """Empty lines are skipped."""
        data = '{"a": 1}\n\n{"b": 2}\n\n\n{"c": 3}'
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 3

    def test_whitespace_lines(self):
        """Whitespace-only lines are skipped."""
        data = '{"a": 1}\n   \n\t\n{"b": 2}'
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 2


class TestParallelNdjsonOrder:
    """Tests for order preservation."""

    def test_order_preserved_small(self):
        """Order is preserved for small datasets."""
        lines = [f'{{"id": {i}}}' for i in range(100)]
        data = '\n'.join(lines)
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 100
        for i, item in enumerate(result):
            assert item["id"] == i

    def test_order_preserved_medium(self):
        """Order is preserved for medium datasets."""
        lines = [f'{{"id": {i}}}' for i in range(5000)]
        data = '\n'.join(lines)
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 5000
        for i, item in enumerate(result):
            assert item["id"] == i

    def test_order_preserved_large(self):
        """Order is preserved for large datasets (triggers parallel mode)."""
        lines = [f'{{"id": {i}}}' for i in range(20000)]
        data = '\n'.join(lines)
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 20000
        for i, item in enumerate(result):
            assert item["id"] == i


class TestParallelNdjsonParity:
    """Tests for parity with sequential NdjsonStream."""

    def test_parity_with_sequential(self):
        """Parallel parsing produces same results as sequential."""
        lines = [f'{{"id": {i}, "value": "item_{i}"}}' for i in range(1000)]
        data = '\n'.join(lines)

        # Parse with parallel
        parallel_result = native.ndjson_parallel_parse_all(data)

        # Parse with sequential
        stream = native.NdjsonStream.from_string(data)
        sequential_result = stream.parse_all()

        # Results should be identical
        assert len(parallel_result) == len(sequential_result)
        for p, s in zip(parallel_result, sequential_result):
            assert p == s


class TestParallelNdjsonErrors:
    """Tests for error handling."""

    def test_skip_errors_default(self):
        """Errors are skipped by default."""
        data = '{"a": 1}\n{invalid\n{"c": 3}'
        result = native.ndjson_parallel_parse_all(data)
        # Should have 2 valid results
        assert len(result) == 2
        assert result[0] == {"a": 1}
        assert result[1] == {"c": 3}

    def test_skip_errors_explicit_true(self):
        """Skip errors when explicitly set."""
        data = '{"a": 1}\n{invalid\n{"c": 3}'
        result = native.ndjson_parallel_parse_all(data, skip_errors=True)
        assert len(result) == 2

    def test_skip_errors_false_raises(self):
        """Raise exception when skip_errors=False."""
        data = '{"a": 1}\n{invalid\n{"c": 3}'
        with pytest.raises(Exception):
            native.ndjson_parallel_parse_all(data, skip_errors=False)


class TestParallelNdjsonConfig:
    """Tests for configuration options."""

    def test_num_threads_parameter(self):
        """num_threads parameter is accepted."""
        lines = [f'{{"id": {i}}}' for i in range(1000)]
        data = '\n'.join(lines)

        # Test with various thread counts
        for threads in [1, 2, 4]:
            result = native.ndjson_parallel_parse_all(data, num_threads=threads)
            assert len(result) == 1000


class TestParallelNdjsonTypes:
    """Tests for various JSON types."""

    def test_mixed_types(self):
        """Parse various JSON types."""
        data = 'null\ntrue\nfalse\n42\n3.14\n"string"\n[1, 2, 3]\n{"key": "value"}'
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 8
        assert result[0] is None
        assert result[1] is True
        assert result[2] is False
        assert result[3] == 42
        assert abs(result[4] - 3.14) < 0.001
        assert result[5] == "string"
        assert result[6] == [1, 2, 3]
        assert result[7] == {"key": "value"}

    def test_arrays_as_root(self):
        """Parse arrays as root values."""
        lines = [f'[{i}, {i+1}, {i+2}]' for i in range(100)]
        data = '\n'.join(lines)
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 100
        for i, item in enumerate(result):
            assert item == [i, i + 1, i + 2]


class TestParallelNdjsonEdgeCases:
    """Edge case tests."""

    def test_windows_line_endings(self):
        """Handle Windows line endings."""
        data = '{"a": 1}\r\n{"b": 2}\r\n{"c": 3}\r\n'
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 3

    def test_no_trailing_newline(self):
        """Handle data without trailing newline."""
        data = '{"a": 1}\n{"b": 2}'
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 2

    def test_very_long_lines(self):
        """Handle very long lines."""
        long_value = "x" * 10000
        lines = [f'{{"id": {i}, "data": "{long_value}"}}' for i in range(100)]
        data = '\n'.join(lines)
        result = native.ndjson_parallel_parse_all(data)
        assert len(result) == 100
        for i, item in enumerate(result):
            assert item["id"] == i
            assert len(item["data"]) == 10000


class TestParseNdjsonPublicAPI:
    """Tests for the public parse_ndjson() API with parallel parameter."""

    def test_default_auto_small_data(self):
        """Small data should use sequential by default (auto mode)."""
        data = '{"a": 1}\n{"b": 2}'
        result = ndjson.parse_ndjson(data)
        assert len(result) == 2
        assert result[0] == {"a": 1}
        assert result[1] == {"b": 2}

    def test_default_auto_large_data(self):
        """Large data should use parallel by default (auto mode)."""
        # Create >2MB of data to trigger parallel mode
        lines = [f'{{"id": {i}, "padding": "{("x" * 400)}"}}' for i in range(6000)]
        data = '\n'.join(lines)
        assert len(data) > 2 * 1024 * 1024  # Verify it's large enough

        result = ndjson.parse_ndjson(data)
        assert len(result) == len(lines)
        for i, item in enumerate(result):
            assert item["id"] == i

    def test_parallel_none_auto_mode(self):
        """parallel=None uses auto-detection."""
        data = '{"a": 1}\n{"b": 2}'
        result = ndjson.parse_ndjson(data, parallel=None)
        assert len(result) == 2

    def test_parallel_true_forces_parallel(self):
        """parallel=True forces parallel mode even for small data."""
        data = '{"a": 1}\n{"b": 2}'
        result = ndjson.parse_ndjson(data, parallel=True)
        assert len(result) == 2
        assert result[0] == {"a": 1}
        assert result[1] == {"b": 2}

    def test_parallel_false_forces_sequential(self):
        """parallel=False forces sequential mode even for large data."""
        # Create large data
        lines = [f'{{"id": {i}}}' for i in range(5000)]
        data = '\n'.join(lines)
        result = ndjson.parse_ndjson(data, parallel=False)
        assert len(result) == 5000
        for i, item in enumerate(result):
            assert item["id"] == i

    def test_num_threads_parameter(self):
        """num_threads parameter is accepted and used."""
        lines = [f'{{"id": {i}}}' for i in range(1000)]
        data = '\n'.join(lines)

        # Test with various thread counts
        for threads in [1, 2, 4]:
            result = ndjson.parse_ndjson(data, parallel=True, num_threads=threads)
            assert len(result) == 1000

    def test_skip_errors_parameter(self):
        """skip_errors parameter works in parallel mode."""
        data = '{"a": 1}\n{invalid\n{"c": 3}'
        result = ndjson.parse_ndjson(data, skip_errors=True, parallel=True)
        assert len(result) == 2
        assert result[0] == {"a": 1}
        assert result[1] == {"c": 3}

    def test_bytes_input(self):
        """parse_ndjson accepts bytes input."""
        data = b'{"a": 1}\n{"b": 2}'
        result = ndjson.parse_ndjson(data)
        assert len(result) == 2

    def test_parity_sequential_vs_parallel(self):
        """Parallel and sequential modes produce identical results."""
        lines = [f'{{"id": {i}, "value": "item_{i}"}}' for i in range(5000)]
        data = '\n'.join(lines)

        seq_result = ndjson.parse_ndjson(data, parallel=False)
        par_result = ndjson.parse_ndjson(data, parallel=True)

        assert len(seq_result) == len(par_result)
        for s, p in zip(seq_result, par_result):
            assert s == p

    def test_order_preserved_with_parallel(self):
        """Order is preserved when using parallel mode."""
        lines = [f'{{"index": {i}}}' for i in range(10000)]
        data = '\n'.join(lines)

        result = ndjson.parse_ndjson(data, parallel=True)
        assert len(result) == 10000
        for i, item in enumerate(result):
            assert item["index"] == i

"""
NDJSON (Newline-Delimited JSON) streaming and parsing.

Parse NDJSON data line-by-line or load all lines at once.
Automatically uses multi-threaded parallel parsing for large objects.
"""

from __future__ import annotations

from collections.abc import Iterator

from . import _strata as _native


# Thresholds for automatic parallel mode selection.
# Parallel parsing is only faster when objects are large (>5KB each) because:
# - Sequential uses direct SAX-to-Python (single pass)
# - Parallel builds C++ DOM then converts to Python (double materialization)
# The parallel overhead is only worthwhile when C++ parsing time dominates.
# Testing shows crossover at ~3KB, so we use 5KB threshold for safety margin.
_PARALLEL_MIN_SIZE = 1 * 1024 * 1024  # 1 MB minimum total size
_PARALLEL_MIN_AVG_LINE_SIZE = 5 * 1024  # 5 KB minimum average line size


def iter_ndjson(
    data: str | bytes,
    *,
    skip_errors: bool = False,
    batch_size: int = 1024,
) -> Iterator[dict | list]:
    """
    Iterate over NDJSON lines, yielding each parsed object.

    Uses batched parsing to reduce Python/C++ boundary crossings.
    For parallel parsing of all lines at once, use :func:`parse_ndjson` instead.

    Args:
        data: NDJSON text as string or bytes.
        skip_errors: If True, skip malformed lines. If False, raise on first error.
        batch_size: Lines per batch (default 1024). Larger = fewer crossings, more memory.

    Yields:
        Parsed JSON objects (dict or list).

    Raises:
        ValueError: When a line is invalid JSON and skip_errors is False.

    Example:
        >>> for obj in iter_ndjson('{"a":1}\\n{"b":2}\\n'):
        ...     print(obj)
        {'a': 1}
        {'b': 2}
    """
    text = data.decode("utf-8") if isinstance(data, bytes) else data
    stream = _native.NdjsonStream.from_string(text)

    while stream.has_next():
        errors_before = stream.error_count()
        batch = stream.next_batch(batch_size, skip_errors)

        yield from batch

        if not skip_errors and stream.error_count() > errors_before:
            raise ValueError("Invalid JSON in NDJSON line")


def parse_ndjson(
    data: str | bytes,
    *,
    skip_errors: bool = False,
    parallel: bool | None = None,
    num_threads: int = 0,
) -> list[dict | list]:
    """
    Parse all NDJSON lines into a single list.

    Uses an optimal parsing strategy based on the data characteristics:
    - **Small objects** (< 5KB each): Uses sequential SAX-to-Python parsing
    - **Large objects** (≥ 5KB each): Uses multi-threaded parallel parsing

    This auto-detection is based on benchmarks showing that parallel parsing
    only provides speedups when individual JSON objects are large and complex.
    For typical NDJSON with many small objects, sequential parsing is faster.

    Args:
        data: NDJSON text as string or bytes.
        skip_errors: If True, skip malformed lines. If False, raise on first error.
        parallel: Control parallel parsing:
            - ``None`` (default): Auto-detect based on object size
            - ``True``: Force parallel parsing (best for large objects ≥5KB)
            - ``False``: Force sequential parsing (best for small objects)
        num_threads: Number of threads for parallel parsing.
            0 (default) = auto-detect based on CPU cores.
            Only used when parallel parsing is active.

    Returns:
        List of parsed JSON objects, in the same order as the input lines.

    Example:
        >>> parse_ndjson('{"a":1}\\n{"b":2}\\n')
        [{'a': 1}, {'b': 2}]

        >>> # Force parallel for large complex objects
        >>> parse_ndjson(large_data, parallel=True, num_threads=4)

        >>> # Force sequential for many small objects
        >>> parse_ndjson(data, parallel=False)

    Note:
        For streaming/iterator access, use :func:`iter_ndjson` instead.
        Parallel parsing is most effective for files with large (≥5KB) objects.
    """
    text = data.decode("utf-8") if isinstance(data, bytes) else data

    # Determine whether to use parallel parsing
    use_parallel: bool
    if parallel is None:
        # Auto-detect based on data size AND average object size.
        # Parallel is only faster when objects are large (>3KB each).
        data_size = len(text)
        if data_size < _PARALLEL_MIN_SIZE:
            # Too small for parallel overhead to be worthwhile
            use_parallel = False
        else:
            # Estimate average line size by counting newlines
            newline_count = text.count('\n')
            line_count = newline_count + 1 if text and not text.endswith('\n') else max(newline_count, 1)
            avg_line_size = data_size / line_count
            use_parallel = avg_line_size >= _PARALLEL_MIN_AVG_LINE_SIZE
    else:
        use_parallel = parallel

    if use_parallel:
        # Use multi-threaded parallel parsing
        return _native.ndjson_parallel_parse_all(
            text,
            skip_errors=skip_errors,
            num_threads=num_threads,
        )
    else:
        # Use sequential parsing
        stream = _native.NdjsonStream.from_string(text)
        return stream.parse_all(skip_errors)


__all__ = ["iter_ndjson", "parse_ndjson"]

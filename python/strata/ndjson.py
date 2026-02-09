"""
NDJSON (Newline-Delimited JSON) streaming and parsing.

Parse NDJSON data line-by-line or load all lines at once.
Automatically uses multi-threaded parallel parsing for large objects.
"""

from __future__ import annotations

from collections.abc import Iterator
from pathlib import Path

from . import _strata as _native


# Thresholds for automatic parallel mode selection.
# Parallel parsing is useful once total input is large enough to amortize
# thread-pool overhead. For smaller lines, use larger chunks to reduce per-task
# overhead from submission/collection/merge.
_PARALLEL_MIN_SIZE = 2 * 1024 * 1024  # 2 MB minimum total size
_PARALLEL_SMALL_LINE_THRESHOLD = 4 * 1024  # 4 KB average line size
_PARALLEL_SMALL_LINE_CHUNK_SIZE = 2 * 1024 * 1024  # 2 MB minimum chunk size


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
    - **Small inputs** (< 2MB total): Uses sequential SAX-to-Python parsing
    - **Large inputs** (≥ 2MB total): Uses multi-threaded parallel parsing
      (with larger chunks for smaller average line sizes to amortize overhead)

    This auto-detection is based on benchmarks showing that parallel parsing
    only provides speedups once total input is large enough to amortize overhead.
    For smaller inputs, sequential parsing is faster.

    Args:
        data: NDJSON text as string or bytes.
        skip_errors: If True, skip malformed lines. If False, raise on first error.
        parallel: Control parallel parsing:
            - ``None`` (default): Auto-detect based on total size, adjust chunking for line size
            - ``True``: Force parallel parsing
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
        Parallel parsing is most effective when total data is ≥2MB. For smaller
        average line sizes, larger chunks are used to reduce per-task overhead.
    """
    text = data.decode("utf-8") if isinstance(data, bytes) else data

    # Determine whether to use parallel parsing
    use_parallel: bool
    min_chunk_size = 0
    if parallel is None:
        # Auto-detect based primarily on total size. For smaller lines, use
        # larger chunks to reduce per-task overhead.
        data_size = len(text)
        if data_size < _PARALLEL_MIN_SIZE:
            # Too small for parallel overhead to be worthwhile
            use_parallel = False
        else:
            newline_count = text.count('\n')
            line_count = newline_count + 1 if text and not text.endswith('\n') else max(newline_count, 1)
            avg_line_size = data_size / line_count
            use_parallel = True
            if (avg_line_size < _PARALLEL_SMALL_LINE_THRESHOLD and
                    data_size >= _PARALLEL_SMALL_LINE_CHUNK_SIZE * 2):
                min_chunk_size = _PARALLEL_SMALL_LINE_CHUNK_SIZE
    else:
        use_parallel = parallel

    if use_parallel:
        # Use multi-threaded parallel parsing
        return _native.ndjson_parallel_parse_all(
            text,
            skip_errors=skip_errors,
            num_threads=num_threads,
            min_chunk_size=min_chunk_size,
        )
    else:
        # Use sequential parsing
        stream = _native.NdjsonStream.from_string(text)
        return stream.parse_all(skip_errors)

def parse_ndjson_cursor(
    data: str | bytes,
    *,
    skip_errors: bool = False,
    on_error: str | None = None,
) -> _native.NdjsonCursor:
    """
    Parse NDJSON text into a cursor for repeated JSONPath queries.

    Args:
        data: NDJSON text as string or bytes.
        skip_errors: If True, skip malformed lines. If False, raise on first error.
        on_error: Override error handling ("skip", "warn", or "error").
    """
    return _native.NdjsonCursor.from_string(data, skip_errors=skip_errors, on_error=on_error)


def parse_ndjson_file(
    path: str | Path,
    *,
    skip_errors: bool = False,
    on_error: str | None = None,
) -> _native.NdjsonCursor:
    """
    Parse an NDJSON file into a cursor for repeated JSONPath queries.

    Args:
        path: File path to the NDJSON file.
        skip_errors: If True, skip malformed lines. If False, raise on first error.
        on_error: Override error handling ("skip", "warn", or "error").
    """
    path_str = str(path) if isinstance(path, Path) else path
    return _native.NdjsonCursor.from_file(path_str, skip_errors=skip_errors, on_error=on_error)


NdjsonCursor = _native.NdjsonCursor


__all__ = [
    "iter_ndjson",
    "parse_ndjson",
    "parse_ndjson_cursor",
    "parse_ndjson_file",
    "NdjsonCursor",
]

"""
NDJSON (Newline-Delimited JSON) streaming and parsing.

Parse NDJSON data line-by-line or load all lines at once.
Automatically uses multi-threaded parallel parsing for large objects.
"""

from __future__ import annotations

from collections.abc import Iterator
from pathlib import Path

from . import _strata as _native


def iter_ndjson(
    data: str | bytes,
    **kwargs,
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
    return _native.iter_ndjson(data, **kwargs)


def parse_ndjson(
    data: str | bytes,
    **kwargs,
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
    return _native.parse_ndjson(data, **kwargs)


def parse_ndjson_cursor(
    data: str | bytes,
    **kwargs,
) -> _native.NdjsonCursor:
    """
    Parse NDJSON text into a cursor for repeated JSONPath queries.

    Args:
        data: NDJSON text as string or bytes.
        skip_errors: If True, skip malformed lines. If False, raise on first error.
        on_error: Override error handling ("skip", "warn", or "error").
    """
    return _native.NdjsonCursor.from_string(data, **kwargs)


def parse_ndjson_file(
    path: str | Path,
    **kwargs,
) -> _native.NdjsonCursor:
    """
    Parse an NDJSON file into a cursor for repeated JSONPath queries.

    Args:
        path: File path to the NDJSON file.
        skip_errors: If True, skip malformed lines. If False, raise on first error.
        on_error: Override error handling ("skip", "warn", or "error").
    """
    return _native.NdjsonCursor.from_file(path, **kwargs)


def parse_ndjson_file_lazy(
    path: str | Path,
    **kwargs,
) -> _native.NdjsonCursor:
    """
    Create a lazy NDJSON cursor from a file path.

    Args:
        path: File path to the NDJSON file.
        skip_errors: If True, skip malformed lines. If False, raise on first error.
        on_error: Override error handling ("skip", "warn", or "error").
    """
    return _native.NdjsonCursor.from_file_lazy(path, **kwargs)


NdjsonCursor = _native.NdjsonCursor


__all__ = [
    "iter_ndjson",
    "parse_ndjson",
    "parse_ndjson_cursor",
    "parse_ndjson_file",
    "parse_ndjson_file_lazy",
    "NdjsonCursor",
]

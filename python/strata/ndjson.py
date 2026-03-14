"""
NDJSON (Newline-Delimited JSON) streaming.

Parse NDJSON data line-by-line or load all lines at once.
"""

from __future__ import annotations

from collections.abc import Iterator

from . import _strata as _native


def iter_ndjson(
    data: str | bytes,
    *,
    skip_errors: bool = False,
    batch_size: int = 1024,
) -> Iterator[dict | list]:
    """
    Iterate over NDJSON lines, yielding each parsed object.

    Uses batched parsing to reduce Python/C++ boundary crossings.

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


def parse_ndjson(data: str | bytes, *, skip_errors: bool = False) -> list[dict | list]:
    """
    Parse all NDJSON lines into a single list.

    Best when you need the full result in memory. For streaming, use iter_ndjson.

    Args:
        data: NDJSON text as string or bytes.
        skip_errors: If True, skip malformed lines. If False, raise on first error.

    Returns:
        List of parsed JSON objects.

    Example:
        >>> parse_ndjson('{"a":1}\\n{"b":2}\\n')
        [{'a': 1}, {'b': 2}]
    """
    text = data.decode("utf-8") if isinstance(data, bytes) else data
    return _native.parse_ndjson(text, skip_errors)


__all__ = ["iter_ndjson", "parse_ndjson"]

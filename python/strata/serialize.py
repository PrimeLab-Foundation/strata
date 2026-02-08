"""
JSON serialization and parsing.

Thin Python wrappers around the C++ engine for loads, dumps, and dumps_bytes.
"""

from __future__ import annotations

from . import _strata as _native

# -----------------------------------------------------------------------------
# Parsing (JSON text → Python object)
# -----------------------------------------------------------------------------


def loads(source: str | bytes) -> dict | list | str | int | float | bool | None:
    """
    Parse JSON text into a Python object.

    Args:
        source: JSON as a string (UTF-8) or raw bytes. Use bytes when testing
            invalid UTF-8 handling, since Python strings are always valid Unicode.

    Returns:
        Parsed value: dict, list, str, int, float, bool, or None.

    Raises:
        ValueError: If the JSON is invalid or contains invalid UTF-8.

    Example:
        >>> loads('{"name": "Alice", "age": 30}')
        {'name': 'Alice', 'age': 30}
    """
    return _native.loads(source)


def loads_tape(source: str | bytes) -> dict | list | str | int | float | bool | None:
    """
    Parse JSON text into a Python object using tape format.

    This is an alternative parsing path that first parses JSON into a compact
    token tape, then builds Python objects from the tape. Useful for:
    - Benchmarking tape-based parsing
    - Cases where the same JSON will be parsed multiple times

    Args:
        source: JSON as a string (UTF-8) or raw bytes.

    Returns:
        Parsed value: dict, list, str, int, float, bool, or None.

    Raises:
        ValueError: If the JSON is invalid or contains invalid UTF-8.

    Example:
        >>> loads_tape('{"name": "Alice", "age": 30}')
        {'name': 'Alice', 'age': 30}
    """
    return _native.loads_tape(source)


# -----------------------------------------------------------------------------
# Serialization (Python object → JSON text)
# -----------------------------------------------------------------------------


def dumps(obj: dict | list | str | int | float | bool | None) -> str:
    """
    Serialize a Python object to a JSON string.

    Args:
        obj: A JSON-serializable value (dict, list, str, int, float, bool, None).

    Returns:
        JSON string representation.

    Raises:
        ValueError: If the object cannot be serialized.
        TypeError: If the object type is not supported.

    Example:
        >>> dumps({"name": "Alice", "age": 30})
        '{"name":"Alice","age":30}'
    """
    return _native.dumps(obj)


def dumps_bytes(obj: dict | list | str | int | float | bool | None) -> bytes:
    """
    Serialize a Python object to JSON bytes (UTF-8).

    Faster than dumps() when you need bytes, since it avoids string encoding.

    Args:
        obj: A JSON-serializable value.

    Returns:
        JSON as UTF-8 encoded bytes.

    Example:
        >>> dumps_bytes({"key": "value"})
        b'{"key":"value"}'
    """
    return _native.dumps_bytes(obj)


def set_duplicate_key_policy(policy: str) -> None:
    """
    Configure how duplicate object keys are handled when parsing.

    Allowed values:
        - "first": keep the first value (default)
        - "last": keep the last value
        - "error": raise ValueError
        - "warn": emit RuntimeWarning, keep first
    """
    _native.set_duplicate_key_policy(policy)


def set_cycle_policy(policy: str) -> None:
    """
    Configure how cycles are handled during serialization.

    Allowed values:
        - "warn": emit RuntimeWarning and write null (default)
        - "error": raise ValueError
        - "ignore": write null silently
        - "nocheck": skip cycle detection entirely (fastest)
    """
    _native.set_cycle_policy(policy)


def set_dumps_type_order(policy: str) -> None:
    """
    Configure type-check ordering for dumps.

    Allowed values:
        - "strings_first": check strings before ints (default)
        - "ints_first": check ints before strings
    """
    _native.set_dumps_type_order(policy)


__all__ = [
    "loads",
    "loads_tape",
    "dumps",
    "dumps_bytes",
    "set_duplicate_key_policy",
    "set_cycle_policy",
    "set_dumps_type_order",
]

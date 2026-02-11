"""
JSON cursor for navigating parsed documents.

A cursor points at a value inside a JSON document. Use it to traverse objects
and arrays and read scalar values.
"""

from __future__ import annotations

from . import _strata as _native

JsonCursor = _native.JsonCursor
JsonDocument = _native.JsonDocument


def parse_json(text: str | bytes):
    """
    Parse JSON text and return a cursor at the root.

    Args:
        text: JSON string or bytes.

    Returns:
        JsonCursor positioned at the root value.

    Raises:
        ValueError: If the JSON is invalid.
    """
    return _native.parse_json(text)


__all__ = ["JsonCursor", "JsonDocument", "parse_json"]

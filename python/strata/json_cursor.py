"""
JSON cursor for navigating parsed documents.

A cursor points at a value inside a JSON document. Use it to traverse objects
and arrays and read scalar values.
"""

from __future__ import annotations

from . import _strata as _native


class JsonCursor:
    """
    A handle to a value inside a parsed JSON document.

    All navigation and access is implemented in C++; this class is a thin
    Python wrapper that keeps the underlying document alive.
    """

    def __init__(
        self,
        cursor: _native.JsonCursor,
        document: _native.JsonDocument | None = None,
    ) -> None:
        self._cursor = cursor
        self._document = document  # Keeps document alive; prevents cursor invalidation

    # -------------------------------------------------------------------------
    # Type checks
    # -------------------------------------------------------------------------

    def is_null(self) -> bool:
        """Return True if the current value is null."""
        return self._cursor.is_null()

    def is_bool(self) -> bool:
        """Return True if the current value is a boolean."""
        return self._cursor.is_bool()

    def is_number(self) -> bool:
        """Return True if the current value is a number."""
        return self._cursor.is_number()

    def is_string(self) -> bool:
        """Return True if the current value is a string."""
        return self._cursor.is_string()

    def is_array(self) -> bool:
        """Return True if the current value is an array."""
        return self._cursor.is_array()

    def is_object(self) -> bool:
        """Return True if the current value is an object."""
        return self._cursor.is_object()

    # -------------------------------------------------------------------------
    # Scalar accessors (call only when type matches)
    # -------------------------------------------------------------------------

    def get_bool(self) -> bool:
        """Return the current value as a boolean."""
        return bool(self._cursor.get_bool())

    def get_int(self) -> int:
        """Return the current value as an integer."""
        return int(self._cursor.get_int())

    def get_float(self) -> float:
        """Return the current value as a float."""
        return float(self._cursor.get_float())

    def get_str(self) -> str:
        """Return the current value as a string."""
        return str(self._cursor.get_str())

    # -------------------------------------------------------------------------
    # Navigation
    # -------------------------------------------------------------------------

    def field(self, key: str) -> JsonCursor:
        """Return a cursor to the object field with the given key."""
        return JsonCursor(self._cursor.field(key), self._document)

    def at(self, index: int) -> JsonCursor:
        """Return a cursor to the array element at the given index."""
        return JsonCursor(self._cursor.at(index), self._document)


def parse_json(text: str | bytes) -> JsonCursor:
    """
    Parse JSON text and return a cursor at the root.

    Args:
        text: JSON string or bytes.

    Returns:
        JsonCursor positioned at the root value.

    Raises:
        ValueError: If the JSON is invalid.
    """
    document = _native.JsonDocument.from_string(text)
    return JsonCursor(document.root(), document)


__all__ = ["JsonCursor", "parse_json"]

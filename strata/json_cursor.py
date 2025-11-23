from __future__ import annotations

from . import _strata as _c


class JsonCursor:
    """
    Thin Python facade over the C++ JsonCursor.

    All logic lives in C++; this class just forwards calls.
    """

    def __init__(self, inner: _c.JsonCursor) -> None:
        self._inner = inner

    # Type predicates

    def is_null(self) -> bool:
        return self._inner.is_null()

    def is_bool(self) -> bool:
        return self._inner.is_bool()

    def is_number(self) -> bool:
        return self._inner.is_number()

    def is_string(self) -> bool:
        return self._inner.is_string()

    def is_array(self) -> bool:
        return self._inner.is_array()

    def is_object(self) -> bool:
        return self._inner.is_object()

    # Scalar accessors

    def get_bool(self) -> bool:
        return bool(self._inner.get_bool())

    def get_int(self) -> int:
        return int(self._inner.get_int())

    def get_float(self) -> float:
        return float(self._inner.get_float())

    def get_str(self) -> str:
        return str(self._inner.get_str())

    # Navigation

    def field(self, key: str) -> JsonCursor:
        return JsonCursor(self._inner.field(key))

    def at(self, index: int) -> JsonCursor:
        return JsonCursor(self._inner.at(index))


def parse_json(text: str) -> JsonCursor:
    """
    Parse JSON text into a C++ JsonDocument and return a cursor at the root.
    """
    doc = _c.JsonDocument.from_string(text)
    return JsonCursor(doc.root())


__all__ = [
    "parse_json",
    "JsonCursor",
]

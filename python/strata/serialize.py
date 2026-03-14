"""
JSON serialization and parsing.

Thin Python wrappers around the C++ engine.
"""

from __future__ import annotations

from . import _strata as _native
from .json_cursor import JsonCursor


def loads(source: str | bytes, *, return_type: str = "dict", iterator: bool = False):
    """Parse JSON text into a Python object, cursor, or iterator."""
    result = _native.loads(source, return_type=return_type, iterator=iterator)
    if return_type == "cursor" and not iterator:
        return JsonCursor(result[1], result[0])
    return result


def dumps(obj, *, return_type: str = "str") -> str | bytes:
    """Serialize a Python object to JSON string or bytes."""
    return _native.dumps(obj, return_type=return_type)

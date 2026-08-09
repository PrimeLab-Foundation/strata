"""Parsing and serialization.

Thin wrappers around the C++ engine: they normalize arguments and delegate.
No parsing, formatting or type dispatch happens in Python
(docs/context/convention.md, "C++ owns CPU work").
"""

from __future__ import annotations

from . import _strata as _native


def loads(source: str | bytes, *, return_type: str = "dict", iterator: bool = False):
    """Parse JSON text into Python objects.

    Args:
        source: JSON text. A ``str`` is already valid Unicode; for ``bytes``
            the parser is the only validator, so invalid UTF-8 is rejected.
        return_type: ``"dict"`` returns the full object tree. ``"cursor"``
            returns a lazy cursor and is not implemented yet.
        iterator: Consume the root lazily. Not implemented yet.

    Returns:
        The parsed value: ``dict``, ``list``, ``str``, ``int``, ``float``,
        ``bool`` or ``None``. Integers are exact at any size.

    Raises:
        ValueError: The text is not valid JSON, or ``return_type`` is unknown.
        TypeError: ``source`` is neither ``str`` nor ``bytes``.
        NotImplementedError: For ``return_type="cursor"`` or ``iterator=True``.
        RuntimeError: An internal engine error.
        RuntimeWarning: Emitted, not raised, for a duplicate key while
            ``duplicate_key_policy`` is ``"warn"``.
    """
    return _native.loads(source, return_type=return_type, iterator=iterator)


def dumps(obj, *, return_type: str = "str") -> str | bytes:
    """Serialize a Python object to compact JSON.

    Args:
        obj: A ``dict``, ``list``, ``tuple``, ``str``, ``int``, ``float``,
            ``bool`` or ``None``, nested arbitrarily. Dict keys must be ``str``.
            NaN and infinity are written as ``null``; integers beyond 64 bits
            keep every digit.
        return_type: ``"str"`` or ``"bytes"``.

    Returns:
        The JSON text, with no whitespace between tokens.

    Raises:
        TypeError: An object of an unsupported type, or a non-``str`` dict key.
        ValueError: Nesting reached ``sys.getrecursionlimit()``, ``return_type``
            is unknown, or a reference cycle was found while ``cycle_policy``
            is ``"error"``.
        RuntimeWarning: Emitted, not raised, for a reference cycle while
            ``cycle_policy`` is ``"warn"``.
    """
    return _native.dumps(obj, return_type=return_type)

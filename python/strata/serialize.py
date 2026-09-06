"""Parsing and serialization.

Thin wrappers around the C++ engine: they normalize arguments and delegate.
No parsing, formatting or type dispatch happens in Python
(docs/context/convention.md, "C++ owns CPU work").
"""

from __future__ import annotations

import os

from . import _strata as _native


def loads(source: str | bytes, *, return_type: str = "dict", iterator: bool = False):
    """Parse JSON text into Python objects.

    Args:
        source: JSON text. A ``str`` is already valid Unicode; for ``bytes``
            the parser is the only validator, so invalid UTF-8 is rejected.
        return_type: ``"dict"`` returns the full object tree; ``"cursor"``
            returns a lazy :class:`JsonCursor`.
        iterator: Consume the root lazily. A dict root yields ``(key, value)``
            pairs, a list root yields elements, and a scalar root is returned
            unchanged.

    Returns:
        The parsed value: ``dict``, ``list``, ``str``, ``int``, ``float``,
        ``bool`` or ``None``. Integers are exact at any size.

    Raises:
        ValueError: The text is not valid JSON, nesting exceeds 1024 open
            containers, or ``return_type`` is unknown.
        TypeError: ``source`` is neither ``str`` nor ``bytes``.
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


def load(
    path: str | os.PathLike,
    *,
    return_type: str = "dict",
    iterator: bool = False,
    skip_errors: bool = False,
):
    """Read JSON or NDJSON from a file.

    Dispatch is by extension: ``.ndjson`` and ``.jsonl`` are line-delimited
    records, anything else is a single document.

    Args:
        path: File to read. ``Path`` is accepted and normalized here.
        return_type: ``"dict"`` for Python objects, ``"cursor"`` for a lazy
            :class:`JsonCursor`. Cursor mode is not available for NDJSON.
        iterator: Consume lazily. For NDJSON each line is parsed as it is
            reached, so a malformed line raises at that line.
        skip_errors: Drop malformed NDJSON lines instead of raising.

    Returns:
        The document, a list of records, a cursor, or an iterator.

    Raises:
        FileNotFoundError: No such file.
        OSError: The file could not be read.
        ValueError: Invalid JSON, nesting past 1024 open containers (per
            document and per NDJSON line), an empty ``.json`` file, an unknown
            ``return_type``, or cursor mode on NDJSON.
    """
    return _native.load(
        os.fspath(path),
        return_type=return_type,
        iterator=iterator,
        skip_errors=skip_errors,
    )


def dump(obj, path: str | os.PathLike, *, split_by=None) -> None:
    """Write ``obj`` to a file as compact JSON with a trailing newline.

    Args:
        obj: The value to serialize; the same types :func:`dumps` accepts.
        path: Destination file, truncated if it exists. ``Path`` is accepted.
        split_by: For a directory target, the key or keys whose values group
            the records into files. One key writes ``dir/<value>.json``; N keys
            nest one directory per key. Required for a directory, and an error
            for a file.

    Raises:
        OSError: The file or directory could not be written.
        TypeError: An unsupported type, a non-``str`` dict key, or -- in folder
            mode -- a non-list ``obj`` or a record that is not a dict.
        ValueError: ``split_by`` given for a file or missing for a directory, a
            record missing a split key, a split value that is not a
            ``str``/``int``/``bool``, or one that is unusable or ambiguous as a
            file name.
    """
    _native.dump(obj, os.fspath(path), split_by=split_by)

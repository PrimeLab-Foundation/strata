"""Parsing and serialization.

Thin wrappers around the C++ engine: they normalize arguments and delegate.
No parsing, formatting or type dispatch happens in Python
(docs/context/convention.md, "C++ owns CPU work").
"""

from __future__ import annotations

import functools
import os

from . import _strata as _native


def loads(
    source: str | bytes,
    *,
    return_type: str = "dict",
    iterator: bool = False,
    parse_types=False,
):
    """Parse JSON text into Python objects.

    Args:
        source: JSON text. A ``str`` is already valid Unicode; for ``bytes``
            the parser is the only validator, so invalid UTF-8 is rejected.
        return_type: ``"dict"`` returns the full object tree; ``"cursor"``
            returns a lazy :class:`JsonCursor`.
        iterator: Consume the root lazily. A dict root yields ``(key, value)``
            pairs, a list root yields elements, and a scalar root is returned
            unchanged.
        parse_types: ``False`` (default) returns strings as parsed. ``True``
            replaces every string *value* that is exactly an RFC 3339 date,
            time or date-time, or a canonical UUID, by a ``date``, ``time``,
            ``datetime`` or ``uuid.UUID``. A ``dict`` mapping member names to
            ``Enum`` subclasses or dataclass types implies ``True`` and revives
            the values of those members as the registered type, best effort.

    Returns:
        The parsed value: ``dict``, ``list``, ``str``, ``int``, ``float``,
        ``bool`` or ``None``, plus the types ``parse_types`` revives. Integers
        are exact at any size.

    Raises:
        ValueError: The text is not valid JSON, nesting exceeds 1024 open
            containers, ``return_type`` is unknown, or ``parse_types`` is set
            with ``return_type="cursor"``.
        TypeError: ``source`` is neither ``str`` nor ``bytes``, or
            ``parse_types`` is not a bool or a valid registry.
        RuntimeError: An internal engine error.
        RuntimeWarning: Emitted, not raised, for a duplicate key while
            ``duplicate_key_policy`` is ``"warn"``.
    """
    if parse_types is False:
        return _native.loads(source, return_type=return_type, iterator=iterator)
    return _hook_module().loads_typed(
        source,
        return_type=return_type,
        iterator=iterator,
        parse_types=parse_types,
    )


def dumps(obj, *, return_type: str = "str", native: bool = False) -> str | bytes:
    """Serialize a Python object to compact JSON.

    Args:
        obj: A ``dict``, ``list``, ``tuple``, ``str``, ``int``, ``float``,
            ``bool`` or ``None``, nested arbitrarily. Dict keys must be ``str``.
            NaN and infinity are written as ``null``; integers beyond 64 bits
            keep every digit.
        return_type: ``"str"`` or ``"bytes"``.
        native: ``False`` (default) serializes only the types listed above.
            ``True`` additionally writes ``datetime``, ``date``, ``time``,
            ``UUID``, ``Decimal``, ``Enum``, dataclasses, ``set``/``frozenset``
            and numpy scalars/arrays natively.

    Returns:
        The JSON text, with no whitespace between tokens.

    Raises:
        TypeError: An object of an unsupported type, a non-``str`` dict key,
            or ``native`` is not a ``bool``.
        ValueError: Nesting reached ``sys.getrecursionlimit()``, ``return_type``
            is unknown, or a reference cycle was found while ``cycle_policy``
            is ``"error"``.
        RuntimeWarning: Emitted, not raised, for a reference cycle while
            ``cycle_policy`` is ``"warn"``.
    """
    if native is False:
        return _native.dumps(obj, return_type=return_type)
    if native is True:
        return _hook_module().dumps_native(obj, return_type=return_type)
    raise TypeError(f"native must be a bool, not {type(native).__name__}")


def dumps_with_default(
    obj,
    default,
    *,
    return_type: str = "str",
    native: bool = True,
) -> str | bytes:
    """Serialize a Python object to compact JSON, with a hook for unsupported types.

    The same output as :func:`dumps` for every object :func:`dumps` supports;
    each object of any other type is passed to ``default``, once, and its
    return value is serialized in the object's place.

    Args:
        obj: The value to serialize.
        default: A callable of one argument (``None`` is refused). It is never
            called for dict keys, nor a second time on its own return value;
            objects nested inside a returned container get their own call.
        return_type: ``"str"`` or ``"bytes"``.
        native: ``True`` (default) writes ``datetime``, ``date``, ``time``,
            ``UUID``, ``Decimal``, ``Enum``, dataclasses, ``set``/``frozenset``
            and numpy scalars/arrays natively, as ``dumps(obj, native=True)``
            does, before ``default`` is consulted. ``False`` supports only what
            ``dumps(obj)`` supports: every native object is passed to
            ``default`` instead, and a native object ``default`` returns is
            unsupported.

    Returns:
        The JSON text, with no whitespace between tokens.

    Raises:
        TypeError: ``default`` is not callable, it returned an unsupported
            object, a dict key is not a ``str``, or ``native`` is not a
            ``bool``.
        ValueError: Nesting reached ``sys.getrecursionlimit()``, ``return_type``
            is unknown, or a reference cycle was found while ``cycle_policy``
            is ``"error"``.
        RuntimeWarning: Emitted, not raised, for a reference cycle while
            ``cycle_policy`` is ``"warn"``.

    Any exception ``default`` raises propagates unchanged. The first call
    imports the hook's own extension (``strata._dumps_hook``); if that import
    fails, the ``ImportError`` is raised here, and every other function of the
    package is unaffected.
    """
    return _hook_module().dumps_with_default(
        obj,
        default,
        return_type=return_type,
        native=native,
    )


@functools.cache
def _hook_module():
    """``strata._dumps_hook``, imported on first use.

    The hook lives in a second extension image so that ``_strata`` does not
    change (docs/architecture/dumps_with_default.md, docs/architecture/native_types.md);
    importing it lazily keeps a missing or broken hook image from taking
    ``import strata`` down with it. A failed import is not cached, so the next
    call retries it. Shared by :mod:`strata.jsonpath` for ``search``/``query``
    with ``parse_types`` set.
    """
    from . import _dumps_hook  # noqa: PLC0415 -- deliberate: first use only

    return _dumps_hook


def load(
    path: str | os.PathLike,
    *,
    return_type: str = "dict",
    iterator: bool = False,
    skip_errors: bool = False,
    parse_types=False,
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
        skip_errors: Drop malformed NDJSON lines instead of raising. It covers
            invalid JSON only: an exception from a registered type propagates.
        parse_types: ``False`` (default) returns strings as parsed. ``True``
            replaces every string *value* that is exactly an RFC 3339 date,
            time or date-time, or a canonical UUID, by a ``date``, ``time``,
            ``datetime`` or ``uuid.UUID``, in every
            document, NDJSON line or folder record. A ``dict`` mapping member names to
            ``Enum`` subclasses or dataclass types implies ``True`` and revives
            the values of those members as the registered type, best effort.

    Returns:
        The document, a list of records, a cursor, or an iterator.

    Raises:
        FileNotFoundError: No such file.
        OSError: The file could not be read.
        ValueError: Invalid JSON, nesting past 1024 open containers (per
            document and per NDJSON line), an empty ``.json`` file, an unknown
            ``return_type``, cursor mode on NDJSON, or ``parse_types`` with
            cursor mode.
        TypeError: ``parse_types`` is not a bool or a valid registry.
    """
    path = os.fspath(path)
    if parse_types is False:
        return _native.load(
            path,
            return_type=return_type,
            iterator=iterator,
            skip_errors=skip_errors,
        )
    return _hook_module().load_typed(
        path,
        return_type=return_type,
        iterator=iterator,
        skip_errors=skip_errors,
        parse_types=parse_types,
    )


def dump(obj, path: str | os.PathLike, *, split_by=None, native: bool = False) -> None:
    """Write ``obj`` to a file as compact JSON with a trailing newline.

    Args:
        obj: The value to serialize; the same types :func:`dumps` accepts.
        path: Destination file, truncated if it exists. ``Path`` is accepted.
        split_by: For a directory target, the key or keys whose values group
            the records into files. One key writes ``dir/<value>.json``; N keys
            nest one directory per key. Required for a directory, and an error
            for a file.
        native: ``False`` (default) serializes only the types :func:`dumps`
            accepts without it. ``True`` additionally writes ``datetime``,
            ``date``, ``time``, ``UUID``, ``Decimal``, ``Enum``, dataclasses,
            ``set``/``frozenset`` and numpy scalars/arrays natively, in file
            and folder mode alike.

    Raises:
        OSError: The file or directory could not be written.
        TypeError: An unsupported type, a non-``str`` dict key, ``native`` is
            not a ``bool``, or -- in folder mode -- a non-list ``obj`` or a
            record that is not a dict.
        ValueError: ``split_by`` given for a file or missing for a directory, a
            record missing a split key, a split value that is not a
            ``str``/``int``/``bool``, or one that is unusable or ambiguous as a
            file name.
    """
    if native is False:
        _native.dump(obj, os.fspath(path), split_by=split_by)
        return
    if native is True:
        _hook_module().dump_native(obj, os.fspath(path), split_by=split_by)
        return
    raise TypeError(f"native must be a bool, not {type(native).__name__}")

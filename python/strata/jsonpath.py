"""JSONPath querying.

Thin wrappers around the C++ engine: they normalize arguments and delegate.
The grammar and the error contract live in docs/context/api.md § JSONPath.
"""

from __future__ import annotations

import os

from . import _strata as _native

VALID_SUFFIXES = (".json", ".ndjson", ".jsonl")


def compile(expression: str):  # noqa: A001 - the documented public name
    """Compile ``expression`` once, to reuse across many queries.

    Raises:
        ValueError: The expression is not valid JSONPath.
        TypeError: ``expression`` is not a string.
    """
    return _native.compile(expression)


def query(data, expression, *, iterator: bool = False):
    """Evaluate ``expression`` against in-memory Python objects.

    Args:
        data: A ``dict``, ``list`` or ``tuple`` to search.
        expression: JSONPath text, or the result of :func:`compile`.
        iterator: Return an iterator over the matches instead of a list. The
            evaluation is eager either way; only consumption is lazy.

    Returns:
        Every matching value, in document order.

    Raises:
        TypeError: ``data`` is not a dict, list or tuple.
        ValueError: The expression is not valid JSONPath.
    """
    return _native.query(data, expression, iterator=iterator)


def search(path: str | os.PathLike, expression, *, iterator: bool = False):
    """Evaluate ``expression`` against a file.

    The file must be ``.json``, ``.ndjson`` or ``.jsonl``. An NDJSON file is
    searched as the list of its records, so
    ``search(f, e) == query(load(f), e)`` holds for every supported expression.

    Args:
        path: File to search. ``Path`` is accepted and normalized here.
        expression: JSONPath text, or the result of :func:`compile`.
        iterator: Return an iterator over the matches instead of a list.

    Raises:
        TypeError: ``path`` does not end in a supported extension.
        FileNotFoundError: No such file.
        ValueError: Invalid JSON, or an invalid expression.
    """
    text = os.fspath(path)
    if not str(text).lower().endswith(VALID_SUFFIXES):
        raise TypeError(f"search() expects a .json, .ndjson or .jsonl path, got {text!r}")
    return _native.search(text, expression, iterator=iterator)

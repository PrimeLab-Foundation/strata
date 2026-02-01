"""
JSONPath query support.

Compile and evaluate JSONPath expressions against JSON data.
"""

from __future__ import annotations

from . import _strata as _native
from .json_cursor import JsonCursor

# Type alias for JSONPath expressions (used in search() signature)
CompiledPath = _native.CompiledPath


def compile_path(expression: str) -> CompiledPath:
    """
    Compile a JSONPath expression for repeated use.

    Use this when you will run the same query many times; compilation is
    done once and the result can be passed to search().

    Args:
        expression: JSONPath string (e.g. "$.users[*].id").

    Returns:
        Compiled path that can be passed to search().

    Raises:
        ValueError: If the expression is invalid.

    Example:
        >>> path = compile_path("$.users[*].name")
        >>> search(doc1, path)
        ['Alice', 'Bob']
        >>> search(doc2, path)
        ['Carol']
    """
    return _native.compile_path(expression)


def search(
    data: str | bytes | JsonCursor | dict | list,
    expression: str | CompiledPath,
) -> list:
    """
    Find all values matching a JSONPath expression.

    Args:
        data: JSON as string, bytes, JsonCursor, or already-parsed dict/list.
        expression: JSONPath string or a pre-compiled CompiledPath.

    Returns:
        List of matching values (primitives, dicts, or lists).

    Example:
        >>> search('{"users": [{"id": 1}, {"id": 2}]}', "$.users[*].id")
        [1, 2]
    """
    # JsonCursor wraps the C++ cursor; unwrap for the native search API
    if isinstance(data, JsonCursor):
        data = data._cursor

    return _native.search(data, expression)


__all__ = ["compile_path", "search"]

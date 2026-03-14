"""
JSONPath query support.

Compile and evaluate JSONPath expressions against JSON data.
"""

from __future__ import annotations

from pathlib import Path

from . import _strata as _native
from .json_cursor import JsonCursor

# Type alias for JSONPath expressions (used in search() signature)
CompiledPath = _native.CompiledPath


def compile_path(expression: str) -> CompiledPath:
    """Compile a JSONPath expression for repeated use."""
    return _native.compile_path(expression)


def search(
    data: str | bytes | Path | JsonCursor | dict | list,
    expression: str | CompiledPath,
    *,
    mem_eff: bool | None = None,
) -> list:
    """
    Find all values matching a JSONPath expression.

    Args:
        data: JSON string, bytes, file path (.json/.ndjson/.jsonl),
              JsonCursor, or already-parsed dict/list.
        expression: JSONPath string or a pre-compiled CompiledPath.
        mem_eff: If True, search on C++ tree without full Python conversion.
                 If None, uses global config (strata.config.get("mem_eff")).
    """
    # Unwrap JsonCursor and Path for the native API
    if isinstance(data, JsonCursor):
        data = data._cursor
    elif isinstance(data, Path):
        data = str(data)

    kwargs = {}
    if mem_eff is not None:
        kwargs["mem_eff"] = mem_eff

    return _native.search(data, expression, **kwargs)


__all__ = ["compile_path", "search"]

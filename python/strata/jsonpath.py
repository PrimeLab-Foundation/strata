"""
JSONPath query support.

Compile and evaluate JSONPath expressions against JSON data.
"""

from __future__ import annotations

from pathlib import Path

from . import _strata as _native

CompiledPath = _native.CompiledPath


def compile_path(expression: str) -> CompiledPath:
    """Compile a JSONPath expression for repeated use."""
    return _native.compile_path(expression)


def search(
    filepath: str | Path,
    expression: str | CompiledPath,
    *,
    mem_eff: bool | None = None,
    iterator: bool = False,
):
    """Search a JSON/NDJSON/JSONL file using JSONPath."""
    if isinstance(filepath, Path):
        filepath = str(filepath)
    kwargs = {"iterator": iterator}
    if mem_eff is not None:
        kwargs["mem_eff"] = mem_eff
    return _native.search(filepath, expression, **kwargs)


def query(data: dict | list, expression: str | CompiledPath, *, iterator: bool = False):
    """Query a dict/list using JSONPath."""
    return _native.query(data, expression, iterator=iterator)

"""Strata — fast JSON for Python."""
from __future__ import annotations
from . import _strata as _native

__version__ = "0.2.0"
CompiledPath = _native.CompiledPath


def compile_path(expression):
    """Compile a search expression for repeated use."""
    return _native.compile_path(expression)


def search(source, expression, **kwargs):
    """Search JSON/NDJSON data for values matching a JSONPath expression."""
    return _native.search(source, expression, **kwargs)


def query(data, expression, **kwargs):
    """Search a Python dict/list using JSONPath."""
    return _native.query(data, expression, **kwargs)


def load(source, **kwargs):
    """Load JSON or NDJSON from a file path or file-like object."""
    return _native.load(source, **kwargs)


def dump(target, obj, **kwargs):
    """Write JSON or NDJSON to a file path or file-like object."""
    return _native.dump(target, obj, **kwargs)


def loads(source):
    """Parse JSON text into a Python object."""
    return _native.loads(source)


def loads_parallel_json_experiment(source, *, num_threads=0, min_chunk_size=0):
    """Experimental: parallel JSON parse using structural tape chunking."""
    return _native.loads_parallel_json_experiment(
        source, num_threads=num_threads, min_chunk_size=min_chunk_size
    )


def dumps(obj, **kwargs):
    """Serialize a Python object to JSON text."""
    return _native.dumps(obj, **kwargs)

__all__ = [
    "compile_path",
    "search",
    "query",
    "load",
    "dump",
    "loads",
    "loads_parallel_json_experiment",
    "dumps",
    "__version__",
]

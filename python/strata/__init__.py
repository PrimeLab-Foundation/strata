"""
Strata — fast JSON for Python.

High-performance parsing, serialization, and querying powered by C++.
"""

from . import _strata as _native
from . import config
from .json_cursor import JsonCursor, parse_json
from .jsonpath import compile_path, query, search
from .serialize import dumps, loads


def load(filepath, *, return_type: str = "dict", iterator: bool = False):
    """Load JSON/NDJSON/JSONL file."""
    filepath = str(filepath)
    result = _native.load(filepath, return_type=return_type, iterator=iterator)
    if return_type == "cursor" and not iterator:
        return JsonCursor(result[1], result[0])
    return result


dump = _native.dump

__version__ = "0.2.0"

__all__ = [
    # Parse / serialize
    "loads",
    "dumps",
    # File I/O
    "load",
    "dump",
    # JSONPath
    "search",
    "query",
    "compile_path",
    # Cursor
    "parse_json",
    # Config
    "config",
    # Version
    "__version__",
]

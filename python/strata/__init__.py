"""
Strata — fast JSON for Python.

High-performance parsing, serialization, and querying powered by C++.
"""

from . import mmap_io
from .json_cursor import JsonCursor, parse_json
from .jsonpath import compile_path, search
from .mmap_io import parse_json_file
from .ndjson import iter_ndjson, parse_ndjson
from .serialize import dumps, dumps_bytes, loads, loads_tape, set_cycle_policy, set_duplicate_key_policy

# Import lazy cursor from the C extension
from ._strata import LazyCursor, lazy

__version__ = "0.2.0"

__all__ = [
    # Parse / serialize
    "loads",
    "loads_tape",
    "dumps",
    "dumps_bytes",
    # NDJSON
    "iter_ndjson",
    "parse_ndjson",
    # JSONPath
    "search",
    "compile_path",
    # File I/O
    "parse_json_file",
    "mmap_io",
    # Cursor API (advanced)
    "JsonCursor",
    "parse_json",
    # Lazy cursor (selective materialization)
    "LazyCursor",
    "lazy",
    # Config
    "set_duplicate_key_policy",
    "set_cycle_policy",
    # Version
    "__version__",
]

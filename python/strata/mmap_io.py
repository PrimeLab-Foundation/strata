"""
Memory-mapped JSON file parsing.

Load large JSON files efficiently using the OS page cache.
"""

from __future__ import annotations

from pathlib import Path

from . import _strata as _native
from .json_cursor import JsonCursor


def parse_json_file(filepath: str | Path) -> JsonCursor:
    """
    Parse a JSON file using memory-mapped I/O.

    The OS maps file pages into memory on demand. Good for large files:
    fast startup, low memory use, and efficient random access.

    Args:
        filepath: Path to the JSON file (str or Path).

    Returns:
        JsonCursor at the root of the parsed document.

    Raises:
        ValueError: If the file cannot be read or the JSON is invalid.

    Example:
        >>> doc = parse_json_file("data.json")
        >>> doc.field("users").at(0).field("name").get_str()
        'Alice'
    """
    path_str = str(filepath) if isinstance(filepath, Path) else filepath
    document, cursor = _native.parse_json_file(path_str)
    return JsonCursor(cursor, document)


__all__ = ["parse_json_file"]

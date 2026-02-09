"""
Strata — fast JSON for Python.

High-performance parsing, serialization, and querying powered by C++.
"""

from __future__ import annotations

from collections.abc import Iterable
from functools import lru_cache
import io
import os
from pathlib import Path
from typing import Any

from . import _strata as _native
from . import ndjson as _ndjson

__version__ = "0.2.0"

_PATH_CACHE_SIZE = 256
_NDJSON_KWARGS = {"skip_errors", "parallel", "num_threads"}


@lru_cache(maxsize=_PATH_CACHE_SIZE)
def _compile_path_cached(expression: str) -> _native.CompiledPath:
    return _native.compile_path(expression)


def _compiled_path(expression: str | _native.CompiledPath) -> _native.CompiledPath:
    if isinstance(expression, _native.CompiledPath):
        return expression
    if not isinstance(expression, str):
        raise TypeError("expression must be a JSONPath string")
    return _compile_path_cached(expression)


def _is_pathlike(value: Any) -> bool:
    return isinstance(value, (str, os.PathLike))


def _detect_ndjson(source: Any, ndjson: bool | None) -> bool:
    if ndjson is not None:
        return bool(ndjson)
    name = None
    if _is_pathlike(source):
        name = str(source)
    else:
        name = getattr(source, "name", None)
    return bool(name) and str(name).endswith(".ndjson")


def _read_source(source: Any) -> str | bytes:
    data = source.read()
    if isinstance(data, bytearray):
        return bytes(data)
    if isinstance(data, memoryview):
        return data.tobytes()
    if isinstance(data, (str, bytes)):
        return data
    raise TypeError("file-like object must return str or bytes from read()")


def _pop_ndjson_kwargs(kwargs: dict[str, Any]) -> dict[str, Any]:
    ndjson_kwargs = {k: kwargs.pop(k) for k in list(kwargs) if k in _NDJSON_KWARGS}
    return ndjson_kwargs


def _ensure_no_extra_kwargs(kwargs: dict[str, Any]) -> None:
    if kwargs:
        keys = ", ".join(sorted(kwargs))
        raise TypeError(f"unsupported keyword argument(s): {keys}")


def _iter_ndjson_records(obj: Any) -> Iterable[Any]:
    if isinstance(obj, dict):
        return (obj,)
    if isinstance(obj, (str, bytes, bytearray)):
        raise TypeError("ndjson dump expects JSON-serializable objects, not raw text")
    if isinstance(obj, Iterable):
        return obj
    return (obj,)


def _write_bytes_or_text(handle: Any, data: str | bytes | bytearray) -> None:
    handle.write(data)


def search(
    source: str | os.PathLike[str] | Any,
    expression: str | _native.CompiledPath,
    **kwargs: Any,
) -> list:
    """
    Search a JSON/NDJSON file for values matching a JSONPath expression.

    Args:
        source: File path or file-like object.
        expression: JSONPath string.
        ndjson: Optional bool to force NDJSON mode.
        skip_errors: Skip malformed NDJSON lines when enabled.
        on_error: NDJSON error handling: "skip", "warn", or "error" (default).
        parallel/num_threads: Accepted for compatibility; NDJSON search streams line-by-line.
    """
    ndjson = kwargs.pop("ndjson", None)
    on_error = kwargs.pop("on_error", None)
    ndjson_kwargs = _pop_ndjson_kwargs(kwargs)
    _ensure_no_extra_kwargs(kwargs)

    if _detect_ndjson(source, ndjson):
        compiled = _compiled_path(expression)
        return _native.search_ndjson(
            source,
            compiled,
            skip_errors=ndjson_kwargs.get("skip_errors", False),
            on_error=on_error,
        )
    if on_error is not None:
        raise TypeError("on_error is only supported for NDJSON search")

    compiled = _compiled_path(expression)
    if _is_pathlike(source):
        _document, cursor = _native.parse_json_file(str(source))
        return _native.search(cursor, compiled)

    data = _read_source(source)
    return _native.search(data, compiled)


def query(
    data: dict | list,
    expression: str | _native.CompiledPath,
    **kwargs: Any,
) -> list:
    """
    Search a Python dict/list for values matching a JSONPath expression.
    """
    _ensure_no_extra_kwargs(kwargs)
    if not isinstance(data, (dict, list)):
        raise TypeError("query() expects a dict or list")
    compiled = _compiled_path(expression)
    payload = _native.dumps_bytes(data)
    return _native.search(payload, compiled)


def load(
    source: str | os.PathLike[str] | Any,
    **kwargs: Any,
) -> dict | list | str | int | float | bool | None:
    """
    Load JSON or NDJSON from a file path or file-like object.
    """
    ndjson = kwargs.pop("ndjson", None)
    ndjson_kwargs = _pop_ndjson_kwargs(kwargs)
    _ensure_no_extra_kwargs(kwargs)

    data = Path(source).read_bytes() if _is_pathlike(source) else _read_source(source)
    if _detect_ndjson(source, ndjson):
        return _ndjson.parse_ndjson(data, **ndjson_kwargs)
    return _native.loads(data)


def dump(
    target: str | os.PathLike[str] | Any,
    obj: Any,
    **kwargs: Any,
) -> int:
    """
    Write JSON or NDJSON to a file path or file-like object.

    Returns:
        Line count written (1 for JSON, N for NDJSON records).
    """
    ndjson = kwargs.pop("ndjson", None)
    return_type = kwargs.pop("return_type", None)
    _ensure_no_extra_kwargs(kwargs)

    use_ndjson = _detect_ndjson(target, ndjson)
    use_path = _is_pathlike(target)

    if return_type is None:
        if use_path:
            return_type = "bytes"
        else:
            return_type = "str" if isinstance(target, io.TextIOBase) else "bytes"

    if isinstance(return_type, type):
        if return_type is str:
            return_type = "str"
        elif return_type is bytes:
            return_type = "bytes"
        elif return_type is bytearray:
            return_type = "bytearray"

    if return_type not in {"str", "bytes", "bytearray"}:
        raise TypeError("return_type must be 'str', 'bytes', or 'bytearray'")

    def _to_payload(value: Any) -> str | bytes | bytearray:
        if return_type == "str":
            return _native.dumps(value)
        payload = _native.dumps_bytes(value)
        return payload if return_type == "bytes" else bytearray(payload)

    def _open_path(path: str) -> Any:
        return open(path, "w" if return_type == "str" else "wb")

    if use_ndjson:
        line_count = 0
        if use_path:
            with _open_path(str(target)) as handle:
                for record in _iter_ndjson_records(obj):
                    _write_bytes_or_text(handle, _to_payload(record))
                    _write_bytes_or_text(handle, "\n" if return_type == "str" else b"\n")
                    line_count += 1
        else:
            if isinstance(target, io.TextIOBase) and return_type != "str":
                raise TypeError("file-like object expects str; use return_type='str'")
            for record in _iter_ndjson_records(obj):
                _write_bytes_or_text(target, _to_payload(record))
                _write_bytes_or_text(target, "\n" if return_type == "str" else b"\n")
                line_count += 1
        return line_count

    if use_path:
        with _open_path(str(target)) as handle:
            _write_bytes_or_text(handle, _to_payload(obj))
    else:
        if isinstance(target, io.TextIOBase) and return_type != "str":
            raise TypeError("file-like object expects str; use return_type='str'")
        _write_bytes_or_text(target, _to_payload(obj))
    return 1


def loads(source: str | bytes | bytearray | memoryview, **kwargs: Any) -> Any:
    """
    Parse JSON text into a Python object.
    """
    _ensure_no_extra_kwargs(kwargs)
    if isinstance(source, bytearray):
        return _native.loads(bytes(source))
    if isinstance(source, memoryview):
        return _native.loads(source.tobytes())
    if isinstance(source, (str, bytes)):
        return _native.loads(source)
    raise TypeError("loads() expects str, bytes, bytearray, or memoryview")


def dumps(obj: Any, **kwargs: Any) -> str | bytes | bytearray:
    """
    Serialize a Python object to JSON text.
    """
    return_type = kwargs.pop("return_type", "str")
    _ensure_no_extra_kwargs(kwargs)

    if isinstance(return_type, type):
        if return_type is str:
            return_type = "str"
        elif return_type is bytes:
            return_type = "bytes"
        elif return_type is bytearray:
            return_type = "bytearray"

    if return_type == "str":
        return _native.dumps(obj)
    if return_type == "bytes":
        return _native.dumps_bytes(obj)
    if return_type == "bytearray":
        return bytearray(_native.dumps_bytes(obj))
    raise TypeError("return_type must be 'str', 'bytes', or 'bytearray'")


__all__ = ["search", "query", "load", "dump", "loads", "dumps", "__version__"]

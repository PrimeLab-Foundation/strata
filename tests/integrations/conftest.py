"""Shared oracle for the third-party integration suites.

`tests/integrations` shows that `strata.dumps_with_default(obj, default)`
composes with third-party types (docs/architecture/dumps_with_default.md, the
reuse table; api.md, dumps_with_default). It is excluded from `testpaths`,
`make test`, `make gate` and the PGO profile;
`make test-integrations` runs it. Every test reads the same way: stdlib `json`
with the same `default` is the oracle, byte for byte against its compact form,
and strata's own parser closes the round trip.

Native types (docs/architecture/native_types.md) are written by strata itself
and never reach `default`, so the oracle's hook gives each native object its
reference spelling first and hands only the rest to `default`. A `Decimal` is
a raw JSON number, which no stdlib `default` can return: the hook writes a
marker string and the expected text has the marker replaced by the number.

The framework adapters' suites (`test_flask.py` and its siblings) take each
framework's default serializer as their oracle instead (api.md, Framework
adapters).
"""

import dataclasses
import datetime as dt
import enum
import json
import re
import sys
import uuid
from collections.abc import Callable
from decimal import Decimal
from typing import Any

import pytest

import strata

_NOT_NATIVE = object()
_DECIMAL_SPLICE = re.compile(r'"\\u0000decimal:([^"\\]*)\\u0000"')


def _offset(delta: dt.timedelta) -> str:
    seconds = delta.days * 86400 + delta.seconds
    minutes = (abs(seconds) + 30) // 60
    return f"{'-' if seconds < 0 else '+'}{minutes // 60:02d}:{minutes % 60:02d}"


def _clock(value: Any) -> str:
    text = f"{value.hour:02d}:{value.minute:02d}:{value.second:02d}"
    return f"{text}.{value.microsecond:06d}" if value.microsecond else text


def native_reference(obj: Any) -> Any:
    """The reference spelling of a native object, or `_NOT_NATIVE`."""
    if isinstance(obj, dt.datetime):
        offset = obj.tzinfo.utcoffset(obj) if obj.tzinfo is not None else None
        date = f"{obj.year:04d}-{obj.month:02d}-{obj.day:02d}"
        return f"{date}T{_clock(obj)}" + (_offset(offset) if offset is not None else "")
    if isinstance(obj, dt.date):
        return f"{obj.year:04d}-{obj.month:02d}-{obj.day:02d}"
    if isinstance(obj, dt.time):
        offset = obj.tzinfo.utcoffset(None) if obj.tzinfo is not None else None
        return _clock(obj) + (_offset(offset) if offset is not None else "")
    if isinstance(obj, uuid.UUID):
        return str(obj)
    if isinstance(obj, Decimal):
        return f"\x00decimal:{obj}\x00" if obj.is_finite() else None
    if isinstance(obj, enum.Enum):
        return obj.value
    if dataclasses.is_dataclass(obj) and not isinstance(obj, type):
        return {field.name: getattr(obj, field.name) for field in dataclasses.fields(obj)}
    if isinstance(obj, (set, frozenset)):
        return list(obj)
    numpy = sys.modules.get("numpy")
    if numpy is not None and isinstance(obj, (numpy.generic, numpy.ndarray)):
        if obj.dtype.kind in "biuf":
            plain = obj.item() if isinstance(obj, numpy.generic) else obj.tolist()
            if not isinstance(plain, (numpy.generic, numpy.ndarray)):
                return plain
    return _NOT_NATIVE


def _counted(default: Callable[[Any], Any], calls: list[int]) -> Callable[[Any], Any]:
    def hook(obj: Any) -> Any:
        native = native_reference(obj)
        if native is not _NOT_NATIVE:
            return native
        calls[0] += 1
        return default(obj)

    return hook


def _expected(obj: Any, default: Callable[[Any], Any], calls: list[int]) -> str:
    text = json.dumps(
        obj,
        default=_counted(default, calls),
        separators=(",", ":"),
        ensure_ascii=False,
    )
    return _DECIMAL_SPLICE.sub(r"\1", text)


def _composes(obj: Any, default: Callable[[Any], Any]) -> Any:
    """Assert strata and stdlib `json` agree on `obj` under `default`; return the decoded value.

    api.md, dumps_with_default: the callable is called only where `dumps`
    would otherwise raise, once per such object, so the call count matches
    stdlib's non-native calls for hooks that return JSON-native values.
    """
    json_calls, strata_calls = [0], [0]
    expected = _expected(obj, default, json_calls)
    text = strata.dumps_with_default(obj, _counted(default, strata_calls))
    assert text == expected
    assert strata_calls[0] == json_calls[0]
    as_bytes = strata.dumps_with_default(obj, default=default, return_type="bytes")
    assert as_bytes == expected.encode()
    decoded = strata.loads(text)
    assert decoded == json.loads(expected)
    return decoded


@pytest.fixture
def composes() -> Callable[[Any, Callable[[Any], Any]], Any]:
    return _composes


def _plainly_counted(default: Callable[[Any], Any], calls: list[int]) -> Callable[[Any], Any]:
    """`default`, counted, with no native reference spelling in front (cf. `_counted`)."""

    def hook(obj: Any) -> Any:
        calls[0] += 1
        return default(obj)

    return hook


def _composes_without_natives(obj: Any, default: Callable[[Any], Any]) -> Any:
    """`_composes` for `dumps_with_default(..., native=False)`: stdlib `json` itself is the oracle.

    docs/decisions.md 2026-09-29: under `native=False` every native object reaches
    `default` as it does under `json.dumps`, so text and call counts both match
    stdlib's -- the pre-M15 contract the adapters that pass a framework's own
    `default` rely on.
    """
    json_calls, strata_calls = [0], [0]
    expected = json.dumps(
        obj,
        default=_plainly_counted(default, json_calls),
        separators=(",", ":"),
        ensure_ascii=False,
    )
    text = strata.dumps_with_default(obj, _plainly_counted(default, strata_calls), native=False)
    assert text == expected
    assert strata_calls[0] == json_calls[0]
    as_bytes = strata.dumps_with_default(obj, default=default, return_type="bytes", native=False)
    assert as_bytes == expected.encode()
    decoded = strata.loads(text)
    assert decoded == json.loads(expected)
    return decoded


@pytest.fixture
def composes_without_natives() -> Callable[[Any, Callable[[Any], Any]], Any]:
    return _composes_without_natives


@pytest.fixture
def json_document() -> dict[str, Any]:
    """A JSON-native document for the framework adapters' round trips.

    Unsorted keys, non-ASCII text, an int beyond int64 and floats whose
    shortest form stdlib `json` and strata must both write.
    """
    return {
        "zeta": "last key first",
        "id": 7,
        "name": "Zoë ☃ 𝄞",
        "big": 2**70,
        "ratios": [0.1, 1e-07, 1e22, -0.0, 1.5],
        "flags": [True, False, None],
        "nested": {"b": [1, {"c": []}], "a": {}},
        "empty": "",
    }


@pytest.fixture
def deep_document() -> list:
    """3000 nested lists: past `dumps`'s limit, `sys.getrecursionlimit()` (1000 by default)."""
    document: list = []
    for _ in range(2999):
        document = [document]
    return document


@pytest.fixture
def deep_request() -> bytes:
    """1025 nested arrays: one past the parser's 1024-container cap (api.md, loads)."""
    return b"[" * 1025 + b"]" * 1025


@pytest.fixture
def stdlib_nests() -> bool:
    """Whether stdlib `json` handles both deep fixtures.

    From 3.12 its C guard is the C stack, which both pass; on 3.10 and 3.11 it
    is the interpreter's recursion limit, and both raise `RecursionError`.
    """
    return sys.version_info >= (3, 12)


@pytest.fixture
def cycle_policy_error():
    """`cycle_policy="error"` for one test, restoring the prior policy."""
    prior = strata.config.get("cycle_policy")
    strata.config.set("cycle_policy", "error")
    try:
        yield
    finally:
        strata.config.set("cycle_policy", prior)


@pytest.fixture
def expected_text() -> Callable[[Any, Callable[[Any], Any]], str]:
    """The compact text the oracle expects for `obj` under `default`."""
    return lambda obj, default: _expected(obj, default, [0])

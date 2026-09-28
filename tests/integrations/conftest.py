"""Shared oracle for the third-party integration suites.

`tests/integrations` shows that `strata.dumps_with_default(obj, default)`
composes with third-party types (docs/architecture/dumps_with_default.md, the
reuse table; api.md, dumps_with_default). It is excluded from `testpaths`,
`make test`, `make gate` and the PGO profile;
`make test-integrations` runs it. Every test reads the same way: stdlib `json`
with the same `default` is the oracle, byte for byte against its compact form,
and strata's own parser closes the round trip. The framework adapters' suites
(`test_flask.py` and its siblings) take each framework's default serializer as
their oracle instead (api.md, Framework adapters).
"""

import json
import sys
from collections.abc import Callable
from typing import Any

import pytest

import strata


def _counted(default: Callable[[Any], Any], calls: list[int]) -> Callable[[Any], Any]:
    def hook(obj: Any) -> Any:
        calls[0] += 1
        return default(obj)

    return hook


def _composes(obj: Any, default: Callable[[Any], Any]) -> Any:
    """Assert strata and stdlib `json` agree on `obj` under `default`; return the decoded value.

    api.md, dumps_with_default: the callable is called only where `dumps`
    would otherwise raise, once per such object, so the call count matches
    stdlib's for hooks that return JSON-native values.
    """
    json_calls, strata_calls = [0], [0]
    expected = json.dumps(
        obj,
        default=_counted(default, json_calls),
        separators=(",", ":"),
        ensure_ascii=False,
    )
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


@pytest.fixture
def native_document() -> dict[str, Any]:
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

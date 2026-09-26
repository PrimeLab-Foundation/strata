"""Shared oracle for the third-party integration suites.

`tests/integrations` shows that `strata.dumps(..., default=...)` composes with
third-party types (docs/architecture/dumps_default_hook.md, "Test placement").
It is excluded from `testpaths`, `make test`, `make gate` and the PGO profile;
`make test-integrations` runs it. Every test reads the same way: stdlib `json`
with the same `default` is the oracle, byte for byte against its compact form,
and strata's own parser closes the round trip.
"""

import json
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

    docs/context/api.md, "Unsupported-type hook": the callable is called only
    where `dumps` would otherwise raise, once per such object, so the call
    count matches stdlib's for hooks that return JSON-native values.
    """
    json_calls, strata_calls = [0], [0]
    expected = json.dumps(
        obj,
        default=_counted(default, json_calls),
        separators=(",", ":"),
        ensure_ascii=False,
    )
    text = strata.dumps(obj, default=_counted(default, strata_calls))
    assert text == expected
    assert strata_calls[0] == json_calls[0]
    assert strata.dumps(obj, default=default, return_type="bytes") == expected.encode()
    decoded = strata.loads(text)
    assert decoded == json.loads(expected)
    return decoded


@pytest.fixture
def composes() -> Callable[[Any, Callable[[Any], Any]], Any]:
    return _composes

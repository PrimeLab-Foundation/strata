"""Shared fixtures for the third-party integration tree.

This tree is outside `testpaths`, so nothing here runs under `make test` or
`make gate`; `make test-integrations` is its only entry point (see README.md).
"""

from __future__ import annotations

import json

import pytest


@pytest.fixture
def stdlib_oracle():
    """Compare a strata `dumps(..., default=f)` against stdlib json's.

    The comparison is on parsed values, not bytes: key order is shared but
    float spelling and separator policy are not part of this contract.
    """

    def compare(value, default):
        assert json.loads(__import__("strata").dumps(value, default=default)) == json.loads(
            json.dumps(value, default=default)
        )

    return compare

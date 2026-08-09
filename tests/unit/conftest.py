"""Shared fixtures for the contract suites."""

import pytest

import strata

CONFIG_KEYS = ("duplicate_key_policy", "cycle_policy")


@pytest.fixture(autouse=True)
def restore_config():
    """Put every setting back after each test.

    `strata.config` is process state; a test that changes it and leaves it
    changed silently rewrites the contract for whatever runs next
    (docs/context/styleguide.md).
    """
    saved = {key: strata.config.get(key) for key in CONFIG_KEYS}
    yield
    for key, value in saved.items():
        strata.config.set(key, value)

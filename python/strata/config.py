"""
Strata configuration.

Thin pass-through to the C++ config registry.
"""

from . import _strata as _native


def set(key: str, value) -> None:
    """Set a config value. Known keys: duplicate_key_policy, cycle_policy."""
    _native.config_set(key, value)


def get(key: str):
    """Get a config value by key."""
    return _native.config_get(key)


def list() -> dict:
    """List all config keys and their current values."""
    return _native.config_list()

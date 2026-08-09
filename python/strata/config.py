"""Engine settings.

A pass-through to the C++ policy variables. Reads come from the policies
themselves rather than from a cached copy, so what ``get`` reports and what the
engine does cannot drift apart.

Settings and their scope (docs/context/api.md):

``duplicate_key_policy``
    ``"first"`` (default) | ``"last"`` | ``"error"`` | ``"warn"``. Consumed
    through a **thread-local**, so setting it affects only the calling thread.

``cycle_policy``
    ``"warn"`` (default) | ``"error"`` | ``"ignore"``. A process-global: every
    thread sees it. Active from the first call, with no configuration needed.
"""

from __future__ import annotations

from . import _strata as _native


def set(key: str, value) -> None:
    """Set a setting.

    Raises:
        KeyError: ``key`` is not a known setting.
        TypeError: ``value`` is not a string.
        ValueError: ``value`` is not one of the allowed values for ``key``.
    """
    _native.config_set(key, value)


def get(key: str):
    """Return the current value of ``key``, or ``None`` if it is unknown."""
    return _native.config_get(key)


def list() -> dict:
    """Return every setting and its current value."""
    return _native.config_list()

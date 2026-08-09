"""Integration smoke tests for the installed package.

Counterpart of ``tests/unit/test_version.py``: this file exercises the
artifact as installed (native extension present, package importable), while
the unit mirror pins the documented contract.
"""

import importlib

import strata


def test_package_imports():
    """`import strata` succeeds against the built extension."""
    assert strata.__name__ == "strata"


def test_native_engine_is_importable():
    """docs/context/api.md: "Native module: `strata._strata`"."""
    native = importlib.import_module("strata._strata")
    assert native.__name__ == "strata._strata"


def test_version_is_exported():
    """docs/context/api.md § Package exports lists `__version__`."""
    assert "__version__" in strata.__all__
    assert isinstance(strata.__version__, str)

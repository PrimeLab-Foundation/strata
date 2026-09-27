"""scripts/py_tests.py gives every gate run a fixed-length ``--basetemp``.

The gate trains the PGO profile, and the lengths of the ``tmp_path`` strings the
extension copies are recorded in it. pytest's default base directory numbers the
runs before it (``pytest-9`` -> ``pytest-10``), which gave M12b's A/B arm B a
different profile from A and A2 (docs/performance/experiment-ledger.md, M12b).

This file lives in ``tests/py`` on purpose: nothing here calls strata, and
``tests/unit/conftest.py``'s autouse fixture would add config calls to the
profile for every test placed there (the M12b follow-up's discrimination draw).
"""

from __future__ import annotations

import os
import shutil
import tempfile

from scripts.py_tests import pytest_root, with_basetemp


def test_default_run_gets_a_fresh_basetemp_as_long_as_pytest_0() -> None:
    first_args, first = with_basetemp(["-q"])
    second_args, second = with_basetemp(["-q"])
    try:
        assert first is not None and second is not None
        assert first != second
        assert first_args == ["-q", f"--basetemp={first}"]
        assert second_args == ["-q", f"--basetemp={second}"]
        root = pytest_root()
        temproot = os.environ.get("PYTEST_DEBUG_TEMPROOT") or tempfile.gettempdir()
        assert os.path.dirname(root) == temproot
        assert os.path.basename(root).startswith("pytest-of-")
        for path in (first, second):
            assert os.path.dirname(path) == root
            # pytest's own single-digit directory, `pytest-0`, is eight characters.
            assert len(os.path.basename(path)) == len("pytest-0")
            assert not os.path.basename(path).startswith("pytest-")
            assert os.path.isdir(path)
    finally:
        for path in (first, second):
            if path is not None:
                shutil.rmtree(path, ignore_errors=True)


def test_caller_basetemp_is_kept() -> None:
    for args in (["--basetemp=/x/y"], ["--basetemp", "/x/y"]):
        assert with_basetemp(args) == (args, None)

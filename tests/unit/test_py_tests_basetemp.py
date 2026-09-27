"""scripts/py_tests.py gives every gate run a fixed-length ``--basetemp``.

The gate trains the PGO profile, and the lengths of the ``tmp_path`` strings the
extension copies are recorded in it. pytest's default base directory numbers the
runs before it (``pytest-9`` -> ``pytest-10``), which gave M12b's A/B arm B a
different profile from A and A2 (docs/performance/experiment-ledger.md, M12b).
"""

from __future__ import annotations

import os
import shutil
import tempfile

from scripts.py_tests import with_basetemp


def test_default_run_gets_a_fresh_fixed_length_basetemp() -> None:
    first_args, first = with_basetemp(["-q"])
    second_args, second = with_basetemp(["-q"])
    try:
        assert first is not None and second is not None
        assert first != second
        assert first_args == ["-q", f"--basetemp={first}"]
        assert second_args == ["-q", f"--basetemp={second}"]
        assert len(first) == len(second)
        assert os.path.dirname(first) == tempfile.gettempdir()
        assert os.path.isdir(first) and os.path.isdir(second)
    finally:
        for path in (first, second):
            if path is not None:
                shutil.rmtree(path, ignore_errors=True)


def test_caller_basetemp_is_kept() -> None:
    for args in (["--basetemp=/x/y"], ["--basetemp", "/x/y"]):
        assert with_basetemp(args) == (args, None)

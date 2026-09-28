#!/usr/bin/env python3
"""Run both Python suites: ``tests/py`` (integration) and ``tests/unit`` (contract).

`make test-py` and the post-build gate in ``setup.py`` share this entry point,
so the two can never disagree about which suites count. The previous
implementation's ``make test-py`` ran ``tests/py`` only and silently skipped
the contract mirrors (docs/build-and-test/SKILL.md).

``--path`` prepends directories to the test process' import path; the build
gate uses it to make the freshly built extension win over any installed copy.

Every run gets a fresh ``--basetemp`` of fixed length unless the caller passes
one. pytest's default, ``<tmp>/pytest-of-<user>/pytest-<N>``, counts the runs
before it, so every ``tmp_path`` grows by a character at N = 10, 100, 1000; the
gate runs on the instrumented build, and the path lengths the extension copies
enter the PGO profile (memcpy-size value profiles, string growth branches).
The A/B arms build one after another on one runner, so the numbering alone gave
arm B a different profile from A and A2 (docs/performance/experiment-ledger.md,
M12b, run 36291977906). The pinned directory sits where pytest's own would and
is exactly as long as a single-digit ``pytest-<N>``, the layout CI's builds
trained on before the pin, so pinning leaves their profile unchanged.

``--training`` is for the PGO instrumented pass only: it ignores the
native-type suites (``TRAINING_IGNORES``), whose cold paths must carry zero
profile counts (docs/architecture/native_types.md, "Hot-path protection" 4).
Every gate runs without it.
"""

from __future__ import annotations

import argparse
import getpass
import importlib.util
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
TEST_PATHS = ("tests/py", "tests/unit")
TRAINING_IGNORES = ("tests/unit/native_types", "tests/py/native_types")


def pytest_root() -> str:
    """pytest's own ``<temproot>/pytest-of-<user>``, created the way pytest creates it."""
    temproot = os.environ.get("PYTEST_DEBUG_TEMPROOT") or tempfile.gettempdir()
    try:
        user = getpass.getuser() or "unknown"
    except (ImportError, OSError, KeyError):
        user = "unknown"
    root = os.path.join(temproot, f"pytest-of-{user}")
    try:
        os.makedirs(root, mode=0o700, exist_ok=True)
    except OSError:
        root = os.path.join(temproot, "pytest-of-unknown")
        os.makedirs(root, mode=0o700, exist_ok=True)
    return root


def with_basetemp(pytest_args: list[str]) -> tuple[list[str], str | None]:
    """Return @p pytest_args plus a fresh fixed-length ``--basetemp``, and its path.

    A caller's own ``--basetemp`` is kept and ``None`` returned in its place.
    The directory is ``pytest_root()`` plus eight random ``mkdtemp`` characters:
    as long as pytest's ``pytest-<N>`` for N below 10, never named like one, and
    never shared by concurrent runs.
    """
    if any(a == "--basetemp" or a.startswith("--basetemp=") for a in pytest_args):
        return pytest_args, None
    basetemp = tempfile.mkdtemp(prefix="", dir=pytest_root())
    return [*pytest_args, f"--basetemp={basetemp}"], basetemp


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--path",
        action="append",
        default=[],
        metavar="DIR",
        help="directory to prepend to the import path (repeatable)",
    )
    parser.add_argument(
        "--training",
        action="store_true",
        help="PGO instrumented pass only: ignore the native-type suites",
    )
    parser.add_argument(
        "pytest_args",
        nargs=argparse.REMAINDER,
        help="extra arguments forwarded to pytest (after --)",
    )
    args = parser.parse_args(argv)

    if importlib.util.find_spec("pytest") is None:
        # A missing test runner must be a loud failure, never a skipped gate.
        sys.stderr.write(
            f"error: pytest is not installed for {sys.executable}.\n"
            "The Python suites are a mandatory gate; install the dev extras "
            "(make dev, or make install-dev) and re-run.\n",
        )
        return 1

    prefix = [str(Path(p).resolve()) for p in args.path]

    env = os.environ.copy()
    inherited = env.get("PYTHONPATH")
    env["PYTHONPATH"] = os.pathsep.join([*prefix, inherited] if inherited else prefix)

    extra, basetemp = with_basetemp([a for a in args.pytest_args if a != "--"])
    ignores = [f"--ignore={path}" for path in TRAINING_IGNORES] if args.training else []
    pytest_argv = [*TEST_PATHS, *ignores, *extra]

    # pytest is launched through a `-c` bootstrap rather than `-m pytest` so the
    # prefix lands on sys.path *inside* the interpreter. PYTHONPATH alone is not
    # enough: pip's build isolation installs a sitecustomize that rewrites
    # sys.path at startup, which silently drops the staging directory and made
    # the post-build gate fail with "No module named 'strata'".
    # Under the sanitized gate (scripts/asan_py_tests.sh) the process that
    # imports the extension proves the runtime is loaded and CPython's
    # allocator routed through it, so a lost preload or allocator setting is
    # a failure and never a silent pass.
    armed_check = ""
    if os.environ.get("STRATA_ASAN_GATE") == "1":
        armed_check = (
            "import ctypes, os; "
            "assert os.environ.get('PYTHONMALLOC') == 'malloc', "
            "'ASan gate: PYTHONMALLOC=malloc did not reach the test process'; "
            "ctypes.CDLL(None).__asan_init; "
        )
    bootstrap = (
        "import sys; "
        f"sys.path[:0] = {prefix!r}; "
        f"{armed_check}"
        "import pytest; "
        f"raise SystemExit(pytest.main({pytest_argv!r}))"
    )
    cmd = [sys.executable, "-c", bootstrap]
    print(f"+ pytest {' '.join(pytest_argv)}", flush=True)
    if prefix:
        print("  import path prefix: " + os.pathsep.join(prefix), flush=True)
    completed = subprocess.run(cmd, cwd=PROJECT_ROOT, env=env, check=False)
    if basetemp is not None:
        if completed.returncode == 0:
            shutil.rmtree(basetemp, ignore_errors=True)
        else:
            print(f"  test temp files kept for inspection: {basetemp}", flush=True)
    return completed.returncode


if __name__ == "__main__":
    raise SystemExit(main())

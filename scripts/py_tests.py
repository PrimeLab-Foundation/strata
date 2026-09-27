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
M12b, run 36291977906).
"""

from __future__ import annotations

import argparse
import importlib.util
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
TEST_PATHS = ("tests/py", "tests/unit")


def with_basetemp(pytest_args: list[str]) -> tuple[list[str], str | None]:
    """Return @p pytest_args plus a fresh fixed-length ``--basetemp``, and its path.

    A caller's own ``--basetemp`` is kept and ``None`` returned in its place.
    ``mkdtemp`` names are a fixed prefix and eight random characters, so the
    length depends only on the temp root, and concurrent runs never share one.
    """
    if any(a == "--basetemp" or a.startswith("--basetemp=") for a in pytest_args):
        return pytest_args, None
    basetemp = tempfile.mkdtemp(prefix="strata-pytest-")
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
    pytest_argv = [*TEST_PATHS, *extra]

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

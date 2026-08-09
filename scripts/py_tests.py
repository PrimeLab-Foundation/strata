#!/usr/bin/env python3
"""Run both Python suites: ``tests/py`` (integration) and ``tests/unit`` (contract).

`make test-py` and the post-build gate in ``setup.py`` share this entry point,
so the two can never disagree about which suites count. The previous
implementation's ``make test-py`` ran ``tests/py`` only and silently skipped
the contract mirrors (docs/build-and-test/SKILL.md).

``--path`` prepends directories to the test process' import path; the build
gate uses it to make the freshly built extension win over any installed copy.
"""

from __future__ import annotations

import argparse
import importlib.util
import os
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
TEST_PATHS = ("tests/py", "tests/unit")


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

    pytest_argv = [*TEST_PATHS, *(a for a in args.pytest_args if a != "--")]

    # pytest is launched through a `-c` bootstrap rather than `-m pytest` so the
    # prefix lands on sys.path *inside* the interpreter. PYTHONPATH alone is not
    # enough: pip's build isolation installs a sitecustomize that rewrites
    # sys.path at startup, which silently drops the staging directory and made
    # the post-build gate fail with "No module named 'strata'".
    bootstrap = (
        "import sys; "
        f"sys.path[:0] = {prefix!r}; "
        "import pytest; "
        f"raise SystemExit(pytest.main({pytest_argv!r}))"
    )
    cmd = [sys.executable, "-c", bootstrap]
    print(f"+ pytest {' '.join(pytest_argv)}", flush=True)
    if prefix:
        print("  import path prefix: " + os.pathsep.join(prefix), flush=True)
    completed = subprocess.run(cmd, cwd=PROJECT_ROOT, env=env, check=False)
    return completed.returncode


if __name__ == "__main__":
    raise SystemExit(main())

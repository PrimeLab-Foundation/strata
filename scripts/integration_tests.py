#!/usr/bin/env python3
"""Run ``tests/integrations``: ``dumps_with_default`` against third-party types, and the framework adapters.

The tree imports pydantic, attrs, numpy, Flask, Django, aiohttp, Falcon,
FastAPI and structlog, so it stays out of ``testpaths``,
``make test``, ``make gate`` and the PGO training run
(docs/architecture/dumps_default_hook.md, "Test placement", which
docs/architecture/dumps_with_default.md inherits). This entry point
installs the ``integrations`` extra's requirements, read from ``pyproject.toml``,
into the running interpreter when any is missing, then runs pytest on that
tree only. Extra arguments after ``--`` are forwarded to pytest.
"""

from __future__ import annotations

import argparse
import importlib.metadata
import importlib.util
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
TEST_PATH = "tests/integrations"
EXTRA = "integrations"


def extra_requirements() -> list[str]:
    """The extra as ``pyproject.toml`` declares it, read with a TOML parser.

    Python 3.10 has no ``tomllib``; there ``tomli`` reads it, which pytest
    itself depends on below 3.11 (as ``missing`` relies on its ``packaging``),
    so it is present wherever this script can run pytest at all. Reading the
    file rather than an installed distribution's metadata keeps the answer
    independent of the distribution's name.
    """
    try:
        import tomllib
    except ModuleNotFoundError:
        try:
            import tomli as tomllib
        except ModuleNotFoundError:
            sys.stderr.write(
                f"error: reading pyproject.toml needs tomllib (Python 3.11+) or tomli "
                f"for {sys.executable}.\n",
            )
            raise SystemExit(1) from None
    with (PROJECT_ROOT / "pyproject.toml").open("rb") as fh:
        project = tomllib.load(fh)["project"]
    requirements = project.get("optional-dependencies", {}).get(EXTRA)
    if not requirements:
        sys.stderr.write(f"error: pyproject.toml declares no '{EXTRA}' extra.\n")
        raise SystemExit(1)
    return requirements


def missing(requirements: list[str]) -> list[str]:
    """The requirements that are absent or installed below their floor.

    Read with ``packaging``, which pytest itself depends on, so a requirement
    is satisfied only when the installed version is inside its specifier --
    presence alone is not enough (``numpy>=1.26`` with numpy 1.24 installed
    is missing).
    """
    from packaging.requirements import InvalidRequirement, Requirement

    absent = []
    for text in requirements:
        try:
            requirement = Requirement(text)
        except InvalidRequirement:
            sys.stderr.write(f"error: unreadable requirement {text!r}.\n")
            raise SystemExit(1) from None
        try:
            installed = importlib.metadata.version(requirement.name)
        except importlib.metadata.PackageNotFoundError:
            absent.append(text)
            continue
        if not requirement.specifier.contains(installed, prereleases=True):
            absent.append(text)
    return absent


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "pytest_args",
        nargs=argparse.REMAINDER,
        help="extra arguments forwarded to pytest (after --)",
    )
    args = parser.parse_args(argv)

    if importlib.util.find_spec("pytest") is None:
        sys.stderr.write(
            f"error: pytest is not installed for {sys.executable}.\n"
            "Install the dev extras (make dev, or make install-dev) and re-run.\n",
        )
        return 1

    to_install = missing(extra_requirements())
    if to_install:
        cmd = [sys.executable, "-m", "pip", "install", *to_install]
        print(f"+ {' '.join(cmd)}", flush=True)
        installed = subprocess.run(cmd, check=False)
        if installed.returncode != 0:
            return installed.returncode

    pytest_argv = [TEST_PATH, *(a for a in args.pytest_args if a != "--")]
    cmd = [sys.executable, "-m", "pytest", *pytest_argv]
    print(f"+ pytest {' '.join(pytest_argv)}", flush=True)
    completed = subprocess.run(cmd, cwd=PROJECT_ROOT, check=False)
    return completed.returncode


if __name__ == "__main__":
    raise SystemExit(main())

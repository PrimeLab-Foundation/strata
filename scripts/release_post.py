#!/usr/bin/env python3
"""Post-release verification glue: the PyPI wheel, installed and proven, before it is measured.

``.github/workflows/post-release.yml`` runs these on every release leg after a
PyPI publish (docs/architecture/release_pipeline.md, "Post-release
verification"). They live beside ``scripts/release.py`` rather than in it
because that file is already past the ~800-line rule and its tests patch its
module globals, so moving code out of it would silently unpatch them.

- ``install VERSION``: ``pip install --only-binary=strata-plf strata-plf==VERSION``
  from PyPI, retried with backoff while the new version propagates through the
  index. VERSION must be a final ``YYYY.M.D[.N]`` (release.py's grammar; an
  ``rcK`` never reaches PyPI). pip's cache is bypassed so that a retry cannot
  read a cached index page from before the upload.
- ``check-installed VERSION``: prints ``strata.__version__`` and where
  ``strata``, ``strata._strata`` and ``strata._dumps_hook`` import from, then
  fails unless the version is VERSION and every file resolves into
  site-packages, outside this checkout. Run it with the benchmark's own
  ``PYTHONPATH`` and working directory, so that a checkout tree shadowing the
  install would be caught here rather than measured.
- ``bench-requirements``: the ``bench`` extra as ``pyproject.toml`` declares it,
  one requirement per line: the competitor set ``make install-bench`` installs,
  without installing strata from the checkout.
"""

from __future__ import annotations

import argparse
import importlib
import runpy
import subprocess
import sys
import sysconfig
import time
from pathlib import Path
from typing import NoReturn

PROJECT_ROOT = Path(__file__).resolve().parents[1]
RELEASE_SCRIPT = PROJECT_ROOT / "scripts" / "release.py"
INDEX_URL = "https://pypi.org/simple/"
MODULES = ("strata", "strata._strata", "strata._dumps_hook")
# 15 + 30 + 60 + 120 * 4 s: about ten minutes of propagation before giving up.
ATTEMPTS = 8
FIRST_DELAY = 15
MAX_DELAY = 120


def _fail(message: str) -> NoReturn:
    raise SystemExit(f"release_post: {message}")


def _release() -> dict:
    return runpy.run_path(str(RELEASE_SCRIPT))


def require_final(version: str) -> None:
    """Exit unless VERSION is a final release in release.py's grammar."""
    if _release()["version_key"](version)[4] == 0:
        _fail(f"{version} is a release candidate; PyPI holds final versions only")


def install(version: str) -> int:
    require_final(version)
    project = _release()["PROJECT_NAME"]
    cmd = [
        sys.executable,
        "-m",
        "pip",
        "install",
        "--no-cache-dir",
        "--index-url",
        INDEX_URL,
        f"--only-binary={project}",
        f"{project}=={version}",
    ]
    delay = FIRST_DELAY
    for attempt in range(1, ATTEMPTS + 1):
        print(f"+ {' '.join(cmd)}  (attempt {attempt}/{ATTEMPTS})", flush=True)
        if subprocess.run(cmd, check=False).returncode == 0:
            return 0
        if attempt < ATTEMPTS:
            print(f"release_post: not installable yet; retrying in {delay} s", flush=True)
            time.sleep(delay)
            delay = min(delay * 2, MAX_DELAY)
    _fail(f"{project}=={version} did not install from {INDEX_URL} in {ATTEMPTS} attempts")


def check_installed(version: str) -> int:
    paths = sysconfig.get_paths()
    site = sorted({Path(paths[key]).resolve() for key in ("purelib", "platlib") if key in paths})
    strata = importlib.import_module("strata")
    print(f"+ strata.__version__ = {strata.__version__!r}", flush=True)
    # The package first: a shadowing tree is named here, before a submodule it
    # lacks can fail to import.
    for name in MODULES:
        path = Path(importlib.import_module(name).__file__).resolve()
        print(f"+ {name}.__file__ = {path}", flush=True)
        if path.is_relative_to(PROJECT_ROOT) or not any(path.is_relative_to(s) for s in site):
            _fail(
                f"{name} imports from {path}, not from the installed wheel in "
                f"{', '.join(map(str, site))} (the checkout {PROJECT_ROOT} must not shadow it)",
            )
    if strata.__version__ != version:
        _fail(f"strata.__version__ is {strata.__version__!r}, not the released {version!r}")
    print(f"release_post: strata {version} is the installed wheel", flush=True)
    return 0


def bench_requirements(pyproject: Path | None = None) -> list[str]:
    """The ``bench`` extra as ``pyproject.toml`` declares it (tomli below Python 3.11)."""
    try:
        import tomllib
    except ModuleNotFoundError:
        try:
            import tomli as tomllib
        except ModuleNotFoundError:
            _fail(f"reading pyproject.toml needs tomllib (Python 3.11+) or tomli: {sys.executable}")
    with (pyproject or PROJECT_ROOT / "pyproject.toml").open("rb") as fh:
        project = tomllib.load(fh)["project"]
    requirements = project.get("optional-dependencies", {}).get("bench")
    if not requirements:
        _fail("pyproject.toml declares no 'bench' extra")
    return requirements


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    commands = parser.add_subparsers(dest="command", required=True)
    for name, text in (
        ("install", "install strata-plf==VERSION from PyPI, retried while the index propagates"),
        ("check-installed", "prove the importable strata is the installed VERSION"),
    ):
        commands.add_parser(name, help=text).add_argument("version", help="final YYYY.M.D[.N]")
    commands.add_parser("bench-requirements", help="print the bench extra, one per line")
    args = parser.parse_args(argv)
    if args.command == "install":
        return install(args.version)
    if args.command == "check-installed":
        return check_installed(args.version)
    print("\n".join(bench_requirements()))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

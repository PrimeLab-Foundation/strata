#!/usr/bin/env python3
"""Configure, build and run the C++ test suite through CMake/ctest.

CMakeLists.txt is the single C++ test registry; this script is the single way
it is driven — from ``make test-cpp`` / ``make cpp-build``, from the pre-build
gate in ``setup.py``, and from CI. Keeping one invocation path is what stops
the source lists from drifting the way the previous implementation's three
parallel harnesses did (docs/build-and-test/SKILL.md).
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]


def _resolve(tool: str) -> str:
    """Find *tool* beside the running interpreter first, then on PATH.

    The interpreter's own bin directory wins so that the pinned `cmake` — from
    the dev extras locally, or from pip's isolated build environment during the
    install gate — is preferred over whatever version the system happens to
    ship. `make gate` configures one build tree from two drivers in a single
    run; letting them disagree about the CMake binary invites stale-cache
    surprises.
    """
    found = shutil.which(tool, path=str(Path(sys.executable).parent)) or shutil.which(tool)
    if found is None:
        sys.stderr.write(
            f"error: '{tool}' not found. The C++ suite is a mandatory build gate; "
            "install CMake (it ships cmake and ctest) and re-run.\n",
        )
        raise SystemExit(1)
    return found


def _run(cmd: list[str]) -> None:
    print("+ " + " ".join(cmd), flush=True)
    completed = subprocess.run(cmd, cwd=PROJECT_ROOT, check=False)
    if completed.returncode != 0:
        raise SystemExit(completed.returncode)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    # Not bare build/: setuptools owns build/lib.* and build/temp.*, and a stale
    # CMakeCache.txt sitting next to them can poison a wheel build.
    parser.add_argument("--build-dir", default="build/cmake", help="CMake binary directory")
    parser.add_argument("--build-type", default="Release", help="CMAKE_BUILD_TYPE")
    parser.add_argument(
        "--build-only",
        action="store_true",
        help="configure and build without running ctest",
    )
    args = parser.parse_args(argv)

    cmake = _resolve("cmake")
    _run([cmake, "-S", ".", "-B", args.build_dir, f"-DCMAKE_BUILD_TYPE={args.build_type}"])
    # --config/-C are ignored by single-config generators and are what makes
    # multi-config ones (Visual Studio, Xcode, Ninja Multi-Config) build and run
    # the configuration we asked for instead of silently falling back to Debug.
    _run([cmake, "--build", args.build_dir, "--config", args.build_type])

    if args.build_only:
        return 0

    ctest = _resolve("ctest")
    # --no-tests=error: without it ctest exits 0 when the registry produced no
    # tests at all, so a broken CMakeLists would report a green C++ gate having
    # run nothing.
    _run(
        [
            ctest,
            "--test-dir",
            args.build_dir,
            "--build-config",
            args.build_type,
            "--output-on-failure",
            "--no-tests=error",
        ]
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

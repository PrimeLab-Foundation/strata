"""Build `strata._strata` behind mandatory test gates.

Metadata lives in pyproject.toml (PEP 621); this file exists for the C++
extension and for the build gates: every ``pip install -e .`` runs the C++
suite *before* compiling and both Python suites *after*, so an install that
succeeds is an install whose tests passed (docs/context/convention.md,
"Test-gated builds").

Escape hatch: ``SKIP_TESTS=1`` — banned in CI and releases.

Both gates shell out to the same scripts the Makefile uses
(``scripts/cpp_tests.py``, ``scripts/py_tests.py``), so there is exactly one
definition of "the test suite".
"""

from __future__ import annotations

import os
import platform
import shutil
import subprocess
import sys
from pathlib import Path
from sysconfig import get_config_var, get_paths

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

PROJECT_ROOT = Path(__file__).resolve().parent
FACADE_DIR = PROJECT_ROOT / "python" / "strata"
SKIP_TESTS = os.environ.get("SKIP_TESTS", "0") == "1"
IN_CI = os.environ.get("CI", "").strip().lower() in ("1", "true", "yes", "on")

RULE = "=" * 70

if SKIP_TESTS and IN_CI:
    # convention.md: "Escape hatch SKIP_TESTS=1 is banned in CI and releases."
    # A warning-only ban is not a ban.
    raise SystemExit(
        "SKIP_TESTS=1 is banned in CI (CI environment detected). Build without the escape hatch.",
    )


def _banner(text: str) -> None:
    print(f"\n{RULE}\n{text}\n{RULE}", flush=True)


class TestGatedBuildExt(build_ext):
    """`build_ext` wrapped in the pre-build C++ gate and post-build Python gate."""

    def run(self) -> None:
        if SKIP_TESTS:
            _banner(
                "WARNING: SKIP_TESTS=1 — build gates disabled.\n"
                "This build is not release-ready and is not allowed in CI.",
            )
        else:
            _banner("GATE 1/2: C++ test suite (pre-build)")
            self._gate("C++", PROJECT_ROOT / "scripts" / "cpp_tests.py")

        _banner("Building the strata._strata extension")
        super().run()

        if not SKIP_TESTS:
            _banner("GATE 2/2: Python test suites (post-build)")
            self._python_gate()
            _banner("Build complete — both gates green")

    def _gate(self, layer: str, script: Path, *script_args: str) -> None:
        cmd = [sys.executable, str(script), *script_args]
        print("+ " + " ".join(cmd), flush=True)
        completed = subprocess.run(cmd, cwd=PROJECT_ROOT, check=False)
        if completed.returncode != 0:
            raise SystemExit(f"{layer} tests failed — aborting build (test-gated build)")

    def _python_gate(self) -> None:
        """Run the Python suites against the extension that was just built.

        The suites must import the *new* artifact, not a previously installed
        copy, so the staging directory holding the fresh extension is prepended
        to the import path and the pure-Python facade is placed beside it.
        """
        package_dir = Path(self.get_ext_fullpath("strata._strata")).resolve().parent
        if package_dir != FACADE_DIR:
            shutil.copytree(
                FACADE_DIR,
                package_dir,
                dirs_exist_ok=True,
                ignore=shutil.ignore_patterns("__pycache__"),
            )
        self._gate(
            "Python",
            PROJECT_ROOT / "scripts" / "py_tests.py",
            "--path",
            str(package_dir.parent),
        )


def _is_universal_build() -> bool:
    """True when building a macOS universal2 wheel (two -arch flags)."""
    parts = [
        os.environ.get("ARCHFLAGS"),
        os.environ.get("CFLAGS"),
        os.environ.get("CXXFLAGS"),
        get_config_var("ARCHFLAGS"),
        get_config_var("CFLAGS"),
        get_config_var("CXXFLAGS"),
    ]
    flags = " ".join(part for part in parts if part)
    return "-arch arm64" in flags and "-arch x86_64" in flags


def _compile_args() -> list[str]:
    if sys.platform == "win32":
        args = ["/std:c++20", "/O2"]
        if platform.machine() in ("AMD64", "x86_64"):
            args.append("/arch:AVX2")
        return args
    args = ["-std=c++20", "-O3", "-D_LIBCPP_DISABLE_AVAILABILITY"]
    # -march=native tunes for the build host; a universal2 wheel targets two
    # architectures at once and cannot use it.
    if not _is_universal_build():
        args.append("-march=native")
    return args


CORE_MANIFEST = PROJECT_ROOT / "src" / "strata" / "core_sources.txt"

# CPython-dependent translation units. Core sources are *not* listed here — they
# come from the shared manifest that CMake reads too, so the test binaries and
# the extension can never be built from different sources.
BINDING_SOURCES = [
    "src/strata/bindings/python_module.cpp",
]


def _core_sources() -> list[str]:
    entries = []
    for raw in CORE_MANIFEST.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        if not (PROJECT_ROOT / line).exists():
            raise SystemExit(f"core_sources.txt lists a file that does not exist: {line}")
        entries.append(line)
    return entries


ext_modules = [
    Extension(
        "strata._strata",
        sources=[*BINDING_SOURCES, *_core_sources()],
        include_dirs=[
            "include",
            get_paths()["include"],
        ],
        extra_compile_args=_compile_args(),
        language="c++",
    ),
]

setup(
    ext_modules=ext_modules,
    cmdclass={"build_ext": TestGatedBuildExt},
)

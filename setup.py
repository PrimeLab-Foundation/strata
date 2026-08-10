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


def _compiler_kind() -> str:
    """'clang', 'gcc' or 'msvc' — PGO and LTO spell everything differently."""
    if sys.platform == "win32":
        return "msvc"
    name = " ".join(filter(None, (os.environ.get("CXX"), get_config_var("CC"), ""))).lower()
    if "clang" in name:
        return "clang"
    if "gcc" in name or "g++" in name:
        return "gcc"
    # Apple ships clang as plain `cc`; elsewhere the GNU toolchain is the
    # safer guess. Either way PGO_MODE=use fails loudly if the guess is wrong.
    return "clang" if sys.platform == "darwin" else "gcc"


def _optimization_args() -> tuple[list[str], list[str]]:
    """(compile, link) flags for LTO and PGO, driven by the environment.

    `make pgo` (scripts/pgo_build.sh) sets these; a plain `make install`
    leaves them unset and gets an ordinary -O3 build.

    - `STRATA_ENABLE_LTO=1`   — link-time optimization.
    - `PGO_MODE=generate`     — instrument; the runtime writes .profraw files
                                to wherever `LLVM_PROFILE_FILE`/`GCOV_PREFIX` point.
    - `PGO_MODE=use`          — optimize against `STRATA_PGO_PROFILE`.
    """
    mode = os.environ.get("PGO_MODE", "").strip().lower()
    lto = os.environ.get("STRATA_ENABLE_LTO", "0").strip() == "1"
    if not mode and not lto:
        return [], []

    if mode not in ("", "generate", "use"):
        raise SystemExit(f"PGO_MODE must be 'generate' or 'use', not {mode!r}.")

    kind = _compiler_kind()
    if kind == "msvc":
        # MSVC PGO needs /GL + /LTCG:PGINSTRUMENT and a separate pgomgr step.
        # Refuse rather than silently produce an unoptimized build.
        raise SystemExit("STRATA_ENABLE_LTO/PGO_MODE are not supported with MSVC.")

    compile_args: list[str] = []
    link_args: list[str] = []

    if lto:
        flag = "-flto=thin" if kind == "clang" else "-flto"
        compile_args.append(flag)
        link_args.append(flag)

    # IR-level instrumentation (-fprofile-generate), NOT the front-end kind
    # (-fprofile-instr-generate). Front-end counters are source-level, so they
    # map poorly onto the IR the optimizer sees after inlining: they are the
    # right tool for coverage and the wrong one for PGO. Measured here, a
    # front-end profile made the "optimized" build ~20% *slower* than plain
    # -O3 — see the PGO entry in docs/performance/SKILL.md.
    if mode == "generate":
        compile_args.append("-fprofile-generate")
        link_args.append("-fprofile-generate")
    elif mode == "use":
        profile = os.environ.get("STRATA_PGO_PROFILE", "").strip()
        if not profile:
            raise SystemExit("PGO_MODE=use requires STRATA_PGO_PROFILE.")
        if not Path(profile).exists():
            raise SystemExit(f"STRATA_PGO_PROFILE does not exist: {profile}")
        compile_args.append(f"-fprofile-use={profile}")
        link_args.append(f"-fprofile-use={profile}")
        if kind == "gcc":
            # gcc errors out on a counter mismatch after an edit; clang warns.
            compile_args.append("-fprofile-correction")

    return compile_args, link_args


def _compile_args() -> list[str]:
    if sys.platform == "win32":
        # /Zc:__cplusplus: MSVC otherwise reports __cplusplus as 199711L and
        # the C++20 guards in the headers misfire.
        args = ["/std:c++20", "/O2", "/Zc:__cplusplus"]
        if platform.machine() in ("AMD64", "x86_64"):
            args.append("/arch:AVX2")
        return args
    args = ["-std=c++20", "-O3", "-D_LIBCPP_DISABLE_AVAILABILITY"]
    # -march=native tunes for the build host; a universal2 wheel targets two
    # architectures at once and cannot use it.
    if not _is_universal_build():
        args.append("-march=native")
    args.extend(_optimization_args()[0])
    return args


def _link_args() -> list[str]:
    if sys.platform == "win32":
        return []
    return _optimization_args()[1]


CORE_MANIFEST = PROJECT_ROOT / "src" / "strata" / "core_sources.txt"

# CPython-dependent translation units. Core sources are *not* listed here — they
# come from the shared manifest that CMake reads too, so the test binaries and
# the extension can never be built from different sources.
BINDING_SOURCES = [
    "src/strata/bindings/python_module.cpp",
    "src/strata/bindings/python_loads.cpp",
    "src/strata/bindings/python_dumps.cpp",
    "src/strata/bindings/python_document.cpp",
    "src/strata/bindings/python_ndjson.cpp",
    "src/strata/bindings/python_files.cpp",
    "src/strata/bindings/python_jsonpath.cpp",
    "src/strata/bindings/python_folder.cpp",
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
        extra_link_args=_link_args(),
        language="c++",
    ),
]

setup(
    ext_modules=ext_modules,
    cmdclass={"build_ext": TestGatedBuildExt},
)

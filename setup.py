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

import functools
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


def _windows_compiler_override() -> str | None:
    """`STRATA_WIN_COMPILER=cl|clang-cl`: build with the named compiler, no LTCG.

    Windows builds are MSVC with the LTCG that setuptools hardcodes (/GL on
    the compile, /LTCG on the link) and, under scripts/pgo_build_msvc.py,
    PGO on top. This override strips LTCG and swaps the compile step's
    executable: `cl` is the plain-MSVC control (no PGO, no LTO); `clang-cl`
    takes the same command line (the /O2, /arch:AVX2 and /std:c++20
    spellings above; it ignores /GL), links with the MSVC linker, and
    carries clang's own profile flags when PGO_MODE asks
    (scripts/pgo_build_clang_cl.py). The headers guard their MSVC paths
    with `_MSC_VER && !__clang__`, so clang-cl takes their GNU branches.
    The profile workflow measured the toolchains on one commit: MSVC
    compiles the serializer's record and float paths 20-30% slower than
    clang-cl, which reads them at parity with the LLVM-built rivals
    (docs/decisions.md 2026-09-03), so the benchmark leg measures the
    clang-cl build and MSVC stays a tested compiler in the CI matrix.
    Fails loudly when the compiler is asked for but not installed.
    """
    wanted = os.environ.get("STRATA_WIN_COMPILER", "").strip().lower()
    if not wanted:
        return None
    if sys.platform != "win32" or wanted not in ("cl", "clang-cl"):
        raise SystemExit(
            f"STRATA_WIN_COMPILER={wanted!r}: only 'cl' or 'clang-cl', and only on Windows."
        )
    if wanted == "cl" and (
        os.environ.get("PGO_MODE") or os.environ.get("STRATA_ENABLE_LTO", "0").strip() == "1"
    ):
        raise SystemExit(
            "STRATA_WIN_COMPILER=cl is a plain build; unset PGO_MODE and STRATA_ENABLE_LTO."
        )
    if wanted == "cl":
        return "cl"  # MSVCCompiler resolves cl.exe itself; it is not on the shell's PATH
    found = shutil.which(wanted) or shutil.which(wanted, path=r"C:\Program Files\LLVM\bin")
    if not found:
        raise SystemExit(f"STRATA_WIN_COMPILER={wanted} but {wanted}.exe is not on PATH.")
    return found


class TestGatedBuildExt(build_ext):
    """`build_ext` wrapped in the pre-build C++ gate and post-build Python gate."""

    def build_extensions(self) -> None:
        override = _windows_compiler_override()
        if override:
            # MSVCCompiler resolves its toolchain lazily in initialize() and
            # asserts against a second call (a cross build has already made
            # it); resolve it, then point the compile step at the override
            # and take LTCG out of both halves so the build is plainly /O2.
            if not self.compiler.initialized:
                self.compiler.initialize()
            if override != "cl":
                self.compiler.cc = override
            self.compiler.compile_options = [
                flag for flag in self.compiler.compile_options if flag != "/GL"
            ]
            # In place: initialize() keeps its own table of references to
            # these very lists and link() reads that table, so a rebound
            # attribute would leave /LTCG on the link line.
            for name in dir(self.compiler):
                if name.startswith("ldflags"):
                    flags = getattr(self.compiler, name)
                    if isinstance(flags, list):
                        flags[:] = [flag for flag in flags if flag != "/LTCG"]
            print(f"+ compiling with {self.compiler.cc}, plain /O2 (no LTCG)", flush=True)
        super().build_extensions()

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
    """'clang', 'gcc', 'msvc' or 'clang-cl' — PGO and LTO spell everything differently."""
    if sys.platform == "win32":
        wanted = os.environ.get("STRATA_WIN_COMPILER", "").strip().lower()
        return "clang-cl" if wanted == "clang-cl" else "msvc"
    name = " ".join(filter(None, (os.environ.get("CXX"), get_config_var("CC"), ""))).lower()
    if "clang" in name:
        return "clang"
    if "gcc" in name or "g++" in name:
        return "gcc"
    # Apple ships clang as plain `cc`; elsewhere the GNU toolchain is the
    # safer guess. Either way PGO_MODE=use fails loudly if the guess is wrong.
    return "clang" if sys.platform == "darwin" else "gcc"


def _msvc_optimization_args(mode: str) -> tuple[list[str], list[str]]:
    """MSVC spelling: LTO is /GL + /LTCG; PGO rides on LTCG via /GENPROFILE → /USEPROFILE.

    `STRATA_PGO_PROFILE` names the .pgd both phases share: the instrumented
    link creates it, the training runs drop `strata!N.pgc` beside it, and the
    /USEPROFILE link merges those automatically — no separate pgomgr step
    (VS2015+). PGO implies LTCG, so /GL+/LTCG are on whenever anything here is
    requested. The instrumented extension needs pgort140.dll resolvable at
    import — Python 3.8+ does not consult PATH for extension-module
    dependencies, so scripts/pgo_build_msvc.py stages it beside python.exe.
    """
    compile_args = ["/GL"]
    link_args = ["/LTCG"]
    if mode:
        profile = os.environ.get("STRATA_PGO_PROFILE", "").strip()
        if not profile:
            raise SystemExit(f"PGO_MODE={mode} requires STRATA_PGO_PROFILE (a .pgd path) on MSVC.")
        if mode == "generate":
            link_args.append(f"/GENPROFILE:PGD={profile}")
        else:
            if not Path(profile).exists():
                raise SystemExit(f"STRATA_PGO_PROFILE does not exist: {profile}")
            link_args.append(f"/USEPROFILE:PGD={profile}")
    return compile_args, link_args


@functools.lru_cache(maxsize=1)
def _clang_runtime_dir() -> Path:
    """The directory holding clang's runtime libraries (the profile runtime).

    clang.exe sits beside clang-cl.exe and answers -print-runtime-dir in
    either layout LLVM ships (per-target `lib/x86_64-pc-windows-msvc` or the
    older `lib/windows`); it exits 0 with a placeholder line when the
    directory is absent, so the answer is checked as a directory and the
    older layout under -print-resource-dir is the fallback. Cached: setup.py
    is imported several times per pip invocation (lru_cache).
    """
    clang_cl = shutil.which("clang-cl") or shutil.which(
        "clang-cl", path=r"C:\Program Files\LLVM\bin"
    )
    if not clang_cl:
        raise SystemExit("STRATA_WIN_COMPILER=clang-cl but clang-cl.exe is not on PATH.")
    clang = Path(clang_cl).with_name("clang.exe")

    def probe(flag: str) -> str:
        completed = subprocess.run([str(clang), flag], capture_output=True, text=True, check=False)
        if completed.returncode != 0:
            raise SystemExit(f"{clang} {flag} failed: {completed.stderr.strip()}")
        return completed.stdout.strip()

    runtime_dir = Path(probe("-print-runtime-dir"))
    if not runtime_dir.is_dir():
        runtime_dir = Path(probe("-print-resource-dir")) / "lib" / "windows"
    if not runtime_dir.is_dir():
        raise SystemExit(f"clang's runtime directory was not found ({runtime_dir}).")
    return runtime_dir


def _clang_cl_optimization_args(mode: str, lto: bool) -> tuple[list[str], list[str]]:
    """clang-cl spelling: clang's own PGO flags behind /clang:, the MSVC linker.

    IR-level instrumentation (-fprofile-generate), as on POSIX: the profile
    runtime writes .profraw wherever LLVM_PROFILE_FILE points when the
    interpreter exits, and llvm-profdata merges them for the -fprofile-use
    phase (scripts/pgo_build_clang_cl.py). Every instrumented object names
    the profile runtime library itself (a /DEFAULTLIB directive in its
    .drectve, spelled for whichever layout is installed) and references
    the runtime hook that pulls it from the archive; what link.exe cannot
    know is where that library lives, so its directory goes on the link
    line. LTO would need lld-link for the bitcode objects and is not
    spelled here yet -- a plain clang-cl build already measured ahead of
    MSVC's PGO+LTCG on the serializer (docs/decisions.md 2026-09-03).
    """
    if lto:
        raise SystemExit("STRATA_ENABLE_LTO=1 is not supported with clang-cl yet (needs lld-link).")
    compile_args: list[str] = []
    link_args: list[str] = []
    if mode == "generate":
        compile_args.append("/clang:-fprofile-generate")
        link_args.append(f"/LIBPATH:{_clang_runtime_dir()}")
    elif mode == "use":
        profile = os.environ.get("STRATA_PGO_PROFILE", "").strip()
        if not profile:
            raise SystemExit("PGO_MODE=use requires STRATA_PGO_PROFILE.")
        if not Path(profile).exists():
            raise SystemExit(f"STRATA_PGO_PROFILE does not exist: {profile}")
        compile_args.append(f"/clang:-fprofile-use={profile}")
    return compile_args, link_args


def _optimization_args() -> tuple[list[str], list[str]]:
    """(compile, link) flags for LTO and PGO, driven by the environment.

    `make pgo` (scripts/pgo_build.sh) sets these on POSIX and
    `scripts/pgo_build_msvc.py` does on Windows; a plain `make install`
    leaves them unset and gets an ordinary -O3 / /O2 build.

    - `STRATA_ENABLE_LTO=1`   — link-time optimization.
    - `PGO_MODE=generate`     — instrument; clang/gcc write .profraw/.gcda to
                                wherever `LLVM_PROFILE_FILE`/`GCOV_PREFIX`
                                point, MSVC writes .pgc beside the .pgd named
                                by `STRATA_PGO_PROFILE`.
    - `PGO_MODE=use`          — optimize against `STRATA_PGO_PROFILE`
                                (clang .profdata / gcc .gcda tree / MSVC .pgd).
    """
    mode = os.environ.get("PGO_MODE", "").strip().lower()
    lto = os.environ.get("STRATA_ENABLE_LTO", "0").strip() == "1"
    if not mode and not lto:
        return [], []

    if mode not in ("", "generate", "use"):
        raise SystemExit(f"PGO_MODE must be 'generate' or 'use', not {mode!r}.")

    kind = _compiler_kind()
    if kind == "msvc":
        return _msvc_optimization_args(mode)
    if kind == "clang-cl":
        return _clang_cl_optimization_args(mode, lto)

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
        if (
            kind == "clang"
            and sys.platform.startswith("linux")
            and platform.machine() in ("x86_64", "AMD64")
        ):
            # Machine function splitting (ELF x86-64 only): the profile moves
            # each function's cold blocks out of the hot text. The benchmark
            # harness interleaves five JSON engines per round, and x86 cores
            # carry a fraction of Apple-silicon's L1I — the serializer reads
            # at parity isolated and behind interleaved on exactly the legs
            # where instruction footprint has to refault per call.
            compile_args.append("-fsplit-machine-functions")
            link_args.append("-fsplit-machine-functions")

    return compile_args, link_args


def _compile_args() -> list[str]:
    if sys.platform == "win32":
        # /Zc:__cplusplus: MSVC otherwise reports __cplusplus as 199711L and
        # the C++20 guards in the headers misfire.
        args = ["/std:c++20", "/O2", "/Zc:__cplusplus"]
        if platform.machine() in ("AMD64", "x86_64"):
            args.append("/arch:AVX2")
        if _compiler_kind() == "clang-cl":
            # clang-cl's /O2 is clang's -O2; the POSIX legs build -O3, and a
            # review measured the parser's dispatcher paying 13% on the bool
            # path at -O2 through code layout alone (nothing at -O3). The
            # later flag wins, so the measured Windows build is the same
            # optimisation level as the others.
            args.append("/clang:-O3")
        args.extend(_optimization_args()[0])
        return args
    args = ["-std=c++20", "-O3", "-D_LIBCPP_DISABLE_AVAILABILITY"]
    # -march=native tunes for the build host; a universal2 wheel targets two
    # architectures at once and cannot use it. STRATA_MARCH names a target
    # explicitly (e.g. x86-64-v3 for a build a cache simulator can run: the
    # hosted x86 runners' native set includes AVX-512, which valgrind does
    # not emulate); the caller owns its validity for the host.
    march = os.environ.get("STRATA_MARCH", "").strip()
    if march:
        args.append(f"-march={march}")
    elif not _is_universal_build():
        args.append("-march=native")
    args.extend(_optimization_args()[0])
    return args


def _link_args() -> list[str]:
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

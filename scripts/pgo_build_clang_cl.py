#!/usr/bin/env python3
"""Two-phase clang-cl PGO build on Windows — the clang twin of scripts/pgo_build.sh.

  phase 1  instrumented build (clang-cl, /clang:-fprofile-generate, the
           profile runtime on the MSVC link line) -> training data
           -> training workload -> gate tests -> llvm-profdata merge
  phase 2  rebuild against the merged profile (/clang:-fprofile-use)
           -> gate tests -> verification benchmarks

Both phases run the full gate, as the POSIX and MSVC scripts do: an
optimized build that fails its tests is worth nothing, and PGO is exactly
the kind of change that can miscompile. The profile is regenerated from
scratch every run.

Why clang-cl: measured on one commit with three toolchains, MSVC compiles
the serializer's record and float paths 20-30% slower than clang-cl, which
reads them at parity with the LLVM-built rivals (docs/decisions.md,
2026-09-03); the string and int paths are ahead under both. The benchmark
leg measures this build; MSVC remains a tested compiler in the CI matrix
(scripts/pgo_build_msvc.py stays its PGO twin).

Differences from the MSVC script that are clang facts, not choices:

- IR-level instrumentation, as on POSIX: the runtime writes .profraw files
  wherever LLVM_PROFILE_FILE points when the interpreter exits, and
  llvm-profdata merges them; setup.py spells the flags behind /clang:.
- No LTO yet: clang-cl's bitcode objects need lld-link, which setuptools
  does not drive; a plain clang-cl build already measured ahead of MSVC's
  PGO+LTCG on the serializer.

Run it under the interpreter that should receive the build. clang-cl,
clang and llvm-profdata come from the LLVM install (the hosted Windows
runners ship it under C:\\Program Files\\LLVM).
"""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
PGO_DIR = PROJECT_ROOT / os.environ.get("PGO_DIR", "build/pgo")
RAW_DIR = PGO_DIR / "raw"
WORK_DIR = PGO_DIR / "work"
PROFILE = PGO_DIR / "strata.profdata"
BENCH_DATA = PROJECT_ROOT / "benchmarks" / "data" / "generated" / "small"
BENCH_REPEAT = os.environ.get("PGO_BENCH_REPEAT", "10")
BENCH_WARMUP = os.environ.get("PGO_BENCH_WARMUP", "2")
LLVM_BIN = Path(r"C:\Program Files\LLVM\bin")


def _run(cmd: list[str], extra_env: dict[str, str] | None = None) -> None:
    env = os.environ.copy()
    if extra_env:
        env.update(extra_env)
    print("+ " + " ".join(cmd), flush=True)
    completed = subprocess.run(cmd, cwd=PROJECT_ROOT, check=False, env=env)
    if completed.returncode != 0:
        raise SystemExit(f"command failed ({completed.returncode}): {cmd[0]}")


def _tool(name: str) -> str:
    found = shutil.which(name) or shutil.which(name, path=str(LLVM_BIN))
    if not found:
        raise SystemExit(f"{name}.exe not found on PATH or under {LLVM_BIN}; install LLVM.")
    return found


def _gate_tests() -> None:
    _run([sys.executable, "scripts/cpp_tests.py"])
    _run([sys.executable, "scripts/py_tests.py"])


def _install(mode: str, extra_env: dict[str, str]) -> None:
    # An editable install builds in temporary directories of its own, so
    # this is housekeeping for any persisted build tree a previous
    # `setup.py build_ext` left behind, not a correctness need.
    for stale in PROJECT_ROOT.glob("build/lib.*"):
        shutil.rmtree(stale)
    for stale in PROJECT_ROOT.glob("build/temp.*"):
        shutil.rmtree(stale)
    env = {"STRATA_WIN_COMPILER": "clang-cl", "PGO_MODE": mode, "STRATA_ENABLE_LTO": "0"}
    env.update(extra_env)
    # --no-deps: the dependency resolver would otherwise reinstall unrelated
    # packages between the two phases and muddy the comparison.
    _run(
        [sys.executable, "-m", "pip", "install", "--force-reinstall", "--no-deps", "-e", "."],
        extra_env=env,
    )


def _extension_dir() -> Path | None:
    probe = subprocess.run(
        [sys.executable, "-c", "import strata._strata as m; print(m.__file__)"],
        capture_output=True,
        text=True,
        check=False,
    )
    if probe.returncode != 0:
        return None
    return Path(probe.stdout.strip()).resolve().parent


def _found_profraw() -> list[Path]:
    """Every raw profile the instrumented runs wrote, wherever it fell.

    They belong in RAW_DIR, where LLVM_PROFILE_FILE points; a run that never
    saw that variable writes `default*.profraw` into its working directory
    instead, and the extension's own directory is the other plausible home.
    Searched everywhere so a misrouted profile is diagnosed, not missed.
    """
    seen: dict[Path, Path] = {}
    for raw in PROJECT_ROOT.rglob("*.profraw"):
        seen[raw.resolve()] = raw
    module_dir = _extension_dir()
    if module_dir is not None:
        for raw in module_dir.glob("*.profraw"):
            seen[raw.resolve()] = raw
    return sorted(seen)


def _collect_profraw() -> list[Path]:
    """Gather the training profiles into RAW_DIR, or fail with a diagnosis."""
    found = _found_profraw()
    if not found:
        raise SystemExit(
            "No .profraw files were written anywhere -- either the link never pulled "
            "the profile runtime (the build was not instrumented) or the runtime never "
            "flushed at exit. Searched the project tree and the extension's directory.",
        )
    moved = 0
    for raw in found:
        if raw.parent.resolve() != RAW_DIR.resolve():
            shutil.move(str(raw), RAW_DIR / raw.name)
            moved += 1
    if moved:
        print(
            f"    {moved} profile(s) landed outside {RAW_DIR} (LLVM_PROFILE_FILE did not "
            "reach that process); moved beside the others",
            flush=True,
        )
    return sorted(RAW_DIR.glob("*.profraw"))


def _assert_not_instrumented() -> None:
    """Prove phase 2 swapped the build: an instrumented import writes a .profraw."""
    before = len(_found_profraw())
    _run(
        [sys.executable, "-c", "import strata; strata.loads('{}')"],
        extra_env={"LLVM_PROFILE_FILE": str(RAW_DIR / "probe-%p.profraw")},
    )
    if len(_found_profraw()) != before:
        raise SystemExit(
            "The phase-2 build still writes .profraw profiles -- the -fprofile-use "
            "rebuild did not replace the instrumented extension.",
        )


def main() -> int:
    if sys.platform != "win32":
        raise SystemExit("This script drives clang-cl on Windows; on POSIX use make pgo.")
    _tool("clang-cl")
    profdata = _tool("llvm-profdata")

    if PGO_DIR.exists():
        shutil.rmtree(PGO_DIR)
    RAW_DIR.mkdir(parents=True)
    WORK_DIR.mkdir(parents=True)

    print("==> PGO phase 1: instrumented build (clang-cl, -fprofile-generate)", flush=True)
    # %p keeps concurrent processes from clobbering one profile file; one
    # file per process is all the merge needs, so no in-place merge pool.
    profile_env = {"LLVM_PROFILE_FILE": str(RAW_DIR / "%p.profraw")}
    _install("generate", profile_env)

    print("==> PGO: generating training data", flush=True)
    _run([sys.executable, "scripts/pgo_training_data.py", "--out-dir", str(PGO_DIR)])

    print("==> PGO: running the training workload", flush=True)
    _run(
        [
            sys.executable,
            "scripts/pgo_training.py",
            "--json",
            str(PGO_DIR / "train.json"),
            "--ndjson",
            str(PGO_DIR / "train.ndjson"),
            "--work-dir",
            str(WORK_DIR),
        ],
        extra_env={"PYTHONPATH": ".", **profile_env},
    )

    print("==> PGO: gate tests on the instrumented build", flush=True)
    for script in ("scripts/cpp_tests.py", "scripts/py_tests.py"):
        _run([sys.executable, script], extra_env=profile_env)

    raw = _collect_profraw()
    print(f"==> PGO: merging {len(raw)} raw profiles", flush=True)
    _run([profdata, "merge", f"-output={PROFILE}", *map(str, raw)])

    print("==> PGO phase 2: optimized build (clang-cl, -fprofile-use)", flush=True)
    _install("use", {"STRATA_PGO_PROFILE": str(PROFILE)})
    _assert_not_instrumented()

    print("==> PGO: gate tests on the optimized build", flush=True)
    _gate_tests()

    if not BENCH_DATA.is_dir():
        print("==> PGO: generating benchmark data", flush=True)
        _run(
            [
                sys.executable,
                "-m",
                "benchmarks.data.generate_bench_data",
                "--out-dir",
                str(BENCH_DATA),
                "--num-users",
                "1000",
                "--max-orders",
                "10",
                "--max-items",
                "5",
                "--records",
                "500",
            ],
            extra_env={"PYTHONPATH": "."},
        )

    print("==> PGO: verification benchmarks", flush=True)
    _run(
        [
            sys.executable,
            "-m",
            "benchmarks.bench_main",
            "--name",
            "pgo",
            "--repeat",
            BENCH_REPEAT,
            "--warmup",
            BENCH_WARMUP,
            "--dataset",
            str(BENCH_DATA / "users.json"),
            "--dataset",
            str(BENCH_DATA / "flat.json"),
            "--dataset",
            str(BENCH_DATA / "nested.json"),
            "--output",
            str(PGO_DIR / "bench_results_pgo.md"),
        ],
        extra_env={"PYTHONPATH": ".", "PGO_MODE": "use", "STRATA_WIN_COMPILER": "clang-cl"},
    )

    print("==> PGO complete", flush=True)
    print(f"    profile: {PROFILE}", flush=True)
    print(f"    results: {PGO_DIR / 'bench_results_pgo.md'}", flush=True)
    print("    The installed extension is now the clang-cl PGO build.", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

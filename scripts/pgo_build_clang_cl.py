#!/usr/bin/env python3
"""Two-phase clang-cl PGO build on Windows — the clang twin of scripts/pgo_build.sh.

  phase 1  instrumented build (clang-cl, /clang:-fprofile-generate, the
           profile runtime on the MSVC link line) -> training data
           -> training workload -> gate tests -> llvm-profdata merge
  phase 2  rebuild against the merged profile (/clang:-fprofile-use)
           -> gate tests
  phase 3  the hook image alone: instrumented -> native training workload
           -> its own profile -> rebuild against it -> gate tests; _strata's
           image is checked unchanged -> verification benchmarks

Both phases run the gate, as the POSIX and MSVC scripts do: an optimized
build that fails its tests is worth nothing, and PGO is exactly the kind of
change that can miscompile. Phase 1 runs it with ``--training``, which leaves
the native-type suites out of the profile; phase 2 runs all of it. The
profile is regenerated from scratch every run.

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

import hashlib
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
HOOK_RAW_DIR = PGO_DIR / "hook-raw"
HOOK_GATE_DIR = PGO_DIR / "hook-gate-discarded"
HOOK_PROFILE = PGO_DIR / "hook.profdata"
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


def _sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _strata_image() -> Path:
    probe = subprocess.run(
        [
            sys.executable,
            "-c",
            "import importlib.util as u; print(u.find_spec('strata._strata').origin)",
        ],
        capture_output=True,
        text=True,
        check=True,
        cwd=PROJECT_ROOT,
    )
    return Path(probe.stdout.strip()).resolve()


def _install_hook(mode: str, extra_env: dict[str, str]) -> None:
    """Rebuild strata._dumps_hook alone (STRATA_EXTENSIONS), in @p mode for its own profile."""
    env = {
        "STRATA_WIN_COMPILER": "clang-cl",
        "PGO_MODE": "",
        "STRATA_ENABLE_LTO": "0",
        "STRATA_EXTENSIONS": "strata._dumps_hook",
        "STRATA_HOOK_PGO_MODE": mode,
    }
    env.update(extra_env)
    _run(
        [sys.executable, "-m", "pip", "install", "--force-reinstall", "--no-deps", "-e", "."],
        extra_env=env,
    )


def _hook_phase(profdata: str) -> None:
    """Phase 3: the hook image's own profile (docs/architecture/native_types.md, "Hook profile").

    The hook is instrumented alone while `_strata` is the phase-2 image,
    trained by scripts/pgo_hook_training.py (the build gate's counts go to a
    directory that is thrown away), and rebuilt against that profile; no LTO
    under clang-cl. `_strata`'s image must come out byte for byte unchanged.
    """
    image = _strata_image()
    before = _sha256(image)
    HOOK_RAW_DIR.mkdir(parents=True)
    HOOK_GATE_DIR.mkdir(parents=True)
    print("==> PGO phase 3a: instrumented hook", flush=True)
    _install_hook("generate", {"LLVM_PROFILE_FILE": str(HOOK_GATE_DIR / "%p.profraw")})
    print("==> PGO: the hook's native training workload", flush=True)
    _run(
        [sys.executable, "scripts/pgo_hook_training.py", "--work-dir", str(WORK_DIR)],
        extra_env={"PYTHONPATH": ".", "LLVM_PROFILE_FILE": str(HOOK_RAW_DIR / "%p.profraw")},
    )
    raw = sorted(HOOK_RAW_DIR.glob("*.profraw"))
    if not raw:
        raise SystemExit("The hook's training wrote no .profraw -- it was not instrumented.")
    _run([profdata, "merge", f"-output={HOOK_PROFILE}", *map(str, raw)])
    _run(
        [
            sys.executable,
            "scripts/build_identity.py",
            "--profile",
            str(HOOK_PROFILE),
            "--raw",
            str(HOOK_RAW_DIR),
            "--recipe",
            "hook-native-training-clang-cl-v1",
        ]
    )
    print("==> PGO phase 3b: the hook against its own profile", flush=True)
    _install_hook("use", {"STRATA_HOOK_PGO_PROFILE": str(HOOK_PROFILE)})
    if _sha256(image) != before:
        raise SystemExit(f"The hook phase changed _strata's image ({image}).")
    for module, profile, foreign in (
        ("strata._dumps_hook", HOOK_PROFILE, PROFILE),
        ("strata._strata", PROFILE, HOOK_PROFILE),
    ):
        _run(
            [
                sys.executable,
                "scripts/build_identity.py",
                "--check-profiled",
                module,
                "--profile",
                str(profile),
                "--foreign",
                str(foreign),
            ]
        )
    print("==> PGO: gate tests on the optimized hook", flush=True)
    _gate_tests()


def _assert_hook_unprofiled(*control: str) -> None:
    """setup.py builds strata._dumps_hook unprofiled: nothing it runs may enter the profile.

    Its raw profiles would not be told apart here (LLVM_PROFILE_FILE names
    files by %p only), so the check reads the hook's build identity and image.
    """
    _run(
        [
            sys.executable,
            "scripts/build_identity.py",
            "--check-unprofiled",
            "strata._dumps_hook",
            *control,
        ]
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
    _assert_hook_unprofiled("--instrumented", "strata._strata")

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
    _run([sys.executable, "scripts/cpp_tests.py"], extra_env=profile_env)
    _run([sys.executable, "scripts/py_tests.py", "--training"], extra_env=profile_env)

    raw = _collect_profraw()
    print(f"==> PGO: merging {len(raw)} raw profiles", flush=True)
    _run([profdata, "merge", f"-output={PROFILE}", *map(str, raw)])
    _run(
        [
            sys.executable,
            "scripts/build_identity.py",
            "--profile",
            str(PROFILE),
            "--raw",
            str(RAW_DIR),
            "--recipe",
            "gate-inclusive-clang-cl-v1",
        ]
    )

    print("==> PGO phase 2: optimized build (clang-cl, -fprofile-use)", flush=True)
    _install("use", {"STRATA_PGO_PROFILE": str(PROFILE)})
    _assert_not_instrumented()
    _assert_hook_unprofiled()

    print("==> PGO: gate tests on the optimized build", flush=True)
    _gate_tests()

    _hook_phase(profdata)

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

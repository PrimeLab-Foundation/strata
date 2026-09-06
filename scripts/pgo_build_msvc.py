#!/usr/bin/env python3
"""Two-phase MSVC PGO + LTCG build — the Windows twin of scripts/pgo_build.sh.

  phase 1  instrumented build (/GL, /LTCG /GENPROFILE) -> gate tests
           -> set the gate's counters aside -> training data
           -> training workload
  phase 2  relink against the profile (/LTCG /USEPROFILE) -> gate tests
           -> verification benchmarks

Both phases run the full gate for the same reason the POSIX script does: an
optimized build that fails its tests is worth nothing, and PGO is exactly the
kind of change that can miscompile. The profile is regenerated from scratch
every run, and — as in pgo_build.sh — the gate's own counters are kept out of
it: the merged profile is the training workload alone.

Differences from the POSIX script that are MSVC facts, not choices:

- PGO rides on LTCG: both phases compile /GL and link /LTCG; there is no
  LTO-off instrumented phase.
- No llvm-profdata step: the training runs drop ``strata!N.pgc`` beside the
  .pgd named by ``STRATA_PGO_PROFILE``, and the /USEPROFILE link merges them
  on its own.
- The .pgd path is baked into the instrumented binary at link time and there
  is no per-process redirect like LLVM_PROFILE_FILE, so the gate's counters
  cannot be *routed* elsewhere. They are separated by order instead: the
  gate runs first, its .pgc files are moved into a junk directory, and the
  training workload is the last instrumented thing phase 1 does.
- The instrumented extension depends on pgort140.dll, and Python 3.8+ does
  not consult PATH when resolving extension-module dependencies — so the DLL
  is staged beside python.exe, whose directory the loader always searches.

Run it under the interpreter that should receive the build. No venv and no
make are assumed: the CI benchmark leg has neither.
"""

from __future__ import annotations

import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
PGO_DIR = PROJECT_ROOT / os.environ.get("PGO_DIR", "build/pgo")
# Counters written by anything other than the training workload land here and
# are never merged. Kept rather than discarded so a run can be audited.
JUNK_DIR = PGO_DIR / "junk"
WORK_DIR = PGO_DIR / "work"
PGD_FILE = PGO_DIR / "strata.pgd"
BENCH_DATA = PROJECT_ROOT / "benchmarks" / "data" / "generated" / "small"
BENCH_REPEAT = os.environ.get("PGO_BENCH_REPEAT", "10")
BENCH_WARMUP = os.environ.get("PGO_BENCH_WARMUP", "2")


def _run(cmd: list[str], extra_env: dict[str, str] | None = None) -> None:
    print("+ " + " ".join(cmd), flush=True)
    env = None
    if extra_env is not None:
        env = os.environ.copy()
        env.update(extra_env)
    completed = subprocess.run(cmd, cwd=PROJECT_ROOT, check=False, env=env)
    if completed.returncode != 0:
        raise SystemExit(completed.returncode)


def _toolset_version(dll: Path) -> tuple[int, ...]:
    """The MSVC toolset version encoded in ``.../VC/Tools/MSVC/<ver>/...``."""
    for part in dll.parts:
        if re.fullmatch(r"\d+(\.\d+)+", part):
            return tuple(int(piece) for piece in part.split("."))
    return (0,)


def _find_pgort() -> Path:
    """Locate pgort140.dll in the newest installed MSVC toolset.

    vswhere is authoritative when present (it ships with the VS installer);
    a filesystem glob over the standard install roots is the fallback.
    """
    pattern = r"VC\Tools\MSVC\**\bin\Hostx64\x64\pgort140.dll"
    candidates: list[Path] = []
    program_files = os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")
    vswhere = Path(program_files) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
    if vswhere.exists():
        found = subprocess.run(
            [str(vswhere), "-latest", "-products", "*", "-find", pattern],
            capture_output=True,
            text=True,
            check=False,
        )
        candidates = [Path(line.strip()) for line in found.stdout.splitlines() if line.strip()]
    if not candidates:
        for root in (r"C:\Program Files", r"C:\Program Files (x86)"):
            base = Path(root) / "Microsoft Visual Studio"
            if base.is_dir():
                candidates.extend(base.glob(r"*/*/" + pattern.replace("\\", "/")))
    if not candidates:
        raise SystemExit(
            "pgort140.dll not found — MSVC PGO needs a Visual Studio C++ toolset. "
            "Install the 'Desktop development with C++' workload and re-run.",
        )
    return max(candidates, key=_toolset_version)


def _stage_pgort() -> None:
    """Copy the PGO runtime beside python.exe so instrumented imports resolve."""
    dll = _find_pgort()
    target = Path(sys.executable).resolve().parent / dll.name
    print(f"==> PGO: staging {dll} -> {target}", flush=True)
    shutil.copy2(dll, target)


def _gate_tests() -> None:
    _run([sys.executable, "scripts/cpp_tests.py"])
    _run([sys.executable, "scripts/py_tests.py"])


def _install(mode: str) -> None:
    # Both phases compile identical /GL objects — only the link line differs —
    # so a persisted setuptools build tree could satisfy the up-to-date check
    # and skip the phase-2 relink. Clean it to force the link to happen.
    for stale in PROJECT_ROOT.glob("build/lib.*"):
        shutil.rmtree(stale)
    for stale in PROJECT_ROOT.glob("build/temp.*"):
        shutil.rmtree(stale)
    # --no-deps: the dependency resolver would otherwise reinstall unrelated
    # packages between the two phases and muddy the comparison.
    _run(
        [sys.executable, "-m", "pip", "install", "--force-reinstall", "--no-deps", "-e", "."],
        extra_env={
            "PGO_MODE": mode,
            "STRATA_ENABLE_LTO": "1",
            "STRATA_PGO_PROFILE": str(PGD_FILE),
        },
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


def _found_pgc() -> list[Path]:
    """Every count file the instrumented runs dropped, wherever it fell.

    The PGO runtime names them `<pgd-base>!N.pgc`; the documentation says
    they land beside the .pgd, but in practice they can appear beside the
    instrumented binary or in the process working directory depending on the
    toolset. Search every plausible home; phase 2 consumes them from the
    .pgd's directory.

    JUNK_DIR is the one place deliberately excluded: the gate's counters were
    moved there to keep them out of the /USEPROFILE merge.
    """
    junk = JUNK_DIR.resolve()
    seen: dict[Path, Path] = {}
    for pgc in PROJECT_ROOT.rglob("strata!*.pgc"):
        resolved = pgc.resolve()
        if junk in resolved.parents:
            continue
        seen[resolved] = pgc
    module_dir = _extension_dir()
    if module_dir is not None:
        for pgc in module_dir.glob("strata!*.pgc"):
            seen[pgc.resolve()] = pgc
    return sorted(seen)


def _junk_pgc(what: str) -> None:
    """Move every .pgc written so far out of the way of the merge.

    The instrumented extension always writes beside the .pgd, so the gate's
    counters cannot be routed elsewhere; they are separated by running the
    gate first and setting its files aside before the training workload runs.
    """
    JUNK_DIR.mkdir(parents=True, exist_ok=True)
    moved = 0
    for pgc in _found_pgc():
        # Renamed off the `strata!*.pgc` pattern as well as moved: neither the
        # /USEPROFILE merge nor _found_pgc can pick a file up by accident.
        shutil.move(str(pgc), JUNK_DIR / f"gate-{moved}-{pgc.name}")
        moved += 1
    print(f"    {moved} .pgc file(s) from {what} set aside in {JUNK_DIR}; not merged", flush=True)


def _collect_pgc() -> None:
    """Gather the training profiles beside the .pgd, or fail with a diagnosis."""
    found = _found_pgc()
    moved = 0
    for pgc in found:
        if pgc.parent != PGO_DIR:
            shutil.move(str(pgc), PGO_DIR / pgc.name)
            moved += 1
    if not found:
        print(f"    strata.pgd exists: {PGD_FILE.exists()}", flush=True)
        for base in (PGO_DIR, PROJECT_ROOT):
            names = sorted(entry.name for entry in base.glob("*"))[:40]
            print(f"    {base}: {names}", flush=True)
        if not PGD_FILE.exists():
            raise SystemExit(
                "No .pgc profiles and no .pgd — /GENPROFILE never reached the "
                "link; the build was not instrumented.",
            )
        raise SystemExit(
            "No .pgc profiles found anywhere despite an instrumented link "
            "(strata.pgd exists) — the PGO runtime did not flush.",
        )
    print(f"    {len(found)} profile(s), {moved} moved beside the pgd", flush=True)


def _assert_not_instrumented() -> None:
    """Prove phase 2 swapped the build: an instrumented import writes a .pgc."""
    before = len(_found_pgc())
    _run([sys.executable, "-c", "import strata; strata.loads('{}')"])
    if len(_found_pgc()) != before:
        raise SystemExit(
            "The phase-2 build still writes .pgc profiles — the /USEPROFILE "
            "relink did not replace the instrumented extension.",
        )


def main() -> int:
    if sys.platform != "win32":
        raise SystemExit("This script drives MSVC; on POSIX use scripts/pgo_build.sh (make pgo).")

    _stage_pgort()

    if PGO_DIR.exists():
        shutil.rmtree(PGO_DIR)
    JUNK_DIR.mkdir(parents=True)
    WORK_DIR.mkdir(parents=True)

    print("==> PGO phase 1: instrumented build (/GL /LTCG /GENPROFILE)", flush=True)
    _install("generate")

    # The gate runs before the training workload here, unlike the POSIX and
    # clang-cl scripts: their counters cannot be redirected per process, so
    # they are separated by order and set aside below. A test suite's counts
    # are not production behaviour (on the POSIX recipe they were 47.5% of the
    # merged counts and 99.7% of dumps_to_python's), and the merged profile is
    # the training workload alone on every platform.
    print("==> PGO: gate tests on the instrumented build", flush=True)
    _gate_tests()

    print("==> PGO: setting the gate's counters aside", flush=True)
    _junk_pgc("the instrumented build's gate")

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
        extra_env={"PYTHONPATH": "."},
    )

    print("==> PGO: collecting .pgc profiles", flush=True)
    _collect_pgc()

    print("==> PGO phase 2: optimized build (/LTCG /USEPROFILE)", flush=True)
    _install("use")
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
        extra_env={"PYTHONPATH": ".", "PGO_MODE": "use", "STRATA_ENABLE_LTO": "1"},
    )

    print("==> PGO complete", flush=True)
    print(f"    profile: {PGD_FILE}", flush=True)
    print(f"    results: {PGO_DIR / 'bench_results_pgo.md'}", flush=True)
    print("    The installed extension is now the PGO+LTCG build.", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

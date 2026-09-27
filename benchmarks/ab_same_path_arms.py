"""Build the A/B arms of M12b's criterion 5 in ONE absolute path, one after another.

Usage (from the checkout that will run the timing):
  python benchmarks/ab_same_path_arms.py --base <ref> --arm-dir <absolute path>

Four PGO builds, each in a fresh git worktree at the same `--arm-dir`, with its
own virtualenv inside it, removed before the next:

  A       the base ref, its own training
  A2      the base ref again, its own training   -- the build-noise control
  B       this checkout's HEAD, its own training
  B_held  HEAD's source rebuilt against A's profile, placed at the very path A's
          own build read it from                 -- the identity proof

Why one path: run 36279771980 stopped linux-arm64 because ThinLTO names each
promoted local with a module hash (`.llvm.<N>`) that moves with the build
directory, and that leg's link order follows it (docs/decisions.md, 2026-09-27).
Builds sharing a path leave the sources and the profile as the only differences.

Outputs: `arms/{A,A2,B,B_held}.<ext>` (+ `.build.json`), `ab/profile-<label>/`,
`ab/build-<label>.log`, `ab/arms.txt`, and the identity diagnostics
`ab/identity*.{txt,json}` and `ab/identity-status.txt`. Nothing here fails the job
on identity: an image that differs is recorded (with the normalised disassembly)
and marked unverified, and the leg is still timed. A build that fails does fail.
"""

from __future__ import annotations

import argparse
import hashlib
import os
import shutil
import subprocess
import sys
from pathlib import Path

WINDOWS = sys.platform == "win32"
EXT = ".pyd" if WINDOWS else ".so"
LLVM_BIN = r"C:\Program Files\LLVM\bin"


def run(cmd, *, cwd=None, env=None, log=None):
    print("+ " + " ".join(map(str, cmd)), flush=True)
    merged = dict(os.environ, **(env or {}))
    if log is None:
        subprocess.run(cmd, cwd=cwd, env=merged, check=True)
        return
    with open(log, "a", encoding="utf-8") as fh:
        completed = subprocess.run(cmd, cwd=cwd, env=merged, stdout=fh, stderr=subprocess.STDOUT)
    if completed.returncode != 0:
        tail = Path(log).read_text(encoding="utf-8", errors="replace").splitlines()[-60:]
        print("\n".join(tail))
        raise SystemExit(f"failed ({completed.returncode}): {' '.join(map(str, cmd))}")


def venv_python(arm_dir: Path) -> Path:
    return arm_dir / ".venv" / ("Scripts/python.exe" if WINDOWS else "bin/python")


def build_env(arm_dir: Path) -> dict[str, str]:
    if not WINDOWS:
        return {}
    scripts = str(arm_dir / ".venv" / "Scripts")
    return {"PATH": os.pathsep.join([scripts, LLVM_BIN, os.environ.get("PATH", "")])}


def pgo_build(arm_dir: Path, log: Path, constraints: Path | None) -> None:
    if WINDOWS:
        run([sys.executable, "-m", "venv", ".venv"], cwd=arm_dir, log=log)
        vpy = venv_python(arm_dir)
        run([vpy, "-m", "pip", "install", "-U", "pip", "setuptools", "wheel"], cwd=arm_dir, log=log)
        extra = ["-c", str(constraints)] if constraints else []
        run([vpy, "-m", "pip", "install", *extra, "-e", ".[dev,bench]"], cwd=arm_dir, log=log)
        run([vpy, "scripts/pgo_build_clang_cl.py"], cwd=arm_dir, env=build_env(arm_dir), log=log)
    else:
        run(["make", "install-bench"], cwd=arm_dir, log=log)
        run(["make", "pgo"], cwd=arm_dir, log=log)


def held_profile_rebuild(arm_dir: Path, profile: Path, log: Path) -> None:
    """Phase 2 again, against @p profile copied to the path phase 2 reads."""
    target = arm_dir / "build" / "pgo" / "strata.profdata"
    shutil.copy2(profile, target)
    for stale in [*arm_dir.glob("build/lib.*"), *arm_dir.glob("build/temp.*")]:
        shutil.rmtree(stale)
    env = {"PGO_MODE": "use", "STRATA_PGO_PROFILE": str(target)}
    env.update(
        {"STRATA_WIN_COMPILER": "clang-cl", "STRATA_ENABLE_LTO": "0"}
        if WINDOWS
        else {"STRATA_ENABLE_LTO": "1"}
    )
    env.update(build_env(arm_dir))
    vpy = venv_python(arm_dir)
    run(
        [vpy, "-m", "pip", "install", "--force-reinstall", "--no-deps", "-e", "."],
        cwd=arm_dir,
        env=env,
        log=log,
    )


def collect(label: str, arm_dir: Path) -> None:
    vpy = venv_python(arm_dir)
    binary = Path(
        subprocess.run(
            [vpy, "-c", "import strata._strata as m; print(m.__file__)"],
            cwd=arm_dir,
            check=True,
            capture_output=True,
            text=True,
        ).stdout.strip()
    )
    shutil.copy2(binary, f"arms/{label}{EXT}")
    identity = Path(str(binary) + ".build.json")
    if identity.is_file():
        shutil.copy2(identity, f"arms/{label}{EXT}.build.json")
    digest = hashlib.md5(Path(f"arms/{label}{EXT}").read_bytes()).hexdigest()
    head = subprocess.run(
        ["git", "-C", str(arm_dir), "rev-parse", "--short", "HEAD"],
        check=True,
        capture_output=True,
        text=True,
    ).stdout.strip()
    with open("ab/arms.txt", "a", encoding="utf-8") as fh:
        fh.write(f"{label} {head} {digest}  arms/{label}{EXT}\n")


def keep_profile(label: str, arm_dir: Path) -> Path:
    destination = Path("ab") / f"profile-{label}"
    destination.mkdir(parents=True, exist_ok=True)
    profile = destination / "strata.profdata"
    shutil.copy2(arm_dir / "build" / "pgo" / "strata.profdata", profile)
    return profile


def worktree(ref: str, arm_dir: Path) -> None:
    if arm_dir.exists():
        subprocess.run(["git", "worktree", "remove", "--force", str(arm_dir)], check=False)
        shutil.rmtree(arm_dir, ignore_errors=True)
        subprocess.run(["git", "worktree", "prune"], check=True)
    run(["git", "worktree", "add", "--force", str(arm_dir), ref])


def identity(a: str, b: str, stem: str) -> int:
    out = Path(f"ab/{stem}.txt")
    completed = subprocess.run(
        [
            sys.executable,
            "benchmarks/image_identity.py",
            f"arms/{a}{EXT}",
            f"arms/{b}{EXT}",
            "--json",
            f"ab/{stem}.json",
        ],
        capture_output=True,
        text=True,
    )
    out.write_text(completed.stdout + completed.stderr, encoding="utf-8")
    return completed.returncode


def normalised(a: str, b: str, stem: str) -> None:
    completed = subprocess.run(
        [
            sys.executable,
            "benchmarks/normalised_disassembly.py",
            f"arms/{a}{EXT}",
            f"arms/{b}{EXT}",
        ],
        capture_output=True,
        text=True,
    )
    Path(f"ab/{stem}.txt").write_text(completed.stdout + completed.stderr, encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", required=True)
    parser.add_argument("--arm-dir", required=True, type=Path)
    parser.add_argument("--constraints", type=Path, default=None)
    args = parser.parse_args()
    arm_dir = args.arm_dir.resolve()
    # The builds run with the worktree as their working directory: a relative
    # constraints path would name a file there (run 36291977906, windows).
    constraints = args.constraints.resolve() if args.constraints else None
    candidate = subprocess.run(
        ["git", "rev-parse", "HEAD"], check=True, capture_output=True, text=True
    ).stdout.strip()
    Path("arms").mkdir(exist_ok=True)
    Path("ab").mkdir(exist_ok=True)

    profiles: dict[str, Path] = {}
    for label, ref in (("A", args.base), ("A2", args.base), ("B", candidate)):
        worktree(ref, arm_dir)
        log = Path(f"ab/build-{label}.log").resolve()
        pgo_build(arm_dir, log, constraints)
        collect(label, arm_dir)
        profiles[label] = keep_profile(label, arm_dir)
        if label == "B":
            held_profile_rebuild(arm_dir, profiles["A"], Path("ab/build-B_held.log").resolve())
            collect("B_held", arm_dir)
    subprocess.run(["git", "worktree", "remove", "--force", str(arm_dir)], check=False)

    held = identity("A", "B_held", "identity")
    identity("A", "B", "identity-own-profiles")
    identity("A", "A2", "identity-A2")
    normalised("A", "B_held", "identity-normalised")
    status = {
        0: "verified: A's and B_held's code sections are byte-identical",
        1: "UNVERIFIED: code sections differ; see identity-normalised.txt",
        2: "UNVERIFIED: an image could not be read",
    }.get(held, f"UNVERIFIED: exit {held}")
    Path("ab/identity-status.txt").write_text(status + "\n", encoding="utf-8")
    print(f"identity: {status}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

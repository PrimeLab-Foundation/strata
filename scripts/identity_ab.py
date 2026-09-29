"""Same-absolute-path A/B build identity check (M15b acceptance item 4).

Two comparisons, both in the SAME absolute worktree path (ThinLTO's
promoted-name suffixes and the profile follow the build directory --
docs/decisions.md, 2026-09-27):

  plain    `--base <ref>` built plain (no PGO), then HEAD built plain, same path.
  held     `--base <ref>` built with a freshly trained `make pgo` -> image A and
           its profile; HEAD then rebuilt in the same path against A's profile
           only (PGO_MODE=use, no retraining) -> image B_held. This is the
           held-profile recipe from benchmarks/ab_same_path_arms.py's
           `held_profile_rebuild` on exp/m15-ab-arm (Windows: the same
           `PGO_MODE=use` install scripts/pgo_build_clang_cl.py's own phase 2
           performs, without its phase-1 training).

Each comparison's code section is checked with `benchmarks/image_identity.py`.
Both gate the exit code (docs/architecture/native_types.md, "Flag shape
(M15b)" -> Acceptance, criterion 4): a code-section difference (exit 1), an
image `image_identity` cannot read (exit 2), a missing image, or two paths
that resolve to the same file all fail the check -- none of those states is
evidence that the code is identical.

Usage:
    python scripts/identity_ab.py --base <ref> --arm-dir <absolute path> --out <dir>
    python scripts/identity_ab.py --base <ref> --arm-dir <absolute path> --out <dir> --dry-run
"""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

WINDOWS = sys.platform == "win32"
EXT = ".pyd" if WINDOWS else ".so"
LLVM_BIN = r"C:\Program Files\LLVM\bin"

PROJECT_ROOT = Path(__file__).resolve().parents[1]


def run(
    cmd: list, *, cwd: Path | None = None, env: dict[str, str] | None = None, dry_run: bool = False
) -> None:
    print("+ " + " ".join(map(str, cmd)), flush=True)
    if dry_run:
        return
    merged = dict(os.environ, **env) if env else None
    subprocess.run(cmd, cwd=cwd, env=merged, check=True)


def venv_python(root: Path) -> Path:
    return root / ".venv" / ("Scripts/python.exe" if WINDOWS else "bin/python")


def find_extension(root: Path) -> Path:
    matches = sorted(root.glob(f"python/strata/_strata*{EXT}")) or sorted(
        root.rglob(f"_strata*{EXT}")
    )
    if not matches:
        raise SystemExit(f"no _strata*{EXT} found under {root}")
    return matches[0]


def copy_ext(arm_dir: Path, dest: Path, *, dry_run: bool = False) -> Path:
    if dry_run:
        print(f"+ copy <built extension under {arm_dir}> -> {dest}", flush=True)
        return dest
    dest.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(find_extension(arm_dir), dest)
    return dest


def posix_env() -> dict[str, str]:
    """The leg's own compiler: Linux legs build with clang (matching the
    llvm-profdata that reads their .profraw files); macOS's system `cc`/`c++`
    is already clang, so only Linux needs the override."""
    return {"CC": "clang", "CXX": "clang++"} if sys.platform.startswith("linux") else {}


def build_env(arm_dir: Path) -> dict[str, str]:
    if not WINDOWS:
        return {}
    scripts = str(arm_dir / ".venv" / "Scripts")
    return {"PATH": os.pathsep.join([scripts, LLVM_BIN, os.environ.get("PATH", "")])}


def worktree_checkout(ref: str, arm_dir: Path, *, dry_run: bool = False) -> None:
    if arm_dir.exists():
        run(["git", "worktree", "remove", "--force", str(arm_dir)], dry_run=dry_run)
        if not dry_run:
            shutil.rmtree(arm_dir, ignore_errors=True)
        run(["git", "worktree", "prune"], dry_run=dry_run)
    run(["git", "worktree", "add", "--force", "--detach", str(arm_dir), ref], dry_run=dry_run)


def worktree_remove(arm_dir: Path, *, dry_run: bool = False) -> None:
    run(["git", "worktree", "remove", "--force", str(arm_dir)], dry_run=dry_run)


def create_windows_venv(arm_dir: Path, *, dry_run: bool = False) -> None:
    """Windows has no Makefile-driven venv: build the same one the Windows
    leg's own steps do, so this script's own venv/python path is the one the
    rest of it (and pgo_build_clang_cl.py) expects -- not the interpreter
    running identity_ab.py itself."""
    run([sys.executable, "-m", "venv", ".venv"], cwd=arm_dir, dry_run=dry_run)
    vpy = str(venv_python(arm_dir))
    run(
        [vpy, "-m", "pip", "install", "-U", "pip", "setuptools", "wheel"],
        cwd=arm_dir,
        dry_run=dry_run,
    )
    run(
        [vpy, "-m", "pip", "install", "-e", ".[dev,bench]"],
        cwd=arm_dir,
        env=build_env(arm_dir),
        dry_run=dry_run,
    )


def build_plain(arm_dir: Path, out_dir: Path, dest_name: str, *, dry_run: bool = False) -> Path:
    """Build a plain (non-PGO) image in the already-checked-out `arm_dir`."""
    if WINDOWS:
        create_windows_venv(arm_dir, dry_run=dry_run)
    else:
        run(["make", "install-bench"], cwd=arm_dir, env=posix_env(), dry_run=dry_run)
    return copy_ext(arm_dir, out_dir / f"{dest_name}{EXT}", dry_run=dry_run)


def build_pgo(
    arm_dir: Path, out_dir: Path, dest_name: str, *, dry_run: bool = False
) -> tuple[Path, Path]:
    """Two-phase PGO+LTO build, its own freshly trained profile. Returns the
    image path and the kept copy of the profile it trained."""
    if WINDOWS:
        run(
            [str(venv_python(arm_dir)), "scripts/pgo_build_clang_cl.py"],
            cwd=arm_dir,
            env=build_env(arm_dir),
            dry_run=dry_run,
        )
    else:
        run(["make", "pgo"], cwd=arm_dir, env=posix_env(), dry_run=dry_run)
    image = copy_ext(arm_dir, out_dir / f"{dest_name}{EXT}", dry_run=dry_run)
    profile_dest = out_dir / f"{dest_name}.profdata"
    if dry_run:
        print(
            f"+ copy <{arm_dir / 'build' / 'pgo' / 'strata.profdata'}> -> {profile_dest}",
            flush=True,
        )
    else:
        profile_dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(arm_dir / "build" / "pgo" / "strata.profdata", profile_dest)
    return image, profile_dest


def held_profile_rebuild(
    arm_dir: Path, profile: Path, out_dir: Path, dest_name: str, *, dry_run: bool = False
) -> Path:
    """Phase 2 again, against `profile` copied to the path phase 2 reads --
    PGO_MODE=use only, no retraining (benchmarks/ab_same_path_arms.py's
    `held_profile_rebuild` on exp/m15-ab-arm is the origin of this recipe; on
    Windows this is the same install scripts/pgo_build_clang_cl.py's own use
    phase performs, without its phase-1 training)."""
    target = arm_dir / "build" / "pgo" / "strata.profdata"
    if dry_run:
        print(f"+ copy {profile} -> {target}", flush=True)
    else:
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(profile, target)
        for stale in [*arm_dir.glob("build/lib.*"), *arm_dir.glob("build/temp.*")]:
            shutil.rmtree(stale)
    env: dict[str, str] = {"PGO_MODE": "use", "STRATA_PGO_PROFILE": str(target)}
    if WINDOWS:
        env.update({"STRATA_WIN_COMPILER": "clang-cl", "STRATA_ENABLE_LTO": "0"})
        env.update(build_env(arm_dir))
    else:
        env.update(posix_env())
        env["STRATA_ENABLE_LTO"] = "1"
    run(
        [
            str(venv_python(arm_dir)),
            "-m",
            "pip",
            "install",
            "--force-reinstall",
            "--no-deps",
            "-e",
            ".",
        ],
        cwd=arm_dir,
        env=env,
        dry_run=dry_run,
    )
    return copy_ext(arm_dir, out_dir / f"{dest_name}{EXT}", dry_run=dry_run)


def compare_images(a: Path, b: Path, out_json: Path, label: str, identity_main) -> int:
    """Run `image_identity` on `a`/`b`, refusing to call it on evidence that
    cannot prove anything: the same file compared with itself, or a missing
    image on either side. Both cases -- and `image_identity`'s own exit 2
    (unreadable) -- come back as a failing (non-zero) status; nothing here
    treats "could not verify" as "passed"."""
    if a.resolve() == b.resolve():
        print(f"{label}: refusing to compare {a} with itself")
        return 2
    missing = [str(path) for path in (a, b) if not path.is_file()]
    if missing:
        print(f"{label}: missing image(s): {', '.join(missing)}")
        return 2
    return identity_main([str(a), str(b), "--json", str(out_json)])


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", required=True, help="the ref to compare HEAD against")
    parser.add_argument("--arm-dir", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="print the planned commands only; build and compare nothing",
    )
    args = parser.parse_args(argv)
    dry_run = args.dry_run
    arm_dir, out_dir = args.arm_dir, args.out

    if not dry_run:
        out_dir.mkdir(parents=True, exist_ok=True)

    try:
        worktree_checkout(args.base, arm_dir, dry_run=dry_run)
        base_plain = build_plain(arm_dir, out_dir, "base_plain", dry_run=dry_run)
        base_pgo, base_profile = build_pgo(arm_dir, out_dir, "base_pgo", dry_run=dry_run)

        worktree_checkout("HEAD", arm_dir, dry_run=dry_run)
        head_plain = build_plain(arm_dir, out_dir, "head_plain", dry_run=dry_run)
        head_held = held_profile_rebuild(
            arm_dir, base_profile, out_dir, "head_pgo_held", dry_run=dry_run
        )
    finally:
        worktree_remove(arm_dir, dry_run=dry_run)

    if dry_run:
        print("dry run: no images were built or compared")
        return 0

    sys.path.insert(0, str(PROJECT_ROOT))
    from benchmarks.image_identity import main as identity_main

    plain_status = compare_images(
        base_plain, head_plain, out_dir / "identity_plain.json", "plain", identity_main
    )
    pgo_status = compare_images(
        base_pgo, head_held, out_dir / "identity_pgo.json", "pgo (held profile)", identity_main
    )

    report = out_dir / "identity.txt"
    report.write_text(
        json.dumps(
            {"base": args.base, "plain_exit": plain_status, "held_pgo_exit": pgo_status}, indent=1
        )
        + f"\nplain build: exit {plain_status}\nheld-profile pgo+lto build: exit {pgo_status}\n",
        encoding="utf-8",
    )
    print(report.read_text())
    # Both comparisons gate: a code-section difference (1), an image
    # image_identity could not read, a missing image, or a self-compare (2,
    # from compare_images or from image_identity itself) all fail the leg --
    # an unverified comparison is not evidence the code is identical.
    return 0 if plain_status == 0 and pgo_status == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())

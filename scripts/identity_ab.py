"""Same-absolute-path A/B build identity check (M15b acceptance item 4).

Builds `--base <ref>` and HEAD, one after another in the SAME absolute
worktree path (ThinLTO's promoted-name suffixes and the profile follow the
build directory -- docs/decisions.md, 2026-09-27, and
benchmarks/ab_same_path_arms.py on exp/m15-ab-arm is the origin of this
recipe), each with its own freshly trained PGO+LTO profile, plus a plain
(non-PGO) build of each. The two builds' `_strata` extension code sections are
then compared with `benchmarks/image_identity.py`.

Usage:
    python scripts/identity_ab.py --base <ref> --arm-dir <absolute path> --out <dir>

Exit 0: the PGO+LTO code section is identical (or unverified -- an image this
script's reader cannot parse is recorded, never a stop). Exit 1: the PGO+LTO
code section differs -- the CI job fails the leg on this. The plain-build
comparison is always run and recorded but never gates the exit code: it is
the M12b criterion-3 diagnostic, not the acceptance gate.
"""

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
from pathlib import Path

WINDOWS = sys.platform == "win32"
EXT = ".pyd" if WINDOWS else ".so"

PROJECT_ROOT = Path(__file__).resolve().parents[1]


def run(cmd: list, *, cwd: Path | None = None) -> None:
    print("+ " + " ".join(map(str, cmd)), flush=True)
    subprocess.run(cmd, cwd=cwd, check=True)


def venv_python(root: Path) -> Path:
    return root / ".venv" / ("Scripts/python.exe" if WINDOWS else "bin/python")


def find_extension(root: Path) -> Path:
    matches = sorted(root.glob(f"python/strata/_strata*{EXT}")) or sorted(
        root.rglob(f"_strata*{EXT}")
    )
    if not matches:
        raise SystemExit(f"no _strata*{EXT} found under {root}")
    return matches[0]


def build_arm(ref: str, arm_dir: Path, label: str, out_dir: Path) -> tuple[Path, Path]:
    """Check out `ref` at `arm_dir`, build plain then PGO+LTO, copy both
    extensions to `out_dir/<label>_plain<ext>` / `<label>_pgo<ext>`."""
    if arm_dir.exists():
        run(["git", "worktree", "remove", "--force", str(arm_dir)])
    run(["git", "worktree", "add", "--detach", str(arm_dir), ref])
    try:
        run(["make", "install-bench"], cwd=arm_dir)
        out_dir.mkdir(parents=True, exist_ok=True)
        plain_dest = out_dir / f"{label}_plain{EXT}"
        shutil.copy2(find_extension(arm_dir), plain_dest)

        if WINDOWS:
            run([str(venv_python(arm_dir)), "scripts/pgo_build_clang_cl.py"], cwd=arm_dir)
        else:
            run(["make", "pgo"], cwd=arm_dir)
        pgo_dest = out_dir / f"{label}_pgo{EXT}"
        shutil.copy2(find_extension(arm_dir), pgo_dest)
        return plain_dest, pgo_dest
    finally:
        run(["git", "worktree", "remove", "--force", str(arm_dir)])


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", required=True, help="the ref to compare HEAD against")
    parser.add_argument("--arm-dir", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    args = parser.parse_args(argv)

    sys.path.insert(0, str(PROJECT_ROOT))
    from benchmarks.image_identity import main as identity_main

    base_plain, base_pgo = build_arm(args.base, args.arm_dir, "base", args.out)
    head_plain, head_pgo = build_arm("HEAD", args.arm_dir, "head", args.out)

    args.out.mkdir(parents=True, exist_ok=True)
    plain_status = identity_main(
        [str(base_plain), str(head_plain), "--json", str(args.out / "identity_plain.json")]
    )
    pgo_status = identity_main(
        [str(base_pgo), str(head_pgo), "--json", str(args.out / "identity_pgo.json")]
    )

    report = args.out / "identity.txt"
    report.write_text(
        json.dumps(
            {"base": args.base, "plain_exit": plain_status, "pgo_exit": pgo_status}, indent=1
        )
        + f"\nplain build: exit {plain_status}\npgo+lto build: exit {pgo_status}\n",
        encoding="utf-8",
    )
    print(report.read_text())
    # Only the PGO+LTO comparison gates the leg (item 4 of the acceptance
    # criteria): exit 1 means the code sections differ, exit 2 means the
    # comparison could not be made and is recorded as unverified, not a stop.
    return 1 if pgo_status == 1 else 0


if __name__ == "__main__":
    raise SystemExit(main())

"""Build-noise control (M15b linux-arm64 replay, step 2).

Rebuilds BASE against BASE's own profile with the identical held-profile
recipe identity_ab.py uses for HEAD, in a fresh worktree at the same
absolute path /work/arm. Produces base_pgo_held, then compares:

  A        (base_pgo, base's own from-scratch PGO+LTO build)  vs  A_held
  A_held                                                        vs  B_held (head_pgo_held from step 1)

If A vs A_held differs the same way step 1's A vs B_held did, the .text /
.rela.plt / .note.gnu.build-id differences are build-noise (profile/path
non-determinism), not code introduced by HEAD's commits.
"""

import sys
from pathlib import Path

sys.path.insert(0, "/work")

from benchmarks.image_identity import main as identity_main  # noqa: E402
from scripts.identity_ab import (  # noqa: E402
    build_plain,
    held_profile_rebuild,
    worktree_checkout,
    worktree_remove,
)

BASE = "38eaa9f248f509d9a4beb75b267189d011c1109e"
ARM_DIR = Path("/work/arm")
OUT_DIR = Path("/work/out")


def main() -> int:
    worktree_checkout(BASE, ARM_DIR)
    try:
        build_plain(ARM_DIR, OUT_DIR, "base_plain_recheck")
        a_held = held_profile_rebuild(
            ARM_DIR, OUT_DIR / "base_pgo.profdata", OUT_DIR, "base_pgo_held"
        )
    finally:
        worktree_remove(ARM_DIR)

    base_pgo = OUT_DIR / "base_pgo.so"
    b_held = OUT_DIR / "head_pgo_held.so"

    print("=== A (base_pgo) vs A_held (base_pgo_held) ===")
    r1 = identity_main([str(base_pgo), str(a_held), "--json", str(OUT_DIR / "identity_A_vs_Aheld.json")])
    print(f"exit {r1}")

    print("=== A_held (base_pgo_held) vs B_held (head_pgo_held) ===")
    r2 = identity_main([str(a_held), str(b_held), "--json", str(OUT_DIR / "identity_Aheld_vs_Bheld.json")])
    print(f"exit {r2}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env bash
# Arm B-held: 10521a9 built with phase 2 of scripts/pgo_build.sh (PGO_MODE=use,
# STRATA_ENABLE_LTO=1) against arm A's profile instead of its own. pip runs
# verbose so the compiler's profile diagnostics reach the log. The gate
# (setup.py TestGatedBuildExt) runs as in phase 2.
# Usage: build_held.sh <worktree at 10521a9> <profile> <log>
set -euo pipefail
TREE="$1"
PROFILE="$2"
LOG="$3"
cd "$TREE"
unset LLVM_PROFILE_FILE GCOV_PREFIX GCOV_PREFIX_STRIP STRATA_MARCH
export PGO_MODE=use
export STRATA_ENABLE_LTO=1
export STRATA_PGO_PROFILE="$PROFILE"
.venv/bin/python -m pip install -v --force-reinstall --no-deps -e . >"$LOG" 2>&1
.venv/bin/python scripts/build_identity.py --check-unprofiled strata._dumps_hook >>"$LOG" 2>&1
echo "held build exit 0" >>"$LOG"

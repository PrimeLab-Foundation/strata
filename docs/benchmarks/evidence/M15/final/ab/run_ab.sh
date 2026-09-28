#!/usr/bin/env bash
# M15 pinned A/B session: one `make probe-ab-rows` over the 21 driver-supported small-tier
# canonical rows + the 5 medium dumps rows, 6 ABBA blocks x repeat 60, driven from
# ~/worktrees/strata/main-pgo (facade 38eaa9f -- main's _strata rejects the parse_types
# keyword that ad04f61's facade passes, so both .so run under main's facade).
# Usage: run_ab.sh <label> <build A .so> <build B .so> <out tsv>
set -euo pipefail
LABEL="$1"; BA="$2"; BB="$3"; OUT="$4"
E=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/final/ab
TREE=~/worktrees/strata/main-pgo
ROWS=""
for d in users flat nested wide_arrays mixed; do
    for op in dumps loads load dump; do ROWS="$ROWS --row small:$d:$op"; done
done
ROWS="$ROWS --row small:users:ndload"
for d in users flat nested wide_arrays mixed; do ROWS="$ROWS --row medium:$d:dumps"; done
state() {
    { echo "== $LABEL $1 $(date '+%F %T')"; uptime; ps -Ao pid,pcpu,comm -r | head -6 || true; } >>"$E/load_log.txt"
}
state before
cd "$TREE"
make probe-ab-rows BUILD_A="$BA" BUILD_B="$BB" PROBE_OUT="$OUT" PROBE_BLOCKS=6 PROBE_REPEAT=60 \
    PROBE_ROWS="$ROWS" >"${OUT%.tsv}.driver.log" 2>&1
state after
echo "session $LABEL exit 0"

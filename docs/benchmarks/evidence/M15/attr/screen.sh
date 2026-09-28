#!/usr/bin/env bash
# M15 attribution screen: the four serializer rows that lost in own3_ab plus one parse control,
# 6 ABBA blocks x repeat 60, driven from ~/worktrees/strata/main-pgo (main's facade, as run_ab.sh).
# Usage: screen.sh <label> <build A .so> <build B .so> <out tsv>
set -euo pipefail
LABEL="$1"; BA="$2"; BB="$3"; OUT="$4"
E=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr
TREE=~/worktrees/strata/main-pgo
ROWS="--row small:flat:dumps --row small:flat:dump --row small:users:dumps --row medium:users:dumps --row small:users:loads"
state() {
    { echo "== $LABEL $1 $(date '+%F %T')"; uptime; ps -Ao pid,pcpu,comm -r | head -6 || true; } >>"$E/load_log.txt"
}
state before
cd "$TREE"
make probe-ab-rows BUILD_A="$BA" BUILD_B="$BB" PROBE_OUT="$OUT" PROBE_BLOCKS=6 PROBE_REPEAT=60 \
    PROBE_ROWS="$ROWS" >"${OUT%.tsv}.driver.log" 2>&1
state after
echo "session $LABEL exit 0"

#!/usr/bin/env bash
# M15 boundary screen: the attribution screen's five rows plus the three linux-x86_64 rows
# run 36502555579 resolved against strata (small dumps mixed, dumps wide_arrays, dump wide_arrays),
# 6 ABBA blocks x repeat 60, driven from ~/worktrees/strata/main-pgo (main's facade, as run_ab.sh).
# Usage: screen.sh <label> <build A .so> <build B .so> <out tsv>
#
# Gate guard (added 06:06 while session.sh was in its fix_screen wait, before this file was first
# executed): a screen never overlaps the gate run in the main checkout. Before measuring it waits
# until no make/pip/pytest/clang/ctest process has its cwd under the main checkout for 4
# consecutive 15 s polls (no cap: an overlapping screen is invalid), then re-waits for the 1-min
# load < 2.5 within the label's 45-min budget (wait_<label>.start), extended by at most 300 s
# after the gate ends when the gate outlasted the budget. Records uptime + top 5; kills nothing.
set -euo pipefail
LABEL="$1"; BA="$2"; BB="$3"; OUT="$4"
E=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/boundary-screen
ROOT=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata
TREE=~/worktrees/strata/main-pgo
ROWS="--row small:flat:dumps --row small:flat:dump --row small:users:dumps --row medium:users:dumps --row small:users:loads --row small:mixed:dumps --row small:wide_arrays:dumps --row small:wide_arrays:dump"
state() {
    { echo "== $LABEL $1 $(date '+%F %T')"; uptime; ps -Ao pid,pcpu,comm -r | head -6 || true; } >>"$E/load_log.txt"
}
gate_busy() {
    local pid cwd
    for pid in $( (pgrep -x make; pgrep -f 'pytest|py_tests\.py|cpp_tests\.py|gate\.sh|ctest|pip install|setup\.py|clang') 2>/dev/null | sort -u); do
        cwd=$(lsof -a -p "$pid" -d cwd -Fn 2>/dev/null | sed -n 's/^n//p' || true)
        case "$cwd" in "$ROOT" | "$ROOT"/*) return 0 ;; esac
    done
    return 1
}
load1() { sysctl -n vm.loadavg | awk '{print $2}'; }
BUDGET=2700
T0=$(cat "$E/wait_$LABEL.start" 2>/dev/null || date +%s)
state gate-wait-start
quiet=0; gate_seen=0
while [ "$quiet" -lt 4 ]; do
    if gate_busy; then quiet=0; gate_seen=1; else quiet=$((quiet + 1)); fi
    [ "$quiet" -lt 4 ] && sleep 15
done
GATE_END=$(date +%s)
DEADLINE=$((T0 + BUDGET))
if [ "$gate_seen" -eq 1 ] && [ "$DEADLINE" -lt $((GATE_END + 300)) ]; then DEADLINE=$((GATE_END + 300)); fi
while :; do
    l=$(load1); now=$(date +%s)
    if awk -v l="$l" 'BEGIN{exit !(l < 2.5)}'; then verdict=QUIET; break; fi
    if [ "$now" -ge "$DEADLINE" ]; then verdict=TIMEOUT; break; fi
    sleep 15
done
echo "GATE $LABEL gate_seen=$gate_seen gate_clear=$(date -r "$GATE_END" '+%T') $verdict load=$l waited=$(($(date +%s) - T0))" | tee -a "$E/load_log.txt"
state before
cd "$TREE"
make probe-ab-rows BUILD_A="$BA" BUILD_B="$BB" PROBE_OUT="$OUT" PROBE_BLOCKS=6 PROBE_REPEAT=60 \
    PROBE_ROWS="$ROWS" >"${OUT%.tsv}.driver.log" 2>&1
state after
echo "session $LABEL exit 0"

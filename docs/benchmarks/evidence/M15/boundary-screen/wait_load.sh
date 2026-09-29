#!/usr/bin/env bash
# Wait for a quiet machine before an A/B session: poll the 1-min load every 15 s until it is
# < 2.5, or until 45 min have passed since the first call for this label (the start time is
# kept in wait_<label>.start so a re-armed wait continues the same budget). Records uptime +
# the top 5 processes by CPU at the start and at the end in load_log.txt. Never kills anything.
# Prints exactly one line at the end: QUIET|TIMEOUT|SLICE <label> load=<1-min> waited=<s>.
# Usage: wait_load.sh <label> [slice-seconds]
set -uo pipefail
E=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/boundary-screen
LABEL="$1"; SLICE="${2:-1740}"
BUDGET=2700
START_FILE="$E/wait_$LABEL.start"
[ -f "$START_FILE" ] || date +%s >"$START_FILE"
T0=$(cat "$START_FILE"); SLICE_T0=$(date +%s)
state() {
    { echo "== $LABEL wait-$1 $(date '+%F %T')"; uptime; ps -Ao pid,pcpu,comm -r | head -6 || true; } >>"$E/load_log.txt"
}
load1() { sysctl -n vm.loadavg | awk '{print $2}'; }
state start
while :; do
    now=$(date +%s); l=$(load1)
    if awk -v l="$l" 'BEGIN{exit !(l < 2.5)}'; then
        state end; echo "QUIET $LABEL load=$l waited=$((now - T0))"; exit 0
    fi
    if [ $((now - T0)) -ge $BUDGET ]; then
        state end; echo "TIMEOUT $LABEL load=$l waited=$((now - T0))"; exit 0
    fi
    if [ $((now - SLICE_T0)) -ge "$SLICE" ]; then
        echo "SLICE $LABEL load=$l waited=$((now - T0))"; exit 0
    fi
    sleep 15
done

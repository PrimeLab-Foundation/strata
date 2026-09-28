#!/usr/bin/env bash
# micro2 driver: wait for 1-min load <= 3.5 (at most 15 min), then the admission
# table (run_micro.sh, micro's method), facade_kw.py (micro's method, B's venv),
# and facade_x.py (cross-build, launch order N M M N N M M N).
set -uo pipefail
M=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/micro2
PN=~/worktrees/strata/m15-pgo/.venv/bin/python
PM=~/worktrees/strata/main-pgo/.venv/bin/python
MIXED=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/benchmarks/data/generated/small/mixed.json
load1() { sysctl -n vm.loadavg | awk '{print $2}'; }
for _ in $(seq 1 90); do
    awk -v l="$(load1)" 'BEGIN { exit !(l <= 3.5) }' && break
    sleep 10
done
: >"$M/load_log.txt"
bash "$M/run_micro.sh" || exit 1
python3 "$M/analyze_micro.py" "$M" >"$M/admission_table.txt" || exit 1
{ echo "== facade_kw $(date '+%F %T')"; uptime; } >>"$M/load_log.txt"
cd "$(mktemp -d)"
"$PN" "$M/facade_kw.py" "$MIXED" 61 >"$M/facade_kw.txt" || exit 1
{ echo "== facade_x before $(date '+%F %T')"; uptime; } >>"$M/load_log.txt"
: >"$M/facade_x.tsv"
i=0
for arm in new main main new new main main new; do
    if [ "$arm" = new ]; then py=$PN; else py=$PM; fi
    "$py" "$M/facade_x.py" --arm "$arm" --launch "L$i" --mixed "$MIXED" --repeat 61 \
        | grep -v '^#' >>"$M/facade_x.tsv" || exit 1
    i=$((i + 1))
done
{ echo "== facade_x after $(date '+%F %T')"; uptime; } >>"$M/load_log.txt"
echo "run_all exit 0"

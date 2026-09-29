#!/usr/bin/env bash
# M15 admission microbenchmark driver: launches N H H N N H H N (N = native arm in the m15-pgo
# venv, armBown2 installed; H = hook arm in the main-pgo venv, main 38eaa9f installed), each
# launch 41 repeats per (kind, engine). Load state logged before and after.
set -uo pipefail
M=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/micro
PN=~/worktrees/strata/m15-pgo/.venv/bin/python
PH=~/worktrees/strata/main-pgo/.venv/bin/python
OUT=$M/admission.tsv
state() { { echo "== micro $1 $(date '+%F %T')"; uptime; ps -Ao pid,pcpu,comm -r | head -6 || true; } >>"$M/load_log.txt"; }
state before
cd "$(mktemp -d)"
: >"$OUT"; : >"$M/admission.meta.txt"
i=0
for arm in native hook hook native native hook hook native; do
    if [ "$arm" = native ]; then py=$PN; else py=$PH; fi
    "$py" "$M/admission.py" --arm "$arm" --launch "L$i" --repeat 41 >"$M/launch_L$i.txt" || { echo "launch L$i failed"; exit 1; }
    grep -v '^#' "$M/launch_L$i.txt" >>"$OUT"
    grep '^#' "$M/launch_L$i.txt" | sed "s/^/L$i /" >>"$M/admission.meta.txt"
    i=$((i + 1))
done
state after
echo "micro exit 0"

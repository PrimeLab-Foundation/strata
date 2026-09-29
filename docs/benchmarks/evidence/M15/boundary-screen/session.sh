#!/usr/bin/env bash
# Boundary screen session: fix screen (armA vs armFix = 74d78ca make pgo), then the same-session
# A/A (armA vs armA2); each after wait_load.sh (1-min load < 2.5, 45 min budget, nothing killed).
set -uo pipefail
E=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/boundary-screen
M=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15
$E/wait_load.sh fix_screen 2700
$E/screen.sh fix_screen $M/armA/_strata.cpython-314-darwin.so $E/armFix/_strata.cpython-314-darwin.so $E/fix_screen.tsv
$E/wait_load.sh aa 2700
$E/screen.sh aa $M/armA/_strata.cpython-314-darwin.so $M/armA2/_strata.cpython-314-darwin.so $E/aa.tsv
$E/analyze.sh $E/fix_screen.tsv $E/aa.tsv > $E/fix_screen_aa.summarize.txt 2>&1
shasum -a 256 ~/worktrees/strata/main-pgo/python/strata/_strata.cpython-314-darwin.so | cut -c1-12

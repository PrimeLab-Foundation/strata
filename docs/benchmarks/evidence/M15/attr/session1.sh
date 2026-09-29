#!/usr/bin/env bash
# Session 1: C2 screen (armA vs armC2), then A/A (armA vs armA2); each after wait_load.sh.
set -uo pipefail
/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/wait_load.sh c2_screen 2700
/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/screen.sh c2_screen /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/armA/_strata.cpython-314-darwin.so /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/armC2/_strata.cpython-314-darwin.so /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/c2_screen.tsv
/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/wait_load.sh aa_s1 2700
/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/screen.sh aa_s1 /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/armA/_strata.cpython-314-darwin.so /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/armA2/_strata.cpython-314-darwin.so /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/aa_s1.tsv
shasum -a 256 ~/worktrees/strata/main-pgo/python/strata/_strata.cpython-314-darwin.so | cut -c1-12

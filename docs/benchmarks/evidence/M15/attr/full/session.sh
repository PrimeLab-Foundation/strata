#!/usr/bin/env bash
# Full 26-row A/B of 809621c's make pgo image vs armA (main 38eaa9f), then the A/A (armA vs armA2),
# each after wait_load.sh (1-min load < 2.5, 45-min budget).
set -uo pipefail
/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/full/wait_load.sh fix_ab 2700
/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/full/run_ab.sh fix_ab /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/armA/_strata.cpython-314-darwin.so /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/arm809621c/_strata.cpython-314-darwin.so /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/full/fix_ab.tsv
/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/full/wait_load.sh aa_full 2700
/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/full/run_ab.sh aa_full /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/armA/_strata.cpython-314-darwin.so /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/armA2/_strata.cpython-314-darwin.so /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr/full/aa_full.tsv
shasum -a 256 ~/worktrees/strata/main-pgo/python/strata/_strata.cpython-314-darwin.so | cut -c1-12
echo "session done"

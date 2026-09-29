#!/usr/bin/env bash
# Usage: analyze.sh <ab tsv> <aa tsv>  -> <ab>.<aa>.analysis.{txt,json} + summarize table on stdout
set -euo pipefail
AB="$1"; AA="$2"
TREE=~/worktrees/strata/main-pgo
E=/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15
stem="${AB%.tsv}.$(basename "${AA%.tsv}")"
cd "$TREE"
PYTHONPATH=. .venv/bin/python benchmarks/ab_blocks.py "$AB" --aa "$AA" --json "$stem.analysis.json" >"$stem.analysis.txt"
python3 "$E/ab/summarize.py" "$stem.analysis.json"

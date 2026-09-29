#!/bin/bash
# Collect the linux-symbols evidence packet (run inside the matching container per ISA).
# usage: collect.sh <isa: x86|arm64> <CI arms dir> <out dir>
set -euo pipefail
ISA=$1; CI=$2; OUT=$3; W=/work
mkdir -p "$OUT"
O=$W/out/$ISA
python3 $W/cmp.py "$CI/A.so" "$O/main/_strata.so" >"$OUT/repro.main-vs-CI-A.$ISA.txt"
python3 $W/cmp.py "$CI/B.so" "$O/borig/_strata.so" >"$OUT/repro.borig-vs-CI-B.$ISA.txt"
python3 $W/table.py "AA,B-orig,B-fix" \
    "$O/main/_strata.so,$O/borig/_strata.so,$O/bfix/_strata.so" \
    'Serializer|dumps_to_python|strata_dumps' >"$OUT/symbols.$ISA.tsv"
python3 $W/opdiff.py "$O/main/_strata.so" "$O/bfix/_strata.so" write,write_mapping_body,write_mapping,write_mapping_uncached \
    >"$OUT/opcodes.main-vs-bfix.$ISA.txt"
python3 $W/opdiff.py "$O/main/_strata.so" "$O/borig/_strata.so" write,write_mapping_body,write_mapping,write_mapping_uncached \
    >"$OUT/opcodes.main-vs-borig.$ISA.txt"
mkdir -p "$OUT/calls.$ISA"
for a in main borig bfix; do $W/calls.sh "$O/$a/_strata.so" "$OUT/calls.$ISA/$a" >/dev/null; rm -f "$OUT/calls.$ISA/$a.dis"; done
for a in main borig bfix; do
    cp "$O/$a/cmds.txt" "$OUT/cmds.$a.$ISA.txt"
    cp "$O/$a/sha256.txt" "$OUT/sha256.$a.$ISA.txt"
    cp "$O/$a/compile.stderr" "$OUT/compile-stderr.$a.$ISA.txt"
    cp "$O/$a/symbols.txt" "$OUT/nm.$a.$ISA.txt"
done
cp "$O/bfix/clang.txt" "$OUT/clang.$ISA.txt"
for p in A B; do
    llvm-profdata show --detailed-summary "$W/prof/$ISA/$p.profdata" | head -12 >"$OUT/profile-summary.$p.$ISA.txt"
done
echo "collected $ISA"

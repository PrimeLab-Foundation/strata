#!/usr/bin/env bash
# M15c linux-arm64 held-identity replay: M15b's recipe, base 234ea15, head = the cloned tip.
set -euo pipefail
git config --global --add safe.directory '*'
git clone -q /hostrepo /work
cd /work
git checkout -q exp/native-gap
echo "HEAD $(git rev-parse HEAD)"
mkdir -p /work/out
python3 scripts/identity_ab.py --base 234ea15861d2f37b1b55c6251281a49021864441 \
    --arm-dir /work/arm --out /work/out > /work/out/step1_identity_ab.log 2>&1 || echo "identity_ab exit $?"
cat /work/out/identity.txt || true
for pair in "base_pgo head_pgo_held"; do
  set -- $pair
  python3 benchmarks/normalised_disassembly.py /work/out/$1.so /work/out/$2.so \
      > /work/out/normalised_$1-vs-$2.txt 2>&1 || true
  cat /work/out/normalised_$1-vs-$2.txt
done
mkdir -p /hostout && cp -r /work/out/. /hostout/

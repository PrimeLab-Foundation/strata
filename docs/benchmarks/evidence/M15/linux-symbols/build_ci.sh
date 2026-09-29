#!/bin/bash
# Replay a CI arm's recorded compile+link commands (arms/<arm>.so.build.json) against a
# source tree and a CI profile, inside ubuntu:24.04 with apt clang 18.1.3.
# usage: build_ci.sh <tree> <build.json> <profile.profdata> <out_dir> <march flag>
set -euo pipefail
TREE=$1; BJ=$2; PROF=$3; OUT=$4; MARCH=$5
mkdir -p "$OUT"
python3 - "$BJ" "$PROF" "$OUT" "$MARCH" >"$OUT/cmds.txt" <<'EOF'
import json, shlex, sys
bj, prof, out, march = sys.argv[1:]
d = json.load(open(bj))
for c in d["commands"]:
    r = []
    for a in c:
        if a.startswith("-I/opt/hostedtoolcache"):
            a = "-I/usr/include/python3.12"
        elif a.startswith("-I/home/runner") or a.startswith("-Wl,--rpath=") or a.startswith("-L/opt/"):
            continue
        elif a.startswith("-fprofile-use="):
            a = "-fprofile-use=" + prof
        elif a == "-march=native":
            a = march
        elif ".build-temp/" in a:
            a = out + "/obj/" + a.split(".build-temp/", 1)[1]
        elif ".build-lib/" in a:
            a = out + "/_strata.so"
        r.append(a)
    print(" ".join(shlex.quote(x) for x in r))
EOF
cd "$TREE"
grep -o "$OUT/obj/[^ ]*\.o" "$OUT/cmds.txt" | xargs -n1 dirname | sort -u | xargs mkdir -p
head -n -1 "$OUT/cmds.txt" | tr '\n' '\0' | xargs -0 -P "${JOBS:-4}" -I{} sh -c '{}' 2>"$OUT/compile.stderr"
tail -n 1 "$OUT/cmds.txt" | sh 2>"$OUT/link.stderr"
clang++ --version | head -1 >"$OUT/clang.txt"
nm -S --size-sort -C "$OUT/_strata.so" | grep -E 'Serializer|dumps' >"$OUT/symbols.txt" || true
nm -S --size-sort -C "$OUT/_strata.so" >"$OUT/symbols_all.txt"
sha256sum "$OUT/_strata.so" "$PROF" >"$OUT/sha256.txt"
echo "built $OUT"

#!/bin/bash
# usage: calls.sh <so> <out prefix>: per-function disassembly of the hot writers, and their call targets
set -e
SO=$1; P=$2
for f in write write_mapping_body write_mapping; do
  nm -C "$SO" | grep -E "Serializer::$f\(" | awk '{print $3}' >/dev/null
done
llvm-objdump -d --no-show-raw-insn -C "$SO" > "$P.dis"
python3 - "$P.dis" "$P" <<'PY'
import re, sys, collections
dis, P = sys.argv[1], sys.argv[2]
funcs = {}; cur = None
for line in open(dis):
    m = re.match(r"^[0-9a-f]+ <(.*)>:$", line.strip())
    if m: cur = m.group(1); funcs[cur] = []; continue
    if cur and line.strip(): funcs[cur].append(line.rstrip())
def norm(n): return re.sub(r" ?\[clone \.llvm\.\d+\]|\.llvm\.\d+", "", n)
want = ["Serializer::write(_object*)", "Serializer::write_mapping_body(", "Serializer::write_mapping(_object*)"]
for w in want:
    tot = collections.Counter(); insns = collections.Counter()
    for name, body in funcs.items():
        if w in norm(name):
            part = "cold" if ".cold" in name else "hot"
            for l in body:
                ins = l.split("\t")
                if len(ins) < 2: continue
                op = ins[1].split()[0] if ins[1].split() else ""
                insns[(part, op)] += 1
                if op in ("call", "callq", "jmp", "jmpq") and "<" in l:
                    tgt = norm(re.search(r"<(.*)>", l).group(1))
                    tgt = re.sub(r"\+0x[0-9a-f]+$", "", tgt)
                    if op.startswith("call") or not tgt.startswith(norm(name).split("(")[0][:20]):
                        tot[(op[:4], tgt)] += 1
    with open(f"{P}.{w.split('::')[1].split('(')[0]}.calls.txt", "w") as o:
        for (op, t), n in sorted(tot.items(), key=lambda x: x[0][1]):
            o.write(f"{n}\t{op}\t{t}\n")
    print(w, "insns hot", sum(v for (p, _), v in insns.items() if p == "hot"), "cold", sum(v for (p, _), v in insns.items() if p == "cold"))
PY

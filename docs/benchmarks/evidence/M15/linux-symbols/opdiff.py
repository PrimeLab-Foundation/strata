import re, sys, collections, subprocess
def funcs(so):
    out = subprocess.run(["llvm-objdump", "-d", "--no-show-raw-insn", "-C", so], capture_output=True, text=True).stdout
    F = {}; cur = None
    for line in out.splitlines():
        m = re.match(r"^[0-9a-f]+ <(.*)>:$", line.strip())
        if m: cur = re.sub(r" ?\[clone \.llvm\.\d+\]|\.llvm\.\d+", "", m.group(1)); F.setdefault(cur, []); continue
        if cur and "\t" in line:
            parts = line.split("\t")
            if len(parts) >= 2 and parts[1].strip(): F[cur].append(parts[1].split()[0])
    return F
A, B = funcs(sys.argv[1]), funcs(sys.argv[2])
for w in sys.argv[3].split(","):
    ha = [k for k in A if f"Serializer::{w}(" in k and "::Frame" not in k]
    hb = [k for k in B if f"Serializer::{w}(" in k and "::Frame" not in k]
    ca = collections.Counter(op for k in ha for op in A[k]); cb = collections.Counter(op for k in hb for op in B[k])
    d = {op: cb[op] - ca[op] for op in set(ca) | set(cb) if cb[op] != ca[op]}
    parts = {k.split(")")[-1].strip() or "hot": len(A[k]) for k in ha}; partsb = {k.split(")")[-1].strip() or "hot": len(B[k]) for k in hb}
    print(f"{w}: insns A {sum(ca.values())} {parts} -> B {sum(cb.values())} {partsb}; opcode delta: {dict(sorted(d.items(), key=lambda x: -abs(x[1])))}")

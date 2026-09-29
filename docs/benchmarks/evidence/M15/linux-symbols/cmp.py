import re, subprocess, sys
def syms(path):
    out = subprocess.run(["nm", "-S", "-C", path], capture_output=True, text=True).stdout
    d = {}
    for l in out.splitlines():
        p = l.split(None, 3)
        if len(p) < 4 or p[2] not in "tTwW": continue
        name = re.sub(r"\.llvm\.\d+", "", p[3]); name = re.sub(r"\[clone \.llvm\.\d+\] ?", "", name).strip()
        d[name] = d.get(name, 0) + int(p[1], 16)
    return d
a, b = syms(sys.argv[1]), syms(sys.argv[2])
filt = sys.argv[3] if len(sys.argv) > 3 else ""
diff = [(k, a.get(k, 0), b.get(k, 0)) for k in sorted(set(a) | set(b)) if a.get(k, 0) != b.get(k, 0) and filt in k]
print(f"{len(a)} vs {len(b)} text symbols; total {sum(a.values())} vs {sum(b.values())}; {len(diff)} differ")
for k, x, y in diff[:60]: print(f"{x:7d} {y:7d}  {k[:150]}")

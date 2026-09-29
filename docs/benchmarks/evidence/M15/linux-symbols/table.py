import re, subprocess, sys
def syms(path):
    out = subprocess.run(["nm", "-S", "-C", path], capture_output=True, text=True).stdout
    d = {}
    for l in out.splitlines():
        p = l.split(None, 3)
        if len(p) < 4 or p[2] not in "tTwW": continue
        name = re.sub(r" ?\[clone \.llvm\.\d+\]", "", p[3]); name = re.sub(r"\.llvm\.\d+", "", name).strip()
        d[name] = d.get(name, 0) + int(p[1], 16)
    return d
labels = sys.argv[1].split(","); paths = sys.argv[2].split(","); filt = sys.argv[3]
S = [syms(p) for p in paths]
keys = sorted({k for s in S for k in s if re.search(filt, k)}, key=lambda k: -max(s.get(k, 0) for s in S))
print("\t".join(labels) + "\tsymbol")
for k in keys:
    row = [s.get(k, 0) for s in S]
    short = re.sub(r"strata::bindings::\(anonymous namespace\)::", "", k)
    short = re.sub(r"\(_object\*.*?\)|\(char const\*.*?\)|\(strata::bindings::SchemaCacheLease.*?\)|\(_object\*\*, long\)", "()", short)
    print("\t".join(str(x) for x in row) + "\t" + short[:110])

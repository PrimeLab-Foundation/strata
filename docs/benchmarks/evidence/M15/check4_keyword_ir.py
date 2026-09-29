"""Check 4, keyword arms: the profile's branch weights on the parse_types arms.

Usage: check4_keyword_ir.py <tree> <_strata .build.json> <out-dir>

Re-runs the build's own compile line for python_module.cpp (from the
.build.json, so the same flags and the same -fprofile-use profile) with
`-S -emit-llvm` and `-mllvm -print-after=pgo-instr-use`, which prints each
keyword function's IR right after the profile was attached. Only diagnostics
are added (value names kept, IR printed); the CFG the profile hash describes is
the build's. Then, for strata_loads/load/query/search, prints the function
entry count and every conditional branch whose condition involves the
parse_types keyword -- its string compare in strata_loads's keyword loop and
the `parse_types != False` arms -- with the branch_weights the profile gave it.
"""

import json
import re
import subprocess
import sys
from pathlib import Path

FUNCS = (
    "_ZN12_GLOBAL__N_112strata_loadsEP7_objectPKS1_lS1_",
    "_ZN12_GLOBAL__N_111strata_loadEP7_objectS1_S1_",
    "_ZN12_GLOBAL__N_112strata_queryEP7_objectS1_S1_",
    "_ZN12_GLOBAL__N_113strata_searchEP7_objectS1_S1_",
)


def compile_ir(tree, build_json, out_dir):
    commands = json.loads(Path(build_json).read_text())["commands"]
    cmd = next(c for c in commands if "src/strata/bindings/python_module.cpp" in c)
    cmd = list(cmd)
    out = cmd.index("-o")
    cmd[out + 1] = str(Path(out_dir) / "python_module.pgo-use.ll")
    cmd += [
        "-S",
        "-emit-llvm",
        "-fno-discard-value-names",
        "-mllvm",
        "-print-after=pgo-instr-use",
    ]  # no -filter-print-funcs: it suppresses the annotation
    Path(out_dir, "python_module.ir-command.json").write_text(json.dumps(cmd, indent=1))
    proc = subprocess.run(cmd, cwd=tree, capture_output=True, text=True)
    Path(out_dir, "python_module.after-pgo-instr-use.ll").write_text(proc.stderr)
    if proc.returncode:
        raise SystemExit(proc.stderr[-3000:])
    return proc.stderr, Path(out_dir, "python_module.pgo-use.ll").read_text()


def metadata(module_text):
    md = {}
    for line in module_text.splitlines():
        m = re.match(r"^(![0-9]+) = (.*)$", line)
        if m:
            md[m.group(1)] = m.group(2)
    return md


def strings(module_text):
    out = {}
    for line in module_text.splitlines():
        m = re.match(r'^(@[\w.$]+) = .*c"([^"]*)\\00"', line)
        if m:
            out[m.group(1)] = m.group(2)
    return out


def main():
    tree, build_json, out_dir = sys.argv[1:4]
    Path(out_dir).mkdir(parents=True, exist_ok=True)
    dump, module = compile_ir(tree, build_json, out_dir)
    md, strs = metadata(module), strings(module)
    # print-after dumps each function once (plus module-level metadata is not
    # printed there), so metadata numbers are resolved against the final module
    # only for names; the dump's own !prof numbers are resolved in the dump.
    md.update(metadata(dump))
    for func in FUNCS:
        m = re.search(r"define [^\n]*@" + re.escape(func) + r"\(.*?\n}\n", dump, re.S)
        if not m:
            print(f"== {func}: not in the print-after dump")
            continue
        body = m.group(0)
        entry = re.search(r"!prof (![0-9]+)", body.split("\n", 1)[0])
        print(f"== {func}  entry: {md.get(entry.group(1)) if entry else 'no !prof'}")
        lines = body.splitlines()
        # values defined from the parse_types keyword: its compare and its local
        tainted = set()
        for line in lines:
            for g, s in strs.items():
                if s == "parse_types" and g in line:
                    d = re.match(r"\s*(%[\w.]+) =", line)
                    if d:
                        tainted.add(d.group(1))
            if re.search(r"%parse_types\b", line):
                d = re.match(r"\s*(%[\w.]+) =", line)
                if d:
                    tainted.add(d.group(1))
        changed = True
        while changed:
            changed = False
            for line in lines:
                d = re.match(r"\s*(%[\w.]+) = (icmp|select|and|or|xor|zext|phi|load)\b", line)
                if (
                    d
                    and d.group(1) not in tainted
                    and any(re.search(re.escape(t) + r"\b", line) for t in tainted)
                ):
                    tainted.add(d.group(1))
                    changed = True
        label = "entry"
        for line in lines:
            lm = re.match(r"^([\w.]+):", line)
            if lm:
                label = lm.group(1)
            if (
                "call" in line
                and ("parse_types" in line or "10parse_types" in line)
                and "@" in line
            ):
                callee = re.search(r"@([\w.$]+)\(", line)
                if callee and "parse_types" in callee.group(1):
                    print(f"   call in block {label}: {callee.group(1)}")
            br = re.match(r"\s*br i1 (%[\w.]+), label %([\w.]+), label %([\w.]+)", line)
            if br and br.group(1) in tainted:
                pm = re.search(r"!prof (![0-9]+)", line)
                w = md.get(pm.group(1), pm.group(1)) if pm else "no !prof"
                print(f"   block {label}: br {br.group(1)} -> {br.group(2)} | {br.group(3)}  {w}")


if __name__ == "__main__":
    main()

"""M15 hot-path item 6: `import strata` time, A (main) vs B (ad04f61), fresh interpreters.

A = ~/worktrees/strata/main-pgo/.venv (38eaa9f facade, armA2 installed)
B = ~/worktrees/strata/m15-pgo/.venv  (ad04f61 facade, armBown2 installed)
Each round launches A, B, B, A; each launch is a fresh interpreter (cwd = a neutral
directory) that times `import strata` in-process with perf_counter_ns; the parent also
records the launch's wall time. Round effect = (B1+B2)/(A1+A2) - 1; point = median of
round effects; 95% interval = percentile bootstrap over rounds (10 000 resamples, seed 7).
Also prints, per arm, which of numpy/datetime/uuid/decimal/dataclasses are in
sys.modules after `import strata` (and before it), and B's extra modules over A's.
usage: python3 import_abba.py <out.tsv> [rounds=60]
"""

from __future__ import annotations

import json
import os
import random
import statistics
import subprocess
import sys
import tempfile
import time

ARMS = {  # M15_ARM_A / M15_ARM_B override (the A/A control points both at one venv)
    "A": os.path.expanduser(
        os.environ.get("M15_ARM_A", "~/worktrees/strata/main-pgo/.venv/bin/python")
    ),
    "B": os.path.expanduser(
        os.environ.get("M15_ARM_B", "~/worktrees/strata/m15-pgo/.venv/bin/python")
    ),
}
WATCH = ("numpy", "datetime", "uuid", "decimal", "dataclasses")
TIMED = (
    "import time, sys\n"
    "t = time.perf_counter_ns()\n"
    "import strata\n"
    "print(time.perf_counter_ns() - t)\n"
)
MODS = (
    "import sys, json\n"
    "before = set(sys.modules)\n"
    "import strata\n"
    "after = set(sys.modules)\n"
    f"w = {WATCH!r}\n"
    "print(json.dumps({'before': [m for m in w if m in before], 'after': [m for m in w if m in after],"
    " 'new': sorted(after - before), 'file': strata.__file__, 'version': strata.__version__}))\n"
)


def launch(py: str, code: str, cwd: str) -> tuple[str, int]:
    t0 = time.perf_counter_ns()
    out = subprocess.run([py, "-c", code], cwd=cwd, capture_output=True, text=True, check=True)
    return out.stdout.strip(), time.perf_counter_ns() - t0


def boot(effects: list[float], n: int = 10000, seed: int = 7) -> tuple[float, float]:
    rng = random.Random(seed)
    meds = sorted(statistics.median(rng.choices(effects, k=len(effects))) for _ in range(n))
    return meds[int(0.025 * n)], meds[int(0.975 * n) - 1]


def main() -> int:
    out_path = sys.argv[1]
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 60
    cwd = tempfile.mkdtemp(prefix="m15-import-")
    mods = {tag: json.loads(launch(py, MODS, cwd)[0]) for tag, py in ARMS.items()}
    for tag in ARMS:  # warm the page cache and __pycache__
        for _ in range(3):
            launch(ARMS[tag], TIMED, cwd)
    rows = []
    eff_imp, eff_wall = [], []
    for r in range(rounds):
        vals = {}
        for i, tag in enumerate("ABBA"):
            s, wall = launch(ARMS[tag], TIMED, cwd)
            rows.append((r, i, tag, int(s), wall))
            vals.setdefault(tag, []).append((int(s), wall))
        a_imp = sum(v[0] for v in vals["A"])
        b_imp = sum(v[0] for v in vals["B"])
        a_wall = sum(v[1] for v in vals["A"])
        b_wall = sum(v[1] for v in vals["B"])
        eff_imp.append(b_imp / a_imp - 1)
        eff_wall.append(b_wall / a_wall - 1)
    with open(out_path, "w") as fh:
        fh.write("round\tpos\tarm\timport_ns\twall_ns\n")
        for row in rows:
            fh.write("\t".join(map(str, row)) + "\n")
    for label, eff, col in (("import", eff_imp, 3), ("wall", eff_wall, 4)):
        lo, hi = boot(eff)
        med = {t: statistics.median(r[col] for r in rows if r[2] == t) for t in "AB"}
        q = {t: statistics.quantiles([r[col] for r in rows if r[2] == t], n=4) for t in "AB"}
        pos = sum(e > 0 for e in eff)
        print(
            f"{label}: A median {med['A'] / 1e3:.1f} us (IQR {q['A'][0] / 1e3:.1f}-{q['A'][2] / 1e3:.1f}),"
            f" B median {med['B'] / 1e3:.1f} us (IQR {q['B'][0] / 1e3:.1f}-{q['B'][2] / 1e3:.1f});"
            f" effect {statistics.median(eff) * 100:+.2f}% [{lo * 100:+.2f}, {hi * 100:+.2f}]"
            f" rounds={len(eff)} B>A in {pos}"
        )
    for tag, m in mods.items():
        print(
            f"sys.modules {tag}: watched before={m['before']} after={m['after']} "
            f"file={m['file']} version={m['version']}"
        )
    extra = sorted(set(mods["B"]["new"]) - set(mods["A"]["new"]))
    missing = sorted(set(mods["A"]["new"]) - set(mods["B"]["new"]))
    print(
        f"modules new on import: A {len(mods['A']['new'])}, B {len(mods['B']['new'])}; "
        f"B extra {extra}; B missing {missing}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

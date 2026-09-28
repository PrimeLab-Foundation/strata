"""The facade share the pinned A/B cannot see: ad04f61's `loads`/`load` pass a third keyword
(`parse_types=False`) to the native call, and the A/B runs both .so under main's facade
(main's _strata rejects that keyword). Same process, same .so (B's venv), two wrappers:
  facade-B   strata.loads(src)                       -- ad04f61's facade, 3 keywords
  facade-A   main_loads(src)                         -- main 38eaa9f's facade body, 2 keywords
Payloads: b'{"a":1}' (tiny) and small mixed.json (the canonical row's bytes).
Each sample = one batch (~3 ms) of calls, ns/call; order alternates per repeat; gc.collect()
before each batch. Prints median ns/call per wrapper and the median of paired differences.
usage: python facade_kw.py <path to small mixed.json> [repeat=61]
"""

from __future__ import annotations

import gc
import statistics
import sys
import time

import strata
from strata import _strata as _native


def main_loads(source, *, return_type="dict", iterator=False):
    return _native.loads(source, return_type=return_type, iterator=iterator)


def main() -> int:
    mixed = open(sys.argv[1], "rb").read()
    repeat = int(sys.argv[2]) if len(sys.argv) > 2 else 61
    for label, src in (("tiny", b'{"a":1}'), ("small-mixed", mixed)):
        fns = {"facade-B": lambda: strata.loads(src), "facade-A": lambda: main_loads(src)}
        assert fns["facade-B"]() == fns["facade-A"]()
        t0 = time.perf_counter_ns()
        fns["facade-A"]()
        one = time.perf_counter_ns() - t0
        n = max(1, int(3e6 / max(one, 1)))
        res = {k: [] for k in fns}
        for r in range(repeat):
            order = list(fns) if r % 2 == 0 else list(fns)[::-1]
            for k in order:
                gc.collect()
                t0 = time.perf_counter_ns()
                for _ in range(n):
                    fns[k]()
                res[k].append((time.perf_counter_ns() - t0) / n)
        diffs = [b - a for b, a in zip(res["facade-B"], res["facade-A"])]
        ma, mb = statistics.median(res["facade-A"]), statistics.median(res["facade-B"])
        qd = statistics.quantiles(diffs, n=4)
        print(
            f"{label}: facade-A {ma:.1f} ns/call, facade-B {mb:.1f} ns/call, paired diff median "
            f"{statistics.median(diffs):+.1f} ns (IQR {qd[0]:+.1f}..{qd[2]:+.1f}), "
            f"{(mb / ma - 1) * 100:+.3f}% ; n/batch={n} repeats={repeat}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

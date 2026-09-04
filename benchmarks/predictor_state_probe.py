"""Does a parse's warm time depend on what the process parsed before it?

The builder's key predictor keeps a per-depth table of record shapes and
retires a depth that thrashes. This probe parses mixed.json warm in a fresh
state, then after alternating the four other small-tier datasets with it,
then after hundreds of mixed-only parses, then after the others alone -- for
strata and orjson. A library whose warm time rises after the alternations
and never recovers carries state across documents that the benchmark
harness's dataset order will always trigger (found 2026-09-04: 128 -> 145
us per parse before the per-input reset of the retirement accounting).

usage: PYTHONPATH=. .venv/bin/python benchmarks/predictor_state_probe.py
"""

import gc, time
import orjson, strata

D = "benchmarks/data/generated/small/"
mixed = open(D + "mixed.json", "rb").read()
others = [
    open(D + n, "rb").read() for n in ("users.json", "flat.json", "nested.json", "wide_arrays.json")
]


def med(fn, data, n=60):
    t = []
    for _ in range(n):
        gc.collect()
        t0 = time.perf_counter()
        r = fn(data)
        del r
        t.append((time.perf_counter() - t0) * 1e6)
    t.sort()
    return t[n // 2]


for lib, fn in (("strata", strata.loads), ("orjson", orjson.loads)):
    for _ in range(20):
        fn(mixed)
    print(f"{lib}: mixed warm, fresh process state        {med(fn, mixed):6.1f} us")
    for _ in range(50):
        for o in others:
            fn(o)
        fn(mixed)
    print(f"{lib}: mixed warm after 50 alternations      {med(fn, mixed):6.1f} us")
    for _ in range(300):
        fn(mixed)
    print(f"{lib}: mixed warm after 300 more mixed parses {med(fn, mixed):6.1f} us")
    for _ in range(50):
        for o in others:
            fn(o)
    print(f"{lib}: mixed warm after 50x others only       {med(fn, mixed):6.1f} us")

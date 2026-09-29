"""Attribute facade_kw.py's residual (same .so): which part of strata.loads(tiny) vs main's
2-keyword body costs what. One process, rotating order, gc.collect() per batch.
usage: python facade_split.py [repeat=41]
"""

import gc
import statistics
import sys
import time

import strata
from strata import _strata as _native

src = b'{"a":1}'


def main2(source, *, return_type="dict", iterator=False):
    return _native.loads(source, return_type=return_type, iterator=iterator)


def py3_pass2(source, *, return_type="dict", iterator=False, parse_types=False):
    return _native.loads(source, return_type=return_type, iterator=iterator)


def py3_pass3(source, *, return_type="dict", iterator=False, parse_types=False):
    return _native.loads(
        source,
        return_type=return_type,
        iterator=iterator,
        parse_types=parse_types,
    )


facade = strata.loads
fns = {
    "main2 (A)": lambda: main2(src),
    "py3_pass2": lambda: py3_pass2(src),
    "py3_pass3": lambda: py3_pass3(src),
    "facade global": lambda: facade(src),
    "strata.loads (B)": lambda: strata.loads(src),
    "native 2kw": lambda: _native.loads(src, return_type="dict", iterator=False),
    "native 3kw": lambda: _native.loads(src, return_type="dict", iterator=False, parse_types=False),
}
repeat = int(sys.argv[1]) if len(sys.argv) > 1 else 41
n = 20000
res = {k: [] for k in fns}
names = list(fns)
for r in range(repeat):
    k0 = r % len(names)
    for k in names[k0:] + names[:k0]:
        f = fns[k]
        gc.collect()
        t0 = time.perf_counter_ns()
        for _ in range(n):
            f()
        res[k].append((time.perf_counter_ns() - t0) / n)
base = res["main2 (A)"]
for k in names:
    d = [a - b for a, b in zip(res[k], base)]
    print(
        f"{k:18s} {statistics.median(res[k]):7.1f} ns  paired vs A {statistics.median(d):+6.1f} ns"
    )

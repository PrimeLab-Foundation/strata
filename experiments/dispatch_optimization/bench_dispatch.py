"""
Dispatch optimization benchmark.

Measures the effect of:
  - Structural tape ON vs OFF (STRATA_USE_STRUCTURAL_TAPE)
  - Static (CRTP template) dispatch vs virtual dispatch

Run as: .venv/bin/python experiments/dispatch_optimization/bench_dispatch.py [dataset] [n]

dataset: small | medium | large  (default: large)
n:       number of iterations    (default: 100)

Control variables:
  STRATA_USE_STRUCTURAL_TAPE=0|1   (default: 0, tape OFF)
  STRATA_PYTHON_EXACT_SIZE_HINTS=0|1|auto  (default: auto >= 1MB)
  STRATA_DEFERRED_GC_TRACK=0|1    (default: 1, ON for >= 256KB)
"""

import gc
import os
import statistics
import sys
import time

import strata

DATASET_PATHS = {
    "small": "benchmarks/data/generated/small/users.json",
    "medium": "benchmarks/data/generated/medium/users.json",
    "large": "benchmarks/data/generated/large/users.json",
}


def bench(data: bytes, n: int = 100, trim: int = 3) -> dict:
    # Warmup
    for _ in range(5):
        strata.loads(data)

    gc.disable()
    times = []
    for _ in range(n + trim * 2):
        t0 = time.perf_counter()
        result = strata.loads(data)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
        del result
    gc.enable()

    times.sort()
    times = times[trim:-trim]
    p95_idx = max(0, int(len(times) * 0.95) - 1)
    return {
        "min": min(times),
        "median": statistics.median(times),
        "p95": times[p95_idx],
        "n": len(times),
    }


def main():
    dataset = sys.argv[1] if len(sys.argv) > 1 else "large"
    n = int(sys.argv[2]) if len(sys.argv) > 2 else 100

    if dataset not in DATASET_PATHS:
        print(f"Unknown dataset '{dataset}'. Choose from: {list(DATASET_PATHS)}")
        sys.exit(1)

    path = DATASET_PATHS[dataset]
    data = open(path, "rb").read()

    tape = os.environ.get("STRATA_USE_STRUCTURAL_TAPE", "default(off)")
    hints = os.environ.get("STRATA_PYTHON_EXACT_SIZE_HINTS", "auto")
    gc_track = os.environ.get("STRATA_DEFERRED_GC_TRACK", "default(on)")

    r = bench(data, n=n)
    print(
        f"{dataset} ({len(data) / 1e6:.1f}MB) "
        f"[tape={tape} hints={hints} gc_track={gc_track}]: "
        f"min={r['min']:.3f}ms  median={r['median']:.3f}ms  p95={r['p95']:.3f}ms  n={r['n']}"
    )


if __name__ == "__main__":
    main()

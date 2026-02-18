#!/usr/bin/env python3
"""
Benchmark for Approach A: Deferred GC tracking.

Controls:
  STRATA_DEFERRED_GC_TRACK=0  baseline (GC tracking always on)
  STRATA_DEFERRED_GC_TRACK=1  experiment (untrack during construction)

Usage:
  # Baseline
  STRATA_DEFERRED_GC_TRACK=0 python experiments/gc_optimization/bench_gc_optimization.py

  # With optimization
  STRATA_DEFERRED_GC_TRACK=1 python experiments/gc_optimization/bench_gc_optimization.py
"""
import gc
import os
import sys
import time

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", ".."))

import strata

DATA_ROOT = os.path.join(os.path.dirname(__file__), "..", "..", "benchmarks", "data", "generated")
DATASETS = [
    ("small",  os.path.join(DATA_ROOT, "small",  "users.json"), 20),
    ("medium", os.path.join(DATA_ROOT, "medium", "users.json"), 30),
    ("large",  os.path.join(DATA_ROOT, "large",  "users.json"), 15),
]


def bench(data: bytes, n: int = 20) -> tuple[float, float, float, float]:
    # Warmup
    for _ in range(5):
        r = strata.loads(data)
        del r
    gc.collect()

    times = []
    for _ in range(n):
        start = time.perf_counter()
        r = strata.loads(data)
        t = time.perf_counter() - start
        times.append(t * 1000)
        del r

    times.sort()
    avg = sum(times) / len(times)
    return times[0], times[n // 2], times[int(n * 0.95)], avg


def main():
    flag = os.environ.get("STRATA_DEFERRED_GC_TRACK", "1")
    mode = "ON (deferred GC)" if flag != "0" else "OFF (baseline)"
    print(f"Deferred GC tracking: {mode}")
    print(f"Python: {sys.version.split()[0]}")
    print()
    print(f"{'Dataset':<10} {'N':>4} {'Min':>8} {'Median':>8} {'P95':>8} {'Avg':>8}  (ms)")
    print("-" * 55)

    for label, path, n in DATASETS:
        if not os.path.exists(path):
            print(f"  {label}: data not found at {path}")
            continue
        with open(path, "rb") as f:
            data = f.read()
        mn, med, p95, avg = bench(data, n)
        print(f"{label:<10} {n:>4} {mn:>8.1f} {med:>8.1f} {p95:>8.1f} {avg:>8.1f}")


if __name__ == "__main__":
    main()

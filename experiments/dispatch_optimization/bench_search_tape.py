"""
Benchmark search/query performance with structural tape ON vs OFF.
Run with: .venv/bin/python experiments/dispatch_optimization/bench_search_tape.py
Reports search() and query() median latency for all datasets.
"""

import gc
import os
import statistics
import time

import strata

DATASETS = {
    "small":  "benchmarks/data/generated/small/users.json",
    "medium": "benchmarks/data/generated/medium/users.json",
    "large":  "benchmarks/data/generated/large/users.json",
}

QUERIES = [
    "$.users[*].name",
    "$.users[*].age",
    "$.users[*].metadata.city",
    "$.users[*].orders[*].total",
]

N_ITER = 30
TRIM   = 2


def bench_search(source: bytes, query: str, n: int = N_ITER) -> dict:
    for _ in range(3):
        strata.search(source, query)
    gc.disable()
    times = []
    for _ in range(n + TRIM * 2):
        t0 = time.perf_counter()
        strata.search(source, query)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
    gc.enable()
    times.sort()
    times = times[TRIM:-TRIM]
    return {"min": min(times), "median": statistics.median(times), "n": len(times)}


def main():
    tape = os.environ.get("STRATA_USE_STRUCTURAL_TAPE", "default(off)")
    print(f"tape={tape}\n{'='*60}")
    for dsname, path in DATASETS.items():
        data = open(path, "rb").read()
        print(f"\n{dsname} ({len(data)/1e6:.1f} MB):")
        for q in QUERIES:
            r = bench_search(data, q)
            print(f"  {q:<35s}  min={r['min']:7.2f}ms  med={r['median']:7.2f}ms")


if __name__ == "__main__":
    main()

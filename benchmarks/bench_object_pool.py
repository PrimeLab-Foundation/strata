#!/usr/bin/env python3
"""Benchmark: Object pool ON vs OFF for dict pre-allocation.

Compares strata.loads() performance with and without the dict object pool
across small, medium, and large JSON files.

Usage:
    python -m benchmarks.bench_object_pool
    python -m benchmarks.bench_object_pool --iterations 20
"""

import argparse
import gc
import json
import os
import statistics
import sys
import time

import strata


def bench_loads(data: str, iterations: int, warmup: int = 3) -> dict:
    """Benchmark strata.loads() and return timing statistics."""
    # Warmup
    for _ in range(warmup):
        strata.loads(data)

    gc.collect()
    gc.disable()
    times = []
    for _ in range(iterations):
        t0 = time.perf_counter_ns()
        strata.loads(data)
        t1 = time.perf_counter_ns()
        times.append((t1 - t0) / 1e6)  # ms
    gc.enable()

    return {
        "min_ms": min(times),
        "median_ms": statistics.median(times),
        "mean_ms": statistics.mean(times),
        "p95_ms": sorted(times)[int(len(times) * 0.95)],
        "stdev_ms": statistics.stdev(times) if len(times) > 1 else 0,
    }


def main():
    parser = argparse.ArgumentParser(description="Object pool benchmark")
    parser.add_argument("--iterations", type=int, default=10, help="Iterations per test")
    args = parser.parse_args()

    datasets = {}

    # Generated test data
    base = os.path.join(os.path.dirname(__file__), "data", "generated")
    for label, path in [
        ("small (1MB)", os.path.join(base, "small", "users.json")),
        ("medium (6.5MB)", os.path.join(base, "medium", "users.json")),
        ("generated (5MB)", os.path.join(base, "users.json")),
        ("large (44MB)", os.path.join(base, "large", "users.json")),
    ]:
        if os.path.exists(path):
            with open(path) as f:
                datasets[label] = f.read()

    # Also test with a small inline payload (below pool threshold)
    small_payload = json.dumps([{"id": i, "name": f"user{i}", "tags": [1, 2, 3]} for i in range(100)])
    datasets["tiny (15KB)"] = small_payload

    if not datasets:
        print("No benchmark data found. Generate with: python -m benchmarks.generate_data")
        return

    print(f"{'=' * 80}")
    print(f"Object Pool Benchmark (iterations={args.iterations})")
    print(f"Pool threshold: 4KB (inputs >= 4KB activate pooling)")
    print(f"{'=' * 80}")
    print()

    # Compare pool ON (default) vs pool OFF (STRATA_OBJECT_POOL_SIZE=0)
    for label, data in sorted(datasets.items(), key=lambda x: len(x[1])):
        size_mb = len(data) / (1024 * 1024)
        print(f"--- {label} ({size_mb:.2f} MB) ---")

        # Pool ON (default)
        result_on = bench_loads(data, args.iterations)

        print(f"  Pool ON:  min={result_on['min_ms']:.2f}ms  "
              f"median={result_on['median_ms']:.2f}ms  "
              f"p95={result_on['p95_ms']:.2f}ms  "
              f"stdev={result_on['stdev_ms']:.2f}ms")

        # For comparison, also show orjson if available
        try:
            import orjson
            data_bytes = data.encode() if isinstance(data, str) else data

            gc.collect()
            gc.disable()
            times = []
            for _ in range(3):
                orjson.loads(data_bytes)
            for _ in range(args.iterations):
                t0 = time.perf_counter_ns()
                orjson.loads(data_bytes)
                t1 = time.perf_counter_ns()
                times.append((t1 - t0) / 1e6)
            gc.enable()

            orjson_median = statistics.median(times)
            ratio = result_on['median_ms'] / orjson_median
            throughput_strata = size_mb / (result_on['median_ms'] / 1000)
            throughput_orjson = size_mb / (orjson_median / 1000)
            print(f"  orjson:   min={min(times):.2f}ms  "
                  f"median={orjson_median:.2f}ms  "
                  f"ratio={ratio:.2f}x")
            print(f"  Throughput: strata={throughput_strata:.1f} MB/s  "
                  f"orjson={throughput_orjson:.1f} MB/s")
        except ImportError:
            pass

        print()

    print(f"{'=' * 80}")
    print("Note: Pool is automatically enabled for inputs >= 4KB.")
    print("Configure pool size: STRATA_OBJECT_POOL_SIZE=<N> (default: 1024)")
    print(f"{'=' * 80}")


if __name__ == "__main__":
    main()

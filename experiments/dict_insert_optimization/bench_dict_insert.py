"""
Micro-benchmark for dict insertion optimization.

Measures strata.loads() performance on datasets with varying dict characteristics
to isolate the impact of dict insertion strategy changes.

Usage:
    .venv/bin/python experiments/dict_insert_optimization/bench_dict_insert.py
"""
import json
import os
import statistics
import subprocess
import sys
import time

# Ensure strata is importable
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

def generate_flat_objects(n_objects: int, n_keys: int) -> str:
    """Array of flat objects — targets dict insertion directly."""
    objects = []
    for i in range(n_objects):
        obj = {f"key_{k}": f"value_{i}_{k}" for k in range(n_keys)}
        objects.append(obj)
    return json.dumps(objects)


def generate_nested_objects(n_objects: int, depth: int, keys_per_level: int) -> str:
    """Nested objects — targets recursive dict construction."""
    def make_obj(d):
        if d == 0:
            return {"leaf": True, "value": 42}
        return {f"level_{d}_key_{k}": make_obj(d - 1) for k in range(keys_per_level)}
    objects = [make_obj(depth) for _ in range(n_objects)]
    return json.dumps(objects)


def bench_loads(data: str, label: str, iterations: int = 50, warmup: int = 5):
    """Benchmark strata.loads with timing."""
    import strata

    data_bytes = data.encode('utf-8') if isinstance(data, str) else data
    size_mb = len(data_bytes) / (1024 * 1024)

    # Warmup
    for _ in range(warmup):
        strata.loads(data)

    times = []
    for _ in range(iterations):
        t0 = time.perf_counter_ns()
        strata.loads(data)
        t1 = time.perf_counter_ns()
        times.append((t1 - t0) / 1e6)  # ms

    med = statistics.median(times)
    mn = min(times)
    p95 = sorted(times)[int(len(times) * 0.95)]
    throughput = size_mb / (med / 1000) if med > 0 else 0

    print(f"  {label}: min={mn:.3f}ms  median={med:.3f}ms  p95={p95:.3f}ms  "
          f"throughput={throughput:.1f} MB/s  ({size_mb:.2f} MB, {iterations} iters)")
    return {"label": label, "min": mn, "median": med, "p95": p95,
            "throughput": throughput, "size_mb": size_mb}


def bench_from_file(path: str, label: str, iterations: int = 20, warmup: int = 3):
    """Benchmark strata.loads on a file dataset."""
    import strata
    with open(path, 'r') as f:
        data = f.read()
    return bench_loads(data, label, iterations, warmup)


def main():
    print("=" * 70)
    print("Dict Insertion Optimization Benchmark")
    print("=" * 70)

    # Synthetic benchmarks
    print("\n--- Synthetic: Flat objects (dict insertion stress test) ---")
    data_3k = generate_flat_objects(1000, 3)
    data_10k = generate_flat_objects(1000, 10)
    data_25k = generate_flat_objects(1000, 25)
    data_50k = generate_flat_objects(500, 50)

    bench_loads(data_3k, "1000 objs × 3 keys")
    bench_loads(data_10k, "1000 objs × 10 keys")
    bench_loads(data_25k, "1000 objs × 25 keys")
    bench_loads(data_50k, "500 objs × 50 keys")

    print("\n--- Synthetic: Nested objects (recursive dict construction) ---")
    data_nested = generate_nested_objects(100, 3, 3)
    bench_loads(data_nested, "100 objs × depth=3 × 3 keys")

    # Real dataset benchmarks
    datasets = [
        ("benchmarks/data/generated/small/users.json", "small (0.96MB)"),
        ("benchmarks/data/generated/medium/users.json", "medium (6.25MB)"),
        ("benchmarks/data/generated/large/users.json", "large (43.85MB)"),
    ]

    print("\n--- Real datasets ---")
    for path, label in datasets:
        full_path = os.path.join(os.path.dirname(__file__), '..', '..', path)
        if os.path.exists(full_path):
            iters = 10 if "large" in label else 20
            bench_from_file(full_path, label, iterations=iters, warmup=3)
        else:
            print(f"  {label}: SKIPPED (file not found: {path})")


if __name__ == '__main__':
    main()

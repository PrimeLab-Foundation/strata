"""
Benchmark for string optimization experiments.
Tests Approach A (ValueCache) and Approach B (Direct ASCII PyUnicode_New).

Run as: python experiments/string_optimization/bench_strings.py
"""
import os
import sys
import time
import gc
import statistics

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

STRATA_AVAILABLE = False
try:
    import strata
    STRATA_AVAILABLE = True
except ImportError:
    pass

ORJSON_AVAILABLE = False
try:
    import orjson
    ORJSON_AVAILABLE = True
except ImportError:
    pass


def get_dataset_path(label):
    base = os.path.join(os.path.dirname(__file__), '..', '..')
    if label == 'large':
        return os.path.join(base, 'benchmarks/data/generated/large/users.json')
    elif label == 'medium':
        return os.path.join(base, 'benchmarks/data/generated/medium/users.json')
    elif label == 'small':
        return os.path.join(base, 'benchmarks/data/generated/small/users.json')
    raise ValueError(f"Unknown dataset: {label}")


def bench_loads(data_bytes, n_warmup=3, n_runs=10):
    """Benchmark loads() with warmup."""
    parse_fn = strata.loads

    # Warmup
    for _ in range(n_warmup):
        gc.collect()
        parse_fn(data_bytes)

    times = []
    for _ in range(n_runs):
        gc.collect()
        t0 = time.perf_counter()
        parse_fn(data_bytes)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)

    return times


def print_stats(label, dataset, times, size_mb):
    mn = min(times)
    med = statistics.median(times)
    p95 = sorted(times)[int(len(times) * 0.95)]
    tp = size_mb / (med / 1000)
    print(f"  {label:30s} | {mn:8.2f} | {med:8.2f} | {p95:8.2f} | {tp:8.1f}")


def main():
    datasets = [
        ('small', 'small'),
        ('medium', 'medium'),
        ('large', 'large'),
    ]

    print(f"\n{'='*80}")
    print(f"String Optimization Benchmark")
    print(f"{'='*80}")
    print(f"\n  {'Library/Config':30s} | {'Min(ms)':>8} | {'Med(ms)':>8} | {'P95(ms)':>8} | {'MB/s':>8}")
    print(f"  {'-'*72}")

    for (dataset_label, _) in datasets:
        path = get_dataset_path(dataset_label)
        if not os.path.exists(path):
            print(f"\n  Dataset {dataset_label}: NOT FOUND at {path}")
            continue

        size_mb = os.path.getsize(path) / (1024 * 1024)
        print(f"\n  Dataset: {dataset_label} ({size_mb:.2f} MB)")
        print(f"  {'-'*72}")

        with open(path, 'rb') as f:
            data_bytes = f.read()

        if STRATA_AVAILABLE:
            # Strata baseline
            times = bench_loads(data_bytes)
            print_stats("strata (current)", dataset_label, times, size_mb)

        if ORJSON_AVAILABLE:
            # orjson baseline for reference
            orjson_times = []
            for _ in range(3):
                gc.collect()
                orjson.loads(data_bytes)
            for _ in range(10):
                gc.collect()
                t0 = time.perf_counter()
                orjson.loads(data_bytes)
                t1 = time.perf_counter()
                orjson_times.append((t1 - t0) * 1000)
            print_stats("orjson", dataset_label, orjson_times, size_mb)

    print(f"\n{'='*80}")


if __name__ == '__main__':
    main()

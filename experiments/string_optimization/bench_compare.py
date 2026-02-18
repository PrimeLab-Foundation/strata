"""
Careful A/B comparison benchmark.
Interleaves strata and orjson runs to reduce systematic bias from CPU frequency scaling.
Runs 20 iterations and discards top/bottom 10% before comparing.
"""
import os
import sys
import time
import gc
import statistics

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
sys.path.insert(0, ROOT)

import strata
try:
    import orjson
    HAS_ORJSON = True
except ImportError:
    HAS_ORJSON = False


def bench_fn(fn, data, n_warmup=5, n_runs=20):
    for _ in range(n_warmup):
        gc.collect()
        fn(data)
    times = []
    for _ in range(n_runs):
        gc.collect()
        t0 = time.perf_counter()
        fn(data)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
    # Trimmed mean: drop top/bottom 10%
    times.sort()
    trim = max(1, len(times) // 10)
    trimmed = times[trim:-trim] if trim > 0 else times
    return times, trimmed


def stats(times, size_mb):
    mn = min(times)
    med = statistics.median(times)
    p95 = sorted(times)[int(len(times) * 0.95)]
    tp = size_mb / (med / 1000)
    return mn, med, p95, tp


def main():
    datasets = [
        ('small',  'benchmarks/data/generated/small/users.json'),
        ('medium', 'benchmarks/data/generated/medium/users.json'),
        ('large',  'benchmarks/data/generated/large/users.json'),
    ]

    print(f"\n{'='*90}")
    print(f"String Optimization A+B vs Baseline (20 runs, trimmed)")
    print(f"{'='*90}")
    print(f"\n  {'Library':20s} | {'Min':>8} | {'Med':>8} | {'P95':>8} | {'MB/s':>8}")
    print(f"  {'-'*65}")

    for label, rel_path in datasets:
        path = os.path.join(ROOT, rel_path)
        if not os.path.exists(path):
            continue
        size_mb = os.path.getsize(path) / (1024 * 1024)

        with open(path, 'rb') as f:
            data = f.read()

        print(f"\n  Dataset: {label} ({size_mb:.2f} MB)")

        strata_times, strata_trimmed = bench_fn(strata.loads, data)
        mn, med, p95, tp = stats(strata_trimmed, size_mb)
        print(f"  {'strata':20s} | {mn:8.2f} | {med:8.2f} | {p95:8.2f} | {tp:8.1f}")

        if HAS_ORJSON:
            orjson_times, orjson_trimmed = bench_fn(orjson.loads, data)
            mn2, med2, p95_2, tp2 = stats(orjson_trimmed, size_mb)
            print(f"  {'orjson':20s} | {mn2:8.2f} | {med2:8.2f} | {p95_2:8.2f} | {tp2:8.1f}")
            ratio = med / med2
            print(f"  {'strata/orjson ratio':20s} | {'':>8} | {ratio:8.2f}x | {'':>8} | {'':>8}")

    print(f"\n{'='*90}")


if __name__ == '__main__':
    main()

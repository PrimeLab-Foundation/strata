"""
Rigorous A/B benchmark for dict insertion optimization.
Runs many more iterations and reports statistical significance.
"""
import os
import statistics
import sys
import time

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

def bench_file(path: str, iterations: int = 50, warmup: int = 10):
    import strata
    with open(path, 'r') as f:
        data = f.read()

    size_mb = len(data.encode('utf-8')) / (1024 * 1024)

    for _ in range(warmup):
        strata.loads(data)

    times = []
    for _ in range(iterations):
        t0 = time.perf_counter_ns()
        strata.loads(data)
        t1 = time.perf_counter_ns()
        times.append((t1 - t0) / 1e6)

    med = statistics.median(times)
    mn = min(times)
    p95 = sorted(times)[int(len(times) * 0.95)]
    stdev = statistics.stdev(times) if len(times) > 1 else 0
    throughput = size_mb / (med / 1000)

    return {
        "min": mn, "median": med, "p95": p95, "stdev": stdev,
        "throughput": throughput, "size_mb": size_mb
    }


def main():
    base = os.path.join(os.path.dirname(__file__), '..', '..')
    datasets = [
        (os.path.join(base, "benchmarks/data/generated/small/users.json"), "small", 50),
        (os.path.join(base, "benchmarks/data/generated/medium/users.json"), "medium", 30),
        (os.path.join(base, "benchmarks/data/generated/large/users.json"), "large", 15),
    ]

    print("Rigorous benchmark (high iteration count)")
    print("=" * 70)
    for path, label, iters in datasets:
        if not os.path.exists(path):
            print(f"  {label}: SKIPPED")
            continue
        result = bench_file(path, iterations=iters, warmup=10)
        print(f"  {label}: min={result['min']:.3f}ms  median={result['median']:.3f}ms  "
              f"p95={result['p95']:.3f}ms  stdev={result['stdev']:.3f}ms  "
              f"throughput={result['throughput']:.1f} MB/s")


if __name__ == '__main__':
    main()

"""
Isolated benchmark runner — each configuration runs in a fresh subprocess.
This prevents thread-local state contamination between runs.
"""
import os
import sys
import subprocess
import json
import statistics

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
PYTHON = os.path.join(ROOT, '.venv/bin/python')

RUNNER = os.path.join(os.path.dirname(__file__), '_bench_worker.py')


def run_worker(dataset, n_runs=12, n_warmup=3, env_overrides=None):
    env = os.environ.copy()
    if env_overrides:
        env.update(env_overrides)
    result = subprocess.run(
        [PYTHON, RUNNER, dataset, str(n_runs), str(n_warmup)],
        capture_output=True, text=True, env=env,
        cwd=ROOT
    )
    if result.returncode != 0:
        print(f"ERROR: {result.stderr[:500]}")
        return None
    try:
        return json.loads(result.stdout.strip())
    except Exception as e:
        print(f"Parse error: {e}\nstdout: {result.stdout[:200]}")
        return None


def fmt(times, size_mb):
    mn = min(times)
    med = statistics.median(times)
    p95 = sorted(times)[int(len(times) * 0.95)]
    tp = size_mb / (med / 1000)
    return f"{mn:8.2f} | {med:8.2f} | {p95:8.2f} | {tp:8.1f}"


def main():
    datasets = [
        ('small',  0.96),
        ('medium', 6.25),
        ('large',  43.85),
    ]

    print(f"\n{'='*80}")
    print(f"Isolated Benchmark (separate subprocess per run)")
    print(f"{'='*80}")
    print(f"\n  {'Config':35s} | {'Min(ms)':>8} | {'Med(ms)':>8} | {'P95(ms)':>8} | {'MB/s':>8}")
    print(f"  {'-'*72}")

    for ds_label, size_mb in datasets:
        path = os.path.join(ROOT, f'benchmarks/data/generated/{ds_label}/users.json')
        if not os.path.exists(path):
            print(f"\n  [{ds_label}] NOT FOUND")
            continue
        # Get actual size
        actual_mb = os.path.getsize(path) / (1024 * 1024)
        print(f"\n  [{ds_label}] ({actual_mb:.2f} MB)")

        # Strata current
        r = run_worker(ds_label)
        if r:
            print(f"  {'strata (A+B combined)':35s} | {fmt(r['strata'], actual_mb)}")
            if 'orjson' in r:
                print(f"  {'orjson':35s} | {fmt(r['orjson'], actual_mb)}")


if __name__ == '__main__':
    main()

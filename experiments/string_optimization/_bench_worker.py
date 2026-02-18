"""Worker subprocess for isolated benchmark runs."""
import sys
import os
import json
import time
import gc
import statistics

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
sys.path.insert(0, ROOT)

dataset = sys.argv[1] if len(sys.argv) > 1 else 'medium'
n_runs = int(sys.argv[2]) if len(sys.argv) > 2 else 12
n_warmup = int(sys.argv[3]) if len(sys.argv) > 3 else 3

path = os.path.join(ROOT, f'benchmarks/data/generated/{dataset}/users.json')
with open(path, 'rb') as f:
    data = f.read()

results = {}

import strata
for _ in range(n_warmup):
    gc.collect()
    strata.loads(data)
times = []
for _ in range(n_runs):
    gc.collect()
    t0 = time.perf_counter()
    strata.loads(data)
    t1 = time.perf_counter()
    times.append((t1 - t0) * 1000)
results['strata'] = times

try:
    import orjson
    for _ in range(n_warmup):
        gc.collect()
        orjson.loads(data)
    otimes = []
    for _ in range(n_runs):
        gc.collect()
        t0 = time.perf_counter()
        orjson.loads(data)
        t1 = time.perf_counter()
        otimes.append((t1 - t0) * 1000)
    results['orjson'] = otimes
except ImportError:
    pass

print(json.dumps(results))

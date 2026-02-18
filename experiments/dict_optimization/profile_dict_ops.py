"""
Detailed profiling of dict operations during JSON parsing.
Measures dict creation, insertion, and deallocation costs.
"""
import os
import sys
import json
import cProfile
import pstats
import io
from contextlib import redirect_stdout

ROOT = os.path.join(os.path.dirname(__file__), '..', '..')
sys.path.insert(0, ROOT)

import strata


def profile_dict_operations(dataset_label='large'):
    """Profile dict operations in detail."""
    base = os.path.join(ROOT, 'benchmarks/data/generated')
    path = os.path.join(base, f'{dataset_label}/users.json')

    if not os.path.exists(path):
        print(f"Dataset not found: {path}")
        return

    with open(path, 'rb') as f:
        data = f.read()

    print(f"\n{'='*80}")
    print(f"Dict Operation Profiling: {dataset_label} dataset")
    print(f"File size: {len(data) / (1024*1024):.2f} MB")
    print(f"{'='*80}\n")

    # Profile with cProfile
    pr = cProfile.Profile()
    pr.enable()

    result = strata.loads(data)

    pr.disable()

    # Get stats
    s = io.StringIO()
    ps = pstats.Stats(pr, stream=s).sort_stats('cumulative')
    ps.print_stats(50)  # Top 50 functions

    output = s.getvalue()

    # Filter for dict-related operations
    lines = output.split('\n')
    print("Top dict-related operations:")
    print(f"{'Function':<50} {'Calls':>12} {'Time':>12}")
    print("-" * 75)

    dict_patterns = ['dict', 'Dict', 'PyDict', 'setitem', 'getitem', 'hash']
    for line in lines:
        for pattern in dict_patterns:
            if pattern in line:
                print(line)
                break

    print("\n" + output)


if __name__ == '__main__':
    for dataset in ['small', 'medium', 'large']:
        profile_dict_operations(dataset)

"""
Analyze string value distributions in benchmark datasets.
Determines whether value string caching/interning is worthwhile.

Step 1 of the string optimization experiment (Rule 12).
"""
import json
import os
import sys
from collections import Counter

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))
sys.setrecursionlimit(100000)


def collect_strings(obj, strings, keys):
    """Recursively collect all string values (not keys) and keys from a JSON object."""
    if isinstance(obj, dict):
        for k, v in obj.items():
            keys.append(k)
            collect_strings(v, strings, keys)
    elif isinstance(obj, list):
        for v in obj:
            collect_strings(v, strings, keys)
    elif isinstance(obj, str):
        strings.append(obj)


def analyze_cache_simulation(values, cache_sizes=(64, 256, 1024, 4096)):
    """Simulate direct-mapped hash cache at various sizes."""
    results = {}
    for size in cache_sizes:
        cache = [None] * size
        hits = 0
        for v in values:
            h = hash(v) & 0xFFFFFFFFFFFFFFFF
            slot = h % size
            if cache[slot] == v:
                hits += 1
            else:
                cache[slot] = v
        results[size] = hits / len(values) if values else 0.0
    return results


def analyze_strings(strings, label="string values"):
    """Analyze string value distribution and cache simulation."""
    if not strings:
        print(f"  No {label} found")
        return

    total = len(strings)
    counter = Counter(strings)
    unique = len(counter)
    dedup = total / unique if unique > 0 else 0

    # Length distribution
    lengths = [len(s) for s in strings]
    short = sum(1 for l in lengths if l <= 32)
    medium = sum(1 for l in lengths if 32 < l <= 64)
    long_ = sum(1 for l in lengths if l > 64)

    # ASCII vs non-ASCII
    ascii_count = sum(1 for s in strings if all(ord(c) < 128 for c in s))

    # Empty strings
    empty_count = sum(1 for s in strings if len(s) == 0)

    print(f"  Total {label}:       {total:>10,}")
    print(f"  Unique values:          {unique:>10,}")
    print(f"  Dedup ratio:            {dedup:>10.1f}x")
    print(f"  Empty strings:          {empty_count:>10,} ({100*empty_count/total:.1f}%)")
    print(f"  ASCII only:             {ascii_count:>10,} ({100*ascii_count/total:.1f}%)")
    print(f"  Length <= 32 (short):   {short:>10,} ({100*short/total:.1f}%)")
    print(f"  Length 33-64 (medium):  {medium:>10,} ({100*medium/total:.1f}%)")
    print(f"  Length > 64 (long):     {long_:>10,} ({100*long_/total:.1f}%)")
    print()

    # For short strings, simulate cache
    short_strings = [s for s in strings if len(s) <= 32]
    if short_strings:
        hit_rates = analyze_cache_simulation(short_strings)
        print(f"  Cache simulation (strings <= 32 bytes, n={len(short_strings):,}):")
        for size, rate in hit_rates.items():
            print(f"    {size:>6}-entry cache: {100*rate:.1f}% hit rate")
        print()

    # Also simulate for all strings
    hit_rates_all = analyze_cache_simulation(strings[:min(len(strings), 500000)])
    print(f"  Cache simulation (all strings, n={min(len(strings),500000):,}):")
    for size, rate in hit_rates_all.items():
        print(f"    {size:>6}-entry cache: {100*rate:.1f}% hit rate")
    print()

    print(f"  Top 30 most common {label}:")
    for val, cnt in counter.most_common(30):
        pct = 100 * cnt / total
        is_ascii = all(ord(c) < 128 for c in val)
        print(f"    {repr(val)[:40]:>42} : {cnt:>8,} ({pct:5.2f}%)  [{'ASCII' if is_ascii else 'UTF-8'}, len={len(val)}]")


def main():
    base = os.path.join(os.path.dirname(__file__), '..', '..')
    datasets = [
        (os.path.join(base, "benchmarks/data/generated/small/users.json"), "small"),
        (os.path.join(base, "benchmarks/data/generated/medium/users.json"), "medium"),
        (os.path.join(base, "benchmarks/data/generated/large/users.json"), "large"),
    ]

    for path, dataset_label in datasets:
        if not os.path.exists(path):
            print(f"\n{'='*70}")
            print(f"Dataset: {dataset_label} — SKIPPED (not found)")
            continue

        size_mb = os.path.getsize(path) / (1024 * 1024)
        print(f"\n{'='*70}")
        print(f"Dataset: {dataset_label} ({size_mb:.2f} MB)")
        print(f"{'='*70}")

        print(f"Loading {path}...")
        with open(path) as f:
            data = json.load(f)

        strings = []
        keys = []
        collect_strings(data, strings, keys)

        print(f"\nString VALUE Analysis:")
        print(f"{'-'*50}")
        analyze_strings(strings, "string values")

        print(f"\nDict KEY Analysis (for comparison):")
        print(f"{'-'*50}")
        analyze_strings(keys, "dict keys")


if __name__ == '__main__':
    main()

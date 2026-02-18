"""
Analyze integer and float value distributions in benchmark datasets.
Determines whether caching/deduplication approaches are worthwhile.
"""
import json
import os
import struct
import sys
from collections import Counter

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))
sys.setrecursionlimit(10000)


def collect_values(obj, ints, floats):
    """Recursively collect all int and float values from a JSON object."""
    if isinstance(obj, dict):
        for v in obj.values():
            collect_values(v, ints, floats)
    elif isinstance(obj, list):
        for v in obj:
            collect_values(v, ints, floats)
    elif isinstance(obj, int) and not isinstance(obj, bool):
        ints.append(obj)
    elif isinstance(obj, float):
        floats.append(obj)


def analyze_ints(values):
    """Analyze integer value distribution."""
    if not values:
        print("  No integers found")
        return

    total = len(values)
    counter = Counter(values)
    unique = len(counter)

    small_cache = sum(1 for v in values if -5 <= v <= 256)
    long_range = sum(1 for v in values if v < -5 or v > 256)
    beyond_long = sum(1 for v in values if v > 2**63 - 1 or v < -(2**63))

    print(f"  Total integers:      {total:>10,}")
    print(f"  Unique values:       {unique:>10,}")
    print(f"  Dedup ratio:         {total / unique:>10.1f}x")
    print(f"  In small cache [-5,256]: {small_cache:>7,} ({100*small_cache/total:.1f}%)")
    print(f"  Outside small cache:     {long_range:>7,} ({100*long_range/total:.1f}%)")
    print(f"  Beyond int64:            {beyond_long:>7,} ({100*beyond_long/total:.1f}%)")
    print()
    print("  Top 20 most common integer values:")
    for val, cnt in counter.most_common(20):
        pct = 100 * cnt / total
        in_cache = "CACHED" if -5 <= val <= 256 else "alloc"
        print(f"    {val:>10} : {cnt:>8,} ({pct:5.2f}%)  [{in_cache}]")


def analyze_floats(values):
    """Analyze float value distribution."""
    if not values:
        print("  No floats found")
        return

    total = len(values)

    # Count unique by bit pattern (exact dedup)
    bit_patterns = Counter()
    for v in values:
        bits = struct.pack('d', v)
        bit_patterns[bits] += 1

    unique = len(bit_patterns)
    dedup_ratio = total / unique if unique > 0 else 0

    # Common values
    value_counter = Counter(values)
    zeros = sum(1 for v in values if v == 0.0)
    ones = sum(1 for v in values if v == 1.0)
    neg_ones = sum(1 for v in values if v == -1.0)

    print(f"  Total floats:        {total:>10,}")
    print(f"  Unique bit patterns: {unique:>10,}")
    print(f"  Dedup ratio:         {dedup_ratio:>10.1f}x")
    print(f"  Zeros (0.0):         {zeros:>10,} ({100*zeros/total:.1f}%)")
    print(f"  Ones (1.0):          {ones:>10,} ({100*ones/total:.1f}%)")
    print(f"  Neg ones (-1.0):     {neg_ones:>10,} ({100*neg_ones/total:.1f}%)")
    print()

    # Cache simulation: 64-entry direct-mapped with Fibonacci hash
    cache_hits = 0
    cache_misses = 0
    cache = [None] * 64
    FIB = 0x9E3779B97F4A7C15
    for v in values:
        bits_int = struct.unpack('Q', struct.pack('d', v))[0]
        slot = ((bits_int * FIB) & 0xFFFFFFFFFFFFFFFF) >> 58
        if cache[slot] == bits_int:
            cache_hits += 1
        else:
            cache_misses += 1
            cache[slot] = bits_int

    hit_rate = 100 * cache_hits / total if total > 0 else 0
    print(f"  64-entry cache simulation:")
    print(f"    Hits:   {cache_hits:>10,} ({hit_rate:.1f}%)")
    print(f"    Misses: {cache_misses:>10,} ({100-hit_rate:.1f}%)")
    print()

    print("  Top 20 most common float values:")
    for val, cnt in value_counter.most_common(20):
        pct = 100 * cnt / total
        print(f"    {val:>12} : {cnt:>8,} ({pct:5.2f}%)")


def main():
    base = os.path.join(os.path.dirname(__file__), '..', '..')
    datasets = [
        (os.path.join(base, "benchmarks/data/generated/small/users.json"), "small"),
        (os.path.join(base, "benchmarks/data/generated/medium/users.json"), "medium"),
        (os.path.join(base, "benchmarks/data/generated/large/users.json"), "large"),
    ]

    for path, label in datasets:
        if not os.path.exists(path):
            print(f"\n{'='*70}")
            print(f"Dataset: {label} — SKIPPED (not found)")
            continue

        size_mb = os.path.getsize(path) / (1024 * 1024)
        print(f"\n{'='*70}")
        print(f"Dataset: {label} ({size_mb:.2f} MB)")
        print(f"{'='*70}")

        print(f"Loading {path}...")
        with open(path) as f:
            data = json.load(f)

        ints = []
        floats = []
        collect_values(data, ints, floats)

        print(f"\nInteger Analysis:")
        print(f"{'-'*50}")
        analyze_ints(ints)

        print(f"\nFloat Analysis:")
        print(f"{'-'*50}")
        analyze_floats(floats)


if __name__ == '__main__':
    main()

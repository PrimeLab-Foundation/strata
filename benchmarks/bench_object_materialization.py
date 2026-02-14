#!/usr/bin/env python3
"""
Micro-benchmark isolating Python object creation overhead.

This benchmark measures the cost of materializing Python objects (dicts, lists,
strings, ints, floats) from pre-parsed C++ data structures, compared to full
JSON parsing including both parse+materialize phases.

The goal is to understand what percentage of total parsing time is spent in
Python object creation vs. actual parsing logic.
"""

from __future__ import annotations

import argparse
import json
import statistics
import time
from pathlib import Path
from typing import Any

try:
    import orjson
except ImportError:
    orjson = None

import strata


def load_and_analyze_json(data_path: Path) -> dict[str, Any]:
    """Load JSON and analyze its structure."""
    print(f"Loading and analyzing: {data_path}")
    data = data_path.read_bytes()
    size_mb = len(data) / 1024 / 1024
    print(f"Data size: {size_mb:.2f} MB")

    # Parse once to analyze structure
    parsed = strata.loads(data)

    # Count object types
    stats = count_objects(parsed)

    print(f"\nObject statistics:")
    print(f"  Dicts:   {stats['dicts']:,}")
    print(f"  Lists:   {stats['lists']:,}")
    print(f"  Strings: {stats['strings']:,}")
    print(f"  Ints:    {stats['ints']:,}")
    print(f"  Floats:  {stats['floats']:,}")
    print(f"  Bools:   {stats['bools']:,}")
    print(f"  Nulls:   {stats['nulls']:,}")
    print(f"  Total:   {stats['total']:,}")

    return {
        'data': data,
        'size_mb': size_mb,
        'parsed': parsed,
        'stats': stats,
    }


def count_objects(obj: Any, stats: dict[str, int] | None = None) -> dict[str, int]:
    """Recursively count Python objects by type."""
    if stats is None:
        stats = {
            'dicts': 0,
            'lists': 0,
            'strings': 0,
            'ints': 0,
            'floats': 0,
            'bools': 0,
            'nulls': 0,
            'total': 0,
        }

    stats['total'] += 1

    if isinstance(obj, dict):
        stats['dicts'] += 1
        for k, v in obj.items():
            count_objects(k, stats)
            count_objects(v, stats)
    elif isinstance(obj, list):
        stats['lists'] += 1
        for item in obj:
            count_objects(item, stats)
    elif isinstance(obj, str):
        stats['strings'] += 1
    elif isinstance(obj, bool):
        stats['bools'] += 1
    elif isinstance(obj, int):
        stats['ints'] += 1
    elif isinstance(obj, float):
        stats['floats'] += 1
    elif obj is None:
        stats['nulls'] += 1

    return stats


def benchmark_parse_materialize(data: bytes, iterations: int = 10, warmup: int = 2) -> tuple[float, float, float]:
    """Benchmark full parse+materialize (strata.loads)."""
    print(f"\nBenchmarking strata.loads (parse + materialize)...")

    # Warmup
    for _ in range(warmup):
        _ = strata.loads(data)

    # Measure
    times = []
    for i in range(iterations):
        start = time.perf_counter()
        result = strata.loads(data)
        elapsed = time.perf_counter() - start
        times.append(elapsed * 1000)  # Convert to ms

        if i == 0:
            print(f"  First run: {times[0]:.2f}ms")

    min_time = min(times)
    median_time = statistics.median(times)
    mean_time = statistics.mean(times)

    print(f"  Min:    {min_time:.2f}ms")
    print(f"  Median: {median_time:.2f}ms")
    print(f"  Mean:   {mean_time:.2f}ms")

    return min_time, median_time, mean_time


def benchmark_orjson(data: bytes, iterations: int = 10, warmup: int = 2) -> tuple[float, float, float] | None:
    """Benchmark orjson.loads for comparison."""
    if orjson is None:
        print("\norjson not available, skipping")
        return None

    print(f"\nBenchmarking orjson.loads (parse + materialize)...")

    # Warmup
    for _ in range(warmup):
        _ = orjson.loads(data)

    # Measure
    times = []
    for i in range(iterations):
        start = time.perf_counter()
        result = orjson.loads(data)
        elapsed = time.perf_counter() - start
        times.append(elapsed * 1000)  # Convert to ms

        if i == 0:
            print(f"  First run: {times[0]:.2f}ms")

    min_time = min(times)
    median_time = statistics.median(times)
    mean_time = statistics.mean(times)

    print(f"  Min:    {min_time:.2f}ms")
    print(f"  Median: {median_time:.2f}ms")
    print(f"  Mean:   {mean_time:.2f}ms")

    return min_time, median_time, mean_time


def benchmark_materialize_only(parsed_obj: Any, iterations: int = 10, warmup: int = 2) -> tuple[float, float, float]:
    """
    Benchmark pure object materialization (reconstruction from parsed data).

    This simulates materializing Python objects from pre-parsed C++ structures
    by recursively rebuilding the object graph.
    """
    print(f"\nBenchmarking materialization-only (object reconstruction)...")

    # Warmup
    for _ in range(warmup):
        _ = reconstruct_object(parsed_obj)

    # Measure
    times = []
    for i in range(iterations):
        start = time.perf_counter()
        result = reconstruct_object(parsed_obj)
        elapsed = time.perf_counter() - start
        times.append(elapsed * 1000)  # Convert to ms

        if i == 0:
            print(f"  First run: {times[0]:.2f}ms")

    min_time = min(times)
    median_time = statistics.median(times)
    mean_time = statistics.mean(times)

    print(f"  Min:    {min_time:.2f}ms")
    print(f"  Median: {median_time:.2f}ms")
    print(f"  Mean:   {mean_time:.2f}ms")

    return min_time, median_time, mean_time


def reconstruct_object(obj: Any) -> Any:
    """
    Recursively reconstruct Python object graph.

    This simulates the materialization phase by creating new Python objects
    (PyDict_New, PyList_New, etc.) from already-parsed data.
    """
    if isinstance(obj, dict):
        # Simulate PyDict_New + PyDict_SetItem for each key-value
        return {reconstruct_object(k): reconstruct_object(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        # Simulate PyList_New + PyList_SetItem for each element
        return [reconstruct_object(item) for item in obj]
    elif isinstance(obj, str):
        # Simulate PyUnicode_FromStringAndSize
        return str(obj)
    elif isinstance(obj, int):
        # Simulate PyLong_FromLongLong
        return int(obj)
    elif isinstance(obj, float):
        # Simulate PyFloat_FromDouble
        return float(obj)
    elif isinstance(obj, bool):
        # Simulate Py_True/Py_False reference
        return bool(obj)
    elif obj is None:
        # Simulate Py_None reference
        return None
    else:
        return obj


def print_summary(info: dict[str, Any], strata_times: tuple, orjson_times: tuple | None, materialize_times: tuple) -> None:
    """Print summary comparison."""
    print("\n" + "="*80)
    print("SUMMARY - Materialization Overhead Analysis")
    print("="*80)
    print()

    strata_min, strata_median, strata_mean = strata_times
    mat_min, mat_median, mat_mean = materialize_times

    # Calculate materialization percentage
    mat_pct = (mat_median / strata_median) * 100
    parse_pct = 100 - mat_pct

    print(f"Data: {info['data'].decode('utf-8')[:50] if isinstance(info['data'], bytes) else str(info['data'])[:50]}...")
    print(f"Size: {info['size_mb']:.2f} MB")
    print(f"Objects: {info['stats']['total']:,}")
    print()

    print(f"{'Benchmark':<40} {'Min (ms)':<12} {'Median (ms)':<12} {'Mean (ms)':<12}")
    print("-" * 80)
    print(f"{'Strata (parse + materialize)':<40} {strata_min:<12.2f} {strata_median:<12.2f} {strata_mean:<12.2f}")
    print(f"{'Materialization only':<40} {mat_min:<12.2f} {mat_median:<12.2f} {mat_mean:<12.2f}")

    if orjson_times:
        orjson_min, orjson_median, orjson_mean = orjson_times
        print(f"{'orjson (parse + materialize)':<40} {orjson_min:<12.2f} {orjson_median:<12.2f} {orjson_mean:<12.2f}")

    print("-" * 80)
    print()

    print("Breakdown (based on median times):")
    print(f"  Materialization overhead: {mat_median:.2f}ms ({mat_pct:.1f}%)")
    print(f"  Parsing logic:            {strata_median - mat_median:.2f}ms ({parse_pct:.1f}%)")
    print()

    if orjson_times:
        orjson_median = orjson_times[1]
        strata_vs_orjson = (strata_median / orjson_median) * 100
        mat_vs_orjson = (mat_median / orjson_median) * 100

        print(f"Comparison to orjson:")
        print(f"  Strata total:        {strata_vs_orjson:.1f}% of orjson time")
        print(f"  Materialization:     {mat_vs_orjson:.1f}% of orjson time")
        print()

        if mat_median < orjson_median:
            print(f"  Note: Materialization-only is {orjson_median/mat_median:.2f}x faster than orjson's total time!")
            print(f"        This suggests strata's parsing phase is the bottleneck.")
        else:
            print(f"  Note: Materialization-only is {mat_median/orjson_median:.2f}x slower than orjson's total time.")
            print(f"        This suggests Python object creation is a significant bottleneck.")

    print()
    print("Interpretation:")
    if mat_pct > 50:
        print(f"  - Materialization is the dominant cost ({mat_pct:.1f}%)")
        print(f"  - Consider optimizing Python object creation (batching, caching, etc.)")
    elif mat_pct > 30:
        print(f"  - Materialization is a significant cost ({mat_pct:.1f}%)")
        print(f"  - Both parsing and materialization could be optimized")
    else:
        print(f"  - Parsing logic dominates ({parse_pct:.1f}%)")
        print(f"  - Focus optimization efforts on parsing, not object creation")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Benchmark Python object materialization overhead"
    )
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("benchmarks/data/generated/large/users.json"),
        help="Path to JSON file",
    )
    parser.add_argument(
        "--iterations",
        type=int,
        default=10,
        help="Number of iterations per benchmark",
    )
    parser.add_argument(
        "--warmup",
        type=int,
        default=2,
        help="Number of warmup iterations",
    )
    parser.add_argument(
        "--compare-orjson",
        action="store_true",
        help="Also benchmark orjson for comparison",
    )
    args = parser.parse_args()

    if not args.data.exists():
        print(f"Error: Data file not found: {args.data}")
        return 1

    print("="*80)
    print("Python Object Materialization Overhead Benchmark")
    print("="*80)

    # Load and analyze
    info = load_and_analyze_json(args.data)

    # Benchmark strata parse+materialize
    strata_times = benchmark_parse_materialize(
        info['data'],
        args.iterations,
        args.warmup
    )

    # Benchmark orjson if requested
    orjson_times = None
    if args.compare_orjson:
        orjson_times = benchmark_orjson(
            info['data'],
            args.iterations,
            args.warmup
        )

    # Benchmark materialization-only
    materialize_times = benchmark_materialize_only(
        info['parsed'],
        args.iterations,
        args.warmup
    )

    # Print summary
    print_summary(info, strata_times, orjson_times, materialize_times)

    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main())

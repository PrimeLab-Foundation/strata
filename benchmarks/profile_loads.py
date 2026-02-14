#!/usr/bin/env python3
"""
Profile strata.loads on large datasets using cProfile.

This script profiles strata.loads performance and generates detailed statistics
about where time is being spent during JSON parsing.
"""

from __future__ import annotations

import argparse
import cProfile
import pstats
import sys
from pathlib import Path
from io import StringIO

import strata

try:
    import orjson
except ImportError:
    orjson = None


def profile_strata_loads(data_path: Path, iterations: int = 10) -> pstats.Stats:
    """Profile strata.loads and return stats object."""
    print(f"Loading data from {data_path}...")
    data = data_path.read_bytes()
    size_mb = len(data) / 1024 / 1024
    print(f"Data size: {size_mb:.2f} MB")
    print(f"Profiling strata.loads for {iterations} iterations...")

    profiler = cProfile.Profile()
    profiler.enable()

    for _ in range(iterations):
        result = strata.loads(data)

    profiler.disable()

    stats = pstats.Stats(profiler)
    return stats


def profile_orjson_loads(data_path: Path, iterations: int = 10) -> pstats.Stats | None:
    """Profile orjson.loads for comparison."""
    if orjson is None:
        return None

    print(f"\nProfiling orjson.loads for {iterations} iterations...")
    data = data_path.read_bytes()

    profiler = cProfile.Profile()
    profiler.enable()

    for _ in range(iterations):
        result = orjson.loads(data)

    profiler.disable()

    stats = pstats.Stats(profiler)
    return stats


def print_top_hotspots(stats: pstats.Stats, n: int = 10, title: str = "Top Hotspots") -> list[tuple[str, float, float]]:
    """Print top N hotspots and return list of (function, cumtime_pct, tottime_pct)."""
    stats.strip_dirs()

    # Get total time
    stats_stream = StringIO()
    stats.stream = stats_stream
    stats.print_stats()
    stats_output = stats_stream.getvalue()

    # Parse for total time
    for line in stats_output.split('\n'):
        if 'function calls' in line and 'seconds' in line.lower():
            parts = line.split()
            for i, part in enumerate(parts):
                if 'seconds' in part.lower() or part == 'CPU':
                    try:
                        total_time = float(parts[i-1])
                        break
                    except (ValueError, IndexError):
                        continue
            else:
                total_time = 1.0
            break
    else:
        total_time = 1.0

    # Get top functions by cumulative time
    stats.sort_stats(pstats.SortKey.CUMULATIVE)

    print(f"\n{'='*80}")
    print(f"{title}")
    print(f"{'='*80}")
    print(f"{'Rank':<6} {'CumTime%':<10} {'TotTime%':<10} {'CumTime(s)':<12} {'Calls':<10} {'Function'}")
    print("-" * 80)

    hotspots = []

    # Manually extract top functions
    func_list = []
    for func, (cc, nc, tt, ct, callers) in stats.stats.items():
        func_list.append((func, cc, nc, tt, ct))

    # Sort by cumulative time
    func_list.sort(key=lambda x: x[4], reverse=True)

    for i, (func, cc, nc, tt, ct) in enumerate(func_list[:n], 1):
        filename, line, func_name = func
        cumtime_pct = (ct / total_time) * 100 if total_time > 0 else 0
        tottime_pct = (tt / total_time) * 100 if total_time > 0 else 0

        # Simplify filename
        if 'strata' in filename:
            filename = filename.split('strata')[-1].lstrip('/')
        elif filename.startswith('/'):
            filename = Path(filename).name

        func_display = f"{filename}:{func_name}"
        if len(func_display) > 50:
            func_display = func_display[:47] + "..."

        print(f"{i:<6} {cumtime_pct:<10.2f} {tottime_pct:<10.2f} {ct:<12.6f} {cc:<10} {func_display}")

        hotspots.append((func_display, cumtime_pct, tottime_pct))

    print("-" * 80)
    print(f"Total profiled time: {total_time:.4f}s")
    print()

    return hotspots


def analyze_python_object_creation(stats: pstats.Stats) -> dict[str, float]:
    """Extract time spent in Python object creation APIs."""
    object_creation_funcs = {
        'PyDict_New': 0.0,
        'PyList_New': 0.0,
        'PyUnicode_FromStringAndSize': 0.0,
        'PyLong_FromLongLong': 0.0,
        'PyFloat_FromDouble': 0.0,
        'PyObject_': 0.0,  # Catch-all for PyObject_* functions
    }

    print("\n" + "="*80)
    print("Python Object Creation Overhead Analysis")
    print("="*80)

    total_time = sum(ct for func, (cc, nc, tt, ct, callers) in stats.stats.items())

    for func, (cc, nc, tt, ct, callers) in stats.stats.items():
        filename, line, func_name = func

        # Check if this is a Python C API object creation function
        for key in object_creation_funcs.keys():
            if key in func_name or key.lower() in filename.lower():
                object_creation_funcs[key] += ct

    # Print breakdown
    total_obj_creation = sum(object_creation_funcs.values())
    obj_creation_pct = (total_obj_creation / total_time * 100) if total_time > 0 else 0

    print(f"{'Function Pattern':<40} {'Time(s)':<12} {'% of Total':<12}")
    print("-" * 80)

    for func_name, time_spent in sorted(object_creation_funcs.items(), key=lambda x: x[1], reverse=True):
        if time_spent > 0:
            pct = (time_spent / total_time * 100) if total_time > 0 else 0
            print(f"{func_name:<40} {time_spent:<12.6f} {pct:<12.2f}")

    print("-" * 80)
    print(f"{'Total Object Creation Overhead':<40} {total_obj_creation:<12.6f} {obj_creation_pct:<12.2f}")
    print()

    return object_creation_funcs


def save_profile_data(stats: pstats.Stats, output_path: Path) -> None:
    """Save profile data to file for later analysis."""
    print(f"Saving profile data to {output_path}...")
    stats.dump_stats(str(output_path))
    print(f"Profile data saved. Load with: python -m pstats {output_path}")


def main() -> int:
    parser = argparse.ArgumentParser(description="Profile strata.loads with cProfile")
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
        help="Number of iterations to profile",
    )
    parser.add_argument(
        "--output",
        type=Path,
        help="Save profile data to file",
    )
    parser.add_argument(
        "--compare-orjson",
        action="store_true",
        help="Also profile orjson for comparison",
    )
    parser.add_argument(
        "--top-n",
        type=int,
        default=10,
        help="Show top N hotspots",
    )
    args = parser.parse_args()

    if not args.data.exists():
        print(f"Error: Data file not found: {args.data}")
        return 1

    # Profile strata
    strata_stats = profile_strata_loads(args.data, args.iterations)
    strata_hotspots = print_top_hotspots(strata_stats, args.top_n, "Strata Loads - Top Hotspots")
    analyze_python_object_creation(strata_stats)

    # Save profile data if requested
    if args.output:
        save_profile_data(strata_stats, args.output)

    # Compare with orjson if requested
    if args.compare_orjson:
        orjson_stats = profile_orjson_loads(args.data, args.iterations)
        if orjson_stats:
            orjson_hotspots = print_top_hotspots(orjson_stats, args.top_n, "orjson Loads - Top Hotspots")

    return 0


if __name__ == "__main__":
    sys.exit(main())

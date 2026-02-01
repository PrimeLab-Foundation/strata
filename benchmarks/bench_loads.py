#!/usr/bin/env python3
"""
JSON parsing (loads) benchmarks.

Compares Strata loads against orjson, ujson, msgspec, pysimdjson, and stdlib json.
Uses shared harness for timing and RSS. Pair with bench_dumps.py (serialize).
"""

from __future__ import annotations

import argparse
import json
import statistics
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable

from .harness import run_single_benchmark

try:
    import orjson
except ImportError:
    orjson = None
try:
    import msgspec
except ImportError:
    msgspec = None
try:
    import ujson
except ImportError:
    ujson = None
try:
    import simdjson
except ImportError:
    simdjson = None

import strata


@dataclass
class LoadsResult:
    """Result for one loads (parse) benchmark run."""

    library: str
    min_ms: float
    median_ms: float
    p95_ms: float
    rss_mb: float


def _p95(times_ms: list[float]) -> float:
    if len(times_ms) >= 20:
        return statistics.quantiles(times_ms, n=20)[18]
    return max(times_ms) if times_ms else 0.0


def _get_loads_runners() -> list[tuple[str, Callable[[bytes], Any]]]:
    """Return [(library_name, parse_func)] where parse_func(bytes) -> parsed value."""

    def strata_run(data: bytes):
        return strata.loads(data)

    def orjson_run(data: bytes):
        return orjson.loads(data)

    def msgspec_run(data: bytes):
        return msgspec.json.decode(data)

    def ujson_run(data: bytes):
        return ujson.loads(data)

    _simdjson_parser = simdjson.Parser() if simdjson else None

    def pysimdjson_run(data: bytes):
        return _simdjson_parser.parse(data)

    def stdlib_run(data: bytes):
        return json.loads(data)

    runners: list[tuple[str, Callable[[bytes], Any]]] = [("strata", strata_run)]
    if orjson is not None:
        runners.append(("orjson", orjson_run))
    if msgspec is not None:
        runners.append(("msgspec", msgspec_run))
    if ujson is not None:
        runners.append(("ujson", ujson_run))
    if simdjson is not None:
        runners.append(("pysimdjson", pysimdjson_run))
    runners.append(("json (stdlib)", stdlib_run))
    return runners


def run_benchmarks(
    data_file: Path,
    *,
    repeat: int = 10,
    warmup: int = 2,
) -> list[LoadsResult]:
    """Run loads (parse) benchmarks; return list of LoadsResult."""
    data = data_file.read_bytes()
    size_mb = len(data) / 1024 / 1024

    print()
    print("=" * 70)
    print("JSON Parsing (loads) Benchmarks")
    print("=" * 70)
    print(f"Data file: {data_file}")
    print(f"Input size: {size_mb:.2f} MB")
    print(f"Repeat: {repeat}, Warmup: {warmup}")
    print()

    results: list[LoadsResult] = []
    for library_name, parse_func in _get_loads_runners():
        print(f"--- Benchmarking {library_name} ---")
        try:
            tr = run_single_benchmark(
                lambda: parse_func(data),
                warmup=warmup,
                repeat=repeat,
                capture_rss=True,
            )
            result = LoadsResult(
                library=library_name,
                min_ms=tr.min_ms,
                median_ms=tr.median_ms,
                p95_ms=_p95(tr.times_ms),
                rss_mb=tr.rss_mb,
            )
            results.append(result)
            print(
                f"  min={result.min_ms:.2f}ms, median={result.median_ms:.2f}ms, "
                f"p95={result.p95_ms:.2f}ms, rss={result.rss_mb:.1f} MB"
            )
        except Exception as e:
            print(f"  ERROR: {e}")

    return results


def print_summary(results: list[LoadsResult]) -> None:
    """Print summary table and Strata rank."""
    if not results:
        print("No results to display.")
        return

    results.sort(key=lambda r: r.median_ms)
    baseline = results[0]

    print()
    print("=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print()
    print(
        f"{'Library':<15} {'Min (ms)':>10} {'Median (ms)':>12} {'P95 (ms)':>10} {'RSS (MB)':>9} {'Speedup':>10}"
    )
    print("-" * 70)

    for r in results:
        speedup = baseline.median_ms / r.median_ms
        print(
            f"{r.library:<15} {r.min_ms:>10.2f} {r.median_ms:>12.2f} {r.p95_ms:>10.2f} {r.rss_mb:>9.1f} {speedup:.2f}x"
        )

    strata_result = next((r for r in results if r.library == "strata"), None)
    if strata_result:
        rank = results.index(strata_result) + 1
        print()
        print(f"Strata: Rank #{rank} / {len(results)}")
        if rank == 1 and len(results) > 1:
            second = results[1]
            pct = (second.median_ms / strata_result.median_ms - 1) * 100
            print(f"  -> {pct:.1f}% faster than #{2} ({second.library})")
        elif rank > 1:
            first = results[0]
            pct = (strata_result.median_ms / first.median_ms - 1) * 100
            print(f"  -> {pct:.1f}% behind #{1} ({first.library})")


def main() -> int:
    parser = argparse.ArgumentParser(description="Benchmark JSON parsing (loads)")
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("benchmarks/data/generated/users.json"),
        help="Path to JSON file",
    )
    parser.add_argument("--repeat", type=int, default=10, help="Iterations per benchmark")
    parser.add_argument("--warmup", type=int, default=2, help="Warmup iterations")
    args = parser.parse_args()

    if not args.data.exists():
        print(f"Error: Data file not found: {args.data}")
        print("Generate with: python -m benchmarks.data.generate_bench_data")
        return 1

    results = run_benchmarks(args.data, repeat=args.repeat, warmup=args.warmup)
    print_summary(results)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

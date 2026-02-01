#!/usr/bin/env python3
"""
JSON serialization (dumps) benchmarks.

Compares Strata dumps/dumps_bytes against orjson, msgspec, ujson, and stdlib json.
Uses shared harness for timing and RSS.
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

import strata


@dataclass
class DumpsResult:
    """Result for one dumps benchmark run."""

    library: str
    min_ms: float
    median_ms: float
    p95_ms: float
    output_size: int
    rss_mb: float


def _p95(times_ms: list[float]) -> float:
    if len(times_ms) >= 20:
        return statistics.quantiles(times_ms, n=20)[18]
    return max(times_ms) if times_ms else 0.0


def _get_dumps_runners(bytes_mode: bool) -> list[tuple[str, Callable[[Any], Any]]]:
    """Return [(library_name, func)] where func(data) returns serialized (str or bytes)."""

    def strata_run(data):
        return strata.dumps_bytes(data) if bytes_mode else strata.dumps(data)

    def orjson_run(data):
        out = orjson.dumps(data)
        return out if bytes_mode else out.decode("utf-8")

    def msgspec_run(data):
        out = msgspec.json.encode(data)
        return out if bytes_mode else out.decode("utf-8")

    def ujson_run(data):
        out = ujson.dumps(data)
        return out.encode("utf-8") if bytes_mode else out

    def stdlib_run(data):
        out = json.dumps(data)
        return out.encode("utf-8") if bytes_mode else out

    runners = [("strata", strata_run)]
    if orjson is not None:
        runners.append(("orjson", orjson_run))
    if msgspec is not None:
        runners.append(("msgspec", msgspec_run))
    if ujson is not None:
        runners.append(("ujson", ujson_run))
    runners.append(("json", stdlib_run))
    return runners


def run_benchmarks(
    data_file: Path,
    *,
    repeat: int = 50,
    warmup: int = 3,
    bytes_mode: bool = False,
) -> list[DumpsResult]:
    """Run dumps benchmarks; return list of DumpsResult."""
    with open(data_file, encoding="utf-8") as f:
        data = json.load(f)

    mode_label = "dumps_bytes" if bytes_mode else "dumps"
    print()
    print("=" * 70)
    print(f"JSON Serialization ({mode_label}) Benchmarks")
    print("=" * 70)
    print(f"Data file: {data_file}")
    print(f"Input size: {data_file.stat().st_size / 1024 / 1024:.2f} MB")
    print(f"Repeat: {repeat}, Warmup: {warmup}")
    print()

    results: list[DumpsResult] = []
    runners = _get_dumps_runners(bytes_mode)

    for library_name, dump_func in runners:
        print(f"--- Benchmarking {library_name} ---")
        try:
            tr = run_single_benchmark(
                lambda: dump_func(data),
                warmup=warmup,
                repeat=repeat,
                capture_rss=True,
            )
            # Output size from last run
            output = dump_func(data)
            output_size = len(output)
            result = DumpsResult(
                library=library_name,
                min_ms=tr.min_ms,
                median_ms=tr.median_ms,
                p95_ms=_p95(tr.times_ms),
                output_size=output_size,
                rss_mb=tr.rss_mb,
            )
            results.append(result)
            print(
                f"  min={result.min_ms:.2f}ms, median={result.median_ms:.2f}ms, "
                f"p95={result.p95_ms:.2f}ms, size={result.output_size} bytes, rss={result.rss_mb:.1f} MB"
            )
        except Exception as e:
            print(f"  ERROR: {e}")

    return results


def print_summary(results: list[DumpsResult]) -> None:
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
        f"{'Library':<15} {'Min (ms)':>10} {'Median (ms)':>12} {'P95 (ms)':>10} {'Size (bytes)':>12} {'RSS (MB)':>9} {'Speedup':>10}"
    )
    print("-" * 90)

    for r in results:
        speedup = baseline.median_ms / r.median_ms
        print(
            f"{r.library:<15} {r.min_ms:>10.2f} {r.median_ms:>12.2f} {r.p95_ms:>10.2f} {r.output_size:>12} {r.rss_mb:>9.1f} {speedup:.2f}x"
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
    parser = argparse.ArgumentParser(description="Benchmark JSON dumps performance")
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("benchmarks/data/generated/users.json"),
        help="Path to JSON file",
    )
    parser.add_argument("--repeat", type=int, default=50, help="Iterations per benchmark")
    parser.add_argument("--warmup", type=int, default=3, help="Warmup iterations")
    parser.add_argument("--bytes", action="store_true", help="Benchmark bytes output (dumps_bytes)")
    args = parser.parse_args()

    if not args.data.exists():
        print(f"Error: Data file not found: {args.data}")
        print("Generate with: python -m benchmarks.data.generate_bench_data")
        return 1

    results = run_benchmarks(
        args.data, repeat=args.repeat, warmup=args.warmup, bytes_mode=args.bytes
    )
    print_summary(results)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

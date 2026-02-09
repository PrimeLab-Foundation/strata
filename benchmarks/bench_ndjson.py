#!/usr/bin/env python3
"""
NDJSON parsing benchmarks.

Compares Strata iter_ndjson against orjson, msgspec, ujson, and stdlib json.
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
class NdjsonResult:
    """Result for one NDJSON benchmark run."""

    library: str
    min_ms: float
    median_ms: float
    p95_ms: float
    lines_parsed: int
    rss_mb: float


def _p95(times_ms: list[float]) -> float:
    if len(times_ms) >= 20:
        return statistics.quantiles(times_ms, n=20)[18]
    return max(times_ms) if times_ms else 0.0


def _lines(ndjson_text: str) -> list[str]:
    return [line for line in ndjson_text.strip().split("\n") if line.strip()]


def _get_ndjson_runners() -> list[tuple[str, Callable[[str], list[Any]]]]:
    """Return [(library_name, parse_func)] where parse_func(ndjson_str) -> list of objects."""

    # Strata with auto-detection (picks optimal mode based on object size)
    def strata_auto_run(text: str) -> list:
        return strata.parse_ndjson(text)

    # Force sequential (for comparison)
    def strata_seq_run(text: str) -> list:
        return strata.parse_ndjson(text, parallel=False)

    # Force parallel (for comparison)
    def strata_par_run(text: str) -> list:
        return strata.parse_ndjson(text, parallel=True)

    def orjson_run(text: str) -> list:
        return [orjson.loads(line) for line in _lines(text)]

    def msgspec_run(text: str) -> list:
        return [msgspec.json.decode(line) for line in _lines(text)]

    def ujson_run(text: str) -> list:
        return [ujson.loads(line) for line in _lines(text)]

    def stdlib_run(text: str) -> list:
        return [json.loads(line) for line in _lines(text)]

    # Strata auto is the main comparison (uses optimal mode automatically)
    runners: list[tuple[str, Callable[[str], list[Any]]]] = [
        ("strata", strata_auto_run),
    ]
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
    repeat: int = 30,
    warmup: int = 2,
) -> list[NdjsonResult]:
    """Run NDJSON benchmarks. data_file can be .json (converted to NDJSON) or .ndjson."""
    if data_file.suffix == ".ndjson":
        ndjson_text = data_file.read_text(encoding="utf-8")
    else:
        with open(data_file, encoding="utf-8") as f:
            data = json.load(f)
        if isinstance(data, dict) and "users" in data:
            items = data["users"]
        elif isinstance(data, list):
            items = data
        else:
            raise ValueError("Expected array or object with 'users' field")
        ndjson_text = "\n".join(json.dumps(item, ensure_ascii=False) for item in items)

    line_count = len(_lines(ndjson_text))
    size_mb = len(ndjson_text) / 1024 / 1024

    print()
    print("=" * 70)
    print("NDJSON Parsing Benchmarks")
    print("=" * 70)
    print(f"Source: {data_file}")
    print(f"NDJSON size: {size_mb:.2f} MB, lines: {line_count}")
    print(f"Repeat: {repeat}, Warmup: {warmup}")
    print()

    results: list[NdjsonResult] = []
    for library_name, parse_func in _get_ndjson_runners():
        print(f"--- Benchmarking {library_name} ---")
        try:
            last_result: list[Any] | None = None

            def run():
                nonlocal last_result
                last_result = parse_func(ndjson_text)
                return last_result

            tr = run_single_benchmark(
                run,
                warmup=warmup,
                repeat=repeat,
                capture_rss=True,
            )
            lines_parsed = len(last_result) if isinstance(last_result, list) else 0
            result = NdjsonResult(
                library=library_name,
                min_ms=tr.min_ms,
                median_ms=tr.median_ms,
                p95_ms=_p95(tr.times_ms),
                lines_parsed=lines_parsed,
                rss_mb=tr.rss_mb,
            )
            results.append(result)
            print(
                f"  min={result.min_ms:.2f}ms, median={result.median_ms:.2f}ms, "
                f"p95={result.p95_ms:.2f}ms, lines={result.lines_parsed}, rss={result.rss_mb:.1f} MB"
            )
        except Exception as e:
            print(f"  ERROR: {e}")

    return results


def print_summary(results: list[NdjsonResult]) -> None:
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
        f"{'Library':<15} {'Min (ms)':>10} {'Median (ms)':>12} {'P95 (ms)':>10} {'Lines':>8} {'RSS (MB)':>9} {'Speedup':>10}"
    )
    print("-" * 85)

    for r in results:
        speedup = baseline.median_ms / r.median_ms
        print(
            f"{r.library:<15} {r.min_ms:>10.2f} {r.median_ms:>12.2f} {r.p95_ms:>10.2f} {r.lines_parsed:>8} {r.rss_mb:>9.1f} {speedup:.2f}x"
        )

    # Report on strata
    strata_result = next((r for r in results if r.library == "strata"), None)
    if strata_result:
        rank = results.index(strata_result) + 1
        print()
        print(f"strata: Rank #{rank} / {len(results)}")
        if rank == 1 and len(results) > 1:
            second = results[1]
            pct = (second.median_ms / strata_result.median_ms - 1) * 100
            print(f"  -> {pct:.1f}% faster than #{2} ({second.library})")
        elif rank > 1:
            first = results[0]
            pct = (strata_result.median_ms / first.median_ms - 1) * 100
            print(f"  -> {pct:.1f}% behind #{1} ({first.library})")


def main() -> int:
    parser = argparse.ArgumentParser(description="Benchmark NDJSON parsing")
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("benchmarks/data/generated/users.json"),
        help="JSON or NDJSON file",
    )
    parser.add_argument("--repeat", type=int, default=30, help="Iterations")
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

#!/usr/bin/env python3
"""
NDJSON cursor benchmarks.

Compares eager vs lazy NdjsonCursor for large NDJSON files,
including selective queries that benefit from lazy parsing.
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
from typing import Callable, Tuple

import strata
from strata import _strata as _native

from .harness import run_single_benchmark
from .data.generate_bench_data import generate_users_datasets


@dataclass
class CursorBenchResult:
    label: str
    mode: str
    median_ms: float
    p95_ms: float
    rss_mb: float


def _ensure_data(path: Path, lines: int) -> Path:
    if path.exists():
        return path
    path.parent.mkdir(parents=True, exist_ok=True)
    info = generate_users_datasets(path.parent, num_users=lines)
    return Path(info["users_ndjson"])


def _run_bench(
    label: str,
    query: str,
    path: Path,
    run: Callable[[], Tuple[object, object]],
    data_size_bytes: int,
) -> CursorBenchResult:
    result = run_single_benchmark(run, warmup=1, repeat=5, capture_rss=True, data_size_bytes=data_size_bytes)
    return CursorBenchResult(
        label=label,
        mode="lazy" if "lazy" in label.lower() else "eager",
        median_ms=result.median_ms,
        p95_ms=result.p95_ms,
        rss_mb=result.rss_mb,
    )


def main() -> int:
    parser = argparse.ArgumentParser(description="Benchmark NDJSON cursor eager vs lazy")
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("benchmarks/data/generated/ndjson_cursor/users_100k.ndjson"),
        help="NDJSON file path",
    )
    parser.add_argument("--lines", type=int, default=100_000, help="Lines to generate if missing")
    parser.add_argument("--full-query", default="$.id", help="Query for full scan benchmark")
    parser.add_argument(
        "--selective-query",
        default="$[0:10].name",
        help="Query for selective benchmark",
    )
    args = parser.parse_args()

    path = _ensure_data(args.data, args.lines)
    data_size_bytes = path.stat().st_size

    print("=" * 70)
    print("NDJSON Cursor Benchmarks")
    print("=" * 70)
    print(f"File: {path} ({args.lines} lines target, {data_size_bytes / 1024 / 1024:.2f} MB)")
    print()

    def run_eager(query: str):
        def _run():
            cursor = _native.NdjsonCursor.from_file(str(path))
            results = strata.search(cursor, query)
            return cursor, results
        return _run

    def run_lazy(query: str):
        def _run():
            cursor = _native.NdjsonCursor.from_file_lazy(str(path))
            results = strata.search(cursor, query)
            return cursor, results
        return _run

    results: list[CursorBenchResult] = []

    print(f"Full scan query: {args.full_query}")
    results.append(_run_bench("eager/full", args.full_query, path, run_eager(args.full_query), data_size_bytes))
    results.append(_run_bench("lazy/full", args.full_query, path, run_lazy(args.full_query), data_size_bytes))

    print(f"Selective query: {args.selective_query}")
    results.append(
        _run_bench(
            "eager/selective",
            args.selective_query,
            path,
            run_eager(args.selective_query),
            data_size_bytes,
        )
    )
    results.append(
        _run_bench(
            "lazy/selective",
            args.selective_query,
            path,
            run_lazy(args.selective_query),
            data_size_bytes,
        )
    )

    print()
    print("Summary")
    print("=" * 70)
    print(f"{'Case':<18} {'Median (ms)':>12} {'P95 (ms)':>10} {'RSS (MB)':>10}")
    for r in results:
        print(f"{r.label:<18} {r.median_ms:>12.2f} {r.p95_ms:>10.2f} {r.rss_mb:>10.1f}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

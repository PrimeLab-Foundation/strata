#!/usr/bin/env python3
"""
Parallel JSON parse experiment benchmark.

Compares standard strata.loads against the experimental parallel chunked parser.
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable

from .harness import run_single_benchmark

import strata


@dataclass
class BenchResult:
    name: str
    min_ms: float
    median_ms: float
    p95_ms: float
    throughput_mbps: float
    rss_mb: float
    items: int | None


def _count_items(obj: Any) -> int | None:
    if isinstance(obj, dict):
        users = obj.get("users")
        if isinstance(users, list):
            return len(users)
        return len(obj)
    if isinstance(obj, list):
        return len(obj)
    return None


def run_one(name: str, func: Callable[[], Any], *, warmup: int, repeat: int, data_size: int) -> BenchResult:
    last_result: Any = None

    def run():
        nonlocal last_result
        last_result = func()
        return last_result

    tr = run_single_benchmark(run, warmup=warmup, repeat=repeat, capture_rss=True, data_size_bytes=data_size)
    return BenchResult(
        name=name,
        min_ms=tr.min_ms,
        median_ms=tr.median_ms,
        p95_ms=tr.p95_ms,
        throughput_mbps=tr.throughput_mbps,
        rss_mb=tr.rss_mb,
        items=_count_items(last_result),
    )


def main() -> None:
    parser = argparse.ArgumentParser(description="Parallel JSON parse experiment benchmark")
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("benchmarks/data/generated/large/users.json"),
        help="Path to JSON input",
    )
    parser.add_argument("--repeat", type=int, default=3)
    parser.add_argument("--warmup", type=int, default=1)
    parser.add_argument("--num-threads", type=int, default=0)
    parser.add_argument("--min-chunk-size", type=int, default=0)
    args = parser.parse_args()

    data_bytes = args.data.read_bytes()
    data_size = len(data_bytes)
    size_mb = data_size / (1024 * 1024)

    print()
    print("=" * 70)
    print("Parallel JSON Parse Experiment")
    print("=" * 70)
    print(f"Data: {args.data}")
    print(f"Size: {size_mb:.2f} MB")
    print(f"Repeat: {args.repeat}, Warmup: {args.warmup}")
    print(f"num_threads: {args.num_threads}, min_chunk_size: {args.min_chunk_size}")
    print()

    results: list[BenchResult] = []

    results.append(
        run_one(
            "strata",
            lambda: strata.loads(data_bytes),
            warmup=args.warmup,
            repeat=args.repeat,
            data_size=data_size,
        )
    )

    results.append(
        run_one(
            "strata_parallel_experiment",
            lambda: strata.loads_parallel_json_experiment(
                data_bytes, num_threads=args.num_threads, min_chunk_size=args.min_chunk_size
            ),
            warmup=args.warmup,
            repeat=args.repeat,
            data_size=data_size,
        )
    )

    print()
    print("SUMMARY")
    print("=" * 70)
    print(
        f"{'Library':<28} {'Min (ms)':>10} {'Median (ms)':>12} {'P95 (ms)':>10} {'MB/s':>10} {'RSS (MB)':>9} {'Items':>7}"
    )
    print("-" * 94)
    for r in results:
        items = r.items if r.items is not None else 0
        print(
            f"{r.name:<28} {r.min_ms:>10.2f} {r.median_ms:>12.2f} {r.p95_ms:>10.2f} "
            f"{r.throughput_mbps:>10.2f} {r.rss_mb:>9.1f} {items:>7}"
        )


if __name__ == "__main__":
    main()

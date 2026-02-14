#!/usr/bin/env python3
"""
JSON/NDJSON parsing (loads) benchmarks.

Compares Strata loads against orjson, ujson, msgspec, pysimdjson, and stdlib json.
Uses shared harness for timing and RSS. Pair with bench_dumps.py (serialize).
"""

from __future__ import annotations

import json
import statistics
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable

from .harness import run_single_benchmark
from .markdown_tables import build_markdown_table

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
import strata.ndjson as strata_ndjson


@dataclass
class LoadsResult:
    """Result for one loads (parse) benchmark run."""

    library: str
    min_ms: float
    median_ms: float
    p95_ms: float
    throughput_mbps: float
    rss_mb: float


@dataclass
class BenchInfo:
    """Metadata about a benchmark run."""

    data_file: Path
    size_mb: float
    is_ndjson: bool
    line_count: int | None = None


def _p95(times_ms: list[float]) -> float:
    if len(times_ms) >= 20:
        return statistics.quantiles(times_ms, n=20)[18]
    return max(times_ms) if times_ms else 0.0


def _ndjson_lines(text: str) -> list[str]:
    return [line for line in text.splitlines() if line.strip()]


def _get_loads_runners(is_ndjson: bool) -> list[tuple[str, Callable[[bytes | str], Any]]]:
    """Return [(library_name, parse_func)] where parse_func(data) -> parsed value."""

    if is_ndjson:

        def strata_run(text: str):
            return strata_ndjson.parse_ndjson(text)

        def orjson_run(text: str):
            return [orjson.loads(line) for line in _ndjson_lines(text)]

        def msgspec_run(text: str):
            return [msgspec.json.decode(line) for line in _ndjson_lines(text)]

        def ujson_run(text: str):
            return [ujson.loads(line) for line in _ndjson_lines(text)]

        def stdlib_run(text: str):
            return [json.loads(line) for line in _ndjson_lines(text)]

        runners: list[tuple[str, Callable[[bytes | str], Any]]] = [("strata", strata_run)]
        if orjson is not None:
            runners.append(("orjson", orjson_run))
        if msgspec is not None:
            runners.append(("msgspec", msgspec_run))
        if ujson is not None:
            runners.append(("ujson", ujson_run))
        runners.append(("json (stdlib)", stdlib_run))
        return runners

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

    runners = [("strata", strata_run)]
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
) -> tuple[list[LoadsResult], BenchInfo]:
    """Run loads (parse) benchmarks; return (results, info)."""
    is_ndjson = data_file.suffix == ".ndjson"
    data_size_bytes = data_file.stat().st_size
    if is_ndjson:
        data = data_file.read_text(encoding="utf-8")
        line_count = len(_ndjson_lines(data))
        size_mb = data_size_bytes / 1024 / 1024
    else:
        data = data_file.read_bytes()
        line_count = None
        size_mb = data_size_bytes / 1024 / 1024

    print()
    print("=" * 70)
    label = "NDJSON" if is_ndjson else "JSON"
    print(f"{label} Parsing (loads) Benchmarks")
    print("=" * 70)
    print(f"Data file: {data_file}")
    print(f"Input size: {size_mb:.2f} MB")
    if line_count is not None:
        print(f"Lines: {line_count}")
    print(f"Repeat: {repeat}, Warmup: {warmup}")
    print()

    results: list[LoadsResult] = []
    for library_name, parse_func in _get_loads_runners(is_ndjson):
        print(f"--- Benchmarking {library_name} ---")
        try:
            tr = run_single_benchmark(
                lambda: parse_func(data),
                warmup=warmup,
                repeat=repeat,
                capture_rss=True,
                data_size_bytes=data_size_bytes,
            )
            result = LoadsResult(
                library=library_name,
                min_ms=tr.min_ms,
                median_ms=tr.median_ms,
                p95_ms=_p95(tr.times_ms),
                throughput_mbps=tr.throughput_mbps,
                rss_mb=tr.rss_mb,
            )
            results.append(result)
            print(
                f"  min={result.min_ms:.2f}ms, median={result.median_ms:.2f}ms, "
                f"p95={result.p95_ms:.2f}ms, mbps={result.throughput_mbps:.2f}, "
                f"rss={result.rss_mb:.1f} MB"
            )
        except Exception as e:
            print(f"  ERROR: {e}")

    info = BenchInfo(
        data_file=data_file,
        size_mb=size_mb,
        is_ndjson=is_ndjson,
        line_count=line_count,
    )
    return results, info


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
        f"{'Library':<15} {'Min (ms)':>10} {'Median (ms)':>12} {'P95 (ms)':>10} {'MB/s':>10} {'RSS (MB)':>9} {'Speedup':>10}"
    )
    print("-" * 82)

    for r in results:
        speedup = baseline.median_ms / r.median_ms
        print(
            f"{r.library:<15} {r.min_ms:>10.2f} {r.median_ms:>12.2f} {r.p95_ms:>10.2f} "
            f"{r.throughput_mbps:>10.2f} {r.rss_mb:>9.1f} {speedup:.2f}x"
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


def _format_markdown(results: list[LoadsResult], info: BenchInfo, repeat: int, warmup: int) -> str:
    label = "NDJSON" if info.is_ndjson else "JSON"
    lines = [
        f"### {info.data_file.name} ({label})",
        "",
        f"- Source: {info.data_file}",
        f"- Input size: {info.size_mb:.2f} MB",
    ]
    if info.line_count is not None:
        lines.append(f"- Lines: {info.line_count}")
    lines.extend(
        [
            f"- Repeat: {repeat}",
            f"- Warmup: {warmup}",
            "",
        ]
    )
    if not results:
        lines.append("*No results available*")
        lines.append("")
        return "\n".join(lines)

    table_rows: list[list[str]] = []
    for r in sorted(results, key=lambda x: x.median_ms):
        table_rows.append(
            [
                r.library,
                f"{r.min_ms:.2f}",
                f"{r.median_ms:.2f}",
                f"{r.p95_ms:.2f}",
                f"{r.throughput_mbps:.2f}",
                f"{r.rss_mb:.1f}",
            ]
        )
    lines.extend(
        build_markdown_table(
            ["Library", "Min (ms)", "Median (ms)", "P95 (ms)", "MB/s", "RSS (MB)"],
            table_rows,
        )
    )
    lines.append("")
    return "\n".join(lines)


def _append_markdown_section(
    output_path: Path,
    section_title: str,
    body: str,
    append: bool,
) -> None:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    header = f"## {section_title}\n\n"
    if append and output_path.exists():
        existing = output_path.read_text(encoding="utf-8")
        include_header = header.strip() not in existing
        prefix = "" if existing.endswith("\n") else "\n"
        addition = (header if include_header else "") + body
        output_path.write_text(existing + prefix + addition, encoding="utf-8")
        return
    output_path.write_text(header + body, encoding="utf-8")


__all__ = [
    "LoadsResult",
    "BenchInfo",
    "run_benchmarks",
    "print_summary",
    "_get_loads_runners",
]

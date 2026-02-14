"""
Experimental benchmark suite for exploratory features.

Covers:
- Object materialization overhead (bench_object_materialization)
- Object pool on/off comparison (bench_object_pool)
- Parallel JSON parse experiment (bench_parallel_json)

Outputs a single Markdown report (optional JSON) for visibility without
changing the primary benchmark interface.
"""

from __future__ import annotations

import argparse
import json
import os
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Any

from . import bench_object_materialization as mat
from . import bench_object_pool as pool
from . import bench_parallel_json as pjson


@dataclass
class Section:
    title: str
    lines: list[str]


def _md_table(headers: list[str], rows: list[list[str]]) -> list[str]:
    widths = [len(h) for h in headers]
    for row in rows:
        for i, cell in enumerate(row):
            widths[i] = max(widths[i], len(cell))
    header = "| " + " | ".join(h.ljust(widths[i]) for i, h in enumerate(headers)) + " |"
    sep = "| " + " | ".join("-" * widths[i] for i in range(len(headers))) + " |"
    data = [
        "| " + " | ".join(row[i].ljust(widths[i]) for i in range(len(headers))) + " |"
        for row in rows
    ]
    return [header, sep, *data]


# ---------------------------------------------------------------------------
# Benchmarks
# ---------------------------------------------------------------------------


def run_materialization(json_path: Path, iterations: int, warmup: int) -> Section:
    info = mat.load_and_analyze_json(json_path)
    strata_times = mat.benchmark_parse_materialize(info["data"], iterations=iterations, warmup=warmup)
    try:
        orjson_times = mat.benchmark_orjson_parse(info["data"], iterations=iterations, warmup=warmup)  # type: ignore[attr-defined]
    except Exception:
        orjson_times = None
    materialize_times = mat.benchmark_materialize_only(info["parsed"], iterations=iterations, warmup=warmup)

    rows = [
        ["Strata (parse+materialize)", f"{strata_times[0]:.2f}", f"{strata_times[1]:.2f}", f"{strata_times[2]:.2f}"],
        ["Materialization only", f"{materialize_times[0]:.2f}", f"{materialize_times[1]:.2f}", f"{materialize_times[2]:.2f}"],
    ]
    if orjson_times:
        rows.append(["orjson (parse+materialize)", f"{orjson_times[0]:.2f}", f"{orjson_times[1]:.2f}", f"{orjson_times[2]:.2f}"])

    lines: list[str] = [
        f"- Data: {json_path}",
        f"- Size: {info['size_mb']:.2f} MB",
        f"- Objects: {info['stats']['total']:,}",
        "",
    ]
    lines.extend(_md_table(["Benchmark", "Min (ms)", "Median (ms)", "Mean (ms)"], rows))
    return Section("Materialization Overhead", lines)


def run_object_pool(json_path: Path, iterations: int, warmup: int) -> Section:
    data = json_path.read_text()
    # Pool ON
    on = pool.bench_loads(data, iterations=iterations, warmup=warmup)
    # Pool OFF
    prev = os.environ.get("STRATA_OBJECT_POOL_SIZE")
    os.environ["STRATA_OBJECT_POOL_SIZE"] = "0"
    off = pool.bench_loads(data, iterations=iterations, warmup=warmup)
    if prev is None:
        os.environ.pop("STRATA_OBJECT_POOL_SIZE", None)
    else:
        os.environ["STRATA_OBJECT_POOL_SIZE"] = prev

    size_mb = len(data.encode()) / (1024 * 1024)
    rows = [
        ["Pool ON", f"{on['min_ms']:.2f}", f"{on['median_ms']:.2f}", f"{on['p95_ms']:.2f}", f"{size_mb / (on['median_ms']/1000):.1f}"],
        ["Pool OFF", f"{off['min_ms']:.2f}", f"{off['median_ms']:.2f}", f"{off['p95_ms']:.2f}", f"{size_mb / (off['median_ms']/1000):.1f}"],
    ]
    lines = [
        f"- Data: {json_path} ({size_mb:.2f} MB)",
        f"- Iterations: {iterations}, Warmup: {warmup}",
        "",
    ]
    lines.extend(_md_table(["Mode", "Min (ms)", "Median (ms)", "P95 (ms)", "MB/s"], rows))
    return Section("Object Pool (dict presize)", lines)


def run_parallel_json(json_path: Path, repeat: int, warmup: int, num_threads: int, min_chunk_size: int) -> Section:
    data = json_path.read_bytes()
    size_mb = len(data) / (1024 * 1024)
    base = pjson.run_one(
        "strata",
        lambda: pjson.strata.loads(data),
        warmup=warmup,
        repeat=repeat,
        data_size=len(data),
    )
    parallel = pjson.run_one(
        "strata_parallel_experiment",
        lambda: pjson.strata.loads_parallel_json_experiment(
            data, num_threads=num_threads, min_chunk_size=min_chunk_size
        ),
        warmup=warmup,
        repeat=repeat,
        data_size=len(data),
    )
    rows = []
    for r in [base, parallel]:
        rows.append(
            [
                r.name,
                f"{r.min_ms:.2f}",
                f"{r.median_ms:.2f}",
                f"{r.p95_ms:.2f}",
                f"{r.throughput_mbps:.2f}",
                f"{r.rss_mb:.1f}",
                str(r.items or 0),
            ]
        )

    lines = [
        f"- Data: {json_path} ({size_mb:.2f} MB)",
        f"- Repeat: {repeat}, Warmup: {warmup}",
        f"- num_threads: {num_threads}, min_chunk_size: {min_chunk_size}",
        "",
    ]
    lines.extend(
        _md_table(
            ["Mode", "Min (ms)", "Median (ms)", "P95 (ms)", "MB/s", "RSS (MB)", "Items"],
            rows,
        )
    )
    return Section("Parallel JSON Experiment", lines)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------


def main() -> int:
    parser = argparse.ArgumentParser(description="Experimental Strata benchmarks")
    parser.add_argument("--json-data", type=Path, required=True, help="Path to JSON dataset")
    parser.add_argument("--repeat", type=int, default=5, help="Iterations (per benchmark)")
    parser.add_argument("--warmup", type=int, default=2, help="Warmup iterations")
    parser.add_argument("--num-threads", type=int, default=0, help="Parallel JSON threads (0=auto)")
    parser.add_argument("--min-chunk-size", type=int, default=0, help="Parallel JSON min chunk size")
    parser.add_argument("--output", type=Path, default=Path("docs/benchmarks/bench_experimental.md"))
    parser.add_argument("--json-output", type=Path, help="Optional JSON output")
    args = parser.parse_args()

    sections: list[Section] = []
    sections.append(run_materialization(args.json_data, iterations=args.repeat, warmup=args.warmup))
    sections.append(run_object_pool(args.json_data, iterations=args.repeat, warmup=args.warmup))
    sections.append(
        run_parallel_json(
            args.json_data,
            repeat=args.repeat,
            warmup=args.warmup,
            num_threads=args.num_threads,
            min_chunk_size=args.min_chunk_size,
        )
    )

    ts = time.strftime("%Y-%m-%d %H:%M:%S")
    lines = [
        "# Strata Experimental Benchmarks",
        f"Generated: {ts}",
        "",
        f"- Data: {args.json_data}",
        f"- Repeat: {args.repeat}, Warmup: {args.warmup}",
        "",
    ]
    for sec in sections:
        lines.append(f"## {sec.title}")
        lines.append("")
        lines.extend(sec.lines)
        lines.append("")

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text("\n".join(lines), encoding="utf-8")

    if args.json_output:
        payload = {
            "generated": ts,
            "data": str(args.json_data),
            "repeat": args.repeat,
            "warmup": args.warmup,
            "sections": [
                {
                    "title": sec.title,
                    "content": sec.lines,
                }
                for sec in sections
            ],
        }
        args.json_output.parent.mkdir(parents=True, exist_ok=True)
        args.json_output.write_text(json.dumps(payload, indent=2), encoding="utf-8")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

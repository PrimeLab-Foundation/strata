"""
Unified benchmark orchestrator.

Runs all benchmark categories in a single process and emits one Markdown
report (plus JSON/CSV sidecars) per dataset size.
"""

from __future__ import annotations

import argparse
import json
import platform
import statistics
import subprocess
import sysconfig
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from .bench_dumps import _get_dumps_runners, _output_size_bytes
from .bench_loads import _get_loads_runners
from .bench_search import NDJSON_QUERIES, QUERIES, run_all as run_query_suite
from .harness import TimingResult, p95, run_single_benchmark
from .reporters import csv_reporter, json_reporter, markdown_reporter


# ---------------------------------------------------------------------------
# Result + metadata models
# ---------------------------------------------------------------------------


@dataclass
class BenchmarkResult:
    category: str
    library: str
    dataset: str
    variant: str
    times_ms: list[float] = field(default_factory=list)
    result_count: int = 0
    output_size: int = 0
    rss_mb: float = 0.0
    throughput_mbps: float = 0.0
    error: str = ""

    @property
    def min_ms(self) -> float:
        return min(self.times_ms) if self.times_ms else 0.0

    @property
    def median_ms(self) -> float:
        return statistics.median(self.times_ms) if self.times_ms else 0.0

    @property
    def p95_ms(self) -> float:
        return p95(self.times_ms)

    def to_dict(self) -> dict[str, Any]:
        return {
            "category": self.category,
            "library": self.library,
            "dataset": self.dataset,
            "variant": self.variant,
            "times_ms": self.times_ms,
            "min_ms": self.min_ms,
            "median_ms": self.median_ms,
            "p95_ms": self.p95_ms,
            "result_count": self.result_count,
            "output_size": self.output_size,
            "rss_mb": self.rss_mb,
            "throughput_mbps": self.throughput_mbps,
            "error": self.error,
        }


@dataclass
class DatasetInfo:
    path: Path
    size_bytes: int
    record_count: int
    line_count: int | None
    data_bytes: bytes | None = None
    text: str | None = None
    json_data: Any | None = None

    @property
    def size_mb(self) -> float:
        return self.size_bytes / 1024 / 1024


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------


def _collect_environment() -> dict[str, str]:
    env = {
        "commit": "",
        "os": platform.platform(),
        "cpu": platform.machine(),
        "python": platform.python_version(),
        "compiler": "",
    }
    try:
        repo_root = Path(__file__).resolve().parents[1]
        env["commit"] = (
            subprocess.check_output([
                "git",
                "rev-parse",
                "HEAD",
            ], cwd=repo_root, stderr=subprocess.DEVNULL)
            .decode()
            .strip()
        )
    except Exception:
        pass

    try:
        out = (
            subprocess.check_output(["clang++", "--version"], stderr=subprocess.DEVNULL)
            .decode()
            .splitlines()[0]
        )
        env["compiler"] = out
    except Exception:
        cxx = sysconfig.get_config_var("CXX")
        if cxx:
            env["compiler"] = cxx
    return env


def _read_json_dataset(path: Path) -> DatasetInfo:
    data_bytes = path.read_bytes()
    text = data_bytes.decode("utf-8")
    data = json.loads(text)
    if isinstance(data, dict) and "users" in data:
        record_count = len(data.get("users") or [])
    elif isinstance(data, list):
        record_count = len(data)
    else:
        record_count = 1
    return DatasetInfo(
        path=path,
        size_bytes=len(data_bytes),
        record_count=record_count,
        line_count=None,
        data_bytes=data_bytes,
        text=text,
        json_data=data,
    )


def _read_ndjson_dataset(path: Path) -> DatasetInfo:
    text = path.read_text(encoding="utf-8")
    lines = [line for line in text.splitlines() if line.strip()]
    data = [json.loads(line) for line in lines]
    size_bytes = path.stat().st_size
    return DatasetInfo(
        path=path,
        size_bytes=size_bytes,
        record_count=len(lines),
        line_count=len(lines),
        text=text,
        json_data=data,
    )


def _speedup(baseline_ms: float, other_ms: float) -> float:
    if other_ms <= 0:
        return 0.0
    return baseline_ms / other_ms


# ---------------------------------------------------------------------------
# Bench suite
# ---------------------------------------------------------------------------


class BenchSuite:
    def __init__(
        self,
        *,
        repeat: int | None = None,
        warmup: int | None = None,
    ) -> None:
        # Category defaults
        self.loads_repeat = repeat if repeat is not None else 5
        self.loads_warmup = warmup if warmup is not None else 2
        self.dumps_repeat = repeat if repeat is not None else 5
        self.dumps_warmup = warmup if warmup is not None else 2
        self.search_repeat = repeat if repeat is not None else 3
        self.search_warmup = warmup if warmup is not None else 1
        self.cursor_repeat = repeat if repeat is not None else 3
        self.cursor_warmup = warmup if warmup is not None else 1

    # -------------------- loads --------------------
    def run_loads(self, dataset: DatasetInfo, *, is_ndjson: bool) -> list[BenchmarkResult]:
        data_size = dataset.size_bytes
        payload = dataset.text if is_ndjson else dataset.data_bytes
        assert payload is not None
        results: list[BenchmarkResult] = []
        for lib, func in _get_loads_runners(is_ndjson):
            try:
                tr: TimingResult = run_single_benchmark(
                    lambda: func(payload),
                    warmup=self.loads_warmup,
                    repeat=self.loads_repeat,
                    capture_rss=True,
                    data_size_bytes=data_size,
                )
                results.append(
                    BenchmarkResult(
                        category="loads",
                        library=lib,
                        dataset=dataset.path.name,
                        variant="ndjson" if is_ndjson else "json",
                        times_ms=tr.times_ms,
                        result_count=dataset.record_count,
                        rss_mb=tr.rss_mb,
                        throughput_mbps=tr.throughput_mbps,
                    )
                )
            except Exception as exc:  # pragma: no cover - defensive
                results.append(
                    BenchmarkResult(
                        category="loads",
                        library=lib,
                        dataset=dataset.path.name,
                        variant="ndjson" if is_ndjson else "json",
                        error=str(exc),
                    )
                )
        return results

    # -------------------- dumps --------------------
    def run_dumps(self, dataset: DatasetInfo, *, bytes_mode: bool) -> list[BenchmarkResult]:
        data_obj = dataset.json_data
        results: list[BenchmarkResult] = []
        runners = _get_dumps_runners(bytes_mode)
        label = "bytes" if bytes_mode else "str"

        for lib, func in runners:
            try:
                probe = func(data_obj)
                output_size = _output_size_bytes(probe)

                def run_once():
                    return func(data_obj)

                tr = run_single_benchmark(
                    run_once,
                    warmup=self.dumps_warmup,
                    repeat=self.dumps_repeat,
                    capture_rss=True,
                    data_size_bytes=output_size,
                )
                results.append(
                    BenchmarkResult(
                        category="dumps",
                        library=lib,
                        dataset=dataset.path.name,
                        variant=label,
                        times_ms=tr.times_ms,
                        output_size=output_size,
                        rss_mb=tr.rss_mb,
                        throughput_mbps=tr.throughput_mbps,
                    )
                )
            except Exception as exc:  # pragma: no cover - defensive
                results.append(
                    BenchmarkResult(
                        category="dumps",
                        library=lib,
                        dataset=dataset.path.name,
                        variant=label,
                        error=str(exc),
                    )
                )
        return results

    # -------------------- search + cursor reuse --------------------
    def run_search(self, data_file: Path, *, is_ndjson: bool) -> tuple[list[BenchmarkResult], list[BenchmarkResult]]:
        query_results, cursor_reuse_results = run_query_suite(
            data_file,
            repeat=self.search_repeat,
            warmup=self.search_warmup,
            strata_mode="cursor",
            ndjson_parallel="auto",
            cursor_reuse=True,
        )

        allowed_libs = {"strata", "jmespath", "jsonpath-ng"}
        results: list[BenchmarkResult] = []
        queries = NDJSON_QUERIES if is_ndjson else QUERIES
        for r in query_results:
            if r.library not in allowed_libs:
                continue
            desc = queries.get(r.query_name, {}).get("description", r.query_name)
            results.append(
                BenchmarkResult(
                    category="search",
                    library=r.library,
                    dataset=data_file.name,
                    variant=r.query_name,
                    times_ms=r.times_ms,
                    result_count=r.result_count,
                    rss_mb=r.rss_mb,
                    throughput_mbps=r.throughput_mbps,
                    error=r.error,
                )
            )

        cursor_results: list[BenchmarkResult] = []
        for r in cursor_reuse_results:
            cursor_results.append(
                BenchmarkResult(
                    category="cursor_reuse",
                    library=r.label,
                    dataset=data_file.name,
                    variant="ndjson" if is_ndjson else "json",
                    times_ms=r.times_ms,
                    rss_mb=r.rss_mb,
                    throughput_mbps=r.throughput_mbps,
                    error=r.error,
                )
            )
        return results, cursor_results

    # -------------------- orchestrator --------------------
    def run_all(self, json_path: Path, ndjson_path: Path) -> tuple[list[BenchmarkResult], dict[str, DatasetInfo]]:
        json_info = _read_json_dataset(json_path)
        ndjson_info = _read_ndjson_dataset(ndjson_path)

        results: list[BenchmarkResult] = []

        # loads
        results.extend(self.run_loads(json_info, is_ndjson=False))
        results.extend(self.run_loads(ndjson_info, is_ndjson=True))

        # dumps
        results.extend(self.run_dumps(json_info, bytes_mode=False))
        results.extend(self.run_dumps(json_info, bytes_mode=True))

        # search + cursor reuse
        search_json, cursor_json = self.run_search(json_path, is_ndjson=False)
        search_ndjson, cursor_ndjson = self.run_search(ndjson_path, is_ndjson=True)
        results.extend(search_json)
        results.extend(search_ndjson)
        results.extend(cursor_json)
        results.extend(cursor_ndjson)

        datasets = {
            "json": json_info,
            "ndjson": ndjson_info,
        }
        return results, datasets


# ---------------------------------------------------------------------------
# Reporting helpers
# ---------------------------------------------------------------------------


def _build_report_context(
    *,
    label: str,
    env: dict[str, str],
    datasets: dict[str, DatasetInfo],
    results: list[BenchmarkResult],
    suite: BenchSuite,
) -> dict[str, Any]:
    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
    config = {
        "loads": {"repeat": suite.loads_repeat, "warmup": suite.loads_warmup},
        "dumps": {"repeat": suite.dumps_repeat, "warmup": suite.dumps_warmup},
        "search": {"repeat": suite.search_repeat, "warmup": suite.search_warmup},
        "cursor_reuse": {"repeat": suite.cursor_repeat, "warmup": suite.cursor_warmup},
    }

    datasets_meta = {
        key: {
            "path": str(info.path),
            "size_mb": info.size_mb,
            "record_count": info.record_count,
            "line_count": info.line_count,
            "average_record_bytes": (info.size_bytes / info.record_count)
            if info.record_count
            else 0,
        }
        for key, info in datasets.items()
    }

    return {
        "label": label,
        "generated": timestamp,
        "environment": env,
        "config": config,
        "datasets": datasets_meta,
        "results": results,
    }


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------


def main() -> int:
    parser = argparse.ArgumentParser(description="Unified benchmark suite")
    parser.add_argument("--json-data", type=Path, required=True, help="Path to JSON dataset")
    parser.add_argument("--ndjson-data", type=Path, required=True, help="Path to NDJSON dataset")
    parser.add_argument("--repeat", type=int, default=None, help="Override repeat for all categories")
    parser.add_argument("--warmup", type=int, default=None, help="Override warmup for all categories")
    parser.add_argument("--output", type=Path, required=True, help="Markdown output path")
    parser.add_argument("--json-output", type=Path, help="Optional JSON output path")
    parser.add_argument("--csv-output", type=Path, help="Optional CSV output path")
    parser.add_argument(
        "--label",
        default=None,
        help="Size label for report header (e.g., small/medium/large)",
    )
    parser.add_argument(
        "--append-progress-log",
        action="store_true",
        help="Append condensed entry to docs/benchmarks/progress_log.md",
    )
    args = parser.parse_args()

    if not args.json_data.exists() or not args.ndjson_data.exists():
        missing = [str(p) for p in (args.json_data, args.ndjson_data) if not p.exists()]
        print(f"Missing dataset(s): {', '.join(missing)}")
        return 2

    suite = BenchSuite(repeat=args.repeat, warmup=args.warmup)
    results, datasets = suite.run_all(args.json_data, args.ndjson_data)

    env = _collect_environment()
    label = args.label or args.output.stem.replace("bench_results_", "")
    report_ctx = _build_report_context(
        label=label,
        env=env,
        datasets=datasets,
        results=results,
        suite=suite,
    )

    args.output.parent.mkdir(parents=True, exist_ok=True)
    md = markdown_reporter.generate_report(report_ctx)
    args.output.write_text(md, encoding="utf-8")

    json_path = args.json_output or args.output.with_suffix(".json")
    json_path.parent.mkdir(parents=True, exist_ok=True)
    json_payload = json_reporter.generate_report(report_ctx)
    json_path.write_text(json_payload, encoding="utf-8")

    if args.csv_output:
        csv_text = csv_reporter.generate_report(report_ctx)
        args.csv_output.parent.mkdir(parents=True, exist_ok=True)
        args.csv_output.write_text(csv_text, encoding="utf-8")

    if args.append_progress_log:
        entry = markdown_reporter.generate_progress_log_entry(report_ctx)
        progress_log = Path("docs/benchmarks/progress_log.md")
        progress_log.parent.mkdir(parents=True, exist_ok=True)
        with open(progress_log, "a", encoding="utf-8") as f:
            f.write("\n" + entry.strip() + "\n")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

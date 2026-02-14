#!/usr/bin/env python3
"""
Strata benchmark orchestrator.

Compares Strata against orjson, ujson, msgspec, pysimdjson (parse) and
jmespath, jsonpath-ng (query). Uses a single harness for timing and RSS.
Output: min/median/p95 and optional Markdown report.
"""

from __future__ import annotations

import argparse
import json
import platform
import statistics
import subprocess
import sys
import sysconfig
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Callable

from .harness import run_single_benchmark
from .data.generate_bench_data import generate_users_datasets
from .markdown_tables import build_markdown_table
from .query_helpers import (
    get_query_description,
    query_users_json,
)


# -----------------------------------------------------------------------------
# Result type
# -----------------------------------------------------------------------------


@dataclass
class BenchResult:
    """Single benchmark result (parse or query)."""

    library: str
    operation: str
    dataset: str
    query: str = ""
    times_ms: list[float] = field(default_factory=list)
    result_count: int = 0
    rss_mb: float = 0.0
    error: str = ""

    @property
    def min_ms(self) -> float:
        return min(self.times_ms) if self.times_ms else 0.0

    @property
    def median_ms(self) -> float:
        return statistics.median(self.times_ms) if self.times_ms else 0.0

    @property
    def p95_ms(self) -> float:
        if not self.times_ms:
            return 0.0
        sorted_times = sorted(self.times_ms)
        idx = min(int(len(sorted_times) * 0.95), len(sorted_times) - 1)
        return sorted_times[idx]


# -----------------------------------------------------------------------------
# Parse runners: (library_name, callable(data) -> result)
# -----------------------------------------------------------------------------


def _get_parse_json_runners(strict_missing: bool) -> list[tuple[str, Callable[[bytes], Any]]]:
    """Return [(library_name, parse_func)] for JSON parsing. parse_func(bytes) -> parsed."""
    runners: list[tuple[str, Callable[[bytes], Any]]] = []

    try:
        import strata

        runners.append(("strata", lambda d: strata.loads(d)))
    except ImportError:
        if strict_missing:
            print("Warning: strata not installed, skipping benchmarks")

    try:
        import orjson

        runners.append(("orjson", lambda d: orjson.loads(d)))
    except ImportError:
        if strict_missing:
            print("Warning: orjson not installed")

    try:
        import ujson

        runners.append(("ujson", lambda d: ujson.loads(d)))
    except ImportError:
        pass

    try:
        import simdjson

        parser = simdjson.Parser()
        runners.append(("pysimdjson", lambda d: parser.parse(d)))
    except ImportError:
        pass

    try:
        import msgspec

        runners.append(("msgspec", lambda d: msgspec.json.decode(d)))
    except ImportError:
        pass

    # Stdlib always available
    runners.append(("json (stdlib)", lambda d: json.loads(d)))

    return runners


def _get_parse_ndjson_runners(strict_missing: bool) -> list[tuple[str, Callable[[str], list]]]:
    """Return [(library_name, parse_func)] for NDJSON. parse_func(str) -> list of objects."""
    runners: list[tuple[str, Callable[[str], list]]] = []
    lines_getter = lambda t: [line for line in t.splitlines() if line.strip()]

    try:
        import strata.ndjson as strata_ndjson

        runners.append(("strata", lambda t: list(strata_ndjson.iter_ndjson(t))))
    except ImportError:
        if strict_missing:
            print("Warning: strata not installed")

    try:
        import orjson

        runners.append(("orjson", lambda t: [orjson.loads(line) for line in lines_getter(t)]))
    except ImportError:
        pass

    try:
        import ujson

        runners.append(("ujson", lambda t: [ujson.loads(line) for line in lines_getter(t)]))
    except ImportError:
        pass

    try:
        import msgspec

        runners.append(
            ("msgspec", lambda t: [msgspec.json.decode(line) for line in lines_getter(t)])
        )
    except ImportError:
        pass

    runners.append(("json (stdlib)", lambda t: [json.loads(line) for line in lines_getter(t)]))
    return runners


# -----------------------------------------------------------------------------
# Orchestrator
# -----------------------------------------------------------------------------


class BenchmarkRunner:
    """Runs parse and query benchmarks, collects results, prints/saves report."""

    def __init__(
        self,
        datasets: list[str],
        *,
        repeat: int = 3,
        warmup: int = 1,
        limit: int | None = None,
        strict_missing: bool = True,
    ):
        self.datasets = datasets
        self.repeat = repeat
        self.warmup = warmup
        self.limit = limit
        self.strict_missing = strict_missing
        self.results: list[BenchResult] = []
        self.environment = self._collect_environment()

    def _collect_environment(self) -> dict[str, str]:
        env = {
            "commit": "",
            "os": platform.platform(),
            "cpu": platform.machine(),
            "python": sys.version.split()[0],
            "compiler": "",
        }
        try:
            repo_root = Path(__file__).resolve().parents[1]
            env["commit"] = (
                subprocess.check_output(
                    ["git", "rev-parse", "HEAD"],
                    cwd=repo_root,
                    stderr=subprocess.DEVNULL,
                )
                .decode()
                .strip()
            )
        except Exception:
            pass
        try:
            out = (
                subprocess.check_output(
                    ["clang++", "--version"],
                    stderr=subprocess.DEVNULL,
                )
                .decode()
                .splitlines()[0]
            )
            env["compiler"] = out
        except Exception:
            cxx = sysconfig.get_config_var("CXX")
            if cxx:
                env["compiler"] = cxx
        return env

    def _run_parse_suite(
        self,
        dataset_path: Path,
        data_bytes: bytes | None,
        data_text: str | None,
        is_ndjson: bool,
    ) -> None:
        """Run parse benchmarks for one dataset; append to self.results."""
        # Use path.name so NDJSON results have dataset e.g. "sample.ndjson"
        dataset_name = dataset_path.name
        label = f"Parsing: {dataset_name}"
        if is_ndjson and data_text is not None:
            size = len(data_text)
        else:
            size = len(data_bytes) if data_bytes else 0
        print(f"\n=== {label} ({size} bytes) ===")

        if is_ndjson:
            runners = _get_parse_ndjson_runners(self.strict_missing)
            data = data_text
        else:
            runners = _get_parse_json_runners(self.strict_missing)
            data = data_bytes

        for library_name, parse_func in runners:
            try:

                def run_once():
                    return parse_func(data)

                tr = run_single_benchmark(
                    run_once,
                    warmup=self.warmup,
                    repeat=self.repeat,
                    capture_rss=True,
                )
                self.results.append(
                    BenchResult(
                        library=library_name,
                        operation="parse",
                        dataset=dataset_name,
                        times_ms=tr.times_ms,
                        rss_mb=tr.rss_mb,
                    )
                )
                print(
                    f"  {library_name:<15} {tr.min_ms:.3f}ms (min), {tr.median_ms:.3f}ms (median)"
                )
            except Exception as e:
                print(f"  {library_name:<15} ERROR: {e}")
                self.results.append(
                    BenchResult(
                        library=library_name,
                        operation="parse",
                        dataset=dataset_name,
                        error=str(e),
                    )
                )

    def bench_parse_json(self, dataset_path: str) -> None:
        """Benchmark JSON parsing (single document)."""
        path = Path(dataset_path)
        data = path.read_bytes()
        self._run_parse_suite(path, data_bytes=data, data_text=None, is_ndjson=False)

    def bench_parse_ndjson(self, dataset_path: str) -> None:
        """Benchmark NDJSON parsing (one object per line)."""
        path = Path(dataset_path)
        text = path.read_text()
        self._run_parse_suite(path, data_bytes=None, data_text=text, is_ndjson=True)

    def bench_query_json(self, dataset_path: str) -> None:
        """Benchmark search-style queries on users.json."""
        path = Path(dataset_path)
        data = path.read_bytes()
        dataset_name = path.stem
        parsed = json.loads(data)
        print(f"\n=== Queries: {dataset_name} ===")

        for query_id in range(1, 6):
            query_desc = get_query_description(query_id, is_ndjson=False)
            print(f"\n  Query {query_id}: {query_desc}")

            # Query (Python)
            try:
                last_result = None

                def run_once():
                    nonlocal last_result
                    last_result = query_users_json(parsed, query_id)
                    return last_result

                tr = run_single_benchmark(
                    run_once,
                    warmup=self.warmup,
                    repeat=self.repeat,
                    capture_rss=True,
                )
                n = len(last_result) if isinstance(last_result, list) else 1
                self.results.append(
                    BenchResult(
                        library="query",
                        operation="query",
                        dataset=dataset_name,
                        query=query_desc,
                        times_ms=tr.times_ms,
                        result_count=n,
                        rss_mb=tr.rss_mb,
                    )
                )
                print(f"    query:     {tr.min_ms:.3f}ms → {n} results")
            except Exception as e:
                print(f"    query:     ERROR: {e}")

            # jmespath (queries 1–3)
            if query_id in (1, 2, 3):
                try:
                    import jmespath

                    exprs = {
                        1: "users[*].id",
                        2: "users[*].orders[*].items[*].price",
                        3: "users[*].orders[*].items[*].price | [*]",
                    }
                    expr = jmespath.compile(exprs[query_id])
                    last_result = None

                    def run_jmespath():
                        nonlocal last_result
                        last_result = expr.search(parsed)
                        return last_result

                    tr = run_single_benchmark(
                        run_jmespath,
                        warmup=self.warmup,
                        repeat=self.repeat,
                        capture_rss=True,
                    )
                    n = len(last_result) if isinstance(last_result, (list, tuple)) else 1
                    self.results.append(
                        BenchResult(
                            library="jmespath",
                            operation="query",
                            dataset=dataset_name,
                            query=query_desc,
                            times_ms=tr.times_ms,
                            result_count=n,
                            rss_mb=tr.rss_mb,
                        )
                    )
                    print(f"    jmespath:    {tr.min_ms:.3f}ms → {n} results")
                except ImportError:
                    pass
                except Exception as e:
                    print(f"    jmespath:    ERROR: {e}")

            # jsonpath-ng
            try:
                from jsonpath_ng import parse as jp_parse

                exprs = {
                    1: "$.users[*].id",
                    2: "$.users[*].orders[*].items[*].price",
                    3: "$..price",
                    4: "$.users[?(@.age > 30)]",
                }
                expr_str = exprs.get(query_id)
                if expr_str:
                    try:
                        compiled = jp_parse(expr_str)
                    except Exception:
                        print("    jsonpath-ng: SKIPPED (unsupported query)")
                        compiled = None
                    if compiled is not None:
                        last_result = None

                        def run_jp():
                            nonlocal last_result
                            last_result = [m.value for m in compiled.find(parsed)]
                            return last_result

                        tr = run_single_benchmark(
                            run_jp,
                            warmup=self.warmup,
                            repeat=self.repeat,
                            capture_rss=True,
                        )
                        n = len(last_result) if isinstance(last_result, list) else 1
                        self.results.append(
                            BenchResult(
                                library="jsonpath-ng",
                                operation="query",
                                dataset=dataset_name,
                                query=query_desc,
                                times_ms=tr.times_ms,
                                result_count=n,
                                rss_mb=tr.rss_mb,
                            )
                        )
                        print(f"    jsonpath-ng: {tr.min_ms:.3f}ms → {n} results")
            except ImportError:
                pass
            except Exception as e:
                print(f"    jsonpath-ng: ERROR: {e}")

    def run_all(self) -> None:
        """Run all benchmarks for configured datasets."""
        for dataset in self.datasets:
            path = Path(dataset)
            if not path.exists():
                print(f"Warning: Dataset not found: {dataset}")
                continue
            if path.suffix == ".ndjson":
                self.bench_parse_ndjson(str(path))
            else:
                self.bench_parse_json(str(path))
            if path.stem == "users" and path.suffix == ".json":
                self.bench_query_json(str(path))

    def print_summary(self) -> None:
        """Print summary tables to stdout."""
        print("\n" + "=" * 80)
        print("BENCHMARK SUMMARY")
        print("=" * 80)

        parse_results = [r for r in self.results if r.operation == "parse"]
        if parse_results:
            print("\nPARSING BENCHMARKS:")
            print(
                f"{'Library':<20} {'Dataset':<15} {'Min (ms)':<12} {'Median (ms)':<12} {'P95 (ms)':<12}"
            )
            print("-" * 80)
            parse_results.sort(key=lambda r: (r.dataset, r.median_ms))
            for r in parse_results:
                if r.error:
                    print(f"{r.library:<20} {r.dataset:<15} ERROR: {r.error}")
                else:
                    print(
                        f"{r.library:<20} {r.dataset:<15} {r.min_ms:>10.3f}  {r.median_ms:>10.3f}  {r.p95_ms:>10.3f}"
                    )

        query_results = [r for r in self.results if r.operation == "query"]
        if query_results:
            print("\nQUERY BENCHMARKS:")
            print(f"{'Library':<20} {'Query':<40} {'Min (ms)':<12} {'Results':<10}")
            print("-" * 80)
            by_query: dict[str, list[BenchResult]] = {}
            for r in query_results:
                by_query.setdefault(r.query, []).append(r)
            for query, results in by_query.items():
                results.sort(key=lambda r: r.min_ms)
                print(f"\n  {query}")
                for r in results:
                    if r.error:
                        print(f"    {r.library:<18} ERROR: {r.error}")
                    else:
                        print(
                            f"    {r.library:<18} {r.min_ms:>10.3f}ms    {r.result_count:>6} results"
                        )

    def save_results(self, output_path: str) -> None:
        """Write Markdown report to output_path."""
        out = Path(output_path)
        out.parent.mkdir(parents=True, exist_ok=True)
        lines = [
            "# Strata Benchmark Results",
            "",
            f"Generated: {time.strftime('%Y-%m-%d %H:%M:%S')}",
            "",
            "## Environment",
            "",
        ]
        if self.environment.get("commit"):
            lines.append(f"- Commit: {self.environment['commit']}")
        lines.extend(
            [
                f"- OS: {self.environment['os']}",
                f"- CPU: {self.environment['cpu']}",
                f"- Python: {self.environment['python']}",
                "",
                "## Configuration",
                "",
                f"- Repeat: {self.repeat}",
                f"- Warmup: {self.warmup}",
                f"- Datasets: {', '.join(self.datasets)}",
                "",
            ]
        )
        parse_results = [r for r in self.results if r.operation == "parse"]
        if parse_results:
            lines.extend(
                [
                    "## Parsing Benchmarks",
                    "",
                ]
            )
            parse_results.sort(key=lambda r: (r.dataset, r.median_ms))
            parse_rows: list[list[str]] = []
            for r in parse_results:
                if r.error:
                    parse_rows.append([r.library, r.dataset, "ERROR", "-", "-", "-"])
                else:
                    parse_rows.append(
                        [
                            r.library,
                            r.dataset,
                            f"{r.min_ms:.3f}",
                            f"{r.median_ms:.3f}",
                            f"{r.p95_ms:.3f}",
                            f"{r.rss_mb:.1f}",
                        ]
                    )
            lines.extend(
                build_markdown_table(
                    ["Library", "Dataset", "Min (ms)", "Median (ms)", "P95 (ms)", "RSS (MB)"],
                    parse_rows,
                )
            )

        query_results = [r for r in self.results if r.operation == "query"]
        if query_results:
            lines.extend(
                [
                    "",
                    "## Query Benchmarks",
                    "",
                ]
            )
            by_query: dict[str, list[BenchResult]] = {}
            for r in query_results:
                by_query.setdefault(r.query, []).append(r)
            query_rows: list[list[str]] = []
            for query, results in by_query.items():
                results.sort(key=lambda r: r.min_ms)
                for i, r in enumerate(results):
                    qcol = query if i == 0 else ""
                    if r.error:
                        query_rows.append([qcol, r.library, "ERROR", "-"])
                    else:
                        query_rows.append(
                            [qcol, r.library, f"{r.min_ms:.3f}", str(r.result_count)]
                        )
            lines.extend(
                build_markdown_table(
                    ["Query", "Library", "Min (ms)", "Results"],
                    query_rows,
                )
            )

        out.write_text("\n".join(lines) + "\n", encoding="utf-8")
        print(f"\nResults saved to {output_path}")


def main() -> None:
    print(
        "DEPRECATION: bench_main is superseded by benchmarks.bench_suite; use make bench-small/medium/large.",
        file=sys.stderr,
    )
    parser = argparse.ArgumentParser(description="Strata benchmark orchestrator")
    parser.add_argument(
        "--dataset", action="append", dest="datasets", help="Dataset path (repeatable)"
    )
    parser.add_argument("--repeat", type=int, default=3, help="Iterations per benchmark")
    parser.add_argument("--warmup", type=int, default=1, help="Warmup iterations")
    parser.add_argument("--limit", type=int, default=None, help="Limit records (unused)")
    parser.add_argument("--mode", default="auto", help="Mode (unused)")
    parser.add_argument("--materialize", default="auto", help="Materialize (unused)")
    parser.add_argument(
        "--strict-missing",
        type=lambda x: x.lower() != "off",
        default=True,
        help="Warn on missing libs",
    )
    parser.add_argument("--html", action="store_true", help="(Reserved)")
    parser.add_argument(
        "--output", default="docs/benchmarks/bench_results.md", help="Output Markdown path"
    )
    parser.add_argument("--regen-data", action="store_true", help="Regenerate benchmark data")
    args = parser.parse_args()

    if args.regen_data or (args.datasets and not all(Path(d).exists() for d in args.datasets)):
        print("Generating benchmark data...")
        info = generate_users_datasets(
            "benchmarks/data/generated",
            num_users=5000,
            max_orders_per_user=10,
            max_items_per_order=5,
        )
        print(f"Generated: {info['users_json']}, {info['users_ndjson']}")

    datasets = args.datasets or [
        "benchmarks/data/generated/users.json",
        "benchmarks/data/generated/users.ndjson",
    ]

    runner = BenchmarkRunner(
        datasets=datasets,
        repeat=args.repeat,
        warmup=args.warmup,
        limit=args.limit,
        strict_missing=args.strict_missing,
    )
    runner.run_all()
    runner.print_summary()
    runner.save_results(args.output)


if __name__ == "__main__":
    main()

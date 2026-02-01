#!/usr/bin/env python3
"""
JSONPath query benchmarks.

Compares Strata search/compile_path against jmespath and jsonpath-ng.
Uses shared harness for timing and RSS. Public API only (no _strata).

Note: Strata search(data, path) with a Python dict (e.g. from loads()) triggers
serialize→parse→query per call; jmespath/jsonpath-ng walk the dict in place.
For fair repeated-query comparison use parse_json_file() + search(cursor, path), or
see docs/benchmarks/strata_performance_analysis.md.
"""

from __future__ import annotations

import argparse
import gc
import json
import statistics
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from .harness import get_rss_mb

try:
    import jmespath
except ImportError:
    jmespath = None
try:
    from jsonpath_ng import parse as jsonpath_parse
except ImportError:
    jsonpath_parse = None

import strata


@dataclass
class QueryBenchResult:
    """Single query benchmark result."""

    library: str
    query_name: str
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


# Query definitions: strata expression, jmespath (or None), description
QUERIES: dict[str, dict[str, Any]] = {
    "simple_field": {
        "strata": "$.users[*].id",
        "jmespath": "users[*].id",
        "description": "Extract all user IDs",
    },
    "nested_field": {
        "strata": "$.users[*].metadata.created",
        "jmespath": "users[*].metadata.created",
        "description": "Extract nested timestamp field",
    },
    "double_wildcard": {
        "strata": "$.users[*].orders[*].items[*].price",
        "jmespath": "users[*].orders[*].items[*].price",
        "description": "Extract order item prices (double wildcard)",
    },
    "deep_path": {
        "strata": "$.users[0].orders[0].items[0].price",
        "jmespath": "users[0].orders[0].items[0].price",
        "description": "Deep path navigation",
    },
    "all_names": {
        "strata": "$.users[*].name",
        "jmespath": "users[*].name",
        "description": "Extract all user names",
    },
    "filter_numeric": {
        "strata": "$.users[?(@.age > 30)].name",
        "jmespath": "users[?age > `30`].name",
        "description": "Filter users by age (numeric predicate)",
    },
    "recursive_prices": {
        "strata": "$..price",
        "jmespath": None,
        "description": "Recursively find all prices",
    },
    "array_slice": {
        "strata": "$.users[0:10].id",
        "jmespath": "users[:10].id",
        "description": "Slice first 10 users, extract IDs",
    },
}


def _run_query_benchmark(
    run_func: Any,
    warmup: int,
    repeat: int,
) -> tuple[list[float], float]:
    """Warmup + repeat, return (times_ms, rss_mb)."""
    for _ in range(warmup):
        gc.collect()
        run_func()
    times_ms = []
    for _ in range(repeat):
        gc.collect()
        start = time.perf_counter()
        run_func()
        times_ms.append((time.perf_counter() - start) * 1000)
    return times_ms, get_rss_mb()


def run_all(
    data_file: Path,
    *,
    repeat: int = 5,
    warmup: int = 1,
    strata_mode: str = "cursor",
) -> list[QueryBenchResult]:
    """Run all query benchmarks; return list of QueryBenchResult.

    strata_mode: "cursor" (default) = parse_json_file once, then search(cursor, path) [query only];
                 "string" = search(text, path) [parse+query per call, no dumps];
                 "dict" = search(loads(text), path) [dumps+parse+query per call]. Use for loads()+search() comparison.
    """
    data_file = Path(data_file)
    json_bytes = data_file.read_bytes()
    json_text = json_bytes.decode("utf-8")
    json_data = json.loads(json_text)

    print()
    print("=" * 70)
    print("JSONPath Query Benchmarks")
    print("=" * 70)
    print(f"Data file: {data_file}")
    print(f"Size: {len(json_bytes) / 1024 / 1024:.2f} MB")
    print(f"Repeat: {repeat}, Warmup: {warmup}, Strata mode: {strata_mode}")
    print()

    results: list[QueryBenchResult] = []

    for query_name, query_def in QUERIES.items():
        description = query_def["description"]
        print(f"\n--- Query: {description} ---")

        # Strata (public API; mode controls input to search() for fair comparison)
        print("  strata:       ", end="", flush=True)
        try:
            path = strata.compile_path(query_def["strata"])

            if strata_mode == "dict":
                parsed_strata = strata.loads(json_text)

                def run_strata():
                    return strata.search(parsed_strata, path)

            elif strata_mode == "string":

                def run_strata():
                    return strata.search(json_text, path)

            else:  # cursor: parse once per query, time query only
                cursor = strata.parse_json_file(str(data_file))

                def run_strata():
                    return strata.search(cursor, path)

            times_ms, rss_mb = _run_query_benchmark(run_strata, warmup, repeat)
            result_list = run_strata()
            n = len(result_list) if isinstance(result_list, list) else 1
            results.append(
                QueryBenchResult(
                    library="strata",
                    query_name=query_name,
                    times_ms=times_ms,
                    result_count=n,
                    rss_mb=rss_mb,
                )
            )
            r = results[-1]
            print(
                f"min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, results={r.result_count}, rss={r.rss_mb:.1f} MB"
            )
        except Exception as e:
            print(f"ERROR: {e}")
            results.append(QueryBenchResult(library="strata", query_name=query_name, error=str(e)))

        # jmespath
        jmes_expr = query_def.get("jmespath")
        if jmespath and jmes_expr:
            print("  jmespath:     ", end="", flush=True)
            try:
                compiled = jmespath.compile(jmes_expr)

                def run_jmespath():
                    return compiled.search(json_data)

                times_ms, rss_mb = _run_query_benchmark(run_jmespath, warmup, repeat)
                res = compiled.search(json_data)
                n = len(res) if isinstance(res, (list, tuple)) else 1
                results.append(
                    QueryBenchResult(
                        library="jmespath",
                        query_name=query_name,
                        times_ms=times_ms,
                        result_count=n,
                        rss_mb=rss_mb,
                    )
                )
                r = results[-1]
                print(
                    f"min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, results={r.result_count}, rss={r.rss_mb:.1f} MB"
                )
            except Exception as e:
                print(f"ERROR: {e}")
                results.append(
                    QueryBenchResult(library="jmespath", query_name=query_name, error=str(e))
                )
        else:
            if jmespath:
                print("  jmespath:     SKIPPED (unsupported query)")
            else:
                print("  jmespath:     NOT INSTALLED")

        # jsonpath-ng (supports strata-style expressions for most)
        if jsonpath_parse:
            print("  jsonpath-ng:  ", end="", flush=True)
            expr_str = query_def.get("strata")
            if expr_str:
                try:
                    compiled = jsonpath_parse(expr_str)

                    def run_jp():
                        return [m.value for m in compiled.find(json_data)]

                    times_ms, rss_mb = _run_query_benchmark(run_jp, warmup, repeat)
                    n = len(compiled.find(json_data))
                    results.append(
                        QueryBenchResult(
                            library="jsonpath-ng",
                            query_name=query_name,
                            times_ms=times_ms,
                            result_count=n,
                            rss_mb=rss_mb,
                        )
                    )
                    r = results[-1]
                    print(
                        f"min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, results={r.result_count}, rss={r.rss_mb:.1f} MB"
                    )
                except Exception as e:
                    print(f"ERROR: {e}")
                    results.append(
                        QueryBenchResult(library="jsonpath-ng", query_name=query_name, error=str(e))
                    )
            else:
                print("  jsonpath-ng:  SKIPPED")
        else:
            print("  jsonpath-ng:  NOT INSTALLED")

    return results


def print_summary(results: list[QueryBenchResult]) -> None:
    """Print summary grouped by query."""
    print()
    print("=" * 70)
    print("SUMMARY")
    print("=" * 70)

    by_query: dict[str, list[QueryBenchResult]] = {}
    for r in results:
        by_query.setdefault(r.query_name, []).append(r)

    for query_name, query_results in by_query.items():
        query_def = QUERIES.get(query_name, {})
        desc = query_def.get("description", query_name)
        print(f"\n{desc}:")
        print(
            f"{'Library':<15} {'Min (ms)':>12} {'Median (ms)':>12} {'P95 (ms)':>12} {'Results':>10} {'RSS (MB)':>9} {'Speedup':>10}"
        )
        print("-" * 92)

        baseline_median = None
        for r in query_results:
            if r.library == "jmespath" and not r.error:
                baseline_median = r.median_ms
                break

        for r in sorted(query_results, key=lambda x: x.median_ms if x.times_ms else float("inf")):
            if r.error:
                print(f"{r.library:<15} ERROR")
            else:
                speedup_str = ""
                if baseline_median and r.median_ms > 0:
                    speedup_str = f"{baseline_median / r.median_ms:.2f}x"
                print(
                    f"{r.library:<15} {r.min_ms:>12.2f} {r.median_ms:>12.2f} "
                    f"{r.p95_ms:>12.2f} {r.result_count:>10} {r.rss_mb:>9.1f} {speedup_str:>10}"
                )


def main() -> int:
    parser = argparse.ArgumentParser(description="Run JSONPath query benchmarks")
    parser.add_argument(
        "--data", type=Path, default=Path("benchmarks/data/generated/users.json"), help="JSON file"
    )
    parser.add_argument("--repeat", type=int, default=5, help="Iterations")
    parser.add_argument("--warmup", type=int, default=1, help="Warmup iterations")
    parser.add_argument(
        "--strata-mode",
        choices=["dict", "string", "cursor"],
        default="cursor",
        help="Strata input: cursor=parse_json_file then search(cursor,path) [query only, default]; dict=search(loads(text),path); string=search(text,path). Use --strata-mode dict for loads()+search() comparison.",
    )
    args = parser.parse_args()

    if not args.data.exists():
        print(f"Error: Data file not found: {args.data}")
        return 1

    results = run_all(
        args.data,
        repeat=args.repeat,
        warmup=args.warmup,
        strata_mode=args.strata_mode,
    )
    print_summary(results)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

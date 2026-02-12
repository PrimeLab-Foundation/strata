#!/usr/bin/env python3
"""
Search query benchmarks.

Compares Strata search against jmespath and jsonpath-ng.
Uses shared harness for timing and RSS. Uses internal Strata bindings for fair modes.

Note: Strata search(data, path) with a Python dict (e.g. from loads()) triggers
serialize→parse→search per call; jmespath/jsonpath-ng walk the dict in place.
For fair repeated-query comparison use parse_json_file() + search(cursor, path)
for JSON or NdjsonCursor.from_file() + search(cursor, path) for NDJSON.
This benchmark aligns parse cost with the selected Strata mode: cursor modes are
query-only (pre-parsed data for all libraries), while string/dict modes include
parsing per call for all libraries.
"""

from __future__ import annotations

import argparse
import gc
import json
import os
import re
import statistics
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from .harness import get_rss_mb, run_single_benchmark
from .markdown_tables import build_markdown_table

try:
    import jmespath
except ImportError:
    jmespath = None
try:
    from jsonpath_ng import parse as jsonpath_parse
except ImportError:
    jsonpath_parse = None

import strata
from strata import _strata as _native


@dataclass
class QueryBenchResult:
    """Single query benchmark result."""

    library: str
    query_name: str
    times_ms: list[float] = field(default_factory=list)
    result_count: int = 0
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
        if not self.times_ms:
            return 0.0
        sorted_times = sorted(self.times_ms)
        idx = min(int(len(sorted_times) * 0.95), len(sorted_times) - 1)
        return sorted_times[idx]


@dataclass
class CursorReuseResult:
    """Cursor reuse benchmark result (batch of queries)."""

    label: str
    times_ms: list[float] = field(default_factory=list)
    rss_mb: float = 0.0
    throughput_mbps: float = 0.0
    queries_run: int = 0
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

# NDJSON variant: root is a list of user objects, not {"users": [...]}
NDJSON_QUERIES: dict[str, dict[str, Any]] = {
    "root_field": {
        "strata": "$.id",
        "jmespath": None,
        "description": "NDJSON root field (id)",
    },
    "simple_field": {
        "strata": "$[*].id",
        "jmespath": "[].id",
        "description": "Extract all user IDs",
    },
    "nested_field": {
        "strata": "$[*].metadata.created",
        "jmespath": "[].metadata.created",
        "description": "Extract nested timestamp field",
    },
    "double_wildcard": {
        "strata": "$[*].orders[*].items[*].price",
        "jmespath": "[].orders[].items[].price",
        "description": "Extract order item prices (double wildcard)",
    },
    "deep_path": {
        "strata": "$[0].orders[0].items[0].price",
        "jmespath": "[0].orders[0].items[0].price",
        "description": "Deep path navigation",
    },
    "all_names": {
        "strata": "$[*].name",
        "jmespath": "[].name",
        "description": "Extract all user names",
    },
    "filter_numeric": {
        "strata": "$[?(@.age > 30)].name",
        "jmespath": "[?age > `30`].name",
        "description": "Filter users by age (numeric predicate)",
    },
    "recursive_prices": {
        "strata": "$..price",
        "jmespath": None,
        "description": "Recursively find all prices",
    },
    "array_slice": {
        "strata": "$[0:10].id",
        "jmespath": "[:10].id",
        "description": "Slice first 10 users, extract IDs",
    },
}

_SIMPLE_FIELD_RE = re.compile(r"^\$\.[A-Za-z0-9_]+$|^\$\[\*\]\.[A-Za-z0-9_]+$")


def _is_simple_field_query(expr: str) -> bool:
    return bool(_SIMPLE_FIELD_RE.match(expr))


def _throughput_mbps(data_size_bytes: int | None, times_ms: list[float]) -> float:
    if not data_size_bytes or not times_ms:
        return 0.0
    median_ms = statistics.median(times_ms)
    if median_ms <= 0:
        return 0.0
    return (data_size_bytes / (median_ms / 1000.0)) / 1e6


def _validate_ndjson_queries() -> None:
    missing = set(QUERIES) - set(NDJSON_QUERIES)
    if missing:
        raise ValueError(f"NDJSON_QUERIES missing entries: {sorted(missing)}")


def _run_query_benchmark(
    run_func: Any,
    warmup: int,
    repeat: int,
) -> tuple[list[float], float, Any]:
    """Warmup + repeat, return (times_ms, rss_mb, last_result)."""
    last_result = None
    for _ in range(warmup):
        gc.collect()
        last_result = run_func()
    times_ms = []
    for _ in range(repeat):
        gc.collect()
        start = time.perf_counter()
        last_result = run_func()
        times_ms.append((time.perf_counter() - start) * 1000)
    return times_ms, get_rss_mb(), last_result


def _ndjson_lines(text: str) -> list[str]:
    return [line for line in text.splitlines() if line.strip()]


def _count_ndjson_matches(result: Any) -> int:
    if isinstance(result, list):
        if result and isinstance(result[0], dict) and "matches" in result[0]:
            return sum(len(entry.get("matches", [])) for entry in result)
        return len(result)
    return 0


def _load_json_data(
    data_file: Path,
) -> tuple[str, Any, int, float, int, bool]:
    """Load JSON or NDJSON data for querying; return (text, data, size_bytes, size_mb, records, is_ndjson)."""
    if data_file.suffix == ".ndjson":
        ndjson_text = data_file.read_text(encoding="utf-8")
        lines = _ndjson_lines(ndjson_text)
        json_text = "[" + ",".join(lines) + "]"
        json_data = [json.loads(line) for line in lines]
        size_bytes = data_file.stat().st_size
        size_mb = size_bytes / 1024 / 1024
        record_count = len(json_data)
        return json_text, json_data, size_bytes, size_mb, record_count, True

    json_bytes = data_file.read_bytes()
    json_text = json_bytes.decode("utf-8")
    json_data = json.loads(json_text)
    size_bytes = len(json_bytes)
    size_mb = size_bytes / 1024 / 1024
    if isinstance(json_data, dict) and "users" in json_data:
        record_count = len(json_data.get("users") or [])
    elif isinstance(json_data, list):
        record_count = len(json_data)
    else:
        record_count = 1
    return json_text, json_data, size_bytes, size_mb, record_count, False


def _run_ndjson_limit_benchmark(
    ndjson_text: str,
    data_size_bytes: int,
    *,
    warmup: int,
    repeat: int,
    parallel_kwargs: dict[str, Any],
) -> None:
    if not ndjson_text:
        return

    expr = "$.id"
    limit = 10

    print("\n--- NDJSON Limit Benchmark (strata.search) ---")
    print(f"  query={expr}, limit={limit}")

    def run_full():
        return strata.search(ndjson_text, expr, ndjson=True, **parallel_kwargs)

    def run_limit():
        return strata.search(ndjson_text, expr, ndjson=True, limit=limit, **parallel_kwargs)

    times_full, rss_full, result_full = _run_query_benchmark(run_full, warmup, repeat)
    times_limit, rss_limit, result_limit = _run_query_benchmark(run_limit, warmup, repeat)

    full_median = statistics.median(times_full) if times_full else 0.0
    limit_median = statistics.median(times_limit) if times_limit else 0.0
    full_matches = _count_ndjson_matches(result_full)
    limit_matches = _count_ndjson_matches(result_limit)

    print(
        f"  full:  median={full_median:.2f}ms, mbps={_throughput_mbps(data_size_bytes, times_full):.2f}, "
        f"matches={full_matches}, rss={rss_full:.1f} MB"
    )
    print(
        f"  limit: median={limit_median:.2f}ms, mbps={_throughput_mbps(data_size_bytes, times_limit):.2f}, "
        f"matches={limit_matches}, rss={rss_limit:.1f} MB"
    )
    if limit_median > 0:
        print(f"  speedup (full vs limit): {full_median / limit_median:.2f}x")


def _run_cursor_reuse_benchmarks(
    data_file: Path,
    queries: dict[str, dict[str, Any]],
    *,
    repeat: int,
    warmup: int,
    is_ndjson: bool,
    data_size_bytes: int,
) -> list[CursorReuseResult]:
    if not queries:
        return []

    compiled_paths: list[Any] = []
    for query_def in queries.values():
        compiled_paths.append(_native.compile_path(query_def["strata"]))

    queries_run = len(compiled_paths)
    total_bytes = data_size_bytes * queries_run if data_size_bytes else 0

    def build_cursor() -> Any:
        if is_ndjson:
            return _native.NdjsonCursor.from_file(str(data_file))
        _document, cursor = _native.parse_json_file(str(data_file))
        return cursor

    def run_reuse():
        cursor = build_cursor()
        last_result = None
        for path in compiled_paths:
            last_result = _native.search(cursor, path)
        return last_result

    def run_reparse():
        last_result = None
        for path in compiled_paths:
            cursor = build_cursor()
            last_result = _native.search(cursor, path)
        return last_result

    results: list[CursorReuseResult] = []
    for label, run_func in [
        ("strata_cursor_reuse", run_reuse),
        ("strata_cursor_reparse", run_reparse),
    ]:
        try:
            tr = run_single_benchmark(
                run_func,
                warmup=warmup,
                repeat=repeat,
                capture_rss=True,
                data_size_bytes=total_bytes,
            )
            results.append(
                CursorReuseResult(
                    label=label,
                    times_ms=tr.times_ms,
                    rss_mb=tr.rss_mb,
                    throughput_mbps=tr.throughput_mbps,
                    queries_run=queries_run,
                )
            )
        except Exception as e:
            results.append(
                CursorReuseResult(label=label, queries_run=queries_run, error=str(e))
            )

    return results


def run_all(
    data_file: Path,
    *,
    repeat: int = 5,
    warmup: int = 1,
    strata_mode: str = "cursor",
    ndjson_parallel: str = "auto",
    cursor_reuse: bool = True,
) -> tuple[list[QueryBenchResult], list[CursorReuseResult]]:
    """Run all query benchmarks; return (query_results, cursor_reuse_results).

    strata_mode: "cursor" (default) = parse_json_file (JSON) or NdjsonCursor.from_file (NDJSON)
                 once, then search(cursor, path) [query only];
                 "string" = search(text, path) [parse+query per call, no dumps];
                 "dict" = search(loads(text), path) [dumps+parse+query per call]. Use for loads()+search() comparison.
    cursor_reuse: if True, run a batch benchmark that compares cursor reuse vs reparse for all queries.
    """
    data_file = Path(data_file)
    json_text, json_data, size_bytes, size_mb, record_count, is_ndjson = _load_json_data(
        data_file
    )
    if is_ndjson:
        _validate_ndjson_queries()
    ndjson_text = data_file.read_text(encoding="utf-8") if is_ndjson else ""
    queries = NDJSON_QUERIES if is_ndjson else QUERIES
    effective_strata_mode = strata_mode
    strata_mode_label = strata_mode
    if is_ndjson and strata_mode == "cursor":
        effective_strata_mode = "ndjson_cursor"
        strata_mode_label = "ndjson_cursor"

    print()
    print("=" * 70)
    print("Search Query Benchmarks")
    print("=" * 70)
    print(f"Data file: {data_file}")
    print(f"Size: {size_mb:.2f} MB")
    if record_count:
        print(f"Records: {record_count}")
    print(f"Repeat: {repeat}, Warmup: {warmup}, Strata mode: {strata_mode_label}")
    print()

    results: list[QueryBenchResult] = []
    cursor_reuse_results: list[CursorReuseResult] = []

    parallel_kwargs: dict[str, Any] = {}
    if is_ndjson and ndjson_parallel != "auto":
        parallel_kwargs["parallel"] = ndjson_parallel == "true"

    parse_each_run = effective_strata_mode in ("string", "dict")

    for query_name, query_def in queries.items():
        description = query_def["description"]
        print(f"\n--- Query: {description} ---")

        # Strata (public API; mode controls input to search() for fair comparison)
        print("  strata:       ", end="", flush=True)
        compiled_ok = False
        try:
            path = _native.compile_path(query_def["strata"])
            compiled_ok = True

            if effective_strata_mode == "dict":
                parsed_strata = strata.loads(json_text)

                def run_strata():
                    return strata.query(parsed_strata, path)

            elif effective_strata_mode == "string":

                def run_strata():
                    return _native.search(json_text, path)

            elif effective_strata_mode == "ndjson_cursor":
                cursor = _native.NdjsonCursor.from_file(str(data_file))

                def run_strata():
                    return _native.search(cursor, path)

            else:  # cursor: parse once per query, time query only
                _document, cursor = _native.parse_json_file(str(data_file))

                def run_strata():
                    return _native.search(cursor, path)

            times_ms, rss_mb, result_list = _run_query_benchmark(run_strata, warmup, repeat)
            n = len(result_list) if isinstance(result_list, list) else 1
            results.append(
                QueryBenchResult(
                    library="strata",
                    query_name=query_name,
                    times_ms=times_ms,
                    result_count=n,
                    rss_mb=rss_mb,
                    throughput_mbps=_throughput_mbps(size_bytes, times_ms),
                )
            )
            r = results[-1]
            print(
                f"min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, "
                f"mbps={r.throughput_mbps:.2f}, results={r.result_count}, rss={r.rss_mb:.1f} MB"
            )
        except Exception as e:
            print(f"ERROR: {e}")
            results.append(QueryBenchResult(library="strata", query_name=query_name, error=str(e)))

        # Fused NDJSON path vs full parse path for simple field extraction
        if compiled_ok and is_ndjson and _is_simple_field_query(query_def["strata"]):
            print("  strata_ndjson_full: ", end="", flush=True)
            try:
                os.environ["STRATA_DISABLE_FUSED_NDJSON"] = "1"

                def run_full():
                    return _native.search(ndjson_text, path, ndjson=True, **parallel_kwargs)

                times_ms, rss_mb, result_list = _run_query_benchmark(run_full, warmup, repeat)
                n = len(result_list) if isinstance(result_list, list) else 1
                results.append(
                    QueryBenchResult(
                        library="strata_ndjson_full",
                        query_name=query_name,
                        times_ms=times_ms,
                        result_count=n,
                        rss_mb=rss_mb,
                        throughput_mbps=_throughput_mbps(size_bytes, times_ms),
                    )
                )
                r = results[-1]
                print(
                    f"min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, "
                    f"mbps={r.throughput_mbps:.2f}, results={r.result_count}, rss={r.rss_mb:.1f} MB"
                )
            except Exception as e:
                print(f"ERROR: {e}")
                results.append(
                    QueryBenchResult(
                        library="strata_ndjson_full", query_name=query_name, error=str(e)
                    )
                )
            finally:
                os.environ.pop("STRATA_DISABLE_FUSED_NDJSON", None)

            print("  strata_ndjson_fused:", end="", flush=True)
            try:

                def run_fused():
                    return _native.search(ndjson_text, path, ndjson=True, **parallel_kwargs)

                times_ms, rss_mb, result_list = _run_query_benchmark(run_fused, warmup, repeat)
                n = len(result_list) if isinstance(result_list, list) else 1
                results.append(
                    QueryBenchResult(
                        library="strata_ndjson_fused",
                        query_name=query_name,
                        times_ms=times_ms,
                        result_count=n,
                        rss_mb=rss_mb,
                        throughput_mbps=_throughput_mbps(size_bytes, times_ms),
                    )
                )
                r = results[-1]
                print(
                    f"min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, "
                    f"mbps={r.throughput_mbps:.2f}, results={r.result_count}, rss={r.rss_mb:.1f} MB"
                )
            except Exception as e:
                print(f"ERROR: {e}")
                results.append(
                    QueryBenchResult(
                        library="strata_ndjson_fused", query_name=query_name, error=str(e)
                    )
                )

        # jmespath
        jmes_expr = query_def.get("jmespath")
        if jmespath and jmes_expr:
            print("  jmespath:     ", end="", flush=True)
            try:
                compiled = jmespath.compile(jmes_expr)

                if parse_each_run:

                    def run_jmespath():
                        return compiled.search(json.loads(json_text))

                else:

                    def run_jmespath():
                        return compiled.search(json_data)

                times_ms, rss_mb, res = _run_query_benchmark(run_jmespath, warmup, repeat)
                n = len(res) if isinstance(res, (list, tuple)) else 1
                results.append(
                    QueryBenchResult(
                        library="jmespath",
                        query_name=query_name,
                        times_ms=times_ms,
                        result_count=n,
                        rss_mb=rss_mb,
                        throughput_mbps=_throughput_mbps(size_bytes, times_ms),
                    )
                )
                r = results[-1]
                print(
                    f"min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, "
                    f"mbps={r.throughput_mbps:.2f}, results={r.result_count}, rss={r.rss_mb:.1f} MB"
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
                except Exception:
                    print("SKIPPED (unsupported query)")
                    continue
                try:

                    def run_jp():
                        if parse_each_run:
                            data = json.loads(json_text)
                        else:
                            data = json_data
                        return [m.value for m in compiled.find(data)]

                    times_ms, rss_mb, res = _run_query_benchmark(run_jp, warmup, repeat)
                    n = len(res) if isinstance(res, list) else 1
                    results.append(
                        QueryBenchResult(
                            library="jsonpath-ng",
                            query_name=query_name,
                            times_ms=times_ms,
                            result_count=n,
                            rss_mb=rss_mb,
                            throughput_mbps=_throughput_mbps(size_bytes, times_ms),
                        )
                    )
                    r = results[-1]
                    print(
                        f"min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, "
                        f"mbps={r.throughput_mbps:.2f}, results={r.result_count}, rss={r.rss_mb:.1f} MB"
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

    if is_ndjson:
        _run_ndjson_limit_benchmark(
            ndjson_text,
            size_bytes,
            warmup=warmup,
            repeat=repeat,
            parallel_kwargs=parallel_kwargs,
        )

    if cursor_reuse:
        print("\n--- Cursor Reuse Benchmark (compiled paths, all queries) ---")
        cursor_reuse_results = _run_cursor_reuse_benchmarks(
            data_file,
            queries,
            repeat=repeat,
            warmup=warmup,
            is_ndjson=is_ndjson,
            data_size_bytes=size_bytes,
        )
        for r in cursor_reuse_results:
            if r.error:
                print(f"  {r.label}: ERROR: {r.error}")
            else:
                print(
                    f"  {r.label}: min={r.min_ms:.2f}ms, median={r.median_ms:.2f}ms, "
                    f"p95={r.p95_ms:.2f}ms, mbps={r.throughput_mbps:.2f}, "
                    f"rss={r.rss_mb:.1f} MB"
                )
        reuse = next((r for r in cursor_reuse_results if r.label == "strata_cursor_reuse"), None)
        reparse = next(
            (r for r in cursor_reuse_results if r.label == "strata_cursor_reparse"), None
        )
        if reuse and reparse and not reuse.error and not reparse.error and reuse.median_ms > 0:
            speedup = reparse.median_ms / reuse.median_ms
            print(f"  speedup (reuse vs reparse): {speedup:.2f}x")

    return results, cursor_reuse_results


def print_summary(
    results: list[QueryBenchResult],
    cursor_reuse_results: list[CursorReuseResult] | None = None,
) -> None:
    """Print summary grouped by query."""
    print()
    print("=" * 70)
    print("SUMMARY")
    print("=" * 70)

    use_ndjson = any(
        r.query_name not in QUERIES and r.query_name in NDJSON_QUERIES for r in results
    )
    query_defs = NDJSON_QUERIES if use_ndjson else QUERIES
    desc_map = {name: q["description"] for name, q in query_defs.items()}

    by_query: dict[str, list[QueryBenchResult]] = {}
    for r in results:
        by_query.setdefault(r.query_name, []).append(r)

    for query_name, query_results in by_query.items():
        desc = desc_map.get(query_name, query_name)
        print(f"\n{desc}:")
        print(
            f"{'Library':<15} {'Min (ms)':>12} {'Median (ms)':>12} {'P95 (ms)':>12} "
            f"{'MB/s':>10} {'Results':>10} {'RSS (MB)':>9} {'Speedup':>10}"
        )
        print("-" * 104)

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
                    f"{r.p95_ms:>12.2f} {r.throughput_mbps:>10.2f} "
                    f"{r.result_count:>10} {r.rss_mb:>9.1f} {speedup_str:>10}"
                )

    if cursor_reuse_results:
        print()
        print("=" * 70)
        print("CURSOR REUSE SUMMARY")
        print("=" * 70)
        print(
            f"{'Mode':<22} {'Min (ms)':>12} {'Median (ms)':>12} {'P95 (ms)':>12} "
            f"{'MB/s':>10} {'RSS (MB)':>9}"
        )
        print("-" * 80)
        for r in cursor_reuse_results:
            if r.error:
                print(f"{r.label:<22} ERROR")
            else:
                print(
                    f"{r.label:<22} {r.min_ms:>12.2f} {r.median_ms:>12.2f} "
                    f"{r.p95_ms:>12.2f} {r.throughput_mbps:>10.2f} {r.rss_mb:>9.1f}"
                )
        reuse = next(
            (r for r in cursor_reuse_results if r.label == "strata_cursor_reuse"), None
        )
        reparse = next(
            (r for r in cursor_reuse_results if r.label == "strata_cursor_reparse"), None
        )
        if reuse and reparse and not reuse.error and not reparse.error and reuse.median_ms > 0:
            speedup = reparse.median_ms / reuse.median_ms
            print(f"\nSpeedup (reuse vs reparse): {speedup:.2f}x")


def _format_markdown(
    results: list[QueryBenchResult],
    cursor_reuse_results: list[CursorReuseResult] | None,
    data_file: Path,
    size_mb: float,
    record_count: int,
    is_ndjson: bool,
    repeat: int,
    warmup: int,
    strata_mode: str,
) -> str:
    label = "NDJSON" if is_ndjson else "JSON"
    lines = [
        f"### {data_file.name} ({label})",
        "",
        f"- Source: {data_file}",
        f"- Size: {size_mb:.2f} MB",
        f"- Records: {record_count}",
        f"- Repeat: {repeat}",
        f"- Warmup: {warmup}",
        f"- Strata mode: {strata_mode}",
        "",
    ]

    if not results:
        lines.append("*No results available*")
        lines.append("")
        return "\n".join(lines)

    queries = NDJSON_QUERIES if is_ndjson else QUERIES
    desc_map = {name: q["description"] for name, q in queries.items()}
    table_rows: list[list[str]] = []
    for r in sorted(results, key=lambda x: (desc_map.get(x.query_name, x.query_name), x.median_ms)):
        desc = desc_map.get(r.query_name, r.query_name)
        if r.error:
            table_rows.append([desc, r.library, "ERROR", "-", "-", "-", "-", "-"])
            continue
        table_rows.append(
            [
                desc,
                r.library,
                f"{r.min_ms:.2f}",
                f"{r.median_ms:.2f}",
                f"{r.p95_ms:.2f}",
                f"{r.throughput_mbps:.2f}",
                str(r.result_count),
                f"{r.rss_mb:.1f}",
            ]
        )
    lines.extend(
        build_markdown_table(
            [
                "Query",
                "Library",
                "Min (ms)",
                "Median (ms)",
                "P95 (ms)",
                "MB/s",
                "Results",
                "RSS (MB)",
            ],
            table_rows,
        )
    )
    if cursor_reuse_results:
        lines.extend(
            [
                "",
                "#### Cursor Reuse (All Queries)",
                "",
            ]
        )
        reuse_rows: list[list[str]] = []
        for r in cursor_reuse_results:
            if r.error:
                reuse_rows.append([r.label, "ERROR", "-", "-", "-", "-"])
            else:
                reuse_rows.append(
                    [
                        r.label,
                        f"{r.min_ms:.2f}",
                        f"{r.median_ms:.2f}",
                        f"{r.p95_ms:.2f}",
                        f"{r.throughput_mbps:.2f}",
                        f"{r.rss_mb:.1f}",
                    ]
                )
        lines.extend(
            build_markdown_table(
                ["Mode", "Min (ms)", "Median (ms)", "P95 (ms)", "MB/s", "RSS (MB)"],
                reuse_rows,
            )
        )
        reuse = next(
            (r for r in cursor_reuse_results if r.label == "strata_cursor_reuse"), None
        )
        reparse = next(
            (r for r in cursor_reuse_results if r.label == "strata_cursor_reparse"), None
        )
        if reuse and reparse and not reuse.error and not reparse.error and reuse.median_ms > 0:
            speedup = reparse.median_ms / reuse.median_ms
            lines.append(f"- Speedup (reuse vs reparse): {speedup:.2f}x")
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


def main() -> int:
    parser = argparse.ArgumentParser(description="Run search query benchmarks")
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("benchmarks/data/generated/users.json"),
        help="JSON or NDJSON file",
    )
    parser.add_argument("--repeat", type=int, default=5, help="Iterations")
    parser.add_argument("--warmup", type=int, default=1, help="Warmup iterations")
    parser.add_argument(
        "--strata-mode",
        choices=["dict", "string", "cursor"],
        default="cursor",
        help="Strata input: cursor=parse_json_file (JSON) or NdjsonCursor.from_file (NDJSON) then search(cursor,path) [query only, default]; dict=search(loads(text),path); string=search(text,path). Use --strata-mode dict for loads()+search() comparison.",
    )
    parser.add_argument(
        "--ndjson-parallel",
        choices=["auto", "true", "false"],
        default="auto",
        help="For NDJSON string benchmarks, pass parallel flag to strata.search (auto, true, false).",
    )
    parser.add_argument(
        "--no-cursor-reuse",
        action="store_true",
        help="Disable cursor reuse benchmark (parse once vs reparse per query).",
    )
    parser.add_argument("--output", type=Path, help="Write Markdown results to file")
    parser.add_argument("--append", action="store_true", help="Append to --output if set")
    args = parser.parse_args()

    if not args.data.exists():
        print(f"Error: Data file not found: {args.data}")
        return 1

    results, cursor_reuse_results = run_all(
        args.data,
        repeat=args.repeat,
        warmup=args.warmup,
        strata_mode=args.strata_mode,
        ndjson_parallel=args.ndjson_parallel,
        cursor_reuse=not args.no_cursor_reuse,
    )
    print_summary(results, cursor_reuse_results)
    if args.output:
        _json_text, _json_data, _size_bytes, size_mb, record_count, is_ndjson = _load_json_data(
            args.data
        )
        strata_mode_label = args.strata_mode
        if is_ndjson and args.strata_mode == "cursor":
            strata_mode_label = "ndjson_cursor"
        body = _format_markdown(
            results,
            cursor_reuse_results,
            args.data,
            size_mb,
            record_count,
            is_ndjson,
            args.repeat,
            args.warmup,
            strata_mode_label,
        )
        _append_markdown_section(args.output, "Search Benchmarks", body, args.append)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

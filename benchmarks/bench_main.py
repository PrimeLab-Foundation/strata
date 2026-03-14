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
import os
import platform
import statistics
import subprocess
import sys
import sysconfig
import tempfile
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Callable

from .harness import run_single_benchmark
from .data.generate_bench_data import generate_users_datasets
from .eval_queries import (
    get_query_description,
)
from .bench_dumps import run_benchmarks as run_dumps_benchmarks

import strata


# -----------------------------------------------------------------------------
# Result type
# -----------------------------------------------------------------------------


@dataclass
class BenchResult:
    """Single benchmark result (parse, dumps, or query)."""

    library: str
    operation: str
    dataset: str
    query: str = ""
    times_ms: list[float] = field(default_factory=list)
    result_count: int = 0
    rss_mb: float = 0.0
    error: str = ""
    # For dumps results where we only get pre-computed stats:
    dumps_p95_ms: float = 0.0

    @property
    def min_ms(self) -> float:
        return min(self.times_ms) if self.times_ms else 0.0

    @property
    def median_ms(self) -> float:
        return statistics.median(self.times_ms) if self.times_ms else 0.0

    @property
    def p95_ms(self) -> float:
        if self.dumps_p95_ms > 0:
            return self.dumps_p95_ms
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


def _get_load_json_runners(strict_missing: bool) -> list[tuple[str, Callable[[str], Any]]]:
    """Return [(library_name, load_func)] for file-based JSON loading. load_func(filepath) -> parsed."""
    runners: list[tuple[str, Callable[[str], Any]]] = []

    try:
        import strata

        runners.append(("strata", lambda p: strata.load(p)))
    except ImportError:
        if strict_missing:
            print("Warning: strata not installed, skipping load benchmarks")

    try:
        import orjson

        runners.append(("orjson", lambda p: orjson.loads(Path(p).read_bytes())))
    except ImportError:
        pass

    try:
        import ujson

        runners.append(("ujson", lambda p: ujson.loads(Path(p).read_text(encoding="utf-8"))))
    except ImportError:
        pass

    try:
        import msgspec

        runners.append(("msgspec", lambda p: msgspec.json.decode(Path(p).read_bytes())))
    except ImportError:
        pass

    runners.append(("json (stdlib)", lambda p: json.load(open(p, encoding="utf-8"))))
    return runners


def _get_dump_json_runners(
    strict_missing: bool,
) -> list[tuple[str, Callable[[Any, str], None]]]:
    """Return [(library_name, dump_func)] for file-based JSON dumping. dump_func(data, filepath)."""
    runners: list[tuple[str, Callable[[Any, str], None]]] = []

    try:
        import strata

        runners.append(("strata", lambda d, p: strata.dump(d, p)))
    except ImportError:
        if strict_missing:
            print("Warning: strata not installed, skipping dump benchmarks")

    try:
        import orjson

        def orjson_dump(d, p):
            Path(p).write_bytes(orjson.dumps(d))

        runners.append(("orjson", orjson_dump))
    except ImportError:
        pass

    try:
        import ujson

        def ujson_dump(d, p):
            with open(p, "w", encoding="utf-8") as f:
                ujson.dump(d, f)

        runners.append(("ujson", ujson_dump))
    except ImportError:
        pass

    try:
        import msgspec

        def msgspec_dump(d, p):
            Path(p).write_bytes(msgspec.json.encode(d))

        runners.append(("msgspec", msgspec_dump))
    except ImportError:
        pass

    def stdlib_dump(d, p):
        with open(p, "w", encoding="utf-8") as f:
            json.dump(d, f)

    runners.append(("json (stdlib)", stdlib_dump))
    return runners


def _get_parse_ndjson_runners(strict_missing: bool) -> list[tuple[str, Callable[[str], list]]]:
    """Return [(library_name, parse_func)] for NDJSON. parse_func(str) -> list of objects."""
    runners: list[tuple[str, Callable[[str], list]]] = []
    lines_getter = lambda t: [line for line in t.splitlines() if line.strip()]

    try:
        import strata

        runners.append(
            ("strata", lambda t: [strata.loads(line) for line in t.splitlines() if line.strip()])
        )
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
        """Benchmark JSONPath-style queries on users.json."""
        path = Path(dataset_path)
        data = path.read_bytes()
        dataset_name = path.stem
        parsed = json.loads(data)

        # Strata: parse once with loads(), query on Python dict (same as other libraries)
        strata_data = strata.loads(data)

        # Queries: strata expression, jmespath expression (or None), jsonpath-ng expression (or None)
        queries = [
            {
                "id": 1,
                "strata": "$.users[*].id",
                "jmespath": "users[*].id",
                "jsonpath_ng": "$.users[*].id",
            },
            {
                "id": 2,
                "strata": "$.users[*].orders[*].items[*].price",
                "jmespath": "users[*].orders[*].items[*].price",
                "jsonpath_ng": "$.users[*].orders[*].items[*].price",
            },
            {
                "id": 3,
                "strata": "$..price",
                "jmespath": None,
                "jsonpath_ng": "$..price",
            },
            {
                "id": 4,
                "strata": "$.users[?(@.age>30)]",
                "jmespath": None,
                "jsonpath_ng": "$.users[?(@.age > 30)]",
            },
            {
                "id": 5,
                "strata": '$..orders[?(@.status=="shipped")]',
                "jmespath": None,
                "jsonpath_ng": None,
            },
        ]

        print(f"\n=== Queries: {dataset_name} ===")

        for q in queries:
            query_desc = get_query_description(q["id"], is_ndjson=False)
            print(f"\n  Query {q['id']}: {query_desc}")

            # Strata (C++ search engine)
            try:
                compiled_path = strata.compile_path(q["strata"])

                def run_strata(cp=compiled_path):
                    return strata.query(strata_data, cp)

                tr = run_single_benchmark(
                    run_strata,
                    warmup=self.warmup,
                    repeat=self.repeat,
                    capture_rss=True,
                )
                result = strata.query(strata_data, compiled_path)
                n = len(result) if isinstance(result, list) else 1
                self.results.append(
                    BenchResult(
                        library="strata",
                        operation="query",
                        dataset=dataset_name,
                        query=query_desc,
                        times_ms=tr.times_ms,
                        result_count=n,
                        rss_mb=tr.rss_mb,
                    )
                )
                print(f"    strata:    {tr.min_ms:.3f}ms → {n} results")
            except Exception as e:
                print(f"    strata:    ERROR: {e}")

            # jmespath
            jmes_expr = q.get("jmespath")
            if jmes_expr:
                try:
                    import jmespath

                    expr = jmespath.compile(jmes_expr)
                    tr = run_single_benchmark(
                        lambda e=expr: e.search(parsed),
                        warmup=self.warmup,
                        repeat=self.repeat,
                        capture_rss=True,
                    )
                    res = expr.search(parsed)
                    n = len(res) if isinstance(res, (list, tuple)) else 1
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
            jp_expr = q.get("jsonpath_ng")
            if jp_expr:
                try:
                    from jsonpath_ng import parse as jp_parse

                    compiled = jp_parse(jp_expr)

                    def run_jp(c=compiled):
                        return [m.value for m in c.find(parsed)]

                    tr = run_single_benchmark(
                        run_jp,
                        warmup=self.warmup,
                        repeat=self.repeat,
                        capture_rss=True,
                    )
                    n = len(compiled.find(parsed))
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

    def bench_load_json(self, dataset_path: str) -> None:
        """Benchmark file-based JSON loading (strata.load vs open+parse)."""
        path = Path(dataset_path)
        size = path.stat().st_size
        dataset_name = path.name
        print(f"\n=== Load (file): {dataset_name} ({size} bytes) ===")

        runners = _get_load_json_runners(self.strict_missing)
        for library_name, load_func in runners:
            try:
                tr = run_single_benchmark(
                    lambda lf=load_func: lf(str(path)),
                    warmup=self.warmup,
                    repeat=self.repeat,
                    capture_rss=True,
                )
                self.results.append(
                    BenchResult(
                        library=library_name,
                        operation="load",
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
                        operation="load",
                        dataset=dataset_name,
                        error=str(e),
                    )
                )

    def bench_dump_json(self, dataset_path: str) -> None:
        """Benchmark file-based JSON dumping (strata.dump vs open+serialize)."""
        path = Path(dataset_path)
        dataset_name = path.name
        with open(path, encoding="utf-8") as f:
            data = json.load(f)
        print(f"\n=== Dump (file): {dataset_name} ===")

        runners = _get_dump_json_runners(self.strict_missing)
        for library_name, dump_func in runners:
            try:
                tmp = tempfile.NamedTemporaryFile(suffix=".json", delete=False)
                tmp_path = tmp.name
                tmp.close()

                tr = run_single_benchmark(
                    lambda df=dump_func, tp=tmp_path: df(data, tp),
                    warmup=self.warmup,
                    repeat=self.repeat,
                    capture_rss=True,
                )
                self.results.append(
                    BenchResult(
                        library=library_name,
                        operation="dump",
                        dataset=dataset_name,
                        times_ms=tr.times_ms,
                        rss_mb=tr.rss_mb,
                    )
                )
                print(
                    f"  {library_name:<15} {tr.min_ms:.3f}ms (min), {tr.median_ms:.3f}ms (median)"
                )
                os.unlink(tmp_path)
            except Exception as e:
                print(f"  {library_name:<15} ERROR: {e}")
                self.results.append(
                    BenchResult(
                        library=library_name,
                        operation="dump",
                        dataset=dataset_name,
                        error=str(e),
                    )
                )

    def bench_search_json(self, dataset_path: str) -> None:
        """Benchmark file-based search: strata.search vs end-to-end (open+parse+query) with competitors."""
        path = Path(dataset_path)
        dataset_name = path.name
        filepath = str(path)
        data_bytes = path.read_bytes()

        # Queries: (strata_expr, jmespath_expr_or_None, jsonpath_ng_expr_or_None, description)
        queries = [
            ("$.users[*].id", "users[*].id", "$.users[*].id", "all user ids"),
            (
                "$.users[*].orders[*].items[*].price",
                "users[*].orders[*].items[*].price",
                "$.users[*].orders[*].items[*].price",
                "all item prices",
            ),
            ("$..price", None, "$..price", "recursive price"),
        ]

        print(f"\n=== Search (file): {dataset_name} ===")

        for strata_expr, jmes_expr, jp_expr, desc in queries:
            compiled_path = strata.compile_path(strata_expr)

            print(f"\n  Search: {desc}")

            # strata search (mem_eff=False)
            try:
                tr = run_single_benchmark(
                    lambda cp=compiled_path: strata.search(filepath, cp),
                    warmup=self.warmup,
                    repeat=self.repeat,
                    capture_rss=True,
                )
                result = strata.search(filepath, compiled_path)
                n = len(result) if isinstance(result, list) else 1
                self.results.append(
                    BenchResult(
                        library="strata",
                        operation="search",
                        dataset=dataset_name,
                        query=desc,
                        times_ms=tr.times_ms,
                        result_count=n,
                        rss_mb=tr.rss_mb,
                    )
                )
                print(f"    strata (search):          {tr.min_ms:.3f}ms → {n} results")
            except Exception as e:
                print(f"    strata (search):          ERROR: {e}")

            # strata search (mem_eff=True)
            try:
                tr = run_single_benchmark(
                    lambda cp=compiled_path: strata.search(filepath, cp, mem_eff=True),
                    warmup=self.warmup,
                    repeat=self.repeat,
                    capture_rss=True,
                )
                result = strata.search(filepath, compiled_path, mem_eff=True)
                n = len(result) if isinstance(result, list) else 1
                self.results.append(
                    BenchResult(
                        library="strata (mem_eff)",
                        operation="search",
                        dataset=dataset_name,
                        query=desc,
                        times_ms=tr.times_ms,
                        result_count=n,
                        rss_mb=tr.rss_mb,
                    )
                )
                print(f"    strata (search mem_eff):  {tr.min_ms:.3f}ms → {n} results")
            except Exception as e:
                print(f"    strata (search mem_eff):  ERROR: {e}")

            # Competitor: orjson (open+parse) + jmespath (end-to-end)
            if jmes_expr:
                try:
                    import jmespath as _jmespath

                    try:
                        import orjson as _orjson

                        jmes_compiled = _jmespath.compile(jmes_expr)

                        def run_orjson_jmes(db=data_bytes, jc=jmes_compiled):
                            parsed = _orjson.loads(db)
                            return jc.search(parsed)

                        tr = run_single_benchmark(
                            run_orjson_jmes,
                            warmup=self.warmup,
                            repeat=self.repeat,
                            capture_rss=True,
                        )
                        res = run_orjson_jmes()
                        n = len(res) if isinstance(res, (list, tuple)) else 1
                        self.results.append(
                            BenchResult(
                                library="orjson+jmespath",
                                operation="search",
                                dataset=dataset_name,
                                query=desc,
                                times_ms=tr.times_ms,
                                result_count=n,
                                rss_mb=tr.rss_mb,
                            )
                        )
                        print(f"    orjson+jmespath:          {tr.min_ms:.3f}ms → {n} results")
                    except ImportError:
                        pass
                except ImportError:
                    pass

            # Competitor: orjson (open+parse) + jsonpath-ng (end-to-end)
            if jp_expr:
                try:
                    from jsonpath_ng import parse as jp_parse

                    try:
                        import orjson as _orjson

                        jp_compiled = jp_parse(jp_expr)

                        def run_orjson_jpng(db=data_bytes, jc=jp_compiled):
                            parsed = _orjson.loads(db)
                            return [m.value for m in jc.find(parsed)]

                        tr = run_single_benchmark(
                            run_orjson_jpng,
                            warmup=self.warmup,
                            repeat=self.repeat,
                            capture_rss=True,
                        )
                        res = run_orjson_jpng()
                        n = len(res) if isinstance(res, list) else 1
                        self.results.append(
                            BenchResult(
                                library="orjson+jsonpath-ng",
                                operation="search",
                                dataset=dataset_name,
                                query=desc,
                                times_ms=tr.times_ms,
                                result_count=n,
                                rss_mb=tr.rss_mb,
                            )
                        )
                        print(f"    orjson+jsonpath-ng:       {tr.min_ms:.3f}ms → {n} results")
                    except ImportError:
                        pass
                except ImportError:
                    pass
                except Exception as e:
                    print(f"    orjson+jsonpath-ng:       ERROR: {e}")

    def bench_dumps_json(self, dataset_path: str) -> None:
        """Benchmark JSON serialization (dumps)."""
        path = Path(dataset_path)
        dumps_results = run_dumps_benchmarks(path, repeat=self.repeat, warmup=self.warmup)
        for dr in dumps_results:
            self.results.append(
                BenchResult(
                    library=dr.library,
                    operation="dumps",
                    dataset=path.name,
                    times_ms=[dr.min_ms, dr.median_ms],
                    rss_mb=dr.rss_mb,
                    dumps_p95_ms=dr.p95_ms,
                )
            )

    def run_all(self) -> None:
        """Run all 7 benchmark sections for configured datasets."""
        for dataset in self.datasets:
            path = Path(dataset)
            if not path.exists():
                print(f"Warning: Dataset not found: {dataset}")
                continue
            if path.suffix == ".ndjson":
                self.bench_parse_ndjson(str(path))
            else:
                # 1. loads (parse in-memory)
                self.bench_parse_json(str(path))
                # 2. load (file-based)
                self.bench_load_json(str(path))
                # 3. dumps (serialize in-memory)
                self.bench_dumps_json(str(path))
                # 4. dump (file-based)
                self.bench_dump_json(str(path))
            if path.stem == "users" and path.suffix == ".json":
                # 5 & 6. search (file-based, with/without mem_eff)
                self.bench_search_json(str(path))
                # 7. query (in-memory dict)
                self.bench_query_json(str(path))

    def print_summary(self) -> None:
        """Print summary tables to stdout."""
        print("\n" + "=" * 80)
        print("BENCHMARK SUMMARY")
        print("=" * 80)

        # Helper for table sections
        def _print_table(title: str, op: str) -> None:
            op_results = [r for r in self.results if r.operation == op]
            if not op_results:
                return
            print(f"\n{title}:")
            print(
                f"{'Library':<20} {'Dataset':<15} {'Min (ms)':<12} {'Median (ms)':<12} {'P95 (ms)':<12}"
            )
            print("-" * 80)
            op_results.sort(key=lambda r: (r.dataset, r.median_ms))
            for r in op_results:
                if r.error:
                    print(f"{r.library:<20} {r.dataset:<15} ERROR: {r.error}")
                else:
                    print(
                        f"{r.library:<20} {r.dataset:<15} {r.min_ms:>10.3f}  {r.median_ms:>10.3f}  {r.p95_ms:>10.3f}"
                    )

        # 1. loads (in-memory parsing)
        _print_table("LOADS (in-memory parsing)", "parse")
        # 2. load (file-based)
        _print_table("LOAD (file-based)", "load")
        # 3. dumps (in-memory serialization)
        _print_table("DUMPS (in-memory serialization)", "dumps")
        # 4. dump (file-based)
        _print_table("DUMP (file-based)", "dump")

        # 5 & 6. search (file-based, mem_eff=True and mem_eff=False)
        search_results = [r for r in self.results if r.operation == "search"]
        if search_results:
            # Split into mem_eff and non-mem_eff
            search_std = [r for r in search_results if "mem_eff" not in r.library]
            search_mem = [r for r in search_results if "mem_eff" in r.library]

            if search_mem:
                print("\nSEARCH (mem_eff=True):")
                print(f"{'Library':<25} {'Query':<30} {'Min (ms)':<12} {'Results':<10}")
                print("-" * 80)
                by_query: dict[str, list[BenchResult]] = {}
                for r in search_mem:
                    by_query.setdefault(r.query, []).append(r)
                for query, results in by_query.items():
                    results.sort(key=lambda r: r.min_ms)
                    print(f"\n  {query}")
                    for r in results:
                        if r.error:
                            print(f"    {r.library:<23} ERROR: {r.error}")
                        else:
                            print(
                                f"    {r.library:<23} {r.min_ms:>10.3f}ms    {r.result_count:>6} results"
                            )

            if search_std:
                print("\nSEARCH (mem_eff=False):")
                print(f"{'Library':<25} {'Query':<30} {'Min (ms)':<12} {'Results':<10}")
                print("-" * 80)
                by_query = {}
                for r in search_std:
                    by_query.setdefault(r.query, []).append(r)
                for query, results in by_query.items():
                    results.sort(key=lambda r: r.min_ms)
                    print(f"\n  {query}")
                    for r in results:
                        if r.error:
                            print(f"    {r.library:<23} ERROR: {r.error}")
                        else:
                            print(
                                f"    {r.library:<23} {r.min_ms:>10.3f}ms    {r.result_count:>6} results"
                            )

        # 7. query (in-memory dict)
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

        # Helper for table sections in Markdown
        def _md_table(title: str, op: str) -> None:
            op_results = [r for r in self.results if r.operation == op]
            if not op_results:
                return
            lines.extend(
                [
                    "",
                    f"## {title}",
                    "",
                    "| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |",
                    "|---------|---------|----------|-------------|----------|---------|",
                ]
            )
            op_results.sort(key=lambda r: (r.dataset, r.median_ms))
            for r in op_results:
                if r.error:
                    lines.append(f"| {r.library} | {r.dataset} | ERROR | - | - | - |")
                else:
                    lines.append(
                        f"| {r.library} | {r.dataset} | {r.min_ms:.3f} | {r.median_ms:.3f} | {r.p95_ms:.3f} | {r.rss_mb:.1f} |"
                    )

        # 1. loads
        _md_table("loads (in-memory parsing)", "parse")
        # 2. load
        _md_table("load (file-based)", "load")
        # 3. dumps
        _md_table("dumps (in-memory serialization)", "dumps")
        # 4. dump
        _md_table("dump (file-based)", "dump")

        # 5. search (mem_eff=True)
        search_results = [r for r in self.results if r.operation == "search"]
        if search_results:
            search_mem = [r for r in search_results if "mem_eff" in r.library]
            search_std = [r for r in search_results if "mem_eff" not in r.library]

            if search_mem:
                lines.extend(
                    [
                        "",
                        "## search (mem_eff=True)",
                        "",
                        "| Query | Library | Min (ms) | Results | RSS (MB) |",
                        "|-------|---------|----------|----------|---------|",
                    ]
                )
                by_query: dict[str, list[BenchResult]] = {}
                for r in search_mem:
                    by_query.setdefault(r.query, []).append(r)
                for query, results in by_query.items():
                    results.sort(key=lambda r: r.min_ms)
                    for i, r in enumerate(results):
                        qcol = query if i == 0 else ""
                        if r.error:
                            lines.append(f"| {qcol} | {r.library} | ERROR | - | - |")
                        else:
                            lines.append(
                                f"| {qcol} | {r.library} | {r.min_ms:.3f} | {r.result_count} | {r.rss_mb:.1f} |"
                            )

            # 6. search (mem_eff=False)
            if search_std:
                lines.extend(
                    [
                        "",
                        "## search (mem_eff=False)",
                        "",
                        "| Query | Library | Min (ms) | Results | RSS (MB) |",
                        "|-------|---------|----------|----------|---------|",
                    ]
                )
                by_query = {}
                for r in search_std:
                    by_query.setdefault(r.query, []).append(r)
                for query, results in by_query.items():
                    results.sort(key=lambda r: r.min_ms)
                    for i, r in enumerate(results):
                        qcol = query if i == 0 else ""
                        if r.error:
                            lines.append(f"| {qcol} | {r.library} | ERROR | - | - |")
                        else:
                            lines.append(
                                f"| {qcol} | {r.library} | {r.min_ms:.3f} | {r.result_count} | {r.rss_mb:.1f} |"
                            )

        # 7. query
        query_results = [r for r in self.results if r.operation == "query"]
        if query_results:
            lines.extend(
                [
                    "",
                    "## Query Benchmarks",
                    "",
                    "| Query | Library | Min (ms) | Results | RSS (MB) |",
                    "|-------|---------|----------|----------|---------|",
                ]
            )
            by_query: dict[str, list[BenchResult]] = {}
            for r in query_results:
                by_query.setdefault(r.query, []).append(r)
            for query, results in by_query.items():
                results.sort(key=lambda r: r.min_ms)
                for i, r in enumerate(results):
                    qcol = query if i == 0 else ""
                    if r.error:
                        lines.append(f"| {qcol} | {r.library} | ERROR | - | - |")
                    else:
                        lines.append(
                            f"| {qcol} | {r.library} | {r.min_ms:.3f} | {r.result_count} | {r.rss_mb:.1f} |"
                        )

        # --- Summary: Strata ranking per category ---
        lines.extend(["", "## Summary", ""])
        lines.append("| Category | Strata Rank | vs #1 |")
        lines.append("|----------|-------------|-------|")

        categories = [
            ("Parsing (JSON)", "parse", ".json"),
            ("Parsing (NDJSON)", "parse", ".ndjson"),
            ("Load (file)", "load", ".json"),
            ("Serialization (dumps)", "dumps", ".json"),
            ("Dump (file)", "dump", ".json"),
        ]
        for label, op, suffix in categories:
            cat_results = [
                r
                for r in self.results
                if r.operation == op and not r.error and r.dataset.endswith(suffix)
            ]
            if not cat_results:
                continue
            cat_results.sort(key=lambda r: r.min_ms)
            strata_r = next(
                (r for r in cat_results if r.library.lower() == "strata"),
                None,
            )
            if not strata_r:
                continue
            rank = cat_results.index(strata_r) + 1
            first = cat_results[0]
            if strata_r is first:
                if len(cat_results) > 1:
                    second = cat_results[1]
                    pct = (second.min_ms / strata_r.min_ms - 1) * 100
                    gap = f"**{pct:.1f}% faster** than #2 ({second.library})"
                else:
                    gap = "-"
            else:
                pct = (strata_r.min_ms / first.min_ms - 1) * 100
                gap = f"{pct:.1f}% behind #1 ({first.library})"
            rank_str = f"**#{rank}** / {len(cat_results)}"
            lines.append(f"| {label} | {rank_str} | {gap} |")

        # Search summary (strata vs strata mem_eff)
        search_results = [r for r in self.results if r.operation == "search" and not r.error]
        if search_results:
            by_query: dict[str, list[BenchResult]] = {}
            for r in search_results:
                by_query.setdefault(r.query, []).append(r)
            mem_eff_wins = 0
            total = 0
            for query, results in by_query.items():
                results.sort(key=lambda r: r.min_ms)
                total += 1
                if "mem_eff" in results[0].library.lower():
                    mem_eff_wins += 1
            if total > 0:
                lines.append(
                    f"| Search (file) | mem_eff faster in {mem_eff_wins}/{total} queries | - |"
                )

        # JSONPath summary
        query_results = [r for r in self.results if r.operation == "query" and not r.error]
        if query_results:
            by_query: dict[str, list[BenchResult]] = {}
            for r in query_results:
                by_query.setdefault(r.query, []).append(r)
            wins = 0
            total = 0
            for query, results in by_query.items():
                results.sort(key=lambda r: r.min_ms)
                total += 1
                if results[0].library.lower() == "strata":
                    wins += 1
            if total > 0:
                lines.append(f"| JSONPath | **#1** in {wins}/{total} queries | - |")

        out.write_text("\n".join(lines) + "\n", encoding="utf-8")
        print(f"\nResults saved to {output_path}")


def main() -> None:
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

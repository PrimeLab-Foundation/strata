#!/usr/bin/env python3
"""
Unified Strata Benchmark Suite.

Comprehensive benchmarks for all Strata features against competing JSON libraries,
using fully randomized benchmark data that varies in structure, size, and complexity.

Usage:
    python -m benchmarks.bench_unified                    # Full suite with random data
    python -m benchmarks.bench_unified --feature loads    # Single feature
    python -m benchmarks.bench_unified --size large       # Specific size
    python -m benchmarks.bench_unified --seed 12345       # Reproducible
    python -m benchmarks.bench_unified --quick            # Quick mode
    python -m benchmarks.bench_unified --comprehensive    # Full comprehensive
"""

from __future__ import annotations

import argparse
import gc
import json
import statistics
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Callable

from .data_generator import DatasetMetadata, RandomBenchmarkDataGenerator
from .harness import get_rss_mb, run_timed

# Optional libraries for comparison
try:
    import orjson

    HAS_ORJSON = True
except ImportError:
    orjson = None  # type: ignore
    HAS_ORJSON = False

try:
    import msgspec

    HAS_MSGSPEC = True
except ImportError:
    msgspec = None  # type: ignore
    HAS_MSGSPEC = False

try:
    import ujson

    HAS_UJSON = True
except ImportError:
    ujson = None  # type: ignore
    HAS_UJSON = False

try:
    import simdjson

    HAS_SIMDJSON = True
except ImportError:
    simdjson = None  # type: ignore
    HAS_SIMDJSON = False

try:
    import jmespath

    HAS_JMESPATH = True
except ImportError:
    jmespath = None  # type: ignore
    HAS_JMESPATH = False

try:
    from jsonpath_ng import parse as jsonpath_parse

    HAS_JSONPATH_NG = True
except ImportError:
    jsonpath_parse = None  # type: ignore
    HAS_JSONPATH_NG = False

import strata


@dataclass
class FeatureReport:
    """Results for a single benchmark feature."""

    feature_name: str
    results: list[dict] = field(default_factory=list)
    errors: list[dict] = field(default_factory=list)


@dataclass
class BenchmarkReport:
    """Complete benchmark report."""

    features: dict[str, FeatureReport] = field(default_factory=dict)
    dataset_info: list[dict] = field(default_factory=list)
    timestamp: str = ""
    seed: int | str = "randomized"


def _p95(times_ms: list[float]) -> float:
    """Calculate 95th percentile."""
    if not times_ms:
        return 0.0
    sorted_times = sorted(times_ms)
    idx = min(int(len(sorted_times) * 0.95), len(sorted_times) - 1)
    return sorted_times[idx]


def _p99(times_ms: list[float]) -> float:
    """Calculate 99th percentile."""
    if not times_ms:
        return 0.0
    sorted_times = sorted(times_ms)
    idx = min(int(len(sorted_times) * 0.99), len(sorted_times) - 1)
    return sorted_times[idx]


def _std_dev(times_ms: list[float]) -> float:
    """Calculate standard deviation."""
    if len(times_ms) < 2:
        return 0.0
    return statistics.stdev(times_ms)


class UnifiedBenchmarkSuite:
    """Single interface for all Strata benchmarks."""

    # Default feature sets
    ALL_FEATURES = ["loads", "loads_tape", "dumps", "dumps_bytes", "ndjson", "iter_ndjson", "jsonpath", "cursor", "mmap"]
    QUICK_FEATURES = ["loads", "dumps", "ndjson"]
    CORE_FEATURES = ["loads", "dumps", "ndjson", "jsonpath"]

    # Default libraries to test
    ALL_LIBRARIES = ["strata", "orjson", "msgspec", "ujson", "json", "simdjson"]
    CORE_LIBRARIES = ["strata", "orjson", "msgspec", "json"]

    def __init__(
        self,
        *,
        repeat: int = 10,
        warmup: int = 3,
        sizes: list[str] | None = None,
        features: list[str] | None = None,
        libraries: list[str] | None = None,
        randomize: bool = True,
        seed: int | None = None,
        verbose: bool = True,
    ):
        """
        Initialize the benchmark suite.

        Args:
            repeat: Number of iterations per benchmark
            warmup: Number of warmup iterations
            sizes: Dataset sizes to test (default: ["small", "medium", "large"])
            features: Features to benchmark (default: all)
            libraries: Libraries to compare (default: all available)
            randomize: Whether to use random data (if False, uses fixed seed)
            seed: Fixed random seed for reproducibility
            verbose: Whether to print progress
        """
        self.repeat = repeat
        self.warmup = warmup
        self.sizes = sizes or ["small", "medium", "large"]
        self.features = features or self.CORE_FEATURES
        self.libraries = libraries or self._detect_available_libraries()
        self.verbose = verbose

        # Initialize data generator
        if seed is not None:
            self._seed = seed
        elif not randomize:
            self._seed = 42
        else:
            self._seed = None  # Will use time-based seed

        self._generator = RandomBenchmarkDataGenerator(seed=self._seed)
        self._generated_data: dict[str, tuple[Any, bytes, DatasetMetadata]] = {}
        self._report = BenchmarkReport(
            timestamp=time.strftime("%Y-%m-%d %H:%M:%S"),
            seed=self._generator.seed,
        )

    def _detect_available_libraries(self) -> list[str]:
        """Detect which comparison libraries are available."""
        available = ["strata", "json"]  # Always available
        if HAS_ORJSON:
            available.append("orjson")
        if HAS_MSGSPEC:
            available.append("msgspec")
        if HAS_UJSON:
            available.append("ujson")
        if HAS_SIMDJSON:
            available.append("simdjson")
        return available

    def _log(self, message: str, end: str = "\n") -> None:
        """Print message if verbose mode is enabled."""
        if self.verbose:
            print(message, end=end, flush=True)

    def _generate_datasets(self) -> None:
        """Generate random datasets for all sizes."""
        self._log("\n" + "=" * 70)
        self._log("Generating Benchmark Data")
        self._log("=" * 70)
        self._log(f"Seed: {self._generator.seed}")

        for size in self.sizes:
            self._log(f"\nGenerating {size} dataset...")

            # Generate JSON data
            data, metadata = self._generator.generate_dataset(
                target_size_bytes=size,
                complexity="mixed",
            )

            # Serialize to bytes for parsing benchmarks
            json_bytes = json.dumps(data, ensure_ascii=False).encode("utf-8")

            # Generate NDJSON if needed
            ndjson_lines = {"tiny": 100, "small": 1000, "medium": 5000, "large": 20000, "xlarge": 100000}.get(size, 1000)
            ndjson_bytes, ndjson_meta = self._generator.generate_ndjson_dataset(
                num_lines=ndjson_lines, line_complexity="mixed"
            )

            self._generated_data[size] = (data, json_bytes, metadata)
            self._generated_data[f"{size}_ndjson"] = (None, ndjson_bytes, ndjson_meta)

            # Track dataset info
            self._report.dataset_info.append(
                {
                    "name": size,
                    "size_bytes": len(json_bytes),
                    "depth": metadata.max_depth,
                    "keys": metadata.total_keys,
                    "values": metadata.total_values,
                    "complexity": metadata.complexity,
                }
            )
            self._log(f"  JSON: {len(json_bytes):,} bytes, depth={metadata.max_depth}")
            self._log(f"  NDJSON: {len(ndjson_bytes):,} bytes, {ndjson_lines} lines")

    def _run_benchmark(
        self,
        name: str,
        func: Callable[[], Any],
    ) -> dict:
        """Run a single benchmark and return results."""
        gc.collect()

        times_ms, result = run_timed(func, warmup=self.warmup, repeat=self.repeat)
        rss_mb = get_rss_mb()

        return {
            "times_ms": times_ms,
            "min_ms": min(times_ms),
            "median_ms": statistics.median(times_ms),
            "mean_ms": statistics.mean(times_ms),
            "p95_ms": _p95(times_ms),
            "p99_ms": _p99(times_ms),
            "std_dev_ms": _std_dev(times_ms),
            "rss_mb": rss_mb,
            "result": result,
        }

    def _bench_loads(self) -> FeatureReport:
        """Benchmark JSON parsing (loads)."""
        report = FeatureReport(feature_name="loads")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: loads (JSON parsing)")
        self._log("=" * 70)

        for size in self.sizes:
            data, json_bytes, metadata = self._generated_data[size]
            self._log(f"\n--- Dataset: {size} ({len(json_bytes):,} bytes) ---")

            runners: list[tuple[str, Callable[[], Any]]] = []

            # Strata
            if "strata" in self.libraries:
                runners.append(("strata", lambda b=json_bytes: strata.loads(b)))

            # orjson
            if "orjson" in self.libraries and HAS_ORJSON:
                runners.append(("orjson", lambda b=json_bytes: orjson.loads(b)))

            # msgspec
            if "msgspec" in self.libraries and HAS_MSGSPEC:
                runners.append(("msgspec", lambda b=json_bytes: msgspec.json.decode(b)))

            # ujson
            if "ujson" in self.libraries and HAS_UJSON:
                runners.append(("ujson", lambda b=json_bytes: ujson.loads(b)))

            # simdjson
            if "simdjson" in self.libraries and HAS_SIMDJSON:
                parser = simdjson.Parser()
                runners.append(("simdjson", lambda b=json_bytes, p=parser: p.parse(b)))

            # stdlib json
            if "json" in self.libraries:
                runners.append(("json", lambda b=json_bytes: json.loads(b)))

            for lib_name, run_func in runners:
                self._log(f"  {lib_name}: ", end="")
                try:
                    result = self._run_benchmark(lib_name, run_func)
                    result["library"] = lib_name
                    result["dataset"] = size
                    result["input_size_bytes"] = len(json_bytes)
                    del result["result"]  # Don't store parsed data
                    del result["times_ms"]  # Don't store all times
                    report.results.append(result)
                    self._log(f"median={result['median_ms']:.2f}ms, rss={result['rss_mb']:.1f}MB")
                except Exception as e:
                    self._log(f"ERROR: {e}")
                    report.errors.append({"library": lib_name, "dataset": size, "error": str(e)})

        return report

    def _bench_loads_tape(self) -> FeatureReport:
        """Benchmark tape-based parsing (loads_tape)."""
        report = FeatureReport(feature_name="loads_tape")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: loads_tape (tape-based parsing)")
        self._log("=" * 70)

        for size in self.sizes:
            data, json_bytes, metadata = self._generated_data[size]
            self._log(f"\n--- Dataset: {size} ({len(json_bytes):,} bytes) ---")

            # Only strata has tape parsing
            if "strata" in self.libraries:
                self._log("  strata (tape): ", end="")
                try:
                    result = self._run_benchmark("strata_tape", lambda b=json_bytes: strata.loads_tape(b))
                    result["library"] = "strata"
                    result["dataset"] = size
                    result["input_size_bytes"] = len(json_bytes)
                    del result["result"]
                    del result["times_ms"]
                    report.results.append(result)
                    self._log(f"median={result['median_ms']:.2f}ms, rss={result['rss_mb']:.1f}MB")
                except Exception as e:
                    self._log(f"ERROR: {e}")
                    report.errors.append({"library": "strata", "dataset": size, "error": str(e)})

        return report

    def _bench_dumps(self) -> FeatureReport:
        """Benchmark JSON serialization (dumps)."""
        report = FeatureReport(feature_name="dumps")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: dumps (JSON serialization)")
        self._log("=" * 70)

        for size in self.sizes:
            data, json_bytes, metadata = self._generated_data[size]
            self._log(f"\n--- Dataset: {size} ---")

            runners: list[tuple[str, Callable[[], Any]]] = []

            # Strata
            if "strata" in self.libraries:
                runners.append(("strata", lambda d=data: strata.dumps(d)))

            # orjson (returns bytes, decode for fair comparison)
            if "orjson" in self.libraries and HAS_ORJSON:
                runners.append(("orjson", lambda d=data: orjson.dumps(d).decode("utf-8")))

            # msgspec (returns bytes, decode for fair comparison)
            if "msgspec" in self.libraries and HAS_MSGSPEC:
                runners.append(("msgspec", lambda d=data: msgspec.json.encode(d).decode("utf-8")))

            # ujson
            if "ujson" in self.libraries and HAS_UJSON:
                runners.append(("ujson", lambda d=data: ujson.dumps(d)))

            # stdlib json
            if "json" in self.libraries:
                runners.append(("json", lambda d=data: json.dumps(d)))

            for lib_name, run_func in runners:
                self._log(f"  {lib_name}: ", end="")
                try:
                    result = self._run_benchmark(lib_name, run_func)
                    output = result["result"]
                    output_size = len(output.encode("utf-8")) if isinstance(output, str) else len(output)
                    result["library"] = lib_name
                    result["dataset"] = size
                    result["output_size"] = output_size
                    del result["result"]
                    del result["times_ms"]
                    report.results.append(result)
                    self._log(f"median={result['median_ms']:.2f}ms, size={output_size}, rss={result['rss_mb']:.1f}MB")
                except Exception as e:
                    self._log(f"ERROR: {e}")
                    report.errors.append({"library": lib_name, "dataset": size, "error": str(e)})

        return report

    def _bench_dumps_bytes(self) -> FeatureReport:
        """Benchmark JSON serialization to bytes (dumps_bytes)."""
        report = FeatureReport(feature_name="dumps_bytes")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: dumps_bytes (JSON to bytes)")
        self._log("=" * 70)

        for size in self.sizes:
            data, json_bytes, metadata = self._generated_data[size]
            self._log(f"\n--- Dataset: {size} ---")

            runners: list[tuple[str, Callable[[], Any]]] = []

            # Strata
            if "strata" in self.libraries:
                runners.append(("strata", lambda d=data: strata.dumps_bytes(d)))

            # orjson (native bytes)
            if "orjson" in self.libraries and HAS_ORJSON:
                runners.append(("orjson", lambda d=data: orjson.dumps(d)))

            # msgspec (native bytes)
            if "msgspec" in self.libraries and HAS_MSGSPEC:
                runners.append(("msgspec", lambda d=data: msgspec.json.encode(d)))

            # ujson + encode
            if "ujson" in self.libraries and HAS_UJSON:
                runners.append(("ujson", lambda d=data: ujson.dumps(d).encode("utf-8")))

            # stdlib json + encode
            if "json" in self.libraries:
                runners.append(("json", lambda d=data: json.dumps(d).encode("utf-8")))

            for lib_name, run_func in runners:
                self._log(f"  {lib_name}: ", end="")
                try:
                    result = self._run_benchmark(lib_name, run_func)
                    output = result["result"]
                    output_size = len(output)
                    result["library"] = lib_name
                    result["dataset"] = size
                    result["output_size"] = output_size
                    del result["result"]
                    del result["times_ms"]
                    report.results.append(result)
                    self._log(f"median={result['median_ms']:.2f}ms, size={output_size}, rss={result['rss_mb']:.1f}MB")
                except Exception as e:
                    self._log(f"ERROR: {e}")
                    report.errors.append({"library": lib_name, "dataset": size, "error": str(e)})

        return report

    def _bench_ndjson(self) -> FeatureReport:
        """Benchmark NDJSON batch parsing."""
        report = FeatureReport(feature_name="ndjson")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: parse_ndjson (NDJSON batch)")
        self._log("=" * 70)

        for size in self.sizes:
            _, ndjson_bytes, ndjson_meta = self._generated_data[f"{size}_ndjson"]
            ndjson_text = ndjson_bytes.decode("utf-8")
            lines = [line for line in ndjson_text.strip().split("\n") if line.strip()]
            self._log(f"\n--- Dataset: {size} ({len(ndjson_bytes):,} bytes, {len(lines)} lines) ---")

            runners: list[tuple[str, Callable[[], Any]]] = []

            # Strata parse_ndjson (auto mode)
            if "strata" in self.libraries:
                runners.append(("strata", lambda t=ndjson_text: strata.parse_ndjson(t)))

            # orjson line-by-line
            if "orjson" in self.libraries and HAS_ORJSON:
                runners.append(("orjson", lambda ls=lines: [orjson.loads(line) for line in ls]))

            # msgspec line-by-line
            if "msgspec" in self.libraries and HAS_MSGSPEC:
                runners.append(("msgspec", lambda ls=lines: [msgspec.json.decode(line) for line in ls]))

            # ujson line-by-line
            if "ujson" in self.libraries and HAS_UJSON:
                runners.append(("ujson", lambda ls=lines: [ujson.loads(line) for line in ls]))

            # stdlib json line-by-line
            if "json" in self.libraries:
                runners.append(("json", lambda ls=lines: [json.loads(line) for line in ls]))

            for lib_name, run_func in runners:
                self._log(f"  {lib_name}: ", end="")
                try:
                    result = self._run_benchmark(lib_name, run_func)
                    parsed = result["result"]
                    lines_parsed = len(parsed) if isinstance(parsed, list) else 0
                    result["library"] = lib_name
                    result["dataset"] = size
                    result["input_size_bytes"] = len(ndjson_bytes)
                    result["lines_parsed"] = lines_parsed
                    del result["result"]
                    del result["times_ms"]
                    report.results.append(result)
                    self._log(f"median={result['median_ms']:.2f}ms, lines={lines_parsed}, rss={result['rss_mb']:.1f}MB")
                except Exception as e:
                    self._log(f"ERROR: {e}")
                    report.errors.append({"library": lib_name, "dataset": size, "error": str(e)})

        return report

    def _bench_iter_ndjson(self) -> FeatureReport:
        """Benchmark NDJSON streaming iteration."""
        report = FeatureReport(feature_name="iter_ndjson")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: iter_ndjson (NDJSON streaming)")
        self._log("=" * 70)

        for size in self.sizes:
            _, ndjson_bytes, ndjson_meta = self._generated_data[f"{size}_ndjson"]
            ndjson_text = ndjson_bytes.decode("utf-8")
            lines = [line for line in ndjson_text.strip().split("\n") if line.strip()]
            self._log(f"\n--- Dataset: {size} ({len(ndjson_bytes):,} bytes, {len(lines)} lines) ---")

            # Strata iter_ndjson
            if "strata" in self.libraries:
                self._log("  strata (iter): ", end="")
                try:
                    def run_iter(t=ndjson_text):
                        return list(strata.iter_ndjson(t))

                    result = self._run_benchmark("strata_iter", run_iter)
                    parsed = result["result"]
                    lines_parsed = len(parsed) if isinstance(parsed, list) else 0
                    result["library"] = "strata"
                    result["dataset"] = size
                    result["input_size_bytes"] = len(ndjson_bytes)
                    result["lines_parsed"] = lines_parsed
                    del result["result"]
                    del result["times_ms"]
                    report.results.append(result)
                    self._log(f"median={result['median_ms']:.2f}ms, lines={lines_parsed}, rss={result['rss_mb']:.1f}MB")
                except Exception as e:
                    self._log(f"ERROR: {e}")
                    report.errors.append({"library": "strata", "dataset": size, "error": str(e)})

        return report

    def _bench_jsonpath(self) -> FeatureReport:
        """Benchmark JSONPath queries."""
        report = FeatureReport(feature_name="jsonpath")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: JSONPath queries")
        self._log("=" * 70)

        # Test queries
        queries = [
            ("$.users[*].id", "users[*].id", "Extract all user IDs"),
            ("$.users[*].name", "users[*].name", "Extract all user names"),
            ("$..price", None, "Recursive price search"),
            ("$.users[?(@.age > 30)].name", "users[?age > `30`].name", "Filter by age"),
        ]

        for size in self.sizes:
            data, json_bytes, metadata = self._generated_data[size]
            json_text = json_bytes.decode("utf-8")
            self._log(f"\n--- Dataset: {size} ---")

            for strata_query, jmespath_query, description in queries:
                self._log(f"\n  Query: {description}")

                # Strata (using string input)
                if "strata" in self.libraries:
                    self._log("    strata: ", end="")
                    try:
                        path = strata.compile_path(strata_query)

                        def run_strata(t=json_text, p=path):
                            return strata.search(t, p)

                        result = self._run_benchmark("strata", run_strata)
                        res_list = result["result"]
                        result_count = len(res_list) if isinstance(res_list, list) else 1
                        result["library"] = "strata"
                        result["dataset"] = size
                        result["query"] = strata_query
                        result["result_count"] = result_count
                        del result["result"]
                        del result["times_ms"]
                        report.results.append(result)
                        self._log(f"median={result['median_ms']:.2f}ms, results={result_count}")
                    except Exception as e:
                        self._log(f"ERROR: {e}")
                        report.errors.append({"library": "strata", "dataset": size, "query": strata_query, "error": str(e)})

                # jmespath (if available and query is supported)
                # NOTE: Include JSON parsing to match strata's parse+search model
                if HAS_JMESPATH and jmespath_query:
                    self._log("    jmespath: ", end="")
                    try:
                        compiled = jmespath.compile(jmespath_query)

                        def run_jmespath(t=json_text, c=compiled):
                            return c.search(json.loads(t))

                        result = self._run_benchmark("jmespath", run_jmespath)
                        res = result["result"]
                        result_count = len(res) if isinstance(res, (list, tuple)) else 1
                        result["library"] = "jmespath"
                        result["dataset"] = size
                        result["query"] = strata_query
                        result["result_count"] = result_count
                        del result["result"]
                        del result["times_ms"]
                        report.results.append(result)
                        self._log(f"median={result['median_ms']:.2f}ms, results={result_count}")
                    except Exception as e:
                        self._log(f"ERROR: {e}")
                        report.errors.append({"library": "jmespath", "dataset": size, "query": strata_query, "error": str(e)})

                # jsonpath-ng (if available)
                # NOTE: Include JSON parsing to match strata's parse+search model
                if HAS_JSONPATH_NG:
                    self._log("    jsonpath-ng: ", end="")
                    try:
                        compiled = jsonpath_parse(strata_query)

                        def run_jpng(t=json_text, c=compiled):
                            return [m.value for m in c.find(json.loads(t))]

                        result = self._run_benchmark("jsonpath-ng", run_jpng)
                        res = result["result"]
                        result_count = len(res) if isinstance(res, list) else 1
                        result["library"] = "jsonpath-ng"
                        result["dataset"] = size
                        result["query"] = strata_query
                        result["result_count"] = result_count
                        del result["result"]
                        del result["times_ms"]
                        report.results.append(result)
                        self._log(f"median={result['median_ms']:.2f}ms, results={result_count}")
                    except Exception as e:
                        self._log(f"ERROR: {e}")
                        report.errors.append({"library": "jsonpath-ng", "dataset": size, "query": strata_query, "error": str(e)})

        return report

    def _bench_cursor(self) -> FeatureReport:
        """Benchmark cursor API (parse_json)."""
        report = FeatureReport(feature_name="cursor")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: Cursor API")
        self._log("=" * 70)

        for size in self.sizes:
            data, json_bytes, metadata = self._generated_data[size]
            json_text = json_bytes.decode("utf-8")
            self._log(f"\n--- Dataset: {size} ---")

            if "strata" in self.libraries:
                self._log("  strata (cursor): ", end="")
                try:
                    result = self._run_benchmark("strata_cursor", lambda t=json_text: strata.parse_json(t))
                    result["library"] = "strata"
                    result["dataset"] = size
                    result["input_size_bytes"] = len(json_bytes)
                    del result["result"]
                    del result["times_ms"]
                    report.results.append(result)
                    self._log(f"median={result['median_ms']:.2f}ms, rss={result['rss_mb']:.1f}MB")
                except Exception as e:
                    self._log(f"ERROR: {e}")
                    report.errors.append({"library": "strata", "dataset": size, "error": str(e)})

        return report

    def _bench_mmap(self) -> FeatureReport:
        """Benchmark memory-mapped file parsing."""
        report = FeatureReport(feature_name="mmap")

        self._log("\n" + "=" * 70)
        self._log("Benchmarking: Memory-mapped I/O")
        self._log("=" * 70)

        # Create temp files for mmap testing
        import tempfile

        for size in self.sizes:
            data, json_bytes, metadata = self._generated_data[size]

            # Write to temp file
            with tempfile.NamedTemporaryFile(mode="wb", suffix=".json", delete=False) as f:
                f.write(json_bytes)
                temp_path = f.name

            self._log(f"\n--- Dataset: {size} ({len(json_bytes):,} bytes) ---")

            if "strata" in self.libraries:
                self._log("  strata (mmap): ", end="")
                try:
                    result = self._run_benchmark("strata_mmap", lambda p=temp_path: strata.parse_json_file(p))
                    result["library"] = "strata"
                    result["dataset"] = size
                    result["input_size_bytes"] = len(json_bytes)
                    del result["result"]
                    del result["times_ms"]
                    report.results.append(result)
                    self._log(f"median={result['median_ms']:.2f}ms, rss={result['rss_mb']:.1f}MB")
                except Exception as e:
                    self._log(f"ERROR: {e}")
                    report.errors.append({"library": "strata", "dataset": size, "error": str(e)})

            # Clean up temp file
            Path(temp_path).unlink()

        return report

    def run_all(self) -> BenchmarkReport:
        """Run complete benchmark suite."""
        self._log("\n" + "=" * 70)
        self._log("STRATA UNIFIED BENCHMARK SUITE")
        self._log("=" * 70)
        self._log(f"Features: {', '.join(self.features)}")
        self._log(f"Sizes: {', '.join(self.sizes)}")
        self._log(f"Libraries: {', '.join(self.libraries)}")
        self._log(f"Repeat: {self.repeat}, Warmup: {self.warmup}")

        # Generate datasets
        self._generate_datasets()

        # Run benchmarks for each feature
        feature_methods = {
            "loads": self._bench_loads,
            "loads_tape": self._bench_loads_tape,
            "dumps": self._bench_dumps,
            "dumps_bytes": self._bench_dumps_bytes,
            "ndjson": self._bench_ndjson,
            "iter_ndjson": self._bench_iter_ndjson,
            "jsonpath": self._bench_jsonpath,
            "cursor": self._bench_cursor,
            "mmap": self._bench_mmap,
        }

        for feature in self.features:
            if feature in feature_methods:
                report = feature_methods[feature]()
                self._report.features[feature] = report

        return self._report

    def run_feature(self, feature: str) -> FeatureReport:
        """Run benchmarks for a single feature."""
        # Generate datasets if not already done
        if not self._generated_data:
            self._generate_datasets()

        feature_methods = {
            "loads": self._bench_loads,
            "loads_tape": self._bench_loads_tape,
            "dumps": self._bench_dumps,
            "dumps_bytes": self._bench_dumps_bytes,
            "ndjson": self._bench_ndjson,
            "iter_ndjson": self._bench_iter_ndjson,
            "jsonpath": self._bench_jsonpath,
            "cursor": self._bench_cursor,
            "mmap": self._bench_mmap,
        }

        if feature not in feature_methods:
            raise ValueError(f"Unknown feature: {feature}. Available: {list(feature_methods.keys())}")

        report = feature_methods[feature]()
        self._report.features[feature] = report
        return report

    def generate_report(self, format: str = "markdown") -> str:
        """
        Generate formatted benchmark report.

        Args:
            format: Output format ("markdown", "json", "csv")
        """
        if format == "markdown":
            from .reporters import MarkdownReporter

            reporter = MarkdownReporter()
            reporter.env_info.seed = self._generator.seed
            return reporter.generate_report(self._report)
        elif format == "json":
            from .reporters import JSONReporter

            reporter = JSONReporter(seed=self._generator.seed)
            return reporter.generate_report(self._report)
        elif format == "csv":
            from .reporters import CSVReporter

            reporter = CSVReporter(seed=self._generator.seed)
            return reporter.generate_report(self._report)
        else:
            raise ValueError(f"Unknown format: {format}. Available: markdown, json, csv")

    def print_summary(self) -> None:
        """Print a summary of benchmark results."""
        self._log("\n" + "=" * 70)
        self._log("BENCHMARK SUMMARY")
        self._log("=" * 70)

        total_benchmarks = 0
        strata_wins = 0

        for feature_name, feature_report in self._report.features.items():
            # Group by dataset
            by_dataset: dict[str, list[dict]] = {}
            for result in feature_report.results:
                ds = result.get("dataset", "default")
                by_dataset.setdefault(ds, []).append(result)

            for ds, ds_results in by_dataset.items():
                if not ds_results:
                    continue

                total_benchmarks += 1
                ds_results.sort(key=lambda r: r.get("median_ms", float("inf")))
                winner = ds_results[0]

                if winner.get("library") == "strata":
                    strata_wins += 1

                # Find strata result
                strata_result = next((r for r in ds_results if r.get("library") == "strata"), None)
                if strata_result:
                    strata_rank = ds_results.index(strata_result) + 1
                    strata_ms = strata_result.get("median_ms", 0)

                    # Find best non-strata
                    others = [r for r in ds_results if r.get("library") != "strata"]
                    if others:
                        best_other = others[0]
                        best_lib = best_other.get("library", "?")
                        best_ms = best_other.get("median_ms", 0)

                        if strata_ms > 0 and best_ms > 0:
                            if strata_ms < best_ms:
                                speedup = (best_ms / strata_ms - 1) * 100
                                self._log(f"  {feature_name}/{ds}: Strata #{strata_rank}, +{speedup:.1f}% vs {best_lib}")
                            else:
                                slowdown = (strata_ms / best_ms - 1) * 100
                                self._log(f"  {feature_name}/{ds}: Strata #{strata_rank}, -{slowdown:.1f}% vs {best_lib}")

        self._log("")
        self._log(f"Strata wins: {strata_wins}/{total_benchmarks} benchmarks")


def main() -> int:
    """Main entry point for CLI."""
    parser = argparse.ArgumentParser(
        description="Unified Strata Benchmark Suite",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    python -m benchmarks.bench_unified                     # Full suite with random data
    python -m benchmarks.bench_unified --feature loads     # Single feature
    python -m benchmarks.bench_unified --size large        # Large dataset only
    python -m benchmarks.bench_unified --seed 12345        # Reproducible run
    python -m benchmarks.bench_unified --quick             # Quick mode
    python -m benchmarks.bench_unified --libs strata,orjson  # Specific libraries
    python -m benchmarks.bench_unified --format json -o results.json
        """,
    )

    parser.add_argument(
        "--feature",
        "-f",
        type=str,
        help="Single feature to benchmark (loads, dumps, ndjson, jsonpath, etc.)",
    )
    parser.add_argument(
        "--size",
        "-s",
        type=str,
        help="Single size to test (tiny, small, medium, large, xlarge)",
    )
    parser.add_argument(
        "--sizes",
        type=str,
        help="Comma-separated list of sizes to test",
    )
    parser.add_argument(
        "--features",
        type=str,
        help="Comma-separated list of features to benchmark",
    )
    parser.add_argument(
        "--libs",
        type=str,
        help="Comma-separated list of libraries to compare",
    )
    parser.add_argument(
        "--seed",
        type=int,
        help="Random seed for reproducible runs",
    )
    parser.add_argument(
        "--repeat",
        type=int,
        default=10,
        help="Iterations per benchmark (default: 10)",
    )
    parser.add_argument(
        "--warmup",
        type=int,
        default=3,
        help="Warmup iterations (default: 3)",
    )
    parser.add_argument(
        "--format",
        choices=["markdown", "json", "csv"],
        default="markdown",
        help="Output format (default: markdown)",
    )
    parser.add_argument(
        "--output",
        "-o",
        type=str,
        help="Output file path",
    )
    parser.add_argument(
        "--quick",
        action="store_true",
        help="Quick mode (fewer iterations, small dataset)",
    )
    parser.add_argument(
        "--comprehensive",
        action="store_true",
        help="Comprehensive mode (all features, all sizes, more iterations)",
    )
    parser.add_argument(
        "--append-progress-log",
        action="store_true",
        help="Append results to docs/benchmarks/progress_log.md",
    )
    parser.add_argument(
        "--quiet",
        "-q",
        action="store_true",
        help="Suppress progress output",
    )

    args = parser.parse_args()

    # Determine configuration
    if args.quick:
        repeat = 5
        warmup = 1
        sizes = ["small"]
        features = UnifiedBenchmarkSuite.QUICK_FEATURES
    elif args.comprehensive:
        repeat = 20
        warmup = 5
        sizes = ["small", "medium", "large"]
        features = UnifiedBenchmarkSuite.ALL_FEATURES
    else:
        repeat = args.repeat
        warmup = args.warmup
        sizes = None
        features = None

    # Override with specific args
    if args.size:
        sizes = [args.size]
    elif args.sizes:
        sizes = [s.strip() for s in args.sizes.split(",")]

    if args.feature:
        features = [args.feature]
    elif args.features:
        features = [f.strip() for f in args.features.split(",")]

    libraries = None
    if args.libs:
        libraries = [lib.strip() for lib in args.libs.split(",")]

    # Create and run suite
    suite = UnifiedBenchmarkSuite(
        repeat=repeat,
        warmup=warmup,
        sizes=sizes,
        features=features,
        libraries=libraries,
        seed=args.seed,
        verbose=not args.quiet,
    )

    report = suite.run_all()
    suite.print_summary()

    # Generate output
    output = suite.generate_report(args.format)

    if args.output:
        Path(args.output).write_text(output, encoding="utf-8")
        print(f"\nReport saved to: {args.output}")
    else:
        print("\n" + output)

    # Append to progress log if requested
    if args.append_progress_log:
        from .reporters import MarkdownReporter

        reporter = MarkdownReporter()
        reporter.env_info.seed = suite._generator.seed
        entry = reporter.generate_progress_log_entry(report)

        progress_log = Path("docs/benchmarks/progress_log.md")
        if progress_log.exists():
            with open(progress_log, "a", encoding="utf-8") as f:
                f.write("\n" + entry)
            print(f"Appended to: {progress_log}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

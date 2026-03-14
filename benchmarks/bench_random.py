#!/usr/bin/env python3
"""
Random-schema JSON benchmarks.

Generates JSON with different schema shapes (flat, nested, wide_arrays, mixed)
and benchmarks loads/dumps/query against other libraries.
"""

from __future__ import annotations

import argparse
import json
import random
import string
import statistics
import tempfile
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from .harness import get_rss_mb, run_single_benchmark

import strata

SCHEMAS = {
    "flat": "Flat objects with many keys",
    "nested": "Deeply nested objects",
    "wide_arrays": "Objects with large arrays",
    "mixed": "Mix of all shapes",
}


def _random_string(length: int = 10) -> str:
    return "".join(random.choices(string.ascii_lowercase, k=length))


def _generate_flat(num_records: int, num_keys: int = 20) -> list[dict]:
    """Generate flat objects with many scalar keys."""
    records = []
    for i in range(num_records):
        obj = {"id": i}
        for k in range(num_keys):
            if k % 3 == 0:
                obj[f"field_{k}"] = random.randint(0, 100000)
            elif k % 3 == 1:
                obj[f"field_{k}"] = random.random() * 1000
            else:
                obj[f"field_{k}"] = _random_string(random.randint(5, 50))
        records.append(obj)
    return records


def _generate_nested(num_records: int, depth: int = 6) -> list[dict]:
    """Generate deeply nested objects."""
    records = []
    for i in range(num_records):
        obj: dict = {"id": i}
        current = obj
        for d in range(depth):
            child: dict[str, Any] = {
                "level": d,
                "value": _random_string(10),
                "count": random.randint(0, 1000),
            }
            current["child"] = child
            current = child
        current["leaf"] = True
        records.append(obj)
    return records


def _generate_wide_arrays(num_records: int, array_size: int = 100) -> list[dict]:
    """Generate objects with large arrays."""
    records = []
    for i in range(num_records):
        obj = {
            "id": i,
            "numbers": [random.random() for _ in range(array_size)],
            "strings": [_random_string(8) for _ in range(array_size // 2)],
            "tags": [_random_string(5) for _ in range(random.randint(1, 10))],
        }
        records.append(obj)
    return records


def _generate_mixed(num_records: int) -> list[dict]:
    """Generate a mix of shapes."""
    records = []
    generators = [
        lambda: _generate_flat(1, num_keys=10)[0],
        lambda: _generate_nested(1, depth=4)[0],
        lambda: _generate_wide_arrays(1, array_size=30)[0],
    ]
    for i in range(num_records):
        gen = random.choice(generators)
        obj = gen()
        obj["id"] = i
        obj["type"] = gen.__name__ if hasattr(gen, "__name__") else "mixed"
        records.append(obj)
    return records


GENERATORS = {
    "flat": _generate_flat,
    "nested": _generate_nested,
    "wide_arrays": _generate_wide_arrays,
    "mixed": _generate_mixed,
}


@dataclass
class RandomBenchResult:
    library: str
    operation: str
    schema: str
    times_ms: list[float] = field(default_factory=list)
    rss_mb: float = 0.0
    error: str = ""

    @property
    def min_ms(self) -> float:
        return min(self.times_ms) if self.times_ms else 0.0

    @property
    def median_ms(self) -> float:
        return statistics.median(self.times_ms) if self.times_ms else 0.0


def _get_libs() -> dict[str, dict]:
    """Detect available libraries."""
    libs: dict[str, dict] = {"strata": {"loads": strata.loads, "dumps": strata.dumps}}

    try:
        import orjson

        libs["orjson"] = {"loads": orjson.loads, "dumps": lambda d: orjson.dumps(d).decode()}
    except ImportError:
        pass

    try:
        import ujson

        libs["ujson"] = {"loads": ujson.loads, "dumps": ujson.dumps}
    except ImportError:
        pass

    try:
        import msgspec

        libs["msgspec"] = {
            "loads": msgspec.json.decode,
            "dumps": lambda d: msgspec.json.encode(d).decode(),
        }
    except ImportError:
        pass

    libs["json (stdlib)"] = {"loads": json.loads, "dumps": json.dumps}
    return libs


def run_schema_benchmarks(
    schema: str,
    num_records: int = 500,
    repeat: int = 3,
    warmup: int = 1,
) -> list[RandomBenchResult]:
    """Run loads/dumps benchmarks for a given schema."""
    gen = GENERATORS[schema]
    data = {"records": gen(num_records)}
    json_text = json.dumps(data)
    json_bytes = json_text.encode()

    print(f"\n--- Schema: {schema} ({SCHEMAS[schema]}) ---")
    print(f"    Records: {num_records}, JSON size: {len(json_bytes) / 1024:.1f} KB")

    libs = _get_libs()
    results: list[RandomBenchResult] = []

    # Loads benchmark
    print("\n  loads:")
    for name, funcs in libs.items():
        try:
            loads_fn = funcs["loads"]
            tr = run_single_benchmark(
                lambda fn=loads_fn: fn(json_bytes),
                warmup=warmup,
                repeat=repeat,
                capture_rss=True,
            )
            results.append(
                RandomBenchResult(
                    library=name,
                    operation="loads",
                    schema=schema,
                    times_ms=tr.times_ms,
                    rss_mb=tr.rss_mb,
                )
            )
            print(f"    {name:<15} min={tr.min_ms:.3f}ms median={tr.median_ms:.3f}ms")
        except Exception as e:
            print(f"    {name:<15} ERROR: {e}")
            results.append(
                RandomBenchResult(library=name, operation="loads", schema=schema, error=str(e))
            )

    # Dumps benchmark
    parsed = json.loads(json_text)
    print("\n  dumps:")
    for name, funcs in libs.items():
        try:
            dumps_fn = funcs["dumps"]
            tr = run_single_benchmark(
                lambda fn=dumps_fn: fn(parsed),
                warmup=warmup,
                repeat=repeat,
                capture_rss=True,
            )
            results.append(
                RandomBenchResult(
                    library=name,
                    operation="dumps",
                    schema=schema,
                    times_ms=tr.times_ms,
                    rss_mb=tr.rss_mb,
                )
            )
            print(f"    {name:<15} min={tr.min_ms:.3f}ms median={tr.median_ms:.3f}ms")
        except Exception as e:
            print(f"    {name:<15} ERROR: {e}")
            results.append(
                RandomBenchResult(library=name, operation="dumps", schema=schema, error=str(e))
            )

    # Query benchmark (strata only)
    print("\n  query (strata):")
    strata_data = strata.loads(json_bytes)
    queries = [
        ("$.records[*].id", "all IDs"),
        ("$.records[0]", "first record"),
    ]
    for expr, desc in queries:
        try:
            path = strata.compile_path(expr)
            tr = run_single_benchmark(
                lambda p=path: strata.query(strata_data, p),
                warmup=warmup,
                repeat=repeat,
                capture_rss=True,
            )
            n = len(strata.query(strata_data, path))
            results.append(
                RandomBenchResult(
                    library="strata",
                    operation=f"query:{desc}",
                    schema=schema,
                    times_ms=tr.times_ms,
                    rss_mb=tr.rss_mb,
                )
            )
            print(f"    {desc:<30} min={tr.min_ms:.3f}ms results={n}")
        except Exception as e:
            print(f"    {desc:<30} ERROR: {e}")

    return results


def main() -> int:
    parser = argparse.ArgumentParser(description="Random-schema JSON benchmarks")
    parser.add_argument(
        "--schema",
        choices=list(SCHEMAS.keys()) + ["all"],
        default="all",
        help="Schema to benchmark",
    )
    parser.add_argument("--records", type=int, default=500, help="Records per schema")
    parser.add_argument("--repeat", type=int, default=3, help="Iterations")
    parser.add_argument("--warmup", type=int, default=1, help="Warmup iterations")
    parser.add_argument("--seed", type=int, default=42, help="Random seed")
    args = parser.parse_args()

    random.seed(args.seed)

    schemas = list(SCHEMAS.keys()) if args.schema == "all" else [args.schema]

    print("=" * 70)
    print("Random Schema Benchmarks")
    print("=" * 70)

    all_results: list[RandomBenchResult] = []
    for schema in schemas:
        results = run_schema_benchmarks(
            schema, num_records=args.records, repeat=args.repeat, warmup=args.warmup
        )
        all_results.extend(results)

    # Summary
    print("\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    for op in ["loads", "dumps"]:
        op_results = [r for r in all_results if r.operation == op and not r.error]
        if not op_results:
            continue
        print(f"\n{op}:")
        print(f"  {'Library':<15} ", end="")
        for s in schemas:
            print(f"{s:>15}", end="")
        print()
        print("  " + "-" * (15 + 15 * len(schemas)))

        libs_seen = []
        for r in op_results:
            if r.library not in libs_seen:
                libs_seen.append(r.library)

        for lib in libs_seen:
            print(f"  {lib:<15} ", end="")
            for s in schemas:
                matching = [r for r in op_results if r.library == lib and r.schema == s]
                if matching:
                    print(f"{matching[0].min_ms:>12.3f}ms", end="")
                else:
                    print(f"{'N/A':>15}", end="")
            print()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

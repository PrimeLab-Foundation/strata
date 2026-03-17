#!/usr/bin/env python3
"""
PGO training workload.

Exercises all hot paths with large, diverse training data to produce
a high-quality PGO profile. Designed to be run during Phase 1 of
the PGO build (instrumented binary).

Hot paths targeted:
  - loads()       → SAX parser + PythonObjectBuilder + KeyCache
  - dumps()       → serialization, SIMD escape, number formatting
  - load()        → file I/O + parsing for both JSON and NDJSON
  - search()      → SAX search handler with diverse JSONPath patterns
  - query()       → in-memory JSONPath with filters, slices, recursion
"""

from __future__ import annotations

import gc
import sys
import time
from pathlib import Path

import strata


def _timed(label: str, func, repeat: int) -> None:
    """Run func repeat times, print elapsed."""
    gc.collect()
    t0 = time.perf_counter()
    for _ in range(repeat):
        func()
    elapsed = time.perf_counter() - t0
    print(f"  {label}: {repeat}x in {elapsed:.2f}s ({elapsed / repeat * 1000:.1f}ms/iter)")
    sys.stdout.flush()


def run(train_json: Path, train_ndjson: Path) -> None:
    """Execute the full training workload.

    Uses a small slice of the data for each step — PGO only needs branch
    coverage, not volume. The instrumented binary is ~100x slower so we
    keep the working set tiny.
    """
    json_text = train_json.read_text(encoding="utf-8")
    json_bytes = json_text.encode("utf-8")
    json_path = str(train_json)
    ndjson_path = str(train_ndjson)

    print("\nPGO Training Workload")
    print(f"  JSON:   {train_json} ({len(json_bytes) / 1024 / 1024:.1f}MB)")
    print(f"  NDJSON: {train_ndjson} ({train_ndjson.stat().st_size / 1024 / 1024:.1f}MB)")
    print()
    sys.stdout.flush()

    # 1. loads — parse JSON text (str and bytes)
    print("[1/6] loads (JSON parsing)", flush=True)
    _timed("loads(str)", lambda: strata.loads(json_text), repeat=1)
    _timed("loads(bytes)", lambda: strata.loads(json_bytes), repeat=1)

    # 2. dumps — serialize
    print("[2/6] dumps (serialization)", flush=True)
    data = strata.loads(json_text)
    _timed("dumps(str)", lambda: strata.dumps(data), repeat=1)
    _timed("dumps(bytes)", lambda: strata.dumps(data, return_type="bytes"), repeat=1)

    # 3. load — file I/O (tiny file)
    print("[3/6] load (file I/O)", flush=True)
    _timed("load(json)", lambda: strata.load(json_path), repeat=1)
    _timed("load(ndjson)", lambda: strata.load(ndjson_path), repeat=1)

    # 4. search — SAX search handler (tiny file, diverse queries)
    print("[4/6] search (SAX file-based)", flush=True)
    search_queries = [
        ("$.records[*].id", "Field + Wildcard"),
        ("$.records[*].nested.deep.val", "Multi-level Field"),
        ("$..price", "RecursiveDescent"),
        ("$.records[0].tags", "Index access"),
        ("$.records[*].tags[*]", "Double Wildcard"),
        ("$.records[*].metadata.scores[*].value", "Deep Wildcard chain"),
    ]
    for expr, desc in search_queries:
        path = strata.compile_path(expr)
        _timed(f"search({desc})", lambda p=path: strata.search(json_path, p), repeat=1)

    # 5. query — in-memory JSONPath (use small data)
    print("[5/6] query (in-memory JSONPath)", flush=True)
    query_queries = [
        ("$.records[*].name", "All names"),
        ("$.records[?(@.age > 50)]", "Filter age > 50"),
        ("$.records[0:10].id", "Slice first 10"),
        ("$..tags", "RecursiveDescent on arrays"),
        ("$.records[*].metadata.scores[*]", "Nested wildcard"),
    ]
    for expr, desc in query_queries:
        path = strata.compile_path(expr)
        _timed(f"query({desc})", lambda p=path: strata.query(data, p), repeat=1)

    # 6. Benchmark-specific training: exercise the exact patterns from bench_main.py
    # This trains PGO branch predictors for flat/nested/mixed/wide_arrays schemas.
    print("[6/8] benchmark schema training", flush=True)
    bench_data_dir = (
        Path(train_json).parent.parent.parent / "benchmarks" / "data" / "generated" / "small"
    )
    for schema in ("flat", "nested", "wide_arrays", "mixed"):
        schema_file = bench_data_dir / f"{schema}.json"
        if schema_file.exists():
            schema_text = schema_file.read_text(encoding="utf-8")
            _timed(f"loads({schema})", lambda t=schema_text: strata.loads(t), repeat=3)
            schema_data = strata.loads(schema_text)
            _timed(f"dumps({schema})", lambda d=schema_data: strata.dumps(d), repeat=3)

    # 7. Roundtrip training with diverse data
    print("[7/8] roundtrip training", flush=True)
    for schema in ("flat", "nested", "wide_arrays", "mixed"):
        schema_file = bench_data_dir / f"{schema}.json"
        if schema_file.exists():
            schema_text = schema_file.read_text(encoding="utf-8")
            for _ in range(2):
                d = strata.loads(schema_text)
                strata.dumps(d)

    # 8. Iterator paths
    print("[8/8] iterator paths", flush=True)
    _timed("loads(iterator)", lambda: list(strata.loads(json_text, iterator=True)), repeat=1)
    _timed(
        "search(iterator)",
        lambda: list(
            strata.search(json_path, strata.compile_path("$.records[*].id"), iterator=True)
        ),
        repeat=1,
    )

    print("\nPGO training workload complete.")


def main() -> int:
    import argparse

    parser = argparse.ArgumentParser(description="Run PGO training workload")
    parser.add_argument("--json", type=Path, default=Path("build/pgo/train.json"))
    parser.add_argument("--ndjson", type=Path, default=Path("build/pgo/train.ndjson"))
    args = parser.parse_args()

    if not args.json.exists():
        print(f"Error: {args.json} not found. Run pgo_training_data.py first.")
        return 1
    if not args.ndjson.exists():
        print(f"Error: {args.ndjson} not found. Run pgo_training_data.py first.")
        return 1

    run(args.json, args.ndjson)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

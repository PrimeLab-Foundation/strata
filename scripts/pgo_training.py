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


def run(train_json: Path, train_ndjson: Path) -> None:
    """Execute the full training workload."""
    json_text = train_json.read_text(encoding="utf-8")
    json_bytes = train_json.read_bytes()
    json_path = str(train_json)
    ndjson_path = str(train_ndjson)

    print("\nPGO Training Workload")
    print(f"  JSON:   {train_json} ({train_json.stat().st_size / 1024 / 1024:.1f}MB)")
    print(f"  NDJSON: {train_ndjson} ({train_ndjson.stat().st_size / 1024 / 1024:.1f}MB)")
    print()

    # 1. loads — parse JSON text (str and bytes)
    print("[1/6] loads (JSON parsing)")
    _timed("loads(str)", lambda: strata.loads(json_text), repeat=15)
    _timed("loads(bytes)", lambda: strata.loads(json_bytes), repeat=15)

    # 2. dumps — serialize
    print("[2/6] dumps (serialization)")
    data = strata.loads(json_text)
    _timed("dumps(str)", lambda: strata.dumps(data), repeat=15)
    _timed("dumps(bytes)", lambda: strata.dumps(data, return_type="bytes"), repeat=15)

    # 3. load — file I/O
    print("[3/6] load (file I/O)")
    _timed("load(json)", lambda: strata.load(json_path), repeat=8)
    _timed("load(ndjson)", lambda: strata.load(ndjson_path), repeat=8)

    # 4. search — SAX search handler (file-based, diverse queries)
    print("[4/6] search (SAX file-based)")
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
        _timed(f"search({desc})", lambda p=path: strata.search(json_path, p), repeat=30)

    # 5. query — in-memory JSONPath
    print("[5/6] query (in-memory JSONPath)")
    query_queries = [
        ("$.records[*].name", "All names"),
        ("$.records[?(@.age > 50)]", "Filter age > 50"),
        ("$.records[0:100].id", "Slice first 100"),
        ("$..tags", "RecursiveDescent on arrays"),
        ("$.records[*].metadata.scores[*]", "Nested wildcard"),
    ]
    for expr, desc in query_queries:
        path = strata.compile_path(expr)
        _timed(f"query({desc})", lambda p=path: strata.query(data, p), repeat=30)

    # 6. Iterator paths
    print("[6/6] iterator paths")
    _timed("loads(iterator)", lambda: list(strata.loads(json_text, iterator=True)), repeat=10)
    _timed(
        "search(iterator)",
        lambda: list(
            strata.search(json_path, strata.compile_path("$.records[*].id"), iterator=True)
        ),
        repeat=15,
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

#!/usr/bin/env python3
"""Run the PGO training workload against an instrumented build.

Every public entry point that has a hot path is exercised here, because a
branch the training run never reaches is a branch the optimizer will guess
about — and PGO guesses *worse* than a static heuristic when the profile says
"never taken" about something that happens all the time in production.

The instrumented extension is roughly two orders of magnitude slower than a
normal build, so each step runs a handful of times over a slice of the corpus
rather than the whole of it. Branch coverage is what matters, not volume.
"""

from __future__ import annotations

import argparse
import gc
import sys
import time
from pathlib import Path

import strata


def _step(label: str, func, repeat: int = 1) -> None:
    gc.collect()
    started = time.perf_counter()
    for _ in range(repeat):
        func()
    elapsed = time.perf_counter() - started
    print(f"  {label}: {repeat}x in {elapsed:.2f}s")
    sys.stdout.flush()


def run(train_json: Path, train_ndjson: Path, work_dir: Path) -> None:
    work_dir.mkdir(parents=True, exist_ok=True)

    text = train_json.read_text(encoding="utf-8")
    data = strata.loads(text)
    # A slice keeps the round-trip steps affordable under instrumentation.
    sample = data[: max(1, len(data) // 20)]
    sample_text = strata.dumps(sample)

    print("PGO training workload")
    print(f"  corpus: {len(data)} records, {len(text) / 1024 / 1024:.1f} MB")

    print("[1/7] loads")
    _step("loads(str)", lambda: strata.loads(text))
    _step("loads(bytes)", lambda: strata.loads(text.encode()))
    _step("loads(sample)", lambda: strata.loads(sample_text), repeat=5)
    _step("loads(cursor)", lambda: strata.loads(sample_text, return_type="cursor"), repeat=5)
    _step("loads(iterator)", lambda: list(strata.loads(sample_text, iterator=True)), repeat=3)

    print("[2/7] dumps")
    _step("dumps(str)", lambda: strata.dumps(data))
    # Repeated: the second call on takes bytes mode's exact-fit path (the
    # block sized to the previous document, the tail staged), which is the
    # steady state every benchmark row measures and one call never reaches.
    _step("dumps(bytes)", lambda: strata.dumps(data, return_type="bytes"), repeat=3)
    _step("dumps(sample, bytes)", lambda: strata.dumps(sample, return_type="bytes"), repeat=5)
    _step("dumps(sample)", lambda: strata.dumps(sample), repeat=5)

    print("[3/7] load")
    _step("load(json)", lambda: strata.load(str(train_json)))
    _step("load(ndjson)", lambda: strata.load(str(train_ndjson)))
    _step("load(ndjson, iterator)", lambda: list(strata.load(str(train_ndjson), iterator=True)))

    print("[4/7] dump")
    out_json = work_dir / "out.json"
    _step("dump(json)", lambda: strata.dump(sample, str(out_json)), repeat=3)

    print("[5/7] query")
    # Plain paths, wildcards, recursive descent, slices and filters — the three
    # evaluators plus the compiler. Filters stay inside the documented grammar
    # (docs/context/api.md): numeric comparisons, and `==`/`!=` for strings.
    expressions = [
        "$[0].name",
        "$[*].id",
        "$[*].items[*].sku",
        "$..leaf",
        "$[0:50].score",
        "$[?(@.id > 100)].name",
        "$[?(@.id <= 500)]",
        "$[?(@.name == 'alpha')]",
        "$[?(@.name != 'alpha')].id",
        "$[?(@['id'] >= 0)].name",
        "$[*].nested.child",
        "$[*].tags[0]",
        "$.*",
    ]
    for expression in expressions:
        _step(f"query {expression}", lambda e=expression: strata.query(sample, e), repeat=3)
    compiled = strata.compile("$[*].id")
    _step("query(compiled)", lambda: strata.query(sample, compiled), repeat=5)

    print("[6/7] search")
    # Streamable paths take the SAX route; the filter forces the full parse.
    for expression in ("$[*].id", "$..leaf", "$[?(@.id > 100)].name"):
        _step(f"search json {expression}", lambda e=expression: strata.search(str(train_json), e))
        _step(
            f"search ndjson {expression}",
            lambda e=expression: strata.search(str(train_ndjson), e),
        )

    print("[7/7] folder mode")
    folder = work_dir / "split"
    # Every training record carries a bool "active", so grouping produces
    # exactly true.json and false.json.
    _step("dump(split_by)", lambda: strata.dump(sample, str(folder), split_by="active"))
    _step("load(folder)", lambda: strata.load(str(folder)))
    _step("search(folder)", lambda: strata.search(str(folder), "$[*].id"))

    print("PGO training workload complete")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", type=Path, required=True)
    parser.add_argument("--ndjson", type=Path, required=True)
    parser.add_argument("--work-dir", type=Path, required=True)
    args = parser.parse_args()
    run(args.json, args.ndjson, args.work_dir)


if __name__ == "__main__":
    main()

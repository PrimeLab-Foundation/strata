#!/usr/bin/env python3
"""
Strata benchmark regression check.

Compares current benchmark results (from bench_main output) against a stored
baseline. Fails if Strata median time or RSS regresses by more than the
configured threshold (default 5%). Used in CI to block performance regressions.

Usage:
  python benchmarks/regression_check.py docs/benchmarks/bench_results.md
  python benchmarks/regression_check.py docs/benchmarks/bench_results.md --save-baseline
  python -m benchmarks.regression_check docs/benchmarks/bench_results.md --threshold 5
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path


def _repo_root() -> Path:
    """Repository root (parent of benchmarks/)."""
    return Path(__file__).resolve().parents[1]


def parse_bench_results_md(path: Path) -> tuple[list[dict], list[dict]]:
    """
    Parse legacy bench_results.md produced by bench_main (backward compat).
    """
    text = path.read_text()
    parsing_rows: list[dict] = []
    query_rows: list[dict] = []

    # Parsing table: | Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
    in_parsing = False
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("## Parsing Benchmarks"):
            in_parsing = True
            continue
        if in_parsing and line.startswith("|") and "Library" in line:
            continue
        if in_parsing and line.startswith("|") and re.match(r"\|-+\|", line):
            continue
        if in_parsing and line.startswith("|"):
            parts = [p.strip() for p in line.split("|") if p.strip()]
            if len(parts) >= 6:
                lib, dataset, _min_ms_s, median_s, _p95_s, rss_s = parts[:6]
                if median_s in {"ERROR", "-", ""} or rss_s in {"ERROR", "-", ""}:
                    continue
                try:
                    median_ms = float(median_s)
                    rss_mb = float(rss_s)
                except ValueError:
                    continue
                parsing_rows.append(
                    {
                        "library": lib,
                        "dataset": dataset,
                        "median_ms": median_ms,
                        "rss_mb": rss_mb,
                    }
                )
            continue
        if in_parsing and line.startswith("##"):
            in_parsing = False

    # Query table: | Query | Library | Min (ms) | Results |
    in_query = False
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("## Query Benchmarks"):
            in_query = True
            continue
        if in_query and line.startswith("|") and "Query" in line:
            continue
        if in_query and line.startswith("|") and re.match(r"\|-+\|", line):
            continue
        if in_query and line.startswith("|"):
            parts = [p.strip() for p in line.split("|") if p.strip()]
            if len(parts) >= 3:
                if len(parts) >= 4:
                    query_col, lib, min_ms_s, results_s = parts[0], parts[1], parts[2], parts[3]
                else:
                    query_col, lib, min_ms_s, results_s = "", parts[0], parts[1], parts[2]
                if min_ms_s in {"ERROR", "-", ""}:
                    continue
                try:
                    min_ms = float(min_ms_s)
                    result_count = int(results_s) if results_s.isdigit() else 0
                except ValueError:
                    continue
                query_rows.append(
                    {
                        "query": query_col or "",
                        "library": lib,
                        "min_ms": min_ms,
                        "result_count": result_count,
                    }
                )
            continue
        if in_query and line.startswith("##"):
            in_query = False

    # Fix query rows: empty query means same as previous
    last_query = ""
    for row in query_rows:
        if row["query"]:
            last_query = row["query"]
        else:
            row["query"] = last_query

    return parsing_rows, query_rows


def parse_bench_results_json(path: Path) -> tuple[list[dict], list[dict]]:
    """
    Parse bench_suite JSON output.
    """
    data = json.loads(path.read_text())
    results = data.get("results", [])
    parsing_rows: list[dict] = []
    query_rows: list[dict] = []

    for r in results:
        if r.get("error"):
            continue
        cat = r.get("category")
        if cat == "loads":
            parsing_rows.append(
                {
                    "library": r.get("library"),
                    "dataset": r.get("dataset"),
                    "median_ms": r.get("median_ms", 0.0),
                    "rss_mb": r.get("rss_mb", 0.0),
                }
            )
        elif cat == "search":
            times = r.get("times_ms") or []
            min_ms = min(times) if times else r.get("min_ms", 0.0)
            query_rows.append(
                {
                    "query": r.get("variant") or "",
                    "library": r.get("library"),
                    "min_ms": min_ms,
                    "result_count": r.get("result_count", 0),
                }
            )

    return parsing_rows, query_rows


def parse_bench_results(path: Path) -> tuple[list[dict], list[dict]]:
    if path.suffix == ".json":
        return parse_bench_results_json(path)
    return parse_bench_results_md(path)


def load_baseline(baseline_path: Path) -> dict | None:
    """Load baseline JSON. Returns None if file missing or invalid."""
    if not baseline_path.is_file():
        return None
    try:
        return json.loads(baseline_path.read_text())
    except (json.JSONDecodeError, OSError):
        return None


def save_baseline(baseline_path: Path, parsing_rows: list[dict], query_rows: list[dict]) -> None:
    """Save Strata-only metrics as baseline."""
    baseline = {
        "parsing": {},
        "query": {},
    }
    for row in parsing_rows:
        if row["library"].lower() == "strata":
            baseline["parsing"][row["dataset"]] = {
                "median_ms": row["median_ms"],
                "rss_mb": row["rss_mb"],
            }
    for row in query_rows:
        if row["library"].lower() == "strata":
            key = row["query"] or "unknown"
            baseline["query"][key] = {"min_ms": row["min_ms"], "result_count": row["result_count"]}
    baseline_path.parent.mkdir(parents=True, exist_ok=True)
    baseline_path.write_text(json.dumps(baseline, indent=2) + "\n")


def check_regression(
    current_parsing: list[dict],
    current_query: list[dict],
    baseline: dict,
    threshold_pct: float,
) -> tuple[bool, list[str]]:
    """
    Compare current Strata metrics to baseline.
    Returns (ok, list of failure messages). ok is False if any regression > threshold_pct.
    """
    failures: list[str] = []
    threshold = 1.0 + (threshold_pct / 100.0)  # e.g. 5% -> 1.05

    baseline_parse = baseline.get("parsing") or {}
    for row in current_parsing:
        if row["library"].lower() != "strata":
            continue
        dataset = row["dataset"]
        bl = baseline_parse.get(dataset)
        if not bl:
            continue
        median = row["median_ms"]
        bl_median = bl.get("median_ms")
        if bl_median is not None and median > bl_median * threshold:
            pct = ((median / bl_median) - 1.0) * 100
            failures.append(
                f"strata parse {dataset}: median {median:.3f}ms vs baseline {bl_median:.3f}ms (+{pct:.1f}% > {threshold_pct}%)"
            )
        rss = row["rss_mb"]
        bl_rss = bl.get("rss_mb")
        if bl_rss is not None and rss > bl_rss * threshold:
            pct = ((rss / bl_rss) - 1.0) * 100
            failures.append(
                f"strata parse {dataset}: RSS {rss:.1f}MB vs baseline {bl_rss:.1f}MB (+{pct:.1f}% > {threshold_pct}%)"
            )

    baseline_query = baseline.get("query") or {}
    for row in current_query:
        if row["library"].lower() != "strata":
            continue
        query = row["query"] or "unknown"
        bl = baseline_query.get(query)
        if not bl:
            continue
        min_ms = row["min_ms"]
        bl_min = bl.get("min_ms")
        if bl_min is not None and min_ms > bl_min * threshold:
            pct = ((min_ms / bl_min) - 1.0) * 100
            failures.append(
                f"strata query {query!r}: min {min_ms:.3f}ms vs baseline {bl_min:.3f}ms (+{pct:.1f}% > {threshold_pct}%)"
            )

    return (len(failures) == 0, failures)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Check benchmark results for regressions vs baseline"
    )
    parser.add_argument(
        "results_file",
        nargs="?",
        default="docs/benchmarks/bench_results_small.json",
        help="Path to benchmark results (bench_suite JSON preferred)",
    )
    parser.add_argument(
        "--baseline",
        default=None,
        help="Path to baseline JSON (default: benchmarks/results/baseline.json)",
    )
    parser.add_argument(
        "--save-baseline",
        action="store_true",
        help="Save current Strata metrics as baseline and exit 0",
    )
    parser.add_argument(
        "--threshold",
        type=float,
        default=5.0,
        help="Regression threshold in percent (default: 5)",
    )
    args = parser.parse_args()

    repo = _repo_root()
    results_path = (
        repo / args.results_file
        if not Path(args.results_file).is_absolute()
        else Path(args.results_file)
    )
    baseline_path = (
        Path(args.baseline) if args.baseline else repo / "benchmarks" / "results" / "baseline.json"
    )

    if not results_path.is_file():
        print(f"Error: results file not found: {results_path}", file=sys.stderr)
        return 2

    parsing_rows, query_rows = parse_bench_results(results_path)

    if args.save_baseline:
        save_baseline(baseline_path, parsing_rows, query_rows)
        print(f"Baseline saved to {baseline_path}")
        return 0

    baseline = load_baseline(baseline_path)
    if not baseline:
        print(
            "No baseline found; run with --save-baseline to create one. Skipping regression check."
        )
        return 0

    ok, failures = check_regression(parsing_rows, query_rows, baseline, args.threshold)
    if ok:
        print("Regression check passed.")
        return 0
    for msg in failures:
        print(msg, file=sys.stderr)
    print("Regression check failed.", file=sys.stderr)
    return 1


if __name__ == "__main__":
    sys.exit(main())

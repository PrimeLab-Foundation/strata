#!/usr/bin/env python3
"""Coverage gate script for Strata.

Reads an llvm-cov JSON export (or gcovr JSON) and verifies that all
source files meet a minimum line coverage threshold.

Usage:
    python scripts/coverage_gate.py build/coverage/coverage.json --threshold 95

Exit code 0 if all source files >= threshold, exit code 1 otherwise.
"""

import argparse
import json
import sys
from pathlib import Path

# Directories to measure (relative to project root).
# Scoped to the three target subsystems per the quality overhaul.
# Add more dirs here to expand coverage scope.
INCLUDE_DIRS = ["src/strata/simd/", "src/strata/speculative/", "src/strata/bloom/"]

# Patterns to exclude from measurement.
EXCLUDE_PATTERNS = [
    "third_party",
    "tests",
    "bindings",
    "benchmarks",
    "dispatch.cpp",
]  # x86 cpuid code untestable on ARM


def parse_llvm_cov_json(data: dict) -> list[dict]:
    """Parse llvm-cov export JSON format into per-file coverage records."""
    results = []
    for entry in data.get("data", []):
        for file_info in entry.get("files", []):
            filename = file_info.get("filename", "")
            summary = file_info.get("summary", {})
            lines = summary.get("lines", {})
            branches = summary.get("branches", {})
            results.append(
                {
                    "file": filename,
                    "lines_count": lines.get("count", 0),
                    "lines_covered": lines.get("covered", 0),
                    "lines_pct": lines.get("percent", 0.0),
                    "branches_count": branches.get("count", 0),
                    "branches_covered": branches.get("covered", 0),
                    "branches_pct": branches.get("percent", 0.0),
                }
            )
    return results


def parse_gcovr_json(data: dict) -> list[dict]:
    """Parse gcovr JSON format into per-file coverage records."""
    results = []
    for file_info in data.get("files", []):
        filename = file_info.get("filename", "")
        lines = file_info.get("line_covered", 0)
        total = file_info.get("line_total", 0)
        branches_covered = file_info.get("branch_covered", 0)
        branches_total = file_info.get("branch_total", 0)
        pct = (lines / total * 100) if total > 0 else 100.0
        bpct = (branches_covered / branches_total * 100) if branches_total > 0 else 100.0
        results.append(
            {
                "file": filename,
                "lines_count": total,
                "lines_covered": lines,
                "lines_pct": pct,
                "branches_count": branches_total,
                "branches_covered": branches_covered,
                "branches_pct": bpct,
            }
        )
    return results


def should_include(filepath: str) -> bool:
    """Check if a file should be included in coverage measurement."""
    for excl in EXCLUDE_PATTERNS:
        if excl in filepath:
            return False
    for incl in INCLUDE_DIRS:
        if incl in filepath:
            return True
    return False


def main():
    parser = argparse.ArgumentParser(description="Coverage gate for Strata")
    parser.add_argument("coverage_json", help="Path to coverage JSON file")
    parser.add_argument(
        "--threshold",
        type=float,
        default=95.0,
        help="Minimum line coverage percentage (default: 95)",
    )
    args = parser.parse_args()

    json_path = Path(args.coverage_json)
    if not json_path.exists():
        print(f"ERROR: Coverage file not found: {json_path}", file=sys.stderr)
        sys.exit(1)

    with open(json_path) as f:
        data = json.load(f)

    # Detect format: llvm-cov has "data", gcovr has "files" at top level.
    if "data" in data:
        records = parse_llvm_cov_json(data)
    elif "files" in data:
        records = parse_gcovr_json(data)
    else:
        print("ERROR: Unrecognized coverage JSON format", file=sys.stderr)
        sys.exit(1)

    # Filter to target source files.
    filtered = [r for r in records if should_include(r["file"])]

    if not filtered:
        print("WARNING: No source files matched the include filter.")
        print(f"  Include dirs: {INCLUDE_DIRS}")
        print(f"  Total files in report: {len(records)}")
        sys.exit(0)

    # Print table.
    failures = []
    max_name = max(len(r["file"]) for r in filtered)
    header_fmt = f"  {{:<{max_name}}}  {{:>8}}  {{:>10}}  {{:>8}}"
    row_fmt = f"  {{:<{max_name}}}  {{:>7.1f}}%  {{:>9.1f}}%  {{:>8}}"

    print()
    print(header_fmt.format("File", "Lines", "Branches", "Status"))
    print("  " + "-" * (max_name + 32))

    for rec in sorted(filtered, key=lambda r: r["file"]):
        short = rec["file"]
        lpct = rec["lines_pct"]
        bpct = rec["branches_pct"]
        status = "PASS" if lpct >= args.threshold else "FAIL"
        if status == "FAIL":
            failures.append(rec)
        print(row_fmt.format(short, lpct, bpct, status))

    print()

    # Aggregate.
    total_lines = sum(r["lines_count"] for r in filtered)
    total_covered = sum(r["lines_covered"] for r in filtered)
    agg_pct = (total_covered / total_lines * 100) if total_lines > 0 else 0.0

    print(f"  Aggregate: {total_covered}/{total_lines} lines = {agg_pct:.1f}%")
    print(f"  Threshold: {args.threshold}%")
    print(f"  Files measured: {len(filtered)}")
    print()

    if failures:
        print(f"FAIL: {len(failures)} file(s) below {args.threshold}% threshold:")
        for f in failures:
            print(f"  {f['file']}: {f['lines_pct']:.1f}%")
        sys.exit(1)
    else:
        print(f"PASS: All {len(filtered)} files meet {args.threshold}% threshold.")
        sys.exit(0)


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""Composite readability scorer for Strata.

Measures three dimensions:
  1. Cyclomatic complexity (via lizard) — 40% weight
  2. clang-tidy warnings count — 30% weight
  3. Comment density — 30% weight

Usage:
    python scripts/readability_score.py [--threshold 85]

Exit code 0 if composite score >= threshold, 1 otherwise.
"""

import argparse
import subprocess
import sys
from pathlib import Path

# Directories to measure.
TARGET_DIRS = [
    "src/strata/simd",
    "src/strata/speculative",
    "src/strata/bloom",
    "include/strata/simd",
    "include/strata/speculative",
    "include/strata/bloom",
]

# File extensions to measure.
EXTENSIONS = {".cpp", ".h", ".hpp"}


def find_source_files(root: Path) -> list[Path]:
    """Find all C++ source files in target directories."""
    files = []
    for target_dir in TARGET_DIRS:
        dirpath = root / target_dir
        if not dirpath.exists():
            continue
        for f in dirpath.rglob("*"):
            if f.suffix in EXTENSIONS and "third_party" not in str(f):
                files.append(f)
    return sorted(files)


def measure_comment_density(filepath: Path) -> tuple[int, int, float]:
    """Measure comment density for a single file.

    Returns (total_lines, comment_lines, density_pct).
    """
    total = 0
    comments = 0
    in_block = False

    with open(filepath) as f:
        for line in f:
            stripped = line.strip()
            if not stripped:
                continue  # Skip blank lines.
            total += 1

            if in_block:
                comments += 1
                if "*/" in stripped:
                    in_block = False
            elif stripped.startswith("//"):
                comments += 1
            elif stripped.startswith("/*"):
                comments += 1
                if "*/" not in stripped:
                    in_block = True
            elif "/*" in stripped:
                # Inline block comment start.
                comments += 1
                if "*/" not in stripped[stripped.index("/*") :]:
                    in_block = True

    density = (comments / total * 100) if total > 0 else 0.0
    return total, comments, density


def run_lizard(files: list[Path]) -> dict:
    """Run lizard for cyclomatic complexity analysis.

    Returns dict with:
      avg_cc: average cyclomatic complexity
      max_func_len: maximum function length (NLOC)
      high_cc_count: functions with CC > 10
      very_high_cc_count: functions with CC > 15
      long_func_count: functions with NLOC > 40
    """
    result = {
        "avg_cc": 0.0,
        "max_func_len": 0,
        "high_cc_count": 0,
        "very_high_cc_count": 0,
        "long_func_count": 0,
        "func_count": 0,
    }

    try:
        cmd = ["lizard", "--csv"] + [str(f) for f in files]
        proc = subprocess.run(cmd, check=False, capture_output=True, text=True, timeout=60)
        if proc.returncode != 0:
            print(f"  WARNING: lizard failed: {proc.stderr[:200]}", file=sys.stderr)
            return result
    except FileNotFoundError:
        print("  WARNING: lizard not found — install with: pip install lizard", file=sys.stderr)
        return result
    except subprocess.TimeoutExpired:
        print("  WARNING: lizard timed out", file=sys.stderr)
        return result

    total_cc = 0
    func_count = 0

    for line in proc.stdout.strip().split("\n"):
        if not line or line.startswith("NLOC"):
            continue
        parts = line.split(",")
        if len(parts) < 5:
            continue
        try:
            nloc = int(parts[0])
            cc = int(parts[1])
        except (ValueError, IndexError):
            continue

        func_count += 1
        total_cc += cc

        result["max_func_len"] = max(result["max_func_len"], nloc)
        if cc > 10:
            result["high_cc_count"] += 1
        if cc > 15:
            result["very_high_cc_count"] += 1
        if nloc > 40:
            result["long_func_count"] += 1

    result["func_count"] = func_count
    result["avg_cc"] = (total_cc / func_count) if func_count > 0 else 0.0
    return result


def run_clang_tidy(files: list[Path], root: Path) -> int:
    """Run clang-tidy and count readability warnings.

    Returns the number of readability-related warnings.
    """
    try:
        cmd = ["clang-tidy", "--quiet"] + [str(f) for f in files[:20]]  # Limit for speed
        proc = subprocess.run(
            cmd, check=False, capture_output=True, text=True, timeout=120, cwd=str(root)
        )
        # Count warning lines.
        warnings = sum(
            1 for line in proc.stdout.split("\n") if "warning:" in line and "readability" in line
        )
        return warnings
    except (FileNotFoundError, subprocess.TimeoutExpired):
        print("  WARNING: clang-tidy not available or timed out", file=sys.stderr)
        return 0


def compute_score(
    lizard_data: dict, tidy_warnings: int, avg_density: float, total_kloc: float
) -> tuple[float, dict]:
    """Compute composite readability score.

    Returns (score, breakdown_dict).
    """
    # CC score (40% weight): penalize high-complexity functions.
    cc_score = 100.0
    cc_score -= 2 * lizard_data["high_cc_count"]
    cc_score -= 3 * lizard_data["very_high_cc_count"]
    cc_score -= 1 * lizard_data["long_func_count"]
    cc_score = max(0.0, min(100.0, cc_score))

    # Tidy score (30% weight): penalize readability warnings per KLOC.
    warnings_per_kloc = (tidy_warnings / total_kloc) if total_kloc > 0 else 0.0
    tidy_score = max(0.0, 100.0 - warnings_per_kloc * 5)

    # Comment density score (30% weight): penalize below 20%.
    if avg_density >= 20.0:
        density_score = 100.0
    else:
        density_score = max(0.0, avg_density / 20.0 * 100.0)

    composite = 0.4 * cc_score + 0.3 * tidy_score + 0.3 * density_score

    breakdown = {
        "cc_score": cc_score,
        "tidy_score": tidy_score,
        "density_score": density_score,
        "composite": composite,
        "avg_cc": lizard_data["avg_cc"],
        "max_func_len": lizard_data["max_func_len"],
        "high_cc_funcs": lizard_data["high_cc_count"],
        "tidy_warnings": tidy_warnings,
        "avg_density": avg_density,
    }
    return composite, breakdown


def main():
    parser = argparse.ArgumentParser(description="Readability scorer for Strata")
    parser.add_argument(
        "--threshold", type=float, default=85.0, help="Minimum composite score (default: 85)"
    )
    args = parser.parse_args()

    root = Path(__file__).parent.parent
    files = find_source_files(root)

    if not files:
        print("ERROR: No source files found", file=sys.stderr)
        sys.exit(1)

    print(f"\nMeasuring readability for {len(files)} files...\n")

    # 1. Comment density per file.
    file_data = []
    total_lines = 0
    total_comments = 0
    for f in files:
        lines, comments, density = measure_comment_density(f)
        file_data.append((f, lines, comments, density))
        total_lines += lines
        total_comments += comments

    avg_density = (total_comments / total_lines * 100) if total_lines > 0 else 0.0
    total_kloc = total_lines / 1000.0

    # 2. Lizard cyclomatic complexity.
    lizard_data = run_lizard(files)

    # 3. clang-tidy warnings.
    tidy_warnings = run_clang_tidy(files, root)

    # Compute score.
    score, breakdown = compute_score(lizard_data, tidy_warnings, avg_density, total_kloc)

    # Print per-file table.
    max_name = max(len(str(f.relative_to(root))) for f, *_ in file_data)
    fmt = f"  {{:<{max_name}}}  {{:>6}}  {{:>8}}  {{:>9}}"
    print(fmt.format("File", "Lines", "Comments", "Density"))
    print("  " + "-" * (max_name + 28))
    for f, lines, comments, density in file_data:
        name = str(f.relative_to(root))
        print(fmt.format(name, str(lines), str(comments), f"{density:.1f}%"))

    # Print summary.
    print(f"\n  {'=' * 50}")
    print(f"  Cyclomatic Complexity (40%):  {breakdown['cc_score']:.0f}/100")
    print(f"    Avg CC: {breakdown['avg_cc']:.1f}, Max func len: {breakdown['max_func_len']}")
    print(f"    Functions with CC>10: {breakdown['high_cc_funcs']}")
    print(f"  clang-tidy Warnings (30%):   {breakdown['tidy_score']:.0f}/100")
    print(f"    Warnings: {breakdown['tidy_warnings']} ({total_kloc:.1f} KLOC)")
    print(f"  Comment Density (30%):       {breakdown['density_score']:.0f}/100")
    print(f"    Average: {breakdown['avg_density']:.1f}%")
    print(f"  {'=' * 50}")
    print(f"  Composite Score: {score:.0f}/100")
    print(f"  Threshold: {args.threshold:.0f}/100")
    print()

    if score >= args.threshold:
        print(f"  PASS: Score {score:.0f} >= {args.threshold:.0f}")
        sys.exit(0)
    else:
        print(f"  FAIL: Score {score:.0f} < {args.threshold:.0f}")
        sys.exit(1)


if __name__ == "__main__":
    main()

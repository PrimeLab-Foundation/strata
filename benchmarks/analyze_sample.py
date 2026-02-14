#!/usr/bin/env python3
"""
Analyze macOS sample output to extract C++ hotspots.
"""

from __future__ import annotations

import re
from pathlib import Path
from collections import defaultdict
import sys


def parse_sample_file(sample_path: Path) -> dict[str, int]:
    """Parse sample file and extract function samples."""
    content = sample_path.read_text()

    # Pattern to match lines with sample counts
    # Format: "    + 123 function_name (in library) + offset [address]"
    pattern = r'\s*\+?\s*(\d+)\s+([^\(]+)\s+\(in ([^\)]+)\)'

    function_samples = defaultdict(int)
    total_samples = 0

    for line in content.split('\n'):
        match = re.search(pattern, line)
        if match:
            count = int(match.group(1))
            func_name = match.group(2).strip()
            library = match.group(3).strip()

            # Aggregate by function name
            key = f"{func_name} (in {library})"
            function_samples[key] += count
            total_samples += count

    return dict(function_samples), total_samples


def categorize_functions(functions: dict[str, int]) -> dict[str, list[tuple[str, int]]]:
    """Categorize functions by subsystem."""
    categories = {
        'Python Object Creation': [],
        'Parsing Logic': [],
        'String Handling': [],
        'Number Parsing': [],
        'Dictionary Operations': [],
        'Memory Allocation': [],
        'Other': [],
    }

    for func, count in functions.items():
        if any(x in func for x in ['PyDict_', 'dict_', 'insert_combined_dict', 'insert_to_emptydict']):
            categories['Dictionary Operations'].append((func, count))
        elif any(x in func for x in ['PyFloat_FromDouble', 'PyLong_From', 'PyUnicode_From', 'PyList_', 'PyObject_New']):
            categories['Python Object Creation'].append((func, count))
        elif any(x in func for x in ['parse_string', 'parse_primitive', 'parse_int', 'parse_double', 'parse_uint']):
            categories['Parsing Logic'].append((func, count))
        elif any(x in func for x in ['unicode_decode', 'count_digits', 'parse_frac_part']):
            categories['Number Parsing'].append((func, count))
        elif any(x in func for x in ['PyUnicode', 'unicode', 'string']):
            categories['String Handling'].append((func, count))
        elif any(x in func for x in ['_PyObject_Malloc', 'pymalloc', 'bzero']):
            categories['Memory Allocation'].append((func, count))
        else:
            categories['Other'].append((func, count))

    # Sort each category by count
    for cat in categories:
        categories[cat].sort(key=lambda x: x[1], reverse=True)

    return categories


def print_hotspots(functions: dict[str, int], total_samples: int, top_n: int = 20):
    """Print top hotspots."""
    print("\n" + "="*100)
    print("TOP HOTSPOTS (by sample count)")
    print("="*100)

    sorted_funcs = sorted(functions.items(), key=lambda x: x[1], reverse=True)[:top_n]

    print(f"{'Rank':<6} {'Samples':<10} {'% Total':<10} {'Function'}")
    print("-" * 100)

    for i, (func, count) in enumerate(sorted_funcs, 1):
        pct = (count / total_samples * 100) if total_samples > 0 else 0
        # Truncate long function names
        func_short = func[:70] + "..." if len(func) > 70 else func
        print(f"{i:<6} {count:<10} {pct:<10.2f} {func_short}")

    print("-" * 100)
    print(f"Total samples: {total_samples:,}")
    print()


def print_category_breakdown(categories: dict[str, list[tuple[str, int]]], total_samples: int):
    """Print category breakdown."""
    print("\n" + "="*100)
    print("CATEGORY BREAKDOWN")
    print("="*100)

    category_totals = {}
    for cat, funcs in categories.items():
        total = sum(count for _, count in funcs)
        if total > 0:
            category_totals[cat] = total

    # Sort by total
    sorted_cats = sorted(category_totals.items(), key=lambda x: x[1], reverse=True)

    print(f"{'Category':<40} {'Samples':<12} {'% Total':<10}")
    print("-" * 100)

    for cat, total in sorted_cats:
        pct = (total / total_samples * 100) if total_samples > 0 else 0
        print(f"{cat:<40} {total:<12,} {pct:<10.2f}")

    print("-" * 100)
    print()

    # Print top functions in each major category
    for cat in ['Python Object Creation', 'Dictionary Operations', 'Parsing Logic', 'Number Parsing']:
        if cat in categories and categories[cat]:
            print(f"\n{cat} - Top Functions:")
            print(f"  {'Function':<70} {'Samples':<10} {'%':<8}")
            print("  " + "-" * 90)
            for func, count in categories[cat][:10]:
                pct = (count / total_samples * 100) if total_samples > 0 else 0
                func_short = func[:65] + "..." if len(func) > 65 else func
                print(f"  {func_short:<70} {count:<10,} {pct:<8.2f}")


def main():
    if len(sys.argv) < 2:
        print("Usage: python analyze_sample.py <sample_file.txt>")
        return 1

    sample_path = Path(sys.argv[1])
    if not sample_path.exists():
        print(f"Error: File not found: {sample_path}")
        return 1

    print(f"Analyzing: {sample_path}")

    functions, total_samples = parse_sample_file(sample_path)

    print(f"Parsed {len(functions)} unique functions")
    print(f"Total samples: {total_samples:,}")

    # Print top hotspots
    print_hotspots(functions, total_samples, top_n=20)

    # Categorize and print breakdown
    categories = categorize_functions(functions)
    print_category_breakdown(categories, total_samples)

    return 0


if __name__ == "__main__":
    sys.exit(main())

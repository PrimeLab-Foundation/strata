"""The cross-platform supportability tripwire.

CI runs the benchmark suite on platforms the development machine cannot
exercise (docs/context/convention.md § Platform supportability). Absolute
times mean nothing across machines, so this check gates on what *is*
platform-independent:

  - the run produced no ERROR rows (a library that failed is a broken build);
  - strata appears in every section the report contains (a category that
    silently vanished is a dispatch or build defect);
  - no row falls behind the best rival by more than ``--max-ratio`` (default
    3.0x) — loose on purpose, because its job is to catch a fast path that
    quietly fell back to scalar or misfired on foreign hardware, not to
    relitigate standings on a noisy shared runner.

Exit codes: 0 pass, 1 tripwire fired, 2 usage/report error.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from benchmarks.harness import parse_report

DEFAULT_MAX_RATIO = 3.0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", type=Path, help="a report written by bench_main")
    parser.add_argument(
        "--max-ratio",
        type=float,
        default=DEFAULT_MAX_RATIO,
        help="worst allowed strata-time / best-rival-time per row",
    )
    args = parser.parse_args(argv)

    if not args.report.is_file():
        sys.stderr.write(f"error: no such report: {args.report}\n")
        return 2

    report = parse_report(args.report.read_text(encoding="utf-8"), name=args.report.name)

    failures: list[str] = []
    sections_with_strata: set[str] = set()
    rows: dict[tuple[str, str], dict[str, float]] = {}

    for measurement in report.measurements:
        if measurement.failed:
            failures.append(
                f"{measurement.section}|{measurement.dataset}|{measurement.library}: "
                f"ERROR ({measurement.error})"
            )
            continue
        if measurement.library == "strata":
            sections_with_strata.add(measurement.section)
        if measurement.median_ms:
            rows.setdefault((measurement.section, measurement.dataset), {})[measurement.library] = (
                measurement.median_ms
            )

    all_sections = {m.section for m in report.measurements}
    for section in sorted(all_sections - sections_with_strata):
        failures.append(f"{section}: strata produced no measurement at all")

    for (section, dataset), libraries in sorted(rows.items()):
        strata_ms = libraries.get("strata")
        rivals = {name: ms for name, ms in libraries.items() if name != "strata"}
        if strata_ms is None or not rivals:
            continue
        best_rival = min(rivals.values())
        ratio = strata_ms / best_rival
        if ratio > args.max_ratio:
            failures.append(
                f"{section}|{dataset}: {ratio:.2f}x behind the best rival "
                f"(limit {args.max_ratio:.1f}x) — a fast path is misfiring on this platform"
            )

    if failures:
        sys.stderr.write("SUPPORTABILITY TRIPWIRE (docs/context/convention.md):\n")
        for failure in failures:
            sys.stderr.write(f"  {failure}\n")
        return 1

    print(f"supportability: {len(rows)} rows within {args.max_ratio:.1f}x, no ERROR rows")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

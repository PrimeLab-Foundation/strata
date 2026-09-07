"""The cross-platform supportability tripwire.

CI runs the benchmark suite on platforms the development machine cannot
exercise (docs/context/convention.md § Platform supportability). Absolute
times mean nothing across machines, so this check gates on what *is*
platform-independent:

  - the report is valid and complete: every declared row of the workload
    carries a usable strata measurement, every number is finite and ordered,
    no row is duplicated, no row is unreadable, and there are no ERROR rows
    (a library that failed is a broken build);
  - strata appears in every section the report contains (a category that
    silently vanished is a dispatch or build defect);
  - no row falls behind the best rival by more than ``--max-ratio`` (default
    3.0x) — loose on purpose, because its job is to catch a fast path that
    quietly fell back to scalar or misfired on foreign hardware, not to
    relitigate standings on a noisy shared runner.

The completeness half is new, and it is the point: the reviewed tripwire
checked only the rows it happened to receive, so an empty report, a dataset
that never ran, or a `nan` median all passed it
(docs/performance/ci-review-2026-09-07.md, finding 2). The 3.0x bound is
deliberately unchanged.

``--expect`` names the workload to require: ``ci`` (the declared 27-row suite,
the default — the CI legs and every bench-small/medium/large tier run it) or
``none`` for an explicitly scoped diagnostic report, where validity is still
checked and completeness is not claimed.

The exit codes are `regression_check`'s, deliberately: the two tools judge the
same reports, and the reviewed pair disagreed about which code an unusable
report body deserved — 1 here, 2 there (build/evidence/T2-REVIEW/REVIEW.md,
defect 4). One convention now, stated identically in both:

* 0 — pass.
* 1 — the report is gateable evidence and the gate's own verdict is negative:
  here, a row more than ``--max-ratio`` behind the best rival, or a category
  the report contains with no strata row in it.
* 2 — the report is not gateable evidence, so no verdict was reached: a
  missing file, unreadable rows, ERROR rows, absent/non-finite/negative/zero
  or out-of-order numbers, a duplicated row, an empty report, or a report
  short of the declared workload.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from benchmarks.harness import (
    MEASURED_LIBRARY,
    WORKLOADS,
    read_report,
    resolve_workload,
    validate_report,
)

DEFAULT_MAX_RATIO = 3.0
DEFAULT_WORKLOAD = "ci"

# Shared, word for word, with regression_check: one convention for two tools
# that read the same reports.
EXIT_CODES = (
    "Exit codes: 0 pass; 1 the report is gateable evidence and the gate's "
    "verdict is negative; 2 the report is not gateable evidence (missing file, "
    "unreadable or ERROR rows, unusable numbers, a duplicated row, an empty "
    "report, or a report short of the declared workload)."
)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, epilog=EXIT_CODES)
    parser.add_argument("report", type=Path, help="a report written by bench_main")
    parser.add_argument(
        "--max-ratio",
        type=float,
        default=DEFAULT_MAX_RATIO,
        help="worst allowed strata-time / best-rival-time per row",
    )
    parser.add_argument(
        "--expect",
        default=DEFAULT_WORKLOAD,
        choices=sorted(WORKLOADS),
        help="the declared workload every strata row of which must be present",
    )
    args = parser.parse_args(argv)

    if not args.report.is_file():
        sys.stderr.write(f"error: no such report: {args.report}\n")
        return 2

    report = read_report(args.report)
    expected = resolve_workload(args.expect)
    validation = validate_report(report, expected=expected)

    # Two buckets, two exit codes: what makes the report ungateable, and what
    # the gate itself found once it could read it.
    ungateable: list[str] = [str(problem) for problem in validation.problems if problem.fatal]
    failures: list[str] = []

    rows: dict[tuple[str, str], dict[str, float]] = {}
    sections_with_strata: set[str] = set()
    for measurement in report.measurements:
        if measurement.failed or measurement.median_ms is None:
            continue
        if measurement.library == MEASURED_LIBRARY:
            sections_with_strata.add(measurement.section)
        rows.setdefault((measurement.section, measurement.dataset), {})[measurement.library] = (
            measurement.median_ms
        )

    all_sections = {m.section for m in report.measurements}
    for section in sorted(all_sections - sections_with_strata):
        failures.append(f"{section}: strata produced no measurement at all")
    if expected is not None:
        for section in sorted({section for section, _ in expected} - sections_with_strata):
            # A declared category nobody measured is the missing-row problem
            # again, named at the category it costs.
            ungateable.append(f"{section}: declared category with no strata measurement")

    for (section, dataset), libraries in sorted(rows.items()):
        strata_ms = libraries.get(MEASURED_LIBRARY)
        rivals = {name: ms for name, ms in libraries.items() if name != MEASURED_LIBRARY}
        if strata_ms is None or not rivals:
            continue
        best_rival = min(rivals.values())
        if best_rival <= 0:
            continue  # flagged as an invalid median above; no ratio to compute
        ratio = strata_ms / best_rival
        if ratio > args.max_ratio:
            failures.append(
                f"{section}|{dataset}: {ratio:.2f}x behind the best rival "
                f"(limit {args.max_ratio:.1f}x) — a fast path is misfiring on this platform"
            )

    disclosed = [str(problem) for problem in validation.problems if not problem.fatal]

    if ungateable:
        sys.stderr.write(f"error: {args.report.name} is not gateable evidence:\n")
        for problem in ungateable:
            sys.stderr.write(f"  {problem}\n")
        for failure in failures:
            sys.stderr.write(f"  (also) {failure}\n")
        for note in disclosed:
            sys.stderr.write(f"  (disclosed) {note}\n")
        return 2

    if failures:
        sys.stderr.write("SUPPORTABILITY TRIPWIRE (docs/context/convention.md):\n")
        for failure in failures:
            sys.stderr.write(f"  {failure}\n")
        for note in disclosed:
            sys.stderr.write(f"  (disclosed) {note}\n")
        return 1

    for note in disclosed:
        print(f"note: {note}")
    print(
        f"supportability: {validation.describe()}; "
        f"{len(rows)} rows within {args.max_ratio:.1f}x, no ERROR rows"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

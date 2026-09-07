"""Compare a benchmark report against the recorded baseline.

Thresholds are the contract's, not this tool's invention: a touched category
whose **median or p95 is more than 2% slower**, or whose **RSS is more than 5%
larger**, is a breach (docs/context/benchmarks.md). The previous tool applied a
single 5% threshold, which was part of what made it useless.

It reads reports through `harness.parse_report`, the inverse of the writer's
`render_report`, so the two cannot drift. Baselines are keyed by the full
report name.

**A gate that compares nothing does not pass.** The reviewed checker skipped
every unmatched key and then reported success, so a report whose filename did
not match its baseline entries printed "compared 0 of 1 entries / no
regressions" and exited 0
(docs/performance/ci-review-2026-09-07.md, finding 3). Coverage is now part of
the verdict: the comparison scope is every baseline entry recorded under this
report's name, each of those rows must be present in the current report with
the metrics the baseline holds, and a scope that is empty or partly missing is
missing evidence — exit 1, the same non-zero a breach gets. Rows the baseline
has never seen stay ungated, named explicitly rather than passed silently.
Thresholds and the full-filename key contract are unchanged.

The exit codes are `supportability_check`'s, deliberately: the two tools judge
the same reports, and the reviewed pair disagreed about which code an unusable
report body deserved — 2 here, 1 there (build/evidence/T2-REVIEW/REVIEW.md,
defect 4). One convention now, stated identically in both:

* 0 — pass.
* 1 — the report is gateable evidence and the gate's own verdict is negative:
  here, a threshold breach, or a baseline scope the report does not cover.
* 2 — the report is not gateable evidence, so no verdict was reached: a
  missing file, unreadable rows, ERROR rows, absent/non-finite/negative/zero
  or out-of-order numbers, a duplicated row, an empty report, or a report
  short of the declared workload. A missing or unusable baseline is the same
  code: there is nothing to gate against.
"""

from __future__ import annotations

import argparse
import json
import sys
from dataclasses import dataclass
from pathlib import Path

from benchmarks.harness import (
    MEASURED_LIBRARY,
    WORKLOADS,
    Report,
    baseline_key,
    parse_report,
    resolve_workload,
    validate_report,
)

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_BASELINE = PROJECT_ROOT / "benchmarks" / "results" / "baseline.json"

MEDIAN_TOLERANCE = 0.02  # 2%
P95_TOLERANCE = 0.02  # 2%
RSS_TOLERANCE = 0.05  # 5%

# The metrics a comparison needs. RSS is gated only where the baseline records
# it: `peak_rss_mb` returns None when psutil is absent, and that is a
# documented capability of the run, not a missing measurement.
REQUIRED_METRICS = ("median_ms", "p95_ms")

DEFAULT_WORKLOAD = "ci"

# Shared, word for word, with supportability_check: one convention for two
# tools that read the same reports.
EXIT_CODES = (
    "Exit codes: 0 pass; 1 the report is gateable evidence and the gate's "
    "verdict is negative; 2 the report is not gateable evidence (missing file, "
    "unreadable or ERROR rows, unusable numbers, a duplicated row, an empty "
    "report, or a report short of the declared workload)."
)


@dataclass(frozen=True)
class Breach:
    key: str
    metric: str
    baseline: float
    current: float

    @property
    def change(self) -> float:
        if self.baseline == 0:
            return 0.0
        return (self.current - self.baseline) / self.baseline

    def __str__(self) -> str:
        return (
            f"{self.key} {self.metric}: {self.baseline:.3f} -> {self.current:.3f} "
            f"({self.change * 100:+.1f}%)"
        )


@dataclass(frozen=True)
class Coverage:
    """What the comparison actually covered, and what it could not."""

    scope: tuple[str, ...]  # baseline entries recorded under this report's name
    compared: tuple[str, ...]  # scope entries found in the current report
    missing_rows: tuple[str, ...]  # scope entries the current report does not have
    missing_metrics: tuple[str, ...]  # "key metric" pairs the current report lacks
    ungated: tuple[str, ...]  # current entries with no baseline evidence

    @property
    def complete(self) -> bool:
        return bool(self.compared) and not self.missing_rows and not self.missing_metrics


def extract(report: Report) -> dict[str, dict[str, float]]:
    """Pull out the entries a baseline records: strata's own numbers."""
    entries: dict[str, dict[str, float]] = {}
    for row in report.measurements:
        if row.library != MEASURED_LIBRARY or row.failed:
            continue
        entry = {}
        if row.median_ms is not None:
            entry["median_ms"] = row.median_ms
        if row.p95_ms is not None:
            entry["p95_ms"] = row.p95_ms
        if row.rss_mb is not None:
            entry["rss_mb"] = row.rss_mb
        entries[baseline_key(report.name, row.section, row.dataset)] = entry
    return entries


def coverage(
    baseline: dict[str, dict[str, float]],
    current: dict[str, dict[str, float]],
    report_name: str,
) -> Coverage:
    """The comparison scope for `report_name`, and everything absent from it.

    The scope is the baseline's own entries for this report: those are the rows
    a gate on this report claims to check. Iterating the current entries alone
    — what the reviewed checker did — cannot see a row that disappeared.
    """
    prefix = f"{report_name}|"
    scope = tuple(sorted(key for key in baseline if key.startswith(prefix)))
    compared, missing_rows, missing_metrics = [], [], []
    for key in scope:
        entry = current.get(key)
        if entry is None:
            missing_rows.append(key)
            continue
        recorded = baseline[key]
        absent = sorted(
            metric
            for metric in (*REQUIRED_METRICS, "rss_mb")
            if metric in recorded and entry.get(metric) is None
        )
        missing_metrics.extend(f"{key} {metric}" for metric in absent)
        compared.append(key)
    ungated = tuple(sorted(key for key in current if key not in baseline))
    return Coverage(
        scope=scope,
        compared=tuple(compared),
        missing_rows=tuple(missing_rows),
        missing_metrics=tuple(missing_metrics),
        ungated=ungated,
    )


def compare(
    baseline: dict[str, dict[str, float]], current: dict[str, dict[str, float]]
) -> list[Breach]:
    tolerances = {
        "median_ms": MEDIAN_TOLERANCE,
        "p95_ms": P95_TOLERANCE,
        "rss_mb": RSS_TOLERANCE,
    }
    breaches: list[Breach] = []
    for key, entry in sorted(current.items()):
        recorded = baseline.get(key)
        if recorded is None:
            continue  # a new category is not a regression
        for metric, tolerance in tolerances.items():
            before, after = recorded.get(metric), entry.get(metric)
            if before is None or after is None or before <= 0:
                continue
            if after > before * (1 + tolerance):
                breaches.append(Breach(key=key, metric=metric, baseline=before, current=after))
    return breaches


def load_baseline(path: Path) -> dict[str, dict[str, float]]:
    if not path.is_file():
        return {}
    return json.loads(path.read_text(encoding="utf-8"))


def save_baseline(path: Path, entries: dict[str, dict[str, float]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    merged = load_baseline(path)
    merged.update(entries)
    path.write_text(json.dumps(merged, indent=2, sort_keys=True) + "\n", encoding="utf-8")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, epilog=EXIT_CODES)
    parser.add_argument("report", type=Path, help="a report written by bench_main")
    parser.add_argument("--baseline", type=Path, default=DEFAULT_BASELINE)
    parser.add_argument(
        "--save-baseline",
        action="store_true",
        help="record this run as the baseline; only after an accepted improvement",
    )
    parser.add_argument(
        "--expect",
        default=DEFAULT_WORKLOAD,
        choices=sorted(WORKLOADS),
        help="the declared workload the report must contain in full",
    )
    args = parser.parse_args(argv)

    if not args.report.is_file():
        sys.stderr.write(f"error: no such report: {args.report}\n")
        return 2

    report = parse_report(args.report.read_text(encoding="utf-8"), name=args.report.name)
    validation = validate_report(report, expected=resolve_workload(args.expect))
    if not validation.ok:
        sys.stderr.write(f"error: {args.report.name} is not gateable evidence:\n")
        for problem in validation.problems:
            if problem.fatal:
                sys.stderr.write(f"  {problem}\n")
        return 2

    entries = extract(report)
    if not entries:
        sys.stderr.write(
            f"error: no '{MEASURED_LIBRARY}' rows found in {args.report.name}. "
            "The report format and this parser have drifted apart.\n",
        )
        return 2

    if args.save_baseline:
        save_baseline(args.baseline, entries)
        print(f"baseline updated: {len(entries)} entries -> {args.baseline}")
        return 0

    baseline = load_baseline(args.baseline)
    if not baseline:
        sys.stderr.write(
            f"error: no baseline at {args.baseline}. "
            "Capture one with --save-baseline before gating.\n",
        )
        return 2

    covered = coverage(baseline, entries, report.name)
    breaches = compare(baseline, entries)

    print(
        f"compared {len(covered.compared)} of {len(covered.scope)} baseline entries for "
        f"{report.name} against {args.baseline.name} "
        f"({len(entries)} strata rows in the report)"
    )
    for key in covered.ungated:
        print(f"ungated (no baseline evidence yet): {key}")

    if breaches:
        sys.stderr.write("REGRESSION: fix or revert (docs/context/benchmarks.md)\n")
        for breach in breaches:
            sys.stderr.write(f"  {breach}\n")
        return 1

    if not covered.complete:
        sys.stderr.write(
            "MISSING EVIDENCE: the gate compared less than the scope it claims "
            "(docs/context/benchmarks.md)\n"
        )
        if not covered.scope:
            sys.stderr.write(
                f"  no baseline entries recorded for {report.name}; "
                "the baseline keys on the full report name, so a renamed report "
                "gates against nothing\n"
            )
        elif not covered.compared:
            sys.stderr.write(f"  none of the {len(covered.scope)} baseline entries was measured\n")
        for key in covered.missing_rows:
            sys.stderr.write(f"  missing row: {key}\n")
        for item in covered.missing_metrics:
            sys.stderr.write(f"  missing metric: {item}\n")
        return 1

    print("no regressions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

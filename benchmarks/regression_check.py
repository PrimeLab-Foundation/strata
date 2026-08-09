"""Compare a benchmark report against the recorded baseline.

Thresholds are the contract's, not this tool's invention: a touched category
whose **median or p95 is more than 2% slower**, or whose **RSS is more than 5%
larger**, is a breach (docs/context/benchmarks.md). The previous tool applied a
single 5% threshold, which was part of what made it useless.

It reads reports through `harness.parse_report`, the inverse of the writer's
`render_report`, so the two cannot drift. Baselines are keyed by the full
report name.
"""

from __future__ import annotations

import argparse
import json
import sys
from dataclasses import dataclass
from pathlib import Path

from benchmarks.harness import Report, baseline_key, parse_report

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_BASELINE = PROJECT_ROOT / "benchmarks" / "results" / "baseline.json"

MEDIAN_TOLERANCE = 0.02  # 2%
P95_TOLERANCE = 0.02  # 2%
RSS_TOLERANCE = 0.05  # 5%

MEASURED_LIBRARY = "strata"


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
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", type=Path, help="a report written by bench_main")
    parser.add_argument("--baseline", type=Path, default=DEFAULT_BASELINE)
    parser.add_argument(
        "--save-baseline",
        action="store_true",
        help="record this run as the baseline; only after an accepted improvement",
    )
    args = parser.parse_args(argv)

    if not args.report.is_file():
        sys.stderr.write(f"error: no such report: {args.report}\n")
        return 2

    report = parse_report(args.report.read_text(encoding="utf-8"), name=args.report.name)
    if report.has_errors:
        sys.stderr.write("error: report contains ERROR rows and cannot be gated on\n")
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

    breaches = compare(baseline, entries)
    compared = sum(1 for key in entries if key in baseline)
    print(f"compared {compared} of {len(entries)} entries against {args.baseline.name}")

    if breaches:
        sys.stderr.write("REGRESSION: fix or revert (docs/context/benchmarks.md)\n")
        for breach in breaches:
            sys.stderr.write(f"  {breach}\n")
        return 1

    print("no regressions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

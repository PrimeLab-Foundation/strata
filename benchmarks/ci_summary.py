"""Cross-platform standings summary over the fetched CI benchmark reports.

Reads every ``bench_results_*.md`` under ``docs/benchmarks/ci/`` (written by
`ci_fetch`) and writes ``docs/benchmarks/ci_summary.md``: strata's rank in
every row on each platform/architecture, the rows still behind, and a verdict
against the goal -- **strata #1 in every row on every supported platform and
architecture**.

What a rank means here: ranks and ratios are computed within one report --
one machine, one interleaved round -- which is the same-machine comparison
the benchmark contract allows. Absolute times are never compared across
reports (docs/context/convention.md, Platform supportability). Shared CI
runners are noisy, so the summary tracks the goal; the supportability
tripwire stays the CI gate, and headline standings still come only from the
quiet-machine protocol.

A report with ERROR rows is invalid (docs/context/benchmarks.md): its
platform is marked INVALID and excluded from the counts rather than published
as partial standings.

Exit codes: 0 written, 2 usage or report error.
"""

from __future__ import annotations

import argparse
import json
import sys
from dataclasses import dataclass
from pathlib import Path

from benchmarks.ci_fetch import RUN_INFO_NAME, platform_key
from benchmarks.harness import SECTIONS, Report, parse_report

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_REPORTS_DIR = PROJECT_ROOT / "docs" / "benchmarks" / "ci"
DEFAULT_OUTPUT = PROJECT_ROOT / "docs" / "benchmarks" / "ci_summary.md"

MEASURED_LIBRARY = "strata"


@dataclass(frozen=True)
class RowStanding:
    """strata's place in one (section, dataset) row of one platform's report."""

    section: str
    dataset: str
    rank: int  # 1 = fastest; an exact median tie resolves as #1
    libraries: int  # libraries measured in the row, strata included
    ratio: float  # strata median / best rival median; <= 1.0 at #1
    best_rival: str


def standings(report: Report) -> list[RowStanding]:
    """Rank strata in every comparable row (strata plus at least one rival)."""
    grouped: dict[tuple[str, str], dict[str, float]] = {}
    for row in report.measurements:
        if row.failed or row.median_ms is None:
            continue
        grouped.setdefault((row.section, row.dataset), {})[row.library] = row.median_ms

    ranked: list[RowStanding] = []
    for (section, dataset), medians in grouped.items():
        strata_ms = medians.get(MEASURED_LIBRARY)
        rivals = {name: ms for name, ms in medians.items() if name != MEASURED_LIBRARY}
        if strata_ms is None or not rivals:
            continue
        best_rival = min(rivals, key=rivals.get)  # type: ignore[arg-type]
        ranked.append(
            RowStanding(
                section=section,
                dataset=dataset,
                rank=1 + sum(ms < strata_ms for ms in rivals.values()),
                libraries=1 + len(rivals),
                ratio=strata_ms / rivals[best_rival],
                best_rival=best_rival,
            )
        )
    return ranked


def _environment_line(report: Report) -> str:
    env = report.environment
    parts = []
    if env.get("python"):
        parts.append(f"python {env['python']}")
    if env.get("repeats"):
        parts.append(f"repeats {env['repeats']}")
    if env.get("commit"):
        parts.append(f"commit {env['commit']}")
    return ", ".join(parts)


def render_summary(platforms: dict[str, Report], run_info: dict | None) -> str:
    keys = sorted(platforms)
    rows_by_key = {key: standings(platforms[key]) for key in keys}
    valid_keys = [key for key in keys if not platforms[key].has_errors]
    invalid_keys = [key for key in keys if platforms[key].has_errors]

    lines = ["# CI benchmark standings by platform and architecture", ""]
    lines.append("Machine-written by `make bench-ci`. Do not hand-edit.")
    lines.append("")
    lines.append("Goal: strata #1 in every row on every supported platform and architecture.")
    lines.append("")
    lines.append("Ranks and ratios are computed within each platform's own CI run -- the")
    lines.append("same-machine comparison the contract allows; absolute times are never")
    lines.append("compared across platforms (docs/context/convention.md, Platform")
    lines.append("supportability). Shared runners are noisy: this file tracks the goal, the")
    lines.append("supportability tripwire stays the CI gate, and headline standings come")
    lines.append("only from the quiet-machine protocol (docs/context/benchmarks.md).")
    lines.append("")

    if run_info:
        lines.append(
            f"- workflow: {run_info.get('workflow')} run {run_info.get('run_id')} "
            f"({run_info.get('event')}, conclusion: {run_info.get('conclusion')})"
        )
        lines.append(f"- branch/commit: {run_info.get('head_branch')} @ {run_info.get('head_sha')}")
        lines.append(f"- run date: {run_info.get('created_at')}")
        if run_info.get("url"):
            lines.append(f"- url: {run_info['url']}")
        lines.append("")

    section_keys = [
        key
        for key, _ in SECTIONS
        if any(row.section == key for rows in rows_by_key.values() for row in rows)
    ]

    lines.append("## Rows at #1, by category")
    lines.append("")
    lines.append('Cells are "#1 rows / comparable rows" within that platform\'s own report.')
    lines.append("")
    lines.append("| platform-arch | " + " | ".join(section_keys) + " | total |")
    lines.append("|" + "|".join(["---"] * (len(section_keys) + 2)) + "|")
    for key in keys:
        if key in invalid_keys:
            cells = ["-"] * len(section_keys) + ["INVALID"]
        else:
            rows = rows_by_key[key]
            cells = []
            for section in section_keys:
                in_section = [row for row in rows if row.section == section]
                if not in_section:
                    cells.append("-")
                    continue
                first = sum(row.rank == 1 for row in in_section)
                cells.append(f"{first}/{len(in_section)}")
            cells.append(f"{sum(row.rank == 1 for row in rows)}/{len(rows)}")
        lines.append(f"| {key} | " + " | ".join(cells) + " |")
    lines.append("")

    met = [
        key
        for key in valid_keys
        if rows_by_key[key] and all(row.rank == 1 for row in rows_by_key[key])
    ]
    behind_total = sum(row.rank > 1 for key in valid_keys for row in rows_by_key[key])
    verdict = f"Goal met on {len(met)}/{len(valid_keys)} platforms"
    if behind_total:
        verdict += f" -- {behind_total} row(s) to close"
    if invalid_keys:
        verdict += f"; {len(invalid_keys)} platform(s) INVALID"
    lines.append(f"**{verdict}.**")
    lines.append("")

    lines.append("## Rows behind, by platform")
    for key in keys:
        report = platforms[key]
        lines.append("")
        lines.append(f"### {key} ({_environment_line(report)})")
        lines.append("")
        if report.has_errors:
            lines.append("INVALID -- the run produced ERROR rows, so these standings are")
            lines.append("excluded from the counts (docs/context/benchmarks.md):")
            lines.append("")
            for row in report.measurements:
                if row.failed:
                    lines.append(f"- {row.section} | {row.dataset} | {row.library}: {row.error}")
            continue
        behind = [row for row in rows_by_key[key] if row.rank > 1]
        if not behind:
            lines.append("All rows #1.")
            continue
        lines.append("| section | dataset | rank | behind best | best rival |")
        lines.append("|" + "|".join(["---"] * 5) + "|")
        for row in behind:
            lines.append(
                f"| {row.section} | {row.dataset} | {row.rank}/{row.libraries} "
                f"| {row.ratio:.2f}x | {row.best_rival} |"
            )
    lines.append("")

    return "\n".join(lines)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reports-dir", type=Path, default=DEFAULT_REPORTS_DIR)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args(argv)

    paths = sorted(args.reports_dir.glob("bench_results_*.md"))
    if not paths:
        sys.stderr.write(
            f"error: no CI reports under {args.reports_dir}; run `make bench-ci` first\n"
        )
        return 2

    platforms: dict[str, Report] = {}
    for path in paths:
        report = parse_report(path.read_text(encoding="utf-8"), name=path.name)
        try:
            key = platform_key(report.environment)
        except ValueError as error:
            sys.stderr.write(f"error: {path.name}: {error}\n")
            return 2
        if key in platforms:
            sys.stderr.write(
                f"error: two reports claim {key}: {platforms[key].name} and {path.name}\n"
            )
            return 2
        platforms[key] = report

    run_info = None
    info_path = args.reports_dir / RUN_INFO_NAME
    if info_path.is_file():
        run_info = json.loads(info_path.read_text(encoding="utf-8"))

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(render_summary(platforms, run_info), encoding="utf-8", newline="\n")

    all_rows = [row for report in platforms.values() for row in standings(report)]
    first = sum(row.rank == 1 for row in all_rows)
    print(f"wrote {args.output} ({len(platforms)} platform(s), {first}/{len(all_rows)} rows at #1)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

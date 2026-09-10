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

**The denominator is declared, not discovered.** Both halves of "N/M rows at
#1 on K/L platforms" come from `harness.WORKLOADS` and `harness.CI_PLATFORMS`
-- the rows and legs the benchmark job is defined to produce -- so a dataset
that did not run, a platform that never uploaded, or a `nan` median subtracts
from the objective instead of shrinking it. The reviewed summary derived both
from whatever it was handed, and read "Goal met on 1/1 platforms" over a
single surviving row (docs/performance/ci-review-2026-09-07.md, finding 2).
Each report's own commit and platform are cross-checked against
``run_info.json``; a report that predates that file keeps its standings under
an explicit *unverified* provenance rather than a provenance it does not have.

**Only declared platforms count.** A report for a leg outside
`harness.CI_PLATFORMS` -- ``windows-arm64``, say, which `platform_key` can
name perfectly well -- is rendered in its own section with its own
denominator and contributes to neither the verdict nor the row tallies. The
reviewed summary unioned it into the counts, so five declared legs plus one
stray read ``Goal met on 6/5 platforms`` and ``162/162 rows at #1``, and a
stray could fill in for a leg that never uploaded
(build/evidence/T2-REVIEW/REVIEW.md, defect 1).

Exit codes:

* 0 -- written from complete evidence (or ``--allow-incomplete``).
* 1 -- written, but the evidence is missing, invalid or misattributed.
* 2 -- usage or report error: no reports, an unnameable platform, two reports
  claiming one leg.
"""

from __future__ import annotations

import argparse
import json
import sys
from dataclasses import dataclass
from pathlib import Path

from benchmarks.ci_fetch import RUN_INFO_NAME, platform_key

# The file name is the leg a fetched report is attributed to.
REPORT_PREFIX = "bench_results_"
from benchmarks.harness import (
    CI_PLATFORMS,
    MEASURED_LIBRARY,
    SECTIONS,
    WORKLOADS,
    Report,
    Validation,
    read_report,
    resolve_workload,
    validate_report,
)

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_REPORTS_DIR = PROJECT_ROOT / "docs" / "benchmarks" / "ci"
DEFAULT_OUTPUT = PROJECT_ROOT / "docs" / "benchmarks" / "ci_summary.md"

DEFAULT_WORKLOAD = "ci"

EXIT_CODES = (
    "Exit codes: 0 the summary was written from complete evidence (or "
    "--allow-incomplete); 1 it was written but the evidence is missing, invalid "
    "or misattributed; 2 usage or report error (no reports, an unnameable "
    "platform, two reports claiming one leg)."
)

# Statuses that still carry usable standings. "unverified" is a legacy fetch
# with no run manifest beside it: its numbers are real, its provenance is not
# claimed.
USABLE_STATUSES = ("complete", "unverified")


@dataclass(frozen=True)
class SummaryResult:
    """The rendered summary and what it is entitled to claim."""

    text: str
    complete: bool  # every declared platform valid and carrying every declared row
    verified: bool  # no report contradicts the run manifest beside it
    platforms: int  # declared platforms whose rows were counted
    first: int  # counted rows at #1, declared platforms and declared rows only
    rows: int  # declared rows across those platforms
    undeclared: tuple[str, ...] = ()  # legs outside CI_PLATFORMS, counted nowhere


@dataclass(frozen=True)
class RowStanding:
    """strata's place in one (section, dataset) row of one platform's report."""

    section: str
    dataset: str
    rank: int  # 1 = fastest; an exact median tie resolves as #1
    libraries: int  # libraries measured in the row, strata included
    ratio: float  # strata median / best rival median; <= 1.0 at #1
    best_rival: str
    tied: bool = False  # strata's median equals the best rival's at the report's precision


@dataclass(frozen=True)
class PlatformEvidence:
    """One expected platform: its report, its verdict, and what it is worth."""

    key: str
    report: Report | None
    validation: Validation | None
    rows: tuple[RowStanding, ...]
    provenance: str
    provenance_ok: bool
    declared: bool = True  # part of the declared matrix; only these are counted

    @property
    def status(self) -> str:
        if self.report is None or self.validation is None:
            return "MISSING"
        if not self.validation.valid:
            return "INVALID"
        if not self.provenance_ok:
            return "MISMATCH"
        if self.validation.expected is not None and not self.validation.complete:
            return "INCOMPLETE"
        if self.validation.expected is None or self.provenance.startswith("unverified"):
            return "unverified"
        return "complete"

    @property
    def usable(self) -> bool:
        """Whether this platform's rows may be counted toward the goal."""
        return self.declared and self.status in USABLE_STATUSES

    @property
    def declared_rows(self) -> tuple[RowStanding, ...]:
        """This platform's standings, restricted to the declared workload.

        `standings()` ranks every comparable row a report contains. A row
        outside the workload is extra evidence, not part of the objective, and
        counting it pushed a numerator past its own denominator: a report with
        26 declared rows plus one extra rendered ``6/5`` for its category and
        ``27/27`` for a platform the Evidence section called INCOMPLETE
        (build/evidence/T2-REVIEW/REVIEW.md, defect 2).
        """
        expected = self.validation.expected if self.validation is not None else None
        if expected is None:
            return self.rows
        wanted = set(expected)
        return tuple(row for row in self.rows if (row.section, row.dataset) in wanted)


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
        if rivals[best_rival] <= 0:
            continue  # an unmeasurable rival median; validation reports it
        ranked.append(
            RowStanding(
                section=section,
                dataset=dataset,
                rank=1 + sum(ms < strata_ms for ms in rivals.values()),
                libraries=1 + len(rivals),
                ratio=strata_ms / rivals[best_rival],
                best_rival=best_rival,
                # Disclose display ties even when a validated companion's
                # full precision resolves the ranking behind the Markdown.
                tied=f"{strata_ms:.3f}" == f"{rivals[best_rival]:.3f}",
            )
        )
    return ranked


def check_provenance(key: str, report: Report, run_info: dict | None) -> tuple[str, bool]:
    """Cross-check one report against the run manifest it was fetched with.

    Returns (description, ok). A missing manifest is *not* a mismatch: legacy
    fetches predate `run_info.json` and keep their standings, described as
    unverified. A manifest that names a different commit, or does not name this
    platform at all, is a misattribution and fails.
    """
    commit = (report.environment.get("commit") or "").strip()
    if report.provenance:
        build = report.provenance.get("extension", {}).get("build")
        source = build.get("source") if build else None
        if not source or not source.get("commit"):
            return "unverified: measured binary has no build source identity", True
        if source.get("dirty") is not False:
            return "MISMATCH: measured binary was built from dirty or unknown source", False
    # `key` comes from the report's own environment; the file name is what
    # attributes it to a leg, so a macOS report saved as the linux-arm64 file
    # is the misattribution this check exists to catch.
    stem = Path(report.name).stem if report.name else ""
    if stem.startswith(REPORT_PREFIX) and stem[len(REPORT_PREFIX) :] != key:
        return f"MISMATCH: {report.name} holds a {key} report", False
    if not run_info:
        return "unverified: no run_info.json beside the reports", True
    head = str(run_info.get("head_sha") or "").strip()
    run_id = run_info.get("run_id")
    reports = run_info.get("reports")
    mapped = isinstance(reports, dict) and bool(reports)
    if mapped and key not in reports:
        return f"MISMATCH: run {run_id} does not list a {key} report", False
    if not commit or commit == "unknown":
        return f"unverified: the report records no commit (run {run_id})", True
    if not head:
        return f"unverified: run {run_id} records no head_sha", True
    if not (head.startswith(commit) or commit.startswith(head)):
        return f"MISMATCH: report commit {commit} is not run {run_id}'s {head[:12]}", False
    if not mapped:
        return (
            f"verified against run {run_id} ({head[:7]}); the manifest records no report map",
            True,
        )
    return f"verified against run {run_id} ({head[:7]})", True


def collect(
    platforms: dict[str, Report],
    run_info: dict | None,
    *,
    expected_rows: tuple[tuple[str, str], ...] | None,
    expected_platforms: tuple[str, ...],
) -> list[PlatformEvidence]:
    """One `PlatformEvidence` per declared platform, then any undeclared report.

    Declared platforms come first and in order; a report for a leg the matrix
    does not declare follows, flagged `declared=False` so nothing counts it.
    """
    wanted = set(expected_platforms)
    declared_keys = sorted(wanted)
    extra_keys = sorted(set(platforms) - wanted)
    evidence: list[PlatformEvidence] = []
    for key in (*declared_keys, *extra_keys):
        declared = key in wanted
        report = platforms.get(key)
        if report is None:
            evidence.append(
                PlatformEvidence(
                    key=key,
                    report=None,
                    validation=None,
                    rows=(),
                    provenance="no report fetched",
                    provenance_ok=True,
                    declared=declared,
                )
            )
            continue
        validation = validate_report(report, expected=expected_rows)
        provenance, provenance_ok = check_provenance(key, report, run_info)
        evidence.append(
            PlatformEvidence(
                key=key,
                report=report,
                validation=validation,
                rows=tuple(standings(report)),
                provenance=provenance,
                provenance_ok=provenance_ok,
                declared=declared,
            )
        )
    return evidence


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


def _section_cells(
    evidence: PlatformEvidence,
    section_keys: list[str],
    expected_rows: tuple[tuple[str, str], ...] | None,
) -> list[str]:
    counted = evidence.declared_rows
    cells = []
    for section in section_keys:
        in_section = [row for row in counted if row.section == section]
        declared = (
            [row for row in expected_rows if row[0] == section] if expected_rows is not None else []
        )
        total = len(declared) if expected_rows is not None else len(in_section)
        if not total:
            cells.append("-")
            continue
        cells.append(f"{sum(row.rank == 1 for row in in_section)}/{total}")
    first = sum(row.rank == 1 for row in counted)
    total_rows = len(expected_rows) if expected_rows is not None else len(counted)
    cells.append(f"{first}/{total_rows}")
    return cells


def render_summary(
    platforms: dict[str, Report],
    run_info: dict | None,
    *,
    expected_rows: tuple[tuple[str, str], ...] | None = None,
    expected_platforms: tuple[str, ...] = CI_PLATFORMS,
) -> SummaryResult:
    """The summary text, and what the evidence behind it is entitled to claim."""
    evidence = collect(
        platforms,
        run_info,
        expected_rows=expected_rows,
        expected_platforms=expected_platforms,
    )
    declared = [item for item in evidence if item.declared]
    undeclared = [item for item in evidence if not item.declared]

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

    present_sections = {row.section for item in evidence for row in item.rows}
    declared_sections = {section for section, _ in (expected_rows or ())}
    section_keys = [
        key for key, _ in SECTIONS if key in present_sections or key in declared_sections
    ]

    lines.append("## Rows at #1, by category")
    lines.append("")
    lines.append('Cells are "#1 rows / declared rows" within that platform\'s own report.')
    if undeclared:
        lines.append("Declared platforms only; the rest are below, under their own heading.")
    lines.append("")
    lines.append("| platform-arch | " + " | ".join(section_keys) + " | total |")
    lines.append("|" + "|".join(["---"] * (len(section_keys) + 2)) + "|")
    for item in declared:
        if item.status in ("MISSING", "INVALID"):
            cells = ["-"] * len(section_keys) + [item.status]
        else:
            cells = _section_cells(item, section_keys, expected_rows)
        lines.append(f"| {item.key} | " + " | ".join(cells) + " |")
    lines.append("")

    met = [item.key for item in declared if item.usable and item.declared_rows and _all_first(item)]
    behind_total = sum(
        row.rank > 1 for item in declared if item.usable for row in item.declared_rows
    )
    tied_total = sum(
        row.rank == 1 and row.tied for item in declared if item.usable for row in item.declared_rows
    )
    verdict = f"Goal met on {len(met)}/{len(expected_platforms)} platforms"
    if behind_total:
        verdict += f" -- {behind_total} row(s) to close"
    if tied_total:
        verdict += f"; {tied_total} #1 row(s) are ties at the report's precision"
    for status, label in (
        ("MISSING", "MISSING"),
        ("INVALID", "INVALID"),
        ("INCOMPLETE", "INCOMPLETE"),
        ("MISMATCH", "MISATTRIBUTED"),
    ):
        count = sum(item.status == status for item in declared)
        if count:
            verdict += f"; {count} platform(s) {label}"
    unverified = sum(item.status == "unverified" for item in declared)
    if unverified:
        verdict += f"; {unverified} platform(s) unverified"
    if undeclared:
        verdict += f"; {len(undeclared)} undeclared leg(s) not counted"
    lines.append(f"**{verdict}.**")
    lines.append("")

    complete = _render_evidence(lines, declared, expected_rows, expected_platforms)
    # Misattribution is refused wherever it sits: a stray report contradicting
    # the manifest beside it is a disclosure defect even off the matrix.
    verified = all(item.status != "MISMATCH" for item in evidence)

    lines.append("## Rows behind, by platform")
    for item in declared:
        lines.append("")
        _render_platform_detail(lines, item)
    lines.append("")

    if undeclared:
        _render_undeclared(lines, undeclared, section_keys, expected_rows)

    counted = [item for item in declared if item.usable]
    return SummaryResult(
        text="\n".join(lines),
        complete=complete,
        verified=verified,
        platforms=len(counted),
        first=sum(row.rank == 1 for item in counted for row in item.declared_rows),
        rows=sum(
            len(expected_rows) if expected_rows is not None else len(item.declared_rows)
            for item in counted
        ),
        undeclared=tuple(item.key for item in undeclared),
    )


def _render_platform_detail(lines: list[str], item: PlatformEvidence) -> None:
    """One platform's heading, disclosures and behind-rows table."""
    if item.report is None:
        lines.append(f"### {item.key} (no report)")
        lines.append("")
        lines.append("MISSING -- this platform is part of the declared benchmark matrix and")
        lines.append("uploaded no report, so its rows are counted as not met.")
        return
    lines.append(f"### {item.key} ({_environment_line(item.report)})")
    lines.append("")
    if item.status == "INVALID":
        lines.append("INVALID -- the run produced ERROR rows or unusable numbers, so these")
        lines.append("standings are excluded from the counts (docs/context/benchmarks.md):")
        lines.append("")
        assert item.validation is not None
        for problem in item.validation.problems:
            if problem.fatal and problem.kind != "missing":
                lines.append(f"- {problem}")
        return
    if item.status == "MISMATCH":
        lines.append(f"MISATTRIBUTED -- {item.provenance}; these standings are excluded")
        lines.append("from the counts until the report and the run manifest agree.")
        lines.append("")
    if item.validation is not None and item.validation.of("missing"):
        lines.append("Declared rows this report does not contain, counted as not met:")
        lines.append("")
        for problem in item.validation.of("missing"):
            lines.append(f"- {problem.where}")
        lines.append("")
    rows = item.declared_rows
    behind = [row for row in rows if row.rank > 1]
    ties = [row for row in rows if row.rank == 1 and row.tied]
    if not behind:
        lines.append("All rows #1.")
    else:
        lines.append("| section | dataset | rank | behind best | best rival |")
        lines.append("|" + "|".join(["---"] * 5) + "|")
        for row in behind:
            lines.append(
                f"| {row.section} | {row.dataset} | {row.rank}/{row.libraries} "
                f"| {row.ratio:.2f}x | {row.best_rival} |"
            )
    if ties:
        lines.append("")
        lines.append(
            "Ties at the report's displayed precision among #1 rows (raw samples determine rank when available):"
        )
        for row in ties:
            lines.append(f"- {row.section} | {row.dataset} | with {row.best_rival}")


def _render_undeclared(
    lines: list[str],
    items: list[PlatformEvidence],
    section_keys: list[str],
    expected_rows: tuple[tuple[str, str], ...] | None,
) -> None:
    """Legs off the declared matrix, with their own denominator and no vote.

    An extra leg is extra evidence, never a substitute for a declared one: it
    cannot raise the platform count above the matrix, and it cannot stand in
    for a platform that uploaded nothing (build/evidence/T2-REVIEW/REVIEW.md,
    defect 1).
    """
    lines.append("## Reports outside the declared matrix")
    lines.append("")
    lines.append("These legs uploaded a report without being part of the declared platform")
    lines.append("list (`harness.CI_PLATFORMS`). Their cells carry their own denominator and")
    lines.append("count toward neither the verdict nor the tallies above.")
    lines.append("")
    header = "| platform-arch | " + " | ".join(section_keys) + " | total | status | provenance |"
    lines.append(header)
    lines.append("|" + "|".join(["---"] * (len(section_keys) + 4)) + "|")
    for item in items:
        if item.status in ("MISSING", "INVALID"):
            cells = ["-"] * len(section_keys) + [item.status]
        else:
            cells = _section_cells(item, section_keys, expected_rows)
        body = " | ".join(cells)
        lines.append(f"| {item.key} | {body} | {item.status} | {item.provenance} |")
    for item in items:
        lines.append("")
        _render_platform_detail(lines, item)
    lines.append("")


def _all_first(item: PlatformEvidence) -> bool:
    """Every declared row of this platform measured, comparable and #1."""
    rows = item.declared_rows
    if item.validation is not None and item.validation.expected is not None:
        if len(rows) != len(item.validation.expected):
            return False
    return all(row.rank == 1 for row in rows)


def _render_evidence(
    lines: list[str],
    evidence: list[PlatformEvidence],
    expected_rows: tuple[tuple[str, str], ...] | None,
    expected_platforms: tuple[str, ...],
) -> bool:
    """The completeness section over the declared platforms.

    Returns True when nothing declared is missing. `evidence` carries the
    declared platforms only; undeclared legs have their own section and no
    vote here.
    """
    lines.append("## Evidence")
    lines.append("")
    lines.append("What the counts above are made of -- the declared platforms, and only")
    lines.append("those. A platform contributes standings only when its report is valid and")
    lines.append("contains every declared row; the goal cannot be met on evidence that is")
    lines.append("absent (docs/context/benchmarks.md).")
    lines.append("")
    lines.append(
        "| platform-arch | status | declared rows measured | comparable rows | provenance |"
    )
    lines.append("|" + "|".join(["---"] * 5) + "|")
    for item in evidence:
        if item.validation is None or expected_rows is None:
            measured = "-"
        else:
            measured = f"{len(item.validation.measured)}/{len(expected_rows)}"
        comparable = "-" if item.report is None else str(len(item.rows))
        lines.append(
            f"| {item.key} | {item.status} | {measured} | {comparable} | {item.provenance} |"
        )
    lines.append("")

    problems = [
        (item.key, problem)
        for item in evidence
        if item.validation is not None
        for problem in item.validation.problems
        if problem.kind != "missing"  # named per platform in the detail sections
    ]
    if problems:
        lines.append("Reported problems:")
        lines.append("")
        for key, problem in problems:
            lines.append(f"- {key}: {problem}")
        lines.append("")

    incomplete = [item for item in evidence if item.status not in USABLE_STATUSES]
    if incomplete:
        named = ", ".join(f"{item.key} ({item.status})" for item in incomplete)
        lines.append(
            f"Evidence is incomplete: {named}. A complete-goal claim needs all "
            f"{len(expected_platforms)} declared platforms valid and complete."
        )
        lines.append("")
        return False
    # "Complete" is a claim about coverage, not about provenance: a legacy set
    # with no run manifest beside it is complete and unverified at once, and
    # the closing sentence has to say both (build/evidence/T2-REVIEW/REVIEW.md,
    # defect 4).
    unverified = sum(item.provenance.startswith("unverified") for item in evidence)
    closing = f"All {len(expected_platforms)} declared platforms reported valid, complete evidence"
    if unverified:
        closing += f"; {unverified} of them with unverified provenance"
    lines.append(f"{closing}.")
    lines.append("")
    return True


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, epilog=EXIT_CODES)
    parser.add_argument("--reports-dir", type=Path, default=DEFAULT_REPORTS_DIR)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument(
        "--expect",
        default=DEFAULT_WORKLOAD,
        choices=sorted(WORKLOADS),
        help="the declared workload each platform's report must contain in full",
    )
    parser.add_argument(
        "--expect-platforms",
        default=",".join(CI_PLATFORMS),
        help="comma-separated platform-arch keys the run is expected to cover",
    )
    parser.add_argument(
        "--allow-incomplete",
        action="store_true",
        help="write the summary and exit 0 even though evidence is missing "
        "(for a deliberately scoped set; the summary still says so)",
    )
    args = parser.parse_args(argv)

    expected_platforms = tuple(
        key.strip() for key in args.expect_platforms.split(",") if key.strip()
    )

    paths = sorted(args.reports_dir.glob("bench_results_*.md"))
    if not paths:
        sys.stderr.write(
            f"error: no CI reports under {args.reports_dir}; run `make bench-ci` first\n"
        )
        return 2

    platforms: dict[str, Report] = {}
    for path in paths:
        report = read_report(path)
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

    result = render_summary(
        platforms,
        run_info,
        expected_rows=resolve_workload(args.expect),
        expected_platforms=expected_platforms,
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(result.text, encoding="utf-8", newline="\n")

    # Counted, not collected: a platform whose evidence is invalid or
    # misattributed contributes no rows to this line either.
    print(
        f"wrote {args.output} ({result.platforms} platform(s), "
        f"{result.first}/{result.rows} rows at #1)"
    )
    if result.undeclared:
        # Named, never added: an extra leg is not one of the platforms the goal
        # is measured over.
        print(
            f"note: {len(result.undeclared)} report(s) outside the declared matrix, "
            f"counted nowhere: {', '.join(result.undeclared)}"
        )
    if not result.verified:
        sys.stderr.write(
            "error: a report contradicts the run manifest beside it; see the summary's "
            "Evidence section. Misattributed evidence is never accepted.\n"
        )
        return 1
    if not result.complete:
        sys.stderr.write(
            "error: the summary was written from incomplete evidence; see its Evidence "
            "section. Pass --allow-incomplete for a deliberately scoped set.\n"
        )
        return 0 if args.allow_incomplete else 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

"""Contract tests for the cross-platform CI standings summary.

Pins docs/benchmarking/SKILL.md (CI standings by platform and architecture):
ranks are computed within one platform's report only; an exact median tie
resolves as #1; a report with ERROR rows is INVALID and excluded from the
counts rather than published as partial standings
(docs/context/benchmarks.md); the verdict tracks the goal of #1 in every row
on every platform/architecture. main() is exercised end-to-end.

Since the 2026-09-07 review it also pins where the denominators come from:
the declared workload and the declared platform list, never the reports that
happened to arrive. A missing leg, a missing row, a `nan`, or a run manifest
naming a different commit each keeps the goal from being claimed, and says so
in the summary rather than shrinking the objective
(docs/performance/ci-review-2026-09-07.md, finding 2).

The follow-up review added the other direction: evidence the matrix never
declared cannot inflate the verdict either. An undeclared leg gets its own
section and its own denominator, and a row outside the declared workload
never counts toward a declared one (build/evidence/T2-REVIEW/REVIEW.md,
defects 1 and 2).
"""

import json
from pathlib import Path

from benchmarks.ci_summary import main
from benchmarks.harness import (
    CI_PLATFORMS,
    Measurement,
    Report,
    render_report,
    workload_rows,
)

LINUX = "Linux-6.8.0-1014-azure-x86_64-with-glibc2.39"
MACOS = "macOS-26.3-arm64-arm-64bit-Mach-O"

# The commit the `complete_report` fixture stamps its reports with.
HEAD_SHA = "16b0a58fe1ed0da3d139b64f59d66cea9822f4a3"

ONE_LINUX = "linux-x86_64"
ONE_MACOS = "macos-arm64"


def write_report(
    directory: Path,
    filename: str,
    platform: str,
    machine: str,
    rows: dict[tuple[str, str], dict[str, float]],
    *,
    errors: list[Measurement] | None = None,
    commit: str = "16b0a58",
) -> None:
    report = Report(
        name="ci-probe",
        environment={
            "commit": commit,
            "python": "3.12.6",
            "platform": platform,
            "machine": machine,
            "repeats": "10",
            "warmup": "2",
        },
    )
    for (section, dataset), medians in rows.items():
        for library, median in medians.items():
            report.measurements.append(
                Measurement(
                    section=section,
                    dataset=dataset,
                    library=library,
                    min_ms=median,
                    median_ms=median,
                    p95_ms=median,
                    rss_mb=10.0,
                )
            )
    report.measurements.extend(errors or [])
    directory.mkdir(parents=True, exist_ok=True)
    (directory / filename).write_text(render_report(report), encoding="utf-8")


def place(directory: Path, filename: str, report: Report) -> None:
    directory.mkdir(parents=True, exist_ok=True)
    (directory / filename).write_text(render_report(report), encoding="utf-8")


def run_summary(tmp_path: Path, *extra: str) -> tuple[int, str]:
    output = tmp_path / "ci_summary.md"
    code = main(["--reports-dir", str(tmp_path / "ci"), "--output", str(output), *extra])
    return code, output.read_text(encoding="utf-8") if output.is_file() else ""


def scoped(*platforms: str) -> list[str]:
    """A deliberately scoped run: no declared workload, named platforms only."""
    return ["--expect", "none", "--expect-platforms", ",".join(platforms)]


# ---------------------------------------------------------------------------
# Ranking, ties and ERROR rows -- unchanged rules, on scoped fixtures
# ---------------------------------------------------------------------------


def test_ranks_verdict_and_behind_rows(tmp_path, capsys):
    reports = tmp_path / "ci"
    write_report(
        reports,
        "bench_results_macos-arm64.md",
        MACOS,
        "arm64",
        {("loads", "users.json"): {"strata": 1.0, "orjson": 1.2, "msgspec": 1.4}},
    )
    write_report(
        reports,
        "bench_results_linux-x86_64.md",
        LINUX,
        "x86_64",
        {
            ("loads", "users.json"): {"strata": 1.2, "orjson": 1.0, "msgspec": 1.1},
            ("query", "users.json $[*].id"): {"strata": 0.1, "jmespath": 0.4},
        },
    )

    code, text = run_summary(tmp_path, *scoped("linux-x86_64", "macos-arm64"))
    assert code == 0

    # Overview: per-category "#1 rows / declared rows" cells per platform.
    assert "| macos-arm64 | 1/1 | - | 1/1 |" in text
    assert "| linux-x86_64 | 0/1 | 1/1 | 1/2 |" in text

    # The behind table names the row, the rank among the row's libraries, the
    # gap to the fastest rival, and that rival.
    assert "| loads | users.json | 3/3 | 1.20x | orjson |" in text
    assert "**Goal met on 1/2 platforms -- 1 row(s) to close" in text
    assert "All rows #1." in text  # the macos-arm64 detail section

    out = capsys.readouterr().out
    assert "2 platform(s), 2/3 rows at #1" in out


def test_exact_median_tie_counts_as_first(tmp_path):
    write_report(
        tmp_path / "ci",
        "bench_results_macos-arm64.md",
        MACOS,
        "arm64",
        {("dumps", "users.json"): {"strata": 1.0, "orjson": 1.0}},
    )
    code, text = run_summary(tmp_path, *scoped(ONE_MACOS))
    assert code == 0
    # The rank rule counts the tie as #1 and the summary says which rows are
    # ties, so a rounded tie never reads as a demonstrated lead.
    assert "**Goal met on 1/1 platforms; 1 #1 row(s) are ties at the report's precision" in text
    assert "All rows #1." in text
    assert "Ties at the report's precision, counted as #1 by the rank rule:" in text
    assert "- dumps | users.json | with orjson" in text


def test_error_rows_invalidate_the_platform(tmp_path):
    reports = tmp_path / "ci"
    write_report(
        reports,
        "bench_results_macos-arm64.md",
        MACOS,
        "arm64",
        {("loads", "users.json"): {"strata": 1.0, "orjson": 1.2}},
    )
    write_report(
        reports,
        "bench_results_linux-x86_64.md",
        LINUX,
        "x86_64",
        {("loads", "users.json"): {"strata": 1.0, "orjson": 1.2}},
        errors=[
            Measurement(section="loads", dataset="users.json", library="ujson", error="ImportError")
        ],
    )
    code, text = run_summary(tmp_path, *scoped("linux-x86_64", "macos-arm64"))
    # An invalid platform is evidence the goal needs and does not have.
    assert code == 1
    assert "| linux-x86_64 | - | INVALID |" in text
    assert "loads|users.json|ujson: ERROR (ImportError)" in text
    # It no longer shrinks the denominator: the goal is over both platforms.
    assert "**Goal met on 1/2 platforms; 1 platform(s) INVALID" in text


def test_run_info_provenance_is_reported(tmp_path):
    reports = tmp_path / "ci"
    write_report(
        reports,
        "bench_results_macos-arm64.md",
        MACOS,
        "arm64",
        {("loads", "users.json"): {"strata": 1.0, "orjson": 1.2}},
    )
    (reports / "run_info.json").write_text(
        json.dumps(
            {
                "workflow": "Benchmarks",
                "run_id": 31392004866,
                "url": "https://example.invalid/actions/runs/31392004866",
                "event": "workflow_dispatch",
                "conclusion": "success",
                "head_branch": "main",
                "head_sha": "16b0a58fe1ed0da3d139b64f59d66cea9822f4a3",
                "created_at": "2026-08-10T13:15:50Z",
            }
        ),
        encoding="utf-8",
    )
    code, text = run_summary(tmp_path, *scoped(ONE_MACOS))
    assert code == 0
    assert "Benchmarks run 31392004866" in text
    assert "main @ 16b0a58fe1ed0da3d139b64f59d66cea9822f4a3" in text
    assert "verified against run 31392004866" in text


def test_no_reports_is_a_usage_error(tmp_path, capsys):
    code = main(
        ["--reports-dir", str(tmp_path / "ci"), "--output", str(tmp_path / "ci_summary.md")]
    )
    assert code == 2
    assert "make bench-ci" in capsys.readouterr().err


def test_two_reports_claiming_one_platform_is_an_error(tmp_path, capsys):
    reports = tmp_path / "ci"
    rows = {("loads", "users.json"): {"strata": 1.0, "orjson": 1.2}}
    write_report(reports, "bench_results_macos-arm64.md", MACOS, "arm64", rows)
    write_report(reports, "bench_results_macos-arm64-again.md", MACOS, "arm64", rows)
    code, _ = run_summary(tmp_path)
    assert code == 2
    assert "two reports claim macos-arm64" in capsys.readouterr().err


# ---------------------------------------------------------------------------
# Declared evidence: the denominators the goal is measured against
# ---------------------------------------------------------------------------


def test_complete_evidence_claims_the_goal(tmp_path, complete_report):
    reports = tmp_path / "ci"
    place(reports, "bench_results_linux-x86_64.md", complete_report())
    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 0
    assert "| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |" in text
    assert "**Goal met on 1/1 platforms" in text
    # No run_info.json beside it: complete coverage, provenance not claimed.
    assert (
        "All 1 declared platforms reported valid, complete evidence; "
        "1 of them with unverified provenance." in text
    )


def test_a_missing_platform_cannot_be_a_met_goal(tmp_path, capsys, complete_report):
    """One passing platform used to read "Goal met on 1/1"; four are absent."""
    reports = tmp_path / "ci"
    place(reports, "bench_results_linux-x86_64.md", complete_report())
    code, text = run_summary(tmp_path)
    assert code == 1
    assert "**Goal met on 1/5 platforms; 4 platform(s) MISSING" in text
    assert "| linux-arm64 | MISSING | - | - | no report fetched |" in text
    assert "Evidence is incomplete: linux-arm64 (MISSING)" in text
    assert "incomplete evidence" in capsys.readouterr().err


def test_allow_incomplete_exits_zero_but_still_discloses(tmp_path, complete_report):
    reports = tmp_path / "ci"
    place(reports, "bench_results_linux-x86_64.md", complete_report())
    code, text = run_summary(tmp_path, "--allow-incomplete")
    assert code == 0
    assert "4 platform(s) MISSING" in text


def test_one_missing_dataset_row_is_incomplete(tmp_path, complete_report):
    reports = tmp_path / "ci"
    place(
        reports,
        "bench_results_linux-x86_64.md",
        complete_report(drop_strata=(("dumps", "mixed.json"),)),
    )
    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 1
    assert "| linux-x86_64 | INCOMPLETE | 26/27 |" in text
    assert "Declared rows this report does not contain, counted as not met:" in text
    assert "- dumps|mixed.json" in text
    # 26 of 27 declared rows are #1; the goal is not met on the platform.
    assert "**Goal met on 0/1 platforms" in text


def test_a_nan_median_invalidates_rather_than_ranks(tmp_path, complete_report):
    """The reviewed summary gave a `nan` row rank 1."""
    report = complete_report(drop_strata=(("loads", "flat.json"),))
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="flat.json",
            library="strata",
            min_ms=float("nan"),
            median_ms=float("nan"),
            p95_ms=float("nan"),
            rss_mb=10.0,
        )
    )
    place(tmp_path / "ci", "bench_results_linux-x86_64.md", report)
    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 1
    assert "| linux-x86_64 | - | - | - | - | - | - | - | INVALID |" in text
    assert "median_ms is not finite" in text


def test_a_duplicated_row_invalidates(tmp_path, complete_report):
    report = complete_report()
    report.measurements.append(report.measurements[0])
    place(tmp_path / "ci", "bench_results_linux-x86_64.md", report)
    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 1
    assert "measured more than once" in text


def test_a_mismatched_sha_is_misattributed(tmp_path, complete_report):
    reports = tmp_path / "ci"
    place(reports, "bench_results_linux-x86_64.md", complete_report(commit="deadbee"))
    (reports / "run_info.json").write_text(
        json.dumps(
            {
                "workflow": "Benchmarks",
                "run_id": 34064174240,
                "head_sha": "79fa3df53ccf9b9486f8c5f2448d7e62e924966d",
                "reports": {"linux-x86_64": "benchmark-linux-x86_64/bench_ci_linux-x86_64.md"},
            }
        ),
        encoding="utf-8",
    )
    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 1
    assert "MISMATCH: report commit deadbee is not run 34064174240's 79fa3df53ccf" in text
    assert "**Goal met on 0/1 platforms" in text
    assert "MISATTRIBUTED" in text


def test_a_platform_absent_from_the_manifest_is_misattributed(tmp_path, complete_report):
    reports = tmp_path / "ci"
    place(reports, "bench_results_linux-x86_64.md", complete_report())
    (reports / "run_info.json").write_text(
        json.dumps(
            {
                "run_id": 7,
                "head_sha": HEAD_SHA,
                "reports": {"macos-arm64": "benchmark-macos-arm64/bench_ci_macos-arm64.md"},
            }
        ),
        encoding="utf-8",
    )
    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 1
    assert "MISMATCH: run 7 does not list a linux-x86_64 report" in text


def test_a_legacy_report_keeps_its_standings_as_unverified(tmp_path, complete_report):
    """No run_info.json: real numbers, no invented provenance."""
    place(tmp_path / "ci", "bench_results_linux-x86_64.md", complete_report())
    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 0
    assert "unverified: no run_info.json beside the reports" in text
    assert "**Goal met on 1/1 platforms" in text


def test_an_uncomparable_row_is_not_counted_as_first(tmp_path, complete_report):
    """A row strata alone measured cannot be ranked -- and is not a win."""
    report = complete_report()
    report.measurements = [
        m
        for m in report.measurements
        if not (m.section == "dumps" and m.dataset == "mixed.json" and m.library != "strata")
    ]
    place(tmp_path / "ci", "bench_results_linux-x86_64.md", report)
    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 0  # valid and complete: strata measured every declared row
    assert "| 26/27 |" in text  # the declared denominator, not the comparable one
    assert "no rival measured beside strata" in text
    assert "**Goal met on 0/1 platforms" in text


def test_the_declared_workload_is_the_twenty_seven_ci_rows():
    rows = workload_rows()
    assert len(rows) == 27
    assert rows.count(("load (ndjson)", "users.ndjson")) == 1
    assert sum(1 for section, _ in rows if section == "query") == 3
    assert ("dumps", "wide_arrays.json") in rows


def test_a_mismatch_is_never_allowed_through(tmp_path, capsys, complete_report):
    """--allow-incomplete forgives a scoped set, never misattributed evidence."""
    reports = tmp_path / "ci"
    place(reports, "bench_results_linux-x86_64.md", complete_report(commit="deadbee"))
    (reports / "run_info.json").write_text(
        json.dumps({"run_id": 9, "head_sha": HEAD_SHA, "reports": {"linux-x86_64": "x.md"}}),
        encoding="utf-8",
    )
    code, _ = run_summary(tmp_path, "--expect-platforms", ONE_LINUX, "--allow-incomplete")
    assert code == 1
    assert "contradicts the run manifest" in capsys.readouterr().err


def test_invalid_evidence_is_not_counted_on_stdout(tmp_path, capsys, complete_report):
    """A `nan` row used to be ranked #1 in the CLI's own tally."""
    report = complete_report(drop_strata=(("loads", "flat.json"),))
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="flat.json",
            library="strata",
            min_ms=float("nan"),
            median_ms=float("nan"),
            p95_ms=float("nan"),
            rss_mb=10.0,
        )
    )
    place(tmp_path / "ci", "bench_results_linux-x86_64.md", report)
    assert run_summary(tmp_path, "--expect-platforms", ONE_LINUX)[0] == 1
    assert "0 platform(s), 0/0 rows at #1" in capsys.readouterr().out


# ---------------------------------------------------------------------------
# Undeclared evidence: counted nowhere, never a substitute
# ---------------------------------------------------------------------------

# `platform_key` can name legs the matrix does not declare -- windows-arm64 is
# a real, buildable combination that simply is not in CI_PLATFORMS.
PLATFORM_HEADERS = {
    "linux-arm64": ("Linux-6.8.0-1014-azure-aarch64-with-glibc2.39", "aarch64"),
    "linux-x86_64": (LINUX, "x86_64"),
    "macos-arm64": (MACOS, "arm64"),
    "macos-x86_64": ("macOS-15.6-x86_64-i386-64bit-Mach-O", "x86_64"),
    "windows-x86_64": ("Windows-2022Server-10.0.20348-SP0", "AMD64"),
    "windows-arm64": ("Windows-11-10.0.26100-SP0", "ARM64"),
}


def place_platform(reports: Path, key: str, complete_report, **kwargs) -> None:
    """A whole declared workload, stamped with `key`'s own environment header."""
    platform, machine = PLATFORM_HEADERS[key]
    place(
        reports,
        f"bench_results_{key}.md",
        complete_report(platform=platform, machine=machine, **kwargs),
    )


def test_an_undeclared_leg_does_not_inflate_the_verdict(tmp_path, capsys, complete_report):
    """Five declared legs plus a stray read "Goal met on 6/5" and 162/162."""
    reports = tmp_path / "ci"
    for key in CI_PLATFORMS:
        place_platform(reports, key, complete_report)
    place_platform(reports, "windows-arm64", complete_report)

    code, text = run_summary(tmp_path)
    assert code == 0
    assert "**Goal met on 5/5 platforms" in text
    assert "6/5" not in text
    assert "; 1 undeclared leg(s) not counted" in text

    # The stray gets its own section, its own denominator, and no vote.
    assert "## Reports outside the declared matrix" in text
    before, after = text.split("## Reports outside the declared matrix")
    assert "windows-arm64" not in before
    assert "| windows-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |" in after

    out = capsys.readouterr().out
    assert "5 platform(s), 135/135 rows at #1" in out
    assert "162/162" not in out
    assert "1 report(s) outside the declared matrix" in out


def test_an_undeclared_leg_cannot_fill_in_for_a_missing_one(tmp_path, capsys, complete_report):
    """linux-arm64 absent, windows-arm64 present: not "Goal met on 5/5"."""
    reports = tmp_path / "ci"
    for key in CI_PLATFORMS:
        if key != "linux-arm64":
            place_platform(reports, key, complete_report)
    place_platform(reports, "windows-arm64", complete_report)

    code, text = run_summary(tmp_path)
    assert code == 1
    assert "**Goal met on 4/5 platforms; 1 platform(s) MISSING" in text
    assert "Goal met on 5/5" not in text
    assert "Evidence is incomplete: linux-arm64 (MISSING)" in text

    out = capsys.readouterr().out
    assert "4 platform(s), 108/108 rows at #1" in out
    assert "135/135" not in out


def test_an_extra_row_cannot_complete_a_platform(tmp_path, complete_report):
    """26 declared rows plus one extra used to render 6/5 and a 27/27 total."""
    extra = [
        Measurement(
            section="loads",
            dataset="extra.json",
            library=library,
            min_ms=median,
            median_ms=median,
            p95_ms=median,
            rss_mb=10.0,
        )
        for library, median in (("strata", 1.0), ("orjson", 1.2))
    ]
    place(
        tmp_path / "ci",
        "bench_results_linux-x86_64.md",
        complete_report(drop=(("dumps", "mixed.json"),), extra=extra),
    )

    code, text = run_summary(tmp_path, "--expect-platforms", ONE_LINUX)
    assert code == 1
    assert "| linux-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |" in text
    assert "6/5" not in text
    assert "27/27" not in text  # no cell may exceed its declared denominator
    assert "| linux-x86_64 | INCOMPLETE | 26/27 |" in text
    assert "extra loads|extra.json: row is outside the declared workload" in text


def test_a_complete_set_with_no_manifest_says_its_provenance_is_unverified(
    tmp_path, complete_report
):
    """ "Complete" is coverage; it is not a claim about where the files came from."""
    reports = tmp_path / "ci"
    for key in CI_PLATFORMS:
        place_platform(reports, key, complete_report)

    code, text = run_summary(tmp_path)
    assert code == 0
    assert (
        "All 5 declared platforms reported valid, complete evidence; "
        "5 of them with unverified provenance." in text
    )

"""Contract tests for the cross-platform CI standings summary.

Pins docs/benchmarking/SKILL.md (CI standings by platform and architecture):
ranks are computed within one platform's report only; an exact median tie
resolves as #1; a report with ERROR rows is INVALID and excluded from the
counts rather than published as partial standings
(docs/context/benchmarks.md); the verdict tracks the goal of #1 in every row
on every platform/architecture. main() is exercised end-to-end.
"""

import json
from pathlib import Path

from benchmarks.ci_summary import main
from benchmarks.harness import Measurement, Report, render_report

LINUX = "Linux-6.8.0-1014-azure-x86_64-with-glibc2.39"
MACOS = "macOS-26.3-arm64-arm-64bit-Mach-O"


def write_report(
    directory: Path,
    filename: str,
    platform: str,
    machine: str,
    rows: dict[tuple[str, str], dict[str, float]],
    *,
    errors: list[Measurement] | None = None,
) -> None:
    report = Report(
        name="ci-probe",
        environment={
            "commit": "16b0a58",
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


def run_summary(tmp_path: Path) -> tuple[int, str]:
    output = tmp_path / "ci_summary.md"
    code = main(["--reports-dir", str(tmp_path / "ci"), "--output", str(output)])
    return code, output.read_text(encoding="utf-8") if output.is_file() else ""


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

    code, text = run_summary(tmp_path)
    assert code == 0

    # Overview: per-category "#1 rows / comparable rows" cells per platform.
    assert "| macos-arm64 | 1/1 | - | 1/1 |" in text
    assert "| linux-x86_64 | 0/1 | 1/1 | 1/2 |" in text

    # The behind table names the row, the rank among the row's libraries, the
    # gap to the fastest rival, and that rival.
    assert "| loads | users.json | 3/3 | 1.20x | orjson |" in text
    assert "**Goal met on 1/2 platforms -- 1 row(s) to close.**" in text
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
    code, text = run_summary(tmp_path)
    assert code == 0
    assert "**Goal met on 1/1 platforms.**" in text
    assert "All rows #1." in text


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
    code, text = run_summary(tmp_path)
    assert code == 0
    assert "| linux-x86_64 | - | INVALID |" in text
    assert "loads | users.json | ujson: ImportError" in text
    # The invalid platform is excluded from the verdict's denominator.
    assert "**Goal met on 1/1 platforms; 1 platform(s) INVALID.**" in text


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
    code, text = run_summary(tmp_path)
    assert code == 0
    assert "Benchmarks run 31392004866" in text
    assert "main @ 16b0a58fe1ed0da3d139b64f59d66cea9822f4a3" in text


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

"""Contract tests for the CI benchmark fetch tool.

Pins docs/benchmarking/SKILL.md (CI standings by platform and architecture):
reports are named by the os/arch in their own environment header, never by
runner label; a fetch replaces the previous one wholesale and records its
provenance in run_info.json; gh failures and report defects are loud, with
distinct exit codes. main() is exercised end-to-end -- the previous
implementation's regression tool shipped with an untested, crashing main().
"""

import json
import subprocess
from pathlib import Path

import pytest

from benchmarks import ci_fetch
from benchmarks.harness import Measurement, Report, parse_report, render_report

RUN = {
    "databaseId": 31392004866,
    "workflowName": "Benchmarks",
    "url": "https://example.invalid/actions/runs/31392004866",
    "event": "workflow_dispatch",
    "status": "completed",
    "conclusion": "success",
    "headBranch": "main",
    "headSha": "16b0a58fe1ed0da3d139b64f59d66cea9822f4a3",
    "createdAt": "2026-08-10T13:15:50Z",
}

LINUX = "Linux-6.8.0-1014-azure-x86_64-with-glibc2.39"
MACOS = "macOS-26.3-arm64-arm-64bit-Mach-O"
WINDOWS = "Windows-2022Server-10.0.20348-SP0"


def ci_report(platform: str, machine: str) -> str:
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
    for library, median in {"strata": 1.0, "orjson": 1.2}.items():
        report.measurements.append(
            Measurement(
                section="loads",
                dataset="users.json",
                library=library,
                min_ms=median,
                median_ms=median,
                p95_ms=median,
                rss_mb=10.0,
            )
        )
    return render_report(report)


def fake_gh(runs=None, view=None, artifacts=None, download_rc=0):
    """A _run_gh substitute serving canned gh output."""

    def run(args):
        if args[:2] == ["run", "list"]:
            return subprocess.CompletedProcess(args, 0, json.dumps(runs or []), "")
        if args[:2] == ["run", "view"]:
            return subprocess.CompletedProcess(args, 0, json.dumps(view or {}), "")
        if args[:2] == ["run", "download"]:
            target = Path(args[args.index("--dir") + 1])
            for artifact, files in (artifacts or {}).items():
                for filename, text in files.items():
                    path = target / artifact / filename
                    path.parent.mkdir(parents=True, exist_ok=True)
                    path.write_text(text, encoding="utf-8")
            stderr = "no artifacts" if download_rc else ""
            return subprocess.CompletedProcess(args, download_rc, "", stderr)
        raise AssertionError(f"unexpected gh invocation: {args}")

    return run


def test_platform_key_reads_the_report_not_the_runner_label():
    assert ci_fetch.platform_key({"platform": MACOS, "machine": "arm64"}) == "macos-arm64"
    assert ci_fetch.platform_key({"platform": LINUX, "machine": "x86_64"}) == "linux-x86_64"
    assert ci_fetch.platform_key({"platform": WINDOWS, "machine": "AMD64"}) == "windows-x86_64"
    aarch64 = {"platform": "Linux-6.8.0-1014-azure-aarch64-with-glibc2.39", "machine": "aarch64"}
    assert ci_fetch.platform_key(aarch64) == "linux-arm64"


def test_platform_key_rejects_what_it_cannot_name():
    with pytest.raises(ValueError, match="unrecognized"):
        ci_fetch.platform_key({"platform": "Plan9-1.0", "machine": "mips"})
    with pytest.raises(ValueError, match="unrecognized"):
        ci_fetch.platform_key({})


def test_fetch_places_reports_and_run_info(tmp_path, monkeypatch, capsys):
    artifacts = {
        "benchmark-ubuntu-latest": {"bench_ci_ubuntu-latest.md": ci_report(LINUX, "x86_64")},
        "benchmark-windows-latest": {"bench_ci_windows-latest.md": ci_report(WINDOWS, "AMD64")},
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=artifacts))

    dest = tmp_path / "ci"
    dest.mkdir()
    stale = dest / "bench_results_stale-arch.md"
    stale.write_text("left over from an earlier fetch\n", encoding="utf-8")

    assert ci_fetch.main(["--dest", str(dest)]) == 0
    assert not stale.exists()

    linux = dest / "bench_results_linux-x86_64.md"
    windows = dest / "bench_results_windows-x86_64.md"
    assert linux.is_file() and windows.is_file()
    parsed = parse_report(linux.read_text(encoding="utf-8"), name=linux.name)
    assert parsed.environment["machine"] == "x86_64"
    assert any(m.library == "strata" for m in parsed.measurements)

    info = json.loads((dest / "run_info.json").read_text(encoding="utf-8"))
    assert info["run_id"] == RUN["databaseId"]
    assert info["head_sha"] == RUN["headSha"]
    assert info["conclusion"] == "success"
    assert info["reports"]["linux-x86_64"] == "benchmark-ubuntu-latest/bench_ci_ubuntu-latest.md"

    out = capsys.readouterr().out
    assert "fetched linux-x86_64" in out
    assert "2 report(s)" in out


def test_fetch_skips_incomplete_runs(tmp_path, monkeypatch):
    running = {**RUN, "databaseId": 2, "status": "in_progress", "conclusion": ""}
    artifacts = {"benchmark-macos-latest": {"report.md": ci_report(MACOS, "arm64")}}
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[running, RUN], artifacts=artifacts))
    dest = tmp_path / "ci"
    assert ci_fetch.main(["--dest", str(dest)]) == 0
    info = json.loads((dest / "run_info.json").read_text(encoding="utf-8"))
    assert info["run_id"] == RUN["databaseId"]


def test_fetch_explicit_run_uses_gh_run_view(tmp_path, monkeypatch):
    artifacts = {"benchmark-macos-latest": {"report.md": ci_report(MACOS, "arm64")}}
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(view=RUN, artifacts=artifacts))
    dest = tmp_path / "ci"
    assert ci_fetch.main(["--run", "31392004866", "--dest", str(dest)]) == 0
    assert (dest / "bench_results_macos-arm64.md").is_file()


def test_fetch_warns_when_the_run_did_not_succeed(tmp_path, monkeypatch, capsys):
    failed = {**RUN, "conclusion": "failure"}
    artifacts = {"benchmark-macos-latest": {"report.md": ci_report(MACOS, "arm64")}}
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[failed], artifacts=artifacts))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci")]) == 0
    assert "warning" in capsys.readouterr().err


def test_fetch_duplicate_platform_is_a_report_error(tmp_path, monkeypatch, capsys):
    artifacts = {
        "benchmark-macos-latest": {"a.md": ci_report(MACOS, "arm64")},
        "benchmark-macos-again": {"b.md": ci_report(MACOS, "arm64")},
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=artifacts))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci")]) == 2
    assert "two artifacts claim macos-arm64" in capsys.readouterr().err


def test_fetch_no_reports_is_a_failure(tmp_path, monkeypatch, capsys):
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts={}))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci")]) == 1
    assert "no benchmark reports" in capsys.readouterr().err


def test_fetch_no_completed_run_is_a_failure(tmp_path, monkeypatch, capsys):
    running = {**RUN, "status": "in_progress"}
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[running]))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci")]) == 1
    assert "no completed run" in capsys.readouterr().err


def test_fetch_gh_failure_is_a_failure(tmp_path, monkeypatch, capsys):
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], download_rc=1))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci")]) == 1
    assert "no artifacts" in capsys.readouterr().err


def test_fetch_missing_gh_is_a_failure(tmp_path, monkeypatch, capsys):
    def no_gh(args):
        raise FileNotFoundError("gh")

    monkeypatch.setattr(ci_fetch, "_run_gh", no_gh)
    assert ci_fetch.main(["--dest", str(tmp_path / "ci")]) == 1
    assert "GitHub CLI" in capsys.readouterr().err

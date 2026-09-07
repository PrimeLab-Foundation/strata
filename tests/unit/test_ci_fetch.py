"""Contract tests for the CI benchmark fetch tool.

Pins docs/benchmarking/SKILL.md (CI standings by platform and architecture):
reports are named by the os/arch in their own environment header, never by
runner label; a fetch replaces the previous one wholesale and records its
provenance in run_info.json; gh failures and report defects are loud, with
distinct exit codes. main() is exercised end-to-end -- the previous
implementation's regression tool shipped with an untested, crashing main().

Since the 2026-09-07 review they also pin the two halves of "all-or-nothing":
the replacement is verified before it is placed (platform identity, the run's
own head SHA, and the declared coverage), and it is staged and swapped, so a
write that fails half way leaves the previous reports and run_info.json
exactly as they were. The reviewed version deleted first and wrote after
(docs/performance/ci-review-2026-09-07.md, finding 10).

The follow-up review closed the third case, the one no exception path can
reach: a fetch *killed* mid-swap. The next start-up finds the debris, puts an
unambiguously interrupted swap back, and refuses anything it would have to
guess at rather than leaving the previous evidence in a directory nothing
names (build/evidence/T2-REVIEW/REVIEW.md, defect 3). A filesystem failure
during placement is exit 3, not a traceback.
"""

import json
import subprocess
from pathlib import Path

import pytest

from benchmarks import ci_fetch
from benchmarks.harness import (
    CI_PLATFORMS,
    Measurement,
    Report,
    parse_report,
    render_report,
    workload_rows,
)

# The fixtures below carry one row each: they exercise identity and placement,
# not the workload, so they say so.
SCOPED = ("--expect", "none", "--expect-platforms", "")

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

    assert ci_fetch.main(["--dest", str(dest), *SCOPED]) == 0
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
    assert ci_fetch.main(["--dest", str(dest), *SCOPED]) == 0
    info = json.loads((dest / "run_info.json").read_text(encoding="utf-8"))
    assert info["run_id"] == RUN["databaseId"]


def test_fetch_explicit_run_uses_gh_run_view(tmp_path, monkeypatch):
    artifacts = {"benchmark-macos-latest": {"report.md": ci_report(MACOS, "arm64")}}
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(view=RUN, artifacts=artifacts))
    dest = tmp_path / "ci"
    assert ci_fetch.main(["--run", "31392004866", "--dest", str(dest), *SCOPED]) == 0
    assert (dest / "bench_results_macos-arm64.md").is_file()


def test_fetch_warns_when_the_run_did_not_succeed(tmp_path, monkeypatch, capsys):
    failed = {**RUN, "conclusion": "failure"}
    artifacts = {"benchmark-macos-latest": {"report.md": ci_report(MACOS, "arm64")}}
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[failed], artifacts=artifacts))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci"), *SCOPED]) == 0
    assert "warning" in capsys.readouterr().err


def test_fetch_duplicate_platform_is_a_report_error(tmp_path, monkeypatch, capsys):
    artifacts = {
        "benchmark-macos-latest": {"a.md": ci_report(MACOS, "arm64")},
        "benchmark-macos-again": {"b.md": ci_report(MACOS, "arm64")},
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=artifacts))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci"), *SCOPED]) == 2
    assert "two artifacts claim macos-arm64" in capsys.readouterr().err


def test_fetch_no_reports_is_a_failure(tmp_path, monkeypatch, capsys):
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts={}))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci"), *SCOPED]) == 1
    assert "no benchmark reports" in capsys.readouterr().err


def test_fetch_no_completed_run_is_a_failure(tmp_path, monkeypatch, capsys):
    running = {**RUN, "status": "in_progress"}
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[running]))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci"), *SCOPED]) == 1
    assert "no completed run" in capsys.readouterr().err


def test_fetch_gh_failure_is_a_failure(tmp_path, monkeypatch, capsys):
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], download_rc=1))
    assert ci_fetch.main(["--dest", str(tmp_path / "ci"), *SCOPED]) == 1
    assert "no artifacts" in capsys.readouterr().err


def test_fetch_missing_gh_is_a_failure(tmp_path, monkeypatch, capsys):
    def no_gh(args):
        raise FileNotFoundError("gh")

    monkeypatch.setattr(ci_fetch, "_run_gh", no_gh)
    assert ci_fetch.main(["--dest", str(tmp_path / "ci"), *SCOPED]) == 1
    assert "GitHub CLI" in capsys.readouterr().err


# ---------------------------------------------------------------------------
# Verified before placed, staged before swapped
# ---------------------------------------------------------------------------


def complete_ci_report(platform: str, machine: str, *, commit: str = "16b0a58", drop=()) -> str:
    """A report carrying the whole declared workload, optionally short a row."""
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
    for section, dataset in workload_rows():
        if (section, dataset) in tuple(drop):
            continue
        for library, median in (("strata", 1.0), ("orjson", 1.2)):
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
    return render_report(report)


def full_run_artifacts(**kwargs) -> dict:
    """One artifact per declared platform, each with the whole workload."""
    machines = {
        "linux-arm64": ("Linux-6.8.0-1014-azure-aarch64-with-glibc2.39", "aarch64"),
        "linux-x86_64": (LINUX, "x86_64"),
        "macos-arm64": (MACOS, "arm64"),
        "macos-x86_64": ("macOS-15.6-x86_64-i386-64bit-Mach-O", "x86_64"),
        "windows-x86_64": (WINDOWS, "AMD64"),
    }
    return {
        f"benchmark-{key}": {f"bench_ci_{key}.md": complete_ci_report(*machines[key], **kwargs)}
        for key in CI_PLATFORMS
    }


def previous_fetch(dest: Path) -> dict[str, str]:
    """A destination holding an earlier, complete fetch."""
    dest.mkdir(parents=True, exist_ok=True)
    (dest / "bench_results_linux-x86_64.md").write_text("earlier evidence\n", encoding="utf-8")
    (dest / "run_info.json").write_text('{"run_id": 1}\n', encoding="utf-8")
    return {path.name: path.read_text(encoding="utf-8") for path in dest.iterdir()}


def test_a_complete_run_is_fetched_and_recorded(tmp_path, monkeypatch, capsys):
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=full_run_artifacts()))
    dest = tmp_path / "ci"
    assert ci_fetch.main(["--dest", str(dest)]) == 0
    for key in CI_PLATFORMS:
        assert (dest / f"bench_results_{key}.md").is_file()
    info = json.loads((dest / "run_info.json").read_text(encoding="utf-8"))
    assert info["complete"] is True
    assert info["expected_platforms"] == list(CI_PLATFORMS)
    assert "problems" not in info
    assert "5 report(s)" in capsys.readouterr().out


def test_a_missing_leg_is_refused_and_changes_nothing(tmp_path, monkeypatch, capsys):
    artifacts = full_run_artifacts()
    del artifacts["benchmark-windows-x86_64"]
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=artifacts))
    dest = tmp_path / "ci"
    before = previous_fetch(dest)

    assert ci_fetch.main(["--dest", str(dest)]) == 1
    err = capsys.readouterr().err
    assert "windows-x86_64: the declared platform uploaded no report" in err
    assert {path.name: path.read_text(encoding="utf-8") for path in dest.iterdir()} == before


def test_a_missing_leg_can_be_fetched_deliberately(tmp_path, monkeypatch, capsys):
    artifacts = full_run_artifacts()
    del artifacts["benchmark-windows-x86_64"]
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=artifacts))
    dest = tmp_path / "ci"

    assert ci_fetch.main(["--dest", str(dest), "--allow-incomplete"]) == 0
    info = json.loads((dest / "run_info.json").read_text(encoding="utf-8"))
    assert info["complete"] is False
    assert info["problems"] == ["windows-x86_64: the declared platform uploaded no report"]
    assert "warning: placed incomplete evidence" in capsys.readouterr().err


def test_a_missing_dataset_row_is_refused(tmp_path, monkeypatch, capsys):
    artifacts = full_run_artifacts()
    artifacts["benchmark-linux-arm64"] = {
        "bench_ci_linux-arm64.md": complete_ci_report(
            "Linux-6.8.0-1014-azure-aarch64-with-glibc2.39",
            "aarch64",
            drop=(("dumps", "mixed.json"),),
        )
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=artifacts))
    dest = tmp_path / "ci"
    before = previous_fetch(dest)

    assert ci_fetch.main(["--dest", str(dest)]) == 1
    assert "linux-arm64: missing dumps|mixed.json" in capsys.readouterr().err
    assert {path.name: path.read_text(encoding="utf-8") for path in dest.iterdir()} == before


def test_a_report_from_another_commit_is_refused(tmp_path, monkeypatch, capsys):
    artifacts = full_run_artifacts()
    artifacts["benchmark-macos-arm64"] = {
        "bench_ci_macos-arm64.md": complete_ci_report(MACOS, "arm64", commit="deadbee")
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=artifacts))
    dest = tmp_path / "ci"
    before = previous_fetch(dest)

    assert ci_fetch.main(["--dest", str(dest)]) == 2
    err = capsys.readouterr().err
    assert "report commit deadbee is not run 31392004866's" in err
    assert {path.name: path.read_text(encoding="utf-8") for path in dest.iterdir()} == before


def test_an_empty_report_is_refused(tmp_path, monkeypatch, capsys):
    artifacts = full_run_artifacts()
    header = complete_ci_report(WINDOWS, "AMD64").split("## ", 1)[0]
    artifacts["benchmark-windows-x86_64"] = {"bench_ci_windows-x86_64.md": header}
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=artifacts))
    dest = tmp_path / "ci"
    before = previous_fetch(dest)

    assert ci_fetch.main(["--dest", str(dest)]) == 2
    assert "contains no measurements" in capsys.readouterr().err
    assert {path.name: path.read_text(encoding="utf-8") for path in dest.iterdir()} == before


def test_a_failed_write_leaves_the_previous_fetch_intact(tmp_path, monkeypatch, capsys):
    """The reviewed placement deleted first: an error on the second write left
    one new report and no previous evidence at all."""
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=full_run_artifacts()))
    dest = tmp_path / "ci"
    before = previous_fetch(dest)

    written = []
    real_write = ci_fetch._write_text

    def failing_write(path, text):
        written.append(path)
        if len(written) == 2:
            raise OSError("no space left on device")
        real_write(path, text)

    monkeypatch.setattr(ci_fetch, "_write_text", failing_write)
    # Documented as exit 3, not an escaped traceback.
    assert ci_fetch.main(["--dest", str(dest)]) == 3
    assert "could not be placed" in capsys.readouterr().err

    assert {path.name: path.read_text(encoding="utf-8") for path in dest.iterdir()} == before
    # And no staging directory is left behind beside it.
    assert [path.name for path in tmp_path.iterdir()] == ["ci"]


def test_a_failed_swap_restores_the_previous_fetch(tmp_path, monkeypatch, capsys):
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=full_run_artifacts()))
    dest = tmp_path / "ci"
    before = previous_fetch(dest)

    real_replace = ci_fetch.os.replace
    calls = []

    def failing_replace(src, dst):
        calls.append((src, dst))
        if len(calls) == 2:  # the staging -> dest swap, after the old set moved
            raise OSError("interrupted")
        real_replace(src, dst)

    monkeypatch.setattr(ci_fetch.os, "replace", failing_replace)
    assert ci_fetch.main(["--dest", str(dest)]) == 3
    assert "could not be placed" in capsys.readouterr().err

    assert {path.name: path.read_text(encoding="utf-8") for path in dest.iterdir()} == before


def test_files_the_fetch_does_not_own_survive_it(tmp_path, monkeypatch):
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=full_run_artifacts()))
    dest = tmp_path / "ci"
    dest.mkdir(parents=True)
    (dest / "NOTES.md").write_text("kept by hand\n", encoding="utf-8")

    assert ci_fetch.main(["--dest", str(dest)]) == 0
    assert (dest / "NOTES.md").read_text(encoding="utf-8") == "kept by hand\n"
    assert (dest / "bench_results_macos-arm64.md").is_file()


# ---------------------------------------------------------------------------
# The debris of a fetch that was killed, not raised
# ---------------------------------------------------------------------------


def interrupted_swap(dest: Path) -> tuple[Path, Path, dict[str, str]]:
    """The state a SIGKILL between the move-aside and the rename leaves.

    The previous evidence is in `.<dest>.previous-*`, the replacement is half
    written in `.<dest>.staging-*`, and `dest` itself does not exist.
    """
    before = previous_fetch(dest)
    orphan = dest.parent / f".{dest.name}.previous-abc123"
    dest.rename(orphan)
    staging = dest.parent / f".{dest.name}.staging-def456"
    staging.mkdir()
    (staging / "bench_results_linux-x86_64.md").write_text("half written\n", encoding="utf-8")
    return orphan, staging, before


def test_an_interrupted_swap_is_recovered_on_the_next_fetch(tmp_path, monkeypatch, capsys):
    """Nothing named where the previous evidence went; now the next run does."""
    dest = tmp_path / "ci"
    orphan, staging, _ = interrupted_swap(dest)
    assert not dest.exists()

    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(runs=[RUN], artifacts=full_run_artifacts()))
    assert ci_fetch.main(["--dest", str(dest)]) == 0

    err = capsys.readouterr().err
    assert "recovered the previous reports from .ci.previous-abc123" in err
    # The half-written staging directory is named too, and left alone: it is
    # never the last copy of anything.
    assert f"an unfinished staging directory is beside {dest}: {staging.name}" in err
    assert not orphan.exists()
    for key in CI_PLATFORMS:
        assert (dest / f"bench_results_{key}.md").is_file()


def test_unrecoverable_debris_is_named_and_refuses_the_fetch(tmp_path, monkeypatch, capsys):
    """Two candidates for the truth: this tool names them, it does not choose."""
    dest = tmp_path / "ci"
    before = previous_fetch(dest)
    orphan = dest.parent / f".{dest.name}.previous-abc123"
    orphan.mkdir()
    (orphan / "bench_results_linux-x86_64.md").write_text("the other copy\n", encoding="utf-8")

    def never(args):
        raise AssertionError("gh must not run before the debris is resolved")

    monkeypatch.setattr(ci_fetch, "_run_gh", never)
    assert ci_fetch.main(["--dest", str(dest)]) == 2

    err = capsys.readouterr().err
    assert ".ci.previous-abc123" in err
    assert "move or remove them by hand" in err
    assert orphan.is_dir()
    assert {path.name: path.read_text(encoding="utf-8") for path in dest.iterdir()} == before


def test_a_clean_destination_reports_no_debris(tmp_path):
    dest = tmp_path / "ci"
    previous_fetch(dest)
    assert ci_fetch.recover_interrupted_install(dest) == (True, [])


def test_make_bench_ci_forwards_flags_to_both_tools():
    """--allow-incomplete has to reach the fetch *and* the summary.

    Fetching a partial run and then summarizing it as if it were complete is
    the false pass these gates exist to stop, so the flag variable goes to
    both recipe lines (build/evidence/T2-REVIEW/REVIEW.md, defect 4).
    """
    makefile = (ci_fetch.PROJECT_ROOT / "Makefile").read_text(encoding="utf-8")
    assert "BENCH_CI_FLAGS ?=" in makefile
    recipe = makefile.split("\nbench-ci:", 1)[1].split("\n\n", 1)[0]
    assert "benchmarks.ci_fetch $(BENCH_CI_FLAGS)" in recipe
    assert "benchmarks.ci_summary $(BENCH_CI_FLAGS)" in recipe

"""native-v1 evidence handling in benchmarks/ci_fetch.py.

Pins docs/decisions.md, 2026-09-29, "benchmarks": a bad native-v1 report or
companion is recorded as invalid native evidence and dropped, but it can
never fail the canonical fetch or change its exit code -- native-v1 coverage
is best-effort, separate from the 135-row canonical verdict `_verify` checks.
"""

import json
import subprocess
from pathlib import Path

from benchmarks import ci_fetch
from benchmarks.harness import NATIVE_REPORT_NAME, Measurement, Report, render_report

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


def canonical_report() -> str:
    report = Report(
        name="ci-probe",
        environment={
            "commit": "16b0a58",
            "python": "3.12.6",
            "platform": LINUX,
            "machine": "x86_64",
            "repeats": "10",
            "warmup": "2",
        },
    )
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="users.json",
            library="strata",
            min_ms=1.0,
            median_ms=1.0,
            p95_ms=1.0,
        )
    )
    return render_report(report)


def native_report(*, commit: str = "16b0a58", with_measurements: bool = True) -> str:
    report = Report(
        NATIVE_REPORT_NAME,
        environment={
            "commit": commit,
            "python": "3.12.6",
            "platform": LINUX,
            "machine": "x86_64",
            "repeats": "10",
            "warmup": "2",
        },
    )
    if with_measurements:
        report.measurements.append(
            Measurement(
                section="dumps",
                dataset="native.small",
                library="strata",
                min_ms=1.0,
                median_ms=1.0,
                p95_ms=1.0,
            )
        )
    return render_report(report)


def fake_gh(artifacts):
    def run(args):
        if args[:2] == ["run", "list"]:
            return subprocess.CompletedProcess(args, 0, json.dumps([RUN]), "")
        if args[:2] == ["run", "view"]:
            return subprocess.CompletedProcess(args, 0, json.dumps(RUN), "")
        if args[:2] == ["run", "download"]:
            target = Path(args[args.index("--dir") + 1])
            for artifact, files in artifacts.items():
                for filename, text in files.items():
                    path = target / artifact / filename
                    path.parent.mkdir(parents=True, exist_ok=True)
                    path.write_text(text, encoding="utf-8")
            return subprocess.CompletedProcess(args, 0, "", "")
        raise AssertionError(f"unexpected gh invocation: {args}")

    return run


def _run_info(dest: Path) -> dict:
    return json.loads((dest / "run_info.json").read_text(encoding="utf-8"))


def test_native_report_with_no_measurements_is_invalid_but_canonical_fetch_still_succeeds(
    tmp_path, monkeypatch, capsys
):
    artifacts = {
        "benchmark-linux-x86_64": {
            "bench_ci_linux-x86_64.md": canonical_report(),
            "native_v1_linux-x86_64.md": native_report(with_measurements=False),
        },
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(artifacts))
    dest = tmp_path / "ci"

    assert ci_fetch.main(["--dest", str(dest), *SCOPED]) == 0
    assert (dest / "bench_results_linux-x86_64.md").is_file()
    assert not (dest / "native_v1_linux-x86_64.md").exists()

    info = _run_info(dest)
    assert info["complete"] is True
    assert "linux-x86_64" in info["native_problems"]
    assert "no measurements" in info["native_problems"]["linux-x86_64"]
    assert "native-v1 linux-x86_64 is invalid" in capsys.readouterr().err


def test_native_report_from_another_commit_is_invalid_but_canonical_fetch_still_succeeds(
    tmp_path, monkeypatch, capsys
):
    artifacts = {
        "benchmark-linux-x86_64": {
            "bench_ci_linux-x86_64.md": canonical_report(),
            "native_v1_linux-x86_64.md": native_report(commit="deadbeef"),
        },
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(artifacts))
    dest = tmp_path / "ci"

    assert ci_fetch.main(["--dest", str(dest), *SCOPED]) == 0
    assert (dest / "bench_results_linux-x86_64.md").is_file()
    assert not (dest / "native_v1_linux-x86_64.md").exists()

    info = _run_info(dest)
    assert "is not run" in info["native_problems"]["linux-x86_64"]


def test_native_report_with_a_missing_required_companion_is_invalid_not_a_fetch_failure(
    tmp_path, monkeypatch, capsys
):
    # A report whose companion is present but corrupt is the case
    # validate_companion refuses with a ValueError -- for a canonical report
    # this is an identity defect (exit 2); for native-v1 it must only be
    # recorded, never raised into the canonical fetch's own exit code.
    # `_verify_native`/`_place` call `ci_fetch.validate_companion` directly,
    # so faking it here for the native report's path is the reliable way to
    # exercise that branch without hand-building a companion JSON.
    real_validate_companion = ci_fetch.validate_companion

    def flaky_validate_companion(path, markdown):
        if path.name == "native_v1_linux-x86_64.md":
            raise ValueError("provenance report hash mismatch")
        return real_validate_companion(path, markdown)

    monkeypatch.setattr(ci_fetch, "validate_companion", flaky_validate_companion)

    artifacts = {
        "benchmark-linux-x86_64": {
            "bench_ci_linux-x86_64.md": canonical_report(),
            "native_v1_linux-x86_64.md": native_report(),
        },
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(artifacts))
    dest = tmp_path / "ci"

    assert ci_fetch.main(["--dest", str(dest), *SCOPED]) == 0
    assert (dest / "bench_results_linux-x86_64.md").is_file()
    assert not (dest / "native_v1_linux-x86_64.md").exists()

    info = _run_info(dest)
    assert "provenance companion" in info["native_problems"]["linux-x86_64"]


def test_a_valid_native_report_is_still_placed_and_recorded(tmp_path, monkeypatch):
    artifacts = {
        "benchmark-linux-x86_64": {
            "bench_ci_linux-x86_64.md": canonical_report(),
            "native_v1_linux-x86_64.md": native_report(),
        },
    }
    monkeypatch.setattr(ci_fetch, "_run_gh", fake_gh(artifacts))
    dest = tmp_path / "ci"

    assert ci_fetch.main(["--dest", str(dest), *SCOPED]) == 0
    assert (dest / "native_v1_linux-x86_64.md").is_file()

    info = _run_info(dest)
    assert "native_problems" not in info
    assert info["native_reports"]["linux-x86_64"] == (
        "benchmark-linux-x86_64/native_v1_linux-x86_64.md"
    )

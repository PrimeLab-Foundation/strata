"""native-v1's own ci_summary section (benchmarks/ci_summary.py).

Pins docs/decisions.md, 2026-09-29, "benchmarks": native reports travel in the
same artifact as the canonical per-platform report but render in a separate
section, outside the 135-row verdict; a sample with no native-v1 evidence
still reads a complete canonical verdict, with exactly one line saying so.
"""

from pathlib import Path

from benchmarks.ci_summary import main, render_native_section
from benchmarks.harness import NATIVE_REPORT_NAME, Measurement, Report, render_report

LINUX = "Linux-6.8.0-1014-azure-x86_64-with-glibc2.39"


def _place(directory: Path, filename: str, report: Report) -> None:
    directory.mkdir(parents=True, exist_ok=True)
    (directory / filename).write_text(render_report(report), encoding="utf-8")


def _canonical_report(commit: str = "16b0a58") -> Report:
    report = Report(
        "ci-probe",
        environment={
            "commit": commit,
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
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="users.json",
            library="orjson",
            min_ms=1.2,
            median_ms=1.2,
            p95_ms=1.2,
        )
    )
    return report


def _native_report(commit: str = "16b0a58") -> Report:
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
    report.measurements.append(
        Measurement(
            section="dumps",
            dataset="native.small",
            library="orjson",
            min_ms=2.0,
            median_ms=2.0,
            p95_ms=2.0,
        )
    )
    report.measurements.append(
        Measurement(
            section="dumps",
            dataset="mixed.small (native flag)",
            library="strata (native=False)",
            min_ms=0.5,
            median_ms=0.5,
            p95_ms=0.5,
        )
    )
    report.measurements.append(
        Measurement(
            section="dumps",
            dataset="mixed.small (native flag)",
            library="strata (native=True)",
            min_ms=0.6,
            median_ms=0.6,
            p95_ms=0.6,
        )
    )
    return report


def _run(tmp_path: Path, *extra: str) -> tuple[int, str]:
    output = tmp_path / "ci_summary.md"
    code = main(["--reports-dir", str(tmp_path / "ci"), "--output", str(output), *extra])
    return code, output.read_text(encoding="utf-8") if output.is_file() else ""


def _scoped() -> list[str]:
    return ["--expect", "none", "--expect-platforms", "linux-x86_64"]


def test_render_native_section_with_no_reports_is_one_line():
    text = render_native_section({})
    lines = [line for line in text.splitlines() if line and not line.startswith("#")]
    assert lines == ["no native-v1 evidence"]


def test_no_native_evidence_renders_the_line_and_keeps_the_canonical_exit_code(tmp_path):
    _place(tmp_path / "ci", "bench_results_linux-x86_64.md", _canonical_report())
    code, text = _run(tmp_path, *_scoped())
    assert code == 0
    assert "## native-v1" in text
    assert "no native-v1 evidence" in text
    assert "Goal met on 1/1 platforms" in text


def test_native_evidence_gets_its_own_section_without_changing_the_canonical_verdict(tmp_path):
    _place(tmp_path / "ci", "bench_results_linux-x86_64.md", _canonical_report())
    code_without, text_without = _run(tmp_path, *_scoped())

    _place(tmp_path / "ci", "native_v1_linux-x86_64.md", _native_report())
    code_with, text_with = _run(tmp_path, *_scoped())

    canonical_without = text_without.split("## native-v1")[0]
    canonical_with = text_with.split("## native-v1")[0]
    assert code_with == code_without == 0
    assert canonical_with == canonical_without

    assert "no native-v1 evidence" not in text_with
    assert "linux-x86_64" in text_with.split("## native-v1")[1]
    assert "native.small" in text_with
    assert "mixed.small (native flag)" in text_with
    assert "strata (native=False)" in text_with
    assert "strata (native=True)" in text_with


def test_a_native_report_with_no_environment_platform_is_skipped_with_a_warning(tmp_path, capsys):
    _place(tmp_path / "ci", "bench_results_linux-x86_64.md", _canonical_report())
    broken = Report(NATIVE_REPORT_NAME, environment={"platform": "?", "machine": "?"})
    _place(tmp_path / "ci", "native_v1_broken.md", broken)
    code, text = _run(tmp_path, *_scoped())
    assert code == 0
    assert "no native-v1 evidence" in text
    assert "warning: native-v1" in capsys.readouterr().err


def test_two_native_reports_claiming_one_platform_keep_the_first_and_warn(tmp_path, capsys):
    _place(tmp_path / "ci", "bench_results_linux-x86_64.md", _canonical_report())
    _place(tmp_path / "ci", "native_v1_linux-x86_64.md", _native_report())
    _place(tmp_path / "ci", "native_v1_linux-x86_64_dup.md", _native_report())
    code, text = _run(tmp_path, *_scoped())
    assert code == 0
    assert "native.small" in text
    assert "two native-v1 reports claim linux-x86_64" in capsys.readouterr().err

"""Contract tests for the cross-platform supportability tripwire.

Pins docs/context/convention.md § Platform supportability: the CI benchmark
gate is platform-independent — ERROR rows, missing strata coverage, or a row
past the ratio bound trip it; absolute times never do.
"""

from pathlib import Path

from benchmarks.supportability_check import main

HEADER = """# Benchmark results - probe

- commit: abc1234

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
"""


def write_report(tmp_path: Path, rows: str) -> Path:
    target = tmp_path / "report.md"
    target.write_text(HEADER + rows, encoding="utf-8")
    return target


def test_clean_report_passes(tmp_path, capsys):
    report = write_report(
        tmp_path,
        "| users.json | strata | 1.0 | 1.2 | 1.3 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 0.9 | 1.0 | 1.1 | 10.0 | 1.20x |\n",
    )
    assert main([str(report)]) == 0
    assert "within 3.0x" in capsys.readouterr().out


def test_error_row_trips(tmp_path, capsys):
    report = write_report(
        tmp_path,
        "| users.json | strata | 1.0 | 1.2 | 1.3 | 10.0 | 1.00x |\n"
        "| users.json | orjson | ERROR | ERROR | ERROR | ERROR | ImportError |\n",
    )
    assert main([str(report)]) == 1
    assert "ERROR" in capsys.readouterr().err


def test_ratio_past_bound_trips(tmp_path, capsys):
    report = write_report(
        tmp_path,
        "| users.json | strata | 9.0 | 9.9 | 10.0 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 0.9 | 1.0 | 1.1 | 10.0 | 9.90x |\n",
    )
    assert main([str(report)]) == 1
    assert "misfiring" in capsys.readouterr().err


def test_bound_is_configurable(tmp_path):
    report = write_report(
        tmp_path,
        "| users.json | strata | 4.0 | 4.0 | 4.1 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 1.0 | 1.0 | 1.1 | 10.0 | 4.00x |\n",
    )
    assert main([str(report)]) == 1
    assert main([str(report), "--max-ratio", "5.0"]) == 0


def test_absolute_times_alone_never_trip(tmp_path):
    # Ten times slower than any dev-machine number — irrelevant, as long as
    # the ratio holds. This is the platform-independence property itself.
    report = write_report(
        tmp_path,
        "| users.json | strata | 100.0 | 120.0 | 130.0 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 90.0 | 100.0 | 110.0 | 10.0 | 1.20x |\n",
    )
    assert main([str(report)]) == 0


def test_missing_report_is_a_usage_error(tmp_path):
    assert main([str(tmp_path / "absent.md")]) == 2

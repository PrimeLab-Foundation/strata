"""Contract tests for the cross-platform supportability tripwire.

Pins docs/context/convention.md § Platform supportability: the CI benchmark
gate is platform-independent — ERROR rows, missing strata coverage, or a row
past the ratio bound trip it; absolute times never do. The 3.0x bound is
deliberately loose and stays where it is.

Since the 2026-09-07 review it also pins the other half: the gate is against
the *declared* workload (`harness.WORKLOADS`), so an empty report, a dataset
that never ran, a `nan` median or a duplicated row fails instead of passing on
whatever survived parsing (docs/performance/ci-review-2026-09-07.md, finding
2). A deliberately scoped report says so with ``--expect none``.

Since the follow-up review it also pins the exit-code convention this tool
shares word for word with `regression_check`: 1 is the gate's own negative
verdict on readable evidence (a row past the ratio bound), 2 is "this is not
gateable evidence" — an unusable report body, or one short of the declared
workload (build/evidence/T2-REVIEW/REVIEW.md, defect 4).
"""

from pathlib import Path

import pytest

from benchmarks import regression_check, supportability_check
from benchmarks.harness import Measurement, render_report, workload_rows
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


def write(tmp_path: Path, report, filename="report.md") -> Path:
    target = tmp_path / filename
    target.write_text(render_report(report), encoding="utf-8")
    return target


# ---------------------------------------------------------------------------
# The ratio bound, on explicitly scoped reports
# ---------------------------------------------------------------------------


def test_clean_report_passes(tmp_path, capsys):
    report = write_report(
        tmp_path,
        "| users.json | strata | 1.0 | 1.2 | 1.3 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 0.9 | 1.0 | 1.1 | 10.0 | 1.20x |\n",
    )
    assert main([str(report), "--expect", "none"]) == 0
    assert "within 3.0x" in capsys.readouterr().out


def test_error_row_trips(tmp_path, capsys):
    report = write_report(
        tmp_path,
        "| users.json | strata | 1.0 | 1.2 | 1.3 | 10.0 | 1.00x |\n"
        "| users.json | orjson | ERROR | ERROR | ERROR | ERROR | ImportError |\n",
    )
    assert main([str(report), "--expect", "none"]) == 2
    assert "ERROR" in capsys.readouterr().err


def test_ratio_past_bound_trips(tmp_path, capsys):
    report = write_report(
        tmp_path,
        "| users.json | strata | 9.0 | 9.9 | 10.0 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 0.9 | 1.0 | 1.1 | 10.0 | 9.90x |\n",
    )
    assert main([str(report), "--expect", "none"]) == 1
    assert "misfiring" in capsys.readouterr().err


def test_bound_is_configurable(tmp_path):
    report = write_report(
        tmp_path,
        "| users.json | strata | 4.0 | 4.0 | 4.1 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 1.0 | 1.0 | 1.1 | 10.0 | 4.00x |\n",
    )
    assert main([str(report), "--expect", "none"]) == 1
    assert main([str(report), "--expect", "none", "--max-ratio", "5.0"]) == 0


def test_absolute_times_alone_never_trip(tmp_path):
    # Ten times slower than any dev-machine number — irrelevant, as long as
    # the ratio holds. This is the platform-independence property itself.
    report = write_report(
        tmp_path,
        "| users.json | strata | 100.0 | 120.0 | 130.0 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 90.0 | 100.0 | 110.0 | 10.0 | 1.20x |\n",
    )
    assert main([str(report), "--expect", "none"]) == 0


def test_missing_report_is_a_usage_error(tmp_path):
    assert main([str(tmp_path / "absent.md")]) == 2


# ---------------------------------------------------------------------------
# The declared workload: what the CI leg actually runs
# ---------------------------------------------------------------------------


def test_the_declared_workload_passes(tmp_path, capsys, complete_report):
    assert main([str(write(tmp_path, complete_report()))]) == 0
    assert "27/27 declared rows" in capsys.readouterr().out


def test_a_scoped_report_fails_the_declared_workload(tmp_path, capsys):
    """The reviewed tripwire passed on a two-row report. It no longer does."""
    report = write_report(
        tmp_path,
        "| users.json | strata | 1.0 | 1.2 | 1.3 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 0.9 | 1.0 | 1.1 | 10.0 | 1.20x |\n",
    )
    assert main([str(report)]) == 2
    assert "no usable strata measurement" in capsys.readouterr().err


def test_an_empty_report_fails(tmp_path, capsys):
    empty = tmp_path / "empty.md"
    empty.write_text("", encoding="utf-8")
    assert main([str(empty), "--expect", "none"]) == 2
    assert "no measurements at all" in capsys.readouterr().err


def test_a_truncated_report_fails(tmp_path, capsys, complete_report):
    text = render_report(complete_report())
    truncated = tmp_path / "truncated.md"
    truncated.write_text(text[: len(text) // 2], encoding="utf-8")
    assert main([str(truncated)]) == 2
    assert "no usable strata measurement" in capsys.readouterr().err


def test_one_missing_strata_dataset_row_fails(tmp_path, capsys, complete_report):
    report = complete_report(drop_strata=(("dumps", "mixed.json"),))
    assert main([str(write(tmp_path, report))]) == 2
    assert "missing dumps|mixed.json" in capsys.readouterr().err


def test_a_missing_category_is_named(tmp_path, capsys, complete_report):
    rows = tuple(row for row in workload_rows() if row[0] != "search")
    assert main([str(write(tmp_path, complete_report(rows=rows)))]) == 2
    err = capsys.readouterr().err
    assert "search: declared category with no strata measurement" in err


def test_a_nan_median_fails(tmp_path, capsys, complete_report):
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
    assert main([str(write(tmp_path, report))]) == 2
    assert "is not finite" in capsys.readouterr().err


def test_an_infinite_value_fails(tmp_path, capsys, complete_report):
    report = complete_report(drop_strata=(("loads", "flat.json"),))
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="flat.json",
            library="strata",
            min_ms=1.0,
            median_ms=float("inf"),
            p95_ms=float("inf"),
            rss_mb=10.0,
        )
    )
    assert main([str(write(tmp_path, report))]) == 2
    assert "is not finite" in capsys.readouterr().err


def test_out_of_order_metrics_fail(tmp_path, capsys, complete_report):
    report = complete_report(drop_strata=(("loads", "flat.json"),))
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="flat.json",
            library="strata",
            min_ms=9.0,
            median_ms=1.0,
            p95_ms=2.0,
            rss_mb=10.0,
        )
    )
    assert main([str(write(tmp_path, report))]) == 2
    assert "out of order" in capsys.readouterr().err


def test_a_duplicated_row_fails(tmp_path, capsys, complete_report):
    report = complete_report()
    report.measurements.append(report.measurements[0])
    assert main([str(write(tmp_path, report))]) == 2
    assert "measured more than once" in capsys.readouterr().err


def test_an_unreadable_row_fails(tmp_path, capsys, complete_report):
    text = render_report(complete_report())
    damaged = text.replace(
        "| users.json | strata | 1.000 | 1.000 | 1.000 | 10.000 | 1.00x |",
        "| users.json | strata | 1.000 | not-a-number | 1.000 | 10.000 | 1.00x |",
        1,
    )
    target = tmp_path / "damaged.md"
    target.write_text(damaged, encoding="utf-8")
    assert main([str(target)]) == 2
    assert "unreadable row" in capsys.readouterr().err


# ---------------------------------------------------------------------------
# The exit-code convention, shared with regression_check
# ---------------------------------------------------------------------------


def test_an_invalid_body_and_a_ratio_breach_get_different_codes(tmp_path, capsys):
    """2 is "not gateable evidence"; 1 is a verdict on evidence that was read."""
    breach = write_report(
        tmp_path,
        "| users.json | strata | 9.0 | 9.9 | 10.0 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 0.9 | 1.0 | 1.1 | 10.0 | 9.90x |\n",
    )
    assert main([str(breach), "--expect", "none"]) == 1

    unusable = write_report(
        tmp_path,
        "| users.json | strata | 1.0 | 1.2 | 1.3 | 10.0 | 1.00x |\n"
        "| users.json | orjson | ERROR | ERROR | ERROR | ERROR | ImportError |\n",
    )
    assert main([str(unusable), "--expect", "none"]) == 2
    assert "not gateable evidence" in capsys.readouterr().err


def test_an_unusable_body_reports_the_ratio_breach_it_also_has(tmp_path, capsys):
    """The 2 does not hide what the gate saw before it refused to rule."""
    report = write_report(
        tmp_path,
        "| users.json | strata | 9.0 | 9.9 | 10.0 | 10.0 | 1.00x |\n"
        "| users.json | orjson | 0.9 | 1.0 | 1.1 | 10.0 | 9.90x |\n"
        "| flat.json | ujson | ERROR | ERROR | ERROR | ERROR | ImportError |\n",
    )
    assert main([str(report), "--expect", "none"]) == 2
    err = capsys.readouterr().err
    assert "ERROR" in err
    assert "(also) loads|users.json: 9.90x behind the best rival" in err


def test_the_exit_codes_are_in_the_help_and_match_regression_check(capsys):
    assert supportability_check.EXIT_CODES == regression_check.EXIT_CODES
    with pytest.raises(SystemExit):
        main(["--help"])
    out = capsys.readouterr().out
    assert "Exit codes:" in out
    assert "not gateable evidence" in out

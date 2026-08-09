"""Contract tests for the benchmark report format and the regression gate.

The previous implementation's checker was broken three ways at once and its
tests never noticed, because they only covered the library functions while
`main()` itself crashed (docs/benchmarking/SKILL.md). So `main()` is exercised
end to end here, through argv and exit codes, and the report format is tested
as a round trip so the writer and the parser cannot drift apart.
"""

import json

import pytest

from benchmarks.harness import (
    Measurement,
    Report,
    baseline_key,
    parse_report,
    render_report,
)
from benchmarks.regression_check import (
    MEDIAN_TOLERANCE,
    RSS_TOLERANCE,
    compare,
    extract,
    main,
)


def _report(name="bench_results_small.md", *, strata_median=10.0, rss=50.0, error=None):
    report = Report(name=name, environment={"commit": "abc1234", "python": "3.14.0"})
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="users.json",
            library="strata",
            min_ms=strata_median - 0.1,
            median_ms=strata_median,
            p95_ms=strata_median + 0.2,
            rss_mb=rss,
            error=error,
        ),
    )
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="users.json",
            library="orjson",
            min_ms=8.0,
            median_ms=8.2,
            p95_ms=8.4,
            rss_mb=49.0,
        ),
    )
    return report


# ---------------------------------------------------------------------------
# One format definition, both directions
# ---------------------------------------------------------------------------


def test_a_rendered_report_parses_back_to_the_same_numbers():
    original = _report()
    restored = parse_report(render_report(original), name=original.name)

    assert len(restored.measurements) == len(original.measurements)
    for before, after in zip(original.measurements, restored.measurements, strict=True):
        assert (after.section, after.dataset, after.library) == (
            before.section,
            before.dataset,
            before.library,
        )
        assert after.median_ms == pytest.approx(before.median_ms)
        assert after.p95_ms == pytest.approx(before.p95_ms)
        assert after.rss_mb == pytest.approx(before.rss_mb)


def test_environment_survives_the_round_trip():
    restored = parse_report(render_report(_report()))
    assert restored.environment["commit"] == "abc1234"


def test_error_rows_survive_and_mark_the_report_invalid():
    report = _report(error="TypeError")
    restored = parse_report(render_report(report))
    assert restored.has_errors


def test_extract_takes_only_strata_rows():
    entries = extract(parse_report(render_report(_report()), name="bench_results_small.md"))
    assert list(entries) == [baseline_key("bench_results_small.md", "loads", "users.json")]


# ---------------------------------------------------------------------------
# Thresholds: >2% median/p95, >5% RSS (docs/context/benchmarks.md)
# ---------------------------------------------------------------------------


def test_a_run_identical_to_the_baseline_is_not_a_breach():
    entries = {"k": {"median_ms": 10.0, "p95_ms": 11.0, "rss_mb": 50.0}}
    assert compare(entries, entries) == []


def test_a_slowdown_inside_the_tolerance_is_not_a_breach():
    baseline = {"k": {"median_ms": 10.0, "p95_ms": 11.0, "rss_mb": 50.0}}
    current = {
        "k": {"median_ms": 10.0 * (1 + MEDIAN_TOLERANCE / 2), "p95_ms": 11.0, "rss_mb": 50.0}
    }
    assert compare(baseline, current) == []


def test_a_median_slowdown_past_the_tolerance_is_a_breach():
    baseline = {"k": {"median_ms": 10.0}}
    current = {"k": {"median_ms": 10.0 * (1 + MEDIAN_TOLERANCE * 2)}}
    breaches = compare(baseline, current)
    assert [b.metric for b in breaches] == ["median_ms"]


def test_a_p95_slowdown_past_the_tolerance_is_a_breach():
    baseline = {"k": {"median_ms": 10.0, "p95_ms": 10.0}}
    current = {"k": {"median_ms": 10.0, "p95_ms": 10.0 * (1 + MEDIAN_TOLERANCE * 2)}}
    assert [b.metric for b in compare(baseline, current)] == ["p95_ms"]


def test_rss_uses_its_own_wider_tolerance():
    baseline = {"k": {"rss_mb": 100.0}}
    # 3% more memory is inside the 5% RSS tolerance, though it would breach the
    # 2% timing tolerance.
    assert compare(baseline, {"k": {"rss_mb": 103.0}}) == []
    assert [
        b.metric for b in compare(baseline, {"k": {"rss_mb": 100.0 * (1 + RSS_TOLERANCE * 2)}})
    ] == [
        "rss_mb",
    ]


def test_an_improvement_is_never_a_breach():
    baseline = {"k": {"median_ms": 10.0, "p95_ms": 11.0, "rss_mb": 50.0}}
    assert compare(baseline, {"k": {"median_ms": 5.0, "p95_ms": 6.0, "rss_mb": 25.0}}) == []


def test_a_category_absent_from_the_baseline_is_not_a_breach():
    assert compare({}, {"new": {"median_ms": 10.0}}) == []


# ---------------------------------------------------------------------------
# main() end to end -- argv in, exit code out
# ---------------------------------------------------------------------------


def _write(tmp_path, report, filename="bench_results_small.md"):
    path = tmp_path / filename
    path.write_text(render_report(report), encoding="utf-8")
    return path


def test_main_saves_a_baseline_then_passes_against_it(tmp_path):
    report_path = _write(tmp_path, _report())
    baseline = tmp_path / "baseline.json"

    assert main([str(report_path), "--baseline", str(baseline), "--save-baseline"]) == 0
    assert baseline.is_file()
    assert json.loads(baseline.read_text())

    assert main([str(report_path), "--baseline", str(baseline)]) == 0


def test_main_fails_on_a_regression(tmp_path):
    baseline = tmp_path / "baseline.json"
    main(
        [
            str(_write(tmp_path, _report(strata_median=10.0))),
            "--baseline",
            str(baseline),
            "--save-baseline",
        ]
    )

    slower = _write(tmp_path, _report(strata_median=20.0), filename="slower.md")
    # A different filename keys differently, so the entry must match to compare.
    assert main([str(slower), "--baseline", str(baseline)]) == 0

    same_name = _write(tmp_path, _report(strata_median=20.0))
    assert main([str(same_name), "--baseline", str(baseline)]) == 1


def test_main_passes_on_an_improvement(tmp_path):
    baseline = tmp_path / "baseline.json"
    main(
        [
            str(_write(tmp_path, _report(strata_median=20.0))),
            "--baseline",
            str(baseline),
            "--save-baseline",
        ]
    )
    assert (
        main([str(_write(tmp_path, _report(strata_median=10.0))), "--baseline", str(baseline)]) == 0
    )


def test_main_refuses_a_missing_report(tmp_path):
    assert main([str(tmp_path / "absent.md"), "--baseline", str(tmp_path / "b.json")]) == 2


def test_main_refuses_to_gate_without_a_baseline(tmp_path):
    report_path = _write(tmp_path, _report())
    assert main([str(report_path), "--baseline", str(tmp_path / "absent.json")]) == 2


def test_main_refuses_a_report_containing_error_rows(tmp_path):
    report_path = _write(tmp_path, _report(error="TypeError"))
    baseline = tmp_path / "baseline.json"
    baseline.write_text("{}", encoding="utf-8")
    assert main([str(report_path), "--baseline", str(baseline)]) == 2


def test_main_refuses_a_report_it_cannot_find_strata_in(tmp_path):
    """The exact failure that made the previous checker silently useless."""
    report = Report(name="bench_results_small.md")
    report.measurements.append(
        Measurement(
            section="loads",
            dataset="users.json",
            library="orjson",
            min_ms=1.0,
            median_ms=1.0,
            p95_ms=1.0,
            rss_mb=1.0,
        ),
    )
    report_path = _write(tmp_path, report)
    assert main([str(report_path), "--baseline", str(tmp_path / "b.json")]) == 2


def test_main_merges_into_an_existing_baseline(tmp_path):
    baseline = tmp_path / "baseline.json"
    baseline.write_text(json.dumps({"other|loads|x.json": {"median_ms": 1.0}}), encoding="utf-8")

    main([str(_write(tmp_path, _report())), "--baseline", str(baseline), "--save-baseline"])
    saved = json.loads(baseline.read_text())
    assert "other|loads|x.json" in saved  # unrelated entries survive
    assert baseline_key("bench_results_small.md", "loads", "users.json") in saved

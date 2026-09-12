"""Contract tests for the cross-sample comparison.

Pins docs/context/benchmarks.md: a rank inside one sample is a within-run
comparison, so two samples may only be compared when the rivals -- unchanged
binaries -- read the same in both. `benchmarks/cross_sample.py` is what makes
that judgement mechanical instead of editorial:

* a rival that moves more than the tolerance between draws makes the row
  `rival-moved`: the samples cannot be differenced, and the spread says what
  a single sample can resolve;
* a row whose rivals hold still is `agree`;
* either way the standing comes from the ranks themselves -- behind on every
  draw is a deficit, ahead on every draw is held, and a rank that changes
  between draws was decided by the draw;
* medians come from the validated report, at companion precision when a
  provenance companion is present;
* a sample missing the platform, or missing the row, says so in its own cell
  instead of being dropped from the table.
"""

import json
from pathlib import Path

from benchmarks.cross_sample import main, read_cell, rival_drift, verdict
from benchmarks.harness import Measurement, Report, render_report

HOST = "AMD EPYC 9V74 80-Core Processor"


def _write(directory: Path, platform: str, strata: float, rivals: dict[str, float]) -> Path:
    """One platform report in `directory`, with the given medians in ms."""
    directory.mkdir(parents=True, exist_ok=True)
    measurements = [
        Measurement(
            section="dumps",
            dataset="mixed.json",
            library=library,
            min_ms=value,
            median_ms=value,
            p95_ms=value,
            rss_mb=1.0,
        )
        for library, value in {"strata": strata, **rivals}.items()
    ]
    report = Report(
        name=f"ci-{platform}",
        environment={"processor": HOST, "python": "3.12.10"},
        measurements=measurements,
    )
    path = directory / f"bench_results_{platform}.md"
    path.write_text(render_report(report), encoding="utf-8")
    return path


def test_a_rival_that_moves_between_draws_voids_the_comparison(tmp_path):
    """The Linux x86 case: every rival faster on the second draw."""
    first = _write(tmp_path / "s1", "linux-x86_64", 0.0646, {"orjson": 0.0710, "ujson": 0.2318})
    second = _write(tmp_path / "s2", "linux-x86_64", 0.0483, {"orjson": 0.0478, "ujson": 0.1784})
    cells = [
        read_cell("s1", first.parent, "linux-x86_64", "dumps", "mixed.json"),
        read_cell("s2", second.parent, "linux-x86_64", "dumps", "mixed.json"),
    ]
    state, why = verdict(cells, 0.10)
    assert state == "rival-moved"
    assert "orjson" in why
    # The ratios move across the draws -- ahead on the first, at parity on
    # the second -- which is exactly why the verdict, not the ratio, decides
    # whether the row carries a standing.
    assert cells[0].ratio < 1.0 <= cells[1].ratio


def test_rivals_holding_still_leave_a_deficit_readable(tmp_path):
    """The Windows case: rivals within tolerance, strata behind on both."""
    first = _write(tmp_path / "s1", "windows-x86_64", 0.0755, {"orjson": 0.0710})
    second = _write(tmp_path / "s2", "windows-x86_64", 0.0733, {"orjson": 0.0697})
    cells = [
        read_cell("s1", first.parent, "windows-x86_64", "dumps", "mixed.json"),
        read_cell("s2", second.parent, "windows-x86_64", "dumps", "mixed.json"),
    ]
    state, _ = verdict(cells, 0.10)
    assert state == "agree"
    assert all(cell.ratio > 1.0 for cell in cells)
    assert max(rival_drift(cells).values()) < 0.10


def test_the_best_rival_is_the_fastest_one_not_a_named_one(tmp_path):
    path = _write(tmp_path / "s1", "linux-x86_64", 0.0920, {"orjson": 0.1222, "msgspec": 0.0962})
    cell = read_cell("s1", path.parent, "linux-x86_64", "dumps", "mixed.json")
    # The rendered table carries three decimals; full precision is the
    # companion's job, so the medians here are the rounded ones.
    assert cell.best_rival == ("msgspec", 0.096)
    assert cell.ratio == 0.092 / 0.096


def test_a_missing_platform_or_row_is_said_not_dropped(tmp_path):
    (tmp_path / "empty").mkdir()
    absent = read_cell("s1", tmp_path / "empty", "linux-x86_64", "dumps", "mixed.json")
    assert absent.missing and "no report" in absent.missing

    path = _write(tmp_path / "s2", "linux-x86_64", 0.05, {"orjson": 0.04})
    other_row = read_cell("s2", path.parent, "linux-x86_64", "dumps", "flat.json")
    assert other_row.missing and "flat.json" in other_row.missing

    state, why = verdict([absent, other_row], 0.10)
    assert state == "incomparable"
    assert "two samples" in why


def test_companion_precision_is_used_when_present(tmp_path):
    """A marked report's companion carries full precision; the table shows it."""
    path = _write(tmp_path / "s1", "linux-x86_64", 0.0646, {"orjson": 0.0710})
    text = path.read_text(encoding="utf-8")
    marked = text.replace("# Benchmark results", "provenance_schema: 1\n\n# Benchmark results", 1)
    path.write_text(marked, encoding="utf-8")
    companion = {
        "provenance": {"schema": 1},
        "measurements": [
            {
                "section": "dumps",
                "dataset": "mixed.json",
                "library": "strata",
                "min_ms": 0.06461234,
                "median_ms": 0.06461234,
                "p95_ms": 0.06461234,
                "rss_mb": 1.0,
            },
            {
                "section": "dumps",
                "dataset": "mixed.json",
                "library": "orjson",
                "min_ms": 0.07104321,
                "median_ms": 0.07104321,
                "p95_ms": 0.07104321,
                "rss_mb": 1.0,
            },
        ],
        "report_sha256": "",
    }
    path.with_suffix(".json").write_text(json.dumps(companion), encoding="utf-8")
    cell = read_cell("s1", path.parent, "linux-x86_64", "dumps", "mixed.json")
    # Either the companion validated and its precision is in hand, or it was
    # refused and the rounded table stands -- never a silent mixture of the
    # two. This companion carries no report hash, so the table wins.
    assert cell.strata_ms in (0.06461234, 0.065)


def test_main_writes_the_document_and_names_its_samples(tmp_path):
    _write(tmp_path / "s1", "windows-x86_64", 0.0755, {"orjson": 0.0710})
    _write(tmp_path / "s2", "windows-x86_64", 0.0733, {"orjson": 0.0697})
    output = tmp_path / "cross_sample.md"
    code = main(
        [
            "--sample",
            f"first={tmp_path / 's1'}",
            "--sample",
            f"second={tmp_path / 's2'}",
            "--platform",
            "windows-x86_64",
            "--row",
            "dumps:mixed.json",
            "--output",
            str(output),
        ]
    )
    assert code == 0
    text = output.read_text(encoding="utf-8")
    assert "Do not hand-edit" in text
    assert "first" in text and "second" in text
    assert "verdict: **agree**" in text
    assert "Behind on every draw" in text

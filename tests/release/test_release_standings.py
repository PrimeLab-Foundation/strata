"""scripts/release_post.py standings-check / standings-combine: the post-release rank gate.

Pins docs/architecture/release_pipeline.md, "Post-release verification": per
leg, strata is ranked within its own report only; a leg fails past
MAX_BEHIND_ROWS rows behind or any row past MAX_BEHIND_RATIO; a report that is
not gateable evidence is INVALID; the combined table fails unless all five
declared legs are present and passed. Fixture reports are rendered by the
harness itself, so the gate reads them exactly as it reads a CI report.
"""

from __future__ import annotations

import importlib.util
import json
from pathlib import Path

import pytest

from benchmarks.harness import CI_PLATFORMS, Measurement, Report, render_report, workload_rows

PROJECT_ROOT = Path(__file__).resolve().parents[2]
SCRIPT = PROJECT_ROOT / "scripts" / "release_post.py"

# platform.platform() and platform.machine() as each leg's runner reports them.
ENVIRONMENTS = {
    "linux-arm64": ("Linux-6.8.0-1014-azure-aarch64-with-glibc2.39", "aarch64"),
    "linux-x86_64": ("Linux-6.8.0-1014-azure-x86_64-with-glibc2.39", "x86_64"),
    "macos-arm64": ("macOS-26.3-arm64-arm-64bit-Mach-O", "arm64"),
    "macos-x86_64": ("macOS-15.7-x86_64-i386-64bit", "x86_64"),
    "windows-x86_64": ("Windows-2022Server-10.0.20348-SP0", "AMD64"),
}
ROWS = workload_rows()


def _load():
    spec = importlib.util.spec_from_file_location("release_post_standings", SCRIPT)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


post = _load()


def write_leg(directory: Path, leg: str, behind=None, errors=None) -> Path:
    """A complete 27-row report for LEG: strata #1 everywhere except ``behind`` (row -> ratio)."""
    platform, machine = ENVIRONMENTS[leg]
    report = Report(
        name=f"post-release-{leg}",
        environment={
            "commit": "6bf7a3f",
            "python": "3.12.10",
            "platform": platform,
            "machine": machine,
            "repeats": "10",
            "warmup": "2",
        },
    )
    for row in ROWS:
        ratio = (behind or {}).get(row)
        medians = {"strata": ratio or 1.0, "orjson": 1.0 if ratio else 1.2, "json": 3.0}
        for library, median in medians.items():
            report.measurements.append(
                Measurement(
                    *row,
                    library,
                    min_ms=median,
                    median_ms=median,
                    p95_ms=median,
                    rss_mb=10.0,
                ),
            )
    report.measurements.extend(errors or [])
    path = directory / f"bench_post_release_{leg}.md"
    path.write_text(render_report(report), encoding="utf-8")
    return path


def check_all(tmp_path: Path, **legs) -> tuple[list[int], Path]:
    """standings-check per leg, as each workflow leg runs it; returns the codes and summary dir."""
    summaries = tmp_path / "legs"
    summaries.mkdir()
    codes = []
    for leg in CI_PLATFORMS:
        report = write_leg(tmp_path, leg, **legs.get(leg.replace("-", "_"), {}))
        summary = summaries / f"post_release_standings_{leg}.json"
        codes.append(post.main(["standings-check", str(report), "--json", str(summary)]))
    return codes, summaries


def combine(summaries: Path, output: Path) -> int:
    return post.main(
        ["standings-combine", "--output", str(output), *map(str, sorted(summaries.glob("*.json")))],
    )


def test_a_clean_sweep_passes_on_every_leg_and_combined(tmp_path, capsys):
    codes, summaries = check_all(tmp_path)
    assert codes == [0] * 5
    document = json.loads((summaries / "post_release_standings_linux-x86_64.json").read_text())
    assert document["schema"] == "post-release-standings/1"
    assert document["gate"] == {"max_behind_rows": 3, "max_behind_ratio": 1.25, "workload": "ci"}
    (leg,) = document["legs"]
    assert (leg["leg"], leg["verdict"], leg["won"], leg["rows"], leg["behind"]) == (
        "linux-x86_64",
        "pass",
        27,
        27,
        [],
    )

    capsys.readouterr()
    output = tmp_path / "post_release_standings.md"
    assert combine(summaries, output) == 0
    text = output.read_text()
    for name in CI_PLATFORMS:
        assert f"| {name} | 27/27 | - | pass |" in text
    assert "Within-run ranks only" in capsys.readouterr().out


def test_the_coin_band_and_the_ratio_bound_are_inclusive(tmp_path):
    """Three rows behind, one of them at exactly 1.25x, is inside both bounds."""
    behind = {ROWS[0]: 1.25, ROWS[1]: 1.10, ROWS[2]: 1.01}
    code = post.main(["standings-check", str(write_leg(tmp_path, "windows-x86_64", behind))])
    assert code == 0


def test_a_leg_with_rows_behind_past_the_coin_band_fails(tmp_path, capsys):
    behind = {row: 1.05 for row in ROWS[:4]}
    codes, summaries = check_all(tmp_path, linux_x86_64={"behind": behind})
    assert codes == [0, 1, 0, 0, 0]
    out = capsys.readouterr().out
    assert "| linux-x86_64 | 23/27 | loads users.json 1.050x (orjson), " in out
    assert "4 rows behind, past the per-leg coin band of 3" in out

    assert combine(summaries, tmp_path / "combined.md") == 1
    text = (tmp_path / "combined.md").read_text()
    assert "| 23/27 |" in text and "| FAIL |" in text
    assert text.count("| pass |") == 4


def test_a_single_row_past_the_ratio_bound_fails(tmp_path, capsys):
    fat = ("dumps", "mixed.json")
    code = post.main(["standings-check", str(write_leg(tmp_path, "macos-arm64", {fat: 1.30}))])
    assert code == 1
    out = capsys.readouterr().out
    assert "| macos-arm64 | 26/27 | dumps mixed.json 1.300x (orjson) | FAIL |" in out
    assert "dumps mixed.json: 1.300x orjson, past the 1.25x bound" in out


def test_an_error_row_makes_the_report_not_gateable(tmp_path, capsys):
    error = Measurement("loads", "users.json", "ujson", error="ImportError")
    summary = tmp_path / "summary.json"
    report = write_leg(tmp_path, "linux-arm64", errors=[error])
    assert post.main(["standings-check", str(report), "--json", str(summary)]) == 2
    (leg,) = json.loads(summary.read_text())["legs"]
    assert leg["verdict"] == "INVALID"
    assert any("ERROR" in problem or "ujson" in problem for problem in leg["problems"])
    assert "| INVALID |" in capsys.readouterr().out


def test_a_missing_report_is_invalid_and_keeps_its_leg_name(tmp_path, capsys):
    missing = tmp_path / "bench_post_release_macos-x86_64.md"
    assert post.main(["standings-check", str(missing)]) == 2
    assert "| macos-x86_64 | 0/27 | - | INVALID |" in capsys.readouterr().out


def test_a_report_measured_on_another_leg_is_invalid(tmp_path):
    report = write_leg(tmp_path, "linux-x86_64")
    renamed = report.with_name("bench_post_release_linux-arm64.md")
    report.rename(renamed)
    assert post.main(["standings-check", str(renamed)]) == 2


def test_a_missing_leg_fails_the_combined_table(tmp_path):
    _, summaries = check_all(tmp_path)
    (summaries / "post_release_standings_windows-x86_64.json").unlink()
    assert combine(summaries, tmp_path / "combined.md") == 1
    assert "| windows-x86_64 | 0/27 | - | MISSING |" in (tmp_path / "combined.md").read_text()


def test_no_summaries_at_all_reads_five_missing_legs(tmp_path):
    assert post.main(["standings-combine", "--output", str(tmp_path / "combined.md")]) == 1
    assert (tmp_path / "combined.md").read_text().count("| MISSING |") == 5


@pytest.mark.parametrize("command", ["standings-check", "standings-combine"])
def test_help_states_a_within_run_gate_that_never_compares_runs(command, capsys):
    with pytest.raises(SystemExit):
        post.main([command, "--help"])
    out = " ".join(capsys.readouterr().out.split())
    assert "within-run rank gate" in out
    assert "never compares a time with another run's" in out

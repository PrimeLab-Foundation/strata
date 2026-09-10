"""Contract tests for the one A/B estimator and its recoverable driver.

The A/B instrument decides which serializer changes are accepted, so it is
tested the way the regression gate is: through a fixture whose answer is known
before the analysis runs, and through the failure modes that previously
produced a confident wrong number (`docs/performance/ci-review-2026-09-07.md`,
findings 4 and 5).

The synthetic session below carries, on purpose:

* a **known injected effect** — arm B is exactly 4% slower than arm A on
  `sm-flat-dumps`, and exactly equal on the control row;
* **linear host drift** — each launch is 1.5% of the baseline time slower
  than the one before it, on *both* engines, which is exactly what the ABBA
  block cancels (the two A launches and the two B launches of a block share a
  midpoint) and what the in-process rival cancels a second time. A
  drift-blind estimator reads the effect wrong; the block estimator must
  recover +4.00% and the trailing baseline must still report all +18% of the
  session's drift.

The rejected fixtures — a launch missing its rival row, a third build label, a
launch out of order, a non-finite sample — must not produce a number at all.
"""

from __future__ import annotations

import argparse
import math
from pathlib import Path

import pytest

from benchmarks import ab_blocks, ab_builds, ab_floor, ab_rows

ORDER = "ABBAABBAABBAA"  # three complete blocks and a trailing baseline
DRIFT = 0.015  # per launch, applied to both engines
EFFECT = 0.04  # arm B's cost on the affected row
ROWS = {"sm-flat-dumps": EFFECT, "sm-wide_arrays-loads": 0.0}
ENGINE_RIVAL = {"sm-flat-dumps": ("strata-bytes", "orjson-bytes")}
CONTROL_RIVAL = {"sm-wide_arrays-loads": ("strata-loads", "orjson-loads")}
SAMPLES = 12


def _session(
    order: str = ORDER,
    *,
    effect: float = EFFECT,
    rival_effect: float = 0.0,
    drift: float = DRIFT,
    drop_rival_at: str | None = None,
    start: int = 0,
    baseline: str = "A",
    samples: int = SAMPLES,
) -> list[str]:
    """A TSV body whose true block effect is `effect` on the affected row.

    `rival_effect` moves orjson too, which is what a shared-process effect
    looks like: raw strata and normalised then disagree, and the packet has to
    show both. `start` continues a session, so two campaigns can be
    concatenated into one three-arm file.
    """
    lines = ["\t".join(ab_blocks.TSV_COLUMNS)] if start == 0 else []
    for offset, build in enumerate(order):
        index = start + offset
        tag = f"L{index:02d}"
        host = 1.0 + drift * index  # linear: exactly what an ABBA block cancels
        candidate = build != baseline
        for row in ROWS:
            engine, rival = {**ENGINE_RIVAL, **CONTROL_RIVAL}[row]
            row_effect = effect if row in ENGINE_RIVAL else 0.0
            strata_ms = 1.0 * host * ((1.0 + row_effect) if candidate else 1.0)
            rival_ms = 2.0 * host * ((1.0 + rival_effect) if candidate else 1.0)
            for number in range(samples):
                # A deterministic ripple, so the medians are not identical
                # across launches and the bootstrap has something to resample.
                jitter = 1.0 + 0.001 * ((number % 5) - 2)
                lines.append(f"{tag}\t{build}\t{engine}\t{row}\t{number}\t{strata_ms * jitter:.9f}")
                if tag == drop_rival_at and row in ENGINE_RIVAL:
                    continue
                lines.append(f"{tag}\t{build}\t{rival}\t{row}\t{number}\t{rival_ms * jitter:.9f}")
    return lines


def _write(tmp_path, lines, name="R1.tsv"):
    path = tmp_path / name
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return path


# --- the analysis recovers the injected effect ------------------------------


def test_block_effect_recovers_the_injected_effect(tmp_path):
    """The known +4% survives 1.5%-per-launch drift (finding 4's estimator)."""
    analysis = ab_blocks.analyze(_write(tmp_path, _session()), min_samples=SAMPLES)
    series = analysis.find("sm-flat-dumps", "strata-bytes")
    assert series is not None
    assert series.blocks == 3
    assert series.effect == pytest.approx(EFFECT, abs=1e-4)
    assert series.interval.low <= EFFECT <= series.interval.high
    assert series.positive == 3


def test_control_row_reads_zero_and_uses_its_own_rival(tmp_path):
    """`loads` rows are read against orjson-loads, not the dumps rival.

    `ab_floor.py` used to default to `--rival orjson-bytes`, so the `loads`
    rows produced no normalised reading at all (review finding 4).
    """
    analysis = ab_blocks.analyze(_write(tmp_path, _session()), min_samples=SAMPLES)
    series = analysis.find("sm-wide_arrays-loads", "strata-loads")
    assert series is not None
    assert series.rival == "orjson-loads"
    assert series.effect == pytest.approx(0.0, abs=1e-6)


def test_raw_and_normalised_are_reported_from_one_packet(tmp_path):
    """A rival that moved on its own must be visible, not normalised away.

    Review finding 4: "a rival's movement need not be pure host drift when
    both engines share process and allocator state". Here orjson is 2% slower
    under arm B as well; the raw strata change is +4%, the raw rival change is
    +2%, and the normalised effect is the ~+1.96% ratio of the two. All three
    come from one packet so a reader can tell which happened.
    """
    path = _write(tmp_path, _session(rival_effect=0.02))
    series = ab_blocks.analyze(path, min_samples=SAMPLES).find("sm-flat-dumps", "strata-bytes")
    assert series.strata_change == pytest.approx(EFFECT, abs=1e-4)
    assert series.rival_change == pytest.approx(0.02, abs=1e-4)
    assert series.effect == pytest.approx(1.04 / 1.02 - 1, abs=1e-4)
    rendered = ab_blocks.render(ab_blocks.analyze(path, min_samples=SAMPLES))
    assert "raw strata" in rendered and "raw rival" in rendered and "normalised" in rendered


def test_the_block_cancels_linear_host_drift(tmp_path):
    """Without a real effect the blocks read zero, drift and all."""
    path = _write(tmp_path, _session(effect=0.0, drift=0.05), name="drifty.tsv")
    series = ab_blocks.analyze(path, min_samples=SAMPLES).find("sm-flat-dumps", "strata-bytes")
    assert series.strata_change == pytest.approx(0.0, abs=1e-9)
    assert series.effect == pytest.approx(0.0, abs=1e-9)
    # The drift itself is not hidden: the trailing baseline reports all of it.
    assert series.drift_strata == pytest.approx(12 * 0.05, rel=1e-6)


def test_aa_control_reads_zero_and_supplies_a_floor(tmp_path):
    """The same estimator on a no-effect session is the floor."""
    control = ab_blocks.analyze(
        _write(tmp_path, _session(effect=0.0), name="AA.tsv"), min_samples=SAMPLES
    )
    aa = control.find("sm-flat-dumps", "strata-bytes")
    assert aa.effect == pytest.approx(0.0, abs=1e-6)
    assert aa.interval.floor < 0.001


def test_trailing_baseline_is_kept_as_a_drift_diagnostic(tmp_path):
    """The trailing A is not a block; it is the session's measured drift."""
    analysis = ab_blocks.analyze(_write(tmp_path, _session()), min_samples=SAMPLES)
    assert [launch.tag for launch in analysis.structure.trailing] == ["L12"]
    series = analysis.find("sm-flat-dumps", "strata-bytes")
    assert series.drift_rival == pytest.approx(12 * DRIFT, rel=1e-6)
    assert series.drift_normalised == pytest.approx(0.0, abs=1e-9)


# --- the bad fixtures are rejected -----------------------------------------


def test_missing_rival_row_is_rejected(tmp_path):
    path = _write(tmp_path, _session(drop_rival_at="L05"))
    with pytest.raises(ab_blocks.AnalysisError, match="L05.*no .*orjson-bytes"):
        ab_blocks.analyze(path, min_samples=SAMPLES)


def test_a_third_build_label_is_rejected_unless_a_pair_is_named(tmp_path):
    """Three source arms are three pairwise comparisons, never a pooled B.

    `ab_blocks` used to treat every non-A build as B, so a third arm would
    have been averaged into the candidate without a word (review finding 4).
    """
    lines = _session("ABBA", effect=EFFECT)
    lines += _session("ACCA", effect=0.09, start=4)
    path = _write(tmp_path, lines, name="three.tsv")
    with pytest.raises(ab_blocks.AnalysisError, match="build labels"):
        ab_blocks.analyze(path, min_samples=SAMPLES)

    against_b = ab_blocks.analyze(path, pair=("A", "B"), min_samples=SAMPLES)
    assert against_b.structure.other == "B"
    assert against_b.find("sm-flat-dumps", "strata-bytes").effect == pytest.approx(EFFECT, abs=1e-4)
    against_c = ab_blocks.analyze(path, pair=("A", "C"), min_samples=SAMPLES)
    assert against_c.structure.other == "C"
    assert against_c.find("sm-flat-dumps", "strata-bytes").effect == pytest.approx(0.09, abs=1e-4)


def test_misordered_launches_are_rejected(tmp_path):
    path = _write(tmp_path, _session("ABABABAB"), name="bad_order.tsv")
    with pytest.raises(ab_blocks.AnalysisError, match="out of ABBA order"):
        ab_blocks.analyze(path, min_samples=SAMPLES)


def test_a_trailing_candidate_launch_is_rejected(tmp_path):
    path = _write(tmp_path, _session("ABBAB"), name="bad_tail.tsv")
    with pytest.raises(ab_blocks.AnalysisError, match="out of ABBA order"):
        ab_blocks.analyze(path, min_samples=SAMPLES)


def test_non_finite_and_non_positive_samples_are_rejected(tmp_path):
    for bad in ("nan", "inf", "0.0", "-1.0"):
        lines = _session()
        lines[3] = "\t".join(lines[3].split("\t")[:-1] + [bad])
        path = _write(tmp_path, lines, name=f"bad_{bad}.tsv")
        with pytest.raises(ab_blocks.AnalysisError, match="positive finite"):
            ab_blocks.analyze(path, min_samples=SAMPLES)


def test_a_short_launch_is_rejected(tmp_path):
    path = _write(tmp_path, _session())
    with pytest.raises(ab_blocks.AnalysisError, match="fewer than the required"):
        ab_blocks.analyze(path, min_samples=SAMPLES + 1)


def test_a_bad_header_is_rejected(tmp_path):
    lines = _session()
    lines[0] = "tag\tbuild\tengine\trow\tms"
    with pytest.raises(ab_blocks.AnalysisError, match="header is"):
        ab_blocks.analyze(_write(tmp_path, lines, name="bad_header.tsv"))


def test_an_unknown_engine_is_rejected(tmp_path):
    lines = [line.replace("strata-bytes", "strata-cursor") for line in _session()]
    with pytest.raises(ab_blocks.AnalysisError, match="no rival is defined"):
        ab_blocks.analyze(_write(tmp_path, lines, name="unknown.tsv"), min_samples=SAMPLES)


# --- the views cannot disagree with the estimator ---------------------------


def test_ab_floor_and_ab_builds_print_the_estimator_s_numbers(tmp_path, capsys):
    """One analysis, three front ends (review finding 4)."""
    path = _write(tmp_path, _session())
    series = ab_blocks.analyze(path, min_samples=SAMPLES).find("sm-flat-dumps", "strata-bytes")
    expected = f"{series.effect * 100:.2f}"

    assert ab_floor.main([str(path), "--min-samples", str(SAMPLES)]) == 0
    floor_text = capsys.readouterr().out
    assert expected in floor_text
    assert "orjson-loads" in floor_text  # the loads row has a rival here too

    assert (
        ab_builds.analyze(
            argparse.Namespace(analyze=str(path), baseline_build="A", min_samples=SAMPLES)
        )
        == 0
    )
    builds_text = capsys.readouterr().out
    assert "launch medians" in builds_text
    assert expected in builds_text


def test_the_cli_reports_the_aa_floor_beside_the_effect(tmp_path, capsys):
    candidate = _write(tmp_path, _session())
    control = _write(tmp_path, _session(effect=0.0), name="AA.tsv")
    out = tmp_path / "analysis.json"
    assert (
        ab_blocks.main(
            [
                str(candidate),
                "--aa",
                str(control),
                "--min-samples",
                str(SAMPLES),
                "--json",
                str(out),
            ]
        )
        == 0
    )
    text = capsys.readouterr().out
    assert "A/A" in text and "floor" in text
    payload = out.read_text(encoding="utf-8")
    assert '"aa_floor"' in payload and '"normalised"' in payload


def test_the_bootstrap_is_deterministic(tmp_path):
    path = _write(tmp_path, _session())
    first = ab_blocks.analyze(path, min_samples=SAMPLES).find("sm-flat-dumps", "strata-bytes")
    second = ab_blocks.analyze(path, min_samples=SAMPLES).find("sm-flat-dumps", "strata-bytes")
    assert (first.interval.low, first.interval.high) == (second.interval.low, second.interval.high)
    assert math.isfinite(first.interval.low)


# --- the driver keeps its evidence and puts the extension back --------------


def _fake_target(tmp_path):
    target = tmp_path / "python" / "strata" / "_strata.so"
    target.parent.mkdir(parents=True)
    target.write_bytes(b"the extension that was installed")
    return target


def _arms(tmp_path):
    arms = {}
    for tag, payload in (("A", b"arm A" * 8), ("B", b"arm B" * 8)):
        path = tmp_path / f"{tag}.so"
        path.write_bytes(payload)
        arms[tag] = path
    return arms


def test_driver_restores_the_original_extension_after_a_successful_run(tmp_path, monkeypatch):
    """A finished campaign must not leave one of its arms installed."""
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", tmp_path)
    target = _fake_target(tmp_path)
    original = target.read_bytes()
    arms = _arms(tmp_path)
    out = tmp_path / "run.tsv"
    seen = []

    def launch(index, tag):
        seen.append((index, tag, target.read_bytes()))
        return [f"L{index:02d}\t{tag}\tstrata-bytes\trow\t0\t1.0"]

    ab_builds.drive(list("ABBAA"), arms, target, out, launch)

    assert target.read_bytes() == original
    assert not (target.parent / (target.name + ".ab_original")).exists()
    assert [tag for _, tag, _ in seen] == list("ABBAA")
    assert [payload for _, _, payload in seen] == [arms[tag].read_bytes() for tag in "ABBAA"]
    assert out.read_text(encoding="utf-8").splitlines()[0].split("\t") == list(
        ab_builds.ab_rounds.TSV_HEADER
    )
    assert len(out.read_text(encoding="utf-8").splitlines()) == 6


def test_driver_keeps_earlier_launches_and_restores_after_a_failure(tmp_path, monkeypatch):
    """A failed launch 3 keeps launches 0-2 and still puts the tree back."""
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", tmp_path)
    target = _fake_target(tmp_path)
    original = target.read_bytes()
    arms = _arms(tmp_path)
    out = tmp_path / "run.tsv"

    def launch(index, tag):
        if index == 3:
            raise SystemExit("launch 03 (A) failed with 1")
        return [f"L{index:02d}\t{tag}\tstrata-bytes\trow\t{number}\t1.0" for number in range(2)]

    with pytest.raises(SystemExit):
        ab_builds.drive(list("ABBAA"), arms, target, out, launch)

    assert target.read_bytes() == original
    body = out.read_text(encoding="utf-8").splitlines()
    assert body[0].split("\t") == list(ab_builds.ab_rounds.TSV_HEADER)
    assert len(body) == 1 + 3 * 2  # three completed launches, two samples each
    assert all(line.split("\t")[0] in {"L00", "L01", "L02"} for line in body[1:])


def test_driver_refuses_to_run_inside_a_process_that_imported_the_target(tmp_path, monkeypatch):
    """This very process has strata loaded; a driver here would measure it."""
    import strata._strata as native

    target = Path(native.__file__)
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", target.resolve().parents[3])
    with pytest.raises(SystemExit, match="already imported"):
        ab_builds.drive(list("ABBA"), _arms(tmp_path), target, tmp_path / "x.tsv", lambda i, t: [])


def test_driver_runs_when_the_target_is_not_the_loaded_extension(tmp_path, monkeypatch):
    """The guard names the actual conflict, not any import called strata."""
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", tmp_path)
    target = _fake_target(tmp_path)
    ab_builds.drive(["A"], _arms(tmp_path), target, tmp_path / "one.tsv", lambda index, tag: [])
    assert target.read_bytes() == b"the extension that was installed"


def test_driver_refuses_a_target_outside_the_checkout(tmp_path, monkeypatch):
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", tmp_path / "checkout")
    (tmp_path / "checkout").mkdir()
    outside = tmp_path / "elsewhere.so"
    outside.write_bytes(b"x")
    with pytest.raises(SystemExit, match="refusing to write outside"):
        ab_builds.InstalledExtension(outside)


# --- the probe identifies itself on every platform -------------------------


def test_machine_state_is_portable(monkeypatch):
    """`os.getloadavg` is Unix-only; the Windows leg must still identify."""
    from benchmarks import rows_probe

    assert rows_probe.machine_state().startswith("load=")
    monkeypatch.delattr("os.getloadavg", raising=False)
    assert rows_probe.machine_state().startswith("load=unavailable")

    def raise_os_error():
        raise OSError("no load average")

    monkeypatch.setattr("os.getloadavg", raise_os_error, raising=False)
    assert rows_probe.machine_state() == "load=unavailable"


def test_every_probe_engine_has_a_rival_in_the_analysis():
    """A row the probe can measure must be a row the analysis can read."""
    from benchmarks import rows_probe

    assert set(rows_probe.SUFFIX_BY_OP) == {"dumps", "loads", "load", "ndload", "dump"}
    for engine in (
        "strata-bytes",
        "strata-str",
        "strata-loads",
        "strata-load",
        "strata-ndload",
        "strata-dump",
    ):
        assert engine in ab_blocks.RIVAL_BY_ENGINE


# --- one minimum, one verdict on a packet's validity ------------------------


def test_every_front_end_shares_one_min_samples_default():
    """Three views of one packet must agree on whether it is readable.

    `ab_builds.analyze` and `ab_rows.run` used to default to one sample per
    launch while `ab_blocks` and `ab_floor` demanded ten, so a short packet
    was refused by two tools and reported by the third (the 7 September
    review's T3 follow-up, defect 2).
    """
    default = ab_blocks.DEFAULT_MIN_SAMPLES
    assert default == 10
    for module in (ab_blocks, ab_floor, ab_builds, ab_rows):
        assert module.build_parser().get_default("min_samples") == default


def test_a_short_packet_is_refused_by_every_front_end(tmp_path):
    """Five samples a launch, read at the shared default: nobody accepts it."""
    path = _write(tmp_path, _session(samples=5), name="short.tsv")

    with pytest.raises(ab_blocks.AnalysisError, match="fewer than the required 10"):
        ab_blocks.main([str(path)])
    with pytest.raises(ab_blocks.AnalysisError, match="fewer than the required 10"):
        ab_floor.main([str(path)])
    with pytest.raises(ab_blocks.AnalysisError, match="fewer than the required 10"):
        ab_builds.main(["--analyze", str(path)])
    # The same call without a `min_samples` attribute at all: the fallback in
    # `ab_builds.analyze` is the shared default, not the old 1.
    with pytest.raises(ab_blocks.AnalysisError, match="fewer than the required 10"):
        ab_builds.analyze(argparse.Namespace(analyze=str(path), baseline_build="A"))


# --- a short A/A control announces that it is one ---------------------------


def test_a_two_block_control_is_announced_as_a_weak_floor(tmp_path, capsys):
    """A floor from two blocks is not a 95% bound and must not read as one."""
    candidate = _write(tmp_path, _session())
    control = _write(tmp_path, _session("ABBAABBAA", effect=0.0), name="AA.tsv")
    assert (
        ab_blocks.main([str(candidate), "--aa", str(control), "--min-samples", str(SAMPLES)]) == 0
    )
    captured = capsys.readouterr()
    assert "WARNING: the A/A control has 2 blocks" in captured.out
    assert "51% coverage" in captured.out
    assert "WARNING" in captured.err  # and on stderr, where a CI log keeps it
    assert "max(|ci low|, |ci high|)" in captured.out  # not "the wider half"


def test_a_four_block_control_carries_no_warning(tmp_path, capsys):
    control = _write(tmp_path, _session("ABBA" * 4 + "A", effect=0.0), name="AA4.tsv")
    candidate = _write(tmp_path, _session())
    assert (
        ab_blocks.main([str(candidate), "--aa", str(control), "--min-samples", str(SAMPLES)]) == 0
    )
    captured = capsys.readouterr()
    assert "A/A control" in captured.out
    assert "WARNING" not in captured.out and "WARNING" not in captured.err


def test_ab_floor_warns_on_its_own_short_control(tmp_path, capsys):
    control = _write(tmp_path, _session("ABBAABBAA", effect=0.0), name="AA.tsv")
    assert ab_floor.main([str(control), "--min-samples", str(SAMPLES)]) == 0
    text = capsys.readouterr().out
    assert "WARNING: the A/A control has 2 blocks" in text
    assert "max(|ci low|, |ci high|)" in text


# --- --pair says what it dropped -------------------------------------------


def test_pair_names_the_launches_it_dropped(tmp_path):
    """A gap in the tag sequence is not an account of what was left out."""
    lines = _session("ABBA", effect=EFFECT)
    lines += _session("ACCA", effect=0.09, start=4)
    path = _write(tmp_path, lines, name="three.tsv")

    analysis = ab_blocks.analyze(path, pair=("A", "C"), min_samples=SAMPLES)
    assert [launch.tag for launch in analysis.structure.dropped] == ["L01", "L02"]
    launches = [
        line for line in ab_blocks.render(analysis).splitlines() if line.startswith("launches:")
    ]
    assert launches == [
        "launches: L00:A L01:B(dropped) L02:B(dropped) L03:A L04:A L05:C L06:C L07:A"
    ]
    assert ab_blocks.to_json(analysis)["dropped"] == [
        {"tag": "L01", "build": "B"},
        {"tag": "L02", "build": "B"},
    ]


# --- a killed campaign's original survives the next run ---------------------


def test_a_stale_backup_matching_the_target_is_reused(tmp_path, monkeypatch, capsys):
    """`.ab_original` equal to the installed file is scrap from a finished run."""
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", tmp_path)
    target = _fake_target(tmp_path)
    original = target.read_bytes()
    backup = target.with_name(target.name + ".ab_original")
    backup.write_bytes(original)

    ab_builds.drive(list("ABBAA"), _arms(tmp_path), target, tmp_path / "run.tsv", lambda i, t: [])

    assert target.read_bytes() == original
    assert not backup.exists()
    assert "matches the installed extension" in capsys.readouterr().err


def test_a_killed_campaigns_original_is_restored_not_clobbered(tmp_path, monkeypatch, capsys):
    """The state a `kill -9` leaves: an arm installed, the product aside.

    The driver used to copy the target over that backup unconditionally, so
    the next run saved the *arm* as the original, "restored" it and deleted
    the only copy of the product (the 7 September review's T3 follow-up,
    defect 1).
    """
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", tmp_path)
    target = _fake_target(tmp_path)
    original = target.read_bytes()
    arms = _arms(tmp_path)
    backup = target.with_name(target.name + ".ab_original")
    backup.write_bytes(original)
    target.write_bytes(arms["B"].read_bytes())  # the killed run's arm, still installed

    ab_builds.drive(list("ABBAA"), arms, target, tmp_path / "run.tsv", lambda i, t: [])

    assert target.read_bytes() == original
    assert not backup.exists()
    message = capsys.readouterr().err
    assert "killed before it could restore" in message
    # and what it put back at the end is the product, not the arm it found
    assert f"restored {target} to md5={ab_builds._digest(target)}" in message


def test_the_recovered_original_is_what_a_failing_run_puts_back(tmp_path, monkeypatch):
    """Recovery then failure: the product is still the product afterwards."""
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", tmp_path)
    target = _fake_target(tmp_path)
    original = target.read_bytes()
    arms = _arms(tmp_path)
    target.with_name(target.name + ".ab_original").write_bytes(original)
    target.write_bytes(arms["A"].read_bytes())

    def launch(index, tag):
        if index == 2:
            raise SystemExit("launch 02 (B) failed with 1")
        return [f"L{index:02d}\t{tag}\tstrata-bytes\trow\t0\t1.0"]

    with pytest.raises(SystemExit):
        ab_builds.drive(list("ABBAA"), arms, target, tmp_path / "run.tsv", launch)

    assert target.read_bytes() == original
    assert not target.with_name(target.name + ".ab_original").exists()


@pytest.mark.parametrize("case", ["missing", "hash", "abi", "valid"])
def test_build_preflight_rejects_unverified_or_incompatible_arms(tmp_path, case):
    import hashlib
    import json
    import sys

    arm = tmp_path / "B.so"
    arm.write_bytes(b"a native build")
    version = f"{sys.version_info.major}.{sys.version_info.minor}"
    metadata = {
        "extension_sha256": hashlib.sha256(arm.read_bytes()).hexdigest(),
        "commands": [[f"-I/include/python{version}"]],
    }
    if case == "hash":
        metadata["extension_sha256"] = "incorrect"
    elif case == "abi":
        metadata["commands"] = [["-I/include/python3.99"]]
    if case != "missing":
        arm.with_name("B.so.build.json").write_text(json.dumps(metadata))
    if case == "valid":
        assert ab_builds.parse_builds([f"B={arm}"]) == {"B": arm.resolve()}
    else:
        with pytest.raises(SystemExit):
            ab_builds.parse_builds([f"B={arm}"])


def test_the_build_identity_travels_with_the_swapped_extension(tmp_path, monkeypatch):
    """provenance.capture reads the sidecar beside the binary: it must describe that binary."""
    monkeypatch.setattr(ab_builds, "PROJECT_ROOT", tmp_path)
    target = _fake_target(tmp_path)
    sidecar = target.with_name(target.name + ".build.json")
    sidecar.write_text('{"extension_sha256": "original"}')
    arms = _arms(tmp_path)
    arms["B"].with_name(arms["B"].name + ".build.json").write_text('{"extension_sha256": "B"}')

    installed = ab_builds.InstalledExtension(target)
    installed.install(arms["B"])
    assert sidecar.read_text() == '{"extension_sha256": "B"}'
    installed.install(arms["A"])
    assert not sidecar.exists(), "an arm without an identity leaves none behind"
    installed.restore()
    assert sidecar.read_text() == '{"extension_sha256": "original"}'
    assert not sidecar.with_name(sidecar.name + ".ab_original").exists()

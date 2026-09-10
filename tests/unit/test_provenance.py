"""Raw evidence must survive transport and cannot silently become legacy data."""

import json

import pytest

from benchmarks import ci_fetch
from benchmarks.harness import Report, read_report, summarize, validate_report
from benchmarks.provenance import companion, write_report
from scripts.build_identity import file_hash, write_identity


def sample_report():
    report = Report("probe", {"commit": "abc1234", "platform": "Linux-6", "machine": "x86_64"})
    report.measurements = [
        summarize("loads", "tiny.json", "strata", [0.00011, 0.00012, 0.00013], 10)
    ]
    report.provenance = {
        "protocol": {"repeat": 3},
        "extension": {"sha256": "binary", "build": None},
    }
    report.excluded = {"orjson": "not installed"}
    return report


def test_full_precision_survives_markdown_rounding(tmp_path):
    path = tmp_path / "report.md"
    write_report(path, sample_report())
    loaded = read_report(path)
    assert validate_report(loaded).ok
    assert loaded.measurements[0].median_ms == 0.00012
    assert loaded.excluded == {"orjson": "not installed"}


@pytest.mark.parametrize("defect", ["missing", "hash", "order", "samples", "binary", "commit"])
def test_corrupt_sidecar_cannot_pass(tmp_path, defect):
    path = tmp_path / "report.md"
    write_report(path, sample_report())
    sidecar = companion(path)
    data = json.loads(sidecar.read_text())
    if defect == "missing":
        sidecar.unlink()
    else:
        if defect == "hash":
            data["report_sha256"] = "bad"
        elif defect == "order":
            data["samples"].reverse()
        elif defect == "samples":
            data["measurements"][0]["samples_ms"][0] = float("nan")
        else:
            data["provenance"]["extension"]["build"] = {
                "schema_version": 1,
                "extension_sha256": "bad" if defect == "binary" else "binary",
                "source": {"commit": "different"},
            }
        sidecar.write_text(json.dumps(data))
    assert not validate_report(read_report(path)).ok


def test_fetch_retains_sidecar_and_failed_write_retains_previous(tmp_path, monkeypatch):
    scratch = tmp_path / "download"
    scratch.mkdir()
    source = scratch / "report.md"
    write_report(source, sample_report())
    destination = tmp_path / "ci"
    ci_fetch._place({"linux-x86_64": source}, {}, destination, scratch)
    fetched = destination / "bench_results_linux-x86_64.md"
    assert validate_report(read_report(fetched)).ok
    before = {p.name: p.read_bytes() for p in destination.iterdir()}
    original = ci_fetch._write_text

    def fail(path, text):
        if path.suffix == ".md":
            raise OSError("injected placement failure")
        original(path, text)

    monkeypatch.setattr(ci_fetch, "_write_text", fail)
    with pytest.raises(OSError, match="injected"):
        ci_fetch._place({"linux-x86_64": source}, {}, destination, scratch)
    assert before == {p.name: p.read_bytes() for p in destination.iterdir()}


def test_incremental_build_preserves_original_identity(tmp_path):
    extension = tmp_path / "extension.so"
    extension.write_bytes(b"old binary")
    path = extension.with_name(extension.name + ".build.json")
    original = {"extension_sha256": file_hash(extension), "source": {"commit": "original"}}
    path.write_text(json.dumps(original))
    write_identity(extension, root=tmp_path, commands=[], profile=None)
    assert json.loads(path.read_text()) == original


def test_unknown_incremental_binary_has_no_invented_source(tmp_path):
    extension = tmp_path / "extension.so"
    extension.write_bytes(b"unknown binary")
    write_identity(extension, root=tmp_path, commands=[], profile=None)
    data = json.loads(extension.with_name(extension.name + ".build.json").read_text())
    assert data["source"] is None
    assert data["limitation"]


def test_regression_cli_uses_raw_values_and_rejects_missing_companion(tmp_path):
    from benchmarks import regression_check
    from benchmarks.harness import baseline_key

    path = tmp_path / "report.md"
    report = sample_report()
    write_report(path, report)
    baseline = tmp_path / "baseline.json"
    key = baseline_key(path.name, "loads", "tiny.json")
    baseline.write_text(json.dumps({key: {"median_ms": 0.0001, "p95_ms": 0.0001, "rss_mb": 10}}))
    arguments = [str(path), "--expect", "none", "--baseline", str(baseline)]
    assert regression_check.main(arguments) == 1
    companion(path).unlink()
    assert regression_check.main(arguments) == 2


@pytest.mark.parametrize("argument", ['src/a file.cpp', '/Tpsrc/a file.cpp', '/Tp"src/a file.cpp"'])
def test_compiler_source_arguments_include_windows_prefixes(tmp_path, monkeypatch, argument):
    from scripts import build_identity

    monkeypatch.setattr(build_identity, "command_output", lambda *args, **kwargs: None)
    binary = tmp_path / "extension.pyd"
    binary.write_bytes(b"new binary")
    source = {"commit": "a" * 40, "dirty": False}
    write_identity(
        binary,
        root=tmp_path,
        commands=[["cl.exe", argument]],
        profile=None,
        source=source,
        required_sources=["src/a file.cpp"],
    )
    data = json.loads(binary.with_name(binary.name + ".build.json").read_text())
    assert data["complete_compilation"] is True
    assert data["source"] == source


def test_missing_clean_source_commit_is_invalid(tmp_path):
    path = tmp_path / "report.md"
    report = sample_report()
    report.provenance["extension"]["build"] = {
        "schema_version": 1,
        "extension_sha256": "binary",
        "source": {"dirty": False},
    }
    write_report(path, report)
    assert not validate_report(read_report(path)).ok


def test_a_failed_diff_is_not_a_clean_tree(monkeypatch, tmp_path):
    """A timed-out or failing `git diff` leaves cleanliness unknown, never False."""
    from scripts import build_identity

    def fake(command, **kwargs):
        if command[1:3] == ["rev-parse", "--show-toplevel"]:
            return str(tmp_path)
        if command[1:3] == ["rev-parse", "HEAD"]:
            return "a" * 40
        if command[1] == "diff":
            return None
        return ""

    monkeypatch.setattr(build_identity, "command_output", fake)
    identity = build_identity.source_identity(tmp_path)
    assert identity["commit"] == "a" * 40
    assert identity["dirty"] is None and identity["patch_sha256"] is None
    assert "unknown" in identity["limitation"]


def test_an_empty_diff_is_a_clean_tree(monkeypatch, tmp_path):
    from scripts import build_identity

    def fake(command, **kwargs):
        if command[1:3] == ["rev-parse", "--show-toplevel"]:
            return str(tmp_path)
        if command[1:3] == ["rev-parse", "HEAD"]:
            return "b" * 40
        return ""

    monkeypatch.setattr(build_identity, "command_output", fake)
    identity = build_identity.source_identity(tmp_path)
    assert identity["dirty"] is False and identity["untracked_sha256"] == {}


def test_the_report_flag_line_is_a_digest_without_paths():
    """The Markdown line names the options once; the commands stay in the companion."""
    from benchmarks.bench_main import _flag_digest

    commands = [
        [
            "clang++",
            "-I/usr/include/python3.14",
            "-O3",
            "-c",
            "src/strata/util/scan.cpp",
            "-o",
            "/tmp/build/scan.o",
            "-arch",
            "arm64",
            "-std=c++20",
        ],
        ["clang++", "-O3", "-flto=thin", "/tmp/build/scan.o", "-o", "python/strata/_strata.so"],
        ["cl.exe", "/O2", "/std:c++20", "/Tpsrc\\strata\\util\\scan.cpp", "/Fobuild\\scan.obj"],
    ]
    digest = _flag_digest(commands)
    assert digest.startswith("-O3 -arch arm64 -std=c++20 -flto=thin /O2 /std:c++20 (3 recorded")
    assert "/tmp/" not in digest and "python3.14" not in digest and "scan" not in digest

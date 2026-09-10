"""Canonical arm staging must preserve the install and adverse evidence."""

import subprocess
from pathlib import Path
from types import SimpleNamespace

import pytest

from benchmarks import canonical_builds as runner


def test_failing_child_uses_selected_binary_and_leaves_install_intact(tmp_path, monkeypatch):
    package = tmp_path / "python/strata"
    package.mkdir(parents=True)
    (package / "__init__.py").write_text("")
    original = package / "_strata.original.so"
    original.write_bytes(b"installed")
    binary = tmp_path / "candidate.so"
    binary.write_bytes(b"candidate")
    binary.with_name(binary.name + ".build.json").write_text('{"arm":"candidate"}')
    monkeypatch.setattr(runner, "PROJECT_ROOT", tmp_path)
    stages = []

    def fail(command, **kwargs):
        stage = Path(command[3])
        stages.append(stage)
        extension = stage / "strata" / ("_strata" + runner.sysconfig.get_config_var("EXT_SUFFIX"))
        assert extension.read_bytes() == b"candidate"
        assert (
            extension.with_name(extension.name + ".build.json").read_text() == '{"arm":"candidate"}'
        )
        assert not (stage / "strata" / original.name).exists()
        raise subprocess.CalledProcessError(1, command)

    monkeypatch.setattr(runner.subprocess, "run", fail)
    with pytest.raises(subprocess.CalledProcessError):
        runner.measure(binary, tmp_path / "out.md", tmp_path, "small", 10)
    assert original.read_bytes() == b"installed"
    assert not stages[0].exists()


def test_preflight_missing_tier_runs_no_measurements(tmp_path, monkeypatch):
    monkeypatch.setattr(runner, "_check_build_identity", lambda path: None)
    monkeypatch.setattr(runner, "measure", lambda *args: pytest.fail("measured before preflight"))
    with pytest.raises(ValueError, match="missing dataset"):
        runner.run(tmp_path / "A", tmp_path / "B", tmp_path / "out", tmp_path, ["small"], 10)
    assert not (tmp_path / "out").exists()


def test_regression_is_retained_and_does_not_skip_later_tiers(tmp_path, monkeypatch):
    monkeypatch.setattr(runner, "_check_build_identity", lambda path: None)
    measured = []
    monkeypatch.setattr(runner, "measure", lambda *args: measured.append(args))
    for tier in ("small", "medium"):
        (tmp_path / tier).mkdir()
        for name in runner.DATASETS:
            (tmp_path / tier / name).write_text("[]")

    def gate(command, **kwargs):
        if "--save-baseline" in command:
            return SimpleNamespace(returncode=0)
        failed = "small" in command[3]
        kwargs["stdout"].write("REGRESSION" if failed else "PASS")
        return SimpleNamespace(returncode=int(failed))

    monkeypatch.setattr(runner.subprocess, "run", gate)
    output = tmp_path / "out"
    assert (
        runner.run(tmp_path / "A", tmp_path / "B", output, tmp_path, ["small", "medium"], 10) == 1
    )
    assert len(measured) == 4
    assert (output / "gate-small.txt").read_text() == "REGRESSION"
    assert (output / "gate-medium.txt").read_text() == "PASS"
    with pytest.raises(FileExistsError):
        runner.run(tmp_path / "A", tmp_path / "B", output, tmp_path, ["small"], 10)


def test_child_keeps_its_import_check_without_python_optimize(tmp_path, monkeypatch):
    """The staged-import check must survive an inherited PYTHONOPTIMIZE."""
    from benchmarks import canonical_builds

    binary = tmp_path / "A.so"
    binary.write_bytes(b"a native build")
    binary.with_name("A.so.build.json").write_text("{}")
    seen = {}

    def fake_run(command, **kwargs):
        seen["command"] = command
        seen["env"] = kwargs.get("env")
        return subprocess.CompletedProcess(command, 0)

    monkeypatch.setattr(canonical_builds.subprocess, "run", fake_run)
    monkeypatch.setenv("PYTHONOPTIMIZE", "1")
    canonical_builds.measure(binary, tmp_path / "out" / "report.md", tmp_path, "small", 10)
    assert seen["env"] is not None and "PYTHONOPTIMIZE" not in seen["env"]
    bootstrap = seen["command"][2]
    assert "raise SystemExit" in bootstrap and "\nassert " not in bootstrap

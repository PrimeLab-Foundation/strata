"""Only the PGO instrumented pass leaves the native-type suites out.

docs/architecture/native_types.md, "Hot-path protection" 4:
``scripts/py_tests.py --training`` ignores ``tests/unit/native_types`` and
``tests/py/native_types``; the instrumented pass of ``pgo_build.sh``,
``pgo_build_msvc.py`` and ``pgo_build_clang_cl.py`` passes it, and so does
setup.py's post-build gate when it tests an instrumented build (it runs inside
that pass and trains the same profile). The optimized pass and every other
gate run everything.

This file lives in an ignored directory on purpose: it is a new test, and new
tests stay out of the training run.
"""

from __future__ import annotations

import ast
import re
import runpy
import subprocess
import sys
import types
from pathlib import Path

import pytest

from scripts import py_tests

PROJECT_ROOT = Path(__file__).resolve().parents[3]
SCRIPTS = PROJECT_ROOT / "scripts"
IGNORES = ["--ignore=tests/unit/native_types", "--ignore=tests/py/native_types"]


def _launched_pytest_argv(monkeypatch, argv: list[str]) -> list[str]:
    """The argument list ``scripts/py_tests.py`` hands ``pytest.main`` for @p argv."""
    launched: list[list[str]] = []

    def run(cmd, **_kwargs):
        launched.append(cmd)
        return subprocess.CompletedProcess(cmd, 0)

    monkeypatch.setattr(py_tests, "subprocess", types.SimpleNamespace(run=run))
    assert py_tests.main(argv) == 0
    (cmd,) = launched
    match = re.search(r"pytest\.main\((\[.*\])\)", cmd[-1])
    assert match, cmd[-1]
    return ast.literal_eval(match.group(1))


def test_training_ignores_both_native_type_directories(monkeypatch):
    argv = _launched_pytest_argv(monkeypatch, ["--training", "--", "-q"])
    assert argv[: len(py_tests.TEST_PATHS) + len(IGNORES)] == [*py_tests.TEST_PATHS, *IGNORES]
    assert "-q" in argv


def test_without_training_nothing_is_ignored(monkeypatch):
    argv = _launched_pytest_argv(monkeypatch, ["--", "-q"])
    assert not [arg for arg in argv if arg.startswith("--ignore")]
    assert argv[: len(py_tests.TEST_PATHS)] == list(py_tests.TEST_PATHS)


@pytest.mark.parametrize("path", py_tests.TRAINING_IGNORES)
def test_ignored_directories_exist_inside_the_test_paths(path):
    directory = PROJECT_ROOT / path
    assert directory.is_dir()
    assert (directory / "conftest.py").is_file()
    assert str(Path(path).parent.as_posix()) in py_tests.TEST_PATHS


def test_posix_script_trains_with_the_scope_and_gates_without():
    text = (SCRIPTS / "pgo_build.sh").read_text(encoding="utf-8")
    assert '"$VPY" scripts/cpp_tests.py\n' in text
    assert '"$VPY" scripts/py_tests.py "$@"\n' in text
    phase_one = text.index("export PGO_MODE=generate")
    phase_two = text.index("export PGO_MODE=use")
    call = re.compile(r"^gate_tests(?!\(\))(.*)$", re.M)  # calls, not the definition
    calls = [(m.start(), m.group(1)) for m in call.finditer(text)]
    assert len(calls) == 2, calls
    (first, first_args), (second, second_args) = calls
    assert phase_one < first < phase_two
    assert first_args == " --training"
    assert second > phase_two
    assert second_args == ""


def test_msvc_script_trains_with_the_scope_and_gates_without():
    text = (SCRIPTS / "pgo_build_msvc.py").read_text(encoding="utf-8")
    assert '_run([sys.executable, "scripts/py_tests.py", *py_args])' in text
    phase_one = text.index('_install("generate")')
    phase_two = text.index('_install("use")')
    assert text.count('"--training"') == 1
    assert phase_one < text.index('_gate_tests("--training")') < phase_two
    assert text.find("_gate_tests()", phase_one, phase_two) == -1
    assert text.find("_gate_tests()", phase_two) > phase_two


def test_clang_cl_script_trains_with_the_scope_and_gates_without():
    text = (SCRIPTS / "pgo_build_clang_cl.py").read_text(encoding="utf-8")
    phase_one = text.index('_install("generate"')
    phase_two = text.index('_install("use"')
    training = '_run([sys.executable, "scripts/py_tests.py", "--training"], extra_env=profile_env)'
    assert text.count('"--training"') == 1
    assert phase_one < text.index(training) < phase_two
    # The optimized pass's gate runs the unscoped suites.
    assert '_run([sys.executable, "scripts/py_tests.py"])' in text
    assert text.find("_gate_tests()", phase_one, phase_two) == -1
    assert text.find("_gate_tests()", phase_two) > phase_two


def _gated_build_ext(monkeypatch):
    """setup.py's build_ext subclass, read without setuptools under a plain build."""
    for name in (
        "PGO_MODE",
        "STRATA_ENABLE_LTO",
        "STRATA_PGO_PROFILE",
        "STRATA_WIN_COMPILER",
        "STRATA_MARCH",
        "SKIP_TESTS",
    ):
        monkeypatch.delenv(name, raising=False)
    setuptools = types.ModuleType("setuptools")
    setuptools.Extension = lambda name, **kwargs: types.SimpleNamespace(name=name, **kwargs)
    setuptools.setup = lambda **kwargs: None
    command = types.ModuleType("setuptools.command")
    build_ext = types.ModuleType("setuptools.command.build_ext")
    build_ext.build_ext = type("build_ext", (), {})
    monkeypatch.setitem(sys.modules, "setuptools", setuptools)
    monkeypatch.setitem(sys.modules, "setuptools.command", command)
    monkeypatch.setitem(sys.modules, "setuptools.command.build_ext", build_ext)
    namespace = runpy.run_path(str(PROJECT_ROOT / "setup.py"), run_name="setup_under_test")
    return namespace["TestGatedBuildExt"], namespace["FACADE_DIR"]


@pytest.mark.parametrize(
    ("mode", "training"),
    [("generate", True), (" Generate ", True), ("use", False), ("", False), (None, False)],
)
def test_setup_gate_scopes_only_an_instrumented_build(monkeypatch, mode, training):
    command, facade = _gated_build_ext(monkeypatch)
    gate = command.__new__(command)
    gate.get_ext_fullpath = lambda name: str(facade / "_strata.so")
    gates: list[tuple] = []
    gate._gate = lambda layer, script, *args: gates.append((layer, Path(script).name, args))
    if mode is not None:
        monkeypatch.setenv("PGO_MODE", mode)
    gate._python_gate()
    ((layer, script, args),) = gates
    assert (layer, script) == ("Python", "py_tests.py")
    assert args[:2] == ("--path", str(facade.parent))
    assert ("--training" in args) is training

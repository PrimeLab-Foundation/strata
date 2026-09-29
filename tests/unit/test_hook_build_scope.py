"""The hook image (`strata._dumps_hook`) is never built for or against a profile.

setup.py builds it unprofiled in both PGO phases, so nothing it runs enters
`_strata`'s profile (docs/architecture/dumps_with_default.md, "PGO"). These
tests pin that at the Extension level and pin the exclusion guard every PGO
script runs after its optimized build (`scripts/build_identity.py
--check-unprofiled`).
"""

from __future__ import annotations

import json
import re
import runpy
import sys
import types
from pathlib import Path

import pytest

from scripts import build_identity

PROJECT_ROOT = Path(__file__).resolve().parents[2]
LTO_FLAG = re.compile(r"-flto|^/GL$|^/LTCG", re.IGNORECASE)


class _Extension:
    def __init__(self, name: str, **kwargs) -> None:
        self.name = name
        self.__dict__.update(kwargs)


def _extensions(
    monkeypatch, tmp_path, mode: str, lto: str, **hook_env: str
) -> dict[str, _Extension]:
    """setup.py's Extensions under one PGO phase, read without setuptools.

    `hook_env` sets the hook's own variables (STRATA_HOOK_PGO_MODE, ...);
    unnamed ones are cleared, so a phase-3 environment around the test run
    cannot leak into what `_strata`'s phases are pinned to produce.
    """
    profile = tmp_path / "strata.profile"
    profile.write_bytes(b"")
    for name in (
        "SKIP_TESTS",
        "STRATA_WIN_COMPILER",
        "STRATA_MARCH",
        "STRATA_HOOK_PGO_MODE",
        "STRATA_HOOK_PGO_PROFILE",
        "STRATA_EXTENSIONS",
    ):
        monkeypatch.delenv(name, raising=False)
    for name, value in hook_env.items():
        monkeypatch.setenv(name, value)
    monkeypatch.setenv("PGO_MODE", mode)
    monkeypatch.setenv("STRATA_ENABLE_LTO", lto)
    monkeypatch.setenv("STRATA_PGO_PROFILE", str(profile))
    captured: dict = {}
    setuptools = types.ModuleType("setuptools")
    setuptools.Extension = _Extension
    setuptools.setup = lambda **kwargs: captured.update(kwargs)
    command = types.ModuleType("setuptools.command")
    build_ext = types.ModuleType("setuptools.command.build_ext")
    build_ext.build_ext = type("build_ext", (), {})
    monkeypatch.setitem(sys.modules, "setuptools", setuptools)
    monkeypatch.setitem(sys.modules, "setuptools.command", command)
    monkeypatch.setitem(sys.modules, "setuptools.command.build_ext", build_ext)
    runpy.run_path(str(PROJECT_ROOT / "setup.py"), run_name="setup_under_test")
    return {extension.name: extension for extension in captured["ext_modules"]}


@pytest.mark.parametrize(("mode", "lto"), [("generate", "0"), ("use", "1")])
def test_hook_extension_gets_no_pgo_or_lto_flags(monkeypatch, tmp_path, mode, lto):
    extensions = _extensions(monkeypatch, tmp_path, mode, lto)
    hook = extensions["strata._dumps_hook"]
    flags = [*hook.extra_compile_args, *hook.extra_link_args]
    assert not [flag for flag in flags if build_identity.PROFILE_FLAG.search(flag)]
    assert not [flag for flag in flags if LTO_FLAG.search(flag)]
    # Control: the same phase does reach `_strata`, in a spelling the guard reads.
    engine = extensions["strata._strata"]
    engine_flags = [*engine.extra_compile_args, *engine.extra_link_args]
    assert [flag for flag in engine_flags if build_identity.PROFILE_FLAG.search(flag)]


@pytest.mark.parametrize(
    "flag",
    [
        "-fprofile-generate",
        "-fprofile-use=/p/strata.profdata",
        "-fprofile-instr-generate",
        "-fcs-profile-generate",
        r"/clang:-fprofile-use=C:\p\strata.profdata",
        r"/GENPROFILE:PGD=C:\p\strata.pgd",
        "/FASTGENPROFILE",
        r"/USEPROFILE:PGD=C:\p\strata.pgd",
        "/LTCG:PGI",
        "/LTCG:PGOPTIMIZE",
    ],
)
def test_guard_recognizes_every_profile_spelling(flag):
    assert build_identity.PROFILE_FLAG.search(flag)


@pytest.mark.parametrize(
    "flag",
    ["-O3", "-flto=thin", "/GL", "/LTCG", "/LTCG:INCREMENTAL", "-fcoverage-mapping", "-g"],
)
def test_guard_ignores_flags_that_carry_no_profile(flag):
    assert not build_identity.PROFILE_FLAG.search(flag)


def _image(tmp_path, content: bytes = b"\x7fELF plain image", **identity) -> Path:
    image = tmp_path / "_dumps_hook.so"
    image.write_bytes(content)
    packet = {
        "extension_sha256": build_identity.file_hash(image),
        "pgo_profile": None,
        "commands": [["c++", "-O3", "-c", "src/strata/bindings/python_dumps_hook.cpp"]],
    }
    packet.update(identity)
    image.with_name(image.name + ".build.json").write_text(json.dumps(packet))
    return image


def test_guard_passes_an_unprofiled_image(tmp_path):
    assert build_identity.unprofiled_problems(_image(tmp_path)) == []


@pytest.mark.parametrize(
    ("identity", "content", "expected"),
    [
        ({"pgo_profile": {"path": "/p/strata.profdata"}}, None, "records a profile"),
        ({"commands": [["c++", "-fprofile-use=/p/x.profdata"]]}, None, "profile flags"),
        ({"commands": [["link.exe", "/LTCG:PGI"]]}, None, "profile flags"),
        ({"commands": []}, None, "no build commands"),
        ({"extension_sha256": "0" * 64}, None, "another binary"),
        ({}, b"...LLVM_PROFILE_FILE...", "profile runtime"),
        ({}, b"...PGORT140.dll...", "profile runtime"),
    ],
)
def test_guard_fails_loudly(tmp_path, identity, content, expected):
    image = _image(tmp_path, content or b"\x7fELF plain image", **identity)
    problems = build_identity.unprofiled_problems(image)
    assert any(expected in problem for problem in problems), problems


def test_guard_fails_without_an_identity(tmp_path):
    image = tmp_path / "_dumps_hook.so"
    image.write_bytes(b"image")
    assert "no readable build identity" in build_identity.unprofiled_problems(image)[0]


def test_posix_pgo_script_guards_the_hook_after_the_optimized_build():
    text = (PROJECT_ROOT / "scripts" / "pgo_build.sh").read_text(encoding="utf-8")
    guard = "scripts/build_identity.py --check-unprofiled strata._dumps_hook"
    phase_two = text.index("export PGO_MODE=use")
    install = text.index("pip install", phase_two)
    assert text.find(guard, install) > install
    # Phase 1 runs it too, with the instrumented `_strata` as the image scan's control.
    assert text.index(f"{guard} --instrumented strata._strata") < phase_two


@pytest.mark.parametrize("script", ["pgo_build_msvc.py", "pgo_build_clang_cl.py"])
def test_windows_pgo_scripts_guard_the_hook_after_the_optimized_build(script):
    text = (PROJECT_ROOT / "scripts" / script).read_text(encoding="utf-8")
    assert '"--check-unprofiled",\n            "strata._dumps_hook",' in text
    phase_one = text.index('_install("generate"')
    phase_two = text.index('_install("use"')
    control = '_assert_hook_unprofiled("--instrumented", "strata._strata")'
    assert phase_one < text.index(control) < phase_two
    assert text.find("_assert_hook_unprofiled()", phase_two) > phase_two

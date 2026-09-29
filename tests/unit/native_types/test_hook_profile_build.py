"""The hook image's own profile (docs/architecture/native_types.md, "Hook profile").

scripts/pgo_build.sh and scripts/pgo_build_clang_cl.py rebuild
`strata._dumps_hook` alone after `_strata`'s two phases, instrumented and then
against a profile of its own; `_strata`'s variables never reach it and its
variables never reach `_strata`. Kept out of the trained scope with the other
native suites (tests/unit/native_types), so `_strata`'s profile does not see
these tests.
"""

from __future__ import annotations

import sys
from pathlib import Path

import pytest

from scripts import build_identity
from tests.unit.test_hook_build_scope import (
    LTO_FLAG,
    PROJECT_ROOT,
    _Extension,
    _extensions,
    _image,
)


def _flags(extension: _Extension) -> list[str]:
    return [*extension.extra_compile_args, *extension.extra_link_args]


def test_hook_generate_instruments_the_hook_alone(monkeypatch, tmp_path):
    extensions = _extensions(monkeypatch, tmp_path, "", "0", STRATA_HOOK_PGO_MODE="generate")
    assert [f for f in _flags(extensions["strata._dumps_hook"]) if "profile-generate" in f]
    assert not [f for f in _flags(extensions["strata._dumps_hook"]) if LTO_FLAG.search(f)]
    engine = _flags(extensions["strata._strata"])
    assert not [f for f in engine if build_identity.PROFILE_FLAG.search(f) or LTO_FLAG.search(f)]


def test_each_image_gets_its_own_profile_and_never_the_others(monkeypatch, tmp_path):
    hook_profile = tmp_path / "hook.profdata"
    hook_profile.write_bytes(b"")
    extensions = _extensions(
        monkeypatch,
        tmp_path,
        "use",
        "1",
        STRATA_HOOK_PGO_MODE="use",
        STRATA_HOOK_PGO_PROFILE=str(hook_profile),
    )
    hook = [f for f in _flags(extensions["strata._dumps_hook"]) if "profile-use=" in f]
    engine = [f for f in _flags(extensions["strata._strata"]) if "profile-use=" in f]
    assert hook and all(str(hook_profile) in f for f in hook)
    assert engine and all("strata.profile" in f and "hook" not in f for f in engine)
    if sys.platform != "win32":  # clang-cl spells no LTO for either image
        assert [f for f in _flags(extensions["strata._dumps_hook"]) if LTO_FLAG.search(f)]


def test_hook_use_requires_its_own_profile(monkeypatch, tmp_path):
    with pytest.raises(SystemExit, match="STRATA_HOOK_PGO_PROFILE"):
        _extensions(monkeypatch, tmp_path, "", "0", STRATA_HOOK_PGO_MODE="use")


def test_extension_filter_builds_the_named_images_only(monkeypatch, tmp_path):
    extensions = _extensions(monkeypatch, tmp_path, "", "0", STRATA_EXTENSIONS="strata._dumps_hook")
    assert list(extensions) == ["strata._dumps_hook"]
    with pytest.raises(SystemExit, match="no such extension"):
        _extensions(monkeypatch, tmp_path, "", "0", STRATA_EXTENSIONS="strata._nope")


def _profiled_image(tmp_path, profile: Path, **identity) -> Path:
    use = f"-fprofile-use={profile}"
    packet = {
        "pgo_profile": build_identity.profile_identity(str(profile)),
        "commands": [["c++", "-O3", use, "-c", "src/strata/bindings/python_dumps_hook.cpp"]],
    }
    packet.update(identity)
    return _image(tmp_path, **packet)


def _profiles(tmp_path) -> tuple[Path, Path]:
    own = tmp_path / "hook.profdata"
    own.write_bytes(b"hook counts")
    foreign = tmp_path / "strata.profdata"
    foreign.write_bytes(b"strata counts")
    return own, foreign


def test_profiled_guard_passes_an_image_on_its_own_profile(tmp_path):
    own, foreign = _profiles(tmp_path)
    assert build_identity.profiled_problems(_profiled_image(tmp_path, own), own, foreign) == []


@pytest.mark.parametrize(
    ("which", "expected"),
    [
        ("foreign-recorded", "does not record the profile"),
        ("foreign-command", "foreign profile"),
        ("instrumented", "instrument the image"),
        ("unprofiled", "do not use"),
    ],
)
def test_profiled_guard_fails_loudly(tmp_path, which, expected):
    own, foreign = _profiles(tmp_path)
    if which == "foreign-recorded":
        image = _profiled_image(tmp_path, foreign)
    elif which == "foreign-command":
        image = _profiled_image(
            tmp_path, own, commands=[["c++", f"-fprofile-use={own}", f"-fprofile-use={foreign}"]]
        )
    elif which == "instrumented":
        image = _profiled_image(
            tmp_path, own, commands=[["c++", f"-fprofile-use={own}", "-fprofile-generate"]]
        )
    else:
        image = _profiled_image(tmp_path, own, commands=[["c++", "-O3"]])
    problems = build_identity.profiled_problems(image, own, foreign)
    assert any(expected in problem for problem in problems), problems


def test_posix_pgo_script_profiles_the_hook_after_strata_and_checks_both():
    text = (PROJECT_ROOT / "scripts" / "pgo_build.sh").read_text(encoding="utf-8")
    phase_two_gate = text.index("gate tests on the optimized build")
    phase_three = text.index("export STRATA_EXTENSIONS=strata._dumps_hook")
    assert phase_two_gate < phase_three
    assert text.index("STRATA_HOOK_PGO_MODE=generate", phase_three) < text.index(
        "scripts/pgo_hook_training.py", phase_three
    )
    tail = text[phase_three:]
    assert (
        '--check-profiled strata._dumps_hook \\\n        --profile "$HOOK_PROFILE" --foreign "$PROFILE"'
        in tail
    )
    assert (
        '--check-profiled strata._strata \\\n        --profile "$PROFILE" --foreign "$HOOK_PROFILE"'
        in tail
    )
    assert "the hook phase changed _strata's image" in tail


def test_clang_cl_script_profiles_the_hook_after_strata():
    text = (PROJECT_ROOT / "scripts" / "pgo_build_clang_cl.py").read_text(encoding="utf-8")
    main = text.index("def main(")
    gate = text.index("gate tests on the optimized build", main)
    assert text.index("_hook_phase(profdata)", main) > gate
    assert '"STRATA_EXTENSIONS": "strata._dumps_hook"' in text
    assert "The hook phase changed _strata's image" in text

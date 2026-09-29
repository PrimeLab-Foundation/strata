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


def test_clang_cl_spells_each_images_own_profile(monkeypatch, tmp_path):
    # setup.py as the Windows leg runs it (clang-cl), simulated: the hook's phase-3b flags
    # name its profile behind /clang:, `_strata` keeps its own, and neither gets LTO.
    hook_profile = tmp_path / "hook.profdata"
    hook_profile.write_bytes(b"")
    monkeypatch.setattr(sys, "platform", "win32")
    extensions = _extensions(
        monkeypatch,
        tmp_path,
        "use",
        "0",
        STRATA_WIN_COMPILER="clang-cl",
        STRATA_HOOK_PGO_MODE="use",
        STRATA_HOOK_PGO_PROFILE=str(hook_profile),
    )
    hook = _flags(extensions["strata._dumps_hook"])
    engine = _flags(extensions["strata._strata"])
    assert f"/clang:-fprofile-use={hook_profile}" in hook
    assert not [f for f in hook if "strata.profile" in f]
    assert [f for f in engine if f.startswith("/clang:-fprofile-use=") and "strata.profile" in f]
    assert not [f for f in [*hook, *engine] if LTO_FLAG.search(f)]


def test_clang_cl_hook_phase_runs_in_order(monkeypatch, tmp_path):
    # scripts/pgo_build_clang_cl.py's phase 3, driven with its commands recorded instead of
    # run: the hook alone instrumented (gate counts discarded), trained, merged, rebuilt
    # against its own profile, both images checked, the gate run -- in that order.
    from scripts import pgo_build_clang_cl as script

    pgo = tmp_path / "pgo"
    for name, value in {
        "PGO_DIR": pgo,
        "WORK_DIR": pgo / "work",
        "PROFILE": pgo / "strata.profdata",
        "HOOK_RAW_DIR": pgo / "hook-raw",
        "HOOK_GATE_DIR": pgo / "hook-gate-discarded",
        "HOOK_PROFILE": pgo / "hook.profdata",
    }.items():
        monkeypatch.setattr(script, name, value)
    image = tmp_path / "_strata.pyd"
    image.write_bytes(b"strata image")
    monkeypatch.setattr(script, "_strata_image", lambda: image)
    calls: list[tuple[list[str], dict]] = []

    def run(cmd, extra_env=None):
        calls.append((list(cmd), dict(extra_env or {})))
        if "scripts/pgo_hook_training.py" in cmd:
            (pgo / "hook-raw" / "1.profraw").write_bytes(b"counts")

    monkeypatch.setattr(script, "_run", run)
    script._hook_phase("llvm-profdata")

    installs = [env for cmd, env in calls if cmd[1:4] == ["-m", "pip", "install"]]
    assert [env["STRATA_HOOK_PGO_MODE"] for env in installs] == ["generate", "use"]
    assert all(env["STRATA_EXTENSIONS"] == "strata._dumps_hook" for env in installs)
    assert all(env["PGO_MODE"] == "" for env in installs)
    assert installs[0]["LLVM_PROFILE_FILE"].startswith(str(pgo / "hook-gate-discarded"))
    assert installs[1]["STRATA_HOOK_PGO_PROFILE"] == str(pgo / "hook.profdata")
    order = [
        next(
            i for i, (cmd, env) in enumerate(calls) if env.get("STRATA_HOOK_PGO_MODE") == "generate"
        ),
        next(i for i, (cmd, _) in enumerate(calls) if "scripts/pgo_hook_training.py" in cmd),
        next(i for i, (cmd, _) in enumerate(calls) if cmd[:2] == ["llvm-profdata", "merge"]),
        next(i for i, (cmd, _) in enumerate(calls) if "hook-native-training-clang-cl-v1" in cmd),
        next(i for i, (cmd, env) in enumerate(calls) if env.get("STRATA_HOOK_PGO_MODE") == "use"),
        next(i for i, (cmd, _) in enumerate(calls) if "--check-profiled" in cmd),
        next(i for i, (cmd, _) in enumerate(calls) if cmd[-1:] == ["scripts/py_tests.py"]),
    ]
    assert order == sorted(order)
    training = next(env for cmd, env in calls if "scripts/pgo_hook_training.py" in cmd)
    assert training["LLVM_PROFILE_FILE"].startswith(str(pgo / "hook-raw"))
    checks = [
        cmd[cmd.index("--check-profiled") + 1 :] for cmd, _ in calls if "--check-profiled" in cmd
    ]
    assert checks == [
        [
            "strata._dumps_hook",
            "--profile",
            str(pgo / "hook.profdata"),
            "--foreign",
            str(pgo / "strata.profdata"),
        ],
        [
            "strata._strata",
            "--profile",
            str(pgo / "strata.profdata"),
            "--foreign",
            str(pgo / "hook.profdata"),
        ],
    ]


def test_clang_cl_hook_phase_refuses_a_changed_strata_image(monkeypatch, tmp_path):
    from scripts import pgo_build_clang_cl as script

    pgo = tmp_path / "pgo"
    for name in ("HOOK_RAW_DIR", "HOOK_GATE_DIR", "HOOK_PROFILE", "WORK_DIR", "PROFILE"):
        monkeypatch.setattr(script, name, pgo / name.lower())
    image = tmp_path / "_strata.pyd"
    image.write_bytes(b"before")
    monkeypatch.setattr(script, "_strata_image", lambda: image)

    def run(cmd, extra_env=None):
        if "scripts/pgo_hook_training.py" in cmd:
            (pgo / "hook_raw_dir" / "1.profraw").write_bytes(b"counts")
        if (extra_env or {}).get("STRATA_HOOK_PGO_MODE") == "use":
            image.write_bytes(b"after")

    monkeypatch.setattr(script, "_run", run)
    with pytest.raises(SystemExit, match="changed _strata's image"):
        script._hook_phase("llvm-profdata")

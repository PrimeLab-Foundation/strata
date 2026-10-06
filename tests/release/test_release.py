"""Release tooling: scripts/release.py's version commands and setup.py's release knobs.

Outside pytest's testpaths (like tests/integrations): run by `make test-release`
and CI's release-tooling job. Nothing here builds or imports the extension.
verify-dist and check-promotion are pinned in test_release_dist.py.
"""

from __future__ import annotations

import importlib.util
import runpy
import shutil
import subprocess
import sys
import types
from pathlib import Path

import pytest

PROJECT_ROOT = Path(__file__).resolve().parents[2]
RELEASE_SCRIPT = PROJECT_ROOT / "scripts" / "release.py"
CURRENT = "2026.8.10"


def _load_release():
    spec = importlib.util.spec_from_file_location("release_under_test", RELEASE_SCRIPT)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


release = _load_release()


def _init_file(tmp_path: Path, version: str = CURRENT) -> Path:
    init = tmp_path / "__init__.py"
    init.write_text(
        '"""Package docstring mentioning __version__ = "0.0.0" in prose."""\n'
        "\n"
        "from strata.serialize import dumps\n"
        "\n"
        "# Single source of truth for the version.\n"
        f'__version__ = "{version}"\n'
        "\n"
        '__all__ = ["dumps", "__version__"]\n',
        encoding="utf-8",
    )
    return init


# --- grammar ---------------------------------------------------------------

ACCEPTED = [
    "2026.10.6",
    "2026.10.6.1",
    "2026.10.6rc1",
    "2026.10.6.1rc2",
    "2026.10.6.12",
    "2026.10.6rc10",
    "2026.1.1",
    "2026.12.31",
    "2024.2.29",
    "9999.12.31",
]

REJECTED = [
    "2026.02.6",  # leading zero, month
    "2026.10.06",  # leading zero, day
    "02026.10.6",
    "0026.10.6",
    "2026.13.1",  # no month 13
    "2026.0.1",
    "2026.1.0",
    "2026.2.30",  # not a calendar date
    "2026.2.29",  # 2026 is not a leap year
    "2026.4.31",
    "2026.10.32",
    "2026.10.6.0",  # .N counts from 1
    "2026.10.6.01",
    "2026.10.6rc0",  # rcK counts from 1
    "2026.10.6rc01",
    "2026.10.6-rc1",
    "2026.10.6.rc1",
    "2026.10.6a1",
    "2026.10.6b1",
    "2026.10.6.post1",
    "2026.10.6.dev1",
    "2026.10.6+local",
    "2026.10.6.1.2",
    "2026.10",
    "v2026.10.6",
    " 2026.10.6",
    "2026.10.6\n",
    "２０２６.10.6",  # fullwidth digits
    "",
]


@pytest.mark.parametrize("version", ACCEPTED)
def test_grammar_accepts(version):
    assert len(release.version_key(version)) == 6


@pytest.mark.parametrize("version", REJECTED)
def test_grammar_rejects(version):
    with pytest.raises(SystemExit, match=r"^release: "):
        release.version_key(version)


def test_ordering_follows_pep440():
    ordered = [
        "2026.8.10",
        "2026.8.10.1rc1",
        "2026.8.10.1",
        "2026.8.10.2",
        "2026.8.11",
        "2026.10.6rc1",
        "2026.10.6rc2",
        "2026.10.6rc10",
        "2026.10.6",
        "2026.10.6.1rc1",
        "2026.10.6.1",
        "2026.10.6.10",
        "2026.10.7",
        "2027.1.1",
    ]
    keys = [release.version_key(v) for v in ordered]
    assert keys == sorted(keys)
    assert len(set(keys)) == len(keys)
    version = pytest.importorskip("packaging.version")
    assert [version.Version(v) for v in ordered] == sorted(version.Version(v) for v in ordered)


# --- bump ------------------------------------------------------------------


@pytest.mark.parametrize("new", ["2026.10.6", "2026.10.6.1", "2026.10.6rc1", "2026.8.10.1"])
def test_bump_rewrites_only_the_version_line(tmp_path, new):
    init = _init_file(tmp_path)
    before = init.read_text(encoding="utf-8").splitlines(keepends=True)
    assert release.bump(new, init) == 0
    after = init.read_text(encoding="utf-8").splitlines(keepends=True)
    changed = [(a, b) for a, b in zip(before, after, strict=True) if a != b]
    assert changed == [(f'__version__ = "{CURRENT}"\n', f'__version__ = "{new}"\n')]
    assert release._version_literal(init) == new


def test_bump_keeps_crlf_line_endings(tmp_path):
    init = tmp_path / "__init__.py"
    init.write_bytes(f'"""Doc."""\r\n__version__ = "{CURRENT}"\r\nX = 1\r\n'.encode())
    release.bump("2026.10.6", init)
    assert init.read_bytes() == b'"""Doc."""\r\n__version__ = "2026.10.6"\r\nX = 1\r\n'


@pytest.mark.parametrize(
    "new",
    [CURRENT, "2026.8.9", "2026.8.10rc1", "2026.8.1", "2025.12.31", "2026.8.9.5"],
)
def test_bump_refuses_a_version_not_strictly_higher(tmp_path, new):
    init = _init_file(tmp_path)
    before = init.read_bytes()
    with pytest.raises(SystemExit, match="not higher than the current 2026.8.10"):
        release.bump(new, init)
    assert init.read_bytes() == before


def test_bump_from_a_candidate_to_its_final(tmp_path):
    init = _init_file(tmp_path, "2026.10.6rc2")
    release.bump("2026.10.6", init)
    assert release._version_literal(init) == "2026.10.6"
    with pytest.raises(SystemExit, match="not higher"):
        release.bump("2026.10.6rc3", init)


@pytest.mark.parametrize("bad", ["2026.02.6", "2026.13.1", "2026.2.30", "2026.10.6.0"])
def test_bump_refuses_an_invalid_version(tmp_path, bad):
    init = _init_file(tmp_path)
    before = init.read_bytes()
    with pytest.raises(SystemExit, match=r"^release: "):
        release.bump(bad, init)
    assert init.read_bytes() == before


def test_bump_refuses_two_version_lines(tmp_path):
    init = _init_file(tmp_path)
    init.write_text(
        init.read_text(encoding="utf-8") + f'__version__ = "{CURRENT}"\n', encoding="utf-8"
    )
    with pytest.raises(SystemExit, match="exactly one"):
        release.bump("2026.10.6", init)


def test_bump_refuses_a_line_the_module_does_not_read(tmp_path):
    init = tmp_path / "__init__.py"
    init.write_text(f'__version__ = ("2026.9.1")\n__version__ = "{CURRENT}"\n', encoding="utf-8")
    with pytest.raises(SystemExit, match="the module's literal '2026.9.1'"):
        release.bump("2026.10.6", init)


def test_bump_cli_on_a_copy(tmp_path, monkeypatch):
    init = _init_file(tmp_path)
    monkeypatch.setattr(release, "INIT_FILE", init)
    assert release.main(["bump", "2026.10.6rc1"]) == 0
    assert release._version_literal(init) == "2026.10.6rc1"
    with pytest.raises(SystemExit, match="not higher"):
        release.main(["bump", "2026.8.9"])


# --- check-tag -------------------------------------------------------------


def test_check_tag_accepts_v_plus_the_literal(tmp_path):
    assert release.check_tag(f"v{CURRENT}", require_final=False, init=_init_file(tmp_path)) == 0
    assert release.check_tag(f"v{CURRENT}", require_final=True, init=_init_file(tmp_path)) == 0


@pytest.mark.parametrize("tag", [CURRENT, f"V{CURRENT}", "v2026.8.9", f"v{CURRENT}.1", ""])
def test_check_tag_rejects_any_other_tag(tmp_path, tag):
    with pytest.raises(SystemExit, match="does not match __version__"):
        release.check_tag(tag, require_final=False, init=_init_file(tmp_path))


def test_check_tag_require_final_rejects_a_candidate(tmp_path):
    init = _init_file(tmp_path, "2026.10.6rc1")
    assert release.check_tag("v2026.10.6rc1", require_final=False, init=init) == 0
    with pytest.raises(SystemExit, match="release candidate"):
        release.check_tag("v2026.10.6rc1", require_final=True, init=init)
    final = _init_file(tmp_path, "2026.10.6.1")
    assert release.check_tag("v2026.10.6.1", require_final=True, init=final) == 0


def test_check_tag_rejects_a_literal_outside_the_grammar(tmp_path):
    init = _init_file(tmp_path, "2026.10.06")
    with pytest.raises(SystemExit, match="is not YYYY.M.D"):
        release.check_tag("v2026.10.06", require_final=False, init=init)


def test_check_tag_cli_on_the_checkout():
    literal = release._version_literal()

    def run(*args):
        cmd = [sys.executable, str(RELEASE_SCRIPT), "check-tag", *args]
        return subprocess.run(cmd, capture_output=True, text=True, check=False)

    ok = run(f"v{literal}")
    assert ok.returncode == 0, ok.stderr
    wrong = run("v0.0.0")
    assert wrong.returncode == 1
    assert "does not match __version__" in wrong.stderr


# --- profile guard ---------------------------------------------------------


@pytest.mark.parametrize("value", [None, "", "  "])
def test_profile_refuses_outside_cibuildwheel(monkeypatch, value):
    if value is None:
        monkeypatch.delenv("CIBUILDWHEEL", raising=False)
    else:
        monkeypatch.setenv("CIBUILDWHEEL", value)

    def untouched(*args, **kwargs):
        raise AssertionError("profile went past its CIBUILDWHEEL guard")

    monkeypatch.setattr(release, "_toolchain", untouched)
    monkeypatch.setattr(release.shutil, "rmtree", untouched)
    with pytest.raises(SystemExit, match="CIBUILDWHEEL is unset"):
        release.profile()


def test_profile_strips_the_build_settings_from_its_children_only(monkeypatch, tmp_path):
    pgo = tmp_path / "release-pgo"
    monkeypatch.setattr(release, "PGO_DIR", pgo)
    monkeypatch.setattr(release, "VENV_DIR", tmp_path / "release-venv")
    monkeypatch.setattr(release, "STRATA_PROFILE", pgo / "strata.profdata")
    monkeypatch.setattr(release, "HOOK_PROFILE", pgo / "hook.profdata")
    monkeypatch.setattr(release, "_toolchain", lambda: {"probe": "ok"})
    monkeypatch.setattr(release, "_leftovers", list)
    monkeypatch.setenv("CIBUILDWHEEL", "1")
    monkeypatch.delenv("SKIP_TESTS", raising=False)
    monkeypatch.setenv("STRATA_MARCH", "x86-64-v3")
    for name in release.STRIPPED_ENV:
        monkeypatch.setenv(name, f"inherited-{name}")
    for name in ("VENV", "PGO_DIR", "PGO_VERIFY_BENCH"):
        monkeypatch.delenv(name, raising=False)
    calls = []

    def fake_run(cmd, env=None):
        calls.append((cmd, env))
        if any("pgo_build" in part for part in cmd):
            (pgo / "strata.profdata").write_bytes(b"strata profile")
            (pgo / "hook.profdata").write_bytes(b"hook profile")

    monkeypatch.setattr(release, "_run", fake_run)
    assert release.profile() == 0

    assert len(calls) == 3
    for cmd, env in calls:
        assert env is not None and env is not release.os.environ, cmd
        assert not set(release.STRIPPED_ENV) & env.keys(), cmd
        assert env["STRATA_MARCH"] == "x86-64-v3"
    pgo_env = calls[-1][1]
    assert pgo_env["VENV"] == str(tmp_path / "release-venv")
    assert pgo_env["PGO_DIR"] == release.PGO_REL
    assert pgo_env["PGO_VERIFY_BENCH"] == "0"
    for name in release.STRIPPED_ENV:
        assert release.os.environ[name] == f"inherited-{name}"
    assert not {"VENV", "PGO_DIR", "PGO_VERIFY_BENCH"} & release.os.environ.keys()
    assert (pgo / "profiles.json").is_file()


def test_profile_refuses_skip_tests(monkeypatch):
    monkeypatch.setenv("CIBUILDWHEEL", "1")
    monkeypatch.setenv("SKIP_TESTS", "1")
    with pytest.raises(SystemExit, match="SKIP_TESTS is set"):
        release.profile()


# --- ISA flags in a build identity -----------------------------------------


@pytest.fixture
def posix(monkeypatch):
    monkeypatch.setattr(release, "WINDOWS", False)
    monkeypatch.delenv("STRATA_MARCH", raising=False)
    return monkeypatch


@pytest.mark.parametrize(
    ("march", "flags"),
    [
        ("x86-64-v3", {"-march=x86-64-v3", "-flto=thin", "-O3"}),
        ("armv8-a", {"-march=armv8-a", "-flto=thin"}),
        ("none", {"-flto=thin", "-O3"}),
        (" none ", {"-flto=thin"}),
    ],
)
def test_isa_accepts_the_declared_target(posix, march, flags):
    posix.setenv("STRATA_MARCH", march)
    assert release._isa_problems(flags) == []


@pytest.mark.parametrize(
    ("march", "flags", "problems"),
    [
        (
            "x86-64-v3",
            {"-march=native", "-march=x86-64-v3", "-flto=thin"},
            [
                "built with -march=native",
                "expected -march=x86-64-v3 alone, built with ['-march=native', '-march=x86-64-v3']",
            ],
        ),
        (
            "x86-64-v3",
            {"-march=x86-64-v2", "-flto=thin"},
            ["expected -march=x86-64-v3 alone, built with ['-march=x86-64-v2']"],
        ),
        ("x86-64-v3", {"-flto=thin"}, ["expected -march=x86-64-v3 alone, built with []"]),
        (
            "none",
            {"-march=armv8-a", "-flto=thin"},
            ["STRATA_MARCH=none, yet built with ['-march=armv8-a']"],
        ),
        ("x86-64-v3", {"-march=x86-64-v3", "-flto"}, ["built without -flto=thin"]),
        (
            None,
            {"-march=x86-64-v3", "-flto=thin"},
            ["STRATA_MARCH is not set, so the expected ISA flag is unknown"],
        ),
    ],
)
def test_isa_refuses_any_other_flag_set(posix, march, flags, problems):
    if march is not None:
        posix.setenv("STRATA_MARCH", march)
    assert release._isa_problems(flags) == problems


@pytest.mark.parametrize(
    ("flags", "problems"),
    [
        ({"/arch:AVX2", "/O2"}, []),
        ({"/O2"}, ["built without /arch:AVX2"]),
        ({"/arch:AVX2", "-march=native"}, ["built with -march=native"]),
    ],
)
def test_isa_on_windows_needs_avx2_and_ignores_strata_march(monkeypatch, flags, problems):
    monkeypatch.setattr(release, "WINDOWS", True)
    monkeypatch.setenv("STRATA_MARCH", "x86-64-v3")
    assert release._isa_problems(flags) == problems


# --- toolchain contract ----------------------------------------------------


@pytest.fixture
def toolchain(monkeypatch):
    """_toolchain with its platform, PATH and subprocess replaced by fakes."""

    class Toolchain:
        def __init__(self) -> None:
            self.versions = {
                "clang-18": "Ubuntu clang version 18.1.8\nTarget: x86_64-pc-linux-gnu\n",
                "clang++-18": "Ubuntu clang version 18.1.8\n",
                "/usr/bin/llvm-profdata": "LLVM (http://llvm.org/):\n  LLVM version 18.1.8\n",
            }
            self.profdata: str | None = "/usr/bin/llvm-profdata"

        def use(self, platform_name: str, machine: str = "x86_64") -> None:
            monkeypatch.setattr(release, "WINDOWS", platform_name == "win32")
            monkeypatch.setattr(release, "sys", types.SimpleNamespace(platform=platform_name))
            monkeypatch.setattr(release, "platform", types.SimpleNamespace(machine=lambda: machine))
            monkeypatch.setattr(
                release, "shutil", types.SimpleNamespace(which=lambda _: self.profdata)
            )
            monkeypatch.setattr(release, "_output", lambda cmd: self.versions[cmd[0]])

    for name in ("STRATA_WIN_COMPILER", "STRATA_MARCH", "MACOSX_DEPLOYMENT_TARGET", "ARCHFLAGS"):
        monkeypatch.delenv(name, raising=False)
    monkeypatch.setenv("CC", "clang-18")
    monkeypatch.setenv("CXX", "clang++-18")
    return Toolchain()


def test_toolchain_windows_needs_clang_cl(toolchain, monkeypatch):
    toolchain.use("win32", "AMD64")
    with pytest.raises(SystemExit, match="STRATA_WIN_COMPILER must be clang-cl, not ''"):
        release._toolchain()
    monkeypatch.setenv("STRATA_WIN_COMPILER", "msvc")
    with pytest.raises(SystemExit, match="must be clang-cl, not 'msvc'"):
        release._toolchain()
    monkeypatch.setenv("STRATA_WIN_COMPILER", "clang-cl")
    assert release._toolchain()["STRATA_WIN_COMPILER"] == "clang-cl"


@pytest.mark.parametrize("platform_name", ["linux", "darwin"])
def test_toolchain_posix_needs_strata_march(toolchain, platform_name):
    toolchain.use(platform_name)
    with pytest.raises(SystemExit, match="STRATA_MARCH must be set for a release build"):
        release._toolchain()


def test_toolchain_macos_needs_a_target_and_one_arch(toolchain, monkeypatch):
    toolchain.use("darwin", "arm64")
    monkeypatch.setenv("STRATA_MARCH", "none")
    with pytest.raises(SystemExit, match="MACOSX_DEPLOYMENT_TARGET must be set"):
        release._toolchain()
    monkeypatch.setenv("MACOSX_DEPLOYMENT_TARGET", "11.0")
    for archflags in ("", "-arch x86_64", "-arch arm64 -arch x86_64"):
        monkeypatch.setenv("ARCHFLAGS", archflags)
        with pytest.raises(SystemExit, match="ARCHFLAGS must be exactly '-arch arm64'"):
            release._toolchain()
    monkeypatch.setenv("ARCHFLAGS", "-arch arm64")
    facts = release._toolchain()
    assert facts["MACOSX_DEPLOYMENT_TARGET"] == "11.0"
    assert facts["ARCHFLAGS"] == "-arch arm64"


def test_toolchain_linux_accepts_matching_clang_and_profdata(toolchain, monkeypatch):
    toolchain.use("linux")
    monkeypatch.setenv("STRATA_MARCH", "x86-64-v3")
    facts = release._toolchain()
    assert facts["CC"] == "clang-18 (Ubuntu clang version 18.1.8)"
    assert facts["llvm-profdata"] == "/usr/bin/llvm-profdata (LLVM 18)"


def test_toolchain_linux_refuses_a_compiler_that_is_not_clang(toolchain, monkeypatch):
    toolchain.use("linux")
    monkeypatch.setenv("STRATA_MARCH", "x86-64-v3")
    toolchain.versions["clang++-18"] = "g++ (GCC) 13.2.0\n"
    with pytest.raises(SystemExit, match=r"CXX=clang\+\+-18 is not clang: g\+\+ \(GCC\) 13.2.0"):
        release._toolchain()


def test_toolchain_linux_needs_llvm_profdata_on_path(toolchain, monkeypatch):
    toolchain.use("linux")
    monkeypatch.setenv("STRATA_MARCH", "x86-64-v3")
    toolchain.profdata = None
    with pytest.raises(SystemExit, match="llvm-profdata is not on PATH"):
        release._toolchain()


def test_toolchain_linux_needs_one_llvm_major(toolchain, monkeypatch):
    toolchain.use("linux")
    monkeypatch.setenv("STRATA_MARCH", "x86-64-v3")
    toolchain.versions["/usr/bin/llvm-profdata"] = "LLVM version 17.0.6\n"
    with pytest.raises(SystemExit, match="CC is clang 18 but llvm-profdata is LLVM 17"):
        release._toolchain()


def test_toolchain_linux_needs_cc_and_cxx(toolchain, monkeypatch):
    toolchain.use("linux")
    monkeypatch.setenv("STRATA_MARCH", "x86-64-v3")
    monkeypatch.delenv("CXX")
    with pytest.raises(SystemExit, match="CXX must be set for a release build"):
        release._toolchain()


# --- check-install's version comparison ------------------------------------


@pytest.fixture
def installed(monkeypatch, tmp_path):
    """check_install against a fake installed wheel: no import, no test run."""
    platlib = tmp_path / "site-packages"
    (platlib / "strata").mkdir(parents=True)
    modules = {
        "strata": types.SimpleNamespace(__version__=CURRENT),
        "strata._strata": types.SimpleNamespace(__file__=str(platlib / "strata" / "_strata.so")),
        "strata._dumps_hook": types.SimpleNamespace(
            __file__=str(platlib / "strata" / "_dumps_hook.so"),
        ),
    }
    runs = []

    def fake_run(cmd, check):
        runs.append(cmd)
        return types.SimpleNamespace(returncode=0)

    monkeypatch.setattr(
        release, "importlib", types.SimpleNamespace(import_module=modules.__getitem__)
    )
    monkeypatch.setattr(
        release,
        "sysconfig",
        types.SimpleNamespace(get_paths=lambda: {"platlib": str(platlib)}),
    )
    monkeypatch.setattr(release, "subprocess", types.SimpleNamespace(run=fake_run))
    monkeypatch.setattr(release, "INIT_FILE", _init_file(tmp_path))
    return types.SimpleNamespace(modules=modules, runs=runs)


def test_check_install_accepts_the_source_literal(installed, capsys):
    assert release.check_install(identity=False) == 0
    assert f"+ strata.__version__ == '{CURRENT}'" in capsys.readouterr().out
    assert len(installed.runs) == 1
    assert installed.runs[0][-1].endswith("py_tests.py")


@pytest.mark.parametrize("version", ["2026.8.9", f"{CURRENT}.1", f"{CURRENT}rc1", ""])
def test_check_install_refuses_another_version(installed, version):
    installed.modules["strata"].__version__ = version
    with pytest.raises(
        SystemExit,
        match=f"strata.__version__ is '{version}', the source literal '{CURRENT}'",
    ):
        release.check_install(identity=False)
    assert installed.runs == []


def test_check_install_identity_scans_both_installed_images(installed, monkeypatch):
    scanned = []
    monkeypatch.setattr(release, "_check_identity", lambda: None)
    monkeypatch.setattr(release, "_check_guard_isa", scanned.append)
    assert release.check_install(identity=True) == 0
    assert [[image.name for image in images] for images in scanned] == [
        ["_strata.so", "_dumps_hook.so"],
    ]


# --- check-install's guard ISA scan ----------------------------------------

ELF_X86_64 = b"\x7fELF" + bytes(14) + (62).to_bytes(2, "little")
MACHO_X86_64 = b"\xcf\xfa\xed\xfe" + (0x01000007).to_bytes(4, "little")
GUARD_TOOLS = frozenset(
    {"objdump", "llvm-objdump", "nm", "llvm-nm", "readelf", "llvm-readelf", "otool", "llvm-otool"},
)


@pytest.fixture
def guard_scan(monkeypatch, tmp_path):
    """_check_guard_isa on two fake x86-64 images, with platform, PATH and subprocess faked."""

    class GuardScan:
        def __init__(self) -> None:
            self.on_path = set(GUARD_TOOLS)
            self.runs: list[list[str]] = []
            self.returncode = 0
            self.images = [tmp_path / "_strata.so", tmp_path / "_dumps_hook.so"]
            self.write(ELF_X86_64)

        def write(self, head: bytes) -> None:
            for image in self.images:
                image.write_bytes(head + bytes(64))

        def use(self, machine: str = "x86_64", windows: bool = False) -> None:
            monkeypatch.setattr(release, "WINDOWS", windows)
            monkeypatch.setattr(release, "platform", types.SimpleNamespace(machine=lambda: machine))

        def run(self, cmd, check):
            self.runs.append(cmd)
            return types.SimpleNamespace(returncode=self.returncode)

    scan = GuardScan()
    monkeypatch.delenv("CI", raising=False)
    monkeypatch.setattr(
        shutil, "which", lambda name: f"/usr/bin/{name}" if name in scan.on_path else None
    )
    monkeypatch.setattr(release, "subprocess", types.SimpleNamespace(run=scan.run))
    return scan


def test_guard_scan_skips_windows_with_a_notice(guard_scan, capsys):
    guard_scan.use("AMD64", windows=True)
    release._check_guard_isa(guard_scan.images)
    out = capsys.readouterr().out
    assert "guard ISA scan SKIPPED on Windows" in out
    assert "T7's static evidence" in out
    assert guard_scan.runs == []


@pytest.mark.parametrize("machine", ["arm64", "aarch64"])
def test_guard_scan_skips_arm64_with_a_notice(guard_scan, capsys, machine):
    guard_scan.use(machine)
    release._check_guard_isa(guard_scan.images)
    assert f"guard ISA scan SKIPPED on {machine}" in capsys.readouterr().out
    assert guard_scan.runs == []


@pytest.mark.parametrize(
    ("head", "absent", "named"),
    [
        (ELF_X86_64, {"objdump", "llvm-objdump"}, "objdump"),
        (ELF_X86_64, {"readelf", "llvm-readelf"}, "readelf"),
        (ELF_X86_64, GUARD_TOOLS, "objdump, nm, readelf"),
        (MACHO_X86_64, {"otool", "llvm-otool"}, "otool"),
    ],
)
def test_guard_scan_skips_without_its_tools_locally_with_a_notice(
    guard_scan, capsys, head, absent, named
):
    guard_scan.use()
    guard_scan.write(head)
    guard_scan.on_path -= absent
    release._check_guard_isa(guard_scan.images)
    assert f"guard ISA scan SKIPPED: no {named} on PATH for _strata.so" in capsys.readouterr().out
    assert guard_scan.runs == []


@pytest.mark.parametrize(
    ("head", "absent", "named"),
    [
        (ELF_X86_64, {"objdump", "llvm-objdump"}, "objdump"),
        (MACHO_X86_64, {"otool", "llvm-otool"}, "otool"),
    ],
)
def test_guard_scan_fails_without_its_tools_under_ci(guard_scan, monkeypatch, head, absent, named):
    guard_scan.use()
    guard_scan.write(head)
    guard_scan.on_path -= absent
    monkeypatch.setenv("CI", "true")
    with pytest.raises(
        SystemExit,
        match=f"guard ISA scan cannot run under CI: no {named} on PATH for _strata.so",
    ):
        release._check_guard_isa(guard_scan.images)
    assert guard_scan.runs == []


def test_guard_scan_runs_under_ci_when_its_tools_are_there(guard_scan, monkeypatch):
    guard_scan.use()
    monkeypatch.setenv("CI", "true")
    release._check_guard_isa(guard_scan.images)
    assert len(guard_scan.runs) == 1


def test_guard_scan_takes_the_llvm_name_when_the_gnu_one_is_absent(guard_scan):
    guard_scan.on_path -= {"objdump", "nm", "readelf", "llvm-otool"}
    guard = runpy.run_path(str(release.GUARD_ISA_SCRIPT))
    assert guard["find_tools"]("elf") == (
        {
            "objdump": "/usr/bin/llvm-objdump",
            "nm": "/usr/bin/llvm-nm",
            "readelf": "/usr/bin/llvm-readelf",
        },
        [],
    )
    assert guard["find_tools"]("macho")[0]["otool"] == "/usr/bin/otool"


@pytest.mark.parametrize("head", [ELF_X86_64, MACHO_X86_64])
def test_guard_scan_runs_the_script_on_both_images(guard_scan, head):
    guard_scan.use()
    guard_scan.write(head)
    release._check_guard_isa(guard_scan.images)
    script = str(release.GUARD_ISA_SCRIPT)
    assert guard_scan.runs == [[sys.executable, script, *map(str, guard_scan.images)]]


def test_guard_scan_follows_the_entries_callees_short_of_the_module_body(capsys):
    guard = runpy.run_path(str(release.GUARD_ISA_SCRIPT))
    table = {
        0x10: "_PyInit__strata",
        0x20: "_PyInit__strata.cold.1",
        0x30: "helper",
        0x40: "deeper",
        0x50: "__ZN12_GLOBAL__N_120create_strata_moduleEv",
    }
    listings = {
        "_PyInit__strata": ["1: call 0x30 <helper>", "2: jmp 0x50 <x>"],
        "_PyInit__strata.cold.1": ["3: call 0x60 <PyErr_SetString@plt>"],
        "helper": ["4: call 0x40 <deeper>", "5: call 0x10 <_PyInit__strata>"],
        "deeper": [],
        "__ZN12_GLOBAL__N_120create_strata_moduleEv": [],
    }
    scanned = []

    def scanner(name):
        scanned.append(name)
        return 1, ["6: VEX vzeroupper"] if name == "deeper" else [], listings[name]

    assert guard["_scan_entries"](table, scanner) == 1
    assert scanned == ["_PyInit__strata", "_PyInit__strata.cold.1", "helper", "deeper"]
    assert "callee [deeper] instructions=1 above-baseline=1" in capsys.readouterr().out


def test_guard_scan_fails_on_a_finding(guard_scan):
    guard_scan.use()
    guard_scan.returncode = 1
    with pytest.raises(SystemExit, match="not held to the x86-64 baseline"):
        release._check_guard_isa(guard_scan.images)


def test_guard_scan_refuses_an_image_that_is_not_x86_64(guard_scan):
    guard_scan.use()
    guard_scan.write(b"\x7fELF" + bytes(14) + (183).to_bytes(2, "little"))
    with pytest.raises(SystemExit, match="not an x86-64 ELF or thin x86-64 Mach-O image"):
        release._check_guard_isa(guard_scan.images)
    assert guard_scan.runs == []


# --- setup.py release knobs ------------------------------------------------


@pytest.fixture
def setup_ns(monkeypatch):
    """setup.py's module namespace, read without setuptools or a build."""
    for name in (
        "PGO_MODE",
        "STRATA_ENABLE_LTO",
        "STRATA_PGO_PROFILE",
        "STRATA_WIN_COMPILER",
        "STRATA_MARCH",
        "SKIP_TESTS",
        "STRATA_HOOK_PGO_MODE",
        "STRATA_HOOK_PGO_PROFILE",
        "STRATA_EXTENSIONS",
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
    # run_path returns a copy; the functions read the module's own globals.
    return namespace["_compile_args"].__globals__


def _marches(args: list[str]) -> list[str]:
    return [arg for arg in args if arg.startswith("-march")]


posix_only = pytest.mark.skipif(sys.platform == "win32", reason="-march is the POSIX branch")


@posix_only
@pytest.mark.parametrize("value", ["none", " none "])
def test_strata_march_none_emits_no_march(setup_ns, monkeypatch, value):
    monkeypatch.setenv("STRATA_MARCH", value)
    monkeypatch.setitem(setup_ns, "_is_universal_build", lambda: False)
    assert _marches(setup_ns["_compile_args"](profiled=False)) == []
    assert _marches(setup_ns["_compile_args"]()) == []


@posix_only
def test_strata_march_names_the_target(setup_ns, monkeypatch):
    monkeypatch.setenv("STRATA_MARCH", "x86-64-v3")
    assert _marches(setup_ns["_compile_args"](profiled=False)) == ["-march=x86-64-v3"]


@posix_only
def test_strata_march_unset_tunes_for_the_host(setup_ns, monkeypatch):
    monkeypatch.setitem(setup_ns, "_is_universal_build", lambda: False)
    assert _marches(setup_ns["_compile_args"](profiled=False)) == ["-march=native"]
    monkeypatch.setitem(setup_ns, "_is_universal_build", lambda: True)
    assert _marches(setup_ns["_compile_args"](profiled=False)) == []


def test_profile_path_anchors_a_relative_path_to_the_checkout(setup_ns, monkeypatch):
    monkeypatch.setenv("STRATA_PGO_PROFILE", "build/release-pgo/strata.profdata")
    expected = PROJECT_ROOT / "build" / "release-pgo" / "strata.profdata"
    assert setup_ns["_profile_path"]("STRATA_PGO_PROFILE") == str(expected)
    assert Path(setup_ns["PROJECT_ROOT"]) == PROJECT_ROOT


def test_profile_path_passes_an_absolute_path_through(setup_ns, monkeypatch, tmp_path):
    absolute = tmp_path / "hook.profdata"
    monkeypatch.setenv("STRATA_HOOK_PGO_PROFILE", f" {absolute} ")
    assert setup_ns["_profile_path"]("STRATA_HOOK_PGO_PROFILE") == str(absolute)


@pytest.mark.parametrize("value", [None, "", "   "])
def test_profile_path_unset_is_empty(setup_ns, monkeypatch, value):
    if value is not None:
        monkeypatch.setenv("STRATA_PGO_PROFILE", value)
    assert setup_ns["_profile_path"]("STRATA_PGO_PROFILE") == ""

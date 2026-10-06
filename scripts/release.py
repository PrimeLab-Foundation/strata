#!/usr/bin/env python3
"""Release wheel engine: cibuildwheel's before-build and test-command steps.

``profile`` (before-build) trains the two PGO profiles a release wheel is
built against, with the project's own PGO script, in a private virtualenv:

- refuses ``SKIP_TESTS`` (the gates are what make a wheel release-ready);
- asserts and prints the toolchain the wheel will be compiled with: on Linux
  clang for ``CC`` and ``CXX``, of the same major as ``llvm-profdata``; on
  POSIX an explicit ``STRATA_MARCH``; on macOS ``MACOSX_DEPLOYMENT_TARGET``
  and an ``ARCHFLAGS`` of exactly the host architecture; on Windows clang-cl;
- runs ``scripts/pgo_build.sh`` (``scripts/pgo_build_clang_cl.py`` on
  Windows) into ``build/release-pgo`` without its verification benchmarks,
  stripped of the wheel build's own PGO variables, which the script sets
  phase by phase itself;
- asserts ``strata.profdata`` and ``hook.profdata`` exist and differ (the hook
  never carries ``_strata``'s profile), records both in ``profiles.json``;
- removes everything the training builds left behind -- ``build/`` apart from
  the profiles, the in-place extensions and the egg-info -- so the wheel
  build compiles every image from scratch against those profiles.

``check-install`` (test-command) runs in cibuildwheel's test virtualenv:
both extensions must import from its platlib and never from the checkout,
and ``strata.__version__`` must equal the literal in
``python/strata/__init__.py``. ``--identity`` also proves each image built
against its own profile alone (``build_identity.check_profiled``), with the
leg's ISA flag, never ``-march=native``, and ``-flto=thin`` on POSIX; on
x86-64 POSIX it then scans what runs before each image's CPU guard
(``scripts/check_guard_isa.py``), and prints why it skipped that scan anywhere
else or when the disassembler tools are not on ``PATH`` (under ``CI`` a missing
tool fails instead). The full Python suite
(``scripts/py_tests.py``) then runs against the installed wheel.

``profile`` deletes ``build/`` (``build/evidence`` included) and the in-tree
extensions, so it refuses to run outside cibuildwheel's build environment,
which sets ``CIBUILDWHEEL``.

``bump VERSION`` rewrites the ``__version__`` line of
``python/strata/__init__.py`` and nothing else. ``VERSION`` is
``YYYY.M.D[.N][rcK]``: no leading zeros, a real calendar date, ``.N`` (from 1)
a same-day re-release, ``rcK`` (from 1) a TestPyPI pre-release; it must be
strictly higher than the current literal in PEP 440 order, which for this
grammar is ``D rcK < D < D.N rcK < D.N``.

``check-tag TAG`` requires ``TAG == "v" + __version__``; ``--require-final``
also rejects an ``rcK`` version.

``sdist-check`` builds the sdist into ``dist/sdist-check``, runs
``twine check --strict`` on it, installs it -- through both build gates, in
pip's isolated build -- into a fresh virtualenv outside the checkout, and runs
``check-install`` there.

``verify-dist DIR --version V --sums FILE`` checks the upload directory of a
release: exactly one sdist and 25 wheels, one per CPython tag cp310--cp314 on
each leg (manylinux no newer than 2_28 on x86_64 and aarch64,
``macosx_13_0_x86_64``, ``macosx_11_0_arm64``, ``win_amd64``) and nothing else;
each wheel's METADATA names ``strata-plf`` at ``V`` with pyproject's
``Requires-Python`` and ``License-Expression: MIT``, and the wheel holds both
extension images with their ``.build.json``; the sdist holds both source lists,
every ``*.inc``, the fuzz corpus and ``scripts/build_identity.py``. It then runs
``twine check --strict`` and writes ``FILE`` -- never inside ``DIR`` -- in
``sha256sum`` format.

``check-promotion RUN_ID`` proves a Release run is fit for PyPI from the files
the publish workflow gathered: ``gh run view``'s JSON (workflow ``Release``,
event ``push``, concluded ``success``, on a ``v`` + final-version tag), the tag
ref's JSON (``refs/tags/<headBranch>`` naming the run's ``headSha``, an annotated
tag dereferenced through its tag object's JSON), the run's ``dist`` files against
its ``SHA256SUMS``, and TestPyPI's JSON for the version listing exactly those
filenames and sha256 digests. It reads no ``__version__``: it runs from the
dispatching ref, and the run's own check job proved the tagged commit's literal.
"""

from __future__ import annotations

import argparse
import ast
import datetime
import email.parser
import hashlib
import importlib
import importlib.util
import json
import os
import platform
import re
import runpy
import shutil
import subprocess
import sys
import sysconfig
import tarfile
import tempfile
import zipfile
from pathlib import Path
from typing import NoReturn

PROJECT_ROOT = Path(__file__).resolve().parents[1]
BUILD_DIR = PROJECT_ROOT / "build"
PGO_REL = "build/release-pgo"
PGO_DIR = PROJECT_ROOT / PGO_REL
VENV_DIR = BUILD_DIR / "release-venv"
STRATA_PROFILE = PGO_DIR / "strata.profdata"
HOOK_PROFILE = PGO_DIR / "hook.profdata"
GUARD_ISA_SCRIPT = PROJECT_ROOT / "scripts" / "check_guard_isa.py"
WINDOWS = sys.platform == "win32"

# The wheel build's own settings: the PGO script sets each of them per phase,
# and an inherited one would leak into a phase that must not see it (the
# hook's `use` into phase 1, a profile into the instrumented build).
STRIPPED_ENV = (
    "PGO_MODE",
    "STRATA_ENABLE_LTO",
    "STRATA_PGO_PROFILE",
    "STRATA_HOOK_PGO_MODE",
    "STRATA_HOOK_PGO_PROFILE",
    "STRATA_EXTENSIONS",
)
LEFTOVER_GLOBS = ("python/strata/_strata*", "python/strata/_dumps_hook*", "python/*.egg-info")

INIT_FILE = PROJECT_ROOT / "python" / "strata" / "__init__.py"
SDIST_DIR = PROJECT_ROOT / "dist" / "sdist-check"
# [0-9], never \d: a str pattern's \d also matches non-ASCII digits.
VERSION_RE = re.compile(
    r"([1-9][0-9]{3})\.([1-9][0-9]?)\.([1-9][0-9]?)(?:\.([1-9][0-9]*))?(?:rc([1-9][0-9]*))?",
)
VERSION_LINE_RE = re.compile(r'^__version__ = "([^"\r\n]*)"\r?$', re.MULTILINE)

PROJECT_NAME = "strata-plf"
DIST_NAME = "strata_plf"
LICENSE_EXPRESSION = "MIT"
RELEASE_WORKFLOW = "Release"
PYTHON_TAGS = ("cp310", "cp311", "cp312", "cp313", "cp314")
FIXED_LEGS = ("macosx_13_0_x86_64", "macosx_11_0_arm64", "win_amd64")
LEGS = ("manylinux_x86_64", "manylinux_aarch64", *FIXED_LEGS)
MANYLINUX_CEILING = (2, 28)
LEGACY_MANYLINUX = {"manylinux1": (2, 5), "manylinux2010": (2, 12), "manylinux2014": (2, 17)}
EXTENSIONS = ("_strata", "_dumps_hook")
SDIST_FILES = (
    "src/strata/core_sources.txt",
    "src/strata/native_sources.txt",
    "scripts/build_identity.py",
)
WHEEL_RE = re.compile(
    rf"{DIST_NAME}-(?P<version>[^-]+)-(?P<python>cp3[0-9]+)-(?P<abi>cp3[0-9]+)"
    r"-(?P<platform>[A-Za-z0-9_.]+)\.whl",
)
MANYLINUX_RE = re.compile(r"manylinux_([0-9]+)_([0-9]+)_(x86_64|aarch64)")
LEGACY_MANYLINUX_RE = re.compile(r"(manylinux1|manylinux2010|manylinux2014)_(x86_64|aarch64)")
SUMS_LINE_RE = re.compile(r"([0-9a-f]{64})  ([^\s/\\]+)")


def _fail(message: str) -> NoReturn:
    raise SystemExit(f"release: {message}")


def _run(cmd: list[str], env: dict[str, str] | None = None) -> None:
    print("+ " + " ".join(cmd), flush=True)
    completed = subprocess.run(cmd, cwd=PROJECT_ROOT, env=env, check=False)
    if completed.returncode != 0:
        _fail(f"command failed ({completed.returncode}): {' '.join(cmd)}")


def _output(cmd: list[str]) -> str:
    try:
        completed = subprocess.run(cmd, capture_output=True, text=True, check=False)
    except OSError as exc:
        _fail(f"cannot run {cmd[0]}: {exc}")
    if completed.returncode != 0:
        _fail(f"{' '.join(cmd)} exited {completed.returncode}: {completed.stderr.strip()}")
    return completed.stdout + completed.stderr


def _major(text: str, pattern: str, what: str) -> int:
    match = re.search(pattern, text)
    if match is None:
        _fail(f"cannot read the major version of {what} from: {text.strip()[:200]}")
    return int(match.group(1))


def _require_env(name: str) -> str:
    value = os.environ.get(name, "").strip()
    if not value:
        _fail(f"{name} must be set for a release build")
    return value


def _toolchain() -> dict[str, str]:
    """Assert the leg's toolchain contract and return what was checked, for the log."""
    facts: dict[str, str] = {"platform": sys.platform, "machine": platform.machine()}
    if WINDOWS:
        compiler = os.environ.get("STRATA_WIN_COMPILER", "").strip()
        if compiler != "clang-cl":
            _fail(f"STRATA_WIN_COMPILER must be clang-cl, not {compiler!r}")
        facts["STRATA_WIN_COMPILER"] = compiler
        return facts
    facts["STRATA_MARCH"] = _require_env("STRATA_MARCH")
    if sys.platform == "darwin":
        facts["MACOSX_DEPLOYMENT_TARGET"] = _require_env("MACOSX_DEPLOYMENT_TARGET")
        archflags = os.environ.get("ARCHFLAGS", "")
        expected = f"-arch {platform.machine()}"
        if archflags != expected:
            _fail(f"ARCHFLAGS must be exactly {expected!r}, not {archflags!r}")
        facts["ARCHFLAGS"] = archflags
        return facts
    majors = {}
    for var in ("CC", "CXX"):
        compiler = _require_env(var)
        version = _output([compiler, "--version"])
        if "clang version" not in version:
            _fail(f"{var}={compiler} is not clang: {version.splitlines()[0] if version else ''}")
        majors[var] = _major(version, r"clang version (\d+)\.", compiler)
        facts[var] = f"{compiler} ({version.splitlines()[0].strip()})"
    profdata = shutil.which("llvm-profdata")
    if profdata is None:
        _fail("llvm-profdata is not on PATH")
    profdata_major = _major(_output([profdata, "--version"]), r"LLVM version (\d+)\.", profdata)
    facts["llvm-profdata"] = f"{profdata} (LLVM {profdata_major})"
    for var, major in majors.items():
        if major != profdata_major:
            _fail(f"{var} is clang {major} but llvm-profdata is LLVM {profdata_major}")
    return facts


def _sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _remove(path: Path) -> None:
    if path.is_dir() and not path.is_symlink():
        shutil.rmtree(path)
    else:
        path.unlink()


def _leftovers() -> list[Path]:
    found = [entry for entry in BUILD_DIR.iterdir() if entry != PGO_DIR]
    for pattern in LEFTOVER_GLOBS:
        found.extend(PROJECT_ROOT.glob(pattern))
    return found


def profile() -> int:
    if not os.environ.get("CIBUILDWHEEL", "").strip():
        _fail(
            "profile runs only inside cibuildwheel's build environment (CIBUILDWHEEL is unset):"
            " it deletes build/ -- build/evidence included -- and the in-tree extensions."
            " Build release wheels through cibuildwheel (make release-wheel-linux).",
        )
    if "SKIP_TESTS" in os.environ:
        _fail("SKIP_TESTS is set; a release wheel is built through both test gates")
    facts = _toolchain()
    print("==> release: toolchain", flush=True)
    for key, value in facts.items():
        print(f"    {key}: {value}", flush=True)

    env = {key: value for key, value in os.environ.items() if key not in STRIPPED_ENV}
    for path in (PGO_DIR, VENV_DIR):
        if path.exists():
            shutil.rmtree(path)
    PGO_DIR.mkdir(parents=True)
    _run([sys.executable, "-m", "venv", str(VENV_DIR)], env=env)
    vpy = VENV_DIR / ("Scripts/python.exe" if WINDOWS else "bin/python")
    _run([str(vpy), "-m", "pip", "install", "cmake>=3.20", "pytest>=7"], env=env)

    env.update({"VENV": str(VENV_DIR), "PGO_DIR": PGO_REL, "PGO_VERIFY_BENCH": "0"})
    if WINDOWS:
        _run([str(vpy), "scripts/pgo_build_clang_cl.py"], env=env)
    else:
        _run(["bash", "scripts/pgo_build.sh"], env=env)

    for path in (STRATA_PROFILE, HOOK_PROFILE):
        if not path.is_file():
            _fail(f"the PGO script wrote no {path}")
    hashes = {path.name: _sha256(path) for path in (STRATA_PROFILE, HOOK_PROFILE)}
    if hashes[STRATA_PROFILE.name] == hashes[HOOK_PROFILE.name]:
        _fail("hook.profdata is _strata's profile; the hook must carry a profile of its own")
    record = {
        "schema_version": 1,
        "profiles": {
            name: {
                "path": f"{PGO_REL}/{name}",
                "sha256": digest,
                "bytes": (PGO_DIR / name).stat().st_size,
            }
            for name, digest in hashes.items()
        },
        "toolchain": facts,
        "python": sys.version,
    }
    (PGO_DIR / "profiles.json").write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
    print(f"==> release: profiles recorded in {PGO_REL}/profiles.json", flush=True)

    for path in _leftovers():
        print(f"    removing {path.relative_to(PROJECT_ROOT)}", flush=True)
        _remove(path)
    remaining = _leftovers()
    if remaining:
        _fail(f"training leftovers survived cleanup: {[str(p) for p in remaining]}")
    print("==> release: profiles ready, build tree clean", flush=True)
    return 0


def _version_literal(init: Path | None = None) -> str:
    init = init or INIT_FILE
    for node in ast.parse(init.read_text(encoding="utf-8")).body:
        if (
            isinstance(node, ast.Assign)
            and any(isinstance(t, ast.Name) and t.id == "__version__" for t in node.targets)
            and isinstance(node.value, ast.Constant)
            and isinstance(node.value.value, str)
        ):
            return node.value.value
    _fail(f"no __version__ string literal in {init}")


def version_key(version: str) -> tuple[int, int, int, int, int, int]:
    """The PEP 440 sort key of a ``YYYY.M.D[.N][rcK]`` version; exits on any other text.

    Release segments compare first (an absent ``.N`` is 0), then a final release
    sorts after every ``rcK`` of the same release.
    """
    match = VERSION_RE.fullmatch(version)
    if match is None:
        _fail(
            f"{version!r} is not YYYY.M.D[.N][rcK] (no leading zeros; .N and rcK count from 1)",
        )
    year, month, day, serial, rc = match.groups()
    try:
        datetime.date(int(year), int(month), int(day))
    except ValueError:
        _fail(f"{version!r} does not name a calendar date")
    pre = (0, int(rc)) if rc else (1, 0)
    return (int(year), int(month), int(day), int(serial or 0), *pre)


def bump(version: str, init: Path | None = None) -> int:
    init = init or INIT_FILE
    new_key = version_key(version)
    text = init.read_bytes().decode("utf-8")
    lines = list(VERSION_LINE_RE.finditer(text))
    if len(lines) != 1:
        _fail(f'expected exactly one `__version__ = "..."` line in {init}, found {len(lines)}')
    line = lines[0]
    current = _version_literal(init)
    if line.group(1) != current:
        _fail(f"the __version__ line reads {line.group(1)!r}, the module's literal {current!r}")
    if new_key <= version_key(current):
        _fail(f"{version} is not higher than the current {current} in PEP 440 order")
    init.write_bytes((text[: line.start(1)] + version + text[line.end(1) :]).encode("utf-8"))
    print(f"release: __version__ {current} -> {version} in {init}", flush=True)
    return 0


def check_tag(tag: str, require_final: bool, init: Path | None = None) -> int:
    literal = _version_literal(init)
    key = version_key(literal)
    if tag != f"v{literal}":
        _fail(f"tag {tag!r} does not match __version__ {literal!r} (expected 'v{literal}')")
    if require_final and key[4] == 0:
        _fail(f"{literal} is a release candidate; --require-final needs a version without rcK")
    print(f"release: tag {tag} matches __version__ {literal}", flush=True)
    return 0


def _isa_problems(flags: set[str]) -> list[str]:
    problems = []
    if "-march=native" in flags:
        problems.append("built with -march=native")
    if WINDOWS:
        if "/arch:AVX2" not in flags:
            problems.append("built without /arch:AVX2")
        return problems
    march = os.environ.get("STRATA_MARCH", "").strip()
    marches = sorted(flag for flag in flags if flag.startswith("-march="))
    if not march:
        problems.append("STRATA_MARCH is not set, so the expected ISA flag is unknown")
    elif march == "none":
        if marches:
            problems.append(f"STRATA_MARCH=none, yet built with {marches}")
    elif marches != [f"-march={march}"]:
        problems.append(f"expected -march={march} alone, built with {marches}")
    if "-flto=thin" not in flags:
        problems.append("built without -flto=thin")
    return problems


def _check_identity() -> None:
    identity = runpy.run_path(str(PROJECT_ROOT / "scripts" / "build_identity.py"))
    for name, own, foreign in (
        ("strata._strata", STRATA_PROFILE, HOOK_PROFILE),
        ("strata._dumps_hook", HOOK_PROFILE, STRATA_PROFILE),
    ):
        if identity["check_profiled"](name, own, foreign) != 0:
            _fail(f"{name} is not built against {own.name} alone")
        image = identity["module_image"](name)
        packet = json.loads(image.with_name(image.name + ".build.json").read_text(encoding="utf-8"))
        flags = {str(part) for command in packet.get("commands") or [] for part in command}
        problems = _isa_problems(flags)
        if problems:
            _fail(f"{name}: " + "; ".join(problems))
        source = packet.get("source") or {}
        print(
            f"+ {name}: ISA flags as the leg declares; source commit {source.get('commit')}"
            f" dirty={source.get('dirty')}",
            flush=True,
        )


def _check_guard_isa(images: list[Path]) -> None:
    if WINDOWS:
        print(
            "+ guard ISA scan SKIPPED on Windows: no disassembler in the test environment;"
            " clang-cl's coverage is T7's static evidence (release_pipeline.md, CPU guard)",
            flush=True,
        )
        return
    machine = platform.machine()
    if machine != "x86_64":
        print(f"+ guard ISA scan SKIPPED on {machine}: the CPU guard is x86-64 only", flush=True)
        return
    guard = runpy.run_path(str(GUARD_ISA_SCRIPT))
    for image in images:
        missing = guard["find_tools"](guard["image_format"](image))[1]
        if missing:
            message = f"no {', '.join(missing)} on PATH for {image.name}"
            if os.environ.get("CI"):
                _fail(f"guard ISA scan cannot run under CI: {message}")
            print(f"+ guard ISA scan SKIPPED: {message}", flush=True)
            return
    cmd = [sys.executable, str(GUARD_ISA_SCRIPT), *map(str, images)]
    print("+ " + " ".join(cmd), flush=True)
    if subprocess.run(cmd, check=False).returncode != 0:
        _fail("code that runs before the CPU guard is not held to the x86-64 baseline (above)")


def check_install(identity: bool) -> int:
    platlib = Path(sysconfig.get_paths()["platlib"]).resolve()
    strata = importlib.import_module("strata")
    images = []
    for name in ("strata._strata", "strata._dumps_hook"):
        image = Path(importlib.import_module(name).__file__).resolve()
        if not image.is_relative_to(platlib) or image.is_relative_to(PROJECT_ROOT):
            _fail(f"{name} imports from {image}, not from the installed wheel in {platlib}")
        print(f"+ {name} imports from {image}", flush=True)
        images.append(image)
    expected = _version_literal()
    if strata.__version__ != expected:
        _fail(f"strata.__version__ is {strata.__version__!r}, the source literal {expected!r}")
    print(f"+ strata.__version__ == {expected!r}", flush=True)
    if identity:
        _check_identity()
        _check_guard_isa(images)
    cmd = [sys.executable, str(PROJECT_ROOT / "scripts" / "py_tests.py")]
    print("+ " + " ".join(cmd), flush=True)
    return subprocess.run(cmd, check=False).returncode


def sdist_check() -> int:
    if "SKIP_TESTS" in os.environ:
        _fail("SKIP_TESTS is set; the sdist is installed through both test gates")
    for module in ("build", "twine"):
        if importlib.util.find_spec(module) is None:
            _fail(f"{module} is not installed for {sys.executable}; run `make install-release`")
    if SDIST_DIR.exists():
        shutil.rmtree(SDIST_DIR)
    _run([sys.executable, "-m", "build", "--sdist", "--outdir", str(SDIST_DIR)])
    sdists = sorted(SDIST_DIR.glob("*.tar.gz"))
    if len(sdists) != 1:
        _fail(f"expected one sdist in {SDIST_DIR}, found {[p.name for p in sdists]}")
    sdist = sdists[0]
    _run([sys.executable, "-m", "twine", "check", "--strict", str(sdist)])
    # A plain build: no PGO or LTO setting of the caller's shell reaches it.
    env = {key: value for key, value in os.environ.items() if key not in STRIPPED_ENV}
    # Outside the checkout: check-install refuses an extension imported from under it.
    with tempfile.TemporaryDirectory(prefix="strata-sdist-check-") as scratch:
        venv = Path(scratch) / "venv"
        _run([sys.executable, "-m", "venv", str(venv)], env=env)
        vpy = venv / ("Scripts/python.exe" if WINDOWS else "bin/python")
        _run([str(vpy), "-m", "pip", "install", "pytest>=7"], env=env)
        _run([str(vpy), "-m", "pip", "install", "--no-cache-dir", str(sdist)], env=env)
        _run([str(vpy), str(PROJECT_ROOT / "scripts" / "release.py"), "check-install"], env=env)
    print(f"==> release: {sdist.relative_to(PROJECT_ROOT)} passed", flush=True)
    return 0


def wheel_slot(name: str, version: str) -> tuple[str, str]:
    """The (CPython tag, leg) a release wheel's filename names; exits on any other name."""
    match = WHEEL_RE.fullmatch(name)
    if match is None:
        _fail(f"{name} is not a {DIST_NAME} CPython wheel filename")
    if match["version"] != version:
        _fail(f"{name} carries version {match['version']}, not {version}")
    python, abi = match["python"], match["abi"]
    if python not in PYTHON_TAGS or abi != python:
        _fail(f"{name}: tags {python}-{abi}, expected one of {PYTHON_TAGS} with the same ABI tag")
    platform_tag = match["platform"]
    if platform_tag in FIXED_LEGS:
        return python, platform_tag
    arches = set()
    for tag in platform_tag.split("."):
        modern, legacy = MANYLINUX_RE.fullmatch(tag), LEGACY_MANYLINUX_RE.fullmatch(tag)
        if modern:
            glibc, arch = (int(modern[1]), int(modern[2])), modern[3]
        elif legacy:
            glibc, arch = LEGACY_MANYLINUX[legacy[1]], legacy[2]
        else:
            _fail(f"{name}: platform tag {tag} is on none of the release legs {LEGS}")
        if glibc > MANYLINUX_CEILING:
            _fail(f"{name}: {tag} is newer than manylinux_2_28")
        arches.add(arch)
    if len(arches) != 1:
        _fail(f"{name}: its platform tags name {len(arches)} architectures")
    return python, f"manylinux_{arches.pop()}"


def dist_layout(names: list[str], version: str) -> tuple[str, dict[tuple[str, str], str]]:
    """One sdist, one wheel per CPython tag and leg, nothing else; returns them by slot."""
    sdist = f"{DIST_NAME}-{version}.tar.gz"
    sdists = [name for name in names if name.endswith(".tar.gz")]
    if sdists != [sdist]:
        _fail(f"expected exactly one sdist, {sdist}; found {sdists}")
    wheels: dict[tuple[str, str], str] = {}
    for name in names:
        if name == sdist:
            continue
        if not name.endswith(".whl"):
            _fail(f"{name} is neither the sdist nor a wheel")
        slot = wheel_slot(name, version)
        if slot in wheels:
            _fail(f"two wheels for {slot[0]} on {slot[1]}: {wheels[slot]} and {name}")
        wheels[slot] = name
    missing = [f"{py}-{leg}" for py in PYTHON_TAGS for leg in LEGS if (py, leg) not in wheels]
    if missing:
        total = len(PYTHON_TAGS) * len(LEGS)
        _fail(f"{len(wheels)} of {total} wheels; missing {', '.join(missing)}")
    return sdist, wheels


def _requires_python() -> str:
    text = (PROJECT_ROOT / "pyproject.toml").read_text(encoding="utf-8")
    match = re.search(r'^requires-python = "([^"]+)"\r?$', text, re.MULTILINE)
    if match is None:
        _fail("pyproject.toml has no requires-python line")
    return match.group(1)


def _wheel_problems(path: Path, version: str, python: str, requires_python: str) -> list[str]:
    metadata_name = f"{DIST_NAME}-{version}.dist-info/METADATA"
    with zipfile.ZipFile(path) as wheel:
        names = set(wheel.namelist())
        if metadata_name not in names:
            return [f"no {metadata_name}"]
        metadata = email.parser.BytesHeaderParser().parsebytes(wheel.read(metadata_name))
    problems = []
    for field, expected in (
        ("Name", PROJECT_NAME),
        ("Version", version),
        ("Requires-Python", requires_python),
        ("License-Expression", LICENSE_EXPRESSION),
    ):
        values = metadata.get_all(field) or []
        if values != [expected]:
            problems.append(f"METADATA {field} is {values}, expected [{expected!r}]")
    digits = python[2:]
    for extension in EXTENSIONS:
        image_re = re.compile(
            rf"strata/{extension}\.(?:cpython-{digits}-[^/]*\.so|cp{digits}-win_amd64\.pyd)",
        )
        images = sorted(name for name in names if image_re.fullmatch(name))
        if len(images) != 1:
            problems.append(f"expected one strata.{extension} image for {python}, found {images}")
        elif images[0] + ".build.json" not in names:
            problems.append(f"no {images[0]}.build.json")
    return problems


def _sdist_required() -> list[str]:
    """What the sdist must carry, read from the checkout so a new seed or table is covered."""
    incs = sorted(
        path.relative_to(PROJECT_ROOT).as_posix()
        for top in ("include", "src")
        for path in (PROJECT_ROOT / top).rglob("*.inc")
    )
    corpus_dir = PROJECT_ROOT / "tests" / "fuzz" / "corpus"
    corpus = sorted(
        p.relative_to(PROJECT_ROOT).as_posix() for p in corpus_dir.rglob("*") if p.is_file()
    )
    if not incs or not corpus:
        _fail("the checkout holds no *.inc file or no fuzz corpus to check the sdist against")
    return [*SDIST_FILES, *incs, *corpus]


def _sdist_problems(path: Path, version: str, required: list[str]) -> list[str]:
    root = f"{DIST_NAME}-{version}/"
    with tarfile.open(path, "r:gz") as sdist:
        names = {member.name for member in sdist.getmembers() if member.isfile()}
    missing = [rel for rel in required if root + rel not in names]
    if missing:
        return [f"lacks {len(missing)} required files: {missing[:10]}"]
    return []


def _dist_files(dist_dir: Path, sums: Path) -> list[Path]:
    if not dist_dir.is_dir():
        _fail(f"{dist_dir} is not a directory")
    if sums.resolve().is_relative_to(dist_dir.resolve()):
        _fail(f"{sums} is inside the upload directory {dist_dir}; SHA256SUMS belongs outside it")
    entries = sorted(dist_dir.iterdir())
    others = [entry.name for entry in entries if not entry.is_file() or entry.is_symlink()]
    if others:
        _fail(f"{dist_dir} holds entries that are not regular files: {others}")
    return entries


def verify_dist(
    dist_dir: Path,
    version: str,
    sums: Path,
    *,
    run_twine: bool = True,
    required: list[str] | None = None,
) -> int:
    version_key(version)
    entries = _dist_files(dist_dir, sums)
    sdist, wheels = dist_layout([entry.name for entry in entries], version)
    requires_python = _requires_python()
    problems = []
    for (python, _leg), name in sorted(wheels.items()):
        found = _wheel_problems(dist_dir / name, version, python, requires_python)
        problems += [f"{name}: {problem}" for problem in found]
    found = _sdist_problems(
        dist_dir / sdist, version, _sdist_required() if required is None else required
    )
    problems += [f"{sdist}: {problem}" for problem in found]
    if problems:
        _fail("the distribution fails verification:\n  " + "\n  ".join(problems))
    if run_twine:
        _run([sys.executable, "-m", "twine", "check", "--strict", *map(str, entries)])
    sums.parent.mkdir(parents=True, exist_ok=True)
    sums.write_bytes("".join(f"{_sha256(e)}  {e.name}\n" for e in entries).encode("utf-8"))
    print(f"release: {len(entries)} files verified for {version}; digests in {sums}", flush=True)
    return 0


def read_sums(path: Path) -> dict[str, str]:
    listed: dict[str, str] = {}
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        match = SUMS_LINE_RE.fullmatch(line)
        if match is None:
            _fail(f"{path}:{number} is not '<sha256>  <filename>': {line!r}")
        digest, name = match.groups()
        if name in listed:
            _fail(f"{path} lists {name} twice")
        listed[name] = digest
    if not listed:
        _fail(f"{path} lists no files")
    return listed


def compare_sums(listed: dict[str, str], actual: dict[str, str], what: str) -> None:
    missing = sorted(listed.keys() - actual.keys())
    extra = sorted(actual.keys() - listed.keys())
    differ = sorted(name for name in listed.keys() & actual.keys() if listed[name] != actual[name])
    if missing or extra or differ:
        _fail(
            f"{what} does not match SHA256SUMS: missing {missing}, extra {extra},"
            f" digest differs {differ}",
        )


def _tag_target(tag: str, tag_ref_json: Path, tag_object_json: Path | None) -> str:
    """The commit ``refs/tags/<tag>`` names, an annotated tag dereferenced once.

    @p tag_ref_json is ``gh api repos/R/git/ref/tags/<tag>``; @p tag_object_json is
    ``gh api repos/R/git/tags/<sha>`` for the tag object that ref names, when it is one.
    """
    ref = json.loads(tag_ref_json.read_text(encoding="utf-8"))
    if ref.get("ref") != f"refs/tags/{tag}":
        _fail(f"headBranch {tag!r} is not the tag ref: the ref JSON describes {ref.get('ref')!r}")
    target = ref.get("object") or {}
    if target.get("type") == "commit":
        return str(target.get("sha"))
    if target.get("type") != "tag":
        _fail(f"refs/tags/{tag} names a {target.get('type')!r}, not a commit or an annotated tag")
    if tag_object_json is None:
        _fail(f"refs/tags/{tag} is an annotated tag; --tag-object-json must give its tag object")
    annotated = json.loads(tag_object_json.read_text(encoding="utf-8"))
    if annotated.get("sha") != target.get("sha"):
        _fail(
            f"the tag object JSON describes {annotated.get('sha')!r},"
            f" not refs/tags/{tag}'s {target.get('sha')!r}",
        )
    commit = annotated.get("object") or {}
    if commit.get("type") != "commit":
        _fail(f"the annotated tag {tag} names a {commit.get('type')!r}, not a commit")
    return str(commit.get("sha"))


def check_promotion(
    run_id: str,
    *,
    run_json: Path,
    dist_dir: Path,
    sums: Path,
    testpypi_json: Path,
    tag_ref_json: Path,
    tag_object_json: Path | None = None,
) -> int:
    """Prove a Release run is fit for PyPI from its run, tag, files and TestPyPI evidence.

    The version comes from the tag, never from the checked-out tree: publish-pypi.yml
    runs the dispatching ref's code, and the Release run's own check job already proved
    the tagged commit's ``__version__``. The tag must name the run's commit.
    """
    if re.fullmatch(r"[0-9]+", run_id) is None:
        _fail(f"{run_id!r} is not a workflow run id")
    run = json.loads(run_json.read_text(encoding="utf-8"))
    for field, expected in (
        ("databaseId", int(run_id)),
        ("workflowName", RELEASE_WORKFLOW),
        ("event", "push"),
        ("status", "completed"),
        ("conclusion", "success"),
    ):
        if run.get(field) != expected:
            _fail(f"run {run_id}: {field} is {run.get(field)!r}, expected {expected!r}")
    tag = run.get("headBranch")
    if not isinstance(tag, str) or not tag.startswith("v"):
        _fail(f"run {run_id}: headBranch is {tag!r}, not a v* tag")
    version = tag[1:]
    if version_key(version)[4] == 0:
        _fail(f"{version} is a release candidate; PyPI takes a version without rcK")
    head_sha = run.get("headSha")
    if not isinstance(head_sha, str) or re.fullmatch(r"[0-9a-f]{40}", head_sha) is None:
        _fail(f"run {run_id}: headSha is {head_sha!r}, not a commit sha")
    target = _tag_target(tag, tag_ref_json, tag_object_json)
    if target != head_sha:
        _fail(f"refs/tags/{tag} names {target}, not run {run_id}'s headSha {head_sha}")
    entries = _dist_files(dist_dir, sums)
    dist_layout([entry.name for entry in entries], version)
    listed = read_sums(sums)
    compare_sums(listed, {entry.name: _sha256(entry) for entry in entries}, f"run {run_id}'s files")
    testpypi = json.loads(testpypi_json.read_text(encoding="utf-8"))
    served = (testpypi.get("info") or {}).get("version")
    if served != version:
        _fail(f"the TestPyPI JSON describes version {served!r}, not {version}")
    uploaded: dict[str, str] = {}
    for url in testpypi.get("urls") or []:
        name = url.get("filename")
        if name in uploaded:
            _fail(f"TestPyPI lists {name} twice")
        uploaded[name] = (url.get("digests") or {}).get("sha256")
    compare_sums(listed, uploaded, f"the TestPyPI release {version}")
    print(
        f"release: run {run_id} ({tag} at {head_sha}) is promotable: its {len(listed)} files"
        " are on TestPyPI with the same sha256 digests",
        flush=True,
    )
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    commands = parser.add_subparsers(dest="command", required=True)
    commands.add_parser("profile", help="cibuildwheel before-build: train the release profiles")
    check = commands.add_parser("check-install", help="cibuildwheel test-command")
    check.add_argument(
        "--identity",
        action="store_true",
        help="also prove each image's profile, ISA flag and LTO from its build identity",
    )
    bump_parser = commands.add_parser("bump", help="rewrite the __version__ literal")
    bump_parser.add_argument("version", help="YYYY.M.D[.N][rcK], higher than the current one")
    tag = commands.add_parser("check-tag", help="the tag must be 'v' + __version__")
    tag.add_argument("tag")
    tag.add_argument("--require-final", action="store_true", help="reject an rcK version")
    commands.add_parser("sdist-check", help="build, twine-check and gate-install the sdist")
    verify = commands.add_parser("verify-dist", help="check the release files, write SHA256SUMS")
    verify.add_argument("dist", type=Path, help="the upload directory")
    verify.add_argument("--version", required=True, help="the version every file must carry")
    verify.add_argument(
        "--sums", type=Path, required=True, help="SHA256SUMS to write, outside DIST"
    )
    promote = commands.add_parser("check-promotion", help="prove a Release run matches TestPyPI")
    promote.add_argument("run_id")
    promote.add_argument(
        "--run-json",
        type=Path,
        required=True,
        help="gh run view RUN_ID --json databaseId,workflowName,event,status,conclusion,"
        "headBranch,headSha",
    )
    promote.add_argument("--dist", type=Path, required=True, help="the run's dist artifact")
    promote.add_argument("--sums", type=Path, required=True, help="the run's SHA256SUMS")
    promote.add_argument("--testpypi-json", type=Path, required=True, help="TestPyPI's JSON for V")
    promote.add_argument(
        "--tag-ref-json",
        type=Path,
        required=True,
        help="gh api repos/R/git/ref/tags/TAG, TAG being the run's headBranch",
    )
    promote.add_argument(
        "--tag-object-json",
        type=Path,
        help="gh api repos/R/git/tags/SHA for the annotated tag object that ref names",
    )
    args = parser.parse_args(argv)
    if args.command == "profile":
        return profile()
    if args.command == "bump":
        return bump(args.version)
    if args.command == "check-tag":
        return check_tag(args.tag, args.require_final)
    if args.command == "sdist-check":
        return sdist_check()
    if args.command == "verify-dist":
        return verify_dist(args.dist, args.version, args.sums)
    if args.command == "check-promotion":
        return check_promotion(
            args.run_id,
            run_json=args.run_json,
            dist_dir=args.dist,
            sums=args.sums,
            testpypi_json=args.testpypi_json,
            tag_ref_json=args.tag_ref_json,
            tag_object_json=args.tag_object_json,
        )
    return check_install(args.identity)


if __name__ == "__main__":
    raise SystemExit(main())

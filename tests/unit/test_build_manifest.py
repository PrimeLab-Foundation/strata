"""Contract test for the shared C++ source manifest.

The previous implementation enumerated its core sources in four places -- the
shell test harness, CMakeLists.txt, the coverage rule and the fuzz CMakeLists --
and they drifted until the coverage build compiled nothing at all
(docs/build-and-test/SKILL.md, known-broken inventory).

The rebuild keeps one list, `src/strata/core_sources.txt`, read by CMake and by
setup.py. These tests make that structural: a core source that is not listed, or
a listed file that does not exist, fails here rather than silently producing an
extension and a test binary built from different code.
"""

from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[2]
MANIFEST = PROJECT_ROOT / "src" / "strata" / "core_sources.txt"
CORE_DIR = PROJECT_ROOT / "src" / "strata"
BINDINGS_DIR = CORE_DIR / "bindings"


def _entries() -> list[str]:
    lines = MANIFEST.read_text(encoding="utf-8").splitlines()
    return [s for s in (line.strip() for line in lines) if s and not s.startswith("#")]


def _core_translation_units() -> set[str]:
    # as_posix(): the manifest uses forward slashes; str(Path) would compare
    # backslashed paths on Windows and report every source as missing.
    return {
        p.relative_to(PROJECT_ROOT).as_posix()
        for p in CORE_DIR.rglob("*.cpp")
        if BINDINGS_DIR not in p.parents
    }


def test_manifest_lists_every_core_translation_unit():
    missing = _core_translation_units() - set(_entries())
    assert not missing, f"core sources missing from core_sources.txt: {sorted(missing)}"


def test_manifest_lists_nothing_that_does_not_exist():
    for entry in _entries():
        assert (PROJECT_ROOT / entry).is_file(), f"{entry} is listed but does not exist"


def test_manifest_has_no_duplicates():
    entries = _entries()
    assert len(entries) == len(set(entries)), "core_sources.txt repeats an entry"


def test_manifest_excludes_binding_sources():
    """Bindings are CPython-dependent and belong to setup.py alone."""
    for entry in _entries():
        assert not entry.startswith("src/strata/bindings/"), (
            f"{entry} is a binding source and must not be in the core manifest"
        )


def test_manifest_stays_ascii():
    """CMake's file(STRINGS) treats non-ASCII bytes as separators."""
    raw = MANIFEST.read_bytes()
    assert all(byte < 0x80 for byte in raw), "core_sources.txt must be ASCII-only"


def test_both_build_systems_read_the_manifest():
    """Neither consumer may keep its own copy of the list."""
    for name in ("CMakeLists.txt", "setup.py"):
        text = (PROJECT_ROOT / name).read_text(encoding="utf-8")
        assert "core_sources.txt" in text, f"{name} does not read the shared manifest"

    # A core source named directly in a build file would be a second list.
    for name in ("CMakeLists.txt", "setup.py", "tests/fuzz/CMakeLists.txt"):
        text = (PROJECT_ROOT / name).read_text(encoding="utf-8")
        for entry in _core_translation_units():
            assert entry not in text, f"{name} names {entry} directly instead of via the manifest"

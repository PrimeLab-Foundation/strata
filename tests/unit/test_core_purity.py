"""Contract test for core purity.

Pins docs/context/convention.md § Architecture boundaries, rule 2:

    "Core purity. The C++ core (`include/strata/`, `src/strata/{json,search,util}`)
     must never include `Python.h`. All CPython code lives in
     `src/strata/bindings/`."

The rule is a build-time invariant with no compiler to enforce it, so it is
checked here instead: these suites run in `make test`, in the post-build gate
of every install, and in CI.

The scan covers everything under `include/` and `src/strata/` except
`src/strata/bindings/`, rather than the three subdirectories the rule names, so
new core directories are covered the day they appear.
"""

import re
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[2]
CORE_ROOTS = (PROJECT_ROOT / "include", PROJECT_ROOT / "src" / "strata")
BINDINGS_DIR = PROJECT_ROOT / "src" / "strata" / "bindings"
SOURCE_SUFFIXES = frozenset({".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".inc"})

# Matches a real include directive, in either bracket or quote form, with any
# spacing the preprocessor allows.
CPYTHON_INCLUDE = re.compile(r"^[ \t]*#[ \t]*include[ \t]*[<\"]Python\.h[>\"]", re.MULTILINE)


def _sources(root: Path) -> list[Path]:
    if not root.exists():
        return []
    return sorted(p for p in root.rglob("*") if p.is_file() and p.suffix in SOURCE_SUFFIXES)


def _core_sources() -> list[Path]:
    found: list[Path] = []
    for root in CORE_ROOTS:
        found.extend(p for p in _sources(root) if BINDINGS_DIR not in p.parents)
    return found


def test_the_core_has_sources_to_check():
    """Guards against a vacuous pass: an empty scan would prove nothing."""
    assert _core_sources(), "no core sources found — the purity scan would be vacuous"


def test_the_scanner_finds_cpython_includes_where_they_are_allowed():
    """Positive control: the same scan must fire on the bindings, which do include it."""
    bindings = _sources(BINDINGS_DIR)
    assert bindings, "no binding sources found — the positive control would be vacuous"
    hits = [p for p in bindings if CPYTHON_INCLUDE.search(p.read_text(encoding="utf-8"))]
    assert hits, "the scanner found no CPython include in src/strata/bindings/ — it is broken"


def test_core_never_includes_the_cpython_header():
    offenders = [
        str(p.relative_to(PROJECT_ROOT))
        for p in _core_sources()
        if CPYTHON_INCLUDE.search(p.read_text(encoding="utf-8"))
    ]
    assert not offenders, f"CPython header included from the core: {offenders}"


def test_the_documented_grep_over_the_core_stays_empty():
    """The roadmap's mechanical check greps for the bare name, not the directive.

    Core files therefore avoid mentioning it even in prose, so the documented
    command reports nothing rather than a comment.
    """
    mentions = [
        str(p.relative_to(PROJECT_ROOT))
        for p in _core_sources()
        if "Python.h" in p.read_text(encoding="utf-8")
    ]
    assert not mentions, f"core files name the CPython header in text: {mentions}"

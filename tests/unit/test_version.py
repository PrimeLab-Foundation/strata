"""Contract tests for version single-sourcing.

Pins docs/context/api.md § Versioning:

    "Versioning is calver (`YYYY.M.D` of release ...). The rebuild started at
     `2026.8.9` and released as `2026.8.10` ... Single source of truth: the literal in
     `python/strata/__init__.py`; pyproject reads it dynamically — no second
     copy anywhere (the previous implementation drifted across three
     locations)."

The literal follows the release grammar `scripts/release.py bump` enforces,
`YYYY.M.D[.N][rcK]`, rather than one pinned value, so a release bump does not
edit this file.
"""

import datetime
import re
from pathlib import Path

import strata

PROJECT_ROOT = Path(__file__).resolve().parents[2]

# Files that historically grew their own copy of the version.
BUILD_FILES = ("pyproject.toml", "setup.py", "Makefile", "CMakeLists.txt")

# No leading zeros; .N (a same-day re-release) and rcK (a candidate) count from 1.
# [0-9], never \d: a str pattern's \d also matches non-ASCII digits.
RELEASE_GRAMMAR = re.compile(
    r"([1-9][0-9]{3})\.([1-9][0-9]?)\.([1-9][0-9]?)(?:\.[1-9][0-9]*)?(?:rc[1-9][0-9]*)?",
)


def test_version_follows_the_release_grammar():
    """`YYYY.M.D[.N][rcK]` — PEP 440 normal form, so it orders correctly."""
    match = RELEASE_GRAMMAR.fullmatch(strata.__version__)
    assert match, f"{strata.__version__!r} is not YYYY.M.D[.N][rcK]"
    year, month, day = (int(part) for part in match.groups())
    datetime.date(year, month, day)  # raises ValueError on a date the calendar lacks


def test_pyproject_reads_the_version_dynamically():
    # Deliberately structural rather than a comparison against
    # importlib.metadata: these tests also run inside the post-build gate,
    # where the distribution registered in site-packages is still the previous
    # install. setuptools already fails the build outright if this attr path
    # cannot be resolved, so the build itself proves the read works.
    text = (PROJECT_ROOT / "pyproject.toml").read_text(encoding="utf-8")
    assert 'dynamic = ["version"]' in text
    assert 'attr = "strata.__version__"' in text


def test_no_build_file_restates_the_version():
    """No second copy anywhere: the literal appears in the facade only."""
    for name in BUILD_FILES:
        text = (PROJECT_ROOT / name).read_text(encoding="utf-8")
        assert strata.__version__ not in text, f"{name} holds a second version literal"

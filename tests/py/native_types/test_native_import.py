"""`import strata` imports none of the native types' modules.

docs/architecture/native_types.md, "Hot-path protection" 6: the type table is
read from `sys.modules` when a walk first needs it and never imports a module,
so a fresh interpreter that imports strata -- and serializes with it, and has
it refuse an unsupported object, which is what resolves the table -- still has
none of `numpy`, `datetime`, `uuid`, `decimal` or `dataclasses` loaded.
"""

import pathlib
import subprocess
import sys
import textwrap

import strata

MODULES = ("numpy", "datetime", "uuid", "decimal", "dataclasses")

#: By argument, not PYTHONPATH: the build gate runs the suites against a fresh
#: build that no environment variable names.
PACKAGE_ROOT = str(pathlib.Path(strata.__file__).resolve().parent.parent)


def _fresh(code):
    prelude = "import sys\nsys.path.insert(0, sys.argv[1])\n"
    result = subprocess.run(
        [sys.executable, "-c", prelude + textwrap.dedent(code), PACKAGE_ROOT],
        capture_output=True,
        text=True,
        timeout=60,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    return result.stdout.split()


def test_importing_strata_imports_none_of_the_native_modules():
    loaded = _fresh(
        f"""
        import sys
        before = {{name for name in {MODULES!r} if name in sys.modules}}
        import strata
        after = {{name for name in {MODULES!r} if name in sys.modules}}
        print(" ".join(sorted(after - before)) or "none")
        """
    )
    assert loaded == ["none"]


def test_serializing_resolves_the_table_without_importing_them():
    loaded = _fresh(
        f"""
        import sys
        before = {{name for name in {MODULES!r} if name in sys.modules}}
        import strata
        strata.dumps({{"a": [1, 2.5, "x", None, {{"b": True}}]}})
        strata.dumps({{1, 2}})
        try:
            strata.dumps(object())
        except TypeError:
            pass
        try:
            strata.dumps_with_default([object()], str)
        except TypeError:
            pass
        after = {{name for name in {MODULES!r} if name in sys.modules}}
        print(" ".join(sorted(after - before)) or "none")
        """
    )
    assert loaded == ["none"]


def test_a_module_imported_after_the_table_first_resolved_is_found():
    out = _fresh(
        """
        import strata
        try:
            strata.dumps(object())
        except TypeError:
            pass
        import datetime, uuid
        print(strata.dumps([datetime.date(2026, 9, 28), uuid.UUID(int=1)]))
        """
    )
    assert out == ['["2026-09-28","00000000-0000-0000-0000-000000000001"]']

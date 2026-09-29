"""Contract tests for the `dumps`/`dump` `native=` keyword itself.

docs/decisions.md 2026-09-29 (M15b): `native` is a `bool` only, tested by
identity (`native is False` binds `_strata`'s call exactly as main's facade
made it, `native is True` routes to the hook, anything else raises
`TypeError` before any work); `native=False` -- and the omitted default --
keep main `38eaa9f`'s exact contract, including
`TypeError("Object of type %s is not JSON serializable")` for every native
family. `strata._dumps_hook` is imported only by a call that needs it.
"""

import dataclasses
import datetime as dt
import decimal
import enum
import subprocess
import sys
import textwrap
import uuid

import pytest

import strata

NOT_A_BOOL = [0, 1, None, "yes"]


@dataclasses.dataclass
class Point:
    x: int
    y: int


class Color(enum.Enum):
    RED = "red"


NATIVE_FAMILIES = [
    (dt.datetime(2026, 1, 1), "datetime.datetime"),
    (dt.date(2026, 1, 1), "datetime.date"),
    (dt.time(1, 2, 3), "datetime.time"),
    (uuid.UUID(int=1), "UUID"),
    (decimal.Decimal("1.5"), "decimal.Decimal"),
    (Color.RED, "Color"),
    (Point(1, 2), "Point"),
    ({1, 2}, "set"),
    (frozenset({1, 2}), "frozenset"),
]


@pytest.mark.parametrize("native", NOT_A_BOOL)
def test_dumps_native_must_be_a_bool(native):
    message = f"^native must be a bool, not {type(native).__name__}$"
    with pytest.raises(TypeError, match=message):
        strata.dumps(1, native=native)


@pytest.mark.parametrize("native", NOT_A_BOOL)
def test_dump_native_must_be_a_bool(native, tmp_path):
    message = f"^native must be a bool, not {type(native).__name__}$"
    with pytest.raises(TypeError, match=message):
        strata.dump(1, tmp_path / "out.json", native=native)


@pytest.mark.parametrize(("value", "name"), NATIVE_FAMILIES, ids=[n for _, n in NATIVE_FAMILIES])
def test_dumps_native_false_raises_the_unchanged_type_error(value, name):
    message = f"^Object of type {name} is not JSON serializable$"
    with pytest.raises(TypeError, match=message):
        strata.dumps(value)
    with pytest.raises(TypeError, match=message):
        strata.dumps(value, native=False)


@pytest.mark.parametrize(("value", "name"), NATIVE_FAMILIES, ids=[n for _, n in NATIVE_FAMILIES])
def test_dump_native_false_raises_the_unchanged_type_error(value, name, tmp_path):
    message = f"^Object of type {name} is not JSON serializable$"
    with pytest.raises(TypeError, match=message):
        strata.dump(value, tmp_path / "out.json")
    with pytest.raises(TypeError, match=message):
        strata.dump(value, tmp_path / "out.json", native=False)


def test_numpy_scalar_and_array_native_false_raise_the_unchanged_type_error():
    np = pytest.importorskip("numpy")
    with pytest.raises(TypeError, match="^Object of type numpy.int8 is not JSON serializable$"):
        strata.dumps(np.int8(1))
    with pytest.raises(TypeError, match="^Object of type numpy.ndarray is not JSON serializable$"):
        strata.dumps(np.array([1, 2, 3]))


# ---------------------------------------------------------------------------
# `strata._dumps_hook` is imported only by a call that needs it.
# ---------------------------------------------------------------------------


def _fresh(code):
    package_root = strata.__file__.rsplit("/python/", 1)[0] + "/python"
    result = subprocess.run(
        [sys.executable, "-c", f"import sys\nsys.path.insert(0, {package_root!r})\n" + code],
        capture_output=True,
        text=True,
        timeout=60,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    return result.stdout.split()


def test_dumps_with_no_native_object_never_imports_the_hook():
    out = _fresh(
        textwrap.dedent(
            """
            import sys
            import strata
            strata.dumps({"a": [1, 2.5, "x", None, True]})
            strata.dumps({"a": 1}, native=False)
            print("imported" if "strata._dumps_hook" in sys.modules else "not-imported")
            """
        )
    )
    assert out == ["not-imported"]


def test_dump_with_no_native_object_never_imports_the_hook(tmp_path):
    out = _fresh(
        textwrap.dedent(
            f"""
            import sys
            import strata
            strata.dump({{"a": 1}}, {str(tmp_path / "a.json")!r})
            strata.dump({{"a": 1}}, {str(tmp_path / "b.json")!r}, native=False)
            print("imported" if "strata._dumps_hook" in sys.modules else "not-imported")
            """
        )
    )
    assert out == ["not-imported"]


def test_dumps_native_false_on_an_unsupported_native_object_does_not_import_the_hook():
    # The TypeError comes from `_strata` itself; the hook is never reached.
    out = _fresh(
        textwrap.dedent(
            """
            import sys
            import datetime
            import strata
            try:
                strata.dumps(datetime.date(2026, 1, 1), native=False)
            except TypeError:
                pass
            print("imported" if "strata._dumps_hook" in sys.modules else "not-imported")
            """
        )
    )
    assert out == ["not-imported"]

"""Equivalence of the native tail's fast paths with the rules they implement.

docs/architecture/native_types.md, "Serializer contract" rows 8-9 and the
`parse_types` keyword: an exact set or frozenset is walked on its own table,
and must write what its iterator yields (a set subclass still takes the
iterator, so it is the oracle for the same table under the same mutation);
numpy's own `bool_`, sized integers, `float16` and `float32` are read through
truth, `__index__` and `__float__`, and must write what `item()` returns;
`loads` recognizes its keywords by interned identity first, and by text for
any other `str` spelling them.
"""

import ctypes
import enum
import operator
import pathlib
import random
import subprocess
import sys

import pytest

import strata
from strata import _strata

MODES = ("str", "bytes")

SEED = 20260929


def text(out):
    return out.decode() if isinstance(out, bytes) else out


class SetSubclass(set):
    """Same table as a set built by the same inserts; written through its iterator."""


def _random_set(rnd):
    values = set()
    for _ in range(rnd.randint(0, 80)):
        values.add(rnd.choice([rnd.randint(-(2**70), 2**70), rnd.randint(-50, 5000)]))
    for value in list(values):
        if rnd.random() < 0.4:
            values.discard(value)  # leaves deleted slots behind
    return values


@pytest.mark.parametrize("mode", MODES)
def test_exact_sets_write_their_iteration_order(mode):
    rnd = random.Random(SEED)
    for _ in range(400):
        values = _random_set(rnd)
        for candidate in (values, frozenset(values)):
            assert strata.dumps(candidate, return_type=mode, native=True) == strata.dumps(
                list(candidate),
                return_type=mode,
                native=True,
            )


@pytest.mark.parametrize("mode", MODES)
def test_sets_of_every_key_kind_match_the_iterator(mode):
    class Tag(enum.Enum):
        A = "a"
        B = 2

    keys = [
        None,
        True,
        False,
        0,
        -1,
        2**63,
        10**40,
        1.5,
        float("nan"),
        "é",
        "k",
        (1, "t"),
        Tag.A,
        Tag.B,
        frozenset({3, 4}),
    ]
    exact = set(keys)
    assert strata.dumps(exact, return_type=mode, native=True) == strata.dumps(
        list(exact), return_type=mode, native=True
    )
    assert strata.dumps(frozenset(keys), return_type=mode, native=True) == strata.dumps(
        list(frozenset(keys)),
        return_type=mode,
        native=True,
    )
    assert text(strata.dumps(set(), return_type=mode, native=True)) == "[]"
    assert text(strata.dumps(frozenset(), return_type=mode, native=True)) == "[]"


@pytest.mark.parametrize("mode", MODES)
def test_a_same_size_mutation_mid_walk_matches_the_iterator(mode):
    # An element whose value swaps one key for another keeps the size, so the
    # iterator does not raise: it goes on from its position in the changed
    # table. The table walk must write what it writes.
    def build(kind):
        target = kind()
        swap = {"done": False}

        class Swapper(enum.Enum):
            A = 1

            @property
            def value(self):
                if not swap["done"]:
                    swap["done"] = True
                    target.discard(0)
                    target.add(10_007)
                return "s"

        for key in [0, 1, 2, 3, Swapper.A, 5, 6, 7]:
            target.add(key)
        return target

    assert strata.dumps(build(set), return_type=mode, native=True) == strata.dumps(
        build(SetSubclass),
        return_type=mode,
        native=True,
    )


@pytest.mark.parametrize("mode", MODES)
def test_a_set_subclass_with_its_own_iter_keeps_it(mode):
    class Reversed(set):
        def __iter__(self):
            return iter(sorted(set.__iter__(self), reverse=True))

    assert text(strata.dumps(Reversed({1, 2, 3}), return_type=mode, native=True)) == "[3,2,1]"


# ---------------------------------------------------------------------------
# numpy scalars: the exact twins of item().
# ---------------------------------------------------------------------------


def _numpy():
    return pytest.importorskip("numpy")


def _integer_scalars(np):
    for name in (
        "int8",
        "uint8",
        "int16",
        "uint16",
        "int32",
        "uint32",
        "int64",
        "uint64",
        "longlong",
        "ulonglong",
        "intc",
        "uintc",
        "intp",
        "uintp",
    ):
        kind = getattr(np, name)
        info = np.iinfo(kind)
        for value in {info.min, info.max, 0, 1, info.max // 3}:
            yield kind(value)


@pytest.mark.parametrize("mode", MODES)
def test_numpy_integers_and_bools_write_what_item_returns(mode):
    np = _numpy()
    scalars = [*_integer_scalars(np), np.bool_(True), np.bool_(False)]
    for scalar in scalars:
        assert strata.dumps(scalar, return_type=mode, native=True) == strata.dumps(
            scalar.item(),
            return_type=mode,
            native=True,
        ), (type(scalar), scalar)
    assert strata.dumps(scalars, return_type=mode, native=True) == strata.dumps(
        [scalar.item() for scalar in scalars],
        return_type=mode,
        native=True,
    )


@pytest.mark.parametrize("mode", MODES)
def test_every_float16_writes_what_item_returns(mode):
    np = _numpy()
    halves = np.arange(65536, dtype=np.uint16).view(np.float16)
    assert strata.dumps(list(halves), return_type=mode, native=True) == strata.dumps(
        [half.item() for half in halves],
        return_type=mode,
        native=True,
    )


@pytest.mark.parametrize("mode", MODES)
def test_float32_writes_what_item_returns(mode):
    np = _numpy()
    rng = np.random.default_rng(SEED)
    bits = rng.integers(0, 2**32, size=20000, dtype=np.uint64).astype(np.uint32)
    singles = list(bits.view(np.float32))
    singles += [
        np.float32(value)
        for value in (0.1, -0.0, 1e-45, 3.4028235e38, float("inf"), float("-inf"), float("nan"))
    ]
    assert strata.dumps(singles, return_type=mode, native=True) == strata.dumps(
        [single.item() for single in singles],
        return_type=mode,
        native=True,
    )


def test_numpy_scalars_outside_the_twins_keep_item():
    np = _numpy()

    class Own(np.float32):
        def item(self):
            return "own"

    class OwnInt(np.int64):
        def item(self):
            return [1]

    assert strata.dumps([Own(1.5), OwnInt(3)], native=True) == '["own",[1]]'
    with pytest.raises(TypeError, match="numpy.longdouble"):
        strata.dumps(np.longdouble(1), native=True)


def test_dumps_with_default_never_calls_default_for_the_twins():
    np = _numpy()
    calls = []

    def default(obj):
        calls.append(obj)

    out = strata.dumps_with_default([np.int16(-3), np.float32(0.5), np.bool_(True)], default)
    assert out == "[-3,0.5,true]"
    assert calls == []


# ---------------------------------------------------------------------------
# numpy scalars: the twins' runtime proof (docs/architecture/native_types.md,
# "Frames, cycles and depth", numpy amendment 2026-09-29; python_native_types.cpp
# `numpy_twins_hold`). Each image proves the type numbers once, when numpy
# resolves; if the proof fails, every numpy scalar keeps `item()`.
# ---------------------------------------------------------------------------

#: The proof's rows: type character, `dtype.num`, `dtype.kind`, C item size.
TWIN_ROWS = (
    ("?", 0, "b", 1),
    ("b", 1, "i", ctypes.sizeof(ctypes.c_byte)),
    ("B", 2, "u", ctypes.sizeof(ctypes.c_ubyte)),
    ("h", 3, "i", ctypes.sizeof(ctypes.c_short)),
    ("H", 4, "u", ctypes.sizeof(ctypes.c_ushort)),
    ("i", 5, "i", ctypes.sizeof(ctypes.c_int)),
    ("I", 6, "u", ctypes.sizeof(ctypes.c_uint)),
    ("l", 7, "i", ctypes.sizeof(ctypes.c_long)),
    ("L", 8, "u", ctypes.sizeof(ctypes.c_ulong)),
    ("q", 9, "i", ctypes.sizeof(ctypes.c_longlong)),
    ("Q", 10, "u", ctypes.sizeof(ctypes.c_ulonglong)),
    ("f", 11, "f", 4),
    ("e", 23, "f", 2),
)

#: By argument, not PYTHONPATH, as in test_native_import.py.
PACKAGE_ROOT = str(pathlib.Path(strata.__file__).resolve().parent.parent)

#: Run in a fresh interpreter, where both images resolve numpy for the first
#: time inside the patched window: `numpy.dtype` is what the proof calls, so a
#: stand-in records which rows each image's proof read, and under REFUSE hands
#: back float64 for every code, which no row accepts. Then every twin kind, at
#: its edges, must write what `item()` returns, in both images and both modes.
PROOF_RUN = """
import hashlib

import numpy as np

import strata

real = np.dtype
seen = {"dumps": [], "hook": []}
phase = "dumps"


def stand_in(code):
    seen[phase].append(code)
    return real("d") if REFUSE else real(code)


np.dtype = stand_in
try:
    strata.dumps(np.int8(1), native=True)
    phase = "hook"
    strata.dumps_with_default(np.int8(1), repr)
finally:
    np.dtype = real

scalars = [np.bool_(True), np.bool_(False)]
for name in ("int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64",
             "longlong", "ulonglong", "intc", "uintc", "intp", "uintp"):
    kind = getattr(np, name)
    info = np.iinfo(kind)
    scalars += [kind(value) for value in sorted({info.min, info.max, 0, 1, info.max // 3})]
scalars += list(np.arange(0, 65536, 61, dtype=np.uint16).view(np.float16))
scalars += [np.float32(value) for value in (0.1, -0.0, 1e-45, 3.4028235e38, float("inf"),
                                            float("nan"))]
oracle = strata.dumps([scalar.item() for scalar in scalars], return_type="bytes", native=True)
assert strata.dumps(scalars, native=True).encode() == oracle
assert strata.dumps(scalars, return_type="bytes", native=True) == oracle
assert strata.dumps_with_default(scalars, repr).encode() == oracle
assert strata.dumps_with_default(scalars, repr, return_type="bytes") == oracle
print("".join(seen["dumps"]) or "-")
print("".join(seen["hook"]) or "-")
print(hashlib.sha256(oracle).hexdigest())
"""


def _proof_run(refuse):
    """`(rows read by _strata's proof, rows read by the hook's, output digest)`."""
    code = f"import sys\nsys.path.insert(0, sys.argv[1])\nREFUSE = {refuse!r}\n{PROOF_RUN}"
    result = subprocess.run(
        [sys.executable, "-c", code, PACKAGE_ROOT],
        capture_output=True,
        text=True,
        timeout=120,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    return result.stdout.split()


def _probe(kind, size):
    """The proof's probe: True, 0.1, or the integer type's extreme (its minimum when signed)."""
    if kind == "b":
        return True
    if kind == "f":
        return 0.1
    return -(2 ** (8 * size - 1)) if kind == "i" else 2 ** (8 * size) - 1


def test_the_numpy_twin_proof_holds_on_the_installed_numpy():
    np = _numpy()
    for code, number, kind, size in TWIN_ROWS:
        scalar_type = np.dtype(code).type
        scalar = scalar_type(_probe(kind, size))
        assert type(scalar) is scalar_type, code
        own = scalar.dtype
        assert (own.num, own.kind, own.itemsize, own.type) == (number, kind, size, scalar_type)
        if kind == "b":
            twin = bool(scalar)
        elif kind == "f":
            twin = float(scalar)
        else:
            twin = operator.index(scalar)
        item = scalar.item()
        assert type(twin) is type(item), code
        assert twin == item, code
    # `native=True` and `dumps_with_default` share one image (`strata._dumps_hook`,
    # docs/decisions.md 2026-09-29, M15b): the proof runs once, on the first call
    # (`dumps`, read every row); the second call (`dumps_with_default`) finds the
    # twins already resolved and probes nothing.
    rows = "".join(code for code, *_ in TWIN_ROWS)
    dumps_rows, hook_rows, _ = _proof_run(refuse=False)
    assert dumps_rows == rows
    assert hook_rows == "-"


def test_a_refused_numpy_twin_proof_writes_every_scalar_through_item():
    _numpy()
    refused_dumps, refused_hook, refused_digest = _proof_run(refuse=True)
    # Refused at its first row on the first call; the second finds the refusal
    # already cached and probes nothing.
    assert refused_dumps == "?"
    assert refused_hook == "-"
    *_, proven_digest = _proof_run(refuse=False)
    assert refused_digest == proven_digest


# ---------------------------------------------------------------------------
# loads keywords: identity first, text second.
# ---------------------------------------------------------------------------


def _spelled(name):
    # A `str` equal to the keyword but not the interned object.
    return "".join(list(name))


def test_loads_keywords_by_identity_and_by_text():
    # `_strata.loads` (the frozen extension) takes `return_type`/`iterator` only;
    # `parse_types` is a facade-only keyword (docs/decisions.md 2026-09-29, M15b).
    for name in ("return_type", "iterator"):
        assert _spelled(name) is not sys.intern(name)
    assert _strata.loads(b"[1]", **{_spelled("return_type"): "dict"}) == [1]
    assert list(_strata.loads(b"[1,2]", **{_spelled("iterator"): True})) == [1, 2]
    assert _strata.loads(b"[1]", return_type="dict", iterator=False) == [1]
    cursor = _strata.loads(b"[7]", **{_spelled("return_type"): "cursor"})
    assert cursor.at(0).get_int() == 7


def test_loads_unknown_keyword_message_is_unchanged():
    with pytest.raises(TypeError, match=r"^loads\(\) got an unexpected keyword argument 'bogus'$"):
        _strata.loads(b"1", **{_spelled("bogus"): 1})
    with pytest.raises(
        TypeError, match=r"^loads\(\) got an unexpected keyword argument 'Iterator'$"
    ):
        _strata.loads(b"1", Iterator=True)
    with pytest.raises(TypeError, match="return_type must be str, not int"):
        _strata.loads(b"1", return_type=1)

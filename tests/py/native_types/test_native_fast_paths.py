"""Equivalence of the native tail's fast paths with the rules they implement.

docs/architecture/native_types.md, "Serializer contract" rows 8-9 and the
`parse_types` keyword: an exact set or frozenset is walked on its own table,
and must write what its iterator yields (a set subclass still takes the
iterator, so it is the oracle for the same table under the same mutation);
numpy's own `bool_`, sized integers, `float16` and `float32` are read through
truth, `__index__` and `__float__`, and must write what `item()` returns;
`loads` recognizes its keywords by interned identity first, and by text for
any other `str` spelling them. Rows 4, 6 and 7 and the type table's resolution
have fast paths of their own -- `UUID.int` split from its digits, `Enum.value`
read as `_value_` while the stock descriptor is provably in place, dataclass
field names cached with their escaped keys, and a lazy type-table pass -- and
each must write what the oracle ("The oracle") writes.
"""

import abc
import contextlib
import ctypes
import dataclasses
import enum
import json
import operator
import pathlib
import random
import subprocess
import sys
import uuid

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


# ---------------------------------------------------------------------------
# Rows 4, 6 and 7 against the oracle (docs/architecture/native_types.md, "The
# oracle"): `strata.dumps(x, native=True)` writes
# `json.dumps(x, default=ref, separators=(",", ":"), ensure_ascii=False)`.
# ---------------------------------------------------------------------------


def ref(obj):
    """Each row's reference spelling: `str(u)`, `.value`, the fields by `getattr`."""
    if isinstance(obj, enum.Enum):
        return obj.value
    if isinstance(obj, uuid.UUID):
        return str(obj)
    if dataclasses.is_dataclass(obj) and not isinstance(obj, type):
        return {field.name: getattr(obj, field.name) for field in dataclasses.fields(obj)}
    raise TypeError(f"Object of type {type(obj).__name__} is not JSON serializable")


def oracle(obj):
    return json.dumps(obj, default=ref, separators=(",", ":"), ensure_ascii=False)


def assert_oracle(obj):
    """Both return types write the oracle's text, which is returned."""
    expected = oracle(obj)
    assert strata.dumps(obj, native=True) == expected
    assert strata.dumps(obj, return_type="bytes", native=True) == expected.encode()
    return expected


@contextlib.contextmanager
def swapped(owner, name, value):
    """`owner.name = value` for the block; restored after (removed if a class inherited it)."""
    inherited = isinstance(owner, type) and name not in vars(owner)
    saved = getattr(owner, name)
    setattr(owner, name, value)
    try:
        yield saved
    finally:
        if inherited:
            delattr(owner, name)
        else:
            setattr(owner, name, saved)


# ---------------------------------------------------------------------------
# Row 6: `Enum.value` read as `_value_` while the stock descriptor is provably
# in place (python_native_types.cpp `capture_enum_value`, `stock_enum_value`,
# `enum_value`); anything else is `getattr(member, "value")`.
# ---------------------------------------------------------------------------


class Plain(enum.Enum):
    A = 1
    B = "b"
    C = (1, "c")


class Perm(enum.Flag):
    R = 4
    W = 2
    X = 1


class OwnValue(enum.Enum):
    @property
    def value(self):
        return ["own value", self._value_]


class InheritsValue(OwnValue):
    A = 1


#: `Enum.value` as the enum module defines it: the descriptor the proof captured.
STOCK_VALUE = enum.Enum.__dict__["value"]


def test_enum_members_match_the_oracle():
    # Row 6: `member.value`, read with `getattr` -- a Flag's single, composite
    # and empty members, and a `value` property a base enum overrides.
    members = [*Plain, Perm.R, Perm.R | Perm.W | Perm.X, Perm(0), InheritsValue.A]
    for member in members:
        assert_oracle(member)
    assert assert_oracle(members) == '[1,"b",[1,"c"],4,7,0,["own value",1]]'


def test_enum_a_class_with_getattribute_is_read_through_it():
    # Row 6 ("read with getattr"): a member whose class defines
    # `__getattribute__` is read attribute for attribute as `getattr` reads it.
    reads = []

    class Watched(enum.Enum):
        A = 1

        def __getattribute__(self, name):
            reads.append(name)
            return super().__getattribute__(name)

    class Intercepting(enum.Enum):
        A = 1

        def __getattribute__(self, name):
            return "intercepted" if name == "value" else super().__getattribute__(name)

    reads.clear()
    assert Watched.A.value == 1
    through_getattr = reads[:]
    for mode in MODES:
        reads.clear()
        assert text(strata.dumps(Watched.A, return_type=mode, native=True)) == "1"
        assert reads == through_getattr
    assert through_getattr[0] == "value"
    assert assert_oracle(Intercepting.A) == '"intercepted"'


def _patch_code():
    return swapped(STOCK_VALUE.fget, "__code__", (lambda self: ["code", self._value_]).__code__)


def _patch_fget():
    return swapped(STOCK_VALUE, "fget", lambda self: ["fget", self._value_])


def _patch_get():
    stock_get = type(STOCK_VALUE).__get__

    def get(self, instance, ownerclass=None):
        if instance is None:
            return stock_get(self, instance, ownerclass)
        return ["get", instance._value_]

    return swapped(type(STOCK_VALUE), "__get__", get)


@pytest.mark.parametrize(
    ("patch", "tag"),
    [(_patch_code, "code"), (_patch_fget, "fget"), (_patch_get, "get")],
)
def test_enum_value_follows_a_patched_stock_descriptor(patch, tag):
    # Row 6 ("read with getattr"): the `_value_` read stands only while the
    # stock descriptor's fget code, its fget and its class's `__get__` are the
    # ones proven; each patch, made after the proof ran, is followed, and once
    # undone the members read as before.
    members = [Plain.A, Plain.C, Perm.R | Perm.W, Perm(0)]
    before = assert_oracle(members)
    with patch():
        assert assert_oracle(members) == (
            f'[["{tag}",1],["{tag}",[1,"c"]],["{tag}",6],["{tag}",0]]'
        )
    assert assert_oracle(members) == before == '[1,[1,"c"],6,0]'


@pytest.mark.parametrize("kind", [Plain, Perm])
def test_enum_a_member_without_value_raises_what_getattr_raises(kind):
    # Row 6 and "Error contract": the `_value_` read raises the AttributeError
    # `getattr(member, "value")` raises, message for message.
    bare = object.__new__(kind)
    with pytest.raises(AttributeError) as through_getattr:
        _ = bare.value
    for mode in MODES:
        with pytest.raises(AttributeError) as raised:
            strata.dumps(bare, return_type=mode, native=True)
        assert str(raised.value) == str(through_getattr.value)
        assert raised.value.args == through_getattr.value.args


# ---------------------------------------------------------------------------
# Row 4: `UUID.int` split into halves from its digits (`split_uuid_int`), from
# the exact type's slot or, for a subclass, from `getattr`.
# ---------------------------------------------------------------------------


class UuidKey(uuid.UUID):
    """Not the exact type: its `.int` is read through `getattr`, never the slot."""


UUID_EDGES = (
    0,
    1,
    2**63 - 1,
    2**63,
    2**64 - 1,
    2**64,
    2**64 + 1,
    2**127 - 1,
    2**127,
    2**128 - 2,
    2**128 - 1,
)


@pytest.mark.parametrize("kind", [uuid.UUID, UuidKey])
def test_uuid_ints_at_every_half_boundary_match_the_oracle(kind):
    # Row 4: `str(u)`, from `.int`; each 64-bit boundary and a random corpus
    # of every width must land in the right half.
    rnd = random.Random(SEED)
    numbers = list(UUID_EDGES)
    for bits in (1, 32, 63, 64, 65, 96, 127, 128):
        numbers += [rnd.getrandbits(bits) for _ in range(200)]
    for number in UUID_EDGES:
        assert_oracle(kind(int=number))
    assert_oracle([kind(int=number) for number in numbers])


@pytest.mark.parametrize("stored", [2**128, 2**128 + 1, 2**200, -1, -(2**64)])
@pytest.mark.parametrize("kind", [uuid.UUID, UuidKey])
def test_uuid_an_int_outside_128_bits_raises_value_error(kind, stored):
    # Row 4 (docs/decisions.md 2026-09-28): only `object.__setattr__` can store
    # one; it is refused, never truncated, at the top level or nested.
    broken = kind(int=1)
    object.__setattr__(broken, "int", stored)
    message = r"^UUID\.int is out of range \(need a 128-bit value\)$"
    for document in (broken, [uuid.UUID(int=2), {"k": broken}]):
        for mode in MODES:
            with pytest.raises(ValueError, match=message):
                strata.dumps(document, return_type=mode, native=True)


# ---------------------------------------------------------------------------
# Row 7: field names cached per type with their escaped keys
# (`dataclass_field_names`, `encode_keys`, `plain_class_attribute`;
# python_dumps.cpp `write_dataclass`).
# ---------------------------------------------------------------------------

#: Pieces of field names: every class of JSON escape, and text that needs none.
NAME_PIECES = (
    '"',
    "\\",
    "\x00",
    "\x01",
    "\n",
    "\t",
    "\x1f",
    "\x7f",
    "/",
    " ",
    "é",
    "ключ",
    "名",
    "\U0001f600",
    "a",
    "_",
)


def _renamed(rnd, index):
    """A dataclass instance whose `Field.name`s were set, before first use, to random text."""
    count = rnd.randint(1, 6)
    kind = dataclasses.make_dataclass(f"Renamed{index}", [f"f{slot}" for slot in range(count)])
    names = []
    while len(names) < count:
        name = "".join(rnd.choice(NAME_PIECES) for _ in range(rnd.randint(1, 4)))
        if name not in names:
            names.append(name)
    for field, name in zip(dataclasses.fields(kind), names, strict=True):
        field.name = name
    instance = kind.__new__(kind)
    for slot, name in enumerate(names):
        setattr(instance, name, rnd.choice([slot, str(slot), None, [slot, name]]))
    return instance


def test_dataclass_names_that_need_escaping_match_the_oracle_filled_and_cached():
    # Row 7: the fields `dataclasses.fields(obj)` lists, each name escaped as a
    # dict key; the first write fills the type's entry, escaped keys included,
    # and every later write is served from it.
    rnd = random.Random(SEED)
    instances = [_renamed(rnd, index) for index in range(300)]
    filled = [assert_oracle(instance) for instance in instances]
    assert [assert_oracle(instance) for instance in instances] == filled
    assert assert_oracle(instances) == "[" + ",".join(filled) + "]"
    for instance, expected in zip(instances, filled, strict=True):
        assert strata.dumps_with_default(instance, repr) == expected


def test_dataclass_under_abcmeta_matches_the_oracle_filled_and_cached():
    # Row 7: a metaclass other than `type` takes the generic
    # `__dataclass_fields__` read (`plain_class_attribute` answers -1) and
    # writes what the plain read writes. Nothing is abstract, so it instantiates.
    @dataclasses.dataclass
    class Shape(abc.ABC):  # noqa: B024
        side: int = 2
        label: str = "sq"

    @dataclasses.dataclass
    class Square(Shape):
        area: float = 4.0

    for value in (Shape(), Square(3, 'a"b', 9.5), [Square(), Shape(1)]):
        first = assert_oracle(value)
        assert assert_oracle(value) == first
    assert oracle(Square()) == '{"side":2,"label":"sq","area":4.0}'


def test_dataclass_a_surrogate_field_name_raises_at_that_field():
    # api.md `dumps` (a lone surrogate is UnicodeEncodeError on every call) and
    # row 7: the fields before the name are read and written, none after it is.
    reads = []

    @dataclasses.dataclass
    class Tail:
        first: int = 1
        bad: int = 2
        last: int = 3

    dataclasses.fields(Tail)[1].name = "\ud800"
    Tail.first = property(lambda self: reads.append("first") or 1)
    Tail.last = property(lambda self: reads.append("last") or 3)
    instance = Tail.__new__(Tail)
    setattr(instance, "\ud800", 2)
    with pytest.raises(UnicodeEncodeError) as as_key:
        strata.dumps({"first": 1, "\ud800": 2}, native=True)
    for _ in range(2):  # filling the entry, then served from it
        for mode in MODES:
            with pytest.raises(UnicodeEncodeError) as raised:
                strata.dumps(instance, return_type=mode, native=True)
            assert str(raised.value) == str(as_key.value)
    assert reads == ["first"] * 4


@pytest.mark.parametrize("metaclass", [type, abc.ABCMeta])
def test_dataclass_fields_replaced_after_first_use_are_re_read(metaclass):
    # Row 7 (docs/decisions.md 2026-09-29, review P2): an entry stands only
    # while `__dataclass_fields__` holds the keys and Field objects it was read
    # from; a replaced dict, or a Field swapped in, is listed and escaped again.
    base = metaclass("Base", (), {})

    @dataclasses.dataclass
    class Record(base):
        a: int = 1
        b: int = 2

    @dataclasses.dataclass
    class Donor:
        c: int = 3
        d: int = 4

    quoted, broken = dataclasses.fields(Donor)
    quoted.name = 'c"q'
    broken.name = "d\\\n"
    value = Record()
    setattr(value, 'c"q', 3)
    setattr(value, "d\\\n", 4)
    for _ in range(2):
        assert assert_oracle(value) == '{"a":1,"b":2}'
    Record.__dataclass_fields__ = {"a": Record.__dataclass_fields__["a"], "c": quoted}
    for _ in range(2):
        assert assert_oracle(value) == '{"a":1,"c\\"q":3}'
    Record.__dataclass_fields__["c"] = broken
    for _ in range(2):
        assert assert_oracle(value) == '{"a":1,"d\\\\\\n":4}'


# ---------------------------------------------------------------------------
# The type table (native_types.md, "Re-entrancy": resolution is a `sys.modules`
# probe and attribute reads on a module the user may have replaced;
# python_native_types.cpp `evaluate`, lazy and then eager after `resolve`).
# ---------------------------------------------------------------------------

#: Run in a fresh interpreter, where numpy, datetime and uuid are not in
#: `sys.modules`, so their groups are unresolved. Enum members, Decimals and
#: dataclasses are written with those modules absent, then with a fake module
#: under each absent name (one bare, one that records and refuses every
#: attribute read), then with `sys.modules` restored; strata imports none of them.
RESOLUTION_RUN = """
import dataclasses
import enum
import types
from decimal import Decimal

import strata

GROUPS = ("datetime", "uuid", "decimal", "enum", "dataclasses", "numpy")


class Tag(enum.Enum):
    A = "a"
    B = 2


class Outer(enum.Enum):
    INNER = Tag.A


@dataclasses.dataclass
class Row:
    tag: object
    price: object
    rest: list


DOCUMENTS = (
    (Tag.B, "2"),
    (Outer.INNER, '"a"'),
    (Decimal("1.50"), "1.50"),
    (Decimal("-NaN"), "null"),
    (
        Row(Tag.A, Decimal("1E+2"), [Outer.INNER, Row(Tag.B, Decimal("-0"), [])]),
        '{"tag":"a","price":1E+2,"rest":["a",{"tag":2,"price":-0,"rest":[]}]}',
    ),
)


def check():
    for document, expected in DOCUMENTS:
        assert strata.dumps(document, native=True) == expected, (document, expected)
        assert strata.dumps(document, return_type="bytes", native=True) == expected.encode()
        assert strata.dumps_with_default(document, repr) == expected
    try:
        strata.dumps(object(), native=True)
    except TypeError as error:
        assert str(error) == "Object of type object is not JSON serializable", error
    else:
        raise AssertionError("an unsupported object was written")


reads = []


def bare(name):
    return types.ModuleType(name)


def refusing(name):
    module = types.ModuleType(name)

    def getattr_hook(attribute):
        reads.append(f"{name}.{attribute}")
        raise AttributeError(attribute)

    module.__getattr__ = getattr_hook
    return module


absent = [name for name in GROUPS if name not in sys.modules]
check()
assert all(name not in sys.modules for name in absent), sys.modules.keys()
for fake in (bare, refusing):
    try:
        for name in absent:
            sys.modules[name] = fake(name)
        check()
    finally:
        for name in absent:
            sys.modules.pop(name, None)
    check()
assert all(name not in sys.modules for name in absent), sys.modules.keys()
print(" ".join(absent))
print(" ".join(sorted(set(reads))))
"""


def test_resolution_absent_and_fake_groups_leave_every_other_row_unchanged():
    # "Re-entrancy" (type-table resolution) and docs/decisions.md (the lazy
    # pass decides what the eager order decides): Enum, Decimal and dataclass
    # are written alike with numpy absent, faked under its name, and restored.
    code = f"import sys\nsys.path.insert(0, sys.argv[1])\n{RESOLUTION_RUN}"
    result = subprocess.run(
        [sys.executable, "-c", code, PACKAGE_ROOT],
        capture_output=True,
        text=True,
        timeout=120,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    absent, reads = result.stdout.split("\n")[:2]
    assert "numpy" in absent.split()
    # The refusing fakes were probed: resolution read them and gave up.
    assert "numpy.generic" in reads.split()


# --- M15c: type verdicts, UUID digits (docs/architecture/native_types.md, "Hook profile and
# native emitter costs"; docs/decisions.md 2026-09-29) --------------------------------------


def test_verdicts_follow_a_class_modified_after_use():
    # Serializer contract rows 6 and 7: a class's verdict and its Enum `value` read are kept
    # only while the class is unmodified; setting or deleting an attribute re-decides.
    class Colour(enum.Enum):
        RED = "r"

    assert strata.dumps([Colour.RED, Colour.RED], native=True) == '["r","r"]'
    Colour.value = property(lambda self: "overridden")
    try:
        assert strata.dumps(Colour.RED, native=True) == '"overridden"'
    finally:
        del Colour.value
    assert strata.dumps(Colour.RED, native=True) == '"r"'

    @dataclasses.dataclass
    class Point:
        x: int = 1

    assert strata.dumps(Point(), native=True) == '{"x":1}'
    del Point.__dataclass_fields__
    with pytest.raises(TypeError, match="not JSON serializable"):
        strata.dumps(Point(), native=True)


def test_verdicts_follow_a_modified_base():
    # Row 7 via the MRO: a base gaining `__dataclass_fields__` after its subclass was
    # classified unsupported makes the subclass a dataclass on the next call.
    class Base:
        pass

    class Sub(Base):
        pass

    with pytest.raises(TypeError, match="not JSON serializable"):
        strata.dumps(Sub(), native=True)
    Base.__dataclass_fields__ = {}
    assert strata.dumps(Sub(), native=True) == "{}"


def test_verdicts_follow_reassigned_bases():
    # Row 6/8 precedence through `__bases__`: the subclass is re-decided after the assignment.
    class Plain:
        pass

    class Other:
        pass

    class Movable(Plain):
        pass

    with pytest.raises(TypeError, match="not JSON serializable"):
        strata.dumps(Movable(), native=True)
    Other.__dataclass_fields__ = {}
    Movable.__bases__ = (Other,)
    assert strata.dumps(Movable(), native=True) == "{}"


def test_many_classes_share_the_verdict_slots():
    # More Enum classes than verdict slots, interleaved: every member is its own value.
    classes = [enum.Enum(f"E{n}", {"A": n, "B": -n}) for n in range(200)]
    members = [member for cls in classes for member in cls] * 2
    assert strata.dumps(members, native=True) == json.dumps(
        [member.value for member in members], separators=(",", ":")
    )


def test_uuid_digit_reader_matches_str_across_widths():
    # Row 4: every width up to 128 bits, and the two ends, through the digit reader.
    rng = random.Random(15)
    values = [0, 1, 2**64 - 1, 2**64, 2**127, 2**128 - 1]
    values += [rng.getrandbits(bits) for bits in range(1, 129) for _ in range(20)]
    items = [uuid.UUID(int=value) for value in values]
    assert strata.dumps(items, native=True) == json.dumps(
        [str(u) for u in items], separators=(",", ":")
    )

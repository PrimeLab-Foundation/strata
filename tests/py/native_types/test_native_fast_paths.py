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

import enum
import random
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
            assert strata.dumps(candidate, return_type=mode) == strata.dumps(
                list(candidate),
                return_type=mode,
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
    assert strata.dumps(exact, return_type=mode) == strata.dumps(list(exact), return_type=mode)
    assert strata.dumps(frozenset(keys), return_type=mode) == strata.dumps(
        list(frozenset(keys)),
        return_type=mode,
    )
    assert text(strata.dumps(set(), return_type=mode)) == "[]"
    assert text(strata.dumps(frozenset(), return_type=mode)) == "[]"


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

    assert strata.dumps(build(set), return_type=mode) == strata.dumps(
        build(SetSubclass),
        return_type=mode,
    )


@pytest.mark.parametrize("mode", MODES)
def test_a_set_subclass_with_its_own_iter_keeps_it(mode):
    class Reversed(set):
        def __iter__(self):
            return iter(sorted(set.__iter__(self), reverse=True))

    assert text(strata.dumps(Reversed({1, 2, 3}), return_type=mode)) == "[3,2,1]"


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
        assert strata.dumps(scalar, return_type=mode) == strata.dumps(
            scalar.item(),
            return_type=mode,
        ), (type(scalar), scalar)
    assert strata.dumps(scalars, return_type=mode) == strata.dumps(
        [scalar.item() for scalar in scalars],
        return_type=mode,
    )


@pytest.mark.parametrize("mode", MODES)
def test_every_float16_writes_what_item_returns(mode):
    np = _numpy()
    halves = np.arange(65536, dtype=np.uint16).view(np.float16)
    assert strata.dumps(list(halves), return_type=mode) == strata.dumps(
        [half.item() for half in halves],
        return_type=mode,
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
    assert strata.dumps(singles, return_type=mode) == strata.dumps(
        [single.item() for single in singles],
        return_type=mode,
    )


def test_numpy_scalars_outside_the_twins_keep_item():
    np = _numpy()

    class Own(np.float32):
        def item(self):
            return "own"

    class OwnInt(np.int64):
        def item(self):
            return [1]

    assert strata.dumps([Own(1.5), OwnInt(3)]) == '["own",[1]]'
    with pytest.raises(TypeError, match="numpy.longdouble"):
        strata.dumps(np.longdouble(1))


def test_dumps_with_default_never_calls_default_for_the_twins():
    np = _numpy()
    calls = []

    def default(obj):
        calls.append(obj)

    out = strata.dumps_with_default([np.int16(-3), np.float32(0.5), np.bool_(True)], default)
    assert out == "[-3,0.5,true]"
    assert calls == []


# ---------------------------------------------------------------------------
# loads keywords: identity first, text second.
# ---------------------------------------------------------------------------


def _spelled(name):
    # A `str` equal to the keyword but not the interned object.
    return "".join(list(name))


def test_loads_keywords_by_identity_and_by_text():
    for name in ("return_type", "iterator", "parse_types"):
        assert _spelled(name) is not sys.intern(name)
    assert _strata.loads(b"[1]", **{_spelled("return_type"): "dict"}) == [1]
    assert list(_strata.loads(b"[1,2]", **{_spelled("iterator"): True})) == [1, 2]
    assert _strata.loads(b'["2026-09-29"]', **{_spelled("parse_types"): False}) == ["2026-09-29"]
    assert _strata.loads(b"[1]", return_type="dict", iterator=False, parse_types=False) == [1]
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

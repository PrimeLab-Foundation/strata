"""Native types against third-party code: numpy (row 9) and an orjson differential.

docs/architecture/native_types.md, "Serializer contract": row 9 writes a numpy
scalar or array of dtype kind `b i u f` as its `item()`/`tolist()` -- any
shape and strides, 0-d included, `float32` widened exactly -- and every other
numpy kind is the unsupported-type `TypeError` with numpy's type name. Rows
1-4 and 6 match orjson byte for byte where the record says they are
identical: naive and fixed-offset datetimes, dates, naive times, UUIDs and
plain Enums. numpy and orjson are not gate dependencies, so these live here.
"""

import datetime as dt
import enum
import random
import uuid

import numpy as np
import orjson
import pytest

import strata

MODES = ("str", "bytes")


def both(obj):
    text = strata.dumps(obj)
    assert strata.dumps(obj, return_type="bytes") == text.encode()
    return text


# ---------------------------------------------------------------------------
# Row 9: numpy
# ---------------------------------------------------------------------------

SCALAR_TYPES = [
    np.bool_,
    np.int8,
    np.int16,
    np.int32,
    np.int64,
    np.uint8,
    np.uint16,
    np.uint32,
    np.uint64,
    np.float16,
    np.float32,
    np.float64,
]


@pytest.mark.parametrize("kind", SCALAR_TYPES, ids=[t.__name__ for t in SCALAR_TYPES])
def test_row9_a_scalar_of_kind_b_i_u_f_is_written_as_its_item(kind):
    for raw in (0, 1, 7):
        value = kind(raw)
        assert both(value) == strata.dumps(value.item())
    if np.issubdtype(kind, np.integer):
        info = np.iinfo(kind)
        assert both(kind(info.max)) == str(int(info.max))
        assert both(kind(info.min)) == str(int(info.min))


def test_row9_float32_and_float16_widen_exactly_to_float():
    assert both(np.float32(0.1)) == "0.10000000149011612"
    assert both(np.float16(0.1)) == strata.dumps(float(np.float16(0.1)))


def test_row9_non_finite_floats_are_null():
    assert (
        both(np.array([1.0, np.nan, np.inf, -np.inf], dtype=np.float32)) == "[1.0,null,null,null]"
    )


@pytest.mark.parametrize(
    "array",
    [
        np.arange(12, dtype=np.int32).reshape(3, 4),
        np.arange(24, dtype=np.float64).reshape(2, 3, 4) / 8,
        np.arange(12, dtype=np.uint16).reshape(3, 4).T,
        np.arange(20, dtype=np.int64)[::3],
        np.array(2.5),
        np.array(True),
        np.empty((0, 3)),
        np.array([[True, False]]),
    ],
    ids=["2d", "3d", "transposed", "strided", "0d-float", "0d-bool", "empty", "bool"],
)
def test_row9_an_array_is_written_as_its_tolist_in_any_shape_and_strides(array):
    assert both(array) == strata.dumps(array.tolist())


def test_row9_an_ndarray_subclass_is_written_as_its_tolist():
    masked = np.ma.array([1, 2, 3], mask=[0, 1, 0])
    assert both(masked) == "[1,null,3]"


@pytest.mark.parametrize(
    ("value", "name"),
    [
        (np.complex128(1), "numpy.complex128"),
        (np.datetime64("2026-09-28"), "numpy.datetime64"),
        (np.timedelta64(5, "s"), "numpy.timedelta64"),
        (np.array(["a"]), "numpy.ndarray"),
        (np.array([1 + 2j]), "numpy.ndarray"),
        (np.array(["2026-09-28"], dtype="datetime64[D]"), "numpy.ndarray"),
        (np.array([object()], dtype=object), "numpy.ndarray"),
        (np.zeros(2, dtype=[("a", np.int32)]), "numpy.ndarray"),
    ],
    ids=[
        "complex",
        "datetime64",
        "timedelta64",
        "str-array",
        "complex-array",
        "M-array",
        "object-array",
        "structured",
    ],
)
def test_error_contract_other_numpy_kinds_raise_with_numpys_type_name(value, name):
    with pytest.raises(TypeError, match=f"^Object of type {name} is not JSON serializable$"):
        strata.dumps(value)


def test_row9_a_longdouble_whose_item_is_itself_is_unsupported():
    # docs/decisions.md 2026-09-28: item() of an extended longdouble returns a
    # longdouble, so it is not written natively rather than looping.
    value = np.longdouble(1.5)
    if isinstance(value.item(), float):
        assert both(value) == "1.5"
    else:
        with pytest.raises(TypeError, match="^Object of type numpy.longdouble is not"):
            strata.dumps(value)
        with pytest.raises(TypeError, match="^Object of type numpy.longdouble is not"):
            strata.dumps(np.array([1.5], dtype=np.longdouble))


@pytest.mark.parametrize("mode", MODES)
def test_row9_default_is_never_called_for_native_numpy(mode):
    seen = []

    def default(obj):
        seen.append(type(obj))
        return obj.tolist()

    document = {
        "ints": np.arange(5),
        "flag": np.bool_(False),
        "ratio": np.float32(0.5),
        "grid": np.ones((2, 2), dtype=np.uint8),
        "names": np.array(["a", "b"]),
    }
    out = strata.dumps_with_default(document, default, return_type=mode)
    text = out.decode() if isinstance(out, bytes) else out
    assert (
        text
        == '{"ints":[0,1,2,3,4],"flag":false,"ratio":0.5,"grid":[[1,1],[1,1]],"names":["a","b"]}'
    )
    assert seen == [np.ndarray]


# ---------------------------------------------------------------------------
# Rows 1-4 and 6 against orjson
# ---------------------------------------------------------------------------


class Color(enum.Enum):
    RED = "red"
    ONE = 1
    NOTHING = None


def _row_values(rng):
    moment = dt.datetime(
        rng.randrange(1, 10000),
        rng.randrange(1, 13),
        rng.randrange(1, 29),
        rng.randrange(24),
        rng.randrange(60),
        rng.randrange(60),
        rng.choice([0, rng.randrange(1_000_000)]),
    )
    zone = rng.choice(
        [dt.timezone.utc, dt.timezone(dt.timedelta(minutes=rng.randrange(-1439, 1440)))]
    )
    return [
        moment,
        moment.replace(tzinfo=zone),
        moment.date(),
        moment.time(),
        uuid.UUID(int=rng.getrandbits(128)),
        rng.choice(list(Color)),
    ]


#: orjson 3.11.9 drops a zero from some `time` fractions (10149 microseconds is
#: written `.10149`, not `.010149`); on such a build row 3 is checked against
#: `isoformat()`, which the record names as the reference spelling, instead.
ORJSON_TIME_IS_EXACT = orjson.dumps(dt.time(0, 49, 52, 10149)) == b'"00:49:52.010149"'

ROWS = ["datetime", "aware-datetime", "date", "time", "uuid", "enum"]


@pytest.mark.parametrize("row", ROWS)
def test_rows_1_to_4_and_6_match_orjson_byte_for_byte(row):
    rng = random.Random(20260928)
    index = ROWS.index(row)
    for _ in range(300):
        value = _row_values(rng)[index]
        ours = strata.dumps(value, return_type="bytes")
        if row == "time" and not ORJSON_TIME_IS_EXACT:
            assert ours == f'"{value.isoformat()}"'.encode(), value
        else:
            assert ours == orjson.dumps(value), value


def test_a_document_of_rows_1_to_4_and_6_matches_orjson():
    rng = random.Random(7)
    keep = [index for index, row in enumerate(ROWS) if row != "time" or ORJSON_TIME_IS_EXACT]
    document = []
    for index in range(200):
        values = _row_values(rng)
        document.append(
            {"row": index, "values": [values[k] for k in keep], "nested": {"at": values[1]}}
        )
    assert strata.dumps(document, return_type="bytes") == orjson.dumps(document)

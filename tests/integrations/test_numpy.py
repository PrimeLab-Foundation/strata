"""`dumps_with_default` composes with numpy arrays and scalars.

api.md, dumps_with_default (docs/architecture/dumps_with_default.md): the callable runs
only where `dumps` would raise. `numpy.float64` and `numpy.str_` subclass
`float` and `str`, so neither library calls it for them; `ndarray` and the
other scalar types reach it and come back as native values via `tolist()` and
`item()`.
"""

import numpy as np

import strata


def to_native(obj):
    if isinstance(obj, np.ndarray):
        return obj.tolist()
    if isinstance(obj, np.generic):
        return obj.item()
    raise TypeError(f"{type(obj).__name__} is not handled")


def test_arrays_of_every_common_dtype(composes):
    rng = np.random.default_rng(42)
    arrays = [
        np.arange(1000, dtype=np.int64),
        np.arange(-50, 50, dtype=np.int8),
        np.array([2**63 - 1, -(2**63)], dtype=np.int64),
        np.array([2**64 - 1], dtype=np.uint64),
        rng.random(1000),
        rng.random(100).astype(np.float32),
        rng.random(12) > 0.5,
        np.array(["α", "beta", ""], dtype=np.str_),
    ]
    decoded = composes(arrays, to_native)
    assert decoded[3] == [2**64 - 1]
    assert decoded[4] == arrays[4].tolist()


def test_shapes_nest_as_lists(composes):
    grid = np.arange(24, dtype=np.float64).reshape(2, 3, 4) / 8
    assert composes(grid, to_native) == grid.tolist()
    assert composes(np.array(2.5), to_native) == 2.5
    assert composes(np.empty((0, 3)), to_native) == []


def test_scalars_in_a_record(composes):
    record = {
        "count": np.int64(7),
        "small": np.uint8(255),
        "ratio": np.float32(0.1),
        "mean": np.float64(1.5),
        "flag": np.bool_(True),
        "label": np.str_("x"),
    }
    assert composes(record, to_native) == {
        "count": 7,
        "small": 255,
        "ratio": 0.10000000149011612,
        "mean": 1.5,
        "flag": True,
        "label": "x",
    }


def test_a_structured_array_becomes_rows(composes):
    table = np.array(
        [(1, 2.5, "a"), (2, -0.125, "bb")],
        dtype=[("id", np.int32), ("score", np.float64), ("tag", "U4")],
    )
    assert composes(table, to_native) == [[1, 2.5, "a"], [2, -0.125, "bb"]]


def test_a_column_frame_and_its_rows(composes):
    rng = np.random.default_rng(7)
    columns = {
        "id": np.arange(500, dtype=np.int64),
        "price": np.round(rng.random(500) * 100, 2),
        "in_stock": rng.random(500) > 0.2,
    }
    decoded = composes(columns, to_native)
    rows = [{name: col[i] for name, col in columns.items()} for i in range(500)]
    assert composes(rows, to_native) == [
        {name: decoded[name][i] for name in columns} for i in range(500)
    ]


def test_a_nan_element_follows_the_documented_divergence():
    """NaN serializes as `null` (docs/context/api.md, `dumps`); stdlib writes `NaN`."""
    assert strata.dumps_with_default(np.array([1.0, np.nan]), to_native) == "[1.0,null]"

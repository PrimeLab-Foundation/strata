"""`dump(..., native=True)` in file and folder mode.

docs/decisions.md 2026-09-29 (M15b): `native=True` carries the M15 serializer
contract for `dump` in file and folder mode alike; the folder-mode error
contract (directory without `split_by`, `split_by` with a file, colliding
group names) is unaffected by `native`.
"""

import datetime as dt
import decimal
import uuid

import pytest

import strata


def test_dump_native_true_writes_a_file_with_native_content(tmp_path):
    obj = {"at": dt.date(2026, 1, 1), "id": uuid.UUID(int=1), "amount": decimal.Decimal("1.50")}
    path = tmp_path / "out.json"
    strata.dump(obj, path, native=True)
    assert path.read_bytes() == strata.dumps(obj, native=True).encode() + b"\n"


def test_dump_native_false_on_a_native_object_raises_the_unchanged_type_error(tmp_path):
    with pytest.raises(TypeError, match="^Object of type datetime.date is not JSON serializable$"):
        strata.dump({"at": dt.date(2026, 1, 1)}, tmp_path / "out.json")


RECORDS = [
    {"id": 1, "region": "eu", "at": dt.date(2026, 1, 1)},
    {"id": 2, "region": "us", "at": dt.date(2026, 1, 2)},
    {"id": 3, "region": "eu", "at": dt.date(2026, 1, 3)},
]


def test_dump_native_true_splits_records_with_native_fields(tmp_path):
    strata.dump(RECORDS, tmp_path, split_by="region", native=True)
    eu = (tmp_path / "eu.json").read_bytes()
    us = (tmp_path / "us.json").read_bytes()
    assert (
        eu
        == b'[{"id":1,"region":"eu","at":"2026-01-01"},{"id":3,"region":"eu","at":"2026-01-03"}]\n'
    )
    assert us == b'[{"id":2,"region":"us","at":"2026-01-02"}]\n'


def test_dump_folder_mode_without_native_on_a_native_field_raises(tmp_path):
    with pytest.raises(TypeError, match="^Object of type datetime.date is not JSON serializable$"):
        strata.dump(RECORDS, tmp_path, split_by="region")


@pytest.mark.parametrize("native", [False, True])
def test_a_directory_target_without_split_by_is_rejected(tmp_path, native):
    with pytest.raises(ValueError, match="split_by"):
        strata.dump([{"k": "v"}], tmp_path, native=native)


@pytest.mark.parametrize("native", [False, True])
def test_split_by_with_a_file_target_is_rejected(tmp_path, native):
    target = tmp_path / "out.json"
    target.write_text("[]", encoding="utf-8")
    with pytest.raises(ValueError, match="split_by"):
        strata.dump([{"k": "v"}], target, split_by="k", native=native)


@pytest.mark.parametrize("native", [False, True])
@pytest.mark.parametrize(
    "records",
    [
        [{"k": 0}, {"k": "0"}],
        [{"k": "A"}, {"k": "a"}],
        [{"k": "Group"}, {"k": "group"}],
    ],
)
def test_colliding_group_names_are_rejected(tmp_path, records, native):
    with pytest.raises(ValueError):
        strata.dump(records, tmp_path, split_by="k", native=native)

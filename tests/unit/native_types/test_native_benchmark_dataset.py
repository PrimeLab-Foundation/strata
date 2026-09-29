"""Determinism of the native-v1 benchmark dataset (benchmarks/native_dataset.py).

Pins docs/decisions.md, 2026-09-29, "benchmarks": seed 42, 500/2000/5000
records per tier, one record shape carrying every native type the row 1-9
table of docs/architecture/native_types.md covers.
"""

import dataclasses
import datetime
import decimal
import enum
import uuid

from benchmarks.native_dataset import TIER_RECORDS, Address, Status, generate, generate_tier


def test_generate_is_deterministic_for_a_fixed_seed_and_count():
    first = generate(50)
    second = generate(50)
    assert first == second
    assert first[0] is not second[0]  # equal, not identical: fresh objects each call


def test_generate_differs_across_seeds():
    assert generate(50, seed=1) != generate(50, seed=2)


def test_tier_record_counts_match_the_canonical_generator():
    # benchmarks/data/generate_bench_data.py and the Makefile's bench-data
    # target: small 500, medium 2000, large 5000.
    assert TIER_RECORDS == {"small": 500, "medium": 2000, "large": 5000}
    for tier, count in TIER_RECORDS.items():
        assert len(generate_tier(tier)) == count


def test_generate_tier_rejects_an_unknown_tier():
    try:
        generate_tier("huge")
    except ValueError as error:
        assert "huge" in str(error)
    else:
        raise AssertionError("expected ValueError")


def test_every_native_type_in_the_architecture_table_is_present():
    (record,) = generate(1)
    assert isinstance(record["uuid"], uuid.UUID)
    assert type(record["created_at"]) is datetime.datetime
    assert record["created_at"].microsecond >= 0
    assert type(record["born"]) is datetime.date
    assert isinstance(record["amount"], decimal.Decimal)
    assert isinstance(record["status"], Status) and isinstance(record["status"], enum.Enum)
    assert dataclasses.is_dataclass(record["address"]) and isinstance(record["address"], Address)
    assert isinstance(record["labels"], set) and all(isinstance(t, str) for t in record["labels"])
    assert isinstance(record["active"], bool)
    assert isinstance(record["score"], float)
    assert isinstance(record["id"], int)
    assert isinstance(record["name"], str)

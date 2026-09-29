"""Deterministic native-type dataset for the `native-v1` benchmark scope.

Seed 42, one record shape carrying every native type
`docs/architecture/native_types.md` covers: `uuid.UUID`, a naive
`datetime.datetime` with microseconds, `datetime.date`, a two-place
`decimal.Decimal`, a plain `enum.Enum` member, a `dataclasses.dataclass`
instance, a `set` of short strings, plus plain JSON-native fields (`int`,
`str`, `float`, `bool`).

This dataset is never written to disk as JSON -- unlike the canonical
users/flat/nested/wide_arrays/mixed shapes, its records are not
JSON-serializable by construction, so `native_v1.py` builds them in memory at
measurement time and times each library's own encoding of the native types it
supports (`docs/decisions.md`, 2026-09-29, "benchmarks").
"""

from __future__ import annotations

import dataclasses
import datetime
import decimal
import enum
import random
import uuid

SEED = 42

# Mirrors benchmarks/data/generate_bench_data.py's per-tier record counts
# (Makefile bench-data: small 500, medium 2000, large 5000).
TIER_RECORDS = {"small": 500, "medium": 2000, "large": 5000}

CITIES = ["Berlin", "Lisbon", "Tokyo", "Nairobi", "Bogota", "Oslo", "Chennai"]
TAG_POOL = [f"tag{n}" for n in range(20)]


class Status(enum.Enum):
    ACTIVE = "active"
    PENDING = "pending"
    CLOSED = "closed"


@dataclasses.dataclass
class Address:
    city: str
    zip_code: str


_EPOCH = datetime.datetime(2024, 1, 1)
_DATE_EPOCH = datetime.date(1970, 1, 1)


def _record(rng: random.Random, index: int) -> dict:
    return {
        "id": index,
        "uuid": uuid.UUID(int=rng.getrandbits(128)),
        "created_at": _EPOCH
        + datetime.timedelta(
            seconds=rng.randint(0, 365 * 86400), microseconds=rng.randint(0, 999_999)
        ),
        "born": _DATE_EPOCH + datetime.timedelta(days=rng.randint(0, 20_000)),
        "amount": decimal.Decimal(f"{rng.randint(0, 100_000)}.{rng.randint(0, 99):02d}"),
        "status": rng.choice(list(Status)),
        "address": Address(city=rng.choice(CITIES), zip_code=f"{rng.randint(10_000, 99_999)}"),
        "labels": set(rng.sample(TAG_POOL, rng.randint(1, 4))),
        "active": rng.random() > 0.5,
        "score": round(rng.uniform(0, 100), 3),
        "name": f"item-{index}",
    }


def generate(count: int, seed: int = SEED) -> list[dict]:
    """`count` deterministic records; the same `(count, seed)` always yields
    equal records (equal, not identical: fresh objects each call)."""
    rng = random.Random(seed)
    return [_record(rng, index) for index in range(count)]


def generate_tier(tier: str, seed: int = SEED) -> list[dict]:
    if tier not in TIER_RECORDS:
        raise ValueError(f"unknown tier {tier!r}; expected one of {sorted(TIER_RECORDS)}")
    return generate(TIER_RECORDS[tier], seed=seed)

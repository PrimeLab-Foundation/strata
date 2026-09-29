"""Training workload for the hook image's own profile (scripts/pgo_build.sh phase 3).

Runs against an instrumented `strata._dumps_hook` while `_strata` is the
optimized, uninstrumented image of phase 2, so every count it produces is the
hook's (docs/architecture/native_types.md, "Hook profile"). It exercises what
`native=True` and `dumps_with_default` callers send the hook: records carrying
the native families (`datetime`, `date`, `time`, aware date-times, `UUID`,
`Decimal`, `Enum`, dataclasses, `set`/`frozenset`) among plain JSON values,
plain documents with no native object, the file writer, and the callable.

Everything is generated here from a fixed seed that no benchmark uses; no
benchmark module is imported and no benchmark dataset is read. numpy is not
used: whether a runner has it must not change the profile.
"""

from __future__ import annotations

import argparse
import dataclasses
import datetime
import decimal
import enum
import random
import uuid
from pathlib import Path

import strata

SEED = 7
ROUNDS = 40


class Tier(enum.Enum):
    FREE = "free"
    PAID = "paid"
    TRIAL = "trial"


class Level(enum.Enum):
    LOW = 1
    MID = 2
    HIGH = 3


@dataclasses.dataclass
class Point:
    x: float
    y: float


@dataclasses.dataclass
class Account:
    handle: str
    tier: Tier
    opened: datetime.date
    balance: decimal.Decimal
    home: Point


def _record(rng: random.Random, index: int) -> dict:
    offset = datetime.timezone(datetime.timedelta(minutes=rng.choice((-300, 0, 60, 330))))
    moment = datetime.datetime(2021, 1, 1) + datetime.timedelta(
        seconds=rng.randint(0, 3 * 365 * 86400), microseconds=rng.randint(0, 999_999)
    )
    return {
        "id": index,
        "ref": uuid.UUID(int=rng.getrandbits(128)),
        "at": moment,
        "at_local": moment.replace(tzinfo=offset),
        "day": datetime.date(1990, 1, 1) + datetime.timedelta(days=rng.randint(0, 15_000)),
        "slot": datetime.time(rng.randint(0, 23), rng.randint(0, 59), rng.randint(0, 59)),
        "price": decimal.Decimal(f"{rng.randint(0, 10**6)}.{rng.randint(0, 99):02d}"),
        "tier": rng.choice(list(Tier)),
        "level": rng.choice(list(Level)),
        "account": Account(
            handle=f"user{index}",
            tier=rng.choice(list(Tier)),
            opened=datetime.date(2015, 1, 1) + datetime.timedelta(days=rng.randint(0, 3000)),
            balance=decimal.Decimal(rng.randint(-(10**5), 10**6)) / 100,
            home=Point(round(rng.uniform(-90, 90), 5), round(rng.uniform(-180, 180), 5)),
        ),
        "tags": {f"t{n}" for n in rng.sample(range(30), rng.randint(0, 5))},
        "flags": frozenset(rng.sample(("a", "b", "c", "d"), rng.randint(0, 3))),
        "score": rng.random() * 100,
        "active": rng.random() > 0.3,
        "note": None if rng.random() > 0.5 else f"note {index}",
        "history": [
            {"when": moment - datetime.timedelta(days=n), "amount": decimal.Decimal(n)}
            for n in range(rng.randint(0, 3))
        ],
    }


def _plain(rng: random.Random, count: int) -> list:
    return [
        {
            "id": index,
            "name": f"name-{index}",
            "values": [rng.randint(-1000, 1000) for _ in range(rng.randint(0, 8))],
            "ratio": rng.random(),
            "nested": {"ok": rng.random() > 0.5, "label": None, "depth": [index, [index]]},
        }
        for index in range(count)
    ]


def _default(obj):
    if isinstance(obj, datetime.timedelta):
        return obj.total_seconds()
    raise TypeError(f"unsupported: {type(obj).__name__}")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work-dir", type=Path, required=True)
    args = parser.parse_args(argv)
    args.work_dir.mkdir(parents=True, exist_ok=True)
    target = str(args.work_dir / "hook_training.json")

    rng = random.Random(SEED)
    records = [_record(rng, index) for index in range(2000)]
    plain = _plain(rng, 2000)
    loose = [[r["ref"], r["at"], r["price"], r["tier"], r["tags"]] for r in records[:500]]
    with_default = [datetime.timedelta(seconds=n) for n in range(50)] + records[:200]
    for _ in range(ROUNDS):
        strata.dumps(records, native=True)
        strata.dumps(records, return_type="bytes", native=True)
        strata.dumps(loose, native=True)
        strata.dumps(plain, native=True)
        strata.dump(records, target, native=True)
        strata.dumps_with_default(with_default, _default)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

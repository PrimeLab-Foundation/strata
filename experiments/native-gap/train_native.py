"""Training workload for a hook-image profile (phase-0 attribution only).

Run against an instrumented `_dumps_hook` (PYTHONPATH at the arm). Exercises
the hook's serializer the way `native=True` callers do: the native record
shape at a seed the benchmark never uses (7, not 42), every native family the
record covers plus the ones it does not (aware datetimes, `time`,
`frozenset`, int-valued enums), the canonical plain datasets through
`native=True`, the file writer, and `dumps_with_default`. Nothing here is a
benchmark payload: the benchmark's seed-42 records are never built.
"""

from __future__ import annotations

import datetime
import decimal
import enum
import json
import random
import sys
import tempfile
import uuid
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(1, str(ROOT))

import strata  # noqa: E402
from benchmarks.native_dataset import generate  # noqa: E402


class Level(enum.Enum):
    LOW = 1
    HIGH = 2


def extra(rng: random.Random, count: int) -> list:
    tz = datetime.timezone(datetime.timedelta(hours=2))
    out = []
    for i in range(count):
        out.append(
            {
                "at": datetime.datetime(2023, 1, 1, tzinfo=tz)
                + datetime.timedelta(seconds=rng.randint(0, 10**7)),
                "t": datetime.time(rng.randint(0, 23), rng.randint(0, 59)),
                "level": rng.choice(list(Level)),
                "tags": frozenset(f"k{n}" for n in range(rng.randint(0, 3))),
                "ids": [uuid.UUID(int=rng.getrandbits(128)) for _ in range(2)],
                "price": decimal.Decimal(rng.randint(0, 10**6)) / 100,
                "n": i,
            }
        )
    return out


def main() -> int:
    records = generate(3000, seed=7)
    others = extra(random.Random(7), 1000)
    generated = ROOT / "benchmarks" / "data" / "generated" / "small"
    plain = [
        json.loads((generated / f"{name}.json").read_bytes())
        for name in ("users", "flat", "nested", "wide_arrays", "mixed")
        if (generated / f"{name}.json").is_file()
    ]
    with tempfile.TemporaryDirectory() as scratch:
        target = str(Path(scratch) / "out.json")
        for _ in range(40):
            strata.dumps(records, native=True)
            strata.dumps(records, return_type="bytes", native=True)
            strata.dumps(others, native=True)
            strata.dump(records, target, native=True)
            for document in plain:
                strata.dumps(document, native=True)
            strata.dumps_with_default([datetime.timedelta(seconds=1), *records[:200]], str)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

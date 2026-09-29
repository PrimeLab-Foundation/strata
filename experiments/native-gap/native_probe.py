"""Per-type and per-field attribution of the native-v1 gap to msgspec (one launch).

Imports `strata` from wherever `sys.path` finds it (the driver points
PYTHONPATH at one arm's package copy), builds the payloads, checks every rival
agrees with strata (parsed JSON, decimals as Decimal), then times each
row x library `--repeat` times. Libraries rotate inside every repeat so drift
lands on all of them alike; `gc.collect()` runs before each sample. One JSON
line per (row, library) goes to stdout: per-call seconds for every sample.

Rows:
  rec            the native-v1 small record list (500 records), `dumps`
  rec-file       the same, `dump` (strata) / encode + write (rivals)
  rec-no-<f>     the record list without field <f> (ablation: what <f> costs)
  rec-plain      the record list with every native field removed
  t-<kind>       1000 distinct objects of one type in a list
  t-int / t-str  1000 small ints / short strings (the list's own cost)
"""

from __future__ import annotations

import argparse
import datetime
import decimal
import enum
import gc
import json
import os
import random
import sys
import tempfile
import time
import uuid
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(1, str(ROOT))  # after the arm's PYTHONPATH entry

from benchmarks.native_dataset import Address, Status, generate_tier  # noqa: E402

NATIVE_FIELDS = ("uuid", "created_at", "born", "amount", "status", "address", "labels")
N = 1000


class Color(enum.Enum):
    RED = 1
    GREEN = 2
    BLUE = 3


def _typed(kind: str, rng: random.Random) -> list:
    base = datetime.datetime(2024, 1, 1)
    make = {
        "dt": lambda i: (
            base
            + datetime.timedelta(
                seconds=rng.randint(0, 10**7), microseconds=rng.randint(1, 999_999)
            )
        ),
        "dt-tz": lambda i: (
            base
            + datetime.timedelta(
                seconds=rng.randint(0, 10**7), microseconds=rng.randint(1, 999_999)
            )
        ).replace(tzinfo=datetime.timezone.utc),
        "date": lambda i: (
            datetime.date(1970, 1, 1) + datetime.timedelta(days=rng.randint(0, 20000))
        ),
        "time": lambda i: datetime.time(
            rng.randint(0, 23), rng.randint(0, 59), rng.randint(0, 59), 1
        ),
        "uuid": lambda i: uuid.UUID(int=rng.getrandbits(128)),
        "dec": lambda i: decimal.Decimal(f"{rng.randint(0, 100_000)}.{rng.randint(0, 99):02d}"),
        "enum": lambda i: rng.choice(list(Status)),
        "enum-int": lambda i: rng.choice(list(Color)),
        "dc": lambda i: Address(city=f"c{i % 7}", zip_code=str(10_000 + i)),
        "set": lambda i: {f"tag{n}" for n in rng.sample(range(20), rng.randint(1, 4))},
        "int": lambda i: i,
        "str": lambda i: f"s{i}",
    }[kind]
    return [make(i) for i in range(N)]


TYPES = (
    "dt",
    "dt-tz",
    "date",
    "time",
    "uuid",
    "dec",
    "enum",
    "enum-int",
    "dc",
    "set",
    "int",
    "str",
)


def payload(row: str):
    if row in ("rec", "rec-file"):
        return generate_tier("small")
    if row == "rec-plain":
        return [
            {k: v for k, v in r.items() if k not in NATIVE_FIELDS} for r in generate_tier("small")
        ]
    if row.startswith("rec-no-"):
        field = row[len("rec-no-") :]
        return [{k: v for k, v in r.items() if k != field} for r in generate_tier("small")]
    if row.startswith("t-"):
        return _typed(row[2:], random.Random(7))
    if row.startswith("plain-"):  # a canonical dataset through native=True (the flag rows)
        path = ROOT / "benchmarks" / "data" / "generated" / "small" / f"{row[len('plain-') :]}.json"
        return json.loads(path.read_bytes())
    raise SystemExit(f"unknown row {row}")


def _orjson_default(obj):
    import orjson

    if isinstance(obj, decimal.Decimal):
        return orjson.Fragment(str(obj).encode())
    if isinstance(obj, (set, frozenset)):
        return list(obj)
    raise TypeError


def calls_for(row: str, value, libs: list[str], scratch: Path) -> dict:
    import strata

    calls = {}
    to_file = row == "rec-file"
    for lib in libs:
        if lib == "strata":
            if to_file:
                target = str(scratch / "strata.json")
                calls[lib] = lambda v=value, t=target: strata.dump(v, t, native=True)
            else:
                calls[lib] = lambda v=value: strata.dumps(v, return_type="bytes", native=True)
        elif lib == "msgspec":
            import msgspec

            encode = msgspec.json.Encoder(decimal_format="number").encode
            if to_file:
                target = scratch / "msgspec.json"

                def call(v=value, t=target, e=encode):
                    with open(t, "wb") as handle:
                        handle.write(e(v))
                        handle.write(b"\n")

                calls[lib] = call
            else:
                calls[lib] = lambda v=value, e=encode: e(v)
        elif lib == "orjson":
            import orjson

            if to_file:
                target = scratch / "orjson.json"

                def call(v=value, t=target):
                    with open(t, "wb") as handle:
                        handle.write(orjson.dumps(v, default=_orjson_default))
                        handle.write(b"\n")

                calls[lib] = call
            else:
                calls[lib] = lambda v=value: orjson.dumps(v, default=_orjson_default)
    return calls


def _parsed(data: bytes):
    return json.loads(data, parse_float=decimal.Decimal, parse_int=decimal.Decimal)


def check_agreement(row: str, calls: dict, scratch: Path) -> dict:
    if row == "rec-file":
        for call in calls.values():
            call()
        texts = {lib: (scratch / f"{lib}.json").read_bytes() for lib in calls}
    else:
        texts = {lib: call() for lib, call in calls.items()}
    reference = _parsed(texts["strata"])
    agreeing = {}
    for lib, call in calls.items():
        parsed = _parsed(texts[lib])
        if row.startswith("t-set") or row == "t-set":
            same = [sorted(x) for x in parsed] == [sorted(x) for x in reference]
        else:
            same = parsed == reference
        if same:
            agreeing[lib] = call
        else:
            print(f"# {row}: {lib} disagrees with strata; dropped", file=sys.stderr)
    return agreeing


def _inner_count(call) -> int:
    start = time.perf_counter()
    call()
    elapsed = time.perf_counter() - start
    return max(1, int(0.002 / max(elapsed, 1e-9)))


def measure(rows: list[str], libs: list[str], repeat: int, warmup: int, tag: str) -> None:
    scratch = Path(tempfile.mkdtemp(prefix="nprobe-"))
    for row in rows:
        value = payload(row)
        calls = check_agreement(row, calls_for(row, value, libs, scratch), scratch)
        for call in calls.values():
            for _ in range(warmup):
                call()
        inner = {lib: _inner_count(call) for lib, call in calls.items()}
        samples: dict[str, list[float]] = {lib: [] for lib in calls}
        order = list(calls)
        for index in range(repeat):
            rotation = order[index % len(order) :] + order[: index % len(order)]
            for lib in rotation:
                call = calls[lib]
                count = inner[lib]
                gc.collect()
                start = time.perf_counter()
                for _ in range(count):
                    call()
                samples[lib].append((time.perf_counter() - start) / count)
        for lib, values in samples.items():
            print(json.dumps({"tag": tag, "row": row, "lib": lib, "s": values}), flush=True)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--row", action="append", default=[])
    parser.add_argument("--lib", action="append", default=[])
    parser.add_argument("--repeat", type=int, default=30)
    parser.add_argument("--warmup", type=int, default=3)
    parser.add_argument("--tag", default=os.environ.get("ARM", ""))
    args = parser.parse_args(argv)
    import strata

    print(f"# strata from {Path(strata.__file__).parent}", file=sys.stderr)
    rows = args.row or ["rec"]
    libs = args.lib or ["strata", "msgspec", "orjson"]
    measure(rows, libs, args.repeat, args.warmup, args.tag)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

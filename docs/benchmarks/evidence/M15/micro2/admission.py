"""M15 admission microbenchmark: one process, one arm.

--arm native  (run with ~/worktrees/strata/m15-pgo/.venv, armBown2 installed):
    strata-native   strata.dumps(lst, return_type="bytes")
    strata-dwd-B    strata.dumps_with_default(lst, ref, return_type="bytes") -- natives are served
                    before the callable, so ref must be called 0 times (counted)
--arm hook    (run with ~/worktrees/strata/main-pgo/.venv, main 38eaa9f installed):
    strata-hook     strata.dumps_with_default(lst, ref, return_type="bytes") -- ref once per object
Both arms: orjson where it supports the kind (same orjson 3.12.0 wheel in both venvs), so the
per-process orjson median normalises the cross-process comparison.

lst = [x] * 1000. Each sample = one timed batch of `inner` calls (batch ~3 ms), ns/object =
batch_ns / inner / 1000. gc.collect() before every batch, gc left enabled inside it. Engine
order rotates every repeat. One TSV line per sample on stdout:
arm  launch  kind  engine  repeat  ns_per_object
Plus one '#out' line per (kind, engine) with the sha256 of the output bytes.
"""

from __future__ import annotations

import argparse
import dataclasses
import datetime as dt
import enum
import gc
import hashlib
import sys
import time
import uuid
import zoneinfo
from decimal import Decimal

import numpy as np
import orjson

import strata

N = 1000


class Color(enum.Enum):
    RED = "red"
    GREEN = "green"


@dataclasses.dataclass
class Point5:
    a: int = 12345
    b: str = "hello"
    c: float = 1.5
    d: bool = True
    e: None = None


def ref_datetime(o):
    return o.isoformat()


def ref_uuid(o):
    return str(o)


def ref_decimal(o):
    # No exact Python spelling produces a raw JSON number from a hook except float();
    # for 12345.6789 float's repr is the same digits, so the bytes match native.
    return float(o)


def ref_enum(o):
    return o.value


def ref_dataclass(o):
    return {f.name: getattr(o, f.name) for f in dataclasses.fields(o)}


def ref_set(o):
    return list(o)


def ref_np_scalar(o):
    return o.item()


def ref_ndarray(o):
    return o.tolist()


def kinds():
    tz_ny = zoneinfo.ZoneInfo("America/New_York")
    base = dt.datetime(2026, 9, 28, 12, 34, 56, 789012)
    np_opt = orjson.OPT_SERIALIZE_NUMPY
    return [
        # name, object, ref, orjson option (None = orjson unsupported)
        ("datetime-naive", base, ref_datetime, 0),
        ("datetime-utc", base.replace(tzinfo=dt.timezone.utc), ref_datetime, 0),
        ("datetime-zoneinfo", base.replace(tzinfo=tz_ny), ref_datetime, 0),
        ("date", dt.date(2026, 9, 28), ref_datetime, 0),
        ("time", dt.time(12, 34, 56, 789012), ref_datetime, 0),
        ("uuid", uuid.UUID("12345678-1234-5678-9abc-def012345678"), ref_uuid, 0),
        ("decimal", Decimal("12345.6789"), ref_decimal, None),
        ("enum", Color.RED, ref_enum, 0),
        ("dataclass5", Point5(), ref_dataclass, 0),
        ("set10", set(range(10)), ref_set, None),
        ("np-int64", np.int64(123456789), ref_np_scalar, np_opt),
        ("np-float32", np.float32(0.1), ref_np_scalar, np_opt),
        ("ndarray1000-f64", np.arange(1000, dtype=np.float64) * 0.5, ref_ndarray, np_opt),
    ]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--arm", choices=("native", "hook"), required=True)
    ap.add_argument("--launch", default="0")
    ap.add_argument("--repeat", type=int, default=41)
    ap.add_argument("--batch-ms", type=float, default=3.0)
    args = ap.parse_args()

    calls = {"n": 0}

    def counting(ref):
        def f(o):
            calls["n"] += 1
            return ref(o)

        return f

    print(
        f"#meta arm={args.arm} strata={strata.__file__} numpy={np.__version__} "
        f"orjson={orjson.__version__} python={sys.version.split()[0]}",
        flush=True,
    )
    for name, x, ref, oj in kinds():
        lst = [x] * N
        engines = {}
        if args.arm == "native":
            engines["strata-native"] = lambda lst=lst: strata.dumps(lst, return_type="bytes")
            cref = counting(ref)
            engines["strata-dwd-B"] = lambda lst=lst, cref=cref: strata.dumps_with_default(
                lst, cref, return_type="bytes"
            )
        else:
            engines["strata-hook"] = lambda lst=lst, ref=ref: strata.dumps_with_default(
                lst, ref, return_type="bytes"
            )
        if oj is not None:
            engines["orjson"] = lambda lst=lst, oj=oj: orjson.dumps(lst, option=oj)
        # outputs, warm-up, calibration
        inner = {}
        for eng, fn in engines.items():
            calls["n"] = 0
            out = fn()
            print(
                f"#out {name} {eng} {hashlib.sha256(out).hexdigest()[:16]} len={len(out)} "
                f"ref_calls={calls['n']} head={out[:60]!r}",
                flush=True,
            )
            for _ in range(3):
                fn()
            t0 = time.perf_counter_ns()
            fn()
            one = max(time.perf_counter_ns() - t0, 1)
            inner[eng] = max(1, int(args.batch_ms * 1e6 / one))
        names = list(engines)
        for r in range(args.repeat):
            k = r % len(names)
            for eng in names[k:] + names[:k]:
                fn, n = engines[eng], inner[eng]
                gc.collect()
                t0 = time.perf_counter_ns()
                for _ in range(n):
                    fn()
                ns = time.perf_counter_ns() - t0
                print(f"{args.arm}\t{args.launch}\t{name}\t{eng}\t{r}\t{ns / n / N:.3f}")
        sys.stdout.flush()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

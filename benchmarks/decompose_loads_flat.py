"""Decompose the loads-flat gap on whatever machine runs this.

The dumps twin (`decompose_dumps_mixed.py`) named every MSVC mechanism of the
serializer campaign by splitting one row into per-kind subsets; this does the
same for the parse side, on the row the Windows leg keeps reading behind
msgspec while every POSIX leg leads it: `loads` of small/flat.json (500
records x 21 scalar fields: ints, 5-decimal floats, bools, "value-NNNN"
strings, nulls). Isolated versus interleaved medians first, then flat.json
rebuilt with one value kind at a time (same keys, same record count -- so key
prediction and dict construction are held constant while the value parser
varies), then key-only and value-only extremes, then homogeneous lists per
scalar kind. Whichever subset's ratio stands out carries the leg's gap. Pure
timing, no profiler; meant to be read from a CI log (used by
.github/workflows/profile.yml on the leg the development machine cannot
instrument).
"""

import gc
import json
import random
import statistics
import time


def median_call(call, repeat=60):
    call()
    times = []
    for _ in range(repeat):
        gc.collect()
        start = time.perf_counter_ns()
        call()
        times.append((time.perf_counter_ns() - start) / 1e6)
    return statistics.median(times)


def main() -> int:
    import msgspec
    import orjson

    import strata

    with open("benchmarks/data/generated/small/flat.json", "rb") as handle:
        payload = handle.read()
    records = json.loads(payload)
    decoder = msgspec.json.Decoder()

    def engines(data: bytes) -> dict:
        return {
            "strata": lambda: strata.loads(data),
            "orjson": lambda: orjson.loads(data),
            "msgspec": lambda: decoder.decode(data),
        }

    def report(label: str, data: bytes) -> None:
        calls = engines(data)
        s = median_call(calls["strata"])
        o = median_call(calls["orjson"])
        m = median_call(calls["msgspec"])
        best = min(o, m)
        print(
            f"{label:28s} bytes={len(data):7d}  strata {s:.4f}  orjson {o:.4f}  "
            f"msgspec {m:.4f}  strata/best {s / best:.3f}x"
        )

    calls = engines(payload)
    for call in calls.values():
        call()
        call()
    s_iso = median_call(calls["strata"])
    o_iso = median_call(calls["orjson"])
    m_iso = median_call(calls["msgspec"])
    print(
        f"ISOLATED    strata {s_iso:.4f} ms  orjson {o_iso:.4f} ms  msgspec {m_iso:.4f} ms  "
        f"strata/best {s_iso / min(o_iso, m_iso):.3f}x"
    )
    rounds = 40
    timings = {name: [] for name in calls}
    for _ in range(rounds):
        for name, call in calls.items():
            gc.collect()
            start = time.perf_counter_ns()
            call()
            timings[name].append((time.perf_counter_ns() - start) / 1e6)
    s_int = statistics.median(timings["strata"])
    best_int = min(statistics.median(timings["orjson"]), statistics.median(timings["msgspec"]))
    print(
        f"INTERLEAVED strata {s_int:.4f} ms  best rival {best_int:.4f} ms  ratio {s_int / best_int:.3f}x"
    )

    # One value kind at a time, same keys and record count: what varies is
    # only the value parser and the object each value becomes.
    rng = random.Random(7)
    kinds = {
        "ints-only": lambda: rng.randint(-(10**6), 10**6),
        "floats-5dp-only": lambda: round(rng.uniform(-1e3, 1e3), 5),
        "floats-17dig-only": lambda: rng.random(),
        "bools-only": lambda: rng.random() > 0.5,
        "strs-only": lambda: f"value-{rng.randint(0, 9999)}",
        "nulls-only": lambda: None,
    }
    for label, make in kinds.items():
        subset = [
            {k: (v if k == "id" else make()) for k, v in record.items()} for record in records
        ]
        report(f"records {label}", json.dumps(subset).encode())

    # Extremes: keys with the cheapest possible values (prediction + dict
    # construction alone) versus the values with the cheapest possible keys.
    keys_only = [{k: 0 for k in record} for record in records]
    report("records keys-only (0s)", json.dumps(keys_only).encode())
    values_only = [[v for v in record.values()] for record in records]
    report("records as lists (no keys)", json.dumps(values_only).encode())

    # Homogeneous lists per scalar kind: the value parsers in isolation.
    lists = {
        "list int-1-3dig": [rng.randint(0, 999) for _ in range(10000)],
        "list int-4-6dig": [rng.randint(1000, 999999) for _ in range(10000)],
        "list int-7dig": [rng.randint(1000000, 9999999) for _ in range(10000)],
        "list int-8dig": [rng.randint(10000000, 99999999) for _ in range(10000)],
        "list int-9-10dig": [rng.randint(100000000, 9999999999) for _ in range(10000)],
        "list int-neg-7dig": [-rng.randint(1000000, 9999999) for _ in range(10000)],
        "list float-2dp": [round(rng.uniform(-1e3, 1e3), 2) for _ in range(10000)],
        "list float-5dp": [round(rng.uniform(-1e3, 1e3), 5) for _ in range(10000)],
        "list float-17dig": [rng.random() for _ in range(10000)],
        "list str-10ch": [f"value-{rng.randint(1000, 9999)}" for _ in range(10000)],
        "list str-3ch": [f"s{rng.randint(10, 99)}" for _ in range(10000)],
        "list bools": [rng.random() > 0.5 for _ in range(10000)],
        "list nulls": [None] * 10000,
    }
    for label, values in lists.items():
        report(label, json.dumps(values).encode())
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

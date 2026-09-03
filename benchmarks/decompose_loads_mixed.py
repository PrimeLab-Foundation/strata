"""Decompose the loads-mixed gap on whatever machine runs this.

`loads` of small/mixed.json is the parse row the development machine reads
behind orjson (1.04x) while its dumps twin leads: 500 records of four
rotating shapes -- {"kind","id","value"}, {"type","payload":{x,y},"n"},
{"label","items":[5 ints]}, {"uuid","meta":{"created","tags":[2]},"active"}
-- so key prediction sees a four-way rotation and the value parsers see
short strings, small ints, 17-digit floats, hex ids, tag words, bools and
one nested dict or list per record. Isolated versus interleaved medians
first, then each shape alone (500 records of it, so the rotation itself is
measured against its parts), then the components each shape carries as a
flat list. Whichever subset's ratio stands out carries the gap. Pure timing;
meant to be read locally and from a CI log.
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


def interleaved(calls: dict, repeat=60) -> dict:
    for call in calls.values():
        call()
    times = {name: [] for name in calls}
    for _ in range(repeat):
        for name, call in calls.items():
            gc.collect()
            start = time.perf_counter_ns()
            call()
            times[name].append((time.perf_counter_ns() - start) / 1e6)
    return {name: statistics.median(values) for name, values in times.items()}


def main() -> int:
    import msgspec
    import orjson

    import strata

    with open("benchmarks/data/generated/small/mixed.json", "rb") as handle:
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
    s = median_call(calls["strata"])
    o = median_call(calls["orjson"])
    m = median_call(calls["msgspec"])
    print(
        f"ISOLATED    strata {s:.4f} ms  orjson {o:.4f}  msgspec {m:.4f}  "
        f"strata/best {s / min(o, m):.3f}x"
    )
    inter = interleaved(calls)
    best = min(inter["orjson"], inter["msgspec"])
    print(
        f"INTERLEAVED strata {inter['strata']:.4f} ms  best rival {best:.4f}  "
        f"ratio {inter['strata'] / best:.3f}x"
    )

    # Each shape alone: 500 records of one shape, so the four-way rotation
    # is measured against its parts.
    shapes = {0: [], 1: [], 2: [], 3: []}
    for record in records:
        if "kind" in record:
            shapes[0].append(record)
        elif "type" in record:
            shapes[1].append(record)
        elif "label" in record:
            shapes[2].append(record)
        else:
            shapes[3].append(record)
    names = {0: "kind/id/value", 1: "type/payload/n", 2: "label/items", 3: "uuid/meta/active"}
    for shape, subset in shapes.items():
        full = (subset * 4)[:500]
        report(f"shape {names[shape]}", json.dumps(full).encode())

    # Components as flat lists, 2000 of each: what each shape's values cost
    # without the record around them.
    rng = random.Random(42)
    components = {
        "list 1-char strs": [rng.choice("ab") for _ in range(2000)],
        "list small ints": [rng.randint(0, 500) for _ in range(2000)],
        "list 1-7dig ints": [rng.randint(0, 10**6) for _ in range(2000)],
        "list 17dig floats": [rng.random() for _ in range(2000)],
        "list hex ids": [f"{i:08x}-{rng.randint(0, 0xFFFF):04x}" for i in range(2000)],
        "list tag words": [
            rng.choice(["new", "sale", "featured", "clearance"]) for _ in range(2000)
        ],
        "list bools": [rng.random() > 0.5 for _ in range(2000)],
        "list payload dicts": [{"x": rng.random(), "y": rng.random()} for _ in range(2000)],
        "list 5-int lists": [[rng.randint(0, 100) for _ in range(5)] for _ in range(2000)],
        "list meta dicts": [
            {"created": i, "tags": rng.sample(["new", "sale", "featured", "clearance"], 2)}
            for i in range(2000)
        ],
    }
    for label, data in components.items():
        report(label, json.dumps(data).encode())
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

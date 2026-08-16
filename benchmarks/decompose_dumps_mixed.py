"""Decompose the dumps-mixed gap on whatever machine runs this.

The technique that named every mechanism of the M10/M11 campaign, portable:
isolated versus five-library-interleaved medians, then per-scalar-type
subsets of mixed.json against orjson — whichever component's ratio stands
out carries the leg's gap. Pure timing, no profiler; the output is meant to
be read from a CI log (used by .github/workflows/profile.yml on the leg the
development machine cannot instrument).
"""

import gc
import json
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
    import ujson

    import strata

    with open("benchmarks/data/generated/small/mixed.json") as handle:
        data = json.load(handle)
    encode = msgspec.json.encode

    calls = {
        "strata": lambda: strata.dumps(data, return_type="bytes"),
        "orjson": lambda: orjson.dumps(data),
        "ujson": lambda: ujson.dumps(data),
        "msgspec": lambda: encode(data),
        "json": lambda: json.dumps(data),
    }
    for call in calls.values():
        call()
        call()

    s_iso = median_call(calls["strata"])
    o_iso = median_call(calls["orjson"])
    print(f"ISOLATED    strata {s_iso:.4f} ms  orjson {o_iso:.4f} ms  ratio {s_iso / o_iso:.3f}x")

    rounds = 40
    timings = {name: [] for name in calls}
    for _ in range(rounds):
        for name, call in calls.items():
            gc.collect()
            start = time.perf_counter_ns()
            call()
            timings[name].append((time.perf_counter_ns() - start) / 1e6)
    s_int = statistics.median(timings["strata"])
    o_int = statistics.median(timings["orjson"])
    print(f"INTERLEAVED strata {s_int:.4f} ms  orjson {o_int:.4f} ms  ratio {s_int / o_int:.3f}x")

    def leaves(node, kind):
        if isinstance(node, dict):
            for value in node.values():
                yield from leaves(value, kind)
        elif isinstance(node, list):
            for value in node:
                yield from leaves(value, kind)
        elif type(node) is kind:
            yield node

    subsets = {
        "floats": list(leaves(data, float)),
        "strs": list(leaves(data, str)),
        "ints": [v for v in leaves(data, int) if not isinstance(v, bool)],
        "records-only": [
            {k: v for k, v in record.items() if type(v) in (str, int)} for record in data
        ],
    }
    for name, subset in subsets.items():
        s = median_call(lambda: strata.dumps(subset, return_type="bytes"))
        o = median_call(lambda: orjson.dumps(subset))
        print(
            f"subset {name:14s} n={len(subset):6d}  strata {s:.4f}  orjson {o:.4f}  "
            f"ratio {s / o:.3f}x"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

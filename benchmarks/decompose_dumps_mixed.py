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

from benchmarks import ab_rounds


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

    # Two readings of the same quantity, printed together because they have
    # disagreed. SEQUENTIAL is the original: every strata sample, then every
    # orjson sample -- so a machine that drifted during the run charges the
    # drift to whichever engine ran second, and 3% is well inside what a
    # shared runner drifts in a few seconds. PAIRED takes the same 60 samples
    # per engine in alternating rounds (`benchmarks/ab_rounds.py`), so the
    # difference carries no position term, and reports a bootstrap interval
    # over the pairs. Believe PAIRED; keep SEQUENTIAL to compare with the
    # logs that only have it.
    s_iso = median_call(calls["strata"])
    o_iso = median_call(calls["orjson"])
    print(
        f"ISOLATED (SEQUENTIAL, strata block then orjson block)  "
        f"strata {s_iso:.4f} ms  orjson {o_iso:.4f} ms  ratio {s_iso / o_iso:.3f}x"
    )
    paired = ab_rounds.alternating_rounds(
        {"strata": calls["strata"], "orjson": calls["orjson"]},
        repeat=60,
        preamble=ab_rounds.collect,
    )
    s_pair = statistics.median(paired["strata"])
    o_pair = statistics.median(paired["orjson"])
    print(
        f"ISOLATED (PAIRED, order alternating)                   "
        f"strata {s_pair:.4f} ms  orjson {o_pair:.4f} ms  "
        f"ratio {ab_rounds.paired_ratio(paired['strata'], paired['orjson'])}"
    )

    # The tier harness's own condition, reproduced deliberately: five engines
    # per round in one fixed order, `gc.collect()` before each call. The order
    # is part of what is being reproduced, so it is NOT balanced here -- the
    # official rank comes from exactly this shape. The rotated variant below
    # says how much of the result is the order itself.
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

    names = list(calls)
    rotated = {name: [] for name in names}
    for index in range(rounds):
        order = names[index % len(names) :] + names[: index % len(names)]
        for name in order:
            gc.collect()
            start = time.perf_counter_ns()
            calls[name]()
            rotated[name].append((time.perf_counter_ns() - start) / 1e6)
    s_rot = statistics.median(rotated["strata"])
    o_rot = statistics.median(rotated["orjson"])
    print(
        f"INTERLEAVED (rotated start) strata {s_rot:.4f} ms  orjson {o_rot:.4f} ms  "
        f"ratio {s_rot / o_rot:.3f}x  "
        f"(order effect: {(s_int / o_int) - (s_rot / o_rot):+.3f}x)"
    )

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

    # Length buckets isolate which string tier the leg's compiler mistreats:
    # 1–7 bytes ride the SWAR words, 8–15 the word pair, 16+ the vector
    # blocks, 32+ the AVX2 wide tier, 480+ leaves the scalar-run fast path.
    buckets = {
        "str-1ch": ["a"] * 4000,
        "str-4ch": ["abcd"] * 4000,
        "str-8ch": ["abcdefgh"] * 4000,
        "str-15ch": ["abcdefghijklmno"] * 4000,
        "str-36ch": ["0123456789abcdef0123456789abcdef0123"] * 4000,
        "str-200ch": ["x" * 200] * 800,
        "str-600ch": ["y" * 600] * 400,
        # Mixed lengths, as mixed.json's own strings come (250 one-character
        # kinds, then labels, tag words and 13-character hex ids side by
        # side): the short path's length tiers change per element here,
        # which the same-length buckets above never make them do. On Linux
        # x86 the real-strings subset reads 1.34x while every bucket above
        # sits at parity.
        "str-mixed-1-13": [
            ("a", "c12", "sale", "restock", "featured", "clearance", "00000003-2c83")[i % 7]
            for i in range(4000)
        ],
    }
    for name, bucket in buckets.items():
        s = median_call(lambda: strata.dumps(bucket, return_type="bytes"))
        o = median_call(lambda: orjson.dumps(bucket))
        print(
            f"bucket {name:14s} n={len(bucket):6d}  strata {s:.4f}  orjson {o:.4f}  "
            f"ratio {s / o:.3f}x"
        )

    # Float buckets split the formatter's tiers: 2dp rides the micro-decimal
    # fast tier, 17-digit is pure Dragonbox + fixed layout, integral is the
    # trailing-".0" layout, scientific the exponent layout. mixed's floats
    # are 100% the 17-digit shape.
    fbuckets = {
        "float-2dp": [round(i * 0.37 + 0.1, 2) for i in range(4000)],
        "float-17dig": [i * 0.1234567890123 + 0.1 for i in range(4000)],
        "float-integral": [float(i % 100000) for i in range(4000)],
        "float-sci": [1.5e-7 * (i + 1) for i in range(4000)],
    }
    for name, bucket in fbuckets.items():
        s = median_call(lambda: strata.dumps(bucket, return_type="bytes"))
        o = median_call(lambda: orjson.dumps(bucket))
        print(
            f"fbucket {name:14s} n={len(bucket):5d}  strata {s:.4f}  orjson {o:.4f}  "
            f"ratio {s / o:.3f}x"
        )
    # Int buckets by width: the x86 legs read mixed's ints subset 1.27x behind
    # orjson under both compilers while arm64 reads 0.96x; the digit writer's
    # cost per width names which of its tiers the x86 build pays for.
    ibuckets = {
        "int-1-3dig": [i % 1000 for i in range(4000)],
        "int-4-6dig": [1000 + (i * 7919) % 999000 for i in range(4000)],
        "int-7dig": [1000000 + (i * 7919) % 9000000 for i in range(4000)],
        "int-8dig": [10000000 + (i * 7919) % 90000000 for i in range(4000)],
        "int-9-10dig": [100000000 + (i * 104729) % 9900000000 for i in range(4000)],
        "int-neg-7dig": [-(1000000 + (i * 7919) % 9000000) for i in range(4000)],
        "int-19dig": [1000000000000000000 + i for i in range(4000)],
        # Mixed widths, as real data has them: the digit loop's trip count
        # changes per element, which the same-width buckets never show.
        "int-mixed-1-7": [
            ((i * 2654435761) & 0x7FFFFFFF) // 7 % 10 ** ((((i * 2654435761) & 0x7FFFFFFF) % 7) + 1)
            for i in range(4000)
        ],
    }
    for name, bucket in ibuckets.items():
        s = median_call(lambda: strata.dumps(bucket, return_type="bytes"))
        o = median_call(lambda: orjson.dumps(bucket))
        print(
            f"ibucket {name:14s} n={len(bucket):5d}  strata {s:.4f}  orjson {o:.4f}  "
            f"ratio {s / o:.3f}x"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

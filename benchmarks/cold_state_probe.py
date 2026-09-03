"""How much does each engine lose when it starts cold?

The one row family still behind on the x86 legs — dumps mixed — reads
strata *ahead* in isolation and behind under the tier harness, whose
five-engine round-robin and gc.collect() before every call hand each engine
a cold cache on entry. Bucket decompositions cannot see that (they run one
engine hot), and on the Windows runners they swing 2-3x between samples of
identical code. This probe measures the cold penalty itself, per engine:
the median of a call made right after a cache-evicting sweep over a buffer
larger than any last-level cache, against the median of the same call made
hot. Whichever engine's penalty is larger carries the interleaved row, and
the penalty's size says whether it is code (instruction fetch), data (the
schema cache, key caches), or neither. Pure timing; meant to be read from a
CI log (used by .github/workflows/profile.yml on the legs the development
machine cannot instrument).
"""

import gc
import json
import statistics
import time


def median_ms(call, repeat):
    call()
    times = []
    for _ in range(repeat):
        start = time.perf_counter_ns()
        call()
        times.append((time.perf_counter_ns() - start) / 1e6)
    return statistics.median(times)


def median_cold_ms(call, evict, repeat):
    call()
    times = []
    for _ in range(repeat):
        evict()
        start = time.perf_counter_ns()
        call()
        times.append((time.perf_counter_ns() - start) / 1e6)
    return statistics.median(times)


def main() -> int:
    import msgspec
    import orjson

    import strata

    with open("benchmarks/data/generated/small/mixed.json", "rb") as handle:
        payload = handle.read()
    data = json.loads(payload)
    encode = msgspec.json.Encoder().encode
    decode = msgspec.json.Decoder().decode

    # 64 MB of writes: past every last-level cache the runners have, so the
    # next call finds neither its code nor its data resident.
    sweep = bytearray(64 * 1024 * 1024)
    stride = 64

    def evict():
        gc.collect()
        for offset in range(0, len(sweep), stride):
            sweep[offset] = 1

    # Cold floors: the same calls on one record and on ten, so the fixed
    # cost of entering each engine cold (its code, thread-local state and
    # caches) separates from the per-record cost the document amortizes.
    one = data[:1]
    ten = data[:10]
    one_payload = json.dumps(one).encode()
    ten_payload = json.dumps(ten).encode()
    rows = {
        "dumps mixed": {
            "strata": lambda: strata.dumps(data, return_type="bytes"),
            "orjson": lambda: orjson.dumps(data),
            "msgspec": lambda: encode(data),
        },
        # str mode: the thread-local buffer path plus one PyUnicode copy,
        # against bytes mode's fresh per-call output block — if this one's
        # cold penalty is materially smaller, the block is the surplus.
        "dumps mixed (str mode)": {
            "strata": lambda: strata.dumps(data),
            "orjson": lambda: orjson.dumps(data),
            "msgspec": lambda: encode(data),
        },
        # str mode plus one copy into a fresh bytes object: the shape a
        # copying bytes mode would have. On one Windows runner family bytes
        # mode's direct writes into its fresh block read 30-45 us slower hot
        # than str mode with the same allocation round trip; if this row
        # reads like str mode there, the copy is the remedy for that leg.
        "dumps mixed (str mode + bytes copy)": {
            "strata": lambda: strata.dumps(data).encode(),
            "orjson": lambda: orjson.dumps(data),
        },
        "dumps 10 records": {
            "strata": lambda: strata.dumps(ten, return_type="bytes"),
            "orjson": lambda: orjson.dumps(ten),
            "msgspec": lambda: encode(ten),
        },
        "dumps 1 record": {
            "strata": lambda: strata.dumps(one, return_type="bytes"),
            "orjson": lambda: orjson.dumps(one),
            "msgspec": lambda: encode(one),
        },
        "loads mixed": {
            "strata": lambda: strata.loads(payload),
            "orjson": lambda: orjson.loads(payload),
            "msgspec": lambda: decode(payload),
        },
        "loads 10 records": {
            "strata": lambda: strata.loads(ten_payload),
            "orjson": lambda: orjson.loads(ten_payload),
            "msgspec": lambda: decode(ten_payload),
        },
        "loads 1 record": {
            "strata": lambda: strata.loads(one_payload),
            "orjson": lambda: orjson.loads(one_payload),
            "msgspec": lambda: decode(one_payload),
        },
    }
    for row, calls in rows.items():
        print(f"== {row}")
        for name, call in calls.items():
            hot = median_ms(call, 200)
            cold = median_cold_ms(call, evict, 40)
            print(
                f"{name:8s} hot {hot:.4f} ms  cold {cold:.4f} ms  penalty {cold - hot:+.4f} ms "
                f"({(cold / hot - 1) * 100:+.0f}%)"
            )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

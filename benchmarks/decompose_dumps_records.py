"""Split the records-only half of the dumps-mixed gap into keys, values and frame.

`decompose_dumps_mixed.py` narrows `dumps mixed` to one subset: the 500 records
of mixed.json reduced to their str/int fields read 1.07x behind orjson on the
Neoverse-N2 while every scalar bucket in that same script sits at or near
parity. 1.07x of 22.6 us is the whole row's deficit, so the cost is in the
*record*, not in the scalars it holds. This probe splits the record itself:

    keys-only      the same key rows, every value the int 1
    values-only    each record's values as a list (no keys, no dict frame)
    one-key         each record's first key and value alone
    empty           500 `{}` — the per-dict floor with nothing inside
    uniform         the same field count, one single key row for all 500
                    records (the four-way schema rotation removed)
    full            the records themselves

keys-only plus values-only against full says whether key emission or value
emission carries the gap; empty gives the frame floor; uniform against full
prices the schema rotation that mixed.json's four shapes force. Every row runs
hot and cold — the harness's condition is a `gc.collect()` and a cold cache
before each call, and the row reads *ahead* hot and behind cold, so a hot-only
decomposition cannot see the thing being decomposed.

The second half prices integer width the same way, as plain `dumps` of 4000-
value lists: read against `experiments/itoa/itoa_bench.cpp` on the same job,
whose per-value nanoseconds are the writer alone, the difference is what
CPython extraction and the list loop cost per element.

Pure timing, no profiler; meant to be read from a CI log (used by
.github/workflows/profile.yml on the leg the development machine cannot
instrument).
"""

import gc
import json
import os
import statistics
import time

# The measurement protocol's numbers; the environment overrides exist so a
# development machine can smoke-test the probe without waiting for it.
REPEATS = int(os.environ.get("STRATA_PROBE_REPEATS", "60"))
# 64 MB of writes: past every last-level cache the runners have, so the next
# call finds neither its code nor its data resident. Written as one strided
# slice assignment at the C level — a Python loop over a million offsets would
# be the measurement.
SWEEP_MB = int(os.environ.get("STRATA_PROBE_SWEEP_MB", "64"))


def median_hot_ms(call, repeat=REPEATS):
    call()
    times = []
    for _ in range(repeat):
        start = time.perf_counter_ns()
        call()
        times.append((time.perf_counter_ns() - start) / 1e6)
    return statistics.median(times)


def make_evict():
    sweep = bytearray(SWEEP_MB * 1024 * 1024)
    stride = memoryview(sweep)[::64]
    ones = b"\x01" * len(stride)

    def evict():
        gc.collect()
        stride[:] = ones

    return evict


def median_cold_ms(call, evict, repeat=REPEATS):
    call()
    times = []
    for _ in range(repeat):
        evict()
        start = time.perf_counter_ns()
        call()
        times.append((time.perf_counter_ns() - start) / 1e6)
    return statistics.median(times)


def report(name, subset, unit_count, evict, strata, orjson):
    """One row: strata against orjson, hot and cold, plus ns per unit."""
    s_hot = median_hot_ms(lambda: strata.dumps(subset, return_type="bytes"))
    o_hot = median_hot_ms(lambda: orjson.dumps(subset))
    s_cold = median_cold_ms(lambda: strata.dumps(subset, return_type="bytes"), evict)
    o_cold = median_cold_ms(lambda: orjson.dumps(subset), evict)
    scale = 1e6 / unit_count if unit_count else 0.0
    print(
        f"{name:14s} n={unit_count:5d}  "
        f"HOT  s {s_hot:.4f} o {o_hot:.4f} {s_hot / o_hot:.3f}x "
        f"({s_hot * scale:6.1f} vs {o_hot * scale:6.1f} ns/unit, {(s_hot - o_hot) * scale:+6.1f})  "
        f"COLD s {s_cold:.4f} o {o_cold:.4f} {s_cold / o_cold:.3f}x "
        f"({s_cold * scale:6.1f} vs {o_cold * scale:6.1f} ns/unit, "
        f"{(s_cold - o_cold) * scale:+6.1f})"
    )


def main() -> int:
    import orjson

    import strata

    with open("benchmarks/data/generated/small/mixed.json") as handle:
        data = json.load(handle)

    # The same reduction decompose_dumps_mixed.py names "records-only".
    records = [{k: v for k, v in record.items() if type(v) in (str, int)} for record in data]
    keys_only = [dict.fromkeys(record, 1) for record in records]
    values_only = [list(record.values()) for record in records]
    one_key = [{next(iter(record)): record[next(iter(record))]} for record in records]
    empty = [{} for _ in records]
    # One key row for every record — same width, same key objects, same order
    # — carrying the same values, padded with the int 1 where a shape is
    # narrower. The four-way rotation is then the only thing that changed, so
    # the difference against `full` is what a schema miss costs per record.
    widest = max(len(record) for record in records)
    uniform_keys = [f"k{index}" for index in range(widest)]
    uniform = []
    for record in records:
        values = list(record.values())
        values += [1] * (widest - len(values))
        uniform.append(dict(zip(uniform_keys, values, strict=True)))

    shapes = sorted({tuple(record.keys()) for record in records})
    print(
        f"records: {len(records)}  key rows: {len(shapes)}  "
        f"keys per record min/mean/max "
        f"{min(len(r) for r in records)}/"
        f"{sum(len(r) for r in records) / len(records):.2f}/{widest}  "
        f"bytes {len(orjson.dumps(records))}"
    )
    for shape in shapes:
        count = sum(1 for record in records if tuple(record.keys()) == shape)
        print(f"  shape {shape} x{count}")

    evict = make_evict()
    print("== record decomposition (ns/unit is per record)")
    total_keys = sum(len(record) for record in records)
    report("full", records, len(records), evict, strata, orjson)
    report("keys-only", keys_only, len(records), evict, strata, orjson)
    report("values-only", values_only, len(records), evict, strata, orjson)
    report("one-key", one_key, len(records), evict, strata, orjson)
    report("empty", empty, len(records), evict, strata, orjson)
    report("uniform", uniform, len(records), evict, strata, orjson)
    print(f"(total keys across the full set: {total_keys})")

    # Integer width, as plain dumps: the difference against the same buckets
    # in experiments/itoa/itoa_bench.cpp is extraction plus list loop.
    print("== integer width, plain dumps (ns/unit is per element)")
    ibuckets = {
        "int-1-3dig": [i % 1000 for i in range(4000)],
        "int-4-6dig": [1000 + (i * 7919) % 999000 for i in range(4000)],
        "int-7dig": [1000000 + (i * 7919) % 9000000 for i in range(4000)],
        "int-9-10dig": [100000000 + (i * 104729) % 9900000000 for i in range(4000)],
    }
    for name, bucket in ibuckets.items():
        report(name, bucket, len(bucket), evict, strata, orjson)

    # The float and string rows of the same records, for the per-kind ns the
    # record rows above are compared against.
    print("== value kinds inside the record set (ns/unit is per element)")
    strings = [value for record in records for value in record.values() if type(value) is str]
    integers = [value for record in records for value in record.values() if type(value) is int]
    report("record-strs", strings, len(strings), evict, strata, orjson)
    report("record-ints", integers, len(integers), evict, strata, orjson)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

"""Split the records-only half of the dumps-mixed gap into keys, values and frame.

`decompose_dumps_mixed.py` narrows `dumps mixed` to one subset: the 500 records
of mixed.json reduced to their str/int fields read 1.07x behind orjson on the
Neoverse-N2 while every scalar bucket in that same script sits at or near
parity. 1.07x of 22.6 us is the whole row's deficit, so the cost is in the
*record*, not in the scalars it holds. This probe splits the record itself:

    keys-only        the same key rows, every value the int 1
    values-only      each record's values as a list (no keys, no dict frame)
    one-key          each record's first key and value alone
    empty            500 `{}` -- the per-dict floor with nothing inside
    grouped          the same 500 records, reordered so equal shapes are
                     adjacent: identical bytes, identical keys, identical
                     values, 3 shape transitions instead of 375
    uniform-matched  one key row per width, the widths and values untouched
    uniform-padded   every record padded to the widest width with `k0..kN`
    full             the records themselves

keys-only plus values-only against full says whether key emission or value
emission carries the gap; empty gives the frame floor.

**On pricing "schema rotation".** `uniform-padded` was the first control here
and it is not a matched one: mixed.json's records average 1.75 fields and the
padded ones carry 3, so key count (875 -> 1500), key text (`kind`/`label` ->
`k0`), and serialized size all move together with schema reuse. Its delta is
kept and printed, with those differences quantified beside it, but the control
to read is `grouped`: the same record objects in a different order, so the
*only* thing that changes is how often the shape at the serializer's cursor
differs from the last one. `uniform-matched` sits between them -- one key row
per width, so widths and values are preserved and only the width-1 shapes'
key text moves (its byte delta is printed too).

Every row is measured under three separately labelled conditions, because this
row inverts between them:

    hot   each engine back to back in its own block, nothing between calls
    gc    one `gc.collect()` before each call -- the tier harness's own
          condition (docs/context/benchmarks.md), and the one that flips
          this row
    cold  the same with a cache-evicting sweep on top

The preamble is outside the timed span in all three, and engine order is
balanced (`benchmarks/ab_rounds.py`): the previous version of this probe timed
every strata sample before every orjson sample, so a session that drifted
charged the difference to whichever engine ran late.

Pure timing, no profiler; meant to be read from a CI log (used by
.github/workflows/profile.yml on the leg the development machine cannot
instrument). `STRATA_PROBE_TSV=<path>` also writes every raw sample.

The second half prices integer width the same way, as plain `dumps` of 4000-
value lists: read against `experiments/itoa/itoa_bench.cpp` on the same job,
whose per-value nanoseconds are the writer alone, the difference is what
CPython extraction and the list loop cost per element.
"""

import gc
import json
import os
import statistics

from benchmarks import ab_rounds

# The measurement protocol's numbers; the environment overrides exist so a
# development machine can smoke-test the probe without waiting for it.
REPEATS = int(os.environ.get("STRATA_PROBE_REPEATS", "60"))
# 64 MB of writes: past every last-level cache the runners have, so the next
# call finds neither its code nor its data resident. Written as one strided
# slice assignment at the C level -- a Python loop over a million offsets would
# be the measurement.
SWEEP_MB = int(os.environ.get("STRATA_PROBE_SWEEP_MB", "64"))
TSV_PATH = os.environ.get("STRATA_PROBE_TSV", "")

RAW: list[tuple[object, ...]] = []


def shape_transitions(records) -> int:
    """How often the key row changes from one record to the next."""
    shapes = [tuple(record.keys()) for record in records]
    return sum(1 for left, right in zip(shapes, shapes[1:], strict=False) if left != right)


def describe(name, subset, orjson) -> str:
    """The three quantities a control must match before its delta means anything."""
    if subset and isinstance(subset[0], dict):
        keys = sum(len(record) for record in subset)
        text = sum(len(key) for record in subset for key in record)
        return (
            f"keys {keys:5d}  key bytes {text:6d}  "
            f"json bytes {len(orjson.dumps(subset)):7d}  "
            f"shape changes {shape_transitions(subset):4d}"
        )
    elements = sum(len(item) if isinstance(item, list) else 1 for item in subset)
    return f"elements {elements:5d}  json bytes {len(orjson.dumps(subset)):7d}"


def report(name, subset, unit_count, sweep, strata, orjson):
    """One row: strata against orjson under all three conditions."""
    calls = {
        "strata": lambda: strata.dumps(subset, return_type="bytes"),
        "orjson": lambda: orjson.dumps(subset),
    }
    conditions = {
        "HOT ": ab_rounds.blocked_rounds(calls, repeat=REPEATS, preamble=None),
        "GC  ": ab_rounds.alternating_rounds(calls, repeat=REPEATS, preamble=ab_rounds.collect),
        "COLD": ab_rounds.alternating_rounds(calls, repeat=REPEATS, preamble=sweep),
    }
    scale = 1e6 / unit_count if unit_count else 0.0
    for label, samples in conditions.items():
        for engine, values in samples.items():
            RAW.extend(ab_rounds.tsv_rows(label.strip(), "current", engine, name, values))
        s = statistics.median(samples["strata"])
        o = statistics.median(samples["orjson"])
        ratio = ab_rounds.paired_ratio(samples["strata"], samples["orjson"])
        print(
            f"{name:16s} n={unit_count:5d} {label}  "
            f"s {s:.4f} o {o:.4f}  {ratio}  "
            f"({s * scale:6.1f} vs {o * scale:6.1f} ns/unit, {(s - o) * scale:+6.1f})"
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

    # The matched control: the same record objects, reordered so that equal
    # shapes are adjacent. Byte for byte the same output (a permutation of the
    # same 500 encodings), the same 875 keys, the same values -- only the
    # number of shape changes moves, 375 -> 3. Sorted by the key row so the
    # order is deterministic; ties keep input order.
    grouped = sorted(records, key=lambda record: tuple(record.keys()))

    # One key row per width: widths and value objects untouched, so only the
    # width-1 shapes' key text moves (`uuid` -> `label`). Its byte delta is
    # printed by `describe`, and it is the residual this control still carries.
    by_width: dict[int, tuple[str, ...]] = {}
    for record in records:
        by_width.setdefault(len(record), tuple(record.keys()))
    uniform_matched = [
        dict(zip(by_width[len(record)], record.values(), strict=True)) for record in records
    ]

    # The original control, kept and labelled: one key row for every record,
    # padded to the widest width with the int 1. Same 500 dicts, but 1500 keys
    # instead of 875 and `k0` where the input had `kind`, so its delta prices
    # width and key text alongside schema reuse.
    widest = max(len(record) for record in records)
    uniform_keys = [f"k{index}" for index in range(widest)]
    uniform_padded = []
    for record in records:
        values = list(record.values())
        values += [1] * (widest - len(values))
        uniform_padded.append(dict(zip(uniform_keys, values, strict=True)))

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

    subsets = [
        ("full", records, len(records)),
        ("grouped", grouped, len(records)),
        ("uniform-matched", uniform_matched, len(records)),
        ("uniform-padded", uniform_padded, len(records)),
        ("keys-only", keys_only, len(records)),
        ("values-only", values_only, len(records)),
        ("one-key", one_key, len(records)),
        ("empty", empty, len(records)),
    ]
    print("== how each control differs from `full` (a delta means the control is not matched)")
    for name, subset, _ in subsets:
        print(f"  {name:16s} {describe(name, subset, orjson)}")

    sweep = ab_rounds.make_sweep(SWEEP_MB)
    print("== record decomposition (ns/unit is per record)")
    total_keys = sum(len(record) for record in records)
    for name, subset, units in subsets:
        report(name, subset, units, sweep, strata, orjson)
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
        report(name, bucket, len(bucket), sweep, strata, orjson)

    # The float and string rows of the same records, for the per-kind ns the
    # record rows above are compared against.
    print("== value kinds inside the record set (ns/unit is per element)")
    strings = [value for record in records for value in record.values() if type(value) is str]
    integers = [value for record in records for value in record.values() if type(value) is int]
    report("record-strs", strings, len(strings), sweep, strata, orjson)
    report("record-ints", integers, len(integers), sweep, strata, orjson)

    if TSV_PATH:
        ab_rounds.write_tsv(TSV_PATH, RAW)
        print(f"(raw samples: {TSV_PATH}, {len(RAW)} rows)")
    gc.collect()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

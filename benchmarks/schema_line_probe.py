"""How many cache lines of leased schema state does one record re-read?

The `dumps mixed` deficit that survives on the x86 legs is a *per-record*
cost: on the Windows runner it reads as a stable ~6 us over 500 records,
about 12 ns each, on the same host across draws. A per-call lever cannot
explain a per-record number, and the instruction-side levers are spent --
out-lining the serializer's cold blocks was measured and rejected in wave 26
(docs/performance/SKILL.md), and what that entry leaves open is "compulsory
first-touch ... movable only by executing fewer bytes".

The data side is not spent, and it has precedent on exactly this row family:
`SchemaCacheLease::Schema` inlines its prepared key bytes *because* the heap
blob and offsets cost "three cold hops per record" under the harness's
per-call `gc.collect()`, and fixing that won the row on every x86 leg. This
probe prices the same quantity for the layout that is there now: for each
benchmark dataset's real record-width distribution, how many distinct 64-byte
lines of a depth's `DepthSchemas` a single record touches -- the header
`select()` scans, the matched way's per-key bytes, and the key pointers the
verify scan compares.

It is a *static* model, deliberately: it counts line touches from measured
struct offsets and measured record shapes, so it says whether a layout change
can reduce cold hops at all before anyone spends a PGO build and an A/B on it.
It says nothing about what a line costs. That is the A/B's job
(`benchmarks/ab_rows.py`), and a reduction here is a necessary condition for a
win, never evidence of one.

The offsets below are not guessed. They come from an `offsetof` probe compiled
against `src/strata/bindings/python_dumps_output.h`; re-run that probe and
update them if the struct changes, rather than adjusting them to taste.

usage: schema_line_probe.py [tier]        # tier defaults to small
"""

import collections
import json
import sys
from pathlib import Path

LINE = 64

# Measured with offsetof against python_dumps_output.h. `counts` (4 x 8 B) and
# `first_keys` (4 x 8 B) fill bytes 0..63 exactly, so select()'s scan is one
# line whatever it finds; `ways` follows at 64 with one Schema of stride.
DEPTH_HEADER = 64
WAYS_OFFSET = 64
SCHEMA_STRIDE = 680
WAYS = 4

# Schema's per-key runs, before and after the interleave.
BEFORE = {"spans": 72, "slots": 96, "slot_stride": 16, "key_row": 480, "key_stride": 8}
AFTER = {"spans": 72, "key_slots": 96, "stride": 24}

DATASETS = ("mixed", "flat", "nested", "users", "wide_arrays")


def distinct_lines(runs):
    """Number of distinct 64-byte lines spanned by (offset, size) byte runs."""
    touched = set()
    for offset, size in runs:
        if size > 0:
            touched.update(range(offset // LINE, (offset + size - 1) // LINE + 1))
    return len(touched)


def lines_before(width, way):
    """Today: spans, then the slot bytes, then the key pointers 384 B later."""
    base = WAYS_OFFSET + way * SCHEMA_STRIDE
    return distinct_lines(
        [
            (0, DEPTH_HEADER),
            (base + BEFORE["spans"], width),
            (base + BEFORE["slots"], width * BEFORE["slot_stride"]),
            # matches_tail compares keys 1..width-1 only; the first key was
            # already matched from the header's inline first_keys array.
            (
                base + BEFORE["key_row"] + BEFORE["key_stride"],
                max(0, width - 1) * BEFORE["key_stride"],
            ),
        ]
    )


def lines_after(width, way):
    """Interleaved: one 24-byte {bytes, key} record per key, contiguous."""
    base = WAYS_OFFSET + way * SCHEMA_STRIDE
    return distinct_lines(
        [
            (0, DEPTH_HEADER),
            (base + AFTER["spans"], width),
            (base + AFTER["key_slots"], width * AFTER["stride"]),
        ]
    )


def over_ways(fn, width):
    """A record's way is whichever one holds its shape; average over all four."""
    return sum(fn(width, way) for way in range(WAYS)) / WAYS


def record_widths(path):
    """Key count of every dict in the document, by the width the cache keys on."""
    widths = collections.Counter()

    def walk(node):
        if isinstance(node, dict):
            widths[len(node)] += 1
            for value in node.values():
                walk(value)
        elif isinstance(node, list):
            for value in node:
                walk(value)

    with open(path) as handle:
        walk(json.load(handle))
    return widths


def main():
    tier = sys.argv[1] if len(sys.argv) > 1 else "small"
    root = Path("benchmarks/data/generated") / tier
    if not root.is_dir():
        print(f"no such tier: {root} -- run `make bench-data` first", file=sys.stderr)
        return 1

    print(f"tier {tier}: distinct 64-byte lines of schema state per record")
    print(f"{'dataset':<13} {'dicts':>6} {'widths':<16} {'before':>7} {'after':>7} {'delta':>8}")
    for name in DATASETS:
        path = root / f"{name}.json"
        if not path.exists():
            continue
        widths = record_widths(path)
        total = sum(widths.values())
        if total == 0:
            continue
        before = sum(n * over_ways(lines_before, k) for k, n in widths.items()) / total
        after = sum(n * over_ways(lines_after, k) for k, n in widths.items()) / total
        shape = ",".join(str(k) for k in sorted(widths))
        print(
            f"{name:<13} {total:>6} {shape:<16} {before:>7.2f} {after:>7.2f}"
            f" {100 * (after - before) / before:>7.1f}%"
        )

    print("\nby record width (the shape of the trade):")
    print(f"{'width':>5} {'before':>7} {'after':>7} {'delta':>7}")
    for width in (1, 2, 3, 4, 5, 10, 21, 24):
        before = over_ways(lines_before, width)
        after = over_ways(lines_after, width)
        print(f"{width:>5} {before:>7.2f} {after:>7.2f} {after - before:>+7.2f}")
    print(
        "\nThe interleave pays at every width a benchmark dataset actually has."
        "\nIt costs a quarter of a line at width 1 -- one key, whose pointer the"
        "\nheader already carries -- and is neutral at the 24-key maximum."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

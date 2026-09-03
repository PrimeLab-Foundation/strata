"""Attribute a cachegrind profile to one engine's own functions.

cachegrind's out-file records costs per `fn=` (function) under `fl=` (file);
an extension built without debug info has no file names (`???`) but keeps
its symbol names, so the engine's share is the sum over functions whose
names belong to it — strata's C++ namespaces, msgspec's C prefixes — the
interpreter's, libc's and the sweep's functions left out. Prints the
engine's sums for every event the profile carries and the functions with
the most first-level instruction- and data-cache misses, which is the
code-versus-data answer the cold-state row needs.

The differential form takes two profiles of the same script at two call
counts: the difference divided by the extra calls is the per-call cost of
every function with import-time work removed (msgspec's type creation,
the interpreter's start-up), and it attributes an engine whose symbols are
stripped — orjson's Rust code shows as `???` — by whatever grows with the
calls. Both forms print the `???` bucket -- valgrind's name for every
function without a symbol in any object, the loader's included -- as its
own row, never as any engine's: it is the per-process baseline the
differential cancels.

usage: cg_attribute.py <cachegrind.out> <strata|orjson|msgspec>
       cg_attribute.py --delta <low.out> <high.out> <extra_calls> <engine>
"""

import re
import sys

ENGINE_PATTERNS = {
    "strata": re.compile(r"^(strata::|jkj::dragonbox|PyInit__strata|strata_)"),
    "orjson": re.compile(
        r"^(orjson::|_ZN6orjson|<orjson|core::|alloc::|std::|itoa|ryu|yyjson|_?orjson)"
    ),
    "msgspec": re.compile(r"^(json_|ms_|mpack_|Encoder|Decoder|msgspec|_msgspec|Raw|Struct|Ext_)"),
}
TOP_KEYS = ("I1mr", "D1mr", "Ir")


def load_profile(path):
    """Return (events, {function: [cost per event]}, skipped) summed over every line.

    `skipped` counts cost lines whose column count did not match the events
    header -- zero for a cachegrind profile; a callgrind one would not fit.
    """
    events: list[str] = []
    functions: dict[str, list[int]] = {}
    current = None
    skipped = 0
    with open(path, encoding="utf-8", errors="replace") as handle:
        for line in handle:
            if line.startswith("events:"):
                events = line.split()[1:]
                continue
            if line.startswith("fn="):
                current = line[3:].strip()
                continue
            if current is None or not line[:1].isdigit():
                continue
            costs = [int(value) for value in line.split()[1:]]
            if len(costs) != len(events):
                skipped += 1
                continue
            bucket = functions.setdefault(current, [0] * len(events))
            for index, cost in enumerate(costs):
                bucket[index] += cost
    return events, functions, skipped


def column_sums(functions, width):
    sums = [0] * width
    for costs in functions.values():
        for index, cost in enumerate(costs):
            sums[index] += cost
    return sums


def print_row(label, events, costs, fmt):
    print(label + " ".join(f"{name}={fmt(value)}" for name, value in zip(events, costs)))


def print_top(title, functions, events, key, count, fmt):
    if key not in events:
        return
    column = events.index(key)
    print(f"-- {title} by {key}")
    ranked = sorted(functions.items(), key=lambda item: item[1][column], reverse=True)
    for name, costs in ranked[:count]:
        print(f"   {fmt(costs[column])}  {name[:110]}")


def report_single(path, engine) -> int:
    pattern = ENGINE_PATTERNS[engine]
    events, functions, skipped = load_profile(path)
    if not events:
        print("no events header found")
        return 1
    if skipped:
        print(f"skipped {skipped} cost lines that did not match the events header")
    own = {name: costs for name, costs in functions.items() if pattern.search(name)}
    width = len(events)
    plain = lambda value: f"{value:,}"  # noqa: E731
    print(f"events: {' '.join(events)}")
    print_row("process totals: ", events, column_sums(functions, width), plain)
    print_row(f"{engine} own functions ({len(own)}): ", events, column_sums(own, width), plain)
    print_row("unattributed `???`: ", events, functions.get("???", [0] * width), plain)
    for key in TOP_KEYS:
        print_top(f"top {engine} functions", own, events, key, 12, lambda v: f"{v:>10,}")
    # The process's top functions by first-level instruction misses, whatever
    # they belong to: the check that the engine's name pattern caught its
    # functions (a rival whose symbols are stripped shows up as `???`).
    print_top("top functions, whole process", functions, events, "I1mr", 25, lambda v: f"{v:>10,}")
    return 0


def report_delta(low, high, extra_calls, engine) -> int:
    pattern = ENGINE_PATTERNS[engine]
    events, low_functions, skipped_low = load_profile(low)
    high_events, high_functions, skipped_high = load_profile(high)
    if not events or events != high_events:
        print("profiles carry different event sets")
        return 1
    if skipped_low or skipped_high:
        print(f"skipped {skipped_low}+{skipped_high} cost lines that did not match the header")
    width = len(events)
    zero = [0] * width
    delta = {}
    for name in set(low_functions) | set(high_functions):
        before = low_functions.get(name, zero)
        after = high_functions.get(name, zero)
        delta[name] = [(after[i] - before[i]) / extra_calls for i in range(width)]
    own = {name: costs for name, costs in delta.items() if pattern.search(name)}
    tenths = lambda value: f"{value:,.1f}"  # noqa: E731
    print(f"events: {' '.join(events)}")
    print_row("per call, whole process: ", events, column_sums(delta, width), tenths)
    print_row(
        f"per call, {engine} own functions ({len(own)}): ", events, column_sums(own, width), tenths
    )
    print_row("per call, unattributed `???`: ", events, delta.get("???", [0.0] * width), tenths)
    for key in TOP_KEYS:
        print_top(
            "per-call top functions, whole process",
            delta,
            events,
            key,
            15,
            lambda v: f"{v:>12,.1f}",
        )
    return 0


def main() -> int:
    if len(sys.argv) < 3:
        print(__doc__.strip().splitlines()[-2].strip())
        print(__doc__.strip().splitlines()[-1].strip())
        return 2
    if sys.argv[1] == "--delta":
        return report_delta(sys.argv[2], sys.argv[3], int(sys.argv[4]), sys.argv[5])
    return report_single(sys.argv[1], sys.argv[2])


if __name__ == "__main__":
    raise SystemExit(main())

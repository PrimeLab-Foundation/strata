"""Attribute a cachegrind profile to one engine's own functions.

cachegrind's out-file records costs per `fn=` (function) under `fl=` (file);
an extension built without debug info has no file names (`???`) but keeps
its symbol names, so the engine's share is the sum over functions whose
names belong to it — strata's C++ namespaces, orjson's Rust crate,
msgspec's C prefixes — the interpreter's, libc's and the sweep's functions
left out. Prints the engine's sums for every event the profile carries and
the functions with the most first-level instruction- and data-cache misses,
which is the code-versus-data answer the cold-state row needs.

usage: cg_attribute.py <cachegrind.out> <strata|orjson|msgspec>
"""

import re
import sys

ENGINE_PATTERNS = {
    "strata": re.compile(r"^(strata::|jkj::dragonbox|PyInit__strata|strata_)"),
    "orjson": re.compile(r"^(orjson::|_ZN6orjson|<orjson|core::|alloc::|std::|itoa|ryu|yyjson|_?orjson)"),
    "msgspec": re.compile(r"^(json_|ms_|mpack_|Encoder|Decoder|msgspec|_msgspec|Raw|Struct|Ext_)"),
}


def main() -> int:
    path, engine = sys.argv[1], sys.argv[2]
    pattern = ENGINE_PATTERNS[engine]
    events: list[str] = []
    totals: list[int] = []
    engine_sums: list[int] = []
    per_function: dict[str, list[int]] = {}
    current = None
    with open(path, encoding="utf-8", errors="replace") as handle:
        for line in handle:
            if line.startswith("events:"):
                events = line.split()[1:]
                totals = [0] * len(events)
                engine_sums = [0] * len(events)
                continue
            if line.startswith("fn="):
                current = line[3:].strip()
                continue
            if line.startswith("summary:") or line.startswith("fl=") or line.startswith("fi="):
                continue
            if line.startswith("fe="):
                continue
            if current is None or not line[:1].isdigit():
                continue
            fields = line.split()
            costs = [int(value) for value in fields[1:]]
            if len(costs) != len(events):
                continue
            for index, cost in enumerate(costs):
                totals[index] += cost
            if pattern.search(current):
                for index, cost in enumerate(costs):
                    engine_sums[index] += cost
                bucket = per_function.setdefault(current, [0] * len(events))
                for index, cost in enumerate(costs):
                    bucket[index] += cost
    if not events:
        print("no events header found")
        return 1
    print(f"events: {' '.join(events)}")
    print("process totals: " + " ".join(f"{name}={value:,}" for name, value in zip(events, totals)))
    print(
        f"{engine} own functions ({len(per_function)}): "
        + " ".join(f"{name}={value:,}" for name, value in zip(events, engine_sums))
    )
    for key in ("I1mr", "D1mr", "Ir"):
        if key not in events:
            continue
        column = events.index(key)
        print(f"-- top {engine} functions by {key}")
        ranked = sorted(per_function.items(), key=lambda item: item[1][column], reverse=True)
        for name, costs in ranked[:12]:
            print(f"   {costs[column]:>10,}  {name[:110]}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

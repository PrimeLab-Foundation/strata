"""Cold calls of one engine, for hardware counters.

`perf stat` counts a whole process, so this script does one thing: the same
cache-evicting sweep the cold-state probe uses, then one dumps call, N times,
for the engine named on the command line. The sweep's own counts are the
same for every engine; the difference between two runs is the engine's cold
footprint — instruction-cache and iTLB misses say whether it is code, the
instruction count whether it is work. Used by .github/workflows/profile.yml
on the Linux leg (the Windows leg has no perf and reads the Linux answer).

usage: cold_loop.py <strata|orjson|msgspec> [calls]
"""

import gc
import json
import sys


def main() -> int:
    engine = sys.argv[1]
    calls = int(sys.argv[2]) if len(sys.argv) > 2 else 200
    with open("benchmarks/data/generated/small/mixed.json", "rb") as handle:
        data = json.loads(handle.read())
    if engine == "strata":
        import strata

        call = lambda: strata.dumps(data, return_type="bytes")  # noqa: E731
    elif engine == "orjson":
        import orjson

        call = lambda: orjson.dumps(data)  # noqa: E731
    elif engine == "msgspec":
        import msgspec

        encode = msgspec.json.Encoder().encode
        call = lambda: encode(data)  # noqa: E731
    else:
        raise SystemExit(f"unknown engine {engine}")
    sweep = bytearray(64 * 1024 * 1024)
    call()
    for _ in range(calls):
        gc.collect()
        for offset in range(0, len(sweep), 64):
            sweep[offset] = 1
        call()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

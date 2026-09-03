"""Cold calls of one engine, for hardware counters.

A whole-process counter (cachegrind's simulated caches — the hosted runners
expose no hardware PMU, every `perf stat` event reads "not supported")
needs a process that does one thing: the same cache-evicting sweep the
cold-state probe uses, then one dumps call, N times, for the engine named
on the command line. The sweep's own counts are the same for every engine;
the difference between two runs is the engine's cold footprint —
instruction-cache misses say whether it is code, data-cache misses whether
it is data, the instruction count whether it is work. Used by
.github/workflows/profile.yml on the Linux leg (the Windows leg has no such
tool and reads the Linux answer). Under cachegrind the sweep is the
expensive part, so the sweep size is an argument (its simulated last-level
cache is far smaller than 64 MB).

usage: cold_loop.py <strata|orjson|msgspec> [calls] [sweep_mb]
"""

import gc
import json
import sys


def main() -> int:
    engine = sys.argv[1]
    calls = int(sys.argv[2]) if len(sys.argv) > 2 else 200
    sweep_mb = int(sys.argv[3]) if len(sys.argv) > 3 else 64
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
    sweep = bytearray(sweep_mb * 1024 * 1024)
    call()
    for _ in range(calls):
        gc.collect()
        for offset in range(0, len(sweep), 64):
            sweep[offset] = 1
        call()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

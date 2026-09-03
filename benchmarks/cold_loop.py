"""Cold calls of one engine, for hardware counters.

cachegrind's simulated caches (the hosted runners expose no hardware PMU;
every `perf stat` event reads "not supported") count a whole process, and
its per-function output (cg_annotate) attributes the counts to the engine's
own shared object. This script keeps the process to one thing: the same
cache-evicting sweep the cold-state probe uses, then one dumps call, N
times, for the engine named on the command line. Read per object, the
engine's instruction-cache misses say whether its cold footprint is code,
its data-cache misses whether it is data, its instruction count whether it
is work. Used by .github/workflows/profile.yml on the Linux leg (the
Windows leg has no such tool and reads the Linux answer). Under cachegrind
the sweep is the expensive part, so its size is an argument (the simulated
last-level cache is far smaller than 64 MB).

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
    # One byte per cache line, written by a single C-level slice assignment:
    # a Python-level loop over a million offsets was three billion
    # interpreter instructions per run, which drowned the engine's own
    # counts in the process totals.
    stride = memoryview(sweep)[::64]
    ones = b"\x01" * len(stride)
    call()
    for _ in range(calls):
        gc.collect()
        stride[:] = ones
        call()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

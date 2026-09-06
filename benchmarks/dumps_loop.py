"""Spin one engine's dumps over mixed.json, hot or cold — the body a profiler wraps.

The serializer twin of `loads_loop.py` (inline in the profile workflow) and of
`cold_loop.py`. `perf stat` needs a process that does nothing but the operation
under study, and the row under study reads *ahead* hot and behind cold, so both
conditions need their own countable process:

- ``hot``: warm once, then N calls back to back. Every counter is the
  serializer's own steady state.
- ``gc``: the harness's own condition and nothing else — one
  ``gc.collect()`` before each call. This is what the tier harness does, and
  it is what flips this row: the collector's traversal re-warms every dict's
  internals while evicting the side structures an engine keeps between calls.
- ``cold``: ``gc`` plus a cache-evicting sweep, for a harsher starting state.
  The sweep is written at the C level (one strided slice assignment) so the
  interpreter's own instructions do not drown the engine's. Keep it small:
  a sweep whose own cost dwarfs the call makes the differential below
  meaningless (a 64 MB sweep costs 8 ms here against the call's 0.07 ms, and
  varies by 16% between identical runs).

The gc and cold modes charge the counters for work that is not the engine's,
so the engine name ``none`` runs the same preamble and no dumps at all:
subtracting its counters leaves the engine's own cost under that condition.
Read a mode's runs together or the totals mean nothing.

usage: dumps_loop.py <strata|orjson|none> [iterations] [hot|gc|cold] [sweep_mb]
"""

import gc
import json
import sys
import time


def main() -> int:
    engine = sys.argv[1]
    iterations = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    mode = sys.argv[3] if len(sys.argv) > 3 else "hot"
    sweep_mb = int(sys.argv[4]) if len(sys.argv) > 4 else 64
    with open("benchmarks/data/generated/small/mixed.json") as handle:
        data = json.load(handle)

    if engine == "strata":
        import strata

        call = lambda: strata.dumps(data, return_type="bytes")  # noqa: E731
    elif engine == "orjson":
        import orjson

        call = lambda: orjson.dumps(data)  # noqa: E731
    elif engine == "none":
        call = lambda: None  # noqa: E731
    else:
        raise SystemExit(f"unknown engine: {engine}")

    call()  # warm caches and lazy state outside the measured span
    if mode == "hot":
        start = time.perf_counter()
        for _ in range(iterations):
            call()
        elapsed = time.perf_counter() - start
    elif mode == "gc":
        start = time.perf_counter()
        for _ in range(iterations):
            gc.collect()
            call()
        elapsed = time.perf_counter() - start
    elif mode == "cold":
        sweep = bytearray(sweep_mb * 1024 * 1024)
        stride = memoryview(sweep)[::64]  # one byte per cache line
        ones = b"\x01" * len(stride)
        start = time.perf_counter()
        for _ in range(iterations):
            gc.collect()
            stride[:] = ones
            call()
        elapsed = time.perf_counter() - start
    else:
        raise SystemExit(f"unknown mode: {mode}")
    print(
        f"{engine} {mode}: {iterations} iterations, "
        f"{elapsed * 1e6 / iterations:.2f} us/iteration "
        f"(the mode's preamble is inside the measured span)"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

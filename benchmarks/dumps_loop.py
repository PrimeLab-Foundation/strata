"""Spin one engine's dumps over mixed.json, hot or cold -- the body a profiler wraps.

The serializer twin of `loads_loop.py` (inline in the profile workflow) and of
`cold_loop.py`. `perf stat` needs a process that does nothing but the operation
under study, and the row under study reads *ahead* hot and behind cold, so both
conditions need their own countable process:

- ``hot``: warm once, then N calls back to back. Every counter is the
  serializer's own steady state.
- ``gc``: the harness's own condition and nothing else -- one
  ``gc.collect()`` before each call. This is what the tier harness does, and
  it is what flips this row: the collector's traversal re-warms every dict's
  internals while evicting the side structures an engine keeps between calls.
- ``cold``: ``gc`` plus a cache-evicting sweep, for a harsher starting state.
  The sweep is written at the C level (one strided slice assignment) so the
  interpreter's own instructions do not drown the engine's. Keep it small:
  a sweep whose own cost dwarfs the call makes the differential below
  meaningless (a 64 MB sweep costs 8 ms here against the call's 0.07 ms, and
  varies by 16% between identical runs).

Those three modes put the preamble *inside* the measured span, which is what
`perf stat` needs -- it counts a whole process -- and what a wall-clock
comparison must not use: a difference between two noisy millisecond sweeps
cannot resolve a two-microsecond serializer effect. So there are two more:

- ``gc-call`` / ``cold-call``: the same preamble, run *outside*
  ``perf_counter_ns``, with each call timed on its own. They report min,
  median, mean, p95 and a median absolute deviation, and with
  ``STRATA_LOOP_TSV=<path>`` they append every sample. This is the deciding
  wall-clock instrument; the three modes above stay for the counters.

**The ``none`` arms.** The gc and cold modes charge the counters for work that
is not the engine's, so a ``none`` arm runs the same preamble and no dumps at
all: subtracting its counters leaves the engine's own cost under that
condition. That subtraction is only valid if the two processes differ in the
call and nothing else, and plain ``none`` imports neither engine -- it is
missing an extension mapping, its relocations, its thread-local state and one
output buffer's worth of live heap. It is kept, unchanged, so older logs stay
readable, and it is *not* the control to subtract. The matched controls are:

- ``none-strata``: imports strata, makes one warm ``dumps`` call, then runs
  the preamble and nothing else.
- ``none-orjson``: the same for orjson.

Each matches exactly one engine arm's imports, warmup and live state. Two
residuals remain and cannot be removed: the engine arm allocates and frees one
output object per iteration, and it executes the call -- the thing being
measured. So a subtracted figure is an upper bound on the engine's share.
Quantify the floor by running a matched ``none`` arm twice: the spread between
two identical runs is the resolution of any subtraction made with it.

Read a mode's runs together or the totals mean nothing.

usage: dumps_loop.py <strata|orjson|none|none-strata|none-orjson|pair>
                     [iterations] [hot|gc|cold|gc-call|cold-call|gc-pair|cold-pair] [sweep_mb]

`gc-pair`/`cold-pair` ignore the engine name and time strata and orjson
alternately in one process: the rung between each engine alone and the
official five-engine row.
"""

import gc
import json
import os
import statistics
import sys
import time

TSV_PATH = os.environ.get("STRATA_LOOP_TSV", "")
# Engines to import and warm *besides* the one being timed. The tier harness
# runs five in one process, and a matched `none` arm showed that merely having
# orjson imported and once-called changes what `gc.collect()` costs in that
# process. So "the same call, measured in a five-engine process" is a separate
# condition from "measured alone", and this names it explicitly instead of
# leaving it as an unstated difference between a probe and the official row.
COMPANIONS = [name for name in os.environ.get("STRATA_LOOP_COMPANIONS", "").split(",") if name]


def warm_companions(data):
    """Import and warm the companion engines; return how many were warmed."""
    warmed = 0
    for name in COMPANIONS:
        if name == "strata":
            import strata

            strata.dumps(data, return_type="bytes")
        elif name == "orjson":
            import orjson

            orjson.dumps(data)
        elif name == "msgspec":
            import msgspec

            msgspec.json.encode(data)
        elif name == "ujson":
            import ujson

            ujson.dumps(data)
        elif name == "json":
            import json as stdlib_json

            stdlib_json.dumps(data, separators=(",", ":"))
        else:
            raise SystemExit(f"unknown companion: {name}")
        warmed += 1
    return warmed


def build_call(engine, data):
    """The timed callable, and the imports/warmup that must match around it."""
    if engine in ("strata", "none-strata"):
        import strata

        call = lambda: strata.dumps(data, return_type="bytes")  # noqa: E731
    elif engine in ("orjson", "none-orjson"):
        import orjson

        call = lambda: orjson.dumps(data)  # noqa: E731
    elif engine in ("none", "pair"):
        return lambda: None  # noqa: E731
    else:
        raise SystemExit(f"unknown engine: {engine}")
    if engine.startswith("none-"):
        call()  # same import, same warmup, same live state -- then nothing
        return lambda: None
    return call


def report_samples(engine, mode, samples) -> None:
    ordered = sorted(samples)
    index = max(0, min(len(ordered) - 1, int(round(0.95 * (len(ordered) - 1)))))
    median = statistics.median(ordered)
    print(
        f"{engine} {mode}: {len(ordered)} calls, "
        f"min {ordered[0]:.2f} median {median:.2f} mean {statistics.fmean(ordered):.2f} "
        f"p95 {ordered[index]:.2f} us/call "
        f"(mad {statistics.median([abs(v - median) for v in ordered]):.2f}; "
        f"the preamble is outside the measured span)"
    )
    if TSV_PATH:
        with open(TSV_PATH, "a", encoding="utf-8") as handle:
            for position, value in enumerate(samples):
                handle.write(f"{engine}\t{mode}\t{position}\t{value:.4f}\n")
        print(f"(raw samples appended to {TSV_PATH})")


def main() -> int:
    engine = sys.argv[1]
    iterations = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    mode = sys.argv[3] if len(sys.argv) > 3 else "hot"
    sweep_mb = int(sys.argv[4]) if len(sys.argv) > 4 else 64
    with open("benchmarks/data/generated/small/mixed.json") as handle:
        data = json.load(handle)

    call = build_call(engine, data)
    warmed = warm_companions(data)
    if warmed:
        print(f"({warmed} companion engine(s) imported and warmed: {','.join(COMPANIONS)})")
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
    elif mode in ("gc-pair", "cold-pair"):
        # Both engines in ONE process, alternating, preamble outside the timer.
        # The rung between "each engine alone" (gc-call) and the official row
        # (five engines, fixed order): it holds the workload and the condition
        # fixed and changes only whether a rival runs between the calls. The
        # engine argument is ignored.
        from benchmarks import ab_rounds

        import orjson  # noqa: PLC0415

        import strata  # noqa: PLC0415

        preamble = ab_rounds.collect
        if mode == "cold-pair":
            preamble = ab_rounds.make_sweep(sweep_mb)
        calls = {
            "strata": lambda: strata.dumps(data, return_type="bytes"),
            "orjson": lambda: orjson.dumps(data),
        }
        paired = ab_rounds.alternating_rounds(calls, repeat=iterations, preamble=preamble)
        for name, values in paired.items():
            report_samples(name, mode, [value * 1000.0 for value in values])
        print(
            f"pair {mode}: strata/orjson "
            f"{ab_rounds.paired_ratio(paired['strata'], paired['orjson'])}"
        )
        return 0
    elif mode in ("gc-call", "cold-call"):
        if mode == "cold-call":
            sweep = bytearray(sweep_mb * 1024 * 1024)
            stride = memoryview(sweep)[::64]
            ones = b"\x01" * len(stride)

            def preamble():
                gc.collect()
                stride[:] = ones

        else:
            preamble = gc.collect
        samples = []
        for _ in range(iterations):
            preamble()
            start = time.perf_counter_ns()
            call()
            samples.append((time.perf_counter_ns() - start) / 1e3)  # microseconds
        report_samples(engine, mode, samples)
        return 0
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

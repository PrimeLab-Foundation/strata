"""Order-balanced rounds, raw samples and a paired effect with an interval.

The diagnostic probes that argue about two microseconds need three things the
tier harness does not owe them, because the tier harness answers a different
question (a rank, from ten repeats, over five engines):

1. **Order balance.** `harness.measure_interleaved` walks its engines in dict
   order every round, so one engine is always the first call after the
   `gc.collect()`. That is right for the official protocol -- every engine is
   measured under one fixed, disclosed condition -- and wrong for a probe that
   wants the *difference* between two engines to carry no position term.
   `alternating_rounds` reverses the order on odd rounds (A B / B A), so each
   participant spends half its samples first and half last.
2. **Raw samples.** A median is not evidence when the claim is 3% on a 67 us
   row. Every function here returns (and `write_tsv` records) every sample.
3. **An interval.** `paired_ratio` reports the ratio of medians *and* a
   bootstrap interval over the paired rounds, so a reader can see whether the
   effect clears the round-to-round spread.

The timed span is one call and nothing else: the preamble (`gc.collect()`, a
cache sweep) runs outside `perf_counter_ns`. `docs/context/benchmarks.md` puts
`gc.collect()` before each timed call, and this keeps that condition while
charging the collector's own milliseconds to nobody.

Not a replacement for `benchmarks/harness.py`: the official reports keep
coming from there, unchanged.
"""

from __future__ import annotations

import gc
import random
import statistics
import time
from collections.abc import Callable, Iterable, Sequence
from dataclasses import dataclass

BOOTSTRAP_SEED = 42
BOOTSTRAP_RESAMPLES = 2000


def collect() -> None:
    """The tier harness's own preamble, as a named callable."""
    gc.collect()


def make_sweep(mb: int) -> Callable[[], None]:
    """A `gc.collect()` plus `mb` megabytes of strided writes, one byte per
    64-byte line, written at the C level so the interpreter's own instructions
    do not dominate the eviction."""
    buffer = bytearray(mb * 1024 * 1024)
    stride = memoryview(buffer)[::64]
    ones = b"\x01" * len(stride)

    def sweep() -> None:
        gc.collect()
        stride[:] = ones

    return sweep


def alternating_rounds(
    calls: dict[str, Callable[[], object]],
    *,
    repeat: int,
    warmup: int = 2,
    preamble: Callable[[], None] | None = collect,
) -> dict[str, list[float]]:
    """Time each call `repeat` times, one round at a time, alternating order.

    Round 0 runs the callables in insertion order, round 1 in reverse, and so
    on. Returns milliseconds per sample, in round order, per name.
    """
    names = list(calls)
    for _ in range(warmup):
        for name in names:
            calls[name]()

    samples: dict[str, list[float]] = {name: [] for name in names}
    for index in range(repeat):
        order = names if index % 2 == 0 else list(reversed(names))
        for name in order:
            if preamble is not None:
                preamble()
            start = time.perf_counter_ns()
            calls[name]()
            samples[name].append((time.perf_counter_ns() - start) / 1e6)
    return samples


def blocked_rounds(
    calls: dict[str, Callable[[], object]],
    *,
    repeat: int,
    warmup: int = 2,
    preamble: Callable[[], None] | None = None,
) -> dict[str, list[float]]:
    """Each call's samples taken back to back, in its own block.

    This is what a "hot" number means: an engine running against itself, its
    caches undisturbed by a rival between calls. Alternating the calls would
    measure something else -- two engines evicting each other -- so the order
    balance here is at the block level: the first half of the repeats runs the
    blocks in insertion order, the second half in reverse, so no engine owns
    the start of the session.
    """
    names = list(calls)
    for _ in range(warmup):
        for name in names:
            calls[name]()

    samples: dict[str, list[float]] = {name: [] for name in names}
    tail = repeat // 2
    for order, count in ((names, repeat - tail), (list(reversed(names)), tail)):
        for name in order:
            for _ in range(count):
                if preamble is not None:
                    preamble()
                start = time.perf_counter_ns()
                calls[name]()
                samples[name].append((time.perf_counter_ns() - start) / 1e6)
    return samples


@dataclass(frozen=True)
class Summary:
    """One sample set, described the way a two-microsecond claim needs."""

    n: int
    min_ms: float
    median_ms: float
    mean_ms: float
    p95_ms: float
    # Median absolute deviation: a spread that one outlying round cannot move.
    mad_ms: float

    def __str__(self) -> str:
        return (
            f"n={self.n} min={self.min_ms:.4f} med={self.median_ms:.4f} "
            f"mean={self.mean_ms:.4f} p95={self.p95_ms:.4f} mad={self.mad_ms:.4f}"
        )


def summarize(samples: Sequence[float]) -> Summary:
    ordered = sorted(samples)
    index = max(0, min(len(ordered) - 1, int(round(0.95 * (len(ordered) - 1)))))
    median = statistics.median(ordered)
    return Summary(
        n=len(ordered),
        min_ms=ordered[0],
        median_ms=median,
        mean_ms=statistics.fmean(ordered),
        p95_ms=ordered[index],
        mad_ms=statistics.median([abs(value - median) for value in ordered]),
    )


@dataclass(frozen=True)
class Ratio:
    """A paired ratio and the interval a reader must clear before believing it."""

    ratio: float
    low: float
    high: float
    n_pairs: int

    def __str__(self) -> str:
        return (
            f"{self.ratio:.4f}x [{self.low:.4f}, {self.high:.4f}] "
            f"({(self.ratio - 1) * 100:+.2f}% over {self.n_pairs} pairs)"
        )


def paired_ratio(numerator: Sequence[float], denominator: Sequence[float]) -> Ratio:
    """median(numerator)/median(denominator) with a paired bootstrap interval.

    The two sequences are paired by index -- which is what `alternating_rounds`
    produces: sample i of each name comes from the same round, so a resample
    keeps the pair together and machine drift cancels inside it. Deterministic:
    the seed is fixed, so re-reading the same samples gives the same interval.
    """
    pairs = list(zip(numerator, denominator, strict=True))
    rng = random.Random(BOOTSTRAP_SEED)
    ratios = []
    for _ in range(BOOTSTRAP_RESAMPLES):
        drawn = [pairs[rng.randrange(len(pairs))] for _ in range(len(pairs))]
        top = statistics.median([pair[0] for pair in drawn])
        bottom = statistics.median([pair[1] for pair in drawn])
        ratios.append(top / bottom if bottom else float("nan"))
    ratios.sort()
    low = ratios[int(0.025 * (len(ratios) - 1))]
    high = ratios[int(0.975 * (len(ratios) - 1))]
    return Ratio(
        ratio=statistics.median(numerator) / statistics.median(denominator),
        low=low,
        high=high,
        n_pairs=len(pairs),
    )


TSV_HEADER = ("tag", "build", "engine", "row", "round", "ms")


def tsv_rows(
    tag: str, build: str, engine: str, row: str, samples: Iterable[float]
) -> list[tuple[str, str, str, str, int, float]]:
    return [(tag, build, engine, row, index, value) for index, value in enumerate(samples)]


def write_tsv(
    path, rows: Iterable[Sequence[object]], *, header: Sequence[str] = TSV_HEADER
) -> None:
    with open(path, "w", encoding="utf-8") as handle:
        handle.write("\t".join(header) + "\n")
        for row in rows:
            handle.write("\t".join(str(field) for field in row) + "\n")


def print_tsv(rows: Iterable[Sequence[object]], *, header: Sequence[str] | None = None) -> None:
    """Raw samples on stdout, for a driver that captures a child's output."""
    if header is not None:
        print("\t".join(header))
    for row in rows:
        print("\t".join(str(field) for field in row))

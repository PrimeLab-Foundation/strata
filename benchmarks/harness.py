"""
Shared benchmark harness.

Provides timing, RSS, result types, and a standard warmup+measure loop so
all benchmark scripts use the same methodology and report the same metrics.
"""

from __future__ import annotations

import gc
import statistics
import time
from dataclasses import dataclass, field
from typing import Any, Callable, TypeVar

T = TypeVar("T")

try:
    import psutil

    HAS_PSUTIL = True
except ImportError:
    HAS_PSUTIL = False


def get_rss_mb() -> float:
    """Current process RSS in MB. Returns 0.0 if psutil is not installed."""
    if HAS_PSUTIL:
        return psutil.Process().memory_info().rss / (1024 * 1024)
    return 0.0


def run_timed(
    func: Callable[[], T],
    *,
    warmup: int = 1,
    repeat: int = 3,
) -> tuple[list[float], T]:
    """
    Run a no-arg callable with warmup then repeated timing.

    Returns:
        (times_ms, last_return_value). times_ms has length repeat.
    """
    for _ in range(warmup):
        gc.collect()
        func()

    times_ms: list[float] = []
    result: T = None  # type: ignore
    for _ in range(repeat):
        gc.collect()
        start = time.perf_counter()
        result = func()
        elapsed_ms = (time.perf_counter() - start) * 1000
        times_ms.append(elapsed_ms)

    return times_ms, result


def p95(times_ms: list[float]) -> float:
    """95th percentile of times in ms. Returns 0.0 if empty."""
    if not times_ms:
        return 0.0
    sorted_times = sorted(times_ms)
    idx = min(int(len(sorted_times) * 0.95), len(sorted_times) - 1)
    return sorted_times[idx]


@dataclass
class TimingResult:
    """Aggregated timing for one benchmark run."""

    times_ms: list[float] = field(default_factory=list)
    rss_mb: float = 0.0
    throughput_mbps: float = 0.0

    @property
    def min_ms(self) -> float:
        return min(self.times_ms) if self.times_ms else 0.0

    @property
    def median_ms(self) -> float:
        return statistics.median(self.times_ms) if self.times_ms else 0.0

    @property
    def p95_ms(self) -> float:
        return p95(self.times_ms)

    @property
    def mean_ms(self) -> float:
        return statistics.mean(self.times_ms) if self.times_ms else 0.0


def run_single_benchmark(
    func: Callable[[], Any],
    *,
    warmup: int = 1,
    repeat: int = 3,
    capture_rss: bool = True,
    data_size_bytes: int | None = None,
) -> TimingResult:
    """
    Run one benchmark (warmup + repeat), optionally capture RSS after last run.

    Returns:
        TimingResult with times_ms, rss_mb (if capture_rss), and throughput_mbps
        if data_size_bytes is provided.
    """
    times_ms, _ = run_timed(func, warmup=warmup, repeat=repeat)
    rss = get_rss_mb() if capture_rss else 0.0
    median_ms = statistics.median(times_ms) if times_ms else 0.0
    throughput_mbps = 0.0
    if data_size_bytes and median_ms > 0:
        throughput_mbps = (data_size_bytes / (median_ms / 1000.0)) / 1e6
    return TimingResult(times_ms=times_ms, rss_mb=rss, throughput_mbps=throughput_mbps)

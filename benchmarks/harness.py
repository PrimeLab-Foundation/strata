"""Measurement and report format for the benchmark suite.

This module owns the report format. `bench_main` writes through
`render_report`, `regression_check` reads through `parse_report`, and both use
`SECTIONS` and `COLUMNS` -- so the writer and the reader cannot drift apart.
The previous implementation kept the two definitions separate; the parser
looked for a heading the writer had stopped emitting, and silently found zero
rows (docs/benchmarking/SKILL.md).

Protocol constants come from docs/context/benchmarks.md.
"""

from __future__ import annotations

import gc
import platform
import re
import statistics
import subprocess
import sys
import time
from dataclasses import dataclass, field

# docs/context/benchmarks.md: warmup >= 1, repeat >= 3 exploratory, and
# regression-gate comparisons use the median of >= 10 repeats.
EXPLORATORY_REPEATS = 3
GATING_REPEATS = 10
DEFAULT_WARMUP = 1

# Report sections, in the order they appear. Only the categories that exist
# today; the rest join as their milestones land.
SECTIONS = (
    ("loads", "loads (in-memory parsing)"),
    ("dumps", "dumps (in-memory serialization)"),
    ("load", "load (file to tree)"),
    ("load (ndjson)", "load (NDJSON file to records)"),
    ("dump", "dump (tree to file)"),
    ("query", "query (JSONPath over an in-memory tree)"),
    ("search", "search (JSONPath over a file)"),
)

COLUMNS = ("dataset", "library", "min_ms", "median_ms", "p95_ms", "rss_mb", "speedup_vs_strata")

ERROR_MARKER = "ERROR"


@dataclass(frozen=True)
class Measurement:
    """One library's timings for one dataset in one category."""

    section: str
    dataset: str
    library: str
    min_ms: float | None = None
    median_ms: float | None = None
    p95_ms: float | None = None
    rss_mb: float | None = None
    error: str | None = None

    @property
    def failed(self) -> bool:
        return self.error is not None


@dataclass
class Report:
    name: str
    environment: dict[str, str] = field(default_factory=dict)
    measurements: list[Measurement] = field(default_factory=list)
    excluded: dict[str, str] = field(default_factory=dict)

    @property
    def has_errors(self) -> bool:
        return any(m.failed for m in self.measurements)


def peak_rss_mb() -> float | None:
    """Peak resident set size, or None when psutil is unavailable."""
    try:
        import psutil
    except ImportError:
        return None
    info = psutil.Process().memory_info()
    return info.rss / (1024 * 1024)


def measure_interleaved(
    calls: dict, *, repeat: int, warmup: int = DEFAULT_WARMUP
) -> dict[str, list[float]]:
    """Time several libraries against each other, one round at a time.

    Running every repeat of one library before starting the next lets slow
    drift -- heap growth, CPU frequency, cache state -- land on whichever
    library happened to run late. Measured here: doing that made orjson look
    4x slower on a dataset than it was in isolation. Interleaving the rounds
    spreads any drift across all of them equally, which is the discipline the
    archived measurement work settled on (docs/performance/SKILL.md).

    `gc.collect()` runs before each timed call so one library's garbage is not
    charged to the next (docs/context/benchmarks.md).
    """
    for _ in range(warmup):
        for call in calls.values():
            call()

    timings: dict[str, list[float]] = {name: [] for name in calls}
    for _ in range(repeat):
        for name, call in calls.items():
            gc.collect()
            start = time.perf_counter()
            call()
            timings[name].append((time.perf_counter() - start) * 1000.0)
    return timings


def measure(func, *, repeat: int, warmup: int = DEFAULT_WARMUP) -> tuple[list[float], float | None]:
    """Time a single callable. Prefer `measure_interleaved` for comparisons."""
    timings = measure_interleaved({"only": func}, repeat=repeat, warmup=warmup)["only"]
    return timings, peak_rss_mb()


def summarize(
    section: str, dataset: str, library: str, timings: list[float], rss_mb: float | None
) -> Measurement:
    ordered = sorted(timings)
    index = max(0, min(len(ordered) - 1, int(round(0.95 * (len(ordered) - 1)))))
    return Measurement(
        section=section,
        dataset=dataset,
        library=library,
        min_ms=ordered[0],
        median_ms=statistics.median(ordered),
        p95_ms=ordered[index],
        rss_mb=rss_mb,
    )


def describe_environment(compiler_flags: str) -> dict[str, str]:
    return {
        "commit": _git_commit(),
        "python": platform.python_version(),
        "implementation": platform.python_implementation(),
        "platform": platform.platform(),
        "machine": platform.machine(),
        "processor": _cpu_name(),
        "compiler_flags": compiler_flags,
    }


def _git_commit() -> str:
    try:
        result = subprocess.run(
            ["git", "rev-parse", "--short", "HEAD"],
            capture_output=True,
            text=True,
            check=False,
        )
    except OSError:
        return "unknown"
    return result.stdout.strip() or "unknown"


def _cpu_name() -> str:
    if sys.platform == "darwin":
        try:
            result = subprocess.run(
                ["sysctl", "-n", "machdep.cpu.brand_string"],
                capture_output=True,
                text=True,
                check=False,
            )
        except OSError:
            return platform.processor() or "unknown"
        return result.stdout.strip() or platform.processor() or "unknown"
    return platform.processor() or "unknown"


# ---------------------------------------------------------------------------
# Report rendering and parsing -- one format, two directions.
# ---------------------------------------------------------------------------

_ROW = re.compile(r"^\|(?P<cells>.*)\|\s*$")


def _format_cell(value: float | None) -> str:
    return "-" if value is None else f"{value:.3f}"


def render_report(report: Report) -> str:
    # A section with no entry here would be measured and then silently dropped
    # from the report -- the exact writer/reader drift this module exists to
    # prevent. Fail instead.
    known = {key for key, _ in SECTIONS}
    unknown = sorted({m.section for m in report.measurements} - known)
    if unknown:
        raise ValueError(
            f"measurements in unknown section(s) {unknown}; add them to harness.SECTIONS"
        )

    lines = [f"# Benchmark results - {report.name}", ""]
    lines.append("Machine-written by `make bench-*`. Do not hand-edit.")
    lines.append("")
    lines.append("`speedup_vs_strata` above 1.00 means that library is faster than strata.")
    lines.append("")
    for key, value in report.environment.items():
        lines.append(f"- {key}: {value}")
    if report.excluded:
        lines.append("")
        lines.append("Excluded libraries (not installed, or no native equivalent):")
        for library, reason in sorted(report.excluded.items()):
            lines.append(f"- {library}: {reason}")
    lines.append("")

    for section_key, section_title in SECTIONS:
        rows = [m for m in report.measurements if m.section == section_key]
        if not rows:
            continue
        lines.append(f"## {section_key} -- {section_title}")
        lines.append("")
        lines.append("| " + " | ".join(COLUMNS) + " |")
        lines.append("|" + "|".join(["---"] * len(COLUMNS)) + "|")

        by_dataset: dict[str, list[Measurement]] = {}
        for row in rows:
            by_dataset.setdefault(row.dataset, []).append(row)

        for dataset, dataset_rows in by_dataset.items():
            reference = next(
                (r.median_ms for r in dataset_rows if r.library == "strata" and not r.failed),
                None,
            )
            for row in dataset_rows:
                if row.failed:
                    cells = [
                        row.dataset,
                        row.library,
                        ERROR_MARKER,
                        ERROR_MARKER,
                        ERROR_MARKER,
                        ERROR_MARKER,
                        row.error or ERROR_MARKER,
                    ]
                else:
                    ratio = (
                        f"{reference / row.median_ms:.2f}x" if reference and row.median_ms else "-"
                    )
                    cells = [
                        row.dataset,
                        row.library,
                        _format_cell(row.min_ms),
                        _format_cell(row.median_ms),
                        _format_cell(row.p95_ms),
                        _format_cell(row.rss_mb),
                        ratio,
                    ]
                lines.append("| " + " | ".join(cells) + " |")
        lines.append("")

    return "\n".join(lines) + "\n"


def parse_report(text: str, name: str = "") -> Report:
    """Read back a rendered report. The inverse of `render_report`."""
    report = Report(name=name)
    section = ""
    titles = {key: title for key, title in SECTIONS}

    for line in text.splitlines():
        stripped = line.strip()
        if stripped.startswith("## "):
            heading = stripped[3:].strip()
            section = heading.split(" -- ", 1)[0].strip()
            if section not in titles:
                section = ""
            continue
        if stripped.startswith("- ") and ":" in stripped and not section:
            key, _, value = stripped[2:].partition(":")
            report.environment[key.strip()] = value.strip()
            continue

        match = _ROW.match(line)
        if not match or not section:
            continue
        cells = [c.strip() for c in match.group("cells").split("|")]
        if len(cells) != len(COLUMNS) or cells[0] in ("dataset", "---"):
            continue
        if set(cells[0]) <= {"-"}:
            continue

        dataset, library = cells[0], cells[1]
        if ERROR_MARKER in cells:
            report.measurements.append(
                Measurement(section=section, dataset=dataset, library=library, error=cells[-1]),
            )
            continue
        report.measurements.append(
            Measurement(
                section=section,
                dataset=dataset,
                library=library,
                min_ms=_read_cell(cells[2]),
                median_ms=_read_cell(cells[3]),
                p95_ms=_read_cell(cells[4]),
                rss_mb=_read_cell(cells[5]),
            ),
        )
    return report


def _read_cell(cell: str) -> float | None:
    if cell == "-":
        return None
    try:
        return float(cell)
    except ValueError:
        return None


def baseline_key(report_name: str, section: str, dataset: str) -> str:
    """Baseline entries are keyed by the full report name, not a stem.

    The previous implementation keyed on `path.stem` from an earlier era, so no
    key ever matched and the gate compared nothing.
    """
    return f"{report_name}|{section}|{dataset}"

"""Measurement and report format for the benchmark suite.

This module owns the report format. `bench_main` writes through
`render_report`, `regression_check` reads through `parse_report`, and both use
`SECTIONS` and `COLUMNS` -- so the writer and the reader cannot drift apart.
The previous implementation kept the two definitions separate; the parser
looked for a heading the writer had stopped emitting, and silently found zero
rows (docs/benchmarking/SKILL.md).

It also owns the *declared workload* and the *validity* of a report, for the
same reason: every gate (`supportability_check`, `ci_summary`,
`regression_check`, `ci_fetch`) has to agree on what a complete, readable run
looks like, and each one deciding for itself is how a gate comes to pass on
evidence that is not there. `WORKLOADS` names the rows a run is expected to
contain and `validate_report` is the single check; a tool chooses its
expectation and its exit code, never its own notion of validity.

Protocol constants come from docs/context/benchmarks.md.
"""

from __future__ import annotations

import gc
import math
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
    ("search (ndjson)", "supplementary v1 NDJSON search"),
    ("folder load", "supplementary v1 folder load"),
    ("folder dump", "supplementary v1 folder dump"),
    ("folder search", "supplementary v1 folder search"),
)

COLUMNS = ("dataset", "library", "min_ms", "median_ms", "p95_ms", "rss_mb", "speedup_vs_strata")

ERROR_MARKER = "ERROR"

MEASURED_LIBRARY = "strata"

# ---------------------------------------------------------------------------
# The declared workload
#
# What a run is *expected* to contain, declared here rather than inferred from
# whatever survived parsing. The reviewed gates counted the rows they were
# given: a report missing a dataset, a platform that never uploaded, or a
# library that vanished from a row all shrank the objective instead of failing
# it (docs/performance/ci-review-2026-09-07.md, finding 2).
#
# `WORKLOAD_DATASETS` and `QUERY_LABELS` mirror the CI benchmark job
# (.github/workflows/benchmark.yml) and the bench-small/medium/large Make
# targets, which pass the same six datasets; the dispatch below mirrors
# `bench_main.run` -- .ndjson goes to `load (ndjson)`, every other dataset to
# loads/dumps/load/dump, and the users shape additionally to query/search once
# per JSONPath expression. That is 27 strata rows per platform.
# ---------------------------------------------------------------------------

WORKLOAD_DATASETS = (
    "users.json",
    "users.ndjson",
    "flat.json",
    "nested.json",
    "wide_arrays.json",
    "mixed.json",
)

# bench_main.QUERIES, by their report labels.
QUERY_LABELS = ("$[*].id", "$[*].orders[*].total", "$..total")

# The platform/architecture legs of the CI benchmark job. A leg that did not
# report is missing evidence, not a smaller goal.
CI_PLATFORMS = ("linux-arm64", "linux-x86_64", "macos-arm64", "macos-x86_64", "windows-x86_64")


def workload_rows(
    datasets: tuple[str, ...] = WORKLOAD_DATASETS,
    query_labels: tuple[str, ...] = QUERY_LABELS,
) -> tuple[tuple[str, str], ...]:
    """The (section, dataset) rows `bench_main.run` produces for `datasets`."""
    rows: list[tuple[str, str]] = []
    for name in datasets:
        if name.endswith((".ndjson", ".jsonl")):
            rows.append(("load (ndjson)", name))
            continue
        rows.extend((("loads", name), ("dumps", name), ("load", name), ("dump", name)))
        if name.rsplit(".", 1)[0] == "users":
            for label in query_labels:
                rows.append(("query", f"{name} {label}"))
                rows.append(("search", f"{name} {label}"))
    return tuple(rows)


# Named expectations a tool can be pointed at. "ci" is the declared 27-row
# workload; "none" is an explicitly scoped diagnostic -- validity is still
# checked, completeness is not claimed.
WORKLOADS: dict[str, tuple[tuple[str, str], ...] | None] = {
    "ci": workload_rows(),
    "none": None,
}


def resolve_workload(name: str) -> tuple[tuple[str, str], ...] | None:
    """The expected rows of a named workload; `ValueError` if it has no name."""
    if name not in WORKLOADS:
        raise ValueError(f"unknown workload {name!r}; expected one of {sorted(WORKLOADS)}")
    return WORKLOADS[name]


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
    samples_ms: tuple[float, ...] = ()

    @property
    def failed(self) -> bool:
        return self.error is not None


@dataclass
class Report:
    name: str
    environment: dict[str, str] = field(default_factory=dict)
    measurements: list[Measurement] = field(default_factory=list)
    excluded: dict[str, str] = field(default_factory=dict)
    # Table rows the parser could not read. Kept rather than dropped: a row
    # silently skipped is a measurement that disappears from every count.
    malformed: list[str] = field(default_factory=list)
    provenance: dict = field(default_factory=dict)

    @property
    def has_errors(self) -> bool:
        return any(m.failed for m in self.measurements)


# ---------------------------------------------------------------------------
# Validity and completeness -- one check, four gates
# ---------------------------------------------------------------------------

# A problem that makes the evidence unusable: the run failed, a number is not
# a number, a row is there twice, a row the parser could not read, or a
# declared row that nobody measured. Everything else is disclosed, not fatal.
FATAL_KINDS = frozenset({"error", "invalid", "duplicate", "malformed", "missing"})


@dataclass(frozen=True)
class Problem:
    """One reason a report cannot be counted, or must be counted with a caveat."""

    kind: str  # error | invalid | duplicate | malformed | missing | extra | uncomparable
    where: str  # section|dataset[|library], or the report name
    detail: str

    @property
    def fatal(self) -> bool:
        return self.kind in FATAL_KINDS

    def __str__(self) -> str:
        return f"{self.kind} {self.where}: {self.detail}"


@dataclass(frozen=True)
class Validation:
    """The verdict on one report: what is wrong, and what was actually measured."""

    name: str
    problems: tuple[Problem, ...] = ()
    expected: tuple[tuple[str, str], ...] | None = None
    measured: tuple[tuple[str, str], ...] = ()

    @property
    def ok(self) -> bool:
        """True when nothing fatal was found -- valid *and*, if a workload was
        declared, complete."""
        return not any(problem.fatal for problem in self.problems)

    @property
    def valid(self) -> bool:
        """True when the numbers themselves are usable, missing rows aside."""
        return not any(problem.fatal and problem.kind != "missing" for problem in self.problems)

    @property
    def complete(self) -> bool:
        return self.expected is not None and not self.of("missing")

    def of(self, *kinds: str) -> tuple[Problem, ...]:
        return tuple(problem for problem in self.problems if problem.kind in kinds)

    def describe(self) -> str:
        """A one-line count of what was found, for a CLI or a report line."""
        counts: dict[str, int] = {}
        for problem in self.problems:
            counts[problem.kind] = counts.get(problem.kind, 0) + 1
        if not counts:
            body = "no problems"
        else:
            body = ", ".join(f"{count} {kind}" for kind, count in sorted(counts.items()))
        if self.expected is None:
            return f"{self.name}: {body} (no declared workload)"
        return f"{self.name}: {len(self.measured)}/{len(self.expected)} declared rows, {body}"


def _metric_problems(measurement: Measurement, where: str) -> list[Problem]:
    problems: list[Problem] = []
    for metric, value in (
        ("min_ms", measurement.min_ms),
        ("median_ms", measurement.median_ms),
        ("p95_ms", measurement.p95_ms),
    ):
        if value is None:
            problems.append(Problem("invalid", where, f"{metric} is absent"))
        elif not math.isfinite(value):
            problems.append(Problem("invalid", where, f"{metric} is not finite ({value})"))
        elif value < 0.0:
            problems.append(Problem("invalid", where, f"{metric} is negative ({value})"))
    # A zero median is not a fast row, it is an unmeasured one -- and every
    # ratio in the gates divides by it.
    if measurement.median_ms == 0.0:
        problems.append(Problem("invalid", where, "median_ms is zero"))
    rss = measurement.rss_mb
    if rss is not None and (not math.isfinite(rss) or rss < 0.0):
        problems.append(Problem("invalid", where, f"rss_mb is not a usable size ({rss})"))
    if problems:
        return problems
    low, median, high = measurement.min_ms, measurement.median_ms, measurement.p95_ms
    if not (low <= median <= high):  # type: ignore[operator]
        problems.append(
            Problem("invalid", where, f"min/median/p95 out of order ({low}, {median}, {high})")
        )
    return problems


def validate_report(
    report: Report,
    *,
    expected: tuple[tuple[str, str], ...] | None = None,
    library: str = MEASURED_LIBRARY,
) -> Validation:
    """Check one report for validity and, when `expected` is given, completeness.

    Validity is: every table row readable, no ERROR rows, every timing finite
    and ordered min <= median <= p95, every (section, dataset, library) entry
    present once, and at least one measurement. Completeness is: every declared
    row carries a usable `library` measurement.

    Rows outside the declared workload are reported as `extra` and rows with no
    rival to rank against as `uncomparable` -- both disclosed, neither fatal:
    extra evidence is still evidence, and a competitor with no native
    equivalent is a documented exclusion (docs/context/benchmarks.md).
    """
    problems: list[Problem] = []
    for line in report.malformed:
        problems.append(Problem("malformed", report.name or "report", f"unreadable row: {line}"))

    if not report.measurements:
        problems.append(Problem("invalid", report.name or "report", "no measurements at all"))

    seen: set[tuple[str, str, str]] = set()
    usable: dict[tuple[str, str], set[str]] = {}
    for measurement in report.measurements:
        where = f"{measurement.section}|{measurement.dataset}|{measurement.library}"
        entry = (measurement.section, measurement.dataset, measurement.library)
        if entry in seen:
            problems.append(Problem("duplicate", where, "measured more than once"))
            continue
        seen.add(entry)
        if measurement.failed:
            detail = f"{ERROR_MARKER} ({measurement.error})" if measurement.error else ERROR_MARKER
            problems.append(Problem("error", where, detail))
            continue
        found = _metric_problems(measurement, where)
        problems.extend(found)
        if not found:
            usable.setdefault((measurement.section, measurement.dataset), set()).add(
                measurement.library
            )

    for row in sorted(usable):
        libraries = usable[row]
        if library in libraries and len(libraries) == 1:
            problems.append(
                Problem("uncomparable", f"{row[0]}|{row[1]}", f"no rival measured beside {library}")
            )

    measured: tuple[tuple[str, str], ...] = ()
    if expected is not None:
        measured = tuple(row for row in expected if library in usable.get(row, ()))
        for row in expected:
            if row not in measured:
                problems.append(
                    Problem("missing", f"{row[0]}|{row[1]}", f"no usable {library} measurement")
                )
        for row in sorted(set(usable) - set(expected)):
            problems.append(
                Problem("extra", f"{row[0]}|{row[1]}", "row is outside the declared workload")
            )

    return Validation(
        name=report.name or "report",
        problems=tuple(problems),
        expected=expected,
        measured=measured,
    )


def peak_rss_mb() -> float | None:
    """The whole process's *current* resident set size -- not a peak, not
    per-library, despite the name -- or None when psutil is unavailable.

    One read per row, recorded for every library in it; kept under this name
    because docs/context/benchmarks.md and docs/benchmarking/SKILL.md cite it.
    """
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
        samples_ms=tuple(timings),
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
    if sys.platform.startswith("linux"):
        from pathlib import Path

        try:
            for line in Path("/proc/cpuinfo").read_text().splitlines():
                key, _, value = line.partition(":")
                if key.strip() in {"model name", "Hardware", "Model"} and value.strip():
                    return value.strip()
        except OSError:
            pass
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
    """Read back a rendered report. The inverse of `render_report`.

    A table row inside a section that this parser cannot read is kept in
    `Report.malformed` instead of being dropped: a silently skipped row is a
    measurement that disappears from every count downstream
    (docs/performance/ci-review-2026-09-07.md, finding 2).
    """
    report = Report(name=name)
    section = ""
    titles = {key: title for key, title in SECTIONS}
    exclusions = False

    for line in text.splitlines():
        stripped = line.strip()
        if stripped == "Excluded libraries (not installed, or no native equivalent):":
            exclusions = True
            continue
        if stripped.startswith("## "):
            heading = stripped[3:].strip()
            section = heading.split(" -- ", 1)[0].strip()
            if section not in titles:
                section = ""
            continue
        if stripped.startswith("- ") and ":" in stripped and not section:
            key, _, value = stripped[2:].partition(":")
            target = report.excluded if exclusions else report.environment
            target[key.strip()] = value.strip()
            continue

        match = _ROW.match(line)
        if not match or not section:
            continue
        cells = [c.strip() for c in match.group("cells").split("|")]
        if cells and cells[0] in ("dataset", "---"):
            continue
        if cells and set(cells[0]) <= {"-"}:
            continue
        if len(cells) != len(COLUMNS):
            report.malformed.append(stripped)
            continue

        dataset, library = cells[0], cells[1]
        if ERROR_MARKER in cells:
            report.measurements.append(
                Measurement(section=section, dataset=dataset, library=library, error=cells[-1]),
            )
            continue
        values = [_read_cell(cell) for cell in cells[2:6]]
        if any(value is _UNREADABLE for value in values):
            report.malformed.append(stripped)
            continue
        report.measurements.append(
            Measurement(
                section=section,
                dataset=dataset,
                library=library,
                min_ms=values[0],
                median_ms=values[1],
                p95_ms=values[2],
                rss_mb=values[3],
            ),
        )
    return report


def read_report(path) -> Report:
    """Read a legacy report or validate its full-precision companion if present."""
    from benchmarks.provenance import validate_companion

    text = path.read_text(encoding="utf-8")
    report = parse_report(text, name=path.name)
    try:
        data = validate_companion(path, text)
    except (ValueError, OSError) as error:
        report.malformed.append(str(error))
    else:
        if data is not None:
            report.provenance = data["provenance"]
            report.measurements = [Measurement(**row) for row in data["measurements"]]
    return report


class _Unreadable:
    """A cell that is neither a number nor the "-" absent marker."""

    def __repr__(self) -> str:  # pragma: no cover - debugging aid
        return "<unreadable cell>"


# One sentinel, so "the writer left this out" and "this is not a number" stay
# distinguishable: the first is a `None` metric, the second a malformed row.
_UNREADABLE = _Unreadable()


def _read_cell(cell: str) -> float | None | _Unreadable:
    if cell == "-":
        return None
    try:
        return float(cell)
    except ValueError:
        return _UNREADABLE


def baseline_key(report_name: str, section: str, dataset: str) -> str:
    """Baseline entries are keyed by the full report name, not a stem.

    The previous implementation keyed on `path.stem` from an earlier era, so no
    key ever matched and the gate compared nothing.
    """
    return f"{report_name}|{section}|{dataset}"

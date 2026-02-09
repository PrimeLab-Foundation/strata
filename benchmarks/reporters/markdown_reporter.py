"""
Markdown benchmark reporter.

Generates detailed Markdown reports suitable for documentation and progress logs.
"""

from __future__ import annotations

import platform
import time
from dataclasses import dataclass
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from ..bench_unified import BenchmarkReport, FeatureReport


@dataclass
class EnvironmentInfo:
    """System environment information."""

    os_name: str
    os_version: str
    cpu: str
    python_version: str
    strata_version: str
    timestamp: str
    seed: int | str


# ---------------------------------------------------------------------------
# Helpers: build aligned Markdown tables from rows of cell values
# ---------------------------------------------------------------------------

# Alignment per column: "l" = left, "r" = right
_Align = str  # "l" or "r"


def _build_table(
    headers: list[str],
    rows: list[list[str]],
    aligns: list[_Align] | None = None,
) -> list[str]:
    """Build a Markdown table with columns padded to equal width.

    Args:
        headers: Column header strings.
        rows: List of rows; each row is a list of cell strings (same length
              as *headers*).
        aligns: Per-column alignment ("l" or "r").  Defaults to left for the
                first column and right for the rest (typical for benchmark
                tables where the first column is a label).

    Returns:
        Lines of text (no trailing newline) ready to be joined with ``\\n``.
    """
    n_cols = len(headers)
    if aligns is None:
        aligns = ["l"] + ["r"] * (n_cols - 1)

    # Compute max width per column (at least as wide as header).
    widths = [len(h) for h in headers]
    for row in rows:
        for i, cell in enumerate(row):
            widths[i] = max(widths[i], len(cell))

    def _pad(text: str, width: int, align: _Align) -> str:
        if align == "r":
            return text.rjust(width)
        return text.ljust(width)

    # Header line
    header_line = "| " + " | ".join(
        _pad(h, widths[i], aligns[i]) for i, h in enumerate(headers)
    ) + " |"

    # Separator line (use :--- or ---: for alignment hints)
    sep_parts: list[str] = []
    for i in range(n_cols):
        dashes = "-" * widths[i]
        if aligns[i] == "r":
            sep_parts.append(dashes[:-1] + ":")
        else:
            sep_parts.append(":" + dashes[1:])
    sep_line = "| " + " | ".join(sep_parts) + " |"

    # Data rows
    data_lines: list[str] = []
    for row in rows:
        cells = " | ".join(
            _pad(row[i] if i < len(row) else "", widths[i], aligns[i])
            for i in range(n_cols)
        )
        data_lines.append(f"| {cells} |")

    return [header_line, sep_line, *data_lines]


def _fmt_size(size_bytes: int | float) -> str:
    """Human-readable size string: ``140 KB`` / ``1.04 MB`` / ``10.1 MB``."""
    kb = size_bytes / 1024
    if kb < 1024:
        return f"{kb:,.0f} KB"
    return f"{kb / 1024:,.2f} MB"


def _fmt_int(n: int | float) -> str:
    """Integer with thousand separators."""
    return f"{int(n):,}"


class MarkdownReporter:
    """Generate Markdown benchmark reports."""

    def __init__(self, env_info: EnvironmentInfo | None = None):
        """Initialize the reporter with optional environment info."""
        self.env_info = env_info or self._collect_environment()

    def _collect_environment(self) -> EnvironmentInfo:
        """Collect current system environment information."""
        try:
            import strata

            strata_version = strata.__version__
        except (ImportError, AttributeError):
            strata_version = "unknown"

        return EnvironmentInfo(
            os_name=platform.system(),
            os_version=platform.release(),
            cpu=platform.processor() or platform.machine(),
            python_version=platform.python_version(),
            strata_version=strata_version,
            timestamp=time.strftime("%Y-%m-%d %H:%M:%S"),
            seed="randomized",
        )

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def generate_report(self, report: BenchmarkReport) -> str:
        """Generate a complete Markdown report from benchmark results."""
        lines: list[str] = []

        # Header
        lines.append(f"### Benchmark Report — {self.env_info.timestamp}")
        lines.append("")

        # Environment section
        lines.append("#### Environment")
        lines.append(f"- **OS**: {self.env_info.os_name} {self.env_info.os_version}")
        lines.append(f"- **CPU**: {self.env_info.cpu}")
        lines.append(f"- **Python**: {self.env_info.python_version}")
        lines.append(f"- **Strata version**: {self.env_info.strata_version}")
        lines.append(f"- **Random seed**: {self.env_info.seed}")
        lines.append("")

        # Dataset characteristics
        if report.dataset_info:
            lines.append("#### Dataset Characteristics")
            ds_rows: list[list[str]] = []
            for ds in report.dataset_info:
                ds_rows.append([
                    ds.get("name", "unknown"),
                    _fmt_size(ds.get("size_bytes", 0)),
                    str(ds.get("depth", "-")),
                    _fmt_int(ds.get("keys", 0)),
                    _fmt_int(ds.get("values", 0)),
                    ds.get("complexity", "mixed"),
                ])
            lines.extend(_build_table(
                ["Dataset", "Size", "Depth", "Keys", "Values", "Types"],
                ds_rows,
                ["l", "r", "r", "r", "r", "l"],
            ))
            lines.append("")

        # Feature results
        for feature_name, feature_report in report.features.items():
            lines.extend(self._format_feature_report(feature_name, feature_report))

        # Summary
        lines.append("#### Summary")
        lines.append(self._generate_summary(report))
        lines.append("")

        return "\n".join(lines)

    # ------------------------------------------------------------------
    # Feature report formatting
    # ------------------------------------------------------------------

    def _format_feature_report(self, feature_name: str, report: FeatureReport) -> list[str]:
        """Format a single feature's benchmark results."""
        lines: list[str] = []

        # Feature header
        display_name = feature_name.replace("_", " ").title()
        lines.append(f"#### {display_name} Results")
        lines.append("")

        if not report.results:
            lines.append("*No results available*")
            lines.append("")
            return lines

        # ----------------------------------------------------------
        # Rank results within groups (dataset, or dataset+query)
        # ----------------------------------------------------------
        group_key = self._group_key_func(feature_name)

        by_group: dict[str, list[dict]] = {}
        for result in report.results:
            key = group_key(result)
            by_group.setdefault(key, []).append(result)

        for g_results in by_group.values():
            g_results.sort(key=lambda r: r.get("median_ms", float("inf")))
            for rank, result in enumerate(g_results, 1):
                result["rank"] = rank

        # Order by dataset size priority, then by rank
        size_order = {"tiny": 0, "small": 1, "medium": 2, "large": 3, "xlarge": 4}

        sorted_results = sorted(
            report.results,
            key=lambda r: (
                size_order.get(r.get("dataset", ""), 99),
                r.get("query", ""),
                r.get("rank", 999),
            ),
        )

        # ----------------------------------------------------------
        # Build the table
        # ----------------------------------------------------------
        if feature_name == "search":
            lines.extend(self._format_search_table(sorted_results))
        else:
            headers, aligns, rows = self._feature_table_data(feature_name, sorted_results)
            lines.extend(_build_table(headers, rows, aligns))

        lines.append("")

        # Speedup notes
        lines.extend(self._format_speedup_notes(feature_name, report))

        return lines

    @staticmethod
    def _group_key_func(feature_name: str):
        """Return a function that extracts the ranking-group key."""
        if feature_name == "search":
            return lambda r: (r.get("dataset", ""), r.get("query", ""))
        return lambda r: r.get("dataset", "default")

    # ----------------------------------------------------------
    # Table builders per feature type
    # ----------------------------------------------------------

    def _feature_table_data(
        self, feature_name: str, sorted_results: list[dict]
    ) -> tuple[list[str], list[_Align], list[list[str]]]:
        """Return (headers, aligns, rows) for non-search features."""

        if feature_name in ("loads", "loads_tape"):
            headers = ["Library", "Dataset", "Median (ms)", "Throughput (MB/s)", "RSS (MB)", "Rank"]
            aligns: list[_Align] = ["l", "l", "r", "r", "r", "r"]
            rows = []
            for r in sorted_results:
                size_bytes = r.get("input_size_bytes", 0)
                median_ms = r.get("median_ms", 0)
                throughput = (size_bytes / 1024 / 1024) / (median_ms / 1000) if median_ms > 0 else 0
                rows.append([
                    r.get("library", "?"),
                    r.get("dataset", "-"),
                    f"{median_ms:.2f}",
                    f"{throughput:.1f}",
                    f"{r.get('rss_mb', 0):.1f}",
                    f"#{r.get('rank', '-')}",
                ])
            return headers, aligns, rows

        if feature_name in ("dumps", "dumps_bytes"):
            headers = ["Library", "Dataset", "Median (ms)", "Size", "Throughput (MB/s)", "Rank"]
            aligns = ["l", "l", "r", "r", "r", "r"]
            rows = []
            for r in sorted_results:
                output_size = r.get("output_size", 0)
                median_ms = r.get("median_ms", 0)
                throughput = (output_size / 1024 / 1024) / (median_ms / 1000) if median_ms > 0 else 0
                rows.append([
                    r.get("library", "?"),
                    r.get("dataset", "-"),
                    f"{median_ms:.2f}",
                    _fmt_int(output_size),
                    f"{throughput:.1f}",
                    f"#{r.get('rank', '-')}",
                ])
            return headers, aligns, rows

        if feature_name in ("ndjson", "iter_ndjson"):
            headers = ["Library", "Dataset", "Median (ms)", "Lines", "Lines/sec", "Rank"]
            aligns = ["l", "l", "r", "r", "r", "r"]
            rows = []
            for r in sorted_results:
                lines_count = r.get("lines_parsed", 0)
                median_ms = r.get("median_ms", 0)
                lines_per_sec = lines_count / (median_ms / 1000) if median_ms > 0 else 0
                rows.append([
                    r.get("library", "?"),
                    r.get("dataset", "-"),
                    f"{median_ms:.2f}",
                    _fmt_int(lines_count),
                    _fmt_int(lines_per_sec),
                    f"#{r.get('rank', '-')}",
                ])
            return headers, aligns, rows

        # cursor / mmap / generic
        headers = ["Library", "Dataset", "Median (ms)", "RSS (MB)", "Rank"]
        aligns = ["l", "l", "r", "r", "r"]
        rows = []
        for r in sorted_results:
            rows.append([
                r.get("library", "?"),
                r.get("dataset", "-"),
                f"{r.get('median_ms', 0):.2f}",
                f"{r.get('rss_mb', 0):.1f}",
                f"#{r.get('rank', '-')}",
            ])
        return headers, aligns, rows

    # ----------------------------------------------------------
    # Search: one sub-table per dataset, grouped by query
    # ----------------------------------------------------------

    def _format_search_table(self, sorted_results: list[dict]) -> list[str]:
        """Format search results as per-dataset sub-tables for readability."""
        lines: list[str] = []

        # Group by dataset
        size_order = {"tiny": 0, "small": 1, "medium": 2, "large": 3, "xlarge": 4}
        by_dataset: dict[str, list[dict]] = {}
        for r in sorted_results:
            by_dataset.setdefault(r.get("dataset", "?"), []).append(r)

        datasets_sorted = sorted(by_dataset.keys(), key=lambda d: size_order.get(d, 99))

        for ds in datasets_sorted:
            ds_results = by_dataset[ds]

            # Further group by query, preserving insertion order
            by_query: dict[str, list[dict]] = {}
            for r in ds_results:
                by_query.setdefault(r.get("query", "?"), []).append(r)

            # Build rows with blank-line separators between queries
            headers = ["Library", "Query", "Median (ms)", "Results", "Rank"]
            aligns: list[_Align] = ["l", "l", "r", "r", "r"]
            rows: list[list[str]] = []
            for query, q_results in by_query.items():
                q_results.sort(key=lambda r: r.get("median_ms", float("inf")))
                for r in q_results:
                    rows.append([
                        r.get("library", "?"),
                        query,
                        f"{r.get('median_ms', 0):.2f}",
                        _fmt_int(r.get("result_count", 0)),
                        f"#{r.get('rank', '-')}",
                    ])

            lines.append(f"**Dataset: {ds}**")
            lines.append("")
            lines.extend(_build_table(headers, rows, aligns))
            lines.append("")

        return lines

    # ----------------------------------------------------------
    # Speedup notes
    # ----------------------------------------------------------

    def _format_speedup_notes(self, feature_name: str, report: FeatureReport) -> list[str]:
        """Generate speedup comparison notes."""
        lines: list[str] = []

        strata_results = [r for r in report.results if r.get("library") == "strata"]
        if not strata_results:
            return lines

        for strata_result in strata_results:
            dataset = strata_result.get("dataset", "default")
            strata_median = strata_result.get("median_ms", 0)
            rank = strata_result.get("rank", 0)
            query = strata_result.get("query")

            # Find comparison results for same dataset (and query for search)
            if feature_name == "search" and query:
                others = [
                    r
                    for r in report.results
                    if r.get("dataset") == dataset
                    and r.get("query") == query
                    and r.get("library") != "strata"
                ]
                label = f"{dataset} ({query})"
            else:
                others = [
                    r
                    for r in report.results
                    if r.get("dataset") == dataset and r.get("library") != "strata"
                ]
                label = dataset

            if not others:
                continue

            fastest_other = min(others, key=lambda r: r.get("median_ms", float("inf")))
            fastest_median = fastest_other.get("median_ms", 0)
            fastest_lib = fastest_other.get("library", "unknown")

            if strata_median > 0 and fastest_median > 0:
                if strata_median < fastest_median:
                    speedup = (fastest_median / strata_median - 1) * 100
                    lines.append(
                        f"- **{label}**: Strata #{rank}, {speedup:.1f}% faster than {fastest_lib}"
                    )
                else:
                    slowdown = (strata_median / fastest_median - 1) * 100
                    lines.append(
                        f"- **{label}**: Strata #{rank}, {slowdown:.1f}% behind {fastest_lib}"
                    )

        if lines:
            lines.insert(0, "**Strata Performance:**")
            lines.append("")

        return lines

    # ----------------------------------------------------------
    # Summary
    # ----------------------------------------------------------

    def _generate_summary(self, report: BenchmarkReport) -> str:
        """Generate overall summary statistics."""
        total_benchmarks = 0
        strata_wins = 0
        best_categories: list[str] = []

        for feature_name, feature_report in report.features.items():
            by_dataset: dict[str, list[dict]] = {}
            for result in feature_report.results:
                ds = result.get("dataset", "default")
                by_dataset.setdefault(ds, []).append(result)

            for ds, ds_results in by_dataset.items():
                if not ds_results:
                    continue
                total_benchmarks += 1

                winner = min(ds_results, key=lambda r: r.get("median_ms", float("inf")))
                if winner.get("library") == "strata":
                    strata_wins += 1
                    best_categories.append(f"{feature_name}/{ds}")

        summary_lines = [
            f"- **Strata wins**: {strata_wins}/{total_benchmarks} benchmarks",
        ]

        if best_categories:
            summary_lines.append(f"- **Best in**: {', '.join(best_categories[:5])}")
            if len(best_categories) > 5:
                summary_lines.append(f"  ...and {len(best_categories) - 5} more")

        return "\n".join(summary_lines)

    # ----------------------------------------------------------
    # Progress log entry
    # ----------------------------------------------------------

    def generate_progress_log_entry(self, report: BenchmarkReport) -> str:
        """Generate a condensed entry suitable for progress_log.md."""
        out: list[str] = []

        out.append(f"### {self.env_info.timestamp}")
        out.append("")
        out.append(
            f"**Environment**: {self.env_info.os_name} {self.env_info.os_version}, "
            f"Python {self.env_info.python_version}, Strata {self.env_info.strata_version}"
        )
        out.append(f"**Seed**: {self.env_info.seed}")
        out.append("")

        headers = ["Feature", "Dataset", "Strata (ms)", "Best Other", "Speedup"]
        aligns: list[_Align] = ["l", "l", "r", "l", "r"]
        rows: list[list[str]] = []

        for feature_name, feature_report in report.features.items():
            for result in feature_report.results:
                if result.get("library") != "strata":
                    continue

                dataset = result.get("dataset", "default")
                strata_ms = result.get("median_ms", 0)
                query = result.get("query")

                if feature_name == "search" and query:
                    others = [
                        r
                        for r in feature_report.results
                        if r.get("dataset") == dataset
                        and r.get("query") == query
                        and r.get("library") != "strata"
                    ]
                    label = f"{dataset} ({query})"
                else:
                    others = [
                        r
                        for r in feature_report.results
                        if r.get("dataset") == dataset and r.get("library") != "strata"
                    ]
                    label = dataset

                if others:
                    best_other = min(others, key=lambda r: r.get("median_ms", float("inf")))
                    best_ms = best_other.get("median_ms", 0)
                    best_lib = best_other.get("library", "?")

                    if strata_ms > 0 and best_ms > 0:
                        if strata_ms < best_ms:
                            speedup = f"+{(best_ms / strata_ms - 1) * 100:.1f}%"
                        else:
                            speedup = f"-{(strata_ms / best_ms - 1) * 100:.1f}%"
                    else:
                        speedup = "-"

                    rows.append([
                        feature_name,
                        label,
                        f"{strata_ms:.2f}",
                        f"{best_lib} ({best_ms:.2f})",
                        speedup,
                    ])
                else:
                    rows.append([feature_name, label, f"{strata_ms:.2f}", "-", "-"])

        out.extend(_build_table(headers, rows, aligns))
        out.append("")
        out.append("---")
        out.append("")

        return "\n".join(out)

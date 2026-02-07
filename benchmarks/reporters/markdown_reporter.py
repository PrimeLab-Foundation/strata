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

    def generate_report(self, report: BenchmarkReport) -> str:
        """Generate a complete Markdown report from benchmark results."""
        lines = []

        # Header
        lines.append(f"### Benchmark Report - {self.env_info.timestamp}")
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
            lines.append("| Dataset | Size | Depth | Keys | Values | Types |")
            lines.append("|---------|------|-------|------|--------|-------|")
            for ds in report.dataset_info:
                size_kb = ds.get("size_bytes", 0) / 1024
                size_str = f"{size_kb:.0f}KB" if size_kb < 1024 else f"{size_kb/1024:.1f}MB"
                types_str = ds.get("complexity", "mixed")
                lines.append(
                    f"| {ds.get('name', 'unknown')} | {size_str} | "
                    f"{ds.get('depth', '-')} | {ds.get('keys', '-')} | "
                    f"{ds.get('values', '-')} | {types_str} |"
                )
            lines.append("")

        # Feature results
        for feature_name, feature_report in report.features.items():
            lines.extend(self._format_feature_report(feature_name, feature_report))

        # Summary
        lines.append("#### Summary")
        lines.append(self._generate_summary(report))
        lines.append("")

        return "\n".join(lines)

    def _format_feature_report(self, feature_name: str, report: FeatureReport) -> list[str]:
        """Format a single feature's benchmark results."""
        lines = []

        # Feature header
        display_name = feature_name.replace("_", " ").title()
        lines.append(f"#### {display_name} Results")
        lines.append("")

        if not report.results:
            lines.append("*No results available*")
            lines.append("")
            return lines

        # Determine columns based on feature type
        if feature_name in ("loads", "loads_tape"):
            lines.append(
                "| Library | Dataset | Median (ms) | Throughput (MB/s) | RSS (MB) | Rank |"
            )
            lines.append("|---------|---------|-------------|-------------------|----------|------|")
        elif feature_name in ("dumps", "dumps_bytes"):
            lines.append(
                "| Library | Dataset | Median (ms) | Size (bytes) | Throughput (MB/s) | Rank |"
            )
            lines.append("|---------|---------|-------------|--------------|-------------------|------|")
        elif feature_name in ("ndjson", "iter_ndjson"):
            lines.append(
                "| Library | Dataset | Median (ms) | Lines | Lines/sec | Rank |"
            )
            lines.append("|---------|---------|-------------|-------|-----------|------|")
        elif feature_name == "jsonpath":
            lines.append(
                "| Library | Query | Dataset | Median (ms) | Results | Rank |"
            )
            lines.append("|---------|-------|---------|-------------|---------|------|")
        else:
            lines.append(
                "| Library | Dataset | Median (ms) | RSS (MB) | Rank |"
            )
            lines.append("|---------|---------|-------------|----------|------|")

        # Sort by median time to assign ranks
        sorted_results = sorted(
            report.results, key=lambda r: (r.get("dataset", ""), r.get("median_ms", float("inf")))
        )

        # Group by dataset for ranking
        by_dataset: dict[str, list[dict]] = {}
        for result in sorted_results:
            ds = result.get("dataset", "default")
            by_dataset.setdefault(ds, []).append(result)

        # Assign ranks within each dataset
        for ds_results in by_dataset.values():
            ds_results.sort(key=lambda r: r.get("median_ms", float("inf")))
            for rank, result in enumerate(ds_results, 1):
                result["rank"] = rank

        # Format rows
        for result in sorted_results:
            lines.append(self._format_result_row(feature_name, result))

        lines.append("")

        # Add speedup notes
        lines.extend(self._format_speedup_notes(feature_name, report))

        return lines

    def _format_result_row(self, feature_name: str, result: dict) -> str:
        """Format a single result row based on feature type."""
        library = result.get("library", "unknown")
        dataset = result.get("dataset", "-")
        median_ms = result.get("median_ms", 0)
        rss_mb = result.get("rss_mb", 0)
        rank = result.get("rank", "-")

        if feature_name in ("loads", "loads_tape"):
            size_bytes = result.get("input_size_bytes", 0)
            throughput = (size_bytes / 1024 / 1024) / (median_ms / 1000) if median_ms > 0 else 0
            return f"| {library} | {dataset} | {median_ms:.2f} | {throughput:.1f} | {rss_mb:.1f} | #{rank} |"

        elif feature_name in ("dumps", "dumps_bytes"):
            output_size = result.get("output_size", 0)
            throughput = (output_size / 1024 / 1024) / (median_ms / 1000) if median_ms > 0 else 0
            return f"| {library} | {dataset} | {median_ms:.2f} | {output_size} | {throughput:.1f} | #{rank} |"

        elif feature_name in ("ndjson", "iter_ndjson"):
            lines_count = result.get("lines_parsed", 0)
            lines_per_sec = lines_count / (median_ms / 1000) if median_ms > 0 else 0
            return f"| {library} | {dataset} | {median_ms:.2f} | {lines_count} | {lines_per_sec:.0f} | #{rank} |"

        elif feature_name == "jsonpath":
            query = result.get("query", "-")
            result_count = result.get("result_count", 0)
            return f"| {library} | {query} | {dataset} | {median_ms:.2f} | {result_count} | #{rank} |"

        else:
            return f"| {library} | {dataset} | {median_ms:.2f} | {rss_mb:.1f} | #{rank} |"

    def _format_speedup_notes(self, feature_name: str, report: FeatureReport) -> list[str]:
        """Generate speedup comparison notes."""
        lines = []

        # Find strata results and compare to others
        strata_results = [r for r in report.results if r.get("library") == "strata"]
        if not strata_results:
            return lines

        for strata_result in strata_results:
            dataset = strata_result.get("dataset", "default")
            strata_median = strata_result.get("median_ms", 0)
            rank = strata_result.get("rank", 0)
            query = strata_result.get("query")  # For jsonpath comparisons

            # Find comparison results for same dataset (and query for jsonpath)
            if feature_name == "jsonpath" and query:
                # For jsonpath, match on both dataset AND query
                others = [
                    r
                    for r in report.results
                    if r.get("dataset") == dataset
                    and r.get("query") == query
                    and r.get("library") != "strata"
                ]
                label = f"{dataset} ({query})"
            else:
                # For other features, match on dataset only
                others = [
                    r
                    for r in report.results
                    if r.get("dataset") == dataset and r.get("library") != "strata"
                ]
                label = dataset

            if not others:
                continue

            # Find fastest non-strata
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

    def _generate_summary(self, report: BenchmarkReport) -> str:
        """Generate overall summary statistics."""
        total_benchmarks = 0
        strata_wins = 0
        best_categories: list[str] = []

        for feature_name, feature_report in report.features.items():
            # Group by dataset
            by_dataset: dict[str, list[dict]] = {}
            for result in feature_report.results:
                ds = result.get("dataset", "default")
                by_dataset.setdefault(ds, []).append(result)

            for ds, ds_results in by_dataset.items():
                if not ds_results:
                    continue
                total_benchmarks += 1

                # Find winner
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

    def generate_progress_log_entry(self, report: BenchmarkReport) -> str:
        """Generate a condensed entry suitable for progress_log.md."""
        lines = []

        # Header with date and commit
        lines.append(f"### {self.env_info.timestamp}")
        lines.append("")
        lines.append(f"**Environment**: {self.env_info.os_name} {self.env_info.os_version}, ")
        lines.append(f"Python {self.env_info.python_version}, Strata {self.env_info.strata_version}")
        lines.append(f"**Seed**: {self.env_info.seed}")
        lines.append("")

        # Condensed results table
        lines.append("| Feature | Dataset | Strata (ms) | Best Other | Speedup |")
        lines.append("|---------|---------|-------------|------------|---------|")

        for feature_name, feature_report in report.features.items():
            for result in feature_report.results:
                if result.get("library") != "strata":
                    continue

                dataset = result.get("dataset", "default")
                strata_ms = result.get("median_ms", 0)
                query = result.get("query")  # For jsonpath comparisons

                # Find best other (match on query too for jsonpath)
                if feature_name == "jsonpath" and query:
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
                            speedup = f"+{(best_ms/strata_ms - 1) * 100:.1f}%"
                        else:
                            speedup = f"-{(strata_ms/best_ms - 1) * 100:.1f}%"
                    else:
                        speedup = "-"

                    lines.append(
                        f"| {feature_name} | {label} | {strata_ms:.2f} | {best_lib} ({best_ms:.2f}) | {speedup} |"
                    )
                else:
                    lines.append(f"| {feature_name} | {label} | {strata_ms:.2f} | - | - |")

        lines.append("")
        lines.append("---")
        lines.append("")

        return "\n".join(lines)

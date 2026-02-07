"""
CSV benchmark reporter.

Generates CSV reports for spreadsheet analysis and data processing.
"""

from __future__ import annotations

import csv
import io
import platform
import time
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from ..bench_unified import BenchmarkReport


class CSVReporter:
    """Generate CSV benchmark reports."""

    def __init__(self, seed: int | str = "randomized"):
        """Initialize the reporter."""
        self.seed = seed
        self.timestamp = time.strftime("%Y-%m-%dT%H:%M:%S")

    def generate_report(self, report: BenchmarkReport) -> str:
        """Generate a complete CSV report from benchmark results."""
        output = io.StringIO()
        writer = csv.writer(output)

        # Header
        writer.writerow(
            [
                "timestamp",
                "feature",
                "library",
                "dataset",
                "min_ms",
                "median_ms",
                "mean_ms",
                "p95_ms",
                "p99_ms",
                "std_dev_ms",
                "rss_mb",
                "input_size_bytes",
                "output_size",
                "lines_parsed",
                "result_count",
                "throughput_mb_s",
                "ops_per_sec",
                "rank",
                "error",
            ]
        )

        # Data rows
        for feature_name, feature_report in report.features.items():
            # Calculate ranks within each dataset
            by_dataset: dict[str, list[dict]] = {}
            for result in feature_report.results:
                ds = result.get("dataset", "default")
                by_dataset.setdefault(ds, []).append(result)

            for ds_results in by_dataset.values():
                ds_results.sort(key=lambda r: r.get("median_ms", float("inf")))
                for rank, result in enumerate(ds_results, 1):
                    result["rank"] = rank

            # Write results
            for result in feature_report.results:
                median_ms = result.get("median_ms", 0)
                input_size = result.get("input_size_bytes", 0)
                output_size = result.get("output_size", 0)

                # Calculate throughput
                if median_ms > 0 and input_size > 0:
                    throughput = (input_size / 1024 / 1024) / (median_ms / 1000)
                elif median_ms > 0 and output_size > 0:
                    throughput = (output_size / 1024 / 1024) / (median_ms / 1000)
                else:
                    throughput = 0

                # Calculate ops/sec
                ops_per_sec = 1000 / median_ms if median_ms > 0 else 0

                writer.writerow(
                    [
                        self.timestamp,
                        feature_name,
                        result.get("library", "unknown"),
                        result.get("dataset", "default"),
                        result.get("min_ms", ""),
                        result.get("median_ms", ""),
                        result.get("mean_ms", ""),
                        result.get("p95_ms", ""),
                        result.get("p99_ms", ""),
                        result.get("std_dev_ms", ""),
                        result.get("rss_mb", ""),
                        input_size or "",
                        output_size or "",
                        result.get("lines_parsed", ""),
                        result.get("result_count", ""),
                        f"{throughput:.2f}" if throughput > 0 else "",
                        f"{ops_per_sec:.2f}" if ops_per_sec > 0 else "",
                        result.get("rank", ""),
                        result.get("error", ""),
                    ]
                )

            # Write errors
            for error in feature_report.errors:
                writer.writerow(
                    [
                        self.timestamp,
                        feature_name,
                        error.get("library", "unknown"),
                        error.get("dataset", "default"),
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        error.get("error", "Unknown error"),
                    ]
                )

        return output.getvalue()

    def generate_summary_csv(self, report: BenchmarkReport) -> str:
        """Generate a summary CSV with one row per feature/dataset combination."""
        output = io.StringIO()
        writer = csv.writer(output)

        # Header
        writer.writerow(
            [
                "timestamp",
                "os",
                "python_version",
                "strata_version",
                "seed",
                "feature",
                "dataset",
                "strata_median_ms",
                "best_other_library",
                "best_other_median_ms",
                "speedup_pct",
                "strata_rank",
                "total_libraries",
            ]
        )

        try:
            import strata

            strata_version = strata.__version__
        except (ImportError, AttributeError):
            strata_version = "unknown"

        for feature_name, feature_report in report.features.items():
            # Group by dataset
            by_dataset: dict[str, list[dict]] = {}
            for result in feature_report.results:
                ds = result.get("dataset", "default")
                by_dataset.setdefault(ds, []).append(result)

            for ds, ds_results in by_dataset.items():
                if not ds_results:
                    continue

                # Sort by median time
                ds_results.sort(key=lambda r: r.get("median_ms", float("inf")))

                # Find strata result
                strata_result = next(
                    (r for r in ds_results if r.get("library") == "strata"), None
                )
                if not strata_result:
                    continue

                strata_median = strata_result.get("median_ms", 0)
                strata_rank = ds_results.index(strata_result) + 1

                # Find best non-strata
                others = [r for r in ds_results if r.get("library") != "strata"]
                if others:
                    best_other = others[0]  # Already sorted
                    best_other_lib = best_other.get("library", "")
                    best_other_median = best_other.get("median_ms", 0)

                    if strata_median > 0 and best_other_median > 0:
                        speedup = (best_other_median / strata_median - 1) * 100
                    else:
                        speedup = 0
                else:
                    best_other_lib = ""
                    best_other_median = 0
                    speedup = 0

                writer.writerow(
                    [
                        self.timestamp,
                        platform.system(),
                        platform.python_version(),
                        strata_version,
                        self.seed,
                        feature_name,
                        ds,
                        strata_median,
                        best_other_lib,
                        best_other_median,
                        f"{speedup:.2f}",
                        strata_rank,
                        len(ds_results),
                    ]
                )

        return output.getvalue()

    def generate_timeseries_row(self, report: BenchmarkReport) -> str:
        """
        Generate a single-row CSV suitable for appending to a timeseries file.

        Format: timestamp, then strata median for each feature/dataset combo.
        """
        output = io.StringIO()
        writer = csv.writer(output)

        values = [self.timestamp]

        for feature_name, feature_report in report.features.items():
            for result in feature_report.results:
                if result.get("library") != "strata":
                    continue
                dataset = result.get("dataset", "default")
                median = result.get("median_ms", "")
                values.append(f"{feature_name}_{dataset}:{median}")

        writer.writerow(values)
        return output.getvalue()

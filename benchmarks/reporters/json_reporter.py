"""
JSON benchmark reporter.

Generates machine-readable JSON reports for CI/CD integration and analysis.
"""

from __future__ import annotations

import json
import platform
import time
from typing import TYPE_CHECKING, Any

if TYPE_CHECKING:
    from ..bench_unified import BenchmarkReport


class JSONReporter:
    """Generate JSON benchmark reports."""

    def __init__(self, seed: int | str = "randomized"):
        """Initialize the reporter."""
        self.seed = seed
        self.timestamp = time.strftime("%Y-%m-%dT%H:%M:%S")

    def _collect_environment(self) -> dict[str, Any]:
        """Collect system environment information."""
        try:
            import strata

            strata_version = strata.__version__
        except (ImportError, AttributeError):
            strata_version = "unknown"

        return {
            "os": platform.system(),
            "os_version": platform.release(),
            "cpu": platform.processor() or platform.machine(),
            "python_version": platform.python_version(),
            "strata_version": strata_version,
            "timestamp": self.timestamp,
            "seed": self.seed,
        }

    def generate_report(self, report: BenchmarkReport) -> str:
        """Generate a complete JSON report from benchmark results."""
        output = {
            "metadata": {
                "format_version": "1.0",
                "generator": "strata_unified_benchmark",
                "timestamp": self.timestamp,
            },
            "environment": self._collect_environment(),
            "datasets": report.dataset_info,
            "features": {},
            "summary": self._generate_summary(report),
        }

        # Add feature results
        for feature_name, feature_report in report.features.items():
            output["features"][feature_name] = {
                "results": feature_report.results,
                "errors": feature_report.errors,
                "metadata": {
                    "total_runs": len(feature_report.results),
                    "libraries_tested": list(
                        set(r.get("library", "unknown") for r in feature_report.results)
                    ),
                },
            }

        return json.dumps(output, indent=2, ensure_ascii=False)

    def _generate_summary(self, report: BenchmarkReport) -> dict[str, Any]:
        """Generate summary statistics."""
        total_benchmarks = 0
        strata_wins = 0
        regressions: list[dict] = []

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

                # Sort by median time
                ds_results.sort(key=lambda r: r.get("median_ms", float("inf")))

                # Check if strata won
                winner = ds_results[0]
                if winner.get("library") == "strata":
                    strata_wins += 1

                # Check for regressions (if baseline info is available)
                strata_result = next(
                    (r for r in ds_results if r.get("library") == "strata"), None
                )
                if strata_result and strata_result.get("baseline_ms"):
                    current = strata_result.get("median_ms", 0)
                    baseline = strata_result.get("baseline_ms", 0)
                    if baseline > 0 and current > baseline * 1.05:
                        regressions.append(
                            {
                                "feature": feature_name,
                                "dataset": ds,
                                "baseline_ms": baseline,
                                "current_ms": current,
                                "regression_pct": (current / baseline - 1) * 100,
                            }
                        )

        return {
            "total_benchmarks": total_benchmarks,
            "strata_wins": strata_wins,
            "win_rate": strata_wins / total_benchmarks if total_benchmarks > 0 else 0,
            "regressions": regressions,
            "has_regressions": len(regressions) > 0,
        }

    def generate_comparison(
        self,
        current_report: BenchmarkReport,
        baseline_report: BenchmarkReport,
    ) -> str:
        """Generate a comparison report between current and baseline runs."""
        comparisons = []

        for feature_name, feature_report in current_report.features.items():
            baseline_feature = baseline_report.features.get(feature_name)
            if not baseline_feature:
                continue

            for result in feature_report.results:
                if result.get("library") != "strata":
                    continue

                dataset = result.get("dataset", "default")
                current_ms = result.get("median_ms", 0)

                # Find baseline result
                baseline_result = next(
                    (
                        r
                        for r in baseline_feature.results
                        if r.get("library") == "strata" and r.get("dataset") == dataset
                    ),
                    None,
                )

                if baseline_result:
                    baseline_ms = baseline_result.get("median_ms", 0)
                    if baseline_ms > 0:
                        change_pct = (current_ms / baseline_ms - 1) * 100
                        comparisons.append(
                            {
                                "feature": feature_name,
                                "dataset": dataset,
                                "baseline_ms": baseline_ms,
                                "current_ms": current_ms,
                                "change_pct": change_pct,
                                "improved": change_pct < 0,
                                "regressed": change_pct > 5,
                            }
                        )

        output = {
            "comparison": {
                "timestamp": self.timestamp,
                "results": comparisons,
                "total_comparisons": len(comparisons),
                "improvements": sum(1 for c in comparisons if c["improved"]),
                "regressions": sum(1 for c in comparisons if c["regressed"]),
                "neutral": sum(
                    1 for c in comparisons if not c["improved"] and not c["regressed"]
                ),
            }
        }

        return json.dumps(output, indent=2)

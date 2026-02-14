"""CSV reporter for unified benchmark results."""

from __future__ import annotations

import csv
import io
from typing import Any


def generate_report(ctx: dict[str, Any]) -> str:
    output = io.StringIO()
    writer = csv.writer(output)

    writer.writerow(
        [
            "category",
            "library",
            "dataset",
            "variant",
            "min_ms",
            "median_ms",
            "p95_ms",
            "throughput_mbps",
            "rss_mb",
            "result_count",
            "output_size",
            "error",
        ]
    )

    for r in ctx.get("results", []):
        writer.writerow(
            [
                r.category,
                r.library,
                r.dataset,
                r.variant,
                getattr(r, "min_ms", 0),
                getattr(r, "median_ms", 0),
                getattr(r, "p95_ms", 0),
                getattr(r, "throughput_mbps", 0),
                getattr(r, "rss_mb", 0),
                getattr(r, "result_count", 0),
                getattr(r, "output_size", 0),
                getattr(r, "error", ""),
            ]
        )

    return output.getvalue()

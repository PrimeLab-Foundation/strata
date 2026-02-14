"""JSON reporter for unified benchmark results."""

from __future__ import annotations

import json
from typing import Any


def generate_report(ctx: dict[str, Any]) -> str:
    """Return machine-readable JSON for BenchSuite results."""
    payload = {
        "metadata": {
            "format_version": "2.0",
            "generator": "bench_suite",
            "label": ctx.get("label"),
            "generated": ctx.get("generated"),
        },
        "environment": ctx.get("environment", {}),
        "config": ctx.get("config", {}),
        "datasets": ctx.get("datasets", {}),
        "results": [r.to_dict() if hasattr(r, "to_dict") else r for r in ctx.get("results", [])],
    }
    return json.dumps(payload, indent=2, ensure_ascii=False)

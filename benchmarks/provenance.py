"""Versioned, full-precision companions to the readable benchmark report."""

from __future__ import annotations

import hashlib
import importlib.metadata
import json
import math
import platform
import re
import sys
from dataclasses import asdict
from pathlib import Path

from benchmarks.harness import Report, render_report, summarize
from scripts.build_identity import file_hash, source_identity


def capture(datasets: list[Path], libraries: dict, *, repeat: int, warmup: int) -> dict:
    import strata._strata as extension

    binary = Path(extension.__file__).resolve()
    identity_path = binary.with_name(binary.name + ".build.json")
    build = json.loads(identity_path.read_text()) if identity_path.is_file() else None
    binary_hash = file_hash(binary)
    if build is not None and build.get("extension_sha256") != binary_hash:
        raise ValueError("build identity does not match the installed extension")
    versions = {}
    for name in libraries:
        distribution = {"simdjson": "pysimdjson", "jsonpath_ng": "jsonpath-ng"}.get(name, name)
        try:
            versions[name] = importlib.metadata.version(distribution)
        except importlib.metadata.PackageNotFoundError:
            versions[name] = platform.python_version() if name == "json" else None
    return {
        "source": source_identity(Path(__file__).resolve().parents[1]),
        "extension": {"path": str(binary), "sha256": binary_hash, "build": build},
        "python": {"executable": sys.executable, "version": sys.version},
        "versions": versions,
        "datasets": [
            {"name": path.name, "path": str(path.resolve()), "sha256": file_hash(path)}
            for path in datasets
        ],
        "protocol": {
            "name": "canonical-interleaved-v1",
            "repeat": repeat,
            "warmup": warmup,
            "preflight_calls": 1,
            "equivalence_calls_per_library": {"query": 1, "search": 1},
            "gc": "collect before each timed call",
            "timer": "perf_counter",
            "units": "ms",
            "rss": "current whole-process RSS after all libraries in each row",
        },
        "limitations": [] if build else ["legacy extension: build provenance unavailable"],
    }


def companion(path: Path) -> Path:
    return path.with_suffix(".json")


def packet(report: Report, markdown: str) -> dict:
    # Measurements preserve library insertion order. The timer runs one sample
    # of each library in that order per round, with no randomization.
    groups: dict[tuple[str, str], list] = {}
    for measurement in report.measurements:
        groups.setdefault((measurement.section, measurement.dataset), []).append(measurement)
    samples = []
    for group in groups.values():
        for round_id in range(max((len(m.samples_ms) for m in group), default=0)):
            for measurement in group:
                if round_id < len(measurement.samples_ms):
                    samples.append(
                        {
                            "order_id": len(samples),
                            "round_id": round_id,
                            "section": measurement.section,
                            "dataset": measurement.dataset,
                            "library": measurement.library,
                            "elapsed_ms": measurement.samples_ms[round_id],
                        }
                    )
    return {
        "schema_version": 1,
        "report_sha256": hashlib.sha256(markdown.encode()).hexdigest(),
        "name": report.name,
        "environment": report.environment,
        "excluded": report.excluded,
        "provenance": report.provenance,
        "measurements": [asdict(m) for m in report.measurements],
        "samples": samples,
    }


def write_report(path: Path, report: Report) -> None:
    report.environment["provenance_schema"] = "1"
    build = report.provenance.get("extension", {}).get("build")
    if build and build.get("source") and build["source"].get("commit"):
        report.environment["commit"] = build["source"]["commit"]
    markdown = render_report(report)
    content = json.dumps(packet(report, markdown), indent=2, allow_nan=False) + "\n"
    path.parent.mkdir(parents=True, exist_ok=True)
    companion(path).write_text(content, encoding="utf-8", newline="\n")
    path.write_text(markdown, encoding="utf-8", newline="\n")


def validate_companion(path: Path, markdown: str) -> dict | None:
    """Legacy absence is explicit; a present but corrupt companion is fatal."""
    sidecar = companion(path)
    if not sidecar.is_file():
        from benchmarks.harness import parse_report

        if "provenance_schema" in parse_report(markdown).environment:
            raise ValueError("required provenance companion is missing")
        return None
    try:
        data = json.loads(sidecar.read_text(encoding="utf-8"))
        if data["schema_version"] != 1:
            raise ValueError("unsupported provenance schema")
        if data["environment"].get("provenance_schema") != "1":
            raise ValueError("report and companion schema markers disagree")
        if data["report_sha256"] != hashlib.sha256(markdown.encode()).hexdigest():
            raise ValueError("provenance report hash mismatch")
        identity = data["provenance"]["extension"]
        build = identity["build"]
        if build is not None:
            if build.get("schema_version") != 1:
                raise ValueError("unsupported build identity schema")
            if build["extension_sha256"] != identity["sha256"]:
                raise ValueError("provenance binary hash mismatch")
            source = build.get("source")
            commit = data["environment"].get("commit", "")
            if (
                source
                and source.get("dirty") is False
                and not re.fullmatch("[0-9a-f]{40}", source.get("commit") or "")
            ):
                raise ValueError("clean build source has no valid commit")
            if (
                source
                and source.get("commit")
                and (not commit or not source["commit"].startswith(commit))
            ):
                raise ValueError("report commit does not match measured binary source")
        from benchmarks.harness import Measurement, parse_report

        parsed = parse_report(markdown)
        if data["environment"] != parsed.environment:
            # Legacy parser also reads exclusion bullets as environment.
            expected_environment = dict(data["environment"], **data["excluded"])
            if expected_environment != parsed.environment:
                raise ValueError("provenance environment mismatch")
        rows = [Measurement(**row) for row in data["measurements"]]
        check = Report(
            data["name"], data["environment"], rows, data["excluded"], provenance=data["provenance"]
        )
        if render_report(check) != markdown:
            raise ValueError("provenance measurements do not render the report")
        for row in rows:
            if row.failed:
                continue
            values = row.samples_ms
            if len(values) != data["provenance"]["protocol"]["repeat"]:
                raise ValueError("provenance sample count mismatch")
            if not values or any(not math.isfinite(v) or v <= 0 for v in values):
                raise ValueError("invalid provenance timing")
            calculated = summarize(row.section, row.dataset, row.library, list(values), row.rss_mb)
            if (calculated.min_ms, calculated.median_ms, calculated.p95_ms) != (
                row.min_ms,
                row.median_ms,
                row.p95_ms,
            ):
                raise ValueError("provenance aggregates do not match samples")
        if packet(check, markdown)["samples"] != data["samples"]:
            raise ValueError("provenance sample order mismatch")
        return data
    except (KeyError, TypeError, AttributeError, OverflowError, json.JSONDecodeError) as error:
        raise ValueError(f"malformed provenance: {sidecar}") from error

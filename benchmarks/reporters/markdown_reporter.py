"""Markdown reporter for BenchSuite unified benchmarks."""

from __future__ import annotations

import time
from typing import Any, Iterable

from ..bench_search import NDJSON_QUERIES, QUERIES
from ..markdown_tables import build_markdown_table


def _table(headers: list[str], rows: list[list[str]]) -> list[str]:
    return build_markdown_table(headers, rows)


def _summary(results: Iterable[Any]) -> list[str]:
    lines: list[str] = []

    def best(cat: str, variant: str | None = None) -> str:
        subset = [
            r
            for r in results
            if r.category == cat and (variant is None or r.variant == variant) and not r.error
        ]
        if not subset:
            return "n/a"
        winner = min(subset, key=lambda r: r.median_ms or float("inf"))
        return f"{winner.library} ({winner.median_ms:.2f} ms)"

    lines.append(f"- loads json: {best('loads', 'json')}")
    lines.append(f"- loads ndjson: {best('loads', 'ndjson')}")
    lines.append(f"- dumps str: {best('dumps', 'str')}")
    lines.append(f"- dumps bytes: {best('dumps', 'bytes')}")
    lines.append(f"- search: {best('search')}")
    lines.append(f"- cursor reuse: {best('cursor_reuse')}")

    # Gaps for Rule 13
    gap_lines: list[str] = []
    for cat, variant in [("loads", "json"), ("loads", "ndjson"), ("dumps", "str"), ("dumps", "bytes")]:
        subset = [
            r
            for r in results
            if r.category == cat and r.variant == variant and not r.error
        ]
        strata = next((r for r in subset if r.library == "strata"), None)
        if not subset or not strata:
            continue
        fastest = min(subset, key=lambda r: r.median_ms or float("inf"))
        if fastest.library == "strata":
            continue
        gap = (strata.median_ms / fastest.median_ms - 1) * 100 if strata.median_ms and fastest.median_ms else 0
        gap_lines.append(f"- Strata gap in {cat}/{variant}: {gap:.1f}% behind {fastest.library}")
    if gap_lines:
        lines.append("- Rule 13: strata not #1 in: " + ", ".join([g.split(':')[0].split('in ')[1] for g in gap_lines]))
        lines.extend(gap_lines)

    return lines


def generate_report(ctx: dict[str, Any]) -> str:
    """Render Markdown for unified BenchmarkResult records."""

    label = ctx["label"]
    env = ctx["environment"]
    datasets: dict[str, Any] = ctx["datasets"]
    results: list[Any] = ctx["results"]

    lines: list[str] = []
    lines.append(f"# Strata Benchmark Results — {label.upper() if isinstance(label, str) else label}")
    lines.append(f"Generated: {ctx['generated']}")
    lines.append("")

    lines.append("## Environment")
    lines.append(f"- Commit: {env.get('commit', '')}")
    lines.append(f"- OS: {env.get('os', '')}")
    lines.append(f"- CPU: {env.get('cpu', '')}")
    lines.append(f"- Python: {env.get('python', '')}")
    lines.append(f"- Compiler: {env.get('compiler', '')}")
    lines.append("")

    cfg = ctx.get("config", {})
    lines.append("## Configuration")
    lines.append(f"- Repeat/Warmup (loads): {cfg.get('loads', {}).get('repeat')} / {cfg.get('loads', {}).get('warmup')}")
    lines.append(f"- Repeat/Warmup (dumps): {cfg.get('dumps', {}).get('repeat')} / {cfg.get('dumps', {}).get('warmup')}")
    lines.append(f"- Repeat/Warmup (search): {cfg.get('search', {}).get('repeat')} / {cfg.get('search', {}).get('warmup')}")
    lines.append(f"- Repeat/Warmup (cursor_reuse): {cfg.get('cursor_reuse', {}).get('repeat')} / {cfg.get('cursor_reuse', {}).get('warmup')}")
    lines.append("")

    lines.append("## Datasets")
    for key, meta in datasets.items():
        desc = f"{meta['path']} ({meta['size_mb']:.2f} MB, {meta['record_count']} records"
        if meta.get("line_count") is not None:
            desc += f", {meta['line_count']} lines"
        desc += ")"
        lines.append(f"- {key}: {desc}")
    lines.append("")

    # Helper to select records
    def select(cat: str, predicate=None) -> list[Any]:
        out: list[Any] = []
        for r in results:
            if r.category != cat:
                continue
            if predicate and not predicate(r):
                continue
            out.append(r)
        return out

    # Parsing
    lines.append("## Parsing Benchmarks (loads)")
    for label_name, variant in [("JSON", "json"), ("NDJSON", "ndjson")]:
        subset = select("loads", lambda r, v=variant: r.variant == v)
        if not subset:
            continue
        dataset_name = subset[0].dataset
        meta_key = "json" if variant == "json" else "ndjson"
        meta = datasets[meta_key]
        size_str = f"{meta['size_mb']:.2f} MB"
        extra = f", {meta['line_count']} lines" if meta.get("line_count") else ""
        lines.append(f"### {label_name} — {dataset_name} ({size_str}{extra})")
        rows: list[list[str]] = []
        for r in sorted(subset, key=lambda x: x.median_ms or float('inf')):
            rows.append([
                r.library,
                f"{r.min_ms:.2f}",
                f"{r.median_ms:.2f}",
                f"{r.p95_ms:.2f}",
                f"{r.throughput_mbps:.2f}",
                f"{r.rss_mb:.1f}",
            ])
        lines.extend(_table(["Library", "Min (ms)", "Median (ms)", "P95 (ms)", "MB/s", "RSS (MB)"], rows))
        lines.append("")

    # Serialization
    lines.append("## Serialization Benchmarks (dumps)")
    for variant in ("str", "bytes"):
        subset = select("dumps", lambda r, v=variant: r.variant == v)
        if not subset:
            continue
        meta = datasets["json"]
        lines.append(f"### dumps ({variant}) — {subset[0].dataset} ({meta['size_mb']:.2f} MB)")
        rows: list[list[str]] = []
        for r in sorted(subset, key=lambda x: x.median_ms or float('inf')):
            rows.append([
                r.library,
                f"{r.min_ms:.2f}",
                f"{r.median_ms:.2f}",
                f"{r.p95_ms:.2f}",
                f"{r.throughput_mbps:.2f}",
                str(r.output_size),
                f"{r.rss_mb:.1f}",
            ])
        lines.extend(_table(
            ["Library", "Min (ms)", "Median (ms)", "P95 (ms)", "MB/s", "Output (bytes)", "RSS (MB)"],
            rows,
        ))
        lines.append("")

    # Search
    lines.append("## Search Benchmarks (query)")
    for label_name, meta_key in [("JSON", "json"), ("NDJSON", "ndjson")]:
        subset = select("search", lambda r, mk=meta_key: r.dataset.endswith(".ndjson") if mk == "ndjson" else not r.dataset.endswith(".ndjson"))
        if not subset:
            continue
        meta = datasets[meta_key]
        size_str = f"{meta['size_mb']:.2f} MB"
        lines.append(f"### {label_name} — {subset[0].dataset} ({size_str}, {meta['record_count']} records, cursor mode)")
        query_map = NDJSON_QUERIES if meta_key == "ndjson" else QUERIES
        rows: list[list[str]] = []
        for r in sorted(subset, key=lambda x: (query_map.get(x.variant, {}).get('description', x.variant), x.median_ms or float('inf'))):
            desc = query_map.get(r.variant, {}).get("description", r.variant)
            rows.append([
                desc,
                r.library,
                f"{r.min_ms:.2f}",
                f"{r.median_ms:.2f}",
                f"{r.p95_ms:.2f}",
                f"{r.throughput_mbps:.2f}",
                str(r.result_count),
                f"{r.rss_mb:.1f}",
            ])
        lines.extend(_table(
            ["Query", "Library", "Min (ms)", "Median (ms)", "P95 (ms)", "MB/s", "Results", "RSS (MB)"],
            rows,
        ))
        lines.append("")

    # Cursor reuse
    lines.append("## Cursor Reuse")
    for label_name, meta_key in [("JSON", "json"), ("NDJSON", "ndjson")]:
        subset = select(
            "cursor_reuse",
            lambda r, mk=meta_key: r.dataset.endswith(".ndjson") if mk == "ndjson" else not r.dataset.endswith(".ndjson"),
        )
        if not subset:
            continue
        lines.append(f"### {label_name}")
        rows: list[list[str]] = []
        for r in sorted(subset, key=lambda x: x.median_ms or float('inf')):
            rows.append([
                r.library,
                f"{r.min_ms:.2f}",
                f"{r.median_ms:.2f}",
                f"{r.p95_ms:.2f}",
                f"{r.throughput_mbps:.2f}",
                f"{r.rss_mb:.1f}",
            ])
        lines.extend(_table(["Mode", "Min (ms)", "Median (ms)", "P95 (ms)", "MB/s", "RSS (MB)"], rows))
        reuse = next((r for r in subset if r.library == "strata_cursor_reuse"), None)
        reparse = next((r for r in subset if r.library == "strata_cursor_reparse"), None)
        if reuse and reparse and reuse.median_ms > 0:
            lines.append(f"- Speedup: {reparse.median_ms / reuse.median_ms:.2f}x")
        lines.append("")

    # Summary
    lines.append("## Summary")
    lines.extend(_summary(results))
    lines.append("")

    return "\n".join(lines)


def generate_progress_log_entry(ctx: dict[str, Any]) -> str:
    label = ctx["label"]
    results: list[Any] = ctx["results"]
    timestamp = ctx.get("generated", time.strftime("%Y-%m-%d %H:%M:%S"))
    lines = [f"### {timestamp} — {label}", ""]

    def best(cat: str, variant: str | None = None) -> str:
        subset = [
            r
            for r in results
            if r.category == cat and (variant is None or r.variant == variant) and not r.error
        ]
        if not subset:
            return "n/a"
        winner = min(subset, key=lambda r: r.median_ms or float("inf"))
        return f"{winner.library} ({winner.median_ms:.2f} ms)"

    lines.append(f"- loads json: {best('loads', 'json')}")
    lines.append(f"- loads ndjson: {best('loads', 'ndjson')}")
    lines.append(f"- dumps str: {best('dumps', 'str')}")
    lines.append(f"- dumps bytes: {best('dumps', 'bytes')}")
    lines.append(f"- search: {best('search')}")
    lines.append(f"- cursor reuse: {best('cursor_reuse')}")
    return "\n".join(lines)

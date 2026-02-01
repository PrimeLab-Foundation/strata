"""Tests for benchmarks/regression_check (parse, compare, save baseline)."""

from __future__ import annotations

import tempfile
from pathlib import Path

# Import from benchmarks (run from repo root or PYTHONPATH)
from benchmarks.regression_check import (
    check_regression,
    load_baseline,
    parse_bench_results_md,
    save_baseline,
)

SAMPLE_PARSING_MD = """
## Parsing Benchmarks

| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|---------|
| strata | users | 17.5 | 20.0 | 23.0 | 100.0 |
| strata | users.ndjson | 50.0 | 52.0 | 54.0 | 165.0 |
| orjson | users | 18.0 | 21.0 | 25.0 | 130.0 |
"""

SAMPLE_QUERY_MD = """
## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | strata | 0.5 | 5000 |
|  | jmespath | 1.2 | 5000 |
"""


def test_parse_bench_results_parsing() -> None:
    with tempfile.NamedTemporaryFile(mode="w", suffix=".md", delete=False) as f:
        f.write(SAMPLE_PARSING_MD)
        path = Path(f.name)
    try:
        parsing, query = parse_bench_results_md(path)
        assert len(parsing) == 3
        strata_rows = [r for r in parsing if r["library"] == "strata"]
        assert len(strata_rows) == 2
        assert strata_rows[0]["dataset"] == "users" and strata_rows[0]["median_ms"] == 20.0
        assert strata_rows[1]["dataset"] == "users.ndjson" and strata_rows[1]["rss_mb"] == 165.0
        assert len(query) == 0
    finally:
        path.unlink(missing_ok=True)


def test_parse_bench_results_query() -> None:
    with tempfile.NamedTemporaryFile(mode="w", suffix=".md", delete=False) as f:
        f.write(SAMPLE_PARSING_MD + "\n" + SAMPLE_QUERY_MD)
        path = Path(f.name)
    try:
        parsing, query = parse_bench_results_md(path)
        assert len(query) >= 2
        strata_q = [r for r in query if r["library"] == "strata"]
        assert len(strata_q) == 1
        assert strata_q[0]["min_ms"] == 0.5 and strata_q[0]["result_count"] == 5000
    finally:
        path.unlink(missing_ok=True)


def test_save_and_load_baseline() -> None:
    parsing = [
        {"library": "strata", "dataset": "users", "median_ms": 20.0, "rss_mb": 100.0},
        {"library": "strata", "dataset": "users.ndjson", "median_ms": 53.0, "rss_mb": 167.0},
    ]
    query: list[dict] = []
    with tempfile.TemporaryDirectory() as d:
        baseline_path = Path(d) / "baseline.json"
        save_baseline(baseline_path, parsing, query)
        assert baseline_path.is_file()
        loaded = load_baseline(baseline_path)
        assert loaded is not None
        assert "users" in loaded["parsing"]
        assert loaded["parsing"]["users"]["median_ms"] == 20.0
        assert loaded["parsing"]["users.ndjson"]["rss_mb"] == 167.0


def test_check_regression_pass() -> None:
    baseline = {
        "parsing": {
            "users": {"median_ms": 20.0, "rss_mb": 100.0},
            "users.ndjson": {"median_ms": 53.0, "rss_mb": 167.0},
        },
        "query": {},
    }
    current_parsing = [
        {"library": "strata", "dataset": "users", "median_ms": 20.5, "rss_mb": 101.0},
        {"library": "strata", "dataset": "users.ndjson", "median_ms": 54.0, "rss_mb": 168.0},
    ]
    ok, failures = check_regression(current_parsing, [], baseline, 5.0)
    assert ok, failures
    assert len(failures) == 0


def test_check_regression_fail_median() -> None:
    baseline = {
        "parsing": {"users": {"median_ms": 20.0, "rss_mb": 100.0}},
        "query": {},
    }
    current_parsing = [
        {"library": "strata", "dataset": "users", "median_ms": 22.0, "rss_mb": 100.0},
    ]
    ok, failures = check_regression(current_parsing, [], baseline, 5.0)
    assert not ok
    assert any("median" in f and "+10" in f for f in failures)


def test_check_regression_fail_rss() -> None:
    baseline = {
        "parsing": {"users": {"median_ms": 20.0, "rss_mb": 100.0}},
        "query": {},
    }
    current_parsing = [
        {"library": "strata", "dataset": "users", "median_ms": 20.0, "rss_mb": 110.0},
    ]
    ok, failures = check_regression(current_parsing, [], baseline, 5.0)
    assert not ok
    assert any("RSS" in f for f in failures)


def test_load_baseline_missing() -> None:
    with tempfile.TemporaryDirectory() as d:
        assert load_baseline(Path(d) / "nonexistent.json") is None

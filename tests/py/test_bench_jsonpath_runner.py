import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from benchmarks import bench_jsonpath


def test_bench_jsonpath_runner_no_errors(tmp_path, monkeypatch):
    """Run JSONPath benchmarks with minimal queries; no jmespath to avoid optional dep."""
    data = {
        "users": [
            {"id": 1, "orders": [{"items": [{"price": 10.5}, {"price": 2.0}]}]},
            {"id": 2, "orders": [{"items": [{"price": 4.25}]}]},
        ],
    }
    data_path = tmp_path / "users.json"
    data_path.write_text(json.dumps(data))

    monkeypatch.setattr(
        bench_jsonpath,
        "QUERIES",
        {
            "simple_ids": {
                "strata": "$.users[*].id",
                "jmespath": None,
                "description": "Extract all user IDs",
            },
            "double_wildcard": {
                "strata": "$.users[*].orders[*].items[*].price",
                "jmespath": None,
                "description": "Extract order item prices (double wildcard)",
            },
        },
    )
    monkeypatch.setattr(bench_jsonpath, "jmespath", None)

    results = bench_jsonpath.run_all(data_path, repeat=1, warmup=0)

    assert results
    assert all(r.error == "" for r in results)

    double_wildcard = [
        r for r in results if r.library == "strata" and r.query_name == "double_wildcard"
    ]
    assert double_wildcard
    assert double_wildcard[0].result_count > 0

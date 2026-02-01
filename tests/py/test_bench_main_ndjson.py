import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from benchmarks.bench_main import BenchmarkRunner


def test_bench_main_ndjson_parses(tmp_path):
    ndjson_path = tmp_path / "sample.ndjson"
    ndjson_path.write_text('{"a": 1}\n{"b": 2}\n')

    runner = BenchmarkRunner(
        datasets=[str(ndjson_path)],
        repeat=1,
        warmup=0,
        strict_missing=False,
    )

    runner.run_all()

    assert runner.results
    assert all(result.error == "" for result in runner.results)
    assert any(result.dataset.endswith(".ndjson") for result in runner.results)
    assert all(result.times_ms for result in runner.results)

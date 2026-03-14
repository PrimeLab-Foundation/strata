"""
Strata benchmarks.

Standalone scripts (run with: python -m benchmarks.<name>):

- bench_loads:  JSON parsing (loads) — strata vs orjson, ujson, msgspec, stdlib.
- bench_dumps:  JSON serialization (dumps) — same competitors.
- bench_ndjson: NDJSON parsing (per-line loads).
- bench_jsonpath: JSONPath query/search performance.
- bench_random: Random schema benchmarks.

Orchestrator (parse + NDJSON + query, single report):

- bench_main:  Full suite; writes docs/benchmarks/bench_results.md.

Shared:

- harness:     Timing, RSS, run_single_benchmark.
- eval_queries: Query helpers for bench_main.
- regression_check: Compare bench_main output to baseline (CI).
- data.generate_bench_data: Generate users.json / users.ndjson.
"""

---
alwaysApply: true
---
# Rule: Benchmarks Are Required for Performance-Touched Code

If you touch hot paths, you must update benchmarks.

## When this applies
Any change that affects:
- parsing/serialization throughput
- ndjson streaming
- search/query execution
- memory arenas / allocations
- zero-copy/view lifetimes
- compiler flags / build settings that impact perf

## Requirements
- Add or update benchmark cases so the change is measurable.
- Compare against competitor libs in the benchmark suite.
- Treat regressions as bugs: document results under `docs/benchmarks/` and open a tracked issue.

## Benchmark hygiene
- same datasets across libs
- warmup + multiple iterations
- report min/median/p95
- capture environment (CPU/OS/Python/compiler flags)
- measure memory (RSS/peak) when possible

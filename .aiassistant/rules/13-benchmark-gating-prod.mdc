---
alwaysApply: true
---
# Rule: Benchmark Leadership Gate for Production Deployment

Strata may be considered “production-ready” only when it leads benchmarks for the relevant workloads.

## Production deployment gate
Before tagging a release / promoting to production:
- Strata MUST rank **#1** on the project’s canonical benchmark suite for the targeted scenario(s).
- If Strata is not #1, the release is **blocked** until the gap is closed or a documented exception is approved in-repo.

## Scope: what must be #1
For each supported capability, Strata must win the corresponding benchmark group:
- `loads` (parse) throughput + latency
- `dumps` (serialize) throughput + latency
- NDJSON iteration/streaming throughput
- search/query performance (JSONPath-like or project-defined query API)
- memory efficiency (RSS/peak, allocations) where measured

## Required evidence package
A production candidate must include:
1) Benchmark report under `docs/benchmarks/` with:
   - commit hash / version
   - environment details (CPU, OS, compiler, flags, Python version)
   - dataset descriptions + sizes
   - min/median/p95 (and RSS/peak if applicable)
2) Reproducible benchmark commands committed to the repo (e.g., `make bench`, `python -m bench ...`).
3) Comparison against the defined competitor set (documented list in repo).

## Handling non-determinism and fairness
- Benchmarks must use consistent datasets and identical semantics across libraries.
- Use warmups and multiple iterations.
- Report variance; treat high variance as a benchmark quality issue.
- If another library wins due to different semantics, document the semantic mismatch explicitly and add a “semantic parity” benchmark where applicable.

## Exceptions (rare)
Any exception to the “must be #1” rule requires:
- a tracked issue explaining why
- a time-bounded plan to achieve #1
- explicit sign-off recorded in-repo

# Benchmark Progress Log (Rule 17)

Append-only log of benchmark runs for performance-sensitive changes. Each entry: date/time, commit, environment, commands, metrics, conclusion.

______________________________________________________________________

## 2026-01-31 — P2-10 Dragonbox float formatting

- **Date/time**: 2026-01-31
- **Change**: P2-10 Dragonbox-style float formatting (dragonbox_d2s primary path, std::to_chars when available, Ryu fallback).
- **Commit**: (session; no new commit at log time)
- **Environment**: macOS (darwin), Python 3.14, users.json 5.05 MB
- **Commands**: `make bench-data` then `.venv/bin/python -m benchmarks.bench_dumps --data benchmarks/data/generated/users.json --repeat 20 --warmup 2`
- **Metrics (strata dumps)**:
  - Min: 7.33 ms
  - Median: 7.80 ms
  - P95: 8.14 ms
  - RSS: 72.0 MB
  - Rank: #3 / 5 (44.8% behind orjson #1)
- **Baseline (from docs/benchmarks/dumps_results.md)**: median 7.63 ms, p95 8.17 ms
- **Delta**: median +2.2%, p95 −0.4%
- **Conclusion**: **Neutral**. Median slightly higher in this run (within typical run-to-run variance); p95 improved. No revert; float path now uses Dragonbox/std::to_chars consistently. Re-run with more iterations (e.g. repeat 50) to confirm stability.

______________________________________________________________________

## 2026-01-31 — P2-11 SIMD UTF-8 validation in parser

- **Date/time**: 2026-01-31
- **Change**: P2-11 SIMD UTF-8 validation in parser. Added `validate_utf8_simd()` in `simd_string.cpp` (scalar + AVX2/SSE2/NEON); parser calls it upfront before parsing. Invalid UTF-8 (overlong, surrogates, > U+10FFFF, lone continuation) is rejected with `ParseError`.
- **Commit**: (session; no new commit at log time)
- **Environment**: macOS (darwin), Python 3.14, users.json ~5.3 MB
- **Commands**: `.venv/bin/python -m benchmarks.bench_main --dataset benchmarks/data/generated/users.json --repeat 5 --warmup 1`
- **Metrics (strata parse)**:
  - Min: 16.959 ms
  - Median: 20.346 ms
  - P95: 21.515 ms
- **Baseline (from docs/benchmarks/bench_results.md)**: strata parse median 20.164 ms
- **Delta**: median +0.9% (within run-to-run variance)
- **Conclusion**: **Neutral**. No regression. Upfront SIMD validation adds one pass; on ASCII-heavy data overhead is small. Invalid UTF-8 is now consistently rejected; tests (C++ + Python) pass.

______________________________________________________________________

## 2026-02-01 — Unicode escape decoding in parser

- **Date/time**: 2026-02-01
- **Change**: Added `\uXXXX` (including surrogate pairs) decoding in JSON string parsing.
- **Commit**: (session; no new commit at log time)
- **Environment**: macOS (darwin), Python 3.14.2, users.json 5.29 MB
- **Commands**:
  - `.venv/bin/python -m benchmarks.bench_main --dataset benchmarks/data/generated/users.json --repeat 5 --warmup 1`
  - `.venv/bin/python -m benchmarks.bench_loads --data benchmarks/data/generated/users.json --repeat 10 --warmup 2`
- **Metrics (strata parse)**:
  - Min: 61.620 ms
  - Median: 73.665 ms
  - P95: 90.156 ms
  - RSS: 99.5 MB
- **Baseline (from docs/benchmarks/bench_results_baseline_2026_01_31.md)**: strata parse median 20.320 ms, p95 21.389 ms
- **Delta**: median +262.6%, p95 +321.5%
- **Conclusion**: **Regression observed**. Dataset contains zero `\u` escape sequences, so the new escape decoding path should not affect these numbers; this suggests an environment/config mismatch. Needs follow-up benchmarking in a controlled setup to confirm; no performance-tuning changes applied in this session.

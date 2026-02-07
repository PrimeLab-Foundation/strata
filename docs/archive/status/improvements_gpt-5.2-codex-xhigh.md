# Strata — Improvement Plan (by gpt-5.2-codex-xhigh)

- Date: 2026-01-31
- Commit/branch analyzed: 0dd6fec82903a7473ecc5709b020843b28cff0e7 (detached HEAD)
- Environment assumptions: macOS 25.1.0 arm64, Apple clang 17.0.0, Python 3.14.2

## 1. Executive summary

- Dumps remains the biggest gap: #3, ~11.3% behind #1 after PGO+LTO, so serialization hot paths still dominate overall ranking.
- NDJSON parsing is #5 and ~92% behind #1, with line splitting overhead called out as a major cost.
- Loads is currently #1 with best memory; maintain this baseline while focusing optimizations elsewhere.
- JSONPath is already decisively faster than jmespath; gaps are feature completeness (AND/OR, unions) rather than speed.
- Benchmark comparability is fragile when competitor libs are missing; enforce fail-fast or explicit “missing deps” gates.
- Documentation and rule alignment need cleanup: rules mention pybind11 while status/docs say Python C API migration complete.
- Coverage is mandated at 100%, but CI enforcement and parity checks should be explicit and automated.
- Build system already has LTO/PGO and a gate command, but reproducibility metadata is not centralized or consistently captured.

## 2. Priority roadmap

| Priority | Area       | Item title                                          | Expected impact | Effort | Evidence                                                                        |
| -------- | ---------- | --------------------------------------------------- | --------------- | ------ | ------------------------------------------------------------------------------- |
| P0       | Perf       | Close dumps gap to #1                               | High            | M      | `docs/benchmarks/dumps_results.md`                                              |
| P0       | Perf       | NDJSON throughput overhaul                          | High            | M      | `docs/benchmarks/ndjson_results.md`                                             |
| P0       | Bench      | Fail-fast on benchmark errors + missing deps        | High            | S      | `docs/benchmarks/methodology.md`, `docs/benchmarks/bench_results_2026_01_31.md` |
| P1       | Build/Docs | Align binding strategy (C API vs pybind11)          | Med             | S      | `docs/status/current_status.md`, `.cursor/rules/01-cpp-owns-cpu.mdc`            |
| P1       | Testing    | Parity enforcement + coverage gates in CI           | Med             | M      | `docs/development/coverage.md`, `docs/test_coverage_summary.md`                 |
| P1       | Bench      | Add missing dataset variants (escape/unicode heavy) | Med             | M      | `docs/benchmarks/datasets.md`                                                   |
| P2       | Search     | JSONPath AND/OR, unions                             | Med             | M      | `docs/benchmarks/jsonpath_results.md`                                           |
| P2       | Docs       | Consolidate benchmark result files and indexing     | Low             | S      | `docs/README.md`, `docs/benchmarks/*`                                           |

## 3. Detailed recommendations

### 3.1 Close dumps gap to #1 (P0)

- Problem: dumps is #3 and still >10% behind orjson/msgspec in median latency.
- Evidence: `docs/benchmarks/dumps_results.md` (latest run table).
- Proposed change:
  - Audit `src/strata/bindings/python_dumps.cpp` for remaining type-check overhead and branching.
  - Experiment with direct PyBytes write path in `OutputBuffer` (zero-copy finalize).
  - Tighten number formatting (integers via `std::to_chars`, floats via Dragonbox/Ryu already present but verify call-site coverage).
- Acceptance criteria:
  - Median within 5% of orjson on `bench_dumps` with repeat 50, warmup 3.
  - No regression in loads/ndjson benchmarks and memory.
- Risks/tradeoffs:
  - Direct PyBytes writing must maintain correctness on reallocation and finalize behavior.
  - Aggressive inlining could increase code size and i-cache pressure.
- Suggested owner modules:
  - `src/strata/bindings/python_dumps.cpp`
  - `include/strata/util/output_buffer.hpp`
  - `src/strata/util/ryu_dtoa.cpp`

### 3.2 NDJSON throughput overhaul (P0)

- Problem: NDJSON median is #5, ~92% behind #1; line splitting overhead identified.
- Evidence: `docs/benchmarks/ndjson_results.md`.
- Proposed change:
  - Integrate SIMD newline search (`simd_string`) into `ndjson_stream.cpp`.
  - Batch parse lines (vectorized scanning + chunked parse) to reduce per-line overhead.
  - Add an optional “batch size” path with the same semantics as `iter_ndjson`.
- Acceptance criteria:
  - Median within 20% of #1 on `bench_ndjson` for `users.json`.
  - No increase in RSS beyond 10% baseline.
- Risks/tradeoffs:
  - Chunked parsing must preserve line-numbered error reporting.
  - SIMD path must have scalar fallback for portability.
- Suggested owner modules:
  - `src/strata/json/ndjson_stream.cpp`
  - `src/strata/util/simd_string.cpp`

### 3.3 Benchmark fail-fast and dependency gating (P0)

- Problem: Benchmark results list missing libraries; rules require all competitors.
- Evidence: `docs/benchmarks/bench_results_2026_01_31.md` (missing libs), `docs/benchmarks/methodology.md` (error handling rules).
- Proposed change:
  - Make `bench_main.py` exit non-zero on any ERROR rows.
  - Add a preflight check for required libs (orjson/ujson/ijson/pysimdjson/msgspec/jmespath) and fail if missing.
- Acceptance criteria:
  - Any benchmark ERROR or missing library stops the run and returns non-zero.
  - Results are only written when all competitors are present.
- Risks/tradeoffs:
  - Bench runs are stricter; developers must install all deps.
- Suggested owner modules:
  - `benchmarks/bench_main.py`
  - `benchmarks/bench_*`

### 3.4 Binding strategy alignment (C API vs pybind11) (P1)

- Problem: Rules mention pybind11, but current status says migration to Python C API is complete.
- Evidence: `docs/status/current_status.md` (C API), `.cursor/rules/*` (pybind11 references).
- Proposed change:
  - Decide and document the binding strategy (C API or pybind11) in a single authoritative doc.
  - Remove unused pybind11 dependency if C API is canonical.
- Acceptance criteria:
  - All binding docs and rules agree on one approach.
  - Build/deps reflect the chosen approach.
- Risks/tradeoffs:
  - Changing rules may require project-level agreement.
- Suggested owner modules:
  - `docs/status/current_status.md`
  - `.cursor/rules/*`
  - `pyproject.toml` / `Makefile`

### 3.5 Parity enforcement + coverage gates in CI (P1)

- Problem: 100% coverage is required; parity between C++ and Python tests must be enforced.
- Evidence: `docs/development/coverage.md`, `docs/test_coverage_summary.md`.
- Proposed change:
  - Add CI job that checks coverage thresholds and parity matrix.
  - Fail CI if coverage drops or parity matrix is outdated.
- Acceptance criteria:
  - Coverage gates enforced in CI with explicit thresholds.
  - Parity matrix updated on test additions.
- Risks/tradeoffs:
  - CI time increases; may need caching.
- Suggested owner modules:
  - `docs/development/coverage.md`
  - `docs/development/test_parity_matrix.md`
  - CI config (if present)

### 3.6 Dataset gaps: escape-heavy + unicode-heavy (P1)

- Problem: datasets note missing variants; escapes/unicode are real-world hotspots.
- Evidence: `docs/benchmarks/datasets.md` (variants to add).
- Proposed change:
  - Extend `benchmarks/data/generate_bench_data.py` to include escape-heavy and unicode-heavy datasets.
  - Add corresponding bench cases for loads/dumps.
- Acceptance criteria:
  - New datasets appear in `benchmarks/data/generated/`.
  - Bench suite runs with these datasets across all libs.
- Risks/tradeoffs:
  - Larger dataset matrix increases benchmark time.
- Suggested owner modules:
  - `benchmarks/data/generate_bench_data.py`
  - `benchmarks/datasets.py`

### 3.7 JSONPath feature completeness (P2)

- Problem: AND/OR and unions not yet implemented.
- Evidence: `docs/benchmarks/jsonpath_results.md` (not implemented section).
- Proposed change:
  - Add parser support for boolean expressions and union syntax.
  - Extend tests and benchmarks for those queries.
- Acceptance criteria:
  - New features pass tests and are benchmarked against jmespath.
- Risks/tradeoffs:
  - Feature complexity might increase execution overhead; must guard against regressions.
- Suggested owner modules:
  - `src/strata/search/jsonpath.cpp`
  - `tests/py/test_jsonpath*.py`

### 3.8 Benchmark result consolidation + indexing (P2)

- Problem: Multiple benchmark result files can drift and conflict.
- Evidence: `docs/benchmarks/*`, `docs/README.md`.
- Proposed change:
  - Establish a single “latest results” file with a date-stamped archive.
  - Update `docs/README.md` to point to the latest.
- Acceptance criteria:
  - One canonical latest benchmark doc, archived by date.
- Risks/tradeoffs:
  - Requires consistent update discipline.
- Suggested owner modules:
  - `docs/benchmarks/`
  - `docs/README.md`

## 4. “Algorithms & research” ideas (optional but recommended)

- SIMD newline scanning + vectorized line splitting for NDJSON.
- Chunked parsing pipeline for NDJSON (scan → slice → parse batch).
- Branchless number parsing/formatting paths for common integers and floats.
- Direct PyBytes-backed buffer for dumps (zero-copy finalize).
- SIMD string escape scanning + bulk copy for ASCII runs.
- Structure-aware parsing (tape/arena) to reduce allocations.
- Compiled JSONPath bytecode VM for complex filters.
- Index-assisted JSONPath (precomputed field maps for hot queries).
- UTF-8 validation fast path (ASCII skip + SIMD validation).

## 5. Next actions (copy/paste checklist)

- [ ] Add bench harness fail-fast on ERROR rows and missing deps.
- [ ] Audit dumps hot path and remove residual slow checks.
- [ ] Prototype direct PyBytes buffer finalize for dumps.
- [ ] Add SIMD newline scan to NDJSON splitter.
- [ ] Extend NDJSON benchmarks to report throughput + memory.
- [ ] Add escape-heavy and unicode-heavy datasets.
- [ ] Update datasets and methodology docs with new datasets.
- [ ] Align binding strategy docs/rules (C API vs pybind11).
- [ ] Add CI coverage gate and parity matrix check.
- [ ] Record build flags (LTO/PGO) in benchmark outputs.
- [ ] Create a single “latest benchmarks” doc and archive older ones by date.

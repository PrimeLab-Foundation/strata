# Changelog

All notable changes to turbojsonpath will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## \[Unreleased\]

### Added

- Native NDJSON execution in the C++ core, exposed via Python `search_file(..., ndjson=True, return_mode="json" | "view")`.
  - Splits files into records, evaluates the compiled JSONPath per record, and streams zero-copy byte views.
  - Supports root-array predicates like `$[?(@.age>30)]` by evaluating against a synthetic single-element array when needed.
  - Skips malformed lines; fails only on file I/O or compile errors.
- Benchmarks: turbojsonpath runner and adapters now use the native NDJSON path (no Python per-line loops).
- Tests: added `tests/test_ndjson_native.py` validating results vs Python reference evaluators for NDJSON queries.

### Changed

- `benchmarks/runners/bench_turbojsonpath.py` simplified to a single `search_file(..., ndjson=...)` call.
- `benchmarks/adapters.py` labels native NDJSON as `adapter=tjp_stream_native` for clarity in artifacts.

______________________________________________________________________

## [0.3.0-rc1] - 2025-11-11

### Added - PR #3f: Unified Parallel Executor

**Major Feature**: Production-ready parallel execution engine with deterministic ordering and safe cancellation.

- **Unified Executor Architecture**

  - Single parallel engine for both NDJSON and Top-Key array processing
  - Replaces legacy PR #3e threading code (removed ~420 lines)
  - Bounded MPSC queue with backpressure (configurable via `Config.queue_capacity`)
  - Deterministic coordinator using min-heap for seq_id ordering
  - Per-worker resources (simdjson parsers, stats, buffers)

- **Thread Safety & Reliability**

  - Cross-thread error propagation via atomic ErrorState
  - Clean shutdown protocol: workers → queue.shutdown() → coordinator drain
  - No deadlocks even with tiny queue sizes (verified with queue=16)
  - Tested with thread counts from 1 to 32

- **Performance Improvements**

  - 2.6-2.7× speedup on 256MB NDJSON with 4-8 threads
  - Zero-copy integration with PR #4 mmap and SmallJsonWriter
  - Efficient backpressure prevents unbounded memory growth
  - Linear scaling up to available CPU cores

- **Configuration API**

  - `Config.queue_capacity` - Bounded result queue size (default: 8192)
  - `Config.worker_batch_flush` - Worker batch size (reserved for future, default: 4096)
  - `Config.shutdown_timeout_ms` - Coordinator timeout (reserved for future, default: 5000)

- **Stats & Observability**

  - Per-worker stats aggregation (thread-safe)
  - `Stats.to_json()` - Export stats as JSON for CI artifacts
  - Stats snapshot support via `TJ_WRITE_STATS=1` environment variable
  - Accurate peak RSS and throughput metrics

- **Testing & Validation**

  - 14 comprehensive stability tests in `test_threading_stability.py`
  - Tests for determinism, serial/parallel equivalence, backpressure, edge cases
  - SHA256 hash-based determinism verification
  - All existing 71 tests pass without modification
  - GitHub Actions CI matrix: Ubuntu/macOS/Windows × Python 3.9-3.12
  - ASan/UBSan sanitizers on Linux
  - Binary wheels via cibuildwheel for all platforms

### Fixed

- **Critical**: Serial and parallel executors now produce byte-identical output
  - Fixed array unwrapping discrepancy via `normalize_result_for_output()`
  - Coordinator normalizes results after ordering to maintain consistency
  - Test coverage: all equivalence tests verify byte-for-byte equality
- Eliminated deadlocks from PR #3e parallel executor
- Fixed race conditions in worker coordination
- Resolved non-deterministic output ordering in parallel mode

### Changed

- `Config.threads > 1` now uses unified executor (previously used legacy code)
- All parallel code paths now share single implementation
- Improved error messages for threading failures

### Internal

- Added `executor.hpp` and `executor.cpp` with complete parallel infrastructure
- Added `BoundedQueue<T>` template for thread-safe producer-consumer
- Added `ResultItem` struct with seq_id for deterministic ordering
- Added `WorkerContext` with per-worker simdjson::ondemand::parser
- Removed legacy `Match`, `WorkerStats`, `ResultQueue` from scanner.cpp
- Added `CMakeLists.txt` build integration for executor.cpp

### Documentation

- Updated `THREADING.md` with unified executor architecture diagram
- Added troubleshooting guide for parallel execution
- Documented backpressure and queue tuning best practices

______________________________________________________________________

## [0.2.0] - 2024-XX-XX (PR #4: mmap + zero-copy)

### Added

- Memory-mapped file I/O for large files (PR #4)
- Zero-copy JSON writer (SmallJsonWriter) for reduced allocations
- `Config.use_mmap` - Enable/disable mmap (default: true)
- `Config.mmap_threshold_bytes` - Minimum file size for mmap (default: 1 MB)
- `Config.json_writer_chunk` - Initial writer capacity (default: 256 KB)
- `Config.json_writer_cap` - Maximum writer capacity (default: 8 MB)

### Performance

- 2-3× speedup on large files with mmap enabled
- 30-50% memory reduction with zero-copy writer
- Efficient handling of 256MB+ files

______________________________________________________________________

## [0.1.0] - 2024-XX-XX (PR #1: Foundation)

### Added

- Initial release with core JSONPath evaluation
- NDJSON streaming support
- Top-level array processing
- simdjson integration for fast parsing
- Python bindings via pybind11
- `Config` and `Stats` APIs
- Basic threading support (PR #3e, later replaced by PR #3f)

### Features

- `search_file()` - Stream over JSON/NDJSON files
- `search_bytes()` - Evaluate on in-memory JSON
- `compile()` - Pre-compile JSONPath expressions
- Format auto-detection (NDJSON, top-level array, single document)
- Memory-efficient streaming for large datasets

______________________________________________________________________

## Release Comparison

| Version    | Key Feature               | Performance Gain | Stability |
| ---------- | ------------------------- | ---------------- | --------- |
| v0.1.0     | Core JSONPath + NDJSON    | Baseline         | Stable    |
| v0.2.0     | mmap + zero-copy          | 2-3× faster      | Stable    |
| v0.3.0-rc1 | Unified parallel executor | 2.7× faster      | RC        |

______________________________________________________________________

[0.1.0]: https://github.com/yourusername/turbojsonpath/releases/tag/v0.1.0
[0.2.0]: https://github.com/yourusername/turbojsonpath/compare/v0.1.0...v0.2.0
[0.3.0-rc1]: https://github.com/yourusername/turbojsonpath/compare/v0.2.0...v0.3.0-rc1

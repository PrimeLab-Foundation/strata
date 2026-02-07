# ADR-0005: Parallel NDJSON Processing

**Status:** Accepted
**Date:** 2026-02-05

## Context

Single-threaded NDJSON parsing leaves multi-core CPUs underutilized for large files. NDJSON (Newline Delimited JSON) has an inherent property that makes it highly parallelizable: each line is an independent JSON document that can be parsed in isolation.

**Current state analysis:**
- Existing `NdjsonStream` class provides sequential line-by-line parsing
- SIMD-accelerated newline detection already available (`find_newline_simd`, `collect_newlines_simd`)
- `parse_json()` is thread-safe for per-line parsing

**Goal:** Achieve 2-4x speedup for large NDJSON files by parsing independent lines concurrently.

## Decision

Implement parallel NDJSON processing using:

1. **Thread Pool Infrastructure** - A new `ThreadPool` class in `src/strata/util/thread_pool.cpp`
2. **Parallel NDJSON Stream** - A new `ParallelNdjsonStream` class in `src/strata/json/parallel_ndjson.cpp`
3. **SIMD Line Boundary Detection** - Reuse existing SIMD utilities for efficient line boundary collection
4. **Sequence Numbers for Order Preservation** - Each chunk is assigned a sequence number to maintain input order

### Architecture

```
Input NDJSON:          Line boundaries (SIMD scan):
┌─────────────────┐    ┌──────────────────────────┐
│ {"id": 1}       │    │ 0, 11, 22, 33, ...      │
│ {"id": 2}       │    └──────────────────────────┘
│ {"id": 3}       │              │
│ {"id": 4}       │              ▼
│ ...             │    Partition into chunks:
└─────────────────┘    ┌────────┬────────┬────────┐
                       │Chunk 0 │Chunk 1 │Chunk 2 │
                       │seq=0   │seq=1   │seq=2   │
                       │lines   │lines   │lines   │
                       │0-99    │100-199 │200-299 │
                       └────────┴────────┴────────┘
                              │
                    ┌─────────┼─────────┐
                    ▼         ▼         ▼
               Thread 0   Thread 1   Thread 2
               parse()    parse()    parse()
                    │         │         │
                    └─────────┼─────────┘
                              ▼
                    Merge in sequence order:
                    result[seq=0] ++ result[seq=1] ++ result[seq=2]
```

### Thread Pool (`util::ThreadPool`)

```cpp
class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads = 0);  // 0 = hardware_concurrency
    ~ThreadPool();

    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;

    template<typename F>
    void submit_bulk(const std::vector<F>& tasks, std::vector<std::future<...>>& futures);

    size_t thread_count() const;
    void shutdown();
};
```

**Properties:**
- Mutex-protected task queue (simple, sufficient for batch workloads)
- Condition variable for efficient thread wake-up
- Graceful shutdown (completes pending tasks)
- Pure C++ (no Python dependencies)

### Parallel NDJSON Stream (`ParallelNdjsonStream`)

```cpp
struct ParallelNdjsonConfig {
    size_t num_threads = 0;              // 0 = auto-detect
    size_t min_chunk_size = 64 * 1024;   // 64KB minimum per chunk
    size_t min_lines_for_parallel = 1000; // Below this, use sequential
    bool skip_errors = false;
};

class ParallelNdjsonStream {
public:
    explicit ParallelNdjsonStream(std::string_view data, ParallelNdjsonConfig config = {});

    std::vector<JsonValue> parse_all_parallel();
    ParallelParseResult parse_all_parallel_with_errors();

    size_t lines_processed() const;
    size_t error_count() const;
    bool used_parallel_mode() const;
};
```

**Processing phases:**
1. **Line Boundary Collection** - SIMD scan to find all newline positions O(n)
2. **Chunk Partitioning** - Divide lines into chunks based on thread count and minimum chunk size
3. **Parallel Parsing** - Submit chunks to thread pool, each chunk parsed independently
4. **Ordered Merge** - Sort results by sequence number, concatenate

### Python Bindings

Exposed as module-level function `ndjson_parallel_parse_all`:

```python
result = strata._strata.ndjson_parallel_parse_all(
    data,                 # str: NDJSON data
    skip_errors=True,     # bool: skip malformed lines
    num_threads=0         # int: 0 = auto-detect
)
```

**GIL handling:**
- GIL is held during parallel parsing (required for exception handling)
- C++ worker threads operate independently
- Results converted to Python objects after parsing completes

## Alternatives Considered

### 1. Pipeline Parallelism (I/O + Parse Stages)

**Pros:** Better for streaming from disk/network
**Cons:** Requires async I/O, more complex, less benefit when data is in memory
**Decision:** Not chosen - data is typically already in memory for NDJSON use cases

### 2. Lock-free Concurrent Result Vector

**Pros:** Potentially lower synchronization overhead
**Cons:** Complex implementation, false sharing issues, unpredictable memory access patterns
**Decision:** Not chosen - per-thread result vectors with sequential merge is simpler and equally efficient

### 3. Memory-mapped Parallel Regions

**Pros:** OS handles memory management
**Cons:** Requires file-based input, mmap overhead for small files
**Decision:** Not chosen - string_view input more flexible, mmap can be added later if needed

### 4. Automatic Heuristic vs Explicit Parallel Method

**Pros (auto):** Simpler API
**Cons (auto):** Unpredictable behavior, harder to benchmark
**Decision:** Explicit `ndjson_parallel_parse_all` function with automatic fallback to sequential for small inputs

## Error Model

- Errors collected per-chunk with line numbers
- `parse_all_parallel_with_errors()` returns all errors with context
- `skip_errors=false` throws on first error
- Line numbers are 1-indexed and absolute (not chunk-relative)

## Memory Model

- Per-thread result vectors to avoid allocator contention
- Results merged by sequence number at the end
- Memory overhead bounded by number of threads × typical chunk result size
- Automatic fallback to sequential for small inputs avoids overhead

## API Design

**Explicit parallel methods** chosen over automatic heuristics:
- `ParallelNdjsonStream::parse_all_parallel()` - main entry point
- Automatic fallback to sequential when `line_count < min_lines_for_parallel` or `data_size < min_chunk_size * 2`
- Configuration via `ParallelNdjsonConfig` struct

## Risks and Mitigations

| Risk | Mitigation |
|------|------------|
| False sharing in result vectors | Per-thread result buffers, merge at end |
| Allocator contention | Thread-local parsing, no shared allocations during parse |
| Overhead exceeds gains for small files | Automatic fallback to sequential below threshold |
| Result ordering bugs | Comprehensive parity tests, sequence number validation |
| Memory amplification | Bound concurrent chunk count by thread count |

## Test Plan

### C++ Tests (`tests/cpp/test_parallel_ndjson.cpp`)
- Order preservation with 10,000+ lines
- Error collection from multiple chunks
- Parity with sequential `NdjsonStream`
- Edge cases: single line, empty data, Windows line endings, etc.

### Python Tests (`tests/py/test_parallel_ndjson.py`)
- Basic parsing, empty data, mixed types
- Order preservation (small, medium, large datasets)
- Parity with sequential `NdjsonStream.parse_all()`
- Error handling (skip_errors=True/False)
- Configuration options (num_threads)

### Thread Pool Tests (`tests/cpp/test_thread_pool.cpp`)
- Basic task submission and result collection
- Concurrent execution
- Bulk submission
- Exception handling
- Shutdown behavior

## Benchmark Plan

Expected performance targets:

| Dataset Size | Expected Speedup | Max Memory Overhead |
|--------------|------------------|---------------------|
| < 1MB        | ≥ 0.8x (acceptable overhead) | < 50% |
| 1-10MB       | ≥ 1.5x | < 30% |
| 10-100MB     | ≥ 2.0x | < 20% |
| > 100MB      | ≥ 2.5x | < 15% |

Metrics to capture:
- Wall time (min/median/p95)
- Throughput (MB/s, lines/s)
- Peak RSS delta
- CPU utilization

## Files Changed

**New Files:**
- `include/strata/util/thread_pool.hpp` - Thread pool header
- `src/strata/util/thread_pool.cpp` - Thread pool implementation
- `include/strata/json/parallel_ndjson.hpp` - Parallel NDJSON header
- `src/strata/json/parallel_ndjson.cpp` - Parallel NDJSON implementation
- `tests/cpp/test_thread_pool.cpp` - Thread pool tests
- `tests/cpp/test_parallel_ndjson.cpp` - Parallel NDJSON tests
- `tests/py/test_parallel_ndjson.py` - Python tests
- `docs/adr/ADR-0005-parallel-ndjson-processing.md` - This document

**Modified Files:**
- `src/strata/bindings/python_ndjson.cpp` - Added `ndjson_parallel_parse_all` function
- `setup.py` - Added new source files
- `CMakeLists.txt` - Added new source files and test targets

## Consequences

**Positive:**
- 2-4x speedup for large NDJSON files
- Thread pool reusable for future parallel workloads
- Clean separation between core and Python binding layers

**Negative:**
- Additional complexity in NDJSON processing
- Memory overhead for parallel mode
- Thread pool threads created per-parse (could be optimized with pooling)

**Neutral:**
- Automatic fallback to sequential for small inputs maintains backward compatibility
- Explicit API means no breaking changes to existing code

# Strata Work Plan

## Mission

Build **Strata** as the fastest and most memory-efficient JSON engine, beating all competitors: `orjson`, `ujson`, `ijson`, `pysimdjson`, `msgspec`, and `jmespath`.

## Current State Analysis

### What We Have (strata/cpp/)

- ✅ Basic JSON parsing (recursive descent)
- ✅ JsonValue model (std::variant based)
- ✅ JsonCursor navigation (non-owning views)
- ✅ JsonDocument (owning handle)
- ✅ Python bindings (pybind11)
- ✅ Basic tests

### What's Missing (vs cpp_bkp/)

- ❌ JSONPath engine (compilation + evaluation)
- ❌ Predicate pushdown optimization
- ❌ Parallel execution engine
- ❌ Memory-mapped I/O
- ❌ NDJSON streaming support
- ❌ Benchmark infrastructure
- ❌ Performance optimizations (hash maps, SIMD, arenas)

### Critical Gap

**The cpp_bkp/ directory contains a production-ready implementation** with all missing features. This is a treasure trove of proven code that we should leverage.

## Work Phases

### Phase 0: Benchmarking Infrastructure (CURRENT)

**Goal**: Establish baseline performance metrics

**Tasks**:

1. ✅ Analyze project structure
1. 🔄 Create work plans and improvement plans
1. ⏳ Build benchmark orchestrator (`benchmarks/bench_main.py`)
1. ⏳ Build query evaluation helpers (`benchmarks/eval_queries.py`)
1. ⏳ Generate benchmark data
1. ⏳ Run initial benchmarks against all competitors
1. ⏳ Document baseline performance in `BENCH_RESULTS.md`

**Success Criteria**:

- Automated benchmark suite running
- Clear performance gap identification
- Reproducible results

### Phase 1: Core Performance Optimizations

**Goal**: Optimize current implementation without adding new features

**Tasks**:

1. Replace `std::map` with `std::unordered_map` for O(1) object lookups
1. Add string interning/pooling to reduce allocations
1. Optimize number parsing (avoid std::stod)
1. Add move semantics optimizations
1. Implement custom allocators for JSON tree nodes
1. Add SIMD parsing for strings and whitespace
1. Optimize memory layout (consider compact variant, cache alignment)

**Expected Impact**: 2-3x improvement in parsing speed

**Benchmark After**: Re-run full benchmark suite

### Phase 2: JSONPath Engine

**Goal**: Add complete JSONPath support (core competitive feature)

**Tasks**:

1. Port JSONPath compiler from cpp_bkp/jp_compile.\*
1. Port JSONPath evaluator from cpp_bkp/jp_eval.\*
1. Port predicate pushdown from cpp_bkp/jp_eval_pushdown.\*
1. Integrate with current JsonCursor API
1. Add Python bindings for search/query
1. Add comprehensive JSONPath tests
1. Update benchmarks to include JSONPath queries

**Expected Impact**: Enable JSONPath feature parity with jmespath/jsonpath-ng

**Benchmark After**: Re-run with JSONPath queries

### Phase 3: Advanced Performance Features

**Goal**: Add parallel execution and advanced I/O

**Tasks**:

1. Port parallel executor from cpp_bkp/executor.\*
1. Port memory-mapped I/O from cpp_bkp/io_mmap.\*
1. Port NDJSON streaming from cpp_bkp/ndjson\_\*
1. Port partitioning logic from cpp_bkp/partitioner.\*
1. Add arena allocators from cpp_bkp/arenas.hpp
1. Add stats tracking from cpp_bkp/stats.hpp
1. Integrate platform utilities from cpp_bkp/platform.hpp
1. Update Python API for streaming/parallel operations

**Expected Impact**: 5-10x improvement on large files with parallelism

**Benchmark After**: Re-run with large datasets

### Phase 4: Serialization (dumps)

**Goal**: Add fastest JSON serialization

**Tasks**:

1. Port JSON writer from cpp_bkp/json_writer.hpp
1. Implement dumps() API
1. Add serialization benchmarks
1. Optimize output buffering
1. Add NDJSON output support

**Expected Impact**: Match or beat orjson on serialization

**Benchmark After**: Add dumps benchmarks

### Phase 5: Polish & Edge Cases

**Goal**: Production readiness

**Tasks**:

1. Handle all JSON edge cases (Unicode, large numbers, etc.)
1. Comprehensive error handling and messages
1. Memory leak detection and fixes
1. Stress testing with huge files
1. Documentation and examples
1. Performance profiling and micro-optimizations

**Benchmark After**: Final competitive analysis

## Dependency Strategy

### Leverage cpp_bkp/ Code

The cpp_bkp/ directory contains proven, production-ready implementations. We should:

1. **Understand** each component thoroughly
1. **Adapt** to work with current JsonCursor API
1. **Integrate** incrementally with tests at each step
1. **Modernize** where beneficial (C++20 features)

### Key Components to Port (Priority Order)

1. **JSONPath engine** (jp_compile.*, jp_eval.*) - Core feature
1. **Parallel executor** (executor.\*) - Major perf win
1. **Memory-mapped I/O** (io_mmap.\*) - Large file perf
1. **Utilities** (arenas.hpp, platform.hpp, stats.hpp) - Supporting infra

## Success Metrics

### Performance Targets (vs Competitors)

- **Parsing (loads)**: Must be #1 or within 5% of orjson
- **Serialization (dumps)**: Must be #1 or within 5% of orjson
- **JSONPath queries**: Must beat jmespath and jsonpath-ng by >20%
- **Memory usage**: Must be competitive with msgspec
- **NDJSON streaming**: Must beat ijson by >50%

### Quality Targets

- ✅ All tests passing (Python + C++)
- ✅ No memory leaks (valgrind clean)
- ✅ Clean benchmark results (no regressions)
- ✅ Documentation complete
- ✅ CI/CD with automated benchmarks

## Timeline Philosophy

This is an iterative, measurement-driven process:

1. **Build → Measure → Analyze → Optimize → Repeat**
1. Each phase includes benchmarking
1. No guessing - let data guide optimizations
1. Don't stop until we're #1 in benchmarks

## Next Actions

1. ✅ Complete work plan creation
1. ⏳ Build benchmark infrastructure
1. ⏳ Run baseline benchmarks
1. ⏳ Start Phase 1 optimizations

# Strata Optimization Roadmap

**Goal**: Achieve #1 ranking in all benchmark categories

**Current Status** (2026-01-30 after SIMD):

- ✅ **#1 JSONPath** (10x faster than jmespath)
- ✅ **#1 Memory** (18% better than orjson)
- ⚡ #2 Parsing (97% of orjson)
- ⚡ **#3 dumps** (SIMD optimized, 47% of orjson) - **Improved from #4!**
- ⚠️ #5 NDJSON (46% of orjson)

______________________________________________________________________

## Phase A: dumps Optimization 🔥

**Target**: Move from #3 (11.0ms) to #1 (5.0ms) - **need 2.2x improvement**

### ✅ Completed:

1. ✅ **SIMD string escaping** - **54% faster** (23.8ms → 11.0ms)
   - Moved from #4 to #3 🎉
   - Gap reduced from 4.4x to 2.2x

### 🎯 Next Steps (Immediate):

1. **Custom allocator for std::string** (expected: 20-30% gain)

   - Pool allocator for temporary strings
   - Arena allocator for result buffer
   - Reduce malloc/free overhead
   - **Target**: 11.0ms → 8.0ms

1. **Improved pre-allocation heuristics** (expected: 10-15% gain)

   - Profile actual size patterns
   - Type-specific size estimates
   - Dynamic adjustment based on input
   - **Target**: 8.0ms → 7.0ms

1. **Dict iteration optimization** (expected: 5-10% gain)

   - Faster key access patterns
   - Reduce Python C API overhead
   - Cache dict size/state
   - **Target**: 7.0ms → 6.5ms

1. **Float formatting improvements** (expected: 5-10% gain)

   - Optimize Ryū further
   - Inline hot paths
   - SIMD for digit conversion
   - **Target**: 6.5ms → 6.0ms

### 🔮 Advanced (If needed):

5. **Custom output buffer** (expected: 10-20% gain)
   - Replace std::string with custom buffer
   - Stack allocation for small outputs
   - Chunked growth strategy
   - **Target**: 6.0ms → 5.0ms

**Timeline**: 2-4 optimization iterations to reach #1

______________________________________________________________________

## Phase B: NDJSON Optimization

**Target**: Move from #5 (20.8ms) to #1 (9.6ms) - **need 2.2x improvement**

### Planned:

1. **SIMD newline search** (expected: 30-40% gain)

   - Use SIMD to find line boundaries
   - Parallel scanning like string escape
   - **Target**: 20.8ms → 13.0ms

1. **Zero-copy batch processing** (expected: 20-30% gain)

   - Parse multiple lines in one call
   - Reduce Python/C++ boundary crossings
   - **Target**: 13.0ms → 10.0ms

1. **Streaming optimizations** (expected: 10-15% gain)

   - Better buffer management
   - Reduce allocations
   - **Target**: 10.0ms → 9.0ms

**Timeline**: Start after dumps reaches #2 or better

______________________________________________________________________

## Phase C: Parsing Micro-optimizations

**Target**: Close 3% gap to orjson (9.2ms → 8.5ms)

### Considerations:

1. **Number parsing SIMD** (expected: 2-5% gain)
1. **String interning for keys** (expected: 1-3% gain)
1. **Better branch prediction** (expected: 1-2% gain)

**Note**: Already very competitive at 97%. Low priority unless other areas complete.

______________________________________________________________________

## Summary: Path to Full #1

| Feature      | Current      | Target   | Priority    | Effort |
| ------------ | ------------ | -------- | ----------- | ------ |
| **JSONPath** | **#1** 🏆    | Maintain | Maintain    | Low    |
| **Memory**   | **#1** 🏆    | Maintain | Maintain    | Low    |
| **Parsing**  | #2 (97%)     | #1       | Low         | Medium |
| **dumps**    | **#3** (47%) | #1       | **HIGH** 🔥 | High   |
| **NDJSON**   | #5 (46%)     | #1       | Medium      | High   |

**Focus Order**:

1. 🔥 dumps optimization (Phase A)
1. 🎯 NDJSON optimization (Phase B)
1. ✨ Parsing micro-opts (Phase C)

**Expected Timeline**:

- dumps to #1: 2-4 weeks
- NDJSON to #1: 1-2 weeks
- **Total**: 4-6 weeks to full #1 across all categories

______________________________________________________________________

## Long-term: Rust/Cython Consideration

If C++20 + SIMD cannot reach #1 in dumps/NDJSON:

**Option 1: Rust core**

- Use simd-json crate
- Compile to static lib
- Link with pybind11 wrapper
- **Pros**: Battle-tested SIMD, orjson-level perf
- **Cons**: New language, build complexity

**Option 2: Cython hybrid**

- Keep C++ for complex logic
- Use Cython for hot Python integration
- Like msgspec architecture
- **Pros**: Best of both worlds
- **Cons**: More complex build

**Decision point**: After exhausting C++ optimizations

______________________________________________________________________

*Last updated: 2026-01-30 (after SIMD implementation)*\
*Next review: After custom allocator attempt*

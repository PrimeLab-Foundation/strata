# Priority Optimizations for 2026

**Goal**: Advance from #3 to #1 overall JSON library ranking  
**Current Gap**: 40% behind orjson (7.73ms vs 5.52ms)  
**Target**: Sub-5.5ms median performance

## 🔥 **Immediate High-Impact Optimizations**

### 1. Memory Allocation Optimization (Est. 20-25% improvement)
**Problem**: Heavy GC triggering during Python object materialization  
**Evidence**: Profiling shows `_PyGC_Collect` dominance in benchmark runs

**Implementation Plan**:
```cpp
// Add to src/strata/json/python_object_builder.cpp
class ThreadLocalArenaPool {
    static Arena* get_arena() {
        thread_local Arena arena(128 * 1024);
        return &arena;
    }
};

// Modify PythonObjectBuilder to use arena
class PythonObjectBuilder : public SAXHandler {
    Arena* arena_;
    // ... use ArenaAllocator for all C++ containers
};
```

**Files to Modify**:
- `src/strata/bindings/python_loads.cpp`
- `src/strata/json/json_parse.cpp` 
- `src/strata/util/arena_allocator.hpp` (expand usage)

### 2. Serialization Output Size Reduction (Est. 15-20% improvement)
**Problem**: Larger output size (1,000,369 vs 895,062 bytes for msgspec)  
**Root Cause**: Suboptimal number formatting and escape sequences

**Implementation**:
```cpp
// Add to src/strata/json/json_serialize.cpp
class FastNumberFormatter {
    // Pre-computed integer strings [0-999]
    static constexpr char integer_table[1000][4] = {...};
    
    // Optimized double formatting with fewer allocations
    void format_double_fast(double d, ArenaStringBuilder& out);
};
```

### 3. SIMD Expansion (Est. 10-15% improvement)
**Current**: UTF-8 validation, string escaping, newline scanning  
**Missing**: Number parsing, string operations, JSON structure parsing

**Scope: SIMD for all JSON data paths**  
SIMD applies to parsing and scanning across all JSON value types (numbers, strings, whitespace, structural chars).  
Python object construction remains in the bindings layer and continues to use the Python C API (no SIMD inside Python objects).

**New SIMD Implementations**:
- `src/strata/util/simd_numbers.cpp` - Vectorized integer/float parsing
- `src/strata/util/simd_json_structure.cpp` - Bracket/brace/comma/colon scanning
- Expand `src/strata/util/simd_string.cpp` - Batch character operations and ASCII fast paths

## 🎯 **Specific Performance Targets**

| Optimization Phase | Target Improvement | Cumulative Performance |
|-------------------|-------------------|------------------------|
| Memory Allocation | 20-25% | 6.2-6.0ms |
| Serialization | 15-20% | 5.3-4.8ms |  
| SIMD Expansion | 10-15% | 4.8-4.1ms |
| **Final Target** | **40-50%** | **Sub-4.5ms (#1 position)** |

## 🛠️ **Implementation Strategy**

### Phase 1: Memory Management (Week 1-2)
1. Implement thread-local arena pools
2. Modify PythonObjectBuilder to use arena allocation
3. Benchmark and validate 20%+ improvement

### Phase 2: Serialization (Week 3-4) 
1. Add fast number formatting with pre-computed tables
2. Optimize escape sequence handling
3. Implement buffer pre-sizing heuristics

### Phase 3: SIMD Expansion (Week 5-6)
1. Add vectorized number parsing
2. Implement SIMD JSON structure scanning
3. Expand string operation vectorization

## 📊 **Validation Protocol**

### Before Each Phase:
```bash
make bench-data     # Generate baseline
make bench-all      # Record current performance
```

### After Each Phase:
```bash
make gate          # Ensure no regressions
make bench-all     # Measure improvement
# Must show >15% improvement or revert changes
```

## 🚀 **Quick Wins** (Can implement today)

1. **String Interning**: Cache common JSON keys
```cpp
// Add to json_parse.cpp
thread_local std::unordered_map<std::string_view, PyObject*> key_cache;
```

2. **Parser State Reuse**: Don't reinitialize per call
```cpp
class JsonParser {
    void reset() { /* reuse existing parser state */ }
};
```

3. **Buffer Pre-sizing**: Estimate output size
```cpp
size_t estimate_output_size(const JsonValue& value) {
    // Heuristic based on input analysis
}
```

## 🎯 **Success Metrics**

- **Primary**: Achieve #1 overall ranking (sub-5.5ms)
- **Secondary**: Maintain #1 memory efficiency
- **Quality**: Zero test regressions, 100% coverage maintained
- **Rules**: Perfect compliance with all 18 project rules

---

**Next Steps**: Begin Phase 1 implementation immediately. The foundation is excellent - these optimizations will push Strata to #1 position.
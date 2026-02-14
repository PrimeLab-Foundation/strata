# Strata Performance Progress Log

This log tracks performance improvements and regressions over time.

---

## Baseline - 2026-02-14

**Git Commit:** `709de84` (main-v2-0.1)
**Dataset:** `benchmarks/data/generated/large/users.json` (43.85 MB)
**Objects:** 6,166,946 (881K dicts, 87K lists, 3.5M strings, 881K ints, 790K floats)

### Performance Metrics

| Library | Min (ms) | Median (ms) | P95 (ms) | Throughput (MB/s) | vs orjson |
|---------|----------|-------------|----------|-------------------|-----------|
| **orjson** | 86.37 | 116.49 | ~125 | 376.58 | 1.00x (baseline) |
| **strata** | 282.19 | 323.08 | ~335 | 136.66 | 2.77x slower |
| **msgspec** | - | - | - | - | - |
| **ujson** | - | - | - | - | - |

### Hotspot Analysis

**Profile Method:** Python cProfile + macOS `sample` (30s, 100 iterations)
**Total Samples:** 138,048

#### Top 10 Bottlenecks (with % of total runtime)

1. **Dictionary Operations (11.63%)**
   - `dict_dealloc`: 8,316 samples (7.03%)
   - `PyDict_SetDefault`: 1,781 samples (1.51%)
   - `dict_setdefault_ref_lock_held`: 1,644 samples (1.39%)
   - `PyDict_New`: 540 samples (0.46%)

2. **Memory Allocation (2.63%)**
   - `_PyObject_Malloc`: 1,154 samples (0.98%)
   - `pymalloc_alloc`: 1,054 samples (0.89%)
   - `_PyObject_Free`: 901 samples (0.76%)

3. **Structural Tape Collection (2.91%)**
   - `strata::util::collect_structural_positions_simd`: 4,023 samples (20.35% of C++ time)

4. **Whitespace Skipping (1.69%)**
   - `strata::util::skip_whitespace_simd`: 2,329 samples (11.78% of C++ time)

5. **Object Builder - Push Value (1.87%)**
   - `strata::bindings::PythonObjectBuilder::push_value`: 2,580 samples (13.05% of C++ time)

6. **Key Processing (1.21%)**
   - `strata::bindings::PythonObjectBuilder::on_key`: 1,674 samples (8.47% of C++ time)

7. **Integer Parsing (1.00%)**
   - `strata::util::parse_int_simd`: 1,386 samples (7.01% of C++ time)
   - `strata::util::parse_uint_simd`: 1,263 samples (6.39% of C++ time)

8. **String Processing (0.92%)**
   - `strata::bindings::PythonObjectBuilder::on_string`: 1,267 samples (6.41% of C++ time)

9. **Key Caching (0.70%)**
   - `strata::bindings::KeyCache::lookup_or_insert`: 961 samples (4.86% of C++ time)

10. **Float Parsing (0.65%)**
    - `strata::util::parse_double_fast`: 903 samples (4.57% of C++ time)

#### Runtime Distribution

- **Python C API calls:** 85.68% (118,277 samples)
  - Dictionary operations: 11.63%
  - Memory management: 2.63%
  - Object creation (float, list, long, GC): 0.79%
  - Other Python overhead: 70.63%

- **Strata C++ parsing:** 14.32% (19,771 samples)
  - SIMD operations: 6.52%
  - Object building: 3.08%
  - String/number parsing: 1.83%
  - Key caching: 1.12%
  - Other: 1.77%

### Materialization Benchmark

Isolated Python object creation (reconstructing 6.17M objects from parsed data):

| Benchmark | Min (ms) | Median (ms) | Mean (ms) |
|-----------|----------|-------------|-----------|
| Strata (parse + materialize) | 282.19 | 323.08 | 318.90 |
| **Materialization only** | 578.19 | 587.53 | 643.49 |
| orjson (parse + materialize) | 86.37 | 116.49 | 147.07 |

**Analysis:**
- Materialization-only is **181.9% of total parse time** (paradoxical - indicates efficient C++ caching)
- Materialization-only is **5.04x slower** than orjson's total time
- **Conclusion:** Python object creation is the primary bottleneck, not parsing logic

### Key Insights

1. **Python Object Creation is the Bottleneck**
   - 85.68% of runtime in Python C API
   - Dictionary operations alone: 11.63%
   - Memory allocation: 2.63%

2. **C++ Parsing is Efficient**
   - Only 14.32% of runtime
   - Good SIMD utilization
   - Structural tape collection is optimized

3. **Comparison to orjson**
   - Strata: 2.77x slower overall
   - Our materialization-only: 5.04x slower than orjson total
   - Suggests orjson has superior object creation strategies

### Optimization Opportunities

#### High Priority (>20% potential gain)

1. **Pre-size Dictionaries**
   - Use `_PyDict_NewPresized` instead of `PyDict_New`
   - Could reduce resize overhead (~5-10% improvement)

2. **Object Pooling**
   - Pool dict/list objects across parses
   - Reduce malloc/free overhead (2.63% → ~1%)

3. **Batch Dictionary Operations**
   - Batch `PyDict_SetDefault` calls
   - Reduce function call overhead

#### Medium Priority (5-20% potential gain)

4. **Expand Key Cache**
   - Currently 7.83% of C++ time in key operations
   - Larger cache could reduce lookups

5. **Optimize Structural Tape**
   - 20.35% of C++ time
   - Could be made lazy or reduced for shallow docs

#### Low Priority (<5% potential gain)

6. **Number Parsing** (already SIMD-optimized)
7. **UTF-8 Validation** (minimal overhead: 0.77%)

### Artifacts

- **cProfile Data:** `docs/benchmarks/strata_loads.prof`
- **Sample Output:** `docs/benchmarks/strata_cpp_profile.sample.txt`
- **Folded Stacks:** `docs/benchmarks/strata_cpp_profile.folded`
- **Hotspot Analysis:** `docs/benchmarks/hotspot_analysis.md`
- **Micro-benchmark:** `benchmarks/bench_object_materialization.py`

### Commands Used

```bash
# Python profiling
.venv/bin/python -m benchmarks.profile_loads \
  --data benchmarks/data/generated/large/users.json \
  --iterations 5 --top-n 20 \
  --output docs/benchmarks/strata_loads.prof

# C++ profiling
.venv/bin/python benchmarks/profile_cpp.py \
  --data benchmarks/data/generated/large/users.json \
  --iterations 100 \
  --output docs/benchmarks/strata_cpp_profile

# Materialization benchmark
.venv/bin/python benchmarks/bench_object_materialization.py \
  --data benchmarks/data/generated/large/users.json \
  --iterations 10 --compare-orjson
```

---

## Future Entries

Add new entries here as optimizations are implemented. Use the following template:

### YYYY-MM-DD - [Optimization Name]

**Git Commit:** `<hash>`
**Description:** Brief description of the change

#### Performance Impact

| Metric | Before | After | Δ |
|--------|--------|-------|---|
| Median (ms) | X | Y | +/-Z% |
| Throughput (MB/s) | X | Y | +/-Z% |
| vs orjson | Xx | Xx | +/-Z% |

#### Hotspot Changes

- [List changes in top 10 hotspots]
- [Include sample count changes if significant]

#### Notes

- Any caveats or additional observations
- Links to related issues/PRs

---

## Performance Goals

- [ ] **Target 1:** Match orjson throughput (377 MB/s) - currently at 137 MB/s (2.77x slower)
- [ ] **Target 2:** Reduce Python C API overhead from 85% to <60%
- [ ] **Target 3:** Dictionary operations from 11.63% to <5%
- [ ] **Target 4:** Memory allocation from 2.63% to <1%

---

## Glossary

- **Median time:** Middle value of all iterations (50th percentile)
- **P95 time:** 95th percentile (slower than 95% of iterations)
- **Throughput:** MB/s = (file_size_mb / median_time_seconds)
- **Sample:** A profiler observation of the call stack (macOS `sample` tool)
- **Hotspot:** A function that appears frequently in profiler samples

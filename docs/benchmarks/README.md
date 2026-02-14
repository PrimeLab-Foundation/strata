# Strata Benchmarks & Profiling

This directory contains benchmark results, profiling data, and performance analysis for the Strata JSON parser.

## Quick Links

- **[Hotspot Analysis](hotspot_analysis.md)** - Detailed profiling results with top 10 hotspots
- **[Progress Log](progress_log.md)** - Performance tracking over time, baseline metrics
- **[Flamegraph (Text)](flamegraph_text.txt)** - Visual representation of runtime distribution

## Latest Results (2026-02-14)

**Dataset:** `large/users.json` (43.85 MB, 6.17M objects)

| Library | Median Time | Throughput | vs orjson |
|---------|-------------|------------|-----------|
| orjson | 116.49ms | 377 MB/s | 1.00x |
| **strata** | **323.08ms** | **137 MB/s** | **2.77x slower** |

## Key Findings

### 🔴 Primary Bottleneck: Python Object Materialization (85.68%)

The profiling analysis reveals that **Python C API calls dominate runtime**:

```
Python C API:        85.68% of runtime
  ├─ Dictionary ops:  11.63% (dict_dealloc, PyDict_SetDefault, etc.)
  ├─ Memory alloc:     2.63% (_PyObject_Malloc, pymalloc_alloc, etc.)
  ├─ Object creation:  0.79% (PyFloat_FromDouble, PyList_New, etc.)
  └─ Other Python:    70.63% (eval frame, overhead, etc.)

Strata C++ parsing:  14.32% of runtime
  ├─ SIMD operations:  6.52% (structural tape, whitespace, number parsing)
  ├─ Object building:  3.08% (push_value, on_key)
  ├─ String/number:    1.83% (on_string, parse_double_fast)
  └─ Other:            2.89% (key cache, misc)
```

### 📊 Top 10 Hotspots

1. **dict_dealloc** (7.03%) - Dictionary cleanup
2. **collect_structural_positions_simd** (2.91%) - Structural tape
3. **skip_whitespace_simd** (1.69%) - Whitespace handling
4. **push_value** (1.87%) - Object builder
5. **PyDict_SetDefault** (1.51%) - Dictionary insertion
6. **dict_setdefault_ref_lock_held** (1.39%) - Dictionary operations
7. **on_key** (1.21%) - Key processing
8. **parse_int_simd** (1.00%) - Integer parsing
9. **_PyObject_Malloc** (0.98%) - Memory allocation
10. **on_string** (0.92%) - String processing

### 🧪 Materialization Benchmark

Isolated Python object creation benchmark:

```
Strata parse + materialize:  323.08ms (baseline)
Materialization only:        587.53ms (181.9% of baseline!)
orjson parse + materialize:  116.49ms (2.77x faster)
```

**Interpretation:** The paradox (materialization > total time) suggests Strata's C++ code efficiently caches/interns objects. However, Python object creation is still the bottleneck.

## Optimization Roadmap

### 🎯 High Priority (>20% potential gain)

1. **Pre-size Dictionaries**
   - Replace `PyDict_New` with `_PyDict_NewPresized`
   - Est. impact: 5-10% improvement

2. **Object Pooling**
   - Pool dict/list objects across parse sessions
   - Est. impact: 1-2% improvement (reduce malloc/free)

3. **Batch Dictionary Operations**
   - Reduce Python C API call overhead
   - Est. impact: 3-5% improvement

### 🔧 Medium Priority (5-20% potential gain)

4. **Expand Key Cache** (currently 1.12% overhead)
5. **Optimize Structural Tape** (currently 2.91% overhead)

### 🔬 Low Priority (<5% potential gain)

6. Number parsing (already SIMD-optimized)
7. UTF-8 validation (minimal overhead: 0.77%)

## Profiling Tools & Scripts

### Available Scripts

| Script | Purpose | Usage |
|--------|---------|-------|
| `benchmarks/profile_loads.py` | Python cProfile profiling | `python -m benchmarks.profile_loads --data <file> --iterations N` |
| `benchmarks/profile_cpp.py` | C++ profiling (macOS sample) | `python benchmarks/profile_cpp.py --data <file> --iterations N` |
| `benchmarks/bench_object_materialization.py` | Materialization overhead | `python benchmarks/bench_object_materialization.py --data <file>` |
| `benchmarks/analyze_sample.py` | Parse sample output | `python benchmarks/analyze_sample.py <sample_file>` |

### Profile a Dataset

```bash
# 1. Python profiling
.venv/bin/python -m benchmarks.profile_loads \
  --data benchmarks/data/generated/large/users.json \
  --iterations 5 \
  --top-n 20 \
  --output docs/benchmarks/strata_loads.prof

# 2. C++ profiling
.venv/bin/python benchmarks/profile_cpp.py \
  --data benchmarks/data/generated/large/users.json \
  --iterations 100 \
  --output docs/benchmarks/strata_cpp_profile

# 3. Analyze C++ profile
.venv/bin/python benchmarks/analyze_sample.py \
  docs/benchmarks/strata_cpp_profile.sample.txt

# 4. Materialization benchmark
.venv/bin/python benchmarks/bench_object_materialization.py \
  --data benchmarks/data/generated/large/users.json \
  --iterations 10 \
  --compare-orjson
```

## Artifacts

### Profiling Data

- `strata_loads.prof` - Python cProfile data (loadable with `python -m pstats`)
- `strata_cpp_profile.sample.txt` - macOS sample output (raw)
- `strata_cpp_profile.folded` - Folded stacks for flamegraph generation

### Analysis Documents

- `hotspot_analysis.md` - Comprehensive analysis with top hotspots
- `progress_log.md` - Baseline metrics and future tracking
- `flamegraph_text.txt` - Text-based flamegraph visualization

### Benchmark Results

- `bench_results_small.md` - Small dataset results
- `bench_results_medium.md` - Medium dataset results
- `bench_results_large.md` - Large dataset results

## Generate Flamegraph (SVG)

To generate an interactive SVG flamegraph:

```bash
# 1. Install FlameGraph tools
git clone https://github.com/brendangregg/FlameGraph /tmp/FlameGraph

# 2. Generate SVG
/tmp/FlameGraph/flamegraph.pl \
  docs/benchmarks/strata_cpp_profile.folded \
  > docs/benchmarks/flamegraph.svg

# 3. Open in browser
open docs/benchmarks/flamegraph.svg
```

## Comparison with Other Libraries

| Feature | strata | orjson | msgspec | ujson |
|---------|--------|--------|---------|-------|
| Speed | 137 MB/s | **377 MB/s** | ~350 MB/s | ~250 MB/s |
| SIMD | ✅ Yes | ✅ Yes | ✅ Yes | ❌ No |
| Structural Tape | ✅ Yes | ❌ No | ❌ No | ❌ No |
| Key Caching | ✅ Yes | ✅ Yes | ❌ No | ❌ No |

## Future Work

- [ ] Implement dictionary pre-sizing
- [ ] Add object pooling for common types
- [ ] Optimize structural tape collection
- [ ] Batch Python C API calls where possible
- [ ] Profile on different dataset types (nested, arrays, numbers-heavy)
- [ ] Add memory profiling (RSS, peak allocations)
- [ ] Compare against simdjson C++ library directly

## Questions?

See the [main README](../../README.md) or open an issue on GitHub.

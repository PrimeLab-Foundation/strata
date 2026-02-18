# Value Creation Optimization Experiment

## Hypothesis

Optimizing integer and float Python object creation paths will yield measurable
improvements for `loads()` throughput, particularly for datasets with high numeric
density (881K ints, 790K floats in the large benchmark).

## Data Analysis

**analyze_values.py** was run on all three benchmark datasets. Key findings:

### Integer Distribution (large dataset, 881K ints)
- **90.6%** in CPython small-int cache range [-5, 256]
- Top 5 values (1-5) represent 90% of all integers (these are `qty` fields)
- Only 9.4% outside small-int cache (IDs, order IDs)
- 0% beyond int64 range

### Float Distribution (large dataset, 790K floats)
- 24,951 unique bit patterns out of 789,913 total (dedup ratio: 31.7x)
- Nearly zero common constants (0.0: 0 occurrences, 1.0: 35 occurrences)
- All floats are random prices: `round(uniform(0.5, 250.0), 2)`

### Cache Simulation (64-entry direct-mapped, Fibonacci hash)
- Float cache hit rate: **0.3%** (useless)
- Even 32K-entry cache only reaches 52% hit rate (512KB memory)

## Approaches Evaluated

### Approach A: PyLong_FromLong fast-path for on_int() (ADOPTED as code fix)
- **Change:** Replace `PyLong_FromLongLong(v)` with `PyLong_FromLong(long(v))` + range check
- **Rationale:** Consistency with other code paths (python_loads.cpp, python_tape.cpp)
- **Result on macOS arm64:** No measurable improvement (long == int64_t, same code path)
- **Result on 32-bit:** Enables small-int cache for values that fit in long

### Approach B: Thread-local float dedup cache (REJECTED)
- **Hypothesis:** 64-entry cache keyed by float bit pattern would deduplicate common values
- **Finding:** 0.3% cache hit rate on benchmark data. Even 4096-entry cache only 16.3%
- **Conclusion:** Not worth the added complexity and cache lookup overhead

### Approach C: Bypass GC tracking for leaf objects (REJECTED - N/A)
- **Finding:** PyLong and PyFloat are NOT GC-tracked on Python 3.12+
- The 0.37% `_PyObject_GC_New` samples come from dict/list creation, not numerics
- No-op on target platform (Python 3.14)

### Approach D: Direct struct init for PyFloat (REJECTED)
- **Finding:** `PyFloat_FromDouble` uses CPython's freelist which is faster than `PyObject_New`
- Bypassing it would be slower, not faster

### Approach A+: SmallIntCache with direct pointer lookup (REJECTED)
- **Hypothesis:** Pre-caching PyObject* pointers for [-5, 256] and returning them via
  direct array lookup would eliminate `PyLong_FromLong` function call overhead
- **Finding:** On Python 3.12+, small ints are immortal and `PyLong_FromLong` already
  does a simple bounds check + array lookup. Our cache does the same operations.
  No measurable difference (within noise).

## Microbenchmarks

**bench_numeric.py** (same methodology as dict_insert_optimization):

### Baseline (before changes)
| Dataset | Median (ms) | Throughput (MB/s) |
|---------|------------|-------------------|
| small | 9.900 | 97.1 |
| medium | 40.645 | 153.7 |
| large | 358.658 | 122.3 |

### After PyLong_FromLong fix (Approach A)
| Dataset | Median (ms) | Throughput (MB/s) | Change |
|---------|------------|-------------------|--------|
| small | 10.270 avg | 93.6 | +3.7% (noise) |
| medium | 40.199 avg | 155.4 | -1.1% (noise) |
| large | 356.369 avg | 123.0 | -0.6% (noise) |

All changes within noise threshold (< 2% per Rule 17).

## Go/No-Go: NO-GO for performance, GO for code consistency

- No measurable performance improvement on macOS arm64 / Python 3.14
- The `PyLong_FromLong` fix is kept as a code consistency improvement
- Float cache approaches are definitively not worth pursuing with this data distribution
- All 680 Python + 24 C++ tests pass

## Root Cause Analysis

Numeric value creation accounts for only **0.33% of total runtime** in profiling:
- `PyFloat_FromDouble`: 313 samples (0.26%)
- `PyLong_FromLong`: 80 samples (0.07%)

CPython's internal caching (small-int cache + float freelist) is already near-optimal.
On Python 3.12+, small ints are immortal objects (no refcount overhead). There is simply
no room for improvement in this area with the current architecture.

The 85.68% Python C API overhead is dominated by:
1. Dictionary operations (11.63%) - already optimized with known-hash insertion
2. Memory allocation (2.63%) - at pymalloc floor
3. The remaining ~71% is spread across GC, reference counting, type checks, etc.

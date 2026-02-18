# Dict Optimization Experiment

## Focus Area

Dictionary operations are the bottleneck at **11.63% of total runtime**:
- `dict_dealloc`: 7.03% — dicts created, used, then discarded
- `PyDict_SetDefault`: 1.51%
- `dict_setdefault_ref_lock_held`: 1.39%
- `PyDict_New`: 0.46%

## Hypothesis

We can reduce `dict_dealloc` overhead by improving dict reuse patterns. Currently:
1. Object pool creates N pre-allocated dicts at parse start
2. Parser uses them during parsing
3. **Pool is drained** after parsing, decref'ing unused dicts
4. Next parse repeats this cycle

**Improvement:** Cross-parse dict pooling (reuse dicts across multiple parse calls)

## Approach 1: Global Per-Thread Dict Pool (Low Complexity)

Extend the per-parse pool to a per-thread long-lived pool:
- Keep dicts in a "free list" after parsing completes
- Reuse them on the next parse call
- Only deallocate dicts if pool grows too large
- Reduces alloc/dealloc churn dramatically

**Implementation:**
- Create `thread_local g_global_dict_pool` (persistent across parses)
- Instead of draining pool: move unused dicts to global pool
- On next parse: fill local pool from global pool before allocating new ones
- Threshold: keep max 200 dicts in global pool per thread

**Expected Improvement:** 2-3% of 11.63% dict overhead = 0.23-0.35% total

**Risk:** Low — no changes to dict insertion logic, only pool lifecycle management

## Approach 2: Compact Dict Builder (Medium Complexity)

Current approach: insert keys one-by-one into pre-sized dict (causes incremental rehashing)

Better approach: batch accumulate keys and values, finalize dict once at end-of-object

**Implementation:**
- Instead of pushing to dict immediately, accumulate in arrays
- At `on_end_object()`: call bulk finalizer to create dict with all items at once
- Reduces dict rehashing from N (incremental) to 1 (final)

**Expected Improvement:** 1-2% of 11.63% dict overhead = 0.12-0.23% total

**Risk:** Medium — requires refactoring SAX handler state machine; complex with duplicate key policies

## Selected Approach: #1 (Global Per-Thread Dict Pool)

**Why:**
- Simple to implement (< 100 lines of code)
- Low risk (pool logic only, no parsing changes)
- Expected 0.23-0.35% total improvement
- Builds on existing infrastructure

## Implementation Plan

1. Create persistent global dict pool in `python_loads.cpp`
2. Add `move_to_global_pool()` and `fill_from_global_pool()` methods
3. Modify `PythonObjectPool::drain()` to move unused dicts instead of deallocing
4. Add tests to verify dict reuse works correctly
5. Benchmark on small/medium/large datasets

## Benchmarking

Baseline: Will capture from commit before changes
Post: Run canonical benchmark suite to measure improvement
Target: > 0.2% improvement on large dataset (123.2 MB/s → 123.5 MB/s)

## Files to Modify

- `src/strata/bindings/python_loads.cpp` — Add global pool
- `src/strata/bindings/python_object_builder.h` — Add pool movement methods
- Benchmark files as needed

## Expected Outcome

- Small (~1% improvement): noise; abort and try Approach #2
- Medium (0.2-0.5% improvement): SUCCESS; document and keep
- Large (>1% improvement): unlikely but great if achieved

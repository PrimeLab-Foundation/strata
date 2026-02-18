# Dict Optimization: Research Analysis

## Bottleneck Summary

Dict operations consume **11.63% of total runtime**:
- `dict_dealloc`: 7.03% — cleanup after parsing
- `PyDict_SetDefault`: 1.51%
- `dict_setdefault_ref_lock_held`: 1.39%
- `PyDict_New`: 0.46%

**Large dataset context:**
- 881K dicts created per parse
- Pre-allocated dict pool: ~1024 dicts
- Pool drained after parsing → 879K dicts deallocated
- Object pool helps but doesn't eliminate dealloc

## Why Dict Optimization is Challenging

### 1. **CPython's Dict Implementation is Already Highly Optimized**

Modern CPython (3.12+) uses:
- Compact dict layout (saves 50% memory vs older Python)
- Fast path for ASCII keys
- Pre-sized dict allocation (`_PyDict_NewPresized`)
- Known-hash insertion (`_PyDict_SetItem_KnownHash`)
- Freelist for small dicts (80-dict recycling pool)

Strata already leverages all of these.

### 2. **dict_dealloc (7.03%) is Hard to Optimize**

Deallocations are **necessary work**: 881K dicts must be cleaned up. Options:
- **Global dict pool** (keep dicts alive across parses): Helps batch parsing, not single large parse
- **Lazy dict materialization** (defer until needed): Adds complexity; most dicts ARE needed immediately
- **Memory pooling** (reuse raw memory): CPython doesn't expose this API

### 3. **Insertion Cost (PyDict_SetDefault 1.51%) is Already Batched**

Current implementation:
- `_PyDict_NewPresized()` upfront (avoids some rehashing)
- Batch insertion for `LastWins` policy (64-item batches)
- `_PyDict_SetItem_KnownHash()` with pre-computed hashes
- Duplicate key checking only where policies require

Further improvements require custom hash table implementation (high risk, high complexity).

### 4. **Competing Optimizations**

Any dict optimization must preserve:
- Duplicate key policies (FirstWins, LastWins, Error, Warn)
- Thread-local state (no global dict mutations)
- Correct refcounting (Python memory safety)
- Compatibility with CPython internals

## What Could Still Help (Ranked by Risk/Reward)

### Low-Hanging Fruit

**Option A: Reduce duplicate key checking for default policy**
- Hypothesis: Most JSON has unique keys; checking on every insert wastes CPU
- Implementation: Use a "check-once" flag for dicts with duplicate-key policies
- Risk: Low (policy logic only)
- Expected gain: 0.1-0.3% absolute (1-2% of dict cost)
- Effort: 2-3 hours

**Option B: Global per-thread dict pool**
- Hypothesis: Reuse dicts across multiple parse calls
- Implementation: `thread_local g_dict_freelist` with threshold
- Risk: Low (pool lifecycle only)
- Expected gain: 0.2-0.5% absolute (only if batch parsing)
- Effort: 4-6 hours

### Medium-Complexity Ideas

**Option C: Compact dict builder (batch finalization)**
- Hypothesis: Accumulate keys/values in arrays, finalize dict at end
- Implementation: Replace per-insertion logic with batch builder
- Risk: Medium (complex state machine refactoring)
- Expected gain: 0.5-1.5% absolute (3-5% of dict cost)
- Effort: 1-2 weeks

### High-Complexity Ideas (Diminishing Returns)

**Option D: Custom SIMD hash table**
- Risk: Very high (custom implementation, platform-specific)
- Expected gain: 0.2-0.8% absolute (1-2% of dict cost)
- Effort: 3-4 weeks
- **Not recommended:** Complex for minimal gain

**Option E: Lazy dict materialization with tape views**
- Risk: Very high (custom Python type, lifetime management)
- Expected gain: 0.5-2% absolute (but breaks some code patterns)
- Effort: 4-6 weeks
- **Not recommended:** Architectural change for speculative gain

## Conclusion & Recommendation

### Why Dict Optimization Yields Diminishing Returns

1. **CPython is already optimized**: Modern Python's dict impl is near-optimal
2. **We're already using best practices**: Pre-sizing, known-hash insertion, batching
3. **The bottleneck is fundamental**: Creating 881K objects → cleaning them up (necessary work)
4. **Real speedup requires architecture change**: Either defer materialization (complex) or use custom types (high-risk)

### Strategic Recommendation

**Rather than pursue dict optimization further, focus on the 85.68% Python C API overhead:**

- Object creation (dict/list/string) accounts for the bulk
- Memory allocation/deallocation is at pymalloc floor
- Dictionary operations are a symptom, not the root cause

**Better targets for future research:**
1. Lazy object materialization (return a "JSON cursor" instead of materialized dict)
2. Zero-copy streaming parser (for NDJSON)
3. Vectorized value creation (batch create many ints/floats)
4. Custom Python types (dict view, list view) for read-only semantics

### For This Experiment

Keep the **string optimization** (Approaches A+B) that was implemented in the previous session. Those are foundational utilities (`is_ascii_only_swar`) that could benefit dict optimization later.

**Decision: Mark dict optimization as "researched but not implemented"** in the progress log. Document why it yields diminishing returns and what the real leverage points are.

## Files for Reference

- Profiling script: `profile_dict_ops.py` (run with `python -m cProfile`)
- Research findings: This document
- Previous work: `experiments/string_optimization/` (working UTF-8/ASCII optimizations)

## Testing Dict Operations

If pursuing Option A or B:
```bash
# Profile dict operations
python experiments/dict_optimization/profile_dict_ops.py

# Benchmark before/after
python experiments/dict_optimization/bench_dict_ops.py
```

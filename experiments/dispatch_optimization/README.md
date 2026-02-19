# Dispatch Optimization Experiments

**Date:** 2026-02-19
**Branch:** main-v2-0.1

## Objective

Reduce virtual-dispatch and parser-dispatch overhead in `loads()` to improve
throughput on medium/large JSON inputs.

Four candidates from the task brief were evaluated:

---

## Approach A: CRTP / Static Dispatch (IMPLEMENTED — ZERO GAIN)

### Hypothesis
Replace `JsonSaxHandler& handler` (virtual vtable calls) with a template
`Handler& handler` so `PythonObjectBuilder::on_key`, `on_string`, `on_int`
etc. become direct, inlinable calls.

### Implementation
- New header: `include/strata/detail/json_parse_impl.hpp`
  - `template<typename Handler> struct Parser<Handler>` — exact copy of the
    production `Parser` struct, but with `Handler& handler` instead of
    `JsonSaxHandler& handler`.
  - `template<typename Handler> Status parse_sax_impl_t(...)` — mirrors
    `parse_sax_impl` using static dispatch.
- `python_loads.cpp`: include the detail header; main `parse_sax` call changed
  to `strata::detail::parse_sax_impl_t(... g_parse_builder ...)`.
- Public `strata::parse_sax(JsonSaxHandler&)` API unchanged.

### Result: NO-GO (0% improvement)

**Why it doesn't help on Apple Silicon (arm64 -O3):**

1. **Monomorphic call-site prediction.** The virtual calls in `parse_string()`
   (three `blr xN` for `on_key`/`on_string`) are always dispatched to the same
   concrete target (`PythonObjectBuilder`). Apple M-series branch predictors
   handle this perfectly at ~1 cycle latency — same as a direct call.

2. **Python C API dominates (85.68% of runtime).** Each `on_string` spends
   hundreds of cycles in `PyUnicode_New` / memcpy / `_PyDict_SetItem_KnownHash`.
   Saving 2 cycles on the vtable load is unmeasurable.

3. **Compiler already outllines.** Even with the template, Clang -O3 outllines
   `Parser::parse_string`, `parse_number`, etc. into separate sub-functions —
   the same structure as the virtual path. Full inlining of the 30 KB function
   body does not occur in practice.

Assembly evidence:
- `parse_sax_impl` (virtual): 590 instructions, 4 blr (vtable)
- `parse_sax_impl_t<PythonObjectBuilder>` (template): 244 instructions in
  entry stub, then outllined sub-functions — identical structure.

**The CRTP change was committed separately** (commit `6a0c6e7`) by the previous
session for minor code-cleanliness; it carries zero benchmark impact.

---

## Approach B: Fused Parse-and-Build (NO-GO — no coverage)

### Hypothesis
Extend the existing `parse_flat_object()` fast path (≤1KB ASCII flat objects)
to cover arrays-of-objects and other common patterns.

### Analysis
`parse_flat_object` triggers only when the **entire input** is ≤1024 bytes and
ASCII-only. On all three benchmark datasets (1MB / 6.5MB / 46MB) it never
fires — 0% coverage. Extending the threshold would require a full structural
scan just to determine eligibility, negating any savings.

For the benchmark data shape (one top-level dict → large array → objects with
nested children), there is no single-pass fused path that avoids the full
recursive parser without essentially reimplementing it.

**Verdict: NO-GO.** The implementation complexity is high and coverage on
real-world shaped data is near-zero.

---

## Approach C: Computed Goto (NO-GO — already a jump table)

### Hypothesis
Replace `switch(ContainerState)` in `process_array_state` and
`process_object_state` with a computed-goto dispatch table.

### Analysis
Clang -O3 already compiles a 4-arm `switch` over a `uint8_t` enum into a
branch-table (equivalent to computed goto). Disassembling `start_array` shows
the same sequence: table base load → indexed jump.

`__builtin_expect` on the hot arm (`ExpectCommaOrEnd`) is already implicit from
PGO-like inlining hints. Computed goto would produce identical machine code and
adds non-portable `__extension__` syntax.

**Verdict: NO-GO.** Clang handles this automatically.

---

## Structural Tape: Discovered Regression (FIXED — +17.8% on large)

### Discovery
While benchmarking the CRTP change (no effect), a control variable sweep
revealed that `STRATA_USE_STRUCTURAL_TAPE` was set to `true` by default and
was **actively harming large-input performance**.

### Root Cause

For a 46 MB JSON input:
- The tape pre-scan collects every structural character (`{}[],:"`) position.
- Structural density ≈ 30% → ~13.8 M structural positions.
- Each stored as `size_t` (8 bytes) → **~110 MB of tape data**.
- The tape is built once (sequential SIMD write) and then **read randomly**
  during parsing as the parser jumps to each next structural position.
- This 110 MB read footprint **completely evicts L2/L3 cache** (~12–24 MB on
  Apple Silicon), causing cache misses on every tape access.
- The SIMD sequential scan (`find_next_structural_simd`) over the hot-in-cache
  46 MB JSON is far cheaper than reading 110 MB of cold tape.

### Search / Query — NOT affected
`python_search.cpp` already hardcodes `use_structural_tape = false` for its
parse path. Search performance is identical with tape=0 and tape=1 (verified).

### Fix
Changed default in `use_structural_tape_for_python()`:
```cpp
static bool cached = false;  // was: true
bool value = false;           // was: true
```
`STRATA_USE_STRUCTURAL_TAPE=1` still enables it for workloads that benefit
(e.g. a single cold pass over a >100 MB file where tape amortises well).

### Comprehensive Tape Benchmark — All Operations (30 iterations, parallel processes)

Results confirm tape=OFF is optimal for **every** public API operation:

| Operation | Dataset | tape=0 (ms) | tape=1 (ms) | Δ | Decision |
|-----------|---------|------------|------------|---|----------|
| loads(bytes) | small (1 MB) | 10.69 | 10.80 | +1.0% | tape=OFF (tape not collected <10MB) |
| loads(bytes) | medium (6.6 MB) | 43.50 | 43.43 | ~0% | tape=OFF (tape not collected <10MB) |
| **loads(bytes)** | **large (46 MB)** | **314.74** | **386.11** | **+22.7%** | **tape=OFF — BIG WIN** |
| load(path) | small (1 MB) | 10.63 | 10.71 | ~0% | tape=OFF |
| **load(path)** | **large (46 MB)** | **318.45** | **391.39** | **+22.9%** | **tape=OFF — BIG WIN** |
| dumps(obj→str) | all datasets | 7.38–435 | 7.37–437 | ~0% | N/A — no parse in dumps |
| dump(file,obj→bytes) | all datasets | 7.49–436 | 7.43–438 | ~0% | N/A — no parse in dump |
| search[*] | all datasets | same | same | ~0–1.7% noise | N/A — hardcoded OFF in C++ |
| query[*] | all datasets | same | same | ~0–1.2% noise | N/A — no parse in query |

**Key findings:**
- `loads`/`load` with ≥10 MB inputs: **tape=OFF is 22–23% faster** (L3 cache thrashing from ~110 MB tape)
- `loads`/`load` with <10 MB inputs: tape is never collected regardless of the setting
- `dumps`/`dump`: no JSON parsing → tape has zero effect
- `search`: `python_search.cpp:516` hardcodes `use_structural_tape = false` → env var is irrelevant
- `query`: operates on a pre-parsed Python object → no parsing occurs → tape is irrelevant

**Conclusion: `tape=false` is the universally correct default for all operations.**

### Files Changed
- `src/strata/bindings/python_loads.cpp` — tape default → `false`

---

## Control Variables

| Variable | Default | Effect |
|----------|---------|--------|
| `STRATA_USE_STRUCTURAL_TAPE` | `0` (off) | Enable structural tape collection |
| `STRATA_PYTHON_EXACT_SIZE_HINTS` | `auto` (on for ≥1MB) | Two-pass exact sizing |
| `STRATA_DEFERRED_GC_TRACK` | `1` (on for ≥256KB) | GC untrack/retrack |

---

## Benchmark Scripts

- `bench_dispatch.py` — loads() latency; accepts `[dataset] [n]`
- `bench_search_tape.py` — search() tape=0 vs tape=1 comparison
- `bench_tape_comprehensive.py` — all operations (loads/load/dumps/dump/search/query) tape comparison

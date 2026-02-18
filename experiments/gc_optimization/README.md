# GC Optimization Experiment

**Branch:** `main-v2-0.1`
**Date:** 2026-02-19
**Status:** Implemented — Approach A deployed

## Problem Statement

During `loads()` on large JSON inputs, every `PyDict_New()` and `PyList_New()` call inserts
the new object into CPython's gen0 doubly-linked GC list (~5 pointer stores + 1 counter
increment ≈ 43 cycles per object).  For the large dataset (43.85 MB) this creates 968K
GC-tracked objects during parse.

Even with `PyGcPause` (which disables the GC *scanner*), the gen0 *insertion* still fires
on every allocation.  CPython's `gc.disable()` only suppresses collection triggers; it does
NOT suppress `_PyObject_GC_TRACK`.

## Approaches Researched

### Approach A: Delayed GC Tracking (IMPLEMENTED)

**Hypothesis:** Untrack dicts/lists immediately after creation and re-track them just before
handing to their parent container.  During construction we hold the only reference, so GC
cannot observe incomplete objects.

**Implementation:**
- `PythonObjectBuilder::on_start_object()`: call `PyObject_GC_UnTrack(dict)` after creation
- `PythonObjectBuilder::on_start_array()`: call `PyObject_GC_UnTrack(list)` after creation
- `PythonObjectBuilder::on_end_object()`: call `PyObject_GC_Track(dict)` before `push_value()`
- `PythonObjectBuilder::on_end_array()`: call `PyObject_GC_Track(list)` before `push_value()`
- Controlled by `STRATA_DEFERRED_GC_TRACK` env var (default: enabled)
- Activated only for inputs ≥ 256 KB (`kDeferredGcTrackMinSize`)

**Safety:** `PyObject_GC_UnTrack` and `PyObject_GC_Track` are public CPython APIs (PyAPI_FUNC).
Both are idempotent and safe to call on any GC-trackable object.

### Approach B: GC Threshold Manipulation

**Hypothesis:** `gc.set_threshold(0,0,0)` disables GC without the gen0 list insertion problem.

**Result:** Same problem — `gc.disable()` and `gc.set_threshold(0,0,0)` both still insert
objects into gen0 on creation.  On Python 3.14 (incremental GC, PEP 794), the gen0 sweep
of millions of objects has negligible latency (~0ms for 968K objects).  **NO-GO.**

### Approach C: `_PyDict_SetItem_Take2` Reference Stealing

**Hypothesis:** Replace `_PyDict_SetItem_KnownHash(dict, key, val, hash)` + 2×DECREF with
`_PyDict_SetItem_Take2((PyDictObject*)dict, key, val)` (steals both references, no hash).

**Result:** Most common JSON keys (`id`, `name`, `type`, `value`, `data`, `status`, etc.)
are *immortal* in Python 3.14 — INCREF/DECREF on them are no-ops.  The savings come only
from non-common cached keys (~30-50% of key insertions).  Trading KnownHash for Take2
gives marginal gains and loses hash-skip optimization.  **NO-GO** (insufficient benefit
to justify losing KnownHash).

### Approach D: Systematic Refcount Audit

**Result:** The hot path (LastWins, batched) already uses `_PyDict_SetItem_KnownHash` which
is optimal.  Common keys are immortal on 3.14 → INCREF/DECREF are no-ops.  **NO-GO**
(already optimal).

## Performance Results

Tested with 50 iterations, separate processes for clean state.

| Dataset | Baseline (ms) | With Deferred GC (ms) | Δ Median |
|---------|--------------|----------------------|----------|
| small (1MB) | min=9.7, med=10.9 | min=9.5, med=10.1 | **+7.3%** |
| medium (6.25MB) | min=38.9, med=40.6 | min=38.5, med=39.5 | **+2.7%** |
| large (43.85MB) | min=344.5, med=350.4 | min=343.5, med=349.6 | +0.2% (noise) |

**Root cause of asymmetry:** The improvement scales with the fraction of time spent in
Python C API calls relative to total runtime.  For medium inputs, GC tracking overhead is
a larger fraction; for large inputs dominated by SIMD parsing, the savings are smaller.

## Test Results

- 680 Python tests: ✅ PASS
- 24 C++ tests: ✅ PASS
- GC correctness (gc.DEBUG_LEAK): ✅ PASS (0 leaks)
- Result objects fully GC-tracked before return: ✅ verified

## Control Variables

```bash
STRATA_DEFERRED_GC_TRACK=0  # Disable (revert to baseline)
STRATA_DEFERRED_GC_TRACK=1  # Enable (default)
```

# ADR 0006: Search and NDJSON Performance Optimizations

## Status
Proposed

## Context
Strata search performance is significantly slower than jmespath on wildcard queries and NDJSON processing.
Key bottlenecks identified:
1. Double materialization: Search results are materialized into C++ `JsonValue` (DOM) and then converted to Python objects.
2. Excessive overhead in Python conversion: `json_value_to_python` creates a new `KeyCache` and `Arena` for every single result.
3. NDJSON overhead: Per-line parsing into DOM before search evaluation.
4. Nested loop overhead in wildcard traversal.
5. Inefficient result collection in NDJSON: Repeated `PyList_Append` calls.

## Decision
We will implement the following optimizations:

### 1. Zero-Copy Search Results
Modify the search evaluator to return `JsonCursor` (pointers to original DOM nodes) instead of `JsonValue` (deep copies). This avoids C++ DOM materialization.

### 2. Optimized Python Conversion
Re-use `KeyCache` and `Arena` across all search results. Use the optimized `PythonObjectBuilder` logic (pools, batches) where possible.

### 3. Fused Wildcard Traversal
Fusing consecutive `Wildcard` and `Field` operations in the search evaluator to reduce recursion depth and overhead.

### 4. Direct-to-Python NDJSON Search
For simple field extractions in NDJSON, use a SAX-based approach that builds Python objects directly, bypassing the intermediate C++ DOM.

### 5. Batch Result Collection
Collect `PyObject*` results in a `std::vector` and create the final Python list in a single operation to avoid repeated `PyList_Append` overhead.

## Alternatives
- Continue using `JsonValue` but optimize its materialization (e.g., using an arena). This still incurs double materialization (C++ DOM then Python objects).
- Full "Direct-to-Python" search evaluator. This would violate Rule 2.2 (Core Purity) as the core evaluator would depend on `Python.h`.

## Risks and Mitigations
- **Complexity**: Fused operations add complexity to the evaluator. Mitigation: Strict parity tests.
- **Memory**: Returning cursors assumes the underlying document remains valid. Mitigation: This is already guaranteed by the Python binding's ownership model.
- **Python Refcounting**: Batch collection requires careful refcount management. Mitigation: Use RAII guards where possible.

## Test Plan
- Run existing search tests to ensure no regressions.
- Add new tests for fused wildcard paths.
- Performance validation using `benchmarks/bench_search.py` and `benchmarks/bench_ndjson_cursor.py`.

## Benchmark Plan
Target: 2-3x improvement on wildcard queries.
Baseline: ~995ms for double-wildcard.
Target: <400ms.

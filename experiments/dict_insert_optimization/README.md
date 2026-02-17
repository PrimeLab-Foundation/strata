# Dict Insert Optimization Experiment

## Hypothesis

Replacing `PyDict_SetItem` with `_PyDict_SetItem_KnownHash` in the dict insertion hot path
will eliminate redundant hash computation, yielding 2-8% improvement on `loads()` throughput.

## Prototype

Three approaches were evaluated:

### Approach A: Replace PyDict_SetItem with _PyDict_SetItem_KnownHash (ADOPTED)
- Pre-compute `Py_hash_t` via `PyObject_Hash(key)` when the key is obtained from `KeyCache`
- Store the hash alongside the key in `key_hashes_` vector and `DictBatch.hashes[]`
- Use `_PyDict_SetItem_KnownHash(dict, key, val, hash)` everywhere instead of `PyDict_SetItem`
- Also replaced `PyDict_SetDefault` (FirstWins path) with explicit check + insert using known hash

### Approach B: _PyDict_SetItem_Take2 for reference stealing (NOT ADOPTED)
- Would eliminate INCREF/DECREF round-trips by stealing references
- Rejected because `_PyDict_SetItem_Take2` computes hash internally, so cannot combine
  with known-hash optimization. The hash elimination provides more benefit than ref stealing.

### Approach C: Full-object batch with _PyDict_FromItems (NOT ADOPTED)
- Would build entire dict from arrays in one shot
- Rejected because `_PyDict_FromItems` still does INCREF per item internally, and the
  existing 64-item batch already amortizes overhead well enough.

## Microbenchmarks

See `bench_dict_insert.py` (synthetic) and `bench_rigorous.py` (real datasets).

## Go/No-Go: GO

- Small: -4.8% (exceeds 3% threshold)
- Medium: -2.7% (meets 2% minimum)
- Large: -3.6% (exceeds 3% threshold)
- All 680+24 tests pass
- No memory leaks

# P21 experiment: out-of-line sequence traversal

P17 samples a global dict-type address load inside sequence traversal. P20
proves that an explicit local alias produces identical instructions under a
fixed profile. The register allocator already knows the address; source-level
hoisting alone does not remove the load.

P21 keeps write_sequence out of the general writer with STRATA_NOINLINE_HOT.
This changes only the compiler's inlining boundary. Traversal order, bounds
reloads after callbacks, deferred cycle registration, scalar runs, output
allocation and error paths remain identical. No new state or allocations are
introduced. Existing recursion and re-entrant mutation tests cover semantics.

The hypothesis is that a separate sequence function reduces general-dispatch
footprint and separates its live registers from scalar dispatch. The risk is
one extra call per sequence, plus changed PGO layout and spills. This is
independent of rejected type-argument and fused-record expansion experiments.
Inspect fresh matched PGO codegen and paired/A/A results before full canonical
and native gates. Keep the patch isolated unless all acceptance gates pass.

Outcome: PGO correctness passes and codegen confirms the smaller writer and
hoisted global type address. Paired/A/A screening finds no mixed gain and a
resolved users-str slowdown. The prototype remains isolated in
`experiments/benchmark-sequence-outline.patch`; no integration is qualified.

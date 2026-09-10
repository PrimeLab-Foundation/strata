# P19 experiment: sequence item type snapshot

The general serializer already reads the object's type once on entry. P17's
sample at f494 is a GOT load of PyDict_Type, not a repeated ob_type load.
Do not claim that rewriting the general dispatch alone removes that sample.

The heterogeneous sequence loop separately checks an item's type for deferred
cycle registration and exact-dict dispatch, then calls the general writer.
P19 tests passing the current item's type through these decisions and into
an overload of the existing writer. The normal one-argument entry still
loads the current type on every call. Only the sequence loop uses the supplied
type; fused record and mapping traversal retain their existing entries.

Capture the type after output growth and item selection. Between capture and
writer dispatch, plain-scalar classification only reads exact-type/value data,
and DeferredOpen::arm only appends a borrowed pointer to a C++ vector. Neither
runs Python callbacks. No snapshot survives a writer call or loop iteration.
The existing user_steps check still reloads sequence bounds after callbacks;
large integers remain non-plain and retain their lifetime protection.
Exceptions and cycle handling follow the same paths as before.

A new argument can increase register pressure or prevent useful inlining.
Keep this prototype isolated unless PGO codegen establishes removal of a
load without material growth, followed by matched paired/A/A screening and
unchanged canonical/native gates. Existing mutation, cycle, scalar-subclass
and float oracle suites remain the correctness gates.

Outcome: both PGO test phases pass, but the larger writer/frame produces no
resolved gain in paired/A/A screening. Rejected for integration; retained in
`experiments/benchmark-sequence-type-snapshot.patch` with evidence in the ledger.

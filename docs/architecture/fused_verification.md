# P22 experiment: omit redundant fused-row checks

The fused writer accepts only combined unicode dictionaries through the
existing raw-layout probe. Before examining schema keys it requires
entry_count == PyDict_GET_SIZE(object). entry_count is dk_nentries;
ma_used counts occupied entries. Equality therefore excludes deletion holes
in the inspected prefix. CPython uses the same equality to identify a clean
combined dictionary in dict_merge (Objects/dictobject.c, 3.14):
https://github.com/python/cpython/blob/3.14/Objects/dictobject.c

No Python callbacks occur between this check and completion of the staged
value row: schema/staged storage growth uses C++ allocation, matching uses
pointer identity, and the GIL stays held. The per-value nullptr checks add no
protection here. Dictionaries with holes still take write_mapping at the
entry-count check. The complete row is still copied before any emission.

Schema selection matches count and first_keys\[way\] against the first entry.
DepthSchemas::select publishes those fields after Schema::remember copies
the key row; invalidation clears the count before releasing keys. Thus a
prepared selected way already proves key_row\[0\] equals the first key.
Copy value zero directly and verify/copy entries starting at index one.
The nonempty size gate covers that copy, including one-key records.

P22 removes only these redundant checks. It retains every tail-key check,
the pre-emission snapshot, deferred container registration, row locking,
callback latching and all fallback paths. Test deletion/reinsertion layouts,
one-key records, equal-first-key differing tails, and existing re-entrant
mutation suites. Keep the candidate isolated until matched PGO timing and
all canonical/native gates pass.

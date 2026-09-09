# P18 experiment: bit-indexed significand digit count

The native P17 interleaved annotation places 3.67% of the general writer's
sampled period on the comparison following its dependent power-of-ten load.
This is a hypothesis location, not proof of cache-miss causality. P18 tests
replacing the integer log approximation and dependent threshold lookup after
Dragonbox with one bit-indexed row containing the base count and threshold.
It does not change conversion, trailing-zero removal, layout or output stores.

For a positive integer with highest bit b, its range is \[2^b, 2^(b+1)-1\].
That range can cross at most one decimal boundary because its endpoints have
ratio less than two. Each constexpr row stores the decimal length of 2^b and
the next power of ten. The answer is that base plus the comparison against
the threshold. The bit-63 row uses UINT64_MAX as its upper bound and the
representable threshold 10^19. A compile-time division-based oracle verifies
the endpoints of all 64 buckets. Zero is handled before Dragonbox and cannot
reach this helper. The existing shortest-output oracle over 20 million double
bit patterns remains the runtime correctness gate.

The private table occupies 1 KiB, versus the existing smaller power table.
That footprint can lose under rival interleaving. Keep the generic integer
counter and micro-decimal path unchanged. Require a codegen check that the
multiply/dependent-index sequence is removed, then matched PGO paired/A/A
screening and unchanged canonical gates before any integration. The prototype
remains isolated until those gates pass.

P18 outcome: the codegen and correctness checks pass, but the full canonical
small-tier regression gate fails. This design is an isolated experiment,
not the production digit counter; see the ledger for paired and gate results.

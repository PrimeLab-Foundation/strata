---
alwaysApply: true
---
# Rule: Build Gates + Cross-Layer Test Mirroring + Coverage

Build and release are gated by tests on both sides (C++ and Python). If tests fail, the build must fail.

## Build gate (hard requirement)
- The build pipeline MUST run **C++ tests** as part of the build gate.
- If **any** C++ test fails, the build MUST exit non-zero and be considered failed.
- The build pipeline MUST also run **Python tests**; failures must also fail the build.
- No merging/tagging/releasing with failing tests.

## Cross-layer test mirroring (mimic both ways)
For every **main** behavioral test in one layer, there must be an equivalent test in the other layer:
- Python → C++:
  - Any user-visible behavior tested in Python must have a C++ equivalent validating the same contract at core level.
- C++ → Python:
  - Any core contract/edge case tested in C++ must have a Python test ensuring wrapper parity (outputs + error semantics).

Mirrored tests should validate:
- same observable outputs
- same error categories and stable messages where feasible
- same edge-case handling and policy decisions (NaN/Inf, UTF-8 policy, etc.)

## Coverage goal (100% target)
Strata targets **100% coverage** on both sides:
- C++: line/branch coverage for core and binding-relevant code paths
- Python: line coverage for wrapper + public API behavior

## Coverage enforcement
- CI MUST collect and report coverage for both languages.
- Coverage regressions are treated as failures.
- New or modified code MUST include tests that keep coverage at target.

## Practical guardrails
- Any exclusions must be:
  - minimal,
  - explicitly documented in-repo,
  - justified with a tracked issue,
  - reviewed and time-bounded (plan to remove exclusion).
- Coverage must reflect **meaningful execution**, not synthetic “touch lines” tests.
- Do not skip/xfail tests to maintain coverage; fix the underlying issue.

## Recommended artifacts
- A single “gate” command (e.g., `make gate` or `./scripts/gate.sh`) that runs:
  1) build
  2) C++ tests
  3) Python tests
  4) coverage collection and threshold checks
and fails fast on any error.

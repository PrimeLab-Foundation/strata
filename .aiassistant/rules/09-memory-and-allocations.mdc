---
alwaysApply: true
---
# Rule: Memory and Allocations Are a First-Class Constraint

Performance is not enough; memory efficiency is a core deliverable.

## Requirements
For hot-path changes, explicitly reason about:
- allocation count and allocation size
- ownership/lifetimes (RAII)
- avoiding O(n²) behavior
- minimal includes and low coupling
- view/zero-copy safety constraints

## Structural rule
Avoid “god files”:
- If a file exceeds ~800 LOC, split by responsibility unless there is a compelling, documented reason.

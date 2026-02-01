---
alwaysApply: true
---
# Rule: C++ Owns CPU Work

Strata is a **C++20 JSON engine** with a **Python wrapper**. All CPU-heavy logic must be implemented in C++.

## Requirements
- Implement in C++20 (core):
  - JSON parsing (`loads`)
  - NDJSON iteration/streaming
  - Serialization (`dumps`)
  - View/slice/zero-copy mode (where applicable)
  - Search/query evaluation
- Python is a **thin wrapper only**:
  - argument normalization
  - dispatch into bindings
  - exception mapping
  - return results
- **Forbidden:** implementing parsing/search/slicing in Python “temporarily”. If not implemented in C++, raise `NotImplementedError`.

## Source of truth
See project operating contract and boundaries.

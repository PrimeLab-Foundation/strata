---
alwaysApply: true
---
# Rule: Error Handling Consistency (C++ ↔ Python)

Errors must be consistent, debuggable, and mapped at the binding boundary.

## Requirements
- C++ hot paths:
  - prefer project `Result<T>` / error-code style over exceptions
  - keep exceptions out of tight loops
- Binding layer:
  - translate C++ errors into Python exceptions
  - preserve meaningful context (offsets, path, reason)
- Python:
  - raise consistent exception types (`ValueError` or project `StrataError` if defined)
  - messages must be actionable and stable for tests

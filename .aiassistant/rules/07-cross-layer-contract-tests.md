---
alwaysApply: true
---
# Rule: Cross-Layer Contract Tests (Mimic Behavior)

Where functionality exists through Python bindings, Python-visible behavior must match the C++ core contract.

## Requirements
For any core behavior exposed via Python (`loads/dumps/ndjson/search/jsonpath/...`):
- Add/maintain mirrored tests:
  - C++ tests: core correctness + edge cases
  - Python tests: parity + wrapper semantics (types, exceptions, lifetimes)
- Python tests should assert parity with the C++ behavior:
  - same outputs
  - same error categories (exception types) and messages where feasible
  - same edge-case handling

## Contract coverage checklist
When applicable, tests must cover:
- `.json` and `.ndjson`
- Unicode handling and escaping
- number parsing boundaries; NaN/Infinity policy (if supported)
- ordering rules (if promised) and whitespace tolerance
- malformed input, truncated input, invalid UTF-8, overflows
- search semantics: missing keys, nested paths, arrays, nulls, duplicates

---
alwaysApply: true
---
# Rule: No Dependency Fallbacks

Strata core must be standalone and must not route core functionality through other JSON engines.

## Requirements
- **Forbidden** in core:
  - depending on other JSON engines (rapidjson/simdjson/orjson/ujson/etc)
  - fallback paths delegating loads/dumps/search to other libs
  - using the `simdjson` Python package
- Competitor libraries are allowed **only** inside benchmark harnesses for comparison.

## Rationale
Avoid hidden complexity and ensure Strata’s performance claims are real and attributable to Strata.

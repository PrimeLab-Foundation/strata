---
alwaysApply: true
---
# Rule: Public API Stays Small and Stable

Avoid API sprawl. Every new public symbol is a product commitment.

## Requirements
- Keep the Python surface small and consistent. The entire public Python API is:

```
strata.load(source, **kwargs)         -> object
strata.dump(target, obj, **kwargs)    -> int
strata.loads(source, **kwargs)        -> object
strata.dumps(obj, **kwargs)           -> str | bytes | bytearray
strata.search(source, expr, **kwargs) -> list
strata.query(data, expr, **kwargs)    -> list

strata.compile_path(expression)       -> CompiledPath
```

No other public functions in `strata.__init__`. Configuration helpers
(`set_cycle_policy`, `set_duplicate_key_policy`, `set_dumps_type_order`) live in
`strata.serialize`.
- Any new public API requires:
  1) C++ tests + Python tests
  2) docs update under `docs/`
  3) benchmark coverage if performance-relevant
- Any mode offered by Python must correspond to a real C++ implementation.
  - If missing in C++, raise `NotImplementedError` (no Python emulation).

## Binding discipline
- Bindings should be minimal and explicit; map C++ errors to Python exceptions at the boundary.

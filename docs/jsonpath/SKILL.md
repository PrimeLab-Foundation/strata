---
name: jsonpath
description: JSONPath subsystem — supported grammar, compile/eval architecture, the three live evaluators (C++ DOM, PyObject-native, SAX streaming), known semantic divergences, and unsupported syntax. Load before touching src/strata/search/, python_jsonpath.cpp, or python/strata/jsonpath.py.
---

# JSONPath / Search

## Grammar (subset of RFC 9535)

Compiler: hand-written recursive descent `PathParser` in `jsonpath_compile.cpp`,
non-throwing (`Result<CompiledPath>`) — with one exception: an unclosed quoted
string throws `std::runtime_error("Unclosed quoted string in JSONPath")`, which
escapes to Python as `RuntimeError` instead of the usual `ValueError`. A
`CompiledPath` is an immutable `std::vector<PathStep>` with ops
Root/Field/Wildcard/Index/RecursiveDescent/Slice/Filter.

Supported: `$` (mandatory) · `.field` (`[A-Za-z0-9_]+`) · `["f"]`/`['f']`
(escapes `\n \t \r \\ \" \'`, no `\uXXXX`) · `.*` / `[*]` · `[n]` (negative ok) ·
`[start:end:step]` (components omittable; negative start/end ok) · `..field`
(single compiled step; matched values are recursed into) ·
`[?(@.field op value)]` with `== != > >= < <=` (numeric RHS or quoted string;
LHS single-level `@.field` or `@['field']` only).

Not supported (parse error): unions `['a','b']`/`[0,2]`, `$..*`, `$..[...]`,
`&&`/`||` (the `FilterOp::And/Or` enum values exist but nothing emits or
evaluates them — aspirational), nested filter paths, existence filters,
bool/null filter literals, regex/functions. Silent quirks: negative slice step
parses but returns `[]` (evaluators loop only for step > 0); string comparisons
support only `==`/`!=`; filters only apply to arrays; `search()` requires a
`.json`/`.ndjson`/`.jsonl` extension regardless of content.

## Evaluators (three live + one dead)

1. **PyObject-native** — `eval_step_pyobj` walks Python dict/list/tuple with
   borrowed refs, no C++ tree. Backs `strata.query()` and the fallback branches
   of `search()`. Preserves exact ints.
2. **SAX streaming** — `SaxSearchHandler` (added `1ad8828`), used by `search()`
   on `.json` files: evaluates the path *during* `parse_sax_inline`, skipping
   irrelevant subtrees structurally (`skip_depth_`); only matched subtrees are
   materialized via a minimal `CaptureBuilder`. Gate: `is_sax_compatible()`
   rejects Filter/Slice steps → those fall back to full parse + PyObject eval.
   Arrays need the `SaxWrapper` shim (no per-element callback in the parser).
   This path is the headline perf feature (~9–10× vs orjson+jsonpath-ng pipelines).
3. **C++ DOM** — `eval_jsonpath` over `JsonValue` (results deep-copied via
   `materialize()`). Exposed only through `CompiledPath.execute(cursor)`; numbers
   are doubles here (int precision loss > 2^53).
4. **Dead** — `eval_step_jsonvalue`/`collect_recursive_jsonvalue`/
   `pyobject_to_json_value` (+memo) in `python_jsonpath.cpp`: unreachable
   pre-SAX pipeline, ~500 lines. Don't extend it; delete on next cleanup.

NDJSON `search()` does **not** use SAX search: each line goes
`NdjsonStream` → `JsonValue` → full Python conversion → `eval_step_pyobj`.
Slower legacy pipeline; candidate for the same SAX treatment.

## Known semantic divergences

- `$..a` when a matched object contains another `"a"` inside it: DOM/PyObject
  eval returns both (nested recursion); the SAX capture path swallows the nested
  occurrence inside the captured subtree — `search()` vs `query()` can disagree.
  Only the query() behavior is test-pinned (`test_recursive_descent_nested_same_field`).
- PyObject filter eval coerces Python bools to 1.0/0.0 in numeric comparisons;
  the C++ DOM path requires an actual number.
- `compile` errors are the generic `ValueError("Invalid JSONPath expression")`
  with no position info — except unclosed quoted strings, which surface as
  `RuntimeError` (uncaught C++ throw, see above).

## API split (enforced, tested)

`search()` = file paths only (else `TypeError`); `query()` = dict/list/tuple
only (else `TypeError`). `iterator=True` on both is eager-evaluate,
lazy-consume (wraps the finished list).

## Tests & benchmarks

`tests/{py,unit}/test_jsonpath*.py` (~1000 lines; unit adds nested-RD, py adds
cursor-execute), `tests/unit/test_search_query.py` (API split),
`tests/cpp/test_jsonpath.cpp`. Benchmarks: `benchmarks/bench_jsonpath.py`
(vs jmespath/jsonpath-ng, modes dict/file) and the search/query sections of
`bench_main.py`; canonical queries live in `eval_queries.py`.

---
name: jsonpath
description: JSONPath subsystem — supported grammar, compile/eval architecture, 
  the three live evaluators (C++ DOM, PyObject-native, SAX streaming), known 
  semantic divergences, and unsupported syntax. Load before touching 
  src/strata/search/, python_jsonpath.cpp, or python/strata/jsonpath.py.
---

# JSONPath / Search

**Framing:** this doc describes the *previous implementation* as the blueprint.
"Current state" says what the rebuild has built.

## Current state (after M10)

Real: `include/strata/search/jsonpath.hpp`, `src/strata/search/{jsonpath_compile, jsonpath_eval}.cpp`, `src/strata/bindings/python_jsonpath.cpp`, and
`python/strata/jsonpath.py`. `strata.query`, `strata.search`, `strata.compile`
and `CompiledPath.execute` all work over the documented grammar.

- **`search` obeys `search(f, e) == query(load(f), e)` by two routes.** The
  fallback route *is* load-then-query (definitionally exact); the streaming
  route (below) is a second implementation of the same law for the
  fixed-depth subset, and it is verified against the definition over
  randomised expression/document corpora at both layers, plus pinned
  duplicate-key and error-contract cases.
- **Recursive descent descends into its own matches**, in both evaluators, so
  `$..a` returns an `a` nested inside an `a`. That resolves the divergence
  recorded below in favour of query semantics.
- Two evaluators, not four: PyObject-native (`query`, and so `search`) and C++
  DOM (`CompiledPath.execute`). The blueprint's ~500 lines of dead pre-SAX
  evaluator were not rebuilt.
- **The SAX streaming evaluator is built** (`include/strata/search/jsonpath_stream.hpp`,
  M10): `search()` on a `.json` file with a plain path — Field / Wildcard /
  non-negative Index — evaluates during the parse and materializes only the
  matches, through the same `PythonObjectBuilder` that `load` uses.
  `is_streamable()` gates it (also rejecting negative indices and bare `$`),
  and it runs only under the default FirstWins duplicate-key policy, which it
  implements: first occurrence per Field, per-key dedup under a Wildcard,
  policy-collapsed captures. Everything else — recursive descent, slices,
  filters, other policies, NDJSON — falls back to load-then-query, so the law
  holds by construction on the fallback and by the pinned suites on the
  stream (`tests/cpp/test_jsonpath_stream.cpp`, `tests/unit/test_search_stream.py`).
  Measured: `search users.json $[*].id` 15.7 → 5.0 ms, ~2.2× ahead of
  orjson+jmespath; recursive descent stays full-parse and still leads ~20×.
- Filter comparisons treat `bool` as not-a-number in both evaluators, so
  `query()` and `execute()` cannot disagree.

## Grammar (subset of RFC 9535)

The canonical contract summary (supported set, error types) is
`docs/context/api.md` §JSONPath; this section is the detailed grammar.

Compiler: hand-written recursive descent `PathParser` in `jsonpath_compile.cpp`,
non-throwing (`Result<CompiledPath>`). Previous-implementation defect — **do
not reproduce**: an unclosed quoted string threw `std::runtime_error`, leaking
to Python as `RuntimeError`; the target is `ValueError` for every invalid
expression. A `CompiledPath` is an immutable `std::vector<PathStep>` with ops
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

- `$..a` when a matched object contains another `"a"` inside it: the previous
  implementation's SAX capture swallowed the nested occurrence and `search()`
  disagreed with `query()`. The rebuild's streaming evaluator excludes
  recursive descent entirely, so the divergence cannot arise: `$..` always
  takes the full-parse path, which recurses into matches.
- PyObject filter eval coerces Python bools to 1.0/0.0 in numeric comparisons;
  the C++ DOM path requires an actual number.
- `compile` errors are the generic `ValueError("Invalid JSONPath expression")`
  with no position info — including unclosed quoted strings (the previous
  implementation's `RuntimeError` leak there is a defect, see above).

## API split (enforced, tested)

`search()` = file or directory paths only (else `TypeError`); `query()` =
dict/list/tuple only (else `TypeError`). Folder mode is new in the target API
(no reference code) — the contract lives in `docs/context/api.md`; the law to
preserve: `search(dir, e) == concat(search(f, e) for each discovered file f)`.
`iterator=True` is eager-evaluate, lazy-consume (wraps the finished list) for
single files; in folder mode it must stream lazily file-by-file.

## Tests & benchmarks

`tests/{py,unit}/test_jsonpath*.py` (~1000 lines; unit adds nested-RD, py adds
cursor-execute), `tests/unit/test_search_query.py` (API split),
`tests/cpp/test_jsonpath.cpp`. Benchmarks: `benchmarks/bench_jsonpath.py`
(vs jmespath/jsonpath-ng, modes dict/file) and the search/query sections of
`bench_main.py`; canonical queries live in `eval_queries.py`.

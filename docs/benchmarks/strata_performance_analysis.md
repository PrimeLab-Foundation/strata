# Strata Performance Analysis: Why We Lag in Each Area

**Date**: 2026-01-31\
**Based on**: Small-dataset benchmarks (≈1 MB users.json / users.ndjson), repeat 5, warmup 2.

______________________________________________________________________

## Summary: Current Ranks and Gaps

| Area                  | Strata rank          | Gap to #1                       | Leader   |
| --------------------- | -------------------- | ------------------------------- | -------- |
| **loads** (parse)     | #5 / 5               | ~166% behind                    | orjson   |
| **dumps** (serialize) | #3 / 5               | ~304% behind                    | msgspec  |
| **NDJSON**            | #5 / 5               | ~144% behind                    | msgspec  |
| **Search**          | Last on most queries | 10–40× slower on simple queries | jmespath |

Strata is slow in almost every area compared to the top libraries. Below are the **root causes** by area and what would be needed to improve.

______________________________________________________________________

## 1. Parsing (loads): Why Strata Is ~2–3× Slower Than orjson/msgspec

**Observed**: Strata median ~9.6 ms vs orjson ~3.6 ms, msgspec ~3.8 ms on ~1 MB JSON.

**Root causes:**

1. **Allocations and data layout**

   - Every string value is stored as `std::string` (copy). Every object is `std::map<std::string, JsonValue>`. orjson/msgspec use compact, cache-friendly layouts (e.g. contiguous buffers, small structs).
   - See `json_parse.cpp`: `parse_string()` builds `std::string result(data + i, scan_pos)`; objects/arrays use `JsonValue::Object` / `JsonValue::Array` (map/vector).

1. **No SIMD in the main parse loop**

   - SIMD is used for UTF-8 validation upfront (`validate_utf8_simd` in `json_parse.cpp`) and for some string scanning (`scan_string_fast`, `skip_whitespace_fast`), but the core value parsing (numbers, structural chars) is scalar. orjson/simdjson use SIMD for structural parsing and number classification.

1. **UTF-8 validation pass**

   - A full pass over the input runs before parsing. That adds a fixed cost; competitors often validate on-the-fly or use SIMD in a single pass.

1. **Python conversion cost**

   - `loads()` ends with `json_value_to_python(result.value)` (`python_loads.cpp`), which recursively builds Python objects (PyDict, PyList, PyUnicode, PyFloat). That’s one full tree walk and many Python API calls. orjson/msgspec produce native structures that map more directly to Python.

**References:** `src/strata/json/json_parse.cpp`, `src/strata/bindings/python_loads.cpp`, `include/strata/util/simd_string.hpp`, `docs/development/simd_utf8_validation.md`.

______________________________________________________________________

## 2. Serialization (dumps): Why Strata Is ~4× Slower and Output Is Larger

**Observed**: Strata median ~3.96 ms, output 1,000,369 bytes vs orjson/msgspec ~1.0 ms, 895,062 bytes.

**Root causes:**

1. **Larger output size**

   - Strata outputs ~12% more bytes (e.g. different formatting: more spaces, or `.0` on integers, or key ordering). More bytes ⇒ more work and more memory. See `json_serialize.cpp` and the “ensure decimal point” logic in `python_dumps.cpp` / number formatting.

1. **Number formatting**

   - Floats use Dragonbox/Ryu-style formatting where available, but integers and edge cases still use `std::snprintf` or similar in some paths (`json_serialize.cpp` uses `std::snprintf(buf, sizeof(buf), "%.0f", d)` for integers; `python_dumps.cpp` has fast paths for int64 but fallbacks). orjson/msgspec use highly tuned number writers.

1. **Recursive serialization and buffer growth**

   - Serialization is recursive over the value tree; output is appended to a growing buffer. Reallocations and extra copies in hot paths add cost. See `serialize_value` / `serialize_object` / `serialize_array` in `json_serialize.cpp` and the Python dumps buffer in `python_dumps.cpp`.

1. **Python → C++ conversion for dumps**

   - When dumping a Python object, the binding walks the PyObject tree and writes to the buffer. Every node incurs type checks and conversions. Competitors often keep a single native representation and serialize from that.

**References:** `src/strata/json/json_serialize.cpp`, `src/strata/bindings/python_dumps.cpp`, `src/strata/util/dragonbox.cpp`, `docs/benchmarks/dumps_results.md`.

______________________________________________________________________

## 3. NDJSON: Why Strata Is ~2× Slower

**Observed**: Strata median ~10.0 ms vs msgspec ~4.1 ms on 1000 lines (~1 MB NDJSON).

**Root causes:**

1. **Same parser as single-doc**

   - NDJSON is implemented as “split lines, then parse each line with the same `parse_json()`”. So NDJSON cost is dominated by **N × (cost of one parse)**. Since single-doc parse is already ~2–3× slower than orjson/msgspec, NDJSON is proportionally slower. See `ndjson_stream.cpp`: line-by-line loop with repeated `parse_json()`.

1. **Line splitting and batching**

   - Lines are found (e.g. SIMD newline scan) and then each line is parsed. There is no shared structural or buffer reuse across lines; each line is a separate parse. Competitors may use a single buffer and parse in place or use a dedicated streaming parser.

1. **No dedicated NDJSON path**

   - There is no “NDJSON-only” parser that avoids per-line overhead or reuses state. So we pay full parse cost per line.

**References:** `src/strata/json/ndjson_stream.cpp`, `docs/benchmarks/ndjson_results.md`.

______________________________________________________________________

## 4. Search: Why Strata Is 10–40× Slower on Simple Queries (Critical)

**Observed**: For “Extract all user IDs”, Strata ~8.2 ms vs jmespath ~0.23 ms. For “Deep path navigation” (single result), Strata ~8.3 ms vs jmespath ~0.01 ms.

**Root cause: dict input forces full conversion → query**

When `search(data, path)` is called with a **Python dict/list** (e.g. the result of `strata.loads()`), the C++ binding converts the entire Python tree into a C++ `JsonValue` before running Search:

1. **Convert** dict/list → `JsonValue` (full tree walk + allocations).
1. **Execute** Search on the resulting C++ value.

So **every** `search(parsed_dict, path)` pays **conversion + query**. That still dominates runtime versus jmespath/jsonpath-ng, which walk the existing Python dict in place (no conversion).

**Code reference:** `src/strata/bindings/python_jsonpath.cpp` (dict/list conversion path).

```cpp
// Handle Python object (dict/list/etc) - serialize and re-parse
extern PyObject* strata_dumps(PyObject* self, PyObject* obj);
PyObject* json_str_raw = strata_dumps(NULL, data_obj);
// ...
auto parse_result = strata::parse_json(std::string_view(json_data, json_len));
// Execute query on parsed C++ value
```

**Implications:**

- **Benchmark fairness**: The current Search benchmark uses `parsed_strata = strata.loads(json_text)` then `strata.search(parsed_strata, path)`. That always hits the “dict → dumps → parse → query” path. So Strata is not “slow at Search evaluation” per se; it is slow because each call does a full round-trip.
- **Fast path**: If the user passes a **JsonCursor** (e.g. from `strata.parse_json_file(path)`) or **JsonDocument**, `search()` runs directly on the C++ tree (no dumps, no re-parse). So for repeated queries on the same document, the recommended pattern is:
  - `cursor = strata.parse_json_file("file.json")` (or keep a C++ document handle if exposed), then
  - `strata.search(cursor, path)` for each query.
- **Alternative**: Passing the **raw JSON string** to `search()` parses once per call (no dumps). That still pays parse cost (~8 ms) but avoids dumps and is fairer when comparing “one parse + one query” to jmespath’s “one parse (already done) + one query”.

**Recommendations:**

1. **Document** in API docs and benchmark methodology: “For Search, pass a JsonCursor or raw JSON string when comparing to libraries that operate on an in-memory dict; passing a dict from `loads()` triggers a full conversion per query.”
1. **Optionally** add a benchmark mode that times `search(json_text, path)` (parse + query) and/or `parse_json_file` + `search(cursor, path)` (one parse, many queries) so that Strata’s query engine is compared on equal footing.
1. **Long-term**: Implement a true PyObject-based cursor so `search(data, path)` walks the Python dict/list without building a C++ tree; that would align behavior with jmespath/jsonpath-ng and remove per-query conversion cost.

**References:** `src/strata/bindings/python_jsonpath.cpp` (dict branch and JsonCursor/JsonDocument branches), `python/strata/jsonpath.py`, `docs/benchmarks/search_results.md`.

______________________________________________________________________

## 5. Cross-Cutting Factors

- **Language and ABI**: orjson (Rust) and msgspec (Cython) are built for minimal overhead and tight integration with CPython. Strata is C++ with a hand-written C API binding; every crossing has a cost.
- **Memory model**: Strata uses `std::map`/`std::vector`/`std::string`; leaders use flatter, cache-friendly layouts and fewer allocations.
- **PGO/LTO**: Docs and status mention that PGO+LTO improve dumps (e.g. ~33%) and help parsing. Default builds are not PGO; so reported gaps are for non-PGO builds. See `scripts/pgo_build.sh`, `docs/development/pgo_performance.md`.

______________________________________________________________________

## 6. What Would Move the Needle

| Area         | High-impact change                                                                                                                                                                                                               |
| ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **loads**    | SIMD structural parsing; fewer/smaller allocations; optional “cursor-only” API (no full Python tree) for query-heavy workloads.                                                                                                  |
| **dumps**    | Smaller output (match competitor formatting); faster number formatting (avoid snprintf in hot path); fewer buffer reallocations.                                                                                                 |
| **NDJSON**   | Faster single-doc parse (same as loads); optional streaming/stateful NDJSON parser to reduce per-line overhead.                                                                                                                  |
| **Search** | **Stop re-serializing and re-parsing on dict input**: either (a) implement PyObject-based traversal for dict/list, or (b) document and benchmark using cursor/string so that “parse once, query many” is the default comparison. |

______________________________________________________________________

## 7. Reproducing the Numbers

```bash
make bench-data   # ensure small/medium/large generated
PYTHONPATH=. .venv/bin/python -m benchmarks.bench_loads   --data benchmarks/data/generated/small/users.json --repeat 5 --warmup 2
PYTHONPATH=. .venv/bin/python -m benchmarks.bench_dumps  --data benchmarks/data/generated/small/users.json --repeat 5 --warmup 2
PYTHONPATH=. .venv/bin/python -m benchmarks.bench_ndjson --data benchmarks/data/generated/small/users.ndjson --repeat 5 --warmup 2
PYTHONPATH=. .venv/bin/python -m benchmarks.bench_search --data benchmarks/data/generated/small/users.json --repeat 5 --warmup 2
```

See `benchmarks/methodology.md` and `docs/benchmarks/` for full methodology and result history.

______________________________________________________________________

## 8. Next Steps to Improve Strata Overall

Prioritized by impact and effort. Each step should include C++ tests + Python tests + benchmark update per project rules.

### Phase 1: Quick wins (1–2 days)

| #   | Step                                      | Why                                                                                                                                                                                                                                    | Where                                                                               |
| --- | ----------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- |
| 1   | **Benchmark Search with cursor/string** | Current bench uses dict → serialize→parse per query; Strata looks 10–40× slower than jmespath. Add a mode that times `search(json_text, path)` or `parse_json_file` + `search(cursor, path)` so query-engine speed is compared fairly. | `benchmarks/bench_search.py`, `docs/benchmarks/methodology.md`                    |
| 2   | **Document “parse once, query many”**     | In API docs and docstrings: for repeated queries, use `parse_json_file(path)` then `search(cursor, path)`, or pass raw JSON string to `search()`; avoid passing a dict from `loads()` if you care about query latency.                 | `python/strata/jsonpath.py`, `docs/api/simplified_api.md`                           |
| 3   | **Enable PGO in CI or release**           | PGO+LTO already improves dumps (~33%); make it the default for release builds or at least run weekly so regressions are visible.                                                                                                       | `scripts/pgo_build.sh`, `.github/workflows/`, `docs/development/pgo_performance.md` |

### Phase 2: Search dict path (high impact, medium effort)

| #   | Step                                                    | Why                                                                                                                                                                                                                                                                                                                                                   | Where                                                                                                       |
| --- | ------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| 4   | **Implement PyObject traversal for search(dict, path)** | When `search(data, path)` receives a Python dict/list, stop doing dumps→parse→query. Add a path that walks the PyObject tree from C++ and runs the existing Search engine on a lightweight cursor/view, or converts dict→JsonValue once and reuses it. Then “load once, query many” with `loads()` + `search()` is fast and comparable to jmespath. | `src/strata/bindings/python_jsonpath.cpp`, possibly `src/strata/search/jsonpath.cpp` (cursor from PyObject) |
| 5   | **C++ tests for PyObject-based query**                  | Ensure Search results match when input is dict vs when input is JsonDocument/cursor.                                                                                                                                                                                                                                                                | `tests/cpp/`, Python contract tests in `tests/py/`                                                          |

### Phase 3: Parsing (loads) — close gap to orjson/msgspec

| #   | Step                                   | Why                                                                                                                                                                                                     | Where                                                                                                  |
| --- | -------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ |
| 6   | **SIMD structural parsing**            | Use SIMD to classify bytes (whitespace, quotes, brackets, commas, colons) and find token boundaries in one pass; feed a scalar or SIMD-friendly state machine. Reduces branches and improves cache use. | `src/strata/json/json_parse.cpp`, `include/strata/util/simd_string.hpp`, experiments first per Rule 12 |
| 7   | **Fewer/smaller allocations in parse** | Consider a single arena or buffer for the whole document; store string values as (ptr, len) where possible; use a flatter structure for objects (e.g. key-value arrays) to improve cache behavior.      | `src/strata/json/json_parse.cpp`, `include/strata/json/json_core.hpp`                                  |
| 8   | **Optional “cursor-only” loads**       | Expose a path that parses into a C++ document only and returns a cursor (no full Python tree). Enables “parse once, query many” without dict conversion; benchmark and document.                        | New API or flag in `python_loads.cpp`, `python/strata/serialize.py`                                    |

### Phase 4: Dumps — smaller output and faster serialization

| #   | Step                               | Why                                                                                                                                                                                    | Where                                                                                     |
| --- | ---------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| 9   | **Match competitor output size**   | Strata output is ~12% larger (formatting, `.0`, spacing). Align with minimal JSON (no extra spaces, same number formatting as orjson/msgspec) so throughput and memory are comparable. | `src/strata/json/json_serialize.cpp`, `src/strata/bindings/python_dumps.cpp`              |
| 10  | **Remove snprintf from hot path**  | Use Dragonbox/Ryu or integer digit-generation everywhere for numbers; no `std::snprintf` or `%.0f` in the hot path.                                                                    | `src/strata/json/json_serialize.cpp`, `src/strata/util/dragonbox.cpp`, `python_dumps.cpp` |
| 11  | **Single buffer / fewer reallocs** | Reserve or grow buffer in larger chunks; avoid repeated small appends that trigger realloc.                                                                                            | `src/strata/util/output_buffer.hpp`, serialization entry points                           |

### Phase 5: NDJSON and long-term

| #   | Step                          | Why                                                                                                                                                                                             | Where                                                               |
| --- | ----------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- |
| 12  | **Faster single-doc parse**   | NDJSON cost is N × parse cost; any loads improvement (SIMD, allocations) directly improves NDJSON.                                                                                              | Same as Phase 3                                                     |
| 13  | **Optional streaming NDJSON** | Experiment with a single-buffer, incremental parser that reuses state across lines to reduce per-line overhead. Document and benchmark; adopt if clearly faster without regressing correctness. | `experiments/` then `src/strata/json/ndjson_stream.cpp` per Rule 12 |

### Ordering and dependencies

- **Do first**: 1, 2, 3 (benchmark fairness, docs, PGO).
- **Then**: 4, 5 (Search dict path) — biggest perceived win for “Strata is slow” on queries.
- **Then**: 6, 7, 8 (parse); 9, 10, 11 (dumps); 12, 13 (NDJSON) — in parallel or by priority (e.g. parse if loads is the bottleneck, dumps if serialization is).

### Rules to respect

- **Rule 1 (C++ owns CPU)**: All hot-path improvements stay in C++; Python is a thin wrapper.
- **Rule 2**: No dependency on orjson/simdjson/etc. for core behavior.
- **Rule 6, 14, 15**: Every change: run C++ and Python tests; keep tests green.
- **Rule 7**: Cross-layer contract tests when behavior is user-visible (e.g. search(dict) vs search(cursor)).
- **Rule 8, 13**: Update benchmarks and docs when touching perf; track regressions.
- **Rule 12**: New algorithms (SIMD parse, streaming NDJSON) prototype in `experiments/` with a go/no-go and then move to core if justified.

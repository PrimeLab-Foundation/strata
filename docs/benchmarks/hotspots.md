# Hotspots (Phase 2 discovery — 2026-02-02)

System: macOS 26.2 (Apple M1 Max), Apple clang 17.0.0, Python 3.14.2. Baseline run: `make bench-small` (repeat=3 warmup=1; JSONPath repeat=2). Additional profiling: `benchmarks.bench_loads --repeat 50 --warmup 5` sampled for 5s with `sample` (output: `docs/benchmarks/raw/20260202_Boryss-MacBook-Pro-2/baseline/sample_loads_5s.txt`).

## Slowest workloads

- `bench_loads` (users.json ~1 MB): Strata median 30.55 ms (rank #5/5; +117% vs orjson) on baseline; long run shows heavy tail (p95 ~84 ms).
- `bench_ndjson` (1k lines): Strata median 33.49 ms (rank #4/5; +76% vs orjson).
- `bench_dumps` (same dataset): Strata median 17.85 ms (rank #4/5; +497% vs msgspec).

## Sampling notes (loads)

- 5s `sample` of `bench_loads --repeat 50` shows the main thread spending a large portion in CPython GC (`_PyGC_Collect`, `deduce_unreachable`) triggered every iteration while converting the parsed tree to Python objects. This aligns with the Python-side materialization cost identified previously.
- Native frames for `_strata` are present but not expanded by `sample`; lack of symbols prevents precise C++ attribution, but GC dominance indicates per-iteration allocation pressure.

## Suspected hotspot causes (cross-checked with code)

- **Loads/NDJSON**: Each parse builds a full C++ tree of `std::string`/`std::vector`/`std::map` and then converts to Python objects, allocating heavily and triggering frequent GC. No arena reuse across iterations; NDJSON simply repeats full parses per line.
- **Dumps**: Output is larger (1,000,369 bytes vs 895,062 bytes for orjson/msgspec), implying more bytes to write/escape; number formatting still routes through slower paths for many cases. Buffer growth and recursive serializer add overhead.
- **JSONPath (recursive `$..price`)**: Full recursive descent over all nodes remains expensive (70 ms median) even in cursor mode; still faster than jsonpath-ng but dominates query suite time.

## Next profiling steps (planned)

- Collect symbolized C++ profiles on loads/ndjson using a tool that resolves `_strata` (e.g., `perf`/`dtrace`/Instruments with dSYM) to break down time in `json_parse`, string unescape, number parsing, and Python conversion helpers.
- Add allocation-focused profiling (heaptrack/massif-equivalent) if available to quantify per-node allocations and buffer growth during parse and dumps.

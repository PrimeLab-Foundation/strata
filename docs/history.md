# Strata — Development History

> A structured narrative of how Strata evolved, derived from the git history.
> Last generated: 2026-03-19 from `2a4332c`.
>
> See also: [Best Practices Summary](summary.md) for a catalog of engineering
> techniques implemented in the codebase.

## Project Timeline

| Phase                     | Date Range              | Commits | Key Achievement                                                           |
| ------------------------- | ----------------------- | ------- | ------------------------------------------------------------------------- |
| Foundation                | 2025-11-20 – 2025-11-23 | 12      | Project scaffolding, rename, CMake, initial C++ parser                    |
| Architecture Pivot        | 2026-02-01 – 2026-02-02 | 5       | Legacy cleanup, configurable policies, JSONPath refactor                  |
| Core Engine Sprint        | 2026-03-14 – 2026-03-15 | 19      | SAX architecture, NDJSON, serialization, Ryu/Dragonbox, PGO, docs         |
| SIMD & Speculative Engine | 2026-03-17 – 2026-03-18 | 12      | Structural indexer (7 backends), Markov speculative parser, Bloom filters |
| Performance Tuning        | 2026-03-18 – 2026-03-19 | 16      | LightweightBuilder, SWAR parsing, cursor fast paths, benchmark iteration  |

## Project Statistics

| Metric                          | Value                           |
| ------------------------------- | ------------------------------- |
| Total commits                   | 64                              |
| Active development period       | ~4 months (Nov 2025 – Mar 2026) |
| Contributors                    | 1 (borysbardysh)                |
| Lines of code (src/ + include/) | 28,869                          |
| Lines of tests (tests/)         | 10,447                          |
| Test-to-code ratio              | 0.36x                           |
| Documentation pages (docs/)     | 13                              |
| Lines added (all time)          | 104,591                         |
| Lines removed (all time)        | 53,323                          |
| Net lines added                 | 51,268                          |

## Activity Chart

```
2025-11 ████████████ 12
2026-02 █████ 5
2026-03 ███████████████████████████████████████████████ 47
```

**Observation**: Development happened in three distinct bursts with two dormant
gaps. November 2025 was project founding; February 2026 was a brief pivot; March
2026 was an intense sprint (47 commits in 19 days, with 35 of those in the final
6 days from March 14-19).

______________________________________________________________________

## Phase 1: Foundation

**Period**: 2025-11-20 → 2025-11-23
**Commits**: 12
**Focus**: Project scaffolding, build system, initial C++ implementation

The project began on November 20, 2025 as `turbojsonpath` — a name that was
changed to `strata` the same day (`4f3ff0a`). The first few commits established
the fundamental project structure: `pyproject.toml`, `Makefile`, benchmarking
infrastructure, and CI configuration. The project was conceived from the start
as a Python library backed by C++.

Within the first three days, the codebase moved rapidly through build system
setup (`a150571` — CMakeLists.txt with C++20), initial C++ JSON parsing and
navigation (`6071b34`), and early Python bindings (`0dd6fec`). By November 23,
there was already a working parse-navigate-query pipeline bridging C++ and
Python.

### Key Commits

| Hash      | Date       | Description                                                |
| --------- | ---------- | ---------------------------------------------------------- |
| `c71fcc8` | 2025-11-20 | Initial commit                                             |
| `4f3ff0a` | 2025-11-20 | Rename from `turbojsonpath` to `strata`                    |
| `99dc6a5` | 2025-11-20 | Add benchmarking dataset generation and CI integration     |
| `a150571` | 2025-11-21 | Add C++ support via CMakeLists.txt (C++20 standard)        |
| `6071b34` | 2025-11-22 | Add JSON parsing and navigation in Python bindings and C++ |
| `0dd6fec` | 2025-11-23 | Enhance JSON handling and testing capabilities             |

### Files Introduced

- `CMakeLists.txt` — C++20 build system with test targets
- `pyproject.toml`, `setup.py`, `Makefile` — Python packaging and dev workflow
- `src/strata/json/` — Core C++ JSON parsing
- `python/strata/` — Python package with ctypes/pybind11 bindings
- `benchmarks/` — Performance measurement infrastructure

______________________________________________________________________

## Phase 2: Architecture Pivot

**Period**: 2026-02-01 → 2026-02-02
**Commits**: 5
**Focus**: Legacy cleanup, configurable policies, JSONPath optimization

After a ~10-week dormant period (no commits from Nov 24 to Jan 31), development
resumed with a brief but significant pivot. The first action was to remove
legacy C++ components and Python bindings (`7ac1789`), suggesting a rethinking
of the architecture. This was followed by adding configurable duplicate-key
policies, validation improvements, and cycle detection in JSONPath evaluation
(`b65dd5e`, `d702fd7`).

The final commits of this phase optimized the core JSON parsing and
serialization routines (`b7f3b5d`) and revised the project's agent guidelines
(`b41fcb5`). This phase reads as a deliberate cleanup before the major
development sprint that would follow six weeks later.

### Key Commits

| Hash      | Date       | Description                                              |
| --------- | ---------- | -------------------------------------------------------- |
| `7ac1789` | 2026-02-01 | Remove legacy C++ components and Python bindings         |
| `b65dd5e` | 2026-02-01 | Add configurable duplicate-key policies and validation   |
| `d702fd7` | 2026-02-02 | Refactor JSONPath handling, add cycle detection          |
| `b7f3b5d` | 2026-02-02 | Optimize JSON parsing, serialization, and core utilities |

______________________________________________________________________

## Phase 3: Core Engine Sprint

**Period**: 2026-03-14 → 2026-03-15
**Commits**: 19
**Focus**: SAX architecture, NDJSON, serialization, fast float, PGO, documentation

This two-day burst is where Strata's core C++ engine took its modern shape. It
began with the introduction of a Hybrid SAX/Visitor architecture (`8f468d2`) —
a fundamental design decision that decoupled parsing from tree building. The
SAX event model enabled downstream features like streaming NDJSON and selective
JSONPath evaluation.

NDJSON support was built out rapidly: batch `parse_ndjson` replaced iterative
processing (`bcf6dd1`), followed by thread-local builders for memory reuse
(`85dcf57`). Serialization was overhauled with template specialization for cycle
detection and iterative strategies (`2aa5566`, `b66049d`, `b1b82c0`).

The fast double-to-ASCII conversion was added via the Ryu algorithm (`ad401a8`),
and the SAX-based JSONPath search was introduced for memory-efficient querying
(`1ad8828`). The `mem_eff` mode was removed to simplify the API (`5c264ca`).
PGO (Profile-Guided Optimization) training scripts were added (`512403f`).

The phase concluded with a comprehensive documentation pass: Doxygen-style
comments, `[[nodiscard]]` and `noexcept` annotations, named constants replacing
magic numbers, and structured file-level comments across all core headers
(`18a3521`). This single commit touched 34 files.

### Key Commits

| Hash      | Date       | Description                                                |
| --------- | ---------- | ---------------------------------------------------------- |
| `8f468d2` | 2026-03-14 | Introduce Hybrid SAX/Visitor architecture                  |
| `bcf6dd1` | 2026-03-14 | Batch-based NDJSON parsing, simplified bindings            |
| `ad401a8` | 2026-03-15 | Add Ryu-based fast double-to-ASCII conversion              |
| `1ad8828` | 2026-03-15 | SAX-based JSONPath search for memory-efficient querying    |
| `18a3521` | 2026-03-15 | Comprehensive Doxygen documentation and safety annotations |
| `512403f` | 2026-03-15 | PGO training scripts and dataset generation                |

### Files Introduced / Major Changes

- `src/strata/json/ndjson_stream.cpp` — Streaming NDJSON parser
- `include/strata/json/json_sax_handler.hpp` — SAX event interface
- `include/strata/util/ryu_dtoa.hpp` — Ryu double-to-ASCII
- `include/strata/util/arena_allocator.hpp` — Arena allocator (documented)
- `include/strata/util/output_buffer.hpp` — Growable/fixed output buffers
- `scripts/pgo_build.sh`, `scripts/pgo_training.py` — PGO pipeline

______________________________________________________________________

## Phase 4: SIMD & Speculative Engine

**Period**: 2026-03-17 – 2026-03-18
**Commits**: 12
**Focus**: Structural indexer, multi-platform SIMD, Markov speculative parser, Bloom filters

This phase introduced Strata's most architecturally significant subsystems. On
March 17, the SIMD structural indexer was added (`2430b0b`) — a complete
implementation of the vpshufb nibble-lookup character classification technique
with support for AVX2, PCLMUL prefix-XOR string masking, and BMI2 bit
extraction. The same commit introduced the `IndexBuilder`, `Classifier`,
`StructuralIndexer`, and `dispatch` modules. The following day, support was
extended to 7 SIMD backends: AVX-512, SVE2, ARM NEON, WebAssembly SIMD128,
RISC-V Vector, and scalar fallback (`5b6b37b`, `61b2175`).

On March 18, the speculative parsing engine was introduced in a single large
commit (`26bebc1`). This added the Markov transition model
(`transition_model.h/cpp`), the fast-path parsers (`fast_paths.h/cpp` —
SWAR integer, Lemire float, branchless literal), the coordinator
(`parser.h/cpp`), and the type system (`types.h`). Accompanying tests and
benchmarks were included.

The same commit also added the Bloom filter subsystem: `bloom_filter.h` with
wyhash and double hashing, `sizing.h` with constexpr information-theoretic
sizing, `dedup_filter.h` for duplicate key detection, `key_filter.h` for
selective parsing, and `schema_filter.h`. A separate Bloom filter test suite
and benchmark were included.

Parser optimizations continued through this phase: Ryu was replaced with
Dragonbox for double-to-string conversion (`33d6835`), the fast_float
third-party library was vendored for Eisel-Lemire float parsing, and inline
number parsing was added with unified number handling (`3f0d7fe`).

### Key Commits

| Hash      | Date       | Description                                               |
| --------- | ---------- | --------------------------------------------------------- |
| `2430b0b` | 2026-03-17 | Add SIMD structural indexer (AVX2, PCLMUL, BMI2)          |
| `5b6b37b` | 2026-03-17 | Extend SIMD to AVX-512, SVE2, NEON, WASM, RVV             |
| `33d6835` | 2026-03-17 | Replace Ryu with Dragonbox; vendor fast_float             |
| `26bebc1` | 2026-03-18 | Speculative parsing engine with Markov model              |
| `23a4c6b` | 2026-03-18 | Bloom filter subsystem (core, sizing, dedup, key, schema) |
| `61b2175` | 2026-03-18 | STRATA_FORCE_SCALAR support for testing                   |

### Files Introduced

- `include/strata/simd/classifier.h` — vpshufb nibble-lookup classifier
- `include/strata/simd/structural_indexer.h` — 64-byte chunk processor
- `include/strata/simd/index_builder.h` — BMI2 PEXT bit extraction
- `include/strata/simd/dispatch.h` — Runtime cpuid backend detection
- `include/strata/speculative/transition_model.h` — Markov model (~120 KB)
- `include/strata/speculative/fast_paths.h` — SWAR, branchless parsers
- `include/strata/speculative/types.h` — ValueType enum, context hashing
- `include/strata/speculative/parser.h` — Speculative coordinator
- `include/strata/bloom/bloom_filter.h` — wyhash + double hashing
- `include/strata/bloom/sizing.h` — constexpr optimal sizing
- `include/strata/bloom/dedup_filter.h` — Stacked Bloom dedup
- `include/strata/bloom/key_filter.h` — Selective parsing filter
- `include/strata/third_party/fast_float/` — Vendored fast_float library
- `include/strata/third_party/dragonbox/` — Vendored Dragonbox library

______________________________________________________________________

## Phase 5: Performance Tuning

**Period**: 2026-03-18 – 2026-03-19
**Commits**: 16
**Focus**: LightweightBuilder, parser micro-optimizations, benchmark iteration

The final phase was a tight optimization loop. Each commit followed a pattern:
make a specific optimization → rebuild → re-run benchmarks → commit updated
results. The benchmark results files (`bench_results_small.md`,
`bench_results_medium.md`, `bench_results_large.md`) were updated in nearly
every commit — 37, 32, and 30 times respectively across the project's lifetime,
making them the most-modified files in the entire repository.

Key optimizations in this phase:

- **LightweightBuilder** (`6da7fbd`) — An optimized deserialization path for
  small multi-schema JSON inputs, with key caching and double-buffering logic.
  Refined over three subsequent commits (`fe648ed`, `dfd6aa5`).
- **Inline numeric parsing** (`f5870e0`) — Moved number parsing into the inline
  parser header to eliminate call overhead.
- **JsonCursor fast paths** (`42f0bfe`) — Added direct `int64_t` and `uint64_t`
  accessor paths in the cursor, avoiding double-to-integer conversion overhead.
- **FNV-1a key hashing** (`c0e3b5a`) — Fast hash-table-based key lookup in the
  parser with 75% load factor cap (`efd00fd`).
- **Unified number parsing** (`3f0d7fe`) — Eliminated double-scanning of digits
  by parsing sign + integer part once, then branching to float only if `.` or
  `e` follows.
- **Array pre-sizing** (`67ef7fc`) — Pre-counting array/object elements to
  reduce allocations, later replaced by skipping pre-counts entirely (`07f85a0`)
  when the overhead of the counting pass exceeded the allocation savings.

This last point is notable: commit `67ef7fc` added pre-sizing, and commit
`07f85a0` (only 1.5 hours later) removed it after benchmarks showed the
counting pass was counterproductive. This demonstrates the tight
measure-change-measure discipline of this phase.

### Key Commits

| Hash      | Date       | Description                                           |
| --------- | ---------- | ----------------------------------------------------- |
| `6da7fbd` | 2026-03-18 | Add LightweightBuilder for small JSON deserialization |
| `f5870e0` | 2026-03-18 | Inline numeric parsing for reduced call overhead      |
| `42f0bfe` | 2026-03-19 | JsonCursor int64/uint64 fast paths                    |
| `c0e3b5a` | 2026-03-16 | FNV-1a key hashing for parser                         |
| `3f0d7fe` | 2026-03-16 | Unified number parsing (no double-scan)               |
| `07f85a0` | 2026-03-15 | Remove pre-counting (benchmarks showed regression)    |

______________________________________________________________________

## Cross-Cutting Observations

### Development Cadence: Burst-Driven

The project's development is strikingly non-uniform. Of 64 total commits:

- 12 in November 2025 (3 days)
- 5 in February 2026 (2 days)
- 47 in March 2026 (6 active days out of 19 calendar days)

This suggests a solo developer working in intense focused sessions rather than
steady daily progress. The two dormant gaps (10 weeks, then 6 weeks) likely
reflect external commitments or design thinking between implementation sprints.

### Benchmark-Driven Development

The most striking pattern in the git history is the benchmark feedback loop.
The three benchmark results files were updated 37, 32, and 30 times
respectively — far more than any source file. Nearly every optimization commit
includes refreshed benchmark results in the same commit, indicating a
disciplined measure-then-commit workflow.

Benchmarks were introduced on day one (`99dc6a5`) and remained central
throughout. The PGO pipeline (`512403f`) further demonstrates commitment to
production-grade performance — profile-guided optimization requires
representative training data and a multi-pass build.

### Testing: Concurrent but Not Test-First

Tests were generally added alongside features rather than before them. The C++
test suite was built up primarily in two moments: the foundation phase
(`0dd6fec`, `6071b34`) and the SIMD/speculative engine phase (`2430b0b`,
`26bebc1`, `23a4c6b`). A dedicated test refresh commit (`a676cc4`) on March 16
updated test cases for precision, parsing, serialization, and stress scenarios.

The fuzz testing infrastructure (`CMakeLists.txt` FUZZ option, `scripts/fuzz.sh`)
was added during the SIMD phase, but there is no evidence in the commit log of
fuzz runs discovering and fixing bugs — suggesting it may have been set up
proactively rather than in response to discovered issues.

### Architecture Evolution: Two Major Pivots

1. **Legacy cleanup** (`7ac1789`, Feb 2026): Removal of earlier C++ components
   and Python bindings, indicating the first implementation was discarded in
   favor of a new architecture.

1. **SAX/Visitor introduction** (`8f468d2`, Mar 14): The shift to a SAX-based
   event model was the most architecturally significant change. It enabled
   streaming NDJSON, selective JSONPath evaluation, and the speculative parsing
   coordinator — none of which would have been feasible with a monolithic parser.

### Optimization Reversals

At least one optimization was introduced and reversed within hours:

- `67ef7fc` (Mar 15, 17:38): Added array/object pre-sizing to reduce allocations.
- `07f85a0` (Mar 15, 19:01): Removed pre-counting because benchmarks showed the
  counting pass was more expensive than the saved allocations.

This is a healthy pattern — the developer trusted benchmarks over intuition
and reverted when data contradicted expectations.

### Float Conversion: Ryu → Dragonbox

The double-to-ASCII conversion algorithm was switched from Ryu (`ad401a8`,
Mar 15) to Dragonbox (`33d6835`, Mar 17) — just two days later. Both are
shortest-representation algorithms, but Dragonbox was measured to perform
better in Strata's workload. The fast_float library (Lemire's Eisel-Lemire
algorithm) was vendored at the same time for the parse direction.

### Single Contributor

All 64 commits are from a single author (`borysbardysh`), with 104,363 lines
added and 53,323 removed. The commit messages are consistently descriptive and
follow a pattern: action verb + what changed + why. Messages like "Optimize
JSON parser: inline float parsing, improve whitespace and colon handling" are
typical — far above the quality threshold of "fix" or "wip".

______________________________________________________________________

## Lessons Learned

**Lesson 1: Benchmark results should be committed alongside code changes.**
Strata's practice of refreshing benchmark markdown files in the same commit
as optimizations creates an auditable performance history. Any commit can be
checked out to see exactly what the performance was at that point.

**Lesson 2: Pre-optimization intuitions should be verified, not trusted.**
The array pre-sizing reversal (`67ef7fc` → `07f85a0`, 1.5 hours apart)
demonstrates that even reasonable-sounding optimizations (reduce allocations)
can be net-negative in practice. The tight benchmark loop caught this
immediately.

**Lesson 3: Architecture pivots are cheaper than incremental patching.**
The February cleanup (`7ac1789`) discarded legacy components rather than
adapting them. The March SAX introduction (`8f468d2`) similarly replaced the
parsing architecture rather than extending it. Both enabled features that
would have been difficult to add incrementally.

**Lesson 4: Multi-platform SIMD benefits from a single abstraction.**
The classifier and structural indexer were built on a common nibble-lookup
algorithm that maps cleanly to every SIMD ISA (AVX-512, AVX2, SVE2, NEON,
WASM, RVV). Adding a new backend required only implementing the same
algorithm with different intrinsics — the structural indexer's
`#if / #elif` priority chain made this mechanical.

**Lesson 5: Intensive development sprints benefit from a clear plan.**
The March 14-19 sprint produced 47 commits across SAX architecture, NDJSON,
SIMD, speculative parsing, Bloom filters, documentation, and PGO — without
visible rework or confusion. The two preceding dormant periods likely
involved design thinking that made the sprint focused and efficient.

______________________________________________________________________

## Appendix: Commit Log

<details>
<summary>Full condensed commit log (click to expand)</summary>

```
2026-03-19 2a4332c Refresh bench_results_small.md with updated benchmark results
2026-03-19 42f0bfe Optimize JsonCursor numeric parsing with int64/uint64 fast paths
2026-03-18 dfd6aa5 Enhance LightweightBuilder with key cache and double-buffering
2026-03-18 fe648ed Reserve double-buffer and flush logic in LightweightBuilder
2026-03-18 6da7fbd Add LightweightBuilder for optimized small JSON deserialization
2026-03-18 f5870e0 Delete legacy benchmarks; optimize inline numeric parsing
2026-03-18 23a4c6b Add Bloom filter subsystem (core, sizing, dedup, key, schema)
2026-03-18 26bebc1 Add speculative parsing engine with Markov transition model
2026-03-18 61b2175 Refactor SIMD platform detection; add STRATA_FORCE_SCALAR
2026-03-17 5b6b37b Extend StructuralIndexer: AVX-512, SVE2, RVV, WASM SIMD
2026-03-17 424c79c Remove commented-out test code
2026-03-17 2430b0b Add SIMD structural indexer (AVX2, PCLMUL, IndexBuilder)
2026-03-17 aa7d859 Optimize parser: speculative keys, whitespace, delimiters
2026-03-17 33d6835 Replace Ryu with Dragonbox; vendor fast_float library
2026-03-16 7520522 Inline float parsing, speculative key matching, unified pow10
2026-03-16 c0e3b5a Introduce FNV-1a key hashing with 75% load factor
2026-03-16 efd00fd Cap key cache at 75% load factor; reduce PGO data size
2026-03-16 a676cc4 Refresh C++ test cases across all subsystems
2026-03-16 ed80eb4 Refresh benchmark results (small, medium)
2026-03-16 3f0d7fe Unified number parsing; tight array loops; inline dispatch
2026-03-15 aa99eea Speculative key matching; optimize delimiter handling
2026-03-15 07f85a0 Remove pre-counting (benchmarks showed regression)
2026-03-15 b45b1fa Remove bench_random.py and outdated results
2026-03-15 67ef7fc Array/object pre-sizing; SIMD whitespace; fast int serialize
2026-03-15 95fd416 Optimize Python bindings for loads and dumps
2026-03-15 e9d792e Refresh benchmark results across all dataset sizes
2026-03-15 18a3521 Comprehensive Doxygen docs, nodiscard, noexcept annotations
2026-03-15 512403f Add PGO training scripts and dataset generation
2026-03-15 5c264ca Remove mem_eff mode; simplify config and bindings
2026-03-15 1ad8828 SAX-based JSONPath search for memory-efficient querying
2026-03-15 ae530e0 Refresh benchmark results across all sizes
2026-03-15 85dcf57 NDJSON thread-local builders for performance
2026-03-15 ad401a8 Add Ryu-based fast double-to-ASCII conversion
2026-03-14 2aa5566 Refactor serialization; template cycle detection; benchmarks
2026-03-14 0e6e809 Re-run benchmarks with detailed results
2026-03-14 f56e5ee Remove outdated benchmark markdown files
2026-03-14 a41eea0 Remove mmap/NDJSON bindings; unify load()/query() APIs
2026-03-14 cbc9162 Update benchmarks; enhance Python JSON bindings
2026-03-14 b1b82c0 Refactor integer serialization; improve cycle detection
2026-03-14 22c9816 Refactor cycle detection in serialization
2026-03-14 2ad6a6b Increase max serialization depth; refactor NDJSON errors
2026-03-14 d8985af Refactor JSON parsing and serialization for efficiency
2026-03-14 bcf6dd1 Batch parse_ndjson; remove memo cycle detection; simplify
2026-03-14 20ad3a0 Add NDJSON parsing with PythonObjectBuilder
2026-03-14 b66049d Simplify iterative serialization; remove memo cycles
2026-03-14 da3fe64 Replace thread-local key cache with instance-level cache
2026-03-14 8f468d2 Introduce Hybrid SAX/Visitor architecture
2026-02-02 b41fcb5 Revise agent guidelines
2026-02-02 b7f3b5d Optimize JSON parsing, serialization, and utilities
2026-02-02 d702fd7 Refactor JSONPath; add cycle detection; optimize dict conv
2026-02-01 b65dd5e Add configurable policies, validation, Python bindings
2026-02-01 7ac1789 Remove legacy C++ components and Python bindings
2025-11-23 0dd6fec Enhance JSON handling and testing capabilities
2025-11-22 6071b34 Add JSON parsing and navigation in Python/C++
2025-11-21 cd3d2c0 Add cmake-check target; integrate with Makefile
2025-11-21 a150571 Add C++ support via CMakeLists.txt (C++20)
2025-11-21 d5706a8 Delete unused CMakeLists.txt; add core C++ files
2025-11-20 81676b0 Update pyproject.toml; add clean-venv target
2025-11-20 4fc7c56 Remove setuptools_scm; add version management
2025-11-20 4f3ff0a Rename turbojsonpath to strata
2025-11-20 99dc6a5 Add benchmarking, config files, CI integration
2025-11-20 15c6c23 Update guidelines for consistent terminology
2025-11-20 2b12513 Initial commit (content)
2025-11-20 c71fcc8 Initial commit
```

</details>

## Appendix: File Hotspots

Most frequently modified files (indicates areas of active development or
benchmark-driven iteration):

| File                                         | Times Modified | Interpretation                                             |
| -------------------------------------------- | -------------- | ---------------------------------------------------------- |
| `docs/benchmarks/bench_results_small.md`     | 37             | Primary benchmark target — updated with every optimization |
| `docs/benchmarks/bench_results_medium.md`    | 32             | Secondary benchmark target                                 |
| `docs/benchmarks/bench_results_large.md`     | 30             | Large-dataset benchmark tracking                           |
| `src/strata/bindings/python_dumps.cpp`       | 25             | Serialization bindings — many optimization passes          |
| `src/strata/bindings/python_loads.cpp`       | 24             | Deserialization bindings — many optimization passes        |
| `src/strata/bindings/python_module.cpp`      | 14             | Module-level configuration and dispatch                    |
| `Makefile`                                   | 13             | Build system evolution                                     |
| `include/strata/json/json_parser_inline.hpp` | 11             | Inline parser — core hot path, many micro-optimizations    |
| `src/strata/json/json_parse.cpp`             | 9              | Main parser implementation                                 |
| `CMakeLists.txt`                             | 9              | Build targets added incrementally                          |
| `include/strata/util/fast_parse.hpp`         | 8              | Number parsing utilities — iterated heavily                |
| `src/strata/util/simd_escape.cpp`            | 7              | SIMD string escape detection                               |

**Notable pattern**: The three benchmark result files are modified 2-3x more
often than any source file, confirming the benchmark-driven development workflow.
The Python binding files (`python_loads.cpp`, `python_dumps.cpp`) are the
most-modified source files, reflecting their role as the interface between the
C++ engine and the Python API — every internal change requires corresponding
binding updates.

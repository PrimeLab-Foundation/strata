# Strata Agent Guidelines

These guidelines define how agents must work on **Strata**: a **C++20 JSON engine** with a **thin Python wrapper**.

This version explicitly **allows architectural approaches** for large performance gains (SAX/Visitor, tape,
direct-to-Python), while keeping the project **independent of any specific binding library** (e.g., pybind11).

______________________________________________________________________

## 1) North Star

- **Correctness first, then performance, then maintainability.**
- Strata’s differentiator is **best-in-class speed with low memory** for:
  - `loads` / parsing
  - `dumps` / serialization
  - NDJSON streaming/iteration
  - search/query (JSONPath-like or project-defined)

______________________________________________________________________

## 2) Architecture Boundaries

### 2.1 Core vs Wrapper

**C++ core owns all CPU-heavy work.** Implement in C++20:

- parsing / tokenization
- serialization
- NDJSON streaming
- search/query evaluation
- memory arenas / allocators
- (optional) IR formats like token tape or DOM nodes

**Python wrapper stays thin.** It may:

- normalize arguments
- dispatch into already-exposed native entrypoints
- map native errors to Python exceptions
- format return values

### 2.2 “Core Purity” rule

The **core library must remain usable outside Python**:

- Core code must not require `Python.h` (or any Python runtime types) unless explicitly placed in a Python-only
  subcomponent.
- Prefer one of these two structures:
  1. **Pure core** (no Python deps) + **Python adapter** (C-API or other binding tech)
  1. Core + optional **python_feature** target that depends on Python (kept separate, off by default)

If you propose direct-to-Python parsing, you must explicitly choose which structure you are implementing and document
the tradeoffs.

______________________________________________________________________

## 3) Binding Technology Policy (No pybind11 dependency)

### 3.1 Not tied to any binding library

Strata must **not depend** on `pybind11` as a project requirement. The binding layer may be implemented using:

- **Python C API** directly (preferred for minimal dependency)
- `nanobind`
- `cffi`/CPython extension (where appropriate)
- another binding approach *only if documented and justified*

### 3.2 Rules

- Do not design core APIs around `pybind11` types (`py::object`, casters, etc.).
- If a binding helper is needed, define **Strata-owned abstraction** in the wrapper layer:
  - e.g., `python/` or `src/bindings/python/`
- The chosen binding approach must be documented in `docs/` with:
  - build requirements
  - error mapping strategy
  - performance considerations

______________________________________________________________________

## 4) Allowed High-Impact Performance Approaches

Strata explicitly allows **any** of the following, provided correctness and docs/tests/bench gates are met.

### 4.1 SAX-style / Visitor parsing

**Goal:** emit parse events (`start_object`, `key`, `string`, `number`, …) into a handler.

**Required properties:**

- handler API lives in core (pure C++ interface)
- string/value lifetimes are documented (e.g., `std::string_view` valid only during callback)
- error model includes byte offset and reason
- iterative stack preferred over deep recursion for safety

**Why:** enables builders (DOM, tape, Python objects) without forcing one representation.

### 4.2 Token tape / IR (two-phase build)

**Goal:** parse into a compact representation (tape) first, then build target objects (DOM/Python) from tape.

**Required properties:**

- tape format documented in `docs/`
- tape building is allocation-efficient
- builder phase is tested for parity

**Why:** often yields large wins with less architectural risk than full direct-to-Python.

### 4.3 Direct-to-Python (single-pass object construction)

**Goal:** create Python objects while parsing to avoid double materialization.

**Two acceptable implementations:**

- **A) Core emits SAX events; Python builder consumes them** (recommended separation)
- **B) Python-specialized parser target** that includes `Python.h` (only if explicitly approved and kept separate from
  pure core)

**Required gates:**

- a dedicated ADR in `docs/adr/`
- strict tests for parity and error reporting
- benchmark evidence showing net improvement on canonical workloads

______________________________________________________________________

## 5) Repository Layout + Naming

### Layout

- C++ production code: `src/`
- C++ public headers: `include/`
- Python package: `python/strata/` (or `src/strata/` if src-layout)
- Binding/adapter code: `src/bindings/python/` (or equivalent explicit folder)
- C++ tests: `tests/cpp/`
- Python tests: `tests/py/`
- Benchmarks: `bench/` or `benchmarks/`
- Docs: `docs/` only

### Naming conventions

**C++**

- namespace: `strata` (and nested namespaces)
- types: `PascalCase`
- functions/methods: `snake_case`
- constants: `kPascalCase`
- filenames: `snake_case` for `.hpp/.cpp`
- prefer `#pragma once`

**Python**

- modules/functions: `snake_case`
- public classes: `PascalCase`
- internal helpers start with `_`

______________________________________________________________________

## 6) Documentation Requirements

Docs live only in `docs/`.

### ADR requirement for architectural changes

Any change that:

- introduces SAX/Visitor
- introduces a tape/IR format
- changes ownership/lifetimes or error model
- changes public API semantics
  must include a new ADR in `docs/adr/` describing:
- problem, decision, alternatives
- detailed API sketch
- risks and mitigations
- test plan and benchmark plan

______________________________________________________________________

## 7) Testing Contract (Hard Gates)

### End-of-session must be green

Before concluding work:

1. C++ tests pass
1. Python tests pass
1. new behavior has tests

### Parity tests (C++ ↔ Python)

For every exposed feature (`loads`, `dumps`, NDJSON, search):

- C++ tests cover correctness and edge cases
- Python tests cover wrapper parity (types, errors)

Minimum edge cases:

- unicode/escaping
- numeric boundaries
- malformed/truncated input with correct error offsets
- deep nesting limits / stack behavior
- NDJSON line endings and empty lines (as specified)

______________________________________________________________________

## 8) Error Handling

- Hot paths should avoid exceptions in tight loops; prefer `Result<T>` or error codes if already used.
- Errors must include:
  - byte offset
  - reason/category
  - (optional) short context snippet
- Python wrapper maps errors to stable exception types and messages.

______________________________________________________________________

## 9) Memory + Allocations

For hot path changes, explicitly evaluate:

- allocation counts
- ownership/lifetimes (RAII)
- avoiding O(n²)
- cache locality and data layout

Avoid “god files”; split > ~800 LOC unless a documented reason exists.

______________________________________________________________________

## 10) Benchmarks (Mandatory for Hot Paths)

### When required

Any change touching:

- parsing/serialization throughput
- NDJSON streaming
- search/query performance
- allocators/arenas
- representation changes (DOM/tape/SAX)
  must run benchmarks.

### Logging

Maintain append-only:

- `docs/benchmarks/progress_log.md`

Each entry must include:

- date/time + commit hash
- environment (CPU/OS/compiler/Python)
- commands
- baseline vs post metrics (min/median/p95) and memory if available
- conclusion: improved / neutral / regressed

Regressions must be fixed or reverted.

______________________________________________________________________

## 11) Automation Interface

- Automation logic goes in `scripts/`.
- Root `Makefile` is the user-facing interface and calls scripts.
- Scripts must be deterministic, fail-fast, and well-logged.

______________________________________________________________________

## 12) Required Agent Work Summary Format

Every work update must include:

1. Files changed
1. Key design decisions (especially lifetimes + errors)
1. Tests run (exact commands)
1. Benchmarks run (if applicable) + baseline/post deltas
1. Risks + follow-ups

______________________________________________________________________

## 13) Official Architecture: Hybrid Direct-to-Python Without Polluting Core

Strata officially supports a **hybrid parsing architecture** that enables “Direct-to-Python” performance while
preserving a **standalone, Python-free C++ core**.

### 13.1 Core remains independent

- The **core parsing engine** must remain **pure C++** and must **not** include `Python.h` or depend on the CPython
  runtime.
- Core must expose parsing via one of these **Python-agnostic** interfaces:
  1. **SAX/Visitor events** into a handler interface (`JsonSaxHandler`)
  1. **Tape/IR** (token tape) plus builders that consume it

### 13.2 Builders live above the core

Strata supports multiple builders that consume the core’s events or tape:

- **C++ DOM builder (pure C++)**

  - Builds Strata’s internal representation (DOM/IR) such as `strata::JsonValue`
  - Used when “heavy work stays in C++” (search/query, repeated traversals, non-Python consumers)

- **Python object builder (Python adapter only)**

  - May include `Python.h` and construct `PyObject*` **during parsing** (single-pass) **or** from tape (two-phase)
  - Must live in an explicit adapter area (e.g., `src/bindings/python/` or `python/` extension sources)
  - Must never leak Python types into core headers or core compilation units

### 13.3 Strategy selection is allowed

Public Python APIs may select the build strategy:

- **explicit mode**: `mode="py" | "dom" | "tape" | "auto"`
- **heuristic mode** (optional): choose DOM/tape for very large inputs or workflows that benefit from C++ representation

### 13.4 Binding-library agnostic requirement remains

- The project must **not require** `pybind11`.
- Python adapter implementation may use the Python C API (recommended) or another documented approach, but core APIs and
  data structures must not be designed around any third-party binding library.

### 13.5 Required documentation and gates

Any introduction or change to this hybrid architecture requires:

- an ADR in `docs/adr/` specifying:
  - interface shape (events/tape), lifetimes, and error model
  - the builder(s) implemented and where they live
  - benchmark plan and expected wins
- correctness tests (C++ + Python parity)
- benchmark evidence recorded in `docs/benchmarks/progress_log.md`

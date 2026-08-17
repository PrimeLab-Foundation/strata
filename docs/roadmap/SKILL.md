---
name: roadmap
description: The rebuild's build order — small milestones with explicit 
  acceptance criteria. Load at the start of every implementation session to pick
  the current increment; a session works on exactly one increment.
---

# Rebuild Roadmap

Milestones are ordered; each ends with the gate green (`make test` both
layers) and docs updated in the same increment. Acceptance criteria are the
definition of done — verify them, don't assume them. Ambiguities hit along
the way go to `docs/decisions.md` (see workflow.md, Precision protocol).

## M0 — Scaffolding (details: workflow.md "Milestone zero")

pyproject + setup.py + Makefile + CMakeLists; importable `strata` with
`__version__ = "2026.8.9"` (single source: `__init__.py`, pyproject reads it
dynamically); one trivial test per layer; fmt/lint/pre-commit wired; CI
skeleton.
**Done when:** `make install` runs both gates green; `make test` green;
`make fmt lint` clean; CMake is the only C++ test registry.

## M1 — Core value model & errors

`JsonValue` variant, `FlatMap` (insertion-ordered, linear scan), `Status` /
`Result<T>`.
**Done when:** C++ tests pin FlatMap ordering/lookup and Result semantics;
`grep -r Python.h include/ src/strata/{json,search,util}` is empty (core
purity, mechanically checked).

## M2 — SAX parser core

`JsonSaxHandler`, templated `ParserInline<Handler>`, `parse_sax_inline`,
`DomBuilderHandler` + `parse_json`. Unified number parsing (ints exact at any
size — slow path beyond int64), full string handling (escapes, surrogate
pairs, zero-copy views), UTF-8 validation.
**Done when:** C++ suites pin the api.md strictness contract (invalid UTF-8
byte sequences, lone surrogates, leading zeros, trailing garbage), number
edge cases incl. big-int exactness, depth-100 stress; fuzz targets compile.

## M3 — C++ serializer & dtoa

`serialize_json`; dtoa via vendored reference Dragonbox + fast_float (the
salvage from archive `main-v2` `33d6835` — not the misnamed Ryu).
**Done when:** float-precision suite green (round-trip ≤ |v|·1e-10, `.0`
retention, sci-notation boundaries); C++ round-trip tests green.

## M4 — Bindings: loads/dumps/config (correctness first)

Module init, `PythonObjectBuilder` (basic), `loads` → tree, generic `dumps`,
error mapping (`STRATA_CPP_TRY/CATCH`), config plumbing with
`cycle_policy` seeded consistently (`"warn"` active from start).
**Done when:** every loads/dumps/config clause of api.md is a named contract
test in `tests/unit` (mirrored in `tests/py`); parity vs stdlib `json` oracle
on the test corpus; error messages match the contract verbatim.

## M5 — Performance layer + benchmark harness

KeyCache + speculative key matching, small-int cache, presized dicts,
homogeneous/batch dumps fast paths, thread-local buffers. Benchmark harness,
datasets, regression checker (contract thresholds), first baseline.
**Done when:** `make bench-small` produces the report; standings recorded vs
the targets in `docs/benchmarking/SKILL.md`; regression gate operational
(its `main()` covered by a test); no gate breaches.

## M6 — NDJSON, file I/O, cursor mode

`NdjsonStream` (+ Python direct path), `load`/`dump` file mode with the
`skip_errors` contract, cursor mode (`JsonDocument`/`JsonCursor`,
cursor-mode `load`).
**Done when:** invalid-line policy tests pass (raise by default / skip on
opt-in, eager and iterator); cursor lifetime test (document kept alive);
api.md file-mode clauses pinned.

## M7 — JSONPath

Compiler (`ValueError` for every invalid expression), PyObject eval, SAX
streaming search, `CompiledPath.execute`, `query`/`search` API split.
**Done when:** grammar and error-type tests pass; property law holds on the
test corpus: `search(f, e) == query(load(f), e)` for every supported
expression (this resolves the previous implementation's SAX-vs-DOM
recursive-descent divergence in favor of query semantics — logged in
decisions.md).

## M8 — Folder mode

Folder `load`/`dump(split_by)`/`search` per api.md (discovery rules,
stringification, collision errors).
**Done when:** round-trip law property tests pass (finite floats); collision
and path-safety `ValueError` cases pinned; folder `search` law
(concat-of-files) pinned.

## M9 — Hardening & tooling completion

Fuzzing **with committed seed corpus** (the previous implementation never
created it — its fuzz CI failed at startup), coverage on the single harness,
PGO pipeline, full CI workflows, README usage refresh.
**Done when:** fuzz runs locally for `FUZZ_TIME` without startup errors;
coverage reports generate on both layers; `make pgo` completes with gates
green.

## M10 — Release readiness ✓ (released 2026-08-10 as `2026.8.10`)

Per convention: #1 rank in targeted categories with a reproducible evidence
report under `docs/benchmarks/`, docs current, version bumped to release
date.

**Closed on the quiet-machine standings sweep** (AC power, Low Power Mode
off, PGO+LTO, repeats 30/20/10): 63/81 rows #1; the users dataset leads every
category at every tier except one 1.00× large-tier dumps tie; `query` and
`search` 9/9 each, NDJSON `load` 3/3, file `load` 13/15. Evidence:
`docs/benchmarks/bench_results_{small,medium,large}.md`. The 18 rows still
behind are enumerated in `docs/benchmarking/SKILL.md` — small-document parse
overhead, the `mixed` per-call floor, and `wide_arrays` parsing at scale —
and are the post-release optimization backlog.

## M11 — The fused record writer (in progress, opened 2026-08-16; POSIX x86 criteria met same day)

Serializer redesign targeting the rows certified resistant to iteration in
`docs/decisions.md` (2026-08-15/16): `dumps mixed` on the x86 CI legs (an
engine-versus-engine gap against orjson's 3.12 wheel, ~1.07–1.09x median
under the interleaved harness, parity isolated) and the wide_arrays
serialization family. Design: `docs/architecture/fused_record_writer.md` —
one-pass emit for array-of-records documents, eliminating the collect/emit
two-pass and its staging arrays; the current path remains the fallback and
the single definition of behavior.
**Done when:** dumps mixed ranks #1 in the majority of ≥ 4 same-code CI
samples on linux-x86_64 *and* macos-x86_64; no row regresses on any leg
(classification-pair verified); both suites green; byte-identity pinned by
the round-trip oracle suites.
**Status 2026-08-16:** linux met (eight sweeps of twelve, then routine);
macos-x86_64 met in practice (official 27/27 sweeps, including a
double-sweep pair; dumps mixed #1 in both samples of later pairs). The
tracker records "Goal met on 3/4 platforms" and the milestone's live
frontier is Windows: its anchor crossed (dumps mixed #1 officially,
0.704x isolated after the string fixes and digit-writer narrowing) and
every remaining row is flip-band — the leg's first full sweep awaits a
healthy-runner sample. Remaining engineering candidate on file: the
17-digit float tier (universal 1.6x vs orjson's printer; Dragonbox
digit-gen is 41% of it and near-optimal — upstream's to_chars printer
adaptation is the recorded next idea, expected value modest).

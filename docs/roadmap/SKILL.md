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
**Status 2026-09-02:** CI was dark 2026-08-17 → 2026-09-01 (account
billing); the first sample after reads 102/108 on a Milan Windows runner
(the 104/108 sample ran on Genoa — Windows rows carry a runner-generation
axis, docs/benchmarking/SKILL.md). Wave 11 landed in-tree: the eight-digit
SWAR word plus a branch-free micro-decimal emission (6-decimal float lists
1.09x behind → 0.83x ahead of orjson; dumps wide_arrays large 0.92x →
0.85x in-process), and the raw-descriptor file reader that gives Windows
the sized read it never had (its `load` rows trailed `loads` by 1.3–2.6 ms
on 1–2 MB files). Awaiting the human push and the next four-leg sample.
**Status 2026-09-02, later:** the wave-11 push (68d6e74) drew a same-commit
CI pair, 99/108 and 101/108 — behind in both only macos-x86_64 dumps mixed
(1.01x/1.04x) and Windows dump flat (1.07x/1.04x); the Windows reader fix
verified (load minus loads 2.6 → 0.8 ms on users). Wave 12 landed in-tree:
the number scanner steps digit runs by word and a short-number head keeps
`[-]d{1..7}[.d{1..7}]` out of the full scanner entirely — 5-decimal float
lists 1.55x → 1.10x, 7-digit ints 1.52x → 1.13x, loads flat small 1.054x →
1.007x, nested 0.93x, wide_arrays large 0.91x, medium/large nested 0.93x
(same-session stash A/B, plain build, py3.14; docs/performance/SKILL.md).
Three companions measured and dropped the same day. Awaiting the human
push and the next four-leg sample; `benchmarks/decompose_loads_flat.py` is
wired into profile.yml's Windows job to name that leg's flat-row sink.
**Status 2026-09-03:** the wave-12 push drew a POSIX sample of 79/81 with
every parse row #1 on every measured leg (the Windows leg died at the C++
gate on an MSVC constant-division error in the new suite, fixed in the
follow-up). Wave 13 then moved the key probe onto a frame-owned cursor:
every keyed row another 3–9% (flat small 0.94x, nested 0.87x, users
0.84x), mixed 2%. In-tree, gated, awaiting the human push.
**Status 2026-09-03, later:** pushed as ff39a70/37a96fb; the 37a96fb sample
reads **106/108 — "Goal met on 3/4 platforms"** (linux, arm64, macos-x86_64
all 27/27; Windows 25/27 with every parse row #1). The remainder is the
Windows serializer's mixed pair (dumps 1.11x, dump 1.06x under MSVC): the
per-record machinery, the 17-digit float tier and the short-string tiers
that the Windows decomposition names — an MSVC-side increment, validated
one profile run per push.
**Status 2026-09-03, evening:** the cold-state probe named the x86 row's
mechanism (the serializer's larger cache-cold entry footprint; the parser's
is equal to orjson's), wave 14 kept one out-of-line mapping body, and the
b294ccd sample reads **105/108** with Windows' `dumps` category at #1 for
the first time; the local quiet roll on the same build reads 79/81 (large
and medium 27/27). Remaining rows are coin-band on both axes; cachegrind
counts on the Linux profile job are the next instrument for what is left
of the serializer's footprint.

## M12 — The `dumps` unsupported-type hook (`default=`)

**Status: attempted, no-go for the in-signature shape (2026-09-26).** Two
implementations met criteria 1–4, 7 and 8; criterion 5 failed and the kill
criterion fired (run 36254514783: linux-x86_64 `dumps flat` lost to the hook's
own code, reproduced across both implementations). The successor is a separate
`dumps_with_default` entry point (M12b), per the kill criterion below; the attempts are
archived on `exp/m12-default-hook-2` and `exp/m12-default-hook`
(docs/performance/experiment-ledger.md, M12).

Design: `docs/architecture/dumps_default_hook.md`. Option C: `default=callable`
lands; native emitters are deferred per type behind the record's admission gate.

**Done when:**

1. Every row of the record's error table is a named contract test in
   `tests/unit` citing its api.md clause, mirrored by integration tests in
   `tests/py`, and `docs/context/api.md` carries the signature, the chain
   bound, the key/`split_by` exclusions and the amended "Mutation during
   serialization" clause (fifth step; the "no collection can run" sentence
   corrected).
2. The E26-FIX2b prerequisite (already met by T1's destructor release,
   `python_dumps_output.h:560`) is re-pinned by a refcount test that drives a
   nested `dumps` through the hook itself and reads zero drift.
3. `make test-py-asan` green with a hook that resizes the list being written,
   clears the dict being written, and calls `dumps` re-entrantly.
4. `default=None` is byte-identical to a no-`default` call across the
   generated corpus, and `write_value`'s object code is unchanged apart from
   the tail's null test, shown by a symbolized-binary diff on both ISAs, with
   `size -m` Section `__text` growth ≤ 512 B.
5. A five-platform same-runner A/B against a tests-matched arm, both
   `make pgo`, ABBA blocks against a fresh A/A floor, resolves **no** canonical
   row of the declared 27-row workload against strata past its floor, and the
   training payload contains no `default=` call.
6. Two five-platform CI samples with complete `ci_summary` evidence lose no
   row any platform held at the 135/135 sweep.
7. `tests/integrations/` exists, is excluded from `testpaths`/`make gate`/the
   profile, and has its own `make test-integrations` target and CI job.
8. Both suites green, coverage 100% on the new lines, and the ledger carries
   the entry (go/no-go per B increment thereafter).

**Kill criterion:** any canonical row resolved against strata past its floor
whose cause is the code rather than the profile ⇒ the shape is abandoned for a
separate `dumps_with_default` entry point that cannot perturb `dumps`'s
codegen.

## M12b — `dumps_with_default`: the hook as a separate entry point (landed)

**Status (2026-09-27): merged to main as `9434607`. Criteria 1–5 and 7–9 met
(criterion 9 on its quiet-window re-measure: the first call +0.500 ms and
128 KB, `import strata` unchanged); criterion 6 unmet by its own wording —
windows-x86_64 `dumps mixed` behind on both CI samples (runs 36307283468,
36308291687), attributed to runner model
(docs/performance/experiment-ledger.md, M12b).** Open thread:

- The gate test that walks the checkout's directories into the PGO training
  profile: pinning it makes both A/B arms' training input identical and gives
  the A2 build-noise control teeth.

Design: `docs/architecture/dumps_with_default.md`. The M12 semantics
behind `strata.dumps_with_default(obj, default, *, return_type="str")`, served
by a second extension module (`strata._dumps_hook`) compiled from the same
serializer source, so that `strata._strata` does not change at all.

**Done when:**

1. Every row of the record's error table is a named contract test in
   `tests/unit` citing its api.md clause, mirrored by integration tests in
   `tests/py`; `docs/context/api.md` carries the signature, the chain bound,
   the key exclusion, the "`default=None` is refused" rule, and a mutation
   clause for this entry point (five steps) while `dumps`'s clause stays
   main's text (four).
2. E26-FIX2b is re-pinned in both directions: a refcount test drives a nested
   `dumps` and a nested `dumps_with_default` through the callable and reads
   zero drift.
3. `make test-py-asan` green with a callable that resizes the list being
   written, clears the dict being written, and calls both entry points
   re-entrantly.
4. **Zero diff for `_strata`, not a budget**: (a) `clang++ -E -P` of every
   `_strata` TU emits the same token stream as main's; (b) every `_strata`
   object's `__text` is byte-identical to main's on arm64 and x86-64 (plain
   `-O3`, the codegen scripts of M12); (c) the linked plain `_strata`
   extension's `__TEXT` and `__DATA` section bytes are identical to main's;
   (d) `setup.py`'s `_strata` `Extension` (sources, order, flags) is
   unchanged. The hook image's own `__text` is reported, not bounded.
5. The five-platform same-runner A/B of M12 criterion 5, unchanged: both arms
   `make pgo`, tests-matched (both on main's gate suite; B carries the hook
   module), 6 ABBA blocks × 60 against a fresh A/A floor, no row of the 21
   instrumented canonical rows resolved against strata past its floor; before
   any timing, the runner's two PGO `_strata` binaries are compared, and a
   `__text` difference stops the run for attribution. **Met 2026-09-27**
   under the record's rule (runs 36279771980, 36291977906, 36297571366):
   identity shown on all five legs — B's source against A's profile, one build
   path; byte for byte on four, normalised on linux-arm64 — and no resolved
   loss repeated on one leg (docs/performance/experiment-ledger.md, M12b).
6. Two five-platform CI samples of the shipped build (whose profile includes the
   new tests), each with complete `ci_summary` evidence: no row behind on both
   samples; a row behind on one sample must sit inside its historical band
   across the archived samples (`benchmarks/cross_sample.py`) or carry a host
   attribution. (Amended 2026-09-27 from "lose no row any platform held at the
   135/135 sweep": docs/decisions.md.)
7. `tests/integrations/` carried over and re-targeted, excluded from
   `testpaths`/`make gate`/the profile, with `make test-integrations` and its
   CI job.
8. Both suites green, coverage 100% on the new lines of both images, the
   ledger carries the entry.
9. The hook image is imported on the first `dumps_with_default` call, not by
   `import strata`: with the hook image missing, `import strata`, `dumps` and
   `loads` still work and `dumps_with_default` raises `ImportError` on every
   call (tested). `import strata`'s own wall time is unchanged against main, and
   the first call's added wall time and resident memory — measured in fresh
   processes (≥ 30 repeats) on the dev M1 in a quiet window — are ≤ 1 ms and
   ≤ 1 MB (bounds signed off 2026-09-26; amended to first-call cost
   2026-09-27).

**Kill criterion:** if criterion 4 cannot be met, the isolation design is wrong
and nothing is timed; if it is met and criterion 5 still resolves a loss, the
cause is host or profile nondeterminism and goes to a second draw and the
identical-binary control, not to the code.

## M13 — Tier-1 framework adapters (landed 2026-09-29)

Depends on M12b: the adapters hand frameworks `dumps_with_default` for their
unsupported types, so M13 starts when M12b's criteria are met.

Flask (`json_provider_class`), Django (`JsonResponse(encoder=)`), aiohttp
(`dumps=`), Falcon (`media.JSONHandler`), structlog (`JSONRenderer(serializer=)`).
One `python/strata/integrations/<framework>.py` each (~50 LOC of glue, lazy
framework import, `import strata` imports none of them — pinned by a test);
per-adapter round trip through the framework's own test client against its
default serializer as oracle; error-mapping tests; semantic-differences table
in the adapter docstring and docs; integrations CI job green on the supported
version floors. `__all__` does not grow.

Landed with the M15b reconciliation (docs/decisions.md, 2026-09-29): the
adapters that hand strata a framework `default` (Flask, Django, structlog,
pydantic) call `dumps_with_default(..., native=False)`, a hook-only per-call
opt-out of native precedence, so every framework's own formatting of
`datetime`, `Decimal`, `UUID` and dataclasses is kept; aiohttp, Falcon and
FastAPI stay on plain `dumps`. `_strata` unchanged.

## M14 — Tier-2 adapters (planned)

FastAPI/Starlette (bytes-mode response class), DRF, Sanic, Litestar,
SQLAlchemy, Celery/kombu, structlog `__structlog__`-protocol follow-ups,
Pydantic-as-post-step. Same rules as M13; each admitted only with a verified
hook contract (four unverified facts from the 2026-09-18 survey pinned against
source first). FastAPI (`StrataJSONResponse`, responses only) and
Pydantic-as-post-step (`strata.integrations.pydantic`) landed ahead of the rest
on 2026-09-28 with contracts read from source
(docs/architecture/framework_adapters.md; docs/decisions.md, 2026-09-28).

## M15 — Native types, default-on (refused, opened 2026-09-28, closed 2026-09-29)

Record: `docs/architecture/native_types.md`. Serializer natives on by default in
`dumps`/`dump`/`dumps_with_default`; opt-in `parse_types` on
`loads`/`load`/`search`/`query`. Acceptance had been:

1. Every clause of the record's serializer, parse and error contracts is a named
   test (`tests/unit/native_types/`, `tests/py/native_types/`), the temporal
   core has C++ tests (`tests/cpp/test_temporal.cpp`), numpy and the orjson
   differential live in `tests/integrations/`; `make test`, `make test-py-asan`
   and `make test-integrations` green.
2. Static: `write()` differs from main only in its tail (arm64, x86-64);
   `sizeof(Serializer)`, `dumps_to_python`'s frame and `stage_`'s offset
   unchanged; parser and builder symbols byte-identical; Section `__text`
   growth reported per ISA.
3. Training scope: the new tests are outside the instrumented pass on all three
   PGO scripts; the native functions carry zero profile counts.
4. `import strata` imports no native-type module; import time unchanged.
5. Admission: each type's native path is faster per object than
   `dumps_with_default` with its reference conversion (≥ 30 repeats).
6. Pinned-profile local A/B against main: no canonical row past its A/A floor
   beyond +1.7%, none past +2%.
7. Five-leg A/B (`ab_x86.yml`, paired then canonical): no row past +2%
   resolved on two draws with the held-profile arm attributing it to code.
8. `docs/context/api.md`, `docs/bindings/SKILL.md`, the ledger and
   `docs/decisions.md` updated in the same change.

**Refused by its own kill criterion (7).** Draw 2 (run 36520746091) and draw 3
(run 36529483744) both resolved macos-x86_64 small `dumps nested` bytes past
+2% on a clean-control leg (draw 2 +3.98% \[+2.37, +4.67\], draw 3 +3.74%
\[+0.47, +4.83\]), the row's own A2 resolving nothing in either draw — two
draws past +2% with the held-profile arm attributing it to code, exactly the
criterion's fire condition. No canonical dispatch followed. Ledger:
`docs/performance/experiment-ledger.md`, "M15 — native types: static checks,
training scope, the local A/B; kill fired on draws 2–3"; decisions:
`docs/decisions.md`, 2026-09-29 (the draw-2 and draw-3 entries). Default-on's
two measured costs (the V4 tail in `write()`, and the ~20 KB `_strata` text
growth from the always-linked native writers) motivated the successor's shape.
Superseded by M15b.

## M15b — Native types behind a flag (in progress, opened 2026-09-29)

User-directed successor to M15 (the M12 → M12b precedent: a refused shape
stays whole as its own record; the successor is a new milestone). Design:
`docs/architecture/native_types.md`, "Flag shape (M15b)"; decision:
`docs/decisions.md`, 2026-09-29 ("User-directed flag shape (M15b) supersedes
default-on and fallback (b)"). `_strata` builds byte-identical to main
`38eaa9f`, serializer and parser both; every native — the serializer rows 1–9,
`parse_types` — lives in `strata._dumps_hook`, reached by facade dispatch on
`native=`/`parse_types=`. Both of M15's measured costs are gone by
construction, so the default path's acceptance is an identity proof, not an
A/B campaign.

Acceptance is `docs/architecture/native_types.md`, "Flag shape (M15b)" →
"Acceptance", in full:

Default path (the M12b identity proof, each an observable):

1. Token streams: every `_strata` translation unit, preprocessed with
   `_strata`'s flags, equals main `38eaa9f`'s, on arm64 and x86-64.
2. Build spec: `_strata`'s `Extension` (sources, order, macros, compile and
   link arguments) and `core_sources.txt` equal main's.
3. Plain images: `_strata`'s code section equals main's
   (`benchmarks/image_identity.py`).
4. Held profile: this branch's `_strata` built PGO+LTO against main's profile,
   in the same path, equals main's image built against it — locally on the M1,
   and on every CI leg in the prepared dispatch (prepared, not yet
   dispatched — no third CI spend without sign-off, per the lead's rule that
   closed M15).
5. Training scope: the native suites stay outside the instrumented pass;
   trained-scope test files equal main's except the recorded removals.

Opt-in path: the existing `tests/{unit,py}/native_types/` corpora pass through
the flag; `native=False` raises main's `TypeError` for every native family;
`dumps_with_default` serves natives first; `import strata` imports neither the
hook image nor a native-type module; the facade's per-call cost is measured in
ns for every routed function (evidence `docs/benchmarks/evidence/M15b/`).

Benchmarks: a separate declared workload, `native-v1`, reported in its own
`ci_summary` section; the canonical 27 rows and 135 denominator unchanged.

`docs/context/api.md`, `docs/bindings/SKILL.md`, the ledger and
`docs/decisions.md` updated in the same change as the code.

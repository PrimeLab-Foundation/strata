---
name: history
description: Project lineage and the archive map — turbojsonpath origins, the 
  strata rename, the abandoned optimization campaigns and rewrite attempts, the 
  shelved new_strata rearchitecture, and where every artifact lives under 
  ../archive/. Load when archaeology, context, or "has this been tried?" 
  questions come up.
---

# Project History & Archive Map

The git history of this repo was reset to a single fresh commit in Aug 2026,
containing **documentation only** — the implementation was deliberately removed
for a docs-driven from-scratch rebuild. Full prior history (including the
complete v0.2.0 implementation) is preserved in `../archive/` and on the
`backup/pre-reset-main` branch (also `origin/main` prior to the reset). This
doc is the narrative index.

## Lineage

1. **turbojsonpath** (Nov 2025, `../archive/turbojsonpath`, 6 commits + ~1 week
   pre-git) — a streaming JSONPath engine wrapping **simdjson + jsoncons** via
   pybind11, with predicate pushdown, a parallel NDJSON executor (2.6–2.7× on
   256MB), and mmap I/O. Its lasting output was benchmark methodology: the
   9-library harness, the users/orders/items dataset generator (carried
   forward and evolved in place), and **strict mode** (no silent Python
   fallbacks — unsupported = excluded, not faked). It lost to orjson on
   whole-document workloads (58 ms vs 42 ms on `$.users[*].id`, per
   `BENCH_RESULTS.md` at commit `3417966` — later shrunk in `593e1ff`), which
   motivated the dependency-free rewrite.

2. **Rename + reboot** (Nov 20 2025) — strata born as a rename-plus-rewrite:
   the old C++ core was parked as inert reference (`cpp_bkp/`, deleted Feb 2026)
   and a new engine grew from a toy `JsonValue` DOM. `../archive/bkp_strata`
   preserves exactly these first 12 commits (a strict ancestor of the pre-reset
   history). The "C++ owns CPU, Python is a facade" doctrine dates from day one.

3. **The big import** (Feb 1–2 2026, commit `7ac1789`, +40k lines) — results of
   an out-of-git "2026-01 migration": production layout (`include/` + `src/` +
   `python/`), raw C-API bindings replacing pybind11, SIMD utils, the 17
   `.aiassistant` rules. Session logs lived in `docs/archive/2026-01-migration/`
   (in the backup history).

4. **The 0.1 research campaign** (Feb 8–22 2026, branch `main-v2-0.1` in
   `../archive/strata`, 77 commits) — breadth-first optimization exploration.
   Produced the quantified findings that shaped everything after: structural
   tape OFF = +22% large loads, static dispatch +13%, thin-LTO +5–6%, PGO+LTO
   ~+15%, plus documented null results (string pooling, dict micro-opts, object
   pools, parallel parse). Distilled into 12 reconstruction prompts
   (`docs/prompts/init.md` @ `b56d372`, branch-only). Never merged.

5. **The clean rebuild = current architecture** (Mar 14–16 2026) — starting at
   `8f468d2` "Hybrid SAX/Visitor", re-implemented the campaign's validated
   winners in ~30 commits and became production main (tip `c0e3b5a`, the state
   this fresh history was cut from). See `docs/performance/SKILL.md` for the
   commit-by-commit optimization narrative.

6. **Post-tip experiments** (Mar 16 – Apr 2026, all unmerged, in `../archive/strata`):

   - `main-v2` — main + 3 commits incl. the **Dragonbox dtoa swap** (`33d6835`);
     the top salvage candidate.
   - `main-v2-long-run-do-nothing` — control run that still produced real work
     (`5e04a18`: LastWins policy, SWAR ASCII, pending-key path).
   - `main-v2-prompt-1..4` — stacked agent-session snapshots: SIMD structural
     indexer, Markov speculative parser, Bloom filters, `LightweightBuilder`,
     `ParserIndexed`; bench payoff ≈ noise (never wired into hot paths).
   - `main-from-scratch-v1/v2` — tree wiped, C++-only primitives rebuild;
     beat nlohmann 5–12× on scalars (1.6–3.2× on structures) but lost to
     simdjson 3.4× on realistic structures; abandoned.

7. **new_strata rearchitecture** (May 31 – Jun 12 2026, `../archive/new_strata`,
   not a git repo) — a from-scratch **C11 core + C++17 SIMD kernels + C-API
   bindings** engine. Seven ADRs: c-core-cpp-kernels, cpython-c-api-binding,
   arena DOM, runtime SIMD dispatch (CPUID/XGETBV), Ryū double formatting,
   direct-to-PyObject decode, PGO build. Further engine features beyond the
   ADRs: genuine Dragonbox (Ryū as fallback), Eisel–Lemire parsing,
   simdjson-style structural index/validate, lazy search navigator,
   `search_many` path-trie. Self-reported **#1 in 23/23 benchmark rows** in
   START_HERE.md (its checked-in results.md shows 22/23, with parse·nested a
   ≈tie #2 behind orjson) on an M1 Max vs orjson/msgspec/source-built
   pysimdjson/ujson, with 983 Python + 691 C tests green. Shelved complete;
   **nothing was merged back**. Its `docs/orjson-gap-analysis.md` and lab-log
   CLAUDE.md are the best reading on where orjson's remaining edges come from
   (codegen monomorphization, compact-int internals) and on "structural wall"
   verdicts that later fell to profiling.

## Archive map

| Path                                       | What it is                                                                                                                                                                                                                                                                                                                                  |
| ------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `../archive/turbojsonpath`                 | Original project, 6 commits + uncommitted final reports                                                                                                                                                                                                                                                                                     |
| `../archive/bkp_strata`                    | First 12 strata commits (ancestor of pre-reset main)                                                                                                                                                                                                                                                                                        |
| `../archive/strata`                        | Full pre-reset repo: 167 commits, all experiment branches listed above                                                                                                                                                                                                                                                                      |
| `../archive/new_strata`                    | The shelved 2026-06 rearchitecture (working tree, builds included)                                                                                                                                                                                                                                                                          |
| `../archive/strata.zip`                    | Feb 2026 full snapshot incl. venv — redundant with the git repos                                                                                                                                                                                                                                                                            |
| branch `backup/pre-reset-main` (this repo) | The 49-commit pre-reset main line                                                                                                                                                                                                                                                                                                           |
| tags `archive/<branch>` (origin and local) | Every experiment and pre-reset branch as it stood on 2026-09-07, when the branches themselves were deleted: `archive/main-v2*`, `archive/main-from-scratch-v1`, `archive/exp/*` (the wave-24/25 records, the rejected levers, the campaign's fix and review branches). A doc that names a branch resolves through the tag of the same name. |

## Hygiene lessons already paid for (don't repeat)

Committed build binaries and `.profraw` files; commit messages describing a
different change than the diff; silently drifting mirrored test files; three
divergent AI-guidance rule sets (now consolidated into `docs/context/`);
generated benchmark markdown overwritten with no append-only progress log.

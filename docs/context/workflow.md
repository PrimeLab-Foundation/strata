# Development Workflow

How a working session on strata proceeds, start to finish. The rules referenced
here are defined in `docs/context/convention.md`; this file sequences them.

## Session start

1. Load context (this folder) and the skill doc for the area you will touch
   (`docs/<topic>/SKILL.md` — see the index in `CLAUDE.md`).
2. Check repo state: `git status` clean? tests green (`make test`)? If the
   tree or suites are broken, fix or flag that first — never build on red.
3. For rebuild work: read the relevant blueprint before writing code — the
   previous implementation on `backup/pre-reset-main` / `../archive/strata`
   is the reference, `docs/` is the contract. Where they disagree, the docs
   win (they deliberately drop known bugs and dead code).

## Change loop

1. **Design first.** State the contract of the change: inputs, outputs,
   errors, allocations, lifetimes. Architecture-level changes need a decision
   record in `docs/architecture/` before code.
2. **Implement bottom-up: C++ core → bindings → Python facade.** CPU work
   never lands in Python (convention rule 1). Keep the facade logic-free.
3. **Test both layers as you go**, not at the end: C++ test (register in both
   CMakeLists.txt and `scripts/run_cpp_tests.sh`) plus mirrored
   `tests/py` + `tests/unit` contract tests. Error messages are part of the
   contract (`docs/context/api.md`).
4. **Benchmark if you touched a hot path** (parse/serialize/NDJSON/search/
   allocations/flags): capture a baseline before the change, compare after
   with `make bench-small` at minimum. Apply the regression thresholds
   (>2% median/p95 or >5% memory ⇒ fix or revert). Check the
   negative-results table in `docs/performance/SKILL.md` before attempting
   any optimization idea.
5. **Update docs in the same change**: `docs/context/api.md` for public API,
   the matching skill doc for behavior/architecture, and record experiment
   outcomes (go or no-go) in `docs/performance/SKILL.md`.

## Experiments

Hypothesis → minimal prototype under `experiments/` (never linked into
production) → microbenchmark with real datasets → written go/no-go conclusion
in `docs/performance/SKILL.md`. Adopted techniques then go through the normal
change loop (tests both layers + benchmarks + docs). No conclusion, no merge.

## Session end (the gate)

1. `make test` green on both layers — never end a session red, even for
   single-layer changes. `make gate` for the full compliance pass.
2. No uncommitted benchmark regressions; refreshed results in
   `docs/benchmarks/` when a perf path was touched.
3. Report using the agent output format (convention, last section): files
   changed; key decisions (lifetimes, errors, allocations); tests added/run
   with commands; benchmark deltas; risks and follow-ups.

## Commits

- **All commits and pushes are made by the human.** The agent never runs
  `git commit` or `git push`; it prepares the change and hands over a short,
  ready-to-use commit message.
- Message style: imperative summary of what changed and why it is
  faster/safer (one line, ≤ 72 chars; a short body only when truly needed);
  mention refreshed benchmark results when applicable.
- **Git history never mentions AI involvement**: no AI/agent/assistant
  references, no co-author or generated-with trailers. Messages describe the
  change, not how or by whom it was produced.

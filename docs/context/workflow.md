# Development Workflow

How a working session on strata proceeds, start to finish. The rules referenced
here are defined in `docs/context/convention.md`; this file sequences them.

## Milestone zero (bootstrap)

The gates below assume tooling that the fresh-start tree does not have yet.
They bind progressively: each gate applies from the moment its tooling
exists, and building that tooling is the first work item, in this order:

1. Packaging + build skeleton: `pyproject.toml`, `setup.py`, `Makefile`,
   `CMakeLists.txt` — `make install` produces an importable `strata` with
   `__version__`.
2. One trivial green test per layer (C++ via ctest, Python via pytest) —
   from here the green gate and test-gated install apply.
3. Lint/format configs wired (`make fmt lint`, pre-commit) — style gates apply.
4. Benchmark harness + datasets — from here the benchmark gates apply and a
   baseline is captured.

Until an item exists, sessions may not be blocked on it — but may not skip
building it either when their work depends on it.

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
3. **Test both layers as you go**, not at the end: C++ test (registered in
   CMakeLists.txt — the single test registry) plus mirrored
   `tests/py` + `tests/unit` contract tests. Error messages are part of the
   contract (`docs/context/api.md`).
4. **Benchmark if you touched a hot path** (parse/serialize/NDJSON/search/
   allocations/flags): capture a baseline before the change, compare after
   with `make bench-small` at minimum, and apply the regression gate in
   `docs/context/benchmarks.md` (breach ⇒ fix or revert). Check the
   negative-results table in `docs/performance/SKILL.md` before attempting
   any optimization idea.
5. **Update docs in the same change**: `docs/context/api.md` for public API,
   the matching skill doc for behavior/architecture, and record experiment
   outcomes (go or no-go) in `docs/performance/SKILL.md`.

## Precision protocol

Rules that make correctness mechanical rather than a matter of judgment:

1. **One increment per session.** Pick the current milestone from
   `docs/roadmap/SKILL.md` and touch only what it names. Its acceptance
   criteria are the definition of done — verify each one, don't assume.
2. **Traceability.** Every behavioral clause in `docs/context/api.md` becomes
   a named contract test citing the clause it pins (a comment is enough).
   Behavior change ⇒ api.md, the test, and `docs/decisions.md` move in the
   same change.
3. **Oracle diffing, not self-assessment.** Verify correctness by diffing
   against executable oracles: stdlib `json` for JSON semantics, and the
   previous implementation (build it once from `backup/pre-reset-main`) for
   strata-specific behavior. Any divergence is either a documented
   do-not-reproduce bug or a new line in `docs/decisions.md` — never
   unexplained.
4. **Never invent.** No path, function, flag, or behavior that isn't in the
   docs or the blueprint. If docs and reality disagree, stop and reconcile —
   don't improvise around it.
5. **Ambiguity goes to the log.** When the spec underdetermines a choice,
   append one line to `docs/decisions.md` (date · area · decision) and
   mention it in the session report. Deciding silently is the one
   unforgivable failure mode.

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

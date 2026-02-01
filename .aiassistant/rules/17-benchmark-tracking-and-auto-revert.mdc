---
alwaysApply: true
---
# Rule: Benchmark Tracking, Regression Detection, and Revert-on-Degradation

The agent must continuously run benchmarks, record results, and automatically revert changes that degrade performance beyond an allowed threshold.

## Mandatory benchmark loop
For any change that can affect performance (parser, serializer, NDJSON, search/query, memory/buffers, bindings):
1) Capture **baseline** benchmark results before the change.
2) Implement the change.
3) Run **post-change** benchmarks.
4) Compare results and decide go/no-go.

Benchmarks must use the project’s canonical suite, datasets, and methodology (warmup, repeats, min/median/p95, env capture).

## Tracking and logging
- Maintain a single benchmark log file under docs (append-only), e.g.:
  - `docs/benchmarks/progress_log.md`
- Each entry MUST include:
  - date/time
  - commit hash
  - environment (CPU/OS/compiler flags/Python)
  - commands used
  - key metrics (min/median/p95 + memory where available)
  - conclusion: improved / neutral / regressed

## Regression definition and thresholds
- A change is considered a regression if:
  - median OR p95 worsens by > **2%** in any primary benchmark category it touches, OR
  - memory (RSS/peak) worsens by > **5%** where measured, OR
  - variance increases materially (unstable results) without explanation.
- Thresholds may be tightened as the project matures; any change to thresholds must be documented in docs.

## Revert-on-degradation behavior
If a regression is detected:
1) **Do not proceed** with the change.
2) Revert the offending commit(s) (or undo the patch) to restore baseline performance.
3) Add a short note to `docs/benchmarks/progress_log.md`:
   - what regressed
   - suspected cause
   - next experiment to try
4) Only keep the change if:
   - you can fix the regression and re-benchmark to confirm recovery, OR
   - there is an explicit, documented exception approved in-repo (rare).

## Fairness and correctness guardrails
- Do not “win” by changing semantics or reducing correctness.
- If semantics differ from competitors, isolate those benchmarks or document mismatch.
- Benchmarks must fail-fast: no partial tables with ERROR rows.

## End-of-session requirement
Before ending any session, if you touched a perf-sensitive area, you MUST provide:
- baseline vs post-change deltas (table or bullets)
- a link/path to the updated progress log entry
- confirmation that no regressions remain

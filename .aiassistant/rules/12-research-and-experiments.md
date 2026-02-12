---
alwaysApply: true
---
# Rule: Research-Driven Experimentation (Math + Algorithms)

Strata aims to be best-in-class. The agent must actively explore state-of-the-art mathematical and algorithmic ideas **without compromising correctness, safety, or maintainability**.

## Core requirement
For any performance-critical feature (parsing, dumping, NDJSON, search/query, memory), the agent MUST propose and, when reasonable, prototype **at least 1–3 research-grade approaches** in addition to straightforward implementations.

## What “research-grade” means (examples)
- Parsing/serialization:
  - SIMD-friendly token classification and vectorized scanning
  - branchless state machines, DFA minimization
  - cache-aware chunking, prefetch, arena allocators
  - UTF-8 validation via vectorized algorithms
- Search/query:
  - compiled query plans / bytecode interpreters
  - trie/automaton approaches for JSONPath-like matching
  - bitset indexes, path hashing, structural indexing
- Memory:
  - small-buffer optimizations, tagged unions
  - zero-copy views with lifetime proofs
  - slab/arena + bump allocators tuned for JSON shapes
- Concurrency:
  - parallel parsing of large inputs (safe chunk boundaries)
  - pipelined NDJSON processing

## Process discipline
- Experiments MUST be isolated under `experiments/` (or an approved folder) and must not pollute production code.
- For each experiment, produce:
  1) a short hypothesis (why it should be faster/smaller)
  2) a minimal prototype
  3) microbenchmarks + datasets
  4) a go/no-go conclusion with evidence (speed, RSS, complexity)
- Any adopted technique must include:
  - correctness tests (C++ + Python parity)
  - benchmark coverage
  - documentation under `docs/` explaining the technique and tradeoffs

## Guardrails
- Never sacrifice correctness for speed.
- Avoid “clever” changes without measurable wins.
- If a technique is too complex, keep it as an experiment and document why it was rejected.

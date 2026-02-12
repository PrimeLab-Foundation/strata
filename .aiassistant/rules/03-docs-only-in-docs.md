---
alwaysApply: true
---
# Rule: Documentation Lives Only in /docs

All documentation must live under `docs/`.

## Requirements
- Put all design notes, ADRs, API docs, benchmark methodology/results, performance notes under `docs/`.
- Root `README.md` may exist but should be minimal and link into `docs/`.
- All `.md` file names should lowercased except `README.md`
- Do not create docs elsewhere (no scattered `.md` in root or random folders).

## Practical
- If you change behavior, benchmarks, API, or constraints → update or add docs under `docs/` accordingly.

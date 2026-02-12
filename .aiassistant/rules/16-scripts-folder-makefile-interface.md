---
alwaysApply: true
---
# Rule: Scripts Live in `scripts/` and Makefile Is the Single Interface

All automation must be implemented as scripts under `scripts/`. The Makefile is the **only** user-facing entrypoint.

## Requirements
- Any new automation logic MUST be placed in `scripts/`:
  - build, test, lint, format, bench, coverage, release, packaging, PGO/LTO workflows, etc.
- The root `Makefile` must provide the stable interface:
  - targets call into `scripts/*` (no complex logic embedded directly in Makefile)
  - Make targets should remain thin wrappers that forward args/env to scripts
- Do not add ad-hoc shell one-liners in docs or CI as the primary interface; wire them through Makefile → scripts.

## Script standards
- Scripts must be:
  - deterministic (fail-fast, non-zero exit on error)
  - portable where practical (prefer bash + POSIX; document requirements if using bashisms)
  - well-logged (clear start/end, key commands, and errors)
- Provide `scripts/common.sh` for shared helpers (logging, OS detection, argument parsing) instead of copy/paste.

## Documentation
- Docs must reference Make targets as the canonical commands (e.g., `make test`, `make bench`).
- Scripts are referenced indirectly via Make unless deep debugging is needed.

---
alwaysApply: true
---
# Rule: End-of-Session "Green Tests" Gate (C++ + Python)

You may not finish a session in a failing state.

## Non-negotiable exit condition
Before concluding a session, ensure:
1) All relevant tests executed
2) All tests pass (C++ + Python)
3) New/modified behavior has tests on BOTH sides when applicable

If anything fails:
- diagnose root cause
- fix
- re-run failing tests
- re-run full suites
Repeat until green.

## Required execution (both sides)
- Run C++ tests (native unit/integration)
- Run Python tests (wrapper + behavior-level)
Even if you changed only one layer, you must run both.

## Commands (canonical by default)
- C++: `ctest` (or project’s canonical C++ runner)
- Python: `pytest` (or project’s canonical Python runner)

## No papering over failures
Do not skip/xfail/disable tests to hide failures unless explicitly documented and approved in-repo.

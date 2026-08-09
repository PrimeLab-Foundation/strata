#!/usr/bin/env bash
#
# Full compliance gate (docs/context/workflow.md § Session end).
#
#   Step 1  C++ test suite
#   Step 2  force reinstall of the extension — itself test-gated
#   Step 3  Python test suites (tests/py + tests/unit)
#
# Coverage collection is the remaining documented phase; it lands together with
# the coverage tooling in milestone M9. Steps are never suffixed with `|| true`:
# the previous implementation's gate printed "GATE PASSED" over a permanently
# broken coverage stage, and a step that cannot fail is not a gate.
set -euo pipefail

cd "$(dirname "$0")/.."

VENV="${VENV:-.venv}"
PY="$VENV/bin/python"
[ -x "$PY" ] || PY="$(command -v python3)"

step() {
    echo
    echo "════════════════════════════════════════════════════════════════"
    echo "  $1"
    echo "════════════════════════════════════════════════════════════════"
}

step "Step 1/3: C++ test suite"
"$PY" scripts/cpp_tests.py

step "Step 2/3: Reinstall the extension (test-gated build)"
"$PY" -m pip install --force-reinstall --no-deps -e .

step "Step 3/3: Python test suites"
"$PY" scripts/py_tests.py

step "GATE PASSED"

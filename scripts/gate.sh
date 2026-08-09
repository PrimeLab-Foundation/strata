#!/usr/bin/env bash
#
# Full compliance gate (docs/context/workflow.md § Session end).
#
#   Step 1  C++ test suite
#   Step 2  force reinstall of the extension — itself test-gated
#   Step 3  Python test suites (tests/py + tests/unit)
#   Step 4  coverage, both layers
#
# Steps are never suffixed with `|| true`: the previous implementation's gate
# printed "GATE PASSED" over a permanently broken coverage stage, and a step
# that cannot fail is not a gate.
#
# GATE_SKIP_COVERAGE=1 drops step 4 for a quick pass. It is a convenience for
# iterating, not an escape hatch — the step it skips reports numbers, it does
# not decide pass or fail.
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

step "Step 1/4: C++ test suite"
"$PY" scripts/cpp_tests.py

step "Step 2/4: Reinstall the extension (test-gated build)"
"$PY" -m pip install --force-reinstall --no-deps -e .

step "Step 3/4: Python test suites"
"$PY" scripts/py_tests.py

if [ "${GATE_SKIP_COVERAGE:-0}" = "1" ]; then
    step "Step 4/4: Coverage — SKIPPED (GATE_SKIP_COVERAGE=1)"
else
    step "Step 4/4: Coverage (both layers)"
    bash scripts/coverage.sh all
fi

step "GATE PASSED"

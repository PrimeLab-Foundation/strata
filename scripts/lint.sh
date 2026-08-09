#!/usr/bin/env bash
#
# Lint the Python sources with ruff (.ruff.toml). Formatting lives in fmt.sh.
set -euo pipefail

cd "$(dirname "$0")/.."

VENV="${VENV:-.venv}"

if [ -x "$VENV/bin/ruff" ]; then
    RUFF="$VENV/bin/ruff"
elif command -v ruff >/dev/null 2>&1; then
    RUFF="$(command -v ruff)"
else
    echo "error: 'ruff' not found. Install the dev extras: make dev" >&2
    exit 1
fi

echo "+ $RUFF check ."
"$RUFF" check .

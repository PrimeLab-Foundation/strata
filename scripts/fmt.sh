#!/usr/bin/env bash
#
# Format every source file in place: ruff for Python, clang-format for C/C++.
# Style is defined by .ruff.toml and .clang-format (docs/context/styleguide.md).
set -euo pipefail

cd "$(dirname "$0")/.."

VENV="${VENV:-.venv}"

pick() {
    if [ -x "$VENV/bin/$1" ]; then
        echo "$VENV/bin/$1"
    elif command -v "$1" >/dev/null 2>&1; then
        command -v "$1"
    else
        echo "error: '$1' not found. Install the dev extras: make dev" >&2
        exit 1
    fi
}

RUFF="$(pick ruff)"
CLANG_FORMAT="$(pick clang-format)"

echo "+ $RUFF format ."
"$RUFF" format .

# clang-format covers include/ and src/ — the previous implementation excluded
# them "temporarily" and the exclusion outlived the reason.
#
# --cached --others --exclude-standard: files added in this working tree but not
# committed yet must be formatted too, otherwise new code escapes the formatter
# until the commit that introduces it has already landed.
cxx_sources() {
    git ls-files -z --cached --others --exclude-standard \
        '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hpp'
}

# Source discovery goes through git, so a non-checkout must fail loudly: inside
# an `if` the pipeline's non-zero status is swallowed, and the else branch would
# report "no C/C++ sources" while silently leaving every file unformatted.
if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    echo "error: not a git repository — fmt.sh discovers C/C++ sources with git." >&2
    exit 1
fi

if cxx_sources | grep -qz .; then
    echo "+ $CLANG_FORMAT -i"
    cxx_sources | xargs -0 "$CLANG_FORMAT" -i
else
    echo "no C/C++ sources yet — skipping clang-format"
fi

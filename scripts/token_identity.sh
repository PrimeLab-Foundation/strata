#!/usr/bin/env bash
# M15b criterion 1: every `_strata` translation unit preprocesses to the same
# token stream on this tree as on main.
#
# Usage: token_identity.sh [<base-ref>]   (default 38eaa9f, main at the M15b design commit)
#
# For each source of `_strata` (setup.py's binding list + src/strata/core_sources.txt,
# read from each tree), run `clang++ -E -P` with the plain-build defines and include
# paths for both ISAs, then compare the outputs as token sequences (whitespace
# normalised: -E's spacing around expanded macros is not part of the language).
#
# Ported from build/evidence/benchmark-lead/M12b/zero-diff/token_identity.sh
# (M12b criterion 4(a)), the same method against this record's base commit.
set -euo pipefail
TREE="$(cd "$(dirname "$0")/.." && pwd)"
BASE_REF="${1:-38eaa9f}"
OUT="$TREE/build/token_identity" # build/ is gitignored; this script's output never belongs in git
PYTHON="$TREE/.venv/bin/python"
[ -x "$PYTHON" ] || PYTHON="python3"
PYINC="$("$PYTHON" -c 'import sysconfig; print(sysconfig.get_paths()["include"])')"
rm -rf "$OUT" && mkdir -p "$OUT/base-src"
git -C "$TREE" archive "$BASE_REF" src include setup.py | tar -x -C "$OUT/base-src"

sources() { # the _strata extension's source list, as setup.py and core_sources.txt name it
    "$PYTHON" - "$1" <<'EOF'
import ast, pathlib, sys
root = pathlib.Path(sys.argv[1])
tree = ast.parse((root / "setup.py").read_text())
for node in ast.walk(tree):
    if isinstance(node, ast.Assign) and any(getattr(t, "id", "") == "BINDING_SOURCES" for t in node.targets):
        for elt in node.value.elts:
            print(elt.value)
for line in (root / "src/strata/core_sources.txt").read_text().splitlines():
    line = line.strip()
    if line and not line.startswith("#"):
        print(line)
EOF
}

status=0

diff <(sources "$OUT/base-src") <(sources "$TREE") >"$OUT/source-list.diff" &&
    echo "source list: identical ($(sources "$TREE" | wc -l | tr -d ' ') TUs)" ||
    { echo "source list: DIFFERS"; cat "$OUT/source-list.diff"; status=1; }

for arch in arm64 x86_64; do
    for src in $(sources "$TREE"); do
        for side in base tree; do
            root="$TREE"; [ "$side" = base ] && root="$OUT/base-src"
            clang++ -E -P -std=c++20 -DNDEBUG -D_LIBCPP_DISABLE_AVAILABILITY -arch "$arch" \
                -I"$root/include" -I"$PYINC" "$root/$src" 2>/dev/null |
                tr -s ' \t\n' '\n' >"$OUT/$side.$arch.$(basename "$src").tok"
        done
        if cmp -s "$OUT/base.$arch.$(basename "$src").tok" "$OUT/tree.$arch.$(basename "$src").tok"; then
            echo "$arch $src: identical ($(wc -l <"$OUT/tree.$arch.$(basename "$src").tok" | tr -d ' ') tokens)"
        else
            echo "$arch $src: DIFFERS"
            status=1
        fi
    done
done
exit $status

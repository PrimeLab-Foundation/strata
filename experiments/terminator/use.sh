#!/usr/bin/env bash
# Put one header revision in place: base | v1 | v2.
#
# All three revisions live under experiments/terminator/, so a swap is a copy
# and never depends on the git state of the working tree.
set -euo pipefail
WHICH="$1"
SRC="experiments/terminator/${WHICH}"
[ "$WHICH" = base ] && SRC="experiments/terminator/baseline"
if [ ! -d "$SRC" ]; then
    echo "usage: use.sh base|v1|v2" >&2
    exit 2
fi
cp "$SRC/fast_parse.hpp" include/strata/util/fast_parse.hpp
cp "$SRC/json_parser_inline.hpp" include/strata/json/json_parser_inline.hpp
echo "headers now: $WHICH"
echo "  table definitions in fast_parse.hpp:  $(grep -c kNumberByteClass include/strata/util/fast_parse.hpp || true)"
echo "  table uses in json_parser_inline.hpp: $(grep -c 'util::detail::number_class\|util::detail::ends_number' include/strata/json/json_parser_inline.hpp || true)"

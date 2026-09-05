#!/usr/bin/env bash
# One measurement block for the terminator-classification A/B.
#
#   ab_measure.sh <label>
#
# Runs against whatever extension is currently installed. Instruction counts
# are the primary reading (they are noise-free where wall time on a shared
# runner is not); the per-element table and the wide_arrays counters are the
# end-to-end context.
set -uo pipefail
LABEL="$1"
PERF=$(ls /usr/lib/linux-tools/*/perf 2>/dev/null | head -1 || echo perf)

echo "##################### MEASURE ${LABEL}"
.venv/bin/python -c "import strata, strata._strata as s; print('strata', strata.__version__, s.__file__)"

echo "----- ${LABEL}: per-element parse floor (strata vs orjson)"
PYTHONPATH=. .venv/bin/python benchmarks/parse_elements_probe.py

echo "----- ${LABEL}: perf stat per number class (300 rounds of a 2000-element list)"
for cls in none nulls int1 int3 int4 int7 int10 f6dp f17; do
    for lib in strata orjson; do
        [ "$cls" = none ] && [ "$lib" = orjson ] && continue
        target="$lib"
        [ "$cls" = none ] && target=none
        PYTHONPATH=. $PERF stat -e instructions,cycles,branches,branch-misses \
            -o "ps_${LABEL}_${cls}_${lib}.txt" -- \
            .venv/bin/python experiments/terminator/numclass_loop.py "$target" "$cls" 300 \
            >/dev/null 2>&1
        printf '== %s %s %s\n' "$LABEL" "$cls" "$lib"
        grep -E "instructions|cycles|branches|branch-misses|elapsed" "ps_${LABEL}_${cls}_${lib}.txt" \
            | sed 's/^/    /'
    done
done

echo "----- ${LABEL}: perf stat, loads wide_arrays x400"
for lib in strata orjson; do
    PYTHONPATH=. $PERF stat -e task-clock,cycles,instructions,branches,branch-misses,cache-misses \
        -o "ps_${LABEL}_wide_${lib}.txt" -- \
        .venv/bin/python experiments/terminator/loads_loop.py "$lib" 400 >/dev/null 2>&1
    printf '== %s wide_arrays %s\n' "$LABEL" "$lib"
    cat "ps_${LABEL}_wide_${lib}.txt" | sed 's/^/    /'
done

echo "----- ${LABEL}: decompose loads mixed"
PYTHONPATH=. .venv/bin/python benchmarks/decompose_loads_mixed.py

echo "##################### END ${LABEL}"

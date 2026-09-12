"""Contract tests for the nested-container probe.

The probe exists because the Windows `dumps mixed` deficit turned out to be
the fixed cost of *opening* a container rather than per-key work
(docs/performance/experiment-ledger.md, E26-P24). What it measures is only
meaningful if its documents differ in exactly one value, and if its
fused-against-general pass really does force the second reading through the
general writer, so both are pinned here rather than trusted.
"""

import pytest

pytest.importorskip("orjson", reason="the probe compares against orjson")

import strata  # noqa: E402
from benchmarks.nested_container_probe import (  # noqa: E402
    RECORDS,
    documents,
    on_a_fresh_thread,
    retire_nested_depth,
)


def test_every_document_differs_from_the_baseline_by_one_value():
    docs = documents()
    base = docs["scalars-only"]
    assert len(base) == RECORDS
    baseline_keys = set(base[0])
    for name, doc in docs.items():
        assert len(doc) == RECORDS, name
        if name == "scalars-only":
            continue
        extra = set(doc[0]) - baseline_keys
        assert extra == {"value"}, name
        # The scalar keys are the baseline's, record for record: only the one
        # added value may differ between two documents of this set.
        stripped = [{k: v for k, v in record.items() if k != "value"} for record in doc]
        assert stripped == base, name


def test_the_named_shapes_are_the_shapes_the_names_claim():
    docs = documents()
    assert docs["value-dict0"][0]["value"] == {}
    assert docs["value-list0"][0]["value"] == []
    for width in (1, 2, 4, 8, 16):
        assert len(docs[f"value-dict{width}"][0]["value"]) == width
    for length in (1, 2, 5, 16, 64):
        assert len(docs[f"value-list{length}"][0]["value"]) == length
    kinds = {type(record["value"]).__name__ for record in docs["value-rotating"]}
    assert kinds == {"dict", "list", "str", "float"}


def test_retiring_the_nested_depth_changes_no_output_only_the_writer():
    """The ablation may only move which writer runs, never the bytes."""

    def run() -> tuple[str, str]:
        doc = documents()["value-dict2"]
        before = strata.dumps(doc)
        retire_nested_depth()
        return before, strata.dumps(doc)

    before, after = on_a_fresh_thread(run)
    assert before == after


def test_the_probe_runs_on_a_thread_of_its_own():
    import threading

    seen = on_a_fresh_thread(threading.current_thread)
    assert seen is not threading.current_thread()

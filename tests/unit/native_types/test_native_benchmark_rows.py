"""Row declaration for the native-v1 benchmark scope (benchmarks/harness.py).

Pins docs/decisions.md, 2026-09-29, "benchmarks": native-v1 is declared beside
`harness.WORKLOADS`, never inside the canonical 27-row / 135-row workload.
"""

from benchmarks import harness


def test_native_v1_is_registered_beside_the_canonical_workload():
    assert "native-v1" in harness.WORKLOADS
    assert harness.resolve_workload("native-v1") == harness.native_workload_rows()


def test_native_v1_has_six_data_rows_and_three_flag_rows():
    data_rows = harness.native_data_rows()
    flag_rows = harness.native_flag_rows()
    assert len(data_rows) == 6
    assert len(flag_rows) == 3
    assert harness.native_workload_rows() == data_rows + flag_rows


def test_native_data_rows_cover_dumps_and_dump_per_tier():
    rows = harness.native_data_rows()
    for tier in harness.NATIVE_TIERS:
        assert ("dumps", f"native.{tier}") in rows
        assert ("dump", f"native.{tier}") in rows


def test_native_flag_rows_cover_the_canonical_mixed_dataset_per_tier():
    rows = harness.native_flag_rows()
    for tier in harness.NATIVE_TIERS:
        assert ("dumps", f"mixed.{tier} (native flag)") in rows


def test_native_v1_rows_are_disjoint_from_the_canonical_ci_workload():
    ci_rows = set(harness.WORKLOADS["ci"])
    native_rows = set(harness.WORKLOADS["native-v1"])
    assert ci_rows.isdisjoint(native_rows)


def test_canonical_workload_is_unchanged_by_the_native_v1_addition():
    # The declared 27-row canonical workload (and therefore the 135-row CI
    # denominator across CI_PLATFORMS) must be exactly what it was before
    # native-v1 existed.
    assert harness.workload_rows() == harness.WORKLOADS["ci"]
    assert len(harness.WORKLOADS["ci"]) == 27
    assert len(harness.WORKLOADS["ci"]) * len(harness.CI_PLATFORMS) == 135


def test_resolve_workload_native_v1_round_trips():
    assert harness.resolve_workload("native-v1") == harness.native_workload_rows()

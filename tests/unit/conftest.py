"""Shared fixtures for the contract suites."""

import pytest

import strata

CONFIG_KEYS = ("duplicate_key_policy", "cycle_policy")


@pytest.fixture(autouse=True)
def restore_config():
    """Put every setting back after each test.

    `strata.config` is process state; a test that changes it and leaves it
    changed silently rewrites the contract for whatever runs next
    (docs/context/styleguide.md).
    """
    saved = {key: strata.config.get(key) for key in CONFIG_KEYS}
    yield
    for key, value in saved.items():
        strata.config.set(key, value)


# ---------------------------------------------------------------------------
# Benchmark-report fixtures
#
# The evidence gates (`supportability_check`, `ci_summary`, `regression_check`,
# `ci_fetch`) all judge a report against the *declared* workload, so their
# tests need a complete one to start from and a way to damage it precisely.
# One builder, so a change to the manifest cannot leave four copies behind.
# ---------------------------------------------------------------------------

LINUX_PLATFORM = "Linux-6.8.0-1014-azure-x86_64-with-glibc2.39"
MACOS_PLATFORM = "macOS-26.3-arm64-arm-64bit-Mach-O"
WINDOWS_PLATFORM = "Windows-2022Server-10.0.20348-SP0"

HEAD_SHA = "16b0a58fe1ed0da3d139b64f59d66cea9822f4a3"


@pytest.fixture
def complete_report():
    """A factory for a report containing every row of the declared workload.

    `drop` removes rows entirely, `drop_strata` leaves the row with only its
    rival, and `rows` overrides the workload for a deliberately scoped report.
    """
    from benchmarks.harness import Measurement, Report, workload_rows

    def build(
        *,
        name="ci-probe",
        platform=LINUX_PLATFORM,
        machine="x86_64",
        commit=HEAD_SHA[:7],
        strata_ms=1.0,
        rival_ms=1.2,
        rival="orjson",
        rows=None,
        drop=(),
        drop_strata=(),
        extra=(),
    ) -> Report:
        report = Report(
            name=name,
            environment={
                "commit": commit,
                "python": "3.12.6",
                "implementation": "CPython",
                "platform": platform,
                "machine": machine,
                "repeats": "10",
                "warmup": "2",
            },
        )
        for section, dataset in workload_rows() if rows is None else rows:
            if (section, dataset) in tuple(drop):
                continue
            if (section, dataset) not in tuple(drop_strata):
                report.measurements.append(
                    Measurement(
                        section=section,
                        dataset=dataset,
                        library="strata",
                        min_ms=strata_ms,
                        median_ms=strata_ms,
                        p95_ms=strata_ms,
                        rss_mb=10.0,
                    )
                )
            report.measurements.append(
                Measurement(
                    section=section,
                    dataset=dataset,
                    library=rival,
                    min_ms=rival_ms,
                    median_ms=rival_ms,
                    p95_ms=rival_ms,
                    rss_mb=10.0,
                )
            )
        report.measurements.extend(extra)
        return report

    return build

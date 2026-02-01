"""
Stress tests: large payloads (100MB+).

- Medium run (~5MB): no marker, runs in normal CI.
- 100MB run: @pytest.mark.stress, for scheduled CI or explicit runs.
"""

import json
import os

import pytest

import strata


def _make_large_payload_target_mb(mb: float):
    """Build a Python structure that serializes to approximately mb MB of JSON."""
    target_bytes = int(mb * 1024 * 1024)
    # Each object is ~100 bytes; we need target_bytes / 100 objects.
    chunk = "x" * 80  # ~80 chars per value + key overhead ~20 -> ~100 bytes per item
    obj = {"k": chunk}
    one_len = len(json.dumps(obj))
    n = max(1, target_bytes // one_len)
    return [{"id": i, "data": chunk} for i in range(n)]


class TestStressLarge:
    """Large-payload parse/serialize stress tests."""

    def test_large_parse_roundtrip_5mb(self):
        """~5MB: parse and round-trip with strata (no stress marker)."""
        data = _make_large_payload_target_mb(5.0)
        json_str = json.dumps(data)
        assert len(json_str) >= 4 * 1024 * 1024  # at least 4MB
        parsed = strata.loads(json_str)
        back = strata.dumps(parsed)
        reparsed = json.loads(back)
        assert len(reparsed) == len(data)
        assert reparsed[0]["id"] == 0 and reparsed[0]["data"] == data[0]["data"]

    @pytest.mark.stress
    def test_stress_large_100mb_parse_roundtrip(self):
        """~100MB: parse and round-trip (stress)."""
        if os.environ.get("RUN_STRESS_100MB", "").lower() in ("0", "false", "no"):
            pytest.skip("RUN_STRESS_100MB not set; skip 100MB stress")
        data = _make_large_payload_target_mb(100.0)
        json_str = json.dumps(data)
        assert len(json_str) >= 95 * 1024 * 1024  # at least 95MB
        parsed = strata.loads(json_str)
        back = strata.dumps(parsed)
        reparsed = json.loads(back)
        assert len(reparsed) == len(data)
        assert reparsed[0]["id"] == 0

    @pytest.mark.stress
    def test_stress_large_100mb_dumps_roundtrip(self):
        """~100MB: build in Python, dumps then loads (stress)."""
        if os.environ.get("RUN_STRESS_100MB", "").lower() in ("0", "false", "no"):
            pytest.skip("RUN_STRESS_100MB not set; skip 100MB stress")
        data = _make_large_payload_target_mb(100.0)
        out = strata.dumps(data)
        assert len(out) >= 95 * 1024 * 1024
        parsed = strata.loads(out)
        assert len(parsed) == len(data)
        assert parsed[0]["id"] == 0

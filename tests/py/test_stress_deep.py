"""
Stress tests: deep nesting (5k+ levels).

- Deep nesting can hit recursion limits or stack overflow if not iterative.
"""

import json

import pytest

import strata


def _make_deep_list(depth: int):
    """Build a list nested `depth` levels: [ [ [ ... ] ] ]."""
    current = []
    for _ in range(depth):
        current = [current]
    return current


def _make_deep_dict(depth: int):
    """Build a dict nested `depth` levels: {"a": {"a": {"a": ...}}}."""
    current = {"leaf": 1}
    for _ in range(depth - 1):
        current = {"a": current}
    return current


class TestStressDeep:
    """Deep-nesting parse/serialize stress tests."""

    def test_deep_list_1000(self):
        """1000-level nested list: dumps/loads round-trip."""
        depth = 1000
        data = _make_deep_list(depth)
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        current = parsed
        for _ in range(depth):
            assert isinstance(current, list)
            assert len(current) == 1
            current = current[0]
        assert current == []

    @pytest.mark.stress
    def test_stress_deep_list_5k(self):
        """5000-level nested list (stress)."""
        depth = 5000
        data = _make_deep_list(depth)
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        current = parsed
        for _ in range(depth):
            assert isinstance(current, list)
            assert len(current) == 1
            current = current[0]
        assert current == []

    def test_deep_dict_1000(self):
        """1000-level nested dict: dumps/loads round-trip."""
        depth = 1000
        data = _make_deep_dict(depth)
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        current = parsed
        for _ in range(depth - 1):
            assert isinstance(current, dict)
            assert "a" in current
            current = current["a"]
        assert current == {"leaf": 1}

    @pytest.mark.stress
    def test_stress_deep_dict_5k(self):
        """5000-level nested dict (stress)."""
        depth = 5000
        data = _make_deep_dict(depth)
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        current = parsed
        for _ in range(depth - 1):
            assert isinstance(current, dict)
            assert "a" in current
            current = current["a"]
        assert current == {"leaf": 1}

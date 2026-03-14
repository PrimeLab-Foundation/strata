"""
Test strata.config API.
"""

import pytest

import strata


class TestConfigSetGet:
    """Test config.set() and config.get()."""

    def test_get_default_mem_eff(self):
        val = strata.config.get("mem_eff")
        assert val is False

    def test_get_default_duplicate_key_policy(self):
        val = strata.config.get("duplicate_key_policy")
        assert val == "first"

    def test_get_default_cycle_policy(self):
        val = strata.config.get("cycle_policy")
        assert val == "warn"

    def test_set_and_get_mem_eff(self):
        original = strata.config.get("mem_eff")
        try:
            strata.config.set("mem_eff", True)
            assert strata.config.get("mem_eff") is True
            strata.config.set("mem_eff", False)
            assert strata.config.get("mem_eff") is False
        finally:
            strata.config.set("mem_eff", original)

    def test_set_and_get_duplicate_key_policy(self):
        original = strata.config.get("duplicate_key_policy")
        try:
            for policy in ("first", "last", "error", "warn"):
                strata.config.set("duplicate_key_policy", policy)
                assert strata.config.get("duplicate_key_policy") == policy
        finally:
            strata.config.set("duplicate_key_policy", original)

    def test_set_and_get_cycle_policy(self):
        original = strata.config.get("cycle_policy")
        try:
            for policy in ("warn", "error", "ignore"):
                strata.config.set("cycle_policy", policy)
                assert strata.config.get("cycle_policy") == policy
        finally:
            strata.config.set("cycle_policy", original)

    def test_set_invalid_key(self):
        with pytest.raises(KeyError):
            strata.config.set("nonexistent_key", "value")

    def test_set_invalid_mem_eff_type(self):
        with pytest.raises(TypeError):
            strata.config.set("mem_eff", "true")

    def test_set_invalid_duplicate_key_policy(self):
        with pytest.raises(ValueError):
            strata.config.set("duplicate_key_policy", "invalid")

    def test_set_invalid_cycle_policy(self):
        with pytest.raises(ValueError):
            strata.config.set("cycle_policy", "invalid")

    def test_get_unknown_key_returns_none(self):
        # Unknown keys should return None (not raise)
        val = strata.config.get("nonexistent")
        assert val is None


class TestConfigList:
    """Test config.list()."""

    def test_list_returns_dict(self):
        result = strata.config.list()
        assert isinstance(result, dict)

    def test_list_contains_defaults(self):
        result = strata.config.list()
        assert "mem_eff" in result
        assert "duplicate_key_policy" in result
        assert "cycle_policy" in result

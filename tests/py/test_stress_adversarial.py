"""
Stress tests: adversarial inputs.

- Very long strings, many escapes, unicode edges, large numbers, etc.
"""

import json

import strata


class TestStressAdversarial:
    """Adversarial input stress tests."""

    def test_very_long_string_1mb(self):
        """1MB single string value: parse and round-trip."""
        s = "a" * (1024 * 1024)
        data = {"key": s}
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        assert len(parsed["key"]) == 1024 * 1024
        assert parsed["key"] == s

    def test_string_with_many_escapes(self):
        """String full of escape sequences."""
        raw = "\\" * 5000 + '"' + "\\n" * 5000
        data = {"x": raw}
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        assert parsed["x"] == raw

    def test_unicode_surrogate_pairs(self):
        """String with surrogate pairs (U+10000 range)."""
        s = "\U0001f600\U0001f601\U0001f602"
        data = {"emoji": s}
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        assert parsed["emoji"] == s

    def test_large_integer(self):
        """Large integer beyond 2^53."""
        big = 9007199254740993  # 2^53 + 1
        data = {"n": big}
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        assert parsed["n"] == big

    def test_negative_large_integer(self):
        """Large negative integer."""
        big = -9007199254740993
        data = {"n": big}
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        assert parsed["n"] == big

    def test_many_keys_flat(self):
        """Object with many keys (10k)."""
        data = {f"k_{i}": i for i in range(10000)}
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        assert len(parsed) == 10000
        assert parsed["k_0"] == 0
        assert parsed["k_9999"] == 9999

    def test_deep_and_wide(self):
        """Moderately deep and wide structure (small enough to run quickly)."""

        # Depth 8, 3 keys per level -> 3^8 = 6561 nodes
        def build(depth):
            if depth == 0:
                return 1
            return {f"k{i}": build(depth - 1) for i in range(3)}

        data = build(8)
        json_str = strata.dumps(data)
        parsed = json.loads(json_str)
        current = parsed
        for _ in range(8):
            assert isinstance(current, dict)
            current = current["k0"]
        assert current == 1

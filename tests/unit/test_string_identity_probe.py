"""Value-preserving controls must change sharing without changing the input."""

import json

from benchmarks.string_identity import pool_strings, string_leaves


def test_pooling_changes_only_value_sharing():
    first = bytearray(b"repeated-value").decode()
    second = bytearray(b"repeated-value").decode()
    assert first == second and first is not second
    value = {"head": [first, {"tail": second}], "unicode": "é", "number": 1.25}
    before = json.dumps(value, ensure_ascii=False)
    pooled = pool_strings(value, {})
    assert json.dumps(pooled, ensure_ascii=False) == before
    assert pooled["head"][0] is pooled["head"][1]["tail"]
    assert value["head"][0] is first
    assert value["head"][1]["tail"] is second
    assert pooled is not value and pooled["head"] is not value["head"]
    assert list(string_leaves(value)) == [first, second, "é"]

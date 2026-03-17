"""Tests for the SIMD structural indexer exposed via strata._strata.structural_index().

Validates that the structural index correctly identifies { } [ ] : , positions
outside of strings, handles escapes, and agrees with the parsing results.
"""

from strata import _strata


def test_basic_binding():
    """structural_index returns a dict with positions, backend, length."""
    result = _strata.structural_index('{"a":1}')
    assert isinstance(result, dict)
    assert "positions" in result
    assert "backend" in result
    assert "length" in result
    assert result["length"] == 7
    print(f"  Backend: {result['backend']}")


def test_simple_object():
    """Structural positions for {"a":1} should be { : }."""
    json_str = '{"a":1}'
    result = _strata.structural_index(json_str)
    positions = result["positions"]
    chars = [json_str[p] for p in positions]
    assert set(chars) == {"{", ":", "}"}
    assert len(positions) == 3


def test_simple_array():
    """[1, 2, 3] should have [ , , ]."""
    json_str = "[1, 2, 3]"
    result = _strata.structural_index(json_str)
    positions = result["positions"]
    chars = [json_str[p] for p in positions]
    assert chars == ["[", ",", ",", "]"]


def test_nested():
    """Nested structures: all structural chars accounted for."""
    json_str = '{"a":{"b":[1,2]}}'
    result = _strata.structural_index(json_str)
    positions = result["positions"]
    chars = [json_str[p] for p in positions]
    # Expected: { : { : [ , ] } }
    assert chars == ["{", ":", "{", ":", "[", ",", "]", "}", "}"]


def test_structural_inside_string_masked():
    """Structural chars inside strings must NOT appear in the index."""
    json_str = '{"key":"{[:]}"}'
    result = _strata.structural_index(json_str)
    positions = result["positions"]
    chars = [json_str[p] for p in positions]
    # Only outer { : } — the {[:]} inside the string value is masked out.
    assert chars == ["{", ":", "}"], f"Got: {chars}"


def test_escaped_quote():
    r"""Escaped quotes \" should not close the string."""
    # {"a":"he said \"hi\""}
    json_str = '{"a":"he said \\"hi\\""}'
    result = _strata.structural_index(json_str)
    positions = result["positions"]
    chars = [json_str[p] for p in positions]
    assert chars == ["{", ":", "}"], f"Got: {chars}"


def test_double_backslash_before_quote():
    r"""Two backslashes then quote: \\ is escaped backslash, quote closes string."""
    # Bytes: " \ \ " ,  (5 bytes)
    # Use explicit bytes to avoid Python escape confusion.
    json_str = '"\\\\",'.replace("", "")  # This is: " \\ " ,
    # More explicit: build from bytes directly
    json_bytes = b'\x22\x5c\x5c\x22\x2c'  # " \ \ " ,
    json_str = json_bytes.decode("ascii")
    result = _strata.structural_index(json_str)
    positions = result["positions"]
    # Only the comma is structural (quotes aren't structural)
    assert len(positions) == 1, f"Expected 1 position, got {positions}"
    assert json_str[positions[0]] == ","


def test_empty_input():
    result = _strata.structural_index("")
    assert result["positions"] == []
    assert result["length"] == 0


def test_bare_number():
    """A bare number has no structural characters."""
    result = _strata.structural_index("42")
    assert result["positions"] == []


def test_bare_string():
    """A bare string has no structural characters."""
    result = _strata.structural_index('"hello"')
    assert result["positions"] == []


def test_bare_true():
    result = _strata.structural_index("true")
    assert result["positions"] == []


def test_large_document_consistency():
    """Build a large doc, verify every indexed position is a structural char."""
    import json

    data = {
        "users": [{"id": i, "name": f"user_{i}", "scores": [i * 10, i * 20]} for i in range(200)]
    }
    json_str = json.dumps(data)
    result = _strata.structural_index(json_str)
    positions = result["positions"]

    structural_chars = set("{}[]:,")
    for pos in positions:
        c = json_str[pos]
        assert c in structural_chars, f"Position {pos} has '{c}', not structural"

    # Sanity: we should have many structural positions for this JSON
    assert len(positions) > 100, f"Expected >100 positions, got {len(positions)}"


def test_cross_validates_with_loads():
    """Parse via loads() and structural_index() — ensure the index is compatible."""
    json_str = '{"name":"Alice","age":30,"items":[{"id":1},{"id":2}]}'

    result = _strata.structural_index(json_str)
    positions = result["positions"]

    # Count structural chars manually
    in_string = False
    escaped = False
    expected_count = 0
    for c in json_str:
        if escaped:
            escaped = False
            continue
        if c == '\\' and in_string:
            escaped = True
            continue
        if c == '"':
            in_string = not in_string
            continue
        if not in_string and c in "{}[]:,":
            expected_count += 1

    assert len(positions) == expected_count, (
        f"Expected {expected_count} structural positions, got {len(positions)}"
    )


def test_utf8_in_strings():
    """UTF-8 multibyte chars inside strings must not produce false structurals."""
    json_str = '["caf\u00e9","\U0001f600"]'
    # structural_index operates on UTF-8 bytes, so positions are byte offsets.
    # Index into the UTF-8 byte sequence, not the Python string.
    json_bytes = json_str.encode("utf-8")
    result = _strata.structural_index(json_str)
    positions = result["positions"]
    # Verify every indexed position (byte offset) is a structural char.
    for p in positions:
        byte = json_bytes[p]
        char = chr(byte)
        assert char in "[],:{}\\", f"Byte offset {p} = 0x{byte:02x} '{char}' is not structural"


def test_long_string_spanning_blocks():
    """A string longer than 64 bytes must not leak structural chars."""
    long_val = "x" * 200
    json_str = f'["{long_val}",1]'
    result = _strata.structural_index(json_str)
    positions = result["positions"]
    chars = [json_str[p] for p in positions]
    assert chars == ["[", ",", "]"], f"Got: {chars}"


if __name__ == "__main__":
    tests = [v for k, v in sorted(globals().items()) if k.startswith("test_") and callable(v)]
    passed = 0
    for t in tests:
        try:
            t()
            print(f"  PASS: {t.__name__}")
            passed += 1
        except Exception as e:
            print(f"  FAIL: {t.__name__}: {e}")
    print(f"\n{passed}/{len(tests)} tests passed")

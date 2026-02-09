"""
Test Unicode handling and escape sequences.

Ensures UTF-8 support and proper escape parsing.
"""

import pytest

import strata
from strata.json_cursor import parse_json


class TestEscapeSequences:
    """Test JSON escape sequence handling."""

    def test_backslash_escape(self):
        root = parse_json(r'"back\\slash"')
        assert root.get_str() == "back\\slash"

    def test_quote_escape(self):
        root = parse_json(r'"quote\"here"')
        assert root.get_str() == 'quote"here'

    def test_forward_slash_escape(self):
        root = parse_json(r'"forward\/slash"')
        assert root.get_str() == "forward/slash"

    def test_backspace_escape(self):
        root = parse_json(r'"back\bspace"')
        assert root.get_str() == "back\bspace"

    def test_formfeed_escape(self):
        root = parse_json(r'"form\ffeed"')
        assert root.get_str() == "form\ffeed"

    def test_newline_escape(self):
        root = parse_json(r'"new\nline"')
        assert root.get_str() == "new\nline"

    def test_carriage_return_escape(self):
        root = parse_json(r'"carriage\rreturn"')
        assert root.get_str() == "carriage\rreturn"

    def test_tab_escape(self):
        root = parse_json(r'"tab\there"')
        assert root.get_str() == "tab\there"

    def test_multiple_escapes(self):
        root = parse_json(r'"line1\nline2\ttab\r\nline3"')
        assert root.get_str() == "line1\nline2\ttab\r\nline3"

    def test_escaped_quote_in_middle(self):
        root = parse_json(r'"He said \"hello\""')
        assert root.get_str() == 'He said "hello"'

    def test_backslash_at_end(self):
        # Escaped backslash followed by closing quote is valid
        root = parse_json(r'"ends with \\"')
        assert root.get_str() == "ends with \\"

    def test_all_valid_escapes_together(self):
        json_str = r'"\"\\\/ \b\f\n\r\t"'
        root = parse_json(json_str)
        # Expected: quote, backslash, forward-slash, space, backspace, form-feed, newline, return, tab
        result = root.get_str()
        # Verify key components are present
        assert result.startswith('"\\/')
        assert '\n' in result
        assert '\r' in result
        assert '\t' in result


class TestUnicodeSupport:
    """Test UTF-8 and Unicode handling."""

    def test_ascii_text(self):
        root = parse_json('"Hello, World!"')
        assert root.get_str() == "Hello, World!"

    def test_utf8_latin_extended(self):
        root = parse_json('"café"')
        assert root.get_str() == "café"

    def test_utf8_cyrillic(self):
        root = parse_json('"Привет"')
        assert root.get_str() == "Привет"

    def test_utf8_chinese(self):
        root = parse_json('"你好"')
        assert root.get_str() == "你好"

    def test_utf8_japanese(self):
        root = parse_json('"こんにちは"')
        assert root.get_str() == "こんにちは"

    def test_utf8_arabic(self):
        root = parse_json('"مرحبا"')
        assert root.get_str() == "مرحبا"

    def test_utf8_emoji(self):
        root = parse_json('"Hello 👋 World 🌍"')
        assert root.get_str() == "Hello 👋 World 🌍"

    def test_utf8_mixed_scripts(self):
        root = parse_json('"English français 中文 العربية"')
        assert root.get_str() == "English français 中文 العربية"

    def test_utf8_in_object_keys(self):
        root = parse_json('{"café": "coffee", "価格": 100}')
        assert root.field("café").get_str() == "coffee"
        assert root.field("価格").get_int() == 100

    def test_utf8_in_nested_structure(self):
        json_str = '{"user": {"name": "José", "city": "São Paulo"}}'
        root = parse_json(json_str)
        assert root.field("user").field("name").get_str() == "José"
        assert root.field("user").field("city").get_str() == "São Paulo"

    def test_zero_width_characters(self):
        # Zero-width space (U+200B)
        root = parse_json('"a\u200bb"')
        result = root.get_str()
        assert len(result) == 3  # a + zero-width space + b
        assert result[0] == "a"
        assert result[2] == "b"

    def test_surrogate_pairs(self):
        # Test characters that require surrogate pairs in UTF-16
        # but are single codepoints in UTF-8 (e.g., emoji)
        root = parse_json('"𝕳𝖊𝖑𝖑𝖔"')  # Mathematical bold text
        assert "𝕳" in root.get_str()

    def test_bom_in_string(self):
        # Byte order mark inside string (valid UTF-8)
        root = parse_json('"\ufeff text"')
        assert root.get_str().startswith("\ufeff")


class TestComplexEscapesAndUnicode:
    """Test combinations of escapes and Unicode."""

    def test_escaped_unicode_characters(self):
        root = parse_json(r'"\u0041"')
        assert root.get_str() == "A"

        root = parse_json(r'"\u03A9"')
        assert root.get_str() == "Ω"

        root = parse_json(r'"\uD83D\uDC4B"')
        assert root.get_str() == "👋"

        with pytest.raises(ValueError):
            parse_json(r'"\uD800"')

        with pytest.raises(ValueError):
            parse_json(r'"\uDC00"')

    def test_mixed_escapes_and_unicode(self):
        json_str = '"Line 1\\n日本語\\tTab"'
        root = parse_json(json_str)
        assert "\n" in root.get_str()
        assert "日本語" in root.get_str()
        assert "\t" in root.get_str()

    def test_long_string_with_unicode(self):
        # Test a longer string with various Unicode characters
        text = "The quick brown fox jumps over the lazy dog. " * 10
        text += " 中文 العربية русский 日本語 한국어 हिन्दी ไทย עברית"
        json_str = f'"{text}"'
        root = parse_json(json_str)
        assert root.get_str() == text

    def test_special_whitespace_characters(self):
        # Various Unicode whitespace characters
        root = parse_json('"a b"')  # Regular space
        assert root.get_str() == "a b"

        # Non-breaking space (U+00A0)
        root = parse_json('"a\u00a0b"')
        assert len(root.get_str()) == 3


class TestStringBoundaries:
    """Test string parsing at various boundaries."""

    def test_empty_string(self):
        root = parse_json('""')
        assert root.get_str() == ""

    def test_single_character(self):
        root = parse_json('"a"')
        assert root.get_str() == "a"

    def test_very_long_string(self):
        # Test a very long string
        long_str = "a" * 10000
        json_str = f'"{long_str}"'
        root = parse_json(json_str)
        assert root.get_str() == long_str
        assert len(root.get_str()) == 10000

    def test_string_with_many_escapes(self):
        # String that's mostly escapes
        json_str = r'"\n\n\n\n\t\t\t\t\r\r\r\r"'
        root = parse_json(json_str)
        assert root.get_str() == "\n\n\n\n\t\t\t\t\r\r\r\r"

    def test_string_all_printable_ascii(self):
        # All printable ASCII characters (except quote and backslash which need escaping)
        printable = ''.join(chr(i) for i in range(32, 127) if chr(i) not in ['"', '\\'])
        json_str = f'"{printable}"'
        root = parse_json(json_str)
        assert root.get_str() == printable


class TestInvalidUtf8Rejected:
    """Invalid UTF-8 in input is rejected (contract with C++ SIMD validation)."""

    def test_lone_continuation_byte(self):
        with pytest.raises(ValueError):
            strata.loads(b'"a\x80b"')

    def test_overlong_nul(self):
        with pytest.raises(ValueError):
            strata.loads(b'"\xc0\x80"')

    def test_invalid_lead_byte(self):
        with pytest.raises(ValueError):
            strata.loads(b'"\xff"')

    def test_truncated_two_byte(self):
        with pytest.raises(ValueError):
            strata.loads(b'"\xc2"')

    def test_overlong_three_byte(self):
        with pytest.raises(ValueError):
            strata.loads(b'"\xe0\x80\x80"')

    def test_surrogate_codepoint(self):
        with pytest.raises(ValueError):
            strata.loads(b'"\xed\xa0\x80"')

    def test_codepoint_above_10ffff(self):
        with pytest.raises(ValueError):
            strata.loads(b'"\xf4\x90\x80\x80"')

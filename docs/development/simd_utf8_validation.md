# SIMD UTF-8 validation in parser (P2-11)

## Overview

The parser validates the entire input as UTF-8 **once upfront** before parsing. Invalid UTF-8 is rejected with `ParseError`. Validation uses SIMD when available (AVX2, SSE4.2, NEON) and falls back to scalar otherwise.

## Implementation

- **API**: `strata::util::validate_utf8_simd(const char* data, size_t len)` in `include/strata/util/simd_string.hpp`, implemented in `src/strata/util/simd_utf8.cpp` (SIMD utilities are split into `simd_escape.cpp`, `simd_newline.cpp`, `simd_utf8.cpp`; see `include/strata/util/simd_string.hpp` for the public API)..
- **Parser**: `parse_json(std::string_view text)` calls `validate_utf8_simd(text.data(), text.size())` at the start; on `false` returns `{Status::ParseError, JsonValue{}}`.
- **Rules**: RFC 3629. Rejects overlong encodings (e.g. `C0 80` for NUL), surrogate codepoints (U+D800–U+DFFF), codepoints above U+10FFFF, lone continuation bytes, and truncated sequences.

## Byte classification and state machine

- Bytes are classified: ASCII (0x00–0x7F), continuation (0x80–0xBF), 2-byte lead (0xC2–0xDF), 3-byte lead (0xE0–0xEF), 4-byte lead (0xF0–0xF4), invalid (else).
- A small state machine tracks expected continuation bytes; second-byte rules for E0/ED/F0/F4 (overlong/surrogate/range) are applied when processing the first continuation after a 3- or 4-byte lead.

## SIMD vs scalar

- **AVX2**: 32 bytes per iteration; classification loop + state machine over 32 bytes; tail handled in scalar.
- **SSE4.2 / NEON**: 16-byte chunks with the same state machine; scalar fallback when no SIMD.
- **Scalar**: Used when no SIMD is defined; same classification and state machine.

## Tests

- **C++**: `tests/cpp/test_json_parse.cpp` — `test_parse_utf8_validation()` (valid UTF-8 parses; invalid cases return `ParseError`).
- **Python**: `tests/py/test_unicode_and_escapes.py`, `tests/unit/test_unicode_and_escapes.py` — `TestInvalidUtf8Rejected` (invalid UTF-8 bytes raise `ValueError` via `loads()`).

## Performance

- One pass over the buffer before parsing. On ASCII-heavy data overhead is small (median parse time within run-to-run variance). On UTF-8-heavy data SIMD classification can improve throughput vs scalar-only validation. See `docs/benchmarks/progress_log.md` for P2-11 entry.

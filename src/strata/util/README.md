# Utility Module

Low-level utilities and SIMD-accelerated functions for Strata.

## Files

### SIMD Operations
- **`simd_escape.cpp`** - SIMD-accelerated JSON string escaping
- **`simd_utf8.cpp`** - SIMD UTF-8 validation
- **`simd_newline.cpp`** - SIMD newline detection for NDJSON
- **`simd_structural.cpp`** - SIMD structural character detection
- **`simd_numbers.cpp`** - SIMD-assisted number parsing

### Number Parsing/Formatting
- **`fast_parse.cpp`** - SWAR-accelerated integer/float parsing
- **`dragonbox.cpp`** - Dragonbox float-to-string conversion
- **`ryu_dtoa.cpp`** - Ryu algorithm for float formatting (fallback)

### Threading
- **`thread_pool.cpp`** - Thread pool for parallel NDJSON

## SIMD Platform Support

All SIMD files support multiple platforms with automatic detection:

| Platform | Instruction Set | Register Width |
|----------|-----------------|----------------|
| x86_64 (Intel/AMD) | AVX2 | 256-bit |
| x86_64 (older) | SSE4.2 | 128-bit |
| ARM64 (Apple Silicon, etc.) | NEON | 128-bit |
| Other | Scalar fallback | N/A |

## Key Optimizations

### String Escaping (simd_escape.cpp)
- Process 16-32 bytes per iteration
- Vector comparison for escape candidates
- ~3-5x faster than byte-by-byte

### UTF-8 Validation (simd_utf8.cpp)
- Lookup table approach for continuation validation
- State machine for multi-byte sequences
- ~10x faster than byte-by-byte

### Newline Detection (simd_newline.cpp)
- String-aware scanning (skip embedded newlines)
- Bitmask extraction for position finding
- ~5x faster than byte-by-byte

### Number Parsing (fast_parse.cpp)
- SWAR (SIMD Within A Register) technique
- Process 4-8 digit bytes in parallel
- ~2-3x faster than strtol/strtod

### Float Formatting (dragonbox.cpp)
- Shortest round-trip representation
- Integer detection for clean output
- ~2x faster than sprintf

## Headers

Corresponding headers in `include/strata/util/`:
- `simd_string.hpp` - SIMD string function declarations
- `fast_parse.hpp` - Fast parsing API
- `dragonbox.hpp` - Float formatting API
- `ryu_dtoa.hpp` - Ryu algorithm API

## See Also

- `docs/development/simd_utf8_validation.md` - UTF-8 validation design

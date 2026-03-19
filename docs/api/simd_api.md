# SIMD Subsystem API Reference

The SIMD subsystem provides branchless character classification and structural indexing for JSON documents. It processes input in 64-byte chunks, classifying every byte into structural, whitespace, quote, and backslash categories using platform-specific SIMD instructions.

**Headers:**

- `strata/simd/classifier.h`
- `strata/simd/structural_indexer.h`
- `strata/simd/index_builder.h`
- `strata/simd/dispatch.h`

**Namespace:** `strata::simd`

______________________________________________________________________

## ClassMask

Aggregate bitmask constants for extracting specific character classes from classification results.

```cpp
struct ClassMask {
    static constexpr uint8_t kStructural = 0x61; // bits 0, 5, 6
    static constexpr uint8_t kQuote      = 0x02; // bit 1
    static constexpr uint8_t kBackslash  = 0x04; // bit 2
    static constexpr uint8_t kWhitespace = 0x88; // bits 3, 7
};
```

Use these constants with bitwise AND on classification output to isolate a specific character class.

______________________________________________________________________

## Classifier

Branchless SIMD character classifier using the vpshufb nibble-lookup trick. Splits each input byte into high and low nibbles, performs two independent shuffles against pre-computed 16-byte lookup tables, and ANDs the results. Only the correct (high, low) combination produces a non-zero classification -- zero false positives for all four classes.

### Methods

#### classify64

```cpp
[[nodiscard]] __m512i classify64(__m512i input) const noexcept;
```

Classify 64 bytes at once using AVX-512BW. Uses a single 512-bit `vpshufb` across four 128-bit lanes with the same nibble-AND trick. Pair with `_mm512_test_epi8_mask` for direct 64-bit class bitmasks.

- **Availability:** Only when `STRATA_SIMD_HAS_AVX512` is defined.
- **Parameters:** `input` -- 512-bit vector of 64 input bytes.
- **Returns:** 512-bit vector where each byte contains the classification bitmask for the corresponding input byte.
- **Complexity:** O(1). Single SIMD instruction sequence.

#### classify32

```cpp
[[nodiscard]] __m256i classify32(__m256i input) const noexcept;
```

Classify 32 bytes at once using AVX2 `vpshufb` across two 128-bit lanes.

- **Availability:** Only when `STRATA_SIMD_HAS_AVX2` is defined.
- **Parameters:** `input` -- 256-bit vector of 32 input bytes.
- **Returns:** 256-bit vector of per-byte classification bitmasks.
- **Complexity:** O(1).

#### classify_sve2

```cpp
[[nodiscard]] svuint8_t classify_sve2(svuint8_t input, svbool_t pg) const noexcept;
```

Classify a scalable vector of bytes using ARM SVE2 `svtbl`.

- **Availability:** Only when `STRATA_SIMD_HAS_SVE2` is defined.
- **Parameters:**
  - `input` -- Scalable vector of input bytes.
  - `pg` -- Governing predicate; only active lanes are classified.
- **Returns:** Scalable vector of per-byte classification bitmasks.

#### classify16

```cpp
[[nodiscard]] uint8x16_t classify16(uint8x16_t input) const noexcept;
```

Classify 16 bytes at once using ARM NEON `vqtbl1q_u8`.

- **Availability:** Only when `STRATA_SIMD_HAS_NEON` is defined.
- **Parameters:** `input` -- 128-bit NEON vector of 16 input bytes.
- **Returns:** 128-bit vector of per-byte classification bitmasks.

#### classify16_wasm

```cpp
[[nodiscard]] v128_t classify16_wasm(v128_t input) const noexcept;
```

Classify 16 bytes at once using WebAssembly SIMD128 `i8x16.swizzle`.

- **Availability:** Only when `STRATA_SIMD_HAS_WASM_SIMD` is defined.

#### classify_rvv

```cpp
[[nodiscard]] vuint8m1_t classify_rvv(vuint8m1_t input, size_t vl) const noexcept;
```

Classify a vector of bytes using RISC-V Vector Extension `vluxei8` gather.

- **Availability:** Only when `STRATA_SIMD_HAS_RVV` is defined.
- **Parameters:**
  - `input` -- RVV vector of input bytes.
  - `vl` -- Current vector length from `vsetvl`.

#### classify_scalar

```cpp
[[nodiscard]] static uint8_t classify_scalar(uint8_t c) noexcept;
```

Scalar classification of a single byte. Uses the same encoding as the SIMD versions.

- **Parameters:** `c` -- Single input byte.
- **Returns:** Classification bitmask (same bit layout as SIMD output).
- **Complexity:** O(1). Always available on all platforms.

______________________________________________________________________

## ChunkResult

Result of classifying a single 64-byte chunk. All fields are 64-bit bitmasks where bit `i` corresponds to byte position `i` within the chunk.

```cpp
struct ChunkResult {
    uint64_t structural;  // positions of { } [ ] : , (outside strings)
    uint64_t whitespace;  // positions of whitespace characters
    uint64_t string_mask; // positions inside JSON strings
    uint64_t quote_mask;  // positions of unescaped " characters
};
```

______________________________________________________________________

## StructuralIndexer

Processes 64-byte chunks of JSON input and classifies every byte. Maintains inter-block state (`prev_escaped_`, `prev_in_string_`) for correct streaming across chunk boundaries. The `classify()` method is fully branchless in the inner loop.

Key algorithms used internally:

- **vpshufb nibble-lookup** for character classification (via `Classifier`)
- **Carry-less multiplication** (PCLMUL) for prefix-XOR to convert quote positions into string-region masks
- **Add-with-carry trick** for detecting characters following odd-length backslash sequences

### Constructor

```cpp
StructuralIndexer() noexcept;
```

Initializes inter-block state to zero (not inside a string, no pending escape).

### Methods

#### classify

```cpp
[[nodiscard]] ChunkResult classify(const uint8_t* input) noexcept;
```

Process exactly 64 bytes of input.

- **Parameters:** `input` -- Pointer to 64 bytes of JSON input. For the final chunk of a document shorter than a 64-byte multiple, the caller must zero-pad the remainder.
- **Returns:** `ChunkResult` with bitmasks for structural, whitespace, string, and quote positions within this 64-byte window.
- **Complexity:** O(1) per 64-byte chunk. Constant number of SIMD instructions.
- **Note:** Structural characters inside string regions are automatically masked out.

#### reset

```cpp
void reset() noexcept;
```

Reset inter-block state for a new document. Must be called before processing a different document on the same indexer instance.

______________________________________________________________________

## IndexBuilder

Drives `StructuralIndexer` over a complete document and produces a flat index of structural character positions. Uses BMI2 `PEXT` when available for dense bit-to-index conversion; falls back to a `ctz` (count-trailing-zeros) loop otherwise.

### StructuralIndex

```cpp
struct StructuralIndex {
    std::vector<uint32_t> positions; // byte offsets of structural chars (outside strings)
    size_t document_length;          // original document length in bytes
};
```

### Methods

#### build

```cpp
[[nodiscard]] StructuralIndex build(const uint8_t* data, size_t length);
```

Build the structural index for a document.

- **Parameters:**
  - `data` -- Pointer to the raw JSON document bytes. Does NOT need to be 64-byte aligned.
  - `length` -- Length of the document in bytes.
- **Returns:** `StructuralIndex` with all structural character positions.
- **Complexity:** O(n) where n is document length. Processes 64 bytes per iteration.
- **Allocation:** Pre-allocates the output vector with estimated capacity of `length / 6` (heuristic: approximately 1 structural char per 6 bytes for typical JSON).

______________________________________________________________________

## Backend

Enumeration of available SIMD backends, ordered roughly by throughput on target hardware.

```cpp
enum class Backend {
    SCALAR,     // No SIMD -- portable C++ fallback
    AVX2,       // x86 AVX2 (vpshufb, PCLMUL, ctz-loop extraction)
    AVX2_BMI2,  // x86 AVX2 + BMI2 (adds PEXT for fast bit extraction)
    AVX512,     // x86 AVX-512BW (64-byte classify, native kmask)
    NEON,       // ARM NEON (vqtbl1q_u8, PMULL prefix-XOR)
    SVE2,       // ARM SVE2 (svtbl, scalable 128-2048-bit vectors)
    WASM_SIMD,  // WebAssembly SIMD128 (i8x16.swizzle, native bitmask)
    RVV,        // RISC-V Vector Extension 1.0 (vluxei8 gather)
};
```

______________________________________________________________________

## Free Functions

### detect_backend

```cpp
[[nodiscard]] Backend detect_backend() noexcept;
```

Detect the best available backend at runtime.

- On x86-64: uses `cpuid` to check AVX2, BMI2, PCLMUL support.
- On ARM: returns `NEON` if `__ARM_NEON` is defined at compile time.
- Otherwise: returns `SCALAR`.
- The result is computed once and cached in a function-local static.
- **Returns:** The `Backend` enum value for the best available instruction set.

### backend_name

```cpp
[[nodiscard]] const char* backend_name(Backend b) noexcept;
```

Return the name of a backend as a null-terminated string (for diagnostics/logging).

### index_document

```cpp
[[nodiscard]] IndexBuilder::StructuralIndex index_document(const uint8_t* data, size_t len);
```

Build a structural index for the document using the compile-time-selected backend. This is the primary entry point for indexing -- it creates an `IndexBuilder` internally and calls `build()`.

- **Parameters:**
  - `data` -- Pointer to the raw JSON document bytes.
  - `len` -- Length of the document in bytes.
- **Returns:** `StructuralIndex` containing positions and document length.

______________________________________________________________________

## Usage Example

```cpp
#include <strata/simd/dispatch.h>
#include <cstdio>

int main() {
    const char* json = R"({"name": "Alice", "age": 30, "scores": [95, 87, 92]})";
    auto len = std::strlen(json);

    // Detect and print the active SIMD backend
    auto backend = strata::simd::detect_backend();
    std::printf("Using backend: %s\n", strata::simd::backend_name(backend));

    // Build the structural index
    auto index = strata::simd::index_document(
        reinterpret_cast<const uint8_t*>(json), len);

    // Print structural character positions
    std::printf("Document length: %zu\n", index.document_length);
    std::printf("Structural positions (%zu):\n", index.positions.size());
    for (auto pos : index.positions) {
        std::printf("  offset %u: '%c'\n", pos, json[pos]);
    }
}
```

______________________________________________________________________

## Compile-Time Configuration

Define `STRATA_FORCE_SCALAR=1` (via `-DSTRATA_FORCE_SCALAR=1` or environment variable `STRATA_SCALAR_ONLY=1` at build time) to disable ALL SIMD backends and compile only the portable scalar fallback.

Multiple SIMD features may be defined simultaneously (e.g., AVX-512 + AVX2). The structural indexer selects the best path at compile time via a priority chain.

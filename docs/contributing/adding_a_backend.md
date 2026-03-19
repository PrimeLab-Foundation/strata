# Adding a New SIMD Backend

This guide walks through adding support for a new SIMD instruction set to the
structural indexer. Strata currently supports AVX-512, AVX2, SVE2, NEON,
WebAssembly SIMD128, RISC-V Vector (RVV), and a scalar fallback.

______________________________________________________________________

## Architecture Overview

The SIMD pipeline processes JSON documents in 64-byte chunks:

```
  raw bytes -> classifier (SIMD) -> 64-bit bitmask -> extract_positions (CTZ)
```

Adding a backend requires implementing the classification step for your ISA.
The escape detection, prefix-XOR, and bit extraction stages operate on 64-bit
integers and are ISA-independent.

**Key source files:**

| File                                     | Role                               |
| ---------------------------------------- | ---------------------------------- |
| `include/strata/simd/classifier.h`       | Lookup tables, classify methods    |
| `src/strata/simd/structural_indexer.cpp` | classify() dispatch, escape/string |
| `src/strata/simd/index_builder.cpp`      | CTZ extraction loop                |
| `include/strata/simd/dispatch.h`         | Backend enum, detect_backend()     |
| `src/strata/simd/dispatch.cpp`           | Runtime detection, backend_name()  |

______________________________________________________________________

## Step-by-Step Checklist

### Step 1: Add platform detection in classifier.h

Open `include/strata/simd/classifier.h` and add your ISA to the detection
chain. The chain is ordered by platform (x86, ARM, WASM, RISC-V), and within
each platform, by capability level.

```cpp
// In the platform detection section (lines 47-76):

#elif defined(__YOUR_ARCH__) || defined(__YOUR_ALT_MACRO__)
// --- YourArch: YourISA ------------------------------------------------
#if defined(__YOUR_ISA_FEATURE__)
#define STRATA_SIMD_HAS_YOUR_ISA 1
#include <your_isa_intrinsics.h>
#endif
```

Also add the same detection block to `src/strata/simd/structural_indexer.cpp`
(lines 15-44), which must mirror the classifier.h detection exactly.

Add a classify method to the `Classifier` class:

```cpp
#ifdef STRATA_SIMD_HAS_YOUR_ISA
    /// Classify N bytes at once (YourISA).
    [[nodiscard]] your_vec_t classifyN(your_vec_t input) const noexcept;
#endif
```

### Step 2: Implement the classify method

Create `src/strata/simd/classifier.cpp` (or add to the existing file) with
your classification implementation. The algorithm is always the same:

1. Extract low nibbles: `low = input & 0x0F`
1. Extract high nibbles: `high = (input >> 4) & 0x0F`
1. Shuffle low nibble table by low nibbles
1. Shuffle high nibble table by high nibbles
1. AND the results

The lookup tables (`kLowNibbleTable` and `kHighNibbleTable`) are ISA-
independent -- the same 16-byte tables work everywhere. You just need to use
your ISA's shuffle/table-lookup instruction.

**ISA equivalents of vpshufb:**

| ISA       | Instruction           | Vector Width |
| --------- | --------------------- | ------------ |
| AVX2      | `_mm256_shuffle_epi8` | 32 bytes     |
| AVX-512   | `_mm512_shuffle_epi8` | 64 bytes     |
| NEON      | `vqtbl1q_u8`          | 16 bytes     |
| SVE2      | `svtbl`               | scalable     |
| WASM SIMD | `i8x16.swizzle`       | 16 bytes     |
| RVV       | `vluxei8`             | scalable     |

### Step 3: Add to structural_indexer.cpp priority chain

Add your backend to the `#if / #elif` chain in `structural_indexer.cpp`
inside the `classify()` function (around line 180). The chain is ordered by
performance:

```
  AVX-512 > AVX2 > SVE2 > NEON > WASM SIMD > RVV > YourISA > Scalar
```

Insert your backend at the appropriate position:

```cpp
#elif defined(STRATA_SIMD_HAS_YOUR_ISA)

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    Classifier cls;

    // Load and classify 64 bytes (adjust for your vector width).
    // If your vector width < 64, use multiple loads like the NEON path.
    // ...

    // Extract 64-bit class bitmasks.
    // Use your ISA's equivalent of movemask or bitmask extraction.
    uint64_t raw_structural = /* extract structural bits */;
    uint64_t raw_whitespace = /* extract whitespace bits */;
    uint64_t raw_quotes     = /* extract quote bits */;
    uint64_t raw_backslash  = /* extract backslash bits */;

    // These three steps are ISA-independent (operate on uint64_t):
    uint64_t escaped = compute_escape_mask(raw_backslash);
    uint64_t unescaped_quotes = raw_quotes & ~escaped;
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);
    uint64_t structural = raw_structural & ~string_mask;

    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}
```

The bitmask extraction is often the tricky part. Different ISAs provide
different mechanisms:

| ISA       | Bitmask Extraction                           |
| --------- | -------------------------------------------- |
| AVX2      | `_mm256_movemask_epi8` (32-bit per 256 bits) |
| AVX-512   | `_mm512_test_epi8_mask` (native 64-bit mask) |
| NEON      | Emulated via `vpaddlq` cascade (see code)    |
| WASM SIMD | `wasm_i8x16_bitmask` (native 16-bit mask)    |
| SVE2      | Store to buffer + reload as NEON (hybrid)    |
| RVV       | `vmsne` + `vsm` (mask store to bytes)        |

### Step 4: Register in dispatch.h and dispatch.cpp

Add your backend to the `Backend` enum in `include/strata/simd/dispatch.h`:

```cpp
enum class Backend {
    SCALAR,
    AVX2,
    AVX2_BMI2,
    AVX512,
    NEON,
    SVE2,
    WASM_SIMD,
    RVV,
    YOUR_ISA,    // <-- add here
};
```

Add detection logic to `src/strata/simd/dispatch.cpp`:

```cpp
#elif defined(__YOUR_ARCH__)
static Backend detect_backend_impl() noexcept {
#if defined(__YOUR_ISA_FEATURE__)
    return Backend::YOUR_ISA;
#else
    return Backend::SCALAR;
#endif
}
```

Add the string name in `backend_name()`:

```cpp
case Backend::YOUR_ISA:
    return "your-isa";
```

### Step 5: Test with STRATA_FORCE_SCALAR

Verify that the scalar fallback still works when your backend is disabled:

```bash
# Compile with scalar-only mode
CXX=clang++ CXXFLAGS="-DSTRATA_FORCE_SCALAR=1" make test-cpp
```

This ensures that:

- The priority chain correctly skips your backend when forced scalar.
- No compile errors from missing intrinsics headers in scalar mode.
- All tests still pass with the scalar fallback.

Then test with your backend enabled (the default on your target hardware):

```bash
make test-cpp
```

______________________________________________________________________

## Pre-Submission Checklist

- [ ] Platform detection added to `classifier.h` (detection block)
- [ ] Same detection block added to `structural_indexer.cpp` (must match)
- [ ] Classify method added to `Classifier` class in `classifier.h`
- [ ] Classify method implemented in `classifier.cpp`
- [ ] `classify()` added to priority chain in `structural_indexer.cpp`
- [ ] `Backend` enum extended in `dispatch.h`
- [ ] `detect_backend_impl()` updated in `dispatch.cpp`
- [ ] `backend_name()` updated in `dispatch.cpp`
- [ ] All existing tests pass with new backend (`make test-cpp`)
- [ ] All tests pass with `STRATA_FORCE_SCALAR=1`
- [ ] clang-format passes (`make fmt`)
- [ ] New code has >= 20% comment density
- [ ] Header comment explains the ISA and any quirks
- [ ] Performance measured: new backend should beat scalar by >= 2x

______________________________________________________________________

## Tips

### Vector width mismatch

If your ISA has a vector width smaller than 64 bytes (e.g., 16 bytes for
NEON, variable for SVE/RVV), you need multiple loads per 64-byte chunk. See
the NEON path (4 loads of 16 bytes each) or the SVE2 path (`svwhilelt` loop)
for patterns.

### Bitmask extraction

The hardest part of a new backend is usually extracting per-byte classification
results into a 64-bit bitmask. Options:

1. **Native bitmask** (AVX-512 `test_epi8_mask`, WASM `bitmask`): ideal,
   one instruction.
1. **Movemask emulation** (NEON `vpaddlq` cascade): works but costs ~8
   instructions per 16-byte vector.
1. **Store-and-reload** (SVE2 hybrid): classify with scalable vectors, store
   to a 64-byte buffer, reload as fixed-width vectors for bitmask extraction.
1. **Mask store** (RVV `vsm`): store mask bits packed into bytes, then load
   as a `uint64_t`.

### Prefix-XOR

If your ISA has a carry-less multiply instruction (analogous to x86 PCLMUL
or ARM PMULL), you can add a fast path in `quotes_to_string_mask()` for
O(1) prefix-XOR. Otherwise, the 6-iteration scalar doubling fallback works
on all platforms at about 12 cycles -- fast enough for most workloads.

### Build flags

Remember to add the necessary compiler flags to both `scripts/run_cpp_tests.sh`
and `CMakeLists.txt`. For example, AVX2 requires `-mavx2 -mpclmul`, ARM NEON
is baseline on AArch64, and WASM SIMD requires `-msimd128`.

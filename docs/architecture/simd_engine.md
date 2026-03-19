# SIMD Structural Indexing Engine

The SIMD engine transforms raw JSON bytes into a flat array of byte offsets pointing to every structural character (`{`, `}`, `[`, `]`, `:`, `,`) that appears outside of string literals. This structural index is the foundation that both the generic parser and speculative parser consume: instead of scanning byte-by-byte for the next token, they index directly into the position array. The engine processes input in fixed 64-byte chunks using branchless SIMD operations, maintaining minimal inter-block state for correct streaming across chunk boundaries.

## 64-Byte Chunk Processing Pipeline

```
  Input: 64 raw JSON bytes (two 32-byte AVX2 loads, or one 64-byte AVX-512 load)
                              |
                              v
  +----------------------------------------------------------+
  |  Stage 1: CLASSIFY (Classifier::classify32/classify64)   |
  |                                                          |
  |  For each byte, split into high nibble and low nibble.   |
  |  Two vpshufb lookups against 16-byte tables.             |
  |  AND the results -> classification byte per input byte.  |
  |                                                          |
  |  Output: 64 classification bytes, each with bits set     |
  |  for the character's class (STRUCTURAL, QUOTE,           |
  |  BACKSLASH, WHITESPACE).                                 |
  +----------------------------------------------------------+
                              |
                              v
  +----------------------------------------------------------+
  |  Stage 2: EXTRACT BITMASKS                               |
  |                                                          |
  |  _mm256_movemask_epi8 (or _mm512_test_epi8_mask) to     |
  |  convert each class to a 64-bit bitmask:                 |
  |                                                          |
  |    structural_mask:  bits set at { } [ ] : , positions   |
  |    quote_mask:       bits set at " positions              |
  |    backslash_mask:   bits set at \ positions              |
  |    whitespace_mask:  bits set at whitespace positions     |
  +----------------------------------------------------------+
                              |
                              v
  +----------------------------------------------------------+
  |  Stage 3: ESCAPE DETECTION (add-with-carry)              |
  |                                                          |
  |  Identify positions that follow an odd-length backslash  |
  |  sequence. For each contiguous run of backslashes:       |
  |                                                          |
  |    starts = backslash_mask & ~(backslash_mask << 1)      |
  |    Add starts to backslash_mask with carry from prev     |
  |    chunk. XOR result with backslash_mask to find exits.  |
  |    Filter by even/odd parity to get escaped positions.   |
  |                                                          |
  |  Output: escaped_mask (positions following odd \ runs)   |
  +----------------------------------------------------------+
                              |
                              v
  +----------------------------------------------------------+
  |  Stage 4: STRING MASKING (PCLMUL prefix-XOR)             |
  |                                                          |
  |  unescaped_quotes = quote_mask & ~escaped_mask           |
  |                                                          |
  |  Carry-less multiply unescaped_quotes with 0xFFFF...F    |
  |  using _mm_clmulepi64_si128 (PCLMUL). This computes a   |
  |  prefix-XOR: each quote toggles the in-string state,     |
  |  producing a mask where bit i is 1 if position i is      |
  |  inside a JSON string.                                   |
  |                                                          |
  |  XOR with prev_in_string_ carry from previous chunk.     |
  |                                                          |
  |  Output: string_mask (1 = inside string, 0 = outside)    |
  +----------------------------------------------------------+
                              |
                              v
  +----------------------------------------------------------+
  |  Stage 5: FILTER STRUCTURALS                             |
  |                                                          |
  |  final_structural = structural_mask & ~string_mask       |
  |                                                          |
  |  Removes any structural characters that fall inside      |
  |  string literals. The result contains only true JSON     |
  |  structural positions.                                   |
  +----------------------------------------------------------+
                              |
                              v
  +----------------------------------------------------------+
  |  Stage 6: BIT EXTRACTION (IndexBuilder)                  |
  |                                                          |
  |  Convert 64-bit final_structural mask to an array of     |
  |  uint32_t byte offsets:                                   |
  |                                                          |
  |  BMI2 path (PEXT):                                       |
  |    _pext_u64 extracts set-bit indices in parallel        |
  |                                                          |
  |  Fallback path (CTZ loop):                               |
  |    while (mask) {                                        |
  |      pos = __builtin_ctzll(mask);                        |
  |      out.push_back(base_offset + pos);                   |
  |      mask &= mask - 1;  // clear lowest set bit          |
  |    }                                                     |
  |                                                          |
  |  Output: appended to flat uint32_t position array        |
  +----------------------------------------------------------+
```

## The vpshufb Nibble-Lookup Classifier

The Classifier uses a technique that classifies all bytes in a SIMD register with zero false positives and zero branches. Each input byte is split into its high nibble (bits 7-4) and low nibble (bits 3-0). Two 16-entry lookup tables are loaded into SIMD registers:

- **Low-nibble table**: For each low nibble value 0-F, stores a bitmask of which character classes have a member with that low nibble.
- **High-nibble table**: Same idea for high nibbles.

The `vpshufb` instruction performs a parallel table lookup -- each byte in the input selects one entry from the 16-byte table based on its value. By ANDing the low-nibble lookup result with the high-nibble lookup result, only bytes where both nibbles agree on a class produce a non-zero result.

For example, the quote character `"` (0x22) has high nibble 2 and low nibble 2. The high-nibble table entry for index 2 has bit 1 set (the QUOTE bit). The low-nibble table entry for index 2 also has bit 1 set. The AND produces 0x02, correctly identifying the character. Meanwhile, byte 0x32 (digit `2`) has high nibble 3 whose table entry does not have bit 1 set, so the AND produces 0x00 -- correctly rejected.

The bit layout assigns disjoint bits to each class:

- Bit 0 (0x01): colon `:`
- Bit 1 (0x02): quote `"`
- Bit 2 (0x04): backslash `\`
- Bits 3,7 (0x88): whitespace (tab, LF, CR, space)
- Bit 5 (0x20): brackets and braces `[ ] { }`
- Bit 6 (0x40): comma `,`

Aggregate masks combine the relevant bits: `STRUCTURAL = 0x61` (bits 0, 5, 6), `WHITESPACE = 0x88` (bits 3, 7).

## Escape Detection: Add-With-Carry

Escaped quotes (like `\"`) must not toggle the in-string state. The challenge is that backslashes can themselves be escaped (`\\"`), so the parity of the backslash run length matters.

The algorithm identifies the start of each contiguous backslash run (positions where the previous byte is not a backslash), then adds this "starts" bitmask to the full backslash mask with carry propagation. The addition propagates a carry through consecutive 1-bits (the backslash run). Where the carry exits determines if the run has odd or even length. The `prev_escaped_` field carries this state across 64-byte chunk boundaries.

This is entirely branchless -- the entire escape detection reduces to about 6 bitwise operations and one addition with carry.

## String Masking: PCLMUL Prefix-XOR

Once unescaped quotes are identified (quote positions minus escaped positions), they need to be converted into a region mask: bit i should be 1 if position i is between an opening quote and its closing quote.

This is equivalent to computing a prefix-XOR (running parity) over the quote bitmask. The PCLMUL instruction (`_mm_clmulepi64_si128`) with one operand set to all-ones computes exactly this: carry-less multiplication by 0xFFFFFFFFFFFFFFFF produces the cumulative XOR. The `prev_in_string_` carry tracks whether the previous chunk ended inside a string.

On platforms without PCLMUL (some ARM configurations), a scalar loop computes the same result by iterating through quote positions and XOR-flipping the state.

## Multi-Backend Dispatch

The `dispatch.h` module provides both compile-time and runtime backend selection:

| Backend   | Register Width          | Classifier            | String Mask             | Bit Extraction   |
| --------- | ----------------------- | --------------------- | ----------------------- | ---------------- |
| AVX-512BW | 512-bit (64 bytes)      | `_mm512_shuffle_epi8` | PCLMUL on 64-bit halves | PEXT or CTZ loop |
| AVX2+BMI2 | 256-bit (32 bytes)      | `_mm256_shuffle_epi8` | `_mm_clmulepi64_si128`  | BMI2 `_pext_u64` |
| AVX2      | 256-bit (32 bytes)      | `_mm256_shuffle_epi8` | `_mm_clmulepi64_si128`  | CTZ loop         |
| SVE2      | 128-2048 bit (scalable) | `svtbl`               | Scalar prefix-XOR       | Scalar CTZ loop  |
| NEON      | 128-bit (16 bytes)      | `vqtbl1q_u8`          | PMULL prefix-XOR        | Scalar CTZ loop  |
| WASM SIMD | 128-bit (16 bytes)      | `i8x16.swizzle`       | Scalar prefix-XOR       | Scalar CTZ loop  |
| RVV       | Scalable                | `vluxei8` gather      | Scalar prefix-XOR       | Scalar CTZ loop  |
| Scalar    | 1 byte                  | Lookup table per byte | Scalar prefix-XOR       | Direct append    |

Runtime detection on x86-64 uses `cpuid` to check for AVX2, BMI2, and PCLMUL support. The result is cached in a function-local static variable (thread-safe under C++11 guarantees). The `STRATA_FORCE_SCALAR` build flag disables all SIMD backends for testing and portability verification.

On AVX-512, the entire 64-byte chunk is classified in a single instruction (`_mm512_shuffle_epi8`), and bitmask extraction uses native kmask registers (`_mm512_test_epi8_mask`) producing 64-bit masks directly. On AVX2, two 32-byte halves are classified separately and the resulting 32-bit masks are combined into a 64-bit mask.

## Performance Characteristics

- **Throughput**: The structural indexer processes input at approximately one cycle per byte on modern x86 with AVX2, limited primarily by the PCLMUL latency (3-7 cycles depending on microarchitecture). AVX-512 improves throughput by ~1.5x through wider classification.
- **Memory access pattern**: Strictly sequential. The indexer reads 64 bytes at a time in order, writes to a growing position array. This is cache-friendly and prefetcher-optimal.
- **Inter-block state**: Only two 64-bit values (`prev_escaped_`, `prev_in_string_`) carry across chunk boundaries, making the indexer trivially streamable for large documents.
- **Output density**: For typical JSON, the structural index contains roughly 1 position per 6 bytes of input. The IndexBuilder pre-allocates `length / 6` capacity to avoid reallocation for most documents.
- **BMI2 advantage**: On CPUs with BMI2, PEXT converts a sparse bitmask into a dense index ~2-4x faster than the CTZ loop for high structural density. The difference is most visible on structurally dense inputs (small objects, many keys).

# SIMD Tricks in Strata

This document explains the four core SIMD techniques used by Strata's structural
indexer to classify 64 bytes of JSON per iteration with zero branches in the hot
path.

______________________________________________________________________

## 1. vpshufb Nibble-Lookup Classification

**Source:** `include/strata/simd/classifier.h:L191-L209`

The classifier must decide, for every byte in a 32-byte (AVX2) or 64-byte
(AVX-512) vector, whether it is a structural character (`{`, `}`, `[`, `]`,
`:`, `,`), a quote (`"`), a backslash (`\`), whitespace, or something else.
A naive approach would require a chain of comparisons per byte. The vpshufb
nibble-lookup trick does it in three instructions for 32 bytes.

### How it works

Every byte has a high nibble (bits 7-4) and a low nibble (bits 3-0). The
classifier builds two 16-entry lookup tables -- one indexed by the low nibble,
one indexed by the high nibble. Each table entry is a bitmask encoding which
character classes *could* match that nibble value. The AND of both lookups
produces a non-zero result only when both nibbles agree on a class.

### Bit assignment

Each character class gets a unique bit:

```
  bit 0  (0x01) -- colon (:)
  bit 1  (0x02) -- quote (")
  bit 2  (0x04) -- backslash (\)
  bit 3  (0x08) -- whitespace (tab, LF, CR)
  bit 5  (0x20) -- bracket/brace ([ ] { })
  bit 6  (0x40) -- comma (,)
  bit 7  (0x80) -- whitespace (space)
```

Aggregate masks used downstream:

```
  STRUCTURAL = 0x61  (bits 0 | 5 | 6)
  WHITESPACE = 0x88  (bits 3 | 7)
  QUOTE      = 0x02  (bit 1)
  BACKSLASH  = 0x04  (bit 2)
```

### Lookup table derivation

Consider the 12 JSON-significant characters and their hex encodings:

```
  Char   Hex   High  Low   Class           Assigned Bit
  ----   ----  ----  ---   --------------  ------------
  \t     0x09   0     9    WHITESPACE       3
  \n     0x0A   0     A    WHITESPACE       3
  \r     0x0D   0     D    WHITESPACE       3
  ' '    0x20   2     0    WHITESPACE       7
  "      0x22   2     2    QUOTE            1
  ,      0x2C   2     C    STRUCTURAL       6
  :      0x3A   3     A    STRUCTURAL       0
  [      0x5B   5     B    STRUCTURAL       5
  \      0x5C   5     C    BACKSLASH        2
  ]      0x5D   5     D    STRUCTURAL       5
  {      0x7B   7     B    STRUCTURAL       5
  }      0x7D   7     D    STRUCTURAL       5
```

For each class, set its bit in the low-nibble table at every low-nibble value
that participates in that class, and similarly for the high-nibble table.

**Low-nibble table** (`kLowNibbleTable[16]`):

```
  Index  Value   Reason
  -----  -----   ------
    0    0x80    space has low nibble 0 -> bit 7
    2    0x02    quote has low nibble 2 -> bit 1
    9    0x08    tab has low nibble 9 -> bit 3
    A    0x09    colon (bit 0) + LF (bit 3) share low nibble A
    B    0x20    brackets/braces share low nibble B -> bit 5
    C    0x44    backslash (bit 2) + comma (bit 6) share low nibble C
    D    0x28    CR (bit 3) + ]/} (bit 5) share low nibble D
    *    0x00    all other indices
```

**High-nibble table** (`kHighNibbleTable[16]`):

```
  Index  Value   Reason
  -----  -----   ------
    0    0x08    tab/LF/CR have high nibble 0 -> bit 3
    2    0xC2    quote (bit 1) + comma (bit 6) + space (bit 7) -> high nibble 2
    3    0x01    colon has high nibble 3 -> bit 0
    5    0x24    backslash (bit 2) + [/] (bit 5) have high nibble 5
    7    0x20    {/} have high nibble 7 -> bit 5
    *    0x00    all other indices
```

### Verification by example

Consider the comma character `,` = 0x2C:

```
  low  nibble = C  -> kLowNibbleTable[C]  = 0x44  (bits 2, 6)
  high nibble = 2  -> kHighNibbleTable[2] = 0xC2  (bits 1, 6, 7)
  AND result  = 0x44 & 0xC2 = 0x40          -> bit 6 set -> STRUCTURAL

  Test against STRUCTURAL mask (0x61):
  0x40 & 0x61 = 0x40 != 0  -> comma IS structural
```

Now consider the digit `5` = 0x35:

```
  low  nibble = 5  -> kLowNibbleTable[5]  = 0x00
  high nibble = 3  -> kHighNibbleTable[3] = 0x01
  AND result  = 0x00 & 0x01 = 0x00          -> no class matches
```

The AND eliminates all false positives. Every non-JSON byte produces zero.

### Instruction sequence (AVX2)

```
  1. vpand    low_nibbles  = input & 0x0F          // extract low nibbles
  2. vpsrlw   high_nibbles = input >> 4             // extract high nibbles
     vpand    high_nibbles = high_nibbles & 0x0F   // mask to 4 bits
  3. vpshufb  low_result   = shuffle(kLowTable,  low_nibbles)
  4. vpshufb  high_result  = shuffle(kHighTable, high_nibbles)
  5. vpand    classified   = low_result & high_result
```

Total: 5 instructions to classify 32 bytes (AVX2) or 64 bytes (AVX-512).

______________________________________________________________________

## 2. PCLMUL Prefix-XOR (Quote-to-String-Mask)

**Source:** `src/strata/simd/structural_indexer.cpp:L106-L137`

After classification, we have a 64-bit bitmask `q` where each set bit marks
an unescaped quote character. We need to convert this to a *string-region
mask* -- a bitmask where bits inside strings are set and bits outside are
clear. This is the classic prefix-XOR (cumulative XOR) operation.

### The mathematical insight

The prefix-XOR of a bitmask toggles state at every set bit. If bit `i` is a
quote, all bits from `i` to the next quote flip from "outside string" to
"inside string" (or vice versa).

Formally, for output bit `j`:

```
  result[j] = q[0] XOR q[1] XOR ... XOR q[j]
```

This is a running parity computation.

### PCLMUL with all-ones

Carry-less multiplication (CLMUL) of a polynomial `A(x)` by `B(x)` where
`B(x) = x^63 + x^62 + ... + x + 1` (the all-ones polynomial, represented
by 0xFFFFFFFFFFFFFFFF) produces exactly the prefix-XOR of `A`.

Why: carry-less multiplication is polynomial multiplication over GF(2). When
multiplying by the all-ones polynomial, each output bit `j` is the XOR of all
input bits at positions 0 through `j` -- the definition of prefix-XOR.

### Bit-level diagram

```
  Input quotes mask q:
    bit:  0  1  2  3  4  5  6  7  ...
    q  :  0  1  0  0  1  0  0  0  ...
          ^        ^
          open     close

  Prefix-XOR (PCLMUL with all-ones):
    bit:  0  1  2  3  4  5  6  7  ...
    out:  0  1  1  1  0  0  0  0  ...
             |--------|
             inside string
```

### Cross-chunk state

The `prev_in_string_` member carries the string-interior state across 64-byte
chunk boundaries. After computing the prefix-XOR:

```cpp
  result ^= prev_in_string_;
  prev_in_string_ = static_cast<uint64_t>(static_cast<int64_t>(result) >> 63);
```

The arithmetic right shift replicates the MSB (bit 63) across all 64 bits,
producing either 0 (not in a string) or ~0 (in a string) for the next chunk.

### Platform implementations

| Platform     | Instruction            | Latency |
| ------------ | ---------------------- | ------- |
| x86 (PCLMUL) | `_mm_clmulepi64_si128` | 3-7 cy  |
| ARM (PMULL)  | `vmull_p64`            | 3 cy    |
| Scalar       | 6 shift-XOR iterations | ~12 cy  |

The scalar fallback uses doubling:

```
  result  = q
  result ^= result << 1
  result ^= result << 2
  result ^= result << 4
  result ^= result << 8
  result ^= result << 16
  result ^= result << 32
```

Each shift doubles the "reach" of the XOR propagation: after all 6 steps,
every bit has been XORed with all bits to its right.

______________________________________________________________________

## 3. Add-with-Carry Escape Detection

**Source:** `src/strata/simd/structural_indexer.cpp:L68-L97`

A backslash in JSON only escapes the next character if it is part of an
*odd-length* run. For example, `\\\"` has two backslashes followed by a
quote -- the backslashes cancel each other, so the quote is real. But `\\\\\\"`
has three trailing backslashes; the first two cancel, the third escapes the
quote.

The `compute_escape_mask()` function identifies which characters immediately
follow an odd-length backslash run, using only bitwise arithmetic on the 64-bit
backslash bitmask -- no loops, no branches.

### Algorithm outline

Given a 64-bit mask `bs` where set bits are backslash positions:

1. **Find run starts**: a backslash at position `i` starts a new run if
   position `i-1` is not a backslash.

```
  start_edges = bs & ~(bs << 1)
```

2. **Separate even/odd starts**: partition start edges by parity of their
   position (even or odd bit index). This determines the alignment of the
   run relative to the bit position parity.

1. **Add backslashes to starts**: `even_carries = bs + even_starts`. The
   binary addition propagates a carry through consecutive set bits (the
   backslash run). The carry "exits" at the first zero bit after the run --
   that is the position following the last backslash.

```
  Backslash positions:  ...0 1 1 1 0...
  Start edge at bit 1:  ...0 1 0 0 0...
  bs + start          = ...1 0 0 0 0...
                              ^
                              carry exits here
```

4. **Mask off the backslashes**: `even_carry_ends = even_carries & ~bs`
   isolates just the carry-out positions.

1. **Check parity**: an odd-length run starting at an even position ends at
   an odd position (and vice versa). Filter the carry-out positions by the
   appropriate parity mask.

1. **Combine**: the union of even-start-odd-end and odd-start-even-end gives
   all positions that follow an odd-length backslash run.

### Handling overflow

When a backslash run extends to bit 63 (the last bit in the chunk), the
addition overflows. The code uses `__builtin_uaddll_overflow` to detect this
and propagates the carry to the next chunk via `prev_escaped_`.

### Worked example

```
  Input:   " h e l l o \ \ \ " w o r l d "
  Byte:    22 68 65 6C 6C 6F 5C 5C 5C 22 ...
  Index:    0  1  2  3  4  5  6  7  8  9

  bs (backslash mask): bit 6, 7, 8 set
    bs = 0b...001 1100 0000  (bits 6,7,8)

  start_edges = bs & ~(bs << 1)
    bs << 1    = 0b...011 1000 0000
    ~(bs << 1) = 0b...100 0111 1111
    start_edges= 0b...000 0100 0000  (bit 6 only -- run starts at 6)

  This start is at an even position (6).
  even_starts = 0b...000 0100 0000

  even_carries = bs + even_starts
    0b...001 1100 0000  (bs)
  + 0b...000 0100 0000  (even_starts)
  = 0b...010 0000 0000  (bit 9)

  even_carry_ends = even_carries & ~bs
    = 0b...010 0000 0000  (bit 9 is not a backslash -> kept)

  Run length = 3 (odd). Start at even pos 6, end carry at pos 9.
  9 is odd -> even_start_odd_end includes bit 9.

  Result: bit 9 is set -> the character at index 9 (the quote) IS escaped.
```

Wait -- three backslashes: `\`, `\`, `\`. The first two cancel, the third
escapes the quote at index 9. The algorithm correctly identifies index 9 as
following an odd-length run.

______________________________________________________________________

## 4. CTZ + Clear-Lowest-Bit Extraction

**Source:** `src/strata/simd/index_builder.cpp`

After classification, the structural bitmask is a 64-bit integer where each
set bit marks a structural character's position within the 64-byte chunk. We
need to convert this to an array of concrete byte offsets.

### The CTZ loop

```
  while (mask != 0) {
      pos = CTZ(mask);           // find lowest set bit
      output.push_back(base + pos);
      mask &= mask - 1;         // clear lowest set bit
  }
```

**CTZ** (Count Trailing Zeros) returns the index of the lowest set bit.
On x86, this compiles to `TZCNT` (BMI1) or `BSF`.

**Clear lowest set bit** uses the identity `mask & (mask - 1)`:

```
  mask       = 0b...01010100
  mask - 1   = 0b...01010011   (borrows through the lowest 1)
  mask & ... = 0b...01010000   (lowest set bit cleared)
```

On BMI1 CPUs, this compiles to a single `BLSR` (Reset Lowest Set Bit)
instruction -- one cycle.

### Bit diagram

```
  Structural mask for a 64-byte chunk:
    bit:  0  1  2  3  4  5  6  7  8  ...  63
    mask: 0  0  1  0  0  1  0  0  0  ...   0
              ^        ^
              CTZ=2    (after clear, CTZ=5)

  Iteration 1:  CTZ(mask) = 2   -> emit (base + 2)
                mask &= mask-1  -> clears bit 2

  Iteration 2:  CTZ(mask) = 5   -> emit (base + 5)
                mask &= mask-1  -> clears bit 5

  mask == 0 -> done.
```

### Performance

The loop runs exactly `popcount(mask)` iterations -- typically 4-10 per
64-byte chunk for well-formed JSON. Each iteration is two instructions
(`TZCNT` + `BLSR`) plus the store, giving roughly 1 cycle per extracted
position after frontend overhead.

The entire pipeline processes about 16 GB/s on a modern x86-64 core (AVX2),
limited by the SIMD classification throughput rather than the extraction loop.

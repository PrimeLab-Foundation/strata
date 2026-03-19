# SWAR Integer Parsing

SWAR (SIMD Within A Register) uses standard 64-bit integer arithmetic to
process 8 bytes in parallel without any SIMD instructions. Strata's
speculative integer fast path uses SWAR to parse up to 8 ASCII digits in a
single reduction, achieving roughly 1 cycle per digit on modern x86-64.

**Source:** `src/strata/speculative/fast_paths.cpp:L44-L119`

______________________________________________________________________

## 1. Overview

The SWAR integer parser works in three phases:

1. **Load and validate** -- load 8 bytes as a `uint64_t`, subtract `'0'`
   from each byte in parallel, detect non-digit bytes.
1. **Reduce** -- combine 8 single-digit values into one integer using
   three multiply-and-add stages.
1. **Accumulate** -- feed 8-digit chunks into the running total.

______________________________________________________________________

## 2. Load and Validate

### Parallel subtraction

Load 8 bytes from the input buffer into a `uint64_t` (little-endian):

```cpp
uint64_t word;
memcpy(&word, data + pos, 8);
```

Subtract `'0'` (0x30) from every byte simultaneously:

```cpp
const uint64_t sub = word - 0x3030303030303030ULL;
```

This works because subtraction in each byte lane does not borrow from
adjacent lanes *as long as* each byte is >= 0x30. If a byte is less than
0x30, the subtraction borrows from the next byte -- but that is fine because
we detect it in the next step.

### Non-digit detection

After subtraction, valid digit bytes have values 0-9. Non-digits have
values >= 10 (or wrapped around to >= 0x80 if \< '0'). The detection:

```cpp
const uint64_t check = (sub + 0x7676767676767676ULL) | sub;
const uint64_t non_digits = check & 0x8080808080808080ULL;
```

For a valid digit `d` (0 \<= d \<= 9):

- `d + 0x76 = 0x76..0x7F` -- bit 7 is never set.
- `d | (d + 0x76)` -- bit 7 still clear.

For a non-digit `d` (d >= 10, or d is a large wrapped value):

- `d + 0x76 >= 0x80` -- bit 7 is set.
- OR: `d` itself has bit 7 set (wrapped subtraction result).

The `non_digits` mask has bit 7 set in each byte that is NOT a valid digit.
If `non_digits == 0`, all 8 bytes are digits.

______________________________________________________________________

## 3. Three-Stage Reduction

When all 8 bytes are valid digits, we combine them into a single integer
using three multiply-and-add stages. On a little-endian machine, the first
digit (leftmost in the string) is in the lowest byte.

### Layout after subtraction

For the string `"12345678"`:

```
  Byte position in uint64_t (little-endian):
    byte 0 (LSB): '1' - '0' = 1
    byte 1:        '2' - '0' = 2
    byte 2:        '3' - '0' = 3
    byte 3:        '4' - '0' = 4
    byte 4:        '5' - '0' = 5
    byte 5:        '6' - '0' = 6
    byte 6:        '7' - '0' = 7
    byte 7 (MSB): '8' - '0' = 8

  sub = 0x08_07_06_05_04_03_02_01
```

### Stage 1: Pair adjacent digits (8 -> 4 values)

Combine each pair of adjacent bytes into a two-digit number:

```
  d[0]*10 + d[1],  d[2]*10 + d[3],  d[4]*10 + d[5],  d[6]*10 + d[7]
  = 12,             34,               56,               78
```

Implementation:

```cpp
uint64_t v_even = v & 0x00FF00FF00FF00FFULL;   // bytes 0, 2, 4, 6
uint64_t v_odd  = (v >> 8) & 0x00FF00FF00FF00FFULL; // bytes 1, 3, 5, 7
v = v_even * 10 + v_odd;
```

After this step, `v` contains four 16-bit values in 16-bit lanes:

```
  lane 0 (bits  0-15): 1*10 + 2 = 12
  lane 1 (bits 16-31): 3*10 + 4 = 34
  lane 2 (bits 32-47): 5*10 + 6 = 56
  lane 3 (bits 48-63): 7*10 + 8 = 78
```

### Stage 2: Pair adjacent pairs (4 -> 2 values)

Combine each pair of 2-digit numbers into a 4-digit number:

```
  12*100 + 34 = 1234,  56*100 + 78 = 5678
```

Implementation:

```cpp
uint64_t v_lo = v & 0x0000FFFF0000FFFFULL;     // lanes 0, 2
uint64_t v_hi = (v >> 16) & 0x0000FFFF0000FFFFULL; // lanes 1, 3
v = v_lo * 100 + v_hi;
```

After this step, `v` contains two 32-bit values:

```
  lower 32 bits: 12*100 + 34 = 1234
  upper 32 bits: 56*100 + 78 = 5678
```

### Stage 3: Final combination (2 -> 1 value)

```
  1234 * 10000 + 5678 = 12345678
```

Implementation:

```cpp
uint64_t v_lower = v & 0x00000000FFFFFFFFULL;
uint64_t v_upper = v >> 32;
uint64_t result = v_lower * 10000 + v_upper;
```

Result: `12345678` in a single `uint64_t`.

______________________________________________________________________

## 4. Complete Worked Example

Input bytes at current position: `"12345678"` (ASCII: 0x31 0x32 0x33 0x34
0x35 0x36 0x37 0x38).

```
  word (LE) = 0x3837363534333231

  Step 0: sub = word - 0x3030303030303030
        = 0x0807060504030201

  Step 0b: check non-digits
        sub + 0x7676767676767676 = 0x7E7D7C7B7A797877
        check = 0x7E7D7C7B7A797877 | 0x0807060504030201
              = 0x7E7F7E7D7E7B7A77
        non_digits = check & 0x8080808080808080 = 0x0000000000000000
        -> all digits!

  Step 1: Pair digits
        v_even = 0x0807060504030201 & 0x00FF00FF00FF00FF
               = 0x0007000500030001  (digits 8, 6, 4, 2 -- wait, LE order)

  Correcting for clarity (byte-by-byte):
        v = 0x08_07_06_05_04_03_02_01
        v_even (bytes 0,2,4,6) = 0x00_07_00_05_00_03_00_01
        v_odd  (bytes 1,3,5,7) = 0x00_08_00_06_00_04_00_02
        v = v_even * 10 + v_odd
          = 0x00_46_00_32_00_1E_00_0C

  In decimal per 16-bit lane:
        lane 0: 1*10+2  =  12  (0x000C)
        lane 1: 3*10+4  =  34  (0x0022)
        lane 2: 5*10+6  =  56  (0x0038)
        lane 3: 7*10+8  =  78  (0x004E)

  Step 2: Pair pairs
        v_lo = lanes 0,2 = 12, 56
        v_hi = lanes 1,3 = 34, 78
        v = v_lo * 100 + v_hi
        lower 32: 12*100 + 34 = 1234
        upper 32: 56*100 + 78 = 5678

  Step 3: Final
        result = 1234 * 10000 + 5678 = 12345678
```

Total: 3 multiplies, 3 adds, 4 masks, 2 shifts -- about 12 instructions
for 8 digits. On a superscalar CPU with sufficient execution ports, this
completes in approximately 5-6 cycles.

______________________________________________________________________

## 5. Partial Chunks and Accumulation

When the input has fewer than 8 leading digits, `parse_8_digits()` returns
the count of valid digits:

```cpp
int first_non_digit = __builtin_ctzll(non_digits) / 8;
```

The position of the first set bit (in the `non_digits` mask) divided by 8
gives the byte index of the first non-digit. The leading digits are then
parsed with a conventional loop:

```cpp
uint64_t result = 0;
for (int i = 0; i < first_non_digit; ++i) {
    result = result * 10 + ((sub >> (i * 8)) & 0xFF);
}
```

For longer numbers (more than 8 digits), the parser calls `parse_8_digits()`
in a loop, accumulating via:

```cpp
value = value * pow10[count] + chunk_val;
```

where `pow10[]` is a precomputed table of powers of 10 (1 through 10^8).
Overflow is checked before each multiplication.

______________________________________________________________________

## 6. Branchless Literal Comparison

The bool/null fast paths use a similar SWAR trick for branchless validation.
Instead of comparing four bytes individually (`t`, `r`, `u`, `e`), load all
four bytes as a `uint32_t` and compare with the expected word in one
instruction:

```cpp
uint32_t word;
memcpy(&word, data, 4);

static constexpr uint8_t expected[] = {'t', 'r', 'u', 'e'};
uint32_t expected_word;
memcpy(&expected_word, expected, 4);

if (word != expected_word) return {false, 0, {}};
```

**Source:** `src/strata/speculative/fast_paths.cpp:L499-L521`

The compiler optimizes both `memcpy` calls into register loads and emits a
single `CMP` instruction. This replaces four conditional branches with one --
a significant win on modern pipelined CPUs where branch mispredictions cost
15-20 cycles.

The `"false"` literal uses the same technique for the first 4 bytes (`"fals"`)
plus a single byte comparison for `'e'`.

______________________________________________________________________

## 7. Performance Characteristics

| Operation         | Digits | Instructions | Cycles (approx) |
| ----------------- | ------ | ------------ | --------------- |
| 8-digit SWAR      | 8      | ~12          | 5-6             |
| 4-digit scalar    | 4      | ~8           | 4               |
| 1-digit scalar    | 1      | ~2           | 1               |
| `"true"` literal  | --     | 2 (load+cmp) | 1               |
| `"false"` literal | --     | 3            | 1-2             |
| `"null"` literal  | --     | 2 (load+cmp) | 1               |

The SWAR parser achieves approximately 1.2 billion digits per second on a
3 GHz core, making integer parsing essentially free compared to the memory
bandwidth required to read the JSON document.

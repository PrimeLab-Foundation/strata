# Code Style Guide

This document describes the coding conventions enforced across the Strata
codebase. All C++ code must pass `clang-format` and `clang-tidy` checks
before merging. Python code must pass `ruff format` and `ruff check`.

______________________________________________________________________

## 1. Naming Conventions

| Element               | Convention           | Example                         |
| --------------------- | -------------------- | ------------------------------- |
| Classes / structs     | PascalCase           | `IndexBuilder`, `ChunkResult`   |
| Functions / methods   | snake_case           | `classify_scalar()`, `build()`  |
| Local variables       | snake_case           | `block_count`, `raw_quotes`     |
| Constants / constexpr | SCREAMING_SNAKE_CASE | `MAX_CONTEXTS`, `DECAY_PERIOD`  |
| Member variables      | trailing underscore  | `prev_escaped_`, `table_`       |
| Template parameters   | PascalCase           | `NumBits`, `NumHashes`          |
| Enum values           | SCREAMING_SNAKE_CASE | `ValueType::SHORT_STRING`       |
| Namespaces            | snake_case           | `strata::simd`, `strata::bloom` |
| File names            | snake_case           | `structural_indexer.cpp`        |
| Macros                | SCREAMING_SNAKE_CASE | `STRATA_SIMD_HAS_AVX2`          |

### Prefix conventions

- `k` prefix for static constexpr data: `kLowNibbleTable`, `kWyP0`
- `STRATA_` prefix for all macros: `STRATA_FORCE_SCALAR`, `STRATA_SIMD_HAS_NEON`
- No Hungarian notation. Do not prefix types with `T`, `C`, `I`, etc.

______________________________________________________________________

## 2. clang-format Configuration

The project uses LLVM as the base style with these overrides. The full
configuration lives in `/.clang-format`:

```yaml
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
PointerAlignment: Left
SortIncludes: true
IncludeBlocks: Regroup
```

### Key rules

- **4-space indentation** (no tabs).
- **100-column line limit**. Break long lines at logical points.
- **Pointers and references align left**: `uint8_t* data`, not `uint8_t *data`.
- **Includes are sorted and regrouped**: system headers, then project headers,
  alphabetically within each group.

### Running the formatter

```bash
# Format all tracked C++ files
make fmt

# Format a single file
clang-format -i path/to/file.cpp
```

______________________________________________________________________

## 3. clang-tidy Configuration

The `.clang-tidy` file enables checks from these categories:

- `bugprone-*` -- common bug patterns (minus `easily-swappable-parameters`)
- `cppcoreguidelines-*` -- Core Guidelines (minus `avoid-magic-numbers`,
  `pro-bounds-pointer-arithmetic`, `pro-type-reinterpret-cast`,
  `avoid-non-const-global-variables`)
- `misc-*` -- miscellaneous (minus `no-recursion`)
- `modernize-*` -- modern C++ idioms (minus `use-trailing-return-type`)
- `performance-*` -- performance pitfalls
- `readability-*` -- readability (minus `magic-numbers`, `identifier-length`)

### Disabled checks and rationale

| Check                                   | Reason                                       |
| --------------------------------------- | -------------------------------------------- |
| `pro-type-reinterpret-cast`             | SIMD intrinsics require reinterpret_cast     |
| `pro-bounds-pointer-arithmetic`         | Structural indexing uses pointer offsets     |
| `avoid-magic-numbers` / `magic-numbers` | Lookup tables and hash constants are numeric |
| `easily-swappable-parameters`           | SIMD APIs have multiple same-type params     |
| `use-trailing-return-type`              | Not the project's preferred style            |
| `no-recursion`                          | JSON parsing is inherently recursive         |
| `identifier-length`                     | Short names (`i`, `n`, `m`) in math code     |

### Function complexity limits

```yaml
readability-function-size.LineThreshold: 60
readability-function-size.StatementThreshold: 40
readability-function-size.BranchThreshold: 10
readability-function-size.ParameterThreshold: 5
readability-function-cognitive-complexity.Threshold: 15
```

Functions exceeding these thresholds should be split into smaller helpers.

### Running clang-tidy

```bash
# Run on all headers
clang-tidy -p build/ include/strata/**/*.h

# Run on a specific file
clang-tidy -p build/ src/strata/simd/structural_indexer.cpp
```

______________________________________________________________________

## 4. Comment Density Requirement

Target comment density: **>= 20%** of non-blank lines should be comments or
documentation. This is measured by the `readability_score.py` script:

```bash
make readability-check  # threshold: 85/100
```

### Comment guidelines

- Use `///` for Doxygen-style brief descriptions on public API members.
- Use `/** ... */` for multi-line documentation blocks on classes and
  functions.
- Use `//` for inline implementation comments.
- Every file should have a header comment explaining its purpose and how it
  fits into the overall pipeline.
- Section dividers use `// ====...` (80 columns) for major sections and
  `// ----...` for subsections.

### What to comment

- **Why**, not what. Do not paraphrase the code; explain the design decision
  or the invariant being maintained.
- Non-obvious bit tricks (e.g., SWAR reductions, carry-propagation).
- Platform-specific behavior (e.g., why PCLMUL requires a separate feature
  check from AVX2).
- Performance constraints (e.g., "\< 20 cycles" budgets in hot paths).

______________________________________________________________________

## 5. Annotation Policy

### `[[nodiscard]]`

Apply to all functions that return a value the caller must not ignore:

- All `const` query methods (getters, size, empty, etc.)
- Factory functions
- Functions returning error codes or optional values

```cpp
[[nodiscard]] Prediction predict(const ParserContext& ctx) const noexcept;
[[nodiscard]] Backend detect_backend() noexcept;
```

### `noexcept`

Apply to all functions that are guaranteed not to throw:

- All SIMD classification and indexing functions
- All hot-path prediction and observation methods
- Hash functions
- Simple getters and setters

Destructors are implicitly `noexcept` in C++11 and later.

### `constexpr`

Use for compile-time evaluation where possible:

- Bloom filter sizing (`optimal_bits`, `optimal_hashes`, `align_bits`)
- Lookup table contents (`kLowNibbleTable`, `kHighNibbleTable`)
- Static constants (`ClassMask`, `CONFIDENCE_THRESHOLD_FP`)

### `alignas`

Use for performance-critical data structures:

- `alignas(64)` for data that should start on a cache line boundary
  (e.g., `TransitionModel::table_`)
- `alignas(32)` for AVX2-aligned bit arrays (e.g., `BloomFilter::bits_`)

______________________________________________________________________

## 6. Include Order

1. Corresponding header (e.g., `foo.cpp` includes `foo.h` first)
1. C system headers (`<cstdint>`, `<cstring>`)
1. C++ standard library (`<algorithm>`, `<vector>`)
1. Platform headers (`<immintrin.h>`, `<arm_neon.h>`)
1. Project headers (`"strata/simd/classifier.h"`)

Within each group, sort alphabetically. The `IncludeBlocks: Regroup` setting
in `.clang-format` enforces this automatically.

______________________________________________________________________

## 7. C++ Standard

The project targets **C++20**. Features used include:

- `constexpr` on more complex functions
- Designated initializers
- `[[nodiscard]]` with message (C++20)
- `std::string_view` (C++17, but widely used)
- Structured bindings (`auto [h1, h2] = hash(key)`)

Avoid features that are not well-supported across GCC 12+, Clang 14+, and
MSVC 2022:

- Modules (not yet portable)
- `std::format` (use `printf` or `iostream` for now)
- Coroutines (not needed)

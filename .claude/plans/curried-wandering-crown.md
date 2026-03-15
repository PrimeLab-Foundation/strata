# C++ Codebase Refactoring — Best Practices & Documentation

## Context

The C++ codebase is already well-architected (clean module separation, RAII, templates for devirtualization, SIMD optimizations). This refactoring focuses on adding systematic C++ best-practice annotations and comprehensive documentation that the codebase currently lacks. No architectural changes — purely annotation, documentation, and minor const/noexcept correctness fixes.

## Scope

**17 public headers** in `include/strata/` + **8 core .cpp files** in `src/strata/json/` and `src/strata/search/`. Python bindings are **out of scope** (C API conventions differ from modern C++).

______________________________________________________________________

## Changes

### 1. `[[nodiscard]]` annotations

Add `[[nodiscard]]` to all functions/methods where ignoring the return value is almost certainly a bug.

**Files & targets:**

- **`include/strata/json/json_core.hpp`**
  - `Result<T>::ok()`, `Result<T>::unwrap()`, `Result<T>::value_or()`
  - `FlatMap::find()`, `FlatMap::at()`, `FlatMap::size()`, `FlatMap::empty()`, `FlatMap::count()`
  - `JsonValue::is_*()` and `as_*()` methods
- **`include/strata/json/json_parse.hpp`**
  - `parse_json()`, `parse_sax()`, `get_duplicate_key_policy()`, `consume_parse_warnings()`
- **`include/strata/json/json_serialize.hpp`**
  - `serialize_json()`
- **`include/strata/json/json_document.hpp`**
  - `JsonDocument::from_string()`, `root()`, `root_value()`
- **`include/strata/json/json_cursor.hpp`**
  - All `get_*()` methods, `is_*()` predicates, `array_size()`, `object_size()`, `object_keys()`, `raw()`
- **`include/strata/json/json_mmap.hpp`**
  - `parse_json_file()`, `parse_json_file_cursor()`
- **`include/strata/json/json_parser_inline.hpp`**
  - `parse_sax_inline()`
- **`include/strata/json/ndjson_stream.hpp`**
  - `next()`, `has_next()`, `parse_all()`, `parse_all_fast()`, `next_batch()`, `line_number()`, `lines_processed()`, `error_count()`, `read_raw_line()`
- **`include/strata/search/jsonpath.hpp`**
  - `compile_jsonpath()`, `eval_jsonpath()`, `CompiledPath::steps()`, `CompiledPath::empty()`
- **`include/strata/util/arena_allocator.hpp`**
  - `Arena::allocate()`, `Arena::total_size()`
  - `ArenaStringBuilder::view()`, `to_string()`, `size()`, `data()`
- **`include/strata/util/output_buffer.hpp`**
  - `OutputBuffer::data()`, `size()`, `capacity()`
  - `FixedOutputBuffer::data()`, `size()`, `capacity()`, `overflowed()`
- **`include/strata/util/fast_parse.hpp`**
  - `parse_int_fast()`, `parse_double_fast()`, `scan_string_fast()`, `skip_whitespace_fast()`
- **`include/strata/util/simd_string.hpp`**
  - All functions: `escape_json_string_simd()`, `try_copy_clean_string()`, `find_newline_simd()`, etc.
- **`include/strata/util/fast_dtoa.hpp`**, **`ryu_dtoa.hpp`**, **`dragonbox.hpp`**
  - `fast_dtoa()`, `ryu_d2s()`, `ryu_d2s_buffered()`, `dragonbox_d2s()`

### 2. `noexcept` annotations

Add `noexcept` to functions that cannot throw (or where exceptions would be fatal anyway).

**Files & targets:**

- **`include/strata/json/json_core.hpp`**
  - `JsonValue()` default constructor → `noexcept`
  - All `is_*()` predicates → `noexcept`
  - `FlatMap::size()`, `empty()`, `clear()` → `noexcept`
  - `FlatMap::begin()`, `end()` (all overloads) → `noexcept`
  - `Result<T>::ok()` → `noexcept`
- **`include/strata/json/json_cursor.hpp`**
  - All `is_*()` predicates → `noexcept`
  - `raw()` → `noexcept`
- **`include/strata/json/ndjson_stream.hpp`**
  - `has_next()`, `line_number()`, `lines_processed()`, `error_count()`, `record_error()` → `noexcept`
- **`include/strata/json/json_parser_inline.hpp`**
  - `ParserInline::eof()`, `peek()` → `noexcept`
- **`include/strata/util/output_buffer.hpp`**
  - `OutputBuffer::clear()`, `data()` (both), `size()`, `capacity()` → `noexcept`
  - `FixedOutputBuffer::clear()`, `data()` (both), `size()`, `capacity()`, `overflowed()` → `noexcept`
- **`include/strata/util/fast_parse.hpp`**
  - `parse_int_fast()`, `scan_string_fast()`, `skip_whitespace_fast()` → `noexcept`

### 3. `const` correctness fixes

- **`include/strata/json/json_core.hpp`**
  - `Result<T>::unwrap() const` — already const, no change needed
- **`include/strata/json/json_parser_inline.hpp`**
  - `hex_value()` is already static, fine
- Verify `.cpp` files match header constness (already correct from reading)

### 4. File-level documentation (Doxygen `@file` blocks)

Add a file-level doc comment to every header that currently lacks one. Format:

```cpp
/**
 * @file json_core.hpp
 * @brief Core JSON value model and utility types.
 *
 * Defines the in-memory representation ... (2-3 sentences)
 */
```

**Headers needing file-level docs:**

- `json_core.hpp` — no file doc
- `json_parse.hpp` — no file doc
- `json_cursor.hpp` — no file doc
- `json_parser_inline.hpp` — has comment but not Doxygen
- `jsonpath.hpp` — no file doc
- `fast_parse.hpp` — has comment but not Doxygen format
- `output_buffer.hpp` — no file doc

**Headers already well-documented (verify/minor touch-up only):**

- `json_serialize.hpp`, `json_document.hpp`, `json_mmap.hpp`, `ndjson_stream.hpp` — already have `@param`/`@return` docs
- `arena_allocator.hpp`, `simd_string.hpp`, `fast_dtoa.hpp`, `ryu_dtoa.hpp`, `dragonbox.hpp` — already well-documented

### 5. Class-level and method-level documentation

Add Doxygen `///` or `/** */` comments to classes and methods that currently use plain `//` comments or have none:

- **`json_core.hpp`**: `FlatMap` class doc, `Status` enum doc, `Result<T>` doc, `JsonValue` doc with member descriptions
- **`json_cursor.hpp`**: `JsonCursor` class doc, group low-level vs throwing methods
- **`json_parser_inline.hpp`**: `ParserInline` class doc explaining devirtualization strategy
- **`jsonpath.hpp`**: `PathOp` enum values, `FilterPredicate` fields, `PathStep` fields, `CompiledPath` class
- **`output_buffer.hpp`**: `OutputBuffer` and `FixedOutputBuffer` class docs

### 6. `.cpp` file documentation

Add file-level comments and function-level docs to implementation files:

- **`json_parse.cpp`**: Document `DomBuilderHandler`, thread-local state, why warnings vector is cleared in `parse_json`
- **`json_serialize.cpp`**: Document escape_string behavior, NaN/Inf policy, recursion limits
- **`json_cursor.cpp`**: Document integer conversion logic (modf), throwing vs non-throwing API rationale
- **`json_document.cpp`**: Brief file header
- **`json_mmap.cpp`**: Document MmapFile RAII, madvise usage
- **`ndjson_stream.cpp`**: Document SIMD newline detection, batch chunking strategy
- **`jsonpath_compile.cpp`**: Document PathParser grammar, supported syntax
- **`jsonpath_eval.cpp`**: Document evaluation strategy, materialization rationale

### 7. Named constants for magic numbers

- **`include/strata/util/output_buffer.hpp`**: `1024` → `static constexpr size_t kDefaultInitialCapacity = 1024;`
- **`include/strata/util/arena_allocator.hpp`**: `64 * 1024` → `static constexpr size_t kDefaultBlockSize = 64 * 1024;`
- **`src/strata/json/json_serialize.cpp`**: `1024` → `constexpr size_t kSerializeInitialCapacity = 1024;`

______________________________________________________________________

## File modification order

1. `include/strata/json/json_core.hpp` — nodiscard, noexcept, docs
1. `include/strata/json/json_parse.hpp` — nodiscard, file doc
1. `include/strata/json/json_sax_handler.hpp` — file doc, noexcept on destructor
1. `include/strata/json/json_serialize.hpp` — nodiscard, file doc (already good)
1. `include/strata/json/json_document.hpp` — nodiscard
1. `include/strata/json/json_cursor.hpp` — nodiscard, noexcept, docs
1. `include/strata/json/json_parser_inline.hpp` — noexcept, docs
1. `include/strata/json/json_mmap.hpp` — nodiscard
1. `include/strata/json/ndjson_stream.hpp` — nodiscard, noexcept
1. `include/strata/search/jsonpath.hpp` — nodiscard, docs
1. `include/strata/util/arena_allocator.hpp` — nodiscard, named constant
1. `include/strata/util/output_buffer.hpp` — nodiscard, noexcept, named constant
1. `include/strata/util/fast_parse.hpp` — nodiscard, noexcept, file doc
1. `include/strata/util/simd_string.hpp` — nodiscard
1. `include/strata/util/fast_dtoa.hpp` — nodiscard
1. `include/strata/util/ryu_dtoa.hpp` — nodiscard
1. `include/strata/util/dragonbox.hpp` — nodiscard
1. `src/strata/json/json_parse.cpp` — docs
1. `src/strata/json/json_serialize.cpp` — docs, named constant
1. `src/strata/json/json_cursor.cpp` — docs
1. `src/strata/json/json_document.cpp` — docs
1. `src/strata/json/json_mmap.cpp` — docs
1. `src/strata/json/ndjson_stream.cpp` — docs
1. `src/strata/search/jsonpath_compile.cpp` — docs
1. `src/strata/search/jsonpath_eval.cpp` — docs

## Verification

1. `make test-cpp` — all C++ tests must pass (nodiscard/noexcept must not break compilation)
1. `make test-py` — all Python tests must pass (no ABI changes)
1. `clang-format` — run on modified files to ensure style compliance
1. Verify no `[[nodiscard]]` warnings in existing test code (tests properly consume results)

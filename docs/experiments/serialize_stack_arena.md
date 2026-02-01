# Experiment: Arena Allocator for Serializer Stack

**Date**: 2026-01-31
**Status**: Completed

______________________________________________________________________

## Hypothesis

Using a bump-allocator-backed `std::vector` for the serializer frame stack will reduce per-serialization allocation overhead, especially for deep nesting, without increasing RSS.

______________________________________________________________________

## Prototype

- Added `ArenaAllocator<T>` (STL allocator wrapper) in `include/strata/util/arena_allocator.hpp`.
- Microbenchmark in `experiments/serialize_stack_arena/bench_stack_arena.cpp`.

______________________________________________________________________

## Microbench

**Command**:

```bash
clang++ -std=c++20 -O3 -Iinclude experiments/serialize_stack_arena/bench_stack_arena.cpp -o experiments/serialize_stack_arena/bench_stack_arena
./experiments/serialize_stack_arena/bench_stack_arena --iterations 20000 --depth 512
```

**Dataset**:

- Iterations: 20,000
- Depth: 512 frames
- Operation: push/pop `Frame` stack

**Results**:

- `std::vector` (default allocator): **11.63 ms**
- `ArenaAllocator`: **10.79 ms**
- **~7.2% faster** in this microbench

______________________________________________________________________

## Conclusion

**Go (microbench)**: Arena-backed stack allocation shows a modest win on deep stack workloads with no observed memory regressions. Kept for serializer stack usage only.

# Benchmark Datasets

## Overview

Strata benchmarks use diverse datasets to test parser performance across different workload patterns. All datasets are deterministically generated with fixed random seeds.

See `docs/benchmarks/data_notes.md` for schema expectations and canonical query descriptions.

## Dataset Categories

### 1. Small & Simple

**Purpose**: Quick sanity checks and microbenchmarks

| Dataset             | Size    | Description                  |
| ------------------- | ------- | ---------------------------- |
| `small_simple.json` | \< 1 KB | Basic types, minimal nesting |

**Use Cases:**

- Fast iteration during development
- Unit test performance validation
- Baseline reference

______________________________________________________________________

### 2. Nested Structures

**Purpose**: Test parser recursion and depth handling

#### Nested Objects

| Dataset                      | Size    | Depth | Width | Description             |
| ---------------------------- | ------- | ----- | ----- | ----------------------- |
| `medium_nested_objects.json` | ~1.8 MB | 8     | 4     | Deep object hierarchies |

**Structure:**

```json
{
  "key_0": {
    "key_0": {
      "key_0": { ... },
      "key_1": { ... },
      ...
    },
    ...
  },
  ...
}
```

**Stresses:**

- Object field lookup (O(log n) with std::map)
- Memory allocation for maps
- Recursive parsing depth

#### Nested Arrays

| Dataset                     | Size    | Depth | Width | Description        |
| --------------------------- | ------- | ----- | ----- | ------------------ |
| `medium_nested_arrays.json` | ~765 KB | 8     | 3     | Deep array nesting |

**Structure:**

```json
[
  [
    [
      [1, 2, 3],
      [1, 2, 3],
      [1, 2, 3]
    ],
    ...
  ],
  ...
]
```

**Stresses:**

- Array element access
- Vector allocation/resizing
- Parser stack depth

______________________________________________________________________

### 3. String-Heavy

**Purpose**: Test string parsing, allocation, and UTF-8 handling

| Dataset                   | Size    | Strings | Avg Length | Description       |
| ------------------------- | ------- | ------- | ---------- | ----------------- |
| `large_string_heavy.json` | ~1.1 MB | 5,000   | 200 chars  | Many long strings |

**Content:**

- Random alphanumeric + spaces
- No special escapes (fast path)
- Fixed seed (reproducible)

**Stresses:**

- String allocation
- Memory bandwidth
- Character-by-character parsing

**Variants to Add:**

- Escape-heavy (many `\n`, `\t`, `\"`)
- Unicode-heavy (multi-byte UTF-8)
- Mixed ASCII + Unicode

______________________________________________________________________

### 4. Number-Heavy

**Purpose**: Test number parsing performance

| Dataset                   | Size    | Numbers | Description        |
| ------------------------- | ------- | ------- | ------------------ |
| `large_number_heavy.json` | ~787 KB | 50,000  | Mixed number types |

**Number Types:**

- **Integers**: -1M to 1M (fast path eligible)
- **Floats**: -1000 to 1000 with decimals
- **Scientific**: 1e-10 to 1e10

**Stresses:**

- Integer fast-path parsing
- `std::stod()` for floats
- Exponent handling

______________________________________________________________________

### 5. Mixed Realistic

**Purpose**: Simulate real-world API responses

| Dataset                      | Size    | Records | Description       |
| ---------------------------- | ------- | ------- | ----------------- |
| `large_mixed_realistic.json` | ~2.5 MB | 10,000  | User-like records |

**Record Structure:**

```json
{
  "id": 123,
  "uuid": "a1b2c3d4-5e6f",
  "name": "User 123",
  "email": "user123@example.com",
  "age": 35,
  "score": 87.52,
  "active": true,
  "tags": ["tag1", "tag2"],
  "metadata": {
    "created": "2024-03-15",
    "updated": "2024-05-22",
    "version": 3
  }
}
```

**Characteristics:**

- Mixed types (ints, floats, strings, bools, nulls)
- Moderate nesting (2-3 levels)
- Realistic field names
- Varied array sizes

**Use Cases:**

- Most representative of production workloads
- Good default benchmark

______________________________________________________________________

### 6. Array-Heavy

**Purpose**: Test large array performance

| Dataset                  | Size    | Arrays | Elements/Array | Description              |
| ------------------------ | ------- | ------ | -------------- | ------------------------ |
| `large_array_heavy.json` | ~490 KB | 50     | 2,000          | Large arrays of integers |

**Structure:**

```json
{
  "arrays": {
    "array_0": [0, 1, 2, ..., 1999],
    "array_1": [...],
    ...
  }
}
```

**Stresses:**

- Vector reserve/resize
- Contiguous memory allocation
- Cache effects

______________________________________________________________________

### 7. Users Dataset (Legacy)

**Purpose**: Backward compatibility with existing benchmarks

| Dataset        | Size    | Records | Description              |
| -------------- | ------- | ------- | ------------------------ |
| `users.json`   | ~5.3 MB | 5,000   | User records with orders |
| `users.ndjson` | ~5.3 MB | 5,000   | Newline-delimited format |

**Schema:**

```json
{
  "users": [
    {
      "id": 0,
      "name": "user_0",
      "age": 35,
      "metadata": {
        "created": "2024-01-15",
        "tags": ["tag0", "tag1"],
        "nested": {"level": 5}
      },
      "orders": [
        {
          "order_id": 100,
          "status": "shipped",
          "items": [
            {"sku": "SKU-0-0-0", "qty": 2, "price": 99.99}
          ]
        }
      ]
    }
  ]
}
```

**Generation:** `benchmarks/data/generate_bench_data.py`

______________________________________________________________________

## Dataset Generation

### Deterministic Generation

All datasets use **fixed random seeds** (default: 42) for reproducibility.

### Generating Datasets

```bash
# Generate all benchmark datasets
python benchmarks/datasets.py benchmarks/data/generated

# Generate users dataset only
python benchmarks/data/generate_bench_data.py \
  --out-dir benchmarks/data/generated \
  --num-users 5000 \
  --seed 42
```

### Custom Datasets

To add a new dataset:

1. Add generator function to `benchmarks/datasets.py`
1. Document characteristics here
1. Run generation script
1. Update benchmark suite to include it

______________________________________________________________________

## Dataset Properties Summary

| Dataset         | Size    | Type Focus | Nesting | Use Case         |
| --------------- | ------- | ---------- | ------- | ---------------- |
| small_simple    | \< 1 KB | Mixed      | Shallow | Quick tests      |
| nested_objects  | 1.8 MB  | Objects    | Deep    | Recursion stress |
| nested_arrays   | 765 KB  | Arrays     | Deep    | Stack stress     |
| string_heavy    | 1.1 MB  | Strings    | Shallow | String parsing   |
| number_heavy    | 787 KB  | Numbers    | Shallow | Number parsing   |
| mixed_realistic | 2.5 MB  | Mixed      | Medium  | Production-like  |
| array_heavy     | 490 KB  | Arrays     | Shallow | Large arrays     |
| users           | 5.3 MB  | Mixed      | Medium  | Legacy compat    |

______________________________________________________________________

## Storage and Versioning

**Location:** `benchmarks/data/generated/`

**Gitignore:** Generated files are not committed (`.gitignore`)

**Regeneration:**

- Run generation scripts before benchmarks
- Or use `--regen-data` flag in benchmark orchestrator
- Scripts are version-controlled

______________________________________________________________________

## Future Dataset Ideas

1. **Escape-heavy**: Strings with many `\n`, `\t`, `\"`
1. **Unicode-heavy**: Multi-byte UTF-8, emoji, RTL text
1. **Pathological**: Very long keys, extremely deep nesting
1. **Sparse objects**: Many null/missing fields
1. **Homogeneous arrays**: All same type (optimization opportunity)
1. **Real corpus**: Sample from actual API responses (anonymized)

______________________________________________________________________

## Dataset Quality Checks

Before using datasets in benchmarks:

1. **Valid JSON**: All datasets must parse correctly
1. **Size verification**: Check file sizes match expectations
1. **Determinism**: Same seed → same output
1. **Variety**: Cover different parser code paths

______________________________________________________________________

## Benchmark Execution

Use datasets in benchmarks:

```bash
# Single dataset
python -m benchmarks.bench_main \
  --dataset benchmarks/data/generated/mixed_realistic.json

# Multiple datasets
python -m benchmarks.bench_main \
  --dataset benchmarks/data/generated/*.json
```

See `docs/benchmarks/methodology.md` for full benchmark rules.

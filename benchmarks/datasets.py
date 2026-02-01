"""
Benchmark dataset generators for various JSON workload patterns.

Generates datasets with different characteristics to test parser performance
across different scenarios.
"""

from __future__ import annotations

import json
import random
from pathlib import Path
from typing import Any


def generate_nested_objects(depth: int = 10, width: int = 3) -> dict:
    """Generate deeply nested objects."""
    if depth == 0:
        return {"value": 42}

    obj = {}
    for i in range(width):
        obj[f"key_{i}"] = generate_nested_objects(depth - 1, width)
    return obj


def generate_nested_arrays(depth: int = 10, width: int = 3) -> list:
    """Generate deeply nested arrays."""
    if depth == 0:
        return [1, 2, 3]

    arr = []
    for _ in range(width):
        arr.append(generate_nested_arrays(depth - 1, width))
    return arr


def generate_string_heavy(n_strings: int = 1000, avg_length: int = 100) -> dict:
    """Generate JSON with many strings."""
    rng = random.Random(42)
    chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 "

    strings = {}
    for i in range(n_strings):
        length = max(10, int(rng.gauss(avg_length, avg_length / 4)))
        text = "".join(rng.choice(chars) for _ in range(length))
        strings[f"str_{i}"] = text

    return {"strings": strings}


def generate_number_heavy(n_numbers: int = 10000) -> dict:
    """Generate JSON with many numbers."""
    rng = random.Random(42)

    numbers = {
        "integers": [rng.randint(-1000000, 1000000) for _ in range(n_numbers // 3)],
        "floats": [rng.uniform(-1000, 1000) for _ in range(n_numbers // 3)],
        "scientific": [rng.uniform(1e-10, 1e10) for _ in range(n_numbers // 3)],
    }

    return numbers


def generate_mixed_realistic(n_records: int = 1000) -> list:
    """Generate realistic mixed-type records (like API responses)."""
    rng = random.Random(42)

    records = []
    for i in range(n_records):
        record = {
            "id": i,
            "uuid": f"{rng.randint(0, 0xFFFFFFFF):08x}-{rng.randint(0, 0xFFFF):04x}",
            "name": f"User {i}",
            "email": f"user{i}@example.com",
            "age": rng.randint(18, 80),
            "score": round(rng.uniform(0, 100), 2),
            "active": rng.choice([True, False]),
            "tags": [f"tag{j}" for j in range(rng.randint(1, 5))],
            "metadata": {
                "created": f"2024-{rng.randint(1, 12):02d}-{rng.randint(1, 28):02d}",
                "updated": f"2024-{rng.randint(1, 12):02d}-{rng.randint(1, 28):02d}",
                "version": rng.randint(1, 10),
            },
        }
        records.append(record)

    return records


def generate_array_heavy(n_arrays: int = 100, array_size: int = 1000) -> dict:
    """Generate JSON with large arrays."""
    rng = random.Random(42)

    return {
        "arrays": {
            f"array_{i}": [rng.randint(0, 1000) for _ in range(array_size)] for i in range(n_arrays)
        }
    }


def save_benchmark_datasets(output_dir: str | Path) -> dict[str, Path]:
    """Generate and save all benchmark datasets."""
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    datasets = {
        "small_simple": {"data": [1, 2, 3, {"key": "value"}]},
        "medium_nested_objects": generate_nested_objects(depth=8, width=4),
        "medium_nested_arrays": generate_nested_arrays(depth=8, width=4),
        "large_string_heavy": generate_string_heavy(n_strings=5000, avg_length=200),
        "large_number_heavy": generate_number_heavy(n_numbers=50000),
        "large_mixed_realistic": generate_mixed_realistic(n_records=10000),
        "large_array_heavy": generate_array_heavy(n_arrays=50, array_size=2000),
    }

    saved_files = {}
    for name, data in datasets.items():
        filepath = output_dir / f"{name}.json"
        with open(filepath, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False)
        saved_files[name] = filepath
        print(f"Generated {name}: {filepath.stat().st_size} bytes")

    return saved_files


if __name__ == "__main__":
    import sys

    output_dir = sys.argv[1] if len(sys.argv) > 1 else "benchmarks/data/generated"
    files = save_benchmark_datasets(output_dir)
    print(f"\nGenerated {len(files)} benchmark datasets in {output_dir}")

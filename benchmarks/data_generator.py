"""
Randomized benchmark data generator.

Generates fully randomized JSON and NDJSON datasets with configurable
complexity, structure patterns, and size targets. Each run produces
different data unless a fixed seed is provided.
"""

from __future__ import annotations

import json
import math
import random
import string
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any


@dataclass
class DatasetMetadata:
    """Metadata describing a generated dataset."""

    actual_size_bytes: int = 0
    max_depth: int = 0
    total_keys: int = 0
    total_values: int = 0
    value_type_distribution: dict[str, int] = field(default_factory=dict)
    complexity: str = "mixed"
    target_size_bytes: int = 0
    seed: int | None = None


class RandomBenchmarkDataGenerator:
    """Generate randomized benchmark datasets with configurable complexity."""

    # Size targets in bytes
    SIZE_TARGETS = {
        "tiny": 1024,  # ~1KB
        "small": 100 * 1024,  # ~100KB
        "medium": 1024 * 1024,  # ~1MB
        "large": 10 * 1024 * 1024,  # ~10MB
        "xlarge": 100 * 1024 * 1024,  # ~100MB
    }

    # Unicode sample characters for diverse string generation
    UNICODE_SAMPLES = [
        "日本語",
        "한국어",
        "中文",
        "العربية",
        "ελληνικά",
        "עברית",
        "हिन्दी",
        "ไทย",
        "😀🎉🔥",
        "α β γ δ ε",
        "∑ ∏ ∫ √",
        "© ® ™",
    ]

    # Escape sequences to include randomly
    ESCAPE_CHARS = [
        "\n",
        "\t",
        "\r",
        "\\",
        '"',
        "\b",
        "\f",
    ]

    def __init__(self, seed: int | None = None):
        """
        Initialize the data generator.

        Args:
            seed: Random seed. If None, use current time for true randomization.
        """
        self._seed = seed if seed is not None else time.time_ns()
        self.rng = random.Random(self._seed)
        self._stats = DatasetMetadata(seed=self._seed)

        # Pre-compute character pools (avoids rebuilding on every call)
        self._ascii_chars = string.ascii_letters + string.digits + " "
        self._lower_chars = string.ascii_lowercase
        self._lower_under_chars = string.ascii_lowercase + "_"

    @property
    def seed(self) -> int:
        """Return the seed used for this generator."""
        return self._seed

    def _reset_stats(self) -> None:
        """Reset statistics for a new dataset generation."""
        self._stats = DatasetMetadata(seed=self._seed)
        self._current_depth = 0
        self._max_depth_seen = 0

    def _track_value(self, value_type: str) -> None:
        """Track a value in the statistics."""
        self._stats.total_values += 1
        self._stats.value_type_distribution[value_type] = (
            self._stats.value_type_distribution.get(value_type, 0) + 1
        )

    def _track_key(self) -> None:
        """Track a key in the statistics."""
        self._stats.total_keys += 1

    def _update_depth(self) -> None:
        """Update depth tracking."""
        self._max_depth_seen = max(self._max_depth_seen, self._current_depth)

    def _random_string(
        self,
        min_len: int = 1,
        max_len: int = 100,
        include_unicode: bool = True,
        include_escapes: bool = True,
    ) -> str:
        """Generate a random string with configurable characteristics."""
        length = self.rng.randint(min_len, max_len)

        # Base ASCII characters
        chars = self._ascii_chars

        parts: list[str] = []
        remaining = length

        while remaining > 0:
            choice = self.rng.random()

            if choice < 0.7:
                # Regular ASCII segment — batch generation via choices()
                seg_len = min(self.rng.randint(5, 20), remaining)
                parts.append("".join(self.rng.choices(chars, k=seg_len)))
                remaining -= seg_len
            elif choice < 0.85 and include_unicode:
                # Unicode segment
                parts.append(self.rng.choice(self.UNICODE_SAMPLES))
                remaining -= 3
            elif include_escapes:
                # Escape character
                parts.append(self.rng.choice(self.ESCAPE_CHARS))
                remaining -= 1
            else:
                # Fallback to ASCII
                parts.append(self.rng.choice(chars))
                remaining -= 1

        return "".join(parts)

    def _random_key(self, prefix: str = "") -> str:
        """Generate a random object key."""
        self._track_key()
        style = self.rng.randint(0, 4)

        if style == 0:  # snake
            parts = [
                "".join(self.rng.choices(self._lower_chars, k=self.rng.randint(3, 8)))
                for _ in range(self.rng.randint(1, 3))
            ]
            return prefix + "_".join(parts)
        elif style == 1:  # camel
            parts = []
            for i in range(self.rng.randint(1, 3)):
                word = "".join(
                    self.rng.choices(self._lower_chars, k=self.rng.randint(3, 8))
                )
                if i > 0:
                    word = word.capitalize()
                parts.append(word)
            return prefix + "".join(parts)
        elif style == 2:  # short
            return prefix + "".join(
                self.rng.choices(self._lower_chars, k=self.rng.randint(1, 4))
            )
        elif style == 3:  # long
            return prefix + "".join(
                self.rng.choices(self._lower_under_chars, k=self.rng.randint(20, 50))
            )
        else:  # unicode
            return prefix + self.rng.choice(self.UNICODE_SAMPLES) + "_key"

    def _random_number(self) -> int | float:
        """Generate a random number (int or float, various ranges)."""
        self._track_value("number")
        choice = self.rng.random()

        if choice < 0.3:
            # Small integer
            return self.rng.randint(-1000, 1000)
        elif choice < 0.5:
            # Large integer
            return self.rng.randint(-(2**31), 2**31 - 1)
        elif choice < 0.7:
            # Regular float
            return round(self.rng.uniform(-10000, 10000), self.rng.randint(1, 6))
        elif choice < 0.85:
            # Scientific notation range
            exp = self.rng.randint(-10, 10)
            base = self.rng.uniform(1, 10)
            return base * (10**exp)
        else:
            # Edge cases
            edge_cases = [
                0,
                -0.0,
                1,
                -1,
                2147483647,
                -2147483648,
                1e-15,
                1e15,
                0.1 + 0.2,  # IEEE 754 fun
            ]
            return self.rng.choice(edge_cases)

    def _random_value(
        self,
        depth: int = 0,
        max_depth: int = 10,
        value_types: str = "all",
    ) -> Any:
        """Generate a random JSON value."""
        # Determine available types based on value_types parameter
        if value_types == "strings":
            type_choices = ["string"]
        elif value_types == "numbers":
            type_choices = ["number"]
        elif value_types == "mixed":
            type_choices = ["string", "number", "boolean", "null"]
        else:  # all
            type_choices = ["string", "number", "boolean", "null", "array", "object"]
            if depth >= max_depth:
                type_choices = ["string", "number", "boolean", "null"]

        chosen_type = self.rng.choice(type_choices)

        if chosen_type == "string":
            self._track_value("string")
            return self._random_string(1, 200)
        elif chosen_type == "number":
            return self._random_number()
        elif chosen_type == "boolean":
            self._track_value("boolean")
            return self.rng.choice([True, False])
        elif chosen_type == "null":
            self._track_value("null")
            return None
        elif chosen_type == "array":
            return self._random_array(depth + 1, max_depth, value_types)
        else:  # object
            return self._random_object(depth + 1, max_depth, value_types)

    def _random_array(
        self,
        depth: int = 0,
        max_depth: int = 10,
        value_types: str = "all",
        min_len: int = 0,
        max_len: int = 20,
    ) -> list:
        """Generate a random array."""
        self._current_depth = depth
        self._update_depth()
        self._track_value("array")

        length = self.rng.randint(min_len, max_len)

        # Sometimes make homogeneous arrays
        if self.rng.random() < 0.3:
            # Homogeneous array
            elem_type = self.rng.choice(["string", "number", "boolean"])
            if elem_type == "string":
                return [self._random_string(5, 50) for _ in range(length)]
            elif elem_type == "number":
                return [self._random_number() for _ in range(length)]
            else:
                return [self.rng.choice([True, False]) for _ in range(length)]

        # Heterogeneous array
        return [
            self._random_value(depth, max_depth, value_types) for _ in range(length)
        ]

    def _random_object(
        self,
        depth: int = 0,
        max_depth: int = 10,
        value_types: str = "all",
        min_keys: int = 1,
        max_keys: int = 10,
    ) -> dict:
        """Generate a random object."""
        self._current_depth = depth
        self._update_depth()
        self._track_value("object")

        num_keys = self.rng.randint(min_keys, max_keys)
        obj = {}

        for _ in range(num_keys):
            key = self._random_key()
            # Avoid duplicate keys
            while key in obj:
                key = self._random_key()
            obj[key] = self._random_value(depth, max_depth, value_types)

        return obj

    def _generate_flat_object(self, target_keys: int) -> dict:
        """Generate a flat object with many keys."""
        obj = {}
        for i in range(target_keys):
            key = self._random_key(f"key_{i}_")
            obj[key] = self._random_value(depth=10, max_depth=10)  # Force no nesting
        return obj

    def _generate_deeply_nested(self, depth: int) -> dict:
        """Generate a deeply nested structure."""
        if depth <= 0:
            return {"leaf": self._random_value(depth=10, max_depth=10)}

        return {
            self._random_key(): self._generate_deeply_nested(depth - 1),
            "sibling": self._random_value(depth=10, max_depth=10),
        }

    def _generate_wide_arrays(self, width: int) -> dict:
        """Generate structure with wide arrays."""
        return {
            "large_array": self._random_array(depth=1, max_depth=2, min_len=width, max_len=width),
            "nested_arrays": [
                self._random_array(depth=2, max_depth=3, min_len=width // 10, max_len=width // 10)
                for _ in range(10)
            ],
        }

    def _generate_realistic_record(self, index: int) -> dict:
        """Generate a realistic API-response-like record."""
        self._track_value("object")

        return {
            "id": index,
            "uuid": f"{self.rng.randint(0, 0xFFFFFFFF):08x}-{self.rng.randint(0, 0xFFFF):04x}-{self.rng.randint(0, 0xFFFF):04x}",
            "name": self._random_string(5, 30, include_unicode=True, include_escapes=False),
            "email": f"user{index}@{self._random_string(5, 10, include_unicode=False, include_escapes=False)}.com",
            "age": self.rng.randint(18, 85),
            "score": round(self.rng.uniform(0, 100), 2),
            "active": self.rng.choice([True, False]),
            "role": self.rng.choice(["admin", "user", "moderator", "guest"]),
            "tags": [self._random_string(3, 15, include_unicode=False, include_escapes=False)
                     for _ in range(self.rng.randint(0, 5))],
            "preferences": {
                "theme": self.rng.choice(["light", "dark", "system"]),
                "notifications": self.rng.choice([True, False]),
                "language": self.rng.choice(["en", "es", "fr", "de", "ja", "zh"]),
            },
            "metadata": {
                "created": f"2024-{self.rng.randint(1, 12):02d}-{self.rng.randint(1, 28):02d}T{self.rng.randint(0, 23):02d}:{self.rng.randint(0, 59):02d}:00Z",
                "updated": f"2025-{self.rng.randint(1, 12):02d}-{self.rng.randint(1, 28):02d}T{self.rng.randint(0, 23):02d}:{self.rng.randint(0, 59):02d}:00Z",
                "version": self.rng.randint(1, 100),
                "source": self.rng.choice(["api", "web", "mobile", "import"]),
            },
            "orders": [
                {
                    "order_id": f"ORD-{self.rng.randint(10000, 99999)}",
                    "total": round(self.rng.uniform(10, 1000), 2),
                    "items": [
                        {
                            "product": self._random_string(10, 40, include_unicode=False, include_escapes=False),
                            "price": round(self.rng.uniform(1, 500), 2),
                            "quantity": self.rng.randint(1, 10),
                        }
                        for _ in range(self.rng.randint(1, 5))
                    ],
                }
                for _ in range(self.rng.randint(0, 3))
            ],
        }

    def generate_dataset(
        self,
        *,
        target_size_bytes: int | str = "medium",
        complexity: str = "mixed",
        value_types: str = "all",
    ) -> tuple[Any, DatasetMetadata]:
        """
        Generate randomized JSON structure.

        Args:
            target_size_bytes: Target size in bytes, or size name ("tiny", "small", "medium", "large", "xlarge")
            complexity: Structure complexity ("simple", "nested", "wide", "mixed", "realistic")
            value_types: Types of values to include ("strings", "numbers", "mixed", "all")

        Returns:
            (data, metadata) where metadata contains generation statistics
        """
        self._reset_stats()

        # Resolve size target
        if isinstance(target_size_bytes, str):
            target_size = self.SIZE_TARGETS.get(target_size_bytes, self.SIZE_TARGETS["medium"])
        else:
            target_size = target_size_bytes

        self._stats.target_size_bytes = target_size
        self._stats.complexity = complexity

        # Generate data based on complexity
        if complexity == "simple":
            data = self._generate_simple_data(target_size, value_types)
        elif complexity == "nested":
            data = self._generate_nested_data(target_size, value_types)
        elif complexity == "wide":
            data = self._generate_wide_data(target_size, value_types)
        elif complexity == "realistic":
            data = self._generate_realistic_data(target_size)
        else:  # mixed
            data = self._generate_mixed_data(target_size, value_types)

        # Calculate actual size
        serialized = json.dumps(data, ensure_ascii=False)
        self._stats.actual_size_bytes = len(serialized.encode("utf-8"))
        self._stats.max_depth = self._max_depth_seen

        return data, self._stats

    @staticmethod
    def _json_size(obj: Any) -> int:
        """Return the UTF-8 byte length of *obj* serialized as JSON.

        Used only for incremental size tracking of individual records so we
        avoid the O(n²) pattern of re-serializing the entire dataset in a
        loop.
        """
        return len(json.dumps(obj, ensure_ascii=False).encode("utf-8"))

    def _generate_simple_data(self, target_size: int, value_types: str) -> dict:
        """Generate simple flat structures."""
        data: dict[str, Any] = {"records": []}
        # Overhead: {"records":[]}  = 14 bytes, plus a comma per element
        current_size = 14

        while current_size < target_size:
            record = self._random_object(depth=5, max_depth=5, value_types=value_types, min_keys=3, max_keys=8)
            data["records"].append(record)
            current_size += self._json_size(record) + 1  # +1 for comma

        return data

    def _generate_nested_data(self, target_size: int, value_types: str) -> dict:
        """Generate deeply nested structures."""
        # Calculate depth based on target size
        depth = min(50, max(5, int(math.log2(target_size / 100))))

        data = {
            "root": self._generate_deeply_nested(depth),
            "metadata": {
                "generator": "strata_benchmark",
                "depth_target": depth,
            },
        }

        # Add more nested structures until target size is reached
        current_size = self._json_size(data)
        counter = 0
        while current_size < target_size:
            nested_depth = self.rng.randint(3, min(20, depth))
            branch = self._generate_deeply_nested(nested_depth)
            key = f"branch_{counter}"
            data[key] = branch
            # +4: quotes around key, colon, comma
            current_size += len(key) + 4 + self._json_size(branch)
            counter += 1

        return data

    def _generate_wide_data(self, target_size: int, value_types: str) -> dict:
        """Generate wide structures with large arrays and many keys."""
        # Estimate number of elements needed
        avg_element_size = 50
        target_elements = target_size // avg_element_size

        data = {
            "wide_object": self._generate_flat_object(min(1000, target_elements // 4)),
            "large_arrays": [],
        }

        current_size = self._json_size(data)
        while current_size < target_size:
            array_size = min(1000, (target_size - current_size) // 20)
            if array_size < 10:
                break
            arr = self._random_array(depth=1, max_depth=2, min_len=array_size, max_len=array_size)
            data["large_arrays"].append(arr)
            current_size += self._json_size(arr) + 1

        return data

    def _generate_realistic_data(self, target_size: int) -> dict:
        """Generate realistic API-response-like data."""
        data = {"users": []}
        current_size = 14
        index = 0

        while current_size < target_size:
            record = self._generate_realistic_record(index)
            data["users"].append(record)
            current_size += self._json_size(record) + 1
            index += 1

        return data

    def _generate_mixed_data(self, target_size: int, value_types: str) -> dict:
        """Generate mixed complexity data."""
        data: dict[str, Any] = {
            "simple_records": [],
            "nested_structures": {},
            "wide_arrays": [],
            "realistic_users": [],
        }

        # Allocate roughly equal portions to each type
        portion_size = target_size // 4

        # Simple records
        current_size = 0
        while current_size < portion_size:
            record = self._random_object(depth=5, max_depth=5, value_types=value_types, min_keys=3, max_keys=8)
            data["simple_records"].append(record)
            current_size += self._json_size(record) + 1

        # Nested structures
        depth = min(20, max(3, int(math.log2(portion_size / 100))))
        counter = 0
        current_size = 0
        while current_size < portion_size:
            branch = self._generate_deeply_nested(self.rng.randint(3, depth))
            key = f"branch_{counter}"
            data["nested_structures"][key] = branch
            current_size += len(key) + 4 + self._json_size(branch)
            counter += 1

        # Wide arrays
        current_size = 0
        while current_size < portion_size:
            array_size = min(500, (portion_size - current_size) // 20)
            if array_size < 5:
                break
            arr = self._random_array(depth=1, max_depth=2, min_len=array_size, max_len=array_size)
            data["wide_arrays"].append(arr)
            current_size += self._json_size(arr) + 1

        # Realistic users
        index = 0
        current_size = 0
        while current_size < portion_size:
            record = self._generate_realistic_record(index)
            data["realistic_users"].append(record)
            current_size += self._json_size(record) + 1
            index += 1

        return data

    def generate_ndjson_dataset(
        self,
        *,
        num_lines: int = 1000,
        line_complexity: str = "mixed",
        line_size_variation: float = 0.5,
    ) -> tuple[bytes, DatasetMetadata]:
        """
        Generate NDJSON with variable line complexity.

        Args:
            num_lines: Number of lines to generate
            line_complexity: Complexity of individual lines ("simple", "nested", "realistic", "mixed")
            line_size_variation: 0.0 = uniform size, 1.0 = highly variable

        Returns:
            (ndjson_bytes, metadata)
        """
        self._reset_stats()
        self._stats.complexity = f"ndjson_{line_complexity}"

        lines = []
        for i in range(num_lines):
            # Vary line complexity based on line_size_variation
            if line_complexity == "mixed":
                complexity_choice = self.rng.choice(["simple", "nested", "realistic"])
            else:
                complexity_choice = line_complexity

            # Generate line based on complexity
            if complexity_choice == "simple":
                line = self._random_object(depth=3, max_depth=3, min_keys=3, max_keys=8)
            elif complexity_choice == "nested":
                depth = self.rng.randint(2, 6)
                line = self._generate_deeply_nested(depth)
            else:  # realistic
                line = self._generate_realistic_record(i)

            # Apply size variation
            if line_size_variation > 0 and self.rng.random() < line_size_variation:
                # Sometimes add extra data to vary size
                extra_keys = self.rng.randint(1, int(10 * line_size_variation))
                for j in range(extra_keys):
                    line[f"extra_{j}"] = self._random_value(depth=5, max_depth=5)

            lines.append(json.dumps(line, ensure_ascii=False))

        ndjson = "\n".join(lines)
        ndjson_bytes = ndjson.encode("utf-8")

        self._stats.actual_size_bytes = len(ndjson_bytes)
        self._stats.max_depth = self._max_depth_seen
        self._stats.total_values = num_lines  # Override with line count

        return ndjson_bytes, self._stats

    def save_dataset(
        self,
        output_dir: str | Path,
        name: str,
        *,
        target_size: int | str = "medium",
        complexity: str = "mixed",
        generate_ndjson: bool = True,
        ndjson_lines: int = 1000,
    ) -> dict[str, Path]:
        """
        Generate and save a dataset to files.

        Args:
            output_dir: Directory to save files
            name: Base name for the dataset
            target_size: Target size for JSON data
            complexity: Complexity level
            generate_ndjson: Whether to also generate NDJSON
            ndjson_lines: Number of lines for NDJSON

        Returns:
            Dictionary mapping file type to path
        """
        output_dir = Path(output_dir)
        output_dir.mkdir(parents=True, exist_ok=True)

        saved_files = {}

        # Generate and save JSON
        data, metadata = self.generate_dataset(
            target_size_bytes=target_size,
            complexity=complexity,
        )
        json_path = output_dir / f"{name}.json"
        with open(json_path, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False)
        saved_files["json"] = json_path

        # Save metadata
        meta_path = output_dir / f"{name}_metadata.json"
        meta_dict = {
            "actual_size_bytes": metadata.actual_size_bytes,
            "max_depth": metadata.max_depth,
            "total_keys": metadata.total_keys,
            "total_values": metadata.total_values,
            "value_type_distribution": metadata.value_type_distribution,
            "complexity": metadata.complexity,
            "target_size_bytes": metadata.target_size_bytes,
            "seed": metadata.seed,
        }
        with open(meta_path, "w", encoding="utf-8") as f:
            json.dump(meta_dict, f, indent=2)
        saved_files["metadata"] = meta_path

        # Generate and save NDJSON if requested
        if generate_ndjson:
            ndjson_data, ndjson_meta = self.generate_ndjson_dataset(
                num_lines=ndjson_lines,
                line_complexity=complexity if complexity != "wide" else "mixed",
            )
            ndjson_path = output_dir / f"{name}.ndjson"
            with open(ndjson_path, "wb") as f:
                f.write(ndjson_data)
            saved_files["ndjson"] = ndjson_path

        return saved_files


def generate_benchmark_session(
    output_dir: str | Path,
    *,
    sizes: list[str] | None = None,
    seed: int | None = None,
) -> dict[str, dict[str, Path]]:
    """
    Generate a complete benchmark session with multiple dataset sizes.

    Args:
        output_dir: Base output directory
        sizes: List of size names to generate (default: ["small", "medium", "large"])
        seed: Random seed for reproducibility

    Returns:
        Nested dictionary: {size: {file_type: path}}
    """
    if sizes is None:
        sizes = ["small", "medium", "large"]

    output_dir = Path(output_dir)
    timestamp = time.strftime("%Y%m%d_%H%M%S")
    session_dir = output_dir / f"session_{timestamp}"

    generator = RandomBenchmarkDataGenerator(seed=seed)
    results = {}

    for size in sizes:
        size_dir = session_dir / size

        # Map sizes to ndjson line counts
        ndjson_lines = {
            "tiny": 100,
            "small": 1000,
            "medium": 5000,
            "large": 20000,
            "xlarge": 100000,
        }.get(size, 1000)

        # Generate with mixed complexity for variety
        files = generator.save_dataset(
            size_dir,
            f"benchmark_{size}",
            target_size=size,
            complexity="mixed",
            ndjson_lines=ndjson_lines,
        )
        results[size] = files
        print(f"Generated {size}: {files['json']} ({files['json'].stat().st_size} bytes)")

    # Save session metadata
    session_meta = {
        "timestamp": timestamp,
        "seed": generator.seed,
        "sizes": sizes,
    }
    meta_path = session_dir / "session_metadata.json"
    with open(meta_path, "w", encoding="utf-8") as f:
        json.dump(session_meta, f, indent=2)

    return results


if __name__ == "__main__":
    import sys

    output_dir = sys.argv[1] if len(sys.argv) > 1 else "benchmarks/data/generated"
    seed = int(sys.argv[2]) if len(sys.argv) > 2 else None

    print(f"Generating benchmark session in {output_dir}")
    if seed:
        print(f"Using fixed seed: {seed}")

    results = generate_benchmark_session(output_dir, seed=seed)
    print(f"\nGenerated {len(results)} datasets")

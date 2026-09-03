#!/usr/bin/env python3
"""Generate the PGO training corpus.

Training data is not benchmark data. Its job is *branch coverage of the hot
paths*, so that the profile tells the optimizer which side of each branch is
taken in practice: integer vs float number paths, cached vs uncached keys,
escape-free vs escaped strings, ASCII vs multi-byte UTF-8, shallow vs deep
nesting. Volume beyond that only makes the instrumented run slower.

Deterministic: seed 0xDA7A, so two PGO builds train on identical input.

Writes `<out-dir>/train.json` and `<out-dir>/train.ndjson`, ~TARGET_MB each.
"""

from __future__ import annotations

import argparse
import json
import random
import string
from pathlib import Path

SEED = 0xDA7A
TARGET_MB = 10

# --- string pools, one per scanner path ------------------------------------

# No escapes, no multi-byte: the fast path the scanner is optimized for.
PLAIN = [
    "alpha",
    "a somewhat longer plain ascii value with spaces",
    "id-00000000-0000-4000-8000-000000000000",
    "".join(string.ascii_letters),
    "x" * 200,  # long enough to matter to a block-wise scan
]

# Every short escape form, so the escape branch is not a rarity.
ESCAPED = [
    'quote:"here"',
    "backslash:C:\\path\\to\\file",
    "line\nbreak",
    "tab\there",
    "carriage\rreturn",
    "form\x0cfeed",
    "back\bspace",
    'all of them: \t\n\r\f\b"\\',
]

# Multi-byte UTF-8 at each width, plus a surrogate-pair-encoding astral char.
UNICODE = [
    "café résumé naïve",
    "Ünïcödé têst",
    "Текст на русском",
    "日本語のテキスト",
    "한국어 텍스트",
    "Ελληνικά",
    "العربية",
    "math: ∑∏∫∂√∞",
    "emoji: 🎉🚀🔥",
    "mixed ascii and 漢字 and 🌍",
]

# Numbers straddling every classification boundary in parse_number_unified.
NUMBERS = [
    0,
    1,
    -1,
    42,
    -42,
    1000000,
    2**31 - 1,
    -(2**31),
    2**53 - 1,
    2**63 - 1,
    -(2**63),
    2**64 + 1,  # big-int slow path
    10**30,
    0.0,
    -0.0,
    0.5,
    -3.25,
    1e10,
    1e-10,
    1.7976931348623157e308,
    5e-324,
    3.141592653589793,
]


def _record(rng: random.Random, index: int) -> dict:
    """One record of a few rotating shapes, so the profile carries the paths the
    benchmark rows measure: records whose keys repeat (the prepared-key emit,
    the schema cache's four-way select), long scalar runs (the run writers),
    and -- kept at depth one, where they cannot retire the top level -- the
    cache's miss and retire paths. A varying key on every top-level record
    made every one of them a miss, retired that depth after 64, and profiled
    the uncached walk as the hot path: the MSVC PGO build then read the
    record path 1.78x behind orjson where the plain build read 1.17x."""
    record = {
        # Stable keys — the cache should hit on these every time.
        "id": index,
        "name": rng.choice(PLAIN),
        "active": rng.choice([True, False]),
        "score": rng.choice(NUMBERS),
        "note": rng.choice(ESCAPED),
        "label": rng.choice(UNICODE),
        "missing": None,
        "tags": [rng.choice(PLAIN) for _ in range(rng.randint(0, 6))],
        "numbers": [rng.choice(NUMBERS) for _ in range(rng.randint(0, 8))],
    }
    # Three shapes rotate at the top level: the four-way select hits after the
    # first sighting of each and every record after that emits prepared keys.
    shape = index % 3
    if shape == 1:
        record["rank"] = index % 97
    elif shape == 2:
        record["ratio"] = rng.random()
        del record["missing"]
    # Varying keys at depth one: misses, remembers and, after 64, retirement
    # of that depth — the cold paths, trained without evicting the hot one.
    record["extra"] = {f"field_{index % 512}": rng.random()}
    # Long homogeneous runs every eighth record: the scalar-run writers take
    # whole 64-element blocks at a time, which short lists never reach.
    if index % 8 == 0:
        record["series"] = [rng.random() * 1000 for _ in range(rng.randint(64, 200))]
        record["ids"] = list(range(index, index + rng.randint(64, 200)))
        record["names"] = [rng.choice(PLAIN) for _ in range(rng.randint(64, 200))]
    # Nested objects, occasionally deep, to exercise the recursion guard's
    # shallow path and the container-reuse logic.
    depth = rng.choice([0, 1, 2, 2, 3, 6])
    nested: dict | list = {"leaf": rng.choice(NUMBERS)}
    for _ in range(depth):
        nested = {"child": nested, "sibling": [rng.choice(PLAIN)]}
    record["nested"] = nested
    # A wide array of small objects — the common shape in real payloads.
    record["items"] = [
        {
            "sku": f"sku-{rng.randint(0, 99999)}",
            "qty": rng.randint(1, 50),
            "price": rng.random() * 100,
        }
        for _ in range(rng.randint(0, 5))
    ]
    return record


def generate(out_dir: Path, target_mb: int = TARGET_MB) -> tuple[Path, Path]:
    out_dir.mkdir(parents=True, exist_ok=True)
    rng = random.Random(SEED)
    target_bytes = target_mb * 1024 * 1024

    records: list[dict] = []
    size = 0
    index = 0
    while size < target_bytes:
        record = _record(rng, index)
        records.append(record)
        size += len(json.dumps(record))
        index += 1

    json_path = out_dir / "train.json"
    ndjson_path = out_dir / "train.ndjson"
    json_path.write_text(json.dumps(records), encoding="utf-8")
    with ndjson_path.open("w", encoding="utf-8") as handle:
        for record in records:
            handle.write(json.dumps(record))
            handle.write("\n")
    return json_path, ndjson_path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out-dir", type=Path, required=True)
    parser.add_argument("--target-mb", type=int, default=TARGET_MB)
    args = parser.parse_args()

    json_path, ndjson_path = generate(args.out_dir, args.target_mb)
    for path in (json_path, ndjson_path):
        print(f"  {path} ({path.stat().st_size / 1024 / 1024:.1f} MB)")


if __name__ == "__main__":
    main()

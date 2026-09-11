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
    the schema cache's four-way select, the parser's key predictor), long
    scalar runs (the run writers), and the cache's miss and retire paths.

    The cache selects a way by (key count, first key) and, after 64 misses at
    one dict depth, retires that depth for the thread's life
    (python_dumps_output.h, DepthSchemas::select). So every shape here keeps
    one key row -- optional content is a value change under a key every record
    carries, never a key that comes and goes -- the top-level shapes carry
    distinct key counts, and the one varying key sits at a depth of its own,
    below a chain of one-key dicts, where the retirement it trains reaches no
    depth a repeating shape uses. The previous corpus varied a key at depth
    two on every record and gave two top-level shapes one (count, first key)
    pair with different rows: both depths retired within the first document,
    and the profile carried the fallback as the hot path (E26-P8)."""
    long_runs = index % 8 == 0

    def run(count: int, make) -> list:
        # Long homogeneous runs every eighth record: the scalar-run writers
        # take whole 64-element blocks at a time, which short lists never reach.
        return [make() for _ in range(rng.randint(64, 200) if long_runs else rng.randint(0, 3))]

    record = {
        # Stable keys -- the cache should hit on these every time.
        "id": index,
        "name": rng.choice(PLAIN),
        "active": rng.choice([True, False]),
        "score": rng.choice(NUMBERS),
        "note": rng.choice(ESCAPED),
        "label": rng.choice(UNICODE),
        "missing": None,
        "tags": [rng.choice(PLAIN) for _ in range(rng.randint(0, 6))],
        "numbers": [rng.choice(NUMBERS) for _ in range(rng.randint(0, 8))],
        "series": run(0, lambda: rng.random() * 1000),
        "ids": run(0, lambda: index + rng.randint(0, 200)),
        "names": run(0, lambda: rng.choice(PLAIN)),
    }
    # Three shapes rotate at the top level with distinct key counts under one
    # first key: three (count, first key) pairs fit the four ways, and every
    # record after the first of its shape emits prepared keys.
    shape = index % 3
    if shape >= 1:
        record["rank"] = index % 97
    if shape == 2:
        record["ratio"] = rng.random()
    # Varying keys: misses, remembers and, after 64, retirement -- at depth
    # nine, below a chain of one-key dicts that repeat, so the cold paths are
    # trained without evicting a hot depth.
    churn: dict = {f"field_{index % 512}": rng.random()}
    for _ in range(7):
        churn = {"deeper": churn}
    record["extra"] = churn
    # Nested objects, occasionally deep, to exercise the recursion guard's
    # shallow path and the container-reuse logic.
    depth = rng.choice([0, 1, 2, 2, 3, 6])
    nested: dict | list = {"leaf": rng.choice(NUMBERS)}
    for _ in range(depth):
        nested = {"child": nested, "sibling": [rng.choice(PLAIN)]}
    record["nested"] = nested
    # A wide array of small objects -- the common shape in real payloads.
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

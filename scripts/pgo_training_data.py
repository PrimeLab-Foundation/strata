#!/usr/bin/env python3
"""
PGO training data generator.

Generates large, diverse JSON and NDJSON files designed to exercise every
hot path in the Strata parser and serializer:
  - SIMD string scan (escapes, unicode, long strings)
  - Integer fast path vs float fallback
  - Key cache hits AND misses (2000+ unique keys)
  - Deep nesting, wide objects, mixed arrays
  - SAX search handler state machine
  - UTF-8 validation paths

Output:
  build/pgo/train.json   (~64MB, single array of diverse records)
  build/pgo/train.ndjson  (~64MB, one record per line)
"""

from __future__ import annotations

import json
import random
import string
from pathlib import Path

SEED = 0xDA7A
TARGET_MB = 10  # ~10MB gives good branch frequency stats; ~20s under instrumentation

# Pre-built string pools
_ESCAPE_STRINGS = [
    'line1\nline2\nline3',
    'tab\there\tand\there',
    'quote: "hello world"',
    'backslash: C:\\Users\\test\\file',
    'mixed\t"escapes"\nare\\fun',
    'form\x0cfeed',
    'cr\rreturn',
    'all escapes: \t\n\r\\"',
]

_UNICODE_STRINGS = [
    "日本語テキスト",
    "中文测试数据",
    "한국어 텍스트",
    "Ελληνικά κείμενο",
    "Текст на русском языке",
    "العربية نص",
    "café résumé naïve",
    "Ñoño año español",
    "Ünïcödé têst",
    "数学: ∑∏∫∂√∞",
]

_EMOJI_STRINGS = [
    "🎉🎊🎈🎁",
    "❤️💙💚💛💜",
    "🚀🌟⭐✨",
    "👍👎👏🤝",
    "🔥💯🎯🏆",
    "Hello 🌍 World 🌎 Earth 🌏",
]


def _make_key_pool(rng: random.Random) -> list[str]:
    """Build ~2200 unique keys covering all key-cache branches."""
    keys: list[str] = []

    # Short keys (1-3 chars)
    for c in string.ascii_lowercase:
        keys.append(c)
    for a in "abcdefghij":
        for b in "0123456789":
            keys.append(a + b)

    # Medium keys (5-15 chars)
    medium = [
        "user_name",
        "order_total",
        "item_price",
        "created_at",
        "updated_at",
        "is_active",
        "email_addr",
        "phone_num",
        "zip_code",
        "country_code",
        "first_name",
        "last_name",
        "birth_date",
        "street_addr",
        "city_name",
        "state_prov",
        "postal_code",
        "account_id",
        "session_id",
        "request_id",
        "response_ms",
        "status_code",
        "error_msg",
        "retry_count",
        "timeout_ms",
        "batch_size",
        "page_number",
        "sort_order",
        "filter_by",
        "group_key",
        "metric_name",
        "metric_val",
        "label_text",
        "icon_path",
        "color_hex",
        "font_size",
        "line_height",
        "margin_top",
        "padding_lr",
        "border_rad",
        "max_width",
        "min_height",
        "flex_grow",
        "grid_cols",
        "z_index_val",
        "anim_delay",
        "trans_dur",
        "opacity_val",
        "rotate_deg",
        "scale_fact",
    ]
    keys.extend(medium)

    # Long keys (30-60 chars)
    for i in range(200):
        keys.append(f"very_long_configuration_parameter_name_{i:04d}")

    # Keys with special chars
    specials = [
        "key with spaces",
        "data-type",
        "ns:element",
        "dot.separated.key",
        "path/to/value",
        "has\"quote",
        "back\\slash",
        "empty=value",
        "at@sign",
        "hash#tag",
        "dollar$amt",
        "pct%done",
        "amp&more",
        "pipe|sep",
        "tilde~end",
        "caret^top",
        "bang!stop",
    ]
    for i in range(50):
        specials.append(f"special key {i} with spaces")
    keys.extend(specials)

    # Unicode keys
    unicode_keys = [
        "名前",
        "größe",
        "цена",
        "가격",
        "価格",
        "données",
        "información",
        "数量",
        "説明",
        "備考",
        "住所",
        "電話",
        "メール",
        "会社",
        "部署",
        "αριθμός",
        "ποσότητα",
        "τιμή",
        "περιγραφή",
        "κατηγορία",
        "عنوان",
        "سعر",
        "كمية",
        "وصف",
        "تاريخ",
    ]
    for i in range(100):
        unicode_keys.append(f"ключ_{i:03d}")
    keys.extend(unicode_keys)

    # Random keys to reach 2000+
    chars = string.ascii_letters + string.digits + "_"
    while len(keys) < 2200:
        length = rng.randint(4, 25)
        keys.append("".join(rng.choices(chars, k=length)))

    return keys


def _pre_generate_string_pool(rng: random.Random, n: int = 500) -> list[str]:
    """Pre-generate a pool of diverse strings to pick from (avoids per-record cost)."""
    pool: list[str] = []
    ascii_chars = string.ascii_letters + string.digits + " .,;:!?-_"

    for _ in range(n):
        kind = rng.randint(0, 9)
        if kind <= 3:
            pool.append("".join(rng.choices(ascii_chars, k=rng.randint(3, 80))))
        elif kind == 4:
            pool.append(rng.choice(_ESCAPE_STRINGS))
        elif kind == 5:
            pool.append(rng.choice(_UNICODE_STRINGS))
        elif kind == 6:
            pool.append(rng.choice(_EMOJI_STRINGS))
        elif kind == 7:
            pool.append("")
        elif kind == 8:
            # Long string (~1KB)
            pool.append("".join(rng.choices(ascii_chars, k=1200)))
        else:
            pool.append(
                f"price: \u00a3{rng.randint(1, 999)}.{rng.randint(0, 99):02d} \u20ac{rng.randint(1, 999)}"
            )

    return pool


def _scalar_value(rng: random.Random, str_pool: list[str]) -> object:
    """Generate a random scalar value (no recursion)."""
    kind = rng.randint(0, 7)
    if kind <= 1:
        return rng.choice(str_pool)
    elif kind == 2:
        return rng.randint(-1000, 1000)
    elif kind == 3:
        return rng.randint(-(10**15), 10**15)
    elif kind == 4:
        return round(rng.uniform(-1000, 1000), rng.randint(1, 8))
    elif kind == 5:
        return rng.uniform(-1e-8, 1e-8)
    elif kind == 6:
        return rng.choice([True, False])
    else:
        return None


def _make_nested_obj(
    rng: random.Random, key_pool: list[str], str_pool: list[str], depth: int
) -> dict:
    """Generate a nested object with bounded depth."""
    if depth <= 0:
        return {rng.choice(key_pool): _scalar_value(rng, str_pool)}

    n_keys = rng.randint(2, 6)
    obj = {}
    for _ in range(n_keys):
        key = rng.choice(key_pool)
        sub = rng.randint(0, 4)
        if sub == 0 and depth > 1:
            obj[key] = _make_nested_obj(rng, key_pool, str_pool, depth - 1)
        elif sub == 1:
            obj[key] = [_scalar_value(rng, str_pool) for _ in range(rng.randint(1, 5))]
        else:
            obj[key] = _scalar_value(rng, str_pool)
    return obj


def _make_record(
    rng: random.Random,
    key_pool: list[str],
    str_pool: list[str],
    record_id: int,
    extra_keys: list[str],
) -> dict:
    """Generate one diverse record for training data."""
    record: dict = {"id": record_id}

    # Searchable fields for SAX queries
    record["name"] = rng.choice(str_pool)
    record["age"] = rng.randint(1, 100)
    record["price"] = round(rng.uniform(0.01, 9999.99), 2)
    record["active"] = rng.choice([True, False])
    record["tags"] = [rng.choice(str_pool) for _ in range(rng.randint(0, 6))]

    # Nested metadata
    record["metadata"] = {
        "created": f"2024-{rng.randint(1, 12):02d}-{rng.randint(1, 28):02d}",
        "scores": [
            {"value": round(rng.uniform(0, 100), 2), "label": rng.choice(str_pool)}
            for _ in range(rng.randint(1, 4))
        ],
    }

    # Deep nested path
    record["nested"] = {"deep": {"val": rng.randint(0, 10000)}}

    # Extra fields from key pool (exercises key cache misses)
    n_extra = rng.randint(3, 15)
    for i in range(n_extra):
        key = extra_keys[(record_id * 17 + i) % len(extra_keys)]
        if key not in record:
            sub = rng.randint(0, 3)
            if sub == 0:
                record[key] = _make_nested_obj(rng, key_pool, str_pool, rng.randint(1, 4))
            elif sub == 1:
                record[key] = [_scalar_value(rng, str_pool) for _ in range(rng.randint(1, 8))]
            else:
                record[key] = _scalar_value(rng, str_pool)

    # Every 100th record: wide object (50+ keys)
    if record_id % 100 == 0:
        for i in range(50):
            k = key_pool[(record_id + i * 7) % len(key_pool)]
            if k not in record:
                record[k] = _scalar_value(rng, str_pool)

    # Every 10th record: wide float arrays (exercises Dragonbox d2d + inline formatting)
    if record_id % 10 == 0:
        record["float_values"] = [rng.random() for _ in range(100)]
        record["float_mixed"] = [rng.random() * 1000 for _ in range(50)]

    # Every 3rd record: flat-like schema (20 keys, strings/ints/floats)
    # Exercises the mixed-schema pattern that alternates between flat/nested/wide_arrays.
    if record_id % 3 == 0:
        for i in range(20):
            k = f"field_{i}"
            kind = i % 3
            if kind == 0:
                record[k] = rng.randint(-10000, 10000)
            elif kind == 1:
                record[k] = rng.random() * 1000
            else:
                record[k] = rng.choice(str_pool)

    # Every 50th record: deep nesting chain (10+ levels)
    if record_id % 50 == 0:
        chain: dict = {}
        cur = chain
        for i in range(rng.randint(8, 14)):
            k = key_pool[(record_id + i) % len(key_pool)]
            child: dict = {}
            cur[k] = child
            cur = child
        cur["leaf"] = _scalar_value(rng, str_pool)
        record["deep_chain"] = chain

    return record


def generate(out_dir: Path, target_mb: int = TARGET_MB) -> dict:
    """Generate training data files. Returns info dict."""
    out_dir.mkdir(parents=True, exist_ok=True)
    rng = random.Random(SEED)
    key_pool = _make_key_pool(rng)
    str_pool = _pre_generate_string_pool(rng, n=800)
    extra_keys = key_pool[100:]  # skip common short keys for extra fields

    json_path = out_dir / "train.json"
    ndjson_path = out_dir / "train.ndjson"

    target_bytes = target_mb * 1024 * 1024
    record_id = 0

    print(f"Generating PGO training data (~{target_mb}MB)...")

    # Stream directly to NDJSON, accumulate size
    total_bytes = 0
    with ndjson_path.open("w", encoding="utf-8") as ndf:
        # Also write JSON simultaneously
        with json_path.open("w", encoding="utf-8") as jf:
            jf.write('{"records":[\n')
            first = True

            while total_bytes < target_bytes:
                record = _make_record(rng, key_pool, str_pool, record_id, extra_keys)
                line = json.dumps(record, ensure_ascii=False)
                line_bytes = len(line.encode("utf-8"))

                # NDJSON
                ndf.write(line)
                ndf.write("\n")

                # JSON array
                if not first:
                    jf.write(",\n")
                jf.write(line)
                first = False

                total_bytes += line_bytes + 2
                record_id += 1

                if record_id % 2000 == 0:
                    mb = total_bytes / (1024 * 1024)
                    print(f"  {record_id} records, ~{mb:.1f}MB...", end="\r")

            jf.write("\n]}")

    json_size = json_path.stat().st_size
    ndjson_size = ndjson_path.stat().st_size
    print(f"  Generated {record_id} records                    ")
    print(f"  train.json:   {json_size / 1024 / 1024:.1f}MB")
    print(f"  train.ndjson: {ndjson_size / 1024 / 1024:.1f}MB")
    print(f"  Unique keys:  {len(key_pool)}")

    return {
        "json_path": str(json_path),
        "ndjson_path": str(ndjson_path),
        "num_records": record_id,
        "json_size_mb": round(json_size / 1024 / 1024, 1),
        "ndjson_size_mb": round(ndjson_size / 1024 / 1024, 1),
        "unique_keys": len(key_pool),
    }


def main() -> int:
    import argparse

    parser = argparse.ArgumentParser(description="Generate PGO training data")
    parser.add_argument("--out-dir", type=Path, default=Path("build/pgo"))
    parser.add_argument("--target-mb", type=int, default=TARGET_MB)
    args = parser.parse_args()
    generate(args.out_dir, args.target_mb)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

"""Deterministic benchmark datasets.

Seed 42 throughout, so a dataset regenerated on another machine is byte
identical and results stay comparable (docs/context/benchmarks.md).

Each tier gets the users/orders/items document in both `.json` and `.ndjson`,
plus four synthetic shapes that stress different parser and serializer paths:

flat
    Wide records of scalars: many keys, no nesting.
nested
    Deep records: the recursive paths, and key reuse down the tree.
wide_arrays
    Long homogeneous arrays: where element-loop and array fast paths show up.
mixed
    Records with different schemas interleaved, which defeats key prediction --
    the previous implementation's known weak spot.

Output goes to `benchmarks/data/generated/<tier>/`, which is gitignored.
"""

from __future__ import annotations

import argparse
import json
import random
from pathlib import Path

SEED = 42

FIRST_NAMES = ["Ada", "Grace", "Alan", "Edsger", "Barbara", "Donald", "Linus", "Ken"]
LAST_NAMES = ["Lovelace", "Hopper", "Turing", "Dijkstra", "Liskov", "Knuth", "Torvalds"]
CITIES = ["Berlin", "Lisbon", "Tokyo", "Nairobi", "Bogota", "Oslo", "Chennai"]
PRODUCTS = ["widget", "sprocket", "gizmo", "cog", "flange", "gasket"]
TAGS = ["new", "sale", "featured", "clearance", "restock", "limited"]


def _user(rng: random.Random, index: int, max_orders: int, max_items: int) -> dict:
    orders = []
    for order_index in range(rng.randint(0, max_orders)):
        items = [
            {
                "sku": f"{rng.choice(PRODUCTS)}-{rng.randint(1000, 9999)}",
                "qty": rng.randint(1, 9),
                "price": round(rng.uniform(0.99, 499.99), 2),
                "tags": rng.sample(TAGS, rng.randint(0, 3)),
            }
            for _ in range(rng.randint(1, max_items))
        ]
        orders.append(
            {
                "id": index * 1000 + order_index,
                "total": round(sum(i["qty"] * i["price"] for i in items), 2),
                "paid": rng.random() > 0.2,
                "items": items,
            },
        )

    return {
        "id": index,
        "name": f"{rng.choice(FIRST_NAMES)} {rng.choice(LAST_NAMES)}",
        "email": f"user{index}@example.com",
        "age": rng.randint(18, 88),
        "active": rng.random() > 0.15,
        "score": round(rng.uniform(0, 100), 4),
        "address": {
            "city": rng.choice(CITIES),
            "zip": f"{rng.randint(10000, 99999)}",
            "geo": {"lat": round(rng.uniform(-90, 90), 6), "lon": round(rng.uniform(-180, 180), 6)},
        },
        "tags": rng.sample(TAGS, rng.randint(0, 4)),
        "orders": orders,
        "notes": None if rng.random() < 0.3 else "lorem ipsum dolor sit amet",
    }


def _flat_record(rng: random.Random, index: int) -> dict:
    record = {"id": index}
    for field_index in range(20):
        record[f"field_{field_index}"] = rng.choice(
            [
                rng.randint(-(10**6), 10**6),
                round(rng.uniform(-1e3, 1e3), 5),
                rng.random() > 0.5,
                f"value-{rng.randint(0, 9999)}",
                None,
            ],
        )
    return record


def _nested_record(rng: random.Random, index: int, depth: int = 6) -> dict:
    node: dict = {"leaf": rng.randint(0, 1000), "name": f"n{index}"}
    for level in range(depth):
        node = {"level": level, "child": node, "score": round(rng.uniform(0, 1), 6)}
    return {"id": index, "tree": node}


def _wide_array_record(rng: random.Random, index: int) -> dict:
    return {
        "id": index,
        "ints": [rng.randint(-(10**9), 10**9) for _ in range(64)],
        "floats": [round(rng.uniform(-1e3, 1e3), 6) for _ in range(64)],
        "strings": [f"s{rng.randint(0, 999)}" for _ in range(64)],
        "flags": [rng.random() > 0.5 for _ in range(64)],
    }


def _mixed_record(rng: random.Random, index: int) -> dict:
    """One of several schemas, chosen at random: key prediction cannot settle."""
    shape = index % 4
    if shape == 0:
        return {"kind": "a", "id": index, "value": rng.randint(0, 10**6)}
    if shape == 1:
        return {"type": "b", "payload": {"x": rng.random(), "y": rng.random()}, "n": index}
    if shape == 2:
        return {"label": f"c{index}", "items": [rng.randint(0, 100) for _ in range(5)]}
    return {
        "uuid": f"{index:08x}-{rng.randint(0, 0xFFFF):04x}",
        "meta": {"created": index, "tags": rng.sample(TAGS, 2)},
        "active": rng.random() > 0.5,
    }


SHAPES = {
    "flat": _flat_record,
    "nested": _nested_record,
    "wide_arrays": _wide_array_record,
    "mixed": _mixed_record,
}


def generate(
    out_dir: Path, num_users: int, max_orders: int, max_items: int, records: int
) -> list[Path]:
    out_dir.mkdir(parents=True, exist_ok=True)
    written = []

    rng = random.Random(SEED)
    users = [_user(rng, index, max_orders, max_items) for index in range(num_users)]

    users_json = out_dir / "users.json"
    users_json.write_text(json.dumps(users, ensure_ascii=False), encoding="utf-8")
    written.append(users_json)

    users_ndjson = out_dir / "users.ndjson"
    users_ndjson.write_text(
        "".join(json.dumps(user, ensure_ascii=False) + "\n" for user in users),
        encoding="utf-8",
    )
    written.append(users_ndjson)

    for name, builder in SHAPES.items():
        shape_rng = random.Random(SEED)
        payload = [builder(shape_rng, index) for index in range(records)]
        path = out_dir / f"{name}.json"
        path.write_text(json.dumps(payload, ensure_ascii=False), encoding="utf-8")
        written.append(path)

    return written


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out-dir", required=True, type=Path)
    parser.add_argument("--num-users", type=int, required=True)
    parser.add_argument("--max-orders", type=int, required=True)
    parser.add_argument("--max-items", type=int, required=True)
    parser.add_argument("--records", type=int, required=True)
    args = parser.parse_args(argv)

    written = generate(args.out_dir, args.num_users, args.max_orders, args.max_items, args.records)
    for path in written:
        print(f"  {path}  ({path.stat().st_size / 1024:.0f} KB)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

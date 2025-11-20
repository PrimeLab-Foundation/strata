#!/usr/bin/env python3
"""
Deterministic benchmark dataset generator.

Generates two files in the output directory:
 - users.json   (single JSON document: {"users": [...]})
 - users.ndjson (one JSON object per line)

By default uses a fixed seed for determinism; override with --seed or BENCH_SEED.

Python API:
    generate_users_datasets(out_dir, num_users=5000, max_orders_per_user=10, max_items_per_order=5, seed=None)
returns a dict with paths and basic stats.
"""

from __future__ import annotations

import argparse
import json
import os
import random
from dataclasses import dataclass
from pathlib import Path

DEFAULT_SEED = 42


@dataclass
class Stats:
    total_users: int
    total_orders: int
    total_items: int


def _gen_user(i: int, rng: random.Random, max_orders: int, max_items: int) -> dict:
    age = rng.randint(18, 80)
    n_orders = rng.randint(0, max_orders)
    user = {
        "id": i,
        "name": f"user_{i}",
        "age": age,
        "metadata": {
            "created": f"2024-{(i % 12) + 1:02d}-{(i % 28) + 1:02d}",
            "tags": [f"tag{j}" for j in range(rng.randint(0, 4))],
            "nested": {"level": rng.randint(0, 10)},
        },
        "orders": [],
    }
    for j in range(n_orders):
        status = ["pending", "shipped", "cancelled"][rng.randint(0, 2)]
        n_items = rng.randint(0, max_items)
        order = {"order_id": i * 100 + j, "status": status, "items": []}
        for k in range(n_items):
            price = round(rng.uniform(0.5, 250.0), 2)
            qty = rng.randint(1, 5)
            order["items"].append({"sku": f"SKU-{i}-{j}-{k}", "qty": qty, "price": price})
        user["orders"].append(order)
    return user


def generate_users_datasets(
    out_dir: str | Path,
    *,
    num_users: int,
    max_orders_per_user: int = 10,
    max_items_per_order: int = 5,
    seed: int | None = None,
) -> dict[str, object]:
    out = Path(out_dir)
    out.mkdir(parents=True, exist_ok=True)

    # Deterministic RNG
    env_seed = os.environ.get("BENCH_SEED")
    if seed is None and env_seed:
        try:
            seed = int(env_seed)
        except Exception:
            seed = DEFAULT_SEED
    if seed is None:
        seed = DEFAULT_SEED
    rng = random.Random(seed)

    # Build all users (deterministic order)
    users = [_gen_user(i, rng, max_orders_per_user, max_items_per_order) for i in range(num_users)]

    # Compute stats
    total_orders = sum(len(u.get("orders", [])) for u in users)
    total_items = 0
    for u in users:
        for o in u.get("orders", []) or []:
            total_items += len(o.get("items", []))

    # Write single JSON document
    json_path = out / "users.json"
    json_path.write_text(json.dumps({"users": users}, ensure_ascii=False), encoding="utf-8")

    # Write NDJSON
    ndjson_path = out / "users.ndjson"
    with ndjson_path.open("w", encoding="utf-8", newline="\n") as f:
        for u in users:
            f.write(json.dumps(u, ensure_ascii=False))
            f.write("\n")

    return {
        "out_dir": str(out),
        "users_json": str(json_path),
        "users_ndjson": str(ndjson_path),
        "stats": Stats(
            total_users=num_users,
            total_orders=total_orders,
            total_items=total_items,
        ).__dict__,
        "seed": seed,
    }


def main() -> None:
    ap = argparse.ArgumentParser(description="Generate deterministic benchmark datasets")
    ap.add_argument("--out-dir", default="benchmarks/data/generated", help="Output directory")
    ap.add_argument("--num-users", type=int, default=5000)
    ap.add_argument("--max-orders", type=int, default=10, dest="max_orders")
    ap.add_argument("--max-items", type=int, default=5, dest="max_items")
    ap.add_argument(
        "--seed",
        type=int,
        default=None,
        help="Override RNG seed (default BENCH_SEED or 42)",
    )
    args = ap.parse_args()

    info = generate_users_datasets(
        args.out_dir,
        num_users=args.num_users,
        max_orders_per_user=args.max_orders,
        max_items_per_order=args.max_items,
        seed=args.seed,
    )
    print(json.dumps(info, indent=2))


if __name__ == "__main__":
    main()

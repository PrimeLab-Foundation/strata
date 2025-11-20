# Benchmark Data Notes

This file documents the dataset schema and usage expectations for the benchmark suite.

## Schema expectations

Two dataset formats are used by the suite:

1. Single JSON document (`users.json`)

```
{
  "users": [
    {
      "id": int,
      "name": str,
      "age": int,
      "metadata": {
        "created": str (YYYY-MM-DD),
        "tags": [str],
        "nested": { "level": int }
      },
      "orders": [
        {
          "order_id": int,
          "status": "pending" | "shipped" | "cancelled",
          "items": [ { "sku": str, "qty": int, "price": float } ]
        }
      ]
    },
    ...
  ]
}
```

2. NDJSON (`users.ndjson`): one JSON object per line. Each line contains a single
   `user` object with the same shape as elements of `users` above.

## Queries used by the suite

The benchmark orchestrator exercises five canonical queries. For `users.json`:

- `$.users[*].id` — simple projection of user IDs
- `$.users[*].orders[*].items[*].price` — deep traversal to item prices
- `$..price` — recursive descent of all `price` fields
- `$.users[?(@.age>30)]` — filter users older than 30
- `$..orders[?(@.status=="shipped")]` — recursively find orders with status shipped

For `users.ndjson`, those are mapped to equivalent record-root queries:

- `$.id`
- `$.orders[*].items[*].price`
- `$..price`
- `$[?(@.age>30)]` (filter over a synthetic array of records)
- `$..orders[?(@.status=="shipped")]`

These queries are implemented in `benchmarks/eval_queries.py` for Python-side
evaluation when needed (e.g., for libraries without a JSONPath engine).

## Dataset sizes and roles

- Small/Quick runs: ~5,000 users (used by CI and local sanity checks)
- Medium/Large: previously used for stress tests; now we generate datasets on demand.

The new generator produces deterministic datasets by default and places them under
`benchmarks/data/generated/`. The orchestrator will auto-generate the default small
dataset when missing or when `--regen-data` is provided.

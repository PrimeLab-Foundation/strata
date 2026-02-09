#!/usr/bin/env python3
"""
Query evaluation helpers for benchmarking.

This module provides Python-side evaluation of the canonical queries
for libraries without search engines. Used to ensure fair comparisons.
"""

from __future__ import annotations

from typing import Any


def query_users_json(parsed_doc: dict, query_id: int) -> list[Any]:
    """
    Evaluate canonical queries on users.json format.

    Args:
        parsed_doc: Parsed JSON document (dict with "users" key)
        query_id: Query identifier (1-5)

    Returns:
        List of matching results
    """
    users = parsed_doc.get("users", [])

    if query_id == 1:
        # $.users[*].id - simple projection
        return [u["id"] for u in users if "id" in u]

    elif query_id == 2:
        # $.users[*].orders[*].items[*].price - deep traversal
        results = []
        for u in users:
            for order in u.get("orders", []):
                for item in order.get("items", []):
                    if "price" in item:
                        results.append(item["price"])
        return results

    elif query_id == 3:
        # $..price - recursive descent
        results = []

        def collect_prices(obj: Any) -> None:
            if isinstance(obj, dict):
                for k, v in obj.items():
                    if k == "price":
                        results.append(v)
                    collect_prices(v)
            elif isinstance(obj, list):
                for item in obj:
                    collect_prices(item)

        collect_prices(parsed_doc)
        return results

    elif query_id == 4:
        # $.users[?(@.age>30)] - filter users older than 30
        return [u for u in users if u.get("age", 0) > 30]

    elif query_id == 5:
        # $..orders[?(@.status=="shipped")] - recursive filter
        results = []

        def collect_shipped_orders(obj: Any) -> None:
            if isinstance(obj, dict):
                if obj.get("status") == "shipped":
                    results.append(obj)
                for v in obj.values():
                    collect_shipped_orders(v)
            elif isinstance(obj, list):
                for item in obj:
                    if isinstance(item, dict) and item.get("status") == "shipped":
                        results.append(item)
                    collect_shipped_orders(item)

        collect_shipped_orders(parsed_doc)
        return results

    else:
        raise ValueError(f"Unknown query_id: {query_id}")


def query_users_ndjson(parsed_record: dict, query_id: int) -> list[Any]:
    """
    Evaluate canonical queries on users.ndjson format (per-record).

    Args:
        parsed_record: Single user record
        query_id: Query identifier (1-5)

    Returns:
        List of matching results (may be empty)
    """
    if query_id == 1:
        # $.id
        return [parsed_record["id"]] if "id" in parsed_record else []

    elif query_id == 2:
        # $.orders[*].items[*].price
        results = []
        for order in parsed_record.get("orders", []):
            for item in order.get("items", []):
                if "price" in item:
                    results.append(item["price"])
        return results

    elif query_id == 3:
        # $..price - recursive descent
        results = []

        def collect_prices(obj: Any) -> None:
            if isinstance(obj, dict):
                for k, v in obj.items():
                    if k == "price":
                        results.append(v)
                    collect_prices(v)
            elif isinstance(obj, list):
                for item in obj:
                    collect_prices(item)

        collect_prices(parsed_record)
        return results

    elif query_id == 4:
        # $[?(@.age>30)]
        return [parsed_record] if parsed_record.get("age", 0) > 30 else []

    elif query_id == 5:
        # $..orders[?(@.status=="shipped")]
        results = []

        def collect_shipped_orders(obj: Any) -> None:
            if isinstance(obj, dict):
                if obj.get("status") == "shipped":
                    results.append(obj)
                for v in obj.values():
                    collect_shipped_orders(v)
            elif isinstance(obj, list):
                for item in obj:
                    if isinstance(item, dict) and item.get("status") == "shipped":
                        results.append(item)
                    collect_shipped_orders(item)

        collect_shipped_orders(parsed_record)
        return results

    else:
        raise ValueError(f"Unknown query_id: {query_id}")


def get_query_description(query_id: int, is_ndjson: bool = False) -> str:
    """Get human-readable query description."""
    if is_ndjson:
        descriptions = {
            1: "$.id",
            2: "$.orders[*].items[*].price",
            3: "$..price",
            4: "$[?(@.age>30)]",
            5: '$..orders[?(@.status=="shipped")]',
        }
    else:
        descriptions = {
            1: "$.users[*].id",
            2: "$.users[*].orders[*].items[*].price",
            3: "$..price",
            4: "$.users[?(@.age>30)]",
            5: '$..orders[?(@.status=="shipped")]',
        }
    return descriptions.get(query_id, f"Unknown query {query_id}")

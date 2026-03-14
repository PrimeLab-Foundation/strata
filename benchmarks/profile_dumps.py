"""Micro-benchmark: isolate each serialization component for dumps/dump optimization.

Tests strata vs orjson vs msgspec on isolated data types to find the bottleneck.
"""

import gc
import time
import json
import sys
import os

# Ensure strata is importable
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import strata
import orjson
import msgspec

REPEAT = 20
WARMUP = 5


def bench(name, fn, repeat=REPEAT, warmup=WARMUP):
    for _ in range(warmup):
        fn()
    gc.disable()
    times = []
    for _ in range(repeat):
        t0 = time.perf_counter_ns()
        fn()
        times.append(time.perf_counter_ns() - t0)
    gc.enable()
    times.sort()
    min_ns = times[0]
    med_ns = times[len(times) // 2]
    return min_ns, med_ns


def fmt(ns):
    if ns < 1000:
        return f"{ns}ns"
    if ns < 1_000_000:
        return f"{ns / 1000:.1f}us"
    return f"{ns / 1_000_000:.3f}ms"


def run_category(label, data):
    print(f"\n{'=' * 60}")
    print(f"  {label}")
    print(f"{'=' * 60}")

    # Pre-encode for orjson/msgspec
    strata_fn = lambda: strata.dumps(data)
    orjson_fn = lambda: orjson.dumps(data)
    msgspec_fn = lambda: msgspec.json.encode(data)
    json_fn = lambda: json.dumps(data)

    results = []
    for lib, fn in [
        ("strata", strata_fn),
        ("orjson", orjson_fn),
        ("msgspec", msgspec_fn),
        ("json", json_fn),
    ]:
        min_ns, med_ns = bench(lib, fn)
        results.append((lib, min_ns, med_ns))

    results.sort(key=lambda x: x[1])
    best = results[0][1]
    for lib, min_ns, med_ns in results:
        ratio = min_ns / best if best > 0 else 0
        marker = " <-- WINNER" if min_ns == best else ""
        gap = f"  ({ratio:.2f}x)" if min_ns != best else ""
        print(f"  {lib:>10s}: min={fmt(min_ns):>10s}  med={fmt(med_ns):>10s}{gap}{marker}")

    return results


def main():
    print("Strata dumps() Component Profiler")
    print(f"Python {sys.version}")
    print(f"Repeat={REPEAT}, Warmup={WARMUP}")

    # 1. Short strings (keys, typical values)
    short_strings = {f"key_{i}": f"value_{i}" for i in range(1000)}
    run_category("1. Short strings (1000 key-value pairs, ~5-10 chars each)", short_strings)

    # 2. Long strings (256 chars each, no escapes)
    long_str = "a" * 256
    long_strings = {f"k{i}": long_str for i in range(1000)}
    run_category("2. Long strings (1000 x 256-char values, no escapes)", long_strings)

    # 3. Strings with escapes
    escape_str = 'hello "world"\nfoo\\bar\ttab'
    escape_strings = {f"k{i}": escape_str for i in range(1000)}
    run_category(
        "3. Strings with escapes (1000 x 25-char values with \" \\\\ \\n \\t)", escape_strings
    )

    # 4. Small integers
    small_ints = {f"k{i}": i for i in range(1000)}
    run_category("4. Small integers (1000 ints, 0-999)", small_ints)

    # 5. Large integers
    large_ints = {f"k{i}": 1234567890 + i for i in range(1000)}
    run_category("5. Large integers (1000 ints, ~10 digits)", large_ints)

    # 6. Floats
    floats = {f"k{i}": 3.14159265 + i * 0.001 for i in range(1000)}
    run_category("6. Floats (1000 non-integer doubles)", floats)

    # 7. Integer-valued floats
    int_floats = {f"k{i}": float(i) for i in range(1000)}
    run_category("7. Integer-valued floats (1000 x float(i))", int_floats)

    # 8. Booleans/None
    bools = {f"k{i}": True if i % 3 == 0 else (False if i % 3 == 1 else None) for i in range(1000)}
    run_category("8. Booleans/None (1000 mixed true/false/null)", bools)

    # 9. Nested dicts (depth)
    def make_nested(depth):
        d = {"value": 42}
        for i in range(depth):
            d = {f"level_{i}": d}
        return d

    nested = make_nested(50)
    run_category("9. Deeply nested dict (50 levels)", nested)

    # 10. Flat list of ints
    flat_list = list(range(10000))
    run_category("10. Flat list of 10000 ints", flat_list)

    # 11. List of small dicts (typical JSON data)
    users = [
        {"id": i, "name": f"user_{i}", "age": 20 + i % 50, "active": i % 2 == 0}
        for i in range(1000)
    ]
    run_category("11. List of 1000 small dicts (id/name/age/active)", users)

    # 12. Realistic: the benchmark dataset shape
    # Mimics users.json structure
    realistic = {
        "users": [
            {
                "id": i,
                "name": f"User {i}",
                "email": f"user{i}@example.com",
                "age": 20 + i % 50,
                "address": {"street": f"{i} Main St", "city": "Anytown", "zip": f"{10000 + i}"},
                "orders": [
                    {
                        "order_id": i * 10 + j,
                        "status": ["pending", "shipped", "delivered"][j % 3],
                        "items": [
                            {"product": f"Product {k}", "price": 9.99 + k, "qty": k + 1}
                            for k in range(3)
                        ],
                    }
                    for j in range(3)
                ],
            }
            for i in range(100)
        ]
    }
    run_category("12. Realistic nested data (100 users with orders/items)", realistic)

    # 13. Empty containers
    empties = {"a": {}, "b": [], "c": {"d": {}, "e": []}}
    run_category("13. Empty containers", empties)

    # 14. PyUnicode_FromStringAndSize overhead: measure raw serialization vs full dumps
    print(f"\n{'=' * 60}")
    print(f"  14. Buffer-to-PyObject overhead (dumps output size)")
    print(f"{'=' * 60}")
    data = realistic
    result = strata.dumps(data)
    print(f"  Output size: {len(result)} bytes")
    result_bytes = strata.dumps(data, return_type='bytes')
    print(f"  Output size (bytes): {len(result_bytes)} bytes")

    str_min, str_med = bench("str", lambda: strata.dumps(data))
    bytes_min, bytes_med = bench("bytes", lambda: strata.dumps(data, return_type='bytes'))
    print(f"  dumps(return_type='str'):   min={fmt(str_min):>10s}  med={fmt(str_med):>10s}")
    print(f"  dumps(return_type='bytes'): min={fmt(bytes_min):>10s}  med={fmt(bytes_med):>10s}")
    diff = str_min - bytes_min
    print(
        f"  PyUnicode overhead: ~{fmt(abs(diff))} ({'str slower' if diff > 0 else 'bytes slower'})"
    )

    # 15. GC impact measurement
    print(f"\n{'=' * 60}")
    print(f"  15. GC impact on dumps()")
    print(f"{'=' * 60}")
    data = realistic
    gc.collect()
    gc.disable()
    no_gc_min, no_gc_med = bench("no_gc", lambda: strata.dumps(data))
    gc.enable()
    gc.collect()
    gc_min, gc_med = bench("with_gc", lambda: strata.dumps(data))
    print(f"  GC disabled: min={fmt(no_gc_min):>10s}  med={fmt(no_gc_med):>10s}")
    print(f"  GC enabled:  min={fmt(gc_min):>10s}  med={fmt(gc_med):>10s}")
    print(f"  GC overhead: ~{fmt(gc_min - no_gc_min)}")


if __name__ == "__main__":
    main()

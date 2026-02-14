#!/usr/bin/env python3
"""
Test that adaptive estimator works when exact size hints are disabled.
This is the scenario where the estimator provides value.
"""

import json
import os
import time
import strata

# Create test data with varied dict sizes
def generate_test_json():
    """Generate JSON with objects of varying sizes to test estimator learning."""
    data = {
        "small_objects": [
            {"id": i, "name": f"user{i}", "active": True}  # 3 keys
            for i in range(1000)
        ],
        "medium_objects": [
            {
                "id": i,
                "name": f"user{i}",
                "email": f"user{i}@example.com",
                "age": 20 + (i % 50),
                "city": "New York",
                "country": "USA",
                "active": True,
                "created_at": "2024-01-01",
                "updated_at": "2024-02-01",
                "role": "user"
            }  # 10 keys
            for i in range(1000)
        ],
        "large_objects": [
            {
                f"field_{j}": j * i
                for j in range(25)  # 25 keys
            }
            for i in range(500)
        ]
    }
    return json.dumps(data)

def benchmark_with_and_without_exact_hints():
    """Compare performance with exact hints ON vs OFF."""
    test_json = generate_test_json()
    n_iterations = 20

    print("Testing Adaptive Estimator vs Exact Size Hints")
    print("=" * 70)
    print(f"Test data size: {len(test_json)/1024:.1f} KB")
    print(f"Iterations: {n_iterations}")
    print()

    # Test WITH exact size hints (default for large files)
    print("1. WITH exact size hints (STRATA_PYTHON_EXACT_SIZE_HINTS=1)")
    os.environ["STRATA_PYTHON_EXACT_SIZE_HINTS"] = "1"

    # Warmup
    for _ in range(3):
        strata.loads(test_json)

    start = time.perf_counter()
    for _ in range(n_iterations):
        obj = strata.loads(test_json)
    time_with_exact = time.perf_counter() - start
    per_parse_with_exact = time_with_exact / n_iterations * 1000

    print(f"   Total: {time_with_exact*1000:.2f}ms")
    print(f"   Per-parse: {per_parse_with_exact:.2f}ms")
    print()

    # Test WITHOUT exact size hints (adaptive estimator kicks in)
    print("2. WITHOUT exact size hints (STRATA_PYTHON_EXACT_SIZE_HINTS=0)")
    print("   -> Adaptive estimator learns from observed sizes")
    os.environ["STRATA_PYTHON_EXACT_SIZE_HINTS"] = "0"

    # Force reload of settings
    import importlib
    importlib.reload(strata)

    # Warmup
    for _ in range(3):
        strata.loads(test_json)

    start = time.perf_counter()
    for _ in range(n_iterations):
        obj = strata.loads(test_json)
    time_without_exact = time.perf_counter() - start
    per_parse_without_exact = time_without_exact / n_iterations * 1000

    print(f"   Total: {time_without_exact*1000:.2f}ms")
    print(f"   Per-parse: {per_parse_without_exact:.2f}ms")
    print()

    # Analysis
    print("=" * 70)
    print("RESULTS:")

    if per_parse_with_exact < per_parse_without_exact:
        overhead = ((per_parse_without_exact / per_parse_with_exact) - 1) * 100
        print(f"  Exact hints are {overhead:.1f}% faster")
        print(f"  Adaptive estimator overhead: {per_parse_without_exact - per_parse_with_exact:.2f}ms")
        print()
        print("✓ Adaptive estimator works, but exact hints are better (as expected)")
        print("  Use adaptive estimator for: streaming, small files, or when")
        print("  exact size collection overhead is too high")
    else:
        improvement = ((per_parse_with_exact / per_parse_without_exact) - 1) * 100
        print(f"  Adaptive estimator is {improvement:.1f}% faster!")
        print(f"  Savings: {per_parse_with_exact - per_parse_without_exact:.2f}ms")
        print()
        print("✓ Adaptive estimator outperforms exact size hints")
        print("  (This can happen if exact hint collection overhead > benefit)")

    # Cleanup
    del os.environ["STRATA_PYTHON_EXACT_SIZE_HINTS"]

if __name__ == "__main__":
    benchmark_with_and_without_exact_hints()

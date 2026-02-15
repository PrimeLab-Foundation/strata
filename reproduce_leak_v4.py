import strata
import os
import psutil
import gc
import json

def get_rss():
    process = psutil.Process(os.getpid())
    return process.memory_info().rss / (1024 * 1024)

def run_test(json_text, path, iterations=1000):
    print(f"Initial RSS: {get_rss():.2f} MB")

    # Create document once
    cursor = strata._native.parse_json(json_text)

    # Warm up
    for _ in range(50):
        strata.search(cursor, path)

    gc.collect()
    start_rss = get_rss()
    print(f"Starting test at RSS: {start_rss:.2f} MB")

    for i in range(iterations):
        res = strata.search(cursor, path)
        if i % 100 == 0:
            gc.collect()
            print(f"Iteration {i}, RSS: {get_rss():.2f} MB, results: {len(res)}")

    gc.collect()
    end_rss = get_rss()
    print(f"Final RSS: {end_rss:.2f} MB")
    print(f"Growth: {end_rss - start_rss:.2f} MB")

if __name__ == "__main__":
    # Create a JSON with 10k items
    data = {
        "items": [{"id": i, "val": "value" + str(i)} for i in range(10000)]
    }
    json_text = json.dumps(data)
    path = "$.items[*]"

    run_test(json_text, path, 1000)

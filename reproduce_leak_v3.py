import strata
import os
import psutil
import gc
import json

def get_rss():
    process = psutil.Process(os.getpid())
    return process.memory_info().rss / (1024 * 1024)

def run_test(json_text, path, iterations=500):
    print(f"Initial RSS: {get_rss():.2f} MB")

    # Warm up
    for _ in range(50):
        strata.search(json_text, path)

    gc.collect()
    start_rss = get_rss()
    print(f"Starting test at RSS: {start_rss:.2f} MB")

    for i in range(iterations):
        res = strata.search(json_text, path)
        if i % 50 == 0:
            gc.collect()
            print(f"Iteration {i}, RSS: {get_rss():.2f} MB, results: {len(res)}")

    gc.collect()
    end_rss = get_rss()
    print(f"Final RSS: {end_rss:.2f} MB")
    print(f"Growth: {end_rss - start_rss:.2f} MB")

if __name__ == "__main__":
    # Create a JSON with 100k items to return in search
    data = {
        "items": [{"id": i, "val": "value" + str(i)} for i in range(100000)]
    }
    json_text = json.dumps(data)
    path = "$.items[*]"

    run_test(json_text, path, 200)

import strata
import os
import psutil
import gc
import json

def get_rss():
    process = psutil.Process(os.getpid())
    return process.memory_info().rss / (1024 * 1024)

def run_test(json_text, iterations=2000):
    print(f"Initial RSS: {get_rss():.2f} MB")

    # Warm up
    for _ in range(100):
        strata.loads(json_text)

    gc.collect()
    start_rss = get_rss()
    print(f"Starting test at RSS: {start_rss:.2f} MB")

    path = "$.nested.key250"
    for i in range(iterations):
        res = strata.search(json_text, path)
        if i % 200 == 0:
            gc.collect()
            print(f"Iteration {i}, RSS: {get_rss():.2f} MB")

    gc.collect()
    end_rss = get_rss()
    print(f"Final RSS: {end_rss:.2f} MB")
    print(f"Growth: {end_rss - start_rss:.2f} MB")

if __name__ == "__main__":
    # Create a reasonably large and complex JSON with MANY matching items
    data = {
        "id": 1,
        "name": "test",
        "items": [{"price": i} for i in range(10000)],
        "nested": {"key" + str(i): i for i in range(500)}
    }
    json_text = json.dumps(data)

    path = "$.items[*].price"
    run_test(json_text, 1000)

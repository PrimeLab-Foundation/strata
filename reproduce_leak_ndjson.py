import strata
import os
import psutil
import gc
import json

def get_rss():
    process = psutil.Process(os.getpid())
    return process.memory_info().rss / (1024 * 1024)

def run_test(ndjson_text, path, iterations=100):
    print(f"Initial RSS: {get_rss():.2f} MB")

    # Warm up
    for _ in range(10):
        strata.search(ndjson_text, path, ndjson=True)

    gc.collect()
    start_rss = get_rss()
    print(f"Starting test at RSS: {start_rss:.2f} MB")

    for i in range(iterations):
        res = strata.search(ndjson_text, path, ndjson=True)
        if i % 10 == 0:
            gc.collect()
            print(f"Iteration {i}, RSS: {get_rss():.2f} MB, results: {len(res)}")

    gc.collect()
    end_rss = get_rss()
    print(f"Final RSS: {end_rss:.2f} MB")
    print(f"Growth: {end_rss - start_rss:.2f} MB")

if __name__ == "__main__":
    # Create an NDJSON with 5000 records
    lines = []
    for i in range(5000):
        lines.append(json.dumps({"id": i, "val": "value" + str(i), "tags": ["a", "b", "c"]}))
    ndjson_text = "\n".join(lines)
    path = "$.tags[*]"

    run_test(ndjson_text, path, 100)

"""Spin one library's dumps over mixed.json — the body a profiler wraps.

Used by .github/workflows/profile.yml: `perf stat` needs a process that does
nothing but the operation under study, so the counter totals are attributable.
Prints one timing line so the wrapped run also documents what it measured.
"""

import json
import sys
import time


def main() -> int:
    library = sys.argv[1]
    iterations = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    with open("benchmarks/data/generated/small/mixed.json") as handle:
        data = json.load(handle)

    if library == "strata":
        import strata

        call = lambda: strata.dumps(data, return_type="bytes")  # noqa: E731
    elif library == "orjson":
        import orjson

        call = lambda: orjson.dumps(data)  # noqa: E731
    else:
        raise SystemExit(f"unknown library: {library}")

    call()  # warm caches and lazy state outside the measured span
    start = time.perf_counter()
    for _ in range(iterations):
        call()
    elapsed = time.perf_counter() - start
    print(f"{library}: {iterations} iterations, {elapsed * 1e6 / iterations:.2f} us/call")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

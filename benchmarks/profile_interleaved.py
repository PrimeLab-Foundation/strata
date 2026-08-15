"""Spin dumps mixed under the tier harness's own condition: five libraries
round-robin with a gc.collect() before every call.

Used by .github/workflows/profile.yml. A perf record of this process shows
strata's frames alongside the other engines'; comparing strata's *relative*
frame shares here against the isolated spin names the code that pays for the
interleave — the frame whose share grows is the predictor/cache victim.
"""

import gc
import json
import sys
import time


def main() -> int:
    rounds = int(sys.argv[1]) if len(sys.argv) > 1 else 800
    with open("benchmarks/data/generated/small/mixed.json") as handle:
        data = json.load(handle)

    import msgspec
    import orjson
    import ujson

    import strata

    encode = msgspec.json.encode
    calls = (
        lambda: strata.dumps(data, return_type="bytes"),
        lambda: orjson.dumps(data),
        lambda: ujson.dumps(data),
        lambda: encode(data),
        lambda: json.dumps(data),
    )
    for call in calls:
        call()

    start = time.perf_counter()
    for _ in range(rounds):
        for call in calls:
            gc.collect()
            call()
    elapsed = time.perf_counter() - start
    print(f"interleaved: {rounds} rounds x 5 libraries, {elapsed:.2f}s total")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

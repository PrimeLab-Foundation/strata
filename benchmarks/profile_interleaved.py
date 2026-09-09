"""Spin dumps mixed under the tier harness's own condition: five libraries
round-robin with a gc.collect() before every call.

Used by .github/workflows/profile.yml. A perf record of this process shows
strata's frames alongside the other engines'. The resident control warms all
engines but then invokes only Strata. Changes in relative frame shares are
descriptive evidence, not proof of a cache or predictor bottleneck.
"""

import argparse
import gc
import json
import time
from pathlib import Path


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("rounds", type=int, nargs="?", default=800)
    parser.add_argument("--condition", choices=("interleaved", "resident"), default="interleaved")
    parser.add_argument("--metadata", type=Path)
    args = parser.parse_args(argv)
    if args.rounds < 1:
        parser.error("rounds must be positive")
    rounds = args.rounds
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

    if args.metadata:
        from benchmarks.provenance import capture

        provenance = capture(
            [Path("benchmarks/data/generated/small/mixed.json")],
            {"strata": strata, "orjson": orjson, "msgspec": msgspec, "ujson": ujson, "json": json},
            repeat=rounds,
            warmup=1,
        )
        provenance["protocol"] = {
            "name": "mixed-native-profile-v1",
            "condition": args.condition,
            "rounds": rounds,
            "warmup": "one call to each encoder",
            "gc": "collect before each call",
            "loop_order": ["strata"]
            if args.condition == "resident"
            else ["strata", "orjson", "ujson", "msgspec", "json"],
        }
        args.metadata.write_text(json.dumps(provenance, indent=2) + "\n")

    if args.condition == "resident":
        calls = calls[:1]

    start = time.perf_counter()
    for _ in range(rounds):
        for call in calls:
            gc.collect()
            call()
    elapsed = time.perf_counter() - start
    print(f"{args.condition}: {rounds} rounds x {len(calls)} libraries, {elapsed:.2f}s total")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

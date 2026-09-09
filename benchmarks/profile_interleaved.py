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

    from benchmarks.bench_main import _dumps_callables

    libraries = {
        "strata": strata,
        "orjson": orjson,
        "msgspec": msgspec,
        "ujson": ujson,
        "json": json,
    }
    calls = _dumps_callables(libraries, data)
    for call in calls.values():
        call()

    if args.metadata:
        from benchmarks.provenance import capture

        provenance = capture(
            [Path("benchmarks/data/generated/small/mixed.json")],
            libraries,
            repeat=rounds,
            warmup=1,
        )
        provenance["protocol"] = {
            "name": "mixed-native-profile-v1",
            "condition": args.condition,
            "rounds": rounds,
            "warmup": "one call to each encoder",
            "gc": "collect before each call",
            "loop_order": ["strata"] if args.condition == "resident" else list(calls),
        }
        args.metadata.write_text(json.dumps(provenance, indent=2) + "\n")

    if args.condition == "resident":
        calls = {"strata": calls["strata"]}

    start = time.perf_counter()
    for _ in range(rounds):
        for call in calls.values():
            gc.collect()
            call()
    elapsed = time.perf_counter() - start
    print(f"{args.condition}: {rounds} rounds x {len(calls)} libraries, {elapsed:.2f}s total")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

"""Value-preserving string sharing controls, not a production optimization.

Pooling equal string values changes object identity and memory layout while
preserving JSON. A difference can implicate object layout/cache effects but
cannot attribute them to a particular cache or instruction. Fixture creation
is outside timing; this does not justify interning user data in production.
"""

from __future__ import annotations

import argparse
from dataclasses import asdict
import json
from pathlib import Path

import strata

from benchmarks import ab_rounds
from benchmarks.provenance import capture


def string_leaves(value):
    if isinstance(value, str):
        yield value
    elif isinstance(value, dict):
        for child in value.values():
            yield from string_leaves(child)
    elif isinstance(value, list):
        for child in value:
            yield from string_leaves(child)


def pool_strings(value, pool):
    if isinstance(value, str):
        return pool.setdefault(value, value)
    if isinstance(value, dict):
        return {key: pool_strings(child, pool) for key, child in value.items()}
    if isinstance(value, list):
        return [pool_strings(child, pool) for child in value]
    return value


def main(argv=None):
    import orjson

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dataset", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--repeat", type=int, default=60)
    args = parser.parse_args(argv)
    if args.repeat < 10:
        parser.error("repeat must be at least 10")
    original = json.loads(args.dataset.read_bytes())

    # A separate recursive clone retains every original string identity.
    def clone(value):
        if isinstance(value, dict):
            return {k: clone(v) for k, v in value.items()}
        if isinstance(value, list):
            return [clone(v) for v in value]
        return value

    strings = list(string_leaves(original))
    fixtures = {
        "tree-original": original,
        "tree-cloned": clone(original),
        "tree-pooled": pool_strings(original, {}),
        "strings-original": strings,
        "strings-pooled": pool_strings(strings, {}),
    }
    encoders = {"strata": lambda v: strata.dumps(v, return_type="bytes"), "orjson": orjson.dumps}
    calls = {}
    metadata = {}
    for name, value in fixtures.items():
        leaves = list(string_leaves(value))
        metadata[name] = {
            "string_count": len(leaves),
            "distinct_values": len(set(leaves)),
            "distinct_objects": len({id(v) for v in leaves}),
        }
        reference = original if name.startswith("tree-") else fixtures["strings-original"]
        for engine, encode in encoders.items():
            if encode(value) != encode(reference):
                raise ValueError(f"{name}/{engine}: pooling changed output bytes")
            calls[f"{name}/{engine}"] = lambda value=value, encode=encode: encode(value)
    provenance = capture(
        [args.dataset], {"strata": strata, "orjson": orjson}, repeat=args.repeat, warmup=2
    )
    provenance["protocol"] = {
        "name": "string-identity-controls-v1",
        "repeat": args.repeat,
        "warmup": 2,
        "order": list(calls),
        "ordering": "listed order on even rounds, reversed on odd rounds",
        "gc": "collect before each timed call",
        "timer": "perf_counter_ns converted to ms",
        "rss": "not collected",
    }
    samples = ab_rounds.alternating_rounds(calls, repeat=args.repeat)
    effects = {}
    for engine in encoders:
        for scope, baseline in (("tree", "cloned"), ("strings", "original")):
            ratio = ab_rounds.paired_ratio(
                samples[f"{scope}-pooled/{engine}"], samples[f"{scope}-{baseline}/{engine}"]
            )
            effects[f"{scope}/{engine}"] = asdict(ratio)
            print(f"{scope}/{engine} pooled/{baseline}: {ratio}")
    packet = {
        "schema_version": 1,
        "provenance": provenance,
        "fixtures": metadata,
        "samples_ms": samples,
        "effects": effects,
        "limitation": __doc__,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(packet, indent=2, allow_nan=False) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

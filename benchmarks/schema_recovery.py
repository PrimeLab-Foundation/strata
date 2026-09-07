"""Diagnostic: stable serialization before/after unrelated schema churn.

Each condition gets a fresh process. These are workload-history controls,
not canonical standings or a replacement for build-paired ABBA measurements.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import subprocess
import sys
from pathlib import Path

from benchmarks import ab_rounds


def worker(condition: str, repeat: int) -> dict:
    import orjson
    import strata

    from benchmarks.provenance import capture

    records = [{"record_id": i, "label": f"record-{i}", "enabled": i % 2 == 0} for i in range(500)]
    expected = orjson.dumps(records)
    for _ in range(3):
        assert strata.dumps(records, return_type="bytes") == expected
    if condition == "churn":
        strata.dumps([{f"unrelated_{i}": i} for i in range(200)])
    elif condition == "separate-calls":
        for i in range(200):
            strata.dumps([{f"unrelated_{i}": i}])
    samples = ab_rounds.alternating_rounds(
        {
            "strata": lambda: strata.dumps(records, return_type="bytes"),
            "orjson": lambda: orjson.dumps(records),
        },
        repeat=repeat,
        preamble=ab_rounds.collect,
    )
    assert strata.dumps(records, return_type="bytes") == expected
    provenance = capture([], {"strata": strata, "orjson": orjson}, repeat=repeat, warmup=3)
    provenance["protocol"] = {
        "name": "schema-recovery-diagnostic-v1",
        "repeat": repeat,
        "setup": "three Strata warmup calls, then condition-specific churn",
        "engine_warmup_after_churn": 2,
        "gc": "collect before each timed call",
        "order": "alternating engines",
        "timer": "perf_counter_ns converted to milliseconds",
    }
    return {
        "condition": condition,
        "samples": samples,
        "provenance": provenance,
        "payload_sha256": hashlib.sha256(expected).hexdigest(),
        "units": "milliseconds",
        "scope": "schema-recovery-diagnostic-v1",
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path)
    parser.add_argument("--repeat", type=int, default=60)
    parser.add_argument("--worker", choices=("warm", "churn", "separate-calls"))
    args = parser.parse_args()
    if args.repeat < 1:
        parser.error("repeat must be positive")
    if args.worker:
        print(json.dumps(worker(args.worker, args.repeat)))
        return 0
    if args.output is None:
        parser.error("--output is required")
    packets = []
    for condition in ("warm", "churn", "separate-calls"):
        result = subprocess.run(
            [
                sys.executable,
                "-m",
                "benchmarks.schema_recovery",
                "--worker",
                condition,
                "--repeat",
                str(args.repeat),
            ],
            check=True,
            capture_output=True,
            text=True,
        )
        packets.append(json.loads(result.stdout))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(packets, indent=2, allow_nan=False) + "\n")
    print(f"wrote {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

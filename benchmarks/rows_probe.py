"""One process, one build: named rows across tiers and operations.

`benchmarks/dumps_rows_probe.py` is the P0 unit of work and it is kept
verbatim; this is the same probe generalised in exactly two ways that the
E26-P2 timing window needs and P0 did not:

* **a row names its tier**, so `small dumps flat` and `medium dumps flat` can
  be measured in the *same* launch (the P0 probe takes one `--tier` for the
  whole process, which would have forced two separate A-B-B-A sessions and
  thrown away the pairing between them);
* **a row names its operation**, so `loads` rows can ride in the same launch
  as `dumps` rows. `loads flat` is a required control here because the G->T
  profile change moves `parse_array` by -7,524 bytes: a build change that
  helps `dumps` and hurts `loads` is a trade, not a win.

Everything else is P0's: `ab_rounds.alternating_rounds`, the `gc.collect()`
preamble *outside* the timed span, order alternating every round, one TSV line
per sample on stdout, the same `ab_rounds.TSV_HEADER` — so
`benchmarks/ab_builds.py --analyze` and `benchmarks/ab_floor.py` read this
tool's output without modification.

The calls are the official rows' calls, copied from `benchmarks/bench_main.py`:
`dumps` is `strata.dumps(value, return_type="bytes")` against
`orjson.dumps(value)` (plus strata's `str` form), and `loads` is
`strata.loads(payload_bytes)` against `orjson.loads(payload_bytes)`.

usage: rows_probe.py --build <tag> --row <tier>:<dataset>:<op> [--row ...]
                     [--repeat 60] [--warmup 2] [--header] [--identify]
"""

from __future__ import annotations

import argparse
import json
import os
import sys
import time
from pathlib import Path

from benchmarks import ab_rounds

PROJECT_ROOT = Path(__file__).resolve().parents[1]

TIER_ABBREVIATION = {"small": "sm", "medium": "md", "large": "lg"}


def _label(tier: str, dataset: str, op: str) -> str:
    return f"{TIER_ABBREVIATION.get(tier, tier)}-{dataset}-{op}"


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", required=True, help="tag naming the extension under test")
    parser.add_argument("--tag", default="", help="free-form tag for this launch")
    parser.add_argument(
        "--row",
        action="append",
        required=True,
        help="tier:dataset:op, e.g. small:flat:dumps or medium:flat:loads",
    )
    parser.add_argument("--repeat", type=int, default=60)
    parser.add_argument("--warmup", type=int, default=2)
    parser.add_argument("--header", action="store_true")
    parser.add_argument("--identify", action="store_true")
    args = parser.parse_args(argv)

    import orjson

    import strata

    if args.identify:
        import hashlib

        import strata._strata as native

        digest = hashlib.md5(Path(native.__file__).read_bytes()).hexdigest()  # noqa: S324
        one, five, fifteen = os.getloadavg()
        print(
            f"# {args.tag} build={args.build} extension={native.__file__} md5={digest} "
            f"load={one:.2f},{five:.2f},{fifteen:.2f} t={time.strftime('%H:%M:%S')}",
            file=sys.stderr,
        )

    rows: list[tuple[object, ...]] = []
    for spec in args.row:
        parts = spec.split(":")
        if len(parts) != 3:
            raise SystemExit(f"--row wants tier:dataset:op, got {spec!r}")
        tier, dataset, op = parts
        path = PROJECT_ROOT / "benchmarks" / "data" / "generated" / tier / f"{dataset}.json"
        payload = path.read_bytes()
        if op == "dumps":
            value = json.loads(payload)
            calls = {
                "strata-bytes": lambda value=value: strata.dumps(value, return_type="bytes"),
                "strata-str": lambda value=value: strata.dumps(value),
                "orjson-bytes": lambda value=value: orjson.dumps(value),
            }
        elif op == "loads":
            value = None
            calls = {
                "strata-loads": lambda payload=payload: strata.loads(payload),
                "orjson-loads": lambda payload=payload: orjson.loads(payload),
            }
        else:
            raise SystemExit(f"unknown op {op!r} in {spec!r}")
        samples = ab_rounds.alternating_rounds(
            calls, repeat=args.repeat, warmup=args.warmup, preamble=ab_rounds.collect
        )
        label = _label(tier, dataset, op)
        for engine, values in samples.items():
            rows.extend(ab_rounds.tsv_rows(args.tag, args.build, engine, label, values))
        del value
        del payload
        del calls

    ab_rounds.print_tsv(rows, header=ab_rounds.TSV_HEADER if args.header else None)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

"""One process, one build: the `dumps` rows measured under the harness's condition.

The unit of work an A/B driver launches. It exists because a shared library
cannot be swapped inside a running interpreter: comparing two builds of the
extension means comparing two *processes*, and the only honest way to do that
is to make each process print every sample it took and let the driver pair them
across launches.

What it measures, per dataset: strata `bytes` (the official `dumps` row's
call -- `benchmarks/bench_main.py` uses `return_type="bytes"`), strata `str`,
and orjson `bytes`. orjson is not here to be beaten; it is the drift control.
It is the same rival binary in every launch, so its movement between launches
is the machine's movement, and a strata effect that matches it is drift.

Order alternates every round (`benchmarks/ab_rounds.py`), the `gc.collect()`
that defines the row's condition runs outside the timed span, and stdout is
raw TSV: one line per sample.

usage: dumps_rows_probe.py --build <tag> [--tier small] [--repeat 60]
                           [--dataset mixed --dataset flat ...] [--header]
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

from benchmarks import ab_rounds

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_DATASETS = ("mixed", "flat", "nested", "users", "wide_arrays")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", required=True, help="tag naming the extension under test")
    parser.add_argument(
        "--tag", default="", help="free-form tag for this launch (e.g. round index)"
    )
    parser.add_argument("--tier", default="small")
    parser.add_argument("--repeat", type=int, default=60)
    parser.add_argument("--warmup", type=int, default=2)
    parser.add_argument("--dataset", action="append", default=None)
    parser.add_argument("--header", action="store_true", help="print the TSV header first")
    parser.add_argument(
        "--identify",
        action="store_true",
        help="print the loaded extension's path and hash to stderr",
    )
    args = parser.parse_args(argv)

    import orjson

    import strata

    if args.identify:
        import hashlib

        import strata._strata as native

        digest = hashlib.md5(Path(native.__file__).read_bytes()).hexdigest()  # noqa: S324
        print(f"# extension {native.__file__} md5={digest}", file=sys.stderr)

    datasets = tuple(args.dataset) if args.dataset else DEFAULT_DATASETS
    rows: list[tuple[object, ...]] = []
    for name in datasets:
        path = PROJECT_ROOT / "benchmarks" / "data" / "generated" / args.tier / f"{name}.json"
        with open(path, "rb") as handle:
            value = json.loads(handle.read())
        calls = {
            "strata-bytes": lambda value=value: strata.dumps(value, return_type="bytes"),
            "strata-str": lambda value=value: strata.dumps(value),
            "orjson-bytes": lambda value=value: orjson.dumps(value),
        }
        samples = ab_rounds.alternating_rounds(
            calls, repeat=args.repeat, warmup=args.warmup, preamble=ab_rounds.collect
        )
        for engine, values in samples.items():
            rows.extend(ab_rounds.tsv_rows(args.tag, args.build, engine, name, values))
        del value

    ab_rounds.print_tsv(rows, header=ab_rounds.TSV_HEADER if args.header else None)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

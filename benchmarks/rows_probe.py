"""One process, one build: named rows across tiers and operations.

`benchmarks/dumps_rows_probe.py` is the P0 unit of work and it is kept
verbatim; this is the same probe generalised in the three ways the campaign's
timing windows need and P0 did not:

* **a row names its tier**, so `small dumps flat` and `medium dumps flat` can
  be measured in the *same* launch (the P0 probe takes one `--tier` for the
  whole process, which would have forced two separate A-B-B-A sessions and
  thrown away the pairing between them);
* **a row names its operation**, so `loads` rows can ride in the same launch
  as `dumps` rows. `loads flat` is a required control here because the G->T
  profile change moves `parse_array` by -7,524 bytes: a build change that
  helps `dumps` and hurts `loads` is a trade, not a win;
* **the file operations are real**, not approximated by subtracting two
  in-memory medians: `load` opens and parses a `.json` file, `ndload` a
  `.ndjson` one, and `dump` serializes and writes a file. Those three rows
  were predeclared in E26-P5 and could not be run at all, because this probe
  used to accept only `dumps` and `loads` (the 7 September review, finding 5).

The calls are the official rows' calls, copied from `benchmarks/bench_main.py`,
including the bytes modes: `dumps` is `strata.dumps(value, return_type="bytes")`
against `orjson.dumps(value)` (plus strata's `str` form), `loads` is
`strata.loads(payload_bytes)` against `orjson.loads(payload_bytes)`, `load` is
`strata.load(path)` against `orjson.loads(Path(path).read_bytes())`, `ndload`
is `strata.load(ndjson)` against orjson's per-line parse of the same bytes, and
`dump` is `strata.dump(value, path)` against `orjson.dumps` written to a file
opened in binary mode. The rival's composition is the one the canonical report
uses; anything else would be comparing two different pipelines. The six
JSONPath rows (`query-<key>` and `search-<key>` on `users`, the keys in
`QUERY_BY_KEY`) are the one exception: their canonical rivals are pure Python
and take up to ~850 ms a call, so their drift control is orjson parsing the same
document (see `_calls`).

Everything else is P0's: `ab_rounds.alternating_rounds`, the `gc.collect()`
preamble *outside* the timed span, order alternating every round, one TSV line
per sample on stdout, the same `ab_rounds.TSV_HEADER` — so
`benchmarks/ab_builds.py --analyze`, `benchmarks/ab_blocks.py` and
`benchmarks/ab_floor.py` read this tool's output without modification.

usage: rows_probe.py --build <tag> --row <tier>:<dataset>:<op> [--row ...]
                     [--repeat 60] [--warmup 2] [--header] [--identify]
"""

from __future__ import annotations

import argparse
import json
import os
import platform
import shutil
import sys
import tempfile
import time
from pathlib import Path

from benchmarks import ab_rounds

PROJECT_ROOT = Path(__file__).resolve().parents[1]

TIER_ABBREVIATION = {"small": "sm", "medium": "md", "large": "lg"}

# The file each operation reads or writes, relative to the tier directory.
SUFFIX_BY_OP = {
    "dumps": ".json",
    "loads": ".json",
    "load": ".json",
    "ndload": ".ndjson",
    "dump": ".json",
    "query": ".json",
    "search": ".json",
}

# The canonical report's three JSONPath rows (`harness.QUERY_LABELS`), keyed so
# that a row spec stays `tier:dataset:op` with nothing shell-active in it: the
# op is `query-<key>` or `search-<key>`, e.g. `small:users:search-rtotal`.
QUERY_BY_KEY = {
    "id": "$[*].id",
    "total": "$[*].orders[*].total",
    "rtotal": "$..total",
}


def _label(tier: str, dataset: str, op: str) -> str:
    return f"{TIER_ABBREVIATION.get(tier, tier)}-{dataset}-{op}"


def machine_state() -> str:
    """Load and machine identity, on every platform the matrix supports.

    `os.getloadavg` is Unix-only and this probe runs on the Windows leg too,
    where an unconditional call ended the launch before it took a sample (the
    7 September review, finding 5). Windows reports no load average at all, so
    the honest reading there is that it is unavailable — never a fabricated
    zero.
    """
    if hasattr(os, "getloadavg"):
        try:
            one, five, fifteen = os.getloadavg()
        except OSError:
            return "load=unavailable"
        return f"load={one:.2f},{five:.2f},{fifteen:.2f}"
    return f"load=unavailable({platform.system()})"


def identify(build: str, tag: str) -> str:
    import hashlib

    import strata._strata as native

    digest = hashlib.md5(Path(native.__file__).read_bytes()).hexdigest()  # noqa: S324
    return (
        f"# {tag} build={build} extension={native.__file__} md5={digest} "
        f"{machine_state()} cpus={os.cpu_count()} t={time.strftime('%H:%M:%S')}"
    )


def _calls(op: str, path: Path, strata, orjson, out_dir: Path) -> dict:
    """The engines for one row, named after the call each one times."""
    name = str(path)
    if op == "dumps":
        value = json.loads(path.read_bytes())
        return {
            "strata-bytes": lambda: strata.dumps(value, return_type="bytes"),
            "strata-str": lambda: strata.dumps(value),
            "orjson-bytes": lambda: orjson.dumps(value),
        }
    if op == "loads":
        payload = path.read_bytes()
        return {
            "strata-loads": lambda: strata.loads(payload),
            "orjson-loads": lambda: orjson.loads(payload),
        }
    if op == "load":
        return {
            "strata-load": lambda: strata.load(name),
            "orjson-load": lambda: orjson.loads(Path(name).read_bytes()),
        }
    if op == "ndload":
        return {
            "strata-ndload": lambda: strata.load(name),
            "orjson-ndload": lambda: [
                orjson.loads(line) for line in Path(name).read_bytes().splitlines() if line
            ],
        }
    if op == "dump":
        value = json.loads(path.read_bytes())
        strata_target = str(out_dir / "strata.json")
        orjson_target = out_dir / "orjson.json"

        def write_orjson():
            with open(orjson_target, "wb") as handle:  # bytes, as bench_main writes it
                handle.write(orjson.dumps(value))

        return {
            "strata-dump": lambda: strata.dump(value, strata_target),
            "orjson-dump": write_orjson,
        }
    base, _, key = op.partition("-")
    if base in ("query", "search"):
        # strata's calls are bench_main's: the expression compiled outside the
        # timed call, `query` over the stdlib-parsed tree, `search` over the
        # file. Their canonical rivals (jmespath, jsonpath-ng) take up to
        # ~850 ms a call on `$..total` and have no orjson form, so the in-process
        # drift control is orjson parsing the same document -- in memory for
        # `query`, read from the file for `search` (the parse half of the
        # canonical orjson+jmespath composition). It is a denominator for
        # machine drift, not a standings rival.
        expression = QUERY_BY_KEY.get(key)
        if expression is None:
            raise SystemExit(f"unknown op {op!r}: the key must be one of {sorted(QUERY_BY_KEY)}")
        compiled = strata.compile(expression)
        payload = path.read_bytes()
        if base == "query":
            value = json.loads(payload)
            return {
                "strata-query": lambda: strata.query(value, compiled),
                "orjson-query": lambda: orjson.loads(payload),
            }
        return {
            "strata-search": lambda: strata.search(name, compiled),
            "orjson-search": lambda: orjson.loads(Path(name).read_bytes()),
        }
    raise SystemExit(f"unknown op {op!r}")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", required=True, help="tag naming the extension under test")
    parser.add_argument("--tag", default="", help="free-form tag for this launch")
    parser.add_argument(
        "--row",
        action="append",
        required=True,
        help="tier:dataset:op, e.g. small:flat:dumps, medium:flat:loads, "
        "small:users:load, small:users:ndload, small:mixed:dump, "
        "small:users:query-id or small:users:search-rtotal",
    )
    parser.add_argument("--repeat", type=int, default=60)
    parser.add_argument("--warmup", type=int, default=2)
    parser.add_argument("--header", action="store_true")
    parser.add_argument("--identify", action="store_true")
    args = parser.parse_args(argv)

    import orjson

    import strata

    if args.identify:
        print(identify(args.build, args.tag), file=sys.stderr)

    out_dir = Path(tempfile.mkdtemp(prefix="rows_probe_"))
    rows: list[tuple[object, ...]] = []
    try:
        for spec in args.row:
            parts = spec.split(":")
            if len(parts) != 3:
                raise SystemExit(f"--row wants tier:dataset:op, got {spec!r}")
            tier, dataset, op = parts
            suffix = SUFFIX_BY_OP.get(op.partition("-")[0])
            if suffix is None:
                raise SystemExit(f"unknown op {op!r} in {spec!r}")
            path = PROJECT_ROOT / "benchmarks" / "data" / "generated" / tier / f"{dataset}{suffix}"
            if not path.exists():
                raise SystemExit(f"{spec}: no dataset at {path}")
            calls = _calls(op, path, strata, orjson, out_dir)
            samples = ab_rounds.alternating_rounds(
                calls, repeat=args.repeat, warmup=args.warmup, preamble=ab_rounds.collect
            )
            label = _label(tier, dataset, op)
            for engine, values in samples.items():
                rows.extend(ab_rounds.tsv_rows(args.tag, args.build, engine, label, values))
            del calls
    finally:
        shutil.rmtree(out_dir, ignore_errors=True)

    ab_rounds.print_tsv(rows, header=ab_rounds.TSV_HEADER if args.header else None)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

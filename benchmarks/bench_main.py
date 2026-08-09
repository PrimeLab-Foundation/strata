"""Benchmark orchestrator.

Runs every implemented category against the competitor set and writes one
machine-generated report. Only categories that exist are measured: `loads` and
`dumps` today, with file, folder and JSONPath categories joining as their
milestones land.

Fairness (docs/context/benchmarks.md): every library sees the same bytes and
the same semantics. A library with no native equivalent for an operation is
excluded and named in the report -- never emulated with a Python shim. A
library that is installed but fails is an ERROR row, and a report with ERROR
rows is invalid.
"""

from __future__ import annotations

import argparse
import gc
import json
import os
import sys
import tempfile
from pathlib import Path

from benchmarks.harness import (
    DEFAULT_WARMUP,
    EXPLORATORY_REPEATS,
    Measurement,
    Report,
    describe_environment,
    measure_interleaved,
    peak_rss_mb,
    render_report,
    summarize,
)

PROJECT_ROOT = Path(__file__).resolve().parents[1]

# The compiler flags the extension is built with (setup.py). Recorded so a
# result can be traced back to the build that produced it.
BASE_COMPILER_FLAGS = "-std=c++20 -O3 -march=native"


def _compiler_flags() -> str:
    """Describe the build actually being measured.

    A hardcoded string would report plain -O3 for a PGO+LTO run and quietly
    make two incomparable reports look comparable — exactly the kind of
    mismatch the fairness rules in docs/context/benchmarks.md exist to stop.
    """
    flags = [BASE_COMPILER_FLAGS]
    if os.environ.get("STRATA_ENABLE_LTO", "0").strip() == "1":
        flags.append("-flto")
    mode = os.environ.get("PGO_MODE", "").strip().lower()
    if mode == "generate":
        flags.append("-fprofile-generate (instrumented; not a performance build)")
    elif mode == "use":
        flags.append("-fprofile-use (PGO)")
    return " ".join(flags)


def _load_competitors() -> tuple[dict, dict[str, str]]:
    """Import what is available; name what is not."""
    available: dict = {}
    excluded: dict[str, str] = {}

    import json as stdlib_json

    import strata

    available["strata"] = strata
    available["json"] = stdlib_json

    for name in ("orjson", "msgspec", "ujson", "simdjson"):
        try:
            available[name] = __import__(name)
        except ImportError:
            excluded[name] = "not installed"
    return available, excluded


def _load_query_libraries(excluded: dict[str, str]) -> dict:
    """JSONPath/JMESPath engines — query and search only."""
    engines = {}
    for name in ("jmespath", "jsonpath_ng"):
        try:
            engines[name] = __import__(name, fromlist=["ext"])
        except ImportError:
            excluded[name] = "not installed"
    return engines


def _loads_callables(libraries: dict, payload: bytes) -> dict:
    """text/bytes -> Python tree, natively in each library."""
    text = payload.decode("utf-8")
    calls = {}
    if "strata" in libraries:
        calls["strata"] = lambda: libraries["strata"].loads(payload)
    if "orjson" in libraries:
        calls["orjson"] = lambda: libraries["orjson"].loads(payload)
    if "msgspec" in libraries:
        decoder = libraries["msgspec"].json.Decoder()
        calls["msgspec"] = lambda: decoder.decode(payload)
    if "ujson" in libraries:
        calls["ujson"] = lambda: libraries["ujson"].loads(text)
    if "simdjson" in libraries:
        parser = libraries["simdjson"].Parser()
        calls["pysimdjson"] = lambda: parser.parse(payload).as_list()
    if "json" in libraries:
        calls["json"] = lambda: libraries["json"].loads(payload)
    return calls


def _dumps_callables(libraries: dict, value) -> dict:
    """tree -> text/bytes. orjson emits bytes, so strata is compared bytes-to-bytes."""
    calls = {}
    if "strata" in libraries:
        calls["strata"] = lambda: libraries["strata"].dumps(value, return_type="bytes")
    if "orjson" in libraries:
        calls["orjson"] = lambda: libraries["orjson"].dumps(value)
    if "msgspec" in libraries:
        encoder = libraries["msgspec"].json.Encoder()
        calls["msgspec"] = lambda: encoder.encode(value)
    if "ujson" in libraries:
        calls["ujson"] = lambda: libraries["ujson"].dumps(value)
    if "json" in libraries:
        calls["json"] = lambda: libraries["json"].dumps(value, separators=(",", ":"))
    return calls


def _load_file_callables(libraries: dict, path: Path) -> dict:
    """file -> tree. Only strata has a file API; the rest read then parse."""
    name = str(path)
    calls = {}
    if "strata" in libraries:
        calls["strata"] = lambda: libraries["strata"].load(name)
    if "orjson" in libraries:
        calls["orjson"] = lambda: libraries["orjson"].loads(Path(name).read_bytes())
    if "msgspec" in libraries:
        decoder = libraries["msgspec"].json.Decoder()
        calls["msgspec"] = lambda: decoder.decode(Path(name).read_bytes())
    if "ujson" in libraries:
        calls["ujson"] = lambda: libraries["ujson"].loads(Path(name).read_text())
    if "json" in libraries:
        calls["json"] = lambda: libraries["json"].loads(Path(name).read_bytes())
    return calls


def _load_ndjson_callables(libraries: dict, path: Path) -> dict:
    """NDJSON file -> list of records. Competitors split lines themselves."""
    name = str(path)
    calls = {}
    if "strata" in libraries:
        calls["strata"] = lambda: libraries["strata"].load(name)

    def _per_line(parse):
        return lambda: [parse(line) for line in Path(name).read_bytes().splitlines() if line]

    if "orjson" in libraries:
        calls["orjson"] = _per_line(libraries["orjson"].loads)
    if "msgspec" in libraries:
        calls["msgspec"] = _per_line(libraries["msgspec"].json.Decoder().decode)
    if "ujson" in libraries:
        calls["ujson"] = _per_line(libraries["ujson"].loads)
    if "json" in libraries:
        calls["json"] = _per_line(libraries["json"].loads)
    return calls


def _dump_callables(libraries: dict, value, out_dir: Path) -> dict:
    """tree -> file. Competitors serialize then write."""
    calls = {}
    if "strata" in libraries:
        target = str(out_dir / "strata.json")
        calls["strata"] = lambda: libraries["strata"].dump(value, target)

    def _serialize_then_write(name: str, serialize, binary: bool):
        target = out_dir / name
        mode = "wb" if binary else "w"

        def call():
            with open(target, mode) as handle:
                handle.write(serialize(value))

        return call

    if "orjson" in libraries:
        calls["orjson"] = _serialize_then_write("orjson.json", libraries["orjson"].dumps, True)
    if "msgspec" in libraries:
        encoder = libraries["msgspec"].json.Encoder()
        calls["msgspec"] = _serialize_then_write("msgspec.json", encoder.encode, True)
    if "ujson" in libraries:
        calls["ujson"] = _serialize_then_write("ujson.json", libraries["ujson"].dumps, False)
    if "json" in libraries:
        calls["json"] = _serialize_then_write(
            "json.json", lambda v: libraries["json"].dumps(v, separators=(",", ":")), False
        )
    return calls


# JSONPath queries, spelled for each library that can express them. A library
# with no equivalent is *excluded* from that row rather than emulated with a
# Python shim (docs/context/benchmarks.md, "Fairness rules").
QUERIES = [
    {"label": "$[*].id", "strata": "$[*].id", "jmespath": "[*].id", "jsonpath_ng": "$[*].id"},
    {
        "label": "$[*].orders[*].total",
        "strata": "$[*].orders[*].total",
        "jmespath": "[].orders[].total",  # flattening form: same result set
        "jsonpath_ng": "$[*].orders[*].total",
    },
    {
        # Recursive descent: jmespath has no equivalent.
        "label": "$..total",
        "strata": "$..total",
        "jmespath": None,
        "jsonpath_ng": "$..total",
    },
]


def _query_callables(libraries: dict, value, query: dict, query_libraries: dict) -> dict:
    # Each compiled expression is bound as a default argument: a bare closure
    # over a shared name would leave every library calling the last one
    # compiled. Compiling outside the timed call is deliberate and equal for
    # all three -- only evaluation is measured.
    calls = {}
    if "strata" in libraries:
        strata_path = libraries["strata"].compile(query["strata"])
        calls["strata"] = lambda p=strata_path: libraries["strata"].query(value, p)
    if "jmespath" in query_libraries and query["jmespath"]:
        jmes_path = query_libraries["jmespath"].compile(query["jmespath"])
        calls["jmespath"] = lambda p=jmes_path: p.search(value)
    if "jsonpath_ng" in query_libraries and query["jsonpath_ng"]:
        ng_path = query_libraries["jsonpath_ng"].parse(query["jsonpath_ng"])
        calls["jsonpath-ng"] = lambda p=ng_path: [match.value for match in p.find(value)]
    return calls


def _search_callables(libraries: dict, path: Path, query: dict, query_libraries: dict) -> dict:
    """file -> matches. No competitor streams, so each parses then queries."""
    name = str(path)
    calls = {}
    loads = libraries["orjson"].loads if "orjson" in libraries else None
    if "strata" in libraries:
        strata_path = libraries["strata"].compile(query["strata"])
        calls["strata"] = lambda p=strata_path: libraries["strata"].search(name, p)
    if loads and "jmespath" in query_libraries and query["jmespath"]:
        jmes_path = query_libraries["jmespath"].compile(query["jmespath"])
        calls["orjson+jmespath"] = lambda p=jmes_path: p.search(loads(Path(name).read_bytes()))
    if loads and "jsonpath_ng" in query_libraries and query["jsonpath_ng"]:
        ng_path = query_libraries["jsonpath_ng"].parse(query["jsonpath_ng"])
        calls["orjson+jsonpath-ng"] = lambda p=ng_path: [
            match.value for match in p.find(loads(Path(name).read_bytes()))
        ]
    return calls


def _drop_disagreeing(calls: dict, report: Report, label: str) -> dict:
    """Keep only the libraries that compute the *same answer* as strata.

    A JSONPath expression can be spelled in a way that quietly does less work —
    jmespath's `[*]` projects where `[]` flattens, and the projecting form
    returns 1000 nested lists where the others return 4994 values. Timing those
    against each other would be a fairness breach dressed up as a win
    (docs/context/benchmarks.md). Rather than trust the expressions to match,
    run them once and compare.
    """
    if "strata" not in calls:
        return calls
    try:
        expected = sorted(repr(item) for item in calls["strata"]())
    except Exception:  # noqa: BLE001 -- a broken strata call is an ERROR row, handled downstream
        return calls

    agreeing = {}
    for library, call in calls.items():
        if library == "strata":
            agreeing[library] = call
            continue
        try:
            actual = sorted(repr(item) for item in call())
        except Exception:  # noqa: BLE001
            agreeing[library] = call  # let _run_section record the real error
            continue
        if actual == expected:
            agreeing[library] = call
        else:
            report.excluded[f"{library} ({label})"] = (
                f"different result set ({len(actual)} vs {len(expected)} values)"
            )
    return agreeing


def _run_section(
    report: Report, section: str, dataset: str, calls: dict, *, repeat: int, warmup: int
) -> None:
    """Measure every library on this dataset, interleaved so drift is shared."""
    working = {}
    for library, call in calls.items():
        try:
            call()  # a library that cannot do the job at all is an ERROR row
        except Exception as error:  # noqa: BLE001
            report.measurements.append(
                Measurement(
                    section=section, dataset=dataset, library=library, error=type(error).__name__
                ),
            )
            continue
        working[library] = call

    if not working:
        return
    timings = measure_interleaved(working, repeat=repeat, warmup=warmup)
    rss = peak_rss_mb()
    for library, samples in timings.items():
        report.measurements.append(summarize(section, dataset, library, samples, rss))


def run(datasets: list[Path], *, name: str, repeat: int, warmup: int) -> Report:
    libraries, excluded = _load_competitors()
    query_libraries = _load_query_libraries(excluded)
    report = Report(
        name=name, environment=describe_environment(_compiler_flags()), excluded=excluded
    )
    report.environment["repeats"] = str(repeat)
    report.environment["warmup"] = str(warmup)

    for path in datasets:
        payload = path.read_bytes()
        label = path.name

        # An NDJSON file is not one JSON document, so it belongs to exactly one
        # row of the contract's table: `load (.ndjson)`.
        if path.suffix == ".ndjson":
            _run_section(
                report,
                "load (ndjson)",
                label,
                _load_ndjson_callables(libraries, path),
                repeat=repeat,
                warmup=warmup,
            )
            del payload
            gc.collect()
            continue

        _run_section(
            report,
            "loads",
            label,
            _loads_callables(libraries, payload),
            repeat=repeat,
            warmup=warmup,
        )

        value = json.loads(payload)
        _run_section(
            report, "dumps", label, _dumps_callables(libraries, value), repeat=repeat, warmup=warmup
        )

        _run_section(
            report,
            "load",
            label,
            _load_file_callables(libraries, path),
            repeat=repeat,
            warmup=warmup,
        )

        with tempfile.TemporaryDirectory() as scratch:
            _run_section(
                report,
                "dump",
                label,
                _dump_callables(libraries, value, Path(scratch)),
                repeat=repeat,
                warmup=warmup,
            )

        # JSONPath is only meaningful against the users shape; the synthetic
        # shapes have no such structure to address.
        if path.stem == "users":
            for query in QUERIES:
                row = f"{label} {query['label']}"
                _run_section(
                    report,
                    "query",
                    row,
                    _drop_disagreeing(
                        _query_callables(libraries, value, query, query_libraries), report, row
                    ),
                    repeat=repeat,
                    warmup=warmup,
                )
                _run_section(
                    report,
                    "search",
                    row,
                    _drop_disagreeing(
                        _search_callables(libraries, path, query, query_libraries), report, row
                    ),
                    repeat=repeat,
                    warmup=warmup,
                )

        # Drop this dataset before the next one is measured, so its tree is not
        # still resident while the next dataset is timed.
        del value, payload
        gc.collect()

    return report


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dataset", action="append", default=[], type=Path, required=True)
    parser.add_argument("--name", required=True, help="report name, e.g. small")
    parser.add_argument("--repeat", type=int, default=EXPLORATORY_REPEATS)
    parser.add_argument("--warmup", type=int, default=DEFAULT_WARMUP)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args(argv)

    missing = [str(p) for p in args.dataset if not p.is_file()]
    if missing:
        sys.stderr.write(f"error: dataset not found: {missing}\nRun `make bench-data` first.\n")
        return 1

    report = run(args.dataset, name=args.name, repeat=args.repeat, warmup=args.warmup)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(render_report(report), encoding="utf-8")
    print(f"wrote {args.output}")

    if report.has_errors:
        sys.stderr.write(
            "error: the run produced ERROR rows, so the report is invalid.\n"
            "Fix the failure or exclude the library explicitly "
            "(docs/context/benchmarks.md).\n",
        )
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

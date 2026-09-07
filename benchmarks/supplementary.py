"""Supplementary v1: NDJSON search and folder operations, outside the 135 rows."""

from __future__ import annotations

import argparse
import json
import os
import tempfile
from pathlib import Path

from benchmarks.bench_main import (
    QUERIES,
    _load_competitors,
    _load_query_libraries,
    _run_section,
)
from benchmarks.harness import Report, describe_environment, validate_report
from benchmarks.provenance import capture, write_report

ROWS = tuple(("search (ndjson)", q["label"]) for q in QUERIES) + (
    ("folder load", "users"),
    ("folder dump", "users"),
    ("folder search", "users $[*].id"),
)


def discover(root: Path) -> list[Path]:
    paths = []
    for directory, dirs, files in os.walk(root, followlinks=False):
        dirs[:] = [
            name
            for name in dirs
            if not name.startswith(".") and not (Path(directory) / name).is_symlink()
        ]
        paths.extend(
            Path(directory) / name
            for name in files
            if not name.startswith(".")
            and not (Path(directory) / name).is_symlink()
            and Path(name).suffix.lower() in {".json", ".ndjson", ".jsonl"}
        )
    return sorted(paths, key=lambda p: p.relative_to(root).as_posix().encode("utf-8"))


def loop_load(strata, root: Path) -> list:
    records = []
    for path in discover(root):
        value = strata.load(path)
        records.extend(value if isinstance(value, list) else [value])
    return records


def loop_search(strata, root: Path, expression) -> list:
    return [match for path in discover(root) for match in strata.search(path, expression)]


def loop_dump(strata, records: list, root: Path) -> None:
    # This fixture's group strings are deliberately safe filename components.
    # Both arms perform grouping and directory creation inside the timed call.
    groups = {}
    for record in records:
        groups.setdefault(record["bench_group"], []).append(record)
    root.mkdir(parents=True, exist_ok=True)
    for group, values in groups.items():
        strata.dump(values, root / (group + ".json"))


def ndjson_calls(libraries: dict, engines: dict, path: Path, query: dict) -> dict:
    strata = libraries["strata"]
    compiled = strata.compile(query["strata"])
    calls = {"strata": lambda: strata.search(path, compiled)}
    if "orjson" not in libraries:
        return calls

    def load():
        return [libraries["orjson"].loads(line) for line in path.read_bytes().splitlines() if line]

    if "jmespath" in engines and query["jmespath"]:
        expression = engines["jmespath"].compile(query["jmespath"])
        calls["orjson+jmespath"] = lambda p=expression: p.search(load())
    if "jsonpath_ng" in engines and query["jsonpath_ng"]:
        expression = engines["jsonpath_ng"].parse(query["jsonpath_ng"])
        calls["orjson+jsonpath-ng"] = lambda p=expression: [m.value for m in p.find(load())]
    return calls


def equivalent(calls: dict) -> None:
    expected = calls["strata"]()
    for name, call in calls.items():
        if name == "strata":
            continue
        if call() != expected:
            raise ValueError(f"{name}: supplementary results differ in value or order")


def file_bytes(root: Path) -> dict:
    return {p.relative_to(root).as_posix(): p.read_bytes() for p in discover(root)}


def run(json_path: Path, ndjson_path: Path, *, repeat: int, warmup: int) -> Report:
    libraries, excluded = _load_competitors()
    engines = _load_query_libraries(excluded)
    strata = libraries["strata"]
    report = Report(
        "supplementary-v1", describe_environment("see build provenance"), excluded=excluded
    )
    report.provenance = capture(
        [json_path, ndjson_path], {**libraries, **engines}, repeat=repeat, warmup=warmup
    )
    report.provenance["protocol"]["name"] = "supplementary-v1"
    report.provenance["protocol"]["preflight_calls"] = 2
    report.provenance["protocol"]["equivalence_calls_per_library"] = "included in preflight_calls"
    report.provenance["protocol"]["equivalence"] = (
        "ordered results or exact file bytes before timing"
    )
    report.excluded["external engines (folder operations)"] = (
        "no native equivalent; strata-loop control"
    )
    report.excluded["jmespath ($..total)"] = "recursive descent unsupported"
    if "orjson" not in libraries:
        report.excluded["composed NDJSON search"] = "requires orjson"
    for query in QUERIES:
        calls = ndjson_calls(libraries, engines, ndjson_path, query)
        equivalent(calls)
        _run_section(report, "search (ndjson)", query["label"], calls, repeat=repeat, warmup=warmup)

    records = [
        dict(record, bench_group=f"group-{i % 8}")
        for i, record in enumerate(json.loads(json_path.read_bytes()))
    ]
    with tempfile.TemporaryDirectory() as temporary:
        root = Path(temporary)
        source, native, loop = root / "source", root / "native", root / "loop"
        strata.dump(records, source, split_by="bench_group")
        compiled = strata.compile("$[*].id")
        sections = [
            (
                "folder load",
                "users",
                {
                    "strata": lambda: strata.load(source),
                    "strata-loop": lambda: loop_load(strata, source),
                },
            ),
            (
                "folder search",
                "users $[*].id",
                {
                    "strata": lambda: strata.search(source, compiled),
                    "strata-loop": lambda: loop_search(strata, source, compiled),
                },
            ),
        ]
        for section, label, calls in sections:
            equivalent(calls)
            _run_section(report, section, label, calls, repeat=repeat, warmup=warmup)
        dumps = {
            "strata": lambda: strata.dump(records, native, split_by="bench_group"),
            "strata-loop": lambda: loop_dump(strata, records, loop),
        }
        for call in dumps.values():
            call()
        if file_bytes(native) != file_bytes(loop):
            raise ValueError("folder dump outputs differ")
        _run_section(report, "folder dump", "users", dumps, repeat=repeat, warmup=warmup)
        if file_bytes(native) != file_bytes(loop):
            raise ValueError("folder dump outputs differ after measurement")
    return report


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--repeat", type=int, default=10)
    parser.add_argument("--warmup", type=int, default=2)
    args = parser.parse_args(argv)
    if args.repeat < 1 or args.warmup < 0:
        parser.error("repeat must be positive and warmup nonnegative")
    report = run(
        args.data / "users.json", args.data / "users.ndjson", repeat=args.repeat, warmup=args.warmup
    )
    write_report(args.output, report)
    verdict = validate_report(report, expected=ROWS)
    controls = validate_report(
        report,
        expected=tuple(row for row in ROWS if row[0].startswith("folder")),
        library="strata-loop",
    )
    print(verdict.describe())
    print(controls.describe())
    return 0 if verdict.ok and controls.ok else 1


if __name__ == "__main__":
    raise SystemExit(main())

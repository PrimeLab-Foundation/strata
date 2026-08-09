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
    report = Report(
        name=name, environment=describe_environment(_compiler_flags()), excluded=excluded
    )
    report.environment["repeats"] = str(repeat)
    report.environment["warmup"] = str(warmup)

    for path in datasets:
        payload = path.read_bytes()
        label = path.name

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

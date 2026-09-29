"""native-v1: a separate declared benchmark scope for strata's native types.

Rows (`harness.native_workload_rows`, registered as `harness.WORKLOADS["native-v1"]`):

- `dumps`/`dump` of a seeded native dataset (`benchmarks/native_dataset.py`) per
  tier -- strata's `native=True` arm against the rivals that can encode the
  same types natively or through their own documented hook.
- three flag rows: `dumps` of the canonical `mixed` dataset per tier, timing
  strata's `native=False` and `native=True` arms beside the canonical
  rivals -- the cost of the flag on a document with no native object.

Neither set joins the canonical 27-row workload or the 135-row denominator
(docs/decisions.md, 2026-09-29, "benchmarks"); `ci_summary` renders them in
their own section. Rivals are excluded and recorded, never emulated, exactly
as the canonical harness does (docs/context/benchmarks.md).

Agreement: a rival's output is accepted only if, parsed back as JSON (numbers
as `decimal.Decimal` so a Decimal-vs-float rival is compared by value, not by
text), it equals strata's. A disagreeing rival is dropped and recorded, not
timed against strata -- `_drop_disagreeing`'s rule, adapted for serialized
text/bytes rather than JSONPath match lists.
"""

from __future__ import annotations

import argparse
import dataclasses
import datetime
import decimal
import enum
import json as stdlib_json
import tempfile
import uuid as uuid_module
from pathlib import Path

from benchmarks.bench_main import _run_section
from benchmarks.harness import (
    NATIVE_REPORT_NAME,
    NATIVE_TIERS,
    Report,
    describe_environment,
    native_data_rows,
    native_flag_rows,
    native_workload_rows,
    validate_report,
)
from benchmarks.native_dataset import generate_tier
from benchmarks.provenance import capture, write_report

DATA_ROWS = native_data_rows()
FLAG_ROWS = native_flag_rows()
ROWS = native_workload_rows()


def _load_rivals() -> tuple[dict, dict[str, str]]:
    """Import what is available; name what is not (mirrors `bench_main`)."""
    available: dict = {}
    excluded: dict[str, str] = {}
    import strata

    available["strata"] = strata
    available["json"] = stdlib_json
    for name in ("orjson", "msgspec", "ujson"):
        try:
            available[name] = __import__(name)
        except ImportError:
            excluded[name] = "not installed"
    return available, excluded


def _stdlib_default(obj):
    if isinstance(obj, uuid_module.UUID):
        return str(obj)
    if isinstance(obj, datetime.datetime):
        return obj.isoformat()
    if isinstance(obj, datetime.date):
        return obj.isoformat()
    if isinstance(obj, decimal.Decimal):
        return float(obj)
    if isinstance(obj, enum.Enum):
        return obj.value
    if dataclasses.is_dataclass(obj) and not isinstance(obj, type):
        return dataclasses.asdict(obj)
    if isinstance(obj, (set, frozenset)):
        return list(obj)
    raise TypeError(f"Object of type {type(obj).__name__} is not JSON serializable")


def _orjson_default(obj):
    import orjson

    if isinstance(obj, decimal.Decimal):
        return orjson.Fragment(str(obj).encode())
    if isinstance(obj, (set, frozenset)):
        return list(obj)
    raise TypeError(f"Object of type {type(obj).__name__} is not JSON serializable")


def _native_dumps_calls(rivals: dict, value) -> dict:
    """`dumps` of the native dataset. ujson is out of scope: no native support."""
    calls: dict = {}
    if "strata" in rivals:
        calls["strata"] = lambda: rivals["strata"].dumps(value, return_type="bytes", native=True)
    if "orjson" in rivals:
        calls["orjson"] = lambda: rivals["orjson"].dumps(value, default=_orjson_default)
    if "msgspec" in rivals:
        encoder = rivals["msgspec"].json.Encoder(decimal_format="number")
        calls["msgspec"] = lambda: encoder.encode(value)
    if "json" in rivals:
        calls["json"] = lambda: rivals["json"].dumps(
            value, default=_stdlib_default, separators=(",", ":")
        )
    return calls


def _native_dump_calls(rivals: dict, value, out_dir: Path) -> dict:
    """`dump` of the native dataset: competitors serialize then write."""
    calls: dict = {}
    if "strata" in rivals:
        target = str(out_dir / "strata.json")
        calls["strata"] = lambda: rivals["strata"].dump(value, target, native=True)

    def _serialize_then_write(name: str, serialize):
        target = out_dir / name

        def call():
            with open(target, "wb") as handle:
                data = serialize(value)
                handle.write(data if isinstance(data, bytes) else data.encode("utf-8"))

        return call

    if "orjson" in rivals:
        calls["orjson"] = _serialize_then_write(
            "orjson.json", lambda v: rivals["orjson"].dumps(v, default=_orjson_default)
        )
    if "msgspec" in rivals:
        encoder = rivals["msgspec"].json.Encoder(decimal_format="number")
        calls["msgspec"] = _serialize_then_write("msgspec.json", encoder.encode)
    if "json" in rivals:
        calls["json"] = _serialize_then_write(
            "json.json",
            lambda v: rivals["json"].dumps(v, default=_stdlib_default, separators=(",", ":")),
        )
    return calls


def _flag_calls(rivals: dict, value) -> dict:
    """`dumps` of the canonical `mixed` dataset: strata's two flag arms beside
    the canonical rivals (no native object is present, so every rival applies)."""
    calls: dict = {}
    if "strata" in rivals:
        calls["strata (native=False)"] = lambda: rivals["strata"].dumps(
            value, return_type="bytes", native=False
        )
        calls["strata (native=True)"] = lambda: rivals["strata"].dumps(
            value, return_type="bytes", native=True
        )
    if "orjson" in rivals:
        calls["orjson"] = lambda: rivals["orjson"].dumps(value)
    if "msgspec" in rivals:
        encoder = rivals["msgspec"].json.Encoder()
        calls["msgspec"] = lambda: encoder.encode(value)
    if "ujson" in rivals:
        calls["ujson"] = lambda: rivals["ujson"].dumps(value)
    if "json" in rivals:
        calls["json"] = lambda: rivals["json"].dumps(value, separators=(",", ":"))
    return calls


def _as_comparable(data: bytes | str):
    text = data.decode("utf-8") if isinstance(data, (bytes, bytearray)) else data
    return stdlib_json.loads(text, parse_float=decimal.Decimal)


def agreeing_calls(calls: dict, report: Report, label: str) -> dict:
    """Keep only the rivals whose parsed-JSON output equals strata's.

    Decimals compare by value (`parse_float=decimal.Decimal`), so a rival that
    represents a decimal as a float (stdlib `json`) still agrees when the
    value round-trips; one that does not is dropped and recorded, never timed.
    """
    if "strata" not in calls:
        return calls
    try:
        expected = _as_comparable(calls["strata"]())
    except Exception:  # noqa: BLE001 -- a broken strata call is an ERROR row downstream
        return calls
    agreeing = {"strata": calls["strata"]}
    for library, call in calls.items():
        if library == "strata":
            continue
        try:
            actual = _as_comparable(call())
        except Exception:  # noqa: BLE001
            agreeing[library] = call  # let _run_section record the real error
            continue
        if actual == expected:
            agreeing[library] = call
        else:
            report.excluded[f"{library} ({label})"] = "output disagrees as parsed JSON"
    return agreeing


def run(
    data_dir: Path, *, repeat: int, warmup: int, tiers: tuple[str, ...] = NATIVE_TIERS
) -> Report:
    rivals, excluded = _load_rivals()
    report = Report(
        NATIVE_REPORT_NAME, describe_environment("see build provenance"), excluded=excluded
    )
    mixed_paths = [data_dir / tier / "mixed.json" for tier in tiers if (data_dir / tier).is_dir()]
    report.provenance = capture(mixed_paths, rivals, repeat=repeat, warmup=warmup)
    report.provenance["protocol"]["name"] = "native-v1"
    report.provenance["protocol"]["equivalence"] = (
        "rivals: parsed-JSON equality with decimals compared as Decimal; a disagreeing "
        "rival is excluded and recorded, never timed"
    )
    report.excluded["ujson (native dataset)"] = "no native type support; not named"

    for tier in tiers:
        records = generate_tier(tier)
        dumps_calls = agreeing_calls(_native_dumps_calls(rivals, records), report, f"native.{tier}")
        _run_section(report, "dumps", f"native.{tier}", dumps_calls, repeat=repeat, warmup=warmup)

        with tempfile.TemporaryDirectory() as scratch:
            out_dir = Path(scratch)
            dump_calls = _native_dump_calls(rivals, records, out_dir)
            dump_calls = agreeing_calls(dump_calls, report, f"native.{tier} (dump)")
            _run_section(report, "dump", f"native.{tier}", dump_calls, repeat=repeat, warmup=warmup)

    for tier in tiers:
        mixed_path = data_dir / tier / "mixed.json"
        if not mixed_path.is_file():
            continue
        value = stdlib_json.loads(mixed_path.read_bytes())
        calls = _flag_calls(rivals, value)
        _run_section(
            report, "dumps", f"mixed.{tier} (native flag)", calls, repeat=repeat, warmup=warmup
        )
    return report


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", type=Path, required=True, help="benchmarks/data/generated")
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--repeat", type=int, default=10)
    parser.add_argument("--warmup", type=int, default=2)
    parser.add_argument(
        "--tiers",
        default=",".join(NATIVE_TIERS),
        help="comma-separated tiers to run; the flag rows need that tier's mixed.json "
        "on disk (a leg that only generated one canonical tier passes just that one)",
    )
    args = parser.parse_args(argv)
    if args.repeat < 1 or args.warmup < 0:
        parser.error("repeat must be positive and warmup nonnegative")
    tiers = tuple(tier.strip() for tier in args.tiers.split(",") if tier.strip())

    report = run(args.data, repeat=args.repeat, warmup=args.warmup, tiers=tiers)
    write_report(args.output, report)

    data_verdict = validate_report(report, expected=native_data_rows(tiers))
    print(data_verdict.describe())
    ok = data_verdict.ok
    flag_rows = native_flag_rows(tiers)
    for library in ("strata (native=False)", "strata (native=True)"):
        verdict = validate_report(report, expected=flag_rows, library=library)
        print(verdict.describe())
        ok = ok and verdict.ok
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())

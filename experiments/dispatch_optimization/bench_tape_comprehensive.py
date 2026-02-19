"""
Comprehensive benchmark: structural tape ON vs OFF for every public strata op.

Public API:
  loads(bytes)           -- parse JSON bytes → Python object
  load(path)             -- parse JSON file → Python object  (same C path as loads)
  dumps(obj)             -- serialize obj   → JSON str
  dump(target, obj)      -- serialize obj   → file/path (bytes-oriented)
  search(bytes, expr)    -- JSONPath on raw bytes   [tape hardcoded OFF in C++]
  query(obj, expr)       -- JSONPath on parsed obj  [no parse, no tape involved]

Tape effect:
  - Only loads/load are affected by STRATA_USE_STRUCTURAL_TAPE.
  - Tape is collected only when input ≥ kPythonStructuralTapeMinInputSize (10 MB).
  - dumps/dump: no parsing → tape is irrelevant.
  - search:     python_search.cpp:516 hardcodes use_structural_tape=false → immune.
  - query:      operates on already-parsed Python object → no tape.

Run via:
    .venv/bin/python experiments/dispatch_optimization/bench_tape_comprehensive.py
"""

import gc
import io
import os
import statistics
import time

import strata

# ── datasets ──────────────────────────────────────────────────────────────────
DATASETS = {
    "small":  "benchmarks/data/generated/small/users.json",
    "medium": "benchmarks/data/generated/medium/users.json",
    "large":  "benchmarks/data/generated/large/users.json",
}

# JSONPath queries (same set used in the main bench suite)
QUERIES = [
    ("Extract user IDs",        "$.users[*].id"),
    ("Extract user names",      "$.users[*].name"),
    ("Nested city field",       "$.users[*].metadata.city"),
    ("Order item prices (WC)",  "$.users[*].orders[*].items[*].price"),
]

N_LOADS  = 30
N_DUMPS  = 30
N_SEARCH = 30
TRIM     = 3


# ── micro-benchmark helpers ───────────────────────────────────────────────────
def trimmed_stats(times: list, trim: int = TRIM) -> dict:
    times = sorted(times)
    if 2 * trim < len(times):
        times = times[trim:-trim]
    return {
        "min":    min(times),
        "median": statistics.median(times),
        "p95":    times[max(0, int(len(times) * 0.95) - 1)],
        "n":      len(times),
    }


def bench_loads(data: bytes, n: int = N_LOADS) -> dict:
    """Parse bytes → Python object (tape may or may not be used)."""
    for _ in range(5):
        strata.loads(data)
    gc.disable()
    times = []
    for _ in range(n + TRIM * 2):
        t0 = time.perf_counter()
        r  = strata.loads(data)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
        del r
    gc.enable()
    return trimmed_stats(times)


def bench_load_file(path: str, n: int = N_LOADS) -> dict:
    """Parse from file path (same C++ path as loads, tape applies)."""
    for _ in range(5):
        strata.load(path)
    gc.disable()
    times = []
    for _ in range(n + TRIM * 2):
        t0 = time.perf_counter()
        r  = strata.load(path)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
        del r
    gc.enable()
    return trimmed_stats(times)


def bench_dumps(data: bytes, n: int = N_DUMPS) -> dict:
    """Serialize obj → JSON str (no tape in serialization)."""
    obj = strata.loads(data)
    for _ in range(5):
        strata.dumps(obj)
    gc.disable()
    times = []
    for _ in range(n + TRIM * 2):
        t0 = time.perf_counter()
        r  = strata.dumps(obj)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
        del r
    gc.enable()
    del obj
    return trimmed_stats(times)


def bench_dump_bytes(data: bytes, n: int = N_DUMPS) -> dict:
    """Serialize obj → file-like (bytes output, no tape)."""
    obj = strata.loads(data)
    buf = io.BytesIO()
    for _ in range(5):
        buf.seek(0); buf.truncate()
        strata.dump(buf, obj)
    gc.disable()
    times = []
    for _ in range(n + TRIM * 2):
        buf.seek(0); buf.truncate()
        t0 = time.perf_counter()
        strata.dump(buf, obj)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
    gc.enable()
    del obj
    return trimmed_stats(times)


def bench_search(data: bytes, query_str: str, n: int = N_SEARCH) -> dict:
    """JSONPath on raw bytes — search; tape is hardcoded OFF in python_search.cpp."""
    for _ in range(3):
        strata.search(data, query_str)
    gc.disable()
    times = []
    for _ in range(n + TRIM * 2):
        t0 = time.perf_counter()
        strata.search(data, query_str)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
    gc.enable()
    return trimmed_stats(times)


def bench_query(data: bytes, query_str: str, n: int = N_SEARCH) -> dict:
    """JSONPath on pre-parsed Python object — no parse, no tape."""
    obj = strata.loads(data)
    for _ in range(3):
        strata.query(obj, query_str)
    gc.disable()
    times = []
    for _ in range(n + TRIM * 2):
        t0 = time.perf_counter()
        strata.query(obj, query_str)
        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)
    gc.enable()
    del obj
    return trimmed_stats(times)


def fmt(s: dict) -> str:
    return f"min={s['min']:7.2f}ms  med={s['median']:7.2f}ms  p95={s['p95']:7.2f}ms"


# ── main ──────────────────────────────────────────────────────────────────────
def main() -> None:
    tape_val = os.environ.get("STRATA_USE_STRUCTURAL_TAPE", "default(off)")
    print(f"\n{'='*72}")
    print(f"  STRATA_USE_STRUCTURAL_TAPE = {tape_val}")
    print(f"{'='*72}")

    for dsname, path in DATASETS.items():
        data = open(path, "rb").read()
        mb   = len(data) / 1e6
        print(f"\n── {dsname} ({mb:.1f} MB) {'─'*50}")

        # ── parse operations (tape DOES affect these) ──────────────────────
        r = bench_loads(data)
        print(f"  loads(bytes)             {fmt(r)}  [TAPE ACTIVE if ≥10MB]")

        r = bench_load_file(path)
        print(f"  load(path)               {fmt(r)}  [TAPE ACTIVE if ≥10MB]")

        # ── serialization (tape NEVER affects these) ───────────────────────
        r = bench_dumps(data)
        print(f"  dumps(obj→str)           {fmt(r)}  [no tape, serialization only]")

        r = bench_dump_bytes(data)
        print(f"  dump(file, obj→bytes)    {fmt(r)}  [no tape, serialization only]")

        # ── search on raw bytes (tape hardcoded OFF in C++ regardless of env) ─
        for qlabel, qstr in QUERIES:
            r = bench_search(data, qstr)
            short = qlabel[:20]
            print(f"  search {short:<20s}   {fmt(r)}  [tape always OFF]")

        # ── query on parsed object (no parse at all) ───────────────────────
        for qlabel, qstr in QUERIES:
            r = bench_query(data, qstr)
            short = qlabel[:20]
            print(f"  query  {short:<20s}   {fmt(r)}  [no tape, obj already parsed]")

    print()


if __name__ == "__main__":
    main()

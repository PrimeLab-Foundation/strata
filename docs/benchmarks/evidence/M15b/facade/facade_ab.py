"""M15b facade per-call cost: main's facade vs the current native=False/parse_types=False
fast path, in one process against one `strata._strata` build (precedent:
docs/benchmarks/evidence/M15/micro2/facade_kw.py).

Arm A -- main 38eaa9f's python/strata/serialize.py and python/strata/jsonpath.py (fetched with
`git show` so the exact merged source is used), exec'd with `_native` bound to this process's
`strata._strata` in place of their `from . import _strata as _native` line (relative imports do
not resolve outside a package, so that one line is stripped before exec). Main's facade has no
`native=`/`parse_types=` branch.
Arm B -- the installed `strata.serialize` / `strata.jsonpath` module functions (this checkout),
called with only default keywords, which take the `native is False` / `parse_types is False`
fast-path branch M15b added.

Every row calls with only the keywords the canonical benchmark harness passes
(benchmarks/bench_main.py): `loads`/`load`/`dump`/`query`/`search` take none; `dumps` passes
`return_type="bytes"` (bytes-to-bytes vs orjson, the one non-default keyword the canonical rows
use) -- "the path every canonical row takes".

Method: ABBA blocks of one batch each, batch size calibrated to ~1 ms from arm A's first call,
`gc.collect()` before every batch, 61 repeats. Reports A/B ns/call medians, the paired B-A median
difference, and its bootstrap 95% CI in ns and in percent of A.

usage: python facade_ab.py <small mixed.json path> [repeat=61]
"""

from __future__ import annotations

import gc
import json
import random
import statistics
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import strata
from strata import _strata as _native

MAIN_COMMIT = "38eaa9f"
TARGET_NS = 1_000_000  # ~1 ms per batch
REPEAT_DEFAULT = 61
BOOTSTRAP_N = 5000


def _repo_root() -> Path:
    out = subprocess.run(
        ["git", "rev-parse", "--show-toplevel"], check=True, capture_output=True, text=True
    )
    return Path(out.stdout.strip())


def _load_main_module(repo: Path, rel_path: str, module_name: str) -> dict:
    """Fetch `rel_path` as of MAIN_COMMIT and exec it with `_native` pre-bound.

    The only relative import in either file is `from . import _strata as _native`; it is
    stripped (the file's own `_hook_entry`/`_hook_module` helpers, which use the same pattern
    for `strata._dumps_hook`, are untouched and unused by the rows below).
    """
    src = subprocess.run(
        ["git", "show", f"{MAIN_COMMIT}:{rel_path}"],
        cwd=repo,
        check=True,
        capture_output=True,
        text=True,
    ).stdout
    lines = [
        line for line in src.splitlines() if line.strip() != "from . import _strata as _native"
    ]
    ns = {"__name__": module_name, "_native": _native}
    exec(compile("\n".join(lines), f"<{MAIN_COMMIT}:{rel_path}>", "exec"), ns)  # noqa: S102
    return ns


def uptime() -> str:
    return subprocess.run(["uptime"], check=True, capture_output=True, text=True).stdout.strip()


def calibrate(fn, target_ns: int) -> int:
    t0 = time.perf_counter_ns()
    fn()
    one = time.perf_counter_ns() - t0
    return max(1, int(target_ns / max(one, 1)))


def batch_median_ns(fn_a, fn_b, n: int, repeat: int) -> tuple[list[float], list[float]]:
    times_a: list[float] = []
    times_b: list[float] = []
    for r in range(repeat):
        order = (("A", fn_a, times_a), ("B", fn_b, times_b))
        if r % 2:
            order = order[::-1]
        for _label, fn, sink in order:
            gc.collect()
            t0 = time.perf_counter_ns()
            for _ in range(n):
                fn()
            sink.append((time.perf_counter_ns() - t0) / n)
    return times_a, times_b


def bootstrap_ci(diffs: list[float], n: int = BOOTSTRAP_N) -> tuple[float, float]:
    rng = random.Random(0)
    medians = []
    k = len(diffs)
    for _ in range(n):
        sample = [diffs[rng.randrange(k)] for _ in range(k)]
        medians.append(statistics.median(sample))
    medians.sort()
    lo = medians[int(0.025 * n)]
    hi = medians[int(0.975 * n) - 1]
    return lo, hi


def report_row(label: str, fn_a, fn_b, repeat: int) -> None:
    assert fn_a() == fn_b(), f"{label}: arm A and arm B disagree"
    n = calibrate(fn_a, TARGET_NS)
    times_a, times_b = batch_median_ns(fn_a, fn_b, n, repeat)
    diffs = [b - a for a, b in zip(times_a, times_b)]
    ma, mb = statistics.median(times_a), statistics.median(times_b)
    md = statistics.median(diffs)
    lo_ns, hi_ns = bootstrap_ci(diffs)
    lo_pct, hi_pct = lo_ns / ma * 100, hi_ns / ma * 100
    print(
        f"{label}: A {ma:.1f} ns/call, B {mb:.1f} ns/call, "
        f"paired B-A median {md:+.1f} ns [{lo_ns:+.1f}, {hi_ns:+.1f}] 95% CI "
        f"({md / ma * 100:+.3f}% [{lo_pct:+.3f}%, {hi_pct:+.3f}%]); n/batch={n} repeats={repeat}"
    )


def main() -> int:
    mixed_path = Path(sys.argv[1]) if len(sys.argv) > 1 else None
    repeat = int(sys.argv[2]) if len(sys.argv) > 2 else REPEAT_DEFAULT
    if mixed_path is None:
        raise SystemExit("usage: python facade_ab.py <small mixed.json path> [repeat=61]")

    print(f"uptime (before): {uptime()}")

    repo = _repo_root()
    main_serialize = _load_main_module(repo, "python/strata/serialize.py", "main_serialize")
    main_jsonpath = _load_main_module(repo, "python/strata/jsonpath.py", "main_jsonpath")
    main_loads = main_serialize["loads"]
    main_dumps = main_serialize["dumps"]
    main_load = main_serialize["load"]
    main_dump = main_serialize["dump"]
    main_query = main_jsonpath["query"]
    main_search = main_jsonpath["search"]

    mixed_bytes = mixed_path.read_bytes()
    mixed_obj = json.loads(mixed_bytes)
    tiny_obj = {"a": 1}
    tiny_bytes = b'{"a":1}'

    tmp_dir = Path(tempfile.mkdtemp(prefix="strata_facade_ab_"))
    tiny_file = tmp_dir / "tiny.json"
    tiny_file.write_text('{"a":1}\n')
    dump_file_a = tmp_dir / "dump_a.json"
    dump_file_b = tmp_dir / "dump_b.json"

    rows = [
        (
            "dumps(tiny) bytes",
            lambda: main_dumps(tiny_obj, return_type="bytes"),
            lambda: strata.dumps(tiny_obj, return_type="bytes"),
        ),
        (
            "loads(tiny bytes)",
            lambda: main_loads(tiny_bytes),
            lambda: strata.loads(tiny_bytes),
        ),
        (
            "dump(tiny -> tmp file)",
            lambda: main_dump(tiny_obj, dump_file_a),
            lambda: strata.dump(tiny_obj, dump_file_b),
        ),
        (
            "load(tiny .json)",
            lambda: main_load(tiny_file),
            lambda: strata.load(tiny_file),
        ),
        (
            'query(tiny, "$.a")',
            lambda: main_query(tiny_obj, "$.a"),
            lambda: strata.query(tiny_obj, "$.a"),
        ),
        (
            'search(tiny .json, "$.a")',
            lambda: main_search(tiny_file, "$.a"),
            lambda: strata.search(tiny_file, "$.a"),
        ),
        (
            "dumps(small mixed.json) bytes",
            lambda: main_dumps(mixed_obj, return_type="bytes"),
            lambda: strata.dumps(mixed_obj, return_type="bytes"),
        ),
        (
            "loads(small mixed.json bytes)",
            lambda: main_loads(mixed_bytes),
            lambda: strata.loads(mixed_bytes),
        ),
    ]

    for label, fn_a, fn_b in rows:
        report_row(label, fn_a, fn_b, repeat)

    print(f"uptime (after): {uptime()}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

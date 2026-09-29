"""strata.loads per call, this venv's facade + .so as installed: one process, one arm.

The cross-build twin of facade_kw.py: facade_kw.py holds the .so equal and swaps the
facade body; this runs each venv's own `strata.loads` (facade + .so) so the new
build is compared with main's actual build. Launch order and the per-process orjson
normaliser are admission.py's. Payloads: b'{"a":1}' (tiny) and small mixed.json.
One TSV line per sample: arm launch payload engine repeat ns_per_call
usage: python facade_x.py --arm new|main --launch L0 --mixed <small mixed.json> [--repeat 61]
"""

from __future__ import annotations

import argparse
import gc
import time

import orjson

import strata


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--arm", required=True)
    ap.add_argument("--launch", default="0")
    ap.add_argument("--mixed", required=True)
    ap.add_argument("--repeat", type=int, default=61)
    args = ap.parse_args()
    mixed = open(args.mixed, "rb").read()
    print(f"#meta arm={args.arm} strata={strata.__file__} orjson={orjson.__version__}", flush=True)
    for label, src in (("tiny", b'{"a":1}'), ("small-mixed", mixed)):
        engines = {
            "strata.loads": lambda src=src: strata.loads(src),
            "orjson.loads": lambda src=src: orjson.loads(src),
        }
        inner = {}
        for name, fn in engines.items():
            for _ in range(3):
                fn()
            t0 = time.perf_counter_ns()
            fn()
            inner[name] = max(1, int(3e6 / max(time.perf_counter_ns() - t0, 1)))
        names = list(engines)
        for r in range(args.repeat):
            order = names if r % 2 == 0 else names[::-1]
            for name in order:
                fn, n = engines[name], inner[name]
                gc.collect()
                t0 = time.perf_counter_ns()
                for _ in range(n):
                    fn()
                ns = (time.perf_counter_ns() - t0) / n
                print(f"{args.arm}\t{args.launch}\t{label}\t{name}\t{r}\t{ns:.3f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

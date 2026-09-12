"""Per-container cost of a nested value, strata against orjson.

`benchmarks/decompose_dumps_mixed.py` localised the Windows `dumps mixed`
deficit to the nested values rather than to the records or the scalars: with
mixed.json's containers removed strata leads, and with them it trails. This
probe prices one container at a time. Every document holds the same 500
records of two scalar keys; the variants add one value under a third key --
nothing, a scalar, a dict of N keys, a list of N elements -- so a difference
between two rows is the cost of that one value, and the scalar row separates
the type dispatch from the container walk.

usage: PYTHONPATH=. python benchmarks/nested_container_probe.py [repeat]
"""

from __future__ import annotations

import gc
import statistics
import sys
import time

import orjson

import strata

RECORDS = 500


def median_call(call, repeat: int = 60) -> float:
    for _ in range(3):
        call()
    samples = []
    for _ in range(repeat):
        gc.collect()
        started = time.perf_counter()
        call()
        samples.append((time.perf_counter() - started) * 1000.0)
    return statistics.median(samples)


def documents() -> dict[str, list]:
    def base(index: int) -> dict:
        return {"kind": f"k{index % 7}", "id": index}

    def with_value(make) -> list:
        return [{**base(i), "value": make(i)} for i in range(RECORDS)]

    docs = {"scalars-only": [base(i) for i in range(RECORDS)]}
    docs["value-float"] = with_value(lambda i: 0.11133106816568039 + i)
    docs["value-int"] = with_value(lambda i: 100000 + i)
    docs["value-str"] = with_value(lambda i: f"v{i}")
    for width in (1, 2, 4, 8, 16):
        docs[f"value-dict{width}"] = with_value(
            lambda i, w=width: {f"f{j}": (0.5 + i if j % 2 else j) for j in range(w)}
        )
    for length in (1, 2, 5, 16, 64):
        docs[f"value-list{length}"] = with_value(
            lambda i, n=length: [0.5 + i + j for j in range(n)]
        )
    # A value whose type rotates per record: the type ladder's branches see a
    # pattern no type-stable slot produces, which is what mixed.json has.
    docs["value-rotating"] = [
        {**base(i), "value": [{"a": 0.5, "b": i}, [i, i + 1], f"s{i}", i * 3.5][i % 4]}
        for i in range(RECORDS)
    ]
    return docs


def main() -> int:
    repeat = int(sys.argv[1]) if len(sys.argv) > 1 else 60
    docs = documents()
    base_s = base_o = None
    print(
        f"{'document':16s} {'strata ms':>10s} {'orjson ms':>10s} {'ratio':>7s} "
        f"{'strata ns/rec':>13s} {'orjson ns/rec':>13s}  delta vs scalars-only (ns/record)"
    )
    for name, doc in docs.items():
        s = median_call(lambda d=doc: strata.dumps(d, return_type="bytes"), repeat)
        o = median_call(lambda d=doc: orjson.dumps(d), repeat)
        if base_s is None:
            base_s, base_o = s, o
        ds = (s - base_s) * 1e6 / RECORDS
        do = (o - base_o) * 1e6 / RECORDS
        extra = (
            ""
            if name == "scalars-only"
            else f"strata {ds:+7.1f}  orjson {do:+7.1f}  gap {ds - do:+7.1f}"
        )
        print(
            f"{name:16s} {s:10.4f} {o:10.4f} {s / o:7.3f} "
            f"{s * 1e6 / RECORDS:13.1f} {o * 1e6 / RECORDS:13.1f}  {extra}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

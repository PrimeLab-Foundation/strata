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
import threading
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

    docs: dict[str, list] = {}
    docs["scalars-only"] = [base(i) for i in range(RECORDS)]
    # One more scalar key at the record's own depth, against one key inside a
    # nested dict below it: the pair separates the cost of a key from the cost
    # of opening a container at all.
    docs["third-scalar"] = [{**base(i), "value": i * 3} for i in range(RECORDS)]
    docs["value-dict0"] = with_value(lambda i: {})
    docs["value-list0"] = with_value(lambda i: [])
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


def on_a_fresh_thread(call):
    """The schema cache is per thread, so a pass that means to start cold, or
    to keep a retirement it caused, runs on a thread of its own."""
    box: list = []

    def run() -> None:
        box.append(call())

    thread = threading.Thread(target=run)
    thread.start()
    thread.join()
    return box[0]


def retire_nested_depth() -> None:
    """Present more distinct shapes at the nested depth than the cache holds
    ways for, past the 64 misses that retire it (python_dumps_output.h,
    DepthSchemas::select), so every later dict at that depth takes the general
    writer instead of the fused record writer."""
    churn = [
        {"kind": "c", "id": i, "value": {f"c{i}_{j}": j for j in range(1 + i % 3)}}
        for i in range(96)
    ]
    for _ in range(3):
        strata.dumps(churn, return_type="bytes")


def fused_against_general(repeat: int) -> None:
    """The same documents through both dict writers in one build: fresh on a
    thread of its own, then again after that thread's nested depth retires."""
    docs = {name: doc for name, doc in documents().items() if name.startswith("value-dict")}
    print()
    print(
        f"{'document':16s} {'fused ms':>9s} {'general ms':>11s} {'general-fused':>14s} "
        f"{'orjson ms':>10s}  ratios against orjson"
    )
    for name, doc in docs.items():

        def both(d=doc):
            fused = median_call(lambda: strata.dumps(d, return_type="bytes"), repeat)
            retire_nested_depth()
            general = median_call(lambda: strata.dumps(d, return_type="bytes"), repeat)
            return fused, general

        fused, general = on_a_fresh_thread(both)
        o = median_call(lambda d=doc: orjson.dumps(d), repeat)
        print(
            f"{name:16s} {fused:9.4f} {general:11.4f} "
            f"{(general - fused) * 1e6 / RECORDS:+13.1f}ns {o:10.4f}  "
            f"fused {fused / o:.3f}  general {general / o:.3f}"
        )


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
    fused_against_general(repeat)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

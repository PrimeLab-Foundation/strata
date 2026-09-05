"""Per-element parse cost by scalar kind: strata against orjson.

Lists of 2000 elements of one kind each -- nulls, bools, integers by width,
floats by shape, strings by width -- timed as whole documents (median of 30
rounds of 20 calls, gc.collect before each timed window, the result freed
inside it) and divided by the element count. The table is the parse side's
per-element floor: what one callback and one push cost against a rival's
walk, class by class, on the machine it runs on. Found the wave-21 string
tail (5-8-character strings 31.5 ns against 27.7) and is the first thing to
read on a platform whose number rows fall behind.

usage: PYTHONPATH=. .venv/bin/python benchmarks/parse_elements_probe.py
"""

import gc
import random
import statistics
import time

import orjson
import strata

N = 2000


def main() -> None:
    rng = random.Random(7)

    def lst(make):
        return ("[" + ",".join(make() for _ in range(N)) + "]").encode()

    words = ("alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel")
    docs = {
        "nulls": lst(lambda: "null"),
        "bools": lst(lambda: rng.choice(("true", "false"))),
        "ints 0-9": lst(lambda: str(rng.randrange(10))),
        "ints 100-999": lst(lambda: str(rng.randrange(100, 1000))),
        "ints 4dig": lst(lambda: str(rng.randrange(1000, 10000))),
        "ints 7dig": lst(lambda: str(rng.randrange(1000000, 10000000))),
        "ints 9-10dig": lst(lambda: str(rng.randrange(10**8, 10**10))),
        "floats 6dp": lst(lambda: "%.6f" % rng.uniform(-1000, 1000)),
        "floats 17dig": lst(lambda: repr(rng.random())),
        "strs 1ch": lst(lambda: '"a"'),
        "strs 8ch": lst(lambda: '"%08x"' % rng.randrange(1 << 32)),
        "strs 5-8 words": lst(lambda: '"%s"' % rng.choice(words)),
        "strs 32ch hex": lst(lambda: '"%032x"' % rng.randrange(1 << 128)),
    }

    def timed(fn, data, rounds=30, reps=20):
        out = []
        for _ in range(rounds):
            gc.collect()
            t0 = time.perf_counter()
            for _ in range(reps):
                r = fn(data)
                del r
            out.append((time.perf_counter() - t0) / reps / N * 1e9)
        return statistics.median(out)

    print(f"{'list of 2000':16s} {'strata':>8s} {'orjson':>8s}  ratio   (ns per element)")
    for name, data in docs.items():
        s = min(timed(strata.loads, data), timed(strata.loads, data))
        o = min(timed(orjson.loads, data), timed(orjson.loads, data))
        print(f"{name:16s} {s:8.2f} {o:8.2f}  {s / o:5.2f}x")


if __name__ == "__main__":
    main()

"""One scalar class per document, parsed in a loop, for perf stat.

usage: numclass_loop.py <lib|none> <class> <rounds>

The documents are the ones benchmarks/parse_elements_probe.py builds (same
seed, same shapes), so the per-element table and the counter runs describe the
same inputs. `none` as the library skips the parse entirely: its counters are
the interpreter constant that every other run also pays.
"""

import importlib
import random
import sys

N = 2000

CLASSES = {
    "nulls": lambda rng: "null",
    "int1": lambda rng: str(rng.randrange(10)),
    "int3": lambda rng: str(rng.randrange(100, 1000)),
    "int4": lambda rng: str(rng.randrange(1000, 10000)),
    "int7": lambda rng: str(rng.randrange(1000000, 10000000)),
    "int10": lambda rng: str(rng.randrange(10**8, 10**10)),
    "f6dp": lambda rng: "%.6f" % rng.uniform(-1000, 1000),
    "f17": lambda rng: repr(rng.random()),
}


def main() -> None:
    name, cls, rounds = sys.argv[1], sys.argv[2], int(sys.argv[3])
    rng = random.Random(7)
    make = CLASSES[cls if cls in CLASSES else "int3"]
    data = ("[" + ",".join(make(rng) for _ in range(N)) + "]").encode()
    if name == "none":
        for _ in range(rounds):
            pass
        return
    lib = importlib.import_module(name)
    for _ in range(rounds):
        r = lib.loads(data)
        del r


if __name__ == "__main__":
    main()

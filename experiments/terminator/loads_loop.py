"""Parse one dataset file in a loop, for perf stat.

usage: loads_loop.py <lib> <rounds> [dataset]
"""

import importlib
import sys


def main() -> None:
    lib = importlib.import_module(sys.argv[1])
    rounds = int(sys.argv[2])
    path = sys.argv[3] if len(sys.argv) > 3 else "benchmarks/data/generated/small/wide_arrays.json"
    with open(path, "rb") as handle:
        data = handle.read()
    for _ in range(rounds):
        r = lib.loads(data)
        del r


if __name__ == "__main__":
    main()

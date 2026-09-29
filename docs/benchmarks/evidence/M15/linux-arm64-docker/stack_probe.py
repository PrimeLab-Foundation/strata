"""Smallest thread stack (KiB, 16 KiB steps) at which each parse of the parse-cap documents succeeds."""

import subprocess
import sys

CHILD = r"""
import dataclasses, sys, threading
sys.path.insert(0, sys.argv[1])
import strata

@dataclasses.dataclass
class Nest:
    n: object

depth = 1023
chain = '{"n":' * depth + '"2024-01-01"' + "}" * depth
lists = "[" * 1024 + '"2024-01-01"' + "]" * 1024
case = sys.argv[3]
calls = {
    "plain-chain": lambda: strata.loads(chain),
    "revive-chain": lambda: strata.loads(chain, parse_types={"n": Nest}),
    "plain-lists": lambda: strata.loads(lists),
    "revive-lists": lambda: strata.loads(lists, parse_types=True),
}
box = {}
threading.stack_size(int(sys.argv[2]) * 1024)
thread = threading.Thread(target=lambda: box.setdefault("v", calls[case]()))
thread.start()
thread.join()
print("ok" if "v" in box else "no")
"""


def ok(root, kib, case):
    result = subprocess.run(
        [sys.executable, "-c", CHILD, root, str(kib), case],
        capture_output=True, text=True, timeout=120, check=False,
    )
    return result.returncode == 0 and result.stdout.strip() == "ok"


def smallest(root, case, lo=32, hi=8192, step=16):
    # binary search over multiples of step; assumes monotone
    lo_i, hi_i = lo // step, hi // step
    if not ok(root, hi_i * step, case):
        return None
    while lo_i < hi_i:
        mid = (lo_i + hi_i) // 2
        if ok(root, mid * step, case):
            hi_i = mid
        else:
            lo_i = mid + 1
    return lo_i * step


if __name__ == "__main__":
    root = sys.argv[1]
    for case in ("plain-chain", "revive-chain", "plain-lists", "revive-lists"):
        print(f"{case}: smallest stack {smallest(root, case)} KiB", flush=True)

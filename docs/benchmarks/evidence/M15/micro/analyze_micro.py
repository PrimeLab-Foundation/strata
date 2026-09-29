"""Read admission.tsv + admission.meta.txt (run_micro.sh) into one table.

Per kind: pooled median ns/object (and p25-p75) of strata-native (B dumps), strata-hook (main
dumps_with_default + ref), orjson (both arms pooled); native/hook raw ratio; the same ratio
normalised by each process's own orjson median (drift control across the two venvs, kinds
orjson supports); byte identity of native vs hook output; ref calls in B's dumps_with_default.
usage: python3 analyze_micro.py <micro dir>
"""

import collections
import statistics
import sys

d = sys.argv[1]
samples = collections.defaultdict(list)  # (kind, engine) -> [ns]
per_launch = collections.defaultdict(list)  # (launch, kind, engine) -> [ns]
arm_of = {}
for line in open(f"{d}/admission.tsv"):
    arm, launch, kind, engine, _rep, ns = line.rstrip("\n").split("\t")
    samples[(kind, engine)].append(float(ns))
    per_launch[(launch, kind, engine)].append(float(ns))
    arm_of[launch] = arm
outs = {}
calls = {}
for line in open(f"{d}/admission.meta.txt"):
    parts = line.split()
    if parts[1] == "#out":
        launch, kind, engine, sha = parts[0], parts[2], parts[3], parts[4]
        outs.setdefault((kind, engine), set()).add(sha)
        for p in parts:
            if p.startswith("ref_calls="):
                calls.setdefault((kind, engine), set()).add(int(p.split("=")[1]))

kinds = []
for kind, _e in samples:
    if kind not in kinds:
        kinds.append(kind)


def med(xs):
    return statistics.median(xs)


def iqr(xs):
    q = statistics.quantiles(xs, n=4)
    return q[0], q[2]


def launch_med(launch, kind, engine):
    xs = per_launch.get((launch, kind, engine))
    return med(xs) if xs else None


print(
    f"{'kind':<18}{'native':>9}{'(p25-p75)':>16}{'hook':>9}{'(p25-p75)':>16}{'orjson':>9}"
    f"{'nat/hook':>9}{'norm':>8}{'nat/orj':>8}  bytes  B-dwd ref calls  n"
)
for kind in kinds:
    nat = samples[(kind, "strata-native")]
    hook = samples[(kind, "strata-hook")]
    orj = samples.get((kind, "orjson"), [])
    raw = med(nat) / med(hook)
    norm = None
    if orj:
        rn = [
            launch_med(l, kind, "strata-native") / launch_med(l, kind, "orjson")
            for l, a in arm_of.items()
            if a == "native"
        ]
        rh = [
            launch_med(l, kind, "strata-hook") / launch_med(l, kind, "orjson")
            for l, a in arm_of.items()
            if a == "hook"
        ]
        norm = med(rn) / med(rh)
    same = outs[(kind, "strata-native")] == outs[(kind, "strata-hook")]
    n1, n2 = iqr(nat)
    h1, h2 = iqr(hook)
    print(
        f"{kind:<18}{med(nat):9.1f}{f'({n1:.1f}-{n2:.1f})':>16}{med(hook):9.1f}"
        f"{f'({h1:.1f}-{h2:.1f})':>16}{(f'{med(orj):9.1f}' if orj else '        -')}"
        f"{raw:9.3f}{(f'{norm:8.3f}' if norm else '       -')}"
        f"{(f'{med(nat) / med(orj):8.2f}' if orj else '       -')}  "
        f"{'same' if same else 'DIFF'}   {sorted(calls.get((kind, 'strata-dwd-B'), []))}"
        f"  {len(nat)}/{len(hook)}"
    )

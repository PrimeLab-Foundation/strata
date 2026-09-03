"""Allocation round trips, timed hot and interleaved, as the tier harness sees them.

Two Windows rows read against every isolated number: strata's `dumps mixed`
in bytes mode runs slower *hot* than its own str mode on both runner
families (Milan +15 us, Genoa +45 us) while Linux and the development
machine read bytes mode faster; and `loads mixed` reads bimodal -- strata's
minimum the best of five engines, its median 60% above it. Both point at
allocation round trips rather than at code: bytes mode writes into a fresh
output block sized from the previous document and shrinks it in place; a
parse builds a tree that the harness frees inside its timed window (the
call's result is dropped before the clock is read).

Every row is timed three ways, and the rows are interleaved the way the
harness interleaves libraries (one repeat of every row, then the next --
running a row's repeats back to back lands slow drift on whichever row
runs late, the harness's own lesson): *dropped* is the harness's window,
gc.collect() before the call and the result freed inside it; *retained*
keeps the result, so the window holds the call alone but every allocation
comes from fresh memory; *free only* builds the result outside the window
and times its release alone. Next to the engines stand allocation
controls of the bytes-mode sizes: `bytes(n)` (a calloc that touches every
page), a str block, and PyBytes blocks made through raw ctypes pointers
(a `py_object` wrapper keeps a stale pointer once a resize moves the
block, so no wrapper ever owns one): untouched at both sizes, the exact
shape bytes mode makes -- the hinted block, written, shrunk in place to
the document -- and the exact-fit shape a repeated document would take
without headroom, which is the candidate remedy if the shrunk block is
what the heap charges. Each timing list is printed
sorted, so a bimodal row shows both modes, and in call order, so drift
shows too. Pure timing; meant to be read from a CI log
(.github/workflows/profile.yml runs it on Windows and Linux).
"""

import ctypes
import gc
import json
import statistics
import time

REPEAT = 30
perf = time.perf_counter_ns


class Row:
    def __init__(self, name, make, release=None):
        self.name = name
        self.make = make
        self.release = release
        self.dropped: list[float] = []
        self.retained: list[float] = []
        self.freed: list[float] = []
        self.keep: list = []


def run_interleaved(rows, repeat):
    for row in rows:
        obj = row.make()
        if row.release:
            row.release(obj)
        del obj
    for _ in range(repeat):
        for row in rows:
            make, release = row.make, row.release
            gc.collect()
            start = perf()
            obj = make()
            if release:
                release(obj)
            del obj
            end = perf()
            row.dropped.append((end - start) / 1e3)
            gc.collect()
            start = perf()
            obj = make()
            end = perf()
            row.keep.append(obj)
            row.retained.append((end - start) / 1e3)
            gc.collect()
            obj = make()
            start = perf()
            if release:
                release(obj)
            del obj
            end = perf()
            row.freed.append((end - start) / 1e3)
    for row in rows:
        if row.release:
            for obj in row.keep:
                row.release(obj)
        row.keep.clear()


def show(name, arm, times):
    ordered = sorted(times)
    print(
        f"{name:40s} {arm:9s} min {ordered[0]:8.1f}  median {statistics.median(ordered):8.1f}  "
        f"max {ordered[-1]:8.1f} us"
    )
    print("    sorted: " + " ".join(f"{value:.0f}" for value in ordered))
    print("    in order: " + " ".join(f"{value:.0f}" for value in times))


def main() -> int:
    import msgspec
    import orjson

    import strata

    with open("benchmarks/data/generated/small/mixed.json", "rb") as handle:
        payload = handle.read()
    data = json.loads(payload)
    encode = msgspec.json.Encoder().encode
    decode = msgspec.json.Decoder().decode
    size = len(strata.dumps(data, return_type="bytes"))
    hint = size + size // 8 + 64

    # Raw object pointers throughout: no ctypes wrapper ever owns a block, so
    # nothing keeps a stale pointer when a resize moves one.
    api = ctypes.pythonapi
    new_bytes = api.PyBytes_FromStringAndSize
    new_bytes.argtypes = [ctypes.c_void_p, ctypes.c_ssize_t]
    new_bytes.restype = ctypes.c_void_p
    data_of = api.PyBytes_AsString
    data_of.argtypes = [ctypes.c_void_p]
    data_of.restype = ctypes.c_void_p
    resize = api._PyBytes_Resize
    resize.argtypes = [ctypes.POINTER(ctypes.c_void_p), ctypes.c_ssize_t]
    resize.restype = ctypes.c_int
    decref = api.Py_DecRef
    decref.argtypes = [ctypes.c_void_p]
    decref.restype = None

    def block(length, written, shrink_to=None):
        pointer = new_bytes(None, length)
        if written:
            ctypes.memset(data_of(pointer), 0x20, written)
        if shrink_to is not None:
            holder = ctypes.c_void_p(pointer)
            if resize(ctypes.byref(holder), shrink_to) != 0:
                raise MemoryError("_PyBytes_Resize failed")
            pointer = holder.value
        return pointer

    rows = [
        Row("dumps strata bytes", lambda: strata.dumps(data, return_type="bytes")),
        Row("dumps strata str", lambda: strata.dumps(data)),
        Row("dumps orjson", lambda: orjson.dumps(data)),
        Row("dumps msgspec", lambda: encode(data)),
        Row(f"bytes({size})", lambda: bytes(size)),
        Row(f"bytes({hint})", lambda: bytes(hint)),
        Row(f"PyBytes({hint}) untouched", lambda: block(hint, 0), decref),
        Row(f"PyBytes({size}) untouched", lambda: block(size, 0), decref),
        # Bytes mode's own shape: the hinted block, the document written into
        # it, the in-place shrink to the document; and the exact-fit shape a
        # repeated document would take if the hint carried no headroom.
        Row(f"PyBytes({hint}) written, shrunk to {size}", lambda: block(hint, size, size), decref),
        Row(f"PyBytes({size}) written, exact", lambda: block(size, size), decref),
        Row(f"str of {size}", lambda: "x" * size),
        Row("loads strata", lambda: strata.loads(payload)),
        Row("loads orjson", lambda: orjson.loads(payload)),
        Row("loads msgspec", lambda: decode(payload)),
    ]
    print(f"== output {size} bytes, bytes-mode block {hint}; {REPEAT} repeats, times in us")
    run_interleaved(rows, REPEAT)
    for row in rows:
        show(row.name, "dropped", row.dropped)
        show(row.name, "retained", row.retained)
        show(row.name, "free only", row.freed)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

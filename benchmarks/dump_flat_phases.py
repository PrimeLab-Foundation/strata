"""`dump flat` split into its phases, on the platform where the row lives.

The macos-arm64 leg reads a different serializer/file row behind orjson on
each CI sample; the third one was `dump flat` -- strata median 0.511 ms
against orjson's 0.474, but strata's *minimum* 0.408 against 0.423 and its
p95 1.253 against 0.695. A row whose minimum leads and whose median and p95
trail is a tail, not a slower code path, so ranks cannot say what to fix.
This probe times the phases separately instead.

`dump` is one C call that does six things: serialize into an exact-fit
bytes object, `open(O_WRONLY|O_CREAT|O_TRUNC, 0644)`, one `writev` of the
document plus the documented trailing newline, `fstat` and a conditional
`fchmod` for the documented exact 0644 mode, `close`, and the release of
the intermediate bytes. The competitor composition is
`open(path, "wb").write(lib.dumps(obj))`. Section 0 reruns the CI row itself
-- all five libraries interleaved, one file each -- and is where the recorded
0.90-0.98x medians come from; section 1 times the whole calls against each
other; section 2 replays strata's syscall sequence from Python
with each syscall timed on its own, next to four alternative write shapes:
one `write` of a pre-joined buffer, two `write`s, the sequence without the
mode enforcement, and a fresh file (unlink then `O_EXCL` create) that
prices `O_TRUNC` on an existing APFS file. Section 3 prices `fchmod` alone,
since the shipped code only reaches it when the mode actually differs.

All sections use the tier harness's window and file placement: every row
writes its own file inside one TemporaryDirectory, `gc.collect()` runs
before each timed call, and a call's result is released inside the window
(benchmarks/harness.py). Per-syscall numbers carry one Python call of
overhead each (~0.1 us) -- they are for comparing shapes against each
other, section 0 is the row-level ground truth and section 1 isolates `dump`
against its composition. Pure timing, meant to
be read from a CI log (.github/workflows/profile.yml).
"""

import gc
import json
import os
import statistics
import tempfile
import time
from pathlib import Path

REPEAT = 60
WARMUP = 3
DATASET = "benchmarks/data/generated/small/flat.json"
DOCUMENTED_MODE = 0o644
perf = time.perf_counter_ns


def show(name, times, unit="us"):
    """min / median / p95 / max, then both orderings: a tail shows in one."""
    ordered = sorted(times)
    index = max(0, min(len(ordered) - 1, round(0.95 * (len(ordered) - 1))))
    print(
        f"{name:44s} min {ordered[0]:9.1f}  median {statistics.median(ordered):9.1f}  "
        f"p95 {ordered[index]:9.1f}  max {ordered[-1]:9.1f} {unit}"
    )
    print("    sorted:   " + " ".join(f"{value:.0f}" for value in ordered))
    print("    in order: " + " ".join(f"{value:.0f}" for value in times))


class Row:
    def __init__(self, name, call):
        self.name = name
        self.call = call
        self.times: list[float] = []


def run_rows(rows, repeat, warmup):
    """The harness's interleave: one repeat of every row, then the next."""
    for _ in range(warmup):
        for row in rows:
            row.call()
    for _ in range(repeat):
        for row in rows:
            call = row.call
            gc.collect()
            start = perf()
            result = call()
            del result
            end = perf()
            row.times.append((end - start) / 1e3)


class Shape:
    """One write sequence, timed syscall by syscall."""

    def __init__(self, name, target, run):
        self.name = name
        self.target = target
        self.run = run
        self.phases: dict[str, list[float]] = {}

    def mark(self, phase, elapsed_ns):
        self.phases.setdefault(phase, []).append(elapsed_ns / 1e3)


def strata_shape(shape, data, newline_data):
    """The shipped POSIX sequence in src/strata/bindings/python_files.cpp."""
    start = perf()
    fd = os.open(shape.target, os.O_WRONLY | os.O_CREAT | os.O_TRUNC | os.O_CLOEXEC, 0o644)
    shape.mark("open O_TRUNC", perf() - start)
    start = perf()
    os.writev(fd, [data, newline_data])
    shape.mark("writev 2 parts", perf() - start)
    start = perf()
    status = os.fstat(fd)
    shape.mark("fstat", perf() - start)
    if (status.st_mode & 0o7777) != DOCUMENTED_MODE:
        start = perf()
        os.fchmod(fd, DOCUMENTED_MODE)
        shape.mark("fchmod (fired)", perf() - start)
    start = perf()
    os.close(fd)
    shape.mark("close", perf() - start)


def joined_shape(shape, joined, _newline_data):
    """Candidate: one write of a buffer that already carries the newline."""
    start = perf()
    fd = os.open(shape.target, os.O_WRONLY | os.O_CREAT | os.O_TRUNC | os.O_CLOEXEC, 0o644)
    shape.mark("open O_TRUNC", perf() - start)
    start = perf()
    os.write(fd, joined)
    shape.mark("write 1 buffer", perf() - start)
    start = perf()
    status = os.fstat(fd)
    shape.mark("fstat", perf() - start)
    if (status.st_mode & 0o7777) != DOCUMENTED_MODE:
        start = perf()
        os.fchmod(fd, DOCUMENTED_MODE)
        shape.mark("fchmod (fired)", perf() - start)
    start = perf()
    os.close(fd)
    shape.mark("close", perf() - start)


def two_writes_shape(shape, data, newline_data):
    """The pre-writev shape: the newline as its own syscall."""
    start = perf()
    fd = os.open(shape.target, os.O_WRONLY | os.O_CREAT | os.O_TRUNC | os.O_CLOEXEC, 0o644)
    shape.mark("open O_TRUNC", perf() - start)
    start = perf()
    os.write(fd, data)
    shape.mark("write document", perf() - start)
    start = perf()
    os.write(fd, newline_data)
    shape.mark("write newline", perf() - start)
    start = perf()
    os.close(fd)
    shape.mark("close", perf() - start)


def no_mode_shape(shape, data, newline_data):
    """Strata's sequence minus the mode enforcement: the price of the check."""
    start = perf()
    fd = os.open(shape.target, os.O_WRONLY | os.O_CREAT | os.O_TRUNC | os.O_CLOEXEC, 0o644)
    shape.mark("open O_TRUNC", perf() - start)
    start = perf()
    os.writev(fd, [data, newline_data])
    shape.mark("writev 2 parts", perf() - start)
    start = perf()
    os.close(fd)
    shape.mark("close", perf() - start)


def fresh_file_shape(shape, data, newline_data):
    """Unlink then create: prices O_TRUNC of an existing file on this filesystem."""
    start = perf()
    os.unlink(shape.target)
    shape.mark("unlink", perf() - start)
    start = perf()
    fd = os.open(shape.target, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_CLOEXEC, 0o644)
    shape.mark("open O_EXCL", perf() - start)
    start = perf()
    os.writev(fd, [data, newline_data])
    shape.mark("writev 2 parts", perf() - start)
    start = perf()
    os.close(fd)
    shape.mark("close", perf() - start)


def competitor_shape(shape, joined, _newline_data):
    """The competitor composition's own file phases (serialization excluded)."""
    start = perf()
    handle = open(shape.target, "wb")  # noqa: SIM115
    shape.mark("python open wb", perf() - start)
    start = perf()
    handle.write(joined)
    shape.mark("handle.write", perf() - start)
    start = perf()
    handle.close()
    shape.mark("handle.close", perf() - start)


def run_shapes(shapes, payloads, repeat, warmup):
    # Every shape starts from an existing file of the right size: that is the
    # steady state the harness measures (its warmup rounds have already
    # written each target), and the unlink shape needs one to remove.
    for shape in shapes:
        with open(shape.target, "wb") as handle:
            handle.write(payloads[shape.name][0])
    for _ in range(warmup):
        for shape in shapes:
            shape.run(shape, *payloads[shape.name])
    for shape in shapes:
        shape.phases.clear()
    for _ in range(repeat):
        for shape in shapes:
            gc.collect()
            shape.run(shape, *payloads[shape.name])


def main() -> int:
    import orjson

    import strata

    payload = Path(DATASET).read_bytes()
    value = json.loads(payload)
    data = strata.dumps(value, return_type="bytes")
    newline_data = b"\n"
    joined = data + newline_data
    print(f"== {DATASET}: {len(payload)} bytes on disk, {len(data)} bytes serialized")
    mask = os.umask(0)
    os.umask(mask)
    print(f"== {REPEAT} repeats, {WARMUP} warmup, process umask {mask:#o}")

    with tempfile.TemporaryDirectory() as scratch:
        out = Path(scratch)
        strata_target = str(out / "strata.json")
        orjson_target = out / "orjson.json"

        def strata_dump():
            strata.dump(value, strata_target)

        def orjson_composition():
            with open(orjson_target, "wb") as handle:
                handle.write(orjson.dumps(value))

        # Section 0 is the CI row itself: the same five callables
        # bench_main._dump_callables builds, in the same order, interleaved
        # the same way. Whether the row reads behind at all on this runner --
        # and by how much, twice -- has to come from the row, not from a
        # two-way comparison that never sees the other three engines' cache
        # and heap state.
        import msgspec
        import ujson

        msgspec_encode = msgspec.json.Encoder().encode

        def serialize_then_write(name, serialize, binary):
            target = out / name
            mode = "wb" if binary else "w"

            def call():
                with open(target, mode) as handle:
                    handle.write(serialize(value))

            return call

        full = [
            Row("strata", strata_dump),
            Row("orjson", serialize_then_write("orjson.json", orjson.dumps, True)),
            Row("msgspec", serialize_then_write("msgspec.json", msgspec_encode, True)),
            Row("ujson", serialize_then_write("ujson.json", ujson.dumps, False)),
            Row(
                "json",
                serialize_then_write(
                    "json.json", lambda v: json.dumps(v, separators=(",", ":")), False
                ),
            ),
        ]
        print(f"\n== section 0: the `dump flat` row itself, five libraries interleaved, us")
        run_rows(full, REPEAT, WARMUP)
        for row in full:
            show(f"dump flat | {row.name}", row.times)

        rows = [
            Row("strata.dump (total)", strata_dump),
            Row("orjson open+write (total)", orjson_composition),
            Row("strata.dumps bytes (serialize)", lambda: strata.dumps(value, return_type="bytes")),
            Row("orjson.dumps (serialize)", lambda: orjson.dumps(value)),
        ]
        print(f"\n== section 1: whole calls, harness window, times in us ({REPEAT} repeats)")
        run_rows(rows, REPEAT, WARMUP)
        for row in rows:
            show(row.name, row.times)

        shapes = [
            Shape("strata sequence", str(out / "shape_strata.json"), strata_shape),
            Shape("one write, joined buffer", str(out / "shape_joined.json"), joined_shape),
            Shape("two writes", str(out / "shape_two.json"), two_writes_shape),
            Shape("no mode enforcement", str(out / "shape_nomode.json"), no_mode_shape),
            Shape("fresh file (unlink, O_EXCL)", str(out / "shape_fresh.json"), fresh_file_shape),
            Shape("competitor file phases", str(out / "shape_python.json"), competitor_shape),
        ]
        payloads = {
            "strata sequence": (data, newline_data),
            "one write, joined buffer": (joined, newline_data),
            "two writes": (data, newline_data),
            "no mode enforcement": (data, newline_data),
            "fresh file (unlink, O_EXCL)": (data, newline_data),
            "competitor file phases": (joined, newline_data),
        }
        print(
            f"\n== section 2: file phases, one syscall per timing, times in us ({REPEAT} repeats)"
        )
        run_shapes(shapes, payloads, REPEAT, WARMUP)
        for shape in shapes:
            total = [0.0] * REPEAT
            for phase, times in shape.phases.items():
                show(f"{shape.name} | {phase}", times)
                if len(times) == REPEAT:
                    total = [a + b for a, b in zip(total, times)]
            show(f"{shape.name} | SUM of phases", total)
            fired = len(shape.phases.get("fchmod (fired)", ()))
            print(f"    fchmod fired on {fired} of {REPEAT} rounds")

        print("\n== section 3: mode enforcement priced alone, times in us")
        chmod_target = str(out / "chmod.json")
        fd = os.open(chmod_target, os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o644)
        fstat_only: list[float] = []
        fchmod_forced: list[float] = []
        for _ in range(REPEAT):
            gc.collect()
            start = perf()
            os.fstat(fd)
            fstat_only.append((perf() - start) / 1e3)
            os.fchmod(fd, 0o600)
            start = perf()
            os.fchmod(fd, DOCUMENTED_MODE)
            fchmod_forced.append((perf() - start) / 1e3)
        os.close(fd)
        show("fstat on the open descriptor", fstat_only)
        show("fchmod when it has to fire", fchmod_forced)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

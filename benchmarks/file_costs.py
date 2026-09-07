"""Real-file phase controls; diagnostic timings, not a subtraction of medians.

The Python syscall composition is a control, not a native Strata profile.
Its per-phase timers add overhead, so these samples cannot certify a small
production gain. Exact output bytes and the POSIX mode are checked separately.
"""

from __future__ import annotations

import argparse
import gc
import json
import os
import platform
import stat
import tempfile
import time
from pathlib import Path

import strata

from benchmarks.provenance import capture


def write_all(fd: int, payload: bytes) -> None:
    remaining = memoryview(payload)
    while remaining:
        count = os.write(fd, remaining)
        if count <= 0:
            raise OSError("write made no progress")
        remaining = remaining[count:]


def phases(value, path: Path) -> dict:
    timestamps = [time.perf_counter_ns()]
    payload = strata.dumps(value, return_type="bytes")
    timestamps.append(time.perf_counter_ns())
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_TRUNC | getattr(os, "O_BINARY", 0), 0o644)
    try:
        timestamps.append(time.perf_counter_ns())
        write_all(fd, payload)
        write_all(fd, b"\n")
        timestamps.append(time.perf_counter_ns())
        if os.name != "nt" and stat.S_IMODE(os.fstat(fd).st_mode) != 0o644:
            os.fchmod(fd, 0o644)
        timestamps.append(time.perf_counter_ns())
    finally:
        os.close(fd)
    timestamps.append(time.perf_counter_ns())
    return {
        name: (end - start) / 1e6
        for name, start, end in zip(
            ("serialize", "open_truncate", "write_and_newline", "metadata", "close"),
            timestamps,
            timestamps[1:],
        )
    }


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dataset", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--repeat", type=int, default=60)
    args = parser.parse_args(argv)
    if args.repeat < 1:
        parser.error("repeat must be positive")
    value = json.loads(args.dataset.read_bytes())
    packet = {
        "schema_version": 1,
        "scope": "file-phase-controls-v1",
        "provenance": capture([args.dataset], {"strata": strata}, repeat=args.repeat, warmup=1),
        "os": platform.platform(),
        "machine": platform.machine(),
        "load_average": os.getloadavg() if hasattr(os, "getloadavg") else None,
        "samples": [],
        "limitation": __doc__,
    }
    packet["provenance"]["protocol"].update(
        {
            "name": "file-phase-controls-v1",
            "timer": "perf_counter_ns converted to ms",
            "preflight_calls": 0,
            "rss": "not collected",
            "order": "native/control on even samples; control/native on odd samples",
            "gc": "collect before each complete native or control operation",
        }
    )
    with tempfile.TemporaryDirectory() as temporary:
        root = Path(temporary)
        native, control = root / "native.json", root / "control.json"
        strata.dump(value, native)
        phases(value, control)
        if native.read_bytes() != control.read_bytes():
            raise ValueError("file control bytes differ from Strata")
        if os.name != "nt":
            for path in (native, control):
                path.chmod(0o600)
        packet["filesystem"] = {"temporary_directory": str(root), "device": root.stat().st_dev}
        for index in range(args.repeat):
            # Alternation counters monotonic drift; neither timer is a deciding
            # canonical comparison against the five-engine workload.
            sample = {"order_id": index}
            for operation in ("native", "control") if index % 2 == 0 else ("control", "native"):
                gc.collect()
                if operation == "control":
                    sample["control_ms"] = phases(value, control)
                else:
                    start = time.perf_counter_ns()
                    strata.dump(value, native)
                    sample["native_dump_ms"] = (time.perf_counter_ns() - start) / 1e6
            packet["samples"].append(sample)
        if native.read_bytes() != control.read_bytes():
            raise ValueError("file control bytes differ after measurement")
        if os.name != "nt" and any(
            stat.S_IMODE(p.stat().st_mode) != 0o644 for p in (native, control)
        ):
            raise ValueError("file control or Strata did not restore mode 0644")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(packet, indent=2, allow_nan=False) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

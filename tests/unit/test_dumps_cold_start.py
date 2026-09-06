"""Contract tests for what `dumps` may do on a *cold* process.

`python_dumps.cpp`'s header enumerates the steps at which the serializer runs
user code, and `docs/context/api.md` states the same thing to users. The
enumeration is only true if nothing on the walk resolves lazily: a GC-tracked
allocation schedules a collection, a collection runs `__del__` and weakref
callbacks, and those are user code at a point that takes no reference —
beneath containers the walk only borrows, and beneath an all-scalar dict that
frame elision keeps off the open list entirely.

The serializer had exactly one such step, and only on the first call of a
process: the raw-dict layout proof is a function-local static whose
initialiser builds two dicts, and its first use is in the middle of the walk.
`prepare_dumps_runtime()` resolves it at module init instead
(build/evidence/FIX1-REVIEW, finding F-A).

Every test here therefore runs in a **fresh interpreter**: in this one the
first `dumps` call is the first `dumps` call. A warm process cannot see the
difference, which is why nothing caught it — every test process runs many
`dumps` calls before it reaches an adversarial one.
"""

import json
import os
import subprocess
import sys

import pytest

MODES = ("str", "bytes")

# The load-bearing one. On the unfixed serializer this script is a
# deterministic SIGSEGV: the layout probe's two dicts schedule a collection
# from inside the walk, the collection's finalizers empty the containers the
# walk borrowed, and the next element read is freed memory. Cheap to state,
# and it kills the interpreter rather than returning a wrong answer, so it
# cannot pass by accident.
# The load-bearing one, verbatim from build/evidence/FIX1-REVIEW/repro/
# probe_gc_segv.py (the review that found F-A). Three documents that fail, two
# output modes, one interpreter: the walk of the first of them is the first
# walk of the process. Unfixed, this is a deterministic SIGSEGV — the layout
# probe's two dicts schedule a collection from inside that walk, the
# collection's finalizers empty the containers the walk borrowed, and the next
# read is freed memory. Kept byte-for-byte rather than paraphrased: which
# allocation lands where is what makes it deterministic.
_PENDING_FINALIZERS_SCRIPT = """
import gc, sys
import strata

fired = []


class Fin:
    def __init__(self, target):
        self.target = target

    def __del__(self):
        fired.append(1)
        for node in self.target:
            try:
                node.clear()
            except Exception:
                pass


def build(kind):
    if kind == "maxdigits":
        sys.set_int_max_str_digits(640)
        leaf = {"a": 1, "b": 10 ** 6000, "c": 3}
    elif kind == "badkey":
        leaf = {"a": 1, 2.5: 3}
    else:
        leaf = {"a": 1, "b": object()}
    chain = []
    node = [leaf]
    for _ in range(30):
        node = [node, {"k": 1}]
        chain.append(node)
    return node, chain


for kind in ("maxdigits", "badkey", "badvalue"):
    for mode in ("str", "bytes"):
        root, chain = build(kind)
        gc.disable()
        garbage = []
        for _ in range(500):
            f = Fin(chain)
            f.self_ref = f
            garbage.append(f)
        garbage.clear()
        del garbage
        before = len(fired)
        gc.set_threshold(1, 1, 1)
        gc.enable()
        try:
            strata.dumps(root, return_type=mode)
            err = "none"
        except Exception as exc:
            err = type(exc).__name__
        gc.set_threshold(700, 10, 10)
        print("%-10s %-6s err=%-14s finalizers-during=%d" % (kind, mode, err, len(fired) - before))
        gc.collect()
print("survived")
"""

# The success-path half: the same cold process, a document that serializes
# cleanly, and the whole of it must come out. (On CPython 3.13+ a single
# scheduled collection promotes the pending cycles rather than finalizing
# them, so this half is a guard rather than a reproducer — the script above
# is the one that fails on an unfixed build.)
_WHOLE_DOCUMENT_SCRIPT = """
import gc, json, sys
import strata

fired = []


class Clearer:
    def __init__(self, targets):
        self.targets = targets
        self.loop = self

    def __del__(self):
        fired.append(1)
        for node in self.targets:
            node.clear()


targets = []
node = [{"a": 1, "b": 2, "c": 3}]
for _ in range(30):
    node = [node, {"k": 1}]
    targets.append(node)
document = node

gc.disable()
expected = json.dumps(document, separators=(",", ":"))
garbage = [Clearer(targets) for _ in range(500)]
garbage.clear()
del garbage
gc.set_threshold(1, 1, 1)
gc.enable()

pending = len(fired)
cold = strata.dumps(document, return_type=sys.argv[1])
cold_fired = len(fired) - pending
warm_before = len(fired)
warm = strata.dumps(document, return_type=sys.argv[1])
warm_fired = len(fired) - warm_before
gc.set_threshold(700, 10, 10)

if isinstance(cold, bytes):
    cold = cold.decode()
    warm = warm.decode()
sys.stdout.write(json.dumps({
    "pending": pending,
    "expected": expected,
    "cold": cold,
    "cold_fired": cold_fired,
    "warm": warm,
    "warm_fired": warm_fired,
}))
"""


def _run_text(script, *args):
    """Run `script` in a fresh interpreter that imports the same strata.

    The import path is planted *inside* the child rather than through
    PYTHONPATH: under the build gate pip's isolated environment installs a
    sitecustomize that rewrites sys.path at startup and drops the staging
    directory holding the freshly built extension (scripts/py_tests.py says
    the same thing about its own bootstrap).
    """
    entries = [entry or os.getcwd() for entry in sys.path]
    bootstrap = f"import sys\nsys.path[:0] = {entries!r}\n{script}"
    result = subprocess.run(
        [sys.executable, "-c", bootstrap, *args],
        capture_output=True,
        text=True,
        check=False,
    )
    assert result.returncode == 0, (
        f"fresh interpreter exited {result.returncode!r}\n{result.stderr}"
    )
    return result.stdout


def _run(script, *args):
    """_run_text, for a child that reports one JSON object."""
    return json.loads(_run_text(script, *args))


EXPECTED_ERRORS = [
    ("maxdigits", "str", "ValueError"),
    ("maxdigits", "bytes", "ValueError"),
    ("badkey", "str", "TypeError"),
    ("badkey", "bytes", "TypeError"),
    ("badvalue", "str", "TypeError"),
    ("badvalue", "bytes", "TypeError"),
]


def test_a_cold_process_reads_no_freed_memory_with_finalizers_pending():
    """The walk must survive a collection that empties it, on a cold process.

    These documents fail, and a failing walk *is* allowed to run finalizers:
    the exception it raises is GC-tracked, so it can schedule a collection
    (python_dumps.cpp, "Error paths do not need the latch" — nothing is
    dereferenced after `return false`). What may not happen is what the
    layout probe used to add on top: a collection scheduled from the middle
    of the *first* walk of the process, under rows and containers that walk
    only borrowed. All six cases run in one interpreter deliberately: it is
    the first of them that is cold, and the crash is what the later ones
    inherit. On an unfixed serializer this child exits 139 (SIGSEGV), which
    `_run` reports; here it must come back with the documented errors.
    """
    lines = _run_text(_PENDING_FINALIZERS_SCRIPT).splitlines()
    assert lines[-1] == "survived", lines
    assert len(lines) == len(EXPECTED_ERRORS) + 1, lines
    for (kind, mode, error), line in zip(EXPECTED_ERRORS, lines, strict=False):
        assert line.split()[:2] == [kind, mode], line
        assert f"err={error}" in line, line


@pytest.mark.parametrize("mode", MODES)
def test_a_cold_process_emits_the_whole_document(mode):
    """The first call must be no different from the second — the point of
    resolving the probe at import is that there is no first-call behaviour."""
    report = _run(_WHOLE_DOCUMENT_SCRIPT, mode)
    assert report["pending"] == 0, "the finalizers ran before the call was made"
    assert report["cold_fired"] == 0, report
    assert report["warm_fired"] == 0, report
    assert report["cold"] == report["expected"]
    assert report["warm"] == report["expected"]

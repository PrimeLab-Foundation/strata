"""`dumps_with_default`'s `native` mode belongs to the call across greenlet switches.

api.md, dumps_with_default: "The mode belongs to the call: a call made from
inside `default` has its own, and so has every other thread or greenlet"
(docs/decisions.md, 2026-09-29: the mode is a context variable, not a
thread-local, because a greenlet switch inside `default` interleaves two walks
of different modes on one OS thread, and greenlet gives each greenlet its own
context). Behaviour pin 7 of the opt-out's specification: a thread-local mode
fails these tests. greenlet comes from the `integrations` extra.
"""

import datetime as dt
import json

import pytest
from greenlet import greenlet

import strata

MODES = ("str", "bytes")
STAMP = dt.datetime(2026, 9, 29, 12, 30, 5, tzinfo=dt.timezone.utc)
LATER = dt.datetime(2026, 9, 30, 8, 0, tzinfo=dt.timezone.utc)


class Hop:
    """A non-native object whose `default` call switches to the other walk."""

    def __init__(self, tag):
        self.tag = tag


def tag(obj):
    return obj.tag if isinstance(obj, Hop) else f"{type(obj).__name__}:{obj.isoformat()}"


def expected(document, natives_on):
    items = [
        strata.dumps(obj, native=True)
        if natives_on and not isinstance(obj, Hop)
        else json.dumps(tag(obj))
        for obj in document
    ]
    return "[" + ",".join(items) + "]"


def called_for(document, natives_on):
    return [obj for obj in document if isinstance(obj, Hop) or not natives_on]


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("a_native", [False, True], ids=["A-opts-out", "B-opts-out"])
def test_two_walks_interleaved_by_greenlet_switches_keep_their_own_modes(mode, a_native):
    # api.md, dumps_with_default: "... and so has every other ... greenlet".
    # Walk A switches inside its `default` to walk B, whose `default` switches
    # back to A; A then meets a datetime in its own mode, and switches to B
    # again; B resumes, with A's walk still in progress, and meets datetimes in
    # its own mode.
    b_native = not a_native
    a_document = [Hop("a1"), STAMP, Hop("a2")]
    b_document = [Hop("b1"), STAMP, LATER]
    seen_a, seen_b, results = [], [], {}

    def a_default(obj):
        seen_a.append(obj)
        if isinstance(obj, Hop):
            # a1 starts B; a2 resumes B, which then runs to its end and hands
            # its result back here (A is B's parent).
            walk_b.switch()
        return tag(obj)

    def b_default(obj):
        seen_b.append(obj)
        if isinstance(obj, Hop):
            walk_a.switch()
        return tag(obj)

    def run_a():
        return strata.dumps_with_default(a_document, a_default, return_type=mode, native=a_native)

    def run_b():
        results["b"] = strata.dumps_with_default(
            b_document,
            b_default,
            return_type=mode,
            native=b_native,
        )

    walk_a = greenlet(run_a)
    walk_b = greenlet(run_b, parent=walk_a)
    results["a"] = walk_a.switch()

    assert walk_a.dead
    assert walk_b.dead
    for name, document, natives_on, seen in (
        ("a", a_document, a_native, seen_a),
        ("b", b_document, b_native, seen_b),
    ):
        out = results[name]
        assert (out.decode() if isinstance(out, bytes) else out) == expected(document, natives_on)
        assert len(seen) == len(called_for(document, natives_on))
        assert all(x is y for x, y in zip(seen, called_for(document, natives_on), strict=True))


class SwitchingZone(dt.tzinfo):
    """UTC, whose first `utcoffset` call switches greenlets: a native step's user code."""

    def __init__(self):
        self.target = None

    def utcoffset(self, when):
        target, self.target = self.target, None
        if target is not None:
            target.switch()
        return dt.timedelta(0)

    def dst(self, when):
        return None


@pytest.mark.parametrize("mode", MODES)
def test_a_switch_from_a_native_step_keeps_each_walk_in_its_own_mode(mode):
    # api.md, dumps_with_default: "... and so has every other ... greenlet".
    # Reviewer's variant: B (natives on) switches back to A from inside a
    # native conversion -- its tzinfo's `utcoffset` -- not from `default`, so a
    # design that re-asserts a thread-local mode around each `default` call
    # would resume A in B's mode and write A's datetime natively.
    zone = SwitchingZone()
    a_document = [Hop("a1"), STAMP, Hop("a2")]
    b_document = [dt.datetime(2026, 9, 29, 6, 0, tzinfo=zone), LATER]
    seen_a, results = [], {}

    def a_default(obj):
        seen_a.append(obj)
        if isinstance(obj, Hop):
            walk_b.switch()
        return tag(obj)

    def b_default(obj):
        raise AssertionError(f"natives-on walk B called default for {obj!r}")

    def run_a():
        return strata.dumps_with_default(a_document, a_default, return_type=mode, native=False)

    def run_b():
        results["b"] = strata.dumps_with_default(b_document, b_default, return_type=mode)

    walk_a = greenlet(run_a)
    walk_b = greenlet(run_b, parent=walk_a)
    zone.target = walk_a
    results["a"] = walk_a.switch()

    assert walk_a.dead
    assert walk_b.dead
    assert zone.target is None
    text_a, text_b = (
        r.decode() if isinstance(r, bytes) else r for r in (results["a"], results["b"])
    )
    assert text_a == expected(a_document, natives_on=False)
    assert seen_a == a_document
    assert text_b == strata.dumps(b_document, native=True)

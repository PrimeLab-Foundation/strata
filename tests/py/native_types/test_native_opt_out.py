"""Integration tests for `dumps_with_default(..., native=False)`: the mode belongs to the call.

api.md, dumps_with_default: "The mode belongs to the call: a call made from
inside `default` has its own, and so has every other thread or greenlet"
(docs/decisions.md, 2026-09-29: the opt-out's state is a context variable plus
a process-wide count of opt-out walks in progress). Here: a seeded stdlib
oracle corpus, and behaviour pins 5 (nesting on one thread), 6 (threads),
8 (failing walks) and 9 (a context copied inside an opt-out walk). The clause
tests (pins 1-4) are in tests/unit/native_types/test_native_opt_out.py; the
greenlet interleave (pin 7) is in
tests/integrations/test_native_opt_out_greenlet.py.
"""

import contextvars
import dataclasses
import datetime as dt
import enum
import json
import random
import threading
import uuid
from decimal import Decimal

import pytest

import strata

MODES = ("str", "bytes")
SEED = 20260929
TIMEOUT = 30.0


class Color(enum.Enum):
    RED = "red"
    DAY = dt.date(2026, 1, 2)


@dataclasses.dataclass
class Point:
    x: int
    y: float


class Opaque:
    def __init__(self, tag="opaque"):
        self.tag = tag


class BoomError(Exception):
    pass


STAMP = dt.datetime(2026, 9, 29, 12, 30, 5, tzinfo=dt.timezone.utc)
DAY = dt.date(2026, 1, 1)
NATIVES = [
    STAMP,
    DAY,
    dt.time(1, 2),
    uuid.UUID(int=7),
    Decimal("2.50"),
    Color.RED,
    Point(1, 2.0),
    {1, 2},
    frozenset({"a"}),
]


def text(out):
    return out.decode() if isinstance(out, bytes) else out


def oracle(document, default):
    """main 38eaa9f's `dumps_with_default` (M12b): stdlib `json`, compact."""
    return json.dumps(document, default=default, separators=(",", ":"), ensure_ascii=False)


def tag(obj):
    return obj.tag if isinstance(obj, Opaque) else f"{type(obj).__name__}:{obj}"


def recording(calls):
    def default(obj):
        calls.append(obj)
        return tag(obj)

    return default


def forbidden(obj):
    raise AssertionError(f"default was called for {obj!r}")


def native(value):
    return strata.dumps(value, native=True)


def same_objects(left, right):
    return len(left) == len(right) and all(a is b for a, b in zip(left, right, strict=True))


def run_on_thread(body):
    """Start `body` on a new thread; its result or exception lands in the dict."""
    outcome = {}

    def target():
        try:
            outcome["value"] = body()
        except BaseException as error:  # handed to the test thread
            outcome["error"] = error

    thread = threading.Thread(target=target)
    thread.start()
    return thread, outcome


def joined(thread, outcome):
    thread.join(TIMEOUT)
    assert not thread.is_alive()
    if "error" in outcome:
        raise outcome["error"]
    return outcome["value"]


# ---------------------------------------------------------------------------
# The stdlib oracle over a seeded corpus.
# ---------------------------------------------------------------------------


def to_json(obj):
    """A `default` for the corpus: every native as a JSON value or container."""
    if isinstance(obj, dt.date | dt.time):
        return obj.isoformat()
    if isinstance(obj, uuid.UUID | Decimal):
        return str(obj)
    if isinstance(obj, enum.Enum):
        return [obj.name, obj.value]
    if dataclasses.is_dataclass(obj):
        return dataclasses.asdict(obj)
    if isinstance(obj, set | frozenset):
        return sorted(obj, key=repr)
    if isinstance(obj, Opaque):
        return {"opaque": obj.tag, "at": STAMP}
    raise TypeError(type(obj).__name__)


LEAVES = [*NATIVES, Opaque("leaf"), 0, -7, 2**70, 0.1, -0.0, 1e22, "é ☃ 𝄞", "", True, None]


def random_document(rnd, depth=0):
    roll = rnd.random()
    if depth >= 4 or roll < 0.4:
        return rnd.choice(LEAVES)
    if roll < 0.7:
        return [random_document(rnd, depth + 1) for _ in range(rnd.randrange(4))]
    return {f"k{i}": random_document(rnd, depth + 1) for i in range(rnd.randrange(4))}


@pytest.mark.parametrize("mode", MODES)
def test_a_seeded_corpus_matches_the_stdlib_oracle_call_for_call(mode):
    # api.md, dumps_with_default: under `native=False` "the output is main
    # `38eaa9f`'s `dumps_with_default` byte for byte", every native going to
    # `default` once per object -- also a native nested in what `default`
    # returned (an Enum's value, the `Opaque` record's stamp).
    rnd = random.Random(SEED)
    for _ in range(300):
        document = random_document(rnd)
        calls, oracle_calls = [], []

        def default(obj, calls=calls):
            calls.append(obj)
            return to_json(obj)

        def oracle_default(obj, calls=oracle_calls):
            calls.append(obj)
            return to_json(obj)

        expected = oracle(document, oracle_default)
        out = strata.dumps_with_default(document, default, return_type=mode, native=False)
        assert text(out) == expected
        assert same_objects(calls, oracle_calls)


# ---------------------------------------------------------------------------
# Pin 5: nesting on one thread -- each call keeps its own mode.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_natives_on_calls_inside_an_opt_out_default_write_natives(mode, tmp_path):
    # api.md, dumps_with_default: "a call made from inside `default` has its
    # own [mode]" -- natives-on calls nested in an opt-out walk write natives,
    # and the opt-out walk sends natives to `default` before and after them.
    inner_document = [STAMP, {"u": uuid.UUID(int=9)}, Decimal("1.10"), Color.DAY]
    path = tmp_path / "inner.json"
    expected_inner = native(inner_document)
    trigger = Opaque("nested")
    inner = {}
    seen = []

    def outer_default(obj):
        seen.append(obj)
        if obj is trigger:
            inner["with_default"] = strata.dumps_with_default(inner_document, forbidden)
            inner["explicit"] = strata.dumps_with_default(inner_document, forbidden, native=True)
            inner["dumps"] = strata.dumps(inner_document, native=True)
            strata.dump(inner_document, path, native=True)
        return tag(obj)

    document = [DAY, trigger, {"after": STAMP}]
    out = strata.dumps_with_default(document, outer_default, return_type=mode, native=False)
    assert text(out) == oracle(document, tag)
    assert same_objects(seen, [DAY, trigger, STAMP])
    assert inner == dict.fromkeys(("with_default", "explicit", "dumps"), expected_inner)
    assert path.read_text(encoding="utf-8") == expected_inner + "\n"


@pytest.mark.parametrize("mode", MODES)
def test_an_opt_out_call_inside_a_natives_on_default_sends_natives_to_its_default(mode):
    # api.md, dumps_with_default: the mirror -- an opt-out call nested in a
    # natives-on walk passes natives to its own `default`, and the outer walk
    # writes natives before and after it.
    inner_document = [STAMP, {"u": uuid.UUID(int=9)}, Decimal("1.10")]
    trigger = Opaque("nested")
    inner_seen = []
    inner = {}

    def outer_default(obj):
        assert obj is trigger
        inner["out"] = strata.dumps_with_default(
            inner_document,
            recording(inner_seen),
            native=False,
        )
        return tag(obj)

    document = [DAY, trigger, {"after": STAMP}]
    out = strata.dumps_with_default(document, outer_default, return_type=mode)
    assert text(out) == f'[{native(DAY)},"nested",{{"after":{native(STAMP)}}}]'
    assert inner["out"] == oracle(inner_document, tag)
    assert same_objects(inner_seen, [STAMP, inner_document[1]["u"], inner_document[2]])


def walk_text(natives_on, trigger):
    """`[STAMP, trigger, STAMP]` under `tag` as the `default`, in either mode."""
    stamp = native(STAMP) if natives_on else json.dumps(tag(STAMP))
    return f'[{stamp},"{trigger.tag}",{stamp}]'


def walk_calls(natives_on, trigger):
    return [trigger] if natives_on else [STAMP, trigger, STAMP]


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("outer_native", [False, True])
def test_modes_alternating_three_calls_deep_each_keep_their_own(mode, outer_native):
    # api.md, dumps_with_default: every nested call has its own mode, however
    # deep -- the innermost runs while an opt-out walk is live, and each level
    # meets a native both before and after its nested call.
    middle_native = not outer_native
    to_middle, to_inner = Opaque("to-middle"), Opaque("to-inner")
    seen = {"outer": [], "middle": [], "inner": []}
    written = {}

    def inner_default(obj):
        seen["inner"].append(obj)
        return tag(obj)

    def middle_default(obj):
        seen["middle"].append(obj)
        if obj is to_inner:
            written["inner"] = strata.dumps_with_default(
                [STAMP, to_inner, STAMP],
                inner_default,
                native=outer_native,
            )
        return tag(obj)

    def outer_default(obj):
        seen["outer"].append(obj)
        if obj is to_middle:
            written["middle"] = strata.dumps_with_default(
                [STAMP, to_inner, STAMP],
                middle_default,
                native=middle_native,
            )
        return tag(obj)

    out = strata.dumps_with_default(
        [STAMP, to_middle, STAMP],
        outer_default,
        return_type=mode,
        native=outer_native,
    )
    assert text(out) == walk_text(outer_native, to_middle)
    assert written["middle"] == walk_text(middle_native, to_inner)
    assert written["inner"] == walk_text(outer_native, to_inner)
    assert same_objects(seen["outer"], walk_calls(outer_native, to_middle))
    assert same_objects(seen["middle"], walk_calls(middle_native, to_inner))
    assert same_objects(seen["inner"], walk_calls(outer_native, to_inner))


# ---------------------------------------------------------------------------
# Pin 6: threads -- an opt-out walk in progress elsewhere changes nothing here.
# ---------------------------------------------------------------------------


class HeldOptOut:
    """An opt-out walk of `[STAMP, gate, DAY]` on its own thread, held inside `default` at `gate`."""

    def __init__(self):
        self.gate = Opaque("held")
        self.document = [STAMP, self.gate, DAY]
        self.entered = threading.Event()
        self.release = threading.Event()
        self.seen = []
        self.thread = None
        self.outcome = None

    def default(self, obj):
        self.seen.append(obj)
        if obj is self.gate:
            self.entered.set()
            assert self.release.wait(TIMEOUT)
        return tag(obj)

    def start(self):
        self.thread, self.outcome = run_on_thread(
            lambda: strata.dumps_with_default(self.document, self.default, native=False),
        )
        assert self.entered.wait(TIMEOUT)

    def finish(self):
        """Release the walk; it must send the native after `gate` to `default` too."""
        self.release.set()
        if self.thread is None:
            return
        assert joined(self.thread, self.outcome) == oracle(self.document, tag)
        assert same_objects(self.seen, self.document)


@pytest.mark.parametrize("mode", MODES)
def test_an_opt_out_walk_held_on_another_thread_leaves_natives_on_calls_native(mode, tmp_path):
    # api.md, dumps_with_default: "... and so has every other thread" -- while
    # an opt-out walk is in progress on one thread, natives-on calls on another
    # write natives.
    document = [*NATIVES, {"nested": [STAMP, {"day": DAY}]}]
    expected = strata.dumps(document, return_type=mode, native=True)
    path = tmp_path / "document.json"
    held = HeldOptOut()
    try:
        held.start()
        assert strata.dumps(document, return_type=mode, native=True) == expected
        assert strata.dumps_with_default(document, forbidden, return_type=mode) == expected
        assert (
            strata.dumps_with_default(document, forbidden, return_type=mode, native=True)
            == expected
        )
        strata.dump(document, path, native=True)
    finally:
        held.finish()
    assert path.read_text(encoding="utf-8") == text(expected) + "\n"


def test_threads_alternating_modes_concurrently_each_keep_their_own():
    # api.md, dumps_with_default: the mode belongs to the call on every thread,
    # with walks of both modes in progress at once.
    document = [*NATIVES, Opaque("plain"), {"nested": [STAMP]}]
    natives_on_text = strata.dumps_with_default(document, tag)
    opt_out_text = oracle(document, tag)
    barrier = threading.Barrier(4)

    def worker(offset):
        barrier.wait(TIMEOUT)
        for index in range(300):
            opt_out = (index + offset) % 2 == 0
            out = strata.dumps_with_default(document, tag, native=not opt_out)
            assert out == (opt_out_text if opt_out else natives_on_text)
        return offset

    started = [run_on_thread(lambda offset=offset: worker(offset)) for offset in range(4)]
    assert [joined(thread, outcome) for thread, outcome in started] == [0, 1, 2, 3]


# ---------------------------------------------------------------------------
# Pin 8: a failing walk leaves no mode behind.
# ---------------------------------------------------------------------------


def context_mode():
    """The hook's mode variable as the current context holds it (None: unset).

    docs/decisions.md 2026-09-29: each hook walk restores the mode it found, so
    after any call returns -- or raises -- the caller's context holds what it
    held before; behaviour alone cannot see a skipped restore, since every
    natives-on entry looks its own mode up.
    """
    for var, value in contextvars.copy_context().items():
        if var.name == "strata._dumps_hook.native_off":
            return value
    return None


def assert_each_call_has_its_own_mode():
    assert context_mode() in (None, False)
    document = [DAY, {"at": STAMP}, Decimal("0.5")]
    expected = native(document)
    assert strata.dumps(document, native=True) == expected
    assert strata.dumps_with_default(document, forbidden) == expected
    seen = []
    assert strata.dumps_with_default(document, recording(seen), native=False) == oracle(
        document,
        tag,
    )
    assert same_objects(seen, [DAY, STAMP, document[2]])
    assert strata.dumps_with_default(document, forbidden, native=True) == expected


def raising(obj):
    raise BoomError(obj)


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("failing_native", [False, True])
def test_a_walk_whose_default_raises_leaves_the_next_call_its_own_mode(mode, failing_native):
    # api.md, dumps_with_default: `default` raises => the exception propagates
    # unchanged; and "The mode belongs to the call", so the next call in the
    # same context reads its own.
    with pytest.raises(BoomError):
        strata.dumps_with_default(
            [STAMP, Opaque()],
            raising,
            return_type=mode,
            native=failing_native,
        )
    assert_each_call_has_its_own_mode()


@pytest.mark.parametrize("mode", MODES)
def test_a_walk_failing_on_the_chain_bound_leaves_the_next_call_its_own_mode(mode):
    # api.md, dumps_with_default: under `native=False` a native `default`
    # returns is the chain-bound `TypeError`; the walk it ends leaves no mode.
    message = "^default\\(\\) returned an object of type datetime.datetime that is"
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default([Opaque()], lambda obj: STAMP, return_type=mode, native=False)
    assert_each_call_has_its_own_mode()


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("outer_native", [False, True])
def test_a_nested_walk_that_fails_leaves_the_outer_walk_its_own_mode(mode, outer_native):
    # api.md, dumps_with_default: a call made from inside `default` has its own
    # mode -- also when it fails, and the outer walk goes on in its own.
    trigger = Opaque("caught")
    seen = []

    def outer_default(obj):
        seen.append(obj)
        if obj is trigger:
            with pytest.raises(BoomError):
                strata.dumps_with_default([STAMP, Opaque()], raising, native=not outer_native)
        return tag(obj)

    out = strata.dumps_with_default(
        [STAMP, trigger, STAMP],
        outer_default,
        return_type=mode,
        native=outer_native,
    )
    assert text(out) == walk_text(outer_native, trigger)
    assert same_objects(seen, walk_calls(outer_native, trigger))
    assert_each_call_has_its_own_mode()


# ---------------------------------------------------------------------------
# Pin 9: a context copied inside an opt-out walk keeps no mode for later walks.
# ---------------------------------------------------------------------------


class HookedZone(dt.tzinfo):
    """UTC, but `utcoffset` is user code: its first call runs `action`."""

    def __init__(self, action):
        self.action = action

    def utcoffset(self, when):
        action, self.action = self.action, None
        if action is not None:
            action()
        return dt.timedelta(0)

    def dst(self, when):
        return None


def copied_inside_an_opt_out_walk():
    copies = []

    def default(obj):
        copies.append(contextvars.copy_context())
        return tag(obj)

    assert strata.dumps_with_default([Opaque()], default, native=False) == '["opaque"]'
    return copies[0]


@pytest.mark.parametrize("walk", ["dumps_with_default", "dumps", "dump"])
def test_a_walk_run_in_a_context_copied_inside_an_opt_out_walk_writes_natives(walk, tmp_path):
    # api.md, dumps_with_default: "The mode belongs to the call" -- a context
    # copied inside an opt-out walk's `default` (a task or thread started there)
    # outlives the walk; a natives-on walk run in it later, while an opt-out
    # walk is in progress elsewhere, still writes natives (docs/decisions.md,
    # 2026-09-29: natives-on entries look the mode up on every call).
    copied = copied_inside_an_opt_out_walk()
    held = HeldOptOut()
    seen = []
    path = tmp_path / "walk.json"

    def default(obj):
        seen.append(obj)
        if isinstance(obj, Opaque):
            held.start()
            return "x"
        return "leaked"

    def run():
        if walk == "dumps_with_default":
            return strata.dumps_with_default([Opaque("x"), STAMP], default)
        document = [dt.datetime(2026, 1, 1, tzinfo=HookedZone(held.start)), STAMP]
        if walk == "dumps":
            return strata.dumps(document, native=True)
        strata.dump(document, path, native=True)
        return path.read_text(encoding="utf-8").removesuffix("\n")

    try:
        out = copied.run(run)
    finally:
        held.finish()
    utc = dt.datetime(2026, 1, 1, tzinfo=dt.timezone.utc)
    first = '"x"' if walk == "dumps_with_default" else native(utc)
    assert out == f"[{first},{native(STAMP)}]"
    assert len(seen) == (1 if walk == "dumps_with_default" else 0)


def test_the_mode_probe_sees_each_walk_and_nothing_after():
    # Validates `context_mode`, which pin 8 relies on: True inside an opt-out
    # walk's `default`, False inside a natives-on walk nested there, and the
    # context left as it was found once both return.
    before = context_mode()
    inner = []

    def inner_default(obj):
        inner.append(context_mode())
        return "i"

    def outer_default(obj):
        outer = context_mode()
        strata.dumps_with_default([Opaque()], inner_default)
        return f"{outer}"

    assert strata.dumps_with_default([Opaque()], outer_default, native=False) == '["True"]'
    assert inner == [False]
    assert context_mode() == before

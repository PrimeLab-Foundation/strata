"""Contract tests for the reference ownership of a nested `dumps`'s schema cache.

A `dumps` call leases the thread's schema cache (`SchemaCacheLease`,
python_dumps_output.h). A *nested* call — user code running inside `dumps`
calling `dumps` again, which api.md's "Mutation during serialization" clause
allows at four documented steps — finds that lease busy and takes a **private**
state instead, so the outer walk keeps its schemas and its staged rows.

The keys a schema remembers are *owned* references: identity comparison is only
sound while the objects live. The shared state is deliberately immortal, so its
references are held for the thread's life on purpose. The private state is not:
it dies with the nested call, and before this file existed nothing released what
it owned. Every nested call leaked one reference per remembered key —
`sys.getrefcount(key)` grew by one per call, for the life of the process, while
the same key remembered by an ordinary call stayed at a single reference.

What these tests pin:

* **A nested call leaves every key's reference count where it found it**, in
  both output modes, for remembered *and* prepared schemas, for wide records
  and for keys too wide for the inline slots, and for nested nesting.
* **Both exits release.** A nested call that raises — an unsupported value, a
  key with no UTF-8 form — releases exactly what a successful one does.
* **The ordinary shared cache is unchanged**: it still takes one reference per
  remembered key and still keeps it, so nothing here turned the leak fix into a
  release of references the thread still uses.
* **Output is unaffected**: a nested call's document is byte-identical to the
  same document serialized at the top level.

The cache is thread-local, so every body that depends on cache state runs on a
fresh thread — a shape remembered by one test must not reach the next.
"""

import gc
import itertools
import json
import sys
import threading
import warnings
import weakref

import pytest

import strata

MODES = ("str", "bytes")

#: Past int64, so `write_int` takes `PyObject_Str` — the documented step that
#: runs an `int` subclass's `__str__` inside the walk (api.md).
BIG = 1 << 80

#: A lone high surrogate: `str.encode()` has no form for it, so a key holding
#: one fails inside `build_schema` (docs/context/api.md, `UnicodeEncodeError`).
LONE_HIGH = "\ud800"

_counter = itertools.count()


def fresh_key(tag):
    """A `str` that is unique, exact, and not interned.

    Built at run time so the only references to it are the test's own: the
    schema cache remembers exact `str` keys by identity, and an interned
    literal would be reachable from elsewhere in the process.
    """
    return "".join(["private-cache-", tag, "-", str(next(_counter))])


def in_fresh_cache(body):
    """Run `body` on a new thread and re-raise whatever it raised.

    `SchemaCacheLease`'s state is `thread_local`, so a new thread starts with an
    empty schema cache, no prepared shape and no retired depth.
    """
    box = {}

    def run():
        try:
            box["value"] = body()
        except BaseException as error:  # re-raised on the calling thread
            box["error"] = error

    thread = threading.Thread(target=run)
    thread.start()
    thread.join()
    if "error" in box:
        raise box["error"]
    return box["value"]


def reentrant(callback):
    """An `int` beyond int64 whose `__str__` runs `callback` inside `dumps`."""

    class Reentrant(int):
        def __str__(self):
            callback()
            return int.__str__(self)

    return Reentrant(BIG)


class Sentinel:
    """Inert and weakref-capable, so its collection is observable."""

    __slots__ = ("__weakref__",)


class SlotClearer(int):
    """Beyond int64, so `__str__` runs inside the walk (api.md) — and drops the
    record the walk has already written out of the document that held it.

    Lists are followed live, element by element, so replacing element 0 while
    the walk stands on element 1 is well defined and leaves the output alone.
    Its only purpose is to release a record mid-call: the keys that record held
    then have exactly one owner left, the schema slot that remembered them.
    """

    def __new__(cls, document, sentinel):
        value = int.__new__(cls, BIG)
        value.document = document
        value.sentinel = sentinel
        return value

    def __str__(self):
        self.document[0] = None
        return int.__str__(self)


def key_delta(document, keys, mode, calls=100):
    """Reference-count growth of `keys` over `calls` serializations of `document`.

    One warmup call first, so the counts compared are steady-state: the shared
    cache legitimately takes one reference the first time it remembers a shape.

    Both readings are taken the same way, one comprehension each: `zip` reuses
    its result tuple, so counting a key while it is still in that tuple reads
    one reference more than counting it in a plain loop.
    """
    strata.dumps(document, return_type=mode)
    before = [sys.getrefcount(key) for key in keys]
    for _ in range(calls):
        strata.dumps(document, return_type=mode)
    after = [sys.getrefcount(key) for key in keys]
    return [now - was for now, was in zip(after, before, strict=True)]


# ---------------------------------------------------------------------------
# The leak itself
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_nested_dumps_releases_the_key_it_remembered(mode):
    """The reported defect: 100 nested calls raised a key's count by 100."""

    def body():
        key = fresh_key("simple")
        inner = {key: 1}
        outer = [reentrant(lambda: strata.dumps(inner, return_type=mode))]
        return key_delta(outer, [key], mode)

    assert in_fresh_cache(body) == [0]


@pytest.mark.parametrize("mode", MODES)
def test_nested_dumps_releases_every_key_of_a_wide_record(mode):
    """All 24 slots of a schema row, not just the first."""

    def body():
        keys = [fresh_key(f"wide{index}") for index in range(24)]
        inner = dict.fromkeys(keys, 1)
        outer = [reentrant(lambda: strata.dumps(inner, return_type=mode))]
        return key_delta(outer, keys, mode, calls=20)

    assert in_fresh_cache(body) == [0] * 24


@pytest.mark.parametrize("mode", MODES)
def test_nested_dumps_releases_the_keys_of_a_prepared_schema(mode):
    """A shape seen twice inside one nested call is *prepared*, not just remembered.

    The first sighting remembers the keys; the second hits the way and runs
    `build_schema`, which fills the inline slots. Both states own the keys.
    """

    def body():
        keys = [fresh_key("prep-a"), fresh_key("prep-b")]
        records = [dict.fromkeys(keys, index) for index in range(3)]
        outer = [reentrant(lambda: strata.dumps(records, return_type=mode))]
        return key_delta(outer, keys, mode)

    assert in_fresh_cache(body) == [0, 0]


@pytest.mark.parametrize("mode", MODES)
def test_nested_dumps_releases_keys_too_wide_for_the_inline_slots(mode):
    """A span past `kSlotBytes` makes the schema `wide` and emits from its blob."""

    def body():
        keys = [fresh_key("x" * 40), fresh_key("y" * 40)]
        records = [dict.fromkeys(keys, 1) for _ in range(3)]
        outer = [reentrant(lambda: strata.dumps(records, return_type=mode))]
        return key_delta(outer, keys, mode)

    assert in_fresh_cache(body) == [0, 0]


@pytest.mark.parametrize("mode", MODES)
def test_nested_dumps_releases_the_keys_of_every_nesting_level(mode):
    """One schema slot per level: the release has to walk all of them."""

    def body():
        keys = [fresh_key(f"level{level}") for level in range(5)]
        document = 1
        for key in reversed(keys):
            document = [{key: document}, {key: document}]
        outer = [reentrant(lambda: strata.dumps(document, return_type=mode))]
        return key_delta(outer, keys, mode, calls=50)

    assert in_fresh_cache(body) == [0] * 5


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_nested_call_releases_its_own_private_state(mode):
    """Three leases deep: the outer shared one and two private ones."""

    def body():
        inner_key = fresh_key("depth2")
        middle_key = fresh_key("depth1")
        innermost = {inner_key: 1}

        def middle():
            document = [{middle_key: reentrant(lambda: strata.dumps(innermost, return_type=mode))}]
            strata.dumps(document, return_type=mode)

        outer = [reentrant(middle)]
        return key_delta(outer, [middle_key, inner_key], mode, calls=50)

    assert in_fresh_cache(body) == [0, 0]


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_call_that_retires_a_depth_releases_every_way(mode):
    """Shapes that never repeat retire the depth, and the release runs anyway.

    Past 64 misses at one depth `DepthSchemas` retires it, and the 64th miss
    invalidated a way to make room. Those ways are already invalidated when the
    lease ends, so `release_keys` invalidates them a *second* time — the double
    release the ownership rule has to survive. It does because `forget()` is
    idempotent: an emptied `keys` releases nothing. A drift of `-1` here would
    be one reference released twice.
    """

    def body():
        keys = [fresh_key(f"retire{index}") for index in range(200)]
        records = [{key: 1} for key in keys]

        def nested():
            for record in records:
                strata.dumps(record, return_type=mode)

        outer = [reentrant(nested)]
        return key_delta(outer, keys, mode, calls=5)

    assert in_fresh_cache(body) == [0] * 200


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_call_frees_the_keys_it_is_the_last_owner_of(mode):
    """The release is a real deallocation, not just a lowered count.

    Every other case keeps its keys alive in an enclosing scope, so the lease's
    `Py_DECREF` only decrements. Here the record is dropped from the document
    *while the same nested call is still walking it* (`SlotClearer`), leaving
    the private schema slot as the keys' only owner — so `release_keys` is what
    frees them, and this is the case that would report a use-after-free under
    the ASan gate if the release ran while anything still read `key_row`.

    The keys are never named outside the callback, so correctness is checked
    structurally: `json.loads` builds its own strings and pins nothing.
    """
    watched = []
    verdicts = []

    def body():
        def nested():
            sentinel = Sentinel()
            watched.append(weakref.ref(sentinel))
            document = [{fresh_key("last-owner"): 1, fresh_key("last-owner"): 2}, None]
            document[1] = SlotClearer(document, sentinel)
            output = strata.dumps(document, return_type=mode)
            parsed = json.loads(output.decode() if mode == "bytes" else output)
            verdicts.append(
                document[0] is None
                and parsed[1] == BIG
                and len(parsed) == 2
                and len(parsed[0]) == 2
                and sorted(parsed[0].values()) == [1, 2],
            )

        outer = [reentrant(nested)]
        for _ in range(200):
            strata.dumps(outer, return_type=mode)

    in_fresh_cache(body)
    # `document` and its `SlotClearer` are a cycle, so the callback's scope
    # needs a collection before the sentinel can go.
    gc.collect()
    assert verdicts == [True] * 200
    assert [reference() for reference in watched] == [None] * 200


def test_a_warning_hook_that_serializes_releases_its_keys():
    """The cycle warning is one of the four documented re-entry points."""

    def body():
        key = fresh_key("cycle")
        inner = {key: 1}
        cycle = []
        cycle.append(cycle)
        fired = []
        try:
            with warnings.catch_warnings():
                warnings.simplefilter("always")

                def hook(*_args, **_kwargs):
                    fired.append(1)
                    strata.dumps(inner)

                warnings.showwarning = hook
                delta = key_delta(cycle, [key], "str", calls=50)
        finally:
            cycle.clear()
        return delta, bool(fired)

    assert in_fresh_cache(body) == ([0], True)


# ---------------------------------------------------------------------------
# Exceptional exits
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_failing_nested_call_releases_what_it_remembered(mode):
    """The shape is remembered before the value that raises is written."""

    def body():
        key = fresh_key("unsupported")
        inner = {key: object()}

        def nested():
            with pytest.raises(TypeError):
                strata.dumps(inner, return_type=mode)

        outer = [reentrant(nested)]
        return key_delta(outer, [key], mode, calls=50)

    assert in_fresh_cache(body) == [0]


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_call_that_cannot_encode_a_key_releases_the_rest(mode):
    """`build_schema`'s failure arm invalidates the way; the lease still owns it."""

    def body():
        key = fresh_key("surrogate")
        inner = {key: 1, LONE_HIGH: 2}

        def nested():
            with pytest.raises(UnicodeEncodeError):
                strata.dumps(inner, return_type=mode)

        outer = [reentrant(nested)]
        return key_delta(outer, [key], mode, calls=50)

    assert in_fresh_cache(body) == [0]


@pytest.mark.parametrize("mode", MODES)
def test_an_exception_out_of_the_nested_call_still_releases(mode):
    """The nested error propagates through the outer walk, which unwinds too."""

    def body():
        key = fresh_key("propagate")
        inner = {key: object()}
        outer = [reentrant(lambda: strata.dumps(inner, return_type=mode))]
        for _ in range(50):
            with pytest.raises(TypeError):
                strata.dumps(outer, return_type=mode)
        before = sys.getrefcount(key)
        for _ in range(50):
            with pytest.raises(TypeError):
                strata.dumps(outer, return_type=mode)
        return sys.getrefcount(key) - before

    assert in_fresh_cache(body) == 0


# ---------------------------------------------------------------------------
# The shared cache is unchanged
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_the_shared_cache_takes_one_reference_and_keeps_it(mode):
    """The control: ordinary calls still remember, and still hold exactly one.

    A fix that released the shared state's keys as well would show 0 here — and
    would be a use-after-free, because the way keeps comparing that pointer.
    """

    def body():
        key = fresh_key("shared")
        record = {key: 1}
        before = sys.getrefcount(key)
        strata.dumps(record, return_type=mode)
        after_first = sys.getrefcount(key) - before
        for _ in range(100):
            strata.dumps(record, return_type=mode)
        return after_first, sys.getrefcount(key) - before

    assert in_fresh_cache(body) == (1, 1)


@pytest.mark.parametrize("mode", MODES)
def test_ordinary_and_nested_calls_interleave_without_drift(mode):
    """The two states side by side: neither leaks, and the shared one still hits."""

    def body():
        shared_key = fresh_key("interleaved-shared")
        private_key = fresh_key("interleaved-private")
        record = {shared_key: 1}
        inner = {private_key: 2}
        outer = [reentrant(lambda: strata.dumps(inner, return_type=mode))]
        strata.dumps(record, return_type=mode)
        strata.dumps(outer, return_type=mode)
        before = [sys.getrefcount(shared_key), sys.getrefcount(private_key)]
        for _ in range(50):
            strata.dumps(record, return_type=mode)
            strata.dumps(outer, return_type=mode)
        after = [sys.getrefcount(shared_key), sys.getrefcount(private_key)]
        return [now - was for now, was in zip(after, before, strict=True)]

    assert in_fresh_cache(body) == [0, 0]


def test_the_cache_stays_private_to_its_thread():
    """A second thread's leases release its own keys, not the first thread's."""

    def body():
        key = fresh_key("thread")
        record = {key: 1}
        strata.dumps(record)
        before = sys.getrefcount(key)
        in_fresh_cache(lambda: [strata.dumps(record) for _ in range(50)])
        # The other thread's shared cache remembered the same key once and is
        # gone with the thread; its state is immortal, so its reference stays.
        return sys.getrefcount(key) - before

    assert in_fresh_cache(body) == 1


# ---------------------------------------------------------------------------
# Behaviour is unchanged
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_a_nested_call_writes_the_same_document_as_a_top_level_one(mode):
    """The private state serializes; it does not merely avoid leaking."""

    keys = [fresh_key("same-a"), fresh_key("same-b")]
    records = [dict.fromkeys(keys, index) for index in range(4)]

    def body():
        captured = []
        outer = [reentrant(lambda: captured.append(strata.dumps(records, return_type=mode)))]
        strata.dumps(outer, return_type=mode)
        strata.dumps(outer, return_type=mode)
        return captured

    captured = in_fresh_cache(body)
    top_level = in_fresh_cache(lambda: strata.dumps(records, return_type=mode))
    assert captured == [top_level, top_level]
    text = top_level.decode() if mode == "bytes" else top_level
    assert json.loads(text) == records


@pytest.mark.parametrize("mode", MODES)
def test_the_outer_document_is_unchanged_by_the_nested_call(mode):
    """The re-entry point is an `int` subclass's `__str__`: it prints its value."""

    def body():
        key = fresh_key("outer")
        inner = {key: 1}
        outer = {"n": reentrant(lambda: strata.dumps(inner, return_type=mode)), "tail": [1, 2]}
        first = strata.dumps(outer, return_type=mode)
        second = strata.dumps(outer, return_type=mode)
        return first, second

    first, second = in_fresh_cache(body)
    assert first == second
    text = first.decode() if mode == "bytes" else first
    assert json.loads(text) == {"n": BIG, "tail": [1, 2]}

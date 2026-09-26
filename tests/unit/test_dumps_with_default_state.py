"""Contract tests for the state `dumps_with_default` shares with `_strata`.

docs/architecture/dumps_with_default.md § "Shared state across the two images"
(api.md, dumps_with_default):

- *Cycle policy.* `config.set("cycle_policy", ...)` writes `_strata`'s policy;
  the hook image reads it at each cycle point, so a policy the callable changes
  mid-walk applies to later cycles, as in `dumps`.
- *Returned open containers.* Error-table row 7: a cycle under the active
  policy, reported where it was returned (the M12 ruling), checked against the
  direct cycle cold and warmed.
- *Schema cache.* Per image; a `dumps` called from the callable leases
  `_strata`'s free state, a `dumps_with_default` nested in its own callable
  takes the hook image's fallback, so E26-FIX2b's refcount pin is re-run in
  both directions — plus the one path from `_strata` into the hook image,
  `dumps`'s own user-code step (`__str__` of an `int` subclass beyond int64).

The schema cache is thread-local, so every refcount pin runs on a fresh thread.
"""

import itertools
import json
import pathlib
import re
import subprocess
import sys
import threading
import warnings

import pytest

import strata

MODES = ("str", "bytes")

CYCLE_POLICIES = ("warn", "error", "ignore")

CYCLE_MESSAGE = "Circular reference detected"


class Opaque:
    """An unsupported type: the serializer has no writer for it."""

    __slots__ = ("__weakref__", "tag")

    def __init__(self, tag=None):
        self.tag = tag


def text(out):
    return out.decode() if isinstance(out, bytes) else out


def exact(message):
    return f"^{re.escape(message)}$"


_counter = itertools.count()


def fresh_key(tag):
    """A unique, exact, non-interned `str`: the test holds its only references."""
    return "".join(["dumps-with-default-", tag, "-", str(next(_counter))])


def in_fresh_cache(body):
    """Run `body` on a new thread (an empty schema cache) and re-raise its error."""
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


def outcome(call):
    """`call()`'s result, `("ok", text)` or `("ValueError", message)`, and its warnings."""
    with warnings.catch_warnings(record=True) as caught:
        warnings.simplefilter("always")
        try:
            result = ("ok", text(call()))
        except ValueError as error:
            result = ("ValueError", str(error))
    return result, [(w.category, str(w.message)) for w in caught]


def never(obj):  # pragma: no cover - must never be called
    raise AssertionError(f"default called for {type(obj).__name__}")


# ---------------------------------------------------------------------------
# Cycle policy: "`config.set("cycle_policy", ...)` writes `_strata`'s
# `g_cycle_policy`; the hook image must honour it ... read at the cycle point."
# ---------------------------------------------------------------------------


def _list_in_itself():
    doc = [1]
    doc.append(doc)
    return doc


def _dict_in_itself():
    doc = {"a": 1, "b": None}
    doc["b"] = doc
    return doc


def _record_in_itself():
    doc = [{"x": index, "y": None} for index in range(4)]
    doc[3]["y"] = doc
    return doc


DIRECT_CYCLES = [
    ("list-in-itself", _list_in_itself),
    ("dict-in-itself", _dict_in_itself),
    ("record-in-itself", _record_in_itself),
]


@pytest.mark.parametrize("build", [b for _, b in DIRECT_CYCLES], ids=[n for n, _ in DIRECT_CYCLES])
@pytest.mark.parametrize("policy", CYCLE_POLICIES)
@pytest.mark.parametrize("mode", MODES)
def test_the_policy_set_through_config_governs_a_direct_cycle(mode, policy, build):
    strata.config.set("cycle_policy", policy)
    doc = build()
    direct = outcome(lambda: strata.dumps(doc, return_type=mode))
    hooked = outcome(lambda: strata.dumps_with_default(doc, never, return_type=mode))
    assert hooked == direct
    (kind, value), caught = hooked
    if policy == "error":
        assert (kind, value, caught) == ("ValueError", CYCLE_MESSAGE, [])
    else:
        assert kind == "ok"
        assert "null" in value
        assert caught == ([(RuntimeWarning, CYCLE_MESSAGE)] if policy == "warn" else [])


def _later_cycle():
    """The document's own list at 0 and 2, an unsupported object between them."""
    doc = []
    doc.extend([doc, Opaque(), doc])
    return doc


def _hook_first():
    doc = [Opaque()]
    doc.append(doc)
    return doc


# (policy before, policy the callable sets, document, result, warnings raised)
MID_WALK = [
    ("ignore", "error", _later_cycle, ("ValueError", CYCLE_MESSAGE), 0),
    ("ignore", "warn", _later_cycle, ("ok", '[null,"x",null]'), 1),
    ("warn", "ignore", _later_cycle, ("ok", '[null,"x",null]'), 1),
    ("warn", "error", _later_cycle, ("ValueError", CYCLE_MESSAGE), 1),
    ("error", "ignore", _hook_first, ("ok", '["x",null]'), 0),
    ("error", "warn", _hook_first, ("ok", '["x",null]'), 1),
]


@pytest.mark.parametrize(
    "case", MID_WALK, ids=[f"{before}-to-{after}" for before, after, *_ in MID_WALK]
)
@pytest.mark.parametrize("mode", MODES)
def test_a_policy_the_callable_sets_applies_to_later_cycles(mode, case):
    before, after, build, result, warned = case
    strata.config.set("cycle_policy", before)
    doc = build()

    def hook(obj):
        strata.config.set("cycle_policy", after)
        return "x"

    got, caught = outcome(lambda: strata.dumps_with_default(doc, hook, return_type=mode))
    assert got == result
    assert caught == [(RuntimeWarning, CYCLE_MESSAGE)] * warned
    assert strata.config.get("cycle_policy") == after


@pytest.mark.parametrize(
    ("before", "after", "result", "warned"),
    [
        ("ignore", "error", ("ValueError", CYCLE_MESSAGE), 0),
        ("error", "ignore", ("ok", "[null]"), 0),
        ("warn", "ignore", ("ok", "[null]"), 0),
        ("ignore", "warn", ("ok", "[null]"), 1),
    ],
    ids=["ignore-to-error", "error-to-ignore", "warn-to-ignore", "ignore-to-warn"],
)
@pytest.mark.parametrize("mode", MODES)
def test_a_policy_the_callable_sets_applies_to_the_container_it_returns(
    mode, before, after, result, warned
):
    # The returned open container is found after the callable returns, so it
    # is a later cycle.
    strata.config.set("cycle_policy", before)
    doc = [Opaque()]

    def hook(obj):
        strata.config.set("cycle_policy", after)
        return doc

    got, caught = outcome(lambda: strata.dumps_with_default(doc, hook, return_type=mode))
    assert got == result
    assert caught == [(RuntimeWarning, CYCLE_MESSAGE)] * warned


@pytest.mark.parametrize("entry", ("dumps", "dumps_with_default"))
@pytest.mark.parametrize("mode", MODES)
def test_both_entry_points_read_the_policy_at_each_cycle(mode, entry):
    # `dumps`'s own user-code step under "warn" (a `showwarning` hook) switches
    # the policy between two cycles; both images honour it at the second.
    strata.config.set("cycle_policy", "warn")
    doc = []
    doc.extend([doc, doc])
    shown = []

    def showwarning(message, category, *args, **kwargs):
        shown.append((category, str(message)))
        strata.config.set("cycle_policy", "error")

    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = showwarning
        with pytest.raises(ValueError, match=exact(CYCLE_MESSAGE)):
            if entry == "dumps":
                strata.dumps(doc, return_type=mode)
            else:
                strata.dumps_with_default(doc, never, return_type=mode)
    assert shown == [(RuntimeWarning, CYCLE_MESSAGE)]


# ---------------------------------------------------------------------------
# Row 7 against the direct cycle, cold and after warming the same shapes, so
# the schema-cache state the placement caveat depends on is exercised too.
#
# One placement differs, by decision (docs/decisions.md, 2026-09-26; the M12
# ruling this record carries over): the callable's return is written through
# the value path, whose record branch probes the open-container stack, so a
# returned open dict is reported where it is returned — on time — even at a
# list-element position where a directly-reached, warmed repeated dict lands
# one container late (the 2026-09-11/12 caveat belongs to the array element
# loop only). The policy behaviour there still matches the direct cycle.
# ---------------------------------------------------------------------------


def _list_in_itself_shape(hooked):
    doc = []
    doc.append(Opaque() if hooked else doc)
    return doc, (lambda obj: doc) if hooked else None


def _dict_in_itself_shape(hooked):
    doc = {"a": 1, "b": None}
    doc["b"] = Opaque() if hooked else doc
    return doc, (lambda obj: doc) if hooked else None


def _list_as_dict_value(hooked):
    doc = [{"x": 1, "y": None}]
    doc[0]["y"] = Opaque() if hooked else doc
    return doc, (lambda obj: doc) if hooked else None


def _dict_as_list_element(hooked):
    doc = {"a": 1, "kids": [None]}
    doc["kids"][0] = Opaque() if hooked else doc
    return doc, (lambda obj: doc) if hooked else None


#: The on-time placement of `_dict_as_list_element`'s returned open dict.
ON_TIME_LIST_ELEMENT = '{"a":1,"kids":[null]}'

# (label, builder, an acyclic document of the same shapes at every depth the
# cycle revisits, serialized first when the case is warmed)
CYCLE_SHAPES = [
    ("list-in-itself", _list_in_itself_shape, [[[]]]),
    ("dict-in-itself", _dict_in_itself_shape, {"a": 1, "b": {"a": 1, "b": {"a": 1, "b": 0}}}),
    ("list-as-dict-value", _list_as_dict_value, [{"x": 1, "y": [{"x": 1, "y": []}]}]),
    (
        "dict-as-list-element",
        _dict_as_list_element,
        {"a": 1, "kids": [{"a": 1, "kids": [{"a": 1, "kids": []}]}]},
    ),
]


def _cycle_outcomes(build, warm, hooked, mode):
    """Two calls on a fresh thread after an optional warmup: outputs and warnings."""

    def body():
        doc, hook = build(hooked)
        for _ in range(3 if warm is not None else 0):
            if hook is None:
                strata.dumps(warm, return_type=mode)
            else:
                strata.dumps_with_default(warm, hook, return_type=mode)
        if hook is None:
            return [outcome(lambda: strata.dumps(doc, return_type=mode)) for _ in range(2)]
        return [
            outcome(lambda: strata.dumps_with_default(doc, hook, return_type=mode))
            for _ in range(2)
        ]

    return in_fresh_cache(body)


@pytest.mark.parametrize(
    ("build", "warm_doc"),
    [(b, w) for _, b, w in CYCLE_SHAPES],
    ids=[n for n, _, _ in CYCLE_SHAPES],
)
@pytest.mark.parametrize("warmed", (False, True), ids=("cold", "warmed"))
@pytest.mark.parametrize("policy", CYCLE_POLICIES)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_open_container_is_the_direct_cycle(mode, policy, warmed, build, warm_doc):
    warm = warm_doc if warmed else None
    strata.config.set("cycle_policy", policy)
    direct = _cycle_outcomes(build, warm, hooked=False, mode=mode)
    hooked = _cycle_outcomes(build, warm, hooked=True, mode=mode)
    for outcomes in (direct, hooked):
        for (kind, _), caught in outcomes:
            if policy == "error":
                assert kind == "ValueError"
                assert caught == []
            elif policy == "warn":
                assert kind == "ok"
                assert [category for category, _ in caught] == [RuntimeWarning]
            else:
                assert kind == "ok"
                assert caught == []
    late_caveat_case = warmed and build is _dict_as_list_element and policy != "error"
    if not late_caveat_case:
        assert hooked == direct
        return
    # The decided divergence: the hooked output is the on-time placement, and
    # the warning each call raises is the direct cycle's.
    for ((_, hooked_out), hooked_caught), (_, direct_caught) in zip(hooked, direct, strict=True):
        assert hooked_out == ON_TIME_LIST_ELEMENT
        assert hooked_caught == direct_caught


# ---------------------------------------------------------------------------
# E26-FIX2b re-pinned in both directions: "A `dumps` called from a hook leases
# `_strata`'s (free) state; a `dumps_with_default` nested in its own hook takes
# the hook image's fallback — the T1 destructor release applies in both
# images." Zero key drift over repeated calls, each on a fresh thread.
# ---------------------------------------------------------------------------

DIRECTIONS = ("dumps", "dumps_with_default")


def _leaf(direction):
    """How a nested document spells a value: plain for `dumps`, hooked otherwise."""
    return (lambda value: value) if direction == "dumps" else Opaque


def _through(direction, inner, mode):
    """A callable that serializes `inner` with a nested call of `direction`."""
    if direction == "dumps":

        def hook(obj):
            return text(strata.dumps(inner, return_type=mode))

    else:

        def hook(obj):
            return text(strata.dumps_with_default(inner, lambda o: o.tag, return_type=mode))

    return hook


def _key_delta(document, keys, mode, hook, calls=100):
    """Refcount growth of `keys` over `calls` hooked serializations, after a warmup."""
    strata.dumps_with_default(document, hook, return_type=mode)
    before = [sys.getrefcount(key) for key in keys]
    for _ in range(calls):
        strata.dumps_with_default(document, hook, return_type=mode)
    after = [sys.getrefcount(key) for key in keys]
    return [now - was for now, was in zip(after, before, strict=True)]


@pytest.mark.parametrize("direction", DIRECTIONS)
@pytest.mark.parametrize("mode", MODES)
def test_a_nested_call_through_the_callable_releases_its_keys(mode, direction):
    def body():
        inner_key, outer_key = fresh_key("inner"), fresh_key("outer")
        inner = {inner_key: _leaf(direction)(1)}
        document = {outer_key: Opaque()}
        hook = _through(direction, inner, mode)
        out = strata.dumps_with_default(document, hook, return_type=mode)
        assert json.loads(out) == {outer_key: json.dumps({inner_key: 1}, separators=(",", ":"))}
        return _key_delta(document, [inner_key, outer_key], mode, hook)

    assert in_fresh_cache(body) == [0, 0]


@pytest.mark.parametrize("direction", DIRECTIONS)
@pytest.mark.parametrize("mode", MODES)
def test_a_nested_call_through_the_callable_releases_a_wide_record(mode, direction):
    def body():
        keys = [fresh_key(f"wide{index}") for index in range(24)]
        leaf = _leaf(direction)
        inner = [{key: leaf(index) for index, key in enumerate(keys)} for _ in range(4)]
        document = [Opaque(), {"pad": 1}]
        return _key_delta(document, keys, mode, _through(direction, inner, mode))

    assert in_fresh_cache(body) == [0] * 24


@pytest.mark.parametrize("direction", DIRECTIONS)
@pytest.mark.parametrize("mode", MODES)
def test_a_doubly_nested_call_through_the_callable_releases_its_keys(mode, direction):
    # The middle call is a `dumps_with_default` whose own callable makes the
    # innermost call in `direction`.
    def body():
        deep_key, mid_key = fresh_key("deep"), fresh_key("mid")
        leaf = _leaf(direction)
        deep = {deep_key: [leaf(1), leaf(2)]}
        mid_hook = _through(direction, deep, mode)
        middle = {mid_key: Opaque()}

        def hook(obj):
            return text(strata.dumps_with_default(middle, mid_hook, return_type=mode))

        document = [Opaque(), Opaque()]
        out = strata.dumps_with_default(document, hook, return_type=mode)
        deep_text = json.dumps({deep_key: [1, 2]}, separators=(",", ":"))
        mid_text = json.dumps({mid_key: deep_text}, separators=(",", ":"))
        assert json.loads(out) == [mid_text, mid_text]
        return _key_delta(document, [deep_key, mid_key], mode, hook)

    assert in_fresh_cache(body) == [0, 0]


@pytest.mark.parametrize("direction", DIRECTIONS)
@pytest.mark.parametrize("mode", MODES)
def test_a_failing_nested_call_through_the_callable_releases_its_keys(mode, direction):
    def body():
        key = fresh_key("failing")
        inner = {key: 1, "bad": Opaque()}
        if direction == "dumps":
            message = exact("Object of type Opaque is not JSON serializable")

            def hook(obj):
                return strata.dumps(inner, return_type=mode)

        else:
            message = exact(
                "default() returned an object of type Opaque that is not JSON serializable"
            )

            def hook(obj):
                return strata.dumps_with_default(inner, lambda o: o, return_type=mode)

        document = [Opaque()]
        with pytest.raises(TypeError, match=message):
            strata.dumps_with_default(document, hook, return_type=mode)
        before = sys.getrefcount(key)
        for _ in range(50):
            with pytest.raises(TypeError, match=message):
                strata.dumps_with_default(document, hook, return_type=mode)
        return sys.getrefcount(key) - before

    assert in_fresh_cache(body) == 0


@pytest.mark.parametrize("mode", MODES)
def test_a_dumps_with_default_inside_dumps_releases_its_keys(mode):
    # The one way `_strata`'s walk reaches the hook image: `__str__` of an
    # `int` subclass beyond int64 (api.md, "Mutation during serialization").
    def body():
        inner_key, outer_key = fresh_key("inner"), fresh_key("outer")
        inner = {inner_key: Opaque(1)}
        nested = []

        class Big(int):
            def __str__(self):
                nested.append(strata.dumps_with_default(inner, lambda o: o.tag))
                return int.__str__(self)

        document = {outer_key: Big(1 << 70)}
        out = text(strata.dumps(document, return_type=mode))
        assert out == json.dumps({outer_key: 1 << 70}, separators=(",", ":"))
        assert nested == [json.dumps({inner_key: 1}, separators=(",", ":"))]
        before = [sys.getrefcount(inner_key), sys.getrefcount(outer_key)]
        for _ in range(100):
            strata.dumps(document, return_type=mode)
        after = [sys.getrefcount(inner_key), sys.getrefcount(outer_key)]
        assert len(nested) == 101
        return [now - was for now, was in zip(after, before, strict=True)]

    assert in_fresh_cache(body) == [0, 0]


# ---------------------------------------------------------------------------
# The hook image's own initialisation (docs/architecture/dumps_with_default.md,
# "Shared state"): it takes `strata._strata.config_get` at init, and refuses to
# initialise rather than run without it. Each case loads the image in a fresh
# interpreter, bypassing `strata/__init__.py` (which imports it eagerly).
# ---------------------------------------------------------------------------

_LOAD_HOOK_IMAGE = """
import importlib.machinery, importlib.util, sys, types
package = types.ModuleType("strata")
package.__path__ = []
sys.modules["strata"] = package
{setup}
loader = importlib.machinery.ExtensionFileLoader("strata._dumps_hook", {path!r})
spec = importlib.util.spec_from_loader("strata._dumps_hook", loader)
try:
    # An extension's init runs in create_module, inside module_from_spec.
    module = importlib.util.module_from_spec(spec)
    loader.exec_module(module)
except Exception as error:
    print(type(error).__name__)
else:
    print("loaded")
{after}
"""


def _run_hook_image(setup, after=""):
    code = _LOAD_HOOK_IMAGE.format(setup=setup, path=strata._dumps_hook.__file__, after=after)
    return subprocess.run([sys.executable, "-c", code], capture_output=True, text=True, check=True)


def _load_hook_image(setup):
    return _run_hook_image(setup).stdout.strip()


def test_the_hook_image_does_not_initialise_without_strata():
    # `import strata._strata` itself fails (a `None` entry in sys.modules is
    # CPython's ModuleNotFoundError): the init propagates it.
    assert _load_hook_image('sys.modules["strata._strata"] = None') == "ModuleNotFoundError"


def test_the_hook_image_does_not_initialise_without_config_get():
    # A `_strata` without `config_get` leaves no cycle policy to read.
    setup = 'sys.modules["strata._strata"] = types.ModuleType("strata._strata")'
    assert _load_hook_image(setup) == "AttributeError"


def test_a_return_type_with_no_utf8_form_is_refused():
    # api.md, dumps_with_default: `return_type` is `dumps`'s option; a `str`
    # with no UTF-8 form cannot name one, and the encoding error propagates.
    with pytest.raises(UnicodeEncodeError):
        strata.dumps_with_default([1], str, return_type="\ud800")


def test_a_failing_policy_read_is_reported_and_the_default_applies():
    # docs/architecture/dumps_with_default.md, "Shared state": the policy is read
    # from `_strata` at the cycle point; if that read fails, the failure is
    # reported (unraisable) and the process default, "warn", applies.
    setup = """
fake = types.ModuleType("strata._strata")
def config_get(key):
    raise RuntimeError("policy unavailable")
fake.config_get = config_get
sys.modules["strata._strata"] = fake
"""
    after = """
import warnings
cyclic = []
cyclic.append(cyclic)
with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    print(module.dumps_with_default(cyclic, str), len(caught))
"""
    completed = _run_hook_image(setup, after)
    assert completed.stdout.split() == ["loaded", "[null]", "1"]
    assert "policy unavailable" in completed.stderr


# The init comment (python_dumps_hook.cpp): single-phase init runs once per
# runtime, and a legacy subinterpreter gets a copy of the module's dict without
# running it again, so the statics the policy read uses stay the first init's.
# Pinned by the outcome: whichever interpreter imports the hook image first, the
# main interpreter still reads `_strata`'s policy after the subinterpreter ends.
_SUBINTERPRETER = """
import gc, sys, _testcapi
root = sys.argv[1]
sys.path.insert(0, root)
import strata
if {main_first}:
    strata.dumps_with_default([1], str)
assert _testcapi.run_in_subinterp(
    "import sys\\nsys.path.insert(0, %r)\\nimport strata\\n"
    "assert strata.dumps_with_default([1], str) == '[1]'" % root
) == 0
gc.collect()
cyclic = []
cyclic.append(cyclic)
strata.config.set("cycle_policy", "error")
try:
    strata.dumps_with_default(cyclic, str)
except ValueError as error:
    print(error)
"""


@pytest.mark.parametrize("main_first", [True, False], ids=["main-first", "subinterpreter-first"])
def test_the_hook_image_survives_a_legacy_subinterpreter(main_first):
    pytest.importorskip("_testcapi")
    root = str(pathlib.Path(strata.__file__).resolve().parent.parent)
    completed = subprocess.run(
        [sys.executable, "-c", _SUBINTERPRETER.format(main_first=main_first), root],
        capture_output=True,
        text=True,
        check=True,
    )
    assert completed.stdout.strip() == "Circular reference detected"

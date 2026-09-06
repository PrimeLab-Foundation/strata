"""Contract tests for the serializer's cycle stack, depth boundary and reentrancy.

Split from ``test_dumps.py`` (the ~800-LOC rule): the cycle/depth/reentrancy
clauses of ``docs/context/api.md`` (dumps, config) and the fused record
writer's frame semantics (``docs/architecture/fused_record_writer.md``), each
test naming the clause it pins. stdlib ``json`` is the oracle where the two
agree; the deliberate differences (cycle placeholders, the depth message) are
stated at the assertion.
"""

import json
import sys
import threading
import warnings

import pytest

import strata


def _compact(obj):
    return json.dumps(obj, separators=(",", ":"), ensure_ascii=False).encode()


def _both_modes(doc):
    out = strata.dumps(doc, return_type="bytes")
    assert out == _compact(doc)
    assert strata.dumps(doc).encode() == out
    return out


# ---------------------------------------------------------------------------
# The cycle stack: what `Serializer::open_` has to mean
#
# api.md § Config: 'On an actual cycle: "warn" emits `null` for the cyclic
# reference and raises `RuntimeWarning`; "error" raises `ValueError`; "ignore"
# emits `null` silently.' test_config.py pins that sentence on a self-
# referencing dict and list in str mode. The serializer reaches the same
# decision from four places -- `write_sequence`'s entry scan, `Frame`'s
# constructor, `write_record_fused`'s deferred push and `write_mapping`'s
# framed path -- and the answer has to be the same shape, the same policy and
# the same bytes through every one of them, in both output modes. These pin
# the stack's meaning: *the containers currently open above me, and nothing
# else*.
# ---------------------------------------------------------------------------


def _self_dict():
    node = {"name": "root"}
    node["self"] = node
    return node, '{"name":"root","self":null}'


def _self_list():
    node = [1]
    node.append(node)
    return node, "[1,null]"


def _mutual():
    first = {"name": "a"}
    second = {"name": "b", "a": first}
    first["b"] = second
    return first, '{"name":"a","b":{"name":"b","a":null}}'


def _through_a_tuple():
    # A tuple cannot hold itself, but it can close a cycle through a list --
    # and `write_sequence` serves both, so the tuple must be on the stack too.
    items = []
    pair = (1, items)
    items.append(pair)
    return pair, "[1,[null]]"


def _tuple_inside_a_dict():
    items = []
    node = {"t": (items,)}
    items.append(node)
    return node, '{"t":[[null]]}'


def _deep():
    root = {"a": {"b": {"c": []}}}
    root["a"]["b"]["c"].append(root)
    return root, '{"a":{"b":{"c":[null]}}}'


def _after_partial_output():
    # The cycle is only reached once three elements are already committed, so
    # the placeholder has to land in a half-written document.
    node = {"k": [1, 2, 3]}
    node["k"].append(node)
    return node, '{"k":[1,2,3,null]}'


def _after_a_stageful():
    # More than one 8 KB stage is committed before the cycle is found, so the
    # placeholder lands after the staged output has been flushed (str mode) or
    # after the block/stage switch (bytes mode).
    node = {"pad": ["x" * 100] * 200}
    node["self"] = node
    return node, json.dumps({"pad": ["x" * 100] * 200}, separators=(",", ":"))[
        :-1
    ] + ',"self":null}'


CYCLE_SHAPES = [
    ("self dict", _self_dict),
    ("self list", _self_list),
    ("mutual dicts", _mutual),
    ("through a tuple", _through_a_tuple),
    ("tuple inside a dict", _tuple_inside_a_dict),
    ("deep", _deep),
    ("after partial output", _after_partial_output),
    ("after a stageful", _after_a_stageful),
]


@pytest.mark.parametrize(("name", "build"), CYCLE_SHAPES, ids=[n for n, _ in CYCLE_SHAPES])
@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_every_cycle_shape_warns_and_emits_null(name, build, return_type):
    strata.config.set("cycle_policy", "warn")
    value, expected = build()
    with pytest.warns(RuntimeWarning, match="[Cc]ircular"):
        out = strata.dumps(value, return_type=return_type)
    assert out == (expected.encode() if return_type == "bytes" else expected)


@pytest.mark.parametrize(("name", "build"), CYCLE_SHAPES, ids=[n for n, _ in CYCLE_SHAPES])
@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_every_cycle_shape_raises_under_the_error_policy(name, build, return_type):
    strata.config.set("cycle_policy", "error")
    value, _ = build()
    with pytest.raises(ValueError, match="^Circular reference detected$"):
        strata.dumps(value, return_type=return_type)


@pytest.mark.parametrize(("name", "build"), CYCLE_SHAPES, ids=[n for n, _ in CYCLE_SHAPES])
@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_every_cycle_shape_is_silent_under_the_ignore_policy(name, build, return_type):
    strata.config.set("cycle_policy", "ignore")
    value, expected = build()
    with warnings.catch_warnings():
        warnings.simplefilter("error")  # any warning becomes a failure
        out = strata.dumps(value, return_type=return_type)
    assert out == (expected.encode() if return_type == "bytes" else expected)


@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_a_cycle_warning_filtered_to_an_error_stops_the_dump(return_type):
    """`PyErr_WarnEx`'s failure is propagated, not swallowed: a warnings filter
    set to "error" turns the cycle warning into a raised RuntimeWarning and the
    serialization stops there (python_dumps.cpp, both cycle paths)."""
    strata.config.set("cycle_policy", "warn")
    for build in (_self_dict, _self_list):
        value, _ = build()
        with warnings.catch_warnings():
            warnings.simplefilter("error")
            with pytest.raises(RuntimeWarning, match="[Cc]ircular"):
                strata.dumps(value, return_type=return_type)


def _on_a_fresh_thread(work):
    """Run `work` on a new thread, and re-raise whatever it raised.

    The prepared-key cache is thread-local and a depth *retires permanently*
    after 64 misses (python_dumps_output.h, `DepthSchemas::select`). The fused
    record writer needs a live, prepared schema, so any test that serializes
    enough one-off shapes at a shallow depth switches the fused path off for
    the rest of that thread's life -- and under the canonical run order
    `tests/py/test_json_parity.py`'s corpus does exactly that before
    `tests/unit` starts. Measured: in a fresh process the fused path serves
    these shapes; after that corpus it does not; on a new thread it does
    again. So a test that means to exercise the fused writer has to run on a
    thread that has not been retired.
    """
    raised = []

    def run():
        try:
            work()
        except BaseException as error:
            raised.append(error)

    thread = threading.Thread(target=run)
    thread.start()
    thread.join()
    if raised:
        raise raised[0]


@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_the_fused_record_writer_reports_the_same_cycles(return_type):
    """docs/architecture/fused_record_writer.md, invariants: 'record dicts
    holding containers get the same deferred-frame semantics (`open_` scan) as
    today'. The fused path only runs once a shape has repeated, so the cycle
    has to sit in a later record of an array of same-shaped records."""
    strata.config.set("cycle_policy", "warn")

    def work():
        records = [{"id": index, "name": f"n{index}", "tags": ["a"]} for index in range(8)]
        records[5]["tags"].append(records)  # record -> list -> the enclosing array
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            out = strata.dumps(records, return_type=return_type)
        assert [str(entry.message) for entry in caught] == ["Circular reference detected"]
        text = out.decode() if return_type == "bytes" else out
        assert text.count("null") == 1
        assert '{"id":5,"name":"n5","tags":["a",null]}' in text
        assert text.startswith('[{"id":0,"name":"n0","tags":["a"]}')

        # A record dict that reappears below itself, in the same array. The
        # placeholder lands on the record, not one level deeper: the fused
        # loop has to have pushed the record before walking its child.
        nested = [{"id": index, "child": {"x": index}} for index in range(6)]
        nested[4]["child"]["me"] = nested[4]
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            out = strata.dumps(nested, return_type=return_type)
        assert [str(entry.message) for entry in caught] == ["Circular reference detected"]
        text = out.decode() if return_type == "bytes" else out
        assert '{"id":4,"child":{"x":4,"me":null}}' in text
        assert text.count("null") == 1

    _on_a_fresh_thread(work)


@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_the_fused_record_writer_agrees_with_the_general_path(return_type):
    """Every deviation the fused writer refuses -- split table, holed table,
    width, way miss, unprepared or retired schema, subclass value, the depth
    boundary -- falls back to `write_mapping` with nothing committed, so the
    output is the general path's output. Run on a fresh thread so the fused
    path is the one actually taken (see `_on_a_fresh_thread`)."""

    class Instance:
        def __init__(self, index):
            self.alpha = index
            self.beta = [index]

    class MyStr(str):
        pass

    def compare(document):
        expected = json.dumps(document, separators=(",", ":"), ensure_ascii=False)
        out = strata.dumps(document, return_type=return_type)
        assert out == (expected.encode() if return_type == "bytes" else expected)

    def work():
        # A prepared schema, then each refusal in turn among its siblings.
        compare([{"id": index, "name": f"n{index}", "v": [index]} for index in range(8)])
        holed = {"id": 1, "gone": 0, "v": [1]}
        del holed["gone"]
        compare(
            [
                {"id": 0, "name": "n0", "v": [0]},
                {"id": 1, "name": "n1", "v": [1]},
                holed,  # entry count != size
                Instance(2).__dict__,  # split table
                {"id": 3, "name": MyStr("n3"), "v": [3]},  # subclass value
                {f"k{index}": index for index in range(25)},  # past kMaxSchemaKeys
                {"other": "shape"},  # way miss
                {"id": 4, "name": "n4", "v": [4]},  # back to the schema
            ],
        )
        # Escaped and non-ASCII keys, prepared once and re-emitted.
        compare([{'q"k': index, "café": index, "k" * 40: index} for index in range(6)])
        # Shapes that never repeat, so the depth retires mid-document.
        compare([{f"k{index}_{field}": field for field in range(3)} for index in range(200)])
        # ...and a repeating shape afterwards, on the retired depth.
        compare([{"p": 1, "q": 2} for _ in range(8)])

    _on_a_fresh_thread(work)


@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_a_child_that_is_not_an_ancestor_is_never_a_cycle(return_type):
    """The stack holds open containers only, so a popped frame must leave no
    trace: the same object may appear any number of times side by side and at
    any number of different depths. Asserted under "error", where a false
    positive is a raised exception rather than a quiet `null`."""
    strata.config.set("cycle_policy", "error")
    child = {"x": 1}
    twin = [1, 2]
    document = [
        child,
        [child, [child, [child, child]]],
        {"a": child, "b": [child], "c": {"d": child}},
        (child, twin, (twin, child)),
        [twin] * 8,
    ]
    expected = json.dumps(document, separators=(",", ":"))
    assert strata.dumps(document, return_type=return_type) == (
        expected.encode() if return_type == "bytes" else expected
    )

    # ...including the shape the fused writer serves: repeated records that
    # all share one inner dict.
    records = [{"id": index, "inner": child} for index in range(8)]
    expected = json.dumps(records, separators=(",", ":"))
    assert strata.dumps(records, return_type=return_type) == (
        expected.encode() if return_type == "bytes" else expected
    )


@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_an_aborted_dump_leaves_no_open_containers_behind(return_type):
    """Every early return out of the walk has to unwind the stack it pushed.
    If any frame survived the call, an object that was open when the dump
    failed would be mistaken for an ancestor of the *next* dump -- so each
    failure below is followed by a dump of the very objects that were open,
    under "error", where a stale frame raises."""
    strata.config.set("cycle_policy", "error")
    marker = {"m": 1}
    reuse = [marker, marker, {"a": marker, "b": [marker]}]
    expected = json.dumps(reuse, separators=(",", ":"))
    expected_out = expected.encode() if return_type == "bytes" else expected

    # 1. the depth limit, tripped far below the root
    chain = marker
    for _ in range(sys.getrecursionlimit() + 5):
        chain = [chain]
    with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
        strata.dumps(chain, return_type=return_type)
    assert strata.dumps(reuse, return_type=return_type) == expected_out

    # 2. a cycle under the error policy, with the marker open above it
    cyclic = {"inner": marker, "deep": {"d": 1}}
    cyclic["deep"]["back"] = cyclic
    with pytest.raises(ValueError, match="^Circular reference detected$"):
        strata.dumps(cyclic, return_type=return_type)
    assert strata.dumps(reuse, return_type=return_type) == expected_out

    # 3. an unsupported type, with the marker open above it
    with pytest.raises(TypeError, match="not JSON serializable"):
        strata.dumps({"a": marker, "b": [marker, object()]}, return_type=return_type)
    assert strata.dumps(reuse, return_type=return_type) == expected_out

    # 4. a non-str key, found after the walk has already descended
    with pytest.raises(TypeError, match="key"):
        strata.dumps({"a": marker, "b": {1: "x"}}, return_type=return_type)
    assert strata.dumps(reuse, return_type=return_type) == expected_out

    # 5. and a *successful* deep dump must not leave the stack loaded either
    deep = marker
    for _ in range(500):
        deep = [deep]
    strata.dumps(deep, return_type=return_type)
    assert strata.dumps(reuse, return_type=return_type) == expected_out


# ---------------------------------------------------------------------------
# 'Raises ValueError ("Maximum serialization depth exceeded" at
#  `sys.getrecursionlimit()`)' -- the boundary itself, not just "deep enough
#  raises". The limit is read once per call, counted in open containers, and
#  the same for every container shape and both output modes; api.md also
#  records the asymmetry with the parser's own 1024-container cap.
# ---------------------------------------------------------------------------


def _nested_lists(depth):
    root = []
    node = root
    for _ in range(depth - 1):
        child = []
        node.append(child)
        node = child
    return root


def _nested_dicts(depth):
    root = {}
    node = root
    for _ in range(depth - 1):
        child = {}
        node["a"] = child
        node = child
    return root


def _nested_mixed(depth):
    # An all-scalar leaf dict is the frame-elided shape: at the boundary it
    # must still take the framed path, so the error stays byte-identical.
    node = {"s": 1}
    for level in range(depth - 1):
        node = [node] if level % 2 == 0 else {"a": node}
    return node


def _nested_records(depth):
    # The fused writer's shape, buried under `depth - 3` containers: the array,
    # its records and their inner list are the last three levels.
    node = [{"id": index, "v": [index]} for index in range(4)]
    for _ in range(depth - 3):
        node = [node]
    return node


DEPTH_SHAPES = [
    ("lists", _nested_lists),
    ("dicts", _nested_dicts),
    ("mixed", _nested_mixed),
    ("records", _nested_records),
]


def _python_stack_depth():
    depth = 0
    frame = sys._getframe()
    while frame is not None:
        depth += 1
        frame = frame.f_back
    return depth


@pytest.mark.parametrize(("name", "build"), DEPTH_SHAPES, ids=[n for n, _ in DEPTH_SHAPES])
@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_the_depth_limit_is_exactly_the_recursion_limit(name, build, return_type):
    """N open containers serialize, N + 1 do not, where N is the interpreter's
    recursion limit at the moment of the call. Checked at a lowered limit so
    the boundary is cheap to reach, on every container shape."""
    saved = sys.getrecursionlimit()
    # Leave the interpreter enough frames for pytest's own machinery; the
    # serializer's containers are C++ frames, not Python ones.
    limit = max(300, _python_stack_depth() + 120)
    try:
        sys.setrecursionlimit(limit)
        at_limit = strata.dumps(build(limit), return_type=return_type)
        assert len(at_limit) > limit  # it really did emit that many containers
        with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
            strata.dumps(build(limit + 1), return_type=return_type)
    finally:
        sys.setrecursionlimit(saved)


@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_the_depth_limit_follows_the_recursion_limit_when_it_moves(return_type):
    """The limit is read per call, so raising it raises the ceiling with it --
    the same document that was refused a moment ago now serializes."""
    saved = sys.getrecursionlimit()
    try:
        document = _nested_lists(saved + 40)
        with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
            strata.dumps(document, return_type=return_type)
        sys.setrecursionlimit(saved + 64)
        out = strata.dumps(document, return_type=return_type)
        assert out.startswith(b"[[[" if return_type == "bytes" else "[[[")
        assert len(out) == 2 * (saved + 40)
    finally:
        sys.setrecursionlimit(saved)


def test_at_the_default_limit_the_boundary_is_the_default_limit():
    """The same boundary without touching `sys.setrecursionlimit` at all."""
    limit = sys.getrecursionlimit()
    assert strata.dumps(_nested_lists(limit)).startswith("[[[")
    with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
        strata.dumps(_nested_lists(limit + 1))


def test_a_document_at_the_parse_cap_needs_a_raised_limit_to_serialize():
    """api.md § Parse & serialize: "The serializer's limit is the interpreter's
    recursion limit (1000 by default), the parser's is 1024 containers, so a
    tree parsed at depth 1001-1024 needs a raised `sys.setrecursionlimit` to
    serialize again". The asymmetry is documented, so it is pinned."""
    saved = sys.getrecursionlimit()
    text = "[" * 1024 + "1" + "]" * 1024
    parsed = strata.loads(text)  # the parser accepts exactly 1024 containers
    try:
        if saved < 1024:
            with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
                strata.dumps(parsed)
        sys.setrecursionlimit(max(saved, 1100))
        assert strata.dumps(parsed) == text
        assert strata.dumps(parsed, return_type="bytes") == text.encode()
    finally:
        sys.setrecursionlimit(saved)


@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_a_cycle_is_reported_before_the_depth_limit(return_type):
    """At the boundary both answers are available -- the container is already
    open *and* one level too deep. The cycle wins, on both the framed and the
    unframed path, so a cyclic document never reports a depth error."""
    strata.config.set("cycle_policy", "warn")
    saved = sys.getrecursionlimit()
    limit = max(300, _python_stack_depth() + 120)
    try:
        sys.setrecursionlimit(limit)
        for depth in (limit - 1, limit):
            root = []
            node = root
            for _ in range(depth - 1):
                child = []
                node.append(child)
                node = child
            node.append(root)  # the innermost list points back at the root
            with pytest.warns(RuntimeWarning, match="[Cc]ircular"):
                out = strata.dumps(root, return_type=return_type)
            expected = "[" * depth + "null" + "]" * depth
            assert out == (expected.encode() if return_type == "bytes" else expected)
    finally:
        sys.setrecursionlimit(saved)


# ---------------------------------------------------------------------------
# Re-entrancy: arbitrary Python can run in the middle of a walk (a cycle
# warning's handler, an int subclass's `__str__`), and it can call `dumps`
# again. api.md pins nothing about the nested call's *state*; the code does
# (python_dumps.cpp `shared_busy`, python_dumps_output.h `SchemaCacheLease`'s
# busy flag, and the per-call cycle stack). These pin the consequence: the
# nested call is a fresh serialization that shares no state with the outer
# one.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("outer_type", ["str", "bytes"])
@pytest.mark.parametrize("inner_type", ["str", "bytes"])
def test_a_nested_dump_does_not_inherit_the_outer_call_s_open_containers(outer_type, inner_type):
    """The container being serialized above is *not* an ancestor of the nested
    document, and must not be reported as a cycle. Asserted under "error", so
    a shared stack raises instead of quietly emitting `null`."""
    strata.config.set("cycle_policy", "error")
    captured = []
    reentered = []
    inner = {}

    class Big(int):
        def __str__(self):
            if not reentered:
                reentered.append(1)
                captured.append(strata.dumps(inner, return_type=inner_type))
            return int.__str__(self)

    inner["big"] = Big(2**70)
    out = strata.dumps({"wrap": inner}, return_type=outer_type)

    expected_inner = '{"big":1180591620717411303424}'
    expected_outer = '{"wrap":{"big":1180591620717411303424}}'
    assert out == (expected_outer.encode() if outer_type == "bytes" else expected_outer)
    assert captured == [expected_inner.encode() if inner_type == "bytes" else expected_inner]


def test_a_nested_dump_finds_its_own_cycles():
    """The nested call keeps its own stack, so it detects the cycles in *its*
    document -- neither inheriting nor disturbing the outer one."""
    strata.config.set("cycle_policy", "warn")
    inner = {"i": 1}
    inner["self"] = inner
    outer = {"o": 1}
    outer["self"] = outer
    seen = []
    guard = []

    def handler(*args, **kwargs):
        if not guard:
            guard.append(1)
            with warnings.catch_warnings():
                warnings.simplefilter("ignore")
                seen.append(strata.dumps(inner))
                seen.append(strata.dumps(inner, return_type="bytes"))

    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = handler
        out = strata.dumps(outer)

    assert out == '{"o":1,"self":null}'
    assert seen == ['{"i":1,"self":null}', b'{"i":1,"self":null}']


def test_a_dump_from_a_duplicate_key_warning_is_unaffected():
    """The other re-entrancy door: `loads` under
    `duplicate_key_policy="warn"` runs a warning handler mid-parse, and that
    handler may serialize (api.md § Config)."""
    strata.config.set("duplicate_key_policy", "warn")
    seen = []
    with warnings.catch_warnings():
        warnings.simplefilter("always")
        warnings.showwarning = lambda *a, **k: seen.append(strata.dumps({"w": [1, 2]}))
        parsed = strata.loads('{"a":1,"a":2}')
    assert parsed == {"a": 1}
    assert seen == ['{"w":[1,2]}']


def test_dumps_on_several_threads_keeps_its_state_apart():
    """The stage, the size hint and the schema cache are per-thread, and the
    cycle stack is per-call: concurrent dumps must not interfere."""
    documents = [
        [{"id": index, "name": f"n{index}", "v": [index, index * 1.5]} for index in range(200)]
        for _ in range(4)
    ]
    expected = [json.dumps(document, separators=(",", ":")) for document in documents]
    failures = []

    def work(slot):
        try:
            for _ in range(20):
                assert strata.dumps(documents[slot]) == expected[slot]
                assert strata.dumps(documents[slot], return_type="bytes") == expected[slot].encode()
                shared = {"s": 1}
                assert strata.dumps([shared, shared]) == '[{"s":1},{"s":1}]'
        except BaseException as error:
            failures.append(repr(error))

    workers = [threading.Thread(target=work, args=(slot,)) for slot in range(4)]
    for worker in workers:
        worker.start()
    for worker in workers:
        worker.join()
    assert failures == []


# ---------------------------------------------------------------------------
# The paths the cycle stack shares its hot loops with: big integers (whose
# `__str__` is the second re-entrancy door), genuine subclasses (which are
# never "plain scalars", so they change what gets a frame), the dict layouts
# the raw walk refuses, and schema churn.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "value",
    [
        2**63 - 1,
        2**63,
        2**63 + 1,
        -(2**63),
        -(2**63) - 1,
        2**64 - 1,
        2**64,
        2**64 + 1,
        -(2**64),
        10**100,
        -(10**100),
    ],
)
def test_big_integers_keep_every_digit_in_both_modes(value):
    assert strata.dumps(value) == str(value)
    assert strata.dumps(value, return_type="bytes") == str(value).encode()


def test_an_integer_run_hands_over_at_the_first_value_past_int64():
    """`run_ints` renders compact words and stops at the first one it cannot;
    the general path then renders that value through its `str`, so a mixed
    list must read exactly as the oracle does."""
    for values in (
        [1, 2**63, -(2**64), 2**63 - 1, 7],
        [2**70, 1, 2, 3],
        [1, 2, 3, 10**40],
        [-(2**63), -(2**63) - 1, 0],
    ):
        _both_modes(values)
        _both_modes({"n": values, "tail": 1})


@pytest.mark.parametrize("return_type", ["str", "bytes"])
def test_genuine_subclasses_serialize_as_their_base_types(return_type):
    """A subclass is not an exact type, so it misses every exact-type fast
    path and is never a "plain scalar" -- which also means a container holding
    one is framed where the exact-typed twin would not be. The output is the
    base type's output all the same (`bool` cannot be subclassed)."""

    class MyDict(dict):
        pass

    class MyList(list):
        pass

    class MyTuple(tuple):
        pass

    class MyStr(str):
        pass

    class MyInt(int):
        pass

    class MyFloat(float):
        pass

    document = {
        "dict": MyDict(a=1, b=[2]),
        "list": MyList([1, {"a": 2}]),
        "tuple": MyTuple([1, 2]),
        "str": MyStr('v"x'),
        "int": MyInt(5),
        "bigint": MyInt(-(2**70)),
        "float": MyFloat(1.5),
        "nonfinite": MyFloat(float("inf")),
        MyStr("subclass key"): 1,
    }
    expected = (
        '{"dict":{"a":1,"b":[2]},"list":[1,{"a":2}],"tuple":[1,2],"str":"v\\"x",'
        '"int":5,"bigint":-1180591620717411303424,"float":1.5,"nonfinite":null,'
        '"subclass key":1}'
    )
    assert strata.dumps(document, return_type=return_type) == (
        expected.encode() if return_type == "bytes" else expected
    )

    # And in the shape the fused writer serves: repeated records whose values
    # are subclasses, so the frame-elision decision differs record by record.
    records = [{"a": MyStr("x"), "b": index, "c": MyInt(index)} for index in range(6)]
    plain = [{"a": "x", "b": index, "c": index} for index in range(6)]
    expected = json.dumps(plain, separators=(",", ":"))
    assert strata.dumps(records, return_type=return_type) == (
        expected.encode() if return_type == "bytes" else expected
    )

    # A list subclass can hold itself, like any other list.
    strata.config.set("cycle_policy", "warn")
    cyclic = MyList([1])
    cyclic.append(cyclic)
    with pytest.warns(RuntimeWarning, match="[Cc]ircular"):
        assert strata.dumps(cyclic, return_type=return_type) == (
            b"[1,null]" if return_type == "bytes" else "[1,null]"
        )


def test_dict_layouts_the_raw_walk_has_to_refuse():
    """python_dumps_output.h `rawdict`: split tables and tables whose entry
    count disagrees with the size fall back to `PyDict_Next`, and the fused
    writer refuses them outright. The fallback is invisible in the output --
    which is exactly what these pin."""

    class Instance:
        def __init__(self, index):
            self.alpha = index
            self.beta = "two"
            self.gamma = [index]

    # Instance dictionaries are the split-table shape.
    _both_modes([Instance(index).__dict__ for index in range(6)])

    # A hole in the entry array: `PyDict_GET_SIZE` and `dk_nentries` disagree.
    holed = []
    for index in range(6):
        record = {"a": index, "b": "gone", "c": index * 2}
        del record["b"]
        holed.append(record)
    _both_modes(holed)

    # Deleted, then refilled: insertion order is what the output follows.
    refilled = []
    for index in range(6):
        record = {"a": index, "b": "gone", "c": index * 2}
        del record["b"]
        record.update({"d": index, "b": index + 1})
        refilled.append(record)
    _both_modes(refilled)
    assert strata.dumps(refilled[0]) == '{"a":0,"c":0,"d":0,"b":1}'

    # One dict reached through several records: the same keys object is walked
    # again at a different depth every time.
    shared = {"x": 1, "y": 2}
    _both_modes([{"id": index, "inner": shared} for index in range(6)])
    _both_modes({"a": shared, "b": {"c": shared}, "d": [shared, shared]})


def test_schemas_that_never_repeat_still_serialize_exactly():
    """A depth whose shapes never repeat retires its schema cache; the plain
    key walk takes over, and later documents that *do* repeat are unaffected."""
    churn = [{f"k{index}_{field}": field for field in range(3)} for index in range(200)]
    _both_modes(churn)
    _both_modes([{"p": 1, "q": 2} for _ in range(8)])
    # Churn nested under a repeating parent, so one depth retires and its
    # parent's depth does not.
    _both_modes([{"id": index, "shape": {f"z{index}": index}} for index in range(200)])


def test_objects_wider_than_the_schema_row():
    """`kMaxSchemaKeys` is 24: at 25 keys the object takes the uncached walk,
    and the output must not notice."""
    for width in (23, 24, 25, 40):
        records = [{f"k{index}": index for index in range(width)} for _ in range(4)]
        _both_modes(records)
    # Mixed widths at one depth, so the cache is asked about both kinds.
    _both_modes([{f"k{index}": index for index in range(width)} for width in (24, 25, 24, 25)])


def test_keys_that_need_escaping_are_prepared_once_and_stay_right():
    """Prepared keys come from the same escaper as any other string, so an
    escaped, non-ASCII or over-wide key repeats byte-for-byte across records."""
    records = [
        {
            'quote"key': index,
            "tab\tkey": index,
            "café": index,
            "\U0001f600": index,
            "ctrl\x01": index,
            "k" * 40: index,  # past the 16-byte inline slot
        }
        for index in range(6)
    ]
    _both_modes(records)


def test_string_runs_hand_back_mid_run():
    """`run_strings` copies while scanning and rewinds the separator and quote
    it had already staged the moment a string needs escaping, is non-ASCII, or
    is too long for the reservation -- and the general path picks up exactly
    there."""
    for values in (
        ["a", "b", 'c"d', "e"],
        ["a", "b", "c\\d", "e", "f"],
        ["a", "café", "b"],
        ["a", "x" * 5000, "b"],
        ["a" * 4090, "b"],
        ["", "", 'q"', ""],
        ["a", "line\nbreak", "b", "\x01", "c"],
    ):
        _both_modes(values)
        _both_modes({"v": values})

"""Dumps contract: the cycle placeholder replaces the first repeated container.

api.md (Config, `cycle_policy`): on an actual cycle, "warn" emits null for the
cyclic reference and raises RuntimeWarning, "error" raises ValueError, and
"ignore" emits null silently. The fused record writer takes a dict whose shape
is already prepared at its depth, so every case here runs twice on a fresh
thread: cold, and after warming the same shape three times. The bytes must be
the same both times -- output is a function of the input, not of the
thread's schema-cache history -- and the null must stand where the repeated
container is reached, not one container later.
"""

import json
import threading
import warnings

import pytest

import strata

MODES = ("str", "bytes")
WIDE_KEYS = tuple(f"k{index}" for index in range(24))


def _self():
    document = {"name": "root"}
    document["self"] = document
    return document


def _two_levels():
    document = {"a": 1, "b": None}
    document["b"] = {"a": 2, "b": document}
    return document


def _mutual():
    first = {"x": 1, "y": None}
    first["y"] = {"p": 2, "q": first}
    return first


def _list_root():
    root = []
    root.append({"k": root})
    return root


def _wide():
    document = dict.fromkeys(WIDE_KEYS[:-1], 1)
    document[WIDE_KEYS[-1]] = document
    return document


def _pair():
    first = {"name": "a", "self": None}
    first["self"] = {"name": "b", "self": first}
    return first


# (label, cyclic document, expected bytes, an acyclic document of the same
# shapes that prepares the schema at every depth the cycle revisits)
CASES = [
    (
        "self",
        _self,
        '{"name":"root","self":null}',
        {"name": "root", "self": {"name": "x", "self": 1}},
    ),
    (
        "two_levels",
        _two_levels,
        '{"a":1,"b":{"a":2,"b":null}}',
        {"a": 1, "b": {"a": 1, "b": {"a": 1, "b": 0}}},
    ),
    (
        "mutual",
        _mutual,
        '{"x":1,"y":{"p":2,"q":null}}',
        {"x": 1, "y": {"p": 1, "q": {"x": 1, "y": {"p": 1, "q": 0}}}},
    ),
    (
        # The re-entered container is the root list itself, so this null
        # comes from the sequence loop's own probe; the case pins the
        # contract's placement, not the record writer's probe.
        "list_root",
        _list_root,
        '[{"k":null}]',
        [{"k": [{"k": [1]}]}],
    ),
    (
        "wide",
        _wide,
        "{" + ",".join(f'"{key}":1' for key in WIDE_KEYS[:-1]) + ',"k23":null}',
        {
            **dict.fromkeys(WIDE_KEYS[:-1], 1),
            WIDE_KEYS[-1]: {**dict.fromkeys(WIDE_KEYS[:-1], 1), WIDE_KEYS[-1]: 0},
        },
    ),
    (
        "pair",
        _pair,
        '{"name":"a","self":{"name":"b","self":null}}',
        {"name": "root", "self": {"name": "x", "self": {"name": "y", "self": 1}}},
    ),
]


def _dump(value, mode):
    if mode == "bytes":
        return strata.dumps(value, return_type="bytes").decode()
    return strata.dumps(value)


def _on_a_fresh_thread(call):
    failure = []

    def run():
        try:
            call()
        except BaseException as error:  # noqa: BLE001 -- re-raised on the test thread
            failure.append(error)

    thread = threading.Thread(target=run)
    thread.start()
    thread.join()
    if failure:
        raise failure[0]


def _with_policy(policy, call):
    previous = strata.config.get("cycle_policy")
    strata.config.set("cycle_policy", policy)
    try:
        _on_a_fresh_thread(call)
    finally:
        strata.config.set("cycle_policy", previous)


def _warm(document, warmed, mode):
    if warmed:
        for _ in range(3):
            _dump(document, mode)


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("warmed", (False, True))
@pytest.mark.parametrize("case", CASES, ids=[case[0] for case in CASES])
def test_warn_places_the_null_at_the_repeated_container(case, warmed, mode):
    _label, make, expected, warm = case

    def run():
        _warm(warm, warmed, mode)
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            assert _dump(make(), mode) == expected
        assert [str(w.message) for w in caught] == ["Circular reference detected"]

    _with_policy("warn", run)


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("warmed", (False, True))
@pytest.mark.parametrize("case", CASES, ids=[case[0] for case in CASES])
def test_error_raises_at_the_repeated_container(case, warmed, mode):
    _label, make, expected, warm = case

    def run():
        _warm(warm, warmed, mode)
        with pytest.raises(ValueError, match="Circular reference detected"):
            _dump(make(), mode)

    _with_policy("error", run)


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("warmed", (False, True))
@pytest.mark.parametrize("case", CASES, ids=[case[0] for case in CASES])
def test_ignore_is_the_same_bytes_without_a_warning(case, warmed, mode):
    _label, make, expected, warm = case

    def run():
        _warm(warm, warmed, mode)
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            assert _dump(make(), mode) == expected
        assert caught == []

    _with_policy("ignore", run)


# A record whose scalar prefix carries the output across the stage (str mode
# flushes every 8 KiB) or past the exact-fit block (bytes mode sizes the
# block to the previous document, so `warm_delta` bytes of growth land the
# boundary inside the record; past the stage's size the block grows and the
# stage moves in). The null must still replace the whole record.
PAD = [{"id": index, "name": f"row-{index}"} for index in range(64)]
BOUNDARIES = [
    (0, 0),
    (100, 40),
    (500, 40),
    (2000, 40),
    (9000, 40),
    (9000, 9000),
    (20000, 40),
    (20000, 9000),
]


def _dict_value_cycle(prefix):
    record = {"s": prefix, "self": None}
    record["self"] = record
    return record


def _dict_value_warm(prefix):
    return {"s": prefix, "self": {"s": "y", "self": 1}}


def _dict_value_expected(prefix):
    return {"s": prefix, "self": None}


BOUNDARY_SHAPES = [("dict_value", _dict_value_cycle, _dict_value_warm, _dict_value_expected)]


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize(("prefix_length", "warm_delta"), BOUNDARIES)
@pytest.mark.parametrize("shape", BOUNDARY_SHAPES, ids=[shape[0] for shape in BOUNDARY_SHAPES])
def test_the_placeholder_survives_an_output_boundary_inside_the_record(
    shape, prefix_length, warm_delta, mode
):
    _label, make, warm, expected = shape

    def run():
        prefix = "x" * prefix_length
        for _ in range(3):
            _dump([*PAD, warm("x" * max(0, prefix_length - warm_delta))], mode)
        document = [*PAD, make(prefix)]
        want = json.dumps([*PAD, expected(prefix)], separators=(",", ":"))
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            assert _dump(document, mode) == want
        assert [str(w.message) for w in caught] == ["Circular reference detected"]

    _with_policy("warn", run)

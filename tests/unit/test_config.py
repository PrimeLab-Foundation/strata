"""Contract tests for `strata.config`.

Each test pins a clause of docs/context/api.md § Config and § Error contract.
The autouse fixture in conftest.py restores every setting afterwards.
"""

import threading
import warnings

import pytest

import strata

# ---------------------------------------------------------------------------
# "strata.config.set(key, value); strata.config.get(key); strata.config.list()"
# ---------------------------------------------------------------------------


def test_list_reports_every_setting():
    settings = strata.config.list()
    assert set(settings) == {"duplicate_key_policy", "cycle_policy"}


def test_get_reflects_what_set_stored():
    strata.config.set("duplicate_key_policy", "last")
    assert strata.config.get("duplicate_key_policy") == "last"
    assert strata.config.list()["duplicate_key_policy"] == "last"


# ---------------------------------------------------------------------------
# '`duplicate_key_policy`: "first" (default) | "last" | "error" | "warn"'
# ---------------------------------------------------------------------------


def test_duplicate_key_policy_defaults_to_first():
    assert strata.config.get("duplicate_key_policy") == "first"


@pytest.mark.parametrize("value", ["first", "last", "error", "warn"])
def test_duplicate_key_policy_accepts_every_documented_value(value):
    strata.config.set("duplicate_key_policy", value)
    assert strata.config.get("duplicate_key_policy") == value


# ---------------------------------------------------------------------------
# '`cycle_policy`: "warn" (default, **active from process start** - reported
#  and actual behavior always agree; the previous implementation started on
#  `ignore` while reporting `warn`, do not reproduce) | "error" | "ignore"'
# ---------------------------------------------------------------------------


def test_cycle_policy_defaults_to_warn():
    assert strata.config.get("cycle_policy") == "warn"


def test_the_default_cycle_policy_is_in_force_without_being_set():
    """The reported default must be the behaviour, before any config.set call.

    The previous implementation seeded the reported value to "warn" while the
    variable driving the behaviour started at "ignore", so a fresh process
    silently ignored cycles while claiming to warn about them.
    """
    assert strata.config.get("cycle_policy") == "warn"

    cyclic = {}
    cyclic["self"] = cyclic
    with pytest.warns(RuntimeWarning, match="[Cc]ircular"):
        assert strata.dumps(cyclic) == '{"self":null}'


@pytest.mark.parametrize("value", ["warn", "error", "ignore"])
def test_cycle_policy_accepts_every_documented_value(value):
    strata.config.set("cycle_policy", value)
    assert strata.config.get("cycle_policy") == value


# ---------------------------------------------------------------------------
# 'On an actual cycle: "warn" emits `null` for the cyclic reference and raises
#  `RuntimeWarning`; "error" raises `ValueError`; "ignore" emits `null`
#  silently.'
# ---------------------------------------------------------------------------


def _self_referencing_dict():
    node = {"name": "root"}
    node["self"] = node
    return node


def _self_referencing_list():
    node = [1]
    node.append(node)
    return node


def test_cycle_policy_warn_emits_null_and_warns():
    strata.config.set("cycle_policy", "warn")
    with pytest.warns(RuntimeWarning, match="[Cc]ircular"):
        assert strata.dumps(_self_referencing_dict()) == '{"name":"root","self":null}'
    with pytest.warns(RuntimeWarning, match="[Cc]ircular"):
        assert strata.dumps(_self_referencing_list()) == "[1,null]"


def test_cycle_policy_error_raises_value_error():
    strata.config.set("cycle_policy", "error")
    with pytest.raises(ValueError, match="[Cc]ircular"):
        strata.dumps(_self_referencing_dict())


def test_cycle_policy_ignore_emits_null_silently():
    strata.config.set("cycle_policy", "ignore")
    with warnings.catch_warnings():
        warnings.simplefilter("error")  # any warning becomes a failure
        assert strata.dumps(_self_referencing_dict()) == '{"name":"root","self":null}'


def test_a_repeated_but_acyclic_reference_is_not_a_cycle():
    """The same object twice side by side is fine; only nesting is a cycle."""
    shared = {"n": 1}
    strata.config.set("cycle_policy", "error")
    assert strata.dumps([shared, shared]) == '[{"n":1},{"n":1}]'


# ---------------------------------------------------------------------------
# "Config state is process-global at the map level. `duplicate_key_policy` is
#  consumed via a **thread-local** variable - it does not propagate to other
#  threads. `cycle_policy` is a plain process-global - it affects all threads."
# ---------------------------------------------------------------------------


def test_duplicate_key_policy_does_not_propagate_to_other_threads():
    strata.config.set("duplicate_key_policy", "last")
    observed = {}

    def read_in_thread():
        observed["parsed"] = strata.loads('{"a": 1, "a": 2}')

    worker = threading.Thread(target=read_in_thread)
    worker.start()
    worker.join()

    assert strata.loads('{"a": 1, "a": 2}') == {"a": 2}  # this thread: last wins
    assert observed["parsed"] == {"a": 1}  # other thread: the default


def test_cycle_policy_does_propagate_to_other_threads():
    strata.config.set("cycle_policy", "error")
    observed = {}

    def dump_in_thread():
        try:
            strata.dumps(_self_referencing_dict())
            observed["raised"] = None
        except ValueError as error:  # noqa: PERF203 - the point of the test
            observed["raised"] = type(error).__name__

    worker = threading.Thread(target=dump_in_thread)
    worker.start()
    worker.join()

    assert observed["raised"] == "ValueError"


# ---------------------------------------------------------------------------
# "Unknown config key => `KeyError` on `config.set` (`config.get` returns
#  `None`); bad config value => `ValueError`; wrong value type => `TypeError`."
# ---------------------------------------------------------------------------


def test_setting_an_unknown_key_raises_key_error():
    with pytest.raises(KeyError, match="unknown config key"):
        strata.config.set("no_such_setting", "value")


def test_getting_an_unknown_key_returns_none():
    assert strata.config.get("no_such_setting") is None


@pytest.mark.parametrize(
    ("key", "value"),
    [
        ("duplicate_key_policy", "nonsense"),
        ("duplicate_key_policy", ""),
        ("cycle_policy", "nonsense"),
        ("cycle_policy", "first"),  # valid for the other key, not this one
    ],
)
def test_a_bad_value_raises_value_error(key, value):
    with pytest.raises(ValueError, match="must be"):
        strata.config.set(key, value)


@pytest.mark.parametrize(("key", "value"), [("duplicate_key_policy", 1), ("cycle_policy", None)])
def test_a_wrong_value_type_raises_type_error(key, value):
    with pytest.raises(TypeError, match="must be a string"):
        strata.config.set(key, value)


def test_a_rejected_set_leaves_the_setting_unchanged():
    strata.config.set("cycle_policy", "ignore")
    with pytest.raises(ValueError):
        strata.config.set("cycle_policy", "nonsense")
    assert strata.config.get("cycle_policy") == "ignore"

"""Contract tests for a `dumps` key the serializer cannot encode.

Pins the docs/context/api.md § Parse & serialize clause: a `str` key or value
with no UTF-8 encoding — a lone surrogate — raises `UnicodeEncodeError`, on
every call, and the schema cache never remembers a shape it failed to prepare.

Before the fix, `build_schema`'s failure arm emptied the schema's key vector
while leaving the way matching, so the third call rebuilt the schema over no
keys, declared it prepared with every span zero, and emitted a record with no
key bytes at all: `strata.dumps({"\\ud800": 1, "b": 2})` returned `'{1,2}'` —
invalid JSON, silently — and `strata.dump` wrote it to disk.

The schema cache is thread-local, so every test that depends on cache state
runs its body on a fresh thread: a shape remembered, or a depth retired, by
one test must not reach the next.
"""

import gc
import json
import sys
import threading
import weakref

import pytest

import strata

# A lone high surrogate; `"".encode()` raises on it, so it has no JSON form.
LONE_HIGH = "\ud800"
LONE_LOW = "\udfff"


def in_fresh_cache(body):
    """Run `body` on a new thread and re-raise whatever it raised.

    `SchemaCacheLease`'s slots are `thread_local`, so a new thread starts with
    an empty schema cache and no retired depth.
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


def dumps_raises(payload, *, return_type="str"):
    """Serialize and return the exception type name, or the value if it passed."""
    try:
        return strata.dumps(payload, return_type=return_type)
    except UnicodeEncodeError:
        return UnicodeEncodeError


# ---------------------------------------------------------------------------
# The reproducer, and the repetition that used to break it
# ---------------------------------------------------------------------------


def test_a_lone_surrogate_key_raises_on_the_first_call():
    def body():
        with pytest.raises(UnicodeEncodeError):
            strata.dumps({LONE_HIGH: 1, "b": 2})

    in_fresh_cache(body)


def test_a_lone_surrogate_key_raises_on_every_repeat():
    """The regression: calls 3+ used to return `'{1,2}'`."""

    def body():
        record = {LONE_HIGH: 1, "b": 2}
        return [dumps_raises(record) for _ in range(20)]

    assert in_fresh_cache(body) == [UnicodeEncodeError] * 20


def test_a_lone_surrogate_key_raises_on_every_repeat_in_bytes_mode():
    def body():
        record = {LONE_HIGH: 1, "b": 2}
        return [dumps_raises(record, return_type="bytes") for _ in range(20)]

    assert in_fresh_cache(body) == [UnicodeEncodeError] * 20


def test_a_nested_lone_surrogate_key_raises_on_every_repeat():
    """Nested it went wrong from the *first* call: `[{1,2}]`."""

    def body():
        return [dumps_raises([{LONE_HIGH: 1, "b": 2}]) for _ in range(20)]

    assert in_fresh_cache(body) == [UnicodeEncodeError] * 20


def test_a_deeply_nested_lone_surrogate_key_raises_on_every_repeat():
    def body():
        payload = {"outer": [{"inner": [{LONE_HIGH: 1, "b": 2}]}]}
        return [dumps_raises(payload) for _ in range(20)]

    assert in_fresh_cache(body) == [UnicodeEncodeError] * 20


# ---------------------------------------------------------------------------
# Every position a failing key can occupy
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "record",
    [
        pytest.param({LONE_HIGH: 1, "b": 2, "c": 3}, id="first-key"),
        pytest.param({"a": 1, LONE_HIGH: 2, "c": 3}, id="middle-key"),
        pytest.param({"a": 1, "b": 2, LONE_HIGH: 3}, id="last-key"),
        pytest.param({LONE_LOW: 1, "b": 2}, id="lone-low-surrogate"),
        pytest.param({"ok" + LONE_HIGH + "tail": 1, "b": 2}, id="surrogate-mid-key"),
        pytest.param({LONE_HIGH: 1}, id="only-key"),
        pytest.param({LONE_HIGH * 12: 1, "b": 2}, id="key-wider-than-a-slot"),
        pytest.param(
            {**{f"k{index}": index for index in range(30)}, LONE_HIGH: 1},
            id="wider-than-the-schema-cache",
        ),
    ],
)
def test_every_failing_key_position_raises_on_every_repeat(record):
    def body():
        return [dumps_raises(record) for _ in range(20)]

    assert in_fresh_cache(body) == [UnicodeEncodeError] * 20


def test_a_lone_surrogate_value_raises_on_every_repeat():
    """Values are not cached, but the contract is the same one."""

    def body():
        return [dumps_raises({"a": LONE_HIGH, "b": 2}) for _ in range(20)]

    assert in_fresh_cache(body) == [UnicodeEncodeError] * 20


def test_the_failure_arrives_only_on_the_second_record_of_a_shape():
    """The shape is remembered and prepared by earlier records, then a later
    record of that same shape carries a key that cannot be encoded."""

    def body():
        payload = [{"k": 1, "m": 2}, {"k": 1, "m": 2}, {LONE_HIGH: 1, "m": 2}]
        return [dumps_raises(payload) for _ in range(20)]

    assert in_fresh_cache(body) == [UnicodeEncodeError] * 20


def test_a_failing_record_among_many_good_ones_raises_on_every_repeat():
    def body():
        good = [{"k": index, "m": index} for index in range(50)]
        payload = good + [{LONE_HIGH: 1, "m": 2}] + good
        return [dumps_raises(payload) for _ in range(20)]

    assert in_fresh_cache(body) == [UnicodeEncodeError] * 20


# ---------------------------------------------------------------------------
# The cache is still usable afterwards
# ---------------------------------------------------------------------------


def test_the_depth_still_serves_valid_keys_after_a_failure():
    def body():
        record = {LONE_HIGH: 1, "b": 2}
        results = []
        for _ in range(20):
            dumps_raises(record)
            results.append(strata.dumps({"p": 1, "q": 2}))
        return results

    assert in_fresh_cache(body) == ['{"p":1,"q":2}'] * 20


def test_the_same_shape_serializes_once_its_key_is_encodable():
    """The failing shape and a valid one share a key count and a first key
    only by identity, so this also pins that the way is not left matching."""

    def body():
        results = []
        for _ in range(20):
            dumps_raises({LONE_HIGH: 1, "b": 2})
            results.append(strata.dumps({"a": 1, "b": 2}))
        return results

    assert in_fresh_cache(body) == ['{"a":1,"b":2}'] * 20


def test_a_long_run_of_failures_leaves_every_shape_correct():
    """Enough failures to retire the depth's schema cache; the plain walk that
    replaces it must produce the same bytes."""

    def body():
        record = {LONE_HIGH: 1, "b": 2}
        for _ in range(400):
            dumps_raises(record)
        return [
            strata.dumps({"p": 1, "q": 2}),
            strata.dumps({f"k{index}": index for index in range(24)}),
            strata.dumps([{"a": 1, "b": [{"c": 2}]}] * 3),
        ]

    assert in_fresh_cache(body) == [
        '{"p":1,"q":2}',
        json.dumps({f"k{index}": index for index in range(24)}, separators=(",", ":")),
        '[{"a":1,"b":[{"c":2}]},{"a":1,"b":[{"c":2}]},{"a":1,"b":[{"c":2}]}]',
    ]


def test_a_failed_call_leaves_no_bytes_in_the_next_result():
    """The failing arms return at different points in the output buffer; a
    later call must not see any of it, in either mode."""

    def body():
        clean_str = strata.dumps({"p": 1, "q": 2})
        clean_bytes = strata.dumps({"p": 1, "q": 2}, return_type="bytes")
        results = []
        for _ in range(20):
            dumps_raises({LONE_HIGH: 1, "b": 2})
            results.append(strata.dumps({"p": 1, "q": 2}))
            dumps_raises({LONE_HIGH: 1, "b": 2}, return_type="bytes")
            results.append(strata.dumps({"p": 1, "q": 2}, return_type="bytes"))
        return clean_str, clean_bytes, results

    clean_str, clean_bytes, results = in_fresh_cache(body)
    assert clean_str == '{"p":1,"q":2}'
    assert clean_bytes == b'{"p":1,"q":2}'
    assert results == [clean_str, clean_bytes] * 20


def test_dump_never_writes_a_file_for_a_failing_key(tmp_path):
    """The file path serializes to bytes first, so the poisoned schema used to
    reach disk: the third `dump` wrote `{1,2}` and `load` could not read it."""

    target = tmp_path / "record.json"

    def attempt():
        try:
            strata.dump({LONE_HIGH: 1, "b": 2}, str(target))
        except UnicodeEncodeError:
            return UnicodeEncodeError
        return "wrote"

    def body():
        return [(attempt(), target.exists()) for _ in range(20)]

    assert in_fresh_cache(body) == [(UnicodeEncodeError, False)] * 20


# ---------------------------------------------------------------------------
# References
# ---------------------------------------------------------------------------


def test_a_failed_schema_releases_the_keys_it_owns():
    """The schema cache holds *owned* references. The failure arm used to drop
    the vector without releasing them, so the key object never died."""

    alive = in_fresh_cache(_probe_key_lifetime)
    assert alive is False


def _probe_key_lifetime():
    class Key(str):
        """A subclass, so the object is never interned or shared."""

    key = Key(LONE_HIGH + "probe")
    other = Key("b" + "probe")
    record = {key: 1, other: 2}
    # Call one remembers the shape; call two reaches build_schema and fails.
    for _ in range(2):
        dumps_raises(record)
    watch = weakref.ref(key)
    del record, key, other
    gc.collect()
    return watch() is not None


def test_a_thousand_failing_calls_do_not_accumulate_references():
    def body():
        key = LONE_HIGH + "refcount"
        record = {key: 1, "b": 2}
        for _ in range(8):  # settle the cache
            dumps_raises(record)
        start = sys.getrefcount(key)
        for _ in range(1000):
            dumps_raises(record)
        return start, sys.getrefcount(key)

    start, end = in_fresh_cache(body)
    # The cache may legitimately hold one owned reference per key in each of
    # its four ways, and may release them when the depth retires. What it must
    # never do is keep one per call.
    assert abs(end - start) <= 2 * 4


def test_a_thousand_failing_calls_do_not_accumulate_references_in_bytes_mode():
    def body():
        key = LONE_HIGH + "refcount-bytes"
        record = {key: 1, "b": 2}
        for _ in range(8):
            dumps_raises(record, return_type="bytes")
        start = sys.getrefcount(key)
        for _ in range(1000):
            dumps_raises(record, return_type="bytes")
        return start, sys.getrefcount(key)

    start, end = in_fresh_cache(body)
    assert abs(end - start) <= 2 * 4


def test_the_error_is_the_same_object_shape_every_call():
    """Both failing arms — the plain walk's `write_string` and `build_schema` —
    go through the same `PyUnicode_AsUTF8AndSize`, so the message is one."""

    def body():
        messages = []
        for _ in range(20):
            try:
                strata.dumps({LONE_HIGH: 1, "b": 2})
            except UnicodeEncodeError as error:
                messages.append(str(error))
        return messages

    messages = in_fresh_cache(body)
    assert len(messages) == 20
    assert len(set(messages)) == 1
    assert "surrogates not allowed" in messages[0]


# ---------------------------------------------------------------------------
# Divergence from stdlib json, recorded in docs/decisions.md
# ---------------------------------------------------------------------------


def test_stdlib_json_accepts_what_strata_refuses():
    assert json.dumps({LONE_HIGH: 1}) == '{"\\ud800": 1}'
    with pytest.raises(UnicodeEncodeError):
        strata.dumps({LONE_HIGH: 1})


def test_the_environment_agrees_the_key_has_no_utf8():
    """Sanity: the refusal is UTF-8's, not strata's invention."""
    with pytest.raises(UnicodeEncodeError):
        LONE_HIGH.encode("utf-8")

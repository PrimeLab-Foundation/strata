"""Integration mirrors of the `strata.dumps_with_default` error table.

docs/architecture/dumps_with_default.md § Public contract and the
`dumps_with_default` bullets of docs/context/api.md, row by row, on realistic
documents -- an order book of nested records, flat rows the fused record writer
takes once a thread knows their shape, a wide dict, a tuple row, a reply chain
-- in both return types, each refusal pinned to its exact message. The unit
twins, on minimal documents, are `tests/unit/test_dumps_with_default.py`; this
file mirrors them and imports nothing from them. It also holds the import
robustness check of M12b criterion 9: `import strata` without the hook image.
"""

import dataclasses
import datetime
import decimal
import enum
import json
import pathlib
import re
import subprocess
import sys
import textwrap
import threading
import uuid
import warnings

import pytest

import strata

MODES = ("str", "bytes")

CYCLE_POLICIES = ("warn", "error", "ignore")

CONFIG_KEYS = ("duplicate_key_policy", "cycle_policy")


@pytest.fixture(autouse=True)
def restore_config():
    """Put every setting back after each test (`strata.config` is process state)."""
    saved = {key: strata.config.get(key) for key in CONFIG_KEYS}
    yield
    for key, value in saved.items():
        strata.config.set(key, value)


def exact(message):
    return f"^{re.escape(message)}$"


def text(out):
    return out.decode() if isinstance(out, bytes) else out


def stdlib(obj, default):
    """The stdlib's compact text; byte-comparable because these documents hold no float."""
    return json.dumps(obj, default=default, separators=(",", ":"), ensure_ascii=False)


def on_a_fresh_thread(body):
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


def on_both_threads(body):
    """Run `body` here (a cache earlier tests warmed) and on a fresh thread."""
    return [body(), on_a_fresh_thread(body)]


# ---------------------------------------------------------------------------
# The documents: an order book with the unsupported types a service meets
# (datetime, Enum, UUID, Decimal, a dataclass), flat rows, a wide dict.
# ---------------------------------------------------------------------------


class Status(enum.Enum):
    OPEN = "open"
    SHIPPED = "shipped"


@dataclasses.dataclass
class LineItem:
    sku: str
    qty: int
    price: decimal.Decimal


class Opaque:
    """An unsupported handle that `convert` has no rule for: the poison a row places."""

    __slots__ = ("tag",)

    def __init__(self, tag="handle"):
        self.tag = tag


class Recorder:
    """A `default` that records every object it is called with, then delegates."""

    def __init__(self, convert_fn):
        self.seen = []
        self.convert = convert_fn

    def __call__(self, obj):
        self.seen.append(obj)
        return self.convert(obj)

    @property
    def calls(self):
        return len(self.seen)


def convert(obj):
    """A realistic `default`. A line item becomes a shallow dict, so its price is
    an unsupported object inside a returned container and gets its own call."""
    if isinstance(obj, (datetime.datetime, datetime.date)):
        return obj.isoformat()
    if isinstance(obj, (uuid.UUID, decimal.Decimal)):
        return str(obj)
    if isinstance(obj, enum.Enum):
        return obj.value
    if isinstance(obj, LineItem):
        return {field.name: getattr(obj, field.name) for field in dataclasses.fields(obj)}
    raise TypeError(f"no conversion for {type(obj).__name__}")


#: `convert` calls per order: placed, status, customer ref, two line items and
#: each item's price.
CALLS_PER_ORDER = 7


def order(index):
    return {
        "id": index,
        "placed": datetime.datetime(2026, 9, 1, 8, 30) + datetime.timedelta(hours=index),
        "status": Status.SHIPPED if index % 2 else Status.OPEN,
        "customer": {"id": 100 + index, "name": f"customer {index}", "ref": uuid.UUID(int=index)},
        "lines": [
            LineItem(f"sku-{index}-{line}", line + 1, decimal.Decimal(index + line) / 4)
            for line in range(2)
        ],
        "notes": ["gift"] if index % 3 == 0 else [],
    }


def order_book(count=6):
    return {"region": "eu", "currency": "EUR", "orders": [order(index) for index in range(count)]}


def records(count=12):
    """Flat rows of one shape: past eight, a warmed thread takes the fused record writer."""
    return [
        {"id": index, "sku": f"sku-{index}", "qty": index % 4, "day": f"2026-09-{index + 1:02d}"}
        for index in range(count)
    ]


def dated_records(count=12):
    """The same rows with their unsupported values left in: one fallback per field."""
    return [
        {
            "id": index,
            "day": datetime.date(2026, 9, index + 1),
            "price": decimal.Decimal(index) / 8,
            "status": Status.OPEN,
        }
        for index in range(count)
    ]


def wide(count=30):
    """Wider than 24 keys: the writers follow it live."""
    return {f"field{index:02d}": index for index in range(count)}


PLACEMENTS = ("top-level", "order-book", "records", "wide-dict", "tuple-row")


def placements(poison):
    """`poison` at one position of each shape, after other positions were written."""
    book = order_book()
    book["orders"][3]["customer"]["ref"] = poison
    rows = records()
    rows[9]["qty"] = poison
    columns = wide()
    columns["field17"] = poison
    return {
        "top-level": poison,
        "order-book": book,
        "records": rows,
        "wide-dict": columns,
        "tuple-row": ("eu", 7, [1, 2, poison]),
    }


# ---------------------------------------------------------------------------
# Signature (api.md, dumps_with_default): "`default` is required, positional or
# keyword, and must be callable ... Missing it, passing it twice, or an unknown
# keyword raise `TypeError` as for any Python function."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_default_by_position_and_by_keyword_writes_the_same_order_book(mode):
    book = order_book()
    by_position = strata.dumps_with_default(book, convert, return_type=mode)
    by_keyword = strata.dumps_with_default(book, default=convert, return_type=mode)
    assert by_position == by_keyword
    assert isinstance(by_position, bytes if mode == "bytes" else str)
    assert text(by_position) == stdlib(book, convert)


def test_a_missing_default_is_a_type_error():
    message = exact("dumps_with_default() missing 1 required positional argument: 'default'")
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default(order_book())
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default(records(), return_type="bytes")


def test_a_default_given_twice_is_a_type_error():
    first, second = Recorder(convert), Recorder(convert)
    message = exact("dumps_with_default() got multiple values for argument 'default'")
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default(order_book(), first, default=second)
    assert first.calls == second.calls == 0


def test_return_type_is_keyword_only():
    hook = Recorder(convert)
    message = exact("dumps_with_default() takes 2 positional arguments but 3 were given")
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default(order_book(), hook, "bytes")
    assert hook.calls == 0


@pytest.mark.parametrize("keyword", ("indent", "sort_keys", "ensure_ascii"))
def test_a_json_dumps_keyword_is_a_type_error(keyword):
    # The keywords a caller carries over from `json.dumps(..., default=...)`.
    hook = Recorder(convert)
    message = exact(f"dumps_with_default() got an unexpected keyword argument '{keyword}'")
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default(order_book(), hook, **{keyword: True})
    assert hook.calls == 0


# ---------------------------------------------------------------------------
# Row 1 (api.md): "`default` ... must be callable; anything else -- `None`
# included -- raises TypeError("default must be callable, not %s") before any
# byte is produced."
# ---------------------------------------------------------------------------

NOT_CALLABLE = [
    (None, "NoneType"),
    (5, "int"),
    ("convert", "str"),
    ([convert], "list"),
    ({"default": convert}, "dict"),
    (json, "module"),
    (Opaque(), "Opaque"),
]


@pytest.mark.parametrize(("value", "name"), NOT_CALLABLE, ids=[n for _, n in NOT_CALLABLE])
@pytest.mark.parametrize("mode", MODES)
def test_a_non_callable_default_refuses_every_document(mode, value, name):
    message = exact(f"default must be callable, not {name}")
    cyclic = order_book()
    cyclic["orders"].append(cyclic)
    # "before any byte is produced": the walk's own refusals -- a bad key, a
    # cycle under "error" -- are never reached, and a fully supported document
    # is not written either.
    strata.config.set("cycle_policy", "error")
    documents = [
        records(),
        wide(),
        order_book(),
        {"daily": {datetime.date(2026, 9, 1): 5}},
        cyclic,
        *placements(Opaque()).values(),
    ]
    for document in documents:
        with pytest.raises(TypeError, match=message):
            strata.dumps_with_default(document, value, return_type=mode)
        with pytest.raises(TypeError, match=message):
            strata.dumps_with_default(document, default=value, return_type=mode)


# ---------------------------------------------------------------------------
# Row 2 (api.md): "each object of any other type is passed to `default` once and
# its return value is serialized in the object's place -- as a value, at that
# object's depth"; "Unsupported objects *nested inside* a returned container are
# ordinary positions and get their own call."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_every_unsupported_object_is_converted_once_in_its_place(mode):
    def body():
        for document in (order_book(), dated_records()):
            ours, theirs = Recorder(convert), Recorder(convert)
            out = text(strata.dumps_with_default(document, ours, return_type=mode))
            assert out == stdlib(document, theirs)
            assert len(ours.seen) == len(theirs.seen)
            assert all(a is b for a, b in zip(ours.seen, theirs.seen, strict=True))
        book = order_book()
        hook = Recorder(convert)
        strata.dumps_with_default(book, hook, return_type=mode)
        assert hook.calls == CALLS_PER_ORDER * len(book["orders"])

    on_both_threads(body)


# ---------------------------------------------------------------------------
# Row 3 (api.md): "`default` raises => that exception **propagates unchanged**:
# same object, same type and args, no wrapping or chaining (`KeyboardInterrupt`,
# `MemoryError` and `SystemExit` included)."
# ---------------------------------------------------------------------------


def _errors():
    return [ValueError("bad row", 42), KeyboardInterrupt("stop"), SystemExit(3), MemoryError("oom")]


ERROR_IDS = ["ValueError", "KeyboardInterrupt", "SystemExit", "MemoryError"]


@pytest.mark.parametrize("placement", PLACEMENTS)
@pytest.mark.parametrize("index", range(len(ERROR_IDS)), ids=ERROR_IDS)
@pytest.mark.parametrize("mode", MODES)
def test_an_exception_from_the_callable_propagates_unchanged(mode, index, placement):
    def body():
        error = _errors()[index]
        args = error.args
        poison = Opaque()
        seen = []

        def hook(obj):
            seen.append(obj)
            if obj is poison:
                raise error
            return convert(obj)

        with pytest.raises(type(error), match=exact(str(error))) as info:
            strata.dumps_with_default(placements(poison)[placement], hook, return_type=mode)
        assert info.value is error
        assert type(info.value) is type(error)
        assert info.value.args == args
        assert info.value.__cause__ is None
        assert info.value.__context__ is None
        assert info.value.__suppress_context__ is False
        # The walk stops at once: the poison is the last object handed over.
        assert seen[-1] is poison
        assert sum(obj is poison for obj in seen) == 1

    on_both_threads(body)
    # Both images' serializers are left usable on this thread.
    rows = records(3)
    expected = strata.dumps(rows, return_type=mode)
    assert strata.dumps_with_default(rows, convert, return_type=mode) == expected


# ---------------------------------------------------------------------------
# Row 4 (api.md): "**Chain bound 1.** `default` returns an unsupported object =>
# TypeError("default() returned an object of type %s that is not JSON
# serializable"), and `default` is **not** called on its own return." The
# callable here could convert a returned Decimal or datetime; it is not asked.
# ---------------------------------------------------------------------------

UNSUPPORTED_RETURNS = [
    ("itself", lambda obj: obj, "Opaque"),
    ("another-handle", lambda obj: Opaque(obj.tag), "Opaque"),
    ("set", lambda obj: {obj.tag}, "set"),
    ("bytes", lambda obj: obj.tag.encode(), "bytes"),
    ("decimal", lambda obj: decimal.Decimal("1.5"), "decimal.Decimal"),
    ("datetime", lambda obj: datetime.datetime(2026, 9, 27), "datetime.datetime"),
]


@pytest.mark.parametrize("placement", PLACEMENTS)
@pytest.mark.parametrize(
    ("make", "name"),
    [(m, n) for _, m, n in UNSUPPORTED_RETURNS],
    ids=[i for i, _, _ in UNSUPPORTED_RETURNS],
)
@pytest.mark.parametrize("mode", MODES)
def test_an_unsupported_return_is_refused_after_one_call(mode, make, name, placement):
    message = exact(f"default() returned an object of type {name} that is not JSON serializable")

    def body():
        poison = Opaque()
        seen = []

        def hook(obj):
            seen.append(obj)
            return make(obj) if obj is poison else convert(obj)

        with pytest.raises(TypeError, match=message):
            strata.dumps_with_default(placements(poison)[placement], hook, return_type=mode)
        assert seen[-1] is poison
        assert sum(obj is poison for obj in seen) == 1

    on_both_threads(body)


@pytest.mark.parametrize("mode", MODES)
def test_an_identity_default_stops_at_the_first_unsupported_object(mode):
    # stdlib `json` re-enters `default=lambda o: o` until its circular marker
    # stops it; strata refuses the first unsupported return.
    book = order_book()
    hook = Recorder(lambda obj: obj)
    message = exact(
        "default() returned an object of type datetime.datetime that is not JSON serializable"
    )
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default(book, hook, return_type=mode)
    assert hook.seen == [book["orders"][0]["placed"]]


# ---------------------------------------------------------------------------
# Row 5 (api.md): "`default` returns `None` => `null`; a returned `str` with no
# UTF-8 encoding => `UnicodeEncodeError`, as for any other `str`."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("placement", PLACEMENTS)
@pytest.mark.parametrize("mode", MODES)
def test_a_none_return_is_null_in_the_objects_place(mode, placement):
    def body():
        poison = Opaque()

        def hook(obj):
            return None if obj is poison else convert(obj)

        document = placements(poison)[placement]
        out = text(strata.dumps_with_default(document, hook, return_type=mode))
        assert out == stdlib(document, hook)

    on_both_threads(body)


LONE_SURROGATES = [("\ud800", 0), ("ab\udfff", 2), ("tag-\ud83d", 4)]


@pytest.mark.parametrize("placement", PLACEMENTS)
@pytest.mark.parametrize(("value", "position"), LONE_SURROGATES, ids=["alone", "tail", "high"])
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_lone_surrogate_is_a_unicode_encode_error(mode, value, position, placement):
    character = f"\\u{ord(value[position]):04x}"
    message = exact(
        f"'utf-8' codec can't encode character '{character}' in position {position}: "
        "surrogates not allowed"
    )
    with pytest.raises(UnicodeEncodeError, match=message):
        strata.dumps([value], return_type=mode)

    def body():
        poison = Opaque()

        def hook(obj):
            return value if obj is poison else convert(obj)

        with pytest.raises(UnicodeEncodeError, match=message) as info:
            strata.dumps_with_default(placements(poison)[placement], hook, return_type=mode)
        assert info.value.object == value

    on_both_threads(body)


# ---------------------------------------------------------------------------
# Row 6 (api.md): "**Keys are excluded.** A non-`str` dict key raises the
# unchanged TypeError("keys must be str, not %s"); `default` is never called for
# a key." `convert` would turn a date key into a string, as stdlib `json` does
# for a few key types; it is never asked.
# ---------------------------------------------------------------------------

BAD_KEYS = [
    (datetime.date(2026, 9, 1), "datetime.date"),
    (2026, "int"),
    (None, "NoneType"),
    (("eu", 1), "tuple"),
    (1.5, "float"),
    (Opaque(), "Opaque"),
]


def key_documents(key):
    """The bad key before any unsupported value, after some, in a row, in a wide dict."""
    first = {"region": "eu", "daily": {"2026-08-31": 3, key: 5}, "orders": [order(0)]}
    later = {"orders": [order(0), order(1)], "daily": {key: 5}}
    rows = records()
    rows[9] = {"id": 9, "sku": "sku-9", key: 1, "day": "2026-09-10"}
    columns = wide()
    columns[key] = 30
    return {"first": first, "later": later, "records": rows, "wide-dict": columns}


@pytest.mark.parametrize(("key", "name"), BAD_KEYS, ids=[n for _, n in BAD_KEYS])
@pytest.mark.parametrize("mode", MODES)
def test_a_non_str_key_is_refused_and_never_handed_to_the_callable(mode, key, name):
    message = exact(f"keys must be str, not {name}")

    def body():
        calls = {}
        for shape, document in key_documents(key).items():
            hook = Recorder(convert)
            with pytest.raises(TypeError, match=message):
                strata.dumps_with_default(document, hook, return_type=mode)
            assert all(obj is not key for obj in hook.seen)
            calls[shape] = hook.calls
        return calls

    # "later": both orders were converted before the key was reached.
    expected = {"first": 0, "later": 2 * CALLS_PER_ORDER, "records": 0, "wide-dict": 0}
    assert on_both_threads(body) == [expected, expected]


# ---------------------------------------------------------------------------
# Row 7 (api.md): "a returned container that is already open is a cycle under
# the active `cycle_policy`, reported where it was returned (the array element
# loop's placement caveat under Config does not apply to it)." A handle
# resolved to a container that is closed again is no cycle: it is written again.
# ---------------------------------------------------------------------------


class Ref:
    """A handle the callable resolves to the object it names."""

    __slots__ = ("target",)

    def __init__(self, target=None):
        self.target = target


def resolve(obj):
    return obj.target if isinstance(obj, Ref) else convert(obj)


LINKED_ORDERS = 10


def linked_book():
    """Orders whose `parent` handle names an open container: the order itself, the
    order list or the book; then a handle to the first order (closed by then) and
    one to the order list (open), as list elements."""
    currency = {"code": "EUR", "digits": 2}
    book = {"region": "eu", "orders": []}
    orders = book["orders"]
    for index in range(LINKED_ORDERS):
        entry = {"id": index, "currency": Ref(currency), "parent": Ref()}
        entry["parent"].target = (entry, orders, book)[index % 3]
        orders.append(entry)
    orders.append(Ref(orders[0]))
    orders.append(Ref(orders))
    return book


def linked_text():
    entries = [
        f'{{"id":{index},"currency":{{"code":"EUR","digits":2}},"parent":null}}'
        for index in range(LINKED_ORDERS)
    ]
    return '{"region":"eu","orders":[' + ",".join([*entries, entries[0], "null"]) + "]}"


#: Each order's currency and parent, then the two list handles and the first
#: order's currency and parent once more.
LINKED_CALLS = 2 * LINKED_ORDERS + 4
#: One cycle per order, one more in the first order written again, and the list.
LINKED_CYCLES = LINKED_ORDERS + 2


@pytest.mark.parametrize("policy", CYCLE_POLICIES)
@pytest.mark.parametrize("mode", MODES)
def test_a_returned_open_container_is_a_cycle_where_it_was_returned(mode, policy):
    strata.config.set("cycle_policy", policy)

    def body():
        hook = Recorder(resolve)
        book = linked_book()
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            if policy == "error":
                with pytest.raises(ValueError, match=exact("Circular reference detected")):
                    strata.dumps_with_default(book, hook, return_type=mode)
                # The first order's currency, then its parent: the order itself.
                assert hook.calls == 2
            else:
                out = strata.dumps_with_default(book, hook, return_type=mode)
                assert text(out) == linked_text()
                assert hook.calls == LINKED_CALLS
        warned = [(w.category, str(w.message)) for w in caught]
        cycles = LINKED_CYCLES if policy == "warn" else 0
        assert warned == [(RuntimeWarning, "Circular reference detected")] * cycles

    on_both_threads(body)
    # Once more here, where the thread's cache now knows the orders' shape.
    body()


# ---------------------------------------------------------------------------
# Depth (api.md): the return is serialized "at that object's depth, so a
# returned container one level past the limit raises 'Maximum serialization
# depth exceeded'". A reply chain: each comment's return opens one dict.
# ---------------------------------------------------------------------------


class Comment:
    __slots__ = ("body", "reply")

    def __init__(self, body, reply=None):
        self.body = body
        self.reply = reply


def as_dict(obj):
    return {"body": obj.body, "reply": obj.reply}


def comment_thread(length):
    node = None
    for index in reversed(range(length)):
        node = Comment(f"comment {index}", node)
    return {"thread": node}


def plain_thread(length):
    node = None
    for index in reversed(range(length)):
        node = {"body": f"comment {index}", "reply": node}
    return {"thread": node}


def _python_stack_depth():
    depth = 0
    frame = sys._getframe()
    while frame is not None:
        depth += 1
        frame = frame.f_back
    return depth


@pytest.mark.parametrize("mode", MODES)
def test_a_reply_chain_meets_the_depth_limit_where_dumps_does(mode):
    saved = sys.getrecursionlimit()
    limit = max(300, _python_stack_depth() + 120)
    try:
        sys.setrecursionlimit(limit)
        # The document's own dict plus one returned dict per comment.
        fits = limit - 1
        hooked = strata.dumps_with_default(comment_thread(fits), as_dict, return_type=mode)
        assert hooked == strata.dumps(plain_thread(fits), return_type=mode)
        message = exact("Maximum serialization depth exceeded")
        with pytest.raises(ValueError, match=message):
            strata.dumps(plain_thread(fits + 1), return_type=mode)
        hook = Recorder(as_dict)
        with pytest.raises(ValueError, match=message):
            strata.dumps_with_default(comment_thread(fits + 1), hook, return_type=mode)
        # The last comment is consulted; its returned dict is the one refused.
        assert hook.calls == fits + 1
    finally:
        sys.setrecursionlimit(saved)


# ---------------------------------------------------------------------------
# Row 8 (api.md): "A document with no unsupported object is byte-identical to
# `dumps(obj)` in both return types, and `default` is never called."
# ---------------------------------------------------------------------------


def supported_documents():
    return {
        "order-book": json.loads(stdlib(order_book(), convert)),
        "records": records(30),
        "wide-dict": wide(),
        "mixed": {
            "floats": [0.1, -0.0, 1e300, 2.5e-8],
            "big": [2**63, -(10**30)],
            "text": ["é 你 \U0001f600", "", '"quoted"\n'],
            "flags": [True, False, None],
            "tuple": (1, ("nested", [])),
            "empty": {},
        },
    }


@pytest.mark.parametrize("mode", MODES)
def test_a_document_with_nothing_unsupported_is_byte_identical_to_dumps(mode):
    def body():
        for document in supported_documents().values():
            hook = Recorder(convert)
            out = strata.dumps_with_default(document, hook, return_type=mode)
            assert out == strata.dumps(document, return_type=mode)
            assert hook.calls == 0

    on_both_threads(body)


# ---------------------------------------------------------------------------
# Row 9 (api.md): "`return_type` is `dumps`'s: "str" or "bytes", else
# ValueError("invalid return_type: %s")."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("value", ("text", "STR", "", "Bytes", "json"))
def test_an_invalid_return_type_is_a_value_error_as_in_dumps(value):
    message = exact(f"invalid return_type: {value}")
    with pytest.raises(ValueError, match=message):
        strata.dumps(records(), return_type=value)
    hook = Recorder(convert)
    with pytest.raises(ValueError, match=message):
        strata.dumps_with_default(order_book(), hook, return_type=value)
    assert hook.calls == 0


@pytest.mark.parametrize(("value", "name"), [(5, "int"), (None, "NoneType"), (b"str", "bytes")])
def test_a_non_str_return_type_is_a_type_error_as_in_dumps(value, name):
    message = exact(f"return_type must be str, not {name}")
    with pytest.raises(TypeError, match=message):
        strata.dumps(records(), return_type=value)
    hook = Recorder(convert)
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default(order_book(), hook, return_type=value)
    assert hook.calls == 0


# ---------------------------------------------------------------------------
# Import robustness (M12b criterion 9; `strata/serialize.py` `_hook_entry`):
# without the hook image `import strata` succeeds and every other function
# works; `dumps_with_default` raises the import error on each call, since a
# failed import is not cached.
# ---------------------------------------------------------------------------

IMPORT_WITHOUT_THE_HOOK = textwrap.dedent(
    """
    import sys

    sys.path.insert(0, sys.argv[1])  # the package under test, wherever it lives
    sys.modules["strata._dumps_hook"] = None  # the hook image cannot be imported
    import strata

    assert strata.dumps({"a": [1, 2]}) == '{"a":[1,2]}'
    assert strata.loads('{"a": [1, 2]}') == {"a": [1, 2]}
    for attempt in (1, 2):
        try:
            strata.dumps_with_default([object()], repr)
        except ModuleNotFoundError as error:
            print(attempt, type(error).__name__, error.name)
        else:
            raise SystemExit(f"call {attempt} did not raise")
    del sys.modules["strata._dumps_hook"]
    print("recovered", strata.dumps_with_default([object()], lambda obj: "x"))
    """,
)


def test_import_strata_survives_a_missing_hook_image():
    # The package root goes in by argument, not PYTHONPATH: the build gate runs
    # the suites against a fresh build that no environment variable names.
    package_root = pathlib.Path(strata.__file__).resolve().parent.parent
    result = subprocess.run(
        [sys.executable, "-c", IMPORT_WITHOUT_THE_HOOK, str(package_root)],
        capture_output=True,
        text=True,
        timeout=120,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    assert result.stdout.splitlines() == [
        "1 ModuleNotFoundError strata._dumps_hook",
        "2 ModuleNotFoundError strata._dumps_hook",
        'recovered ["x"]',
    ]

"""Integration tests for `parse_types` (docs/architecture/native_types.md, "Parse contract").

Corpora and modes the contract suite (tests/unit/native_types/test_parse_contract.py)
pins clause by clause: the round trip `loads(dumps(x), parse_types=True) == x`
over a generated corpus; recognition against a Python oracle of the record's
grammar over valid, mutated and near-miss strings (every rejection stays a
`str`); NDJSON, folder, iterator and `skip_errors` modes; the registry's
Enum/dataclass/list/nesting/precedence rules; the law
`search(f, e, parse_types=p) == query(load(f, parse_types=p), e)` over a
generated expression set with filters; and user code run by registered types.
"""

import dataclasses
import datetime as dt
import enum
import gc
import json
import random
import re
import uuid

import pytest

import strata

UTC = dt.timezone.utc
SEED = 20260928


class Color(enum.Enum):
    RED = "red"
    GREEN = "green"


class Size(enum.IntEnum):
    S = 1
    M = 2


class BoomError(Exception):
    pass


@dataclasses.dataclass
class Item:
    name: str
    size: Size = Size.S
    color: Color = Color.RED
    tags: list = dataclasses.field(default_factory=list)


@dataclasses.dataclass
class Order:
    id: int
    items: list
    at: dt.datetime = None


@dataclasses.dataclass
class Refuses:
    name: str

    def __post_init__(self):
        raise BoomError(self.name)


REGISTRY = {"item": Item, "items": Item, "order": Order, "size": Size, "color": Color}


# ---------------------------------------------------------------------------
# Round trip
# ---------------------------------------------------------------------------


def _random_offset(rng):
    choice = rng.random()
    if choice < 0.2:
        return None
    if choice < 0.35:
        return UTC
    return dt.timezone(dt.timedelta(minutes=rng.randint(-1439, 1439)))


def _random_leaf(rng):
    kind = rng.randrange(6)
    micro = rng.choice([0, rng.randrange(1, 1_000_000)])
    if kind == 0:
        return dt.date(rng.randint(1, 9999), rng.randint(1, 12), rng.randint(1, 28))
    if kind == 1:
        return dt.time(rng.randrange(24), rng.randrange(60), rng.randrange(60), micro)
    if kind == 2:
        return dt.time(
            rng.randrange(24),
            rng.randrange(60),
            rng.randrange(60),
            micro,
            tzinfo=_random_offset(rng),
        )
    if kind == 3:
        return dt.datetime(
            rng.randint(1, 9999),
            rng.randint(1, 12),
            rng.randint(1, 28),
            rng.randrange(24),
            rng.randrange(60),
            rng.randrange(60),
            micro,
            tzinfo=_random_offset(rng),
        )
    if kind == 4:
        return uuid.UUID(int=rng.getrandbits(128))
    return rng.choice(["plain", "", 17, 2.5, None, True, "2024-13-01"])


def _random_tree(rng, depth=0):
    if depth > 3 or rng.random() < 0.4:
        return _random_leaf(rng)
    if rng.random() < 0.5:
        return [_random_tree(rng, depth + 1) for _ in range(rng.randrange(5))]
    return {f"k{index}": _random_tree(rng, depth + 1) for index in range(rng.randrange(5))}


def _same(left, right):
    """Equal, and equal in type and offset all the way down."""
    assert type(left) is type(right), (left, right)
    if isinstance(left, dict):
        assert list(left) == list(right)
        for key in left:
            _same(left[key], right[key])
    elif isinstance(left, list):
        assert len(left) == len(right)
        for a, b in zip(left, right, strict=True):
            _same(a, b)
    else:
        assert left == right
        if isinstance(left, (dt.datetime, dt.time)):
            assert left.utcoffset() == right.utcoffset()


def test_round_trip_over_a_generated_corpus():
    rng = random.Random(SEED)
    for _ in range(400):
        value = _random_tree(rng)
        _same(strata.loads(strata.dumps(value), parse_types=True), value)
        _same(strata.loads(strata.dumps(value).encode(), parse_types=True), value)


def test_round_trip_through_a_file(tmp_path):
    rng = random.Random(SEED + 1)
    records = [_random_tree(rng) for _ in range(50)]
    strata.dump(records, tmp_path / "a.json")
    _same(strata.load(tmp_path / "a.json", parse_types=True), records)


# ---------------------------------------------------------------------------
# Recognition against an oracle of the grammar
# ---------------------------------------------------------------------------

_D = r"([0-9]{4})-([0-9]{2})-([0-9]{2})"
_T = r"([0-9]{2}):([0-9]{2}):([0-9]{2})(?:\.([0-9]{1,6}))?(?:([Zz])|([+-])([0-9]{2}):([0-9]{2}))?"
_DATE = re.compile(_D)
_TIME = re.compile(_T)
_DATETIME = re.compile(_D + "[Tt]" + _T)
_UUID = re.compile(r"[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}")


def _zone(zulu, sign, hours, minutes):
    if zulu:
        return UTC
    if sign is None:
        return None
    hours, minutes = int(hours), int(minutes)
    if hours >= 24 or minutes >= 60:  # noqa: PLR2004 -- the grammar's bounds
        raise ValueError("offset out of range")
    total = hours * 60 + minutes
    if total == 0:
        return UTC
    return dt.timezone(dt.timedelta(minutes=-total if sign == "-" else total))


def _time_parts(groups):
    hour, minute, second, fraction, zulu, sign, off_h, off_m = groups
    micro = int((fraction or "0").ljust(6, "0"))
    return (int(hour), int(minute), int(second), micro), _zone(zulu, sign, off_h, off_m)


def oracle(text):
    """What `parse_types=True` must make of @text, by the record's grammar."""
    try:
        if match := _DATE.fullmatch(text):
            return dt.date(*map(int, match.groups()))
        if match := _TIME.fullmatch(text):
            clock, zone = _time_parts(match.groups())
            return dt.time(*clock, tzinfo=zone)
        if match := _DATETIME.fullmatch(text):
            clock, zone = _time_parts(match.groups()[3:])
            return dt.datetime(*map(int, match.groups()[:3]), *clock, tzinfo=zone)
        if _UUID.fullmatch(text):
            return uuid.UUID(text)
    except ValueError:
        return text
    return text


VALID = [
    "2024-02-29",
    "0001-01-01",
    "9999-12-31",
    "00:00:00",
    "23:59:59.999999",
    "12:34:56.7",
    "12:34:56Z",
    "12:34:56-00:00",
    "12:34:56+23:59",
    "2024-01-01T00:00:00",
    "2024-01-01t00:00:00.123z",
    "2024-01-01T00:00:00.000001+05:45",
    "2024-01-01T00:00:00-12:00",
    "0f1e2d3c-4b5a-6978-8796-a5b4c3d2e1f0",
    "0F1E2D3C-4B5A-6978-8796-A5B4C3D2E1F0",
]

NEAR_MISSES = [
    "2024-1-01",
    "2024-01-1",
    "24-01-01",
    "2024/01/01",
    "2024-01-01 00:00:00",
    "2024-01-01T00:00",
    "2024-01-01T00:00:00.",
    "2024-01-01T00:00:00.1234567",
    "2024-01-01T00:00:00+0530",
    "2024-01-01T00:00:00+05",
    "2024-01-01T00:00:00+05:30:00",
    "2024-01-01T00:00:00+24:00",
    "2024-01-01T00:00:00+05:60",
    "2024-01-01T00:00:00UTC",
    "2024-01-01T24:00:00",
    "2024-01-01T23:60:00",
    "2024-01-01T23:59:60",
    "2024-00-01",
    "2024-13-01",
    "2023-02-29",
    "2100-02-29",
    "0000-12-31",
    " 2024-01-01",
    "2024-01-01 ",
    "2024-01-01\n",
    "٢024-01-01",
    "２024-01-01",
    "12:00",
    "12:00:00.1234567",
    "1:00:00",
    "12:00:00 Z",
    "0f1e2d3c4b5a69788796a5b4c3d2e1f0",
    "{0f1e2d3c-4b5a-6978-8796-a5b4c3d2e1f0}",
    "urn:uuid:0f1e2d3c-4b5a-6978-8796-a5b4c3d2e1f0",
    "0f1e2d3c-4b5a-6978-8796-a5b4c3d2e1f",
    "0f1e2d3c-4b5a-6978-8796-a5b4c3d2e1f0a",
    "0f1e2d3g-4b5a-6978-8796-a5b4c3d2e1f0",
    "0f1e2d3c-4b5a6978-8796-a5b4c3d2e1f0-",
    "",
    "T",
    "Z",
]


def _check(text):
    got = strata.loads(json.dumps(text), parse_types=True)
    expected = oracle(text)
    assert type(got) is type(expected), (text, got, expected)
    assert got == expected, text
    if isinstance(expected, (dt.datetime, dt.time)):
        assert got.utcoffset() == expected.utcoffset(), text
        if expected.tzinfo is UTC:
            assert got.tzinfo is UTC, text


@pytest.mark.parametrize("text", VALID)
def test_valid_strings_are_recognized(text):
    assert type(oracle(text)) is not str
    _check(text)


@pytest.mark.parametrize("text", NEAR_MISSES)
def test_every_near_miss_stays_str(text):
    assert oracle(text) == text
    got = strata.loads(json.dumps(text), parse_types=True)
    assert type(got) is str
    assert got == text


def test_mutated_strings_agree_with_the_oracle():
    rng = random.Random(SEED + 2)
    alphabet = "0123456789-:.TtZz+ abcdefABCDEFé"
    for _ in range(4000):
        text = list(rng.choice(VALID))
        for _ in range(rng.randint(1, 3)):
            operation = rng.randrange(3)
            position = rng.randrange(len(text) + 1)
            if operation == 0 and text:
                del text[min(position, len(text) - 1)]
            elif operation == 1:
                text.insert(position, rng.choice(alphabet))
            elif text:
                text[min(position, len(text) - 1)] = rng.choice(alphabet)
        _check("".join(text))


# ---------------------------------------------------------------------------
# NDJSON, folders, iterators, skip_errors
# ---------------------------------------------------------------------------


def _write(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")
    return path


LINES = [
    {"order": {"id": 1, "items": [{"name": "a", "size": 2}], "at": "2024-01-01T10:00:00Z"}},
    "2024-06-01",
    [{"item": {"name": "b", "color": "green", "tags": ["12:00:00"]}}],
    {"when": "2024-01-01", "color": "blue"},
]


def _expected_lines():
    return [
        {
            "order": Order(
                id=1,
                items=[Item("a", Size.M)],
                at=dt.datetime(2024, 1, 1, 10, tzinfo=UTC),
            ),
        },
        dt.date(2024, 6, 1),
        [{"item": Item("b", color=Color.GREEN, tags=[dt.time(12)])}],
        {"when": dt.date(2024, 1, 1), "color": "blue"},
    ]


@pytest.fixture
def ndjson(tmp_path):
    return _write(tmp_path / "records.ndjson", "".join(json.dumps(line) + "\n" for line in LINES))


def test_ndjson_eager_and_lazy(ndjson):
    expected = _expected_lines()
    assert strata.load(ndjson, parse_types=REGISTRY) == expected
    lazy = strata.load(ndjson, iterator=True, parse_types=REGISTRY)
    assert not isinstance(lazy, list)
    assert list(lazy) == expected


def test_ndjson_lazy_iterator_revives_line_by_line(tmp_path):
    # A registered type's exception surfaces at the line that raises it.
    path = _write(tmp_path / "a.ndjson", '"2024-01-01"\n{"r": {"name": "x"}}\n"12:00:00"\n')
    lazy = strata.load(path, iterator=True, parse_types={"r": Refuses})
    assert next(lazy) == dt.date(2024, 1, 1)
    with pytest.raises(BoomError, match="x"):
        next(lazy)


def test_skip_errors_drops_invalid_lines_only(tmp_path):
    path = _write(tmp_path / "a.ndjson", '"2024-01-01"\n{bad\n"12:00:00"\n')
    expected = [dt.date(2024, 1, 1), dt.time(12)]
    assert strata.load(path, skip_errors=True, parse_types=True) == expected
    assert list(strata.load(path, skip_errors=True, iterator=True, parse_types=True)) == expected
    with pytest.raises(ValueError, match="Invalid JSON on line 2"):
        strata.load(path, parse_types=True)
    refused = _write(tmp_path / "b.ndjson", '{bad\n{"r": {"name": "y"}}\n')
    with pytest.raises(BoomError):
        strata.load(refused, skip_errors=True, parse_types={"r": Refuses})
    with pytest.raises(BoomError):
        list(strata.load(refused, skip_errors=True, iterator=True, parse_types={"r": Refuses}))


def test_json_file_iterator_revives_before_iterating(tmp_path):
    path = _write(tmp_path / "a.json", '{"a": "2024-01-01", "size": 1}')
    pairs = list(strata.load(path, iterator=True, parse_types={"size": Size}))
    assert pairs == [("a", dt.date(2024, 1, 1)), ("size", Size.S)]


def _folder(tmp_path):
    root = tmp_path / "folder"
    _write(root / "a.json", json.dumps(LINES))
    _write(root / "b" / "c.ndjson", "".join(json.dumps(line) + "\n" for line in LINES))
    _write(root / "d.json", json.dumps("2030-01-01"))
    return root


def test_folder_eager_and_lazy(tmp_path):
    root = _folder(tmp_path)
    expected = [*_expected_lines(), *_expected_lines(), dt.date(2030, 1, 1)]
    assert strata.load(root, parse_types=REGISTRY) == expected
    assert list(strata.load(root, iterator=True, parse_types=REGISTRY)) == expected


def test_folder_skip_errors_and_registered_exceptions(tmp_path):
    root = tmp_path / "folder"
    _write(root / "a.json", "{bad")
    _write(root / "b.json", '"2024-01-01"')
    assert strata.load(root, skip_errors=True, parse_types=True) == [dt.date(2024, 1, 1)]
    _write(root / "c.json", '{"r": {"name": "z"}}')
    with pytest.raises(BoomError, match="z"):
        strata.load(root, skip_errors=True, parse_types={"r": Refuses})
    lazy = strata.load(root, skip_errors=True, iterator=True, parse_types={"r": Refuses})
    assert next(lazy) == dt.date(2024, 1, 1)
    with pytest.raises(BoomError, match="z"):
        next(lazy)


# ---------------------------------------------------------------------------
# Registry rules
# ---------------------------------------------------------------------------


def test_registry_enum_and_dataclass_nesting():
    text = json.dumps(
        {
            "order": {
                "id": 7,
                "items": [{"name": "a", "size": 2, "tags": ["2024-01-01"]}, {"name": "b"}],
                "at": "2024-01-01T00:00:00+01:00",
            },
            "deep": [[{"size": 1, "color": "green"}]],
        },
    )
    result = strata.loads(text, parse_types=REGISTRY)
    assert result == {
        "order": Order(
            7,
            [Item("a", Size.M, tags=[dt.date(2024, 1, 1)]), Item("b")],
            dt.datetime(2024, 1, 1, tzinfo=dt.timezone(dt.timedelta(hours=1))),
        ),
        "deep": [[{"size": Size.S, "color": Color.GREEN}]],
    }


def test_registry_list_is_one_level():
    # A nested list is not revived by the registry, a value no member has is
    # left as parsed, and neither is ever type-recognized.
    result = strata.loads('{"size": [1, [2], 3, "2024-01-01"]}', parse_types=REGISTRY)
    assert result == {"size": [Size.S, [2], 3, "2024-01-01"]}
    assert type(result["size"][3]) is str


def test_registry_takes_precedence_over_recognition():
    result = strata.loads(
        '{"color": "2024-01-01", "item": "2024-01-01", "other": "2024-01-01"}',
        parse_types=REGISTRY,
    )
    assert result == {"color": "2024-01-01", "item": "2024-01-01", "other": dt.date(2024, 1, 1)}


def test_registry_is_a_snapshot():
    registry = {"a": Size}

    class Mutating(enum.Enum):
        X = "x"

        @classmethod
        def _missing_(cls, value):
            registry.clear()
            registry["b"] = Color

    registry["m"] = Mutating
    result = strata.loads('{"m": "y", "a": 1, "b": "red"}', parse_types=registry)
    assert result == {"m": "y", "a": Size.S, "b": "red"}
    assert registry == {"b": Color}


def test_dataclass_with_a_required_init_false_field_is_left_as_parsed():
    @dataclasses.dataclass
    class Counted:
        name: str
        count: int = dataclasses.field(init=False)

        def __post_init__(self):
            self.count = len(self.name)

    result = strata.loads(
        '{"c": {"name": "abc"}, "d": {"name": "x", "count": 1}}',
        parse_types={
            "c": Counted,
            "d": Counted,
        },
    )
    assert result["c"].count == 3  # noqa: PLR2004
    assert result["d"] == {"name": "x", "count": 1}


# ---------------------------------------------------------------------------
# search == query(load) over a generated expression set
# ---------------------------------------------------------------------------

EXPRESSIONS = [
    "$",
    "$.*",
    "$[*]",
    "$[0]",
    "$[-1]",
    "$[0:2]",
    "$[1:4:2]",
    "$..when",
    "$..color",
    "$..size",
    "$..order",
    "$..items",
    "$..tags",
    "$[*].order.items[0]",
    "$[*].order.at",
    "$[*]['when']",
    "$[?(@.color == 'blue')]",
    "$[?(@.color != 'blue')]",
    "$[*].order.items[?(@.size > 1)]",
    "$[*].order.items[?(@.size >= 1)]",
    "$[*].order.items[?(@.name == 'a')]",
    "$[?(@.id < 3)]",
    "$.when",
    "$.order.items[*].name",
]


def _search_equals_query_of_load(path, parse_types):
    for expression in EXPRESSIONS:
        compiled = strata.compile(expression)
        expected = strata.query(strata.load(path, parse_types=parse_types), expression)
        got = strata.search(path, expression, parse_types=parse_types)
        assert got == expected, (path.name, expression)
        assert strata.search(path, compiled, parse_types=parse_types) == got
        assert list(strata.search(path, expression, iterator=True, parse_types=parse_types)) == got


@pytest.mark.parametrize("parse_types", [True, REGISTRY], ids=["true", "registry"])
def test_search_law_on_json_and_ndjson(tmp_path, ndjson, parse_types):
    document = _write(tmp_path / "doc.json", json.dumps(LINES))
    single = _write(tmp_path / "one.json", json.dumps(LINES[0]))
    for path in (document, single, ndjson):
        _search_equals_query_of_load(path, parse_types)


@pytest.mark.parametrize("parse_types", [True, REGISTRY], ids=["true", "registry"])
def test_search_law_on_a_folder(tmp_path, parse_types):
    root = _folder(tmp_path)
    files = [root / "a.json", root / "b" / "c.ndjson", root / "d.json"]
    for expression in EXPRESSIONS:
        expected = []
        for path in files:
            expected.extend(strata.search(path, expression, parse_types=parse_types))
        assert strata.search(root, expression, parse_types=parse_types) == expected
        lazy = strata.search(root, expression, iterator=True, parse_types=parse_types)
        assert list(lazy) == expected


def test_search_scalar_root_matches_only_the_root(tmp_path):
    path = _write(tmp_path / "a.json", '"2024-01-01"')
    assert strata.search(path, "$", parse_types=True) == [dt.date(2024, 1, 1)]
    assert strata.search(path, "$") == ["2024-01-01"]
    for expression in ("$.a", "$[0]", "$[*]", "$..a", "$[?(@.a == 1)]", "$[0:1]"):
        assert strata.search(path, expression, parse_types=True) == strata.search(path, expression)


def test_search_without_parse_types_is_unchanged(tmp_path, ndjson):
    document = _write(tmp_path / "doc.json", json.dumps(LINES))
    for path in (document, ndjson):
        for expression in EXPRESSIONS:
            assert strata.search(path, expression, parse_types=False) == strata.search(
                path,
                expression,
            )


def test_search_errors_keep_their_order(tmp_path):
    path = _write(tmp_path / "a.json", "{}")
    with pytest.raises(TypeError, match="parse_types must be a bool or a dict"):
        strata.search(path, "not valid", parse_types=None)
    with pytest.raises(ValueError, match="Invalid JSONPath expression"):
        strata.search(path, "not valid", parse_types=True)
    with pytest.raises(FileNotFoundError):
        strata.search(tmp_path / "missing.json", "$", parse_types=True)


# ---------------------------------------------------------------------------
# query, cursor, errors
# ---------------------------------------------------------------------------


def test_query_recognizes_str_matches_without_touching_data():
    data = {"a": ["2024-01-01", {"b": "12:00:00"}], "c": "x"}
    snapshot = json.dumps(data)
    matches = strata.query(data, "$.a[*]", parse_types=True)
    assert matches == [dt.date(2024, 1, 1), {"b": "12:00:00"}]
    assert matches[1] is data["a"][1]
    assert json.dumps(data) == snapshot
    assert list(strata.query(data, "$.a[0]", iterator=True, parse_types=True)) == [
        dt.date(2024, 1, 1),
    ]


def test_query_recognizes_a_str_subclass_match():
    class Text(str):
        pass

    assert strata.query([Text("2024-01-01")], "$[0]", parse_types=True) == [dt.date(2024, 1, 1)]


def test_query_dict_error():
    with pytest.raises(TypeError, match=r"^query\(\) parse_types must be a bool, not dict$"):
        strata.query({"a": 1}, "$.a", parse_types=REGISTRY)


def test_cursor_error_on_every_parse_entry_point(tmp_path, ndjson):
    document = _write(tmp_path / "a.json", "{}")
    for call in (
        lambda: strata.loads("{}", return_type="cursor", parse_types=True),
        lambda: strata.load(document, return_type="cursor", parse_types=True),
        lambda: strata.load(ndjson, return_type="cursor", parse_types=REGISTRY),
        lambda: strata.load(tmp_path, return_type="cursor", parse_types=True),
    ):
        with pytest.raises(ValueError, match=r"^parse_types needs return_type='dict'$"):
            call()


# ---------------------------------------------------------------------------
# User code run by registered types
# ---------------------------------------------------------------------------


def test_post_init_exception_propagates_from_every_entry_point(tmp_path):
    registry = {"r": Refuses}
    text = '[{"r": {"name": "boom"}}]'
    document = _write(tmp_path / "f" / "a.json", text)
    lines = _write(tmp_path / "f" / "b.ndjson", text + "\n")
    calls = [
        lambda: strata.loads(text, parse_types=registry),
        lambda: strata.load(document, parse_types=registry),
        lambda: list(strata.load(lines, iterator=True, parse_types=registry)),
        lambda: list(strata.load(tmp_path / "f", iterator=True, parse_types=registry)),
        lambda: strata.search(document, "$", parse_types=registry),
        lambda: strata.search(tmp_path / "f", "$", parse_types=registry),
        lambda: list(strata.search(tmp_path / "f", "$", iterator=True, parse_types=registry)),
    ]
    for call in calls:
        with pytest.raises(BoomError, match="boom"):
            call()


def _containers_with(marker):
    """Every tracked dict keyed by @p marker and list starting with it."""
    found = []
    for obj in gc.get_objects():
        if type(obj) is dict and marker in obj:
            found.append(obj)
        elif type(obj) is list and obj and type(obj[0]) is str and obj[0] == marker:
            found.append(obj)
    return found


def test_user_code_that_clears_the_containers_being_walked():
    # The walk holds strong references across user code and writes back only
    # where the slot still holds what it read (python_parse_types.cpp's header).
    marker = "marker-" + uuid.uuid4().hex

    @dataclasses.dataclass
    class Clearing:
        name: str

        def __post_init__(self):
            for container in _containers_with(marker):
                container.clear()

    text = json.dumps(
        [
            marker,
            {marker: 1, "c": {"name": "a"}, "d": "2024-01-01", "e": [{"c": {"name": "b"}}]},
            {"c": {"name": "c"}},
            "2024-01-01",
        ],
    )
    result = strata.loads(text, parse_types={"c": Clearing})
    assert result == []


def test_user_code_that_mutates_a_revived_value():
    @dataclasses.dataclass
    class Draining:
        values: list

        def __post_init__(self):
            self.values.clear()
            gc.collect()

    result = strata.loads(
        '{"x": {"values": ["2024-01-01", 1]}, "y": "12:00:00"}',
        parse_types={
            "x": Draining,
        },
    )
    assert result == {"x": Draining([]), "y": dt.time(12)}


def test_user_code_that_reenters_strata():
    @dataclasses.dataclass
    class Nested:
        text: str

        def __post_init__(self):
            self.text = strata.loads(self.text, parse_types={"inner": Color})

    result = strata.loads(
        json.dumps({"n": {"text": json.dumps({"inner": "red", "at": "2024-01-01"})}}),
        parse_types={"n": Nested},
    )
    assert type(result["n"]) is Nested
    assert result["n"].text == {"inner": Color.RED, "at": dt.date(2024, 1, 1)}

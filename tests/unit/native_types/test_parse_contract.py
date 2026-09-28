"""Contract tests for `parse_types` on `loads`, `load`, `search` and `query`.

One named test per clause of docs/architecture/native_types.md -- "Parse
contract (`parse_types`)", its registry bullets and per-entry-point bullets,
and the parse rows of "Error contract" -- each citing the clause it pins, plus
the api.md `parse_types` section that carries them. Corpora, the search law
over a generated expression set, NDJSON/folder/iterator modes and re-entrancy
are in tests/py/native_types/test_parse_types.py.
"""

import dataclasses
import datetime as dt
import enum
import json
import pathlib
import subprocess
import sys
import textwrap
import uuid

import pytest

import strata

UTC = dt.timezone.utc


class Color(enum.Enum):
    RED = "red"
    BLUE = "blue"


class Level(enum.IntEnum):
    LOW = 1
    HIGH = 2


class BoomError(Exception):
    pass


class Strict(enum.Enum):
    ONE = "one"

    @classmethod
    def _missing_(cls, value):
        raise BoomError(value)


@dataclasses.dataclass
class Point:
    x: int
    y: int = 0
    tags: list = dataclasses.field(default_factory=list)
    note: str = dataclasses.field(default="", init=False)


@dataclasses.dataclass
class Event:
    color: Color
    at: dt.date
    point: Point = None


@dataclasses.dataclass
class Refuses:
    x: int

    def __post_init__(self):
        raise BoomError("post_init")


def revived(value, parse_types=True):
    """`loads(json.dumps(value), parse_types=...)`."""
    return strata.loads(json.dumps(value), parse_types=parse_types)


def _write(path, text):
    path.write_text(text, encoding="utf-8")
    return path


# ---------------------------------------------------------------------------
# Parse contract: the default, and what is recognized
# ---------------------------------------------------------------------------


def test_default_false_is_todays_behaviour():
    # "`parse_types=False` (default) is today's behaviour, bit for bit."
    text = json.dumps({"d": "2024-01-01", "u": str(uuid.UUID(int=7)), "l": ["12:00:00"]})
    default = strata.loads(text)
    assert strata.loads(text, parse_types=False) == default
    assert default == json.loads(text)
    assert all(isinstance(v, str) for v in (default["d"], default["u"], default["l"][0]))


def test_only_string_values_are_replaced_never_keys():
    # "every JSON **string value** (never a key)"
    result = revived({"2024-01-01": "2024-01-01"})
    assert list(result) == ["2024-01-01"]
    assert isinstance(next(iter(result)), str)
    assert result["2024-01-01"] == dt.date(2024, 1, 1)


def test_date_time_becomes_a_naive_datetime_without_an_offset():
    # Grammar row "date-time": naive without an offset; `T` or `t`.
    assert revived("2024-03-05T06:07:08") == dt.datetime(2024, 3, 5, 6, 7, 8)
    assert revived("2024-03-05t06:07:08") == dt.datetime(2024, 3, 5, 6, 7, 8)
    assert revived("2024-03-05T06:07:08").tzinfo is None


def test_date_time_fraction_of_one_to_six_digits():
    # Grammar row "date-time": [`.` 1-6 digits].
    assert revived("2024-03-05T06:07:08.5").microsecond == 500000
    assert revived("2024-03-05T06:07:08.000001").microsecond == 1
    assert revived("2024-03-05T06:07:08.123456").microsecond == 123456


@pytest.mark.parametrize("offset", ["Z", "z", "+00:00", "-00:00"])
def test_zulu_and_zero_offsets_become_timezone_utc(offset):
    # "`Z`, `+00:00` and `-00:00` -> `timezone.utc`" (and `z`, "Ambiguities").
    value = revived(f"2024-03-05T06:07:08{offset}")
    assert value.tzinfo is UTC


@pytest.mark.parametrize(("text", "minutes"), [("+05:30", 330), ("-03:15", -195), ("+23:59", 1439)])
def test_other_offsets_become_a_fixed_timezone(text, minutes):
    # "others a fixed `timezone`"
    value = revived(f"2024-03-05T06:07:08{text}")
    assert type(value.tzinfo) is dt.timezone
    assert value.utcoffset() == dt.timedelta(minutes=minutes)
    assert value == dt.datetime(
        2024, 3, 5, 6, 7, 8, tzinfo=dt.timezone(dt.timedelta(minutes=minutes))
    )


def test_equal_offsets_share_one_timezone_object():
    # Implementation note (brief): fixed-offset timezones are cached.
    first, second = revived(["2024-01-01T00:00:00+01:00", "12:00:00+01:00"])
    assert first.tzinfo is second.tzinfo


def test_date_becomes_a_date():
    # Grammar row "date".
    value = revived("0001-01-01")
    assert type(value) is dt.date
    assert value == dt.date(1, 1, 1)
    assert revived("9999-12-31") == dt.date(9999, 12, 31)
    assert revived("2024-02-29") == dt.date(2024, 2, 29)


def test_time_becomes_a_time_aware_when_an_offset_is_present():
    # Grammar row "time".
    naive = revived("23:59:59.999999")
    assert type(naive) is dt.time
    assert naive == dt.time(23, 59, 59, 999999)
    assert naive.tzinfo is None
    aware = revived("10:00:00+02:00")
    assert aware.tzinfo == dt.timezone(dt.timedelta(hours=2))
    assert revived("10:00:00Z").tzinfo is UTC


@pytest.mark.parametrize(
    "text",
    ["12345678-9abc-def0-1234-56789abcdef0", "12345678-9ABC-DEF0-1234-56789ABCDEF0"],
)
def test_uuid_in_either_case_becomes_a_uuid(text):
    # Grammar row "UUID": hyphenated, either case.
    value = revived(text)
    assert type(value) is uuid.UUID
    assert value == uuid.UUID(text)


@pytest.mark.parametrize("text", ["2024-02-30", "2023-02-29", "23:59:60", "0000-01-01", "24:00:00"])
def test_impossible_values_stay_str(text):
    # "a string that matches the grammar but names an impossible value ... stays a `str`"
    assert revived(text) == text
    assert type(revived(text)) is str


@pytest.mark.parametrize(
    "text",
    [
        "2024-03-05T06:07:08.1234567",  # 7 fraction digits: no silent truncation
        "2024-03-05 06:07:08",  # space separator
        "10:00:00+02:00:00",  # +HH:MM:SS offset
        "10:00",  # HH:MM without seconds
    ],
)
def test_near_misses_stay_str(text):
    # "A fraction of 7 or more digits stays a `str`; so does a space separator,
    # a `+HH:MM:SS` offset, or `HH:MM` without seconds."
    assert type(revived(text)) is str


def test_round_trip_of_what_the_serializer_writes():
    # "`loads(dumps(x), parse_types=True) == x` holds for naive values, for
    # `timezone`-aware values with whole-minute offsets, and for UUIDs"
    value = {
        "naive": dt.datetime(2024, 1, 2, 3, 4, 5, 6),
        "aware": dt.datetime(2024, 1, 2, 3, 4, 5, tzinfo=dt.timezone(dt.timedelta(hours=-7))),
        "utc": dt.datetime(1999, 12, 31, 23, 59, 59, tzinfo=UTC),
        "date": dt.date(5, 6, 7),
        "time": dt.time(1, 2, 3, 400000),
        "aware_time": dt.time(1, 2, tzinfo=dt.timezone(dt.timedelta(minutes=90))),
        "uuid": uuid.UUID(int=(1 << 128) - 1),
    }
    assert strata.loads(strata.dumps(value), parse_types=True) == value


class _Summer(dt.tzinfo):
    """A zone that is not a fixed `timezone` (what a `ZoneInfo` is)."""

    def utcoffset(self, when):
        return dt.timedelta(hours=2)

    def dst(self, when):
        return dt.timedelta(hours=1)


def test_a_zone_comes_back_as_the_fixed_offset_it_had():
    # "a `ZoneInfo` comes back as the fixed offset it had"
    original = dt.datetime(2024, 7, 1, 12, 0, tzinfo=_Summer())
    back = strata.loads(strata.dumps(original), parse_types=True)
    assert back == original
    assert back.tzinfo == dt.timezone(dt.timedelta(hours=2))


# ---------------------------------------------------------------------------
# Registry
# ---------------------------------------------------------------------------


def test_registry_implies_true():
    # "Registry (`parse_types={"name": T, ...}`, implies `True`)"
    result = revived({"color": "red", "at": "2024-01-01"}, {"color": Color})
    assert result == {"color": Color.RED, "at": dt.date(2024, 1, 1)}


def test_an_empty_registry_is_true():
    assert revived(["2024-01-01"], {}) == [dt.date(2024, 1, 1)]


def test_registry_is_checked_before_parsing():
    # "keys must be `str`, values ... checked before parsing"
    with pytest.raises(TypeError, match="parse_types keys must be str"):
        strata.loads("not json", parse_types={1: Color})


def test_registry_applies_at_every_depth():
    # R1: "`{member_name: type}`, applied at every depth"
    result = revived({"a": [{"b": {"color": "blue"}}], "color": "red"}, {"color": Color})
    assert result == {"a": [{"b": {"color": Color.BLUE}}], "color": Color.RED}


def test_walk_is_post_order():
    # "The walk is post-order, so a container's contents are revived before
    # the container itself."
    text = json.dumps({"event": {"color": "red", "at": "2024-05-06", "point": {"x": 1}}})
    result = strata.loads(text, parse_types={"event": Event, "color": Color, "point": Point})
    assert result == {"event": Event(Color.RED, dt.date(2024, 5, 6), Point(1))}


def test_enum_member_replaces_its_value():
    # "an `Enum` type `E`: the value is replaced by `E(value)`"
    assert revived({"level": 2}, {"level": Level}) == {"level": Level.HIGH}
    assert revived({"color": "blue"}, {"color": Color})["color"] is Color.BLUE


def test_enum_value_error_leaves_the_value_as_parsed():
    # "a `ValueError` (no member has that value) leaves it as parsed"
    assert revived({"color": "green"}, {"color": Color}) == {"color": "green"}
    assert revived({"color": {"k": 1}}, {"color": Color}) == {"color": {"k": 1}}


def test_enum_other_exception_propagates():
    # "any other exception propagates"
    with pytest.raises(BoomError):
        revived({"s": "two"}, {"s": Strict})


def test_dataclass_replaces_a_fitting_dict():
    # "a dict value whose keys are all init fields of `D` and include every
    # init field without a default is replaced by `D(**value)`"
    assert revived({"p": {"x": 1}}, {"p": Point}) == {"p": Point(1)}
    assert revived({"p": {"x": 1, "y": 2, "tags": ["a"]}}, {"p": Point}) == {
        "p": Point(1, 2, ["a"]),
    }


@pytest.mark.parametrize(
    "value",
    [
        {"y": 2},  # a required init field is missing
        {"x": 1, "z": 3},  # a key that is not a field
        {"x": 1, "note": "n"},  # an init=False field is not an init field
        "2024-01-01",  # not a dict
        7,
        None,
    ],
)
def test_dataclass_leaves_any_other_value_as_parsed(value):
    # "any other value is left as parsed"
    assert revived({"p": value}, {"p": Point}) == {"p": value}


def test_dataclass_init_and_post_init_exceptions_propagate():
    # "exceptions from `D`'s `__init__` or `__post_init__` propagate"
    with pytest.raises(BoomError, match="post_init"):
        revived({"r": {"x": 1}}, {"r": Refuses})


def test_list_value_has_each_element_revived_one_level():
    # "a list value has each element revived by the same rule (one level)"
    result = revived({"color": ["red", "green", ["blue"]]}, {"color": Color})
    assert result == {"color": [Color.RED, "green", ["blue"]]}
    points = revived({"p": [{"x": 1}, {"q": 2}]}, {"p": Point})
    assert points == {"p": [Point(1), {"q": 2}]}


def test_registered_value_is_never_type_recognized():
    # "a value under a registered name is **never** type-recognized"
    result = revived({"color": "2024-01-01", "p": "2024-01-01"}, {"color": Color, "p": Point})
    assert result == {"color": "2024-01-01", "p": "2024-01-01"}
    assert revived({"color": ["2024-01-01"]}, {"color": Color}) == {"color": ["2024-01-01"]}


def test_registered_value_contents_follow_the_ordinary_rule():
    # docs/decisions.md 2026-09-28 (parse): the contents of a registered
    # member's container are walked as everywhere else.
    result = revived({"p": {"x": 1, "tags": ["2024-01-01"]}}, {"p": Point})
    assert result == {"p": Point(1, tags=[dt.date(2024, 1, 1)])}


# ---------------------------------------------------------------------------
# Per entry point
# ---------------------------------------------------------------------------


def test_loads_iterator_revives_before_iterating():
    # "the tree is revived before it is returned or iterated"
    pairs = list(strata.loads('{"a": "2024-01-01"}', iterator=True, parse_types=True))
    assert pairs == [("a", dt.date(2024, 1, 1))]
    items = list(strata.loads('["12:00:00"]', iterator=True, parse_types=True))
    assert items == [dt.time(12)]


def test_loads_cursor_is_refused():
    # "`return_type="cursor"` with `parse_types` set -> ValueError"
    with pytest.raises(ValueError, match=r"^parse_types needs return_type='dict'$"):
        strata.loads("{}", return_type="cursor", parse_types=True)
    assert strata.loads("{}", return_type="cursor", parse_types=False).is_object()


def test_load_file_ndjson_and_folder_are_revived(tmp_path):
    # "`loads`, `load` (file, NDJSON line by line, folder record by record,
    # eager and `iterator=True`)"
    directory = tmp_path / "d"
    directory.mkdir()
    document = _write(directory / "a.json", '{"a": "2024-01-01"}')
    lines = _write(directory / "b.ndjson", f'"12:00:00"\n{{"u": "{uuid.UUID(int=1)}"}}\n')
    expected_lines = [dt.time(12), {"u": uuid.UUID(int=1)}]
    assert strata.load(document, parse_types=True) == {"a": dt.date(2024, 1, 1)}
    assert strata.load(lines, parse_types=True) == expected_lines
    assert list(strata.load(lines, iterator=True, parse_types=True)) == expected_lines
    folder = [{"a": dt.date(2024, 1, 1)}, *expected_lines]
    assert strata.load(directory, parse_types=True) == folder
    assert list(strata.load(directory, iterator=True, parse_types=True)) == folder


def test_load_cursor_is_refused(tmp_path):
    document = _write(tmp_path / "a.json", "{}")
    with pytest.raises(ValueError, match=r"^parse_types needs return_type='dict'$"):
        strata.load(document, return_type="cursor", parse_types=True)


def test_skip_errors_covers_invalid_json_only(tmp_path):
    # "`skip_errors` still covers invalid JSON only; an exception from a
    # registered type propagates."
    lines = _write(tmp_path / "a.ndjson", '{"r": {"x": 1}}\nnot json\n')
    with pytest.raises(BoomError):
        strata.load(lines, skip_errors=True, parse_types={"r": Refuses})
    good = _write(tmp_path / "b.ndjson", '"2024-01-01"\nnot json\n')
    assert strata.load(good, skip_errors=True, parse_types=True) == [dt.date(2024, 1, 1)]


def test_search_equals_query_of_load(tmp_path):
    # "`search(f, e, parse_types=p) == query(load(f, parse_types=p), e)`"
    document = _write(tmp_path / "a.json", '{"a": ["2024-01-01", {"b": "12:00:00"}]}')
    for expression in ("$.a[*]", "$..b", "$.a[0:1]"):
        assert strata.search(document, expression, parse_types=True) == strata.query(
            strata.load(document, parse_types=True),
            expression,
        )


def test_search_takes_the_full_parse_path(tmp_path):
    # "with `parse_types` set a `.json` file takes the full-parse path that
    # Filter/Slice expressions already take (parse, revive, evaluate)": a plain
    # path, which streams by default, answers as load-then-query does.
    document = _write(tmp_path / "a.json", '{"a": {"x": 1, "tags": ["12:00:00"]}, "b": 2}')
    registry = {"a": Point}
    assert strata.search(document, "$.a", parse_types=registry) == [Point(1, tags=[dt.time(12)])]
    assert strata.search(document, "$.a") == [{"x": 1, "tags": ["12:00:00"]}]


def test_search_ndjson_line_is_revived_before_it_is_evaluated(tmp_path):
    lines = _write(tmp_path / "a.ndjson", '{"c": "red"}\n{"c": "blue"}\n')
    assert strata.search(lines, "$[*].c", parse_types={"c": Color}) == [Color.RED, Color.BLUE]


def test_query_true_replaces_str_matches_only():
    # "`query`: `True` replaces each match that is a `str` by the rule above;
    # container and other matches are returned as they are -- the caller's own
    # objects, never copied or mutated."
    inner = ["2024-01-01"]
    data = {"a": inner, "b": "2024-01-01", "c": 5}
    matches = strata.query(data, "$.*", parse_types=True)
    assert matches == [inner, dt.date(2024, 1, 1), 5]
    assert matches[0] is inner
    assert inner == ["2024-01-01"]
    assert data["b"] == "2024-01-01"


def test_query_dict_is_refused():
    # "A `dict` -> TypeError("query() parse_types must be a bool, not dict")"
    with pytest.raises(TypeError, match=r"^query\(\) parse_types must be a bool, not dict$"):
        strata.query({}, "$", parse_types={"a": Color})


def test_query_false_is_todays_behaviour():
    data = {"a": "2024-01-01"}
    assert strata.query(data, "$.a", parse_types=False) == strata.query(data, "$.a")


def _fresh(code):
    package_root = str(pathlib.Path(strata.__file__).resolve().parent.parent)
    prelude = "import sys\nsys.path.insert(0, sys.argv[1])\n"
    result = subprocess.run(
        [sys.executable, "-c", prelude + textwrap.dedent(code), package_root],
        capture_output=True,
        text=True,
        timeout=60,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    return result.stdout.split()


def test_first_use_imports_datetime_and_uuid_only_then():
    # "The first call with `parse_types` set imports `datetime` and `uuid` if
    # they are not already imported; `import strata` still imports neither."
    loaded = _fresh(
        """
        import sys
        names = ("datetime", "uuid")
        before = {name for name in names if name in sys.modules}
        import strata
        strata.loads('"2024-01-01"')
        strata.loads('"x"', parse_types=False)
        middle = {name for name in names if name in sys.modules}
        strata.loads('"x"', parse_types=True)
        after = {name for name in names if name in sys.modules}
        print(" ".join(sorted(middle - before)) or "none")
        print(" ".join(sorted(after)))
        """,
    )
    assert loaded == ["none", "datetime", "uuid"]


# ---------------------------------------------------------------------------
# Error contract: the parse rows
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("value", [None, 0, 1, "yes", ["a"], Color])
def test_error_parse_types_not_bool_or_dict(value, tmp_path):
    # Row: "`parse_types` not a `bool` or `dict`"
    document = _write(tmp_path / "a.json", "{}")
    message = rf"^parse_types must be a bool or a dict, not {type(value).__name__}$"
    with pytest.raises(TypeError, match=message):
        strata.loads("{}", parse_types=value)
    with pytest.raises(TypeError, match=message):
        strata.load(document, parse_types=value)
    with pytest.raises(TypeError, match=message):
        strata.search(document, "$", parse_types=value)


def test_error_registry_key_not_str():
    # Row: "registry key not `str`"
    with pytest.raises(TypeError, match=r"^parse_types keys must be str, not int$"):
        strata.loads("{}", parse_types={1: Color})


@pytest.mark.parametrize("value", [int, Point(1), Color.RED, "Color", None])
def test_error_registry_value_not_enum_or_dataclass_type(value):
    # Row: "registry value not an `Enum` subclass or dataclass type"
    message = "parse_types values must be Enum subclasses or dataclass types, not " + repr(value)
    with pytest.raises(TypeError) as raised:
        strata.loads("{}", parse_types={"a": value})
    assert str(raised.value) == message


def test_error_cursor_with_parse_types(tmp_path):
    # Row: "`parse_types` with `return_type="cursor"`"
    with pytest.raises(ValueError, match=r"^parse_types needs return_type='dict'$"):
        strata.loads("[]", return_type="cursor", parse_types={})
    with pytest.raises(ValueError, match=r"^parse_types needs return_type='dict'$"):
        strata.load(_write(tmp_path / "a.ndjson", "1\n"), return_type="cursor", parse_types=True)


@pytest.mark.parametrize("value", [{}, {"a": Color}])
def test_error_query_with_a_dict(value):
    # Row: "`query(..., parse_types=<dict>)`"
    with pytest.raises(TypeError, match=r"^query\(\) parse_types must be a bool, not dict$"):
        strata.query([], "$", parse_types=value)


def test_error_query_with_another_non_bool():
    # docs/decisions.md 2026-09-28 (parse): query names the type it got.
    with pytest.raises(TypeError, match=r"^query\(\) parse_types must be a bool, not int$"):
        strata.query([], "$", parse_types=1)

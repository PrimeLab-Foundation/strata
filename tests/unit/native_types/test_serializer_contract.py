"""Contract tests for native types in `dumps`, `dump` and `dumps_with_default`.

One named test per clause of docs/architecture/native_types.md -- "Serializer
contract" (rows 1-8; row 9, numpy, lives in tests/integrations because numpy is
not a gate dependency), "Frames, cycles and depth", "dumps_with_default" and
the serializer rows of "Error contract" -- each citing the clause it pins, plus
the api.md `dumps` bullets that carry them. Integration corpora, re-entrancy and
file output are in tests/py/native_types/.
"""

import dataclasses
import datetime as dt
import enum
import pathlib
import re
import subprocess
import sys
import textwrap
import typing
import uuid
import warnings
from decimal import Decimal

import pytest

import strata

MODES = ("str", "bytes")


def both(obj):
    """`dumps` in both return types; the two must agree byte for byte."""
    text = strata.dumps(obj)
    assert strata.dumps(obj, return_type="bytes") == text.encode()
    return text


def text_of(out):
    return out.decode() if isinstance(out, bytes) else out


def _python_stack_depth():
    depth = 0
    frame = sys._getframe()
    while frame is not None:
        depth += 1
        frame = frame.f_back
    return depth


class FixedOffset(dt.tzinfo):
    """A tzinfo that is not `datetime.timezone`: its utcoffset is user code."""

    def __init__(self, offset):
        self.offset = offset
        self.calls = []

    def utcoffset(self, when):
        self.calls.append(when)
        return self.offset

    def dst(self, when):
        return None


class FoldAware(dt.tzinfo):
    def utcoffset(self, when):
        return dt.timedelta(hours=1 if when.fold else 2)

    def dst(self, when):
        return None


# ---------------------------------------------------------------------------
# Row 1: `datetime.datetime`
# ---------------------------------------------------------------------------


def test_row1_naive_datetime_is_iso_text_without_a_fraction():
    assert both(dt.datetime(2026, 9, 28, 12, 3, 4)) == '"2026-09-28T12:03:04"'


def test_row1_a_nonzero_microsecond_writes_six_digits():
    assert both(dt.datetime(2026, 9, 28, 12, 3, 4, 50)) == '"2026-09-28T12:03:04.000050"'
    assert both(dt.datetime(2026, 9, 28, 12, 3, 4, 999999)) == '"2026-09-28T12:03:04.999999"'


def test_row1_years_are_zero_padded_to_four_digits():
    assert both(dt.datetime(5, 1, 2)) == '"0005-01-02T00:00:00"'
    assert both(dt.datetime(9999, 12, 31, 23, 59, 59)) == '"9999-12-31T23:59:59"'


def test_row1_utc_is_written_as_plus_zero():
    assert both(dt.datetime(2026, 1, 1, tzinfo=dt.timezone.utc)) == '"2026-01-01T00:00:00+00:00"'


def test_row1_an_aware_datetime_writes_its_offset():
    east = dt.timezone(dt.timedelta(hours=5, minutes=30))
    west = dt.timezone(dt.timedelta(hours=-8))
    assert both(dt.datetime(2026, 1, 1, tzinfo=east)) == '"2026-01-01T00:00:00+05:30"'
    assert both(dt.datetime(2026, 1, 1, 1, tzinfo=west)) == '"2026-01-01T01:00:00-08:00"'


@pytest.mark.parametrize(
    ("seconds", "text"),
    [
        (29, "+00:00"),
        (30, "+00:01"),
        (-29, "-00:00"),
        (-30, "-00:01"),
        (3600 + 89, "+01:01"),
        (3600 + 90, "+01:02"),
        (-(5 * 3600 + 30 * 60 + 30), "-05:31"),
    ],
)
def test_row1_the_offset_rounds_to_the_minute_half_up_in_magnitude(seconds, text):
    zone = dt.timezone(dt.timedelta(seconds=seconds))
    assert both(dt.datetime(2026, 1, 1, tzinfo=zone)) == f'"2026-01-01T00:00:00{text}"'


def test_row1_the_offset_drops_its_microseconds():
    zone = dt.timezone(dt.timedelta(seconds=29, microseconds=999999))
    assert both(dt.datetime(2026, 1, 1, tzinfo=zone)) == '"2026-01-01T00:00:00+00:00"'


def test_row1_a_tzinfo_whose_utcoffset_is_none_is_naive():
    zone = FixedOffset(None)
    assert both(dt.datetime(2026, 1, 1, tzinfo=zone)) == '"2026-01-01T00:00:00"'


def test_row1_another_tzinfo_is_asked_with_the_datetime_itself():
    zone = FixedOffset(dt.timedelta(hours=-3))
    moment = dt.datetime(2026, 1, 1, tzinfo=zone)
    assert strata.dumps(moment) == '"2026-01-01T00:00:00-03:00"'
    assert zone.calls == [moment]


def test_row1_fold_is_honoured_through_utcoffset():
    zone = FoldAware()
    first = dt.datetime(2026, 10, 25, 2, 30, tzinfo=zone)
    second = first.replace(fold=1)
    assert both(first) == '"2026-10-25T02:30:00+02:00"'
    assert both(second) == '"2026-10-25T02:30:00+01:00"'


class Stamp(dt.datetime):
    pass


class Day(dt.date):
    pass


class Clock(dt.time):
    pass


TEMPORAL_SUBCLASSES = [
    Stamp(2026, 9, 28, 1, 2, 3, tzinfo=dt.timezone.utc),
    Stamp(2026, 9, 28, 1, 2, 3),
    Day(2026, 2, 3),
    Clock(1, 2, 3),
]


@pytest.mark.parametrize("value", TEMPORAL_SUBCLASSES, ids=repr)
def test_rows1_to_3_a_subclass_is_unsupported(value):
    # docs/decisions.md 2026-09-28 (review P1): the exact types only, as orjson;
    # a subclass can carry state its C fields do not (pandas' NaT).
    message = f"^Object of type {type(value).__name__} is not JSON serializable$"
    for document in (value, [value], {"a": value}):
        with pytest.raises(TypeError, match=message):
            strata.dumps(document)
        with pytest.raises(TypeError, match=message):
            strata.dumps(document, return_type="bytes")


def test_row1_a_utcoffset_that_is_not_a_timedelta_raises_type_error():
    # docs/decisions.md 2026-09-28: the checks `isoformat()` makes, same messages.
    message = "tzinfo.utcoffset() must return None or timedelta, not 'int'"
    with pytest.raises(TypeError, match=f"^{re.escape(message)}$"):
        strata.dumps(dt.datetime(2026, 1, 1, tzinfo=FixedOffset(5)))


def test_row1_an_offset_of_a_day_or_more_raises_value_error():
    with pytest.raises(ValueError, match="strictly between"):
        strata.dumps(dt.datetime(2026, 1, 1, tzinfo=FixedOffset(dt.timedelta(days=1))))
    with pytest.raises(ValueError, match="strictly between"):
        strata.dumps(dt.datetime(2026, 1, 1, tzinfo=FixedOffset(dt.timedelta(days=-1))))


# ---------------------------------------------------------------------------
# Row 2: `datetime.date`; row 3: `datetime.time`
# ---------------------------------------------------------------------------


def test_row2_a_date_is_year_month_day():
    assert both(dt.date(2026, 9, 28)) == '"2026-09-28"'
    assert both(dt.date(1, 1, 1)) == '"0001-01-01"'


def test_row1_a_datetime_is_not_written_as_its_date_base():
    # Precedence: datetime before date, because it subclasses it.
    assert both(dt.datetime(2026, 2, 3)) == '"2026-02-03T00:00:00"'


def test_row3_a_naive_time_with_and_without_a_fraction():
    assert both(dt.time(1, 2, 3)) == '"01:02:03"'
    assert both(dt.time(23, 59, 59, 1)) == '"23:59:59.000001"'


def test_row3_an_aware_time_writes_its_offset():
    assert both(dt.time(1, 2, 3, tzinfo=dt.timezone.utc)) == '"01:02:03+00:00"'
    zone = dt.timezone(dt.timedelta(hours=-2, minutes=-30))
    assert both(dt.time(1, 2, 3, tzinfo=zone)) == '"01:02:03-02:30"'


def test_row3_a_time_asks_its_tzinfo_with_none():
    zone = FixedOffset(dt.timedelta(hours=1))
    assert strata.dumps(dt.time(1, 2, 3, tzinfo=zone)) == '"01:02:03+01:00"'
    assert zone.calls == [None]


def test_row3_a_time_whose_tzinfo_returns_none_is_naive():
    assert both(dt.time(1, 2, 3, tzinfo=FixedOffset(None))) == '"01:02:03"'


# ---------------------------------------------------------------------------
# Row 4: `uuid.UUID`
# ---------------------------------------------------------------------------


def test_row4_a_uuid_is_lowercase_8_4_4_4_12_from_its_int():
    value = uuid.UUID("A1B2C3D4-E5F6-4711-8899-AABBCCDDEEFF")
    assert both(value) == '"a1b2c3d4-e5f6-4711-8899-aabbccddeeff"'
    assert both(uuid.UUID(int=0)) == '"00000000-0000-0000-0000-000000000000"'
    assert both(uuid.UUID(int=(1 << 128) - 1)) == '"ffffffff-ffff-ffff-ffff-ffffffffffff"'


def test_row4_a_uuid_subclass_is_written_from_its_int():
    class Key(uuid.UUID):
        pass

    key = Key(int=0x1234)
    assert both(key) == f'"{key}"'


def test_row4_an_int_outside_128_bits_raises_value_error():
    # docs/decisions.md 2026-09-28: only object.__setattr__ can store one.
    broken = uuid.UUID(int=1)
    object.__setattr__(broken, "int", 1 << 128)
    with pytest.raises(ValueError, match=r"^UUID\.int is out of range \(need a 128-bit value\)$"):
        strata.dumps(broken)
    object.__setattr__(broken, "int", -1)
    with pytest.raises(ValueError, match="out of range"):
        strata.dumps(broken)


def test_row4_an_int_that_is_not_an_int_raises_type_error():
    broken = uuid.UUID(int=1)
    object.__setattr__(broken, "int", "1")
    with pytest.raises(TypeError, match=r"^UUID\.int must be an int, not str$"):
        strata.dumps(broken)


# ---------------------------------------------------------------------------
# Row 5: `decimal.Decimal`
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "text", ["1.50", "1E+2", "-0", "0E-7", "123456789012345678901234567890.000001", "-1.5E-300"]
)
def test_row5_a_finite_decimal_is_the_raw_text_of_str(text):
    assert both(Decimal(text)) == str(Decimal(text))


@pytest.mark.parametrize("text", ["NaN", "-NaN", "sNaN", "-sNaN12", "NaN123", "Infinity", "-Inf"])
def test_row5_a_non_finite_decimal_is_null(text):
    assert both(Decimal(text)) == "null"


def test_row5_a_decimal_subclass_whose_str_is_a_number_is_written_as_that_text():
    class Money(Decimal):
        def __str__(self):
            return "12.50"

    assert both(Money("1")) == "12.50"


@pytest.mark.parametrize("spelling", ["12,5", "1.", ".5", "+1", "0x10", "inf", " 1", ""])
def test_row5_a_decimal_subclass_whose_str_is_not_a_number_raises_value_error(spelling):
    class Odd(Decimal):
        def __str__(self):
            return spelling

    message = "^str\\(\\) of a Decimal returned text that is not a JSON number$"
    with pytest.raises(ValueError, match=message):
        strata.dumps(Odd("1"))


# ---------------------------------------------------------------------------
# Row 6: `enum.Enum`
# ---------------------------------------------------------------------------


class Color(enum.Enum):
    RED = "red"
    ONE = 1
    PAIR = (1, 2)
    NOTHING = None
    DAY = dt.date(2026, 1, 2)


class Wrapper(enum.Enum):
    INNER = Color.RED


class Rank(enum.IntEnum):
    HIGH = 5


class Mixed(str, enum.Enum):
    X = "x"


class Perm(enum.Flag):
    R = 4
    W = 2


def test_row6_a_member_is_written_as_its_value():
    assert both(Color.RED) == '"red"'
    assert both(Color.ONE) == "1"
    assert both(Color.PAIR) == "[1,2]"
    assert both(Color.NOTHING) == "null"
    assert both(Perm.R | Perm.W) == "6"


def test_row6_a_value_that_is_native_is_written_natively():
    assert both(Color.DAY) == '"2026-01-02"'


def test_row6_a_value_that_is_an_enum_is_followed():
    assert both(Wrapper.INNER) == '"red"'


def test_row6_int_and_str_mixins_are_written_by_the_subclass_branch():
    # docs/decisions.md 2026-09-28: IntEnum -> the int, (str, Enum) -> the str.
    assert both(Rank.HIGH) == "5"
    assert both(Mixed.X) == '"x"'
    assert both({"rank": Rank.HIGH, "mixed": [Mixed.X]}) == '{"rank":5,"mixed":["x"]}'


def test_row6_value_is_read_with_getattr():
    class Custom(enum.Enum):
        A = 1

        @property
        def value(self):
            return "from the property"

    assert both(Custom.A) == '"from the property"'


def test_error_contract_an_enum_chain_longer_than_the_depth_limit_raises():
    # The chain recurses to the depth limit; how much C stack that takes is the
    # build's own, so it runs in a child interpreter an overflow cannot outlive.
    code = """
        import enum, sys
        sys.path.insert(0, sys.argv[1])
        import strata

        class Loop(enum.Enum):
            SELF = 1

        member = Loop.SELF
        member._value_ = member
        for document in (member, [{"a": member}]):
            try:
                strata.dumps(document)
            except ValueError as error:
                print(error)
        """
    package_root = str(pathlib.Path(strata.__file__).resolve().parent.parent)
    result = subprocess.run(
        [sys.executable, "-c", textwrap.dedent(code), package_root],
        capture_output=True,
        text=True,
        timeout=120,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    assert result.stdout.splitlines() == ["Maximum serialization depth exceeded"] * 2


def test_row6_a_chain_of_exactly_the_depth_limit_is_written():
    saved = sys.getrecursionlimit()
    limit = max(200, _python_stack_depth() + 100)
    try:
        sys.setrecursionlimit(limit)
        members = []
        value = "end"
        for index in range(limit + 1):
            value = enum.Enum(f"Hop{index}", {"M": value}).M
            members.append(value)
        # members[k] needs k + 1 reads of `.value` to reach "end".
        assert strata.dumps(members[limit - 1]) == '"end"'
        with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
            strata.dumps(members[limit])
    finally:
        sys.setrecursionlimit(saved)


# ---------------------------------------------------------------------------
# Row 7: dataclass instances
# ---------------------------------------------------------------------------


@dataclasses.dataclass
class Point:
    x: int
    y: float
    label: str = "p"


@dataclasses.dataclass(frozen=True, slots=True)
class Frozen:
    b: int
    a: object


def test_row7_a_dataclass_is_an_object_of_its_fields_in_order():
    assert both(Point(1, 2.5)) == '{"x":1,"y":2.5,"label":"p"}'
    assert both(Frozen(2, [Point(0, 0.0)])) == '{"b":2,"a":[{"x":0,"y":0.0,"label":"p"}]}'


def test_row7_extra_instance_attributes_are_not_written():
    point = Point(1, 2.0)
    point.extra = "hidden"
    point._private = 1
    assert both(point) == '{"x":1,"y":2.0,"label":"p"}'


def test_row7_classvars_and_initvars_are_not_fields():
    @dataclasses.dataclass
    class WithPseudo:
        kept: int
        shared: typing.ClassVar[int] = 3
        seed: dataclasses.InitVar[int] = 0

        def __post_init__(self, seed):
            self.derived = seed

    assert both(WithPseudo(1, 9)) == '{"kept":1}'


def test_row7_the_field_name_cache_follows_fields_replaced_or_grown():
    # Review P2: a cached entry stands only while the type's
    # `__dataclass_fields__` is the object it was read from, at the length it had.
    @dataclasses.dataclass
    class Grows:
        a: int = 1

    @dataclasses.dataclass
    class Other:
        a: int = 1
        b: int = 2
        c: int = 3

    value = Grows()
    value.b = 2
    value.c = 3

    def listed():
        names = ",".join(
            f'"{field.name}":{getattr(value, field.name)}' for field in dataclasses.fields(value)
        )
        return "{" + names + "}"

    assert both(value) == listed() == '{"a":1}'
    Grows.__dataclass_fields__["b"] = Other.__dataclass_fields__["b"]  # grown in place
    assert both(value) == listed() == '{"a":1,"b":2}'
    Grows.__dataclass_fields__ = {  # replaced, at the same length
        "a": Other.__dataclass_fields__["a"],
        "c": Other.__dataclass_fields__["c"],
    }
    assert both(value) == listed() == '{"a":1,"c":3}'


def test_row7_the_field_name_cache_follows_fields_swapped_in_place():
    # Review P2: an entry stands only while the dict holds the keys and field
    # objects it was read from, by identity and in order -- the same dict at
    # the same length is not enough.
    @dataclasses.dataclass
    class Swapped:
        a: int = 1

    @dataclasses.dataclass
    class Donor:
        b: int = 2
        c: int = 3

    value = Swapped()
    value.b = 2
    value.c = 3

    def written():
        listed = ",".join(
            f'"{field.name}":{getattr(value, field.name)}' for field in dataclasses.fields(value)
        )
        out = both(value)
        assert strata.dumps_with_default(value, repr) == out == "{" + listed + "}"
        return out

    assert written() == '{"a":1}'
    declared = Swapped.__dataclass_fields__
    del declared["a"]  # the review's case: one key out, one in, same dict, same length
    declared["b"] = Donor.__dataclass_fields__["b"]
    assert written() == '{"b":2}'
    declared["b"] = Donor.__dataclass_fields__["c"]  # the value swapped under the same key
    assert written() == '{"c":3}'


def test_row7_pseudo_fields_stay_out_of_cached_and_swapped_entries():
    @dataclasses.dataclass
    class WithPseudo:
        kept: int = 1
        dropped: int = 2
        shared: typing.ClassVar[int] = 3
        seed: dataclasses.InitVar[int] = 0

    value = WithPseudo()
    assert both(value) == '{"kept":1,"dropped":2}'
    assert both(value) == '{"kept":1,"dropped":2}'  # from the cached entry
    declared = WithPseudo.__dataclass_fields__
    field = declared["dropped"]
    for pseudo in ("shared", "seed"):  # a ClassVar, then an InitVar, under a field's key
        declared["dropped"] = declared[pseudo]
        assert both(value) == '{"kept":1}'
        assert strata.dumps_with_default(value, repr) == '{"kept":1}'
    declared["dropped"] = field
    assert both(value) == '{"kept":1,"dropped":2}'


def test_row7_more_dataclass_types_than_the_cache_holds_are_each_written_by_their_fields():
    # python_native_types.h: at most 1024 types are cached; a full cache is cleared.
    kinds = [dataclasses.make_dataclass(f"K{index}", [(f"f{index}", int)]) for index in range(1100)]
    for _ in range(2):
        for index, kind in enumerate(kinds):
            assert strata.dumps(kind(index)) == f'{{"f{index}":{index}}}'


def test_row7_fields_are_read_with_getattr():
    @dataclasses.dataclass
    class Computed:
        raw: int

    Computed.raw = property(lambda self: 41 + 1)
    instance = Computed.__new__(Computed)
    assert both(instance) == '{"raw":42}'


def test_row7_field_names_are_escaped_as_dict_keys():
    namespace = {}
    exec(
        "import dataclasses\n@dataclasses.dataclass\nclass Café:\n    naïve: int\n",
        namespace,
    )
    instance = namespace["Café"](1)
    assert both(instance) == strata.dumps({"naïve": 1})


def test_error_contract_an_unset_field_raises_its_attribute_error():
    @dataclasses.dataclass
    class Late:
        early: int
        late: int = dataclasses.field(init=False)

    with pytest.raises(AttributeError, match="late"):
        strata.dumps(Late(1))


def test_row7_a_dataclass_class_is_not_an_instance():
    with pytest.raises(TypeError, match="^Object of type type is not JSON serializable$"):
        strata.dumps(Point)


# ---------------------------------------------------------------------------
# Row 8: `set` and `frozenset`
# ---------------------------------------------------------------------------


def test_row8_a_set_is_an_array_in_iteration_order():
    values = {3, 1, 2, "a", None}
    assert both(values) == strata.dumps(list(values))
    frozen = frozenset({"x", "y"})
    assert both(frozen) == strata.dumps(list(frozen))
    assert both(set()) == "[]"


def test_row8_a_set_subclass_is_written_in_its_own_iteration_order():
    class Ordered(set):
        def __iter__(self):
            return iter(sorted(super().__iter__(), reverse=True))

    assert both(Ordered({1, 2, 3})) == "[3,2,1]"


def test_error_contract_a_set_resized_while_written_raises_its_runtime_error():
    victim = {1, 2, 3}

    class Grow(enum.Enum):
        A = 1

        @property
        def value(self):
            victim.add(len(victim) + 100)
            return 0

    victim.add(Grow.A)
    with pytest.raises(RuntimeError, match="changed size during iteration"):
        strata.dumps(victim)


# ---------------------------------------------------------------------------
# Unchanged: keys, `dump`'s split values and every other type.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    ("key", "name"),
    [
        (dt.date(2026, 1, 1), "datetime.date"),
        (uuid.UUID(int=1), "UUID"),
        (Decimal(1), "decimal.Decimal"),
        (Color.RED, "Color"),
        (frozenset(), "frozenset"),
    ],
)
def test_unchanged_a_native_key_is_the_keys_must_be_str_error(key, name):
    with pytest.raises(TypeError, match=f"^keys must be str, not {name}$"):
        strata.dumps({key: 1})


@pytest.mark.parametrize(
    ("value", "name"),
    [
        (dt.timedelta(1), "datetime.timedelta"),
        (1 + 2j, "complex"),
        (b"x", "bytes"),
        (object(), "object"),
    ],
)
def test_error_contract_every_other_type_is_the_unchanged_type_error(value, name):
    message = f"^Object of type {name} is not JSON serializable$"
    with pytest.raises(TypeError, match=message):
        strata.dumps(value)
    with pytest.raises(TypeError, match=message):
        strata.dumps([Point(1, 1.0), {"a": value}])


def test_unchanged_dump_split_values_stay_str_int_bool(tmp_path):
    message = "^split_by values must be str, int or bool, not datetime.date$"
    with pytest.raises(TypeError, match=message):
        strata.dump([{"k": dt.date(2026, 1, 1)}], tmp_path / "out", split_by="k")


# ---------------------------------------------------------------------------
# Frames, cycles and depth
# ---------------------------------------------------------------------------


@dataclasses.dataclass
class Node:
    name: str
    child: object = None


def _self_cycle():
    node = Node("a")
    node.child = node
    return node


@pytest.fixture
def cycle_policy():
    saved = strata.config.get("cycle_policy")

    def apply(policy):
        strata.config.set("cycle_policy", policy)

    yield apply
    strata.config.set("cycle_policy", saved)


def test_frames_a_dataclass_that_contains_itself_warns_and_writes_null(cycle_policy):
    cycle_policy("warn")
    with pytest.warns(RuntimeWarning, match="Circular reference detected"):
        assert strata.dumps(_self_cycle()) == '{"name":"a","child":null}'


def test_frames_a_dataclass_cycle_raises_under_error(cycle_policy):
    cycle_policy("error")
    with pytest.raises(ValueError, match="^Circular reference detected$"):
        strata.dumps(_self_cycle())


def test_frames_a_dataclass_cycle_is_silent_under_ignore(cycle_policy):
    cycle_policy("ignore")
    with warnings.catch_warnings():
        warnings.simplefilter("error")
        assert strata.dumps([_self_cycle()]) == '[{"name":"a","child":null}]'


def test_frames_a_frozenset_reached_back_through_a_frozen_dataclass_is_a_cycle(cycle_policy):
    cycle_policy("ignore")
    holder = Frozen(1, None)
    ring = frozenset({holder})
    object.__setattr__(holder, "a", ring)
    assert strata.dumps(ring) == '[{"b":1,"a":null}]'


def test_frames_a_repeated_dataclass_that_is_not_a_cycle_is_written_twice():
    shared = Point(1, 1.0)
    assert strata.dumps([shared, {"again": shared}]) == (
        '[{"x":1,"y":1.0,"label":"p"},{"again":{"x":1,"y":1.0,"label":"p"}}]'
    )


@pytest.mark.parametrize("wrap", ["dataclass", "set"])
def test_frames_a_dataclass_or_set_takes_one_level_of_the_depth_limit(wrap):
    saved = sys.getrecursionlimit()
    limit = max(200, _python_stack_depth() + 100)

    def nest(levels):
        node = 1
        for _ in range(levels):
            node = Node("n", node) if wrap == "dataclass" else frozenset({node})
        return node

    try:
        sys.setrecursionlimit(limit)
        opened = "{" if wrap == "dataclass" else "["
        assert strata.dumps(nest(limit)).count(opened) == limit
        with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
            strata.dumps(nest(limit + 1))
        with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
            strata.dumps([nest(limit)])
    finally:
        sys.setrecursionlimit(saved)


def test_frames_an_enum_member_takes_one_level_of_the_depth_limit():
    # docs/decisions.md 2026-09-28 (review P0): the member opens a Frame.
    saved = sys.getrecursionlimit()
    limit = max(200, _python_stack_depth() + 100)
    holder = enum.Enum("Holder", {"LEAF": Node("leaf")}).LEAF

    def nest(levels, leaf):
        node = leaf
        for _ in range(levels):
            node = Node("n", node)
        return node

    try:
        sys.setrecursionlimit(limit)
        assert strata.dumps(nest(limit - 1, Node("leaf"))).count("{") == limit
        assert strata.dumps(nest(limit - 2, holder)).count("{") == limit - 1
        with pytest.raises(ValueError, match="^Maximum serialization depth exceeded$"):
            strata.dumps(nest(limit - 1, holder))
    finally:
        sys.setrecursionlimit(saved)


class _Opaque:
    pass


@pytest.mark.parametrize("mode", MODES)
@pytest.mark.parametrize("policy", ["warn", "error", "ignore"])
def test_hook_a_member_whose_value_is_the_object_default_was_called_on_is_a_cycle(
    cycle_policy, policy, mode
):
    # docs/decisions.md 2026-09-28 (review P0): `default` -> E.A -> its value ->
    # `default` -> E.A recursed without bound; the member's Frame makes the
    # second E.A a cycle under the policy.
    opaque = _Opaque()
    member = enum.Enum("Loops", {"A": opaque}).A
    calls = []

    def default(obj):
        calls.append(obj)
        return member

    cycle_policy(policy)
    for document, expected in ((opaque, "null"), ([opaque, 1], "[null,1]")):
        calls.clear()
        if policy == "error":
            with pytest.raises(ValueError, match="^Circular reference detected$"):
                strata.dumps_with_default(document, default, return_type=mode)
        elif policy == "warn":
            with pytest.warns(RuntimeWarning, match="Circular reference detected"):
                out = strata.dumps_with_default(document, default, return_type=mode)
            assert text_of(out) == expected
        else:
            with warnings.catch_warnings():
                warnings.simplefilter("error")
                out = strata.dumps_with_default(document, default, return_type=mode)
            assert text_of(out) == expected
        assert calls == [opaque, opaque]


# ---------------------------------------------------------------------------
# dumps_with_default: natives come before the callable.
# ---------------------------------------------------------------------------


NATIVES = [
    dt.datetime(2026, 1, 1, 1, tzinfo=dt.timezone.utc),
    dt.date(2026, 1, 1),
    dt.time(1, 2),
    uuid.UUID(int=7),
    Decimal("2.50"),
    Color.RED,
    Point(1, 2.0),
    {1, 2},
    frozenset({"a"}),
]


@pytest.mark.parametrize("mode", MODES)
def test_hook_default_is_never_called_for_a_native_object(mode):
    calls = []

    def default(obj):
        calls.append(obj)
        return "hooked"

    for value in NATIVES:
        for document in (value, [value], {"a": value}, [{"a": [value]}]):
            expected = strata.dumps(document, return_type=mode)
            assert strata.dumps_with_default(document, default, return_type=mode) == expected
    assert calls == []


@pytest.mark.parametrize("mode", MODES)
def test_hook_a_native_the_callable_returns_is_written_natively(mode):
    class Opaque:
        pass

    for value in NATIVES:
        expected = strata.dumps([value], return_type=mode)
        assert strata.dumps_with_default([Opaque()], lambda obj: value, return_type=mode) == (
            expected
        )


@pytest.mark.parametrize("mode", MODES)
def test_hook_an_unsupported_value_inside_a_native_gets_its_own_call(mode):
    class Opaque:
        def __init__(self, tag):
            self.tag = tag

    class Holder(enum.Enum):
        A = 1

    inner = [Opaque("in-field"), Opaque("in-set"), Opaque("in-value")]
    object.__setattr__(Holder.A, "_value_", inner[2])
    document = [Node("n", inner[0]), frozenset({inner[1]}), Holder.A]
    seen = []

    def default(obj):
        seen.append(obj)
        return obj.tag

    try:
        out = strata.dumps_with_default(document, default, return_type=mode)
    finally:
        object.__setattr__(Holder.A, "_value_", 1)
    text = out.decode() if isinstance(out, bytes) else out
    assert text == '[{"name":"n","child":"in-field"},["in-set"],"in-value"]'
    assert seen == inner


@pytest.mark.parametrize("mode", MODES)
def test_hook_the_chain_bound_still_refuses_an_unsupported_return(mode):
    class Opaque:
        pass

    message = "^default\\(\\) returned an object of type Opaque that is not JSON serializable$"
    with pytest.raises(TypeError, match=message):
        strata.dumps_with_default([Point(1, 1.0), Opaque()], lambda obj: obj, return_type=mode)


@pytest.mark.parametrize("mode", MODES)
def test_hook_a_temporal_subclass_is_passed_to_default(mode):
    # docs/decisions.md 2026-09-28 (review P1): the caller's route for subclasses.
    seen = []

    def default(obj):
        seen.append(obj)
        return obj.isoformat()

    out = strata.dumps_with_default(TEMPORAL_SUBCLASSES, default, return_type=mode)
    assert text_of(out) == "[" + ",".join(f'"{v.isoformat()}"' for v in TEMPORAL_SUBCLASSES) + "]"
    assert seen == TEMPORAL_SUBCLASSES

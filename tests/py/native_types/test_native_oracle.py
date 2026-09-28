"""The oracle for native types: every row's reference spelling in Python.

docs/architecture/native_types.md, "The oracle": over generated corpora,
`strata.dumps(x)` is byte-identical to `strata.dumps(ref(x))` -- the document
with every native object replaced by its reference spelling, which takes the
supported-type path -- and `json.loads(strata.dumps(x))` equals
`json.loads(json.dumps(x, default=ref))`. `dump` writes the same bytes to a
file and to folder records; `dumps_with_default` writes them too and never
calls its callable for a native object.
"""

import dataclasses
import datetime as dt
import enum
import json
import random
import re
import uuid
from decimal import Decimal

import pytest

import strata

MODES = ("str", "bytes")

SEED = 20260928

#: Marks a Decimal's text inside a reference document; `splice` replaces the
#: quoted marker with the raw number, which no supported type can spell.
_DECIMAL = "\x00decimal:"
_SPLICE = re.compile(r'"\\u0000decimal:([^"\\]*)\\u0000"')


def _offset(delta):
    seconds = delta.days * 86400 + delta.seconds
    sign = "-" if seconds < 0 else "+"
    minutes = (abs(seconds) + 30) // 60
    return f"{sign}{minutes // 60:02d}:{minutes % 60:02d}"


def _clock(hour, minute, second, microsecond):
    text = f"{hour:02d}:{minute:02d}:{second:02d}"
    return f"{text}.{microsecond:06d}" if microsecond else text


def reference(obj):
    """One level of a native object's reference spelling; non-natives unchanged."""
    if isinstance(obj, dt.datetime):
        text = f"{obj.year:04d}-{obj.month:02d}-{obj.day:02d}T" + _clock(
            obj.hour, obj.minute, obj.second, obj.microsecond
        )
        offset = obj.tzinfo.utcoffset(obj) if obj.tzinfo is not None else None
        return text + (_offset(offset) if offset is not None else "")
    if isinstance(obj, dt.date):
        return f"{obj.year:04d}-{obj.month:02d}-{obj.day:02d}"
    if isinstance(obj, dt.time):
        text = _clock(obj.hour, obj.minute, obj.second, obj.microsecond)
        offset = obj.tzinfo.utcoffset(None) if obj.tzinfo is not None else None
        return text + (_offset(offset) if offset is not None else "")
    if isinstance(obj, uuid.UUID):
        return str(obj)
    if isinstance(obj, Decimal):
        return f"{_DECIMAL}{obj}\x00" if obj.is_finite() else None
    if isinstance(obj, enum.Enum) and not isinstance(obj, (int, str, float)):
        return obj.value
    if dataclasses.is_dataclass(obj) and not isinstance(obj, type):
        return {field.name: getattr(obj, field.name) for field in dataclasses.fields(obj)}
    if isinstance(obj, (set, frozenset)):
        return list(obj)
    return obj


def deep_reference(obj):
    """The whole document with every native replaced by its reference spelling."""
    converted = reference(obj)
    while converted is not obj:
        obj, converted = converted, reference(converted)
    if isinstance(obj, dict):
        return {key: deep_reference(value) for key, value in obj.items()}
    if isinstance(obj, (list, tuple)):
        return [deep_reference(value) for value in obj]
    return obj


def splice(text):
    return _SPLICE.sub(r"\1", text)


def loads_reference(obj):
    """`default` for the stdlib: a Decimal as the float `json.loads` reads its text as."""
    if isinstance(obj, Decimal):
        return float(obj) if obj.is_finite() else None
    return reference(obj)


# ---------------------------------------------------------------------------
# The corpus
# ---------------------------------------------------------------------------


class Color(enum.Enum):
    RED = "red"
    ONE = 1
    PAIR = (1, "two")
    NOTHING = None


class Wrapped(enum.Enum):
    DAY = dt.date(2026, 9, 28)
    COLOR = Color.RED


class Rank(enum.IntEnum):
    LOW = 1


class Tag(str, enum.Enum):
    A = "a"


class Zone(dt.tzinfo):
    def __init__(self, seconds):
        self.seconds = seconds

    def utcoffset(self, when):
        return None if self.seconds is None else dt.timedelta(seconds=self.seconds)

    def dst(self, when):
        return None


class Key(uuid.UUID):
    pass


@dataclasses.dataclass
class Line:
    sku: str
    qty: int
    price: Decimal


@dataclasses.dataclass(frozen=True, slots=True)
class Order:
    id: uuid.UUID
    placed: dt.datetime
    status: Color
    lines: list
    tags: frozenset


def _zone(rng):
    return rng.choice(
        [
            None,
            dt.timezone.utc,
            dt.timezone(dt.timedelta(minutes=rng.randrange(-1439, 1440))),
            dt.timezone(dt.timedelta(seconds=rng.randrange(-86399, 86400))),
            Zone(rng.randrange(-86399, 86400)),
            Zone(None),
        ]
    )


def _datetime(rng):
    return dt.datetime(
        rng.randrange(1, 10000),
        rng.randrange(1, 13),
        rng.randrange(1, 29),
        rng.randrange(24),
        rng.randrange(60),
        rng.randrange(60),
        rng.choice([0, rng.randrange(1_000_000)]),
        tzinfo=_zone(rng),
        fold=rng.randrange(2),
    )


def _decimal(rng):
    return rng.choice(
        [
            Decimal(rng.randrange(-(10**12), 10**12)).scaleb(-rng.randrange(8)),
            Decimal(f"{rng.randrange(1, 10)}E{rng.randrange(-400, 400)}"),
            Decimal("-0"),
            Decimal("NaN"),
            Decimal("-Infinity"),
            Decimal("1.50"),
        ]
    )


def _native(rng):
    roll = rng.randrange(14)
    if roll == 0:
        return _datetime(rng)
    if roll == 1:
        return _datetime(rng).date()
    if roll == 2:
        return _datetime(rng).timetz()
    if roll == 3:
        return uuid.UUID(int=rng.getrandbits(128))
    if roll == 4:
        return _decimal(rng)
    if roll == 5:
        return rng.choice([*Color, *Wrapped, *Rank, *Tag])
    if roll == 6:
        return Line(f"sku-{rng.randrange(99)}", rng.randrange(9), _decimal(rng))
    if roll == 7:
        return {rng.randrange(50) for _ in range(rng.randrange(5))}
    if roll == 8:
        return frozenset(f"t{rng.randrange(9)}" for _ in range(rng.randrange(4)))
    if roll == 9:
        # A datetime subclass is unsupported (docs/decisions.md 2026-09-28);
        # this roll is the naive datetime instead.
        return _datetime(rng).replace(tzinfo=None)
    if roll == 10:
        return Key(int=rng.getrandbits(128))
    if roll == 11:
        # Hashable natives only: a set cannot hold a set or a mutable dataclass.
        return frozenset({_datetime(rng), uuid.UUID(int=rng.getrandbits(128)), _decimal(rng)})
    return Order(
        id=uuid.UUID(int=rng.getrandbits(128)),
        placed=_datetime(rng),
        status=rng.choice(list(Color)),
        lines=[Line("a", 1, _decimal(rng)) for _ in range(rng.randrange(3))],
        tags=frozenset({"x"}),
    )


def _value(rng, depth=0):
    roll = rng.random()
    if depth > 3 or roll < 0.25:
        return rng.choice([None, True, 0, -7, 2**70, 0.5, "", "é"])
    if roll < 0.55:
        return _native(rng)
    if roll < 0.7:
        return [_value(rng, depth + 1) for _ in range(rng.randint(0, 5))]
    if roll < 0.8:
        width = rng.randint(1, 6)
        return [
            {f"f{index}": _value(rng, depth + 2) for index in range(width)}
            for _ in range(rng.randint(1, 8))
        ]
    return {f"k{index}": _value(rng, depth + 1) for index in range(rng.randint(0, 7))}


def _corpus(count):
    rng = random.Random(SEED)
    return [_value(rng) for _ in range(count)]


CORPUS = _corpus(400)


# ---------------------------------------------------------------------------
# The two oracles
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_the_corpus_is_byte_identical_to_its_reference_spelling(mode):
    for document in CORPUS:
        expected = splice(strata.dumps(deep_reference(document)))
        out = strata.dumps(document, return_type=mode)
        assert (out.decode() if mode == "bytes" else out) == expected


def test_the_corpus_reads_back_as_the_stdlib_writes_it_with_the_reference_default():
    for document in CORPUS:
        expected = json.loads(json.dumps(document, default=loads_reference))
        assert json.loads(strata.dumps(document)) == expected


def test_each_row_has_a_reference_spelling_in_the_corpus():
    kinds = set()

    def record(obj):
        kinds.add(type(obj))
        return loads_reference(obj)

    for document in CORPUS:
        json.dumps(document, default=record)
    for kind in (dt.datetime, dt.date, dt.time, uuid.UUID, Decimal, Color, Wrapped, Line, Order):
        assert kind in kinds
    assert set in kinds and frozenset in kinds and Key in kinds


# ---------------------------------------------------------------------------
# dump: a file, NDJSON lines read back, and folder records.
# ---------------------------------------------------------------------------


def _records(count):
    rng = random.Random(SEED + 1)
    return [
        {
            "region": rng.choice(["eu", "us", "apac"]),
            "id": uuid.UUID(int=index),
            "at": dt.datetime(2026, 1, 1, tzinfo=dt.timezone.utc) + dt.timedelta(hours=index),
            "amount": Decimal(index) / 4,
            "tags": frozenset({"a"}),
            "color": rng.choice(list(Color)),
            "line": Line("x", index, Decimal("0.10")),
        }
        for index in range(count)
    ]


def test_dump_writes_the_bytes_dumps_writes(tmp_path):
    for index, document in enumerate(CORPUS[:40]):
        target = tmp_path / f"doc{index}.json"
        strata.dump(document, target)
        assert target.read_bytes() == strata.dumps(document, return_type="bytes") + b"\n"
        assert strata.load(target) == json.loads(strata.dumps(document))


def test_dump_splits_records_holding_natives_into_folders(tmp_path):
    records = _records(40)
    strata.dump(records, tmp_path / "out", split_by="region")
    loaded = strata.load(tmp_path / "out")
    by_region = {}
    for record in records:
        by_region.setdefault(record["region"], []).append(json.loads(strata.dumps(record)))
    assert loaded == [row for region in sorted(by_region) for row in by_region[region]]
    for region, rows in by_region.items():
        text = (tmp_path / "out" / f"{region}.json").read_text(encoding="utf-8")
        assert json.loads(text) == rows


def test_ndjson_lines_of_native_records_load_back(tmp_path):
    records = _records(25)
    target = tmp_path / "records.ndjson"
    target.write_text("\n".join(strata.dumps(record) for record in records) + "\n")
    expected = [json.loads(json.dumps(record, default=loads_reference)) for record in records]
    assert strata.load(target) == expected


# ---------------------------------------------------------------------------
# dumps_with_default: natives before the callable, over the corpus.
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("mode", MODES)
def test_the_callable_is_never_called_over_the_corpus(mode):
    calls = []

    def default(obj):
        calls.append(obj)

    for document in CORPUS:
        expected = strata.dumps(document, return_type=mode)
        assert strata.dumps_with_default(document, default, return_type=mode) == expected
    assert calls == []

"""`dumps_with_default` composes with dataclass-heavy ORM rows.

The rows stand in for an ORM's dataclass-mapped models (slots, frozen line
items, UUID keys, Decimal money, timezone-aware timestamps, str/int/plain
enums) without adding an ORM dependency. api.md, dumps_with_default;
docs/architecture/dumps_with_default.md, whose "What does not get a hook"
gives the file composition that stands in for a hooked `dump`.
"""

import dataclasses
import datetime as dt
import enum
import json
import uuid
from decimal import Decimal

import strata


class Status(str, enum.Enum):
    OPEN = "open"
    SHIPPED = "shipped"


class Priority(enum.IntEnum):
    LOW = 1
    HIGH = 2


class Channel(enum.Enum):
    WEB = "web"
    STORE = "store"


@dataclasses.dataclass(frozen=True, slots=True)
class OrderLine:
    sku: str
    quantity: int
    unit_price: Decimal


@dataclasses.dataclass(slots=True)
class Order:
    id: uuid.UUID
    customer_id: int
    status: Status
    priority: Priority
    channel: Channel
    placed_at: dt.datetime
    ship_by: dt.date | None
    lines: list[OrderLine] = dataclasses.field(default_factory=list)
    note: str | None = None


def _order(i: int) -> Order:
    return Order(
        id=uuid.UUID(int=10_000 + i),
        customer_id=i % 7,
        status=Status.SHIPPED if i % 2 else Status.OPEN,
        priority=Priority.HIGH if i % 5 == 0 else Priority.LOW,
        channel=Channel.WEB if i % 3 else Channel.STORE,
        placed_at=dt.datetime(2026, 9, 1, 12, tzinfo=dt.timezone.utc) + dt.timedelta(minutes=i),
        ship_by=None if i % 4 == 0 else dt.date(2026, 9, 10),
        lines=[OrderLine(f"SKU-{j}", j + 1, Decimal("9.99") * (j + 1)) for j in range(i % 6)],
        note=f"gift — {i}" if i % 11 == 0 else None,
    )


def row_default(obj):
    """Shallow: nested rows and field values inside the returned dict get their own call."""
    if dataclasses.is_dataclass(obj) and not isinstance(obj, type):
        return {f.name: getattr(obj, f.name) for f in dataclasses.fields(obj)}
    if isinstance(obj, dt.date):
        return obj.isoformat()
    if isinstance(obj, uuid.UUID | Decimal):
        return str(obj)
    if isinstance(obj, enum.Enum):
        return obj.value
    raise TypeError(f"{type(obj).__name__} is not handled")


def asdict_default(obj):
    """Deep: `dataclasses.asdict` recurses but leaves UUID, Decimal, dates and enums."""
    if dataclasses.is_dataclass(obj) and not isinstance(obj, type):
        return dataclasses.asdict(obj)
    return row_default(obj)


def test_a_fetched_page_of_rows(composes):
    rows = [_order(i) for i in range(500)]
    decoded = composes(rows, row_default)
    assert decoded[5]["lines"][2] == {"sku": "SKU-2", "quantity": 3, "unit_price": "29.97"}
    assert decoded[5]["priority"] == 2
    assert decoded[3]["channel"] == "store"
    assert decoded[0]["ship_by"] is None


def test_a_deep_asdict_default_matches_the_shallow_one(composes):
    rows = [_order(i) for i in range(200)]
    assert composes(rows, asdict_default) == composes(rows, row_default)


def test_rows_grouped_under_native_keys(composes):
    grouped: dict[str, list[Order]] = {}
    for i in range(300):
        order = _order(i)
        grouped.setdefault(str(order.customer_id), []).append(order)
    decoded = composes(grouped, row_default)
    assert sorted(decoded) == [str(c) for c in range(7)]


def test_the_file_composition_writes_the_rows_to_a_file(tmp_path):
    rows = [_order(i) for i in range(50)]
    path = tmp_path / "orders.json"
    path.write_bytes(strata.dumps_with_default(rows, row_default, return_type="bytes") + b"\n")
    expected = json.dumps(rows, default=row_default, separators=(",", ":"), ensure_ascii=False)
    assert path.read_text(encoding="utf-8") == expected + "\n"
    assert strata.load(path) == json.loads(expected)

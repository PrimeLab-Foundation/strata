"""`dumps(default=...)` composes with pydantic v2 models.

docs/context/api.md, "Unsupported-type hook (`default=`)": the callable runs
only where `dumps` would raise, once per such object; unsupported objects
nested inside a returned container are ordinary positions and get their own
call; an exception the callable raises propagates unchanged.
"""

import datetime as dt
import enum
import json
import uuid
from decimal import Decimal

import pydantic
import pydantic_core
import pytest

import strata


class Tier(enum.Enum):
    FREE = "free"
    PRO = "pro"


class Address(pydantic.BaseModel):
    street: str
    city: str
    postcode: str | None = None


class Customer(pydantic.BaseModel):
    id: uuid.UUID
    name: str
    tier: Tier
    balance: Decimal
    joined: dt.datetime
    tags: list[str] = []
    address: Address


def _customer(i: int) -> Customer:
    return Customer(
        id=uuid.UUID(int=i),
        name=f"customer {i} — ü",
        tier=Tier.PRO if i % 3 == 0 else Tier.FREE,
        balance=Decimal(i * 7) / 4,
        joined=dt.datetime(2026, 1, 1, tzinfo=dt.timezone.utc) + dt.timedelta(hours=i),
        tags=[f"t{j}" for j in range(i % 4)],
        address=Address(street=f"{i} Main St", city="Kyiv", postcode=None if i % 2 else "01001"),
    )


def json_mode(obj):
    if isinstance(obj, pydantic.BaseModel):
        return obj.model_dump(mode="json")
    raise TypeError(f"{type(obj).__name__} is not handled")


def python_mode(obj):
    """`model_dump()` leaves UUID, Decimal, datetime and Enum in the returned dict."""
    if isinstance(obj, pydantic.BaseModel):
        return obj.model_dump()
    if isinstance(obj, dt.datetime):
        return obj.isoformat()
    if isinstance(obj, uuid.UUID | Decimal):
        return str(obj)
    if isinstance(obj, enum.Enum):
        return obj.value
    raise TypeError(f"{type(obj).__name__} is not handled")


def test_a_model_serializes_as_its_json_mode_dump(composes):
    customer = _customer(3)
    decoded = composes(customer, json_mode)
    assert decoded == json.loads(customer.model_dump_json())


def test_models_nested_in_native_containers_each_get_a_call(composes):
    customers = [_customer(i) for i in range(200)]
    payload = {"page": 1, "items": customers, "by_id": {str(c.id): c for c in customers[:5]}}
    decoded = composes(payload, json_mode)
    assert decoded["items"] == [json.loads(c.model_dump_json()) for c in customers]


def test_values_inside_a_returned_dict_get_their_own_call(composes):
    decoded = composes([_customer(i) for i in range(20)], python_mode)
    assert decoded[3]["tier"] == "pro"
    assert decoded[3]["id"] == str(uuid.UUID(int=3))
    assert decoded[3]["balance"] == "5.25"


def test_to_jsonable_python_is_a_ready_made_default(composes):
    payload = {
        "customer": _customer(6),
        "seen": dt.date(2026, 9, 26),
        "ids": [uuid.UUID(int=1), uuid.UUID(int=2)],
        "ratio": Decimal("0.125"),
    }
    decoded = composes(payload, pydantic_core.to_jsonable_python)
    assert decoded["customer"] == json.loads(payload["customer"].model_dump_json())


def test_a_serialization_error_from_the_default_propagates_unchanged():
    class Opaque:
        pass

    payload = [_customer(1), Opaque()]
    with pytest.raises(pydantic_core.PydanticSerializationError) as from_json:
        json.dumps(payload, default=pydantic_core.to_jsonable_python)
    with pytest.raises(pydantic_core.PydanticSerializationError) as from_strata:
        strata.dumps(payload, default=pydantic_core.to_jsonable_python)
    assert from_strata.value.args == from_json.value.args

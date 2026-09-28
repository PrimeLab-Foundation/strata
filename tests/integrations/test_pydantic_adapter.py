"""pydantic adapter (api.md, Framework adapters).

Oracles: a model's own ``model_dump_json()`` (decoded), and stdlib ``json``
with the adapter's ``default`` byte for byte (the ``composes`` fixture).
``test_pydantic.py`` is the ``dumps_with_default`` composition suite this
adapter packages.
"""

import dataclasses
import datetime as dt
import enum
import json
import uuid
from decimal import Decimal

import pydantic
import pydantic_core
import pytest
from strata.integrations.pydantic import default, dumps

import strata


class Tier(enum.Enum):
    FREE = "free"
    PRO = "pro"


class Address(pydantic.BaseModel):
    street: str
    city: str


class Customer(pydantic.BaseModel):
    id: uuid.UUID
    name: str
    tier: Tier
    balance: Decimal
    joined: dt.datetime
    address: Address
    previous: list[Address] = []
    by_label: dict[str, Address] = {}
    contact: Address | str | None = None


class Aliased(pydantic.BaseModel):
    user_id: int = pydantic.Field(alias="userId")


class AliasedOut(pydantic.BaseModel):
    model_config = pydantic.ConfigDict(serialize_by_alias=True)
    user_id: int = pydantic.Field(alias="userId")


class Measured(pydantic.BaseModel):
    x: float


class MeasuredConstants(pydantic.BaseModel):
    model_config = pydantic.ConfigDict(ser_json_inf_nan="constants")
    x: float


class Text(pydantic.BaseModel):
    s: str


class ByNumber(pydantic.BaseModel):
    d: dict[int, str]


class Node(pydantic.BaseModel):
    child: "Node | None" = None


@dataclasses.dataclass
class Envelope:
    inner: pydantic.BaseModel


def _customer(i: int) -> Customer:
    home = Address(street=f"{i} Main St", city="Kyiv")
    return Customer(
        id=uuid.UUID(int=i),
        name=f"customer {i} — ü",
        tier=Tier.PRO if i % 3 == 0 else Tier.FREE,
        balance=Decimal(i * 7) / 4,
        joined=dt.datetime(2026, 1, 1, tzinfo=dt.timezone.utc) + dt.timedelta(hours=i),
        address=home,
        previous=[home, Address(street="Old Rd", city="Lviv")],
        by_label={"work": Address(street="Office", city="Kyiv")},
        contact=home if i % 2 else "phone",
    )


def _counted():
    calls = []

    def hook(obj):
        calls.append(type(obj))
        return default(obj)

    return hook, calls


def test_a_model_decodes_as_its_model_dump_json(composes):
    customer = _customer(3)
    assert composes(customer, default) == json.loads(customer.model_dump_json())
    assert json.loads(dumps(customer)) == json.loads(customer.model_dump_json())
    # Float-free, so separators, key order and raw non-ASCII match byte for byte.
    assert dumps(customer) == customer.model_dump_json()


def test_nested_models_are_one_call_per_top_level_model():
    hook, calls = _counted()
    customer = _customer(3)
    text = strata.dumps_with_default(customer, hook)
    assert calls == [Customer]
    assert text == dumps(customer)


def test_models_in_native_containers_get_one_call_each(composes):
    customers = [_customer(i) for i in range(20)]
    payload = {"page": 1, "items": customers, "by_id": {str(c.id): c for c in customers[:5]}}
    hook, calls = _counted()
    strata.dumps_with_default(payload, hook)
    assert calls == [Customer] * 25
    decoded = composes(payload, default)
    assert decoded["items"] == [json.loads(c.model_dump_json()) for c in customers]


def test_an_aliased_model_serializes_as_model_dump_json(composes):
    plain, by_alias = Aliased(userId=1), AliasedOut(userId=1)
    assert composes(plain, default) == json.loads(plain.model_dump_json()) == {"user_id": 1}
    assert composes(by_alias, default) == json.loads(by_alias.model_dump_json()) == {"userId": 1}


def test_a_model_inside_a_dataclass_uses_field_names(composes):
    envelope = Envelope(inner=Aliased(userId=2))
    assert composes(envelope, default) == {"inner": {"user_id": 2}}
    assert pydantic_core.to_jsonable_python(envelope) == {"inner": {"userId": 2}}


def test_a_model_inside_a_dataclass_ignores_serialize_by_alias():
    by_alias = AliasedOut(userId=1)
    assert dumps(by_alias) == '{"userId":1}'
    assert dumps(Envelope(inner=by_alias)) == '{"inner":{"user_id":1}}'


def test_a_root_model_serializes_as_model_dump_json(composes):
    root = pydantic.RootModel[list[Address]]([Address(street="s", city="c")])
    assert composes(root, default) == json.loads(root.model_dump_json())


def test_values_outside_models_are_to_jsonable_python(composes):
    payload = {
        "seen": dt.date(2026, 9, 26),
        "at": dt.datetime(2026, 9, 26, 8, 0, tzinfo=dt.timezone.utc),
        "ids": [uuid.UUID(int=1), uuid.UUID(int=2)],
        "ratio": Decimal("0.125"),
        "tier": Tier.PRO,
        "set": {3},
    }
    expected = json.loads(json.dumps(pydantic_core.to_jsonable_python(payload)))
    assert composes(payload, default) == expected


def test_nan_in_a_model_is_null_as_in_model_dump_json():
    measured = Measured(x=float("nan"))
    assert dumps(measured) == measured.model_dump_json() == '{"x":null}'


def test_nan_under_constants_is_null_where_model_dump_json_writes_nan():
    measured = MeasuredConstants(x=float("nan"))
    assert measured.model_dump_json() == '{"x":NaN}'
    assert dumps(measured) == '{"x":null}'


def test_an_int_keyed_field_writes_string_keys_as_model_dump_json():
    by_number = ByNumber(d={1: "a"})
    assert dumps(by_number) == by_number.model_dump_json() == '{"d":{"1":"a"}}'


def test_an_int_key_in_a_native_dict_is_a_type_error():
    assert pydantic_core.to_json({1: "a"}) == b'{"1":"a"}'
    with pytest.raises(TypeError, match="keys must be str, not int"):
        dumps({1: "a"})


def test_an_unknown_type_propagates_pydantics_error_unchanged():
    class Opaque:
        pass

    with pytest.raises(pydantic_core.PydanticSerializationError) as from_pydantic:
        pydantic_core.to_json(Opaque())
    with pytest.raises(pydantic_core.PydanticSerializationError) as from_strata:
        dumps([_customer(1), Opaque()])
    assert from_strata.value.args == from_pydantic.value.args


def test_a_cycle_through_models_propagates_pydantics_value_error():
    node = Node()
    node.child = node
    with pytest.raises(pydantic_core.PydanticSerializationError):
        node.model_dump_json()
    with pytest.raises(ValueError) as raised:
        dumps(node)
    assert type(raised.value) is ValueError
    assert raised.value.args == ("Circular reference detected (id repeated)",)


def test_a_cycle_through_native_containers_is_null_and_a_warning():
    cycle: list = []
    cycle.append(cycle)
    with pytest.warns(RuntimeWarning, match="Circular reference detected"):
        assert dumps([Aliased(userId=1), cycle]) == '[{"user_id":1},[null]]'


def test_a_hook_returning_a_model_hits_the_chain_bound():
    class Opaque:
        pass

    with pytest.raises(TypeError) as raised:
        strata.dumps_with_default(Opaque(), lambda obj: Aliased(userId=1))
    assert raised.value.args == (
        "default() returned an object of type Aliased that is not JSON serializable",
    )
    assert strata.dumps_with_default(Opaque(), lambda obj: default(Aliased(userId=1))) == (
        '{"user_id":1}'
    )


def test_bytes_return_type():
    customer = _customer(4)
    assert dumps(customer, return_type="bytes") == dumps(customer).encode()


def test_an_unknown_keyword_is_a_type_error():
    with pytest.raises(TypeError):
        dumps(Aliased(userId=1), by_alias=True)


def test_a_lone_surrogate_is_a_unicode_encode_error():
    text = Text(s="\ud800")
    with pytest.raises(pydantic_core.PydanticSerializationError):
        text.model_dump_json()
    with pytest.raises(UnicodeEncodeError):
        dumps(text)


def test_a_deep_native_list_with_a_model_leaf_hits_the_depth_limit():
    document: list = [Aliased(userId=1)]
    for _ in range(2999):
        document = [document]
    with pytest.raises(ValueError, match="Maximum serialization depth exceeded"):
        dumps(document)

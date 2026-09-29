"""FastAPI adapter through FastAPI's own test client (api.md, Framework adapters).

Oracle: the same app with no ``response_class`` — Starlette's ``JSONResponse``
for a route without a response model, and for a route with one pydantic's
``dump_json`` bytes path from FastAPI 0.130.0 (Starlette's ``JSONResponse``
before it). Its bytes must match the adapter's wherever neither side hits a
documented difference.
"""

import datetime as dt
import enum
import json
import uuid
from decimal import Decimal

import fastapi
import pydantic
import pytest
from fastapi import FastAPI
from fastapi.encoders import jsonable_encoder
from fastapi.responses import JSONResponse
from fastapi.testclient import TestClient
from strata.integrations.fastapi import StrataJSONResponse

import strata

# fastapi/routing.py `use_dump_json`: from 0.130.0 a response-model route with no
# `response_class` is written by pydantic's `dump_json`, which spells 1e-07 `1e-7`.
DUMP_JSON = tuple(int(part) for part in fastapi.__version__.split(".")[:2]) >= (0, 130)

CYCLE: list = []
CYCLE.append(CYCLE)

VALUES = {
    "nan": {"n": float("nan")},
    "int_key": {1: "one"},
    "surrogate": {"s": "\ud800"},
    "unsupported": {"x": object()},
    "cycle": {"c": CYCLE},
}


class Tier(enum.Enum):
    FREE = "free"
    PRO = "pro"


class Line(pydantic.BaseModel):
    sku: str
    quantity: int


class Order(pydantic.BaseModel):
    id: uuid.UUID
    placed: dt.datetime
    total: Decimal
    tier: Tier
    customer_name: str = pydantic.Field(alias="customerName")
    first: Line
    lines: list[Line]


class Measured(pydantic.BaseModel):
    ratios: list[float]


ORDER = Order(
    id=uuid.UUID(int=7),
    placed=dt.datetime(2026, 9, 28, 12, 30, tzinfo=dt.timezone.utc),
    total=Decimal("12.50"),
    tier=Tier.PRO,
    customerName="Zoë ☃",
    first=Line(sku="a-1", quantity=2),
    lines=[Line(sku="a-1", quantity=2), Line(sku="b-2", quantity=1)],
)

MEASURED = Measured(ratios=[0.1, 1e-07, 1e22, -0.0, 1.5])


def _app(response_class, documents, **app_options):
    app = FastAPI(**app_options)
    route = {} if response_class is None else {"response_class": response_class}
    direct_class = response_class or JSONResponse

    @app.get("/value/{name}", **route)
    def value(name: str):
        return documents[name]

    @app.get("/direct/{name}")
    def direct(name: str):
        return direct_class(documents[name])

    @app.get("/order", response_model=Order, **route)
    def order():
        return ORDER

    @app.get("/measured", response_model=Measured, **route)
    def measured():
        return MEASURED

    @app.post("/echo", **route)
    def echo(body: dict):
        return body

    return TestClient(app, raise_server_exceptions=False)


@pytest.fixture
def clients(json_document):
    documents = {**VALUES, "native": json_document}
    return {
        "strata": _app(StrataJSONResponse, documents),
        "default": _app(None, documents),
    }


def test_the_response_is_starlettes_json_response_class():
    assert issubclass(StrataJSONResponse, JSONResponse)
    assert StrataJSONResponse.media_type == "application/json"


def test_a_direct_response_is_starlettes_body_byte_for_byte(json_document):
    assert StrataJSONResponse(json_document).body == JSONResponse(json_document).body


def test_a_route_without_a_model_is_the_default_byte_for_byte(clients, json_document):
    got = clients["strata"].get("/value/native")
    expected = clients["default"].get("/value/native")
    assert got.status_code == expected.status_code == 200
    assert got.content == expected.content
    assert got.headers["content-type"] == expected.headers["content-type"]
    assert got.json() == json_document


def test_a_response_model_route_matches_pydantics_dump_json(clients):
    got = clients["strata"].get("/order")
    expected = clients["default"].get("/order")
    assert got.status_code == expected.status_code == 200
    assert got.content == expected.content
    assert got.json() == json.loads(ORDER.model_dump_json(by_alias=True))
    assert "customerName" in got.json()


def test_a_response_model_route_with_floats_decodes_equal(clients):
    got = clients["strata"].get("/measured")
    expected = clients["default"].get("/measured")
    assert got.status_code == expected.status_code == 200
    assert got.json() == expected.json() == MEASURED.model_dump()
    assert b"1e-07" in got.content
    assert (b"1e-7" if DUMP_JSON else b"1e-07") in expected.content


def test_default_response_class_on_the_app_applies(json_document):
    documents = {**VALUES, "native": json_document}
    client = _app(None, documents, default_response_class=StrataJSONResponse)
    response = client.get("/value/nan")
    assert response.status_code == 200
    assert response.content == b'{"n":null}'


@pytest.mark.parametrize("scope", ["route", "app"])
def test_render_runs_on_a_response_model_route(scope):
    """``response_class=`` and ``default_response_class=`` both opt out of ``dump_json``."""

    class Counting(StrataJSONResponse):
        calls = 0

        def render(self, content):
            type(self).calls += 1
            return super().render(content)

    if scope == "route":
        client = _app(Counting, VALUES)
    else:
        client = _app(None, VALUES, default_response_class=Counting)
    response = client.get("/order")
    assert response.status_code == 200
    assert Counting.calls == 1
    assert response.content == _app(None, VALUES).get("/order").content


def test_the_jsonable_encoder_recipe_serializes_unsupported_types():
    class Encoded(StrataJSONResponse):
        def render(self, content):
            return strata.dumps_with_default(content, jsonable_encoder, return_type="bytes")

    document = {
        "at": dt.datetime(2026, 9, 28, 12, 30, tzinfo=dt.timezone.utc),
        "id": uuid.UUID(int=9),
        "order": ORDER,
    }
    response = _app(Encoded, {"rich": document}).get("/direct/rich")
    assert response.status_code == 200
    assert response.content == JSONResponse(jsonable_encoder(document)).body
    assert response.json()["order"]["customerName"] == "Zoë ☃"


def test_a_malformed_request_body_is_fastapis_422_in_both(clients):
    for arm in ("default", "strata"):
        response = clients[arm].post(
            "/echo",
            content=b'{"a": ',
            headers={"content-type": "application/json"},
        )
        assert response.status_code == 422
        assert response.json()["detail"][0]["type"] == "json_invalid"


@pytest.mark.parametrize(
    "path",
    [
        "/value/surrogate",
        "/direct/surrogate",
        "/direct/unsupported",
        "/value/unsupported",
        "/value/cycle",
    ],
)
def test_failures_are_a_500_in_both(clients, path):
    for arm in ("default", "strata"):
        assert clients[arm].get(path).status_code == 500


def test_documented_differences(clients):
    strata_client, default = clients["strata"], clients["default"]
    for route in ("value", "direct"):
        assert default.get(f"/{route}/nan").status_code == 500
        assert strata_client.get(f"/{route}/nan").content == b'{"n":null}'
        assert default.get(f"/{route}/int_key").content == b'{"1":"one"}'
        assert strata_client.get(f"/{route}/int_key").status_code == 500
    assert default.get("/direct/cycle").status_code == 500
    with pytest.warns(RuntimeWarning, match="Circular reference detected"):
        cycle = strata_client.get("/direct/cycle")
    assert cycle.status_code == 200
    assert cycle.content == b'{"c":[null]}'


def test_a_cycle_under_the_error_policy_is_a_500_in_both(clients, cycle_policy_error):
    for arm in ("default", "strata"):
        assert clients[arm].get("/direct/cycle").status_code == 500


def test_a_response_past_the_recursion_limit_fails_where_stdlib_nests(
    json_document,
    deep_document,
    stdlib_nests,
):
    documents = {**VALUES, "native": json_document, "deep": deep_document}
    strata_client = _app(StrataJSONResponse, documents)
    default = _app(None, documents)
    assert strata_client.get("/direct/deep").status_code == 500
    assert default.get("/direct/deep").status_code == (200 if stdlib_nests else 500)
    for client in (strata_client, default):
        assert client.get("/value/deep").status_code == 500

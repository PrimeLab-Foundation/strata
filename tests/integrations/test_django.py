"""Django adapter through Django's own test client (api.md, Framework adapters).

Oracle: `JsonResponse` with its default `DjangoJSONEncoder`. With compact
separators and `ensure_ascii=False` in `json_dumps_params` it must produce the
adapter's bytes exactly; as shipped, the same decoded value.
"""

import datetime as dt
import decimal
import io
import json
import uuid

import django
import pytest
from django.conf import settings
from django.core.serializers.json import DjangoJSONEncoder
from django.http import JsonResponse
from django.urls import path
from django.utils.functional import lazy
from strata.integrations.django import StrataJSONEncoder

import strata

COMPACT = {"separators": (",", ":"), "ensure_ascii": False}

CYCLE: list = []
CYCLE.append(CYCLE)

RICH = {
    "zeta": 1,
    "when": dt.datetime(2026, 9, 28, 12, 30, 5, 123456, tzinfo=dt.timezone.utc),
    "day": dt.date(2026, 9, 28),
    "at": dt.time(8, 15, 0, 250000),
    "took": dt.timedelta(days=1, seconds=3.5),
    "price": decimal.Decimal("19.99"),
    "uid": uuid.UUID(int=0x1234),
    "label": lazy(lambda: "Zoë, lazily", str)(),
    "alpha": ["x", 2**70, None],
}

VALUES = {
    "nan": {"n": float("nan")},
    "int_key": {1: "one"},
    "surrogate": {"s": "\ud800"},
    "unsupported": {"x": object()},
    "aware_time": {"t": dt.time(8, 15, tzinfo=dt.timezone.utc)},
    "cycle": {"c": CYCLE},
}


def rich(request, encoder):
    if encoder == "strata":
        return JsonResponse(RICH, encoder=StrataJSONEncoder)
    params = COMPACT if encoder == "compact" else None
    return JsonResponse(RICH, json_dumps_params=params)


def value(request, encoder, name):
    if encoder == "strata":
        return JsonResponse(VALUES[name], encoder=StrataJSONEncoder)
    return JsonResponse(VALUES[name])


def echo(request):
    return JsonResponse(strata.loads(request.body), encoder=StrataJSONEncoder)


class URLConf:
    urlpatterns = [
        path("rich/<str:encoder>", rich),
        path("value/<str:encoder>/<str:name>", value),
        path("echo", echo),
    ]


if not settings.configured:
    settings.configure(
        ROOT_URLCONF=URLConf,
        ALLOWED_HOSTS=["testserver"],
        SECRET_KEY="integration-tests",
        MIDDLEWARE=[],
        INSTALLED_APPS=[],
        USE_TZ=True,
    )
    django.setup()

from django.test import Client  # noqa: E402  (needs the settings above)


@pytest.fixture
def client():
    return Client(json_encoder=StrataJSONEncoder)


def test_a_response_is_the_compact_default_byte_for_byte(client):
    got = client.get("/rich/strata")
    assert got.content == client.get("/rich/compact").content
    assert got["Content-Type"] == "application/json"
    assert json.loads(got.content) == json.loads(client.get("/rich/default").content)


def test_a_request_round_trips_through_the_test_client(client, json_document):
    response = client.post("/echo", json_document, content_type="application/json")
    assert response.status_code == 200
    assert json.loads(response.content) == json_document


def test_json_dumps_params_default_is_honoured_and_ensure_ascii_check_circular_ignored():
    response = JsonResponse(
        {"x": object(), "y": ["é", 2]},
        encoder=StrataJSONEncoder,
        json_dumps_params={
            "default": lambda o: "custom",
            "ensure_ascii": True,
            "check_circular": False,
        },
    )
    assert response.content == '{"x":"custom","y":["é",2]}'.encode()


@pytest.mark.parametrize(
    ("params", "named"),
    [
        ({"allow_nan": False}, "allow_nan"),
        ({"indent": 2}, "indent"),
        ({"sort_keys": True}, "sort_keys"),
        ({"separators": (", ", ":")}, "separators"),
        ({"skipkeys": True}, "skipkeys"),
        ({"indent": 2, "sort_keys": True}, "sort_keys, indent"),
    ],
)
def test_any_other_encoder_option_off_its_default_is_refused(params, named):
    with pytest.raises(TypeError, match=f"^StrataJSONEncoder cannot honour {named}$"):
        JsonResponse({"n": float("nan")}, encoder=StrataJSONEncoder, json_dumps_params=params)
    with pytest.raises(TypeError, match="cannot honour"):
        json.dumps({"n": 1}, cls=StrataJSONEncoder, **params)


def test_compact_separators_are_strata_s_own_form_and_accepted():
    data = {"a": [1, "é"], "b": None}
    params = {"separators": (",", ":"), "ensure_ascii": False}
    strata_response = JsonResponse(data, encoder=StrataJSONEncoder, json_dumps_params=params)
    assert strata_response.content == JsonResponse(data, json_dumps_params=params).content


def test_allow_nan_false_raises_in_stdlib_too_rather_than_passing_silently():
    with pytest.raises(ValueError, match="Out of range float values"):
        JsonResponse({"n": float("nan")}, json_dumps_params={"allow_nan": False})
    with pytest.raises(TypeError, match="allow_nan"):
        JsonResponse(
            {"n": float("nan")},
            encoder=StrataJSONEncoder,
            json_dumps_params={"allow_nan": False},
        )


def test_the_chain_bound_workaround_is_one_line_in_the_callable():
    class Money:
        amount = decimal.Decimal("1.50")

    class Returns(StrataJSONEncoder):
        def default(self, o):
            return o.amount if isinstance(o, Money) else super().default(o)

    class Loops(StrataJSONEncoder):
        def default(self, o):
            return super().default(o.amount) if isinstance(o, Money) else super().default(o)

    class StdlibReturns(DjangoJSONEncoder):
        def default(self, o):
            return o.amount if isinstance(o, Money) else super().default(o)

    assert JsonResponse({"m": Money()}, encoder=StdlibReturns).content == b'{"m": "1.50"}'
    with pytest.raises(TypeError, match="returned an object of type decimal.Decimal"):
        JsonResponse({"m": Money()}, encoder=Returns)
    assert JsonResponse({"m": Money()}, encoder=Loops).content == b'{"m":"1.50"}'


def test_a_response_past_the_recursion_limit_fails_where_stdlib_nests(
    client,
    monkeypatch,
    deep_document,
    stdlib_nests,
):
    monkeypatch.setitem(VALUES, "deep", {"d": deep_document})
    with pytest.raises(ValueError, match="Maximum serialization depth exceeded"):
        client.get("/value/strata/deep")
    if stdlib_nests:
        assert client.get("/value/default/deep").status_code == 200
    else:
        with pytest.raises(RecursionError):
            client.get("/value/default/deep")


def test_a_default_returning_an_unsupported_object_hits_chain_bound_1():
    class Marker:
        pass

    def default(obj):
        return "done" if isinstance(obj, Marker) else Marker()

    params = {"default": default}
    assert JsonResponse({"x": object()}, json_dumps_params=params).content == b'{"x": "done"}'
    with pytest.raises(TypeError, match="default\\(\\) returned an object of type Marker"):
        JsonResponse({"x": object()}, encoder=StrataJSONEncoder, json_dumps_params=params)


def test_json_dump_to_a_file_takes_the_same_path():
    written = io.StringIO()
    json.dump(RICH, written, cls=StrataJSONEncoder)
    assert written.getvalue() == json.dumps(RICH, cls=StrataJSONEncoder)
    assert written.getvalue() == strata.dumps_with_default(
        RICH, StrataJSONEncoder().default, native=False
    )


@pytest.mark.parametrize(
    ("name", "error"), [("unsupported", TypeError), ("aware_time", ValueError)]
)
def test_an_error_from_djangos_default_propagates_unchanged(client, name, error):
    with pytest.raises(error) as from_default:
        client.get(f"/value/default/{name}")
    with pytest.raises(error) as from_strata:
        client.get(f"/value/strata/{name}")
    assert from_strata.value.args == from_default.value.args


def test_documented_differences(client):
    assert client.get("/value/default/nan").content == b'{"n": NaN}'
    assert client.get("/value/strata/nan").content == b'{"n":null}'
    assert client.get("/value/default/int_key").content == b'{"1": "one"}'
    with pytest.raises(TypeError, match="keys must be str, not int"):
        client.get("/value/strata/int_key")
    assert client.get("/value/default/surrogate").content == b'{"s": "\\ud800"}'
    with pytest.raises(UnicodeEncodeError):
        client.get("/value/strata/surrogate")
    with pytest.raises(ValueError, match="Circular reference detected"):
        client.get("/value/default/cycle")
    with pytest.warns(RuntimeWarning, match="Circular reference detected"):
        assert client.get("/value/strata/cycle").content == b'{"c":[null]}'


def test_a_cycle_under_the_error_policy_is_djangos_value_error(client, cycle_policy_error):
    with pytest.raises(ValueError, match="Circular reference detected"):
        client.get("/value/strata/cycle")

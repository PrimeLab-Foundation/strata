"""Flask adapter through Flask's own test client (api.md, Framework adapters).

Oracle: Flask's `DefaultJSONProvider`. With `sort_keys` and `ensure_ascii`
off it must produce the adapter's bytes exactly; as shipped, the same decoded
value. The differences are the rows of the adapter's table.
"""

import dataclasses
import datetime as dt
import decimal
import json
import uuid

import pytest
from flask import Flask, jsonify, render_template_string, request, session
from flask.json.provider import DefaultJSONProvider
from markupsafe import Markup
from strata.integrations.flask import StrataJSONProvider


@dataclasses.dataclass
class Point:
    x: int
    y: float


class CompactDefault(DefaultJSONProvider):
    sort_keys = False
    ensure_ascii = False


RICH = {
    "zeta": 1,
    "when": dt.datetime(2026, 9, 28, 12, 30, 5, tzinfo=dt.timezone.utc),
    "day": dt.date(2026, 9, 28),
    "price": decimal.Decimal("19.99"),
    "uid": uuid.UUID(int=0x1234),
    "point": Point(1, 2.5),
    "html": Markup("<b>Zoë</b>"),
    "alpha": ["x", 2**70, None],
}

CYCLE: list = []
CYCLE.append(CYCLE)

VALUES = {
    "nan": float("nan"),
    "int_key": {1: "one"},
    "surrogate": "\ud800",
    "unsupported": {"x": object()},
    "cycle": CYCLE,
}


def _client(provider):
    app = Flask(__name__)
    app.json = provider(app)
    app.config.update(TESTING=True, SECRET_KEY="integration-tests")

    @app.get("/rich")
    def rich():
        return jsonify(RICH)

    @app.get("/value/<name>")
    def value(name):
        return jsonify(VALUES[name])

    @app.post("/echo")
    def echo():
        return request.get_json()

    @app.post("/session")
    def remember():
        session["kept"] = (1, b"\x00bytes", "Zoë", uuid.UUID(int=5))
        return "ok"

    @app.post("/tojson")
    def tojson():
        return render_template_string("{{ data|tojson }}", data=request.get_json())

    return app.test_client()


@pytest.fixture
def strata_client():
    return _client(StrataJSONProvider)


def test_a_response_is_the_compact_default_byte_for_byte(strata_client):
    got = strata_client.get("/rich")
    expected = _client(CompactDefault).get("/rich")
    assert got.data == expected.data
    assert got.data.endswith(b"\n")
    assert got.mimetype == expected.mimetype == "application/json"
    assert json.loads(got.data) == json.loads(_client(DefaultJSONProvider).get("/rich").data)


def test_a_request_round_trips_through_the_test_client(strata_client, json_document):
    response = strata_client.post("/echo", json=json_document)
    assert response.status_code == 200
    assert response.get_json() == json_document
    assert json.loads(response.data) == json_document


def test_debug_responses_stay_compact():
    strata_client, default = _client(StrataJSONProvider), _client(DefaultJSONProvider)
    strata_client.application.debug = default.application.debug = True
    assert b"\n  " in default.get("/rich").data
    assert strata_client.get("/rich").data == _client(CompactDefault).get("/rich").data


def test_the_session_cookie_round_trips(strata_client):
    assert strata_client.post("/session").status_code == 200
    with strata_client.session_transaction() as saved:
        assert saved["kept"] == (1, b"\x00bytes", "Zoë", uuid.UUID(int=5))


def test_tojson_ignores_sort_keys_and_stays_valid_json(strata_client, json_document):
    rendered = strata_client.post("/tojson", json=json_document).get_data(as_text=True)
    assert json.loads(rendered) == json_document
    assert rendered.index('"zeta"') < rendered.index('"id"')


def test_an_unsupported_type_raises_flasks_own_type_error(strata_client):
    with pytest.raises(TypeError) as from_default:
        _client(DefaultJSONProvider).get("/value/unsupported")
    with pytest.raises(TypeError) as from_strata:
        strata_client.get("/value/unsupported")
    assert from_strata.value.args == from_default.value.args
    assert from_strata.value.args == ("Object of type object is not JSON serializable",)


def test_malformed_request_json_is_a_400_in_both(strata_client):
    for client in (strata_client, _client(DefaultJSONProvider)):
        response = client.post("/echo", data=b'{"a": ', content_type="application/json")
        assert response.status_code == 400


def test_documented_differences(strata_client):
    default = _client(DefaultJSONProvider)
    assert strata_client.get("/value/nan").data == b"null\n"
    assert default.get("/value/nan").data == b"NaN\n"
    assert default.get("/value/int_key").data == b'{"1":"one"}\n'
    with pytest.raises(TypeError, match="keys must be str, not int"):
        strata_client.get("/value/int_key")
    assert default.get("/value/surrogate").data == b'"\\ud800"\n'
    with pytest.raises(UnicodeEncodeError):
        strata_client.get("/value/surrogate")
    for body in (b"[NaN]", b"\xef\xbb\xbf[1]", "[1]".encode("utf-16")):
        assert default.post("/echo", data=body, content_type="application/json").status_code == 200
        response = strata_client.post("/echo", data=body, content_type="application/json")
        assert response.status_code == 400
    duplicate = strata_client.post("/echo", data=b'{"a":1,"a":2}', content_type="application/json")
    assert duplicate.get_json() == {"a": 1}
    with pytest.raises(ValueError, match="Circular reference detected"):
        default.get("/value/cycle")
    with pytest.warns(RuntimeWarning, match="Circular reference detected"):
        assert strata_client.get("/value/cycle").data == b"[null]\n"


def test_a_dumps_default_keyword_is_honoured_with_chain_bound_1(strata_client):
    class Marker:
        pass

    def default(obj):
        return "done" if isinstance(obj, Marker) else Marker()

    stdlib = DefaultJSONProvider(strata_client.application)
    assert stdlib.dumps([object()], default=default) == '["done"]'
    assert (
        strata_client.application.json.dumps([object()], default=lambda o: "custom") == '["custom"]'
    )
    with pytest.raises(TypeError, match="default\\(\\) returned an object of type Marker"):
        strata_client.application.json.dumps([object()], default=default)


def test_the_chain_bound_workaround_is_one_line_in_the_callable():
    class Money:
        amount = decimal.Decimal("1.50")

    class Returns(StrataJSONProvider):
        @staticmethod
        def default(o):
            return o.amount if isinstance(o, Money) else DefaultJSONProvider.default(o)

    class Loops(StrataJSONProvider):
        @staticmethod
        def default(o):
            if isinstance(o, Money):
                return DefaultJSONProvider.default(o.amount)
            return DefaultJSONProvider.default(o)

    class StdlibReturns(DefaultJSONProvider):
        default = staticmethod(Returns.default)

    app = Flask(__name__)
    assert StdlibReturns(app).dumps([Money()]) == '["1.50"]'
    with pytest.raises(TypeError, match="returned an object of type decimal.Decimal"):
        Returns(app).dumps([Money()])
    assert Loops(app).dumps([Money()]) == '["1.50"]'


def test_flasks_own_keywords_are_ignored_and_any_other_is_refused(strata_client):
    provider = strata_client.application.json
    flask_passes = {"separators": (", ", ": "), "indent": 2, "sort_keys": True}
    assert provider.dumps({"b": 1, "a": [2]}, **flask_passes) == '{"b":1,"a":[2]}'
    for refused in ({"allow_nan": False}, {"ensure_ascii": True}, {"skipkeys": True}):
        with pytest.raises(TypeError, match=f"unexpected keyword argument '{next(iter(refused))}'"):
            provider.dumps([float("nan")], **refused)
    with pytest.raises(TypeError, match="default must be callable, not NoneType"):
        provider.dumps([1], default=None)


def test_nesting_past_either_cap_fails_where_stdlib_nests(
    strata_client,
    monkeypatch,
    deep_document,
    deep_request,
    stdlib_nests,
):
    monkeypatch.setitem(VALUES, "deep", deep_document)
    default = _client(DefaultJSONProvider)
    with pytest.raises(ValueError, match="Maximum serialization depth exceeded"):
        strata_client.get("/value/deep")
    request = {"data": deep_request, "content_type": "application/json"}
    assert strata_client.post("/echo", **request).status_code == 400
    if stdlib_nests:
        assert default.get("/value/deep").status_code == 200
        assert default.post("/echo", **request).status_code == 200
    else:
        with pytest.raises(RecursionError):
            default.get("/value/deep")
        with pytest.raises(RecursionError):
            default.post("/echo", **request)


def test_a_cycle_under_the_error_policy_is_flasks_value_error(strata_client, cycle_policy_error):
    with pytest.raises(ValueError, match="Circular reference detected"):
        strata_client.get("/value/cycle")

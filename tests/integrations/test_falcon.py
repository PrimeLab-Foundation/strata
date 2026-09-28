"""Falcon adapter through Falcon's own test client (api.md, Framework adapters).

Oracle: Falcon's default `JSONHandler`. A handler whose `json.dumps` also has
compact separators must produce the adapter's bytes exactly; as shipped, the
same decoded value.
"""

import functools
import json

import falcon
import falcon.media
import falcon.testing
import pytest
from strata.integrations.falcon import json_handler

CYCLE: list = []
CYCLE.append(CYCLE)

VALUES = {
    "nan": {"n": float("nan")},
    "int_key": {1: "one"},
    "surrogate": {"s": "\ud800"},
    "unsupported": {"x": object()},
    "cycle": {"c": CYCLE},
}


class Values:
    def __init__(self, documents):
        self.documents = documents

    def on_get(self, req, resp, name):
        resp.media = self.documents[name]


class Echo:
    def on_post(self, req, resp):
        resp.media = req.get_media()


def _client(handler, native_document):
    app = falcon.App()
    if handler is not None:
        app.req_options.media_handlers[falcon.MEDIA_JSON] = handler
        app.resp_options.media_handlers[falcon.MEDIA_JSON] = handler
    app.add_route("/value/{name}", Values({**VALUES, "native": native_document}))
    app.add_route("/echo", Echo())
    return falcon.testing.TestClient(app)


@pytest.fixture
def clients(native_document):
    compact = falcon.media.JSONHandler(
        dumps=functools.partial(json.dumps, ensure_ascii=False, separators=(",", ":")),
    )
    return {
        "strata": _client(json_handler(), native_document),
        "compact": _client(compact, native_document),
        "default": _client(None, native_document),
    }


def test_the_handler_is_falcons_own_class():
    assert type(json_handler()) is falcon.media.JSONHandler


def test_a_response_is_the_compact_default_byte_for_byte(clients, native_document):
    got = clients["strata"].simulate_get("/value/native")
    assert got.status_code == 200
    assert got.content == clients["compact"].simulate_get("/value/native").content
    assert got.headers["content-type"] == falcon.MEDIA_JSON
    assert json.loads(got.content) == clients["default"].simulate_get("/value/native").json
    assert got.json == native_document


def test_a_request_round_trips_through_the_test_client(clients, native_document):
    response = clients["strata"].simulate_post("/echo", json=native_document)
    assert response.status_code == 200
    assert response.json == native_document


@pytest.mark.parametrize(("name", "status"), [("unsupported", 500), ("surrogate", 500)])
def test_serialization_failures_are_the_same_status_in_both(clients, name, status):
    for arm in ("default", "strata"):
        assert clients[arm].simulate_get(f"/value/{name}").status_code == status


@pytest.mark.parametrize("body", [b'{"a": ', b""])
def test_malformed_or_missing_request_json_is_a_400_in_both(clients, body):
    for arm in ("default", "strata"):
        response = clients[arm].simulate_post("/echo", body=body, content_type=falcon.MEDIA_JSON)
        assert response.status_code == 400


def test_documented_differences(clients):
    strata_client, default = clients["strata"], clients["default"]
    assert default.simulate_get("/value/nan").content == b'{"n": NaN}'
    assert strata_client.simulate_get("/value/nan").content == b'{"n":null}'
    assert default.simulate_get("/value/int_key").content == b'{"1": "one"}'
    assert strata_client.simulate_get("/value/int_key").status_code == 500
    nan_request = {"body": b"[NaN]", "content_type": falcon.MEDIA_JSON}
    assert default.simulate_post("/echo", **nan_request).status_code == 200
    assert strata_client.simulate_post("/echo", **nan_request).status_code == 400
    duplicate = strata_client.simulate_post(
        "/echo",
        body=b'{"a":1,"a":2}',
        content_type=falcon.MEDIA_JSON,
    )
    assert duplicate.json == {"a": 1}
    assert default.simulate_get("/value/cycle").status_code == 500
    with pytest.warns(RuntimeWarning, match="Circular reference detected"):
        assert strata_client.simulate_get("/value/cycle").content == b'{"c":[null]}'


def test_a_cycle_under_the_error_policy_is_a_500_as_in_falcon(clients, cycle_policy_error):
    assert clients["strata"].simulate_get("/value/cycle").status_code == 500


def test_nesting_past_either_cap_fails_where_stdlib_nests(
    native_document,
    monkeypatch,
    deep_document,
    deep_request,
    stdlib_nests,
):
    monkeypatch.setitem(VALUES, "deep", deep_document)
    strata_client = _client(json_handler(), native_document)
    default = _client(None, native_document)
    request = {"body": deep_request, "content_type": falcon.MEDIA_JSON}
    assert strata_client.simulate_get("/value/deep").status_code == 500
    assert strata_client.simulate_post("/echo", **request).status_code == 400
    stdlib_status = 200 if stdlib_nests else 500
    assert default.simulate_get("/value/deep").status_code == stdlib_status
    assert default.simulate_post("/echo", **request).status_code == stdlib_status

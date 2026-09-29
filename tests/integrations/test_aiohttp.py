"""aiohttp adapter through aiohttp's own test client (api.md, Framework adapters).

Oracle: aiohttp's default `json.dumps` / `json.loads`. `json.dumps` with
compact separators and `ensure_ascii=False` must produce the adapter's bytes
exactly; as shipped, the same decoded value.
"""

import asyncio
import functools
import json

import pytest
from aiohttp import web
from aiohttp.test_utils import TestClient, TestServer
from strata.integrations import aiohttp as adapter

COMPACT = functools.partial(json.dumps, separators=(",", ":"), ensure_ascii=False)

CYCLE: list = []
CYCLE.append(CYCLE)

VALUES = {
    "nan": {"n": float("nan")},
    "int_key": {1: "one"},
    "surrogate": {"s": "\ud800"},
    "unsupported": {"x": object()},
    "cycle": {"c": CYCLE},
}


def _app(json_document):
    documents = {**VALUES, "native": json_document}

    async def strata_value(request):
        return adapter.json_response(documents[request.match_info["name"]])

    async def default_value(request):
        return web.json_response(documents[request.match_info["name"]])

    async def compact_value(request):
        return web.json_response(documents[request.match_info["name"]], dumps=COMPACT)

    async def strata_echo(request):
        return adapter.json_response(await request.json(loads=adapter.loads))

    async def default_echo(request):
        return web.json_response(await request.json())

    app = web.Application()
    app.router.add_get("/strata/{name}", strata_value)
    app.router.add_get("/default/{name}", default_value)
    app.router.add_get("/compact/{name}", compact_value)
    app.router.add_post("/strata/echo", strata_echo)
    app.router.add_post("/default/echo", default_echo)
    return app


def _exchange(json_document, requests):
    """Run `(method, path, body)` requests; return `(status, body bytes)` for each."""

    async def run():
        server = TestServer(_app(json_document))
        async with TestClient(server, json_serialize=adapter.dumps) as client:
            results = []
            for method, target, body in requests:
                if body is None:
                    response = await client.request(method, target)
                else:
                    response = await client.request(
                        method,
                        target,
                        data=body,
                        headers={"Content-Type": "application/json"},
                    )
                results.append((response.status, await response.read()))
            return results

    return asyncio.run(run())


def test_a_response_is_the_compact_default_byte_for_byte(json_document):
    (got, compact, default) = _exchange(
        json_document,
        [("GET", f"/{arm}/native", None) for arm in ("strata", "compact", "default")],
    )
    assert got == compact
    assert got[0] == 200
    assert json.loads(got[1]) == json.loads(default[1]) == json_document


def test_a_request_round_trips_through_the_test_client(json_document):
    async def run():
        async with TestClient(
            TestServer(_app(json_document)), json_serialize=adapter.dumps
        ) as client:
            response = await client.post("/strata/echo", json=json_document)
            return response.status, await response.json(loads=adapter.loads)

    assert asyncio.run(run()) == (200, json_document)


def test_an_unsupported_type_is_a_500_in_both(json_document):
    results = _exchange(
        json_document,
        [("GET", "/default/unsupported", None), ("GET", "/strata/unsupported", None)],
    )
    assert [status for status, _ in results] == [500, 500]


def test_malformed_request_json_is_a_500_in_both(json_document):
    results = _exchange(
        json_document,
        [("POST", f"/{arm}/echo", b'{"a": ') for arm in ("default", "strata")],
    )
    assert [status for status, _ in results] == [500, 500]


def test_documented_differences(json_document):
    results = _exchange(
        json_document,
        [
            ("GET", "/default/nan", None),
            ("GET", "/strata/nan", None),
            ("GET", "/default/int_key", None),
            ("GET", "/strata/int_key", None),
            ("GET", "/default/surrogate", None),
            ("GET", "/strata/surrogate", None),
            ("GET", "/default/cycle", None),
            ("POST", "/default/echo", b"[NaN]"),
            ("POST", "/strata/echo", b"[NaN]"),
            ("POST", "/strata/echo", b'{"a":1,"a":2}'),
        ],
    )
    assert results == [
        (200, b'{"n": NaN}'),
        (200, b'{"n":null}'),
        (200, b'{"1": "one"}'),
        (500, results[3][1]),
        (200, b'{"s": "\\ud800"}'),
        (500, results[5][1]),
        (500, results[6][1]),
        (200, b"[NaN]"),
        (500, results[8][1]),
        (200, b'{"a":1}'),
    ]
    with pytest.warns(RuntimeWarning, match="Circular reference detected"):
        assert _exchange(json_document, [("GET", "/strata/cycle", None)]) == [
            (200, b'{"c":[null]}'),
        ]


def test_a_cycle_under_the_error_policy_is_a_500_as_in_aiohttp(json_document, cycle_policy_error):
    assert _exchange(json_document, [("GET", "/strata/cycle", None)])[0][0] == 500


def test_nesting_past_either_cap_fails_where_stdlib_nests(
    json_document,
    monkeypatch,
    deep_document,
    deep_request,
    stdlib_nests,
):
    monkeypatch.setitem(VALUES, "deep", deep_document)
    results = _exchange(
        json_document,
        [
            ("GET", "/strata/deep", None),
            ("POST", "/strata/echo", deep_request),
            ("GET", "/default/deep", None),
            ("POST", "/default/echo", deep_request),
        ],
    )
    stdlib_status = 200 if stdlib_nests else 500
    assert [status for status, _ in results] == [500, 500, stdlib_status, stdlib_status]

"""FastAPI adapter: ``StrataJSONResponse``, a ``JSONResponse`` rendered by strata.

    @app.get("/items", response_class=StrataJSONResponse)  # one route
    app = FastAPI(default_response_class=StrataJSONResponse)  # or APIRouter(...)
    return StrataJSONResponse(data)  # or returned directly

``default_response_class=`` also takes the app's response-model routes off
pydantic's ``dump_json`` path (FastAPI 0.130+), exactly as ``response_class=``
does for one route (below).

The class is Starlette's ``JSONResponse`` with ``render`` replaced, so it works
in a plain Starlette app too. Only responses change: FastAPI parses request
bodies with Starlette's ``Request.json()`` (``json.loads``) and has no decoder
hook, and it maps only ``json.JSONDecodeError`` to its 422 ``json_invalid``
error, so strata's ``ValueError`` would become a 400 there; requests are left
to FastAPI.

Starlette's ``JSONResponse`` has no hook for unsupported types, so neither does
this one. A caller who wants one subclasses it::

    class Response(StrataJSONResponse):
        def render(self, content):
            return strata.dumps_with_default(content, jsonable_encoder, return_type="bytes")

with ``fastapi.encoders.jsonable_encoder``, which returns JSON-native values, so
the chain bound never applies.

From FastAPI 0.130.0 a route with a response model and no ``response_class``
is serialized by pydantic's ``dump_json`` straight to bytes
(``fastapi/routing.py``, the ``use_dump_json`` test); any ``response_class``,
this one included, opts the route out of that path, to a
``field.serialize(mode="json", by_alias=True)`` dump rendered by strata. That
dump alone costs more than ``dump_json`` does, so leave response-model routes
without ``response_class`` there; this class pays where ``render`` is the
serializer: a returned ``StrataJSONResponse(data)`` and routes without a
response model (docs/architecture/framework_adapters.md).

Semantic differences against Starlette's ``JSONResponse``:

=========================  ==============================  =================================
Case                       Starlette default               StrataJSONResponse
=========================  ==============================  =================================
NaN, +-Inf                 ``ValueError`` (500)            ``null``
Non-``str`` dict key       coerced to ``str``              ``TypeError`` (500)
Cycle                      ``ValueError`` (500)            ``null`` + ``RuntimeWarning``
                                                           (``cycle_policy="warn"``)
Response deeper than the   written (3.12+; 3.10-3.11:      ``ValueError`` (500)
recursion limit (1000)     ``RecursionError``, 500)
=========================  ==============================  =================================

A non-``str`` key survives to ``render`` where ``jsonable_encoder`` or a
returned response leaves one; a response model coerces keys itself. The cycle
and depth rows hold for a returned response only: a route's return value
without a response model passes through ``jsonable_encoder`` first, which fails
on either in both (500). Separators and key order are the same in both, and
non-ASCII text is raw UTF-8 in both (Starlette passes ``ensure_ascii=False``);
a lone surrogate, an unsupported type and a cycle under ``cycle_policy="error"``
fail in both (500).
"""

from __future__ import annotations

from typing import Any

from fastapi.responses import JSONResponse

import strata

__all__ = ["StrataJSONResponse"]


class StrataJSONResponse(JSONResponse):
    """Starlette's ``JSONResponse`` rendered by ``strata.dumps``; everything else is inherited."""

    def render(self, content: Any) -> bytes:
        return strata.dumps(content, return_type="bytes")

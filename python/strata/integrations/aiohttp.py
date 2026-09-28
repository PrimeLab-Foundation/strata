"""aiohttp adapter: ``json_response(data)``, or ``dumps``/``loads`` for aiohttp's own hooks.

``dumps`` fits every ``JSONEncoder`` slot aiohttp has (``web.json_response(
dumps=)``, ``ClientSession(json_serialize=)``, ``WebSocketResponse.send_json(
dumps=)``) and ``loads`` every ``JSONDecoder`` slot (``await request.json(
loads=)``, ``await response.json(loads=)``, ``receive_json(loads=)``).
aiohttp's default is plain ``json.dumps``, with no hook for unsupported types,
so neither is ``dumps``; a caller who wants one passes
``functools.partial(strata.dumps_with_default, default=f)`` instead.

Semantic differences against aiohttp's default ``json.dumps`` / ``json.loads``:

=========================  ==============================  =================================
Case                       aiohttp default                 strata
=========================  ==============================  =================================
Separators                 ``", "`` and ``": "``           compact
Non-ASCII                  ``\\uXXXX`` escapes             raw UTF-8
NaN, +-Inf                 ``NaN``, ``Infinity``           ``null``
Non-``str`` dict key       coerced to ``str``              ``TypeError``
Lone surrogate             ``"\\ud800"`` escape            ``UnicodeEncodeError``
Cycle                      ``ValueError``                  ``null`` + ``RuntimeWarning``
                                                           (``cycle_policy="warn"``)
Response deeper than the   written (3.12+; 3.10-3.11:      ``ValueError`` (500)
recursion limit (1000)     ``RecursionError``, 500)
Request nested past 1024   parsed (3.12+; 3.10-3.11:       ``ValueError`` (500)
                           ``RecursionError``, 500)
Request ``NaN`` token      accepted                        ``ValueError`` (500)
Duplicate request keys     last wins                       first wins (``duplicate_key_policy``)
=========================  ==============================  =================================
"""

from __future__ import annotations

from typing import Any

from aiohttp import web

from strata import dumps, loads

__all__ = ["dumps", "json_response", "loads"]


def json_response(*args: Any, **kwargs: Any) -> web.Response:
    """``aiohttp.web.json_response`` with ``dumps=strata.dumps``; same arguments otherwise."""
    return web.json_response(*args, dumps=dumps, **kwargs)

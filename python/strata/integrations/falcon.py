"""Falcon adapter: ``json_handler()`` for ``media_handlers[falcon.MEDIA_JSON]``.

    handler = json_handler()
    app.req_options.media_handlers[falcon.MEDIA_JSON] = handler
    app.resp_options.media_handlers[falcon.MEDIA_JSON] = handler

The handler is a plain ``falcon.media.JSONHandler``, not a subclass, so Falcon
keeps its optimized media path for it; ``dumps`` returns ``bytes``, which the
handler writes as they are. A ``ValueError`` from ``loads`` becomes Falcon's
``MediaMalformedError`` (400), as ``json.loads``'s does. Falcon's default
``json.dumps`` has no hook for unsupported types, so neither does this one; a
caller who wants one builds ``JSONHandler(dumps=functools.partial(
strata.dumps_with_default, default=f, return_type="bytes"), loads=strata.loads)``.

Semantic differences against Falcon's default ``JSONHandler``:

=========================  ==============================  =================================
Case                       Falcon default                  json_handler()
=========================  ==============================  =================================
Separators                 ``", "`` and ``": "``           compact
NaN, +-Inf                 ``NaN``, ``Infinity``           ``null``
Non-``str`` dict key       coerced to ``str``              ``TypeError`` (500)
Cycle                      ``ValueError`` (500)            ``null`` + ``RuntimeWarning``
                                                           (``cycle_policy="warn"``)
Response deeper than the   written (3.12+; 3.10-3.11:      ``ValueError`` (500)
recursion limit (1000)     ``RecursionError``, 500)
Request nested past 1024   parsed (3.12+; 3.10-3.11:       400 (``MediaMalformedError``)
                           ``RecursionError``, 500)
Request ``NaN`` token      accepted                        400 (``MediaMalformedError``)
Duplicate request keys     last wins                       first wins (``duplicate_key_policy``)
=========================  ==============================  =================================

Non-ASCII text is raw UTF-8 in both (Falcon passes ``ensure_ascii=False``),
and a lone surrogate fails in both (500).
"""

from __future__ import annotations

from functools import partial

import falcon.media

import strata

__all__ = ["json_handler"]


def json_handler() -> falcon.media.JSONHandler:
    """A ``falcon.media.JSONHandler`` serializing with ``strata.dumps`` and parsing with ``strata.loads``."""
    return falcon.media.JSONHandler(
        dumps=partial(strata.dumps, return_type="bytes"),
        loads=strata.loads,
    )

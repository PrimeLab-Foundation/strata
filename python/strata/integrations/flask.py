"""Flask adapter: ``app.json = StrataJSONProvider(app)``.

Or, before the app exists, ``class App(Flask): json_provider_class =
StrataJSONProvider``. Every Flask JSON path then runs on strata: ``jsonify``
and returned dicts, ``request.get_json``, the test client's ``json=``, the
session cookie serializer and Jinja's ``tojson``. Unsupported types go to
Flask's own ``DefaultJSONProvider.default`` (dates, ``Decimal``, ``UUID``,
dataclasses, ``__html__``) through ``strata.dumps_with_default``.

Semantic differences against Flask's ``DefaultJSONProvider``:

=========================  ==============================  =================================
Case                       Flask default                   StrataJSONProvider
=========================  ==============================  =================================
Key order                  sorted (``sort_keys = True``)   insertion order
Non-ASCII                  ``\\uXXXX`` escapes             raw UTF-8
Debug / ``compact=False``  indented responses              compact; ``indent`` ignored
``dumps`` keywords         passed to ``json.dumps``        ``separators``, ``indent``,
                                                           ``sort_keys`` ignored;
                                                           ``default`` honoured; any other
                                                           (``allow_nan``, ...) ``TypeError``
``dumps(x, default=None)`` no hook                         ``TypeError`` on every call
                                                           (``default`` must be callable)
NaN, +-Inf                 ``NaN``, ``Infinity``           ``null``
Non-``str`` dict key       coerced to ``str``              ``TypeError``
Lone surrogate             ``"\\ud800"`` escape            ``UnicodeEncodeError``
Cycle                      ``ValueError``                  ``null`` + ``RuntimeWarning``
                                                           (``cycle_policy="warn"``)
``default`` returns an     ``default`` called again        ``TypeError`` (chain bound 1)
unsupported object
Response deeper than the   written (3.12+; 3.10-3.11:      ``ValueError`` ("Maximum
recursion limit (1000)     ``RecursionError``)             serialization depth exceeded")
Request nested past 1024   parsed (3.12+; 3.10-3.11:       400 Bad Request
                           ``RecursionError``)
Request ``NaN`` token      accepted                        400 Bad Request
Duplicate request keys     last wins                       first wins (``duplicate_key_policy``)
Request bytes              UTF-8/16/32, UTF-8 BOM          UTF-8 only; else 400
``loads`` keywords         passed to ``json.loads``        ``TypeError``
=========================  ==============================  =================================

Flask itself passes ``separators`` (session cookies), ``indent`` (debug
responses) and ``sort_keys`` (``tojson``) to ``dumps``; refusing them would
break those callers, so those three are ignored and every other keyword is
refused (docs/decisions.md, 2026-09-28).

Chain bound 1: a ``default`` that returns another unsupported object is a
``TypeError`` here, where stdlib calls ``default`` again. A subclass whose
``default`` returns ``o.amount`` (a ``Decimal``) writes ``"1.50"`` under
Flask's provider and raises here; the loop belongs inside the callable::

    return DefaultJSONProvider.default(o.amount)  # not: return o.amount
"""

from __future__ import annotations

from typing import Any

from flask.json.provider import DefaultJSONProvider

import strata

__all__ = ["StrataJSONProvider"]

_OWN_DEFAULT: Any = object()


class StrataJSONProvider(DefaultJSONProvider):
    """Flask's default provider with strata as the serializer and parser.

    ``response`` is inherited unchanged (mimetype, trailing newline). The
    three attributes below state what the output is; setting them has no
    effect.
    """

    ensure_ascii = False
    sort_keys = False
    compact = True

    def dumps(  # type: ignore[override]
        self,
        obj: Any,
        *,
        default: Any = _OWN_DEFAULT,
        separators: Any = None,
        indent: Any = None,
        sort_keys: Any = None,
    ) -> str:
        """Serialize ``obj``; ``separators``, ``indent`` and ``sort_keys`` are Flask's own and ignored."""
        return strata.dumps_with_default(obj, self.default if default is _OWN_DEFAULT else default)

    def loads(self, s: str | bytes) -> Any:  # type: ignore[override]
        return strata.loads(s)

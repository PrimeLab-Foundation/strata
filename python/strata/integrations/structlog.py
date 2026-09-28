"""structlog adapter: ``JSONRenderer(serializer=dumps)``.

``JSONRenderer`` calls ``serializer(event_dict, **dumps_kw)`` with
``dumps_kw`` defaulting ``default`` to structlog's own fallback handler
(``__structlog__``, else ``repr``), so ``strata.dumps_with_default`` fits the
slot as it is: ``dumps`` is that function. ``JSONRenderer(serializer=dumps,
return_type="bytes")`` renders ``bytes`` for a ``BytesLogger``. This module
imports nothing from structlog.

Semantic differences against ``JSONRenderer``'s default ``json.dumps``:

=========================  ==============================  =================================
Case                       structlog default               dumps
=========================  ==============================  =================================
Separators                 ``", "`` and ``": "``           compact
Non-ASCII                  ``\\uXXXX`` escapes             raw UTF-8
``JSONRenderer`` keywords  passed to ``json.dumps``        ``TypeError`` at the first log
(``sort_keys``, ...)                                       call, except ``default`` and
                                                           ``return_type``
``default=None``           no fallback: ``TypeError``      ``TypeError`` on every call
                           per unsupported value           (``default`` must be callable)
``__structlog__`` returns  ``default`` called again        ``TypeError`` (chain bound 1)
an unsupported object      (``repr``)
NaN, +-Inf                 ``NaN``, ``Infinity``           ``null``
Non-``str`` dict key       coerced to ``str``              ``TypeError``
Lone surrogate             ``"\\ud800"`` escape            ``UnicodeEncodeError``
Cycle                      ``ValueError``                  ``null`` + ``RuntimeWarning``
                                                           (``cycle_policy="warn"``)
Event deeper than the      rendered (3.12+; 3.10-3.11:     ``ValueError`` ("Maximum
recursion limit (1000)     ``RecursionError``)             serialization depth exceeded")
=========================  ==============================  =================================

Chain bound 1: an object whose ``__structlog__`` returns ``self.amount`` (a
``Decimal``) renders ``"Decimal('1.50')"`` under the default renderer, which
passes the ``Decimal`` to the fallback handler again, and raises ``TypeError``
here; the loop belongs inside the callable::

    return str(self.amount)  # not: return self.amount

structlog forwards only what the caller gave ``JSONRenderer``, so a keyword
strata cannot honour is refused rather than ignored (docs/decisions.md,
2026-09-28).
"""

from strata import dumps_with_default as dumps

__all__ = ["dumps"]

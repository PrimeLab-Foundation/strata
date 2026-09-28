"""Django adapter: ``JsonResponse(data, encoder=StrataJSONEncoder)``.

``JsonResponse`` calls ``json.dumps(data, cls=encoder, **json_dumps_params)``;
this encoder's ``encode`` hands the whole document to
``strata.dumps_with_default`` with the encoder's ``default`` — Django's own
``DjangoJSONEncoder.default`` (datetimes, dates, times, timedeltas,
``Decimal``, ``UUID``, lazy strings), or a ``default`` passed in
``json_dumps_params``. The test client's ``Client(json_encoder=...)`` and
``json.dump(obj, fp, cls=StrataJSONEncoder)`` take the same path.

Semantic differences against ``JsonResponse``'s default ``DjangoJSONEncoder``:

=========================  ==============================  =================================
Case                       Django default                  StrataJSONEncoder
=========================  ==============================  =================================
Separators                 ``", "`` and ``": "``           compact
Non-ASCII                  ``\\uXXXX`` escapes             raw UTF-8
``json_dumps_params``      passed to ``json.dumps``        ``default`` honoured;
                                                           ``ensure_ascii``,
                                                           ``check_circular`` ignored;
                                                           ``separators=(",", ":")``
                                                           accepted (strata's own form);
                                                           any other option off its
                                                           default (``allow_nan=False``,
                                                           ``indent``, ``sort_keys``,
                                                           other ``separators``,
                                                           ``skipkeys``) ``TypeError``
NaN, +-Inf                 ``NaN``, ``Infinity``           ``null``
Non-``str`` dict key       coerced to ``str``              ``TypeError``
Lone surrogate             ``"\\ud800"`` escape            ``UnicodeEncodeError``
Cycle                      ``ValueError``                  ``null`` + ``RuntimeWarning``
                                                           (``cycle_policy="warn"``)
``default`` returns an     ``default`` called again        ``TypeError`` (chain bound 1)
unsupported object
Response deeper than the   written (3.12+; 3.10-3.11:      ``ValueError`` ("Maximum
recursion limit (1000)     ``RecursionError``)             serialization depth exceeded")
=========================  ==============================  =================================

Chain bound 1: a subclass whose ``default`` returns ``o.amount`` (a
``Decimal``) writes ``"1.50"`` under ``DjangoJSONEncoder`` and raises
``TypeError`` here; the loop belongs inside the callable::

    return super().default(o.amount)  # not: return o.amount
"""

from __future__ import annotations

from collections.abc import Iterator
from typing import Any

from django.core.serializers.json import DjangoJSONEncoder

import strata

__all__ = ["StrataJSONEncoder"]

_REFUSED_UNLESS = {"skipkeys": False, "allow_nan": True, "sort_keys": False, "indent": None}
_ACCEPTED_SEPARATORS = {(", ", ": "), (",", ":")}


class StrataJSONEncoder(DjangoJSONEncoder):
    """``DjangoJSONEncoder`` with strata as the serializer."""

    def encode(self, o: Any) -> str:
        refused = [name for name, value in _REFUSED_UNLESS.items() if getattr(self, name) != value]
        separators = (self.item_separator, self.key_separator)
        if self.indent is None and separators not in _ACCEPTED_SEPARATORS:
            refused.append("separators")
        if refused:
            raise TypeError(f"StrataJSONEncoder cannot honour {', '.join(refused)}")
        return strata.dumps_with_default(o, self.default)

    def iterencode(self, o: Any, _one_shot: bool = False) -> Iterator[str]:
        return iter((self.encode(o),))

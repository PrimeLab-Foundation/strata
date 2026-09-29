"""pydantic adapter: ``dumps(obj)`` and the ``default`` it passes to ``dumps_with_default``.

``default`` serializes a model by its own ``model_dump(mode="json")`` and any
other unsupported object by ``pydantic_core.to_jsonable_python(obj,
by_alias=False)``. For a model, ``dumps`` decodes equal to the model's own
``model_dump_json()`` (the bytes can differ in how a float is spelled), and for
any document free of NaN/+-Inf, non-``str`` keys and lone surrogates it is
byte-for-byte ``json.dumps(obj, default=default, separators=(",", ":"),
ensure_ascii=False)``. That holds for strata's native types too: ``dumps``
passes ``native=False``, so a ``datetime``, ``Decimal``, ``UUID``, dataclass or
set outside a model reaches ``default`` and is written as pydantic writes it
(a UTC ``datetime`` ending ``Z``, a ``Decimal`` as a string), never natively.

A model's fields — nested models, ``list[Model]``, ``dict[str, Model]``,
unions, at any depth — are serialized by pydantic inside that one
``model_dump`` call, so strata receives a JSON-native dict and the chain bound
never applies. A model inside a native container is an ordinary position with
its own call. A model inside a non-model object (a stdlib dataclass) is
serialized by ``to_jsonable_python`` with ``by_alias=False``, which always
writes field names, even where the model's config sets ``serialize_by_alias``
(reached directly, such a model writes its aliases).

FastAPI serializes response models by alias; this adapter serializes as
``model_dump_json`` does: field names, unless the model's config sets
``serialize_by_alias``.

Chain bound 1 applies only to a caller's own hook that returns a model:
``TypeError("default() returned an object of type X that is not JSON
serializable")``. The model belongs inside the callable's own conversion::

    return default(Model(...))  # not: return Model(...)

Errors propagate unchanged: ``PydanticSerializationError`` for a type pydantic
cannot serialize, pydantic's ``ValueError("Circular reference detected (id
repeated)")`` for a cycle through models; a cycle through native containers is
strata's ``null`` + ``RuntimeWarning`` (``cycle_policy="warn"``).

Semantic differences against ``model_dump_json()``:

================================  ==============================  ==========================
Case                              ``model_dump_json()``           dumps
================================  ==============================  ==========================
NaN, +-Inf under                  ``NaN``, ``Infinity``           ``null``
``ser_json_inf_nan="constants"``
Lone surrogate in a ``str``       ``PydanticSerializationError``  ``UnicodeEncodeError``
Cycle through models              ``PydanticSerializationError``  ``ValueError``
                                  (wraps the ``ValueError``)
Non-``str`` key in a native       coerced to ``str`` (as          ``TypeError``
dict                              ``pydantic_core.to_json``)
================================  ==============================  ==========================

NaN under the default ``ser_json_inf_nan="null"`` is ``null`` in both, a
``dict[int, ...]`` field's keys are ``"1"`` in both, and separators, key order
and raw UTF-8 non-ASCII text are the same in both.
"""

from __future__ import annotations

from typing import Any

from pydantic import BaseModel
from pydantic_core import to_jsonable_python

import strata

__all__ = ["default", "dumps"]


def default(obj: Any) -> Any:
    """``model_dump(mode="json")`` for a model, else ``to_jsonable_python(obj, by_alias=False)``."""
    if isinstance(obj, BaseModel):
        return obj.model_dump(mode="json")
    return to_jsonable_python(obj, by_alias=False)


def dumps(obj: Any, *, return_type: str = "str") -> str | bytes:
    """``strata.dumps_with_default(obj, default, return_type=return_type, native=False)``."""
    return strata.dumps_with_default(obj, default, return_type=return_type, native=False)

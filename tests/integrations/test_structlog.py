"""structlog adapter through structlog's own testing loggers (api.md, Framework adapters).

Oracle: `JSONRenderer`'s default `json.dumps`. With compact separators and
`ensure_ascii=False` in its keywords it must render the adapter's text
exactly; as shipped, the same decoded value.
"""

import decimal
import json

import pytest
import structlog
from strata.integrations.structlog import dumps
from structlog.processors import JSONRenderer
from structlog.testing import ReturnLogger

CYCLE: list = []
CYCLE.append(CYCLE)


class Custom:
    def __structlog__(self):
        return {"custom": ["via", "__structlog__"]}


class Opaque:
    def __repr__(self):
        return "<Opaque>"


class Chained:
    def __structlog__(self):
        return Opaque()


def _render(renderer, event="hello", **fields):
    return structlog.wrap_logger(ReturnLogger(), processors=[renderer]).info(event, **fields)


STRATA = JSONRenderer(serializer=dumps)
COMPACT = JSONRenderer(separators=(",", ":"), ensure_ascii=False)
DEFAULT = JSONRenderer()


def test_a_rendered_event_is_the_compact_default_byte_for_byte(native_document):
    fields = {**native_document, "custom": Custom(), "opaque": Opaque()}
    got = _render(STRATA, **fields)
    assert got == _render(COMPACT, **fields)
    assert json.loads(got) == json.loads(_render(DEFAULT, **fields))
    assert json.loads(got)["opaque"] == "<Opaque>"
    assert json.loads(got)["custom"] == {"custom": ["via", "__structlog__"]}


def test_a_rendered_event_round_trips(native_document):
    assert json.loads(_render(STRATA, **native_document)) == {**native_document, "event": "hello"}


def test_return_type_bytes_renders_bytes_for_a_bytes_logger(native_document):
    rendered = _render(JSONRenderer(serializer=dumps, return_type="bytes"), **native_document)
    assert rendered == _render(STRATA, **native_document).encode()


def test_a_keyword_strata_cannot_honour_is_refused_at_the_first_log_call():
    renderer = JSONRenderer(serializer=dumps, sort_keys=True)
    with pytest.raises(TypeError, match="unexpected keyword argument 'sort_keys'"):
        _render(renderer)
    with pytest.raises(TypeError, match="default must be callable, not NoneType"):
        _render(JSONRenderer(serializer=dumps, default=None))


def test_documented_differences():
    assert _render(DEFAULT, n=float("nan")) == '{"n": NaN, "event": "hello"}'
    assert _render(STRATA, n=float("nan")) == '{"n":null,"event":"hello"}'
    assert _render(DEFAULT, data={1: "one"}) == '{"data": {"1": "one"}, "event": "hello"}'
    with pytest.raises(TypeError, match="keys must be str, not int"):
        _render(STRATA, data={1: "one"})
    assert _render(DEFAULT, s="\ud800") == '{"s": "\\ud800", "event": "hello"}'
    with pytest.raises(UnicodeEncodeError):
        _render(STRATA, s="\ud800")
    assert json.loads(_render(DEFAULT, chained=Chained()))["chained"] == "<Opaque>"
    with pytest.raises(TypeError, match="default\\(\\) returned an object of type Opaque"):
        _render(STRATA, chained=Chained())
    with pytest.raises(ValueError, match="Circular reference detected"):
        _render(DEFAULT, c=CYCLE)
    with pytest.warns(RuntimeWarning, match="Circular reference detected"):
        assert _render(STRATA, c=CYCLE) == '{"c":[null],"event":"hello"}'


def test_a_cycle_under_the_error_policy_is_structlogs_value_error(cycle_policy_error):
    with pytest.raises(ValueError, match="Circular reference detected"):
        _render(STRATA, c=CYCLE)


def test_an_event_past_the_recursion_limit_fails_where_stdlib_nests(deep_document, stdlib_nests):
    with pytest.raises(ValueError, match="Maximum serialization depth exceeded"):
        _render(STRATA, d=deep_document)
    if stdlib_nests:
        assert _render(DEFAULT, d=deep_document).startswith('{"d": [[[')
    else:
        with pytest.raises(RecursionError):
            _render(DEFAULT, d=deep_document)


def test_the_chain_bound_workaround_is_one_line_in_the_callable():
    class Returns:
        amount = decimal.Decimal("1.50")

        def __structlog__(self):
            return self.amount

    class Loops(Returns):
        def __structlog__(self):
            return str(self.amount)

    assert _render(DEFAULT, m=Returns()) == '{"m": "Decimal(\'1.50\')", "event": "hello"}'
    with pytest.raises(TypeError, match="returned an object of type decimal.Decimal"):
        _render(STRATA, m=Returns())
    assert _render(STRATA, m=Loops()) == '{"m":"1.50","event":"hello"}'

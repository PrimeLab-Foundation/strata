"""`dumps(default=...)` composes with attrs classes.

docs/context/api.md, "Unsupported-type hook (`default=`)": a shallow
`attrs.asdict` returns a dict whose nested instances are ordinary positions
and get their own call; a deep one returns a JSON-native tree in one call.
"""

import datetime as dt
import json

import attrs
import pytest

import strata


@attrs.frozen
class Point:
    x: float
    y: float


@attrs.define
class Segment:
    start: Point
    end: Point
    label: str = ""


@attrs.define
class Survey:
    id: int
    taken: dt.date
    segments: list[Segment] = attrs.field(factory=list)
    notes: dict[str, str] = attrs.field(factory=dict)


def _survey(i: int) -> Survey:
    return Survey(
        id=i,
        taken=dt.date(2026, 9, 1) + dt.timedelta(days=i),
        segments=[
            Segment(Point(j * 0.5, -j / 3), Point(j + 0.25, 1e-7 * j), label=f"s{j}")
            for j in range(i % 5)
        ],
        notes={"surveyor": f"crew {i % 3}"} if i % 2 else {},
    )


def shallow(obj):
    if attrs.has(type(obj)):
        return attrs.asdict(obj, recurse=False)
    if isinstance(obj, dt.date):
        return obj.isoformat()
    raise TypeError(f"{type(obj).__name__} is not handled")


def _iso_dates(_inst, _field, value):
    return value.isoformat() if isinstance(value, dt.date) else value


def deep(obj):
    if attrs.has(type(obj)):
        return attrs.asdict(obj, value_serializer=_iso_dates)
    raise TypeError(f"{type(obj).__name__} is not handled")


def test_a_frozen_instance_serializes_as_its_fields(composes):
    assert composes(Point(1.5, -2.0), shallow) == {"x": 1.5, "y": -2.0}


def test_nested_instances_inside_a_shallow_dict_get_their_own_call(composes):
    surveys = [_survey(i) for i in range(100)]
    decoded = composes(surveys, shallow)
    assert decoded[4]["taken"] == "2026-09-05"
    assert decoded[4]["segments"][1]["start"] == {"x": 0.5, "y": -1 / 3}


def test_a_deep_asdict_default_matches_the_shallow_one(composes):
    surveys = [_survey(i) for i in range(100)]
    assert composes(surveys, deep) == composes(surveys, shallow)


def test_an_error_from_the_default_propagates_unchanged():
    class Opaque:
        pass

    payload = {"survey": _survey(2), "attachment": Opaque()}
    with pytest.raises(TypeError) as from_json:
        json.dumps(payload, default=shallow)
    with pytest.raises(TypeError) as from_strata:
        strata.dumps(payload, default=shallow)
    assert from_strata.value.args == from_json.value.args == ("Opaque is not handled",)

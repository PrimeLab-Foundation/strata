"""Integration tests for the `default=` hook: files, folders, NDJSON, oracles.

The contract mirrors live in tests/unit/test_dumps_default_hook.py; this suite
exercises the hook through the entry points that compose with it and against
stdlib `json` as the oracle (docs/architecture/dumps_default_hook.md, "Test
placement").
"""

from __future__ import annotations

import gc
import json
import random
import sys
from datetime import date, datetime
from decimal import Decimal
from enum import Enum
from pathlib import Path
from uuid import UUID

import pytest

import strata


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y


class Colour(Enum):
    RED = "red"
    BLUE = "blue"


def encode(value):
    """The kind of hook the feature exists for -- the six common types."""
    if isinstance(value, (datetime, date)):
        return value.isoformat()
    if isinstance(value, UUID):
        return str(value)
    if isinstance(value, Decimal):
        return float(value)
    if isinstance(value, Enum):
        return value.value
    if isinstance(value, Point):
        return {"x": value.x, "y": value.y}
    if isinstance(value, set):
        return sorted(value)
    raise TypeError(f"unhandled {type(value).__name__}")


REAL_TREE = {
    "when": datetime(2026, 9, 18, 14, 11, 0),
    "day": date(2026, 9, 18),
    "id": UUID("12345678-1234-5678-1234-567812345678"),
    "amount": Decimal("12.50"),
    "colour": Colour.RED,
    "where": Point(1, 2),
    "tags": {"b", "a"},
    "plain": [1, 2.5, "three", None, True],
}


def test_dumps_with_a_real_hook_matches_stdlib():
    assert json.loads(strata.dumps(REAL_TREE, default=encode)) == json.loads(
        json.dumps(REAL_TREE, default=encode)
    )


def test_dumps_bytes_mode_with_a_real_hook_matches_str_mode():
    assert strata.dumps(REAL_TREE, default=encode, return_type="bytes") == strata.dumps(
        REAL_TREE, default=encode
    ).encode("utf-8")


def test_dump_file_round_trip(tmp_path: Path):
    target = tmp_path / "records.json"
    strata.dump([REAL_TREE, REAL_TREE], str(target), default=encode)
    assert strata.load(str(target)) == json.loads(target.read_text(encoding="utf-8"))
    assert target.read_bytes().endswith(b"\n")


def test_dump_file_hook_error_leaves_no_file(tmp_path: Path):
    target = tmp_path / "never.json"

    def hook(_value):
        raise RuntimeError("refused")

    with pytest.raises(RuntimeError, match="refused"):
        strata.dump({"p": Point(1, 2)}, str(target), default=hook)
    assert not target.exists()


def test_folder_dump_round_trip(tmp_path: Path):
    folder = tmp_path / "grouped"
    records = [
        {"group": "a", "at": datetime(2026, 1, 1), "n": 1},
        {"group": "b", "at": datetime(2026, 1, 2), "n": 2},
        {"group": "a", "at": datetime(2026, 1, 3), "n": 3},
    ]
    strata.dump(records, str(folder), split_by="group", default=encode)
    # api.md's round-trip law: grouped in bytewise key-path order, intra-group
    # order preserved -- so a.json's two records come back before b.json's.
    expected = json.loads(json.dumps(records, default=encode))
    assert strata.load(str(folder)) == [expected[0], expected[2], expected[1]]
    assert sorted(p.name for p in folder.iterdir()) == ["a.json", "b.json"]


def test_folder_dump_hook_error_leaves_earlier_groups_written(tmp_path: Path):
    """api.md, File & folder I/O: folder mode writes one file per group, in
    order, so a raise on a later group leaves the earlier files on disk.

    This is folder mode's pre-existing behavior for every error it can raise —
    the hook does not introduce it. Pinned so the file-mode guarantee is never
    mistaken for a folder-mode one (docs/architecture/dumps_default_hook.md,
    error table row 3).
    """
    folder = tmp_path / "partial"
    records = [{"group": "a", "n": 1}, {"group": "b", "bad": Point(1, 2)}]

    def hook(_value):
        raise RuntimeError("refused")

    with pytest.raises(RuntimeError, match="refused"):
        strata.dump(records, str(folder), split_by="group", default=hook)
    # "a" is bytewise before "b", so the first group completed.
    assert (folder / "a.json").is_file()
    assert not (folder / "b.json").exists()


def test_folder_dump_nested_keys_with_a_hook(tmp_path: Path):
    folder = tmp_path / "nested"
    records = [{"g1": "x", "g2": "y", "when": date(2026, 2, 3)}]
    strata.dump(records, str(folder), split_by=["g1", "g2"], default=encode)
    written = folder / "x" / "y.json"
    assert json.loads(written.read_text(encoding="utf-8")) == [
        {"g1": "x", "g2": "y", "when": "2026-02-03"}
    ]


def test_ndjson_round_trip_through_a_hook(tmp_path: Path):
    target = tmp_path / "records.ndjson"
    records = [{"i": i, "at": datetime(2026, 1, 1 + i)} for i in range(5)]
    target.write_text(
        "\n".join(strata.dumps(record, default=encode) for record in records) + "\n",
        encoding="utf-8",
    )
    assert strata.load(str(target)) == json.loads(json.dumps(records, default=encode))


def test_hook_calling_dumps_re_entrantly(tmp_path: Path):
    """The nested lease made ordinary (record, SchemaCacheLease)."""

    def hook(value):
        if isinstance(value, Point):
            # A nested `dumps` of a record wide enough to be remembered by the
            # private (fallback) schema state.
            return json.loads(strata.dumps({"x": value.x, "y": value.y, "k": [1, 2, 3]}))
        raise TypeError(type(value).__name__)

    target = tmp_path / "reentrant.json"
    strata.dump([{"p": Point(i, i)} for i in range(20)], str(target), default=hook)
    assert strata.load(str(target))[3] == {"p": {"x": 3, "y": 3, "k": [1, 2, 3]}}


def test_re_entrant_hook_reads_zero_refcount_drift():
    """Criterion 2, through the file-free path, over many iterations."""
    point = Point(1, 2)
    payload = {"p": point}

    def hook(value):
        return json.loads(strata.dumps({"x": value.x, "y": value.y}))

    for _ in range(3):
        strata.dumps(payload, default=hook)
    gc.collect()
    before = sys.getrefcount(point)
    for _ in range(200):
        strata.dumps(payload, default=hook)
    gc.collect()
    assert sys.getrefcount(point) - before == 0


def test_hook_that_resizes_the_list_being_written():
    """Criterion 3's mutation shapes, on an unsanitized build too."""
    live = [Point(0, 0), 1, 2, 3, 4, 5]

    def shrink(_value):
        del live[3:]
        return "cut"

    assert json.loads(strata.dumps(live, default=shrink)) == ["cut", 1, 2]

    grown = [Point(0, 0)]
    added = []

    def grow(_value):
        if not added:
            added.append(True)
            grown.extend(["one", "two"])
        return "start"

    assert json.loads(strata.dumps(grown, default=grow)) == ["start", "one", "two"]


def test_hook_that_clears_the_dict_being_written():
    live = {"p": Point(0, 0), "b": 2, "c": 3}

    def hook(_value):
        live.clear()
        return "gone"

    parsed = json.loads(strata.dumps(live, default=hook))
    assert parsed["p"] == "gone"


def test_hook_that_clears_a_wide_dict_being_written():
    """Past the fused writer's 24-key bound the dict is followed live."""
    live = {f"k{i}": i for i in range(30)}
    live["target"] = Point(0, 0)

    def hook(_value):
        live.clear()
        return "gone"

    parsed = json.loads(strata.dumps(live, default=hook))
    assert parsed["target"] == "gone"


def _random_tree(rng, depth=0):
    choice = rng.randrange(8 if depth < 3 else 5)
    if choice == 0:
        return rng.randint(-(10**12), 10**12)
    if choice == 1:
        return rng.random() * 1000
    if choice == 2:
        return rng.choice(["", "a", "é中", 'q"t', "back\\slash"])
    if choice == 3:
        return rng.choice([True, False, None])
    if choice == 4:
        return Point(rng.randint(0, 9), rng.randint(0, 9))
    if choice == 5:
        return [_random_tree(rng, depth + 1) for _ in range(rng.randrange(4))]
    if choice == 6:
        return {f"k{i}": _random_tree(rng, depth + 1) for i in range(rng.randrange(6))}
    return datetime(2026, 1, 1 + rng.randrange(28))


@pytest.mark.parametrize("seed", range(20))
def test_generated_corpus_matches_the_stdlib_oracle(seed):
    """The oracle clause: strata and stdlib agree for JSON-safe hooks."""
    rng = random.Random(seed)
    tree = [_random_tree(rng) for _ in range(30)]
    assert json.loads(strata.dumps(tree, default=encode)) == json.loads(
        json.dumps(tree, default=encode)
    )


@pytest.mark.parametrize("seed", range(5))
def test_generated_corpus_default_none_is_byte_identical(seed):
    """`default=None` changes nothing on a tree that needs no hook."""
    rng = random.Random(1000 + seed)
    tree = [json.loads(json.dumps(_random_tree(rng), default=encode)) for _ in range(30)]
    assert strata.dumps(tree, default=None) == strata.dumps(tree)

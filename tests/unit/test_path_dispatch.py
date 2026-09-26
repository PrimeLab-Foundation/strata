"""Contract tests for how `load`, `dump` and `search` tell a file from a directory.

docs/context/api.md § File & folder I/O: "`path` is a file" is file mode and a
directory is folder mode. File mode opens the path first and asks what it is
only when the open says it was not a file (E26-P27: the stat that used to come
first is 30-45 us per call on Windows). These tests pin that the *answers* did
not move -- every outcome a directory had before, it still has, whatever its
name looks like and whatever else is wrong with the call -- and that a file
call never pays for the question.
"""

import json
import os

import pytest

import strata

RECORDS = [{"k": "a", "n": 1}, {"k": "b", "n": 2}]


@pytest.fixture
def named_like_a_file(tmp_path):
    """A directory whose own name carries a JSON suffix, holding one document."""

    def make(name):
        directory = tmp_path / name
        directory.mkdir()
        (directory / "inner.json").write_text(json.dumps(RECORDS))
        return directory

    return make


# ---------------------------------------------------------------------------
# load: "`load(dirpath)` returns one list: each file's records concatenated"
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("name", ["plain", "data.json", "data.ndjson", "data.JSONL"])
def test_a_directory_loads_in_folder_mode_whatever_its_name(named_like_a_file, name):
    assert strata.load(named_like_a_file(name)) == RECORDS


@pytest.mark.parametrize("name", ["plain", "data.json", "data.ndjson"])
def test_a_directory_streams_in_folder_mode_whatever_its_name(named_like_a_file, name):
    assert list(strata.load(named_like_a_file(name), iterator=True)) == RECORDS


@pytest.mark.parametrize("name", ["plain", "data.json", "data.ndjson"])
def test_cursor_on_a_directory_names_the_directory(named_like_a_file, name):
    # "`return_type="cursor"` -> `ValueError`" for a directory -- the
    # directory's refusal, not the NDJSON file's, even for `data.ndjson/`.
    with pytest.raises(ValueError, match="not supported for a directory"):
        strata.load(named_like_a_file(name), return_type="cursor")


@pytest.mark.parametrize("name", ["plain", "data.json", "data.ndjson"])
def test_a_bad_return_type_on_a_directory_is_still_rejected(named_like_a_file, name):
    with pytest.raises(ValueError, match="invalid return_type"):
        strata.load(named_like_a_file(name), return_type="tuple")


def test_a_file_refusal_is_raised_before_the_file_is_touched(tmp_path):
    # "`return_type="cursor"` on NDJSON is a `ValueError`" -- decided from the
    # name, so it outranks the missing file, as it always has.
    with pytest.raises(ValueError, match="not supported for NDJSON"):
        strata.load(tmp_path / "missing.ndjson", return_type="cursor")
    with pytest.raises(ValueError, match="invalid return_type"):
        strata.load(tmp_path / "missing.json", return_type="tuple")


def test_a_missing_path_is_file_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        strata.load(tmp_path / "missing.json")
    with pytest.raises(FileNotFoundError):
        strata.load(tmp_path / "missing")


def test_a_file_still_loads_as_a_file(tmp_path):
    path = tmp_path / "doc.json"
    path.write_text(json.dumps(RECORDS))
    assert strata.load(path) == RECORDS
    lines = tmp_path / "doc.ndjson"
    lines.write_text("".join(json.dumps(r) + "\n" for r in RECORDS))
    assert strata.load(lines) == RECORDS


# ---------------------------------------------------------------------------
# dump: "A directory target without `split_by` -> `ValueError`."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("name", ["plain", "out.json"])
def test_a_directory_target_is_rejected_and_left_alone(named_like_a_file, name):
    directory = named_like_a_file(name)
    before = sorted(p.name for p in directory.iterdir())
    with pytest.raises(ValueError, match="a directory target requires split_by"):
        strata.dump(RECORDS, directory)
    assert sorted(p.name for p in directory.iterdir()) == before
    assert json.loads((directory / "inner.json").read_text()) == RECORDS


def test_a_directory_target_outranks_an_unserializable_value(tmp_path):
    # The directory is the error whatever else is wrong with the call: the
    # value is no longer looked at *after* the path, but the answer is the one
    # the path-first order gave.
    with pytest.raises(ValueError, match="a directory target requires split_by"):
        strata.dump({"k": object()}, tmp_path)
    with pytest.raises(ValueError, match="a directory target requires split_by"):
        strata.dump({1: "non-str key"}, tmp_path)


def test_an_unserializable_value_does_not_touch_an_existing_file(tmp_path):
    # Serialize-before-truncate: the destination keeps its bytes.
    path = tmp_path / "keep.json"
    path.write_text("[1]\n")
    with pytest.raises(TypeError):
        strata.dump({"k": object()}, path)
    assert path.read_text() == "[1]\n"


def test_an_unwritable_target_is_an_os_error(tmp_path):
    with pytest.raises(OSError):
        strata.dump(RECORDS, tmp_path / "no-such-directory" / "out.json")


def test_split_by_with_a_file_target_is_still_rejected(tmp_path):
    path = tmp_path / "out.json"
    path.write_text("[]\n")
    with pytest.raises(ValueError, match="split_by requires a directory target"):
        strata.dump(RECORDS, path, split_by="k")
    assert path.read_text() == "[]\n"


def test_a_file_target_is_written_as_a_file(tmp_path):
    path = tmp_path / "out.json"
    strata.dump(RECORDS, path)
    assert path.read_bytes() == json.dumps(RECORDS, separators=(",", ":")).encode() + b"\n"


# ---------------------------------------------------------------------------
# search: "`search` operates on a file or a directory."
# ---------------------------------------------------------------------------


@pytest.mark.parametrize("name", ["plain", "data.json", "data.ndjson"])
@pytest.mark.parametrize("expression", ["$[*].n", "$[?(@.n > 1)].k"])
def test_a_directory_is_searched_in_folder_mode_whatever_its_name(
    named_like_a_file, name, expression
):
    # One streamable path and one that falls back to a full parse: both file
    # legs have to hand a directory over to folder mode.
    directory = named_like_a_file(name)
    expected = strata.search(directory / "inner.json", expression)
    assert strata.search(directory, expression) == expected
    assert list(strata.search(directory, expression, iterator=True)) == expected


def test_a_bad_expression_is_rejected_for_a_directory_named_like_a_file(named_like_a_file):
    with pytest.raises(ValueError, match="Invalid JSONPath expression"):
        strata.search(named_like_a_file("data.json"), "$[")


def test_search_keeps_the_extension_rule_for_files(tmp_path):
    path = tmp_path / "doc.txt"
    path.write_text(json.dumps(RECORDS))
    with pytest.raises(TypeError, match="expects a .json, .ndjson or .jsonl path"):
        strata.search(path, "$[*].n")
    with pytest.raises(FileNotFoundError):
        strata.search(tmp_path / "missing.json", "$[*].n")


# ---------------------------------------------------------------------------
# The point of the change: a file call asks the filesystem nothing extra.
# ---------------------------------------------------------------------------


def test_the_facade_does_not_stat_a_path_that_carries_a_json_suffix(tmp_path, monkeypatch):
    path = tmp_path / "doc.json"
    path.write_text(json.dumps(RECORDS))
    asked = []
    real = os.path.isdir
    monkeypatch.setattr(os.path, "isdir", lambda p: asked.append(p) or real(p))
    assert strata.search(path, "$[*].n") == [1, 2]
    assert asked == []
    with pytest.raises(TypeError):
        strata.search(tmp_path / "doc.txt", "$[*].n")
    assert len(asked) == 1

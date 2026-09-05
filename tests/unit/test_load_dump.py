"""Contract tests for `strata.load` and `strata.dump` in file mode.

Each test pins a clause of docs/context/api.md § File & folder I/O. Folder mode
is a later milestone; what is pinned here is the single-file behaviour and the
invalid-line policy that the whole NDJSON contract rests on.
"""

import json
import os
import stat

import pytest

import strata

RECORDS = '{"i": 1}\n{"i": 2}\n{"i": 3}\n'
BROKEN = '{"i": 1}\n{oops}\n{"i": 3}\nnot json\n{"i": 5}\n'


@pytest.fixture
def files(tmp_path):
    (tmp_path / "doc.json").write_text('{"a": [1, 2], "b": "x"}', encoding="utf-8")
    (tmp_path / "records.ndjson").write_text(RECORDS, encoding="utf-8")
    (tmp_path / "records.jsonl").write_text(RECORDS, encoding="utf-8")
    (tmp_path / "broken.ndjson").write_text(BROKEN, encoding="utf-8")
    (tmp_path / "empty.json").write_text("", encoding="utf-8")
    return tmp_path


# ---------------------------------------------------------------------------
# "`load` dispatches on extension: `.ndjson`/`.jsonl` -> NDJSON list of
#  records; anything else -> single JSON document"
# ---------------------------------------------------------------------------


def test_a_json_file_loads_as_one_document(files):
    assert strata.load(files / "doc.json") == {"a": [1, 2], "b": "x"}


@pytest.mark.parametrize("name", ["records.ndjson", "records.jsonl"])
def test_line_delimited_extensions_load_as_a_list_of_records(files, name):
    assert strata.load(files / name) == [{"i": 1}, {"i": 2}, {"i": 3}]


def test_the_extension_test_is_case_insensitive(tmp_path):
    path = tmp_path / "RECORDS.NDJSON"
    path.write_text(RECORDS, encoding="utf-8")
    assert strata.load(path) == [{"i": 1}, {"i": 2}, {"i": 3}]


def test_an_unknown_extension_is_read_as_one_document(tmp_path):
    path = tmp_path / "data.txt"
    path.write_text('{"a": 1}', encoding="utf-8")
    assert strata.load(path) == {"a": 1}


def test_a_path_object_is_accepted(files):
    assert strata.load(files / "doc.json") == strata.load(str(files / "doc.json"))


# ---------------------------------------------------------------------------
# "**Invalid NDJSON lines raise `ValueError` unless `skip_errors=True`** --
#  uniform across eager, iterator, and folder modes (the previous
#  implementation silently skipped, do not reproduce)"
# ---------------------------------------------------------------------------


def test_a_malformed_line_raises_by_default(files):
    with pytest.raises(ValueError, match="Invalid JSON"):
        strata.load(files / "broken.ndjson")


def test_a_malformed_line_raises_in_iterator_mode_too(files):
    records = strata.load(files / "broken.ndjson", iterator=True)
    assert next(records) == {"i": 1}  # the good line arrives first...
    with pytest.raises(ValueError, match="Invalid JSON"):
        next(records)  # ...and the bad one raises where it is reached


def test_skip_errors_drops_the_bad_lines_eagerly(files):
    assert strata.load(files / "broken.ndjson", skip_errors=True) == [{"i": 1}, {"i": 3}, {"i": 5}]


def test_skip_errors_drops_the_bad_lines_lazily(files):
    records = strata.load(files / "broken.ndjson", iterator=True, skip_errors=True)
    assert list(records) == [{"i": 1}, {"i": 3}, {"i": 5}]


def test_the_error_names_the_failing_line(files):
    with pytest.raises(ValueError, match="line 2"):
        strata.load(files / "broken.ndjson")


# ---------------------------------------------------------------------------
# "`iterator=True` parses lazily line-by-line"; "`return_type="cursor"` on
#  NDJSON is a `ValueError`"
# ---------------------------------------------------------------------------


def test_iterator_mode_yields_the_same_records(files):
    assert list(strata.load(files / "records.ndjson", iterator=True)) == strata.load(
        files / "records.ndjson",
    )


def test_the_ndjson_iterator_is_an_iterator(files):
    records = strata.load(files / "records.ndjson", iterator=True)
    assert iter(records) is records
    assert next(records) == {"i": 1}


def test_cursor_mode_on_ndjson_is_rejected(files):
    with pytest.raises(ValueError, match="cursor"):
        strata.load(files / "records.ndjson", return_type="cursor")


def test_cursor_mode_on_json_returns_a_cursor(files):
    cursor = strata.load(files / "doc.json", return_type="cursor")
    assert isinstance(cursor, strata.JsonCursor)
    assert cursor.field("b").get_str() == "x"


def test_iterator_mode_on_a_json_document(files):
    assert list(strata.load(files / "doc.json", iterator=True)) == [("a", [1, 2]), ("b", "x")]


# ---------------------------------------------------------------------------
# "Raises `FileNotFoundError`, `OSError`, `ValueError` ("Empty file" for JSON)"
# ---------------------------------------------------------------------------


def test_a_missing_file_raises_file_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        strata.load(tmp_path / "absent.json")


def test_a_directory_is_folder_mode_not_an_error(tmp_path):
    """A directory is not a malformed file argument; it selects folder mode.

    The folder-mode contract lives in tests/unit/test_folder.py.
    """
    assert strata.load(tmp_path) == []


def test_an_empty_json_file_raises_value_error(files):
    with pytest.raises(ValueError, match="^Empty file$"):
        strata.load(files / "empty.json")


def test_an_empty_ndjson_file_is_simply_empty(tmp_path):
    path = tmp_path / "empty.ndjson"
    path.write_text("", encoding="utf-8")
    assert strata.load(path) == []


def test_invalid_json_in_a_document_raises(tmp_path):
    path = tmp_path / "bad.json"
    path.write_text("{oops}", encoding="utf-8")
    with pytest.raises(ValueError, match="Invalid JSON"):
        strata.load(path)


def test_an_unknown_return_type_raises(files):
    with pytest.raises(ValueError, match="return_type"):
        strata.load(files / "doc.json", return_type="nonsense")


# ---------------------------------------------------------------------------
# "`dump` writes compact JSON + trailing newline, mode 0644, truncating;
#  `split_by` with a file path is a `ValueError`"
# ---------------------------------------------------------------------------


def test_dump_writes_compact_json_with_a_trailing_newline(tmp_path):
    path = tmp_path / "out.json"
    strata.dump({"a": [1, 2]}, path)
    assert path.read_text(encoding="utf-8") == '{"a":[1,2]}\n'


def test_dump_truncates_an_existing_file(tmp_path):
    path = tmp_path / "out.json"
    path.write_text("x" * 5000, encoding="utf-8")
    strata.dump({"a": 1}, path)
    assert path.read_text(encoding="utf-8") == '{"a":1}\n'


@pytest.mark.skipif(os.name == "nt", reason="POSIX file modes")
def test_dump_writes_mode_0644(tmp_path):
    path = tmp_path / "out.json"
    strata.dump({"a": 1}, path)
    assert stat.S_IMODE(path.stat().st_mode) == 0o644


def test_split_by_with_a_file_target_is_rejected(tmp_path):
    # An *existing* file is unambiguously a file target. A path that does not
    # exist yet is a directory to be created, because dump creates dirpath.
    target = tmp_path / "out.json"
    target.write_text("[]", encoding="utf-8")
    with pytest.raises(ValueError, match="split_by"):
        strata.dump([{"k": "v"}], target, split_by="k")


def test_dump_rejects_what_dumps_rejects(tmp_path):
    with pytest.raises(TypeError):
        strata.dump(object(), tmp_path / "out.json")


def test_dump_accepts_a_path_object(tmp_path):
    strata.dump({"a": 1}, tmp_path / "out.json")
    assert (tmp_path / "out.json").is_file()


# ---------------------------------------------------------------------------
# Round trips
# ---------------------------------------------------------------------------


def test_dump_then_load_round_trips(tmp_path):
    value = {"s": "café", "n": [1, -2.5, 10**30], "b": [True, None], "nested": {"deep": [1]}}
    path = tmp_path / "round.json"
    strata.dump(value, path)
    assert strata.load(path) == value
    assert json.loads(path.read_text(encoding="utf-8")) == value


@pytest.mark.parametrize("suffix", [".json", ".ndjson"])
def test_files_larger_than_the_read_chunk_load_whole(tmp_path, suffix):
    # The reader sizes one read from fstat and only then drains a 64 KB
    # chunk loop for anything the size did not cover; a file several chunks
    # long must come back byte-complete either way (the whole content
    # round-trips through stdlib json as the oracle).
    records = [{"i": index, "text": "x" * 40} for index in range(4000)]
    path = tmp_path / f"big{suffix}"
    if suffix == ".json":
        path.write_text(json.dumps(records), encoding="utf-8")
    else:
        path.write_text("".join(json.dumps(r) + "\n" for r in records), encoding="utf-8")
    assert path.stat().st_size > 3 * 65536
    assert strata.load(path) == records


def test_big_integers_survive_ndjson(tmp_path):
    path = tmp_path / "big.ndjson"
    path.write_text('{"n": 123456789012345678901234567890}\n', encoding="utf-8")
    assert strata.load(path)[0]["n"] == 123456789012345678901234567890


def test_blank_lines_are_not_records(tmp_path):
    path = tmp_path / "gaps.ndjson"
    path.write_text('{"i":1}\n\n\n{"i":2}\n   \n', encoding="utf-8")
    assert strata.load(path) == [{"i": 1}, {"i": 2}]


def test_crlf_line_endings(tmp_path):
    path = tmp_path / "crlf.ndjson"
    path.write_bytes(b'{"i":1}\r\n{"i":2}\r\n')
    assert strata.load(path) == [{"i": 1}, {"i": 2}]


def test_a_missing_final_newline_still_yields_the_last_record(tmp_path):
    path = tmp_path / "tail.ndjson"
    path.write_text('{"i":1}\n{"i":2}', encoding="utf-8")
    assert strata.load(path) == [{"i": 1}, {"i": 2}]


def test_invalid_utf8_in_an_ndjson_line_follows_the_skip_errors_contract(tmp_path):
    """api.md: invalid NDJSON lines raise ValueError unless skip_errors=True.

    A line whose string carries a bad UTF-8 sequence is an invalid line like
    any other: the error is the contract's ValueError("Invalid JSON"), never
    the codec's, and skip_errors drops the line eagerly and lazily alike.
    """
    path = tmp_path / "bad.ndjson"
    path.write_bytes(b'{"i": 1}\n{"s": "\xff"}\n{"i": 3}\n')
    # The NDJSON loader names the line, as for any malformed line.
    with pytest.raises(ValueError, match="^Invalid JSON on line 2$") as caught:
        strata.load(path)
    assert not isinstance(caught.value, UnicodeDecodeError)
    assert strata.load(path, skip_errors=True) == [{"i": 1}, {"i": 3}]
    assert list(strata.load(path, iterator=True, skip_errors=True)) == [{"i": 1}, {"i": 3}]

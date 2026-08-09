"""Contract tests for folder mode.

Each test pins a clause of docs/context/api.md § File & folder I/O (folder
mode) or § JSONPath (folder search). Folder mode is new in the target API, so
these tests are the only definition of the behaviour besides the prose.

The two laws are property-tested at the end:

    dump(records, d, split_by=ks) then load(d) == records regrouped
    search(dir, e) == concat(search(f, e) for each discovered file f)
"""

import json
import os
import random

import pytest

import strata

RECORDS = [
    {"region": "eu", "team": "red", "id": 1},
    {"region": "us", "team": "blue", "id": 2},
    {"region": "eu", "team": "blue", "id": 3},
    {"region": "eu", "team": "red", "id": 4},
]


# ---------------------------------------------------------------------------
# Discovery: "every `*.json`/`*.ndjson`/`*.jsonl` under the directory,
# recursive; extensions matched case-insensitively; hidden files and hidden
# directories pruned; symlinks not followed; ordering is bytewise on the
# `/`-joined relative path."
# ---------------------------------------------------------------------------


@pytest.fixture
def tree(tmp_path):
    (tmp_path / "sub" / "deep").mkdir(parents=True)
    (tmp_path / ".hidden").mkdir()
    (tmp_path / "b.json").write_text('[{"v": "b"}]', encoding="utf-8")
    (tmp_path / "a.JSON").write_text('[{"v": "a"}]', encoding="utf-8")
    (tmp_path / "sub" / "c.NdJson").write_text('{"v": "c"}\n', encoding="utf-8")
    (tmp_path / "sub" / "deep" / "d.jsonl").write_text('{"v": "d"}\n', encoding="utf-8")
    (tmp_path / "ignored.txt").write_text("not json", encoding="utf-8")
    (tmp_path / "noextension").write_text("not json", encoding="utf-8")
    (tmp_path / ".secret.json").write_text('[{"v": "secret"}]', encoding="utf-8")
    (tmp_path / ".hidden" / "visible.json").write_text('[{"v": "hidden"}]', encoding="utf-8")
    return tmp_path


def test_discovery_is_recursive_and_case_insensitive(tree):
    assert [r["v"] for r in strata.load(tree)] == ["a", "b", "c", "d"]


def test_non_json_extensions_are_ignored_silently(tree):
    values = [r["v"] for r in strata.load(tree)]
    assert "not json" not in values


def test_hidden_files_and_directories_are_pruned(tree):
    values = [r["v"] for r in strata.load(tree)]
    assert "secret" not in values  # a hidden file
    assert "hidden" not in values  # a visible file inside a hidden directory


def test_ordering_is_bytewise_on_the_relative_path(tmp_path):
    # '-' (0x2D) sorts before '/' (0x2F), and 'Z' (0x5A) before 'a' (0x61):
    # neither holds under directory-then-name or case-insensitive ordering.
    (tmp_path / "a").mkdir()
    (tmp_path / "a" / "b.json").write_text('["a/b"]', encoding="utf-8")
    (tmp_path / "a-x.json").write_text('["a-x"]', encoding="utf-8")
    (tmp_path / "Z.json").write_text('["Z"]', encoding="utf-8")
    (tmp_path / "aa.json").write_text('["aa"]', encoding="utf-8")
    assert strata.load(tmp_path) == ["Z", "a-x", "a/b", "aa"]


def test_ordering_does_not_depend_on_where_the_tree_lives(tmp_path):
    for name in ("one", "two"):
        root = tmp_path / name
        (root / "sub").mkdir(parents=True)
        (root / "b.json").write_text('["b"]', encoding="utf-8")
        (root / "sub" / "a.json").write_text('["sub-a"]', encoding="utf-8")
    assert strata.load(tmp_path / "one") == strata.load(tmp_path / "two")


@pytest.mark.skipif(os.name == "nt", reason="POSIX symlinks")
def test_symlinks_are_not_followed(tmp_path):
    outside = tmp_path / "outside"
    outside.mkdir()
    (outside / "x.json").write_text('["outside"]', encoding="utf-8")

    root = tmp_path / "root"
    root.mkdir()
    (root / "real.json").write_text('["real"]', encoding="utf-8")
    (root / "link_dir").symlink_to(outside, target_is_directory=True)
    (root / "link_file.json").symlink_to(outside / "x.json")

    assert strata.load(root) == ["real"]


@pytest.mark.skipif(os.name == "nt", reason="POSIX symlinks")
def test_a_symlink_loop_does_not_hang(tmp_path):
    root = tmp_path / "root"
    (root / "sub").mkdir(parents=True)
    (root / "sub" / "loop").symlink_to(root, target_is_directory=True)
    (root / "a.json").write_text('["a"]', encoding="utf-8")
    assert strata.load(root) == ["a"]


def test_discovery_is_shared_by_load_and_search(tree):
    assert strata.search(tree, "$..v") == [r["v"] for r in strata.load(tree)]


# ---------------------------------------------------------------------------
# "load(dirpath) returns one list: each file's records concatenated in
#  discovery order -- a `.json` file with a list root contributes its elements,
#  any other root contributes the document itself, NDJSON contributes its lines."
# ---------------------------------------------------------------------------


def test_a_list_root_contributes_its_elements(tmp_path):
    (tmp_path / "a.json").write_text("[1, 2]", encoding="utf-8")
    (tmp_path / "b.json").write_text("[3]", encoding="utf-8")
    assert strata.load(tmp_path) == [1, 2, 3]


@pytest.mark.parametrize(
    ("content", "expected"),
    [('{"k": 1}', {"k": 1}), ("null", None), ("42", 42), ('"text"', "text"), ("true", True)],
)
def test_a_non_list_root_contributes_itself(tmp_path, content, expected):
    (tmp_path / "a.json").write_text(content, encoding="utf-8")
    assert strata.load(tmp_path) == [expected]


def test_ndjson_contributes_one_record_per_line(tmp_path):
    (tmp_path / "a.ndjson").write_text('{"i":1}\n{"i":2}\n{"i":3}\n', encoding="utf-8")
    assert strata.load(tmp_path) == [{"i": 1}, {"i": 2}, {"i": 3}]


def test_an_empty_directory_loads_as_an_empty_list(tmp_path):
    assert strata.load(tmp_path) == []


def test_a_directory_of_only_ignored_files_loads_as_empty(tmp_path):
    (tmp_path / "a.txt").write_text("x", encoding="utf-8")
    assert strata.load(tmp_path) == []


def test_str_and_path_arguments_agree(tree):
    assert strata.load(str(tree)) == strata.load(tree)


# ---------------------------------------------------------------------------
# "Per-file errors follow `skip_errors` (False -> propagate at the point the
#  file is consumed; True -> skip the offending file/line)."
# ---------------------------------------------------------------------------


@pytest.fixture
def broken(tmp_path):
    (tmp_path / "a.json").write_text('[{"i": 1}]', encoding="utf-8")
    (tmp_path / "b.json").write_text("{oops}", encoding="utf-8")
    (tmp_path / "c.ndjson").write_text('{"i":3}\nnot json\n{"i":5}\n', encoding="utf-8")
    return tmp_path


def test_a_broken_file_propagates_by_default(broken):
    with pytest.raises(ValueError):
        strata.load(broken)


def test_skip_errors_drops_the_file_and_keeps_the_good_lines(broken):
    # The whole of b.json is lost; only the bad *line* of c.ndjson is.
    assert strata.load(broken, skip_errors=True) == [{"i": 1}, {"i": 3}, {"i": 5}]


def test_the_iterator_yields_earlier_records_before_the_failure(broken):
    records = strata.load(broken, iterator=True)
    assert next(records) == {"i": 1}  # a.json arrives first...
    with pytest.raises(ValueError):
        next(records)  # ...then b.json fails where it is reached


def test_the_iterator_honours_skip_errors(broken):
    assert list(strata.load(broken, iterator=True, skip_errors=True)) == [
        {"i": 1},
        {"i": 3},
        {"i": 5},
    ]


def test_an_empty_json_file_in_a_directory_is_an_error(tmp_path):
    (tmp_path / "a.json").write_text("", encoding="utf-8")
    with pytest.raises(ValueError, match="Empty file"):
        strata.load(tmp_path)
    assert strata.load(tmp_path, skip_errors=True) == []


def test_cursor_mode_on_a_directory_is_rejected(tmp_path):
    with pytest.raises(ValueError, match="cursor"):
        strata.load(tmp_path, return_type="cursor")


# ---------------------------------------------------------------------------
# "dump(records, dirpath, split_by=key_or_keys) splits a list of dicts into
#  files grouped by the value(s) of the given key(s)."
# ---------------------------------------------------------------------------


def test_one_key_writes_one_file_per_group(tmp_path):
    strata.dump(RECORDS, tmp_path, split_by="region")
    assert sorted(p.name for p in tmp_path.rglob("*.json")) == ["eu.json", "us.json"]


def test_several_keys_nest_one_directory_per_key(tmp_path):
    strata.dump(RECORDS, tmp_path, split_by=["region", "team"])
    written = sorted(str(p.relative_to(tmp_path)) for p in tmp_path.rglob("*.json"))
    assert written == [
        os.path.join("eu", "blue.json"),
        os.path.join("eu", "red.json"),
        os.path.join("us", "blue.json"),
    ]


def test_a_single_key_string_and_a_one_element_sequence_agree(tmp_path):
    one, other = tmp_path / "one", tmp_path / "other"
    strata.dump(RECORDS, one, split_by="region")
    strata.dump(RECORDS, other, split_by=["region"])
    assert strata.load(one) == strata.load(other)


def test_split_by_accepts_a_tuple(tmp_path):
    strata.dump(RECORDS, tmp_path, split_by=("region", "team"))
    assert (tmp_path / "eu" / "red.json").is_file()


def test_each_file_is_a_compact_array_with_a_trailing_newline(tmp_path):
    strata.dump(RECORDS, tmp_path, split_by="region")
    raw = (tmp_path / "eu.json").read_bytes()
    assert raw.startswith(b"[") and raw.endswith(b"]\n")
    assert b" " not in raw and b"\n" not in raw[:-1]


def test_input_order_is_preserved_within_a_group(tmp_path):
    strata.dump(RECORDS, tmp_path, split_by="region")
    assert [r["id"] for r in json.loads((tmp_path / "eu.json").read_text())] == [1, 3, 4]


@pytest.mark.parametrize(
    ("value", "name"),
    [
        ("plain", "plain.json"),
        ("a b", "a b.json"),
        (7, "7.json"),
        (-7, "-7.json"),
        (True, "true.json"),
        (False, "false.json"),
    ],
)
def test_split_values_use_their_json_string_form(tmp_path, value, name):
    strata.dump([{"k": value}], tmp_path, split_by="k")
    assert (tmp_path / name).is_file()


def test_a_target_directory_is_created_if_missing(tmp_path):
    target = tmp_path / "not" / "yet"
    strata.dump(RECORDS, target, split_by=["region", "team"])
    assert (target / "eu" / "red.json").is_file()


def test_dump_returns_none(tmp_path):
    assert strata.dump(RECORDS, tmp_path, split_by="region") is None


def test_a_previous_run_is_overwritten_not_appended(tmp_path):
    strata.dump(RECORDS, tmp_path, split_by="region")
    strata.dump([{"region": "eu", "id": 99}], tmp_path, split_by="region")
    assert json.loads((tmp_path / "eu.json").read_text()) == [{"region": "eu", "id": 99}]


def test_unrelated_files_are_left_alone(tmp_path):
    keep = tmp_path / "keep.txt"
    keep.write_text("untouched", encoding="utf-8")
    other = tmp_path / "other.json"
    other.write_text('["untouched"]', encoding="utf-8")

    strata.dump(RECORDS, tmp_path, split_by="region")
    assert keep.read_text(encoding="utf-8") == "untouched"
    assert other.read_text(encoding="utf-8") == '["untouched"]'


def test_empty_records_create_the_directory_and_write_nothing(tmp_path):
    target = tmp_path / "empty"
    strata.dump([], target, split_by="k")
    assert target.is_dir()
    assert list(target.iterdir()) == []
    assert strata.load(target) == []


def test_str_and_path_targets_agree(tmp_path):
    strata.dump(RECORDS, str(tmp_path / "a"), split_by="region")
    strata.dump(RECORDS, tmp_path / "b", split_by="region")
    assert strata.load(tmp_path / "a") == strata.load(tmp_path / "b")


# ---------------------------------------------------------------------------
# Errors: collisions, path safety, and wrong types
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    "records",
    [
        [{"k": 1}, {"k": "1"}],  # int vs str
        [{"k": True}, {"k": "true"}],  # bool vs str
        [{"k": 0}, {"k": "0"}],
        [{"k": "A"}, {"k": "a"}],  # differ only by case
        [{"k": "Group"}, {"k": "group"}],
    ],
)
def test_colliding_group_names_are_rejected(tmp_path, records):
    with pytest.raises(ValueError):
        strata.dump(records, tmp_path, split_by="k")


def test_the_same_text_at_two_levels_is_not_a_collision(tmp_path):
    """Levels are separate namespaces: `1/1.json` is one unambiguous path."""
    strata.dump([{"a": 1, "b": "1"}], tmp_path, split_by=["a", "b"])
    assert (tmp_path / "1" / "1.json").is_file()


@pytest.mark.parametrize("value", ["", ".", "..", "a/b", "a\\b", "a\x00b"])
def test_path_unsafe_names_are_rejected(tmp_path, value):
    with pytest.raises(ValueError):
        strata.dump([{"k": value}], tmp_path, split_by="k")


def test_nothing_escapes_the_target_directory(tmp_path):
    target = tmp_path / "target"
    with pytest.raises(ValueError):
        strata.dump([{"k": "../escaped"}], target, split_by="k")
    assert not (tmp_path / "escaped.json").exists()


def test_a_missing_split_key_is_rejected(tmp_path):
    with pytest.raises(ValueError, match="split key"):
        strata.dump([{"other": 1}], tmp_path, split_by="k")
    with pytest.raises(ValueError, match="split key"):
        strata.dump([{"a": 1}], tmp_path, split_by=["a", "b"])


@pytest.mark.parametrize("value", [1.5, None, [1], {"a": 1}])
def test_a_non_scalar_split_value_is_a_type_error(tmp_path, value):
    with pytest.raises(TypeError):
        strata.dump([{"k": value}], tmp_path, split_by="k")


@pytest.mark.parametrize("obj", [{"k": 1}, "text", 42, None])
def test_a_non_list_object_is_a_type_error(tmp_path, obj):
    with pytest.raises(TypeError):
        strata.dump(obj, tmp_path, split_by="k")


def test_a_non_dict_record_is_a_type_error(tmp_path):
    with pytest.raises(TypeError):
        strata.dump([{"k": "a"}, 5], tmp_path, split_by="k")


def test_an_empty_split_by_is_rejected(tmp_path):
    with pytest.raises(ValueError):
        strata.dump(RECORDS, tmp_path, split_by=[])


def test_a_directory_target_without_split_by_is_rejected(tmp_path):
    with pytest.raises(ValueError, match="split_by"):
        strata.dump(RECORDS, tmp_path)


def test_nothing_is_written_when_validation_fails(tmp_path):
    """Grouping is decided before anything is written, so a failure is clean."""
    records = [{"k": "good"}] * 3 + [{"k": 1.5}]
    with pytest.raises(TypeError):
        strata.dump(records, tmp_path, split_by="k")
    assert list(tmp_path.iterdir()) == []


# ---------------------------------------------------------------------------
# Folder search
# ---------------------------------------------------------------------------


def test_folder_search_concatenates_in_discovery_order(tree):
    assert strata.search(tree, "$..v") == ["a", "b", "c", "d"]


def test_folder_search_on_an_empty_directory(tmp_path):
    assert strata.search(tmp_path, "$.a") == []
    assert list(strata.search(tmp_path, "$.a", iterator=True)) == []


def test_folder_search_iterator_yields_the_same_matches(tree):
    assert list(strata.search(tree, "$..v", iterator=True)) == strata.search(tree, "$..v")


def test_folder_search_accepts_a_compiled_path(tree):
    compiled = strata.compile("$..v")
    assert strata.search(tree, compiled) == strata.search(tree, "$..v")


def test_a_bad_expression_is_rejected_for_a_directory(tree):
    with pytest.raises(ValueError, match="^Invalid JSONPath expression$"):
        strata.search(tree, "$[")


def test_a_non_json_file_still_needs_a_known_extension(tmp_path):
    path = tmp_path / "data.txt"
    path.write_text("{}", encoding="utf-8")
    with pytest.raises(TypeError):
        strata.search(path, "$")
    # ...but the same file inside a directory is simply ignored.
    assert strata.search(tmp_path, "$") == []


# ---------------------------------------------------------------------------
# The laws
# ---------------------------------------------------------------------------


def _discovered_files(root):
    """The files discovery would find, in its order, computed independently."""
    found = []
    for base, directories, names in os.walk(root):
        directories[:] = sorted(d for d in directories if not d.startswith("."))
        for name in names:
            if name.startswith("."):
                continue
            if not name.lower().endswith((".json", ".ndjson", ".jsonl")):
                continue
            full = os.path.join(base, name)
            found.append((os.path.relpath(full, root).replace(os.sep, "/"), full))
    return [full for _, full in sorted(found)]


@pytest.mark.parametrize(
    "expression",
    ["$", "$..v", "$[*]", "$.v", "$..id", "$[0]", "$[?(@.v == 'a')]", "$..absent"],
)
def test_the_folder_search_law(tree, expression):
    """search(dir, e) == concat(search(f, e) for each discovered file f)."""
    expected = []
    for path in _discovered_files(tree):
        expected.extend(strata.search(path, expression))
    assert strata.search(tree, expression) == expected


def _regrouped(records, keys):
    """The records, grouped by key path and ordered bytewise by that path."""

    def name(value):
        if isinstance(value, bool):
            return "true" if value else "false"
        return str(value)

    groups = {}
    for record in records:
        path = "/".join(name(record[key]) for key in keys)
        groups.setdefault(path, []).append(record)
    return [r for path in sorted(groups) for r in groups[path]]


@pytest.mark.parametrize("keys", [["a"], ["a", "b"], ["b", "a"], ["a", "b", "c"]])
def test_the_round_trip_law(tmp_path, keys):
    """dump then load returns the same records, regrouped in key-path order."""
    rng = random.Random(4242)
    records = []
    for index in range(120):
        records.append(
            {
                "a": rng.choice(["x", "y", "z"]),
                "b": rng.choice([1, 2, 3]),
                "c": rng.choice([True, False]),
                "index": index,
                # Finite floats only: NaN and infinity serialize as null and
                # lose their identity, so they are outside the law.
                "value": rng.uniform(-1e6, 1e6),
                "text": rng.choice(["", "plain", "café \U0001f600"]),
                "nested": {"deep": [rng.randint(0, 10), None]},
                "big": rng.randint(10**19, 10**25),
            },
        )

    target = tmp_path / "out"
    strata.dump(records, target, split_by=keys)
    assert strata.load(target) == _regrouped(records, keys)


def test_the_round_trip_law_survives_a_second_dump(tmp_path):
    """Dumping the loaded records again reproduces the same directory."""
    target = tmp_path / "once"
    strata.dump(RECORDS, target, split_by=["region", "team"])
    loaded = strata.load(target)

    again = tmp_path / "twice"
    strata.dump(loaded, again, split_by=["region", "team"])
    assert strata.load(again) == loaded

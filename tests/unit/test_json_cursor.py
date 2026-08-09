"""Contract tests for `strata.JsonCursor`.

Each test pins a clause of docs/context/api.md § Cursor and § Error contract.
The messages matter: they are the contract, not just diagnostics.
"""

import gc

import pytest

import strata

SAMPLE = """
{
    "name": "Alice",
    "age": 30,
    "active": true,
    "score": 99.5,
    "nothing": null,
    "tags": ["dev", "json"],
    "address": {"city": "Berlin"}
}
"""


@pytest.fixture
def cursor():
    return strata.loads(SAMPLE, return_type="cursor")


# ---------------------------------------------------------------------------
# "`strata.loads(source, return_type="cursor") -> JsonCursor`" and the
#  predicate/accessor surface
# ---------------------------------------------------------------------------


def test_loads_returns_a_cursor(cursor):
    assert isinstance(cursor, strata.JsonCursor)
    assert type(cursor).__name__ == "JsonCursor"


def test_predicates(cursor):
    assert cursor.is_object()
    assert cursor.field("name").is_string()
    assert cursor.field("age").is_number()
    assert cursor.field("active").is_bool()
    assert cursor.field("nothing").is_null()
    assert cursor.field("tags").is_array()
    assert cursor.field("address").is_object()

    # Exactly one predicate holds for any value.
    checks = ["is_null", "is_bool", "is_number", "is_string", "is_array", "is_object"]
    for key in ("name", "age", "active", "nothing", "tags", "address"):
        target = cursor.field(key)
        assert sum(getattr(target, check)() for check in checks) == 1


def test_accessors(cursor):
    assert cursor.field("name").get_str() == "Alice"
    assert cursor.field("age").get_int() == 30
    assert cursor.field("age").get_float() == 30.0
    assert cursor.field("score").get_float() == 99.5
    assert cursor.field("active").get_bool() is True


def test_navigation(cursor):
    assert cursor.field("address").field("city").get_str() == "Berlin"
    assert cursor.field("tags").at(0).get_str() == "dev"
    assert cursor.field("tags").at(1).get_str() == "json"
    assert cursor.field("tags").at(-1).get_str() == "json"  # negative indices count back


# ---------------------------------------------------------------------------
# 'type mismatch -> RuntimeError'; 'Missing key -> RuntimeError("field not
#  found"); index out of bounds -> RuntimeError("index out of range") (raised
#  immediately; test-pinned)'
# ---------------------------------------------------------------------------


def test_a_missing_key_raises_immediately(cursor):
    with pytest.raises(RuntimeError, match="^field not found$"):
        cursor.field("absent")


def test_an_out_of_range_index_raises_immediately(cursor):
    with pytest.raises(RuntimeError, match="^index out of range$"):
        cursor.field("tags").at(99)
    with pytest.raises(RuntimeError, match="^index out of range$"):
        cursor.field("tags").at(-99)


def test_field_on_a_non_object_raises(cursor):
    with pytest.raises(RuntimeError, match="not an object"):
        cursor.field("tags").field("x")


def test_at_on_a_non_array_raises(cursor):
    with pytest.raises(RuntimeError, match="not an array"):
        cursor.at(0)


@pytest.mark.parametrize(
    ("key", "getter", "expected"),
    [
        ("name", "get_bool", "not a bool"),
        ("name", "get_int", "not a number"),
        ("name", "get_float", "not a number"),
        ("age", "get_str", "not a string"),
        ("tags", "get_int", "not a number"),
        ("nothing", "get_str", "not a string"),
    ],
)
def test_a_type_mismatch_raises_runtime_error(cursor, key, getter, expected):
    with pytest.raises(RuntimeError, match=expected):
        getattr(cursor.field(key), getter)()


def test_a_non_integral_number_is_not_an_int():
    cursor = strata.loads('{"x": 1.5}', return_type="cursor")
    with pytest.raises(RuntimeError, match="not a number"):
        cursor.field("x").get_int()
    assert cursor.field("x").get_float() == 1.5


# ---------------------------------------------------------------------------
# "The returned `JsonCursor` holds a strong reference to its owning document --
#  the C++ document-outlives-cursor invariant is satisfied inside the binding,
#  never exposed to the user."
# ---------------------------------------------------------------------------


def test_a_cursor_keeps_its_document_alive():
    """Nothing the caller can drop leaves a cursor dangling."""
    child = strata.loads(SAMPLE, return_type="cursor").field("address")
    gc.collect()  # the root cursor is unreachable now
    assert child.field("city").get_str() == "Berlin"


def test_a_deep_cursor_survives_every_intermediate():
    root = strata.loads('{"a": {"b": {"c": ["deep"]}}}', return_type="cursor")
    leaf = root.field("a").field("b").field("c").at(0)
    del root
    gc.collect()
    assert leaf.get_str() == "deep"


def test_a_cursor_from_a_file_survives_the_load(tmp_path):
    path = tmp_path / "doc.json"
    path.write_text('{"k": {"v": "value"}}', encoding="utf-8")
    child = strata.load(path, return_type="cursor").field("k")
    gc.collect()
    assert child.field("v").get_str() == "value"


def test_many_cursors_share_one_document():
    root = strata.loads('{"a": 1, "b": 2}', return_type="cursor")
    children = [root.field("a"), root.field("b")]
    del root
    gc.collect()
    assert [child.get_int() for child in children] == [1, 2]


# ---------------------------------------------------------------------------
# Roots and error propagation
# ---------------------------------------------------------------------------


@pytest.mark.parametrize(
    ("text", "check"),
    [
        ("null", "is_null"),
        ("true", "is_bool"),
        ("42", "is_number"),
        ('"s"', "is_string"),
        ("[]", "is_array"),
        ("{}", "is_object"),
    ],
)
def test_any_value_can_be_the_root(text, check):
    assert getattr(strata.loads(text, return_type="cursor"), check)()


def test_invalid_json_still_raises_in_cursor_mode():
    with pytest.raises(ValueError, match="^Invalid JSON$"):
        strata.loads("{oops}", return_type="cursor")


def test_cursor_numbers_come_from_the_cpp_dom():
    """The DOM stores every number as a double, so large ints lose precision.

    That asymmetry is documented: `loads` keeps integers exact, cursor mode is
    DOM-backed and does not (docs/architecture/SKILL.md).
    """
    cursor = strata.loads('{"n": 12345678901234567890}', return_type="cursor")
    assert cursor.field("n").get_float() == 12345678901234567890.0
    assert strata.loads('{"n": 12345678901234567890}')["n"] == 12345678901234567890

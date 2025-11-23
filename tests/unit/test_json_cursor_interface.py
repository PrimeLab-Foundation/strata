from strata import JsonCursor


def test_root_is_object(json_root: JsonCursor):
    assert json_root.is_object()
    assert not json_root.is_array()
    assert not json_root.is_null()


def test_scalar_fields(json_root: JsonCursor):
    name = json_root.field("name")
    assert name.is_string()
    assert name.get_str() == "Alice"

    age = json_root.field("age")
    assert age.is_number()
    assert age.get_int() == 30

    active = json_root.field("active")
    assert active.is_bool()
    assert active.get_bool() is True

    score = json_root.field("score")
    assert score.is_number()
    assert score.get_float() == 99.5


def test_array_navigation(json_root: JsonCursor):
    tags = json_root.field("tags")
    assert tags.is_array()

    tag0 = tags.at(0)
    tag1 = tags.at(1)
    assert tag0.is_string()
    assert tag0.get_str() == "dev"
    assert tag1.is_string()
    assert tag1.get_str() == "json"


def test_nested_object(json_root: JsonCursor):
    address = json_root.field("address")
    assert address.is_object()

    city = address.field("city")
    zip_code = address.field("zip")

    assert city.get_str() == "Berlin"
    assert zip_code.get_str() == "10115"

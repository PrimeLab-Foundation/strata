#include "strata/json/json_core.hpp"
#include "strata/json/json_cursor.hpp"
#include "strata/json/json_document.hpp"

#include <cassert>
#include <iostream>

using namespace strata;

int main() {
    const char* json_text = R"json(
    {
      "name": "Alice",
      "age": 30,
      "active": true,
      "score": 99.5,
      "tags": ["dev", "json"],
      "address": {
        "city": "Berlin",
        "zip": "10115"
      }
    }
    )json";

    // 1) Parse into JsonDocument
    auto doc_res = JsonDocument::from_string(json_text);
    assert(doc_res.ok());
    JsonDocument doc = std::move(doc_res.value);

    // 2) Root cursor
    JsonCursor root = doc.root();
    assert(root.is_object());

    // 3) Simple scalar fields
    auto name_cur = root.get_field("name");
    assert(name_cur.ok());
    auto name_str = name_cur.value.get_string();
    assert(name_str.ok());
    assert(name_str.value == "Alice");

    auto age_cur = root.get_field("age");
    assert(age_cur.ok());
    auto age_int = age_cur.value.get_int64();
    assert(age_int.ok());
    assert(age_int.value == 30);

    auto active_cur = root.get_field("active");
    assert(active_cur.ok());
    auto active_bool = active_cur.value.get_bool();
    assert(active_bool.ok());
    assert(active_bool.value == true);

    auto score_cur = root.get_field("score");
    assert(score_cur.ok());
    auto score_double = score_cur.value.get_double();
    assert(score_double.ok());
    assert(score_double.value == 99.5);

    // 4) Array navigation
    auto tags_cur = root.get_field("tags");
    assert(tags_cur.ok());
    assert(tags_cur.value.is_array());

    auto tag0 = tags_cur.value.get_at(0);
    assert(tag0.ok());
    auto tag0_str = tag0.value.get_string();
    assert(tag0_str.ok());
    assert(tag0_str.value == "dev");

    auto tag1 = tags_cur.value.get_at(1);
    assert(tag1.ok());
    auto tag1_str = tag1.value.get_string();
    assert(tag1_str.ok());
    assert(tag1_str.value == "json");

    // Out-of-bounds should report IndexOutOfBounds
    auto tag2 = tags_cur.value.get_at(2);
    assert(!tag2.ok());
    assert(tag2.status == Status::IndexOutOfBounds);

    // 5) Nested object navigation
    auto addr_cur = root.get_field("address");
    assert(addr_cur.ok());
    assert(addr_cur.value.is_object());

    auto city_cur = addr_cur.value.get_field("city");
    assert(city_cur.ok());
    auto city_str = city_cur.value.get_string();
    assert(city_str.ok());
    assert(city_str.value == "Berlin");

    auto zip_cur = addr_cur.value.get_field("zip");
    assert(zip_cur.ok());
    auto zip_str = zip_cur.value.get_string();
    assert(zip_str.ok());
    assert(zip_str.value == "10115");

    // Missing key should report KeyNotFound
    auto missing = root.get_field("does_not_exist");
    assert(!missing.ok());
    assert(missing.status == Status::KeyNotFound);

    std::cout << "test_json_document: all assertions passed\n";
    return 0;
}

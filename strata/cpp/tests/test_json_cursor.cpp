#include "json_core.hpp"
#include "json_cursor.hpp"

#include <cassert>
#include <iostream>

using namespace strata;

int main() {
    // Build a small JSON object manually:
    // {
    //   "name": "Alice",
    //   "age":  30,
    //   "tags": ["dev", "json"]
    // }
    JsonValue root(JsonValue::Object{});
    auto& obj = root.as_object();
    obj["name"] = JsonValue(std::string("Alice"));
    obj["age"] = JsonValue(JsonValue::Number(30));
    obj["tags"] = JsonValue(JsonValue::Array{
        JsonValue(std::string("dev")),
        JsonValue(std::string("json")),
    });

    JsonCursor c(&root);

    // Object checks
    assert(c.is_object());

    // Field "name"
    auto name_res = c.get_field("name");
    assert(name_res.ok());
    auto name_str = name_res.value.get_string();
    assert(name_str.ok());
    assert(name_str.value == "Alice");

    // Field "age"
    auto age_res = c.get_field("age");
    assert(age_res.ok());
    auto age_num = age_res.value.get_int64();
    assert(age_num.ok());
    assert(age_num.value == 30);

    // Field "tags[1]"
    auto tags_res = c.get_field("tags");
    assert(tags_res.ok());
    auto tag1 = tags_res.value.get_at(1);
    assert(tag1.ok());
    auto tag1_str = tag1.value.get_string();
    assert(tag1_str.ok());
    assert(tag1_str.value == "json");

    std::cout << "test_json_cursor: all assertions passed\n";
    return 0;
}

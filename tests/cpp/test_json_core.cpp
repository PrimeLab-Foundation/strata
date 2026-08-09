/**
 * @file test_json_core.cpp
 * @brief Contract suite for the core value model: FlatMap, Status/Result, JsonValue.
 *
 * Pins the two properties the rest of the engine builds on — FlatMap's
 * insertion order and lookup rules, and Result's status/value semantics —
 * plus the JsonValue predicates and accessors.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_core.hpp"

#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

using strata::FlatMap;
using strata::JsonValue;
using strata::Result;
using strata::Status;

// ---------------------------------------------------------------------------
// FlatMap — ordering
// ---------------------------------------------------------------------------

/**
 * Iteration yields keys in the order they were first inserted, not sorted.
 *
 * Both operator[] overloads are covered: a string literal materialises a
 * temporary and binds to operator[](K&&), while a named std::string binds to
 * operator[](const K&). They are separate loops over separate insert calls, so
 * a change to one must not be assumed to cover the other.
 */
static void test_iteration_follows_insertion_order() {
    FlatMap<std::string, int> from_rvalue_keys;
    from_rvalue_keys["zebra"] = 1;
    from_rvalue_keys["alpha"] = 2;
    from_rvalue_keys["middle"] = 3;

    const std::string zebra = "zebra";
    const std::string alpha = "alpha";
    const std::string middle = "middle";
    FlatMap<std::string, int> from_lvalue_keys;
    from_lvalue_keys[zebra] = 1;
    from_lvalue_keys[alpha] = 2;
    from_lvalue_keys[middle] = 3;

    for (const auto* map : {&from_rvalue_keys, &from_lvalue_keys}) {
        std::vector<std::string> keys;
        for (const auto& entry : *map)
            keys.push_back(entry.first);

        assert(keys.size() == 3);
        assert(keys[0] == "zebra");
        assert(keys[1] == "alpha");
        assert(keys[2] == "middle");
    }
}

/// Assigning through an existing key updates in place: no duplicate, no reorder.
static void test_existing_key_keeps_its_position() {
    FlatMap<std::string, int> map;
    map["a"] = 1;
    map["b"] = 2;

    map["a"] = 99; // rvalue overload, existing key
    const std::string b = "b";
    map[b] = 98; // lvalue overload, existing key

    assert(map.size() == 2);
    assert(map.begin()->first == "a");
    assert(map.begin()->second == 99);
    assert((map.begin() + 1)->first == "b");
    assert((map.begin() + 1)->second == 98);
}

/// The rvalue overload moves the key rather than copying it.
static void test_rvalue_key_is_moved_in() {
    FlatMap<std::string, int> map;
    std::string key = "a-key-long-enough-to-defeat-the-small-string-optimisation";
    const std::string expected = key;

    map[std::move(key)] = 7;

    assert(map.size() == 1);
    assert(map.begin()->first == expected);
    assert(map.at(expected) == 7);
}

/// Values are mutable through iteration; keys keep their slots.
static void test_iteration_can_mutate_values() {
    FlatMap<std::string, int> map;
    map["a"] = 1;
    map["b"] = 2;

    for (auto& entry : map)
        entry.second *= 10;

    assert(map.at("a") == 10);
    assert(map.at("b") == 20);
}

// ---------------------------------------------------------------------------
// FlatMap — lookup
// ---------------------------------------------------------------------------

static void test_find_locates_and_reports_absence() {
    FlatMap<std::string, int> map;
    map["present"] = 42;

    auto found = map.find("present");
    assert(found != map.end());
    assert(found->second == 42);

    assert(map.find("absent") == map.end());
}

/// The const overloads see the same entries.
static void test_const_lookup_matches_mutable_lookup() {
    FlatMap<std::string, int> map;
    map["present"] = 42;

    const FlatMap<std::string, int>& ref = map;
    auto found = ref.find("present");
    assert(found != ref.end());
    assert(found->second == 42);
    assert(ref.find("absent") == ref.end());
    assert(ref.at("present") == 42);
}

static void test_at_throws_when_the_key_is_absent() {
    FlatMap<std::string, int> map;
    map["present"] = 1;

    bool threw = false;
    try {
        (void)map.at("absent");
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    const FlatMap<std::string, int>& ref = map;
    threw = false;
    try {
        (void)ref.at("absent");
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}

/**
 * emplace() appends unconditionally — the documented contract that lets the
 * parser own duplicate-key policy. Both entries exist; lookup sees the first.
 */
static void test_emplace_appends_without_deduplicating() {
    FlatMap<std::string, int> map;
    map.emplace(std::string("dup"), 1);
    map.emplace(std::string("dup"), 2);

    assert(map.size() == 2);
    assert(map.find("dup")->second == 1);
    assert(map.at("dup") == 1);

    // Assigning through the first iterator is how "last wins" is implemented.
    map.find("dup")->second = 2;
    assert(map.at("dup") == 2);
    assert(map.size() == 2);
}

static void test_size_empty_and_clear() {
    FlatMap<std::string, int> map;
    assert(map.empty());
    assert(map.size() == 0);

    map["a"] = 1;
    map["b"] = 2;
    assert(!map.empty());
    assert(map.size() == 2);

    map.clear();
    assert(map.empty());
    assert(map.size() == 0);
    assert(map.find("a") == map.end());
}

// ---------------------------------------------------------------------------
// Status / Result
// ---------------------------------------------------------------------------

/// Ok must be the zero value: it is what value-initialisation produces.
static void test_ok_is_the_zero_status() {
    static_assert(static_cast<int>(Status::Ok) == 0, "Ok must be the zero value");
    assert(Status{} == Status::Ok);
}

static void test_ok_tracks_the_status() {
    assert((Result<int>{Status::Ok, 1}.ok()));
    assert(!(Result<int>{Status::TypeMismatch, 0}.ok()));
    assert(!(Result<int>{Status::KeyNotFound, 0}.ok()));
    assert(!(Result<int>{Status::IndexOutOfBounds, 0}.ok()));
    assert(!(Result<int>{Status::ParseError, 0}.ok()));
}

/// A Result built without a value holds a default-constructed T, never garbage.
static void test_omitted_value_is_default_constructed() {
    Result<int> missing{Status::KeyNotFound};
    assert(!missing.ok());
    assert(missing.value == 0);

    Result<std::string> text{Status::ParseError};
    assert(!text.ok());
    assert(text.value.empty());

    Result<JsonValue> node{Status::TypeMismatch};
    assert(!node.ok());
    assert(node.value.is_null());
}

/**
 * A default-constructed Result is a success carrying an initialised value.
 *
 * Checked at compile time on purpose: `Result<int> r;` default-initialises, so
 * a missing default member initialiser on `value` would leave it indeterminate
 * — and a runtime `assert(r.value == 0)` would then be reading uninitialised
 * memory that usually happens to be zero. Inside a constant expression the
 * same read is ill-formed, so this fails the build instead of passing by luck.
 */
static consteval bool default_result_is_fully_initialised() {
    Result<int> result;
    return result.status == Status::Ok && result.value == 0 && result.ok();
}
static_assert(default_result_is_fully_initialised());

static void test_default_result_is_ok() {
    Result<int> result;
    assert(result.ok());
    assert(result.status == Status::Ok);
    assert(result.value == 0);
}

static void test_result_carries_its_value() {
    Result<double> number{Status::Ok, 3.5};
    assert(number.ok());
    assert(number.value == 3.5);

    Result<std::string> text{Status::Ok, std::string("hello")};
    assert(text.ok());
    assert(text.value == "hello");
}

/// Large payloads move out of `value` rather than being copied.
static void test_value_can_be_moved_out() {
    JsonValue::Array items;
    for (int i = 0; i < 128; ++i)
        items.push_back(JsonValue(JsonValue::Variant(static_cast<double>(i))));

    Result<JsonValue> result{Status::Ok, JsonValue(JsonValue::Variant(std::move(items)))};
    assert(result.ok());

    JsonValue taken = std::move(result.value);
    assert(taken.is_array());
    assert(taken.as_array().size() == 128);
    assert(taken.as_array()[127].as_number() == 127.0);
}

// ---------------------------------------------------------------------------
// JsonValue
// ---------------------------------------------------------------------------

static void test_default_value_is_null() {
    JsonValue value;
    assert(value.is_null());
    assert(!value.is_bool());
    assert(!value.is_number());
    assert(!value.is_string());
    assert(!value.is_array());
    assert(!value.is_object());
}

/// Exactly one predicate holds for any value.
static void test_predicates_are_mutually_exclusive() {
    const JsonValue values[] = {
        JsonValue(),
        JsonValue(JsonValue::Variant(true)),
        JsonValue(JsonValue::Variant(1.5)),
        JsonValue(JsonValue::Variant(std::string("s"))),
        JsonValue(JsonValue::Variant(JsonValue::Array{})),
        JsonValue(JsonValue::Variant(JsonValue::Object{})),
    };

    for (const auto& value : values) {
        const int matches =
            static_cast<int>(value.is_null()) + static_cast<int>(value.is_bool()) +
            static_cast<int>(value.is_number()) + static_cast<int>(value.is_string()) +
            static_cast<int>(value.is_array()) + static_cast<int>(value.is_object());
        assert(matches == 1);
    }
}

static void test_accessors_return_the_stored_value() {
    JsonValue flag(JsonValue::Variant(true));
    assert(flag.is_bool());
    assert(flag.as_bool());

    JsonValue number(JsonValue::Variant(2.5));
    assert(number.as_number() == 2.5);

    JsonValue text(JsonValue::Variant(std::string("strata")));
    assert(text.as_string() == "strata");

    // Mutable accessors write through.
    number.as_number() = 4.0;
    assert(number.as_number() == 4.0);
    text.as_string() += "!";
    assert(text.as_string() == "strata!");
}

static void test_accessor_mismatch_throws() {
    JsonValue text(JsonValue::Variant(std::string("not a number")));

    bool threw = false;
    try {
        (void)text.as_number();
    } catch (const std::bad_variant_access&) {
        threw = true;
    }
    assert(threw);
}

/// Numbers are doubles on this path: integers past 2^53 are not exact.
static void test_numbers_are_doubles() {
    static_assert(std::is_same<JsonValue::Number, double>::value, "DOM numbers are doubles");

    const double big = 9007199254740993.0; // 2^53 + 1
    JsonValue value{JsonValue::Variant{big}};
    assert(value.as_number() == 9007199254740992.0);
}

static void test_objects_and_arrays_nest() {
    JsonValue::Object inner;
    inner["city"] = JsonValue(JsonValue::Variant(std::string("Berlin")));

    JsonValue::Array tags;
    tags.push_back(JsonValue(JsonValue::Variant(std::string("dev"))));
    tags.push_back(JsonValue(JsonValue::Variant(std::string("json"))));

    JsonValue::Object root;
    root["name"] = JsonValue(JsonValue::Variant(std::string("Alice")));
    root["address"] = JsonValue(JsonValue::Variant(std::move(inner)));
    root["tags"] = JsonValue(JsonValue::Variant(std::move(tags)));

    JsonValue document(JsonValue::Variant(std::move(root)));
    assert(document.is_object());

    const auto& object = document.as_object();
    assert(object.size() == 3);
    assert(object.begin()->first == "name"); // insertion order survives nesting
    assert(object.at("name").as_string() == "Alice");
    assert(object.at("address").as_object().at("city").as_string() == "Berlin");
    assert(object.at("tags").as_array().size() == 2);
    assert(object.at("tags").as_array()[1].as_string() == "json");
}

int main() {
    test_iteration_follows_insertion_order();
    test_existing_key_keeps_its_position();
    test_rvalue_key_is_moved_in();
    test_iteration_can_mutate_values();

    test_find_locates_and_reports_absence();
    test_const_lookup_matches_mutable_lookup();
    test_at_throws_when_the_key_is_absent();
    test_emplace_appends_without_deduplicating();
    test_size_empty_and_clear();

    test_ok_is_the_zero_status();
    test_ok_tracks_the_status();
    test_omitted_value_is_default_constructed();
    test_default_result_is_ok();
    test_result_carries_its_value();
    test_value_can_be_moved_out();

    test_default_value_is_null();
    test_predicates_are_mutually_exclusive();
    test_accessors_return_the_stored_value();
    test_accessor_mismatch_throws();
    test_numbers_are_doubles();
    test_objects_and_arrays_nest();

    std::puts("json_core_tests: OK");
    return 0;
}

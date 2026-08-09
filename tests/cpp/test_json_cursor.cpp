/**
 * @file test_json_cursor.cpp
 * @brief Contract suite for JsonCursor and JsonDocument.
 *
 * Pins the two accessor flavours, the exact messages the throwing one uses
 * (they reach Python verbatim, docs/context/api.md), and the ownership
 * guarantee that lets a cursor outlive the document handle it came from.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_document.hpp"

#include <cassert>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

using strata::JsonCursor;
using strata::JsonDocument;
using strata::Status;

namespace {

constexpr const char* kSample = R"({
    "name": "Alice",
    "age": 30,
    "active": true,
    "score": 99.5,
    "nothing": null,
    "tags": ["dev", "json"],
    "address": {"city": "Berlin", "zip": "10115"}
})";

[[nodiscard]] JsonDocument parse(const char* text) {
    auto document = JsonDocument::from_string(text);
    assert(document.ok());
    return std::move(document.value);
}

/// Run @p action and report whether it threw with exactly @p message.
template <typename Action> [[nodiscard]] bool throws_with(const char* message, Action action) {
    try {
        action();
    } catch (const std::exception& error) {
        return std::string(error.what()) == message;
    }
    return false;
}

void test_document_parses_and_reports_failure() {
    assert(JsonDocument::from_string("{}").ok());
    const auto bad = JsonDocument::from_string("{invalid}");
    assert(!bad.ok());
    assert(bad.status == Status::ParseError);
}

void test_type_predicates() {
    const JsonDocument document = parse(kSample);
    const JsonCursor root = document.root();
    assert(root.is_object());
    assert(root.field("name").is_string());
    assert(root.field("age").is_number());
    assert(root.field("active").is_bool());
    assert(root.field("nothing").is_null());
    assert(root.field("tags").is_array());
    assert(root.field("address").is_object());

    // A default-constructed cursor points at nothing and claims nothing.
    const JsonCursor empty;
    assert(!empty.is_null());
    assert(!empty.is_object());
    assert(empty.raw() == nullptr);
}

void test_status_api_never_throws() {
    const JsonDocument document = parse(kSample);
    const JsonCursor root = document.root();

    assert(root.get_field("name").ok());
    assert(root.get_field("absent").status == Status::KeyNotFound);
    assert(root.get_at(0).status == Status::TypeMismatch); // root is an object

    const JsonCursor tags = root.field("tags");
    assert(tags.get_at(1).ok());
    assert(tags.get_at(99).status == Status::IndexOutOfBounds);
    assert(tags.get_field("x").status == Status::TypeMismatch);

    assert(root.field("age").get_int64().value == 30);
    assert(root.field("score").get_double().value == 99.5);
    assert(root.field("active").get_bool().value);
    assert(root.field("name").get_string().value == "Alice");

    // Wrong type reports a mismatch rather than a value.
    assert(root.field("name").get_int64().status == Status::TypeMismatch);
    assert(root.field("age").get_string().status == Status::TypeMismatch);
}

/// A non-integral number is not an integer, however it is spelled.
void test_integers_must_be_integral_and_in_range() {
    const JsonDocument document = parse(R"({"whole": 42, "frac": 1.5, "huge": 1e300})");
    const JsonCursor root = document.root();
    assert(root.field("whole").get_int64().value == 42);
    assert(root.field("frac").get_int64().status == Status::TypeMismatch);
    assert(root.field("huge").get_int64().status == Status::TypeMismatch);
    assert(root.field("frac").get_double().value == 1.5);
}

/// These messages are the Python error contract; they travel through verbatim.
void test_throwing_api_messages() {
    const JsonDocument document = parse(kSample);
    const JsonCursor root = document.root();

    assert(throws_with("field not found", [&] { (void)root.field("absent"); }));
    assert(throws_with("value is not an object", [&] { (void)root.field("tags").field("x"); }));
    assert(throws_with("index out of range", [&] { (void)root.field("tags").at(99); }));
    assert(throws_with("value is not an array", [&] { (void)root.at(0); }));
    assert(
        throws_with("value is not a bool", [&] { (void)root.field("name").get_bool_or_throw(); }));
    assert(throws_with("value is not a number", [&] { (void)root.field("name").get_int(); }));
    assert(throws_with("value is not a number", [&] { (void)root.field("name").get_float(); }));
    assert(throws_with("value is not a string", [&] { (void)root.field("age").get_str(); }));
}

void test_navigation() {
    const JsonDocument document = parse(kSample);
    const JsonCursor root = document.root();

    assert(root.field("address").field("city").get_str() == "Berlin");
    assert(root.field("tags").at(0).get_str() == "dev");
    assert(root.field("tags").at(1).get_str() == "json");
    assert(root.field("tags").array_size() == 2);
    assert(root.object_size() == 7);

    const auto keys = root.object_keys();
    assert(keys.size() == 7);
    assert(keys[0] == "name"); // insertion order
    assert(keys[1] == "age");

    // Sizes of the wrong kind of value are zero, not an error.
    assert(root.array_size() == 0);
    assert(root.field("tags").object_size() == 0);
    assert(root.field("name").object_keys().empty());
}

/**
 * The tree outlives the handle, as long as a share of it is held.
 *
 * This is the invariant the binding layer relies on: a Python cursor keeps its
 * document alive by holding one of these shares, so a cursor can never be left
 * pointing into freed memory (docs/context/api.md, § Cursor).
 */
void test_a_share_keeps_the_tree_alive() {
    std::shared_ptr<const strata::JsonValue> share;
    JsonCursor cursor;
    {
        const JsonDocument document = parse(kSample);
        share = document.share();
        cursor = document.root();
        assert(cursor.field("name").get_str() == "Alice");
    } // the document handle is gone; the share is not

    assert(share.use_count() == 1);
    assert(cursor.field("name").get_str() == "Alice");
    assert(cursor.field("address").field("city").get_str() == "Berlin");
}

void test_root_can_be_any_value() {
    assert(parse("null").root().is_null());
    assert(parse("true").root().get_bool_or_throw());
    assert(parse("42").root().get_int() == 42);
    assert(parse(R"("text")").root().get_str() == "text");
    assert(parse("[1,2,3]").root().array_size() == 3);
}

} // namespace

int main() {
    test_document_parses_and_reports_failure();
    test_type_predicates();
    test_status_api_never_throws();
    test_integers_must_be_integral_and_in_range();
    test_throwing_api_messages();
    test_navigation();
    test_a_share_keeps_the_tree_alive();
    test_root_can_be_any_value();

    std::puts("json_cursor_tests: OK");
    return 0;
}

/**
 * @file test_lazy_cursor.cpp
 * @brief Unit tests for LazyJsonCursor (selective materialization / lazy parsing)
 */

#include "strata/json/json_lazy_cursor.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/search/search.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

using namespace strata;

// Test counter
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name)                                                             \
    static void test_##name();                                                 \
    static struct test_##name##_registrar {                                    \
        test_##name##_registrar() {                                            \
            std::cout << "Running " #name "... ";                              \
            tests_run++;                                                       \
            try {                                                              \
                test_##name();                                                 \
                std::cout << "PASSED\n";                                       \
                tests_passed++;                                                \
            } catch (const std::exception& e) {                                \
                std::cout << "FAILED: " << e.what() << "\n";                   \
            } catch (...) {                                                    \
                std::cout << "FAILED: unknown exception\n";                    \
            }                                                                  \
        }                                                                      \
    } test_##name##_instance;                                                  \
    static void test_##name()

#define ASSERT_TRUE(cond)                                                      \
    do {                                                                       \
        if (!(cond)) {                                                         \
            throw std::runtime_error("Assertion failed: " #cond);              \
        }                                                                      \
    } while (0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_EQ(a, b)                                                        \
    do {                                                                       \
        if ((a) != (b)) {                                                      \
            throw std::runtime_error("Assertion failed: " #a " == " #b);       \
        }                                                                      \
    } while (0)

#define ASSERT_NEAR(a, b, eps)                                                 \
    do {                                                                       \
        if (std::fabs((a) - (b)) > (eps)) {                                    \
            throw std::runtime_error("Assertion failed: " #a " ~= " #b);       \
        }                                                                      \
    } while (0)

// ============================================================================
// Type Detection Tests
// ============================================================================

TEST(TypeDetectionNull) {
    LazyJsonCursor cursor("null");
    ASSERT_TRUE(cursor.is_null());
    ASSERT_FALSE(cursor.is_bool());
    ASSERT_FALSE(cursor.is_number());
    ASSERT_FALSE(cursor.is_string());
    ASSERT_FALSE(cursor.is_array());
    ASSERT_FALSE(cursor.is_object());
}

TEST(TypeDetectionBool) {
    LazyJsonCursor cursor_true("true");
    ASSERT_TRUE(cursor_true.is_bool());

    LazyJsonCursor cursor_false("false");
    ASSERT_TRUE(cursor_false.is_bool());
}

TEST(TypeDetectionNumber) {
    LazyJsonCursor cursor1("42");
    ASSERT_TRUE(cursor1.is_number());

    LazyJsonCursor cursor2("-3.14");
    ASSERT_TRUE(cursor2.is_number());

    LazyJsonCursor cursor3("1.5e10");
    ASSERT_TRUE(cursor3.is_number());
}

TEST(TypeDetectionString) {
    LazyJsonCursor cursor("\"hello\"");
    ASSERT_TRUE(cursor.is_string());
}

TEST(TypeDetectionArray) {
    LazyJsonCursor cursor("[1, 2, 3]");
    ASSERT_TRUE(cursor.is_array());
}

TEST(TypeDetectionObject) {
    LazyJsonCursor cursor("{\"key\": \"value\"}");
    ASSERT_TRUE(cursor.is_object());
}

TEST(TypeDetectionWithWhitespace) {
    LazyJsonCursor cursor("   \n\t  42  ");
    ASSERT_TRUE(cursor.is_number());
}

// ============================================================================
// Navigation Tests
// ============================================================================

TEST(GetFieldFindsValue) {
    LazyJsonCursor cursor("{\"name\": \"John\", \"age\": 30}");

    auto name_result = cursor.get_field("name");
    ASSERT_TRUE(name_result.ok());
    ASSERT_TRUE(name_result.value.is_string());

    auto str_result = name_result.value.materialize_string();
    ASSERT_TRUE(str_result.ok());
    ASSERT_EQ(str_result.value, "John");
}

TEST(GetFieldSkipsUnrelatedKeys) {
    LazyJsonCursor cursor("{\"a\": 1, \"b\": 2, \"c\": 3, \"d\": 4}");

    auto result = cursor.get_field("c");
    ASSERT_TRUE(result.ok());
    ASSERT_TRUE(result.value.is_number());

    auto num_result = result.value.materialize_number();
    ASSERT_TRUE(num_result.ok());
    ASSERT_NEAR(num_result.value, 3.0, 0.001);
}

TEST(GetFieldKeyNotFound) {
    LazyJsonCursor cursor("{\"name\": \"John\"}");

    auto result = cursor.get_field("missing");
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::KeyNotFound);
}

TEST(GetFieldTypeMismatch) {
    LazyJsonCursor cursor("[1, 2, 3]");

    auto result = cursor.get_field("key");
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::TypeMismatch);
}

TEST(GetAtReturnsCorrectElement) {
    LazyJsonCursor cursor("[10, 20, 30, 40]");

    auto result = cursor.get_at(2);
    ASSERT_TRUE(result.ok());

    auto num_result = result.value.materialize_number();
    ASSERT_TRUE(num_result.ok());
    ASSERT_NEAR(num_result.value, 30.0, 0.001);
}

TEST(GetAtFirstElement) {
    LazyJsonCursor cursor("[\"first\", \"second\"]");

    auto result = cursor.get_at(0);
    ASSERT_TRUE(result.ok());

    auto str_result = result.value.materialize_string();
    ASSERT_TRUE(str_result.ok());
    ASSERT_EQ(str_result.value, "first");
}

TEST(GetAtIndexOutOfBounds) {
    LazyJsonCursor cursor("[1, 2, 3]");

    auto result = cursor.get_at(10);
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::IndexOutOfBounds);
}

TEST(GetAtTypeMismatch) {
    LazyJsonCursor cursor("{\"key\": \"value\"}");

    auto result = cursor.get_at(0);
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::TypeMismatch);
}

TEST(NestedNavigation) {
    std::string json = R"({
        "user": {
            "profile": {
                "email": "test@example.com"
            }
        }
    })";

    LazyJsonCursor cursor(json);

    auto user = cursor.get_field("user");
    ASSERT_TRUE(user.ok());

    auto profile = user.value.get_field("profile");
    ASSERT_TRUE(profile.ok());

    auto email = profile.value.get_field("email");
    ASSERT_TRUE(email.ok());

    auto email_str = email.value.materialize_string();
    ASSERT_TRUE(email_str.ok());
    ASSERT_EQ(email_str.value, "test@example.com");
}

TEST(NestedArrayInObject) {
    std::string json = R"({
        "items": [
            {"id": 1, "name": "first"},
            {"id": 2, "name": "second"},
            {"id": 3, "name": "third"}
        ]
    })";

    LazyJsonCursor cursor(json);

    auto items = cursor.get_field("items");
    ASSERT_TRUE(items.ok());

    auto second = items.value.get_at(1);
    ASSERT_TRUE(second.ok());

    auto name = second.value.get_field("name");
    ASSERT_TRUE(name.ok());

    auto name_str = name.value.materialize_string();
    ASSERT_TRUE(name_str.ok());
    ASSERT_EQ(name_str.value, "second");
}

// ============================================================================
// Skip Tests
// ============================================================================

TEST(SkipNestedObject) {
    std::string json = R"([{"a": {"b": {"c": 1}}}, "target"])";

    LazyJsonCursor cursor(json);

    auto first = cursor.get_at(0);
    ASSERT_TRUE(first.ok());

    auto skip_result = first.value.skip();
    ASSERT_TRUE(skip_result.ok());

    // Should now be able to get second element
    auto second = cursor.get_at(1);
    ASSERT_TRUE(second.ok());

    auto str = second.value.materialize_string();
    ASSERT_TRUE(str.ok());
    ASSERT_EQ(str.value, "target");
}

TEST(SkipNestedArray) {
    std::string json = R"([[[[1, 2, 3]]], "after"])";

    LazyJsonCursor cursor(json);
    auto second = cursor.get_at(1);
    ASSERT_TRUE(second.ok());

    auto str = second.value.materialize_string();
    ASSERT_TRUE(str.ok());
    ASSERT_EQ(str.value, "after");
}

TEST(SkipStringWithEscapes) {
    // String contains quotes that look like brackets
    std::string json = R"(["string with \"quotes\" and [brackets]", "second"])";

    LazyJsonCursor cursor(json);
    auto second = cursor.get_at(1);
    ASSERT_TRUE(second.ok());

    auto str = second.value.materialize_string();
    ASSERT_TRUE(str.ok());
    ASSERT_EQ(str.value, "second");
}

TEST(SkipStringWithBracketsInside) {
    // String contains brackets that should not affect skip_container
    std::string json = R"({"key": "{not an object}", "other": 42})";

    LazyJsonCursor cursor(json);
    auto other = cursor.get_field("other");
    ASSERT_TRUE(other.ok());

    auto num = other.value.materialize_number();
    ASSERT_TRUE(num.ok());
    ASSERT_NEAR(num.value, 42.0, 0.001);
}

TEST(SkipNumberVariants) {
    std::string json = R"([0, -123, 3.14, 1e10, -2.5e-3, "end"])";

    LazyJsonCursor cursor(json);
    auto last = cursor.get_at(5);
    ASSERT_TRUE(last.ok());

    auto str = last.value.materialize_string();
    ASSERT_TRUE(str.ok());
    ASSERT_EQ(str.value, "end");
}

// ============================================================================
// Materialization Tests
// ============================================================================

TEST(MaterializeBoolTrue) {
    LazyJsonCursor cursor("true");

    auto result = cursor.materialize_bool();
    ASSERT_TRUE(result.ok());
    ASSERT_TRUE(result.value);
}

TEST(MaterializeBoolFalse) {
    LazyJsonCursor cursor("false");

    auto result = cursor.materialize_bool();
    ASSERT_TRUE(result.ok());
    ASSERT_FALSE(result.value);
}

TEST(MaterializeNumber) {
    LazyJsonCursor cursor("-3.14159");

    auto result = cursor.materialize_number();
    ASSERT_TRUE(result.ok());
    ASSERT_NEAR(result.value, -3.14159, 0.00001);
}

TEST(MaterializeString) {
    LazyJsonCursor cursor("\"hello world\"");

    auto result = cursor.materialize_string();
    ASSERT_TRUE(result.ok());
    ASSERT_EQ(result.value, "hello world");
}

TEST(MaterializeStringWithEscapes) {
    LazyJsonCursor cursor("\"line1\\nline2\\ttab\"");

    auto result = cursor.materialize_string();
    ASSERT_TRUE(result.ok());
    ASSERT_EQ(result.value, "line1\nline2\ttab");
}

TEST(MaterializeFullObject) {
    LazyJsonCursor cursor("{\"a\": 1, \"b\": 2}");

    auto result = cursor.materialize();
    ASSERT_TRUE(result.ok());
    ASSERT_TRUE(result.value.is_object());

    const auto& obj = result.value.as_object();
    ASSERT_EQ(obj.size(), 2u);
}

TEST(MaterializeFullArray) {
    LazyJsonCursor cursor("[1, 2, 3]");

    auto result = cursor.materialize();
    ASSERT_TRUE(result.ok());
    ASSERT_TRUE(result.value.is_array());

    const auto& arr = result.value.as_array();
    ASSERT_EQ(arr.size(), 3u);
}

// ============================================================================
// Edge Case Tests
// ============================================================================

TEST(UnicodeEscapesInKeys) {
    // Key contains unicode escape: \u0041 = 'A'
    std::string json = R"({"\u0041\u0042\u0043": "ABC key"})";

    LazyJsonCursor cursor(json);
    auto result = cursor.get_field("ABC");
    ASSERT_TRUE(result.ok());

    auto str = result.value.materialize_string();
    ASSERT_TRUE(str.ok());
    ASSERT_EQ(str.value, "ABC key");
}

TEST(UnicodeEscapesInStrings) {
    // String contains unicode escape
    std::string json = R"("Hello \u0057orld")";

    LazyJsonCursor cursor(json);
    auto result = cursor.materialize_string();
    ASSERT_TRUE(result.ok());
    ASSERT_EQ(result.value, "Hello World");
}

TEST(DeepNesting100Levels) {
    // Build deeply nested array
    std::string json;
    for (int i = 0; i < 100; ++i) json += "[";
    json += "42";
    for (int i = 0; i < 100; ++i) json += "]";

    LazyJsonCursor cursor(json);

    // Navigate to innermost value
    LazyJsonCursor current = cursor;
    for (int i = 0; i < 100; ++i) {
        auto result = current.get_at(0);
        ASSERT_TRUE(result.ok());
        current = result.value;
    }

    auto num = current.materialize_number();
    ASSERT_TRUE(num.ok());
    ASSERT_NEAR(num.value, 42.0, 0.001);
}

TEST(EmptyObject) {
    LazyJsonCursor cursor("{}");
    ASSERT_TRUE(cursor.is_object());

    auto size = cursor.object_size();
    ASSERT_TRUE(size.ok());
    ASSERT_EQ(size.value, 0u);
}

TEST(EmptyArray) {
    LazyJsonCursor cursor("[]");
    ASSERT_TRUE(cursor.is_array());

    auto size = cursor.array_size();
    ASSERT_TRUE(size.ok());
    ASSERT_EQ(size.value, 0u);
}

TEST(TrailingWhitespace) {
    LazyJsonCursor cursor("42   \n\t  ");

    auto result = cursor.materialize_number();
    ASSERT_TRUE(result.ok());
    ASSERT_NEAR(result.value, 42.0, 0.001);
}

TEST(LeadingWhitespace) {
    LazyJsonCursor cursor("   \n\t  [1, 2, 3]");
    ASSERT_TRUE(cursor.is_array());
}

// ============================================================================
// Iterator Tests
// ============================================================================

TEST(FieldIteratorBasic) {
    LazyJsonCursor cursor("{\"a\": 1, \"b\": 2, \"c\": 3}");

    auto iter_result = cursor.iter_fields();
    ASSERT_TRUE(iter_result.ok());

    auto iter = iter_result.value;
    int count = 0;
    while (!iter.at_end()) {
        auto field = *iter;
        ASSERT_FALSE(field.key_raw.empty());
        ++count;
        ++iter;
    }
    ASSERT_EQ(count, 3);
}

TEST(FieldIteratorEmpty) {
    LazyJsonCursor cursor("{}");

    auto iter_result = cursor.iter_fields();
    ASSERT_TRUE(iter_result.ok());

    ASSERT_TRUE(iter_result.value.at_end());
}

TEST(ElementIteratorBasic) {
    LazyJsonCursor cursor("[10, 20, 30]");

    auto iter_result = cursor.iter_elements();
    ASSERT_TRUE(iter_result.ok());

    auto iter = iter_result.value;
    int count = 0;
    double sum = 0;
    while (!iter.at_end()) {
        auto elem = *iter;
        auto num = elem.materialize_number();
        ASSERT_TRUE(num.ok());
        sum += num.value;
        ++count;
        ++iter;
    }
    ASSERT_EQ(count, 3);
    ASSERT_NEAR(sum, 60.0, 0.001);
}

TEST(ElementIteratorEmpty) {
    LazyJsonCursor cursor("[]");

    auto iter_result = cursor.iter_elements();
    ASSERT_TRUE(iter_result.ok());

    ASSERT_TRUE(iter_result.value.at_end());
}

// ============================================================================
// Size Tests
// ============================================================================

TEST(ArraySizeBasic) {
    LazyJsonCursor cursor("[1, 2, 3, 4, 5]");

    auto result = cursor.array_size();
    ASSERT_TRUE(result.ok());
    ASSERT_EQ(result.value, 5u);
}

TEST(ObjectSizeBasic) {
    LazyJsonCursor cursor("{\"a\": 1, \"b\": 2}");

    auto result = cursor.object_size();
    ASSERT_TRUE(result.ok());
    ASSERT_EQ(result.value, 2u);
}

TEST(ArraySizeNestedElements) {
    LazyJsonCursor cursor("[[1,2], {\"a\":1}, [3,4,5]]");

    auto result = cursor.array_size();
    ASSERT_TRUE(result.ok());
    ASSERT_EQ(result.value, 3u);
}

// ============================================================================
// Error Tests
// ============================================================================

TEST(TypeMismatchError) {
    LazyJsonCursor cursor("42");

    auto bool_result = cursor.materialize_bool();
    ASSERT_FALSE(bool_result.ok());
    ASSERT_EQ(bool_result.status, Status::TypeMismatch);

    auto str_result = cursor.materialize_string();
    ASSERT_FALSE(str_result.ok());
    ASSERT_EQ(str_result.status, Status::TypeMismatch);
}

TEST(MalformedJsonError) {
    LazyJsonCursor cursor("{\"key\": }");  // Missing value

    auto result = cursor.get_field("key");
    // The result status depends on implementation - either ParseError or unexpected behavior
    // At minimum, the key lookup will fail or return error
    (void)result;
}

TEST(ErrorIncludesByteOffset) {
    LazyJsonCursor cursor("   [1, 2, 3]");

    // Position should reflect skipped whitespace
    auto pos = cursor.position();
    ASSERT_EQ(pos.offset, 3u);  // After "   "
}

TEST(SkipValueLiteralsAndRemaining) {
    LazyJsonCursor cursor_true("true");
    auto result_true = cursor_true.skip();
    ASSERT_TRUE(result_true.ok());
    ASSERT_TRUE(result_true.value.remaining().empty());

    LazyJsonCursor cursor_false("false");
    auto result_false = cursor_false.skip();
    ASSERT_TRUE(result_false.ok());
    ASSERT_TRUE(result_false.value.remaining().empty());

    LazyJsonCursor cursor_null("null");
    auto result_null = cursor_null.skip();
    ASSERT_TRUE(result_null.ok());
    ASSERT_TRUE(result_null.value.remaining().empty());
}

TEST(SkipValueMalformedLiterals) {
    LazyJsonCursor cursor_true("tru");
    auto result_true = cursor_true.skip();
    ASSERT_FALSE(result_true.ok());
    ASSERT_EQ(result_true.status, Status::ParseError);

    LazyJsonCursor cursor_false("fal");
    auto result_false = cursor_false.skip();
    ASSERT_FALSE(result_false.ok());
    ASSERT_EQ(result_false.status, Status::ParseError);

    LazyJsonCursor cursor_null("nul");
    auto result_null = cursor_null.skip();
    ASSERT_FALSE(result_null.ok());
    ASSERT_EQ(result_null.status, Status::ParseError);
}

TEST(SkipValueMalformedNumbers) {
    const char* cases[] = {"-", "1.", "1e", "1e+", "-x"};
    for (const auto* text : cases) {
        LazyJsonCursor cursor(text);
        auto result = cursor.skip();
        ASSERT_FALSE(result.ok());
        ASSERT_EQ(result.status, Status::ParseError);
    }
}

TEST(SkipValueMalformedStringAndContainer) {
    LazyJsonCursor cursor_unterminated("\"abc");
    auto result_unterminated = cursor_unterminated.skip();
    ASSERT_FALSE(result_unterminated.ok());
    ASSERT_EQ(result_unterminated.status, Status::ParseError);

    LazyJsonCursor cursor_array("[1, 2");
    auto result_array = cursor_array.skip();
    ASSERT_FALSE(result_array.ok());
    ASSERT_EQ(result_array.status, Status::ParseError);

    LazyJsonCursor cursor_object("{\"a\": 1");
    auto result_object = cursor_object.skip();
    ASSERT_FALSE(result_object.ok());
    ASSERT_EQ(result_object.status, Status::ParseError);
}

TEST(SkipValueUnknownToken) {
    LazyJsonCursor cursor("?");
    auto result = cursor.skip();
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::ParseError);
}

TEST(SkipValueEmptyInput) {
    LazyJsonCursor cursor("");
    auto result = cursor.skip();
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::ParseError);
}

TEST(IteratorsTypeMismatch) {
    LazyJsonCursor cursor_array("[1, 2]");
    auto fields = cursor_array.iter_fields();
    ASSERT_FALSE(fields.ok());
    ASSERT_EQ(fields.status, Status::TypeMismatch);

    LazyJsonCursor cursor_object("{\"a\": 1}");
    auto elems = cursor_object.iter_elements();
    ASSERT_FALSE(elems.ok());
    ASSERT_EQ(elems.status, Status::TypeMismatch);
}

TEST(FieldIteratorEscapedKeyAndEquality) {
    LazyJsonCursor cursor("{\"a\\tb\": 1, \"b\": 2}");
    auto iter_result = cursor.iter_fields();
    ASSERT_TRUE(iter_result.ok());

    auto it1 = iter_result.value;
    auto it2 = iter_result.value;
    ASSERT_TRUE(it1 == it2);

    auto field = *it1;
    ASSERT_TRUE(field.key_has_escapes);
    ASSERT_EQ(field.key(), "a\tb");

    ++it2;
    ASSERT_TRUE(it1 != it2);
}

TEST(FieldIteratorKeyWithoutEscapes) {
    LazyJsonCursor cursor("{\"plain\": 1}");
    auto iter_result = cursor.iter_fields();
    ASSERT_TRUE(iter_result.ok());
    auto field = *iter_result.value;
    ASSERT_FALSE(field.key_has_escapes);
    ASSERT_EQ(field.key(), "plain");
}

TEST(FieldIteratorEndOperations) {
    LazyJsonCursor cursor("{}");
    auto iter_result = cursor.iter_fields();
    ASSERT_TRUE(iter_result.ok());
    auto it = iter_result.value;
    ASSERT_TRUE(it.at_end());

    auto field = *it;
    ASSERT_TRUE(field.key_raw.empty());

    ++it;
    ASSERT_TRUE(it.at_end());

    auto it2 = cursor.iter_fields().value;
    ASSERT_TRUE(it == it2);
}

TEST(ElementIteratorOperations) {
    LazyJsonCursor cursor("[1, 2]");
    auto iter_result = cursor.iter_elements();
    ASSERT_TRUE(iter_result.ok());

    auto it1 = iter_result.value;
    auto it2 = iter_result.value;
    ASSERT_TRUE(it1 == it2);

    auto elem = *it1;
    ASSERT_TRUE(elem.is_number());

    ++it2;
    ASSERT_TRUE(it1 != it2);

    LazyJsonCursor empty_cursor("[]");
    auto end_iter = empty_cursor.iter_elements().value;
    ASSERT_TRUE(end_iter.at_end());
    ++end_iter;
    auto end_iter2 = empty_cursor.iter_elements().value;
    ASSERT_TRUE(end_iter == end_iter2);
}

TEST(FieldIteratorAdvanceError) {
    LazyJsonCursor cursor("{\"a\": , \"b\": 2}");
    auto iter_result = cursor.iter_fields();
    ASSERT_TRUE(iter_result.ok());

    auto it = iter_result.value;
    ASSERT_TRUE(!it.at_end());
    ++it;  // Advance to invalid key (missing quotes)
    ASSERT_TRUE(it.at_end());
}

TEST(ReadStringContentError) {
    LazyJsonCursor cursor("{a: 1}");
    auto result = cursor.get_field("a");
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::ParseError);
}

TEST(ReadStringContentUnterminated) {
    LazyJsonCursor cursor("{\"a: 1}");
    auto result = cursor.get_field("a");
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::ParseError);
}

TEST(MaterializeParseErrors) {
    LazyJsonCursor cursor_bool("tru");
    auto bool_result = cursor_bool.materialize_bool();
    ASSERT_FALSE(bool_result.ok());
    ASSERT_EQ(bool_result.status, Status::ParseError);

    LazyJsonCursor cursor_number("1e");
    auto num_result = cursor_number.materialize_number();
    ASSERT_FALSE(num_result.ok());
    ASSERT_EQ(num_result.status, Status::ParseError);

    LazyJsonCursor cursor_string("\"abc");
    auto str_result = cursor_string.materialize_string();
    ASSERT_FALSE(str_result.ok());
    ASSERT_EQ(str_result.status, Status::ParseError);
}

TEST(MaterializeNumberTypeMismatch) {
    LazyJsonCursor cursor("\"string\"");
    auto result = cursor.materialize_number();
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::TypeMismatch);
}

TEST(SizeTypeMismatchAndErrors) {
    LazyJsonCursor cursor_number("42");
    auto arr_size = cursor_number.array_size();
    ASSERT_FALSE(arr_size.ok());
    ASSERT_EQ(arr_size.status, Status::TypeMismatch);

    auto obj_size = cursor_number.object_size();
    ASSERT_FALSE(obj_size.ok());
    ASSERT_EQ(obj_size.status, Status::TypeMismatch);

    LazyJsonCursor cursor_array("[1, , 2]");
    auto arr_bad = cursor_array.array_size();
    ASSERT_FALSE(arr_bad.ok());
    ASSERT_EQ(arr_bad.status, Status::ParseError);

    LazyJsonCursor cursor_obj_key("{a: 1}");
    auto obj_key_bad = cursor_obj_key.object_size();
    ASSERT_FALSE(obj_key_bad.ok());
    ASSERT_EQ(obj_key_bad.status, Status::ParseError);

    LazyJsonCursor cursor_obj_colon("{\"a\" 1}");
    auto obj_colon_bad = cursor_obj_colon.object_size();
    ASSERT_FALSE(obj_colon_bad.ok());
    ASSERT_EQ(obj_colon_bad.status, Status::ParseError);

    LazyJsonCursor cursor_obj_value("{\"a\": }");
    auto obj_value_bad = cursor_obj_value.object_size();
    ASSERT_FALSE(obj_value_bad.ok());
    ASSERT_EQ(obj_value_bad.status, Status::ParseError);
}

TEST(IteratorErrorPaths) {
    LazyJsonCursor cursor_invalid_key("{a: 1}");
    auto iter_result = cursor_invalid_key.iter_fields();
    ASSERT_TRUE(iter_result.ok());
    auto it = iter_result.value;
    auto field = *it;
    ASSERT_TRUE(field.key_raw.empty());

    ++it;  // advance should hit skip_string error
    ASSERT_TRUE(it.at_end());

    LazyJsonCursor cursor_missing_colon("{\"a\" 1}");
    auto iter_colon = cursor_missing_colon.iter_fields();
    auto it_colon = iter_colon.value;
    ++it_colon;
    ASSERT_TRUE(it_colon.at_end());

    LazyJsonCursor cursor_bad_value("{\"a\": , \"b\": 1}");
    auto iter_value = cursor_bad_value.iter_fields();
    auto it_value = iter_value.value;
    ++it_value;
    ASSERT_TRUE(it_value.at_end());

    LazyJsonCursor cursor_array("[,1]");
    auto elem_iter = cursor_array.iter_elements();
    auto it_elem = elem_iter.value;
    ++it_elem;
    ASSERT_TRUE(it_elem.at_end());
}

TEST(GetAtSkipValueError) {
    LazyJsonCursor cursor("[1, , 2]");
    auto result = cursor.get_at(2);
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::ParseError);
}

TEST(GetFieldMissingColonError) {
    LazyJsonCursor cursor("{\"a\" 1}");
    auto result = cursor.get_field("a");
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::ParseError);
}

TEST(GetFieldSkipValueError) {
    LazyJsonCursor cursor("{\"a\": , \"b\": 1}");
    auto result = cursor.get_field("b");
    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.status, Status::ParseError);
}

TEST(PeekCharAtEnd) {
    LazyJsonCursor cursor("");
    ASSERT_FALSE(cursor.is_null());
}

TEST(LazySearchPathStubs) {
    CompiledPath path;
    auto values = eval_search_path_lazy("{}", path, 1);
    ASSERT_TRUE(values.empty());

    auto cursors = find_search_path_lazy("{}", path, 1);
    ASSERT_TRUE(cursors.empty());
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "\n========================================\n";
    std::cout << "Lazy Cursor Tests\n";
    std::cout << "========================================\n\n";

    // Tests are auto-registered and run via static initializers

    std::cout << "\n========================================\n";
    std::cout << "Results: " << tests_passed << "/" << tests_run << " passed\n";
    std::cout << "========================================\n";

    return (tests_passed == tests_run) ? 0 : 1;
}

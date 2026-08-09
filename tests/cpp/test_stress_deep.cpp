/**
 * @file test_stress_deep.cpp
 * @brief Deep-nesting stress for the recursive parser.
 *
 * The parser recurses, and by design nothing caps the depth — recursion *is*
 * the limit (docs/architecture/SKILL.md, contributor invariant 1). This suite
 * therefore stops at depth 100, the documented ceiling for the C++ stress
 * tests, and checks that nesting is parsed and destroyed correctly rather than
 * probing for the point where the stack gives out.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_parse.hpp"

#include <cassert>
#include <cstdio>
#include <string>

using strata::JsonValue;
using strata::parse_json;

namespace {

constexpr int kDepth = 100;

[[nodiscard]] std::string nested(const std::string& open, const std::string& close, int depth,
                                 const std::string& core) {
    std::string text;
    text.reserve(open.size() * static_cast<size_t>(depth) * 2 + core.size());
    for (int level = 0; level < depth; ++level)
        text += open;
    text += core;
    for (int level = 0; level < depth; ++level)
        text += close;
    return text;
}

void test_deeply_nested_arrays() {
    const auto result = parse_json(nested("[", "]", kDepth, "1"));
    assert(result.ok());

    const JsonValue* node = &result.value;
    for (int level = 0; level < kDepth; ++level) {
        assert(node->is_array());
        assert(node->as_array().size() == 1);
        node = &node->as_array()[0];
    }
    assert(node->as_number() == 1.0);
}

void test_deeply_nested_objects() {
    const auto result = parse_json(nested(R"({"a":)", "}", kDepth, "true"));
    assert(result.ok());

    const JsonValue* node = &result.value;
    for (int level = 0; level < kDepth; ++level) {
        assert(node->is_object());
        assert(node->as_object().size() == 1);
        node = &node->as_object().at("a");
    }
    assert(node->as_bool());
}

void test_alternating_nesting() {
    std::string text;
    for (int level = 0; level < kDepth / 2; ++level)
        text += R"({"a":[)";
    text += "0";
    for (int level = 0; level < kDepth / 2; ++level)
        text += "]}";

    assert(parse_json(text).ok());
}

/// Unbalanced nesting must fail, not be quietly accepted at depth.
void test_unbalanced_deep_nesting_is_rejected() {
    std::string missing_close;
    for (int level = 0; level < kDepth; ++level)
        missing_close += "[";
    missing_close += "1";
    assert(!parse_json(missing_close).ok());

    std::string extra_close = nested("[", "]", kDepth, "1");
    extra_close += "]";
    assert(!parse_json(extra_close).ok());
}

/// A wide document is not a deep one: no recursion, but plenty of elements.
void test_wide_documents() {
    std::string text = "[";
    for (int index = 0; index < 10000; ++index) {
        if (index != 0)
            text += ',';
        text += std::to_string(index);
    }
    text += ']';

    const auto result = parse_json(text);
    assert(result.ok());
    assert(result.value.as_array().size() == 10000);
    assert(result.value.as_array()[9999].as_number() == 9999.0);
}

} // namespace

int main() {
    test_deeply_nested_arrays();
    test_deeply_nested_objects();
    test_alternating_nesting();
    test_unbalanced_deep_nesting_is_rejected();
    test_wide_documents();

    std::puts("stress_deep_tests: OK");
    return 0;
}

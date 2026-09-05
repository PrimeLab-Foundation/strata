/**
 * @file test_stress_deep.cpp
 * @brief Deep-nesting stress for the recursive parser, up to its stated cap.
 *
 * The parser recurses, so nesting is C stack — and strata::kMaxNestingDepth is
 * what keeps that from being a cliff (docs/architecture/SKILL.md, contributor
 * invariant 1). The everyday cases stay at depth 100, as they always have; the
 * cap itself is exercised at the boundary, because a limit nobody parses *at*
 * is a limit nobody knows the stack survives.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_parse.hpp"
#include "strata/json/ndjson_stream.hpp"

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

/// The cap parses *at* the limit, on every container shape, and the tree it
/// builds is walked and destroyed at that depth — the stress the constant's
/// margin is claimed for (docs/architecture/value-cursor.md).
void test_nesting_at_the_limit_parses() {
    const int limit = static_cast<int>(strata::kMaxNestingDepth);

    const auto arrays = parse_json(nested("[", "]", limit, "1"));
    assert(arrays.ok());
    const JsonValue* node = &arrays.value;
    for (int level = 0; level < limit; ++level) {
        assert(node->is_array());
        node = &node->as_array()[0];
    }
    assert(node->as_number() == 1.0);

    const auto objects = parse_json(nested(R"({"a":)", "}", limit, "true"));
    assert(objects.ok());
    const JsonValue* member = &objects.value;
    for (int level = 0; level < limit; ++level) {
        assert(member->is_object());
        member = &member->as_object().at("a");
    }
    assert(member->as_bool());

    // Alternating containers: one level each, limit levels in total.
    std::string mixed;
    std::string closing;
    for (int level = 0; level < limit / 2; ++level) {
        mixed += R"({"a":[)";
        closing = "]}" + closing;
    }
    mixed += "0" + closing;
    assert(parse_json(mixed).ok());
}

/// One container past the limit is refused — as a refusal, not as a syntax
/// error, and not as a crash — however that container is spelled.
void test_nesting_past_the_limit_is_refused() {
    const int past = static_cast<int>(strata::kMaxNestingDepth) + 1;

    const auto arrays = parse_json(nested("[", "]", past, "1"));
    assert(!arrays.ok());
    assert(arrays.status == strata::Status::DepthExceeded);

    const auto objects = parse_json(nested(R"({"a":)", "}", past, "true"));
    assert(objects.status == strata::Status::DepthExceeded);

    // Deeper still, and unbalanced past the cap: still the refusal, because
    // the cap is reached before the document runs out.
    assert(parse_json(nested("[", "]", past * 4, "1")).status == strata::Status::DepthExceeded);
    std::string unbalanced;
    for (int level = 0; level < past; ++level)
        unbalanced += "[";
    assert(parse_json(unbalanced).status == strata::Status::DepthExceeded);

    // A malformed document that is *also* too deep reports the depth, because
    // the cap is what stops the parse first.
    assert(parse_json(nested("[", "]", past, "01")).status == strata::Status::DepthExceeded);
}

/// Nesting inside an NDJSON line is capped like any other document, and the
/// failing line is still named.
void test_ndjson_lines_are_capped() {
    const int past = static_cast<int>(strata::kMaxNestingDepth) + 1;
    std::string text = "[1]\n";
    text += nested("[", "]", past, "2");
    text += "\n[3]\n";

    strata::NdjsonStream stream(text);
    size_t failed_line = 0;
    const auto records = stream.parse_all(/*skip_errors=*/false, &failed_line);
    assert(records.status == strata::Status::DepthExceeded);
    assert(failed_line == 2);

    strata::NdjsonStream skipping(text);
    const auto kept = skipping.parse_all(/*skip_errors=*/true, nullptr);
    assert(kept.ok());
    assert(kept.value.size() == 2);
}

int main() {
    test_deeply_nested_arrays();
    test_deeply_nested_objects();
    test_alternating_nesting();
    test_unbalanced_deep_nesting_is_rejected();
    test_wide_documents();
    test_nesting_at_the_limit_parses();
    test_nesting_past_the_limit_is_refused();
    test_ndjson_lines_are_capped();

    std::puts("stress_deep_tests: OK");
    return 0;
}

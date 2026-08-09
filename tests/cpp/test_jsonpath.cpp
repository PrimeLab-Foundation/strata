/**
 * @file test_jsonpath.cpp
 * @brief Contract suite for the JSONPath compiler and the DOM evaluator.
 *
 * The compiler must never throw: every malformed expression is a
 * Status::ParseError, including the unclosed quoted string that the previous
 * implementation threw on (docs/jsonpath/SKILL.md).
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_document.hpp"
#include "strata/search/jsonpath.hpp"

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

using strata::compile_jsonpath;
using strata::eval_jsonpath;
using strata::is_streamable;
using strata::JsonDocument;
using strata::PathOp;
using strata::Status;

namespace {

constexpr const char* kSample = R"({
    "store": {
        "book": [
            {"title": "Moby Dick", "price": 8.99},
            {"title": "Dune", "price": 12.5}
        ],
        "bicycle": {"color": "red", "price": 19.95}
    },
    "nested": {"price": {"price": 1}}
})";

[[nodiscard]] bool compiles(const char* expression) { return compile_jsonpath(expression).ok(); }

/// Titles (or other strings) selected by @p expression, for easy comparison.
[[nodiscard]] std::vector<std::string> strings_matched(const JsonDocument& document,
                                                       const char* expression) {
    const auto path = compile_jsonpath(expression);
    assert(path.ok());
    std::vector<std::string> out;
    for (const strata::JsonValue* value : eval_jsonpath(path.value, document.root_value())) {
        if (value->is_string())
            out.push_back(value->as_string());
    }
    return out;
}

[[nodiscard]] size_t count_matched(const JsonDocument& document, const char* expression) {
    const auto path = compile_jsonpath(expression);
    assert(path.ok());
    return eval_jsonpath(path.value, document.root_value()).size();
}

void test_the_supported_grammar_compiles() {
    assert(compiles("$"));
    assert(compiles("$.a"));
    assert(compiles("$.a.b.c"));
    assert(compiles("$['a']"));
    assert(compiles(R"($["a"])"));
    assert(compiles("$['a b']"));
    assert(compiles(R"($['a\n\t\r\\\"\'b'])"));
    assert(compiles("$[0]"));
    assert(compiles("$[-1]"));
    assert(compiles("$[*]"));
    assert(compiles("$.*"));
    assert(compiles("$..a"));
    assert(compiles("$[0:2]"));
    assert(compiles("$[:]"));
    assert(compiles("$[1:]"));
    assert(compiles("$[:2]"));
    assert(compiles("$[0:4:2]"));
    assert(compiles("$[-2:-1]"));
    assert(compiles("$[?(@.a == 1)]"));
    assert(compiles("$[?(@.a != 1.5)]"));
    assert(compiles("$[?(@.a > -2)]"));
    assert(compiles("$[?(@.a >= 1e3)]"));
    assert(compiles("$[?(@['a'] == 'text')]"));
    assert(compiles("$[?( @.a  ==  1 )]")); // spaces are tolerated
}

/// Every rejection is a status, never an exception.
void test_invalid_expressions_are_rejected_without_throwing() {
    const char* invalid[] = {
        "",
        "a",
        "$.",
        "$..",
        "$[",
        "$]",
        "$[]",
        "$['x",
        "$[\"x",
        "$['a\\q']",
        "$.a b",
        "$[1,2]",
        "$['a','b']",
        "$..*",
        "$..[0]",
        "$[?(@.a)]",
        "$[?(@.a == )]",
        "$[?(@.a > 'text')]",
        "$[?@.a==1]",
        "$[0:1:0]",
        "$[?(@.a && @.b == 1)]",
        "$.a[",
        "$[?(@ == 1)]",
    };
    for (const char* expression : invalid) {
        const auto result = compile_jsonpath(expression);
        if (result.ok()) {
            std::printf("expected rejection: %s\n", expression);
            assert(false);
        }
        assert(result.status == Status::ParseError);
    }
}

void test_compiled_shape() {
    const auto path = compile_jsonpath("$.store.book[0]");
    assert(path.ok());
    assert(path.value.size() == 4);
    assert(path.value[0].op == PathOp::Root);
    assert(path.value[1].op == PathOp::Field);
    assert(path.value[1].name == "store");
    assert(path.value[3].op == PathOp::Index);
    assert(path.value[3].index == 0);
}

void test_evaluation() {
    const auto document = JsonDocument::from_string(kSample);
    assert(document.ok());
    const JsonDocument& tree = document.value;

    assert(strings_matched(tree, "$.store.bicycle.color") == std::vector<std::string>{"red"});
    assert((strings_matched(tree, "$.store.book[*].title") ==
            std::vector<std::string>{"Moby Dick", "Dune"}));
    assert(strings_matched(tree, "$.store.book[-1].title") == std::vector<std::string>{"Dune"});
    assert(strings_matched(tree, "$..title") == (std::vector<std::string>{"Moby Dick", "Dune"}));
    assert(count_matched(tree, "$.store.book[0:1]") == 1);
    assert(count_matched(tree, "$.store.book[?(@.price > 10)]") == 1);
    assert(count_matched(tree, "$.absent") == 0);
    assert(count_matched(tree, "$.store.book[99]") == 0);
    assert(count_matched(tree, "$.store.book[::-1]") == 0); // negative step selects nothing
}

/// Recursive descent returns a match nested inside a match.
void test_recursive_descent_includes_nested_matches() {
    const auto document = JsonDocument::from_string(kSample);
    assert(document.ok());
    // 8.99, 12.5, 19.95, the {"price": 1} wrapper, and the inner 1.
    assert(count_matched(document.value, "$..price") == 5);
}

/// The gate that keeps a streaming evaluator honest.
void test_streamability_gate() {
    assert(is_streamable(compile_jsonpath("$.a.b[0][*]").value));
    assert(!is_streamable(compile_jsonpath("$..a").value));
    assert(!is_streamable(compile_jsonpath("$.a[0:2]").value));
    assert(!is_streamable(compile_jsonpath("$.a[?(@.b == 1)]").value));
}

} // namespace

int main() {
    test_the_supported_grammar_compiles();
    test_invalid_expressions_are_rejected_without_throwing();
    test_compiled_shape();
    test_evaluation();
    test_recursive_descent_includes_nested_matches();
    test_streamability_gate();

    std::puts("jsonpath_tests: OK");
    return 0;
}

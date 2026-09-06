/**
 * @file test_jsonpath_stream.cpp
 * @brief The streaming evaluator against the DOM evaluator, as a law.
 *
 * `StreamSearchHandler` exists to make `search()` cheaper, never different:
 * for every streamable expression its matches must serialize identically to
 * `eval_jsonpath` over the parsed tree. This suite runs the two against each
 * other across documents crafted to hit every selector kind, every capture
 * shape, and the mid-path dead ends.
 *
 * Duplicate keys are the one place the layers legitimately part ways: the
 * C++ DOM keeps every occurrence (FlatMap appends), while streaming
 * implements the *Python* law -- `search == query(load)` under the FirstWins
 * policy, where later duplicates never exist. Those cases are pinned to the
 * FirstWins expectations explicitly rather than compared against the DOM.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_parse.hpp"
#include "strata/json/json_parser_inline.hpp"
#include "strata/json/json_serialize.hpp"
#include "strata/search/jsonpath.hpp"
#include "strata/search/jsonpath_stream.hpp"
#include "strata/util/fast_parse.hpp"

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

using strata::compile_jsonpath;
using strata::eval_jsonpath;
using strata::is_streamable;
using strata::JsonValue;
using strata::parse_json;
using strata::parse_sax_inline;
using strata::serialize_json;
using strata::Status;
using strata::StreamSearchHandler;

namespace {

/// Builds each match as a JsonValue; an independent reimplementation so the
/// law is checked between two genuinely different constructions.
class DomSink {
  public:
    std::vector<JsonValue> results;

    bool on_null() { return push(JsonValue()); }
    bool on_bool(bool value) { return push(JsonValue(JsonValue::Variant(value))); }
    bool on_int(int64_t value) {
        // The C++ DOM stores every number as double; mirror that exactly.
        return push(JsonValue(JsonValue::Variant(static_cast<double>(value))));
    }
    bool on_big_int(std::string_view text) {
        // The DOM builder widens big integers through the double path too.
        double widened = 0.0;
        (void)strata::util::from_chars_double(text.data(), text.data() + text.size(), widened);
        return push(JsonValue(JsonValue::Variant(widened)));
    }
    bool on_double(double value) { return push(JsonValue(JsonValue::Variant(value))); }
    bool on_string(std::string_view value) {
        return push(JsonValue(JsonValue::Variant(std::string(value))));
    }

    bool on_key(std::string_view key) {
        keys_.emplace_back(key);
        return true;
    }

    bool on_start_object() {
        open_.push_back(Open{JsonValue(JsonValue::Variant(JsonValue::Object{})), true});
        return true;
    }
    bool on_start_array() {
        open_.push_back(Open{JsonValue(JsonValue::Variant(JsonValue::Array{})), false});
        return true;
    }
    bool on_end_object() { return close(); }
    bool on_end_array() { return close(); }

    [[nodiscard]] bool finish_value() {
        if (!finished_ || !open_.empty())
            return false;
        results.push_back(std::move(finished_value_));
        finished_ = false;
        return true;
    }

  private:
    struct Open {
        JsonValue value;
        bool is_object;
    };

    bool push(JsonValue value) {
        if (open_.empty()) {
            finished_value_ = std::move(value);
            finished_ = true;
            return true;
        }
        Open& top = open_.back();
        if (top.is_object) {
            if (keys_.empty())
                return false;
            top.value.as_object().emplace(std::move(keys_.back()), std::move(value));
            keys_.pop_back();
        } else {
            top.value.as_array().push_back(std::move(value));
        }
        return true;
    }

    bool close() {
        if (open_.empty())
            return false;
        JsonValue value = std::move(open_.back().value);
        open_.pop_back();
        return push(std::move(value));
    }

    std::vector<Open> open_;
    std::vector<std::string> keys_;
    JsonValue finished_value_;
    bool finished_ = false;
};

/// Run the streaming evaluator over @p text; empty optional on parse failure.
[[nodiscard]] bool stream_search(const std::string& text, const char* expression,
                                 std::vector<std::string>& out) {
    const auto compiled = compile_jsonpath(expression);
    assert(compiled.ok());
    assert(is_streamable(compiled.value));

    DomSink sink;
    StreamSearchHandler<DomSink> handler(compiled.value, sink);
    if (parse_sax_inline(std::string_view(text), handler, true) != Status::Ok)
        return false;
    out.clear();
    for (const JsonValue& match : sink.results)
        out.push_back(serialize_json(match));
    return true;
}

/// The law: streaming matches == DOM-eval matches, value for value.
void check_law(const std::string& text, const char* expression) {
    std::vector<std::string> streamed;
    const bool ok = stream_search(text, expression, streamed);
    assert(ok);

    const auto tree = parse_json(text);
    assert(tree.ok());
    const auto compiled = compile_jsonpath(expression);
    assert(compiled.ok());
    const auto matches = eval_jsonpath(compiled.value, tree.value);

    if (streamed.size() != matches.size()) {
        std::printf("law break: %s on %s -> %zu streamed vs %zu eval\n", expression, text.c_str(),
                    streamed.size(), matches.size());
        assert(false);
    }
    for (size_t index = 0; index < matches.size(); ++index) {
        const std::string expected = serialize_json(*matches[index]);
        if (streamed[index] != expected) {
            std::printf("law break: %s on %s at %zu: %s vs %s\n", expression, text.c_str(), index,
                        streamed[index].c_str(), expected.c_str());
            assert(false);
        }
    }
}

void test_every_selector_shape() {
    const std::string doc = R"({
        "users": [
            {"id": 1, "name": "ada", "tags": ["a", "b"], "address": {"city": "x"}},
            {"id": 2, "name": "bob", "tags": [], "address": {"city": "y"}},
            {"id": 3, "extra": true, "address": {"city": "z", "zip": 9}}
        ],
        "count": 3,
        "meta": {"users": {"id": 99}}
    })";
    for (const char* expression :
         {"$.count", "$.users", "$.users[0]", "$.users[2]", "$.users[9]", "$.users[*].id",
          "$.users[*].name", "$.users[*].address.city", "$.users[*].address", "$.users[1].tags",
          "$.users[*].tags[0]", "$.users[*].tags[*]", "$.*", "$.meta.users.id", "$[0]",
          "$.users[*].missing", "$.missing[*].x"}) {
        check_law(doc, expression);
    }
}

void test_scalar_and_container_matches_interleave() {
    // Wildcard over mixed value kinds: scalars match directly, containers
    // capture, and both arrive in document order.
    check_law(R"({"a": 1, "b": [2, 3], "c": {"d": 4}, "e": "five", "f": null, "g": true})", "$.*");
    check_law(R"([[1], 2, {"x": 3}, "four", [], {}])", "$[*]");
    check_law(R"([[[1, 2], [3]], [[4]]])", "$[*][*][*]");
}

void test_deep_and_edge_documents() {
    check_law("[]", "$[*]");
    check_law("{}", "$.*");
    check_law("[0]", "$[0]");
    check_law(R"({"a": {"a": {"a": 1}}})", "$.a.a.a");
    // Big ints inside matches keep their digits (serialized via raw span).
    check_law(R"([{"n": 123456789012345678901234567890}])", "$[*].n");
    // Unicode keys and values pass through capture untouched.
    check_law("[{\"k\\u00e9y\": \"caf\\u00e9\"}]", "$[*]");
}

void test_duplicate_keys_pin_firstwins() {
    // The Python law's semantics, pinned directly: first occurrence wins for
    // a Field selector, and a Wildcard yields one value per distinct key.
    std::vector<std::string> out;

    assert(stream_search(R"({"a": 1, "a": 2})", "$.a", out));
    assert(out.size() == 1 && out[0] == "1.0"); // this sink is the double DOM

    assert(stream_search(R"({"a": 1, "b": 2, "a": 3})", "$.*", out));
    assert(out.size() == 2 && out[0] == "1.0" && out[1] == "2.0");

    // Inside a captured subtree the sink's builder decides; this DomSink
    // keeps both (FlatMap semantics) -- the Python sink applies the dict
    // policy instead. What streaming itself guarantees is the *selection*.
    assert(stream_search(R"([{"a": 1, "a": 2}])", "$[0]", out));
    assert(out.size() == 1);
}

void test_invalid_documents_fail_after_matches() {
    // A match followed by garbage must fail the whole search: query(load)
    // would have raised, so the law demands the same here.
    std::vector<std::string> out;
    assert(!stream_search(R"([{"id": 1}] trailing)", "$[*].id", out));
    assert(!stream_search(R"([{"id": 1}, {"id": )", "$[*].id", out));
}

void test_streamability_gate() {
    assert(is_streamable(compile_jsonpath("$[*].a").value));
    assert(is_streamable(compile_jsonpath("$.a[0].b").value));
    assert(!is_streamable(compile_jsonpath("$").value));      // whole document
    assert(!is_streamable(compile_jsonpath("$[-1]").value));  // needs the end
    assert(!is_streamable(compile_jsonpath("$..a").value));   // descends into matches
    assert(!is_streamable(compile_jsonpath("$[0:2]").value)); // sibling knowledge
    assert(!is_streamable(compile_jsonpath("$[?(@.a > 1)]").value));
}

/// The streaming leg is capped like every other parse: the law is
/// `search == query(load)`, and `load` refuses a document nested past
/// strata::kMaxNestingDepth, so the stream must refuse it identically -- with
/// DepthExceeded, not ParseError, and whatever it had already matched.
void test_nesting_past_the_limit_is_refused() {
    const auto compiled = compile_jsonpath("$[*].id");
    assert(compiled.ok());

    std::string document = R"([{"id": 1}, )";
    for (size_t level = 0; level <= strata::kMaxNestingDepth; ++level)
        document += "[";
    document += "2";
    for (size_t level = 0; level <= strata::kMaxNestingDepth; ++level)
        document += "]";
    document += "]";

    DomSink sink;
    StreamSearchHandler<DomSink> handler(compiled.value, sink);
    assert(parse_sax_inline(std::string_view(document), handler, true) == Status::DepthExceeded);
    // The same document through the DOM evaluator's own parse: same verdict.
    assert(parse_json(document).status == Status::DepthExceeded);

    // Exactly at the limit both legs accept it.
    std::string at_limit = R"([{"id": 1}, )";
    for (size_t level = 0; level + 1 < strata::kMaxNestingDepth; ++level)
        at_limit += "[";
    at_limit += "2";
    for (size_t level = 0; level + 1 < strata::kMaxNestingDepth; ++level)
        at_limit += "]";
    at_limit += "]";

    DomSink accepted;
    StreamSearchHandler<DomSink> accepting(compiled.value, accepted);
    assert(parse_sax_inline(std::string_view(at_limit), accepting, true) == Status::Ok);
    assert(accepted.results.size() == 1);
    assert(parse_json(at_limit).ok());
}

} // namespace

int main() {
    test_every_selector_shape();
    test_scalar_and_container_matches_interleave();
    test_deep_and_edge_documents();
    test_duplicate_keys_pin_firstwins();
    test_invalid_documents_fail_after_matches();
    test_streamability_gate();
    test_nesting_past_the_limit_is_refused();

    std::puts("jsonpath_stream_tests: OK");
    return 0;
}

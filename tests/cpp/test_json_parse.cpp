/**
 * @file test_json_parse.cpp
 * @brief Contract suite for the SAX parser and DOM builder.
 *
 * Pins the strictness contract in docs/context/api.md — invalid UTF-8, lone
 * surrogates, leading zeros, trailing garbage, raw control characters — plus
 * number classification, string decoding, and the duplicate-key policies.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_parse.hpp"
#include "strata/json/json_parser_inline.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

using strata::DuplicateKeyPolicy;
using strata::JsonSaxHandler;
using strata::JsonValue;
using strata::parse_json;
using strata::parse_sax;
using strata::Status;

namespace {

[[nodiscard]] bool parses(std::string_view text) { return parse_json(text).ok(); }

[[nodiscard]] bool rejects(std::string_view text) {
    const auto result = parse_json(text);
    return !result.ok() && result.status == Status::ParseError;
}

/// Records the event stream so tests can assert on what the parser emitted.
class RecordingHandler final : public JsonSaxHandler {
  public:
    std::vector<std::string> events;

    bool on_null() override { return record("null"); }
    bool on_bool(bool value) override { return record(value ? "bool:true" : "bool:false"); }
    bool on_int(int64_t value) override { return record("int:" + std::to_string(value)); }
    bool on_big_int(std::string_view text) override {
        return record("bigint:" + std::string(text));
    }
    bool on_double(double value) override { return record("double:" + std::to_string(value)); }
    bool on_string(std::string_view value) override { return record("str:" + std::string(value)); }
    bool on_start_object() override { return record("{"); }
    bool on_key(std::string_view key) override { return record("key:" + std::string(key)); }
    bool on_end_object() override { return record("}"); }
    bool on_start_array() override { return record("["); }
    bool on_end_array() override { return record("]"); }

  private:
    bool record(std::string event) {
        events.push_back(std::move(event));
        return true;
    }
};

/// Aborts the parse at its first scalar, to prove a false return stops parsing.
class AbortingHandler final : public JsonSaxHandler {
  public:
    bool on_null() override { return false; }
    bool on_bool(bool) override { return false; }
    bool on_int(int64_t) override { return false; }
    bool on_big_int(std::string_view) override { return false; }
    bool on_double(double) override { return false; }
    bool on_string(std::string_view) override { return false; }
    bool on_start_object() override { return true; }
    bool on_key(std::string_view) override { return true; }
    bool on_end_object() override { return true; }
    bool on_start_array() override { return true; }
    bool on_end_array() override { return true; }
};

[[nodiscard]] std::vector<std::string> events_of(std::string_view text) {
    RecordingHandler handler;
    const Status status = parse_sax(text, handler);
    assert(status == Status::Ok);
    return handler.events;
}

// ---------------------------------------------------------------------------
// Literals and structure
// ---------------------------------------------------------------------------

void test_literals() {
    assert(parse_json("null").value.is_null());
    assert(parse_json("true").value.as_bool());
    assert(!parse_json("false").value.as_bool());

    // Prefixes and near-misses are not literals.
    assert(rejects("nul"));
    assert(rejects("nulll"));
    assert(rejects("tru"));
    assert(rejects("truex"));
    assert(rejects("False"));
    assert(rejects("NaN"));
    assert(rejects("Infinity"));
    assert(rejects("-Infinity"));
}

void test_structure() {
    assert(parse_json("[]").value.as_array().empty());
    assert(parse_json("{}").value.as_object().empty());

    const auto array = parse_json("[1, 2, 3]");
    assert(array.ok());
    assert(array.value.as_array().size() == 3);
    assert(array.value.as_array()[2].as_number() == 3.0);

    const auto object = parse_json(R"({"a": 1, "b": [true, null], "c": {"d": "e"}})");
    assert(object.ok());
    const auto& root = object.value.as_object();
    assert(root.size() == 3);
    assert(root.at("a").as_number() == 1.0);
    assert(root.at("b").as_array().size() == 2);
    assert(root.at("c").as_object().at("d").as_string() == "e");

    // Insertion order survives the parse.
    assert(root.begin()->first == "a");
}

void test_whitespace_is_tolerated_between_tokens() {
    assert(parses(" \t\r\n {\n \"a\" : \t [ 1 , 2 ] \r\n } \n "));
    assert(parses("[]"));
    // ...but not inside tokens.
    assert(rejects("nu ll"));
    assert(rejects("1 2"));
}

void test_trailing_content_is_rejected() {
    assert(rejects("{} {}"));
    assert(rejects("[1] junk"));
    assert(rejects("null null"));
    assert(rejects("1,"));
}

void test_malformed_structure_is_rejected() {
    assert(rejects(""));
    assert(rejects("   "));
    assert(rejects("{invalid}"));
    assert(rejects("{\"a\":"));
    assert(rejects("{\"a\" 1}"));
    assert(rejects("{a: 1}"));
    assert(rejects("{'a': 1}"));
    assert(rejects("["));
    assert(rejects("{"));
    assert(rejects("[1,]"));
    assert(rejects("[,1]"));
    assert(rejects("[1 2]"));
    assert(rejects(R"({"a":1,})"));
    assert(rejects(R"({"a":1 "b":2})"));
    assert(rejects("}"));
    assert(rejects("]"));
}

// ---------------------------------------------------------------------------
// Numbers
// ---------------------------------------------------------------------------

void test_number_classification() {
    assert(events_of("0") == std::vector<std::string>{"int:0"});
    assert(events_of("-7") == std::vector<std::string>{"int:-7"});
    assert(events_of("1.5").at(0).rfind("double:", 0) == 0);
    assert(events_of("1e3").at(0).rfind("double:", 0) == 0);
    assert(events_of("1E+3").at(0).rfind("double:", 0) == 0);
    // An integral value written with an exponent is still a double token.
    assert(events_of("10e2").at(0).rfind("double:", 0) == 0);
}

void test_int64_boundaries_stay_integers() {
    assert(events_of("9223372036854775807") == std::vector<std::string>{"int:9223372036854775807"});
    assert(events_of("-9223372036854775808") ==
           std::vector<std::string>{"int:-9223372036854775808"});
}

/**
 * Integers past int64 are reported verbatim, never truncated.
 *
 * The previous implementation accumulated only the first 19 digits and
 * returned that as the value, so a 20-digit literal silently parsed to a
 * different number (docs/context/api.md: "do not reproduce").
 */
void test_big_integers_are_reported_exactly() {
    assert(events_of("9223372036854775808") ==
           std::vector<std::string>{"bigint:9223372036854775808"});
    assert(events_of("-9223372036854775809") ==
           std::vector<std::string>{"bigint:-9223372036854775809"});
    assert(events_of("12345678901234567890") ==
           std::vector<std::string>{"bigint:12345678901234567890"});

    const std::string huge(400, '9');
    assert(events_of(huge) == std::vector<std::string>{"bigint:" + huge});

    // In the DOM the value widens to double: lossy, but the right magnitude —
    // emphatically not the 19-digit truncation 1234567890123456789.
    const auto parsed = parse_json("12345678901234567890");
    assert(parsed.ok());
    assert(parsed.value.as_number() == 12345678901234567890.0);
    assert(parsed.value.as_number() != 1234567890123456789.0);
}

void test_number_grammar_is_strict() {
    // Leading zeros
    assert(rejects("01"));
    assert(rejects("-01"));
    assert(rejects("00"));
    assert(rejects("[01]"));
    assert(parses("0"));
    assert(parses("-0"));
    assert(parses("0.5"));

    // Incomplete fractions and exponents
    assert(rejects("1."));
    assert(rejects(".1"));
    assert(rejects("-"));
    assert(rejects("-."));
    assert(rejects("1e"));
    assert(rejects("1e+"));
    assert(rejects("1e-"));
    assert(rejects("1.e5"));
    assert(rejects("+1"));
    assert(rejects("0x10"));

    assert(parses("1e5"));
    assert(parses("1E5"));
    assert(parses("1e+5"));
    assert(parses("1e-5"));
    assert(parses("-1.5e-5"));
}

void test_extreme_exponents_saturate_like_stdlib() {
    const auto overflow = parse_json("1e400");
    assert(overflow.ok());
    assert(std::isinf(overflow.value.as_number()));
    assert(overflow.value.as_number() > 0);

    const auto negative_overflow = parse_json("-1e400");
    assert(negative_overflow.ok());
    assert(std::isinf(negative_overflow.value.as_number()));
    assert(negative_overflow.value.as_number() < 0);

    const auto underflow = parse_json("1e-400");
    assert(underflow.ok());
    assert(underflow.value.as_number() == 0.0);
}

void test_double_values_round_trip() {
    assert(parse_json("0.1").value.as_number() == 0.1);
    assert(parse_json("-0.0").value.as_number() == 0.0);
    assert(std::signbit(parse_json("-0.0").value.as_number()));
    assert(parse_json("1.7976931348623157e308").value.as_number() ==
           std::numeric_limits<double>::max());
    assert(parse_json("3.141592653589793").value.as_number() == 3.141592653589793);
}

// ---------------------------------------------------------------------------
// Strings
// ---------------------------------------------------------------------------

void test_plain_and_escaped_strings() {
    assert(parse_json(R"("")").value.as_string().empty());
    assert(parse_json(R"("hello")").value.as_string() == "hello");
    assert(parse_json(R"("tab\there")").value.as_string() == "tab\there");
    assert(parse_json(R"("q\"q")").value.as_string() == "q\"q");
    assert(parse_json(R"("back\\slash")").value.as_string() == "back\\slash");
    assert(parse_json(R"("sol\/idus")").value.as_string() == "sol/idus");
    assert(parse_json(R"("\b\f\n\r\t")").value.as_string() == "\b\f\n\r\t");

    // The clean prefix before an escape is preserved.
    assert(parse_json(R"("prefix\nsuffix")").value.as_string() == "prefix\nsuffix");
}

void test_unicode_escapes_and_surrogate_pairs() {
    assert(parse_json(R"("\u0041")").value.as_string() == "A");
    assert(parse_json(R"("\u00e9")").value.as_string() == "\xc3\xa9");                       // é
    assert(parse_json(R"("\u4f60\u597d")").value.as_string() == "\xe4\xbd\xa0\xe5\xa5\xbd"); // 你好
    // U+1F600, encoded as a UTF-16 surrogate pair.
    assert(parse_json(R"("\ud83d\ude00")").value.as_string() == "\xf0\x9f\x98\x80");
    assert(parse_json(R"("\u0000")").value.as_string() == std::string(1, '\0'));
}

void test_string_grammar_is_strict() {
    assert(rejects(R"("unterminated)"));
    assert(rejects("\"bad escape \\x\""));
    assert(rejects("\"short \\u12\""));
    assert(rejects("\"short \\u\""));
    assert(rejects("\"\\uZZZZ\""));

    // Raw control characters must be escaped.
    assert(rejects(std::string("\"a\x01"
                               "b\"")));
    assert(rejects(std::string("\"line\nbreak\"")));
    assert(rejects(std::string("\"tab\there\"")));
}

void test_lone_surrogates_are_rejected() {
    assert(rejects(R"("\ud800")"));       // high surrogate, nothing follows
    assert(rejects(R"("\udc00")"));       // lone low surrogate
    assert(rejects(R"("\ud800\u0041")")); // high followed by a non-surrogate
    assert(rejects(R"("\ud800\ud800")")); // high followed by another high
    assert(rejects(R"("\udfff")"));
    assert(parses(R"("\ud800\udc00")")); // a valid pair still works
}

void test_invalid_utf8_is_rejected() {
    // Valid sequences parse.
    assert(parses("\"caf\xc3\xa9\""));
    assert(parses("\"\xe4\xbd\xa0\xe5\xa5\xbd\""));
    assert(parses("\"\xf0\x9f\x98\x80\""));

    assert(rejects("\"a\x80"
                   "b\""));                  // continuation byte with no lead
    assert(rejects("\"\xc0\x80\""));         // overlong NUL
    assert(rejects("\"\xc1\xbf\""));         // overlong
    assert(rejects("\"\xff\""));             // invalid lead byte
    assert(rejects("\"\xc2\""));             // truncated two-byte
    assert(rejects("\"\xe0\x80\x80\""));     // overlong three-byte
    assert(rejects("\"\xed\xa0\x80\""));     // encoded surrogate U+D800
    assert(rejects("\"\xf0\x80\x80\x80\"")); // overlong four-byte
    assert(rejects("\"\xf4\x90\x80\x80\"")); // above U+10FFFF
    assert(rejects("\"\xf5\x80\x80\x80\"")); // lead byte out of range
    assert(rejects("\"\xe4\xbd\""));         // truncated three-byte
}

/**
 * Where strata is deliberately stricter than stdlib `json`.
 *
 * Established by diffing the parser against `json.loads` over a generated
 * corpus: these were the only disagreements, and each is intended.
 */
void test_deliberate_divergences_from_stdlib_json() {
    // stdlib accepts NaN/Infinity as an extension; they are not JSON.
    assert(rejects("NaN"));
    assert(rejects("Infinity"));

    // stdlib decodes bytes with the "surrogatepass" handler, so it accepts
    // both escaped lone surrogates and their CESU-8 encoding. Strata does not.
    assert(rejects(R"("\ud800")"));
    assert(rejects("\"\xed\xa0\x80\""));

    // stdlib strips a UTF-8 BOM from bytes input. A BOM is not part of a JSON
    // text, so strata treats it as what it is: bytes before the value.
    assert(rejects("\xef\xbb\xbf{}"));
    assert(rejects("\xef\xbb\xbf"
                   "null"));
    assert(parses("{}"));
    // ...and U+FEFF inside a string is ordinary content.
    assert(parses("\"\xef\xbb\xbf\""));
}

/// Skipping validation is opt-in, and it is the caller's problem afterwards.
void test_utf8_validation_can_be_skipped() {
    RecordingHandler handler;
    const std::string_view invalid = "\"a\x80\"";
    assert(parse_sax(invalid, handler, true) == Status::ParseError);

    RecordingHandler unchecked;
    assert(parse_sax(invalid, unchecked, false) == Status::Ok);
}

// ---------------------------------------------------------------------------
// SAX layer
// ---------------------------------------------------------------------------

void test_event_stream_shape() {
    const auto events = events_of(R"({"a":[1,null],"b":true})");
    const std::vector<std::string> expected{
        "{", "key:a", "[", "int:1", "null", "]", "key:b", "bool:true", "}",
    };
    assert(events == expected);
}

void test_a_handler_returning_false_aborts_the_parse() {
    AbortingHandler handler;
    assert(parse_sax("[1,2,3]", handler) == Status::ParseError);
    assert(parse_sax("null", handler) == Status::ParseError);
}

// ---------------------------------------------------------------------------
// Duplicate keys
// ---------------------------------------------------------------------------

void test_duplicate_key_policies() {
    const std::string_view text = R"({"a":1,"a":2,"b":3})";
    const DuplicateKeyPolicy previous = strata::get_duplicate_key_policy();

    strata::set_duplicate_key_policy(DuplicateKeyPolicy::FirstWins);
    assert(strata::get_duplicate_key_policy() == DuplicateKeyPolicy::FirstWins);
    auto first = parse_json(text);
    assert(first.ok());
    assert(first.value.as_object().size() == 2);
    assert(first.value.as_object().at("a").as_number() == 1.0);

    strata::set_duplicate_key_policy(DuplicateKeyPolicy::LastWins);
    auto last = parse_json(text);
    assert(last.ok());
    assert(last.value.as_object().size() == 2);
    assert(last.value.as_object().at("a").as_number() == 2.0);

    strata::set_duplicate_key_policy(DuplicateKeyPolicy::Error);
    assert(!parse_json(text).ok());

    strata::set_duplicate_key_policy(DuplicateKeyPolicy::Warn);
    auto warned = parse_json(text);
    assert(warned.ok());
    assert(warned.value.as_object().at("a").as_number() == 1.0);
    const auto warnings = strata::consume_parse_warnings();
    assert(warnings.size() == 1);
    assert(warnings[0].find("a") != std::string::npos);
    assert(strata::consume_parse_warnings().empty()); // consuming clears

    strata::set_duplicate_key_policy(previous);
    assert(strata::get_duplicate_key_policy() == DuplicateKeyPolicy::FirstWins);
}

} // namespace

int main() {
    test_literals();
    test_structure();
    test_whitespace_is_tolerated_between_tokens();
    test_trailing_content_is_rejected();
    test_malformed_structure_is_rejected();

    test_number_classification();
    test_int64_boundaries_stay_integers();
    test_big_integers_are_reported_exactly();
    test_number_grammar_is_strict();
    test_extreme_exponents_saturate_like_stdlib();
    test_double_values_round_trip();

    test_plain_and_escaped_strings();
    test_unicode_escapes_and_surrogate_pairs();
    test_string_grammar_is_strict();
    test_lone_surrogates_are_rejected();
    test_invalid_utf8_is_rejected();
    test_deliberate_divergences_from_stdlib_json();
    test_utf8_validation_can_be_skipped();

    test_event_stream_shape();
    test_a_handler_returning_false_aborts_the_parse();
    test_duplicate_key_policies();

    std::puts("json_parse_tests: OK");
    return 0;
}

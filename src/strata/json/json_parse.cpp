/**
 * @file json_parse.cpp
 * @brief DOM construction on top of the SAX parser.
 *
 * DomBuilderHandler turns the event stream into a JsonValue tree, and
 * parse_json() drives it through ParserInline<DomBuilderHandler> so every
 * callback is inlined rather than dispatched virtually.
 *
 * Thread-local state: the duplicate-key policy and the warning list. Settings
 * made on one thread do not reach another — that asymmetry is part of the
 * documented contract.
 */

#include "strata/json/json_parse.hpp"

#include "strata/json/json_parser_inline.hpp"
#include "strata/util/fast_parse.hpp"

#include <limits>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

namespace strata {

namespace {

thread_local DuplicateKeyPolicy g_duplicate_policy = DuplicateKeyPolicy::FirstWins;
thread_local std::vector<std::string> g_parse_warnings;

/// Widen an integer literal that outgrew int64_t into the DOM's double.
[[nodiscard]] double big_int_to_double(std::string_view text) noexcept {
    double value = 0.0;
    const auto result = util::from_chars_double(text.data(), text.data() + text.size(), value);
    if (result.ec != std::errc{}) {
        // Out of double's range — or, on the strtod twin only, the token copy
        // failed to allocate. Saturate in the literal's direction either way:
        // any BigInt is far above int64, so ±infinity is the honest bound and
        // a fabricated zero would be silently wrong.
        value = std::numeric_limits<double>::infinity();
        if (!text.empty() && text.front() == '-')
            value = -value;
    }
    return value;
}

/**
 * SAX handler that builds a JsonValue tree.
 *
 * Containers under construction live on `stack_`; every completed value is
 * pushed into the container on top, or becomes the root when the stack is
 * empty. Keys wait on `keys_` between on_key() and the value that follows.
 */
class DomBuilderHandler final : public JsonSaxHandler {
  public:
    bool on_null() override { return push_value(JsonValue()); }

    bool on_bool(bool value) override { return push_value(JsonValue(JsonValue::Variant(value))); }

    bool on_int(int64_t value) override {
        return push_value(JsonValue(JsonValue::Variant(static_cast<double>(value))));
    }

    bool on_big_int(std::string_view text) override {
        return push_value(JsonValue(JsonValue::Variant(big_int_to_double(text))));
    }

    bool on_double(double value) override {
        return push_value(JsonValue(JsonValue::Variant(value)));
    }

    bool on_string(std::string_view value) override {
        return push_value(JsonValue(JsonValue::Variant(std::string(value))));
    }

    bool on_start_object() override {
        stack_.emplace_back(JsonValue::Variant(JsonValue::Object()));
        return true;
    }

    bool on_key(std::string_view key) override {
        keys_.emplace_back(key);
        return true;
    }

    bool on_end_object() override { return close_container(); }

    bool on_start_array() override {
        stack_.emplace_back(JsonValue::Variant(JsonValue::Array()));
        return true;
    }

    bool on_end_array() override { return close_container(); }

    [[nodiscard]] JsonValue take_root() noexcept { return std::move(root_); }

  private:
    bool close_container() {
        if (stack_.empty())
            return false;
        JsonValue container = std::move(stack_.back());
        stack_.pop_back();
        return push_value(std::move(container));
    }

    bool push_value(JsonValue&& value) {
        if (stack_.empty()) {
            root_ = std::move(value);
            return true;
        }

        JsonValue& top = stack_.back();
        if (top.is_array()) {
            top.as_array().push_back(std::move(value));
            return true;
        }
        if (!top.is_object())
            return false;

        if (keys_.empty())
            return false;
        std::string key = std::move(keys_.back());
        keys_.pop_back();

        JsonValue::Object& object = top.as_object();
        auto existing = object.find(key);
        if (existing == object.end()) {
            // FlatMap::emplace does not deduplicate; the lookup above is what
            // makes that safe, and is where the policy applies.
            object.emplace(std::move(key), std::move(value));
            return true;
        }

        switch (g_duplicate_policy) {
        case DuplicateKeyPolicy::FirstWins:
            return true;
        case DuplicateKeyPolicy::Warn:
            g_parse_warnings.push_back("Duplicate key encountered: " + key);
            return true;
        case DuplicateKeyPolicy::LastWins:
            existing->second = std::move(value);
            return true;
        case DuplicateKeyPolicy::Error:
            return false;
        }
        return false;
    }

    JsonValue root_;
    std::vector<JsonValue> stack_;
    std::vector<std::string> keys_;
};

} // namespace

Result<JsonValue> parse_json(std::string_view text) {
    g_parse_warnings.clear();
    DomBuilderHandler handler;
    const Status status = parse_sax_inline(text, handler);
    if (status != Status::Ok)
        return {status, JsonValue{}};
    return {Status::Ok, handler.take_root()};
}

Status parse_sax(std::string_view text, JsonSaxHandler& handler, bool validate_utf8) {
    return parse_sax_inline(text, handler, validate_utf8);
}

void set_duplicate_key_policy(DuplicateKeyPolicy policy) noexcept { g_duplicate_policy = policy; }

DuplicateKeyPolicy get_duplicate_key_policy() noexcept { return g_duplicate_policy; }

std::vector<std::string> consume_parse_warnings() {
    std::vector<std::string> warnings = std::move(g_parse_warnings);
    g_parse_warnings.clear();
    return warnings;
}

} // namespace strata

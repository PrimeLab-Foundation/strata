/**
 * @file json_parse.cpp
 * @brief Implementation of parse_json() and parse_sax().
 *
 * Uses ParserInline<DomBuilderHandler> for DOM construction, which
 * lets the compiler devirtualise every SAX callback.
 *
 * Thread-local state:
 *   g_duplicate_policy — controls how duplicate object keys are handled.
 *   g_parse_warnings   — collects warnings (e.g. duplicate keys in Warn mode).
 *                         Cleared at the start of each parse_json() call.
 */

#include "strata/json/json_parse.hpp"

#include "strata/json/json_parser_inline.hpp"

#include <cctype>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace strata {

namespace {

/// Thread-local duplicate-key policy (default: last occurrence wins).
/// LastWins is the standard JSON behavior (RFC 8259), matches Python's
/// json.loads(), and enables the _PyDict_SetItem_KnownHash fast path.
thread_local DuplicateKeyPolicy g_duplicate_policy = DuplicateKeyPolicy::LastWins;

/// Thread-local vector of parse warnings, consumed via consume_parse_warnings().
thread_local std::vector<std::string> g_parse_warnings;

/**
 * SAX handler that builds a full JsonValue DOM tree.
 *
 * Uses a stack of in-progress containers (objects/arrays).  When a
 * value is produced it is pushed into the top-of-stack container;
 * when the stack is empty the value becomes the root.
 */
class DomBuilderHandler : public JsonSaxHandler {
  public:
    DomBuilderHandler() = default;

    bool on_null() override { return push_value(JsonValue()); }
    bool on_bool(bool v) override { return push_value(JsonValue(JsonValue::Variant(v))); }
    bool on_int(int64_t v) override {
        return push_value(JsonValue(JsonValue::Variant(static_cast<double>(v))));
    }
    bool on_uint(uint64_t v) override {
        return push_value(JsonValue(JsonValue::Variant(static_cast<double>(v))));
    }
    bool on_double(double v) override { return push_value(JsonValue(JsonValue::Variant(v))); }
    bool on_string(std::string_view v) override {
        return push_value(JsonValue(JsonValue::Variant(std::string(v))));
    }

    bool on_start_object(size_t) override {
        stack_.emplace_back(JsonValue::Variant(JsonValue::Object()));
        return true;
    }

    bool on_key(std::string_view v) override {
        keys_.emplace_back(std::string(v));
        return true;
    }

    bool on_end_object() override {
        if (stack_.empty())
            return false;
        auto obj = std::move(stack_.back());
        stack_.pop_back();
        return push_value(std::move(obj));
    }

    bool on_start_array(size_t) override {
        stack_.emplace_back(JsonValue::Variant(JsonValue::Array()));
        return true;
    }

    bool on_end_array() override {
        if (stack_.empty())
            return false;
        auto arr = std::move(stack_.back());
        stack_.pop_back();
        return push_value(std::move(arr));
    }

    JsonValue&& take_root() { return std::move(root_); }

  private:
    bool push_value(JsonValue&& val) {
        if (stack_.empty()) {
            root_ = std::move(val);
            return true;
        }

        auto& top = stack_.back();
        if (std::holds_alternative<JsonValue::Array>(top.data)) {
            std::get<JsonValue::Array>(top.data).push_back(std::move(val));
            return true;
        } else if (std::holds_alternative<JsonValue::Object>(top.data)) {
            if (keys_.empty())
                return false;
            auto& obj = std::get<JsonValue::Object>(top.data);
            std::string key = std::move(keys_.back());
            keys_.pop_back();

            auto existing = obj.find(key);
            if (existing != obj.end()) {
                switch (g_duplicate_policy) {
                case DuplicateKeyPolicy::FirstWins:
                    break;
                case DuplicateKeyPolicy::Warn:
                    g_parse_warnings.push_back("Duplicate key encountered: " + key);
                    break;
                case DuplicateKeyPolicy::LastWins:
                    existing->second = std::move(val);
                    break;
                case DuplicateKeyPolicy::Error:
                    return false;
                }
            } else {
                obj.emplace(std::move(key), std::move(val));
            }
            return true;
        }
        return false;
    }

    JsonValue root_;
    std::vector<JsonValue> stack_;
    std::vector<std::string> keys_;
};

// Parser is now in json_parser_inline.hpp as a template (ParserInline<Handler>).
// This enables devirtualisation when instantiated with a concrete handler type.

} // namespace

Result<JsonValue> parse_json(std::string_view text) {
    g_parse_warnings.clear();
    DomBuilderHandler handler;
    Status status = parse_sax_inline(text, handler);
    if (status != Status::Ok) {
        return {status, JsonValue{}};
    }
    return {Status::Ok, handler.take_root()};
}

Status parse_sax(std::string_view text, JsonSaxHandler& handler, bool validate_utf8) {
    // Thin wrapper: uses the templated parser with JsonSaxHandler (virtual dispatch).
    // For concrete handler types, callers should use parse_sax_inline<T> directly.
    return parse_sax_inline(text, handler, validate_utf8);
}

void set_duplicate_key_policy(DuplicateKeyPolicy policy) { g_duplicate_policy = policy; }

DuplicateKeyPolicy get_duplicate_key_policy() { return g_duplicate_policy; }

std::vector<std::string> consume_parse_warnings() {
    std::vector<std::string> warnings = std::move(g_parse_warnings);
    g_parse_warnings.clear();
    return warnings;
}

} // namespace strata

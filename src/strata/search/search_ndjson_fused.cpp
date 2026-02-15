#include "strata/search/search_ndjson_fused.hpp"
#include "strata/search/simple_field_extractor.hpp"
#include "strata/util/lazy_string.hpp"

#include <limits>
#include <string>
#include <vector>

namespace strata {
namespace {

class SingleValueBuilder : public JsonSaxHandler {
  public:
    SingleValueBuilder() = default;

    void reset() {
        root_ = JsonValue();
        has_root_ = false;
        stack_.clear();
        keys_.clear();
    }

    bool has_root() const { return has_root_; }

    JsonValue take_root() {
        has_root_ = false;
        return std::move(root_);
    }

    bool on_null() override { return push_value(JsonValue()); }
    bool on_bool(bool v) override { return push_value(JsonValue(JsonValue::Variant(v))); }
    bool on_int(int64_t v) override { return push_value(JsonValue(JsonValue::Variant(v))); }
    bool on_uint(uint64_t v) override {
        if (v <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
            return push_value(JsonValue(JsonValue::Variant(static_cast<int64_t>(v))));
        }
        return push_value(JsonValue(JsonValue::Variant(static_cast<double>(v))));
    }
    bool on_double(double v) override { return push_value(JsonValue(JsonValue::Variant(v))); }

    bool on_string(std::string_view v, bool has_escapes = false) override {
        if (has_escapes) {
            return push_value(JsonValue(JsonValue::Variant(LazyString(v, true))));
        }
        return push_value(JsonValue(JsonValue::Variant(std::string(v))));
    }

    bool on_start_object(size_t size_hint) override {
        JsonValue::Object obj;
        if (size_hint > 0) {
            obj.reserve(size_hint);
        }
        stack_.emplace_back(JsonValue::Variant(std::move(obj)));
        return true;
    }

    bool on_key(std::string_view v, bool has_escapes = false) override {
        if (has_escapes) {
            LazyString lazy(v, true);
            keys_.emplace_back(lazy.value());
        } else {
            keys_.emplace_back(v);
        }
        return true;
    }

    bool on_end_object() override {
        if (stack_.empty()) {
            return false;
        }
        auto obj = std::move(stack_.back());
        stack_.pop_back();
        return push_value(std::move(obj));
    }

    bool on_start_array(size_t size_hint) override {
        JsonValue::Array arr;
        if (size_hint > 0) {
            arr.reserve(size_hint);
        }
        stack_.emplace_back(JsonValue::Variant(std::move(arr)));
        return true;
    }

    bool on_end_array() override {
        if (stack_.empty()) {
            return false;
        }
        auto arr = std::move(stack_.back());
        stack_.pop_back();
        return push_value(std::move(arr));
    }

  private:
    bool push_value(JsonValue&& val) {
        if (stack_.empty()) {
            root_ = std::move(val);
            has_root_ = true;
            return true;
        }

        auto& top = stack_.back();
        if (std::holds_alternative<JsonValue::Array>(top.data)) {
            std::get<JsonValue::Array>(top.data).push_back(std::move(val));
            return true;
        }
        if (std::holds_alternative<JsonValue::Object>(top.data)) {
            if (keys_.empty()) {
                return false;
            }
            auto& obj = std::get<JsonValue::Object>(top.data);
            std::string key = std::move(keys_.back());
            keys_.pop_back();
            obj.emplace(std::move(key), std::move(val));
            return true;
        }
        return false;
    }

    JsonValue root_;
    bool has_root_ = false;
    std::vector<JsonValue> stack_;
    std::vector<std::string> keys_;
};

using SimpleFieldExtractor = internal::SimpleFieldExtractorBase<SingleValueBuilder, JsonValue>;

} // namespace

Status extract_simple_field_matches(std::string_view text, std::string_view field,
                                   SimpleFieldMode mode, std::vector<JsonValue>& matches,
                                   const ParseSaxOptions& options, ParseSaxContext* context) {
    matches.clear();
    SingleValueBuilder builder;
    SimpleFieldExtractor handler(field, mode, matches, builder);
    ParseSaxOptions tuned = options;
    if (mode == SimpleFieldMode::RootField) {
        tuned.allow_abort = true;
        tuned.use_structural_tape = false;
    }
    return parse_sax(text, handler, tuned, context);
}

} // namespace strata

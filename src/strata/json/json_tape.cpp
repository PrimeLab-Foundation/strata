#include "strata/json/json_tape.hpp"

#include <vector>

namespace strata {

namespace {

// Helper class to build JsonValue from tape
class TapeToDomBuilder {
  public:
    explicit TapeToDomBuilder(const JsonTape& tape) : tape_(tape), it_(tape.begin()) {}

    Result<JsonValue> build() {
        if (tape_.empty()) {
            return {Status::ParseError, JsonValue()};
        }

        auto result = build_value();
        if (result.status != Status::Ok) {
            return result;
        }

        // Verify we consumed all tokens (except RootEnd)
        if (it_.token() != TapeToken::RootEnd) {
            return {Status::ParseError, JsonValue()};
        }

        return result;
    }

  private:
    Result<JsonValue> build_value() {
        TapeToken tok = it_.token();

        switch (tok) {
            case TapeToken::Null:
                ++it_;
                return {Status::Ok, JsonValue()};

            case TapeToken::True:
                ++it_;
                return {Status::Ok, JsonValue(JsonValue::Variant(true))};

            case TapeToken::False:
                ++it_;
                return {Status::Ok, JsonValue(JsonValue::Variant(false))};

            case TapeToken::Int64: {
                int64_t v = it_.as_int64();
                ++it_;
                // Convert to double to match DomBuilderHandler behavior
                return {Status::Ok, JsonValue(JsonValue::Variant(static_cast<double>(v)))};
            }

            case TapeToken::Uint64: {
                uint64_t v = it_.as_uint64();
                ++it_;
                // Convert to double to match DomBuilderHandler behavior
                return {Status::Ok, JsonValue(JsonValue::Variant(static_cast<double>(v)))};
            }

            case TapeToken::Double: {
                double v = it_.as_double();
                ++it_;
                return {Status::Ok, JsonValue(JsonValue::Variant(v))};
            }

            case TapeToken::String: {
                std::string_view sv = it_.as_string();
                ++it_;
                return {Status::Ok, JsonValue(JsonValue::Variant(std::string(sv)))};
            }

            case TapeToken::StartObject:
                return build_object();

            case TapeToken::StartArray:
                return build_array();

            case TapeToken::Key:
            case TapeToken::EndObject:
            case TapeToken::EndArray:
            case TapeToken::RootEnd:
                return {Status::ParseError, JsonValue()};
        }

        return {Status::ParseError, JsonValue()};
    }

    Result<JsonValue> build_object() {
        size_t count = it_.container_size();
        ++it_;  // Consume StartObject

        JsonValue::Object obj;

        for (size_t i = 0; i < count; ++i) {
            // Expect Key token
            if (it_.token() != TapeToken::Key) {
                return {Status::ParseError, JsonValue()};
            }
            std::string key(it_.as_string());
            ++it_;  // Consume Key

            // Build value
            auto value_result = build_value();
            if (value_result.status != Status::Ok) {
                return value_result;
            }

            obj.emplace(std::move(key), std::move(value_result.value));
        }

        // Expect EndObject token
        if (it_.token() != TapeToken::EndObject) {
            return {Status::ParseError, JsonValue()};
        }
        ++it_;  // Consume EndObject

        return {Status::Ok, JsonValue(JsonValue::Variant(std::move(obj)))};
    }

    Result<JsonValue> build_array() {
        size_t count = it_.container_size();
        ++it_;  // Consume StartArray

        JsonValue::Array arr;
        arr.reserve(count);

        for (size_t i = 0; i < count; ++i) {
            auto value_result = build_value();
            if (value_result.status != Status::Ok) {
                return value_result;
            }
            arr.push_back(std::move(value_result.value));
        }

        // Expect EndArray token
        if (it_.token() != TapeToken::EndArray) {
            return {Status::ParseError, JsonValue()};
        }
        ++it_;  // Consume EndArray

        return {Status::Ok, JsonValue(JsonValue::Variant(std::move(arr)))};
    }

    const JsonTape& tape_;
    JsonTape::Iterator it_;
};

}  // namespace

Result<JsonValue> tape_to_dom(const JsonTape& tape) {
    TapeToDomBuilder builder(tape);
    return builder.build();
}

}  // namespace strata

#pragma once

#include "strata/json/json_core.hpp"
#include "strata/json/json_sax_handler.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

/**
 * Token types for the tape format.
 * Each token is stored as a single byte in the token stream.
 */
enum class TapeToken : uint8_t {
    Null       = 0,   // No value entry
    True       = 1,   // No value entry
    False      = 2,   // No value entry
    Int64      = 3,   // values[i] = bit_cast<uint64_t>(int64_value)
    Uint64     = 4,   // values[i] = uint64_value
    Double     = 5,   // values[i] = bit_cast<uint64_t>(double_value)
    String     = 6,   // values[i] = offset, values[i+1] = length
    Key        = 7,   // values[i] = offset, values[i+1] = length
    StartObject= 8,   // values[i] = element_count (number of key-value pairs)
    EndObject  = 9,   // No value entry
    StartArray = 10,  // values[i] = element_count
    EndArray   = 11,  // No value entry
    RootEnd    = 255, // Sentinel marking end of tape
};

/**
 * Immutable tape representation of parsed JSON.
 *
 * The tape consists of three buffers:
 * - tokens_: Stream of token types (1 byte each)
 * - values_: Packed 64-bit values (numeric values, string offsets/lengths, counts)
 * - strings_: Concatenated strings and keys
 *
 * Thread safety: JsonTape is immutable after construction, safe for concurrent reads.
 */
class JsonTape {
  public:
    class Iterator;

    // Default constructor creates empty tape
    JsonTape() = default;

    // Move constructor and assignment
    JsonTape(JsonTape&&) = default;
    JsonTape& operator=(JsonTape&&) = default;

    // No copy (use move semantics for efficiency)
    JsonTape(const JsonTape&) = delete;
    JsonTape& operator=(const JsonTape&) = delete;

    // Iteration
    Iterator begin() const;
    Iterator end() const;

    // Direct access to buffers (for builders)
    const std::vector<uint8_t>& tokens() const { return tokens_; }
    const std::vector<uint64_t>& values() const { return values_; }
    const std::string& strings() const { return strings_; }

    // Stats
    size_t token_count() const { return tokens_.size(); }
    size_t value_count() const { return values_.size(); }
    size_t string_size() const { return strings_.size(); }
    size_t memory_usage() const {
        return tokens_.capacity() * sizeof(uint8_t) +
               values_.capacity() * sizeof(uint64_t) +
               strings_.capacity();
    }

    // Check if tape is empty
    bool empty() const { return tokens_.empty(); }

  private:
    friend class TapeBuilder;

    std::vector<uint8_t> tokens_;
    std::vector<uint64_t> values_;
    std::string strings_;
};

/**
 * Iterator for reading tape tokens.
 *
 * Provides read-only access to tape contents. String views returned by as_string()
 * point into the tape's string buffer and are valid as long as the tape lives.
 */
class JsonTape::Iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = TapeToken;
    using difference_type = std::ptrdiff_t;
    using pointer = const TapeToken*;
    using reference = TapeToken;

    Iterator() = default;

    // Current token type
    TapeToken token() const {
        if (token_idx_ >= tape_->tokens_.size()) {
            return TapeToken::RootEnd;
        }
        return static_cast<TapeToken>(tape_->tokens_[token_idx_]);
    }

    // Dereference operator returns token type
    TapeToken operator*() const { return token(); }

    // Value access (only valid for appropriate token types)
    int64_t as_int64() const {
        assert(token() == TapeToken::Int64);
        return std::bit_cast<int64_t>(tape_->values_[value_idx_]);
    }

    uint64_t as_uint64() const {
        assert(token() == TapeToken::Uint64);
        return tape_->values_[value_idx_];
    }

    double as_double() const {
        assert(token() == TapeToken::Double);
        return std::bit_cast<double>(tape_->values_[value_idx_]);
    }

    std::string_view as_string() const {
        assert(token() == TapeToken::String || token() == TapeToken::Key);
        uint64_t offset = tape_->values_[value_idx_];
        uint64_t length = tape_->values_[value_idx_ + 1];
        return std::string_view(tape_->strings_.data() + offset, length);
    }

    // Container size (for StartObject/StartArray)
    size_t container_size() const {
        assert(token() == TapeToken::StartObject || token() == TapeToken::StartArray);
        return static_cast<size_t>(tape_->values_[value_idx_]);
    }

    // Current position in tape
    size_t token_index() const { return token_idx_; }
    size_t value_index() const { return value_idx_; }

    // Advance to next token
    Iterator& operator++() {
        // Advance value index based on current token type
        value_idx_ += values_consumed(token());
        // Advance token index
        ++token_idx_;
        return *this;
    }

    Iterator operator++(int) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const Iterator& other) const {
        return tape_ == other.tape_ && token_idx_ == other.token_idx_;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }

  private:
    friend class JsonTape;

    Iterator(const JsonTape* tape, size_t token_idx, size_t value_idx)
        : tape_(tape), token_idx_(token_idx), value_idx_(value_idx) {}

    // Number of values consumed by each token type
    static constexpr size_t values_consumed(TapeToken tok) {
        switch (tok) {
            case TapeToken::Null:
            case TapeToken::True:
            case TapeToken::False:
            case TapeToken::EndObject:
            case TapeToken::EndArray:
            case TapeToken::RootEnd:
                return 0;
            case TapeToken::Int64:
            case TapeToken::Uint64:
            case TapeToken::Double:
            case TapeToken::StartObject:
            case TapeToken::StartArray:
                return 1;
            case TapeToken::String:
            case TapeToken::Key:
                return 2;
        }
        return 0;
    }

    const JsonTape* tape_ = nullptr;
    size_t token_idx_ = 0;
    size_t value_idx_ = 0;
};

inline JsonTape::Iterator JsonTape::begin() const {
    return Iterator(this, 0, 0);
}

inline JsonTape::Iterator JsonTape::end() const {
    return Iterator(this, tokens_.size(), values_.size());
}

/**
 * SAX handler that builds a JsonTape from parse events.
 *
 * Usage:
 *   TapeBuilder builder;
 *   parse_sax(json_text, builder);
 *   JsonTape tape = builder.build();
 */
class TapeBuilder : public JsonSaxHandler {
  public:
    TapeBuilder() {
        // Reserve some initial capacity
        tape_.tokens_.reserve(256);
        tape_.values_.reserve(256);
        tape_.strings_.reserve(1024);
        container_stack_.reserve(32);
    }

    // JsonSaxHandler interface
    bool on_null() override {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::Null));
        increment_container_count();
        return true;
    }

    bool on_bool(bool v) override {
        tape_.tokens_.push_back(static_cast<uint8_t>(v ? TapeToken::True : TapeToken::False));
        increment_container_count();
        return true;
    }

    bool on_int(int64_t v) override {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::Int64));
        tape_.values_.push_back(std::bit_cast<uint64_t>(v));
        increment_container_count();
        return true;
    }

    bool on_uint(uint64_t v) override {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::Uint64));
        tape_.values_.push_back(v);
        increment_container_count();
        return true;
    }

    bool on_double(double v) override {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::Double));
        tape_.values_.push_back(std::bit_cast<uint64_t>(v));
        increment_container_count();
        return true;
    }

    bool on_string(std::string_view v, bool has_escapes = false) override {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::String));
        write_string(v, has_escapes);
        increment_container_count();
        return true;
    }

    bool on_start_object(size_t /*size_hint*/ = 0) override {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::StartObject));
        // Remember position where we'll write the count
        size_t count_idx = tape_.values_.size();
        tape_.values_.push_back(0);  // Placeholder for count
        container_stack_.push_back({count_idx, 0, true});
        return true;
    }

    bool on_key(std::string_view v, bool has_escapes = false) override {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::Key));
        write_string(v, has_escapes);
        // Key doesn't count as an element, the value will count
        return true;
    }

    bool on_end_object() override {
        if (container_stack_.empty()) return false;
        auto& frame = container_stack_.back();
        if (!frame.is_object) return false;

        // Write the actual count to the placeholder position
        tape_.values_[frame.count_value_idx] = frame.element_count;
        container_stack_.pop_back();

        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::EndObject));
        increment_container_count();  // Object itself counts as an element in parent
        return true;
    }

    bool on_start_array(size_t /*size_hint*/ = 0) override {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::StartArray));
        // Remember position where we'll write the count
        size_t count_idx = tape_.values_.size();
        tape_.values_.push_back(0);  // Placeholder for count
        container_stack_.push_back({count_idx, 0, false});
        return true;
    }

    bool on_end_array() override {
        if (container_stack_.empty()) return false;
        auto& frame = container_stack_.back();
        if (frame.is_object) return false;

        // Write the actual count to the placeholder position
        tape_.values_[frame.count_value_idx] = frame.element_count;
        container_stack_.pop_back();

        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::EndArray));
        increment_container_count();  // Array itself counts as an element in parent
        return true;
    }

    // Extract built tape (moves ownership)
    JsonTape build() {
        tape_.tokens_.push_back(static_cast<uint8_t>(TapeToken::RootEnd));
        return std::move(tape_);
    }

    // Reset builder for reuse
    void reset() {
        tape_.tokens_.clear();
        tape_.values_.clear();
        tape_.strings_.clear();
        container_stack_.clear();
    }

  private:
    struct ContainerFrame {
        size_t count_value_idx;  // Index in values_ where count is stored
        size_t element_count;    // Number of elements so far
        bool is_object;          // true = object, false = array
    };

    void write_string(std::string_view v, bool has_escapes) {
        uint64_t offset = tape_.strings_.size();
        if (has_escapes) {
            // Need to unescape - use LazyString logic
            std::string unescaped = unescape_string(v);
            tape_.strings_.append(unescaped);
            tape_.values_.push_back(offset);
            tape_.values_.push_back(unescaped.size());
        } else {
            tape_.strings_.append(v);
            tape_.values_.push_back(offset);
            tape_.values_.push_back(v.size());
        }
    }

    void increment_container_count() {
        if (!container_stack_.empty()) {
            ++container_stack_.back().element_count;
        }
    }

    // Simple unescape implementation for JSON strings
    static std::string unescape_string(std::string_view v) {
        std::string result;
        result.reserve(v.size());

        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i] == '\\' && i + 1 < v.size()) {
                char next = v[i + 1];
                switch (next) {
                    case '"':  result += '"';  ++i; break;
                    case '\\': result += '\\'; ++i; break;
                    case '/':  result += '/';  ++i; break;
                    case 'b':  result += '\b'; ++i; break;
                    case 'f':  result += '\f'; ++i; break;
                    case 'n':  result += '\n'; ++i; break;
                    case 'r':  result += '\r'; ++i; break;
                    case 't':  result += '\t'; ++i; break;
                    case 'u':
                        // Unicode escape \uXXXX
                        if (i + 5 < v.size()) {
                            uint32_t codepoint = 0;
                            bool valid = true;
                            for (int j = 0; j < 4 && valid; ++j) {
                                char c = v[i + 2 + j];
                                codepoint <<= 4;
                                if (c >= '0' && c <= '9') codepoint |= (c - '0');
                                else if (c >= 'a' && c <= 'f') codepoint |= (10 + c - 'a');
                                else if (c >= 'A' && c <= 'F') codepoint |= (10 + c - 'A');
                                else valid = false;
                            }
                            if (valid) {
                                // Handle surrogate pairs
                                if (codepoint >= 0xD800 && codepoint <= 0xDBFF &&
                                    i + 11 < v.size() && v[i + 6] == '\\' && v[i + 7] == 'u') {
                                    // High surrogate, check for low surrogate
                                    uint32_t low = 0;
                                    bool low_valid = true;
                                    for (int j = 0; j < 4 && low_valid; ++j) {
                                        char c = v[i + 8 + j];
                                        low <<= 4;
                                        if (c >= '0' && c <= '9') low |= (c - '0');
                                        else if (c >= 'a' && c <= 'f') low |= (10 + c - 'a');
                                        else if (c >= 'A' && c <= 'F') low |= (10 + c - 'A');
                                        else low_valid = false;
                                    }
                                    if (low_valid && low >= 0xDC00 && low <= 0xDFFF) {
                                        // Valid surrogate pair
                                        codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                                        i += 6;  // Skip extra \uXXXX
                                    }
                                }
                                append_utf8(result, codepoint);
                                i += 5;
                            } else {
                                result += v[i];
                            }
                        } else {
                            result += v[i];
                        }
                        break;
                    default:
                        result += v[i];
                        break;
                }
            } else {
                result += v[i];
            }
        }
        return result;
    }

    static void append_utf8(std::string& out, uint32_t codepoint) {
        if (codepoint <= 0x7F) {
            out += static_cast<char>(codepoint);
        } else if (codepoint <= 0x7FF) {
            out += static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F));
            out += static_cast<char>(0x80 | (codepoint & 0x3F));
        } else if (codepoint <= 0xFFFF) {
            out += static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F));
            out += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (codepoint & 0x3F));
        } else if (codepoint <= 0x10FFFF) {
            out += static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07));
            out += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
            out += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (codepoint & 0x3F));
        }
    }

    JsonTape tape_;
    std::vector<ContainerFrame> container_stack_;
};

// Build JsonValue DOM from tape
Result<JsonValue> tape_to_dom(const JsonTape& tape);

} // namespace strata

#include "strata/json/json_lazy_cursor.hpp"

#include "strata/json/json_parse.hpp"
#include "strata/util/fast_parse.hpp"
#include "strata/util/lazy_string.hpp"
#include "strata/util/simd_string.hpp"

#include <cctype>

namespace strata {

// ============================================================================
// LazyJsonCursor Implementation
// ============================================================================

LazyJsonCursor::LazyJsonCursor(std::string_view input)
    : input_(input), pos_{0, 1, 1} {
    // Skip leading whitespace
    pos_.offset = skip_whitespace(0);
}

LazyJsonCursor::LazyJsonCursor(std::string_view input, CursorPosition pos)
    : input_(input), pos_(pos) {
    // Skip whitespace at current position
    pos_.offset = skip_whitespace(pos_.offset);
}

char LazyJsonCursor::peek_char() const {
    if (pos_.offset >= input_.size()) return '\0';
    return input_[pos_.offset];
}

std::string_view LazyJsonCursor::remaining() const {
    if (pos_.offset >= input_.size()) return {};
    return input_.substr(pos_.offset);
}

// Type detection methods
bool LazyJsonCursor::is_null() const {
    return peek_char() == 'n';
}

bool LazyJsonCursor::is_bool() const {
    char c = peek_char();
    return c == 't' || c == 'f';
}

bool LazyJsonCursor::is_number() const {
    char c = peek_char();
    return c == '-' || (c >= '0' && c <= '9');
}

bool LazyJsonCursor::is_string() const {
    return peek_char() == '"';
}

bool LazyJsonCursor::is_array() const {
    return peek_char() == '[';
}

bool LazyJsonCursor::is_object() const {
    return peek_char() == '{';
}

// ============================================================================
// Skip helpers
// ============================================================================

size_t LazyJsonCursor::skip_whitespace(size_t from) const {
    return util::skip_whitespace_simd(input_.data(), input_.size(), from);
}

Result<size_t> LazyJsonCursor::skip_string(size_t from) const {
    if (from >= input_.size() || input_[from] != '"') {
        return {Status::ParseError, from};
    }

    size_t pos = from + 1;  // Skip opening quote
    while (pos < input_.size()) {
        char c = input_[pos];
        if (c == '"') {
            return {Status::Ok, pos + 1};  // Position after closing quote
        }
        if (c == '\\') {
            // Skip escape sequence (\ plus next character)
            pos += 2;
            continue;
        }
        ++pos;
    }
    // Unterminated string
    return {Status::ParseError, pos};
}

Result<size_t> LazyJsonCursor::skip_number(size_t from) const {
    size_t pos = from;

    // Optional minus
    if (pos < input_.size() && input_[pos] == '-') {
        ++pos;
    }

    // Integer part
    if (pos >= input_.size()) {
        return {Status::ParseError, pos};
    }

    if (input_[pos] == '0') {
        ++pos;
    } else if (input_[pos] >= '1' && input_[pos] <= '9') {
        while (pos < input_.size() && input_[pos] >= '0' && input_[pos] <= '9') {
            ++pos;
        }
    } else {
        return {Status::ParseError, pos};
    }

    // Optional fraction
    if (pos < input_.size() && input_[pos] == '.') {
        ++pos;
        if (pos >= input_.size() || input_[pos] < '0' || input_[pos] > '9') {
            return {Status::ParseError, pos};
        }
        while (pos < input_.size() && input_[pos] >= '0' && input_[pos] <= '9') {
            ++pos;
        }
    }

    // Optional exponent
    if (pos < input_.size() && (input_[pos] == 'e' || input_[pos] == 'E')) {
        ++pos;
        if (pos < input_.size() && (input_[pos] == '+' || input_[pos] == '-')) {
            ++pos;
        }
        if (pos >= input_.size() || input_[pos] < '0' || input_[pos] > '9') {
            return {Status::ParseError, pos};
        }
        while (pos < input_.size() && input_[pos] >= '0' && input_[pos] <= '9') {
            ++pos;
        }
    }

    return {Status::Ok, pos};
}

Result<size_t> LazyJsonCursor::skip_container(size_t from) const {
    int depth = 1;
    size_t pos = from + 1;

    while (pos < input_.size() && depth > 0) {
        char c = input_[pos];
        switch (c) {
            case '{':
            case '[':
                ++depth;
                ++pos;
                break;
            case '}':
            case ']':
                --depth;
                ++pos;
                break;
            case '"': {
                // Skip string to handle brackets inside strings
                auto result = skip_string(pos);
                if (!result.ok()) return result;
                pos = result.value;
                break;
            }
            default:
                ++pos;
                break;
        }
    }

    if (depth != 0) {
        return {Status::ParseError, pos};
    }
    return {Status::Ok, pos};
}

Result<size_t> LazyJsonCursor::skip_value(size_t from) const {
    if (from >= input_.size()) {
        return {Status::ParseError, from};
    }

    char c = input_[from];
    switch (c) {
        case '"':
            return skip_string(from);
        case '{':
        case '[':
            return skip_container(from);
        case 't':
            // true
            if (from + 4 <= input_.size() &&
                input_.substr(from, 4) == "true") {
                return {Status::Ok, from + 4};
            }
            return {Status::ParseError, from};
        case 'f':
            // false
            if (from + 5 <= input_.size() &&
                input_.substr(from, 5) == "false") {
                return {Status::Ok, from + 5};
            }
            return {Status::ParseError, from};
        case 'n':
            // null
            if (from + 4 <= input_.size() &&
                input_.substr(from, 4) == "null") {
                return {Status::Ok, from + 4};
            }
            return {Status::ParseError, from};
        default:
            if (c == '-' || (c >= '0' && c <= '9')) {
                return skip_number(from);
            }
            return {Status::ParseError, from};
    }
}

Result<StringReadResult> LazyJsonCursor::read_string_content(size_t from) const {
    if (from >= input_.size() || input_[from] != '"') {
        return {Status::ParseError, {}};
    }

    size_t start = from + 1;  // After opening quote
    size_t pos = start;
    bool has_escapes = false;

    while (pos < input_.size()) {
        char c = input_[pos];
        if (c == '"') {
            // Found closing quote
            return {Status::Ok, {
                input_.substr(start, pos - start),
                pos + 1,  // Position after closing quote
                has_escapes
            }};
        }
        if (c == '\\') {
            has_escapes = true;
            pos += 2;  // Skip escape sequence
            continue;
        }
        ++pos;
    }

    // Unterminated string
    return {Status::ParseError, {}};
}

// ============================================================================
// Navigation
// ============================================================================

Result<LazyJsonCursor> LazyJsonCursor::get_field(std::string_view key) const {
    if (!is_object()) {
        return {Status::TypeMismatch, LazyJsonCursor(input_, pos_)};
    }

    size_t pos = skip_whitespace(pos_.offset + 1);  // Skip '{'

    while (pos < input_.size() && input_[pos] != '}') {
        // Parse key
        auto key_result = read_string_content(pos);
        if (!key_result.ok()) {
            return {key_result.status, LazyJsonCursor(input_, pos_)};
        }

        std::string_view found_key = key_result.value.content;
        bool key_has_escapes = key_result.value.has_escapes;
        pos = skip_whitespace(key_result.value.end_pos);

        // Expect colon
        if (pos >= input_.size() || input_[pos] != ':') {
            return {Status::ParseError, LazyJsonCursor(input_, pos_)};
        }
        pos = skip_whitespace(pos + 1);  // Skip ':'

        // Compare key
        bool matches;
        if (!key_has_escapes) {
            matches = (found_key == key);
        } else {
            // Need to unescape the key for comparison
            LazyString lazy_key(found_key, true);
            matches = (lazy_key.value() == key);
        }

        if (matches) {
            // Found the key! Return cursor at value position
            return {Status::Ok, LazyJsonCursor(input_, {pos, pos_.line, 0})};
        }

        // Skip value we don't need
        auto skip_result = skip_value(pos);
        if (!skip_result.ok()) {
            return {skip_result.status, LazyJsonCursor(input_, pos_)};
        }

        pos = skip_whitespace(skip_result.value);

        // Handle comma
        if (pos < input_.size() && input_[pos] == ',') {
            pos = skip_whitespace(pos + 1);
        }
    }

    return {Status::KeyNotFound, LazyJsonCursor(input_, pos_)};
}

Result<LazyJsonCursor> LazyJsonCursor::get_at(size_t index) const {
    if (!is_array()) {
        return {Status::TypeMismatch, LazyJsonCursor(input_, pos_)};
    }

    size_t pos = skip_whitespace(pos_.offset + 1);  // Skip '['
    size_t current_index = 0;

    while (pos < input_.size() && input_[pos] != ']') {
        if (current_index == index) {
            // Found the element
            return {Status::Ok, LazyJsonCursor(input_, {pos, pos_.line, 0})};
        }

        // Skip this element
        auto skip_result = skip_value(pos);
        if (!skip_result.ok()) {
            return {skip_result.status, LazyJsonCursor(input_, pos_)};
        }

        pos = skip_whitespace(skip_result.value);
        ++current_index;

        // Handle comma
        if (pos < input_.size() && input_[pos] == ',') {
            pos = skip_whitespace(pos + 1);
        }
    }

    return {Status::IndexOutOfBounds, LazyJsonCursor(input_, pos_)};
}

Result<LazyJsonCursor> LazyJsonCursor::skip() const {
    auto result = skip_value(pos_.offset);
    if (!result.ok()) {
        return {result.status, LazyJsonCursor(input_, pos_)};
    }
    return {Status::Ok, LazyJsonCursor(input_, {result.value, pos_.line, 0})};
}

// ============================================================================
// Materialization
// ============================================================================

Result<JsonValue> LazyJsonCursor::materialize() const {
    // Use remaining input from current position
    std::string_view rem = remaining();
    return parse_json(rem);
}

Result<bool> LazyJsonCursor::materialize_bool() const {
    if (!is_bool()) {
        return {Status::TypeMismatch, false};
    }

    if (input_[pos_.offset] == 't') {
        if (pos_.offset + 4 <= input_.size() &&
            input_.substr(pos_.offset, 4) == "true") {
            return {Status::Ok, true};
        }
    } else if (input_[pos_.offset] == 'f') {
        if (pos_.offset + 5 <= input_.size() &&
            input_.substr(pos_.offset, 5) == "false") {
            return {Status::Ok, false};
        }
    }

    return {Status::ParseError, false};
}

Result<double> LazyJsonCursor::materialize_number() const {
    if (!is_number()) {
        return {Status::TypeMismatch, 0.0};
    }

    // Find end of number
    auto end_result = skip_number(pos_.offset);
    if (!end_result.ok()) {
        return {Status::ParseError, 0.0};
    }

    std::string_view num_str = input_.substr(pos_.offset, end_result.value - pos_.offset);

    // Parse using fast_parse utilities
    char* end_ptr = nullptr;
    double value = std::strtod(std::string(num_str).c_str(), &end_ptr);

    return {Status::Ok, value};
}

Result<std::string> LazyJsonCursor::materialize_string() const {
    if (!is_string()) {
        return {Status::TypeMismatch, {}};
    }

    auto result = read_string_content(pos_.offset);
    if (!result.ok()) {
        return {result.status, {}};
    }

    if (!result.value.has_escapes) {
        return {Status::Ok, std::string(result.value.content)};
    }

    // Need to unescape
    LazyString lazy_str(result.value.content, true);
    return {Status::Ok, lazy_str.value()};
}

// ============================================================================
// Size methods
// ============================================================================

Result<size_t> LazyJsonCursor::array_size() const {
    if (!is_array()) {
        return {Status::TypeMismatch, 0};
    }

    size_t pos = skip_whitespace(pos_.offset + 1);  // Skip '['
    size_t count = 0;

    if (pos < input_.size() && input_[pos] == ']') {
        return {Status::Ok, 0};  // Empty array
    }

    while (pos < input_.size() && input_[pos] != ']') {
        ++count;

        // Skip this element
        auto skip_result = skip_value(pos);
        if (!skip_result.ok()) {
            return {skip_result.status, 0};
        }

        pos = skip_whitespace(skip_result.value);

        // Handle comma
        if (pos < input_.size() && input_[pos] == ',') {
            pos = skip_whitespace(pos + 1);
        }
    }

    return {Status::Ok, count};
}

Result<size_t> LazyJsonCursor::object_size() const {
    if (!is_object()) {
        return {Status::TypeMismatch, 0};
    }

    size_t pos = skip_whitespace(pos_.offset + 1);  // Skip '{'
    size_t count = 0;

    if (pos < input_.size() && input_[pos] == '}') {
        return {Status::Ok, 0};  // Empty object
    }

    while (pos < input_.size() && input_[pos] != '}') {
        ++count;

        // Skip key
        auto key_result = skip_string(pos);
        if (!key_result.ok()) {
            return {key_result.status, 0};
        }

        pos = skip_whitespace(key_result.value);

        // Expect colon
        if (pos >= input_.size() || input_[pos] != ':') {
            return {Status::ParseError, 0};
        }
        pos = skip_whitespace(pos + 1);

        // Skip value
        auto skip_result = skip_value(pos);
        if (!skip_result.ok()) {
            return {skip_result.status, 0};
        }

        pos = skip_whitespace(skip_result.value);

        // Handle comma
        if (pos < input_.size() && input_[pos] == ',') {
            pos = skip_whitespace(pos + 1);
        }
    }

    return {Status::Ok, count};
}

// ============================================================================
// Iterators
// ============================================================================

Result<LazyJsonCursor::FieldIterator> LazyJsonCursor::iter_fields() const {
    if (!is_object()) {
        return {Status::TypeMismatch, FieldIterator()};
    }

    size_t pos = skip_whitespace(pos_.offset + 1);  // Skip '{'
    return {Status::Ok, FieldIterator(input_, pos)};
}

Result<LazyJsonCursor::ElementIterator> LazyJsonCursor::iter_elements() const {
    if (!is_array()) {
        return {Status::TypeMismatch, ElementIterator()};
    }

    size_t pos = skip_whitespace(pos_.offset + 1);  // Skip '['
    return {Status::Ok, ElementIterator(input_, pos)};
}

// ============================================================================
// FieldIterator Implementation
// ============================================================================

std::string LazyJsonCursor::FieldIterator::Field::key() const {
    if (!key_has_escapes) {
        return std::string(key_raw);
    }
    LazyString lazy_key(key_raw, true);
    return lazy_key.value();
}

LazyJsonCursor::FieldIterator::FieldIterator(std::string_view input, size_t pos)
    : input_(input), pos_(pos), at_end_(false) {
    // Skip whitespace
    pos_ = util::skip_whitespace_simd(input_.data(), input_.size(), pos_);
    // Check if empty object
    if (pos_ >= input_.size() || input_[pos_] == '}') {
        at_end_ = true;
    }
}

LazyJsonCursor::FieldIterator& LazyJsonCursor::FieldIterator::operator++() {
    if (at_end_) return *this;
    advance_to_next();
    return *this;
}

LazyJsonCursor::FieldIterator::Field LazyJsonCursor::FieldIterator::operator*() const {
    if (at_end_) {
        return {{}, false, LazyJsonCursor(input_, {pos_, 1, 1})};
    }

    // Parse key
    LazyJsonCursor cursor(input_, {pos_, 1, 1});
    auto key_result = cursor.read_string_content(pos_);
    if (!key_result.ok()) {
        return {{}, false, LazyJsonCursor(input_, {pos_, 1, 1})};
    }

    size_t value_pos = util::skip_whitespace_simd(
        input_.data(), input_.size(), key_result.value.end_pos);

    // Skip colon
    if (value_pos < input_.size() && input_[value_pos] == ':') {
        value_pos = util::skip_whitespace_simd(
            input_.data(), input_.size(), value_pos + 1);
    }

    return {
        key_result.value.content,
        key_result.value.has_escapes,
        LazyJsonCursor(input_, {value_pos, 1, 1})
    };
}

bool LazyJsonCursor::FieldIterator::operator==(const FieldIterator& other) const {
    if (at_end_ && other.at_end_) return true;
    if (at_end_ != other.at_end_) return false;
    return input_.data() == other.input_.data() && pos_ == other.pos_;
}

void LazyJsonCursor::FieldIterator::advance_to_next() {
    // Current position should be at a key string
    // Skip key
    LazyJsonCursor cursor(input_, {pos_, 1, 1});
    auto key_result = cursor.skip_string(pos_);
    if (!key_result.ok()) {
        at_end_ = true;
        return;
    }

    pos_ = util::skip_whitespace_simd(input_.data(), input_.size(), key_result.value);

    // Skip colon
    if (pos_ < input_.size() && input_[pos_] == ':') {
        pos_ = util::skip_whitespace_simd(input_.data(), input_.size(), pos_ + 1);
    } else {
        at_end_ = true;
        return;
    }

    // Skip value
    auto value_result = cursor.skip_value(pos_);
    if (!value_result.ok()) {
        at_end_ = true;
        return;
    }

    pos_ = util::skip_whitespace_simd(input_.data(), input_.size(), value_result.value);

    // Handle comma
    if (pos_ < input_.size() && input_[pos_] == ',') {
        pos_ = util::skip_whitespace_simd(input_.data(), input_.size(), pos_ + 1);
    }

    // Check for end
    if (pos_ >= input_.size() || input_[pos_] == '}') {
        at_end_ = true;
    }
}

// ============================================================================
// ElementIterator Implementation
// ============================================================================

LazyJsonCursor::ElementIterator::ElementIterator(std::string_view input, size_t pos)
    : input_(input), pos_(pos), at_end_(false) {
    // Skip whitespace
    pos_ = util::skip_whitespace_simd(input_.data(), input_.size(), pos_);
    // Check if empty array
    if (pos_ >= input_.size() || input_[pos_] == ']') {
        at_end_ = true;
    }
}

LazyJsonCursor::ElementIterator& LazyJsonCursor::ElementIterator::operator++() {
    if (at_end_) return *this;
    advance_to_next();
    return *this;
}

LazyJsonCursor LazyJsonCursor::ElementIterator::operator*() const {
    return LazyJsonCursor(input_, {pos_, 1, 1});
}

bool LazyJsonCursor::ElementIterator::operator==(const ElementIterator& other) const {
    if (at_end_ && other.at_end_) return true;
    if (at_end_ != other.at_end_) return false;
    return input_.data() == other.input_.data() && pos_ == other.pos_;
}

void LazyJsonCursor::ElementIterator::advance_to_next() {
    // Skip current element
    LazyJsonCursor cursor(input_, {pos_, 1, 1});
    auto skip_result = cursor.skip_value(pos_);
    if (!skip_result.ok()) {
        at_end_ = true;
        return;
    }

    pos_ = util::skip_whitespace_simd(input_.data(), input_.size(), skip_result.value);

    // Handle comma
    if (pos_ < input_.size() && input_[pos_] == ',') {
        pos_ = util::skip_whitespace_simd(input_.data(), input_.size(), pos_ + 1);
    }

    // Check for end
    if (pos_ >= input_.size() || input_[pos_] == ']') {
        at_end_ = true;
    }
}

// ============================================================================
// Lazy JSONPath (stub implementations - full implementation in future)
// ============================================================================

std::vector<JsonValue> eval_jsonpath_lazy(
    std::string_view json,
    const CompiledPath& path,
    size_t limit
) {
    // Stub implementation - returns empty for now
    // Full lazy implementation would walk path segments using cursor
    // without materializing unneeded values
    (void)json;
    (void)path;
    (void)limit;
    return {};
}

std::vector<LazyJsonCursor> find_jsonpath_lazy(
    std::string_view json,
    const CompiledPath& path,
    size_t limit
) {
    // Stub implementation - returns empty for now
    // Full implementation would walk path segments lazily
    (void)json;
    (void)path;
    (void)limit;
    return {};
}

} // namespace strata

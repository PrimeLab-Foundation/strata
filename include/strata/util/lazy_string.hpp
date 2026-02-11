#pragma once

#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <string_view>

namespace strata {

/**
 * LazyString provides deferred escape processing for JSON strings.
 *
 * Key features:
 * - Stores raw JSON string bytes (without quotes)
 * - Tracks whether escape sequences are present
 * - Unescapes on-demand when value is accessed
 * - Enables raw byte comparison for keys without escapes
 *
 * This optimization is useful for JSONPath queries that filter
 * without materializing string values.
 */
class LazyString {
  public:
    /**
     * Construct from raw JSON string bytes (std::string).
     * @param raw The raw string content (between quotes, not including quotes)
     * @param has_escapes True if the string contains backslash escapes
     */
    explicit LazyString(std::string raw, bool has_escapes = false)
        : raw_(std::move(raw)), has_escapes_(has_escapes) {
        if (!has_escapes_) {
            // No escapes: raw IS the value, no unescaping needed
            value_ = raw_;
        }
    }

    /**
     * Construct from string_view (copies the data).
     * @param raw The raw string content
     * @param has_escapes True if the string contains backslash escapes
     */
    LazyString(std::string_view raw, bool has_escapes)
        : LazyString(std::string(raw), has_escapes) {}

    /**
     * Construct from C-string (const char*).
     * @param raw The raw string content
     * @param has_escapes True if the string contains backslash escapes
     */
    LazyString(const char* raw, bool has_escapes)
        : LazyString(std::string(raw), has_escapes) {}

    /**
     * Default constructor - creates empty string with no escapes.
     */
    LazyString() : raw_(), has_escapes_(false), value_(std::string{}) {}

    /**
     * Move constructor.
     */
    LazyString(LazyString&& other) noexcept
        : raw_(std::move(other.raw_)), has_escapes_(other.has_escapes_),
          value_(std::move(other.value_)) {}

    /**
     * Copy constructor.
     */
    LazyString(const LazyString& other)
        : raw_(other.raw_), has_escapes_(other.has_escapes_), value_(other.value_) {}

    /**
     * Move assignment.
     */
    LazyString& operator=(LazyString&& other) noexcept {
        if (this != &other) {
            raw_ = std::move(other.raw_);
            has_escapes_ = other.has_escapes_;
            value_ = std::move(other.value_);
        }
        return *this;
    }

    /**
     * Copy assignment.
     */
    LazyString& operator=(const LazyString& other) {
        if (this != &other) {
            raw_ = other.raw_;
            has_escapes_ = other.has_escapes_;
            value_ = other.value_;
        }
        return *this;
    }

    /**
     * Get the unescaped string value.
     * Triggers unescaping on first access if the string has escapes.
     * @return Reference to the unescaped string
     */
    const std::string& value() const {
        if (!value_.has_value()) {
            value_ = unescape(raw_);
        }
        return *value_;
    }

    /**
     * Get the raw bytes (before unescaping).
     * @return View of the raw string content
     */
    std::string_view raw() const { return raw_; }

    /**
     * Check if the string contains escape sequences.
     * @return True if the string has escapes and may need unescaping
     */
    bool has_escapes() const { return has_escapes_; }

    /**
     * Check if the value has been materialized (unescaped).
     * @return True if value() has been called and cached
     */
    bool is_materialized() const { return value_.has_value(); }

    /**
     * Get the size of the unescaped string.
     * Note: This may trigger unescaping if the string has escapes.
     * @return Length of the unescaped string
     */
    size_t size() const { return value().size(); }

    /**
     * Check if the string is empty.
     * @return True if the string is empty
     */
    bool empty() const { return raw_.empty(); }

    /**
     * Implicit conversion to string_view (of the unescaped value).
     * May trigger unescaping.
     */
    operator std::string_view() const { return value(); }

    /**
     * Equality comparison with another LazyString.
     * Optimized: if neither has escapes, compares raw bytes directly.
     */
    bool operator==(const LazyString& other) const {
        // Fast path: if neither has escapes, compare raw bytes
        if (!has_escapes_ && !other.has_escapes_) {
            return raw_ == other.raw_;
        }
        // Slow path: compare unescaped values
        return value() == other.value();
    }

    bool operator!=(const LazyString& other) const { return !(*this == other); }

    /**
     * Equality comparison with std::string.
     * If no escapes, compares raw bytes (fast path).
     */
    bool operator==(const std::string& other) const {
        if (!has_escapes_) {
            return raw_ == other;
        }
        return value() == other;
    }

    bool operator!=(const std::string& other) const { return !(*this == other); }

    /**
     * Equality comparison with string_view.
     * If no escapes, compares raw bytes (fast path).
     */
    bool operator==(std::string_view other) const {
        if (!has_escapes_) {
            return raw_ == other;
        }
        return value() == other;
    }

    bool operator!=(std::string_view other) const { return !(*this == other); }

    /**
     * Equality comparison with C-string.
     */
    bool operator==(const char* other) const { return *this == std::string_view(other); }

    bool operator!=(const char* other) const { return !(*this == other); }

    /**
     * Less-than comparison for use in ordered containers.
     */
    bool operator<(const LazyString& other) const { return value() < other.value(); }

    /**
     * Get the unescaped value as std::string (copy).
     * @return Copy of the unescaped string
     */
    std::string to_string() const { return value(); }

    /**
     * Force materialization of the unescaped value.
     * Useful when you know you'll access the value multiple times.
     */
    void materialize() const { (void)value(); }

    /**
     * Create a LazyString from an already-unescaped string.
     * The string will be stored as-is with no escape flag.
     * @param str The unescaped string value
     * @return LazyString with no escapes
     */
    static LazyString from_unescaped(std::string str) { return LazyString(std::move(str), false); }

  private:
    /**
     * Unescape a JSON string.
     * Handles: \", \\, \/, \b, \f, \n, \r, \t, \uXXXX (including surrogate pairs)
     */
    static std::string unescape(const std::string& raw) {
        std::string result;
        result.reserve(raw.size());

        const char* data = raw.data();
        const size_t len = raw.size();
        size_t i = 0;

        while (i < len) {
            const char* slash =
                static_cast<const char*>(std::memchr(data + i, '\\', len - i));
            if (!slash) {
                result.append(data + i, len - i);
                break;
            }

            size_t run = static_cast<size_t>(slash - (data + i));
            if (run > 0) {
                result.append(data + i, run);
                i += run;
            }

            if (i >= len || data[i] != '\\') {
                continue;
            }
            if (i + 1 >= len) {
                break;
            }

            char esc = data[i + 1];
            switch (esc) {
            case '"':
                result.push_back('"');
                i += 2;
                break;
            case '\\':
                result.push_back('\\');
                i += 2;
                break;
            case '/':
                result.push_back('/');
                i += 2;
                break;
            case 'b':
                result.push_back('\b');
                i += 2;
                break;
            case 'f':
                result.push_back('\f');
                i += 2;
                break;
            case 'n':
                result.push_back('\n');
                i += 2;
                break;
            case 'r':
                result.push_back('\r');
                i += 2;
                break;
            case 't':
                result.push_back('\t');
                i += 2;
                break;
            case 'u': {
                // Parse \uXXXX
                if (i + 5 >= len) {
                    i += 2;
                    break;
                }
                uint32_t codepoint = parse_hex4(raw, i + 2);
                i += 6;

                // Handle surrogate pairs
                if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                    // High surrogate - look for low surrogate
                    if (i + 5 < len && data[i] == '\\' && data[i + 1] == 'u') {
                        uint32_t low = parse_hex4(raw, i + 2);
                        if (low >= 0xDC00 && low <= 0xDFFF) {
                            codepoint =
                                0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                            i += 6;
                        }
                    }
                }

                append_utf8(result, codepoint);
                break;
            }
            default:
                // Unknown escape - keep as-is (shouldn't happen in valid JSON)
                result.push_back(data[i]);
                i += 1;
                break;
            }
        }

        return result;
    }

    /**
     * Parse 4 hex digits at the given position.
     */
    static uint32_t parse_hex4(const std::string& str, size_t pos) {
        uint32_t result = 0;
        for (int j = 0; j < 4 && pos + j < str.size(); ++j) {
            char c = str[pos + j];
            uint32_t digit = 0;
            if (c >= '0' && c <= '9')
                digit = c - '0';
            else if (c >= 'a' && c <= 'f')
                digit = 10 + (c - 'a');
            else if (c >= 'A' && c <= 'F')
                digit = 10 + (c - 'A');
            result = (result << 4) | digit;
        }
        return result;
    }

    /**
     * Append a Unicode codepoint as UTF-8.
     */
    static void append_utf8(std::string& out, uint32_t codepoint) {
        if (codepoint < 0x80) {
            out.push_back(static_cast<char>(codepoint));
        } else if (codepoint < 0x800) {
            out.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else if (codepoint < 0x10000) {
            out.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else if (codepoint <= 0x10FFFF) {
            out.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
    }

    std::string raw_;                       // Raw JSON string content (without quotes)
    bool has_escapes_;                      // True if raw_ contains backslash escapes
    mutable std::optional<std::string> value_; // Cached unescaped value
};

// Reverse comparison operators for std::string and string_view
inline bool operator==(const std::string& lhs, const LazyString& rhs) { return rhs == lhs; }
inline bool operator!=(const std::string& lhs, const LazyString& rhs) { return rhs != lhs; }
inline bool operator==(std::string_view lhs, const LazyString& rhs) { return rhs == lhs; }
inline bool operator!=(std::string_view lhs, const LazyString& rhs) { return rhs != lhs; }
inline bool operator==(const char* lhs, const LazyString& rhs) { return rhs == lhs; }
inline bool operator!=(const char* lhs, const LazyString& rhs) { return rhs != lhs; }

} // namespace strata

#pragma once

#include "strata/json/json_core.hpp"

#include <cstddef>
#include <limits>
#include <string>
#include <string_view>

namespace strata {

/**
 * Position state for lazy cursor.
 * Tracks byte offset and line/column for error reporting.
 */
struct CursorPosition {
    size_t offset = 0;   // Current byte position in input
    size_t line = 1;     // Line number (1-based, for errors)
    size_t column = 1;   // Column number (1-based, for errors)
};

/**
 * Result of reading a string's raw content (without parsing escapes).
 */
struct StringReadResult {
    std::string_view content;  // String content (between quotes)
    size_t end_pos;            // Position after closing quote
    bool has_escapes;          // True if content contains backslash escapes
};

/**
 * Lazy cursor over raw JSON input.
 *
 * Operates directly on raw JSON bytes, parsing only the minimum needed
 * for each operation. This enables efficient partial access to large
 * JSON documents without materializing unused portions.
 *
 * Lifetime: The cursor stores a string_view, not a copy. The input
 * must outlive all cursors derived from it.
 *
 * Thread safety: Not thread-safe for concurrent operations on the same
 * cursor instance. Multiple threads can safely use separate cursors
 * over the same (read-only) input.
 */
class LazyJsonCursor {
public:
    /**
     * Create a cursor at the start of JSON input.
     * Automatically skips leading whitespace.
     */
    explicit LazyJsonCursor(std::string_view input);

    /**
     * Create a cursor at a specific position in JSON input.
     */
    LazyJsonCursor(std::string_view input, CursorPosition pos);

    // Type detection (peeks at current position after whitespace)
    bool is_null() const;
    bool is_bool() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    /**
     * Navigate to object field by key.
     * Returns cursor positioned at the field's value, or error if not found.
     */
    Result<LazyJsonCursor> get_field(std::string_view key) const;

    /**
     * Navigate to array element by index.
     * Returns cursor positioned at the element, or error if out of bounds.
     */
    Result<LazyJsonCursor> get_at(size_t index) const;

    /**
     * Materialize current value as JsonValue (full subtree).
     * Parses the complete value at current position.
     */
    Result<JsonValue> materialize() const;

    /**
     * Materialize current value as bool.
     * Returns TypeMismatch if not a boolean.
     */
    Result<bool> materialize_bool() const;

    /**
     * Materialize current value as number.
     * Returns TypeMismatch if not a number.
     */
    Result<double> materialize_number() const;

    /**
     * Materialize current value as string.
     * Handles escape sequences. Returns TypeMismatch if not a string.
     */
    Result<std::string> materialize_string() const;

    /**
     * Skip current value, return cursor after it.
     * Useful for iteration without materialization.
     */
    Result<LazyJsonCursor> skip() const;

    /**
     * Get current position in input.
     */
    CursorPosition position() const { return pos_; }

    /**
     * Get remaining input from current position.
     */
    std::string_view remaining() const;

    /**
     * Get the full input string_view.
     */
    std::string_view input() const { return input_; }

    // Forward declarations for iterators
    class FieldIterator;
    class ElementIterator;

    /**
     * Iterate over object fields lazily.
     * Returns TypeMismatch if not an object.
     */
    Result<FieldIterator> iter_fields() const;

    /**
     * Iterate over array elements lazily.
     * Returns TypeMismatch if not an array.
     */
    Result<ElementIterator> iter_elements() const;

    /**
     * Get number of elements in array (requires scanning).
     * Returns TypeMismatch if not an array.
     */
    Result<size_t> array_size() const;

    /**
     * Get number of fields in object (requires scanning).
     * Returns TypeMismatch if not an object.
     */
    Result<size_t> object_size() const;

private:
    std::string_view input_;
    CursorPosition pos_;

    // Internal helpers - all positions are byte offsets into input_

    /**
     * Skip whitespace starting from given position.
     * Returns position of first non-whitespace character.
     */
    size_t skip_whitespace(size_t from) const;

    /**
     * Skip a JSON string, handling escape sequences.
     * @param from Position of opening quote
     * @return Position after closing quote, or error
     */
    Result<size_t> skip_string(size_t from) const;

    /**
     * Skip a JSON number.
     * @param from Position of first character (digit or minus)
     * @return Position after number, or error
     */
    Result<size_t> skip_number(size_t from) const;

    /**
     * Skip a JSON container (object or array).
     * Uses iterative bracket counting to avoid stack overflow.
     * @param from Position of opening bracket
     * @return Position after closing bracket, or error
     */
    Result<size_t> skip_container(size_t from) const;

    /**
     * Skip any JSON value at the given position.
     * @param from Position of value start (after whitespace)
     * @return Position after value, or error
     */
    Result<size_t> skip_value(size_t from) const;

    /**
     * Read string content without unescaping.
     * @param from Position of opening quote
     * @return String content, end position, and escape flag
     */
    Result<StringReadResult> read_string_content(size_t from) const;

    /**
     * Get character at current position (after whitespace skip).
     */
    char peek_char() const;
};

/**
 * Iterator over object fields.
 * Yields (key, value cursor) pairs lazily.
 */
class LazyJsonCursor::FieldIterator {
public:
    /**
     * Field entry with lazy key access.
     */
    struct Field {
        std::string_view key_raw;     // Raw key content (may contain escapes)
        bool key_has_escapes;         // True if key needs unescaping
        LazyJsonCursor value;         // Cursor at field value

        /**
         * Get unescaped key string.
         */
        std::string key() const;
    };

    FieldIterator() : input_(), pos_(0), at_end_(true) {}
    FieldIterator(std::string_view input, size_t pos);

    FieldIterator& operator++();
    Field operator*() const;
    bool operator==(const FieldIterator& other) const;
    bool operator!=(const FieldIterator& other) const { return !(*this == other); }

    /**
     * Check if iterator has reached end.
     */
    bool at_end() const { return at_end_; }

private:
    std::string_view input_;
    size_t pos_;
    bool at_end_;

    void advance_to_next();
};

/**
 * Iterator over array elements.
 * Yields LazyJsonCursor for each element.
 */
class LazyJsonCursor::ElementIterator {
public:
    ElementIterator() : input_(), pos_(0), at_end_(true) {}
    ElementIterator(std::string_view input, size_t pos);

    ElementIterator& operator++();
    LazyJsonCursor operator*() const;
    bool operator==(const ElementIterator& other) const;
    bool operator!=(const ElementIterator& other) const { return !(*this == other); }

    /**
     * Check if iterator has reached end.
     */
    bool at_end() const { return at_end_; }

private:
    std::string_view input_;
    size_t pos_;
    bool at_end_;

    void advance_to_next();
};

// JSONPath integration for lazy parsing

/**
 * Evaluate JSONPath lazily, materializing only matched values.
 * @param json Raw JSON input
 * @param path Compiled JSONPath expression
 * @param limit Maximum number of results (default unlimited)
 * @return Vector of materialized JsonValue results
 */
std::vector<JsonValue> eval_jsonpath_lazy(
    std::string_view json,
    const class CompiledPath& path,
    size_t limit = std::numeric_limits<size_t>::max()
);

/**
 * Find JSONPath matches without materializing.
 * @param json Raw JSON input
 * @param path Compiled JSONPath expression
 * @param limit Maximum number of results (default unlimited)
 * @return Vector of LazyJsonCursor positioned at matches
 */
std::vector<LazyJsonCursor> find_jsonpath_lazy(
    std::string_view json,
    const class CompiledPath& path,
    size_t limit = std::numeric_limits<size_t>::max()
);

} // namespace strata

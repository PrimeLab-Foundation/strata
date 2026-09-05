#pragma once

/**
 * @file ndjson_stream.hpp
 * @brief Line-delimited JSON: one document per line.
 *
 *     {"name": "Alice", "age": 30}
 *     {"name": "Bob", "age": 25}
 *
 * The stream **borrows** its buffer: the caller keeps the data alive for as
 * long as the stream is used (docs/architecture/SKILL.md, invariant 7). Blank
 * lines are skipped and `\r\n` endings are handled, so a file written on
 * Windows reads the same.
 */

#include "strata/json/json_core.hpp"

#include <cstddef>
#include <string_view>
#include <vector>

namespace strata {

class NdjsonStream {
  public:
    /// @param data The whole NDJSON text; must outlive the stream.
    explicit NdjsonStream(std::string_view data) noexcept : data_(data) {}

    /**
     * Parse the next line.
     *
     * @return Status::Ok with the value, Status::ParseError for a malformed
     *         line, Status::DepthExceeded for one nested past
     *         strata::kMaxNestingDepth, or **Status::KeyNotFound at end of
     *         stream** — a deliberate reuse of that code as the end marker, so
     *         no separate sentinel is needed.
     */
    [[nodiscard]] Result<JsonValue> next();

    /**
     * The next non-blank line, unparsed.
     *
     * Lets a caller that has its own representation - the Python builder, for
     * one - parse the line directly instead of building a C++ tree first and
     * converting it, which would also round integers through a double.
     *
     * @return false at end of input. The view borrows the stream's buffer.
     */
    [[nodiscard]] bool next_line(std::string_view& line) noexcept { return take_line(line); }

    /// Whether another non-blank line remains.
    [[nodiscard]] bool has_next() const noexcept;

    /// Byte offset of the line `next()` will read.
    [[nodiscard]] size_t position() const noexcept { return position_; }

    /// 1-based number of the line most recently returned by `next()`.
    [[nodiscard]] size_t line_number() const noexcept { return line_number_; }

    /**
     * Parse every remaining line.
     *
     * @param skip_errors When false, the first malformed line stops the parse
     *        and @p failed_line reports which one it was. When true, malformed
     *        lines are dropped and the rest are returned.
     * @param failed_line Set to the 1-based number of the offending line when
     *        the parse stops; left at 0 otherwise. The returned status says
     *        why that line stopped it: ParseError, or DepthExceeded.
     */
    [[nodiscard]] Result<std::vector<JsonValue>> parse_all(bool skip_errors,
                                                           size_t* failed_line = nullptr);

  private:
    /// The next line's bounds, or false at end of input.
    [[nodiscard]] bool take_line(std::string_view& line) noexcept;

    std::string_view data_;
    size_t position_ = 0;
    size_t line_number_ = 0;
};

} // namespace strata

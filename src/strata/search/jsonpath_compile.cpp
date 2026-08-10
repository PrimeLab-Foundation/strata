/**
 * @file jsonpath_compile.cpp
 * @brief Recursive-descent compiler for the supported JSONPath subset.
 *
 * Non-throwing throughout: a malformed expression is a `Status::ParseError`,
 * never an exception. That includes the cases the previous implementation got
 * wrong, an unclosed quoted string most of all.
 */

#include "strata/search/jsonpath.hpp"
#include "strata/util/fast_parse.hpp"

#include <charconv>
#include <cstddef>
#include <system_error>

namespace strata {

namespace {

[[nodiscard]] constexpr bool is_digit(char c) noexcept { return c >= '0' && c <= '9'; }

/// Field names are `[A-Za-z0-9_]+` in dot form.
[[nodiscard]] constexpr bool is_name_char(char c) noexcept {
    return is_digit(c) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

class PathParser {
  public:
    explicit PathParser(std::string_view text) noexcept : text_(text) {}

    [[nodiscard]] bool parse(CompiledPath& out) {
        skip_spaces();
        if (!take('$')) // the root is mandatory
            return false;
        out.push_back(PathStep{.op = PathOp::Root});

        for (;;) {
            skip_spaces();
            if (at_end())
                return true;
            if (peek() == '.') {
                if (!parse_dot(out))
                    return false;
            } else if (peek() == '[') {
                if (!parse_bracket(out))
                    return false;
            } else {
                return false; // stray text between steps
            }
        }
    }

  private:
    [[nodiscard]] bool at_end() const noexcept { return position_ >= text_.size(); }
    [[nodiscard]] char peek() const noexcept { return at_end() ? '\0' : text_[position_]; }
    [[nodiscard]] char peek_at(size_t offset) const noexcept {
        return position_ + offset < text_.size() ? text_[position_ + offset] : '\0';
    }

    bool take(char expected) noexcept {
        if (peek() != expected)
            return false;
        ++position_;
        return true;
    }

    void skip_spaces() noexcept {
        while (!at_end() && (text_[position_] == ' ' || text_[position_] == '\t'))
            ++position_;
    }

    /// `.name`, `.*`, or `..name`
    [[nodiscard]] bool parse_dot(CompiledPath& out) {
        ++position_; // the first '.'
        if (peek() == '.') {
            ++position_;
            // `$..*` and `$..[...]` are outside the supported subset.
            std::string name;
            if (!read_name(name))
                return false;
            out.push_back(PathStep{.op = PathOp::RecursiveDescent, .name = std::move(name)});
            return true;
        }
        if (peek() == '*') {
            ++position_;
            out.push_back(PathStep{.op = PathOp::Wildcard});
            return true;
        }
        std::string name;
        if (!read_name(name))
            return false;
        out.push_back(PathStep{.op = PathOp::Field, .name = std::move(name)});
        return true;
    }

    [[nodiscard]] bool read_name(std::string& out) {
        const size_t start = position_;
        while (!at_end() && is_name_char(text_[position_]))
            ++position_;
        if (position_ == start)
            return false; // an empty name is not a name
        out.assign(text_.substr(start, position_ - start));
        return true;
    }

    /// Everything inside `[...]`.
    [[nodiscard]] bool parse_bracket(CompiledPath& out) {
        ++position_; // '['
        skip_spaces();

        if (peek() == '*') {
            ++position_;
            skip_spaces();
            if (!take(']'))
                return false;
            out.push_back(PathStep{.op = PathOp::Wildcard});
            return true;
        }
        if (peek() == '?')
            return parse_filter(out);
        if (peek() == '\'' || peek() == '"')
            return parse_quoted_field(out);
        return parse_index_or_slice(out);
    }

    /// `["name"]` / `['name']`, with the documented escapes and no others.
    [[nodiscard]] bool parse_quoted_field(CompiledPath& out) {
        std::string name;
        if (!read_quoted(name))
            return false;
        skip_spaces();
        if (peek() == ',')
            return false; // unions are not supported
        if (!take(']'))
            return false;
        out.push_back(PathStep{.op = PathOp::Field, .name = std::move(name)});
        return true;
    }

    [[nodiscard]] bool read_quoted(std::string& out) {
        const char quote = peek();
        if (quote != '\'' && quote != '"')
            return false;
        ++position_;

        out.clear();
        while (!at_end()) {
            const char c = text_[position_++];
            if (c == quote)
                return true;
            if (c != '\\') {
                out.push_back(c);
                continue;
            }
            if (at_end())
                return false; // trailing backslash
            switch (text_[position_++]) {
            case 'n':
                out.push_back('\n');
                break;
            case 't':
                out.push_back('\t');
                break;
            case 'r':
                out.push_back('\r');
                break;
            case '\\':
                out.push_back('\\');
                break;
            case '"':
                out.push_back('"');
                break;
            case '\'':
                out.push_back('\'');
                break;
            default:
                return false; // \uXXXX and friends are not supported
            }
        }
        return false; // unterminated: an error, never an exception
    }

    /// `[n]` or `[start:end:step]`, components omittable.
    [[nodiscard]] bool parse_index_or_slice(CompiledPath& out) {
        int64_t first = 0;
        const bool has_first = read_int(first);

        skip_spaces();
        if (peek() == ']') {
            ++position_;
            if (!has_first)
                return false; // `[]` means nothing
            out.push_back(PathStep{.op = PathOp::Index, .index = first});
            return true;
        }
        if (peek() == ',')
            return false; // unions are not supported
        if (peek() != ':')
            return false;

        PathStep slice{.op = PathOp::Slice};
        slice.has_start = has_first;
        slice.start = first;

        ++position_; // ':'
        skip_spaces();
        int64_t second = 0;
        if (read_int(second)) {
            slice.has_end = true;
            slice.end = second;
        }

        skip_spaces();
        if (peek() == ':') {
            ++position_;
            skip_spaces();
            int64_t third = 0;
            if (read_int(third)) {
                if (third == 0)
                    return false; // a zero step would not advance
                slice.step = third;
            }
            skip_spaces();
        }
        if (!take(']'))
            return false;

        out.push_back(std::move(slice));
        return true;
    }

    [[nodiscard]] bool read_int(int64_t& out) {
        const size_t start = position_;
        if (peek() == '-' || peek() == '+')
            ++position_;
        const size_t digits_start = position_;
        while (!at_end() && is_digit(text_[position_]))
            ++position_;
        if (position_ == digits_start) {
            position_ = start; // nothing consumed
            return false;
        }
        const auto result = std::from_chars(text_.data() + start, text_.data() + position_, out);
        if (result.ec != std::errc{}) {
            position_ = start;
            return false;
        }
        return true;
    }

    /// `[?(@.name op value)]`, single level, one comparison.
    [[nodiscard]] bool parse_filter(CompiledPath& out) {
        ++position_; // '?'
        skip_spaces();
        if (!take('('))
            return false;
        skip_spaces();
        if (!take('@'))
            return false;

        PathStep filter{.op = PathOp::Filter};
        if (peek() == '.') {
            ++position_;
            if (!read_name(filter.name))
                return false;
        } else if (peek() == '[') {
            ++position_;
            skip_spaces();
            if (!read_quoted(filter.name))
                return false;
            skip_spaces();
            if (!take(']'))
                return false;
        } else {
            return false; // existence filters and bare `@` are not supported
        }

        skip_spaces();
        if (!read_filter_op(filter.filter_op))
            return false;
        skip_spaces();

        if (peek() == '\'' || peek() == '"') {
            if (!read_quoted(filter.filter_string))
                return false;
            filter.filter_is_number = false;
            // Ordering comparisons on strings are outside the subset.
            if (filter.filter_op != FilterOp::Equal && filter.filter_op != FilterOp::NotEqual)
                return false;
        } else {
            double number = 0.0;
            if (!read_number(number))
                return false;
            filter.filter_is_number = true;
            filter.filter_number = number;
        }

        skip_spaces();
        if (!take(')'))
            return false;
        skip_spaces();
        if (!take(']'))
            return false;

        out.push_back(std::move(filter));
        return true;
    }

    [[nodiscard]] bool read_filter_op(FilterOp& out) noexcept {
        if (peek() == '=' && peek_at(1) == '=') {
            position_ += 2;
            out = FilterOp::Equal;
            return true;
        }
        if (peek() == '!' && peek_at(1) == '=') {
            position_ += 2;
            out = FilterOp::NotEqual;
            return true;
        }
        if (peek() == '<') {
            ++position_;
            out = take('=') ? FilterOp::LessEqual : FilterOp::Less;
            return true;
        }
        if (peek() == '>') {
            ++position_;
            out = take('=') ? FilterOp::GreaterEqual : FilterOp::Greater;
            return true;
        }
        return false; // includes && and ||, which are not supported
    }

    [[nodiscard]] bool read_number(double& out) {
        const size_t start = position_;
        if (peek() == '-' || peek() == '+')
            ++position_;
        while (!at_end() &&
               (is_digit(text_[position_]) || text_[position_] == '.' || text_[position_] == 'e' ||
                text_[position_] == 'E' || text_[position_] == '-' || text_[position_] == '+'))
            ++position_;
        if (position_ == start)
            return false;
        const auto result =
            util::from_chars_double(text_.data() + start, text_.data() + position_, out);
        if (result.ec != std::errc{} || result.ptr != text_.data() + position_) {
            position_ = start;
            return false;
        }
        return true;
    }

    std::string_view text_;
    size_t position_ = 0;
};

} // namespace

Result<CompiledPath> compile_jsonpath(std::string_view expression) {
    CompiledPath path;
    PathParser parser(expression);
    if (!parser.parse(path))
        return {Status::ParseError, {}};
    return {Status::Ok, std::move(path)};
}

bool is_streamable(const CompiledPath& path) noexcept {
    size_t selectors = 0;
    for (const PathStep& step : path) {
        if (step.op == PathOp::Slice || step.op == PathOp::Filter ||
            step.op == PathOp::RecursiveDescent)
            return false;
        // A negative index counts from the end, and a stream does not know
        // the end until the array is over -- resolving it means buffering,
        // which is exactly what streaming exists to avoid.
        if (step.op == PathOp::Index && step.index < 0)
            return false;
        if (step.op != PathOp::Root)
            ++selectors;
    }
    // Bare `$` selects the whole document; "streaming" it would build the
    // full tree anyway, which is just load() with extra steps.
    return selectors > 0;
}

} // namespace strata

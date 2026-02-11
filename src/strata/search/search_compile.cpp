#include "strata/search/search.hpp"

#include <cctype>
#include <cstdint>
#include <stdexcept>

namespace strata {

// ============================================================================
// JSONPath Parser / Compiler
// ============================================================================

struct PathParser {
    std::string_view expr;
    size_t pos = 0;

    PathParser(std::string_view e) : expr(e) {}

    bool eof() const { return pos >= expr.size(); }
    char peek() const { return eof() ? '\0' : expr[pos]; }
    char get() { return eof() ? '\0' : expr[pos++]; }

    void skip_ws() {
        while (!eof() && std::isspace(static_cast<unsigned char>(peek()))) {
            ++pos;
        }
    }

    bool consume(char c) {
        skip_ws();
        if (peek() == c) {
            ++pos;
            return true;
        }
        return false;
    }

    std::string parse_identifier() {
        std::string result;
        while (!eof() && (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')) {
            result += get();
        }
        return result;
    }

    std::string parse_quoted_string() {
        // Assumes we're at the opening quote
        char quote = get(); // ' or "
        std::string result;

        while (!eof()) {
            char c = get();
            if (c == quote) {
                return result;
            }
            if (c == '\\' && !eof()) {
                // Simple escape handling
                char next = get();
                switch (next) {
                case 'n':
                    result += '\n';
                    break;
                case 't':
                    result += '\t';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case '"':
                    result += '"';
                    break;
                case '\'':
                    result += '\'';
                    break;
                default:
                    result += next;
                }
            } else {
                result += c;
            }
        }

        throw std::runtime_error("Unclosed quoted string in JSONPath");
    }

    int64_t parse_number() {
        skip_ws();
        bool negative = false;
        if (peek() == '-') {
            negative = true;
            ++pos;
        }

        int64_t result = 0;
        if (!std::isdigit(static_cast<unsigned char>(peek()))) {
            return 0;
        }

        while (!eof() && std::isdigit(static_cast<unsigned char>(peek()))) {
            result = result * 10 + (get() - '0');
        }

        return negative ? -result : result;
    }

    // Parse a number that can be floating point (for filters)
    double parse_double() {
        skip_ws();
        size_t start = pos;

        // Handle negative
        if (peek() == '-') {
            ++pos;
        }

        // Must have at least one digit
        if (!std::isdigit(static_cast<unsigned char>(peek()))) {
            throw std::runtime_error("Expected digit in number");
        }

        // Parse integer part
        while (!eof() && std::isdigit(static_cast<unsigned char>(peek()))) {
            ++pos;
        }

        // Parse decimal part if present
        if (peek() == '.') {
            ++pos;
            while (!eof() && std::isdigit(static_cast<unsigned char>(peek()))) {
                ++pos;
            }
        }

        // Parse exponent if present
        if (peek() == 'e' || peek() == 'E') {
            ++pos;
            if (peek() == '+' || peek() == '-') {
                ++pos;
            }
            while (!eof() && std::isdigit(static_cast<unsigned char>(peek()))) {
                ++pos;
            }
        }

        std::string num_str(expr.substr(start, pos - start));
        try {
            return std::stod(num_str);
        } catch (...) {
            throw std::runtime_error("Invalid number format");
        }
    }

    // Parse a filter predicate (simplified)
    Result<FilterPredicate> parse_filter() {
        FilterPredicate filter;
        skip_ws();

        // We've already seen [?, so expect ( next
        if (!consume('(')) {
            return {Status::ParseError, filter};
        }

        skip_ws();
        // Expect @.field or @['field']
        if (!consume('@')) {
            return {Status::ParseError, filter};
        }

        skip_ws();
        if (consume('.')) {
            filter.field = parse_identifier();
            if (filter.field.empty()) {
                return {Status::ParseError, filter};
            }
        } else if (consume('[')) {
            skip_ws();
            if (peek() == '"' || peek() == '\'') {
                filter.field = parse_quoted_string();
            } else {
                return {Status::ParseError, filter};
            }
            skip_ws();
            if (!consume(']')) {
                return {Status::ParseError, filter};
            }
        } else {
            return {Status::ParseError, filter};
        }

        skip_ws();

        // Parse operator - need to check multi-char operators first
        char first = peek();
        if (first == '=' || first == '!' || first == '>' || first == '<') {
            get(); // consume first char
            char second = peek();

            if (first == '=' && second == '=') {
                get();
                filter.op = FilterOp::Equal;
            } else if (first == '!' && second == '=') {
                get();
                filter.op = FilterOp::NotEqual;
            } else if (first == '>' && second == '=') {
                get();
                filter.op = FilterOp::GreaterEqual;
            } else if (first == '>') {
                filter.op = FilterOp::GreaterThan;
            } else if (first == '<' && second == '=') {
                get();
                filter.op = FilterOp::LessEqual;
            } else if (first == '<') {
                filter.op = FilterOp::LessThan;
            } else {
                return {Status::ParseError, filter};
            }
        } else {
            return {Status::ParseError, filter};
        }

        skip_ws();

        // Parse value (number or string)
        if (peek() == '"' || peek() == '\'') {
            filter.is_numeric = false;
            filter.string_value = parse_quoted_string();
        } else if (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '-' ||
                   peek() == '.') {
            filter.is_numeric = true;
            try {
                filter.numeric_value = parse_double();
            } catch (...) {
                return {Status::ParseError, filter};
            }
        } else {
            return {Status::ParseError, filter};
        }

        skip_ws();
        if (!consume(')')) {
            return {Status::ParseError, filter};
        }

        return {Status::Ok, filter};
    }

    Result<CompiledPath> parse() {
        std::vector<PathStep> steps;

        skip_ws();

        // Must start with $
        if (!consume('$')) {
            return {Status::ParseError, CompiledPath{}};
        }

        steps.emplace_back(PathOp::Root);

        while (!eof()) {
            skip_ws();

            if (consume('.')) {
                skip_ws();

                // Check for recursive descent (..)
                if (consume('.')) {
                    // Recursive descent followed by field name
                    std::string field = parse_identifier();
                    if (field.empty()) {
                        return {Status::ParseError, CompiledPath{}};
                    }
                    PathStep step(PathOp::RecursiveDescent);
                    step.field = std::move(field);
                    steps.push_back(step);
                } else if (consume('*')) {
                    // Wildcard
                    steps.emplace_back(PathOp::Wildcard);
                } else {
                    // Regular field
                    std::string field = parse_identifier();
                    if (field.empty()) {
                        return {Status::ParseError, CompiledPath{}};
                    }
                    steps.emplace_back(PathOp::Field, std::move(field));
                }

            } else if (consume('[')) {
                skip_ws();

                if (consume('?')) {
                    // Filter predicate - parse_filter expects to see '(' next
                    auto filter_result = parse_filter();
                    if (!filter_result.ok()) {
                        return {Status::ParseError, CompiledPath{}};
                    }
                    PathStep step(PathOp::Filter);
                    step.filter = filter_result.value;
                    steps.push_back(step);
                    skip_ws();
                    if (!consume(']')) {
                        return {Status::ParseError, CompiledPath{}};
                    }

                } else if (consume('*')) {
                    // Wildcard
                    steps.emplace_back(PathOp::Wildcard);
                    skip_ws();
                    if (!consume(']')) {
                        return {Status::ParseError, CompiledPath{}};
                    }

                } else if (peek() == '"' || peek() == '\'') {
                    // Quoted field name
                    std::string field = parse_quoted_string();
                    steps.emplace_back(PathOp::Field, std::move(field));
                    skip_ws();
                    if (!consume(']')) {
                        return {Status::ParseError, CompiledPath{}};
                    }

                } else if (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '-' ||
                           peek() == ':') {
                    // Array index or slice
                    PathStep step(PathOp::Slice);

                    // Check if this starts with ':' (omitted start)
                    if (peek() == ':') {
                        step.slice_start = 0;
                        consume(':');
                    } else {
                        int64_t first_num = parse_number();
                        skip_ws();

                        if (consume(':')) {
                            // It's a slice
                            step.slice_start = first_num;
                        } else {
                            // It's just an index
                            steps.emplace_back(PathOp::Index, first_num);
                            skip_ws();
                            if (!consume(']')) {
                                return {Status::ParseError, CompiledPath{}};
                            }
                            continue;
                        }
                    }

                    // Parse end (optional)
                    skip_ws();
                    if (peek() == ':' || peek() == ']') {
                        step.slice_end = INT64_MAX;
                    } else if (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '-') {
                        step.slice_end = parse_number();
                    } else {
                        step.slice_end = INT64_MAX;
                    }

                    // Parse step (optional)
                    skip_ws();
                    if (consume(':')) {
                        skip_ws();
                        if (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '-') {
                            step.slice_step = parse_number();
                        } else {
                            step.slice_step = 1;
                        }
                    } else {
                        step.slice_step = 1;
                    }

                    steps.push_back(step);
                    skip_ws();
                    if (!consume(']')) {
                        return {Status::ParseError, CompiledPath{}};
                    }

                } else {
                    return {Status::ParseError, CompiledPath{}};
                }

            } else if (!eof()) {
                // Unknown character
                return {Status::ParseError, CompiledPath{}};
            }
        }

        return {Status::Ok, CompiledPath(std::move(steps))};
    }
};

Result<CompiledPath> compile_search_path(std::string_view expr) {
    PathParser parser(expr);
    return parser.parse();
}

bool CompiledPath::is_simple_field_extraction() const {
    if (steps_.size() == 2 && steps_[0].op == PathOp::Root &&
        steps_[1].op == PathOp::Field) {
        return true;
    }
    if (steps_.size() == 3 && steps_[0].op == PathOp::Root &&
        steps_[1].op == PathOp::Wildcard && steps_[2].op == PathOp::Field) {
        return true;
    }
    return false;
}

} // namespace strata

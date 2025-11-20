#include "jp_compile.hpp"

#include <regex>

namespace strata {

CompiledExpr::CompiledExpr(const std::string& expr) : expression_(expr), valid_(false) {

    try {
        // Compile the expression with jsoncons (for validation and fallback)
        compiled_expr_ = jsoncons::jsonpath::make_expression<jsoncons::json>(expr);
        valid_ = true;

        // Extract field names for future optimization
        extract_field_names(expr);

        // Build pushdown plan (PR #2)
        build_pushdown_plan(expr);

    } catch (const std::exception& e) {
        valid_ = false;
        error_message_ = std::string("Failed to compile JSONPath expression: ") + e.what();
    }
}

std::vector<jsoncons::json> CompiledExpr::evaluate(const jsoncons::json& doc) const {
    if (!valid_ || !compiled_expr_.has_value()) {
        return {};
    }

    std::vector<jsoncons::json> results;

    try {
        auto result = compiled_expr_->evaluate(doc);

        // Handle result (can be single value or array)
        if (result.is_array()) {
            for (const auto& item : result.array_range()) {
                results.push_back(item);
            }
        } else {
            // Push all non-array results, INCLUDING null
            // v0.1 compatibility: preserve null matches
            results.push_back(result);
        }

    } catch (const std::exception&) {
        // Evaluation failed (e.g., path doesn't match) - return empty
    }

    return results;
}

void CompiledExpr::extract_field_names(const std::string& expr) {
    // Simple regex-based extraction of field names
    // Matches patterns like: .fieldname, ['fieldname'], ["fieldname"], @.fieldname
    std::regex field_pattern(R"([@$]?\.([a-zA-Z_][a-zA-Z0-9_]*)|['"]([^'"]+)['"])");
    std::smatch match;

    std::string::const_iterator search_start(expr.cbegin());
    while (std::regex_search(search_start, expr.cend(), match, field_pattern)) {
        if (match[1].length() > 0) {
            referenced_fields_.insert(match[1].str());
        } else if (match[2].length() > 0) {
            referenced_fields_.insert(match[2].str());
        }
        search_start = match.suffix().first;
    }
}

// ============================================================================
// SECTION: Pushdown Plan Builder (PR #2 Phase 2)
// ============================================================================

/**
 * Helper: Parse literal value from string (Phase 2)
 */
static bool parse_literal(const std::string& str, Literal& out) {
    std::string trimmed = str;
    // Trim whitespace
    size_t start = 0;
    while (start < trimmed.size() && std::isspace(trimmed[start]))
        ++start;
    size_t end = trimmed.size();
    while (end > start && std::isspace(trimmed[end - 1]))
        --end;
    trimmed = trimmed.substr(start, end - start);

    if (trimmed.empty())
        return false;

    // null
    if (trimmed == "null") {
        out = nullptr;
        return true;
    }

    // Boolean
    if (trimmed == "true") {
        out = true;
        return true;
    }
    if (trimmed == "false") {
        out = false;
        return true;
    }

    // String (quoted)
    if (trimmed.size() >= 2 && (trimmed.front() == '\'' || trimmed.front() == '"')) {
        if (trimmed.back() == trimmed.front()) {
            out = trimmed.substr(1, trimmed.size() - 2);
            return true;
        }
    }

    // Number (int or double)
    try {
        size_t pos = 0;
        // Try parsing as integer first
        if (trimmed.find('.') == std::string::npos && trimmed.find('e') == std::string::npos &&
            trimmed.find('E') == std::string::npos) {
            int64_t ival = std::stoll(trimmed, &pos);
            if (pos == trimmed.size()) {
                out = ival;
                return true;
            }
        }
        // Parse as double
        double dval = std::stod(trimmed, &pos);
        if (pos == trimmed.size()) {
            out = dval;
            return true;
        }
    } catch (...) {
        return false;
    }

    return false;
}

/**
 * Helper: Parse filter expression (Phase 2 - DNF support)
 * Simplified parser for: ?(@.field <op> value) with &&/|| support
 * Returns true if successfully parsed into DNF
 */
static bool parse_filter(const std::string& filter_expr, FilterDNF& out_dnf) {
    // This is a simplified parser for Phase 2
    // Full JSONPath filter parsing is complex; we support common patterns
    // Pattern: ?(@.field <op> value) [&& @.field2 <op2> value2] [|| (...)]

    std::string expr = filter_expr;
    if (expr.empty() || expr[0] != '?')
        return false;

    // Remove '?' prefix and surrounding parens if present
    expr = expr.substr(1);
    size_t start = 0;
    while (start < expr.size() && std::isspace(expr[start]))
        ++start;
    if (start < expr.size() && expr[start] == '(') {
        expr = expr.substr(start + 1);
        size_t end = expr.rfind(')');
        if (end != std::string::npos) {
            expr = expr.substr(0, end);
        }
    }

    // For Phase 2, we'll implement basic support for simple filters
    // Full boolean logic parsing would need a proper expression parser
    // For now: single condition or multiple ANDs

    // Split by || for OR clauses (simple approach)
    std::vector<std::string> or_clauses;
    size_t or_pos = 0;
    size_t last_or = 0;
    int paren_depth = 0;
    for (size_t i = 0; i < expr.size(); ++i) {
        if (expr[i] == '(')
            ++paren_depth;
        else if (expr[i] == ')')
            --paren_depth;
        else if (paren_depth == 0 && i + 1 < expr.size() && expr[i] == '|' && expr[i + 1] == '|') {
            or_clauses.push_back(expr.substr(last_or, i - last_or));
            last_or = i + 2;
            ++i;
        }
    }
    or_clauses.push_back(expr.substr(last_or));

    // Parse each OR clause (which contains ANDs)
    for (const auto& or_clause : or_clauses) {
        std::vector<FilterAtom> and_atoms;

        // Split by && for AND atoms
        std::vector<std::string> and_parts;
        size_t last_and = 0;
        paren_depth = 0;
        std::string clause = or_clause;
        for (size_t i = 0; i < clause.size(); ++i) {
            if (clause[i] == '(')
                ++paren_depth;
            else if (clause[i] == ')')
                --paren_depth;
            else if (paren_depth == 0 && i + 1 < clause.size() && clause[i] == '&' &&
                     clause[i + 1] == '&') {
                and_parts.push_back(clause.substr(last_and, i - last_and));
                last_and = i + 2;
                ++i;
            }
        }
        and_parts.push_back(clause.substr(last_and));

        // Parse each AND atom
        for (const auto& and_part : and_parts) {
            std::string atom_str = and_part;
            // Trim
            size_t s = 0;
            while (s < atom_str.size() && std::isspace(atom_str[s]))
                ++s;
            size_t e = atom_str.size();
            while (e > s && std::isspace(atom_str[e - 1]))
                --e;
            atom_str = atom_str.substr(s, e - s);

            if (atom_str.empty())
                continue;

            // Parse atom: @.field <op> value
            // Look for @.
            if (atom_str.size() < 2 || atom_str[0] != '@' || atom_str[1] != '.') {
                return false; // Unsupported pattern
            }

            // Extract field path (support @.a.b.c)
            size_t field_start = 2;
            size_t field_end = field_start;
            RelPath lhs;
            while (field_end < atom_str.size()) {
                if (atom_str[field_end] == '.') {
                    std::string fname = atom_str.substr(field_start, field_end - field_start);
                    lhs.fields.emplace_back(fname);
                    field_start = field_end + 1;
                    ++field_end;
                } else if (std::isalnum(atom_str[field_end]) || atom_str[field_end] == '_') {
                    ++field_end;
                } else {
                    break;
                }
            }
            if (field_end > field_start) {
                std::string fname = atom_str.substr(field_start, field_end - field_start);
                lhs.fields.emplace_back(fname);
            }

            if (lhs.fields.empty())
                return false;

            // Check if this is just existence check (@.field)
            std::string remainder = atom_str.substr(field_end);
            // Trim
            s = 0;
            while (s < remainder.size() && std::isspace(remainder[s]))
                ++s;
            remainder = remainder.substr(s);

            if (remainder.empty()) {
                // EXISTS check
                FilterAtom atom;
                atom.lhs = lhs;
                atom.op = Cmp::EXISTS;
                and_atoms.push_back(atom);
                continue;
            }

            // Parse operator
            Cmp op;
            size_t op_len = 0;
            if (remainder.size() >= 2 && remainder.substr(0, 2) == "==") {
                op = Cmp::EQ;
                op_len = 2;
            } else if (remainder.size() >= 2 && remainder.substr(0, 2) == "!=") {
                op = Cmp::NE;
                op_len = 2;
            } else if (remainder.size() >= 2 && remainder.substr(0, 2) == ">=") {
                op = Cmp::GE;
                op_len = 2;
            } else if (remainder.size() >= 2 && remainder.substr(0, 2) == "<=") {
                op = Cmp::LE;
                op_len = 2;
            } else if (remainder.size() >= 1 && remainder[0] == '>') {
                op = Cmp::GT;
                op_len = 1;
            } else if (remainder.size() >= 1 && remainder[0] == '<') {
                op = Cmp::LT;
                op_len = 1;
            } else if (remainder.size() >= 2 && remainder.substr(0, 2) == "in") {
                op = Cmp::IN;
                op_len = 2;
            } else {
                return false; // Unknown operator
            }

            remainder = remainder.substr(op_len);
            // Trim
            s = 0;
            while (s < remainder.size() && std::isspace(remainder[s]))
                ++s;
            remainder = remainder.substr(s);

            FilterAtom atom;
            atom.lhs = lhs;
            atom.op = op;

            // Parse RHS
            if (op == Cmp::IN) {
                // Parse array: [val1, val2, ...]
                if (remainder.empty() || remainder[0] != '[')
                    return false;
                size_t arr_end = remainder.rfind(']');
                if (arr_end == std::string::npos)
                    return false;
                std::string arr_content = remainder.substr(1, arr_end - 1);

                // Split by comma (simple split, doesn't handle nested arrays)
                size_t pos = 0;
                while (pos < arr_content.size()) {
                    size_t comma_pos = arr_content.find(',', pos);
                    if (comma_pos == std::string::npos)
                        comma_pos = arr_content.size();
                    std::string val_str = arr_content.substr(pos, comma_pos - pos);
                    Literal lit;
                    if (!parse_literal(val_str, lit))
                        return false;
                    atom.rhs_list.push_back(lit);
                    pos = comma_pos + 1;
                }
            } else {
                // Parse single literal
                if (!parse_literal(remainder, atom.rhs))
                    return false;
            }

            and_atoms.push_back(atom);
        }

        if (!and_atoms.empty()) {
            out_dnf.clauses.push_back(and_atoms);
        }
    }

    return !out_dnf.clauses.empty();
}

/**
 * Build pushdown execution plan from JSONPath expression
 *
 * Phase 2 supports:
 * - Path: $, .field, [*], [n], [start:end:step]
 * - Filters: Full DNF with ==, !=, >, >=, <, <=, in, &&, ||
 * - Nested field paths in filters: @.a.b.c
 *
 * Unsupported (triggers fallback):
 * - Recursive descent (..)
 * - Negative indices/slices
 * - Union (comma-separated paths)
 * - Functions
 * - Complex filter expressions (nested parentheses, NOT)
 */
void CompiledExpr::build_pushdown_plan(const std::string& expr) {
    plan_ = PushdownPlan();
    plan_.supports_pushdown = false;

    // Phase 1: Simple pattern-based parser
    // We'll use regex to identify supported patterns
    // More robust parser can be added in future phases

    size_t pos = 0;
    bool supported = true;

    // Must start with $
    if (expr.empty() || expr[0] != '$') {
        return; // Not supported
    }

    Step root_step(Op::Root);
    plan_.steps.push_back(root_step);
    pos = 1;

    // Parse path components
    while (pos < expr.size() && supported) {
        // Skip whitespace
        while (pos < expr.size() && std::isspace(expr[pos])) {
            ++pos;
        }

        if (pos >= expr.size()) {
            break;
        }

        char ch = expr[pos];

        // Dot notation: .field
        if (ch == '.') {
            ++pos;
            if (pos >= expr.size()) {
                supported = false;
                break;
            }

            // Check for recursive descent (..)
            if (expr[pos] == '.') {
                // Recursive descent not supported in Phase 1
                supported = false;
                break;
            }

            // Extract field name
            size_t field_start = pos;
            while (pos < expr.size() && (std::isalnum(expr[pos]) || expr[pos] == '_')) {
                ++pos;
            }

            if (pos == field_start) {
                supported = false;
                break;
            }

            std::string field_name = expr.substr(field_start, pos - field_start);
            Step field_step(Op::Field);
            field_step.field = Symbol(field_name);
            plan_.steps.push_back(field_step);
        }
        // Bracket notation: [...]
        else if (ch == '[') {
            ++pos;
            size_t bracket_start = pos;

            // Find matching ]
            int depth = 1;
            while (pos < expr.size() && depth > 0) {
                if (expr[pos] == '[')
                    ++depth;
                else if (expr[pos] == ']')
                    --depth;
                ++pos;
            }

            if (depth != 0) {
                supported = false;
                break;
            }

            std::string bracket_content = expr.substr(bracket_start, pos - bracket_start - 1);

            // Trim whitespace
            size_t content_start = 0;
            while (content_start < bracket_content.size() &&
                   std::isspace(bracket_content[content_start])) {
                ++content_start;
            }
            size_t content_end = bracket_content.size();
            while (content_end > content_start && std::isspace(bracket_content[content_end - 1])) {
                --content_end;
            }
            bracket_content = bracket_content.substr(content_start, content_end - content_start);

            if (bracket_content.empty()) {
                supported = false;
                break;
            }

            // Wildcard: [*]
            if (bracket_content == "*") {
                Step wildcard_step(Op::Wildcard);
                plan_.steps.push_back(wildcard_step);
            }
            // Filter: [?(...)] - Phase 2
            // Filters iterate array elements and test each one
            else if (bracket_content[0] == '?') {
                FilterDNF dnf;
                if (!parse_filter(bracket_content, dnf)) {
                    supported = false;
                    break;
                }
                // Add implicit wildcard iteration before filter
                Step wildcard_step(Op::Wildcard);
                plan_.steps.push_back(wildcard_step);

                Step filter_step(Op::Filter);
                filter_step.dnf = dnf;
                plan_.steps.push_back(filter_step);
            }
            // Field name: ['field'] or ["field"]
            else if ((bracket_content[0] == '\'' || bracket_content[0] == '"') &&
                     bracket_content.size() >= 2 && bracket_content.back() == bracket_content[0]) {
                std::string field_name = bracket_content.substr(1, bracket_content.size() - 2);
                Step field_step(Op::Field);
                field_step.field = Symbol(field_name);
                plan_.steps.push_back(field_step);
            }
            // Slice or Index: [n] or [start:end] or [start:end:step]
            else {
                // Check for colon (slice notation)
                size_t colon_pos = bracket_content.find(':');
                if (colon_pos != std::string::npos) {
                    // Slice: [start:end:step]
                    std::vector<std::string> parts;
                    size_t last_pos = 0;
                    for (size_t i = 0; i <= bracket_content.size(); ++i) {
                        if (i == bracket_content.size() || bracket_content[i] == ':') {
                            parts.push_back(bracket_content.substr(last_pos, i - last_pos));
                            last_pos = i + 1;
                        }
                    }

                    if (parts.size() < 2 || parts.size() > 3) {
                        supported = false;
                        break;
                    }

                    try {
                        int64_t start = 0;
                        int64_t end = INT64_MAX;
                        int64_t step = 1;

                        // Parse start
                        if (!parts[0].empty()) {
                            std::string trimmed = parts[0];
                            size_t s = 0;
                            while (s < trimmed.size() && std::isspace(trimmed[s]))
                                ++s;
                            trimmed = trimmed.substr(s);
                            start = std::stoll(trimmed);
                            if (start < 0)
                                start = 0; // Clamp negative to 0
                        }

                        // Parse end
                        if (!parts[1].empty()) {
                            std::string trimmed = parts[1];
                            size_t s = 0;
                            while (s < trimmed.size() && std::isspace(trimmed[s]))
                                ++s;
                            trimmed = trimmed.substr(s);
                            end = std::stoll(trimmed);
                            if (end < 0)
                                end = 0; // Clamp negative to 0
                        }

                        // Parse step
                        if (parts.size() == 3 && !parts[2].empty()) {
                            std::string trimmed = parts[2];
                            size_t s = 0;
                            while (s < trimmed.size() && std::isspace(trimmed[s]))
                                ++s;
                            trimmed = trimmed.substr(s);
                            step = std::stoll(trimmed);
                            if (step <= 0) {
                                // Non-positive step not supported
                                supported = false;
                                break;
                            }
                        }

                        Step slice_step(Op::Slice);
                        slice_step.slice_start = start;
                        slice_step.slice_end = end;
                        slice_step.slice_step = step;
                        plan_.steps.push_back(slice_step);
                    } catch (...) {
                        supported = false;
                        break;
                    }
                } else {
                    // Index: [n]
                    try {
                        int64_t index = std::stoll(bracket_content);
                        if (index < 0) {
                            // Negative indices not supported
                            supported = false;
                            break;
                        }
                        Step index_step(Op::Index);
                        index_step.index = index;
                        plan_.steps.push_back(index_step);
                    } catch (...) {
                        // Not a valid index
                        supported = false;
                        break;
                    }
                }
            }
        } else {
            // Unknown syntax
            supported = false;
            break;
        }
    }

    // Phase 2 limitation: Filter followed by field access not supported
    // (simdjson OnDemand cursor consumption issue)
    if (supported) {
        for (size_t i = 0; i + 1 < plan_.steps.size(); ++i) {
            if (plan_.steps[i].op == Op::Filter && plan_.steps[i + 1].op == Op::Field) {
                supported = false;
                break;
            }
        }
    }

    plan_.supports_pushdown = supported && !plan_.steps.empty();
}

} // namespace strata

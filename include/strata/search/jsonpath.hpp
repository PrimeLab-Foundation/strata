#pragma once

/**
 * @file jsonpath.hpp
 * @brief JSONPath expression compiler and evaluator.
 *
 * Provides a two-phase JSONPath implementation:
 * 1. **Compile** — parse a JSONPath string (e.g. "$.users[*].name") into
 *    a CompiledPath, a vector of PathStep operations.
 * 2. **Evaluate** — walk the compiled steps over a JsonDocument/JsonCursor,
 *    collecting all matching JsonValues.
 *
 * Supported JSONPath syntax:
 *   $              — root
 *   $.field        — field access (dot notation)
 *   $["field"]     — field access (bracket notation)
 *   $.*  / $[*]    — wildcard (all values/elements)
 *   $[n]           — array index
 *   $[start:end:step] — array slice
 *   $..field       — recursive descent
 *   $[?(@.f > v)]  — filter predicate
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_cursor.hpp"
#include "strata/json/json_document.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

/// JSONPath operation types (one per PathStep).
enum class PathOp {
    Root,             ///< $ — document root
    Field,            ///< .field or ["field"] — object field access
    Wildcard,         ///< * — all elements/values
    Index,            ///< [n] — array index
    RecursiveDescent, ///< .. — recursive descent
    Slice,            ///< [start:end:step] — array slice
    Filter,           ///< [?(@.field > value)] — filter predicate
    End               ///< Sentinel marking end of path
};

/// Filter comparison operators used in filter predicates.
enum class FilterOp {
    Equal,        ///< ==
    NotEqual,     ///< !=
    GreaterThan,  ///< >
    GreaterEqual, ///< >=
    LessThan,     ///< <
    LessEqual,    ///< <=
    And,          ///< &&
    Or            ///< ||
};

/// Filter predicate for [?(@.field op value)] expressions.
struct FilterPredicate {
    std::string field;        ///< Field name (e.g. "age" from @.age)
    FilterOp op;              ///< Comparison operator
    double numeric_value;     ///< Comparand for numeric comparisons
    std::string string_value; ///< Comparand for string comparisons
    bool is_numeric;          ///< true → numeric comparison, false → string

    FilterPredicate() : op(FilterOp::Equal), numeric_value(0), is_numeric(true) {}
};

/// Single step in a compiled JSONPath expression.
struct PathStep {
    PathOp op;
    std::string field;      ///< For PathOp::Field
    int64_t index;          ///< For PathOp::Index
    int64_t slice_start;    ///< For PathOp::Slice
    int64_t slice_end;      ///< For PathOp::Slice
    int64_t slice_step;     ///< For PathOp::Slice
    FilterPredicate filter; ///< For PathOp::Filter

    PathStep() : op(PathOp::End), index(0), slice_start(0), slice_end(0), slice_step(1) {}
    explicit PathStep(PathOp o) : op(o), index(0), slice_start(0), slice_end(0), slice_step(1) {}
    PathStep(PathOp o, std::string f)
        : op(o), field(std::move(f)), index(0), slice_start(0), slice_end(0), slice_step(1) {}
    PathStep(PathOp o, int64_t idx)
        : op(o), index(idx), slice_start(0), slice_end(0), slice_step(1) {}
};

/// Compiled JSONPath expression (immutable after construction).
class CompiledPath {
  public:
    CompiledPath() = default;
    explicit CompiledPath(std::vector<PathStep> steps) : steps_(std::move(steps)) {}

    [[nodiscard]] const std::vector<PathStep>& steps() const noexcept { return steps_; }
    [[nodiscard]] bool empty() const noexcept { return steps_.empty(); }

  private:
    std::vector<PathStep> steps_;
};

/**
 * Compile a JSONPath expression string into a CompiledPath.
 *
 * @param expr  JSONPath expression (e.g. "$.users[*].name").
 * @return Result with CompiledPath on success, Status::ParseError on invalid syntax.
 */
[[nodiscard]] Result<CompiledPath> compile_jsonpath(std::string_view expr);

/**
 * Evaluate a compiled JSONPath on a document.
 *
 * Returns a vector of materialised JsonValue copies for each match.
 */
[[nodiscard]] std::vector<JsonValue> eval_jsonpath(const JsonDocument& doc,
                                                   const CompiledPath& path);

/**
 * Evaluate a compiled JSONPath from a cursor position.
 *
 * More flexible than the document overload — works with any subtree.
 */
[[nodiscard]] std::vector<JsonValue> eval_jsonpath(const JsonCursor& cursor,
                                                   const CompiledPath& path);

} // namespace strata

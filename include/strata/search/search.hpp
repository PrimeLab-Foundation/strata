#pragma once

#include "strata/json/json_core.hpp"
#include "strata/json/json_cursor.hpp"
#include "strata/json/json_document.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

// JSONPath operation types
enum class PathOp {
    Root,             // $ - document root
    Field,            // .field or ["field"] - object field access
    Wildcard,         // * - all elements/values
    Index,            // [n] - array index
    RecursiveDescent, // .. - recursive descent
    Slice,            // [start:end:step] - array slice
    Filter,           // [?(@.field > value)] - filter predicate
    End               // Sentinel
};

// Filter comparison operators
enum class FilterOp {
    Equal,        // ==
    NotEqual,     // !=
    GreaterThan,  // >
    GreaterEqual, // >=
    LessThan,     // <
    LessEqual,    // <=
    And,          // &&
    Or            // ||
};

// Filter predicate (simplified for Phase 2.2)
struct FilterPredicate {
    std::string field;        // Field name (e.g., "age" from @.age)
    FilterOp op;              // Comparison operator
    double numeric_value;     // For numeric comparisons
    std::string string_value; // For string comparisons
    bool is_numeric;          // True for numeric, false for string

    FilterPredicate() : op(FilterOp::Equal), numeric_value(0), is_numeric(true) {}
};

// Single path step in a JSONPath expression
struct PathStep {
    PathOp op;
    std::string field;      // For Field operation
    int64_t index;          // For Index operation
    int64_t slice_start;    // For Slice operation
    int64_t slice_end;      // For Slice operation
    int64_t slice_step;     // For Slice operation
    FilterPredicate filter; // For Filter operation

    PathStep() : op(PathOp::End), index(0), slice_start(0), slice_end(0), slice_step(1) {}
    explicit PathStep(PathOp o) : op(o), index(0), slice_start(0), slice_end(0), slice_step(1) {}
    PathStep(PathOp o, std::string f)
        : op(o), field(std::move(f)), index(0), slice_start(0), slice_end(0), slice_step(1) {}
    PathStep(PathOp o, int64_t idx)
        : op(o), index(idx), slice_start(0), slice_end(0), slice_step(1) {}
};

// Compiled JSONPath expression
class CompiledPath {
  public:
    CompiledPath() = default;
    explicit CompiledPath(std::vector<PathStep> steps) : steps_(std::move(steps)) {}

    const std::vector<PathStep>& steps() const { return steps_; }
    bool empty() const { return steps_.empty(); }

  private:
    std::vector<PathStep> steps_;
};

// Compile a JSONPath expression string into a search plan
// Supported syntax (Phase 1):
//   $ - root
//   $.field - field access
//   $["field"] - field access (bracket notation)
//   $.* - all values
//   $[*] - all elements
//   $[n] - array index
//   $.a.b.c - nested paths
//   $.a[*].b - mixed navigation
Result<CompiledPath> compile_search_path(std::string_view expr);

// Evaluate a compiled JSONPath on a document
// Returns a vector of JsonValue results (materializes matches)
std::vector<JsonValue> eval_search_path(const JsonDocument& doc, const CompiledPath& path);

// Evaluate on a cursor (more flexible)
std::vector<JsonValue> eval_search_path(const JsonCursor& cursor, const CompiledPath& path);

// Evaluate with early termination after finding 'limit' results
// Useful for queries like "$..price" with limit=1 for finding first match
std::vector<JsonValue> eval_search_path(const JsonDocument& doc, const CompiledPath& path, size_t limit);
std::vector<JsonValue> eval_search_path(const JsonCursor& cursor, const CompiledPath& path, size_t limit);

} // namespace strata

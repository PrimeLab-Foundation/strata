#pragma once

#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

namespace strata {

// ============================================================================
// SECTION: Pushdown Plan Structures (PR #2 Phase 1)
// ============================================================================

/**
 * Symbol: field name with precomputed hash for fast lookup
 */
struct Symbol {
    uint64_t hash;    // Hash of field name (std::hash for now)
    std::string name; // Original field name

    Symbol() : hash(0) {}
    Symbol(const std::string& n) : name(n) { hash = std::hash<std::string>{}(n); }
};

/**
 * Path operation types supported in pushdown evaluator
 */
enum class Op {
    Root,     // $ - document root
    Field,    // .field or ["field"] - object field access
    Wildcard, // * - iterate all elements (array) or values (object)
    Index,    // [n] - array index access (positive only in Phase 1)
    Slice,    // [start:end:step] - array slice (Phase 2)
    Filter,   // ?(...) - predicate filter
    End       // Sentinel
};

/**
 * Comparison operators for filter predicates (Phase 2 extended)
 */
enum class Cmp {
    EQ,     // ==
    NE,     // !=
    GT,     // >
    GE,     // >=
    LT,     // <
    LE,     // <=
    EXISTS, // @.field (key presence check)
    IN,     // @.field in [...]
    NOT,    // ! (negation)
    AND,    // && (logical and)
    OR      // || (logical or)
};

/**
 * Literal value types for filter comparisons (Phase 2: separated int/double)
 */
using Literal = std::variant<std::monostate, // No value (for EXISTS)
                             std::nullptr_t, // Null
                             bool,           // Boolean
                             int64_t,        // Integer
                             double,         // Floating-point
                             std::string     // String
                             >;

/**
 * Relative path for filter LHS (Phase 2: support @.a.b.c)
 */
struct RelPath {
    std::vector<Symbol> fields; // Path segments starting from @

    RelPath() = default;
    explicit RelPath(const std::string& single_field) {
        if (!single_field.empty()) {
            fields.emplace_back(single_field);
        }
    }
};

/**
 * Single atomic filter condition (Phase 2: extended)
 * Example: @.age > 30 or @.name in ["Ann", "Bob"]
 */
struct FilterAtom {
    RelPath lhs;                   // Field path relative to @ (e.g., @.age or @.a.b)
    Cmp op;                        // Comparison operator
    Literal rhs;                   // Right-hand side literal value (for simple comparisons)
    std::vector<Literal> rhs_list; // Right-hand side list (for IN operator)

    FilterAtom() : op(Cmp::EXISTS) {}
};

/**
 * Filter in Disjunctive Normal Form (OR of ANDs)
 * Phase 1: Support single AND clause (no OR combinations yet)
 * Example: (age > 30 AND country == "UA")
 */
struct FilterDNF {
    std::vector<std::vector<FilterAtom>> clauses; // OR of ANDs

    bool empty() const { return clauses.empty(); }
};

/**
 * Single step in JSONPath execution plan (Phase 2: add slice support)
 */
struct Step {
    Op op;

    // For Field operations
    Symbol field;

    // For Index operations
    int64_t index = 0;

    // For Slice operations (Phase 2)
    int64_t slice_start = 0;
    int64_t slice_end = INT64_MAX; // Default: to end
    int64_t slice_step = 1;

    // For Filter operations
    FilterDNF dnf;

    Step(Op o = Op::Root) : op(o), index(0), slice_start(0), slice_end(INT64_MAX), slice_step(1) {}
};

/**
 * Complete pushdown execution plan
 * Built during compilation; marks whether expression can use pushdown
 */
struct PushdownPlan {
    std::vector<Step> steps;
    bool supports_pushdown = false;

    PushdownPlan() : supports_pushdown(false) {}
};

/**
 * Compiled JSONPath expression
 * Wraps jsoncons implementation + stores pushdown plan for optimization
 */
class CompiledExpr {
  public:
    explicit CompiledExpr(const std::string& expr);

    /**
     * Get the original expression string
     */
    const std::string& expression() const { return expression_; }

    /**
     * Get field names referenced in the expression (for future pushdown optimization)
     */
    const std::unordered_set<std::string>& referenced_fields() const { return referenced_fields_; }

    /**
     * Get pushdown execution plan (PR #2)
     */
    const PushdownPlan& plan() const { return plan_; }

    /**
     * Evaluate expression on jsoncons document (fallback path)
     * Returns array of matching values
     */
    std::vector<jsoncons::json> evaluate(const jsoncons::json& doc) const;

    /**
     * Check if expression is valid (compilation succeeded)
     */
    bool is_valid() const { return valid_; }

    /**
     * Get compilation error message (if invalid)
     */
    const std::string& error_message() const { return error_message_; }

  private:
    void extract_field_names(const std::string& expr);
    void build_pushdown_plan(const std::string& expr);

    std::string expression_;
    std::optional<jsoncons::jsonpath::jsonpath_expression<jsoncons::json>> compiled_expr_;
    std::unordered_set<std::string> referenced_fields_;
    PushdownPlan plan_; // PR #2: Pushdown execution plan
    bool valid_;
    std::string error_message_;
};

} // namespace strata

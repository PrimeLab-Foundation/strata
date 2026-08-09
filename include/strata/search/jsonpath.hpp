#pragma once

/**
 * @file jsonpath.hpp
 * @brief JSONPath compilation and evaluation over the C++ DOM.
 *
 * A compiled path is an immutable list of steps. Compilation never throws:
 * every malformed expression comes back as `Status::ParseError`, which the
 * binding layer reports as `ValueError("Invalid JSONPath expression")`. The
 * previous implementation threw `std::runtime_error` for an unclosed quote,
 * which leaked to Python as `RuntimeError` (docs/jsonpath/SKILL.md).
 *
 * The supported grammar is a subset of RFC 9535, listed in
 * docs/context/api.md § JSONPath.
 */

#include "strata/json/json_core.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

enum class PathOp {
    Root,             ///< `$`
    Field,            ///< `.name` or `["name"]`
    Wildcard,         ///< `.*` or `[*]`
    Index,            ///< `[n]`, negative counts from the end
    RecursiveDescent, ///< `..name`
    Slice,            ///< `[start:end:step]`
    Filter,           ///< `[?(@.name op value)]`
};

enum class FilterOp { Equal, NotEqual, Less, LessEqual, Greater, GreaterEqual };

/// One step of a compiled path. Which fields matter depends on `op`.
struct PathStep {
    PathOp op = PathOp::Root;

    std::string name;  ///< Field, RecursiveDescent, and the Filter's left side
    int64_t index = 0; ///< Index

    // Slice. An omitted component keeps its `has_` flag false.
    bool has_start = false;
    bool has_end = false;
    int64_t start = 0;
    int64_t end = 0;
    int64_t step = 1;

    // Filter right-hand side: a number or a quoted string, never both.
    FilterOp filter_op = FilterOp::Equal;
    bool filter_is_number = false;
    double filter_number = 0.0;
    std::string filter_string;
};

using CompiledPath = std::vector<PathStep>;

/// Compile @p expression, or Status::ParseError for anything malformed.
[[nodiscard]] Result<CompiledPath> compile_jsonpath(std::string_view expression);

/**
 * Whether a path can be evaluated while parsing, rather than over a finished
 * tree.
 *
 * False for Slice and Filter, which need a container in hand, and false for
 * RecursiveDescent: a streaming match would have to keep searching inside a
 * subtree it has already captured, and the previous implementation's streaming
 * path did not, so `search()` and `query()` disagreed on `$..a` when an `a`
 * contained another `a`. Excluding it keeps the two identical by construction.
 */
[[nodiscard]] bool is_streamable(const CompiledPath& path) noexcept;

/// Evaluate @p path against @p root, returning pointers into that tree.
[[nodiscard]] std::vector<const JsonValue*> eval_jsonpath(const CompiledPath& path,
                                                          const JsonValue& root);

} // namespace strata

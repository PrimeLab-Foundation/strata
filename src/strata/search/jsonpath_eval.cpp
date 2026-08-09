/**
 * @file jsonpath_eval.cpp
 * @brief Evaluating a compiled path over the C++ DOM.
 *
 * Results are pointers into the tree, so the caller keeps it alive. This
 * evaluator backs `CompiledPath.execute(cursor)`; `query()` has its own
 * evaluator that walks Python objects directly, and the two must agree.
 */

#include "strata/search/jsonpath.hpp"

#include <cstddef>
#include <string>

namespace strata {

namespace {

using Matches = std::vector<const JsonValue*>;

/// Resolve a possibly-negative index against @p size; false if out of range.
[[nodiscard]] bool resolve_index(int64_t index, size_t size, size_t& out) noexcept {
    if (index < 0)
        index += static_cast<int64_t>(size);
    if (index < 0 || index >= static_cast<int64_t>(size))
        return false;
    out = static_cast<size_t>(index);
    return true;
}

/// Clamp a slice bound into [0, size].
[[nodiscard]] int64_t clamp_bound(int64_t value, size_t size) noexcept {
    if (value < 0)
        value += static_cast<int64_t>(size);
    if (value < 0)
        return 0;
    if (value > static_cast<int64_t>(size))
        return static_cast<int64_t>(size);
    return value;
}

/// Depth-first collection of every `name` at or below @p value, outermost first.
void collect_recursive(const JsonValue& value, const std::string& name, Matches& out) {
    if (value.is_object()) {
        for (const auto& [key, member] : value.as_object()) {
            if (key == name)
                out.push_back(&member);
            // Keep descending into the match as well: an "a" inside an "a" is
            // itself a result. This is the behaviour `search()` must match.
            collect_recursive(member, name, out);
        }
        return;
    }
    if (value.is_array()) {
        for (const JsonValue& element : value.as_array())
            collect_recursive(element, name, out);
    }
}

[[nodiscard]] bool compare(double left, FilterOp op, double right) noexcept {
    switch (op) {
    case FilterOp::Equal:
        return left == right;
    case FilterOp::NotEqual:
        return left != right;
    case FilterOp::Less:
        return left < right;
    case FilterOp::LessEqual:
        return left <= right;
    case FilterOp::Greater:
        return left > right;
    case FilterOp::GreaterEqual:
        return left >= right;
    }
    return false;
}

/// Whether @p candidate satisfies the filter in @p step.
[[nodiscard]] bool passes_filter(const JsonValue& candidate, const PathStep& step) {
    if (!candidate.is_object())
        return false;
    const auto found = candidate.as_object().find(step.name);
    if (found == candidate.as_object().end())
        return false;

    const JsonValue& field = found->second;
    if (step.filter_is_number) {
        if (!field.is_number())
            return false;
        return compare(field.as_number(), step.filter_op, step.filter_number);
    }
    if (!field.is_string())
        return false;
    const bool equal = field.as_string() == step.filter_string;
    return step.filter_op == FilterOp::Equal ? equal : !equal;
}

void apply(const PathStep& step, const Matches& input, Matches& output) {
    for (const JsonValue* value : input) {
        switch (step.op) {
        case PathOp::Root:
            output.push_back(value);
            break;

        case PathOp::Field:
            if (value->is_object()) {
                const auto found = value->as_object().find(step.name);
                if (found != value->as_object().end())
                    output.push_back(&found->second);
            }
            break;

        case PathOp::Wildcard:
            if (value->is_object()) {
                for (const auto& [key, member] : value->as_object()) {
                    (void)key;
                    output.push_back(&member);
                }
            } else if (value->is_array()) {
                for (const JsonValue& element : value->as_array())
                    output.push_back(&element);
            }
            break;

        case PathOp::Index:
            if (value->is_array()) {
                size_t resolved = 0;
                if (resolve_index(step.index, value->as_array().size(), resolved))
                    output.push_back(&value->as_array()[resolved]);
            }
            break;

        case PathOp::RecursiveDescent:
            collect_recursive(*value, step.name, output);
            break;

        case PathOp::Slice: {
            if (!value->is_array())
                break;
            // A negative step parses but selects nothing: the loop only runs
            // forwards (docs/jsonpath/SKILL.md).
            if (step.step <= 0)
                break;
            const auto& array = value->as_array();
            const size_t size = array.size();
            const int64_t start = step.has_start ? clamp_bound(step.start, size) : 0;
            const int64_t end =
                step.has_end ? clamp_bound(step.end, size) : static_cast<int64_t>(size);
            for (int64_t index = start; index < end; index += step.step)
                output.push_back(&array[static_cast<size_t>(index)]);
            break;
        }

        case PathOp::Filter:
            // Filters select from arrays only.
            if (value->is_array()) {
                for (const JsonValue& element : value->as_array()) {
                    if (passes_filter(element, step))
                        output.push_back(&element);
                }
            }
            break;
        }
    }
}

} // namespace

std::vector<const JsonValue*> eval_jsonpath(const CompiledPath& path, const JsonValue& root) {
    Matches current{&root};
    Matches next;

    for (size_t index = 0; index < path.size(); ++index) {
        if (path[index].op == PathOp::Root)
            continue; // the root is already where we start
        next.clear();
        apply(path[index], current, next);
        current.swap(next);
        if (current.empty())
            break;
    }
    return current;
}

} // namespace strata

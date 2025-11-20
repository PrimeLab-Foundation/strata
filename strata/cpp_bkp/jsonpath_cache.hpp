#pragma once

#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>
#include <memory>
#include <string>

namespace strata {

/**
 * Compiled JSONPath expression for reuse
 * Stores the parsed expression to avoid reparsing on every query
 */
class CompiledPath {
  public:
    explicit CompiledPath(const std::string& expr)
        : expression_(expr),
          compiled_expr_(jsoncons::jsonpath::make_expression<jsoncons::json>(expr)) {}

    /**
     * Evaluate the compiled expression on a JSON document
     * Returns all matching values
     */
    std::vector<jsoncons::json> evaluate(const jsoncons::json& doc) const {
        std::vector<jsoncons::json> results;
        auto result = compiled_expr_.evaluate(doc);

        // Handle result (can be single value or array)
        if (result.is_array()) {
            for (const auto& item : result.array_range()) {
                results.push_back(item);
            }
        } else if (!result.is_null()) {
            results.push_back(result);
        }
        return results;
    }

    const std::string& get_expression() const { return expression_; }

  private:
    std::string expression_;
    jsoncons::jsonpath::jsonpath_expression<jsoncons::json> compiled_expr_;
};

} // namespace strata

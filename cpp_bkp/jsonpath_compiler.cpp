#include "tjp/jsonpath_compiler.hpp"

#include "jp_compile.hpp"

namespace tjp {

Result<CompiledPath> compile_jsonpath(const std::string& expr) {
    try {
        // Use full JSONPath compiler
        strata::CompiledExpr compiled(expr);

        if (!compiled.is_valid()) {
            Error e;
            e.code = ErrorCode::JsonPathError;
            e.message = compiled.error_message();
            return e;
        }

        CompiledPath cp;
        return cp;
    } catch (const std::exception& ex) {
        Error e;
        e.code = ErrorCode::JsonPathError;
        e.message = std::string("Failed to compile JSONPath: ") + ex.what();
        return e;
    }
}

} // namespace tjp

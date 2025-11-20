#include "tjp/jsonpath_executor.hpp"

namespace tjp {

Result<std::vector<View>> execute_view(const CompiledPath& cp, const char* data, size_t len,
                                       const ExecuteOptions& opts) {
    std::vector<View> out;
    if (opts.limit == 0)
        return out;
    // For MVP: empty AST == root, return whole buffer
    out.push_back({0, len});
    if (out.size() > opts.limit)
        out.resize(opts.limit);
    return out;
}

} // namespace tjp

#include "tjp/json_parser.hpp"

#include "tjp/scanner.hpp"

#include <cstring>

namespace tjp {

Result<Parsed> parse(const char* data, size_t len, const ParseOptions& opts) {
    Parsed p;
    auto tape_res = scan_structurals(data, len, {});
    if (!tape_res)
        return tape_res.error();
    p.tape = std::move(tape_res.value());
    if (opts.materialize) {
        // Minimal materialization: treat whole buffer as a JSON text node placeholder
        p.dom.root.v = std::string(data, len);
    }
    return p;
}

} // namespace tjp

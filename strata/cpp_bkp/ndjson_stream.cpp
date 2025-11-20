#include "tjp/ndjson_stream.hpp"

namespace tjp {

std::vector<LineView> split_lines(const char* data, size_t len, size_t limit) {
    std::vector<LineView> out;
    size_t start = 0;
    for (size_t i = 0; i < len; ++i) {
        if (data[i] == '\n') {
            if (limit && out.size() >= limit)
                break;
            size_t l = (i > start && data[i - 1] == '\r') ? (i - 1 - start) : (i - start);
            out.push_back({start, l});
            start = i + 1;
        }
    }
    if (start < len) {
        if (!limit || out.size() < limit)
            out.push_back({start, len - start});
    }
    return out;
}

} // namespace tjp

#include "partitioner.hpp"

#include <algorithm>

namespace strata {

std::vector<size_t> index_ndjson_rows(std::string_view s) {
    std::vector<size_t> starts;
    if (s.empty())
        return starts;

    starts.reserve(1024);
    starts.push_back(0);

    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\n') {
            if (i + 1 < s.size()) {
                starts.push_back(i + 1);
            }
        }
    }

    // Trim trailing empty line if the buffer ends with '\n'
    if (!starts.empty() && starts.back() == s.size()) {
        starts.pop_back();
    }

    return starts;
}

std::vector<Partition> pack_ndjson_partitions(std::string_view s,
                                              const std::vector<size_t>& row_starts,
                                              size_t target_bytes, size_t min_rows) {
    std::vector<Partition> out;
    if (row_starts.empty())
        return out;

    auto row_end_offset = [&](size_t row_idx) -> size_t {
        // End offset = next row start or end of buffer
        if (row_idx + 1 < row_starts.size())
            return row_starts[row_idx + 1];
        return s.size();
    };

    size_t begin_row = 0;
    size_t begin_off = row_starts[0];

    for (size_t r = 0; r < row_starts.size(); ++r) {
        const size_t cur_end_off = row_end_offset(r);
        const size_t cur_rows = (r + 1) - begin_row;
        const size_t cur_len = cur_end_off - begin_off;

        const bool hit_target = (cur_len >= target_bytes) && (cur_rows >= min_rows);
        const bool last_row = (r + 1 == row_starts.size());

        if (hit_target || last_row) {
            // Close partition [begin_row, r+1) exclusive
            out.push_back(Partition{.offset = begin_off,
                                    .length = cur_end_off - begin_off,
                                    .row_begin = begin_row,
                                    .row_end = r + 1});

            if (!last_row) {
                begin_row = r + 1;
                begin_off = row_starts[begin_row];
            }
        }
    }

    return out;
}

ExecutionPlan build_ndjson_plan(std::string_view s, size_t target_bytes, size_t min_rows) {
    ExecutionPlan plan{};

    // Enforce minimums consistent with Config::validate() from PR #3a
    const size_t tgt = std::max(target_bytes, static_cast<size_t>(1 << 16)); // 64 KiB min
    const size_t mnr = std::max(min_rows, static_cast<size_t>(1));

    auto idx = index_ndjson_rows(s);
    plan.parts = pack_ndjson_partitions(s, idx, tgt, mnr);

    return plan;
}

} // namespace strata

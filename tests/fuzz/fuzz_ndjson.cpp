/**
 * @file fuzz_ndjson.cpp
 * @brief libFuzzer target for NdjsonStream.
 *
 * Feeds arbitrary bytes to the line splitter and the per-line parser, which is
 * where the stream's borrowed-buffer contract and its CRLF/blank-line handling
 * are easiest to get wrong.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/ndjson_stream.hpp"

#include <cstddef>
#include <cstdint>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    const std::string_view input(reinterpret_cast<const char*>(data), size);

    // Line-at-a-time, exercising the end-of-stream convention.
    strata::NdjsonStream stream(input);
    for (;;) {
        const auto record = stream.next();
        if (record.status == strata::Status::KeyNotFound)
            break;
        if (record.ok() && record.value.is_object())
            (void)record.value.as_object().size();
    }

    // ...and the batch path, both ways round on skip_errors.
    strata::NdjsonStream eager(input);
    const auto all = eager.parse_all(/*skip_errors=*/true);
    if (all.ok())
        (void)all.value.size();

    strata::NdjsonStream strict(input);
    size_t failed_line = 0;
    (void)strict.parse_all(/*skip_errors=*/false, &failed_line);

    return 0;
}

/**
 * libFuzzer target for strata::parse_json (loads).
 * Build with -fsanitize=fuzzer,address,undefined; run with corpus and -max_total_time=60.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"

#include <cstdint>
#include <cstdlib>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size == 0)
        return 0;
    std::string_view input(reinterpret_cast<const char*>(data), size);
    auto result = strata::parse_json(input);
    (void)result;
    return 0;
}

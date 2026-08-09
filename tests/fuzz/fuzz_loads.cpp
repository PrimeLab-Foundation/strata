/**
 * @file fuzz_loads.cpp
 * @brief libFuzzer target for parse_json().
 *
 * Feeds arbitrary bytes to the parser and requires that it neither crashes nor
 * trips a sanitizer. Build with -DFUZZ=ON; a seed corpus and the scheduled runs
 * arrive with the hardening milestone.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"

#include <cstddef>
#include <cstdint>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    const std::string_view input(reinterpret_cast<const char*>(data), size);
    const auto result = strata::parse_json(input);

    // Touch the parsed value so the DOM build is not optimised away.
    if (result.ok() && result.value.is_object())
        (void)result.value.as_object().size();

    return 0;
}

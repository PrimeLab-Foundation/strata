/**
 * @file head_shape_probe.cpp
 * @brief The parser head's static shape, on whichever include tree -I names.
 *
 * `parse_number` is deliberately not inlined, so it comes out of `-S` as its
 * own symbol: counting the instructions between its label and its
 * `.cfi_endproc`, once per include tree, says what the digit-chain change did
 * to the head's code before any clock is involved — and whether the compiler
 * still emits `leading_digit_value` out of line, which is the difference a
 * shorter chain makes that an instruction count alone would miss.
 *
 * Never linked into production; `experiments/` is off the build.
 *
 * usage: clang++ -O3 -std=c++20 -DNDEBUG -I <tree> -S -o head.s head_shape_probe.cpp
 */

#include "strata/json/json_parser_inline.hpp"

#include <cstdint>
#include <string_view>

namespace {
struct CountingHandler {
    uint64_t ints = 0;
    uint64_t doubles = 0;
    bool on_null() { return true; }
    bool on_bool(bool) { return true; }
    bool on_int(int64_t value) {
        ints += static_cast<uint64_t>(value);
        return true;
    }
    bool on_big_int(std::string_view) { return true; }
    bool on_double(double value) {
        doubles += static_cast<uint64_t>(value);
        return true;
    }
    bool on_string(std::string_view) { return true; }
    bool on_key(std::string_view) { return true; }
    bool on_start_object() { return true; }
    bool on_end_object() { return true; }
    bool on_start_array() { return true; }
    bool on_end_array() { return true; }
};
} // namespace

extern "C" int probe_parse(const char* text, size_t len) {
    CountingHandler handler;
    const auto status = strata::parse_sax_inline(std::string_view(text, len), handler, false);
    return status == strata::Status::Ok ? static_cast<int>(handler.ints + handler.doubles) : -1;
}

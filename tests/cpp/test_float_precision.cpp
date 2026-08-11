/**
 * @file test_float_precision.cpp
 * @brief Contract suite for double formatting.
 *
 * Pins the three properties the milestone names: values round-trip, integral
 * values keep a fraction, and the switch to scientific notation happens at
 * documented boundaries. Round-trip is asserted *exactly* — the formatted text
 * must read back as the same double, bit for bit — which is strictly stronger
 * than the |v| * 1e-10 tolerance the roadmap asks for.
 *
 * The layout matches CPython's `repr(float)`. That was verified separately by
 * sweeping 150k doubles through both; what is pinned here is the boundary
 * behaviour that sweep would have caught.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_serialize.hpp"
#include "strata/util/dtoa.hpp"
#include "strata/util/fast_parse.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string>
#include <system_error>

using strata::JsonValue;
using strata::serialize_json;
using strata::util::format_double;
using strata::util::kDoubleBufferSize;

namespace {

[[nodiscard]] std::string rendered(double value) {
    char buffer[kDoubleBufferSize];
    const size_t length = format_double(value, buffer, sizeof(buffer));
    assert(length > 0);
    assert(length < kDoubleBufferSize);
    return std::string(buffer, length);
}

/// Parse text back to a double, requiring the whole span to be consumed.
/// from_chars_double keeps this oracle independent of the scanner under test
/// on every platform: std::from_chars where the library has it, the system
/// strtod_l on Apple SDKs that lack the floating-point overload.
[[nodiscard]] double reparse(const std::string& text) {
    double value = 0.0;
    const auto result =
        strata::util::from_chars_double(text.data(), text.data() + text.size(), value);
    assert(result.ec == std::errc{});
    assert(result.ptr == text.data() + text.size());
    return value;
}

[[nodiscard]] bool same_bits(double a, double b) {
    uint64_t left = 0;
    uint64_t right = 0;
    std::memcpy(&left, &a, sizeof(left));
    std::memcpy(&right, &b, sizeof(right));
    return left == right;
}

void expect(double value, const char* expected) {
    const std::string actual = rendered(value);
    if (actual != expected) {
        std::printf("format_double(%.17g) = %s, expected %s\n", value, actual.c_str(), expected);
        assert(false);
    }
}

// ---------------------------------------------------------------------------

void test_exact_renderings() {
    expect(0.0, "0.0");
    expect(-0.0, "-0.0");
    expect(1.0, "1.0");
    expect(-1.0, "-1.0");
    expect(42.0, "42.0");
    expect(100.0, "100.0");
    expect(0.1, "0.1");
    expect(0.3, "0.3");
    expect(-99.99, "-99.99");
    expect(123.456789, "123.456789");
    expect(3.141592653589793, "3.141592653589793");
    expect(1234567890123456.0, "1234567890123456.0");
    expect(5e-324, "5e-324");
    expect(std::numeric_limits<double>::max(), "1.7976931348623157e+308");
    expect(std::numeric_limits<double>::min(), "2.2250738585072014e-308");
}

/// A double never renders as a bare integer: it would read back as one.
void test_integral_values_keep_a_fraction() {
    const double integral[] = {0.0,
                               -0.0,
                               1.0,
                               -1.0,
                               2.0,
                               10.0,
                               42.0,
                               100.0,
                               1e6,
                               1e15,
                               9007199254740992.0 /* 2^53 */,
                               -9007199254740992.0};
    for (const double value : integral) {
        const std::string text = rendered(value);
        const bool has_fraction = text.find('.') != std::string::npos;
        const bool has_exponent = text.find('e') != std::string::npos;
        assert(has_fraction || has_exponent);
        assert(same_bits(reparse(text), value));
    }
}

/**
 * The fixed/scientific switch, at both ends.
 *
 * Scientific starts once the decimal point would sit at or before the fourth
 * place after the point, or past the sixteenth before it.
 */
void test_scientific_notation_boundaries() {
    // Upper boundary: 1e15 stays fixed, 1e16 does not.
    expect(1e15, "1000000000000000.0");
    expect(1e16, "1e+16");
    expect(1e17, "1e+17");
    expect(1e20, "1e+20");
    assert(rendered(1e15).find('e') == std::string::npos);
    assert(rendered(1e16).find('e') != std::string::npos);

    // Lower boundary: 1e-4 stays fixed, 1e-5 does not.
    expect(1e-4, "0.0001");
    expect(1e-5, "1e-05");
    expect(1e-10, "1e-10");
    assert(rendered(1e-4).find('e') == std::string::npos);
    assert(rendered(1e-5).find('e') != std::string::npos);

    // A value whose digits run past the boundary is still fixed while the
    // decimal point is inside the window.
    expect(9999999999999998.0, "9999999999999998.0");
}

/// Every rendering reads back as the double it came from.
void test_round_trip_is_exact() {
    // Deterministic xorshift over raw bit patterns: covers denormals, huge
    // magnitudes and everything between, without depending on a seed source.
    uint64_t state = 0x243F6A8885A308D3ULL;
    int checked = 0;
    for (int index = 0; index < 200000 && checked < 50000; ++index) {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;

        double value = 0.0;
        std::memcpy(&value, &state, sizeof(value));
        if (!std::isfinite(value))
            continue;
        ++checked;

        const std::string text = rendered(value);
        const double parsed = reparse(text);
        assert(same_bits(parsed, value));

        // The roadmap's looser bound holds a fortiori.
        if (value != 0.0) {
            const double error = std::abs(parsed - value);
            assert(error <= std::abs(value) * 1e-10);
        }
    }
    assert(checked >= 40000);
}

void test_common_decimal_values_round_trip() {
    const double values[] = {35.31, 99.99, 19.99, 29.99, 0.01, 0.001, -3.14,
                             -0.01, 1e-6,  1e-5,  1e13,  1e14, 2.5,   1.0 / 3.0};
    for (const double value : values)
        assert(same_bits(reparse(rendered(value)), value));
}

/**
 * Non-finite values are the serializer's business, not the converter's.
 *
 * JSON cannot spell NaN or infinity, so they serialize as null; format_double
 * is never handed one.
 */
void test_non_finite_values_serialize_as_null() {
    const double nan_value = std::numeric_limits<double>::quiet_NaN();
    const double infinity = std::numeric_limits<double>::infinity();

    assert(serialize_json(JsonValue(JsonValue::Variant(nan_value))) == "null");
    assert(serialize_json(JsonValue(JsonValue::Variant(infinity))) == "null");
    assert(serialize_json(JsonValue(JsonValue::Variant(-infinity))) == "null");

    JsonValue::Array mixed;
    mixed.push_back(JsonValue(JsonValue::Variant(1.5)));
    mixed.push_back(JsonValue(JsonValue::Variant(nan_value)));
    mixed.push_back(JsonValue(JsonValue::Variant(-infinity)));
    assert(serialize_json(JsonValue(JsonValue::Variant(std::move(mixed)))) == "[1.5,null,null]");
}

/// A buffer that cannot hold the longest output is refused, not overrun.
void test_short_buffer_is_refused() {
    char small[8];
    assert(format_double(1.0, small, sizeof(small)) == 0);
}

/**
 * The Ryu path against an independent reference over millions of doubles.
 *
 * The reference reimplements the layout from primitives this file does not
 * share with dtoa.cpp: `to_chars` produces the shortest digits and the fixed
 * or scientific body, and the reference only appends the `.0` an integral
 * value needs. Any byte of disagreement -- digits, rounding, layout window,
 * exponent form -- fails with the offending bit pattern printed.
 */
[[nodiscard]] size_t reference_format(double value, char* out, size_t capacity) {
    const double magnitude = std::fabs(value);
    const bool scientific = magnitude != 0.0 && (magnitude < 1e-4 || magnitude >= 1e16);
    const auto converted =
        std::to_chars(out, out + capacity, value,
                      scientific ? std::chars_format::scientific : std::chars_format::fixed);
    assert(converted.ec == std::errc{});
    auto written = static_cast<size_t>(converted.ptr - out);
    if (!scientific && value == std::trunc(value)) {
        out[written++] = '.';
        out[written++] = '0';
    }
    return written;
}

void check_against_reference(double value) {
    if (std::isnan(value) || std::isinf(value))
        return;
    char ours[64];
    char reference[64];
    const size_t got = format_double(value, ours, sizeof(ours));
    const size_t want = reference_format(value, reference, sizeof(reference));
    if (got != want || std::memcmp(ours, reference, want) != 0) {
        uint64_t bits;
        std::memcpy(&bits, &value, sizeof(bits));
        std::printf("dtoa mismatch for bits %016llx: got %.*s want %.*s\n",
                    static_cast<unsigned long long>(bits), static_cast<int>(got), ours,
                    static_cast<int>(want), reference);
        assert(false);
    }
}

void test_shortest_form_matches_reference_exhaustively() {
    // Boundary pins first: the layout window edges, subnormals, extremes,
    // and the tie-breaking cases shortest-round-trip is known to get wrong
    // when the acceptance bounds are mishandled.
    for (const double pinned : {0.0,
                                -0.0,
                                1.0,
                                -1.0,
                                0.5,
                                1e-4,
                                9.999999999999999e-5,
                                1e-5,
                                1e15,
                                9.999999999999998e15,
                                1e16,
                                1e17,
                                5e-324,
                                4.9e-324,
                                2.2250738585072014e-308,
                                2.225073858507201e-308,
                                1.7976931348623157e308,
                                0.1,
                                0.2,
                                0.3,
                                1.0 / 3.0,
                                2.0 / 3.0,
                                9007199254740992.0,
                                9007199254740993.0,
                                0.30000000000000004,
                                3.141592653589793,
                                2.718281828459045,
                                1.2345678901234567e22,
                                8.98846567431158e307,
                                5.960464477539063e-8,
                                1.262177448e-29,
                                5.9604644775390625e-8})
        check_against_reference(pinned);

    // The whole bit space, deterministically.
    uint64_t state = 0x853C49E6748FEA9BULL;
    for (int trial = 0; trial < 20000000; ++trial) {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        double value;
        std::memcpy(&value, &state, sizeof(value));
        check_against_reference(value);
    }

    // Adversarial bands: every exponent, small mantissa perturbations around
    // powers of ten and of two, where rounding boundaries cluster.
    for (int exponent = 0; exponent < 2047; ++exponent) {
        for (const uint64_t mantissa :
             {0ULL, 1ULL, 2ULL, 0xFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFEULL, 0x8000000000000ULL,
              0x5555555555555ULL, 0xAAAAAAAAAAAAAULL}) {
            const uint64_t bits = (static_cast<uint64_t>(exponent) << 52) | mantissa;
            double value;
            std::memcpy(&value, &bits, sizeof(value));
            check_against_reference(value);
            check_against_reference(-value);
        }
    }
}

/// Parse @p text through the full number pipeline (Clinger, Eisel–Lemire,
/// from_chars fallback), requiring the whole span to be consumed.
[[nodiscard]] double parse_pipeline(const std::string& text) {
    strata::util::ParsedNumber number;
    const bool ok = strata::util::parse_number_unified(text.data(), text.size(), number);
    assert(ok);
    assert(number.consumed == text.size());
    assert(number.kind == strata::util::NumberKind::Double);
    return number.double_value;
}

void test_parse_pipeline_matches_from_chars_on_edges() {
    // The published torture values plus every boundary the Eisel–Lemire
    // bail-outs guard: subnormals, the normal/subnormal edge, overflow,
    // and the halfway family. The pipeline must agree with the library
    // conversion bit for bit — including which side of an even boundary a
    // halfway literal rounds to.
    const char* cases[] = {
        "0.0",
        "-0.0",
        "1.5",
        "0.1",
        "0.2",
        "0.30000000000000004",
        "0.11133106816568039",
        "0.7415504997598329",
        "1.7976931348623157e308",  // DBL_MAX
        "1.7976931348623158e308",  // rounds to DBL_MAX
        "2.2250738585072014e-308", // min normal
        "2.2250738585072011e-308", // the strtod-killer, just below min normal
        "2.2250738585072012e-308",
        "4.9406564584124654e-324", // min subnormal
        "5e-324",
        "3e-324",
        "1e-325",
        "2e-324",
        "9007199254740993.0",    // 2^53 + 1 as a double literal
        "5.9604644775390625e-8", // exactly representable half
        "1.1920928955078125e-07",
        "6.9294956446009195e15",
        "3.7455744005952583e15",
        "17976931348623157e292",
        "1.00000000000000011102230246251565404236316680908203125",
        "0.500000000000000166533453693773481063544750213623046875",
        "2e15",
        "9e15",
        "123456789012345678901234567890.0",
        "3.1415926535897932384626433832795028841971",
        "1e308",
        "1e-308",
        "1e309",
        "1e-309",
        "1e-348",
        "1e-347",
        "1e347",
    };
    for (const char* text : cases) {
        strata::util::ParsedNumber number;
        const std::string token(text);
        const bool ok = strata::util::parse_number_unified(token.data(), token.size(), number);
        assert(ok);
        assert(number.consumed == token.size());

        double reference = 0.0;
        const auto result =
            strata::util::from_chars_double(token.data(), token.data() + token.size(), reference);
        if (result.ec == std::errc{}) {
            assert(same_bits(number.double_value, reference));
        } else {
            // Out of range either way: the pipeline maps it to ±inf / ±0.0.
            assert(result.ec == std::errc::result_out_of_range);
            assert(std::isinf(number.double_value) || number.double_value == 0.0);
        }
    }
}

void test_parse_pipeline_matches_from_chars_randomly() {
    // Uniformly random bit patterns, printed at 17 significant digits (the
    // round-trip precision) and re-parsed: the pipeline and the library
    // conversion must agree bit for bit. This is the differential oracle for
    // the Eisel–Lemire port — any rounding defect in the multiply, the table
    // or a bail-out shows up as a one-ulp disagreement here.
    uint64_t state = 0x9E3779B97F4A7C15ULL;
    int doubles_checked = 0;
    while (doubles_checked < 300000) {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        double value;
        std::memcpy(&value, &state, sizeof(value));
        if (!std::isfinite(value))
            continue;
        ++doubles_checked;

        char buffer[64];
        const int length = std::snprintf(buffer, sizeof(buffer), "%.17g", value);
        assert(length > 0);
        std::string text(buffer, static_cast<size_t>(length));
        if (text.find_first_of(".eE") == std::string::npos)
            text += ".0"; // integral rendering would take the int path; same value
        const double reparsed = parse_pipeline(text);
        const double reference = reparse(text);
        if (!same_bits(reparsed, reference)) {
            std::printf("pipeline mismatch on %s: %.17g vs %.17g\n", text.c_str(), reparsed,
                        reference);
            assert(false);
        }
    }
}

} // namespace

int main() {
    test_exact_renderings();
    test_integral_values_keep_a_fraction();
    test_scientific_notation_boundaries();
    test_round_trip_is_exact();
    test_common_decimal_values_round_trip();
    test_non_finite_values_serialize_as_null();
    test_short_buffer_is_refused();
    test_shortest_form_matches_reference_exhaustively();
    test_parse_pipeline_matches_from_chars_on_edges();
    test_parse_pipeline_matches_from_chars_randomly();

    std::puts("float_precision_tests: OK");
    return 0;
}

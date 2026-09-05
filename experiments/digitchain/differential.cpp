/**
 * Old-versus-new differential for the parser's digit chain.
 *
 * Compiled twice — once with -I pointing at the baseline include tree, once
 * at the variant's — it dumps one canonical line per input: whether the parse
 * was accepted, how many bytes it consumed, and the exact event stream, with
 * doubles printed as their 64 bits. Two dumps that are byte-identical are the
 * bit-identity proof; the FNV-1a digest at the end is the same statement in
 * one line.
 *
 * The cross product: integer widths 1..25, fraction widths 0..24, leading and
 * trailing zeros in the fraction, both signs, every exponent suffix and every
 * terminator the head must look past, in four syntactic contexts, at three
 * padding lengths (the head needs eight readable bytes from a run's start and
 * the long-fraction step twenty-four, so without padding the short widths
 * never enter them, and with padding alone the near-end hand-off never runs).
 *
 * Every input is copied into an exactly-sized heap buffer before it is
 * parsed, so a read of even one byte past the document is an ASan report
 * rather than a silent success.
 *
 * The value oracle is independent of strata: std::from_chars over the
 * consumed span where the library has the floating-point overload, strtod in
 * the C locale where it does not (Apple's libc++ through Xcode 16). It runs
 * on the bare-number context, where the consumed span is the whole document.
 *
 * usage: differential [dump-path]
 */

#include "strata/json/json_parser_inline.hpp"
#include "strata/util/fast_parse.hpp"

#include <cerrno>
#include <charconv>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

namespace {

/// Records the SAX stream as text, doubles by their bits.
struct RecordingHandler {
    std::string log;

    bool on_null() {
        log += "n;";
        return true;
    }
    bool on_bool(bool value) {
        log += value ? "b1;" : "b0;";
        return true;
    }
    bool on_int(int64_t value) {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "i%lld;", static_cast<long long>(value));
        log += buffer;
        return true;
    }
    bool on_big_int(std::string_view span) {
        log += "B";
        log.append(span.data(), span.size());
        log += ";";
        return true;
    }
    bool on_double(double value) {
        uint64_t bits;
        std::memcpy(&bits, &value, sizeof(bits));
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "d%016llx;", static_cast<unsigned long long>(bits));
        log += buffer;
        return true;
    }
    bool on_string(std::string_view span) {
        log += "s";
        log.append(span.data(), span.size());
        log += ";";
        return true;
    }
    bool on_key(std::string_view span) {
        log += "k";
        log.append(span.data(), span.size());
        log += ";";
        return true;
    }
    bool on_start_object() {
        log += "{;";
        return true;
    }
    bool on_end_object() {
        log += "};";
        return true;
    }
    bool on_start_array() {
        log += "[;";
        return true;
    }
    bool on_end_array() {
        log += "];";
        return true;
    }
};

/// Deterministic digits, so the sweep is reproducible without a seed source.
uint64_t next_random(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

std::string digits_of(uint64_t& state, size_t length, size_t leading_zeros, bool trailing_zero) {
    std::string out;
    out.reserve(length);
    for (size_t index = 0; index < length; ++index) {
        if (index < leading_zeros) {
            out.push_back('0');
        } else {
            out.push_back(static_cast<char>('0' + next_random(state) % 10));
        }
    }
    if (!out.empty() && out[0] == '0' && leading_zeros == 0)
        out[0] = '7';
    if (trailing_zero && !out.empty())
        out.back() = '0';
    return out;
}

size_t failures = 0;

/// FNV-1a over every dumped byte: the whole run in one number.
uint64_t digest = 1469598103934665603ULL;

void absorb(const std::string& text) {
    for (const char c : text)
        digest = (digest ^ static_cast<unsigned char>(c)) * 1099511628211ULL;
}

/// Parse one document out of an exactly-sized buffer and append its outcome.
std::string run_one(const std::string& document) {
    // Exact size: no NUL, no slack, so ASan sees any read past the end.
    std::vector<char> exact(document.size());
    if (!document.empty())
        std::memcpy(exact.data(), document.data(), document.size());
    RecordingHandler handler;
    const strata::Status status = strata::parse_sax_inline(
        std::string_view(exact.data(), exact.size()), handler, /*validate_utf8=*/false);
    std::string line = status == strata::Status::Ok ? "ok " : "no ";
    line += handler.log;
    return line;
}

/// The independent value oracle, on the bare-number context only.
void check_value(const std::string& number, const std::string& outcome) {
    if (outcome.rfind("ok ", 0) != 0 || outcome.size() < 4)
        return;
    const char kind = outcome[3];
    const char* const first = number.data();
    const char* const last = first + number.size();
    if (kind == 'i') {
        long long parsed = 0;
        const auto result = std::from_chars(first, last, parsed);
        if (result.ec != std::errc{} || result.ptr != last) {
            std::printf("oracle: int span rejected: %s\n", number.c_str());
            ++failures;
            return;
        }
        char buffer[48];
        std::snprintf(buffer, sizeof(buffer), "i%lld;", parsed);
        if (outcome.compare(3, std::string::npos, buffer) != 0) {
            std::printf("oracle: int mismatch on %s: %s vs %s\n", number.c_str(),
                        outcome.c_str() + 3, buffer);
            ++failures;
        }
        return;
    }
    if (kind != 'd')
        return; // big integers and rejections are not this oracle's business
    double want = 0.0;
#if STRATA_HAS_FP_FROM_CHARS
    const auto result = std::from_chars(first, last, want);
    if (result.ec != std::errc{} && result.ec != std::errc::result_out_of_range) {
        std::printf("oracle: double span rejected: %s\n", number.c_str());
        ++failures;
        return;
    }
    if (result.ec == std::errc::result_out_of_range)
        return; // the out-of-range direction is the scanner's own decision
#else
    errno = 0;
    char* end = nullptr;
    want = std::strtod(number.c_str(), &end);
    if (end != number.c_str() + number.size())
        return;
    if (errno == ERANGE)
        return;
#endif
    uint64_t bits;
    std::memcpy(&bits, &want, sizeof(bits));
    char buffer[48];
    std::snprintf(buffer, sizeof(buffer), "d%016llx;", static_cast<unsigned long long>(bits));
    if (outcome.compare(3, std::string::npos, buffer) != 0) {
        std::printf("oracle: double mismatch on %s: %s vs %s\n", number.c_str(),
                    outcome.c_str() + 3, buffer);
        ++failures;
    }
}

} // namespace

int main(int argc, char** argv) {
    const char* const dump_path = argc > 1 ? argv[1] : "differential_dump.txt";
    std::FILE* const dump = std::fopen(dump_path, "w");
    if (dump == nullptr) {
        std::printf("cannot open %s\n", dump_path);
        return 2;
    }

    uint64_t state = 0x1234567887654321ULL;
    const char* const tails[] = {",1", "]", "}", " ", "e5", "E-2", ".5", "x", ""};
    const char* const exponents[] = {"", "e5", "E-2", "e+12", "e-330", "e309"};
    size_t cases = 0;

    for (size_t int_len = 1; int_len <= 25; ++int_len) {
        const uint64_t width_digest_before = digest;
        for (size_t frac_len = 0; frac_len <= 24; ++frac_len) {
            for (int trial = 0; trial < 6; ++trial) {
                std::string text = digits_of(state, int_len, 0, false);
                if (trial % 6 == 1)
                    text = "0"; // the lone zero
                if (trial % 6 == 2 && int_len > 1)
                    text[0] = '0'; // a leading zero: must be rejected
                if (trial % 6 == 3 && int_len > 1)
                    text[int_len - 1] = '0';
                if (frac_len > 0) {
                    text += "." + digits_of(state, frac_len, static_cast<size_t>(trial % 4),
                                            trial % 5 == 0);
                }
                const char* const exponent = exponents[static_cast<size_t>(trial) %
                                                       (sizeof(exponents) / sizeof(*exponents))];
                text += exponent;
                for (const bool negative : {false, true}) {
                    const std::string number = (negative ? "-" : "") + text;
                    for (const char* tail : tails) {
                        for (const size_t padding : {size_t{0}, size_t{9}, size_t{24}}) {
                            const std::string trailer =
                                std::string(tail) + std::string(padding, 'x');
                            // Four contexts: bare, in an array, in an object,
                            // and behind an element the array loop already
                            // pushed (the position the wide_arrays rows hit).
                            const std::string documents[] = {
                                number + trailer,
                                "[" + number + trailer + "]",
                                "{\"k\":" + number + trailer + "}",
                                "[1," + number + trailer + ",2]",
                            };
                            for (size_t which = 0; which < 4; ++which) {
                                const std::string outcome = run_one(documents[which]);
                                std::string line = documents[which] + " => " + outcome + "\n";
                                std::fwrite(line.data(), 1, line.size(), dump);
                                absorb(line);
                                ++cases;
                                if (which == 0 && trailer.empty())
                                    check_value(number, outcome);
                            }
                        }
                    }
                }
            }
        }
        std::printf("int width %2zu: digest %016llx\n", int_len,
                    static_cast<unsigned long long>(digest ^ width_digest_before));
    }

    std::fclose(dump);
    std::printf("cases %zu, oracle failures %zu, digest %016llx\n", cases, failures,
                static_cast<unsigned long long>(digest));
    return failures == 0 ? 0 : 1;
}

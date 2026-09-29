/**
 * @file temporal.cpp
 * @brief Formatting and strict recognition of dates, times, offsets and UUIDs.
 *
 * Scalar and branchy on purpose: every caller is a cold path (the serializer's
 * native tail, the opt-in `parse_types` revival), so the code is written to be
 * checked against its grammar, not to be fast (docs/architecture/native_types.md).
 */

#include "strata/util/temporal.hpp"

#include <cstdint>
#include <cstring>
#include <string_view>

namespace strata::util {

namespace {

/// Days per month of a common year, January first.
constexpr uint8_t kDaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/// Scale of a fraction of n digits to microseconds, indexed by n (1–6).
constexpr int32_t kFractionScale[7] = {0, 100000, 10000, 1000, 100, 10, 1};

constexpr size_t kMaxFractionDigits = 6;
constexpr size_t kOffsetLength = 6; // `±HH:MM`
constexpr size_t kMinDateTimeLength = kDateLength + 1 + kTimeLength;

inline void write_two(uint32_t value, char* out) noexcept {
    out[0] = static_cast<char>('0' + value / 10 % 10);
    out[1] = static_cast<char>('0' + value % 10);
}

/**
 * The eight lowercase hex digits of @p value, most significant first, with no
 * per-digit loop: the nibbles are spread one per byte of a 64-bit word (byte
 * i holds nibble i), each byte becomes its ASCII digit by one add -- `'0'`,
 * plus `'a' - '0' - 10` where the nibble is 10 or more, a flag read from bit
 * 4 of nibble + 6 -- and the bytes are stored most significant first. No
 * byte can carry into its neighbour (at most 15 + 6 before the flag, 15 + 87
 * after), and the shifts fix the order independently of the host's
 * endianness. Equal to the table lookup per digit it replaced
 * (tests/cpp/test_temporal.cpp checks it against that reference).
 */
inline void write_hex8(uint32_t value, char* out) noexcept {
    uint64_t x = value;
    x = ((x & 0xFFFF0000ULL) << 16) | (x & 0x0000FFFFULL);
    x = ((x & 0x0000FF000000FF00ULL) << 8) | (x & 0x000000FF000000FFULL);
    x = ((x & 0x00F000F000F000F0ULL) << 4) | (x & 0x000F000F000F000FULL);
    const uint64_t letters = ((x + 0x0606060606060606ULL) >> 4) & 0x0101010101010101ULL;
    x += 0x3030303030303030ULL + letters * static_cast<uint64_t>('a' - '0' - 10);
    for (int index = 0; index < 8; ++index)
        out[index] = static_cast<char>(x >> (8 * (7 - index)));
}

[[nodiscard]] inline bool is_digit(char c) noexcept {
    return static_cast<unsigned>(static_cast<unsigned char>(c)) - '0' < 10U;
}

/// The value of an ASCII hexadecimal digit of either case, or -1.
[[nodiscard]] inline int hex_value(char c) noexcept {
    if (is_digit(c)) {
        return c - '0';
    }
    const char lower = static_cast<char>(c | 0x20);
    if (lower >= 'a' && lower <= 'f') {
        return lower - 'a' + 10;
    }
    return -1;
}

[[nodiscard]] inline bool read_two(const char* p, int32_t& out) noexcept {
    if (!is_digit(p[0]) || !is_digit(p[1])) {
        return false;
    }
    out = (p[0] - '0') * 10 + (p[1] - '0');
    return true;
}

[[nodiscard]] inline bool is_leap_year(int32_t year) noexcept {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

/// `YYYY-MM-DD` at @p p (kDateLength bytes readable), a real day of years 1–9999.
[[nodiscard]] bool scan_date(const char* p, TemporalFields& fields) noexcept {
    int32_t century = 0;
    int32_t year_in_century = 0;
    if (!read_two(p, century) || !read_two(p + 2, year_in_century) || p[4] != '-' ||
        !read_two(p + 5, fields.month) || p[7] != '-' || !read_two(p + 8, fields.day)) {
        return false;
    }
    fields.year = century * 100 + year_in_century;
    if (fields.year < 1 || fields.month < 1 || fields.month > 12 || fields.day < 1) {
        return false;
    }
    const int32_t last_day =
        fields.month == 2 && is_leap_year(fields.year) ? 29 : kDaysInMonth[fields.month - 1];
    return fields.day <= last_day;
}

/// `HH:MM:SS` [`.` 1–6 digits] [`Z` | `z` | `±HH:MM`], the whole of @p text.
[[nodiscard]] bool scan_time(std::string_view text, TemporalFields& fields) noexcept {
    const char* p = text.data();
    const size_t length = text.size();
    if (length < kTimeLength || !read_two(p, fields.hour) || p[2] != ':' ||
        !read_two(p + 3, fields.minute) || p[5] != ':' || !read_two(p + 6, fields.second) ||
        fields.hour > 23 || fields.minute > 59 || fields.second > 59) {
        return false;
    }
    size_t pos = kTimeLength;
    if (pos < length && p[pos] == '.') {
        ++pos;
        const size_t start = pos;
        int32_t fraction = 0;
        while (pos < length && is_digit(p[pos])) {
            if (pos - start == kMaxFractionDigits) {
                return false; // a seventh digit: never truncated silently
            }
            fraction = fraction * 10 + (p[pos] - '0');
            ++pos;
        }
        const size_t digits = pos - start;
        if (digits == 0) {
            return false;
        }
        fields.microsecond = fraction * kFractionScale[digits];
    }
    if (pos == length) {
        return true;
    }
    if (p[pos] == 'Z' || p[pos] == 'z') {
        fields.offset = TemporalOffset::Minutes;
        fields.offset_minutes = 0;
        return pos + 1 == length;
    }
    if ((p[pos] != '+' && p[pos] != '-') || length - pos != kOffsetLength) {
        return false;
    }
    int32_t hours = 0;
    int32_t minutes = 0;
    if (!read_two(p + pos + 1, hours) || p[pos + 3] != ':' || !read_two(p + pos + 4, minutes) ||
        hours > 23 || minutes > 59) {
        return false;
    }
    const int32_t magnitude = hours * 60 + minutes;
    fields.offset = TemporalOffset::Minutes;
    fields.offset_minutes = p[pos] == '-' ? -magnitude : magnitude;
    return true;
}

/// 8-4-4-4-12 hexadecimal digits at @p p (kUuidLength bytes readable).
[[nodiscard]] bool scan_uuid(const char* p, TemporalFields& fields) noexcept {
    uint64_t hi = 0;
    uint64_t lo = 0;
    size_t digits = 0;
    for (size_t i = 0; i < kUuidLength; ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (p[i] != '-') {
                return false;
            }
            continue;
        }
        const int value = hex_value(p[i]);
        if (value < 0) {
            return false;
        }
        uint64_t& half = digits < 16 ? hi : lo;
        half = half << 4 | static_cast<uint64_t>(value);
        ++digits;
    }
    fields.uuid_hi = hi;
    fields.uuid_lo = lo;
    return true;
}

} // namespace

size_t format_date(int year, int month, int day, char* out) noexcept {
    const auto y = static_cast<uint32_t>(year);
    write_two(y / 100, out);
    write_two(y, out + 2);
    out[4] = '-';
    write_two(static_cast<uint32_t>(month), out + 5);
    out[7] = '-';
    write_two(static_cast<uint32_t>(day), out + 8);
    return kDateLength;
}

size_t format_time(int hour, int minute, int second, int microsecond, char* out) noexcept {
    write_two(static_cast<uint32_t>(hour), out);
    out[2] = ':';
    write_two(static_cast<uint32_t>(minute), out + 3);
    out[5] = ':';
    write_two(static_cast<uint32_t>(second), out + 6);
    if (microsecond == 0) {
        return kTimeLength;
    }
    out[8] = '.';
    auto rest = static_cast<uint32_t>(microsecond);
    for (size_t i = kTimeFractionLength; i > kTimeLength + 1; --i) {
        out[i - 1] = static_cast<char>('0' + rest % 10);
        rest /= 10;
    }
    return kTimeFractionLength;
}

size_t format_utc_offset(int64_t seconds, char* out) noexcept {
    const bool negative = seconds < 0;
    // Unsigned negation: well defined for INT64_MIN too.
    const uint64_t magnitude =
        negative ? uint64_t{0} - static_cast<uint64_t>(seconds) : static_cast<uint64_t>(seconds);
    const uint64_t minutes = (magnitude + 30) / 60;
    out[0] = negative ? '-' : '+';
    write_two(static_cast<uint32_t>(minutes / 60 % 100), out + 1);
    out[3] = ':';
    write_two(static_cast<uint32_t>(minutes % 60), out + 4);
    return kUtcOffsetLength;
}

size_t format_uuid(uint64_t hi, uint64_t lo, char* out) noexcept {
    // The 32 digits first, eight at a time, then the 8-4-4-4-12 grouping.
    char hex[32];
    write_hex8(static_cast<uint32_t>(hi >> 32), hex);
    write_hex8(static_cast<uint32_t>(hi), hex + 8);
    write_hex8(static_cast<uint32_t>(lo >> 32), hex + 16);
    write_hex8(static_cast<uint32_t>(lo), hex + 24);
    std::memcpy(out, hex, 8);
    out[8] = '-';
    std::memcpy(out + 9, hex + 8, 4);
    out[13] = '-';
    std::memcpy(out + 14, hex + 12, 4);
    out[18] = '-';
    std::memcpy(out + 19, hex + 16, 4);
    out[23] = '-';
    std::memcpy(out + 24, hex + 20, 12);
    return kUuidLength;
}

bool is_json_number(std::string_view text) noexcept {
    const size_t length = text.size();
    size_t pos = 0;
    if (pos < length && text[pos] == '-') {
        ++pos;
    }
    if (pos == length) {
        return false;
    }
    if (text[pos] == '0') {
        ++pos;
    } else if (is_digit(text[pos])) {
        while (pos < length && is_digit(text[pos])) {
            ++pos;
        }
    } else {
        return false;
    }
    if (pos < length && text[pos] == '.') {
        const size_t start = ++pos;
        while (pos < length && is_digit(text[pos])) {
            ++pos;
        }
        if (pos == start) {
            return false;
        }
    }
    if (pos < length && (text[pos] == 'e' || text[pos] == 'E')) {
        ++pos;
        if (pos < length && (text[pos] == '+' || text[pos] == '-')) {
            ++pos;
        }
        const size_t start = pos;
        while (pos < length && is_digit(text[pos])) {
            ++pos;
        }
        if (pos == start) {
            return false;
        }
    }
    return pos == length;
}

TemporalFields scan_temporal(std::string_view text) noexcept {
    TemporalFields fields;
    const size_t length = text.size();
    const char* p = text.data();
    TemporalKind kind = TemporalKind::None;
    // The third byte tells a time (`:`) from a date, date-time or UUID (a
    // digit), and a ten-byte time (`HH:MM:SS.f`) is as long as a date.
    if (length >= kTimeLength && p[2] == ':') {
        kind = scan_time(text, fields) ? TemporalKind::Time : TemporalKind::None;
    } else if (length == kDateLength) {
        kind = scan_date(p, fields) ? TemporalKind::Date : TemporalKind::None;
    } else if (length == kUuidLength) {
        kind = scan_uuid(p, fields) ? TemporalKind::Uuid : TemporalKind::None;
    } else if (length >= kMinDateTimeLength && (p[kDateLength] == 'T' || p[kDateLength] == 't')) {
        kind = scan_date(p, fields) && scan_time(text.substr(kDateLength + 1), fields)
                   ? TemporalKind::DateTime
                   : TemporalKind::None;
    }
    if (kind == TemporalKind::None) {
        return TemporalFields{};
    }
    fields.kind = kind;
    return fields;
}

} // namespace strata::util

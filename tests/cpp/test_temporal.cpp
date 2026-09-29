/**
 * @file test_temporal.cpp
 * @brief Formatting and recognition of dates, times, offsets and UUIDs.
 *
 * Pins the core half of docs/architecture/native_types.md: the four
 * formatters the serializer's native tail writes with, the JSON-number check
 * a `Decimal`'s text goes through, and `scan_temporal`, which decides for
 * `parse_types` whether a string becomes a date, time, date-time or UUID.
 * Every rejection the record's "Parse contract" names has a case here, the
 * calendar is checked month by month, and every prefix of the longest forms is
 * scanned so that no length boundary is taken on trust.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/util/temporal.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>

namespace {

using strata::util::format_date;
using strata::util::format_time;
using strata::util::format_utc_offset;
using strata::util::format_uuid;
using strata::util::is_json_number;
using strata::util::scan_temporal;
using strata::util::TemporalFields;
using strata::util::TemporalKind;
using strata::util::TemporalOffset;

std::string date_text(int year, int month, int day) {
    char buffer[strata::util::kDateLength];
    const size_t written = format_date(year, month, day, buffer);
    assert(written == strata::util::kDateLength);
    return {buffer, written};
}

std::string time_text(int hour, int minute, int second, int microsecond) {
    char buffer[strata::util::kTimeFractionLength];
    const size_t written = format_time(hour, minute, second, microsecond, buffer);
    return {buffer, written};
}

std::string offset_text(int64_t seconds) {
    char buffer[strata::util::kUtcOffsetLength];
    const size_t written = format_utc_offset(seconds, buffer);
    assert(written == strata::util::kUtcOffsetLength);
    return {buffer, written};
}

std::string uuid_text(uint64_t hi, uint64_t lo) {
    char buffer[strata::util::kUuidLength];
    const size_t written = format_uuid(hi, lo, buffer);
    assert(written == strata::util::kUuidLength);
    return {buffer, written};
}

void expect_none(std::string_view text) {
    const TemporalFields fields = scan_temporal(text);
    if (fields.kind != TemporalKind::None) {
        std::printf("expected no match for \"%.*s\"\n", static_cast<int>(text.size()), text.data());
    }
    assert(fields.kind == TemporalKind::None);
    // A refusal carries no stale fields.
    assert(fields.offset == TemporalOffset::None && fields.year == 0 && fields.hour == 0 &&
           fields.microsecond == 0 && fields.uuid_hi == 0 && fields.uuid_lo == 0);
}

TemporalFields expect_kind(std::string_view text, TemporalKind kind) {
    const TemporalFields fields = scan_temporal(text);
    if (fields.kind != kind) {
        std::printf("unexpected kind for \"%.*s\"\n", static_cast<int>(text.size()), text.data());
    }
    assert(fields.kind == kind);
    return fields;
}

void expect_date(std::string_view text, int year, int month, int day) {
    const TemporalFields fields = expect_kind(text, TemporalKind::Date);
    assert(fields.year == year && fields.month == month && fields.day == day);
    assert(fields.offset == TemporalOffset::None);
}

// --- formatters ---------------------------------------------------------------

void test_format_date() {
    assert(date_text(1, 1, 1) == "0001-01-01");
    assert(date_text(9999, 12, 31) == "9999-12-31");
    assert(date_text(2024, 2, 29) == "2024-02-29");
    assert(date_text(999, 10, 9) == "0999-10-09");
    assert(date_text(10, 11, 30) == "0010-11-30");
}

void test_format_time() {
    assert(time_text(0, 0, 0, 0) == "00:00:00");
    assert(time_text(23, 59, 59, 0) == "23:59:59");
    assert(time_text(9, 5, 7, 1) == "09:05:07.000001");
    assert(time_text(12, 30, 45, 500000) == "12:30:45.500000");
    assert(time_text(23, 59, 59, 999999) == "23:59:59.999999");
    assert(time_text(1, 2, 3, 120) == "01:02:03.000120");
    char buffer[strata::util::kTimeFractionLength];
    assert(format_time(1, 2, 3, 0, buffer) == strata::util::kTimeLength);
    assert(format_time(1, 2, 3, 4, buffer) == strata::util::kTimeFractionLength);
}

void test_format_utc_offset_rounds_the_magnitude_half_up() {
    assert(offset_text(0) == "+00:00");
    // 29 s rounds down, 30 s up; the sign is kept even when the minutes are 0.
    assert(offset_text(29) == "+00:00");
    assert(offset_text(-29) == "-00:00");
    assert(offset_text(30) == "+00:01");
    assert(offset_text(-30) == "-00:01");
    assert(offset_text(59) == "+00:01");
    assert(offset_text(-59) == "-00:01");
    assert(offset_text(60) == "+00:01");
    assert(offset_text(89) == "+00:01");
    assert(offset_text(90) == "+00:02");
    assert(offset_text(-90) == "-00:02");
    // RFC 3339's own example: 19 min 32.13 s is written as the closest minute.
    assert(offset_text(19 * 60 + 32) == "+00:20");
    // Rounding carries into the hour instead of writing `:60`.
    assert(offset_text(3570) == "+01:00");
    assert(offset_text(-3570) == "-01:00");
    assert(offset_text(3569) == "+00:59");
    assert(offset_text(3600) == "+01:00");
    assert(offset_text(-18000) == "-05:00");
    assert(offset_text(19800) == "+05:30");
    assert(offset_text(-(3 * 3600 + 30 * 60)) == "-03:30");
    assert(offset_text(86340) == "+23:59");
    assert(offset_text(86369) == "+23:59");
    assert(offset_text(-86369) == "-23:59");
    // The documented edge: the last 30 s below a day round to 24:00.
    assert(offset_text(86370) == "+24:00");
    assert(offset_text(86399) == "+24:00");
    // Out of `datetime`'s range the write stays six bytes.
    assert(offset_text(INT64_MIN).size() == 6);
    assert(offset_text(INT64_MAX).size() == 6);
}

void test_format_uuid() {
    assert(uuid_text(0, 0) == "00000000-0000-0000-0000-000000000000");
    assert(uuid_text(UINT64_MAX, UINT64_MAX) == "ffffffff-ffff-ffff-ffff-ffffffffffff");
    assert(uuid_text(0x123456789abcdef0ULL, 0x0fedcba987654321ULL) ==
           "12345678-9abc-def0-0fed-cba987654321");
    // The halves are not swapped: only the first half's top nibble is set.
    assert(uuid_text(0x8000000000000000ULL, 0) == "80000000-0000-0000-0000-000000000000");
    assert(uuid_text(0, 1) == "00000000-0000-0000-0000-000000000001");

    // The word-at-a-time digits against the table-per-digit reference they
    // replaced (M15c): every nibble value in every position, then a stream of
    // pseudo-random halves.
    const auto reference = [](uint64_t hi, uint64_t lo) {
        static constexpr char kDigits[] = "0123456789abcdef";
        std::string text;
        for (int digit = 0; digit < 32; ++digit) {
            if (digit == 8 || digit == 12 || digit == 16 || digit == 20)
                text.push_back('-');
            const uint64_t half = digit < 16 ? hi : lo;
            text.push_back(kDigits[(half >> (60 - 4 * (digit % 16))) & 0xF]);
        }
        return text;
    };
    for (int position = 0; position < 16; ++position) {
        for (uint64_t nibble = 0; nibble < 16; ++nibble) {
            const uint64_t word = nibble << (4 * position);
            assert(uuid_text(word, ~word) == reference(word, ~word));
            assert(uuid_text(~word, word) == reference(~word, word));
        }
    }
    uint64_t state = 0x9e3779b97f4a7c15ULL;
    const auto next = [&state]() {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        return state;
    };
    for (int round = 0; round < 100000; ++round) {
        const uint64_t hi = next();
        const uint64_t lo = next();
        assert(uuid_text(hi, lo) == reference(hi, lo));
    }
}

// --- JSON number grammar ---------------------------------------------------------

void test_is_json_number() {
    for (const char* valid :
         {"0", "-0", "1", "-1", "10", "1234567890", "0.5", "-0.5", "1.50", "0.000001", "1E+2",
          "1e2", "1E-7", "1e+0", "-1.5E+3", "0E-7", "0.0e0", "123456789012345678901234567890"}) {
        if (!is_json_number(valid)) {
            std::printf("rejected valid number \"%s\"\n", valid);
        }
        assert(is_json_number(valid));
    }
    for (const char* invalid :
         {"",          "-",    "+1",  "01",  "-01",         "00",  "1.",    ".5",
          "-.5",       "1e",   "1e+", "1E-", "1.e5",        "NaN", "sNaN",  "Infinity",
          "-Infinity", "inf",  " 1",  "1 ",  "1_0",         "0x1", "1.5.5", "--1",
          "1e5.0",     "1ee5", "e5",  "1,5", "\xef\xbc\x91"}) {
        if (is_json_number(invalid)) {
            std::printf("accepted invalid number \"%s\"\n", invalid);
        }
        assert(!is_json_number(invalid));
    }
    assert(!is_json_number(std::string_view("1\0", 2)));
}

// --- dates -------------------------------------------------------------------------

void test_every_month_accepts_its_last_day_and_refuses_the_next() {
    constexpr int kCommon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int year : {2023, 2024, 1900, 2000}) {
        const bool leap = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
        for (int month = 1; month <= 12; ++month) {
            const int last = month == 2 && leap ? 29 : kCommon[month - 1];
            expect_date(date_text(year, month, 1), year, month, 1);
            expect_date(date_text(year, month, last), year, month, last);
            expect_none(date_text(year, month, last + 1));
            expect_none(date_text(year, month, 0));
        }
    }
}

void test_leap_year_rules() {
    expect_date("2024-02-29", 2024, 2, 29); // divisible by 4
    expect_none("2023-02-29");              // not divisible by 4
    expect_none("1900-02-29");              // by 100, not by 400
    expect_none("2100-02-29");
    expect_date("2000-02-29", 2000, 2, 29); // by 400
    expect_date("2400-02-29", 2400, 2, 29);
    expect_date("0004-02-29", 4, 2, 29);
    expect_none("0100-02-29");
    expect_date("0400-02-29", 400, 2, 29);
    expect_none("2024-02-30");
}

void test_date_range_and_shape() {
    expect_date("0001-01-01", 1, 1, 1);
    expect_date("9999-12-31", 9999, 12, 31);
    expect_none("0000-01-01"); // year 0 is not a `date`
    expect_none("2024-00-10");
    expect_none("2024-13-10");
    expect_none("2024-99-10");
    expect_none("2024-01-32");
    expect_none("2024-01-99");
    expect_none("2024/01/01");
    expect_none("2024-1-01");
    expect_none("24-01-01");
    expect_none("+2024-01-01");
    expect_none("-2024-01-01");
    expect_none("12024-01-01");
    expect_none("2024-01-01Z"); // a date takes no offset
    expect_none("2024-01-01 ");
    expect_none(" 2024-01-01");
    expect_none("2024-0a-01");
    expect_none("２０２４-01-01"); // fullwidth digits are not ASCII digits
    expect_none(std::string_view("2024-01-0\0", 10));
}

// --- times -------------------------------------------------------------------------

void expect_time(std::string_view text, int hour, int minute, int second, int microsecond) {
    const TemporalFields fields = expect_kind(text, TemporalKind::Time);
    assert(fields.hour == hour && fields.minute == minute && fields.second == second);
    assert(fields.microsecond == microsecond);
    assert(fields.year == 0 && fields.month == 0 && fields.day == 0);
}

void test_time_fields_and_ranges() {
    expect_time("00:00:00", 0, 0, 0, 0);
    expect_time("23:59:59", 23, 59, 59, 0);
    expect_time("09:05:07", 9, 5, 7, 0);
    assert(scan_temporal("12:00:00").offset == TemporalOffset::None);
    expect_none("24:00:00");
    expect_none("23:60:00");
    expect_none("23:59:60"); // no leap second
    expect_none("99:00:00");
    expect_none("12:30"); // seconds are required
    expect_none("12:30:0");
    expect_none("1:02:03");
    expect_none("12-30-00");
    expect_none("12:30:00 ");
    expect_none("T12:30:00");
}

void test_fraction_is_right_padded_to_microseconds() {
    expect_time("12:00:00.5", 12, 0, 0, 500000);
    expect_time("12:00:00.12", 12, 0, 0, 120000);
    expect_time("12:00:00.123", 12, 0, 0, 123000);
    expect_time("12:00:00.1234", 12, 0, 0, 123400);
    expect_time("12:00:00.12345", 12, 0, 0, 123450);
    expect_time("12:00:00.123456", 12, 0, 0, 123456);
    expect_time("12:00:00.000001", 12, 0, 0, 1);
    expect_time("12:00:00.0", 12, 0, 0, 0);
    expect_time("12:00:00.000000", 12, 0, 0, 0);
    expect_time("12:00:00.999999", 12, 0, 0, 999999);
    expect_none("12:00:00.1234567"); // seven digits: never truncated
    expect_none("12:00:00.0000000");
    expect_none("12:00:00.");
    expect_none("12:00:00.Z");
    expect_none("12:00:00.a");
    expect_none("12:00:00,5");
}

void expect_offset(std::string_view text, TemporalKind kind, int minutes) {
    const TemporalFields fields = expect_kind(text, kind);
    assert(fields.offset == TemporalOffset::Minutes);
    assert(fields.offset_minutes == minutes);
}

void test_offsets() {
    expect_offset("12:00:00Z", TemporalKind::Time, 0);
    expect_offset("12:00:00z", TemporalKind::Time, 0);
    expect_offset("12:00:00+00:00", TemporalKind::Time, 0);
    expect_offset("12:00:00-00:00", TemporalKind::Time, 0);
    expect_offset("12:00:00+05:30", TemporalKind::Time, 330);
    expect_offset("12:00:00-05:30", TemporalKind::Time, -330);
    expect_offset("12:00:00+23:59", TemporalKind::Time, 1439);
    expect_offset("12:00:00-23:59", TemporalKind::Time, -1439);
    expect_offset("12:00:00.5+01:00", TemporalKind::Time, 60);
    expect_offset("2024-01-01T00:00:00Z", TemporalKind::DateTime, 0);
    expect_offset("2024-01-01T00:00:00-00:00", TemporalKind::DateTime, 0);
    expect_offset("2024-01-01T00:00:00.123456-11:45", TemporalKind::DateTime, -705);
    expect_none("12:00:00+24:00");
    expect_none("12:00:00+05:60");
    expect_none("12:00:00+05:30:00"); // `±HH:MM:SS` is not RFC 3339
    expect_none("12:00:00+0530");
    expect_none("12:00:00+5:30");
    expect_none("12:00:00+05");
    expect_none("12:00:00 +05:30");
    expect_none("12:00:00Z ");
    expect_none("12:00:00ZZ");
    expect_none("12:00:00UTC");
    expect_none("12:00:00+05:3a");
    expect_none("12:00:00*05:30");
}

// --- date-times --------------------------------------------------------------------

void test_date_times() {
    TemporalFields fields = expect_kind("2024-02-29T23:59:59.25", TemporalKind::DateTime);
    assert(fields.year == 2024 && fields.month == 2 && fields.day == 29);
    assert(fields.hour == 23 && fields.minute == 59 && fields.second == 59);
    assert(fields.microsecond == 250000 && fields.offset == TemporalOffset::None);
    fields = expect_kind("0001-01-01t00:00:00", TemporalKind::DateTime);
    assert(fields.year == 1 && fields.hour == 0);
    expect_kind("9999-12-31T23:59:59.999999+23:59", TemporalKind::DateTime);
    expect_none("2024-01-01 12:00:00"); // a space separator
    expect_none("2024-01-01_12:00:00");
    expect_none("2024-01-01T12:00"); // no seconds
    expect_none("2024-02-30T00:00:00");
    expect_none("0000-01-01T00:00:00");
    expect_none("2024-01-01T24:00:00");
    expect_none("2024-01-01T23:59:60Z");
    expect_none("2024-01-01T12:00:00.1234567");
    expect_none("2024-01-01T12:00:00+05:30:00");
    expect_none("2024-01-01TT12:00:00");
    expect_none("2024-01-01T12:00:00.");
}

// --- UUIDs -------------------------------------------------------------------------

void test_uuids() {
    const uint64_t hi = 0x123e4567e89b12d3ULL;
    const uint64_t lo = 0xa456426614174000ULL;
    for (const char* text :
         {"123e4567-e89b-12d3-a456-426614174000", "123E4567-E89B-12D3-A456-426614174000",
          "123e4567-E89b-12D3-a456-426614174000"}) {
        const TemporalFields fields = expect_kind(text, TemporalKind::Uuid);
        assert(fields.uuid_hi == hi && fields.uuid_lo == lo);
        assert(fields.offset == TemporalOffset::None && fields.year == 0);
    }
    const TemporalFields top =
        expect_kind("ffffffff-ffff-ffff-ffff-ffffffffffff", TemporalKind::Uuid);
    assert(top.uuid_hi == UINT64_MAX && top.uuid_lo == UINT64_MAX);
    const TemporalFields zero =
        expect_kind("00000000-0000-0000-0000-000000000000", TemporalKind::Uuid);
    assert(zero.uuid_hi == 0 && zero.uuid_lo == 0);
    expect_none("123e4567-e89b-12d3-a456-42661417400g");
    expect_none("123e4567-e89b-12d3-a456_426614174000");
    expect_none("123e4567e89b-12d3-a456-4266141740000"); // a hyphen moved
    expect_none("123e45670e89b-12d3-a456-42661417400");
    expect_none("123e4567-e89b-12d3-a456-42661417400");   // 35
    expect_none("123e4567-e89b-12d3-a456-4266141740000"); // 37
    expect_none("123e4567e89b12d3a456426614174000");      // no hyphens
    expect_none("{123e4567-e89b-12d3-a456-426614174000}");
    expect_none("urn:uuid:123e4567-e89b-12d3-a456-426614174000");
    expect_none(" 23e4567-e89b-12d3-a456-426614174000");
    expect_none("123e4567-e89b-12d3-a456-42661417400 ");
    // `@`, `` ` `` and `G`/`g` sit next to the hex letters in ASCII.
    expect_none("@23e4567-e89b-12d3-a456-426614174000");
    expect_none("`23e4567-e89b-12d3-a456-426614174000");
    expect_none("G23e4567-e89b-12d3-a456-426614174000");
}

// --- boundaries and round trips ----------------------------------------------------

void test_every_prefix_of_the_longest_forms() {
    const std::string date_time = "9999-12-31T23:59:59.999999+23:59";
    assert(date_time.size() == 32);
    for (size_t n = 0; n <= date_time.size(); ++n) {
        const std::string_view prefix(date_time.data(), n);
        TemporalKind expected = TemporalKind::None;
        if (n == 10) {
            expected = TemporalKind::Date;
        } else if (n == 19 || (n >= 21 && n <= 26) || n == 32) {
            expected = TemporalKind::DateTime;
        }
        expect_kind(prefix, expected);
    }
    expect_none(date_time + "0");
    expect_none(date_time + "Z");

    const std::string time = "23:59:59.999999-23:59";
    assert(time.size() == 21);
    for (size_t n = 0; n <= time.size(); ++n) {
        const std::string_view prefix(time.data(), n);
        const bool whole = n == 8 || (n >= 10 && n <= 15) || n == 21;
        expect_kind(prefix, whole ? TemporalKind::Time : TemporalKind::None);
    }
    expect_none(time + "0");

    const std::string uuid = "123e4567-e89b-12d3-a456-426614174000";
    for (size_t n = 0; n < uuid.size(); ++n) {
        expect_none(std::string_view(uuid.data(), n));
    }
    expect_kind(uuid, TemporalKind::Uuid);
}

void test_what_the_formatters_write_is_recognized() {
    for (int year : {1, 999, 1970, 2024, 9999}) {
        for (int micro : {0, 1, 500000, 999999}) {
            for (int64_t offset :
                 {int64_t{0}, int64_t{-29}, int64_t{19800}, int64_t{-43200}, int64_t{86340}}) {
                const std::string text = date_text(year, 12, 31) + "T" +
                                         time_text(13, 14, 15, micro) + offset_text(offset);
                const TemporalFields fields = expect_kind(text, TemporalKind::DateTime);
                assert(fields.year == year && fields.month == 12 && fields.day == 31);
                assert(fields.hour == 13 && fields.minute == 14 && fields.second == 15);
                assert(fields.microsecond == micro);
                assert(fields.offset == TemporalOffset::Minutes);
                const int64_t magnitude = offset < 0 ? -offset : offset;
                const int64_t minutes = (magnitude + 30) / 60;
                assert(fields.offset_minutes == (offset < 0 ? -minutes : minutes));
            }
        }
    }
    const uint64_t halves[][2] = {{0, 0},
                                  {UINT64_MAX, UINT64_MAX},
                                  {0x0123456789abcdefULL, 0xfedcba9876543210ULL},
                                  {1, 0x8000000000000000ULL}};
    for (const auto& half : halves) {
        const TemporalFields fields = expect_kind(uuid_text(half[0], half[1]), TemporalKind::Uuid);
        assert(fields.uuid_hi == half[0] && fields.uuid_lo == half[1]);
    }
}

void test_unrelated_strings_are_none() {
    for (const char* text : {"", "a", "hello", "2024", "12:00", "true", "null", "0", "12345678",
                             "abcdefghij", "1234567890", "12:00:00:00", "::::::::"}) {
        expect_none(text);
    }
}

} // namespace

int main() {
    test_format_date();
    test_format_time();
    test_format_utc_offset_rounds_the_magnitude_half_up();
    test_format_uuid();
    test_is_json_number();
    test_every_month_accepts_its_last_day_and_refuses_the_next();
    test_leap_year_rules();
    test_date_range_and_shape();
    test_time_fields_and_ranges();
    test_fraction_is_right_padded_to_microseconds();
    test_offsets();
    test_date_times();
    test_uuids();
    test_every_prefix_of_the_longest_forms();
    test_what_the_formatters_write_is_recognized();
    test_unrelated_strings_are_none();
    std::printf("temporal tests passed\n");
    return 0;
}

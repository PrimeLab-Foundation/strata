#pragma once

/**
 * @file temporal.hpp
 * @brief Text forms of dates, times, UTC offsets and UUIDs, and their recognition.
 *
 * The pure half of native-type support (docs/architecture/native_types.md): the
 * binding layer reads a Python object into integers and calls a formatter here,
 * and the `parse_types` revival hands a string here and builds an object from
 * the fields it gets back. Nothing in this file knows about CPython.
 *
 * Formatters write into a caller buffer at least as long as the maximum they
 * document and return the number of bytes written; they never NUL-terminate.
 * Their inputs are the ranges `datetime` and `uuid` guarantee; outside them the
 * output is unspecified but the write never exceeds the documented maximum.
 */

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace strata::util {

/// Bytes `format_date` writes: `YYYY-MM-DD`.
inline constexpr size_t kDateLength = 10;
/// Bytes `format_time` writes without a fraction: `HH:MM:SS`.
inline constexpr size_t kTimeLength = 8;
/// Bytes `format_time` writes with a fraction: `HH:MM:SS.ffffff`.
inline constexpr size_t kTimeFractionLength = 15;
/// Bytes `format_utc_offset` writes: `+HH:MM` or `-HH:MM`.
inline constexpr size_t kUtcOffsetLength = 6;
/// Bytes `format_uuid` writes: `xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx`.
inline constexpr size_t kUuidLength = 36;

/**
 * Write @p year-@p month-@p day as `YYYY-MM-DD`, the year zero-padded to four
 * digits. Expects 0 <= year <= 9999, 1 <= month <= 12, 1 <= day <= 31.
 * @return kDateLength.
 */
[[nodiscard]] size_t format_date(int year, int month, int day, char* out) noexcept;

/**
 * Write `HH:MM:SS`, then `.ffffff` (six digits) when @p microsecond is nonzero.
 * Expects hour < 24, minute < 60, second < 60, microsecond < 1'000'000.
 * @return kTimeLength, or kTimeFractionLength when a fraction was written.
 */
[[nodiscard]] size_t format_time(int hour, int minute, int second, int microsecond,
                                 char* out) noexcept;

/**
 * Write a UTC offset of @p seconds east of UTC as `+HH:MM` or `-HH:MM`.
 *
 * The magnitude is rounded to the minute half-up (29 s down, 30 s up) and the
 * sign is kept, so -29 s is `-00:00`; 0 is `+00:00`. Expects |seconds| < 86400
 * (`datetime`'s bound); from 86370 s the rounding reaches `24:00`, which
 * `scan_temporal` does not accept back.
 * @return kUtcOffsetLength.
 */
[[nodiscard]] size_t format_utc_offset(int64_t seconds, char* out) noexcept;

/**
 * Write the 128-bit value @p hi:@p lo (hi the most significant half) as a
 * lowercase 8-4-4-4-12 UUID.
 * @return kUuidLength.
 */
[[nodiscard]] size_t format_uuid(uint64_t hi, uint64_t lo, char* out) noexcept;

/**
 * True when @p text is exactly one JSON number (RFC 8259 §6):
 * `-? (0 | [1-9][0-9]*) (. [0-9]+)? ([eE] [+-]? [0-9]+)?`, nothing before or after.
 */
[[nodiscard]] bool is_json_number(std::string_view text) noexcept;

/// What `scan_temporal` recognized.
enum class TemporalKind : uint8_t { None, Date, Time, DateTime, Uuid };

/// Whether a recognized time or date-time carried an offset.
enum class TemporalOffset : uint8_t { None, Minutes };

/**
 * Fields of a recognized string. Only the fields of @ref kind are meaningful;
 * the rest are zero.
 */
struct TemporalFields {
    TemporalKind kind = TemporalKind::None;
    /// `Minutes` when a `Z`, `z` or `±HH:MM` offset was present.
    TemporalOffset offset = TemporalOffset::None;
    /// Minutes east of UTC; `Z`, `z`, `+00:00` and `-00:00` are all 0.
    int32_t offset_minutes = 0;
    int32_t year = 0;
    int32_t month = 0;
    int32_t day = 0;
    int32_t hour = 0;
    int32_t minute = 0;
    int32_t second = 0;
    /// The 1–6 fraction digits, right-padded to six (`.5` is 500000).
    int32_t microsecond = 0;
    /// Most significant 64 bits of a UUID.
    uint64_t uuid_hi = 0;
    /// Least significant 64 bits of a UUID.
    uint64_t uuid_lo = 0;
};

/**
 * Recognize @p text, whole, as one of the `parse_types` kinds
 * (docs/architecture/native_types.md, "Parse contract"):
 *
 * - date: `YYYY-MM-DD`, a real calendar day, year 1–9999;
 * - time: `HH:MM:SS` [`.` 1–6 digits] [`Z` | `z` | `±HH:MM`], H < 24, M < 60,
 *   S < 60 (no leap second), offset HH < 24 and MM < 60;
 * - date-time: a date, `T` or `t`, a time;
 * - UUID: 8-4-4-4-12 hexadecimal digits, each of either case.
 *
 * Anything else — a space separator, seven fraction digits, `HH:MM` without
 * seconds, a `±HH:MM:SS` offset, surrounding whitespace — is `None`.
 */
[[nodiscard]] TemporalFields scan_temporal(std::string_view text) noexcept;

} // namespace strata::util

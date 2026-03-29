#pragma once

#include <cstdint>
#include <expected>
#include <string_view>

namespace strata {

    enum class ErrorCode : uint8_t {
        UnexpectedEnd,
        UnexpectedChar,
        InvalidLiteral,
        InvalidEscape,
        InvalidUnicode,
        UnterminatedString,
        NumberOverflow,
        LeadingZero,
        InvalidNumber,
    };

    struct ParseError {
        ErrorCode   code;
        const char* where;
    };

    template <typename T>
    struct Parsed {
        T           value;
        const char* rest;
    };

    template <typename T>
    using Result = std::expected<Parsed<T>, ParseError>;

}
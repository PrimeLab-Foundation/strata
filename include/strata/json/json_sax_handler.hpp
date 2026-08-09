#pragma once

/**
 * @file json_sax_handler.hpp
 * @brief Abstract interface for SAX-style JSON parsing.
 *
 * Handlers receive one callback per JSON token. Returning false from any
 * callback aborts the parse immediately and surfaces as Status::ParseError.
 *
 * This abstract base exists for callers that need runtime polymorphism. The
 * parser itself is a template, so instantiating
 * `parse_sax_inline<ConcreteHandler>()` devirtualises and inlines every
 * callback (see json_parser_inline.hpp) — that is the path both the DOM
 * builder and the Python builder take.
 *
 * **String lifetime:** the views handed to on_string(), on_key() and
 * on_big_int() are valid only for the duration of the call. Escape-free
 * strings point straight into the caller's input buffer; escaped ones point
 * into a scratch buffer the parser reuses. Copy anything you keep.
 */

#include <cstdint>
#include <string_view>

namespace strata {

/**
 * Abstract base for SAX-style JSON event handlers.
 *
 * Each callback returns true to continue parsing, false to abort.
 */
class JsonSaxHandler {
  public:
    virtual ~JsonSaxHandler() = default;

    // --- Scalar events -----------------------------------------------------
    virtual bool on_null() = 0;
    virtual bool on_bool(bool value) = 0;

    /// An integer literal that fits in int64_t.
    virtual bool on_int(int64_t value) = 0;

    /**
     * An integer literal that does *not* fit in int64_t, as written.
     *
     * @param text The raw token, including any leading '-'. Handlers that need
     *        exactness convert it themselves (the Python builder builds an
     *        arbitrary-precision int); the C++ DOM widens it to double and
     *        accepts the precision loss.
     */
    virtual bool on_big_int(std::string_view text) = 0;

    /// A number with a fraction or an exponent.
    virtual bool on_double(double value) = 0;

    virtual bool on_string(std::string_view value) = 0;

    // --- Object events -----------------------------------------------------
    virtual bool on_start_object() = 0;
    virtual bool on_key(std::string_view key) = 0;
    virtual bool on_end_object() = 0;

    // --- Array events ------------------------------------------------------
    virtual bool on_start_array() = 0;
    virtual bool on_end_array() = 0;
};

} // namespace strata

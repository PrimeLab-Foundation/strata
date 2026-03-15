#pragma once

/**
 * @file json_sax_handler.hpp
 * @brief Abstract interface for SAX-style JSON parsing.
 *
 * Handlers receive events as the parser encounters JSON tokens.
 * Returning false from any callback aborts the parse immediately.
 *
 * For maximum performance use parse_sax_inline<ConcreteHandler>() which
 * enables the compiler to devirtualise every callback through template
 * instantiation (see json_parser_inline.hpp).
 */

#include <cstdint>
#include <string_view>

namespace strata {

/**
 * Abstract base for SAX-style JSON event handlers.
 *
 * Each callback returns true to continue parsing, false to abort.
 * Subclasses must implement every pure-virtual method.
 */
class JsonSaxHandler {
  public:
    virtual ~JsonSaxHandler() = default;

    // --- Scalar events -----------------------------------------------------
    virtual bool on_null() = 0;
    virtual bool on_bool(bool v) = 0;
    virtual bool on_int(int64_t v) = 0;
    virtual bool on_uint(uint64_t v) = 0;
    virtual bool on_double(double v) = 0;
    virtual bool on_string(std::string_view v) = 0;

    // --- Object events -----------------------------------------------------
    virtual bool on_start_object(size_t size_hint = 0) = 0;
    virtual bool on_key(std::string_view v) = 0;
    virtual bool on_end_object() = 0;

    // --- Array events ------------------------------------------------------
    virtual bool on_start_array(size_t size_hint = 0) = 0;
    virtual bool on_end_array() = 0;
};

} // namespace strata

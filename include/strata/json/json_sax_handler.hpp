#pragma once

#include <cstdint>
#include <string_view>

namespace strata {

/**
 * Interface for SAX-style JSON parsing.
 * Handlers should return true to continue parsing, false to abort.
 *
 * String handling:
 * - on_string and on_key receive the string content (without quotes)
 * - The has_escapes parameter indicates if the string contains backslash escapes
 * - If has_escapes is false, the string_view can be used directly
 * - If has_escapes is true, the string needs to be unescaped before use
 * - Handlers can use LazyString to defer unescaping until the value is accessed
 */
class JsonSaxHandler {
  public:
    virtual ~JsonSaxHandler() = default;

    virtual bool on_null() = 0;
    virtual bool on_bool(bool v) = 0;
    virtual bool on_int(int64_t v) = 0;
    virtual bool on_uint(uint64_t v) = 0;
    virtual bool on_double(double v) = 0;

    /**
     * Called when a string value is parsed.
     * @param v The string content (without quotes)
     * @param has_escapes True if the string contains backslash escapes requiring processing
     * @return true to continue parsing, false to abort
     */
    virtual bool on_string(std::string_view v, bool has_escapes = false) = 0;

    virtual bool on_start_object(size_t size_hint = 0) = 0;

    /**
     * Called when an object key is parsed.
     * @param v The key content (without quotes)
     * @param has_escapes True if the key contains backslash escapes requiring processing
     * @return true to continue parsing, false to abort
     */
    virtual bool on_key(std::string_view v, bool has_escapes = false) = 0;

    virtual bool on_end_object() = 0;

    virtual bool on_start_array(size_t size_hint = 0) = 0;
    virtual bool on_end_array() = 0;
};

} // namespace strata

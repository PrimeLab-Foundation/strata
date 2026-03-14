#pragma once

#include <cstdint>
#include <string_view>

namespace strata {

/**
 * Interface for SAX-style JSON parsing.
 * Handlers should return true to continue parsing, false to abort.
 */
class JsonSaxHandler {
  public:
    virtual ~JsonSaxHandler() = default;

    virtual bool on_null() = 0;
    virtual bool on_bool(bool v) = 0;
    virtual bool on_int(int64_t v) = 0;
    virtual bool on_uint(uint64_t v) = 0;
    virtual bool on_double(double v) = 0;
    virtual bool on_string(std::string_view v) = 0;

    virtual bool on_start_object(size_t size_hint = 0) = 0;
    virtual bool on_key(std::string_view v) = 0;
    virtual bool on_end_object() = 0;

    virtual bool on_start_array(size_t size_hint = 0) = 0;
    virtual bool on_end_array() = 0;
};

} // namespace strata

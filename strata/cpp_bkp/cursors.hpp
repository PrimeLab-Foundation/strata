#pragma once

#include "platform.hpp"

#include <cstdint>
#include <optional>
#include <simdjson.h>
#include <string_view>

namespace strata {

/**
 * Status codes for cursor operations (no exceptions in hot path)
 */
enum class Status { Ok, TypeMismatch, KeyNotFound, IndexOutOfBounds, ParseError };

/**
 * Result type for operations that may fail
 */
template <typename T> struct Result {
    Status status;
    T value;

    bool ok() const { return status == Status::Ok; }
    T unwrap() const { return value; }
    T value_or(T default_val) const { return ok() ? value : default_val; }
};

/**
 * Lightweight wrapper around simdjson::ondemand::value
 * Provides status-code based access without exceptions
 */
class Cursor {
  public:
    explicit Cursor(simdjson::ondemand::value val) : value_(val) {}

    /**
     * Type checking
     */
    TJ_ALWAYS_INLINE bool is_null() const { return value_.is_null().value_or(false); }

    TJ_ALWAYS_INLINE bool is_bool() const {
        simdjson::ondemand::json_type type;
        if (value_.type().get(type) != simdjson::SUCCESS)
            return false;
        return type == simdjson::ondemand::json_type::boolean;
    }

    TJ_ALWAYS_INLINE bool is_number() const {
        simdjson::ondemand::json_type type;
        if (value_.type().get(type) != simdjson::SUCCESS)
            return false;
        return type == simdjson::ondemand::json_type::number;
    }

    TJ_ALWAYS_INLINE bool is_string() const {
        simdjson::ondemand::json_type type;
        if (value_.type().get(type) != simdjson::SUCCESS)
            return false;
        return type == simdjson::ondemand::json_type::string;
    }

    TJ_ALWAYS_INLINE bool is_array() const {
        simdjson::ondemand::json_type type;
        if (value_.type().get(type) != simdjson::SUCCESS)
            return false;
        return type == simdjson::ondemand::json_type::array;
    }

    TJ_ALWAYS_INLINE bool is_object() const {
        simdjson::ondemand::json_type type;
        if (value_.type().get(type) != simdjson::SUCCESS)
            return false;
        return type == simdjson::ondemand::json_type::object;
    }

    /**
     * Scalar accessors (status-code based)
     */
    Result<bool> get_bool() const {
        bool val;
        auto err = value_.get_bool().get(val);
        return {err == simdjson::SUCCESS ? Status::Ok : Status::TypeMismatch, val};
    }

    Result<int64_t> get_int64() const {
        int64_t val;
        auto err = value_.get_int64().get(val);
        return {err == simdjson::SUCCESS ? Status::Ok : Status::TypeMismatch, val};
    }

    Result<uint64_t> get_uint64() const {
        uint64_t val;
        auto err = value_.get_uint64().get(val);
        return {err == simdjson::SUCCESS ? Status::Ok : Status::TypeMismatch, val};
    }

    Result<double> get_double() const {
        double val;
        auto err = value_.get_double().get(val);
        return {err == simdjson::SUCCESS ? Status::Ok : Status::TypeMismatch, val};
    }

    Result<std::string_view> get_string() const {
        std::string_view val;
        auto err = value_.get_string().get(val);
        return {err == simdjson::SUCCESS ? Status::Ok : Status::TypeMismatch, val};
    }

    /**
     * Get number as double (tries int64, uint64, then double)
     */
    Result<double> get_number() const {
        // Try int64 first
        int64_t i64;
        if (value_.get_int64().get(i64) == simdjson::SUCCESS) {
            return {Status::Ok, static_cast<double>(i64)};
        }

        // Try uint64
        uint64_t u64;
        if (value_.get_uint64().get(u64) == simdjson::SUCCESS) {
            return {Status::Ok, static_cast<double>(u64)};
        }

        // Try double
        double d;
        if (value_.get_double().get(d) == simdjson::SUCCESS) {
            return {Status::Ok, d};
        }

        return {Status::TypeMismatch, 0.0};
    }

    /**
     * Object field access
     */
    Result<Cursor> get_field(std::string_view key) const {
        if (!is_object()) {
            return {Status::TypeMismatch, Cursor(value_)};
        }

        auto obj_result = value_.get_object();
        if (obj_result.error()) {
            return {Status::TypeMismatch, Cursor(value_)};
        }

        auto obj = obj_result.value();
        auto field_result = obj[key];

        if (field_result.error()) {
            return {Status::KeyNotFound, Cursor(value_)};
        }

        return {Status::Ok, Cursor(field_result.value())};
    }

    /**
     * Array element access
     */
    Result<Cursor> get_at(size_t index) const {
        if (!is_array()) {
            return {Status::TypeMismatch, Cursor(value_)};
        }

        auto arr_result = value_.get_array();
        if (arr_result.error()) {
            return {Status::TypeMismatch, Cursor(value_)};
        }

        auto arr = arr_result.value();
        auto elem_result = arr.at(index);

        if (elem_result.error()) {
            return {Status::IndexOutOfBounds, Cursor(value_)};
        }

        return {Status::Ok, Cursor(elem_result.value())};
    }

    /**
     * Get underlying simdjson value (for advanced usage)
     */
    simdjson::ondemand::value& raw() { return value_; }
    const simdjson::ondemand::value& raw() const { return value_; }

  private:
    simdjson::ondemand::value value_;
};

/**
 * Object iterator wrapper
 */
class ObjectIterator {
  public:
    explicit ObjectIterator(simdjson::ondemand::object obj) : obj_(obj) {}

    class Iterator {
      public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::pair<std::string_view, Cursor>;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        explicit Iterator(simdjson::ondemand::object::iterator it) : it_(it) {}

        value_type operator*() const {
            auto field = *it_;
            return {field.unescaped_key().value(), Cursor(field.value())};
        }

        Iterator& operator++() {
            ++it_;
            return *this;
        }

        bool operator!=(const Iterator& other) const { return it_ != other.it_; }

      private:
        simdjson::ondemand::object::iterator it_;
    };

    Iterator begin() { return Iterator(obj_.begin()); }
    Iterator end() { return Iterator(obj_.end()); }

  private:
    simdjson::ondemand::object obj_;
};

/**
 * Array iterator wrapper
 */
class ArrayIterator {
  public:
    explicit ArrayIterator(simdjson::ondemand::array arr) : arr_(arr) {}

    class Iterator {
      public:
        using iterator_category = std::input_iterator_tag;
        using value_type = Cursor;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        explicit Iterator(simdjson::ondemand::array::iterator it) : it_(it) {}

        value_type operator*() const { return Cursor((*it_).value()); }

        Iterator& operator++() {
            ++it_;
            return *this;
        }

        bool operator!=(const Iterator& other) const { return it_ != other.it_; }

      private:
        simdjson::ondemand::array::iterator it_;
    };

    Iterator begin() { return Iterator(arr_.begin()); }
    Iterator end() { return Iterator(arr_.end()); }

  private:
    simdjson::ondemand::array arr_;
};

} // namespace strata

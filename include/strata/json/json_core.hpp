#pragma once

/**
 * @file json_core.hpp
 * @brief Core JSON value model and utility types.
 *
 * Defines the in-memory representation of JSON values using a type-safe
 * std::variant, a cache-friendly FlatMap for JSON objects, and a
 * Status/Result<T> error-handling model for the hot path (no exceptions).
 */

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace strata {

/**
 * Vector-backed ordered map optimised for small JSON objects.
 *
 * JSON objects typically have fewer than ~20 keys, making linear
 * search faster than hash-map lookup thanks to better cache locality
 * (keys and values sit in a single contiguous allocation).
 *
 * @tparam K Key type   (usually std::string)
 * @tparam V Value type (usually JsonValue)
 */
template <typename K, typename V> class FlatMap {
  public:
    using value_type = std::pair<K, V>;
    using vector_type = std::vector<value_type>;
    using iterator = typename vector_type::iterator;
    using const_iterator = typename vector_type::const_iterator;

    [[nodiscard]] iterator begin() noexcept { return data_.begin(); }
    [[nodiscard]] iterator end() noexcept { return data_.end(); }
    [[nodiscard]] const_iterator begin() const noexcept { return data_.begin(); }
    [[nodiscard]] const_iterator end() const noexcept { return data_.end(); }

    /// Insert-or-access by lvalue key (mirrors std::map::operator[]).
    V& operator[](const K& key) {
        for (auto& p : data_) {
            if (p.first == key)
                return p.second;
        }
        data_.emplace_back(key, V());
        return data_.back().second;
    }

    /// Insert-or-access by rvalue key (avoids copy when inserting).
    V& operator[](K&& key) {
        for (auto& p : data_) {
            if (p.first == key)
                return p.second;
        }
        data_.emplace_back(std::move(key), V());
        return data_.back().second;
    }

    [[nodiscard]] iterator find(const K& key) {
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->first == key)
                return it;
        }
        return data_.end();
    }

    [[nodiscard]] const_iterator find(const K& key) const {
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->first == key)
                return it;
        }
        return data_.end();
    }

    /// Throws std::out_of_range if @p key is absent.
    [[nodiscard]] const V& at(const K& key) const {
        auto it = find(key);
        if (it == data_.end())
            throw std::out_of_range("FlatMap::at");
        return it->second;
    }

    /// Throws std::out_of_range if @p key is absent.
    [[nodiscard]] V& at(const K& key) {
        auto it = find(key);
        if (it == data_.end())
            throw std::out_of_range("FlatMap::at");
        return it->second;
    }

    template <typename... Args> void emplace(Args&&... args) {
        data_.emplace_back(std::forward<Args>(args)...);
    }

    [[nodiscard]] size_t size() const noexcept { return data_.size(); }
    [[nodiscard]] bool empty() const noexcept { return data_.empty(); }
    void clear() noexcept { data_.clear(); }

    [[nodiscard]] size_t count(const K& key) const { return find(key) != end() ? 1 : 0; }

  private:
    vector_type data_;
};

/// Status codes for cursor operations (no exceptions on the hot path).
enum class Status { Ok, TypeMismatch, KeyNotFound, IndexOutOfBounds, ParseError };

/**
 * Lightweight result type for operations that may fail.
 *
 * Prefer the status-code API on hot paths; the throwing helpers in
 * JsonCursor wrap these for callers that prefer exceptions.
 *
 * @tparam T The value type on success.
 */
template <typename T> struct Result {
    Status status;
    T value;

    [[nodiscard]] bool ok() const noexcept { return status == Status::Ok; }
    [[nodiscard]] T unwrap() const { return value; }
    [[nodiscard]] T value_or(T default_val) const { return ok() ? value : default_val; }
};

/**
 * In-memory JSON value using a type-safe variant.
 *
 * Supported types: null, bool, number (double), string, array, object.
 * Objects use FlatMap for cache-friendly iteration over small key sets.
 */
struct JsonValue {
    using Array = std::vector<JsonValue>;
    using Object = FlatMap<std::string, JsonValue>;
    using Number = double; // All JSON numbers stored as IEEE 754 double

    using Variant = std::variant<std::nullptr_t, bool, Number, std::string, Array, Object>;

    Variant data;

    JsonValue() noexcept : data(nullptr) {}
    explicit JsonValue(Variant v) : data(std::move(v)) {}

    // --- Type predicates ---------------------------------------------------
    [[nodiscard]] bool is_null() const noexcept {
        return std::holds_alternative<std::nullptr_t>(data);
    }
    [[nodiscard]] bool is_bool() const noexcept { return std::holds_alternative<bool>(data); }
    [[nodiscard]] bool is_number() const noexcept { return std::holds_alternative<Number>(data); }
    [[nodiscard]] bool is_string() const noexcept {
        return std::holds_alternative<std::string>(data);
    }
    [[nodiscard]] bool is_array() const noexcept { return std::holds_alternative<Array>(data); }
    [[nodiscard]] bool is_object() const noexcept { return std::holds_alternative<Object>(data); }

    // --- Const accessors (throw std::bad_variant_access on mismatch) -------
    [[nodiscard]] const bool& as_bool() const { return std::get<bool>(data); }
    [[nodiscard]] const Number& as_number() const { return std::get<Number>(data); }
    [[nodiscard]] const std::string& as_string() const { return std::get<std::string>(data); }
    [[nodiscard]] const Array& as_array() const { return std::get<Array>(data); }
    [[nodiscard]] const Object& as_object() const { return std::get<Object>(data); }

    // --- Mutable accessors -------------------------------------------------
    [[nodiscard]] bool& as_bool() { return std::get<bool>(data); }
    [[nodiscard]] Number& as_number() { return std::get<Number>(data); }
    [[nodiscard]] std::string& as_string() { return std::get<std::string>(data); }
    [[nodiscard]] Array& as_array() { return std::get<Array>(data); }
    [[nodiscard]] Object& as_object() { return std::get<Object>(data); }
};

} // namespace strata

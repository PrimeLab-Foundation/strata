/**
 * @file json_core.hpp
 * @brief Core JSON value types and utilities for Strata.
 *
 * This header defines the fundamental types used throughout Strata:
 *
 * Types:
 * - JsonValue: Variant-based JSON value (null, bool, number, string, array, object)
 * - FlatMap<K,V>: Vector-based map with an optional hash index for larger objects
 * - Status: Error codes for cursor operations
 * - Result<T>: Error-or-value type for exception-free APIs
 *
 * Design rationale:
 * - JsonValue uses std::variant for type-safe value storage
 * - FlatMap provides O(n) lookup for small objects and a hash index for larger ones
 * - Number type is double (sufficient for JSON's numeric range)
 * - Status/Result enable exception-free hot paths
 *
 * @note This header is pure C++ with no Python dependencies.
 */

#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace strata {

// Simple vector-based map for JSON objects with optional hash index for larger objects
template <typename K, typename V> class FlatMap {
  public:
    using value_type = std::pair<K, V>;
    using vector_type = std::vector<value_type>;
    using iterator = typename vector_type::iterator;
    using const_iterator = typename vector_type::const_iterator;

    iterator begin() { return data_.begin(); }
    iterator end() { return data_.end(); }
    const_iterator begin() const { return data_.begin(); }
    const_iterator end() const { return data_.end(); }

    V& operator[](const K& key) {
        auto it = find(key);
        if (it != data_.end())
            return it->second;
        data_.emplace_back(key, V());
        on_inserted(data_.size() - 1);
        return data_.back().second;
    }

    V& operator[](K&& key) {
        auto it = find(key);
        if (it != data_.end())
            return it->second;
        data_.emplace_back(std::move(key), V());
        on_inserted(data_.size() - 1);
        return data_.back().second;
    }

    iterator find(const K& key) {
        if (index_active_) {
            auto it = index_.find(key);
            if (it == index_.end())
                return data_.end();
            return data_.begin() + it->second;
        }
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->first == key)
                return it;
        }
        return data_.end();
    }

    const_iterator find(const K& key) const {
        if (index_active_) {
            auto it = index_.find(key);
            if (it == index_.end())
                return data_.end();
            return data_.begin() + it->second;
        }
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->first == key)
                return it;
        }
        return data_.end();
    }

    const V& at(const K& key) const {
        auto it = find(key);
        if (it == data_.end())
            throw std::out_of_range("FlatMap::at");
        return it->second;
    }

    V& at(const K& key) {
        auto it = find(key);
        if (it == data_.end())
            throw std::out_of_range("FlatMap::at");
        return it->second;
    }

    template <typename... Args> void emplace(Args&&... args) {
        data_.emplace_back(std::forward<Args>(args)...);
        on_inserted(data_.size() - 1);
    }

    size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
    void clear() {
        data_.clear();
        index_.clear();
        index_active_ = false;
    }

    size_t count(const K& key) const { return find(key) != end() ? 1 : 0; }

  private:
    static constexpr size_t kIndexThreshold = 16;

    void build_index() {
        index_.clear();
        index_.reserve(data_.size());
        for (size_t i = 0; i < data_.size(); ++i) {
            index_.emplace(data_[i].first, i);
        }
        index_active_ = true;
    }

    void on_inserted(size_t idx) {
        if (index_active_) {
            index_.emplace(data_[idx].first, idx);
            return;
        }
        if (data_.size() > kIndexThreshold) {
            build_index();
        }
    }

    vector_type data_;
    std::unordered_map<K, size_t> index_;
    bool index_active_ = false;
};

// Status codes for cursor operations (no exceptions in hot path)
enum class Status { Ok, TypeMismatch, KeyNotFound, IndexOutOfBounds, ParseError };

// Result type for operations that may fail
template <typename T> struct Result {
    Status status;
    T value;

    bool ok() const { return status == Status::Ok; }
    T unwrap() const { return value; }
    T value_or(T default_val) const { return ok() ? value : default_val; }
};

// Simple in‑memory JSON value model
struct JsonValue {
    using Array = std::vector<JsonValue>;
    using Object = FlatMap<std::string, JsonValue>; // FlatMap + hash index for larger objects
    using Number = double;                          // keep it simple for now

    using Variant = std::variant<std::nullptr_t, bool, Number, std::string, Array, Object>;

    Variant data;

    JsonValue() : data(nullptr) {}
    explicit JsonValue(Variant v) : data(std::move(v)) {}

    bool is_null() const { return std::holds_alternative<std::nullptr_t>(data); }
    bool is_bool() const { return std::holds_alternative<bool>(data); }
    bool is_number() const { return std::holds_alternative<Number>(data); }
    bool is_string() const { return std::holds_alternative<std::string>(data); }
    bool is_array() const { return std::holds_alternative<Array>(data); }
    bool is_object() const { return std::holds_alternative<Object>(data); }

    const bool& as_bool() const { return std::get<bool>(data); }
    const Number& as_number() const { return std::get<Number>(data); }
    const std::string& as_string() const { return std::get<std::string>(data); }
    const Array& as_array() const { return std::get<Array>(data); }
    const Object& as_object() const { return std::get<Object>(data); }

    bool& as_bool() { return std::get<bool>(data); }
    Number& as_number() { return std::get<Number>(data); }
    std::string& as_string() { return std::get<std::string>(data); }
    Array& as_array() { return std::get<Array>(data); }
    Object& as_object() { return std::get<Object>(data); }
};

} // namespace strata

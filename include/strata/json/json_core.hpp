#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace strata {

// Simple vector-based map for JSON objects (better cache locality for small objects)
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
        for (auto& p : data_) {
            if (p.first == key)
                return p.second;
        }
        data_.emplace_back(key, V());
        return data_.back().second;
    }

    V& operator[](K&& key) {
        for (auto& p : data_) {
            if (p.first == key)
                return p.second;
        }
        data_.emplace_back(std::move(key), V());
        return data_.back().second;
    }

    iterator find(const K& key) {
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->first == key)
                return it;
        }
        return data_.end();
    }

    const_iterator find(const K& key) const {
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
    }

    size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
    void clear() { data_.clear(); }

    size_t count(const K& key) const { return find(key) != end() ? 1 : 0; }

  private:
    vector_type data_;
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
    using Object = FlatMap<std::string, JsonValue>; // FlatMap - better cache locality
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

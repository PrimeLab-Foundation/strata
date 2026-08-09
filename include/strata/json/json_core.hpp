#pragma once

/**
 * @file json_core.hpp
 * @brief Core JSON value model and the hot-path error types.
 *
 * Three things live here:
 *
 *  - @ref strata::FlatMap — the insertion-ordered, linear-scan map that backs
 *    JSON objects.
 *  - @ref strata::Status / @ref strata::Result — the status-code error model,
 *    so no tight loop pays for exceptions.
 *  - @ref strata::JsonValue — the DOM node, a variant over the six JSON types.
 *
 * Header-only, and free of any CPython dependency: this is core, so everything
 * that touches the C API stays in `src/strata/bindings/`
 * (docs/context/convention.md). Core files avoid even naming the CPython
 * header, so that grepping the core for it stays literally empty.
 */

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace strata {

/**
 * Vector-backed, insertion-ordered map sized for small JSON objects.
 *
 * JSON objects typically carry fewer than ~20 keys, where a linear scan across
 * one contiguous allocation beats a hash map on cache locality. Iteration
 * yields insertion order, which is what lets a document round-trip with its
 * keys in the order they were read.
 *
 * Complexity: lookup and keyed insertion are O(n); emplace() is O(1) amortised
 * but performs **no** duplicate check — see emplace().
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

    /// Insert-or-access by lvalue key; a new key is appended at the back.
    V& operator[](const K& key) {
        for (auto& entry : data_) {
            if (entry.first == key)
                return entry.second;
        }
        data_.emplace_back(key, V());
        return data_.back().second;
    }

    /// Insert-or-access by rvalue key (avoids copying the key on insert).
    V& operator[](K&& key) {
        for (auto& entry : data_) {
            if (entry.first == key)
                return entry.second;
        }
        data_.emplace_back(std::move(key), V());
        return data_.back().second;
    }

    /// First entry with @p key, or end() when absent.
    [[nodiscard]] iterator find(const K& key) {
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->first == key)
                return it;
        }
        return data_.end();
    }

    /// First entry with @p key, or end() when absent.
    [[nodiscard]] const_iterator find(const K& key) const {
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->first == key)
                return it;
        }
        return data_.end();
    }

    /// @throws std::out_of_range when @p key is absent.
    [[nodiscard]] V& at(const K& key) {
        auto it = find(key);
        if (it == data_.end())
            throw std::out_of_range("FlatMap::at");
        return it->second;
    }

    /// @throws std::out_of_range when @p key is absent.
    [[nodiscard]] const V& at(const K& key) const {
        auto it = find(key);
        if (it == data_.end())
            throw std::out_of_range("FlatMap::at");
        return it->second;
    }

    /**
     * Append a key/value pair unconditionally.
     *
     * **No duplicate check.** Unlike `std::map::emplace` this never inspects
     * the keys already present, so emplacing an existing key leaves the map
     * holding both entries and find()/at() keep returning the first. That is
     * deliberate: it keeps the hot path free of a scan the caller has usually
     * just done, and it is where the parser hangs its duplicate-key policy —
     * find() first, then either emplace() or assign through the iterator.
     */
    template <typename... Args> void emplace(Args&&... args) {
        data_.emplace_back(std::forward<Args>(args)...);
    }

    [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
    [[nodiscard]] bool empty() const noexcept { return data_.empty(); }
    void clear() noexcept { data_.clear(); }

  private:
    vector_type data_;
};

/// Outcome of an operation that does not throw. `Ok` is the zero value.
enum class Status { Ok, TypeMismatch, KeyNotFound, IndexOutOfBounds, ParseError };

/**
 * A value plus the status that produced it — the hot-path alternative to
 * throwing.
 *
 * Deliberately an aggregate, so call sites read `return {Status::Ok, v};` and
 * `return {Status::TypeMismatch, 0};`. Both members carry defaults, which means
 * a `Result` built without a value (`{Status::KeyNotFound}`) holds a
 * default-constructed T rather than an indeterminate one: reading `value` after
 * a failure is well-defined, if meaningless.
 *
 * Check ok() before trusting `value`. Move large payloads out directly —
 * `std::move(result.value)` — rather than copying them.
 *
 * @tparam T The value type on success.
 */
template <typename T> struct Result {
    Status status = Status::Ok;
    T value{};

    [[nodiscard]] constexpr bool ok() const noexcept { return status == Status::Ok; }
};

/**
 * One JSON value: null, bool, number, string, array or object.
 *
 * Numbers are IEEE-754 doubles throughout the C++ DOM, so integers beyond
 * 2^53 lose precision on this path. That is a property of the DOM, not of the
 * library: the Python builder consumes the same SAX events and keeps integers
 * exact (docs/architecture/SKILL.md).
 *
 * Objects use FlatMap, so key order survives a round trip.
 */
struct JsonValue {
    using Array = std::vector<JsonValue>;
    using Object = FlatMap<std::string, JsonValue>;
    using Number = double;

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

    // --- Accessors ---------------------------------------------------------
    // Each throws std::bad_variant_access on a type mismatch. Callers on hot
    // paths test the matching predicate first; the status-code API that wraps
    // these for the binding layer arrives with JsonCursor.
    [[nodiscard]] const bool& as_bool() const { return std::get<bool>(data); }
    [[nodiscard]] const Number& as_number() const { return std::get<Number>(data); }
    [[nodiscard]] const std::string& as_string() const { return std::get<std::string>(data); }
    [[nodiscard]] const Array& as_array() const { return std::get<Array>(data); }
    [[nodiscard]] const Object& as_object() const { return std::get<Object>(data); }

    [[nodiscard]] bool& as_bool() { return std::get<bool>(data); }
    [[nodiscard]] Number& as_number() { return std::get<Number>(data); }
    [[nodiscard]] std::string& as_string() { return std::get<std::string>(data); }
    [[nodiscard]] Array& as_array() { return std::get<Array>(data); }
    [[nodiscard]] Object& as_object() { return std::get<Object>(data); }
};

} // namespace strata

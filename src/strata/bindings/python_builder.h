#pragma once

/**
 * @file python_builder.h
 * @brief The one definition of "JSON events to Python objects".
 *
 * PythonObjectBuilder is the SAX handler behind `loads`, `load`, NDJSON and
 * the streaming-search capture path. It lives in a header because two
 * translation units instantiate the templated parser over it, and because
 * anything that builds Python values for the user -- including a captured
 * search match -- must apply the duplicate-key policy identically, or the
 * `search == query(load)` law breaks on exactly the documents nobody tests.
 *
 * The class is deliberately *not* derived from JsonSaxHandler: the parser is
 * a template, and instantiating it on this concrete type lets the compiler
 * inline every callback.
 */

#include "python_types.h"
#include "strata/json/json_parse.hpp"

#include <cstdint>
#include <cstring>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <vector>

namespace strata::bindings {

/**
 * Reuses the `PyObject*` for a key across every record that repeats it.
 *
 * Documents are overwhelmingly arrays of same-shaped records, so the same
 * handful of key strings recur once per record. Creating each one afresh means
 * a decode, an allocation and a hash on every occurrence; here the first
 * occurrence pays for all of them.
 *
 * Open addressing over a fixed table, FNV-1a over the raw bytes, and a short
 * probe budget: a key that cannot find a slot is simply created uncached, so a
 * pathological document degrades to the previous behaviour rather than growing
 * an unbounded table.
 */
class KeyCache {
  public:
    KeyCache() = default;
    KeyCache(const KeyCache&) = delete;
    KeyCache& operator=(const KeyCache&) = delete;

    ~KeyCache() {
        for (PyObject* key : slots_)
            Py_XDECREF(key);
    }

    /// A new reference to the interned key, or nullptr with an error set.
    [[nodiscard]] PyObject* intern(std::string_view text) {
        size_t slot = hash(text) & (kSlots - 1);
        for (size_t probe = 0; probe < kMaxProbes; ++probe) {
            PyObject*& candidate = slots_[slot];
            if (candidate == nullptr) {
                PyObject* created = make(text);
                if (created == nullptr)
                    return nullptr;
                candidate = Py_NewRef(created);
                return created;
            }
            if (matches(candidate, text))
                return Py_NewRef(candidate);
            slot = (slot + 1) & (kSlots - 1);
        }
        return make(text); // table crowded here: create without caching
    }

  private:
    static constexpr size_t kSlots = 512; // power of two, so the mask works
    static constexpr size_t kMaxProbes = 8;

    [[nodiscard]] static PyObject* make(std::string_view text) {
        return PyUnicode_FromStringAndSize(text.data(), static_cast<Py_ssize_t>(text.size()));
    }

    [[nodiscard]] static bool matches(PyObject* key, std::string_view text) {
        Py_ssize_t size = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(key, &size);
        if (utf8 == nullptr) {
            PyErr_Clear();
            return false;
        }
        return static_cast<size_t>(size) == text.size() &&
               std::memcmp(utf8, text.data(), text.size()) == 0;
    }

    [[nodiscard]] static size_t hash(std::string_view text) noexcept {
        size_t value = 1469598103934665603ULL; // FNV-1a offset basis
        for (const char c : text) {
            value ^= static_cast<unsigned char>(c);
            value *= 1099511628211ULL;
        }
        return value;
    }

    PyObject* slots_[kSlots] = {};
};

/**
 * Builds Python objects directly from parser events.
 *
 * Finished values accumulate in one flat `values_` vector; `frames_` records
 * where each open container started. An array is created once, at its closing
 * bracket, with its final size already known — so there is no repeated list
 * growth and no per-element refcount churn, because PyList_SET_ITEM steals the
 * references the vector was holding.
 *
 * Every callback returns false on failure with a Python exception already set,
 * which aborts the parse; loads() then reports that exception rather than
 * overwriting it with a generic parse error.
 */
class PythonObjectBuilder {
  public:
    ~PythonObjectBuilder() {
        reset();
        clear_predictions();
    }

    /**
     * Return to the empty state, releasing whatever an aborted parse left
     * behind. The KeyCache survives deliberately -- it is why reusing one
     * builder across calls pays -- and so does the prediction table, which
     * `begin_input` invalidates the moment the input buffer changes.
     */
    void reset() noexcept {
        Py_XDECREF(root_);
        root_ = nullptr;
        for (PyObject* value : values_)
            Py_XDECREF(value);
        values_.clear();
        for (const Frame& frame : frames_)
            Py_XDECREF(frame.mapping);
        frames_.clear();
        for (PyObject* key : keys_)
            Py_XDECREF(key);
        keys_.clear();
    }

    /**
     * Declare the buffer the next parse reads from.
     *
     * Only zero-copy keys are recordable, and "zero-copy" is detected by the
     * key view pointing into this range. The predictions themselves own their
     * bytes, so they survive across buffers -- an address alone could not be
     * trusted anyway, since a freed buffer's address can come back holding a
     * different document.
     */
    void begin_input(const char* begin, const char* end) noexcept {
        input_begin_ = begin;
        input_end_ = end;
    }

    /**
     * The parser's speculative-key hook (see has_try_match_key).
     *
     * Same-schema records repeat their keys in order, so the key most likely
     * to appear next is the one that appeared at this position, at this
     * depth, in the previous record. One memcmp against its raw bytes --
     * escapes, closing quote and all -- replaces the escape scan, the decode
     * and the cache lookup.
     */
    [[nodiscard]] size_t try_match_key(const char* after_quote, size_t remaining) {
        if (frames_.empty() || frames_.back().mapping == nullptr)
            return 0;
        Frame& frame = frames_.back();
        const size_t depth = frames_.size();
        if (depth >= predicted_.size())
            return 0;
        Schema& schema_slot = predicted_[depth];
        if (schema_slot.retired)
            return 0;
        auto& schema = schema_slot.keys;
        if (frame.cursor >= schema.size())
            return 0;
        const PredictedKey& entry = schema[frame.cursor];
        if (entry.raw.size() > remaining ||
            std::memcmp(after_quote, entry.raw.data(), entry.raw.size()) != 0)
            return 0;
        keys_.push_back(Py_NewRef(entry.object));
        ++frame.cursor;
        return entry.raw.size();
    }

    bool on_null() { return push(Py_NewRef(Py_None)); }

    bool on_bool(bool value) { return push(Py_NewRef(value ? Py_True : Py_False)); }

    bool on_int(int64_t value) { return push(PyLong_FromLongLong(value)); }

    /// Integers past int64 keep every digit: Python has no size limit.
    bool on_big_int(std::string_view text) {
        const std::string digits(text); // PyLong_FromString needs a terminator
        return push(PyLong_FromString(digits.c_str(), nullptr, 10));
    }

    bool on_double(double value) { return push(PyFloat_FromDouble(value)); }

    bool on_string(std::string_view value) {
        return push(
            PyUnicode_FromStringAndSize(value.data(), static_cast<Py_ssize_t>(value.size())));
    }

    bool on_start_object() {
        PyObject* mapping = PyDict_New();
        if (mapping == nullptr)
            return false;
        frames_.push_back(Frame{mapping, values_.size(), 0});
        return true;
    }

    /// The prediction missed (or there was none): intern normally, then teach
    /// the table this position so the next record can predict it.
    bool on_key(std::string_view key) {
        PyObject* object = keys_cache_.intern(key);
        if (object == nullptr)
            return false;
        keys_.push_back(object);
        record_key(key, object);
        return true;
    }

    bool on_end_object() {
        if (frames_.empty())
            return false;
        PyObject* mapping = frames_.back().mapping;
        frames_.pop_back();
        return push(mapping);
    }

    bool on_start_array() {
        frames_.push_back(Frame{nullptr, values_.size()});
        return true;
    }

    bool on_end_array() {
        if (frames_.empty() || frames_.back().mapping != nullptr)
            return false;
        const size_t start = frames_.back().start;
        frames_.pop_back();

        const size_t count = values_.size() - start;
        PyObject* list = PyList_New(static_cast<Py_ssize_t>(count));
        if (list == nullptr)
            return false;
        for (size_t index = 0; index < count; ++index) {
            // SET_ITEM steals the reference the vector was holding.
            PyList_SET_ITEM(list, static_cast<Py_ssize_t>(index), values_[start + index]);
        }
        values_.resize(start);
        return push(list);
    }

    /// Hand the finished tree to the caller.
    [[nodiscard]] PyObject* take_root() noexcept {
        PyObject* root = root_;
        root_ = nullptr;
        return root;
    }

  private:
    /// An open container: a dict, or an array holding from `start` in values_.
    struct Frame {
        PyObject* mapping;
        size_t start;
        uint32_t cursor; ///< next prediction slot for this object's depth
    };

    /// One remembered key: its raw bytes (through the closing quote) and its
    /// interned object. The bytes are owned -- typical keys sit in the small-
    /// string buffer -- so a prediction never dangles, whatever happened to
    /// the buffer it was learned from, and by construction the object's text
    /// is always those bytes minus the quote.
    struct PredictedKey {
        std::string raw;
        PyObject* object;
    };

    /// Depths beyond this predict nothing; documents this deep are not
    /// record-shaped and the table would only churn.
    static constexpr size_t kMaxPredictedDepth = 64;
    /// Keys per object worth remembering; wider objects stop recording.
    static constexpr size_t kMaxPredictedKeys = 64;
    /// Shape changes tolerated at one depth before it stops predicting.
    static constexpr uint32_t kMaxDivergences = 16;

    /// The prediction state of one nesting depth.
    struct Schema {
        std::vector<PredictedKey> keys;
        uint32_t divergences = 0;
        /// Set once the depth has proven shape-unstable: objects there keep
        /// disagreeing, so recording would only churn allocations -- measured
        /// as a 15% regression on a mixed-shape document before this guard.
        bool retired = false;
    };

    void clear_predictions() noexcept {
        for (Schema& schema : predicted_)
            for (PredictedKey& entry : schema.keys)
                Py_DECREF(entry.object);
        predicted_.clear();
        input_begin_ = nullptr;
        input_end_ = nullptr;
    }

    void record_key(std::string_view key, PyObject* object) {
        const size_t depth = frames_.size();
        if (depth == 0 || depth >= kMaxPredictedDepth || frames_.back().mapping == nullptr)
            return;
        // Only zero-copy keys are recordable: their view points into the
        // input, so the raw span is the key plus its closing quote. A key
        // that needed decoding lives in the parser's scratch buffer and has
        // no stable raw form to compare against.
        const char* begin = key.data();
        if (begin < input_begin_ || begin + key.size() >= input_end_)
            return;

        if (predicted_.size() <= depth)
            predicted_.resize(depth + 1);
        Schema& schema_slot = predicted_[depth];
        if (schema_slot.retired)
            return;
        auto& schema = schema_slot.keys;
        Frame& frame = frames_.back();
        if (frame.cursor >= kMaxPredictedKeys)
            return;

        // The shape diverged at this position: drop the stale tail. The next
        // record either follows the new shape (and now predicts it) or pays
        // one more miss here. A depth that keeps diverging is not
        // record-shaped at all; it retires rather than churning forever.
        if (frame.cursor < schema.size()) {
            if (++schema_slot.divergences > kMaxDivergences) {
                for (PredictedKey& entry : schema)
                    Py_DECREF(entry.object);
                schema.clear();
                schema_slot.retired = true;
                return;
            }
            for (size_t index = frame.cursor; index < schema.size(); ++index)
                Py_DECREF(schema[index].object);
            schema.resize(frame.cursor);
        }

        schema.push_back(PredictedKey{std::string(begin, key.size() + 1), Py_NewRef(object)});
        ++frame.cursor;
    }

    /// Consume one reference to @p value, placing it where it belongs.
    bool push(PyObject* value) {
        if (value == nullptr)
            return false;

        if (frames_.empty()) {
            Py_XDECREF(root_);
            root_ = value;
            return true;
        }
        if (frames_.back().mapping == nullptr) {
            values_.push_back(value); // the vector now owns it
            return true;
        }
        return insert_into_object(frames_.back().mapping, value);
    }

    bool insert_into_object(PyObject* object, PyObject* value) {
        if (keys_.empty()) {
            Py_DECREF(value);
            return false;
        }
        PyRef key(keys_.back());
        keys_.pop_back();

        // The overwhelmingly common case -- a key seen once, under the default
        // policy -- must cost exactly one dict lookup. SetDefault inserts when
        // absent and returns the existing entry when present, which is both
        // the FirstWins semantics and the duplicate probe in one operation;
        // the Contains-then-SetItem shape this replaces paid two lookups on
        // every single member.
        const DuplicateKeyPolicy policy = get_duplicate_key_policy();
        if (policy == DuplicateKeyPolicy::FirstWins) {
            PyObject* settled = PyDict_SetDefault(object, key.get(), value);
            Py_DECREF(value);
            return settled != nullptr;
        }
        if (policy == DuplicateKeyPolicy::LastWins) {
            // Overwriting *is* the policy, so no duplicate probe is needed.
            const int failed = PyDict_SetItem(object, key.get(), value);
            Py_DECREF(value);
            return failed == 0;
        }

        // Warn and Error both need to know a duplicate happened.
        const int present = PyDict_Contains(object, key.get());
        if (present < 0) {
            Py_DECREF(value);
            return false;
        }

        if (present == 0) {
            const int failed = PyDict_SetItem(object, key.get(), value);
            Py_DECREF(value);
            return failed == 0;
        }

        // The key is already there; the policy decides what that means.
        switch (get_duplicate_key_policy()) {
        case DuplicateKeyPolicy::FirstWins:
            Py_DECREF(value);
            return true;
        case DuplicateKeyPolicy::LastWins: {
            const int failed = PyDict_SetItem(object, key.get(), value);
            Py_DECREF(value);
            return failed == 0;
        }
        case DuplicateKeyPolicy::Warn: {
            Py_DECREF(value);
            // A warning filter set to "error" turns this into a raised
            // exception, which aborts the parse rather than being swallowed.
            return PyErr_WarnFormat(PyExc_RuntimeWarning, 1, "Duplicate key encountered: %U",
                                    key.get()) == 0;
        }
        case DuplicateKeyPolicy::Error:
            Py_DECREF(value);
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return false;
        }

        Py_DECREF(value);
        return false;
    }

    PyObject* root_ = nullptr;
    std::vector<Frame> frames_;
    std::vector<PyObject*> values_;
    std::vector<PyObject*> keys_;
    std::vector<Schema> predicted_; ///< indexed by depth
    const char* input_begin_ = nullptr;
    const char* input_end_ = nullptr;
    KeyCache keys_cache_;
};



} // namespace strata::bindings

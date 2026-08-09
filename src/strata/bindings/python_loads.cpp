/**
 * @file python_loads.cpp
 * @brief `strata.loads` — JSON text straight to Python objects.
 *
 * PythonObjectBuilder is a SAX handler that builds the final Python tree as
 * the parser walks the input, so there is no intermediate C++ DOM to convert
 * afterwards. It is deliberately *not* derived from JsonSaxHandler: the parser
 * is a template, and instantiating it on this concrete type lets the compiler
 * inline every callback.
 *
 * This is the correctness-first build. The key cache, speculative key
 * matching, small-int cache and presized dicts described in
 * docs/bindings/SKILL.md are performance work and land with that milestone.
 */

#include "python_types.h"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_parser_inline.hpp"

#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

namespace strata::bindings {

namespace {

/**
 * Builds Python objects directly from parser events.
 *
 * Containers under construction live on `stack_`; each finished value is added
 * to the container on top, or becomes the root when the stack is empty. Keys
 * wait on `keys_` between on_key() and the value that follows.
 *
 * Every callback returns false on failure with a Python exception already set,
 * which aborts the parse; loads() then reports that exception rather than
 * overwriting it with a generic parse error.
 */
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
        Py_XDECREF(root_);
        for (PyObject* value : values_)
            Py_XDECREF(value);
        for (const Frame& frame : frames_)
            Py_XDECREF(frame.mapping);
        for (PyObject* key : keys_)
            Py_XDECREF(key);
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
        frames_.push_back(Frame{mapping, values_.size()});
        return true;
    }

    /// Keys come from the cache, so a repeated key costs one comparison.
    bool on_key(std::string_view key) {
        PyObject* object = keys_cache_.intern(key);
        if (object == nullptr)
            return false;
        keys_.push_back(object);
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
    };

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
    KeyCache keys_cache_;
};

} // namespace

PyObject* loads_to_python(std::string_view text, bool validate_utf8) {
    PythonObjectBuilder builder;
    Status status;
    {
        GcPause pause;
        status = parse_sax_inline(text, builder, validate_utf8);
    }

    if (status != Status::Ok) {
        // A callback that failed has already described the problem precisely;
        // only a genuine syntax error needs the generic message.
        if (!PyErr_Occurred())
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        return nullptr;
    }

    PyObject* root = builder.take_root();
    if (root == nullptr && !PyErr_Occurred())
        PyErr_SetString(PyExc_ValueError, "Invalid JSON");
    return root;
}

PyObject* make_root_iterator(PyObject* value) {
    // A mapping iterates as pairs, a sequence as elements, and anything else
    // has nothing to iterate -- the flag is simply ignored there.
    if (PyDict_Check(value)) {
        PyRef items(PyObject_CallMethod(value, "items", nullptr));
        if (!items)
            return nullptr;
        return PyObject_GetIter(items.get());
    }
    if (PyList_Check(value))
        return PyObject_GetIter(value);
    return Py_NewRef(value);
}

} // namespace strata::bindings

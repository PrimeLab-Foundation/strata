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
class PythonObjectBuilder {
  public:
    ~PythonObjectBuilder() {
        Py_XDECREF(root_);
        for (PyObject* container : stack_)
            Py_XDECREF(container);
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

    bool on_string(std::string_view value) { return push(make_string(value)); }

    bool on_start_object() { return open(PyDict_New()); }

    bool on_key(std::string_view key) {
        PyObject* object = make_string(key);
        if (object == nullptr)
            return false;
        keys_.push_back(object);
        return true;
    }

    bool on_end_object() { return close(); }

    bool on_start_array() { return open(PyList_New(0)); }

    bool on_end_array() { return close(); }

    /// Hand the finished tree to the caller.
    [[nodiscard]] PyObject* take_root() noexcept {
        PyObject* root = root_;
        root_ = nullptr;
        return root;
    }

  private:
    /// PyUnicode validates UTF-8 as it decodes, so invalid bytes surface here.
    [[nodiscard]] static PyObject* make_string(std::string_view text) {
        return PyUnicode_FromStringAndSize(text.data(), static_cast<Py_ssize_t>(text.size()));
    }

    bool open(PyObject* container) {
        if (container == nullptr)
            return false;
        stack_.push_back(container);
        return true;
    }

    bool close() {
        if (stack_.empty())
            return false;
        PyObject* container = stack_.back();
        stack_.pop_back();
        return push(container); // push takes the reference
    }

    /// Consume one reference to @p value, placing it where it belongs.
    bool push(PyObject* value) {
        if (value == nullptr)
            return false;

        if (stack_.empty()) {
            Py_XDECREF(root_);
            root_ = value;
            return true;
        }

        PyObject* container = stack_.back();
        if (PyList_CheckExact(container)) {
            const int failed = PyList_Append(container, value);
            Py_DECREF(value);
            return failed == 0;
        }
        return insert_into_object(container, value);
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
    std::vector<PyObject*> stack_;
    std::vector<PyObject*> keys_;
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

} // namespace strata::bindings

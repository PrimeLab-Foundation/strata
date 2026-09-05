/**
 * @file python_loads.cpp
 * @brief `strata.loads` — JSON text straight to Python objects.
 *
 * The builder itself lives in python_builder.h (shared with the streaming
 * search capture path); this file owns the thread-reuse lease and the module
 * entry points.
 */

#include "python_builder.h"
#include "python_types.h"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_parser_inline.hpp"

#include <cstdint>
#include <cstring>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <vector>

namespace strata::bindings {

namespace {

/**
 * Lease of the one builder this thread reuses across loads() calls.
 *
 * Reuse is what lets the KeyCache and the prediction table pay across calls
 * and across NDJSON lines instead of warming up once per document. The shared
 * instance is deliberately leaked: a thread_local with a destructor may run
 * after interpreter shutdown, when Py_DECREF is no longer legal.
 *
 * Re-entrancy: a duplicate-key warning can run arbitrary Python (a warning
 * filter), which can call loads() again mid-parse. The nested call finds the
 * shared builder busy and quietly pays for a private one instead.
 */
class BuilderLease {
  public:
    BuilderLease() {
        if (!busy_ && shared() != nullptr) {
            busy_ = true;
            builder_ = shared();
            owns_flag_ = true;
        } else {
            fallback_ = std::make_unique<PythonObjectBuilder>();
            builder_ = fallback_.get();
        }
    }

    ~BuilderLease() {
        builder_->reset();
        if (owns_flag_)
            busy_ = false;
    }

    BuilderLease(const BuilderLease&) = delete;
    BuilderLease& operator=(const BuilderLease&) = delete;

    [[nodiscard]] PythonObjectBuilder& operator*() noexcept { return *builder_; }
    [[nodiscard]] PythonObjectBuilder* operator->() noexcept { return builder_; }

  private:
    [[nodiscard]] static PythonObjectBuilder* shared() {
        static thread_local PythonObjectBuilder* instance =
            new (std::nothrow) PythonObjectBuilder();
        return instance;
    }

    static thread_local bool busy_;

    PythonObjectBuilder* builder_;
    std::unique_ptr<PythonObjectBuilder> fallback_;
    bool owns_flag_ = false;
};

thread_local bool BuilderLease::busy_ = false;

} // namespace

PyObject* loads_to_python(std::string_view text, bool validate_utf8, Status* status_out) {
    BuilderLease builder;
    builder->begin_input(text.data(), text.data() + text.size());
    Status status;
    {
        GcPause pause;
        status = parse_sax_inline(text, *builder, validate_utf8);
    }
    if (status_out != nullptr)
        *status_out = status;

    if (status == Status::DepthExceeded) {
        // A refusal, not a malformed document, and raised by the parser rather
        // than by a callback -- so nothing else has described it and the
        // pinned message is set here (docs/context/api.md, error contract).
        PyErr_SetString(PyExc_ValueError, kDepthExceededMessage);
        return nullptr;
    }

    if (status != Status::Ok) {
        // A callback that failed has already described the problem precisely;
        // only a genuine syntax error needs the generic message. Invalid
        // UTF-8 inside a string reaches here as the decoder's error -- the
        // builder's ASCII path admits only bytes below 0x80 and every other
        // string goes through CPython's strict decoder, which is why no pass
        // over the input precedes the parse -- and it is malformed input,
        // reported as the contract's ValueError, never as a codec error.
        if (PyErr_Occurred() && PyErr_ExceptionMatches(PyExc_UnicodeDecodeError))
            PyErr_Clear();
        if (!PyErr_Occurred())
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        return nullptr;
    }

    PyObject* root = builder->take_root();
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

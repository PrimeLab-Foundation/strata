#pragma once

/**
 * @file python_types.h
 * @brief Shared plumbing for the CPython binding layer.
 *
 * Everything here exists to keep the exported functions honest at the
 * C++/Python boundary: owning pointers that cannot leak, a GC pause for bulk
 * object construction, and a translation of C++ exceptions into Python ones so
 * nothing escapes into the interpreter unhandled.
 *
 * Binding-only. The core never includes this file, and never includes the
 * CPython API at all (docs/context/convention.md, "Core purity").
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <cstddef>
#include <exception>
#include <string_view>

namespace strata::bindings {

// --- Entry points shared between the binding translation units -------------
// Declared here rather than redeclared `extern` at each use site.

/// Starting capacity for a dumps() output buffer.
inline constexpr size_t kDumpsInitialCapacity = 1024;

/// Parse @p text into a new Python object tree, or nullptr with an error set.
[[nodiscard]] PyObject* loads_to_python(std::string_view text, bool validate_utf8);

/// Serialize @p object to JSON as `str`, or as `bytes` when @p as_bytes.
[[nodiscard]] PyObject* dumps_to_python(PyObject* object, bool as_bytes);

/// What dumps() does when a container contains itself.
enum class CyclePolicyValue { Warn, Error, Ignore };

[[nodiscard]] CyclePolicyValue get_cycle_policy() noexcept;

/// Apply a policy by name; false if @p name is not one of warn/error/ignore.
bool set_cycle_policy(std::string_view name) noexcept;

/// Owning reference; releases on scope exit unless release() takes it.
class PyRef {
  public:
    PyRef() noexcept = default;
    explicit PyRef(PyObject* object) noexcept : object_(object) {}

    ~PyRef() { Py_XDECREF(object_); }

    PyRef(const PyRef&) = delete;
    PyRef& operator=(const PyRef&) = delete;

    PyRef(PyRef&& other) noexcept : object_(other.object_) { other.object_ = nullptr; }
    PyRef& operator=(PyRef&& other) noexcept {
        if (this != &other) {
            Py_XDECREF(object_);
            object_ = other.object_;
            other.object_ = nullptr;
        }
        return *this;
    }

    [[nodiscard]] PyObject* get() const noexcept { return object_; }
    [[nodiscard]] explicit operator bool() const noexcept { return object_ != nullptr; }

    /// Hand ownership to the caller.
    [[nodiscard]] PyObject* release() noexcept {
        PyObject* released = object_;
        object_ = nullptr;
        return released;
    }

  private:
    PyObject* object_ = nullptr;
};

/**
 * Suspend cyclic garbage collection for the duration of a bulk build.
 *
 * Parsing a large document creates a great many containers at once; leaving
 * the collector enabled makes it walk the half-built tree repeatedly. The GIL
 * is held throughout, so no other thread observes the pause.
 */
class GcPause {
  public:
    GcPause() noexcept : was_enabled_(PyGC_IsEnabled()) {
        if (was_enabled_)
            PyGC_Disable();
    }
    ~GcPause() {
        if (was_enabled_)
            PyGC_Enable();
    }

    GcPause(const GcPause&) = delete;
    GcPause& operator=(const GcPause&) = delete;

  private:
    int was_enabled_;
};

} // namespace strata::bindings

/**
 * Wrap the body of an exported function so no C++ exception crosses into the
 * interpreter. An exception that arrives with no Python error already set
 * becomes RuntimeError, matching the documented error contract.
 */
#define STRATA_CPP_TRY try {

#define STRATA_CPP_CATCH                                                                           \
    }                                                                                              \
    catch (const std::exception& error) {                                                          \
        if (!PyErr_Occurred())                                                                     \
            PyErr_SetString(PyExc_RuntimeError, error.what());                                     \
        return nullptr;                                                                            \
    }                                                                                              \
    catch (...) {                                                                                  \
        if (!PyErr_Occurred())                                                                     \
            PyErr_SetString(PyExc_RuntimeError, "Unknown C++ exception");                          \
        return nullptr;                                                                            \
    }

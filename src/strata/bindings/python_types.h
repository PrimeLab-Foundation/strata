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
#include "strata/json/json_core.hpp"

#include <Python.h>
#include <cstddef>
#include <exception>
#include <memory>
#include <string>
#include <string_view>

namespace strata {
struct JsonValue;
}

namespace strata::bindings {

// --- Entry points shared between the binding translation units -------------
// Declared here rather than redeclared `extern` at each use site.

/// Starting capacity for a dumps() output buffer.
inline constexpr size_t kDumpsInitialCapacity = 1024;

/// Parse @p text into a new Python object tree, or nullptr with an error set.
///
/// @param status_out When given, receives the parse's status — the one caller
///        that needs it is NDJSON, whose messages name the failing line and so
///        must re-raise rather than pass the exception through.
[[nodiscard]] PyObject* loads_to_python(std::string_view text, bool validate_utf8,
                                        Status* status_out = nullptr);

/// The one spelling of the depth refusal, shared by every entry point so the
/// message is a contract and not a per-file string (docs/context/api.md).
inline constexpr const char* kDepthExceededMessage = "Maximum nesting depth exceeded";

/// Serialize @p object to JSON as `str`, or as `bytes` when @p as_bytes.
[[nodiscard]] PyObject* dumps_to_python(PyObject* object, bool as_bytes);

/// Resolve everything the serializer would otherwise resolve lazily *inside*
/// a walk. Call once, from module init: the raw-dict layout proof allocates
/// dicts, and a GC-tracked allocation under a walk can run a finalizer where
/// the walk's enumeration of user-code steps says none can run
/// (python_dumps.cpp's header). Never fails; a probe that cannot run leaves
/// the raw walk off for the process.
void prepare_dumps_runtime() noexcept;

/// Register the JsonCursor type on @p module. False with an error set on failure.
bool register_cursor_type(PyObject* module);

/// A cursor at the root of a tree, sharing ownership of it.
[[nodiscard]] PyObject* make_root_cursor(const std::shared_ptr<const JsonValue>& owner);

/// Register the lazy NDJSON iterator type. False with an error set on failure.
bool register_ndjson_iterator_type(PyObject* module);

/// Every record of @p text as a list, raising unless @p skip_errors.
[[nodiscard]] PyObject* ndjson_to_list(std::string_view text, bool skip_errors);

/// A lazy iterator that takes ownership of @p text and parses per line.
[[nodiscard]] PyObject* make_ndjson_iterator(std::string&& text, bool skip_errors);

/// An iterator over a parsed root: dict yields (key, value), list yields
/// elements, and a scalar is returned unchanged (docs/context/api.md).
[[nodiscard]] PyObject* make_root_iterator(PyObject* value);

/// `load(path, ...)` in file mode.
[[nodiscard]] PyObject* load_from_file(const char* path, const char* return_type, bool iterator,
                                       bool skip_errors);

/// `dump(obj, path)` in file mode.
[[nodiscard]] PyObject* dump_to_file(PyObject* object, const char* path);

/// Register the CompiledPath type. False with an error set on failure.
bool register_jsonpath_types(PyObject* module);

/// Compile @p expression (str or CompiledPath) into a CompiledPath object.
[[nodiscard]] PyObject* compile_expression(PyObject* expression);

/// Evaluate @p expression over Python objects.
[[nodiscard]] PyObject* query_object(PyObject* data, PyObject* expression);

/// The tree node a JsonCursor points at, or nullptr if not a cursor.
[[nodiscard]] const JsonValue* cursor_value(PyObject* object);

/// A cursor at @p value, sharing the tree that @p object holds.
[[nodiscard]] PyObject* cursor_child(PyObject* object, const JsonValue* value);

/// `search(path, expression)` over one file.
[[nodiscard]] PyObject* search_file(const char* path, PyObject* expression);

/// Whether @p path names an NDJSON file by extension (case-insensitive).
[[nodiscard]] bool file_is_ndjson(const char* path);

/// Read a whole file, mapping failures onto the documented exceptions.
[[nodiscard]] bool read_file_to_string(const char* path, std::string& out);

/// `load(dirpath, ...)` in folder mode.
[[nodiscard]] PyObject* load_from_folder(const char* directory, bool iterator, bool skip_errors);

/// `dump(records, dirpath, split_by=...)` in folder mode.
[[nodiscard]] PyObject* dump_to_folder(PyObject* records, const char* directory,
                                       PyObject* split_by);

/// `search(dirpath, expression)` in folder mode.
[[nodiscard]] PyObject* search_folder(const char* directory, PyObject* expression, bool iterator);

/// Register the lazy folder iterator type.
bool register_folder_iterator_type(PyObject* module);

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

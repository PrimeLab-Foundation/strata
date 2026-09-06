/**
 * @file python_module.cpp
 * @brief Entry point of the `strata._strata` extension module.
 *
 * Holds the module definition, the method table, and the configuration store.
 * The heavy lifting lives in python_loads.cpp and python_dumps.cpp; what is
 * here is argument handling and the mapping from C++ outcomes to Python
 * exceptions.
 *
 * `Python.h` is confined to `src/strata/bindings/` — the C++ core never
 * includes it (docs/context/convention.md, "Core purity").
 */

#include "python_types.h"
#include "strata/json/json_document.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/util/folder.hpp"

#include <cstring>
#include <string>
#include <string_view>

namespace {

using strata::bindings::CyclePolicyValue;

// ---------------------------------------------------------------------------
// Configuration
//
// The map itself is process-global. What each setting *reaches* differs, and
// the difference is part of the documented contract: the duplicate-key policy
// is consumed through a thread-local in the core, so setting it affects only
// the calling thread, while the cycle policy is a plain global that every
// thread sees (docs/context/api.md, "Config").
// ---------------------------------------------------------------------------

constexpr const char* kDuplicateKeyPolicy = "duplicate_key_policy";
constexpr const char* kCyclePolicy = "cycle_policy";

/// Current duplicate-key policy for *this* thread, as a name.
[[nodiscard]] const char* duplicate_key_policy_name() {
    switch (strata::get_duplicate_key_policy()) {
    case strata::DuplicateKeyPolicy::FirstWins:
        return "first";
    case strata::DuplicateKeyPolicy::LastWins:
        return "last";
    case strata::DuplicateKeyPolicy::Error:
        return "error";
    case strata::DuplicateKeyPolicy::Warn:
        return "warn";
    }
    return "first";
}

[[nodiscard]] const char* cycle_policy_name() {
    switch (strata::bindings::get_cycle_policy()) {
    case CyclePolicyValue::Warn:
        return "warn";
    case CyclePolicyValue::Error:
        return "error";
    case CyclePolicyValue::Ignore:
        return "ignore";
    }
    return "warn";
}

/// Apply a duplicate-key policy by name; false if the name is not one of ours.
bool apply_duplicate_key_policy(std::string_view name) {
    if (name == "first") {
        strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::FirstWins);
    } else if (name == "last") {
        strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::LastWins);
    } else if (name == "error") {
        strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::Error);
    } else if (name == "warn") {
        strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::Warn);
    } else {
        return false;
    }
    return true;
}

PyObject* strata_config_set(PyObject* /*self*/, PyObject* args) {
    STRATA_CPP_TRY
    const char* key = nullptr;
    PyObject* value = nullptr;
    if (!PyArg_ParseTuple(args, "sO", &key, &value))
        return nullptr;

    const bool is_duplicate_key = std::strcmp(key, kDuplicateKeyPolicy) == 0;
    const bool is_cycle = std::strcmp(key, kCyclePolicy) == 0;
    if (!is_duplicate_key && !is_cycle) {
        PyErr_Format(PyExc_KeyError, "unknown config key: %s", key);
        return nullptr;
    }

    if (!PyUnicode_Check(value)) {
        PyErr_Format(PyExc_TypeError, "%s must be a string", key);
        return nullptr;
    }
    Py_ssize_t size = 0;
    const char* name = PyUnicode_AsUTF8AndSize(value, &size);
    if (name == nullptr)
        return nullptr;
    const std::string_view policy(name, static_cast<size_t>(size));

    if (is_duplicate_key) {
        if (!apply_duplicate_key_policy(policy)) {
            PyErr_SetString(PyExc_ValueError, "duplicate_key_policy must be first|last|error|warn");
            return nullptr;
        }
    } else if (!strata::bindings::set_cycle_policy(policy)) {
        PyErr_SetString(PyExc_ValueError, "cycle_policy must be warn|error|ignore");
        return nullptr;
    }

    Py_RETURN_NONE;
    STRATA_CPP_CATCH
}

PyObject* strata_config_get(PyObject* /*self*/, PyObject* args) {
    STRATA_CPP_TRY
    const char* key = nullptr;
    if (!PyArg_ParseTuple(args, "s", &key))
        return nullptr;

    // Read straight from the policies themselves, so what is reported and what
    // is in force cannot drift apart.
    if (std::strcmp(key, kDuplicateKeyPolicy) == 0)
        return PyUnicode_FromString(duplicate_key_policy_name());
    if (std::strcmp(key, kCyclePolicy) == 0)
        return PyUnicode_FromString(cycle_policy_name());

    Py_RETURN_NONE; // unknown keys read as None; only set() raises
    STRATA_CPP_CATCH
}

PyObject* strata_config_list(PyObject* /*self*/, PyObject* /*ignored*/) {
    STRATA_CPP_TRY
    strata::bindings::PyRef mapping(PyDict_New());
    if (!mapping)
        return nullptr;

    const struct {
        const char* key;
        const char* value;
    } entries[] = {
        {kDuplicateKeyPolicy, duplicate_key_policy_name()},
        {kCyclePolicy, cycle_policy_name()},
    };

    for (const auto& entry : entries) {
        strata::bindings::PyRef value(PyUnicode_FromString(entry.value));
        if (!value)
            return nullptr;
        if (PyDict_SetItemString(mapping.get(), entry.key, value.get()) != 0)
            return nullptr;
    }
    return mapping.release();
    STRATA_CPP_CATCH
}

// ---------------------------------------------------------------------------
// loads / dumps
// ---------------------------------------------------------------------------

/// Turn parsed text into whatever shape the caller asked for.
PyObject* finish_loads(std::string_view text, bool validate_utf8, bool want_cursor, bool iterator) {
    if (want_cursor) {
        auto document = strata::JsonDocument::from_string(text);
        if (!document.ok()) {
            // The C++ document path is capped by the same constant as the
            // Python builder path, and reports it with the same message.
            PyErr_SetString(PyExc_ValueError, document.status == strata::Status::DepthExceeded
                                                  ? strata::bindings::kDepthExceededMessage
                                                  : "Invalid JSON");
            return nullptr;
        }
        return strata::bindings::make_root_cursor(document.value.share());
    }

    strata::bindings::PyRef value(strata::bindings::loads_to_python(text, validate_utf8));
    if (!value)
        return nullptr;
    if (!iterator)
        return value.release();
    return strata::bindings::make_root_iterator(value.get());
}

/// Keyword-only string option from a FASTCALL kwnames tuple, by exact name.
[[nodiscard]] const char* fastcall_str_option(PyObject* name, PyObject* value) {
    if (!PyUnicode_Check(value)) {
        PyErr_Format(PyExc_TypeError, "%U must be str, not %s", name, Py_TYPE(value)->tp_name);
        return nullptr;
    }
    return PyUnicode_AsUTF8(value);
}

// loads and dumps use METH_FASTCALL: they are called once per benchmark-row
// operation and often with tiny documents, where VARARGS' argument tuple and
// PyArg_ParseTupleAndKeywords' format-string machinery are a measurable slice
// of the per-call floor. The hand parse mirrors the old signature exactly —
// one positional argument, keyword-only options.
PyObject* strata_loads(PyObject* /*self*/, PyObject* const* args, Py_ssize_t nargs,
                       PyObject* kwnames) {
    STRATA_CPP_TRY
    if (nargs != 1) {
        PyErr_Format(PyExc_TypeError, "loads() takes exactly 1 positional argument (%zd given)",
                     nargs);
        return nullptr;
    }
    PyObject* source = args[0];
    const char* return_type = "dict";
    int iterator = 0;
    if (kwnames != nullptr) {
        for (Py_ssize_t index = 0; index < PyTuple_GET_SIZE(kwnames); ++index) {
            PyObject* name = PyTuple_GET_ITEM(kwnames, index);
            PyObject* value = args[nargs + index];
            if (PyUnicode_CompareWithASCIIString(name, "return_type") == 0) {
                return_type = fastcall_str_option(name, value);
                if (return_type == nullptr)
                    return nullptr;
            } else if (PyUnicode_CompareWithASCIIString(name, "iterator") == 0) {
                iterator = PyObject_IsTrue(value);
                if (iterator < 0)
                    return nullptr;
            } else {
                PyErr_Format(PyExc_TypeError, "loads() got an unexpected keyword argument '%U'",
                             name);
                return nullptr;
            }
        }
    }

    const bool want_cursor = std::strcmp(return_type, "cursor") == 0;
    if (!want_cursor && std::strcmp(return_type, "dict") != 0) {
        PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
        return nullptr;
    }

    if (PyUnicode_Check(source)) {
        Py_ssize_t size = 0;
        const char* text = PyUnicode_AsUTF8AndSize(source, &size);
        if (text == nullptr) {
            // A str holding lone surrogates cannot be encoded; that is not
            // valid JSON text either way.
            PyErr_Clear();
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return nullptr;
        }
        // Already valid UTF-8 by construction, so the parser need not re-check.
        return finish_loads(std::string_view(text, static_cast<size_t>(size)),
                            /*validate_utf8=*/false, want_cursor, iterator != 0);
    }

    if (PyBytes_Check(source)) {
        char* data = nullptr;
        Py_ssize_t size = 0;
        if (PyBytes_AsStringAndSize(source, &data, &size) != 0)
            return nullptr;
        // For bytes the parser is the only validator there is.
        // No pass over the bytes ahead of the parse: the builder admits only
        // ASCII on its zero-copy path and hands every other string to
        // CPython's strict decoder, and the grammar rejects a high byte
        // anywhere else -- so invalid UTF-8 is caught where it lies, once,
        // and loads_to_python reports it as the contract's ValueError. The
        // cursor path builds the C++ document, which validates for itself.
        return finish_loads(std::string_view(data, static_cast<size_t>(size)),
                            /*validate_utf8=*/false, want_cursor, iterator != 0);
    }

    PyErr_Format(PyExc_TypeError, "loads() expects str or bytes, not %s", Py_TYPE(source)->tp_name);
    return nullptr;
    STRATA_CPP_CATCH
}

PyObject* strata_dumps(PyObject* /*self*/, PyObject* const* args, Py_ssize_t nargs,
                       PyObject* kwnames) {
    STRATA_CPP_TRY
    if (nargs != 1) {
        PyErr_Format(PyExc_TypeError, "dumps() takes exactly 1 positional argument (%zd given)",
                     nargs);
        return nullptr;
    }
    PyObject* object = args[0];
    const char* return_type = "str";
    if (kwnames != nullptr) {
        for (Py_ssize_t index = 0; index < PyTuple_GET_SIZE(kwnames); ++index) {
            PyObject* name = PyTuple_GET_ITEM(kwnames, index);
            if (PyUnicode_CompareWithASCIIString(name, "return_type") != 0) {
                PyErr_Format(PyExc_TypeError, "dumps() got an unexpected keyword argument '%U'",
                             name);
                return nullptr;
            }
            return_type = fastcall_str_option(name, args[nargs + index]);
            if (return_type == nullptr)
                return nullptr;
        }
    }

    const bool as_bytes = std::strcmp(return_type, "bytes") == 0;
    if (!as_bytes && std::strcmp(return_type, "str") != 0) {
        PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
        return nullptr;
    }

    return strata::bindings::dumps_to_python(object, as_bytes);
    STRATA_CPP_CATCH
}

/// CPython's documented spelling for METH_KEYWORDS and METH_FASTCALL function
/// pointers: the table stores PyCFunction, and the call site casts back by
/// the method flags.
#define STRATA_KEYWORD_FN(fn) reinterpret_cast<PyCFunction>(reinterpret_cast<void (*)()>(fn))

PyObject* strata_load(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    static const char* keywords[] = {"", "return_type", "iterator", "skip_errors", nullptr};
    const char* path = nullptr;
    const char* return_type = "dict";
    int iterator = 0;
    int skip_errors = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|$spp", const_cast<char**>(keywords), &path,
                                     &return_type, &iterator, &skip_errors))
        return nullptr;

    if (strata::util::is_directory(path)) {
        if (std::strcmp(return_type, "cursor") == 0) {
            PyErr_SetString(PyExc_ValueError,
                            "return_type=\"cursor\" is not supported for a directory");
            return nullptr;
        }
        if (std::strcmp(return_type, "dict") != 0) {
            PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
            return nullptr;
        }
        return strata::bindings::load_from_folder(path, iterator != 0, skip_errors != 0);
    }
    return strata::bindings::load_from_file(path, return_type, iterator != 0, skip_errors != 0);
    STRATA_CPP_CATCH
}

PyObject* strata_dump(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    static const char* keywords[] = {"", "", "split_by", nullptr};
    PyObject* object = nullptr;
    const char* path = nullptr;
    PyObject* split_by = Py_None;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Os|$O", const_cast<char**>(keywords), &object,
                                     &path, &split_by))
        return nullptr;

    // An existing directory is folder mode. A path that does not exist yet is
    // folder mode too when split_by says so -- api.md has dump create
    // directories as needed, so the target need not exist first.
    const bool to_directory = strata::util::is_directory(path) ||
                              (split_by != Py_None && !strata::util::path_exists(path));
    if (to_directory) {
        if (split_by == Py_None) {
            PyErr_SetString(PyExc_ValueError, "a directory target requires split_by");
            return nullptr;
        }
        return strata::bindings::dump_to_folder(object, path, split_by);
    }
    if (split_by != Py_None) {
        // split_by only means something for a directory; silently writing one
        // file instead would lose data the caller expected to be split.
        PyErr_SetString(PyExc_ValueError, "split_by requires a directory target");
        return nullptr;
    }
    return strata::bindings::dump_to_file(object, path);
    STRATA_CPP_CATCH
}

PyObject* strata_compile(PyObject* /*self*/, PyObject* args) {
    STRATA_CPP_TRY
    PyObject* expression = nullptr;
    if (!PyArg_ParseTuple(args, "O", &expression))
        return nullptr;
    return strata::bindings::compile_expression(expression);
    STRATA_CPP_CATCH
}

PyObject* strata_query(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    static const char* keywords[] = {"", "", "iterator", nullptr};
    PyObject* data = nullptr;
    PyObject* expression = nullptr;
    int iterator = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|$p", const_cast<char**>(keywords), &data,
                                     &expression, &iterator))
        return nullptr;

    strata::bindings::PyRef matches(strata::bindings::query_object(data, expression));
    if (!matches)
        return nullptr;
    // Eager evaluate, lazy consume: the flag changes how results are handed
    // back, not when the work happens (docs/context/api.md).
    return iterator ? PyObject_GetIter(matches.get()) : matches.release();
    STRATA_CPP_CATCH
}

PyObject* strata_search(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    static const char* keywords[] = {"", "", "iterator", nullptr};
    const char* path = nullptr;
    PyObject* expression = nullptr;
    int iterator = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sO|$p", const_cast<char**>(keywords), &path,
                                     &expression, &iterator))
        return nullptr;

    strata::bindings::PyRef matches(
        strata::util::is_directory(path)
            ? strata::bindings::search_folder(path, expression, iterator != 0)
            : strata::bindings::search_file(path, expression));
    if (!matches)
        return nullptr;
    if (strata::util::is_directory(path))
        return matches.release(); // already an iterator when one was asked for
    return iterator ? PyObject_GetIter(matches.get()) : matches.release();
    STRATA_CPP_CATCH
}

PyMethodDef kModuleMethods[] = {
    {"loads", STRATA_KEYWORD_FN(strata_loads), METH_FASTCALL | METH_KEYWORDS,
     "loads(source, *, return_type='dict', iterator=False)\n\nParse JSON text."},
    {"dumps", STRATA_KEYWORD_FN(strata_dumps), METH_FASTCALL | METH_KEYWORDS,
     "dumps(obj, *, return_type='str')\n\nSerialize an object to JSON."},
    {"load", STRATA_KEYWORD_FN(strata_load), METH_VARARGS | METH_KEYWORDS,
     "load(path, *, return_type='dict', iterator=False, skip_errors=False)"},
    {"dump", STRATA_KEYWORD_FN(strata_dump), METH_VARARGS | METH_KEYWORDS,
     "dump(obj, path, *, split_by=None)"},
    {"compile", strata_compile, METH_VARARGS, "compile(expression) -> CompiledPath"},
    {"query", STRATA_KEYWORD_FN(strata_query), METH_VARARGS | METH_KEYWORDS,
     "query(data, expression, *, iterator=False) -> list"},
    {"search", STRATA_KEYWORD_FN(strata_search), METH_VARARGS | METH_KEYWORDS,
     "search(path, expression, *, iterator=False) -> list"},
    {"config_set", strata_config_set, METH_VARARGS, "config_set(key, value)\n\nSet a setting."},
    {"config_get", strata_config_get, METH_VARARGS, "config_get(key)\n\nRead a setting."},
    {"config_list", strata_config_list, METH_NOARGS, "config_list()\n\nAll settings."},
    {nullptr, nullptr, 0, nullptr},
};

PyModuleDef kModuleDef = {
    PyModuleDef_HEAD_INIT,
    "strata._strata",
    "Strata native engine: C++20 JSON parsing, serialization and search.",
    -1, // m_size: module state lives in file-static storage
    kModuleMethods,
    nullptr, // m_slots
    nullptr, // m_traverse
    nullptr, // m_clear
    nullptr, // m_free
};

} // namespace

PyMODINIT_FUNC PyInit__strata(void) {
    // Seed both policies to their documented defaults before anything can
    // observe them, so a fresh process reports exactly what it will do.
    strata::set_duplicate_key_policy(strata::DuplicateKeyPolicy::FirstWins);
    strata::bindings::set_cycle_policy("warn");
    // Resolve the serializer's one-time runtime probe here rather than on the
    // first dumps(): it allocates, and allocating inside the walk can run a
    // finalizer at a point the walk's contract says runs no user code.
    strata::bindings::prepare_dumps_runtime();

    PyObject* module = PyModule_Create(&kModuleDef);
    if (module == nullptr)
        return nullptr;
    if (!strata::bindings::register_cursor_type(module) ||
        !strata::bindings::register_ndjson_iterator_type(module) ||
        !strata::bindings::register_jsonpath_types(module) ||
        !strata::bindings::register_folder_iterator_type(module)) {
        Py_DECREF(module);
        return nullptr;
    }
    return module;
}

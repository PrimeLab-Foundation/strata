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
#include "strata/json/json_parse.hpp"

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

PyObject* strata_loads(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    static const char* keywords[] = {"", "return_type", "iterator", nullptr};
    PyObject* source = nullptr;
    const char* return_type = "dict";
    int iterator = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|$sp", const_cast<char**>(keywords), &source,
                                     &return_type, &iterator))
        return nullptr;

    if (std::strcmp(return_type, "cursor") == 0) {
        PyErr_SetString(PyExc_NotImplementedError, "return_type=\"cursor\" is not implemented yet");
        return nullptr;
    }
    if (std::strcmp(return_type, "dict") != 0) {
        PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
        return nullptr;
    }
    if (iterator) {
        PyErr_SetString(PyExc_NotImplementedError, "iterator=True is not implemented yet");
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
        return strata::bindings::loads_to_python(std::string_view(text, static_cast<size_t>(size)),
                                                 /*validate_utf8=*/false);
    }

    if (PyBytes_Check(source)) {
        char* data = nullptr;
        Py_ssize_t size = 0;
        if (PyBytes_AsStringAndSize(source, &data, &size) != 0)
            return nullptr;
        // For bytes the parser is the only validator there is.
        return strata::bindings::loads_to_python(std::string_view(data, static_cast<size_t>(size)),
                                                 /*validate_utf8=*/true);
    }

    PyErr_Format(PyExc_TypeError, "loads() expects str or bytes, not %s", Py_TYPE(source)->tp_name);
    return nullptr;
    STRATA_CPP_CATCH
}

PyObject* strata_dumps(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    static const char* keywords[] = {"", "return_type", nullptr};
    PyObject* object = nullptr;
    const char* return_type = "str";

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|$s", const_cast<char**>(keywords), &object,
                                     &return_type))
        return nullptr;

    const bool as_bytes = std::strcmp(return_type, "bytes") == 0;
    if (!as_bytes && std::strcmp(return_type, "str") != 0) {
        PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
        return nullptr;
    }

    return strata::bindings::dumps_to_python(object, as_bytes);
    STRATA_CPP_CATCH
}

/// CPython's documented spelling for a METH_KEYWORDS function pointer: the
/// table stores PyCFunction, and the call site casts back by flag.
#define STRATA_KEYWORD_FN(fn) reinterpret_cast<PyCFunction>(reinterpret_cast<void (*)()>(fn))

PyMethodDef kModuleMethods[] = {
    {"loads", STRATA_KEYWORD_FN(strata_loads), METH_VARARGS | METH_KEYWORDS,
     "loads(source, *, return_type='dict', iterator=False)\n\nParse JSON text."},
    {"dumps", STRATA_KEYWORD_FN(strata_dumps), METH_VARARGS | METH_KEYWORDS,
     "dumps(obj, *, return_type='str')\n\nSerialize an object to JSON."},
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

    return PyModule_Create(&kModuleDef);
}

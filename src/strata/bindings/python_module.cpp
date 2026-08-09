/**
 * @file python_module.cpp
 * @brief Entry point of the `strata._strata` extension module.
 *
 * Holds the module definition and the method table; parse, serialize and
 * search entry points are registered here as they are implemented. This is
 * the only translation unit allowed to define the module object.
 *
 * `Python.h` is confined to `src/strata/bindings/` — the C++ core never
 * includes it (docs/context/convention.md, "Core purity").
 */

#include <Python.h>

namespace {

/// Module-level functions. Empty until the binding milestones register theirs.
PyMethodDef kModuleMethods[] = {
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

PyMODINIT_FUNC PyInit__strata(void) { return PyModule_Create(&kModuleDef); }

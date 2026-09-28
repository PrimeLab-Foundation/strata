/**
 * @file python_dumps_hook.cpp
 * @brief `strata._dumps_hook` — `dumps_with_default`, the serializer with the
 * unsupported-type hook, in an extension image of its own.
 *
 * Design: docs/architecture/dumps_with_default.md. The M12 hook inside `dumps`
 * was refused by its own kill criterion: a null test on the writer's tail and
 * two words of walker state were enough to move `dumps flat` on linux-x86_64
 * across two implementations (docs/performance/experiment-ledger.md, M12). So
 * the hook does not live in `_strata` at all. This translation unit compiles
 * the one serializer source, `python_dumps.cpp`, a second time with
 * `STRATA_DUMPS_HOOK` defined — every writer keeps one definition — into a
 * second image, while `_strata` builds that file to the same token stream as
 * before the hook existed (M12b criterion 4, checked by
 * build/evidence/benchmark-lead/M12b/zero-diff/).
 *
 * What the two images share is `_strata`'s cycle policy, which `config.set`
 * writes there; this image reads it where a cycle is found, through
 * `strata._strata.config_get` and after latching the walk (the read can run a
 * collection), because exporting the variable would change `_strata`. Everything else is per image:
 * the raw-dict layout proofs (resolved at this module's init, before any walk, as `_strata`
 * resolves its own), the thread's schema cache lease and the output buffer.
 */

#define STRATA_DUMPS_HOOK 1

#include "python_types.h"

namespace strata::bindings {
namespace {

/// `_strata`'s cycle policy, read at the cycle point (defined below).
[[nodiscard]] CyclePolicyValue hook_cycle_policy() noexcept;

} // namespace
} // namespace strata::bindings

#include "python_dumps.cpp"

namespace strata::bindings {
namespace {

/// `strata._strata.config_get` and the interned key it is asked about, held
/// for the life of the process (module init takes them; nothing releases a
/// module's statics).
PyObject* g_config_get = nullptr;
PyObject* g_policy_key = nullptr;

/**
 * `_strata`'s cycle policy, asked of `_strata` itself.
 *
 * Reached only from the walk's two cold cycle handlers, through
 * `STRATA_CYCLE_POLICY`, which **latches the walk first**. The latch is not
 * optional: `config_get` is a `METH_VARARGS` builtin, so the call allocates an
 * argument tuple the collector tracks, and on CPython 3.10/3.11 that
 * allocation can run a collection -- `gc.callbacks`, finalizers, weakref
 * callbacks: user code -- right here. Unlatched, such code could clear the
 * dict being written while the walk still borrowed its row (a use-after-free,
 * pinned by tests/py/test_dumps_with_default.py). The unraisable hook below
 * runs user code too, and runs inside the same latched window. It is read per
 * cycle rather than per call because that is what `dumps` does: a policy the
 * callable changes mid-walk applies to the cycles found after it. If the read
 * fails the process default applies, and the failure is reported, not dropped.
 */
STRATA_COLD_FN CyclePolicyValue hook_cycle_policy() noexcept {
    const PyRef name(PyObject_CallOneArg(g_config_get, g_policy_key));
    if (name && PyUnicode_Check(name.get())) {
        if (PyUnicode_CompareWithASCIIString(name.get(), "error") == 0)
            return CyclePolicyValue::Error;
        if (PyUnicode_CompareWithASCIIString(name.get(), "ignore") == 0)
            return CyclePolicyValue::Ignore;
        if (PyUnicode_CompareWithASCIIString(name.get(), "warn") == 0)
            return CyclePolicyValue::Warn;
    }
    if (PyErr_Occurred())
        PyErr_WriteUnraisable(g_config_get);
    return CyclePolicyValue::Warn;
}

/**
 * `dumps_with_default(obj, default, *, return_type="str")`.
 *
 * `default` is required, positional or keyword, and must be callable — `None`
 * is refused, not "absent": this entry point exists only to run a hook
 * (docs/decisions.md, 2026-09-26). The refusal is raised before any byte is
 * produced.
 */
PyObject* hook_dumps_with_default(PyObject* /*self*/, PyObject* const* args, Py_ssize_t nargs,
                                  PyObject* kwnames) {
    STRATA_CPP_TRY
    if (nargs < 1 || nargs > 2) {
        PyErr_Format(PyExc_TypeError,
                     "dumps_with_default() takes 1 or 2 positional arguments (%zd given)", nargs);
        return nullptr;
    }
    PyObject* const object = args[0];
    PyObject* default_fn = nargs == 2 ? args[1] : nullptr;
    const char* return_type = "str";
    if (kwnames != nullptr) {
        for (Py_ssize_t index = 0; index < PyTuple_GET_SIZE(kwnames); ++index) {
            PyObject* const name = PyTuple_GET_ITEM(kwnames, index);
            PyObject* const value = args[nargs + index];
            if (PyUnicode_CompareWithASCIIString(name, "return_type") == 0) {
                if (!PyUnicode_Check(value)) {
                    PyErr_Format(PyExc_TypeError, "%U must be str, not %s", name,
                                 Py_TYPE(value)->tp_name);
                    return nullptr;
                }
                return_type = PyUnicode_AsUTF8(value);
                if (return_type == nullptr)
                    return nullptr;
            } else if (PyUnicode_CompareWithASCIIString(name, "default") == 0) {
                if (default_fn != nullptr) {
                    PyErr_SetString(PyExc_TypeError,
                                    "dumps_with_default() got multiple values for argument "
                                    "'default'");
                    return nullptr;
                }
                default_fn = value;
            } else {
                PyErr_Format(PyExc_TypeError,
                             "dumps_with_default() got an unexpected keyword argument '%U'", name);
                return nullptr;
            }
        }
    }
    if (default_fn == nullptr) {
        PyErr_SetString(PyExc_TypeError,
                        "dumps_with_default() missing required argument 'default'");
        return nullptr;
    }
    if (!PyCallable_Check(default_fn)) {
        PyErr_Format(PyExc_TypeError, "default must be callable, not %s",
                     Py_TYPE(default_fn)->tp_name);
        return nullptr;
    }
    const bool as_bytes = std::strcmp(return_type, "bytes") == 0;
    if (!as_bytes && std::strcmp(return_type, "str") != 0) {
        PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
        return nullptr;
    }
    return dumps_with_default_to_python(object, as_bytes, default_fn);
    STRATA_CPP_CATCH
}

PyMethodDef kHookMethods[] = {
    {"dumps_with_default",
     reinterpret_cast<PyCFunction>(reinterpret_cast<void (*)()>(hook_dumps_with_default)),
     METH_FASTCALL | METH_KEYWORDS,
     "dumps_with_default(obj, default, *, return_type='str')\n\n"
     "Serialize an object to JSON, calling default for each unsupported object."},
    {nullptr, nullptr, 0, nullptr},
};

PyModuleDef kHookModuleDef = {
    PyModuleDef_HEAD_INIT,
    "strata._dumps_hook",
    "strata's serializer with the unsupported-type hook (dumps_with_default).",
    -1,
    kHookMethods,
};

} // namespace
} // namespace strata::bindings

PyMODINIT_FUNC PyInit__dumps_hook(void) {
    using namespace strata::bindings;
    // Before any walk, as `_strata` does at its own init: the layout proofs
    // are this image's own copies, and resolving one mid-walk allocates where
    // the walk's contract says nothing runs.
    prepare_dumps_runtime();
    // This image's own copy of the native type table and its names.
    if (!native::prepare_native_runtime())
        return nullptr;

    const PyRef strata_module(PyImport_ImportModule("strata._strata"));
    if (!strata_module)
        return nullptr;
    PyRef config_get(PyObject_GetAttrString(strata_module.get(), "config_get"));
    if (!config_get)
        return nullptr;
    PyRef key(PyUnicode_InternFromString("cycle_policy"));
    if (!key)
        return nullptr;

    PyObject* module = PyModule_Create(&kHookModuleDef);
    if (module == nullptr)
        return nullptr;
    // Single-phase init (m_size -1): CPython runs this once per runtime and
    // hands a later subinterpreter a copy of the module's dict without calling
    // it again, so these statics are set once. A runtime finalized and
    // initialized again (embedding) does run it again; whatever the statics
    // held then belonged to the finalized runtime and is overwritten, never
    // released.
    g_config_get = config_get.release();
    g_policy_key = key.release();
    return module;
}

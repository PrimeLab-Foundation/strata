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

#include "python_parse_types.h"
#include "python_types.h"
#include "strata/util/folder.hpp"

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

/// Keyword-only string option from a FASTCALL kwnames tuple, by exact name --
/// the same shape as `_strata`'s `fastcall_str_option` (python_module.cpp),
/// duplicated here since that translation unit is not linked into this image.
[[nodiscard]] const char* hook_fastcall_str_option(PyObject* name, PyObject* value) {
    if (!PyUnicode_Check(value)) {
        PyErr_Format(PyExc_TypeError, "%U must be str, not %s", name, Py_TYPE(value)->tp_name);
        return nullptr;
    }
    return PyUnicode_AsUTF8(value);
}

/**
 * `dumps_native(obj, *, return_type="str")` -- `dumps` with every native type
 * family (docs/architecture/native_types.md, "Flag shape (M15b)") and no
 * `default`: `dumps_to_python` above arms the walker with no callable, so an
 * unsupported object raises main's exact
 * `TypeError("Object of type %s is not JSON serializable")`
 * (`write_unsupported` with `default_ == nullptr`). Argument parsing mirrors
 * `_strata.dumps` exactly (python_module.cpp, `strata_dumps`), so its errors
 * read the same but for the function's own name.
 */
PyObject* hook_dumps_native(PyObject* /*self*/, PyObject* const* args, Py_ssize_t nargs,
                            PyObject* kwnames) {
    STRATA_CPP_TRY
    if (nargs != 1) {
        PyErr_Format(PyExc_TypeError,
                     "dumps_native() takes exactly 1 positional argument (%zd given)", nargs);
        return nullptr;
    }
    PyObject* object = args[0];
    const char* return_type = "str";
    if (kwnames != nullptr) {
        for (Py_ssize_t index = 0; index < PyTuple_GET_SIZE(kwnames); ++index) {
            PyObject* name = PyTuple_GET_ITEM(kwnames, index);
            if (PyUnicode_CompareWithASCIIString(name, "return_type") != 0) {
                PyErr_Format(PyExc_TypeError,
                             "dumps_native() got an unexpected keyword argument '%U'", name);
                return nullptr;
            }
            return_type = hook_fastcall_str_option(name, args[nargs + index]);
            if (return_type == nullptr)
                return nullptr;
        }
    }

    const bool as_bytes = std::strcmp(return_type, "bytes") == 0;
    if (!as_bytes && std::strcmp(return_type, "str") != 0) {
        PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
        return nullptr;
    }

    return dumps_to_python(object, as_bytes);
    STRATA_CPP_CATCH
}

/**
 * `dump_native(obj, path, *, split_by=None)` -- `dump` with every native type
 * family. Repeats `strata_dump`'s dispatch exactly (python_module.cpp,
 * `strata_dump`: `split_by` picks folder mode, a directory target with no
 * `split_by` is the documented `ValueError`), against this image's own
 * `dump_to_file`/`dump_to_folder` (python_files.cpp, python_folder.cpp,
 * whose writer halves are compiled into this image too).
 */
PyObject* hook_dump_native(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    static const char* keywords[] = {"", "", "split_by", nullptr};
    PyObject* object = nullptr;
    const char* path = nullptr;
    PyObject* split_by = Py_None;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Os|$O", const_cast<char**>(keywords), &object,
                                     &path, &split_by))
        return nullptr;

    if (split_by != Py_None) {
        if (strata::util::is_directory(path) || !strata::util::path_exists(path))
            return strata::bindings::dump_to_folder(object, path, split_by);
        PyErr_SetString(PyExc_ValueError, "split_by requires a directory target");
        return nullptr;
    }

    PyObject* written = strata::bindings::dump_to_file(object, path);
    if (written == nullptr && strata::util::is_directory(path)) {
        PyErr_Clear();
        PyErr_SetString(PyExc_ValueError, "a directory target requires split_by");
    }
    return written;
    STRATA_CPP_CATCH
}

/// CPython's documented spelling for METH_KEYWORDS function pointers: the
/// table stores PyCFunction, and the call site casts back by the method
/// flags. A direct PyCFunction cast of an incompatible function-pointer type
/// warns (-Wcast-function-type-mismatch); this two-step cast, through a
/// function pointer of no fixed signature, is the documented workaround --
/// the same shape as `_strata`'s own `STRATA_KEYWORD_FN` (python_module.cpp),
/// duplicated here since that translation unit is not linked into this image.
#define STRATA_HOOK_KEYWORD_FN(fn) reinterpret_cast<PyCFunction>(reinterpret_cast<void (*)()>(fn))

PyMethodDef kHookMethods[] = {
    {"dumps_with_default", STRATA_HOOK_KEYWORD_FN(hook_dumps_with_default),
     METH_FASTCALL | METH_KEYWORDS,
     "dumps_with_default(obj, default, *, return_type='str')\n\n"
     "Serialize an object to JSON, calling default for each unsupported object."},
    {"dumps_native", STRATA_HOOK_KEYWORD_FN(hook_dumps_native), METH_FASTCALL | METH_KEYWORDS,
     "dumps_native(obj, *, return_type='str')\n\n"
     "Serialize an object to JSON, including every native type family."},
    {"dump_native", STRATA_HOOK_KEYWORD_FN(hook_dump_native), METH_VARARGS | METH_KEYWORDS,
     "dump_native(obj, path, *, split_by=None)\n\n"
     "Write an object as JSON to a file, or a directory of files with split_by."},
    {"loads_typed", STRATA_HOOK_KEYWORD_FN(strata::bindings::parse_types::loads_typed),
     METH_FASTCALL | METH_KEYWORDS,
     "loads_typed(source, *, return_type='dict', iterator=False, parse_types)\n\n"
     "loads() with parse_types set."},
    {"load_typed", STRATA_HOOK_KEYWORD_FN(strata::bindings::parse_types::load_typed),
     METH_VARARGS | METH_KEYWORDS,
     "load_typed(path, *, return_type='dict', iterator=False, skip_errors=False, parse_types)\n\n"
     "load() with parse_types set."},
    {"search_typed", STRATA_HOOK_KEYWORD_FN(strata::bindings::parse_types::search_typed),
     METH_VARARGS | METH_KEYWORDS,
     "search_typed(path, expression, *, iterator=False, parse_types)\n\n"
     "search() with parse_types set."},
    {"query_typed", STRATA_HOOK_KEYWORD_FN(strata::bindings::parse_types::query_typed),
     METH_VARARGS | METH_KEYWORDS,
     "query_typed(data, expression, *, iterator=False, parse_types)\n\n"
     "query() with parse_types set."},
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
    // A runtime finalized and reinitialized (embedding) runs this function
    // again; the parse walk's own statics (datetime's C API, uuid.UUID) belong
    // to the finalized runtime and must be re-resolved, not reused.
    parse_types::reset_runtime();

    const PyRef strata_module(PyImport_ImportModule("strata._strata"));
    if (!strata_module)
        return nullptr;
    PyRef config_get(PyObject_GetAttrString(strata_module.get(), "config_get"));
    if (!config_get)
        return nullptr;
    PyRef key(PyUnicode_InternFromString("cycle_policy"));
    if (!key)
        return nullptr;
    // `parse_types`'s four entry points parse through these, never through a
    // parser of this image's own (docs/architecture/native_types.md, "Flag
    // shape (M15b)"). A real `strata._strata` always provides them; this can
    // only fail against a minimal stand-in (tests/unit/test_dumps_with_default_state.py
    // loads this image against a fake `_strata` with `config_get` alone, to
    // drive `dumps_with_default` on its own), which must still load the hook
    // for `dumps_with_default`'s sake -- so a missing entry here is not fatal
    // to the module, and surfaces instead from the first `parse_types` call.
    if (!parse_types::prepare_runtime(strata_module.get()))
        PyErr_Clear();

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

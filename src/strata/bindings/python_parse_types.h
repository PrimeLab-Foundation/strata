#pragma once

/**
 * @file python_parse_types.h
 * @brief `parse_types`: the opt-in revival of a freshly parsed tree.
 *
 * Design: docs/architecture/native_types.md ("Parse side", "Registry shape",
 * "Parse contract", "Flag shape (M15b)"). `strata._dumps_hook` only: the
 * facade routes `loads`/`load`/`search`/`query` to `_strata`'s own entry when
 * `parse_types` is `False`, and to the four functions below otherwise. Each
 * one parses through `_strata`'s own public `loads`/`load`/`query`/`compile`
 * -- resolved once, by prepare_runtime(), from module init -- and then hands
 * the tree to a separate walk (python_parse_types_walk.cpp) that replaces, in
 * place, the strings that name a date, time, date-time or UUID, and the
 * members a caller's registry names. This translation unit links no parser of
 * its own, so `duplicate_key_policy` (a thread-local of `_strata`) is honoured
 * because `_strata` is what parses.
 *
 * Each entry point validates its own arguments -- with the record's messages,
 * and the shape `_strata`'s same-named entry uses -- then the `parse_types`
 * keyword, before touching its input.
 */

#include "python_types.h"

namespace strata::bindings::parse_types {

/// Resolve `_strata`'s public `loads`/`load`/`query`/`compile` from
/// @p strata_module, held for the process. False with an error set on
/// failure. Call once, from `PyInit__dumps_hook`, before any entry point below
/// runs.
[[nodiscard]] bool prepare_runtime(PyObject* strata_module);

/// Adopt the exception `prepare_runtime` left set (module init only, on
/// failure): stored so the first entry point call that finds the runtime
/// unready chains it as that call's `__cause__` instead of losing it to
/// `PyErr_Clear()`. Consumes the currently set exception.
void adopt_prepare_failure() noexcept;

/// `loads_typed(source, *, return_type="dict", iterator=False, parse_types)`.
[[nodiscard]] PyObject* loads_typed(PyObject* self, PyObject* const* args, Py_ssize_t nargs,
                                    PyObject* kwnames);

/// `load_typed(path, *, return_type="dict", iterator=False, skip_errors=False, parse_types)`.
[[nodiscard]] PyObject* load_typed(PyObject* self, PyObject* args, PyObject* kwargs);

/// `search_typed(path, expression, *, iterator=False, parse_types)`.
[[nodiscard]] PyObject* search_typed(PyObject* self, PyObject* args, PyObject* kwargs);

/// `query_typed(data, expression, *, iterator=False, parse_types)`.
[[nodiscard]] PyObject* query_typed(PyObject* self, PyObject* args, PyObject* kwargs);

/// Forget what the first call resolved. Called from hook module init: after a
/// runtime restart the references belong to the finalized runtime.
void reset_runtime() noexcept;

} // namespace strata::bindings::parse_types

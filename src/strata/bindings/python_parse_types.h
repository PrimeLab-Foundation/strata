#pragma once

/**
 * @file python_parse_types.h
 * @brief `parse_types`: the opt-in revival of a freshly parsed tree.
 *
 * Design: docs/architecture/native_types.md ("Parse side", "Registry shape",
 * "Parse contract"). `_strata` only.
 *
 * The option never reaches the parser or the builder. Each entry point below
 * parses exactly as its default path does -- through the same functions --
 * and then hands the tree to a separate walk that replaces, in place, the
 * strings that name a date, time, date-time or UUID, and the members a
 * caller's registry names. The entry points in python_module.cpp call these
 * functions only when the keyword is present and is not `False`, so a default
 * call reaches nothing in this file; each is out of line and cold.
 *
 * Each function takes the keyword's raw value (anything but `False`) and
 * validates it first, with the record's messages, before touching its input.
 * The first valid call imports `datetime` and `uuid` if they are not already
 * imported -- the only imports strata makes on its own behalf.
 */

#include "python_types.h"

namespace strata::bindings::parse_types {

/// `loads(source, parse_types=option)`: parse, revive, then hand back the tree
/// or an iterator over its root.
[[nodiscard]] STRATA_COLD_FN PyObject* loads(PyObject* source, bool want_cursor, bool iterator,
                                             PyObject* option);

/// `load(path, parse_types=option)`: a file, NDJSON line by line (eager or
/// lazy) or a folder record by record, each revived before it is returned.
[[nodiscard]] STRATA_COLD_FN PyObject* load(const char* path, const char* return_type,
                                            bool iterator, bool skip_errors, PyObject* option);

/// `search(path, expression, parse_types=option)`: always the full-parse
/// path, so the result is `query(load(path, parse_types=option), expression)`.
[[nodiscard]] STRATA_COLD_FN PyObject* search(const char* path, PyObject* expression, bool iterator,
                                              PyObject* option);

/// `query(data, expression, parse_types=option)`: @p option must be a `bool`;
/// `True` replaces each `str` match by the recognition rule, in the result
/// list only.
[[nodiscard]] STRATA_COLD_FN PyObject* query(PyObject* data, PyObject* expression, bool iterator,
                                             PyObject* option);

/// Forget what the first call resolved. Called from module init: after a
/// runtime restart the references belong to the finalized runtime.
void reset_runtime() noexcept;

} // namespace strata::bindings::parse_types

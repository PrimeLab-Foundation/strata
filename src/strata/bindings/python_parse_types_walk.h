#pragma once

/**
 * @file python_parse_types_walk.h
 * @brief Internal interface between the parse-side revival's two translation
 * units (docs/architecture/native_types.md, "Parse side").
 *
 * python_parse_types_walk.cpp owns the process-global runtime resolved on the
 * first call (datetime's C API, `uuid.UUID`), the recognition of a date, time,
 * date-time or UUID string, and the post-order revival walk.
 * python_parse_types.cpp owns the option and its private registry, the four
 * cold entry points and the lazy iterator, and reaches the walk only through
 * the three functions declared here. `_strata` only.
 */

#include "python_types.h"

namespace strata::bindings::parse_types {

/// Import `datetime`'s C API and `uuid.UUID` if this is the first call. False
/// with an error set when an import fails.
[[nodiscard]] bool ensure_runtime();

/// The date, time, date-time or UUID @p text names, else @p text itself: a new
/// reference, or nullptr with an error set. Requires a prior ensure_runtime().
[[nodiscard]] PyObject* recognize(PyObject* text);

/// Revive a freshly parsed root against @p registry (may be null). Consumes
/// @p root; a new reference or nullptr.
[[nodiscard]] PyObject* revive(PyObject* root, PyObject* registry);

} // namespace strata::bindings::parse_types

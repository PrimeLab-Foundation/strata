#pragma once

/**
 * @file python_numpy_twins.h
 * @brief The runtime proof behind numpy's `item()` twins
 * (docs/architecture/native_types.md, "Frames, cycles and depth", numpy).
 *
 * `numpy_kind` in python_native_types.cpp admits numpy's own `bool_`, sized
 * integers, `float32` and `float16` by their type numbers and reads them
 * through truth, `__index__` and `__float__` instead of `item()`. Those numbers
 * are the running numpy's, not this build's: each image proves them once, when
 * numpy resolves, and reads every numpy scalar through `item()` -- identical
 * output -- when the proof fails (convention, "Portable by construction").
 * Compiled into both images, beside python_native_types.cpp.
 */

#include "python_types.h"

namespace strata::bindings::native {

/// numpy's type numbers (`dtype.num`, `NPY_TYPES`: equal in numpy 1.x and 2.x)
/// of the scalars whose `item()` has an exact twin. `longdouble` (13), whose
/// `item()` returns itself, is not among them. Used only once
/// numpy_twins_hold() has held.
inline constexpr long kNumpyBool = 0;         // NPY_BOOL
inline constexpr long kNumpyFirstInteger = 1; // NPY_BYTE
inline constexpr long kNumpyLastInteger = 10; // NPY_ULONGLONG
inline constexpr long kNumpyFloat32 = 11;     // NPY_FLOAT
inline constexpr long kNumpyFloat16 = 23;     // NPY_HALF

/**
 * The proof, against the resolved @p numpy module, for every number above and
 * each once: for the type characters `? b B h H i I l L q Q f e`, the type
 * `numpy.dtype(code).type` names, called with a probe (`True`, the integer
 * type's extreme -- its minimum when signed --, or `0.1`), builds a scalar of
 * exactly that type whose own dtype has the row's number, kind (`b i u f`), C
 * item size and that type; and its twin, through numpy_plain(), is of the type
 * and value its `item()` returns. Runs numpy's Python code: latched caller.
 * False, possibly with an error set, at the first row that does not hold.
 */
[[nodiscard]] bool numpy_twins_hold(PyObject* numpy);

} // namespace strata::bindings::native

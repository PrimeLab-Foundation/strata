#pragma once

/**
 * @file python_native_types.h
 * @brief The serializer's native types: the lazily resolved type table and the
 * conversions from a Python object to text or to plain values.
 *
 * Design: docs/architecture/native_types.md ("Serializer", "Re-entrancy").
 * Compiled into both images; the writers that use it (`write_native` and its
 * per-kind members) stay in python_dumps.cpp, inside `Serializer`. The serializer's
 * uses of the `datetime` C API are all in this header's translation unit.
 *
 * Nothing here imports a module. The type table is read from `sys.modules`
 * when a walk first meets an object `write()` has no branch for, so
 * `import strata` imports none of `datetime`, `uuid`, `decimal`,
 * `dataclasses` or `numpy`; a module imported later is found by the next
 * lookup, and a type once found is kept for the life of the process.
 *
 * Two kinds of function, and the difference is the serializer's re-entrancy
 * contract (python_dumps.cpp's header):
 *
 *  - `format_pure_leaf` is **pure**: it calls nothing the user wrote and
 *    allocates nothing the collector tracks, so the walk calls it without
 *    latching;
 *  - everything else can run Python -- resolution reads attributes of modules
 *    the user may have replaced, and the conversions call `utcoffset()`,
 *    `str()`, `.value`, `dataclasses.fields`, `item()`/`tolist()` and read a
 *    dtype -- so the caller has latched the walk and holds a strong reference
 *    to @p object before calling any of them.
 */

#include "python_types.h"

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace strata::bindings::native {

/// Buffer size for the text a native leaf formats: a date-time with a fraction
/// and an offset is 32 bytes, a UUID 36.
inline constexpr size_t kTextCapacity = 48;

/// What `classify` found, in the record's precedence order.
enum class Kind : uint8_t {
    None,  ///< not a native type: the unsupported-type sink applies
    Error, ///< classification raised; the exception is set
    DateTime,
    Date,
    Time,
    Uuid,
    Decimal,
    Enum,
    Dataclass,
    Set,
    NumpyScalar,
    NumpyArray,
};

/**
 * Intern the attribute and module names the conversions use. Call once, from
 * the module init of each image, before any walk. False with an error set.
 */
[[nodiscard]] bool prepare_native_runtime() noexcept;

/**
 * Format @p object into @p out (kTextCapacity bytes) when it is a pure leaf:
 * an exact `datetime`/`date`/`time` whose `tzinfo` is `None` or exactly
 * `datetime.timezone`, or an exact `uuid.UUID` with an in-range `int` slot,
 * and the type table already holds its type.
 *
 * @return the number of bytes written, or 0 when @p object is not a pure leaf
 *         (the caller then takes the latched path). Never raises.
 */
[[nodiscard]] size_t format_pure_leaf(PyObject* object, char* out) noexcept;

/**
 * Resolve what is still unresolved of the type table, then classify @p object
 * by the record's precedence: datetime, date, time, UUID, Decimal, Enum,
 * dataclass, set/frozenset, numpy. Latched caller. A numpy object classifies
 * as native only for dtype kinds `b i u f`.
 */
[[nodiscard]] Kind classify(PyObject* object);

/// True when @p object is a numpy scalar or array (resolved types only). Pure.
[[nodiscard]] bool is_numpy(PyObject* object) noexcept;

/**
 * The text of a `datetime`, `date` or `time` (@p kind), subclasses and any
 * `tzinfo` included, into @p out (kTextCapacity bytes). Latched caller.
 * @return bytes written, or -1 with an error set.
 */
[[nodiscard]] Py_ssize_t format_temporal(PyObject* object, Kind kind, char* out);

/// The text of a `uuid.UUID` (subclasses included) from its `int`, into
/// @p out. Latched caller. @return bytes written, or -1 with an error set.
[[nodiscard]] Py_ssize_t format_uuid(PyObject* object, char* out);

/// What a `Decimal`'s `str()` turned out to be.
enum class DecimalText : uint8_t { Number, NonFinite, Error };

/**
 * `str(@p object)` for a `Decimal`: @p text receives the string (owned), and
 * @p view its UTF-8 when the result is `Number`. `NaN`, `sNaN` and
 * `±Infinity` spellings are `NonFinite`; anything else raises the record's
 * `ValueError`. Latched caller.
 */
[[nodiscard]] DecimalText decimal_text(PyObject* object, PyRef& text, std::string_view& view);

/// `member.value`, a new reference, or nullptr with an error set. Latched caller.
[[nodiscard]] PyObject* enum_value(PyObject* member);

/**
 * The names `dataclasses.fields(type(@p object))` lists, in order, as a new
 * reference to a tuple of `str`. Cached per type (at most kFieldCacheLimit
 * types; a full cache is cleared). Latched caller; nullptr with an error set.
 */
[[nodiscard]] PyObject* dataclass_field_names(PyObject* object);

/// Types the dataclass field-name cache holds before it is cleared.
inline constexpr Py_ssize_t kFieldCacheLimit = 1024;

/// `getattr(@p object, @p name)`, a new reference. Latched caller.
[[nodiscard]] PyObject* field_value(PyObject* object, PyObject* name);

/// `item()` of a numpy scalar or `tolist()` of an array (@p kind), a new
/// reference. Latched caller.
[[nodiscard]] PyObject* numpy_plain(PyObject* object, Kind kind);

} // namespace strata::bindings::native

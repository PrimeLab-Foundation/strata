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

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace strata::bindings::native {

/// Buffer size for the text a native leaf formats: a date-time with a fraction
/// and an offset is 32 bytes, a UUID 36.
inline constexpr size_t kTextCapacity = 48;

/// What `classify` found, in the record's precedence order. The three
/// temporal kinds are the exact types only.
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
    /// A numpy scalar written through `item()`: a subclass, `longdouble`, a
    /// user dtype -- any scalar the three kinds below do not name, and every
    /// scalar when the running numpy did not pass the twins' runtime proof.
    NumpyScalar,
    /// numpy's own `bool_`, whose `item()` is its truth as a `bool`.
    NumpyBool,
    /// numpy's own sized integers, whose `item()` is the `int` `__index__`
    /// returns -- without the 0-d array `item()` builds first.
    NumpyInteger,
    /// numpy's own `float16`/`float32`, whose `item()` is the value widened
    /// to the `float` `__float__` returns.
    NumpyFloat,
    NumpyArray,
};

/**
 * Intern the attribute and module names the conversions use. Call once, from
 * the module init of each image, before any walk. False with an error set.
 */
[[nodiscard]] bool prepare_native_runtime() noexcept;

/// True once prepare_native_runtime() succeeded in this image; the serializer
/// reaches its native tail only then.
extern bool g_runtime_ready;

/**
 * The number of `dumps_with_default(..., native=False)` walks in progress in
 * the process -- any thread, any greenlet. Defined, with the mode it gates, in
 * python_dumps_hook.cpp (the entry scope that moves it lives there).
 *
 * It is the gate that keeps the opt-out off the natives' path: while it is 0,
 * no walk anywhere has natives off, so format_pure_leaf and classify pay one
 * relaxed load and never look the mode up. Relaxed suffices: a walk only
 * needs its own increment, made on its own thread, to be visible to itself;
 * an increment from another thread only sends this thread's natives through
 * the latched path, which asks natives_off() for this walk's own mode and
 * writes the same bytes.
 */
extern std::atomic<int> g_opt_outs;

/**
 * Whether natives are off for the innermost hook walk in the calling context:
 * 1 off, 0 on, -1 with an error set. The natives' reads (format_pure_leaf,
 * classify) make it only while g_opt_outs is not 0; every natives-on entry of
 * the hook makes it once, before its walk (NativeModeScope).
 *
 * The mode is a context variable, not a thread-local: a greenlet (gevent)
 * switch inside `default` can interleave two walks with different modes on one
 * OS thread, and greenlet gives each greenlet its own contextvars context. The
 * lookup allocates nothing and runs no user code, so a latched walk may call it.
 */
[[nodiscard]] int natives_off() noexcept;

/**
 * Format @p object into @p out (kTextCapacity bytes) when it is a pure leaf:
 * an exact `datetime`/`date`/`time` whose `tzinfo` is `None` or exactly
 * `datetime.timezone`, or an exact `uuid.UUID` with an in-range `int` slot,
 * and the type table already holds its type.
 *
 * @return the number of bytes written, or 0 when @p object is not a pure leaf
 *         or g_opt_outs is not 0 (the caller then takes the latched path,
 *         where classify reads the walk's mode). Never raises.
 */
[[nodiscard]] size_t format_pure_leaf(PyObject* object, char* out) noexcept;

/**
 * Resolve what is still unresolved of the type table, then classify @p object
 * by the record's precedence: datetime, date, time, UUID, Decimal, Enum,
 * dataclass, set/frozenset, numpy. Latched caller. A numpy object classifies
 * as native only for dtype kinds `b i u f`. `Kind::None` for every object
 * while natives_off() says the walk opted out: the object goes to `default`.
 */
[[nodiscard]] Kind classify(PyObject* object);

/**
 * The key CPython stores in a deleted set slot, which the set iterator skips.
 * Set by prepare_native_runtime() only when its proof holds: walking a set's
 * table (`PySetObject`) slot by slot, skipping empty slots and this key,
 * listed exactly what the iterator listed, for a set with deleted slots and
 * a grown table and for a frozenset of it. nullptr otherwise, and always on
 * a free-threaded build.
 */
extern PyObject* g_set_dummy;

/// Whether an exact set or frozenset may be walked on its own table.
[[nodiscard]] inline bool set_table_walk_ready() noexcept { return g_set_dummy != nullptr; }

/// True when @p object is a numpy scalar or array (resolved types only). Pure.
[[nodiscard]] bool is_numpy(PyObject* object) noexcept;

/**
 * The text of an exact `datetime`, `date` or `time` (@p kind; `classify` admits
 * no subclass), any `tzinfo` included, into @p out (kTextCapacity bytes).
 * Latched caller. @return bytes written, or -1 with an error set.
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
 * reference to a tuple of `str`. Cached per type, at most kFieldCacheLimit
 * types (a full cache is cleared). An entry is used only while the type's
 * `__dataclass_fields__` is an exact dict holding the keys and field objects
 * it held when read, by identity and in the same order, and each field still
 * has the `name` and `_field_type` it had then (by identity), so a type whose
 * fields are replaced, grown, shrunk, swapped, renamed or re-kinded in place
 * after its first use is read again. Latched caller; nullptr with an error set.
 *
 * @p keys receives, when the names come from a cache entry, a new reference
 * to a tuple of `bytes`: each name's JSON key exactly as the dataclass writer
 * would emit it through the core escaper -- `,` ahead of every name but the
 * first, the escaped name in quotes, `:` -- else nullptr (the writer then
 * escapes each name itself, and a name with no UTF-8 encoding raises there).
 */
[[nodiscard]] PyObject* dataclass_field_names(PyObject* object, PyObject*& keys);

/// Types the dataclass field-name cache holds before it is cleared.
inline constexpr Py_ssize_t kFieldCacheLimit = 1024;

/// `getattr(@p object, @p name)`, a new reference. Latched caller.
[[nodiscard]] PyObject* field_value(PyObject* object, PyObject* name);

/// `item()` of a numpy scalar or `tolist()` of an array (@p kind), or for the
/// three exact kinds the equal `bool`, `int` or `float` read through truth,
/// `__index__` or `__float__`; a new reference. Latched caller.
[[nodiscard]] PyObject* numpy_plain(PyObject* object, Kind kind);

} // namespace strata::bindings::native

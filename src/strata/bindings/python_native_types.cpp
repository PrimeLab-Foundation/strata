/**
 * @file python_native_types.cpp
 * @brief The native type table and conversions behind the serializer's native
 * tail (docs/architecture/native_types.md).
 *
 * The serializer's one translation unit that includes `datetime.h`. The field
 * macros it uses read the C `datetime` layout, so the table's temporal types
 * come from the `datetime_CAPI` capsule -- which only the C module carries --
 * and never from a class a pure-Python `datetime` could supply.
 */

#include "python_native_types.h"

#include "strata/util/temporal.hpp"

#include <datetime.h>

#if !defined(Py_T_OBJECT_EX)
#include <structmember.h>
#define Py_T_OBJECT_EX T_OBJECT_EX
#endif

namespace strata::bindings::native {

namespace {

/// Interned names, set by prepare_native_runtime() and held for the process.
struct Names {
    PyObject* datetime_module;
    PyObject* uuid_module;
    PyObject* decimal_module;
    PyObject* enum_module;
    PyObject* dataclasses_module;
    PyObject* numpy_module;
    PyObject* datetime_capi;
    PyObject* uuid_class;
    PyObject* decimal_class;
    PyObject* enum_class;
    PyObject* fields;
    PyObject* generic;
    PyObject* ndarray;
    PyObject* type_dict;
    PyObject* int_slot;
    PyObject* dataclass_fields;
    PyObject* name;
    PyObject* value;
    PyObject* utcoffset;
    PyObject* item;
    PyObject* tolist;
    PyObject* dtype;
    PyObject* kind;
};

Names g_names{};

/**
 * What has been found in `sys.modules` so far. Each group is filled at most
 * once, by the first lookup that finds its module, and holds strong
 * references for the life of the process; an unfilled group is looked for
 * again at the next lookup.
 */
struct Table {
    PyObject* datetime_module = nullptr;
    PyObject* datetime_capsule = nullptr;
    PyDateTime_CAPI* datetime_api = nullptr;
    PyTypeObject* timezone_type = nullptr;
    PyTypeObject* uuid_type = nullptr;
    /// Offset of `UUID.int`'s slot in an exact UUID, or -1 when the class
    /// does not store it in a slot (the pure path is then off for UUIDs).
    Py_ssize_t uuid_int_offset = -1;
    PyObject* uuid_bound = nullptr; ///< 2**128
    PyTypeObject* decimal_type = nullptr;
    PyTypeObject* enum_type = nullptr;
    PyObject* dataclasses_fields = nullptr;
    PyTypeObject* numpy_generic = nullptr;
    PyTypeObject* numpy_ndarray = nullptr;
    /// `type -> (__dataclass_fields__ as read, its length then, field names)`,
    /// created at the first dataclass.
    PyObject* field_cache = nullptr;
};

Table g_table{};

/// `sys.modules[name]` as a new reference, or nullptr. Never raises.
PyObject* loaded_module(PyObject* name) {
    PyObject* const modules = PyImport_GetModuleDict();
    if (modules == nullptr || !PyDict_Check(modules))
        return nullptr;
    PyObject* const module = PyDict_GetItemWithError(modules, name);
    if (module == nullptr) {
        PyErr_Clear();
        return nullptr;
    }
    return Py_NewRef(module);
}

/// `getattr(module, name)` when it is a class, as a new reference; else nullptr. Never raises.
PyTypeObject* class_attribute(PyObject* module, PyObject* name) {
    PyObject* const value = PyObject_GetAttr(module, name);
    if (value == nullptr) {
        PyErr_Clear();
        return nullptr;
    }
    if (!PyType_Check(value)) {
        Py_DECREF(value);
        return nullptr;
    }
    return reinterpret_cast<PyTypeObject*>(value);
}

void resolve_datetime() {
    if (g_table.datetime_api != nullptr)
        return;
    PyRef module(loaded_module(g_names.datetime_module));
    if (!module)
        return;
    PyRef capsule(PyObject_GetAttr(module.get(), g_names.datetime_capi));
    if (!capsule) {
        PyErr_Clear();
        return;
    }
    auto* const api =
        static_cast<PyDateTime_CAPI*>(PyCapsule_GetPointer(capsule.get(), PyDateTime_CAPSULE_NAME));
    if (api == nullptr) {
        PyErr_Clear();
        return;
    }
    g_table.datetime_module = module.release();
    g_table.datetime_capsule = capsule.release();
    g_table.timezone_type = Py_TYPE(api->TimeZone_UTC);
    PyDateTimeAPI = api;
    g_table.datetime_api = api;
}

/// The slot offset of `UUID.int` on @p type, or -1.
Py_ssize_t int_slot_offset(PyTypeObject* type) {
    PyRef dict(PyObject_GetAttr(reinterpret_cast<PyObject*>(type), g_names.type_dict));
    if (!dict) {
        PyErr_Clear();
        return -1;
    }
    PyRef descriptor(PyObject_GetItem(dict.get(), g_names.int_slot));
    if (!descriptor) {
        PyErr_Clear();
        return -1;
    }
    if (Py_TYPE(descriptor.get()) != &PyMemberDescr_Type)
        return -1;
    const auto* const member = reinterpret_cast<PyMemberDescrObject*>(descriptor.get());
    if (member->d_common.d_type != type || member->d_member->type != Py_T_OBJECT_EX)
        return -1;
    return member->d_member->offset;
}

void resolve_uuid() {
    if (g_table.uuid_type != nullptr)
        return;
    PyRef module(loaded_module(g_names.uuid_module));
    if (!module)
        return;
    PyRef type(reinterpret_cast<PyObject*>(class_attribute(module.get(), g_names.uuid_class)));
    if (!type)
        return;
    PyRef one(PyLong_FromLong(1));
    PyRef bits(PyLong_FromLong(128));
    PyRef bound(one && bits ? PyNumber_Lshift(one.get(), bits.get()) : nullptr);
    if (!bound) {
        PyErr_Clear();
        return;
    }
    g_table.uuid_int_offset = int_slot_offset(reinterpret_cast<PyTypeObject*>(type.get()));
    g_table.uuid_bound = bound.release();
    g_table.uuid_type = reinterpret_cast<PyTypeObject*>(type.release());
}

void resolve_class(PyTypeObject*& slot, PyObject* module_name, PyObject* class_name) {
    if (slot != nullptr)
        return;
    PyRef module(loaded_module(module_name));
    if (module)
        slot = class_attribute(module.get(), class_name);
}

void resolve_dataclasses() {
    if (g_table.dataclasses_fields != nullptr)
        return;
    PyRef module(loaded_module(g_names.dataclasses_module));
    if (!module)
        return;
    PyObject* const fields = PyObject_GetAttr(module.get(), g_names.fields);
    if (fields == nullptr) {
        PyErr_Clear();
        return;
    }
    if (!PyCallable_Check(fields)) {
        Py_DECREF(fields);
        return;
    }
    g_table.dataclasses_fields = fields;
}

void resolve_numpy() {
    if (g_table.numpy_generic != nullptr)
        return;
    PyRef module(loaded_module(g_names.numpy_module));
    if (!module)
        return;
    PyTypeObject* const generic = class_attribute(module.get(), g_names.generic);
    if (generic == nullptr)
        return;
    PyTypeObject* const ndarray = class_attribute(module.get(), g_names.ndarray);
    if (ndarray == nullptr) {
        Py_DECREF(generic);
        return;
    }
    g_table.numpy_ndarray = ndarray;
    g_table.numpy_generic = generic;
}

/// Look for every module the table does not hold yet. Never raises.
void resolve() {
    resolve_datetime();
    resolve_uuid();
    resolve_class(g_table.decimal_type, g_names.decimal_module, g_names.decimal_class);
    resolve_class(g_table.enum_type, g_names.enum_module, g_names.enum_class);
    resolve_dataclasses();
    resolve_numpy();
}

/// 1, 0, or -1 with an error set: `hasattr` without swallowing other errors.
int has_attribute(PyObject* owner, PyObject* name) {
#if PY_VERSION_HEX >= 0x030D0000
    return PyObject_HasAttrWithError(owner, name);
#else
    PyObject* value = nullptr;
    const int found = _PyObject_LookupAttr(owner, name, &value);
    Py_XDECREF(value);
    return found;
#endif
}

/// Whether a numpy object's dtype kind is one of `b i u f`; -1 with an error set.
int numpy_kind_is_plain(PyObject* object) {
    PyRef dtype(PyObject_GetAttr(object, g_names.dtype));
    if (!dtype)
        return -1;
    PyRef kind(PyObject_GetAttr(dtype.get(), g_names.kind));
    if (!kind)
        return -1;
    if (!PyUnicode_Check(kind.get()) || PyUnicode_GET_LENGTH(kind.get()) != 1)
        return 0;
    const Py_UCS4 code = PyUnicode_READ_CHAR(kind.get(), 0);
    return code == 'b' || code == 'i' || code == 'u' || code == 'f' ? 1 : 0;
}

size_t format_date_fields(PyObject* object, char* out) noexcept {
    return util::format_date(PyDateTime_GET_YEAR(object), PyDateTime_GET_MONTH(object),
                             PyDateTime_GET_DAY(object), out);
}

size_t format_datetime_fields(PyObject* object, char* out) noexcept {
    size_t size = format_date_fields(object, out);
    out[size++] = 'T';
    return size + util::format_time(PyDateTime_DATE_GET_HOUR(object),
                                    PyDateTime_DATE_GET_MINUTE(object),
                                    PyDateTime_DATE_GET_SECOND(object),
                                    PyDateTime_DATE_GET_MICROSECOND(object), out + size);
}

size_t format_time_fields(PyObject* object, char* out) noexcept {
    return util::format_time(PyDateTime_TIME_GET_HOUR(object), PyDateTime_TIME_GET_MINUTE(object),
                             PyDateTime_TIME_GET_SECOND(object),
                             PyDateTime_TIME_GET_MICROSECOND(object), out);
}

/// `days * 86400 + seconds` of a normalized timedelta (microseconds dropped).
int64_t delta_seconds(PyObject* delta) noexcept {
    return static_cast<int64_t>(PyDateTime_DELTA_GET_DAYS(delta)) * 86400 +
           PyDateTime_DELTA_GET_SECONDS(delta);
}

/**
 * The offset of an exact `datetime.timezone`, read without running anything
 * the user wrote: `timezone.utcoffset` is C and returns the `timedelta` the
 * instance stores. False (nothing raised) when @p tzinfo is anything else.
 */
bool pure_offset(PyObject* tzinfo, PyObject* argument, int64_t& seconds, bool& aware) noexcept {
    if (tzinfo == Py_None) {
        aware = false;
        return true;
    }
    if (Py_TYPE(tzinfo) != g_table.timezone_type)
        return false;
    PyObject* const delta = PyObject_CallMethodOneArg(tzinfo, g_names.utcoffset, argument);
    if (delta == nullptr) {
        PyErr_Clear();
        return false;
    }
    const bool exact = Py_TYPE(delta) == g_table.datetime_api->DeltaType;
    if (exact)
        seconds = delta_seconds(delta);
    Py_DECREF(delta);
    aware = exact;
    return exact;
}

/**
 * Split an exact `int` in [0, 2**128) into halves. False when it is out of
 * range or the shift fails (then an error may be set).
 */
bool split_uuid_int(PyObject* value, uint64_t& hi, uint64_t& lo) {
    PyRef zero(PyLong_FromLong(0));
    PyRef shift(PyLong_FromLong(64));
    if (!zero || !shift)
        return false;
    if (PyObject_RichCompareBool(value, zero.get(), Py_GE) != 1 ||
        PyObject_RichCompareBool(value, g_table.uuid_bound, Py_LT) != 1)
        return false;
    PyRef high(PyNumber_Rshift(value, shift.get()));
    if (!high)
        return false;
    lo = PyLong_AsUnsignedLongLongMask(value);
    hi = PyLong_AsUnsignedLongLongMask(high.get());
    return true;
}

/// The Decimal spellings of a value JSON cannot hold: `[-](Infinity | NaN… | sNaN…)`.
bool is_non_finite(std::string_view text) noexcept {
    if (!text.empty() && text.front() == '-')
        text.remove_prefix(1);
    if (text == "Infinity")
        return true;
    if (text.substr(0, 1) == "s")
        text.remove_prefix(1);
    if (text.substr(0, 3) != "NaN")
        return false;
    for (const char digit : text.substr(3))
        if (digit < '0' || digit > '9')
            return false;
    return true;
}

bool intern(PyObject*& slot, const char* text) noexcept {
    slot = PyUnicode_InternFromString(text);
    return slot != nullptr;
}

} // namespace

bool g_runtime_ready = false;

bool prepare_native_runtime() noexcept {
    // A runtime finalized and initialized again (embedding) runs module init
    // again: whatever the table held belonged to the finalized runtime and is
    // dropped, never released.
    g_table = Table{};
    g_runtime_ready = false;
    g_runtime_ready =
        intern(g_names.datetime_module, "datetime") && intern(g_names.uuid_module, "uuid") &&
        intern(g_names.decimal_module, "decimal") && intern(g_names.enum_module, "enum") &&
        intern(g_names.dataclasses_module, "dataclasses") &&
        intern(g_names.numpy_module, "numpy") && intern(g_names.datetime_capi, "datetime_CAPI") &&
        intern(g_names.uuid_class, "UUID") && intern(g_names.decimal_class, "Decimal") &&
        intern(g_names.enum_class, "Enum") && intern(g_names.fields, "fields") &&
        intern(g_names.generic, "generic") && intern(g_names.ndarray, "ndarray") &&
        intern(g_names.type_dict, "__dict__") && intern(g_names.int_slot, "int") &&
        intern(g_names.dataclass_fields, "__dataclass_fields__") && intern(g_names.name, "name") &&
        intern(g_names.value, "value") && intern(g_names.utcoffset, "utcoffset") &&
        intern(g_names.item, "item") && intern(g_names.tolist, "tolist") &&
        intern(g_names.dtype, "dtype") && intern(g_names.kind, "kind");
    return g_runtime_ready;
}

size_t format_pure_leaf(PyObject* object, char* out) noexcept {
    PyTypeObject* const type = Py_TYPE(object);
    const PyDateTime_CAPI* const api = g_table.datetime_api;
    if (api != nullptr) {
        int64_t seconds = 0;
        bool aware = false;
        if (type == api->DateTimeType) {
            if (!pure_offset(PyDateTime_DATE_GET_TZINFO(object), object, seconds, aware))
                return 0;
            const size_t size = format_datetime_fields(object, out);
            return aware ? size + util::format_utc_offset(seconds, out + size) : size;
        }
        if (type == api->DateType)
            return format_date_fields(object, out);
        if (type == api->TimeType) {
            if (!pure_offset(PyDateTime_TIME_GET_TZINFO(object), Py_None, seconds, aware))
                return 0;
            const size_t size = format_time_fields(object, out);
            return aware ? size + util::format_utc_offset(seconds, out + size) : size;
        }
    }
    if (type == g_table.uuid_type && g_table.uuid_int_offset >= 0) {
        PyObject* const value = *reinterpret_cast<PyObject**>(reinterpret_cast<char*>(object) +
                                                              g_table.uuid_int_offset);
        uint64_t hi = 0;
        uint64_t lo = 0;
        if (value == nullptr || !PyLong_CheckExact(value) || !split_uuid_int(value, hi, lo)) {
            PyErr_Clear();
            return 0;
        }
        return util::format_uuid(hi, lo, out);
    }
    return 0;
}

Kind classify(PyObject* object) {
    resolve();
    PyTypeObject* const type = Py_TYPE(object);
    // Held to the end: the attribute reads below can run code that reassigns
    // `object.__class__`, and the type may then have no other owner.
    const PyRef held(Py_NewRef(reinterpret_cast<PyObject*>(type)));
    // The exact types only: a subclass can carry state its C fields do not
    // (pandas' `NaT`, a `Timestamp`'s nanoseconds), so it is unsupported.
    if (const PyDateTime_CAPI* const api = g_table.datetime_api; api != nullptr) {
        if (type == api->DateTimeType)
            return Kind::DateTime;
        if (type == api->DateType)
            return Kind::Date;
        if (type == api->TimeType)
            return Kind::Time;
    }
    if (g_table.uuid_type != nullptr && PyType_IsSubtype(type, g_table.uuid_type))
        return Kind::Uuid;
    if (g_table.decimal_type != nullptr && PyType_IsSubtype(type, g_table.decimal_type))
        return Kind::Decimal;
    if (g_table.enum_type != nullptr && PyType_IsSubtype(type, g_table.enum_type))
        return Kind::Enum;
    if (g_table.dataclasses_fields != nullptr && !PyType_Check(object)) {
        const int found =
            has_attribute(reinterpret_cast<PyObject*>(type), g_names.dataclass_fields);
        if (found < 0)
            return Kind::Error;
        if (found > 0)
            return Kind::Dataclass;
    }
    if (PyAnySet_Check(object))
        return Kind::Set;
    if (is_numpy(object)) {
        const int plain = numpy_kind_is_plain(object);
        if (plain < 0)
            return Kind::Error;
        if (plain > 0)
            return PyType_IsSubtype(type, g_table.numpy_ndarray) ? Kind::NumpyArray
                                                                 : Kind::NumpyScalar;
    }
    return Kind::None;
}

bool is_numpy(PyObject* object) noexcept {
    if (g_table.numpy_generic == nullptr)
        return false;
    PyTypeObject* const type = Py_TYPE(object);
    return PyType_IsSubtype(type, g_table.numpy_generic) ||
           PyType_IsSubtype(type, g_table.numpy_ndarray);
}

Py_ssize_t format_temporal(PyObject* object, Kind kind, char* out) {
    if (kind == Kind::Date)
        return static_cast<Py_ssize_t>(format_date_fields(object, out));
    const bool is_datetime = kind == Kind::DateTime;
    const size_t size =
        is_datetime ? format_datetime_fields(object, out) : format_time_fields(object, out);
    PyObject* const tzinfo =
        is_datetime ? PyDateTime_DATE_GET_TZINFO(object) : PyDateTime_TIME_GET_TZINFO(object);
    if (tzinfo == Py_None)
        return static_cast<Py_ssize_t>(size);
    // What `isoformat()` asks: `tzinfo.utcoffset(dt)` for a datetime (so
    // `fold` reaches the tzinfo), `utcoffset(None)` for a time. The tzinfo is
    // the object's own, and the caller holds the object.
    const PyRef keep(Py_NewRef(tzinfo));
    const PyRef delta(
        PyObject_CallMethodOneArg(tzinfo, g_names.utcoffset, is_datetime ? object : Py_None));
    if (!delta)
        return -1;
    if (delta.get() == Py_None)
        return static_cast<Py_ssize_t>(size);
    if (!PyType_IsSubtype(Py_TYPE(delta.get()), g_table.datetime_api->DeltaType)) {
        PyErr_Format(PyExc_TypeError,
                     "tzinfo.utcoffset() must return None or timedelta, not '%.200s'",
                     Py_TYPE(delta.get())->tp_name);
        return -1;
    }
    const int days = PyDateTime_DELTA_GET_DAYS(delta.get());
    const bool whole_negative_day = days == -1 && PyDateTime_DELTA_GET_SECONDS(delta.get()) == 0 &&
                                    PyDateTime_DELTA_GET_MICROSECONDS(delta.get()) == 0;
    if (days < -1 || days > 0 || whole_negative_day) {
        PyErr_Format(PyExc_ValueError,
                     "offset must be a timedelta strictly between -timedelta(hours=24) and "
                     "timedelta(hours=24), not %R.",
                     delta.get());
        return -1;
    }
    return static_cast<Py_ssize_t>(size +
                                   util::format_utc_offset(delta_seconds(delta.get()), out + size));
}

Py_ssize_t format_uuid(PyObject* object, char* out) {
    const PyRef value(PyObject_GetAttr(object, g_names.int_slot));
    if (!value)
        return -1;
    if (!PyLong_Check(value.get())) {
        PyErr_Format(PyExc_TypeError, "UUID.int must be an int, not %s",
                     Py_TYPE(value.get())->tp_name);
        return -1;
    }
    const PyRef exact(PyNumber_Index(value.get()));
    if (!exact)
        return -1;
    uint64_t hi = 0;
    uint64_t lo = 0;
    if (!split_uuid_int(exact.get(), hi, lo)) {
        if (!PyErr_Occurred())
            PyErr_SetString(PyExc_ValueError, "UUID.int is out of range (need a 128-bit value)");
        return -1;
    }
    return static_cast<Py_ssize_t>(util::format_uuid(hi, lo, out));
}

DecimalText decimal_text(PyObject* object, PyRef& text, std::string_view& view) {
    text = PyRef(PyObject_Str(object));
    if (!text)
        return DecimalText::Error;
    Py_ssize_t size = 0;
    const char* const utf8 = PyUnicode_AsUTF8AndSize(text.get(), &size);
    if (utf8 == nullptr)
        return DecimalText::Error;
    view = std::string_view(utf8, static_cast<size_t>(size));
    if (util::is_json_number(view))
        return DecimalText::Number;
    if (is_non_finite(view))
        return DecimalText::NonFinite;
    PyErr_SetString(PyExc_ValueError, "str() of a Decimal returned text that is not a JSON number");
    return DecimalText::Error;
}

PyObject* enum_value(PyObject* member) { return PyObject_GetAttr(member, g_names.value); }

PyObject* dataclass_field_names(PyObject* object) {
    // Held across every call below: `dataclasses.fields` and the attribute
    // read run code that can reassign `object.__class__`, and the type may
    // then have no other owner.
    const PyRef held(Py_NewRef(reinterpret_cast<PyObject*>(Py_TYPE(object))));
    PyObject* const type = held.get();
    if (g_table.field_cache == nullptr) {
        g_table.field_cache = PyDict_New();
        if (g_table.field_cache == nullptr)
            return nullptr;
    }
    PyObject* const cache = g_table.field_cache;
    // What `dataclasses.fields` reads. A cached entry stands while this is the
    // same object with the same length; anything else is a miss.
    const PyRef declared(PyObject_GetAttr(type, g_names.dataclass_fields));
    if (!declared)
        return nullptr;
    const Py_ssize_t declared_size =
        PyDict_Check(declared.get()) ? PyDict_GET_SIZE(declared.get()) : -1;
    PyObject* const cached = PyDict_GetItemWithError(cache, type);
    if (cached != nullptr && declared_size >= 0 && PyTuple_GET_ITEM(cached, 0) == declared.get() &&
        PyLong_AsSsize_t(PyTuple_GET_ITEM(cached, 1)) == declared_size)
        return Py_NewRef(PyTuple_GET_ITEM(cached, 2));
    if (cached == nullptr && PyErr_Occurred())
        return nullptr;
    const PyRef fields(PyObject_CallOneArg(g_table.dataclasses_fields, type));
    if (!fields)
        return nullptr;
    const PyRef sequence(PySequence_Tuple(fields.get()));
    if (!sequence)
        return nullptr;
    const Py_ssize_t count = PyTuple_GET_SIZE(sequence.get());
    PyRef names(PyTuple_New(count));
    if (!names)
        return nullptr;
    for (Py_ssize_t index = 0; index < count; ++index) {
        PyObject* const name =
            PyObject_GetAttr(PyTuple_GET_ITEM(sequence.get(), index), g_names.name);
        if (name == nullptr)
            return nullptr;
        if (!PyUnicode_Check(name)) {
            PyErr_Format(PyExc_TypeError, "keys must be str, not %s", Py_TYPE(name)->tp_name);
            Py_DECREF(name);
            return nullptr;
        }
        PyTuple_SET_ITEM(names.get(), index, name);
    }
    if (declared_size < 0) // not a dict: nothing stable to check an entry against
        return names.release();
    const PyRef size(PyLong_FromSsize_t(declared_size));
    const PyRef entry(size ? PyTuple_Pack(3, declared.get(), size.get(), names.get()) : nullptr);
    if (!entry)
        return nullptr;
    if (PyDict_GET_SIZE(cache) >= kFieldCacheLimit)
        PyDict_Clear(cache);
    if (PyDict_SetItem(cache, type, entry.get()) < 0)
        return nullptr;
    return names.release();
}

PyObject* field_value(PyObject* object, PyObject* name) { return PyObject_GetAttr(object, name); }

PyObject* numpy_plain(PyObject* object, Kind kind) {
    return PyObject_CallMethodNoArgs(object,
                                     kind == Kind::NumpyScalar ? g_names.item : g_names.tolist);
}

} // namespace strata::bindings::native

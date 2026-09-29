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

#include "python_numpy_twins.h"
#include "strata/json/json_serialize.hpp"
#include "strata/util/temporal.hpp"

#include <datetime.h>
#include <string>

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
    PyObject* scalar_type;
    PyObject* number;
    PyObject* value_slot;
    PyObject* get;
    PyObject* fget;
    PyObject* field_class;
    PyObject* field_kind;
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
    PyTypeObject* decimal_type = nullptr;
    PyTypeObject* enum_type = nullptr;
    PyObject* dataclasses_fields = nullptr;
    PyTypeObject* numpy_generic = nullptr;
    PyTypeObject* numpy_ndarray = nullptr;
    /// Whether numpy_twins_hold() held when numpy resolved. False: every numpy
    /// scalar is read through `item()`.
    bool numpy_twins = false;
    /**
     * `Enum.value` as the enum module defines it, captured and proven once
     * when enum resolves (capture_enum_value); `descriptor` stays nullptr
     * when the proof fails, and every member is then read through `value`.
     * While each piece is still in place (stock_enum_value), `member.value`
     * is `member._value_` by construction, and enum_value reads that.
     */
    struct EnumValue {
        PyObject* descriptor = nullptr;          ///< `Enum.__dict__["value"]`
        PyTypeObject* descriptor_type = nullptr; ///< its class (`enum.property`)
        PyObject* get = nullptr;                 ///< that class's `__get__`, a function
        PyObject* get_code = nullptr;            ///< and its code
        PyObject* fget = nullptr;                ///< the descriptor's `fget`, a function
        PyObject* fget_code = nullptr;           ///< and its code: `return self._value_`
        /// The descriptor class's version tag when captured; while it is
        /// current (tag_current) its `__get__` and absent `fget` need no lookup.
        unsigned int descriptor_type_tag = 0;
        bool tried = false;
    } enum_value;
    /// `type -> ((key, field, ...) of __dataclass_fields__ as read, field
    /// names, their key bytes or None, (name, _field_type, ...) of each field)`,
    /// created at the first dataclass.
    PyObject* field_cache = nullptr;
    /// `dataclasses.Field` and the slots of its `name` and `_field_type`, read
    /// in place by field_attribute while the class's version tag is current;
    /// -1 (or no class) when they are not plain object slots of it.
    PyTypeObject* field_type = nullptr;
    Py_ssize_t field_name_offset = -1;
    Py_ssize_t field_kind_offset = -1;
    unsigned int field_type_tag = 0;
};

Table g_table{};

/// Whether uuid_digits reads this interpreter's `int` layout: proven by
/// uuid_digits_proven() when uuid resolves; until then, and if it fails,
/// split_uuid_int takes uuid_native_bytes.
bool g_uuid_digits = false;

bool uuid_digits_proven();

/**
 * How a cached type verdict is checked against the type (tag_current): Off
 * (no cache), or the version tag CPython keeps per type -- set when the type
 * is next looked up, reset whenever the type or any base is modified (an
 * attribute set or deleted, `__bases__` assigned) -- alone, or with the flag
 * that marks it valid. Chosen by prove_tag_check() at module init, which
 * requires a fresh tag to read current and a modified type's, or a modified
 * base's subclass's, to read stale; Off when neither holds, and on a
 * free-threaded build.
 */
enum class TagCheck : uint8_t { Off, TagOnly, TagAndFlag };
TagCheck g_tag_check = TagCheck::Off;

/// Whether @p tag, read from @p type earlier, still describes it.
bool tag_current(PyTypeObject* type, unsigned int tag) noexcept {
    if (tag == 0 || type->tp_version_tag != tag)
        return false;
    return g_tag_check == TagCheck::TagOnly ||
           (g_tag_check == TagCheck::TagAndFlag &&
            (type->tp_flags & Py_TPFLAGS_VALID_VERSION_TAG) != 0);
}

/// Give @p type a version tag if it has none; its tag, 0 when none is available.
unsigned int assign_tag(PyTypeObject* type) noexcept {
#if defined(Py_GIL_DISABLED)
    (void)type;
    return 0;
#elif PY_VERSION_HEX >= 0x030C0000
    if (PyUnstable_Type_AssignVersionTag(type) == 0)
        return 0;
    return type->tp_version_tag;
#else
    // The type cache assigns a tag on a lookup, found or not.
    (void)_PyType_Lookup(type, g_names.value);
    return type->tp_version_tag;
#endif
}

/**
 * What classify decided for one type, kept while the type's version tag is
 * current: everything the precedence reads of a type -- its identity, its MRO
 * (`PyType_IsSubtype`, `PyAnySet_Check`), `__dataclass_fields__` found
 * through the MRO -- changes only through a modification that resets the tag.
 * `stock_value` is stock_enum_value's per-type half for an Enum verdict
 * (`tp_getattro` generic and `value` resolving to the captured descriptor),
 * which a modification resets as well. Borrowed type pointers: a freed type's
 * address reused by a new type carries another tag.
 */
struct Verdict {
    PyTypeObject* type = nullptr;
    unsigned int tag = 0;
    Kind kind = Kind::None;
    bool stock_value = false;
};

constexpr unsigned kVerdictBits = 6;
constexpr size_t kVerdictSlots = size_t{1} << kVerdictBits;
Verdict g_verdicts[kVerdictSlots];

Verdict& verdict_slot(PyTypeObject* type) noexcept {
    // Fibonacci hashing of the address: type objects are allocated at strides
    // whose low bits agree (measured: a record's Decimal, Enum and dataclass
    // classes shared one slot under `>> 4`), so the high product bits index.
    const uint64_t hashed =
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(type) >> 4) * 0x9E3779B97F4A7C15ULL;
    return g_verdicts[hashed >> (64 - kVerdictBits)];
}

/// The verdict for @p type if one is cached and current, else nullptr.
const Verdict* cached_verdict(PyTypeObject* type) noexcept {
    if (g_tag_check == TagCheck::Off)
        return nullptr;
    const Verdict& slot = verdict_slot(type);
    return slot.type == type && tag_current(type, slot.tag) ? &slot : nullptr;
}

/**
 * Whether tag_current under @p check tells a fresh tag from a stale one on
 * types made here: a class and its subclass read current once tagged, and
 * stale once an attribute is set on the class; the subclass, tagged again,
 * reads stale once its `__bases__` is assigned. Runs no user code.
 */
bool tag_check_holds(TagCheck check) {
    const TagCheck saved = g_tag_check;
    g_tag_check = check;
    auto* const meta = reinterpret_cast<PyObject*>(&PyType_Type);
    PyRef base(PyObject_CallFunction(meta, "s(){}", "_strata_tag_probe"));
    PyRef derived(base ? PyObject_CallFunction(meta, "s(O){}", "_strata_tag_probe_sub", base.get())
                       : nullptr);
    PyRef other(derived ? PyObject_CallFunction(meta, "s(){}", "_strata_tag_probe_other")
                        : nullptr);
    PyRef bases(other ? PyTuple_Pack(1, other.get()) : nullptr);
    bool holds = false;
    if (bases) {
        auto* const b = reinterpret_cast<PyTypeObject*>(base.get());
        auto* const d = reinterpret_cast<PyTypeObject*>(derived.get());
        const unsigned int base_tag = assign_tag(b);
        const unsigned int derived_tag = assign_tag(d);
        holds = tag_current(b, base_tag) && tag_current(d, derived_tag) &&
                PyObject_SetAttrString(base.get(), "probe", Py_None) == 0 &&
                !tag_current(b, base_tag) && !tag_current(d, derived_tag);
        const unsigned int retagged = holds ? assign_tag(d) : 0;
        holds = holds && retagged != derived_tag && tag_current(d, retagged) &&
                PyObject_SetAttrString(derived.get(), "__bases__", bases.get()) == 0 &&
                !tag_current(d, retagged);
    }
    PyErr_Clear();
    g_tag_check = saved;
    return holds;
}

TagCheck prove_tag_check() {
#if defined(Py_GIL_DISABLED)
    return TagCheck::Off;
#else
    if (tag_check_holds(TagCheck::TagAndFlag))
        return TagCheck::TagAndFlag;
    if (tag_check_holds(TagCheck::TagOnly))
        return TagCheck::TagOnly;
    return TagCheck::Off;
#endif
}

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

/// The offset of @p type's own object slot @p name (a member descriptor in
/// its `__dict__`), or -1.
Py_ssize_t object_slot_offset(PyTypeObject* type, PyObject* name) {
    PyRef dict(PyObject_GetAttr(reinterpret_cast<PyObject*>(type), g_names.type_dict));
    if (!dict) {
        PyErr_Clear();
        return -1;
    }
    PyRef descriptor(PyObject_GetItem(dict.get(), name));
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
    auto* const uuid_class = reinterpret_cast<PyTypeObject*>(type.get());
    g_table.uuid_int_offset = object_slot_offset(uuid_class, g_names.int_slot);
    g_uuid_digits = uuid_digits_proven();
    PyErr_Clear();
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
    // Optional: without it field_attribute reads each Field generically.
    if (PyTypeObject* const field_type = class_attribute(module.get(), g_names.field_class)) {
        g_table.field_name_offset = object_slot_offset(field_type, g_names.name);
        g_table.field_kind_offset = object_slot_offset(field_type, g_names.field_kind);
        g_table.field_type_tag =
            field_type->tp_getattro == PyObject_GenericGetAttr ? assign_tag(field_type) : 0;
        g_table.field_type = field_type;
    }
    PyErr_Clear();
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
    // After the types are in the table: a walk the proof's own calls start
    // finds numpy resolved, twins off, and reads its scalars through item().
    g_table.numpy_twins = numpy_twins_hold(module.get());
    if (!g_table.numpy_twins)
        PyErr_Clear();
}

/// Whether @p left and @p right have equal attribute @p name. Runs code.
bool same_attribute(PyObject* left, PyObject* right, const char* name) {
    PyRef a(PyObject_GetAttrString(left, name));
    PyRef b(a ? PyObject_GetAttrString(right, name) : nullptr);
    return b && PyObject_RichCompareBool(a.get(), b.get(), Py_EQ) == 1;
}

/**
 * Whether the function @p getter is `def value(self): return self._value_`:
 * its bytecode, names, arguments and cells equal those of that source,
 * compiled now by this interpreter. Equal bytecode over equal names, one
 * positional argument and no cells is the same computation: the getter
 * returns `getattr(self, "_value_")` and does nothing else. Runs code.
 */
bool reads_value_slot(PyObject* getter) {
    PyObject* const code = PyFunction_GetCode(getter);
    PyRef module(
        Py_CompileString("def value(self):\n    return self._value_\n", "<strata>", Py_file_input));
    PyRef constants(module ? PyObject_GetAttrString(module.get(), "co_consts") : nullptr);
    if (!constants || !PyTuple_Check(constants.get()))
        return false;
    PyObject* reference = nullptr;
    for (Py_ssize_t index = 0; index < PyTuple_GET_SIZE(constants.get()); ++index)
        if (PyCode_Check(PyTuple_GET_ITEM(constants.get(), index)))
            reference = PyTuple_GET_ITEM(constants.get(), index);
    if (reference == nullptr)
        return false;
    for (const char* name : {"co_code", "co_names", "co_argcount", "co_posonlyargcount",
                             "co_kwonlyargcount", "co_freevars", "co_cellvars"})
        if (!same_attribute(code, reference, name))
            return false;
    const auto* const raw = reinterpret_cast<PyCodeObject*>(code);
    return (raw->co_flags & (CO_VARARGS | CO_VARKEYWORDS)) == 0;
}

/**
 * Whether @p get, a descriptor class's `__get__`, returns `fget(instance)` for
 * an instance that is not None: a probe descriptor of @p descriptor_type
 * built on `id` must hand back the id of the instance it is read from. Runs
 * code.
 */
bool get_calls_fget(PyObject* get, PyTypeObject* descriptor_type) {
    PyObject* const builtins = PyEval_GetBuiltins();
    PyObject* const id = builtins ? PyDict_GetItemString(builtins, "id") : nullptr;
    if (id == nullptr)
        return false;
    PyRef probe(PyObject_CallOneArg(reinterpret_cast<PyObject*>(descriptor_type), id));
    if (!probe)
        return false;
    PyObject* const instance = probe.get(); // any object that is not None
    PyRef read(PyObject_CallFunctionObjArgs(
        get, probe.get(), instance, reinterpret_cast<PyObject*>(Py_TYPE(instance)), nullptr));
    PyRef expected(PyLong_FromVoidPtr(instance));
    return read && expected && PyObject_RichCompareBool(read.get(), expected.get(), Py_EQ) == 1;
}

/**
 * Capture `Enum.value` for enum_value's `_value_` read, once, after enum
 * resolved. What the proof establishes, for a member whose class resolves
 * `value` to this very descriptor through generic attribute access
 * (stock_enum_value checks that part per member): the descriptor's class
 * reads `fget` from the instance (no class attribute shadows it) and its
 * `__get__` returns `fget(member)`; `fget` returns `member._value_`. So
 * `getattr(member, "value")` is `getattr(member, "_value_")`, with the same
 * result, the same exception and the same code run under it -- two Python
 * frames fewer. A failed proof leaves the fast read off. Never raises.
 */
void capture_enum_value() {
    auto& stock = g_table.enum_value;
    if (stock.tried || g_table.enum_type == nullptr)
        return;
    stock.tried = true;
#if !defined(Py_GIL_DISABLED)
    PyObject* const descriptor = _PyType_Lookup(g_table.enum_type, g_names.value);
    if (descriptor == nullptr)
        return;
    const PyRef held(Py_NewRef(descriptor));
    PyTypeObject* const descriptor_type = Py_TYPE(descriptor);
    PyObject* const get = _PyType_Lookup(descriptor_type, g_names.get);
    if (get == nullptr || !PyFunction_Check(get) ||
        descriptor_type->tp_getattro != PyObject_GenericGetAttr ||
        _PyType_Lookup(descriptor_type, g_names.fget) != nullptr)
        return;
    const PyRef held_get(Py_NewRef(get));
    PyRef fget(PyObject_GetAttr(descriptor, g_names.fget));
    const bool proven = fget && PyFunction_Check(fget.get()) && reads_value_slot(fget.get()) &&
                        get_calls_fget(get, descriptor_type);
    PyErr_Clear();
    if (!proven)
        return;
    stock.get = Py_NewRef(get);
    stock.get_code = Py_NewRef(PyFunction_GetCode(get));
    stock.fget_code = Py_NewRef(PyFunction_GetCode(fget.get()));
    stock.fget = fget.release();
    stock.descriptor_type =
        reinterpret_cast<PyTypeObject*>(Py_NewRef(reinterpret_cast<PyObject*>(descriptor_type)));
    stock.descriptor = Py_NewRef(descriptor);
    stock.descriptor_type_tag = assign_tag(descriptor_type);
#endif
}

/// stock_enum_value's per-type half: generic attribute access on @p type's
/// instances, and `value` resolving through its MRO to the captured descriptor.
bool stock_enum_type(PyTypeObject* type) {
#if defined(Py_GIL_DISABLED)
    (void)type;
    return false;
#else
    return type->tp_getattro == PyObject_GenericGetAttr &&
           _PyType_Lookup(type, g_names.value) == g_table.enum_value.descriptor;
#endif
}

/**
 * Whether `getattr(member, "value")` for an instance of @p type is, right
 * now, the captured `Enum.value` read generically: @p type resolves `value`
 * to the captured descriptor, attribute access on the member and on the
 * descriptor is the generic one, and the descriptor's class, its `__get__`
 * and code, and its `fget` and code are the ones capture_enum_value proved.
 * Latched caller: the lookups can run a metaclass's or a key's `__eq__`.
 */
bool stock_enum_value(PyTypeObject* type) {
#if defined(Py_GIL_DISABLED)
    (void)type;
    return false;
#else
    const auto& stock = g_table.enum_value;
    PyObject* const descriptor = stock.descriptor;
    if (descriptor == nullptr)
        return false;
    // The per-type half: from the type's verdict while its tag is current.
    const Verdict* const verdict = cached_verdict(type);
    if (!(verdict != nullptr && verdict->kind == Kind::Enum ? verdict->stock_value
                                                            : stock_enum_type(type)))
        return false;
    PyTypeObject* const descriptor_type = Py_TYPE(descriptor);
    if (descriptor_type != stock.descriptor_type || PyFunction_GetCode(stock.get) != stock.get_code)
        return false;
    // The descriptor's class as capture_enum_value proved it -- generic access,
    // its `__get__`, no `fget` of its own -- read off its version tag while
    // that is current, else looked up again.
    if (!tag_current(descriptor_type, stock.descriptor_type_tag) &&
        (descriptor_type->tp_getattro != PyObject_GenericGetAttr ||
         _PyType_Lookup(descriptor_type, g_names.get) != stock.get ||
         _PyType_Lookup(descriptor_type, g_names.fget) != nullptr))
        return false;
    // `self.fget` inside `__get__`: no class attribute shadows it, so the
    // instance dictionary.
    PyObject* const dict = PyObject_GenericGetDict(descriptor, nullptr);
    if (dict == nullptr) {
        PyErr_Clear();
        return false;
    }
    PyObject* const fget = PyDict_GetItemWithError(dict, g_names.fget);
    const bool stock_fget =
        fget != nullptr && fget == stock.fget && PyFunction_GetCode(fget) == stock.fget_code;
    Py_DECREF(dict);
    if (!stock_fget)
        PyErr_Clear();
    return stock_fget;
#endif
}

/// Look for every module the table does not hold yet. Never raises.
void resolve() {
    resolve_datetime();
    resolve_uuid();
    resolve_class(g_table.decimal_type, g_names.decimal_module, g_names.decimal_class);
    resolve_class(g_table.enum_type, g_names.enum_module, g_names.enum_class);
    capture_enum_value();
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

/**
 * What a numpy object's dtype makes of it: `None` unless the dtype kind is
 * one of `b i u f`; for an array `NumpyArray`; for a scalar one of the three
 * exact kinds when the twins' proof held, the dtype's scalar type is the
 * object's own type (not a subclass, whose `item()` may be its own) and its
 * number is one whose `item()` converts as truth, `__index__` or `__float__`
 * does, else `NumpyScalar`. `Error` with an exception set.
 */
Kind numpy_kind(PyObject* object, bool array) {
    PyRef dtype(PyObject_GetAttr(object, g_names.dtype));
    if (!dtype)
        return Kind::Error;
    PyRef kind(PyObject_GetAttr(dtype.get(), g_names.kind));
    if (!kind)
        return Kind::Error;
    if (!PyUnicode_Check(kind.get()) || PyUnicode_GET_LENGTH(kind.get()) != 1)
        return Kind::None;
    const Py_UCS4 code = PyUnicode_READ_CHAR(kind.get(), 0);
    if (code != 'b' && code != 'i' && code != 'u' && code != 'f')
        return Kind::None;
    if (array)
        return Kind::NumpyArray;
    if (!g_table.numpy_twins)
        return Kind::NumpyScalar;
    PyRef scalar_type(PyObject_GetAttr(dtype.get(), g_names.scalar_type));
    if (!scalar_type)
        return Kind::Error;
    PyRef number_object(PyObject_GetAttr(dtype.get(), g_names.number));
    if (!number_object)
        return Kind::Error;
    if (scalar_type.get() != reinterpret_cast<PyObject*>(Py_TYPE(object)) ||
        !PyLong_CheckExact(number_object.get()))
        return Kind::NumpyScalar;
    const long number = PyLong_AsLong(number_object.get());
    if (number == -1 && PyErr_Occurred())
        return Kind::Error;
    if (code == 'b' && number == kNumpyBool)
        return Kind::NumpyBool;
    if ((code == 'i' || code == 'u') && number >= kNumpyFirstInteger && number <= kNumpyLastInteger)
        return Kind::NumpyInteger;
    if (code == 'f' && (number == kNumpyFloat32 || number == kNumpyFloat16))
        return Kind::NumpyFloat;
    return Kind::NumpyScalar;
}

#if PY_VERSION_HEX >= 0x030A0000 && PY_VERSION_HEX < 0x030F0000 && !defined(Py_GIL_DISABLED)
/// Whether walking @p set's table -- slots in index order, skipping empty
/// slots and @p dummy -- lists exactly its iterator's keys, in order.
bool table_walk_matches_iterator(PyObject* set, PyObject* dummy) {
    const auto* const table_set = reinterpret_cast<PySetObject*>(set);
    PyRef iterator(PyObject_GetIter(set));
    if (!iterator)
        return false;
    Py_ssize_t position = 0;
    Py_ssize_t listed = 0;
    for (;;) {
        PyRef item(PyIter_Next(iterator.get()));
        const setentry* const table = table_set->table;
        while (position <= table_set->mask &&
               (table[position].key == nullptr || table[position].key == dummy))
            ++position;
        if (!item)
            return !PyErr_Occurred() && position > table_set->mask && listed == table_set->used;
        if (position > table_set->mask || table[position].key != item.get())
            return false;
        ++position;
        ++listed;
    }
}

/**
 * The set walk's proof. A fresh set with one key added and discarded has one
 * non-empty slot, holding the dummy (hash -1, as CPython marks a deleted
 * slot); the walk must then list what the iterator lists for a set whose
 * table grew past the inline one and holds deleted slots, and for a
 * frozenset of it. The dummy, or nullptr.
 */
PyObject* probe_set_dummy() {
    PyRef set(PySet_New(nullptr));
    PyRef key(PyLong_FromLong(7));
    if (!set || !key || PySet_Add(set.get(), key.get()) < 0 ||
        PySet_Discard(set.get(), key.get()) != 1)
        return nullptr;
    const auto* const probe = reinterpret_cast<PySetObject*>(set.get());
    PyObject* dummy = nullptr;
    for (Py_ssize_t index = 0; index <= probe->mask; ++index) {
        const setentry& entry = probe->table[index];
        if (entry.key == nullptr)
            continue;
        if (dummy != nullptr || entry.hash != -1)
            return nullptr;
        dummy = entry.key;
    }
    if (dummy == nullptr || probe->used != 0)
        return nullptr;
    constexpr long kKeys = 64;
    PyRef grown(PySet_New(nullptr));
    for (long pass = 0; grown && pass < 2; ++pass) {
        // Add every key, then discard every third: deleted slots in a grown table.
        for (long value = pass; value < kKeys; value += pass == 0 ? 1 : 3) {
            PyRef item(PyLong_FromLong(value * 7));
            if (!item || (pass == 0 ? PySet_Add(grown.get(), item.get()) < 0
                                    : PySet_Discard(grown.get(), item.get()) != 1))
                return nullptr;
        }
    }
    const auto* const grown_set = reinterpret_cast<PySetObject*>(grown.get());
    PyRef frozen(grown ? PyFrozenSet_New(grown.get()) : nullptr);
    if (!frozen || grown_set->fill == grown_set->used ||
        !table_walk_matches_iterator(grown.get(), dummy) ||
        !table_walk_matches_iterator(frozen.get(), dummy))
        return nullptr;
    return dummy;
}
#endif

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
 * An exact non-negative `int` below 2**128 as halves, read straight from its
 * digits (CPython's layout: `PyLong_SHIFT`-bit digits, least significant
 * first; the sign and count in `lv_tag` from 3.12, in `ob_size` before).
 * False when it is negative or 2**128 or more. Reads memory only.
 */
bool uuid_digits(PyObject* value, uint64_t& hi, uint64_t& lo) noexcept {
    const auto* const number = reinterpret_cast<const PyLongObject*>(value);
#if PY_VERSION_HEX >= 0x030C0000
    const uintptr_t tag = number->long_value.lv_tag;
    const uintptr_t sign = tag & _PyLong_SIGN_MASK; // 0 positive, 1 zero, 2 negative
    if (sign == 2)
        return false;
    const size_t count = sign == 1 ? 0 : static_cast<size_t>(tag >> _PyLong_NON_SIZE_BITS);
    const digit* const digits = number->long_value.ob_digit;
#else
    const Py_ssize_t size = Py_SIZE(value);
    if (size < 0)
        return false;
    const auto count = static_cast<size_t>(size);
    const digit* const digits = number->ob_digit;
#endif
    constexpr unsigned kShift = PyLong_SHIFT;
    hi = 0;
    lo = 0;
    for (size_t index = count; index-- > 0;) {
        if ((hi >> (64 - kShift)) != 0)
            return false; // this shift would carry past bit 127
        hi = (hi << kShift) | (lo >> (64 - kShift));
        lo = (lo << kShift) | digits[index];
    }
    return true;
}

/**
 * Split an exact `int` in [0, 2**128) into halves through the C API's byte
 * export: no object is created and nothing is called. False, with no error
 * set, when it is negative or 2**128 or more.
 */
bool uuid_native_bytes(PyObject* value, uint64_t& hi, uint64_t& lo) noexcept {
    unsigned char bytes[16];
#if PY_VERSION_HEX >= 0x030D0000
    const Py_ssize_t needed =
        PyLong_AsNativeBytes(value, bytes, sizeof(bytes),
                             Py_ASNATIVEBYTES_LITTLE_ENDIAN | Py_ASNATIVEBYTES_UNSIGNED_BUFFER |
                                 Py_ASNATIVEBYTES_REJECT_NEGATIVE);
    if (needed < 0) {
        PyErr_Clear(); // negative: ValueError
        return false;
    }
    if (needed > static_cast<Py_ssize_t>(sizeof(bytes)))
        return false;
#else
    if (_PyLong_AsByteArray(reinterpret_cast<PyLongObject*>(value), bytes, sizeof(bytes),
                            /*little_endian=*/1, /*is_signed=*/0) < 0) {
        PyErr_Clear(); // negative, or 2**128 or more: OverflowError
        return false;
    }
#endif
    lo = 0;
    hi = 0;
    for (int index = 7; index >= 0; --index) {
        lo = (lo << 8) | bytes[index];
        hi = (hi << 8) | bytes[index + 8];
    }
    return true;
}

/// An exact `int` in [0, 2**128) as halves; false otherwise, no error set.
bool split_uuid_int(PyObject* value, uint64_t& hi, uint64_t& lo) noexcept {
    return g_uuid_digits ? uuid_digits(value, hi, lo) : uuid_native_bytes(value, hi, lo);
}

/**
 * Whether uuid_digits agrees with uuid_native_bytes, answer and halves, on
 * ints that put a set bit in and around every digit boundary up to 2**128
 * and on the refusals either side of the range (negative, 2**128, 2**129).
 * The layout is CPython's, not the C API's, so it is proven where it runs,
 * as the set-table walk is. Allocates; uuid's resolution calls it once.
 */
bool uuid_digits_proven() {
    const char* const probes[] = {
        "0",
        "1",
        "-1",
        "3fffffff",
        "40000000",
        "7fff",
        "8000",
        "7fffffffffffffff",
        "8000000000000000",
        "ffffffffffffffff",
        "10000000000000000",
        "10000000000000001",
        "123456789abcdef0fedcba9876543210",
        "80000000000000000000000000000000",
        "ffffffffffffffffffffffffffffffff",
        "100000000000000000000000000000000",
        "200000000000000000000000000000000",
        "-ffffffffffffffffffffffffffffffff",
    };
    for (const char* const probe : probes) {
        PyRef number(PyLong_FromString(probe, nullptr, 16));
        if (!number || !PyLong_CheckExact(number.get())) {
            PyErr_Clear();
            return false;
        }
        uint64_t hi_digits = 0;
        uint64_t lo_digits = 0;
        uint64_t hi_bytes = 0;
        uint64_t lo_bytes = 0;
        const bool by_digits = uuid_digits(number.get(), hi_digits, lo_digits);
        const bool by_bytes = uuid_native_bytes(number.get(), hi_bytes, lo_bytes);
        if (by_digits != by_bytes ||
            (by_digits && (hi_digits != hi_bytes || lo_digits != lo_bytes)))
            return false;
    }
    // Every bit position alone, 0 through 128.
    PyRef one(PyLong_FromLong(1));
    for (long bit = 0; one && bit <= 128; ++bit) {
        PyRef shift(PyLong_FromLong(bit));
        PyRef number(shift ? PyNumber_Lshift(one.get(), shift.get()) : nullptr);
        if (!number) {
            PyErr_Clear();
            return false;
        }
        uint64_t hi_digits = 0;
        uint64_t lo_digits = 0;
        uint64_t hi_bytes = 0;
        uint64_t lo_bytes = 0;
        const bool by_digits = uuid_digits(number.get(), hi_digits, lo_digits);
        const bool by_bytes = uuid_native_bytes(number.get(), hi_bytes, lo_bytes);
        if (by_digits != by_bytes ||
            (by_digits && (hi_digits != hi_bytes || lo_digits != lo_bytes)))
            return false;
    }
    return static_cast<bool>(one);
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

/// `(key, value, key, value, ...)` of the exact dict @p fields, in order; a new
/// reference, or nullptr with an error set. Runs no code once the tuple exists.
PyObject* field_pairs(PyObject* fields) {
    const Py_ssize_t size = PyDict_GET_SIZE(fields);
    PyObject* const pairs = PyTuple_New(2 * size);
    if (pairs == nullptr)
        return nullptr;
    Py_ssize_t position = 0;
    Py_ssize_t index = 0;
    PyObject* key = nullptr;
    PyObject* value = nullptr;
    while (index < 2 * size && PyDict_Next(fields, &position, &key, &value)) {
        PyTuple_SET_ITEM(pairs, index++, Py_NewRef(key));
        PyTuple_SET_ITEM(pairs, index++, Py_NewRef(value));
    }
    return pairs;
}

/// Whether the exact dict @p fields holds @p pairs' keys and values, by
/// identity and in order, and nothing else. Runs no code.
bool fields_match(PyObject* fields, PyObject* pairs) noexcept {
    if (2 * PyDict_GET_SIZE(fields) != PyTuple_GET_SIZE(pairs))
        return false;
    Py_ssize_t position = 0;
    Py_ssize_t index = 0;
    PyObject* key = nullptr;
    PyObject* value = nullptr;
    while (PyDict_Next(fields, &position, &key, &value)) {
        if (PyTuple_GET_ITEM(pairs, index) != key || PyTuple_GET_ITEM(pairs, index + 1) != value)
            return false;
        index += 2;
    }
    return true;
}

/**
 * Each of @p names' JSON keys as write_dataclass emits them -- `,` ahead of
 * every name but the first, the name through the core escaper (the one
 * definition write_key_cold also uses), `:` -- as a new tuple of `bytes`;
 * `None` (a new reference, no error set) when a name has no UTF-8 encoding,
 * so the writer escapes each name itself and raises at that field; nullptr
 * with an error set when an allocation fails. Runs no code.
 */
PyObject* encode_keys(PyObject* names) {
    const Py_ssize_t count = PyTuple_GET_SIZE(names);
    PyRef keys(PyTuple_New(count));
    if (!keys)
        return nullptr;
    std::string scratch;
    for (Py_ssize_t index = 0; index < count; ++index) {
        Py_ssize_t size = 0;
        const char* const utf8 = PyUnicode_AsUTF8AndSize(PyTuple_GET_ITEM(names, index), &size);
        if (utf8 == nullptr) {
            PyErr_Clear();
            return Py_NewRef(Py_None);
        }
        scratch.clear();
        if (index != 0)
            scratch.push_back(',');
        append_escaped_json_string(std::string_view(utf8, static_cast<size_t>(size)), scratch);
        scratch.push_back(':');
        PyObject* const key =
            PyBytes_FromStringAndSize(scratch.data(), static_cast<Py_ssize_t>(scratch.size()));
        if (key == nullptr)
            return nullptr;
        PyTuple_SET_ITEM(keys.get(), index, key);
    }
    return keys.release();
}

/**
 * `getattr(field, name)` for one of a dataclass Field's attributes, a new
 * reference or nullptr (no error set). An exact `dataclasses.Field` whose
 * class is unmodified since it resolved (its version tag current) has the
 * attribute in a plain object slot, read in place -- what the member
 * descriptor's read returns; anything else is read generically.
 */
PyObject* field_attribute(PyObject* field, Py_ssize_t offset, PyObject* name) {
    PyTypeObject* const field_type = g_table.field_type;
    if (offset >= 0 && Py_TYPE(field) == field_type &&
        tag_current(field_type, g_table.field_type_tag))
        return Py_XNewRef(*reinterpret_cast<PyObject**>(reinterpret_cast<char*>(field) + offset));
    PyObject* const value = PyObject_GetAttr(field, name);
    if (value == nullptr)
        PyErr_Clear();
    return value;
}

/**
 * What `dataclasses.fields` reads of each field in @p pairs (the
 * `(key, field, ...)` of fields_match): its `name` and `_field_type`, as
 * `(name, kind, name, kind, ...)`; nullptr (no error set) when a read fails.
 * Runs code for a field that is not an exact, unmodified `Field`.
 */
PyObject* field_states(PyObject* pairs) {
    const Py_ssize_t count = PyTuple_GET_SIZE(pairs) / 2;
    PyRef states(PyTuple_New(2 * count));
    if (!states) {
        PyErr_Clear();
        return nullptr;
    }
    for (Py_ssize_t index = 0; index < count; ++index) {
        PyObject* const field = PyTuple_GET_ITEM(pairs, 2 * index + 1);
        PyObject* const name = field_attribute(field, g_table.field_name_offset, g_names.name);
        if (name == nullptr)
            return nullptr;
        PyTuple_SET_ITEM(states.get(), 2 * index, name);
        PyObject* const kind =
            field_attribute(field, g_table.field_kind_offset, g_names.field_kind);
        if (kind == nullptr)
            return nullptr;
        PyTuple_SET_ITEM(states.get(), 2 * index + 1, kind);
    }
    return states.release();
}

/**
 * Whether every field in @p pairs still has the `name` and `_field_type`
 * @p states recorded, by identity: a Field renamed, or re-kinded, in place
 * changes what `dataclasses.fields` lists without changing the dict
 * fields_match compares.
 */
bool field_states_match(PyObject* pairs, PyObject* states) {
    const Py_ssize_t count = PyTuple_GET_SIZE(pairs) / 2;
    if (PyTuple_GET_SIZE(states) != 2 * count)
        return false;
    // field_attribute's in-place read, decided once per call and compared
    // borrowed: nothing runs between the loads and the compares.
    PyTypeObject* const field_type = g_table.field_type;
    const Py_ssize_t name_offset = g_table.field_name_offset;
    const Py_ssize_t kind_offset = g_table.field_kind_offset;
    const bool in_place = name_offset >= 0 && kind_offset >= 0 && field_type != nullptr &&
                          tag_current(field_type, g_table.field_type_tag);
    for (Py_ssize_t index = 0; index < count; ++index) {
        PyObject* const field = PyTuple_GET_ITEM(pairs, 2 * index + 1);
        if (in_place && Py_TYPE(field) == field_type) {
            char* const base = reinterpret_cast<char*>(field);
            if (*reinterpret_cast<PyObject**>(base + name_offset) !=
                    PyTuple_GET_ITEM(states, 2 * index) ||
                *reinterpret_cast<PyObject**>(base + kind_offset) !=
                    PyTuple_GET_ITEM(states, 2 * index + 1))
                return false;
            continue;
        }
        const PyRef name(field_attribute(field, g_table.field_name_offset, g_names.name));
        if (name.get() != PyTuple_GET_ITEM(states, 2 * index))
            return false;
        const PyRef kind(field_attribute(field, g_table.field_kind_offset, g_names.field_kind));
        if (kind.get() != PyTuple_GET_ITEM(states, 2 * index + 1))
            return false;
    }
    return true;
}

bool intern(PyObject*& slot, const char* text) noexcept {
    slot = PyUnicode_InternFromString(text);
    return slot != nullptr;
}

} // namespace

bool g_runtime_ready = false;
PyObject* g_set_dummy = nullptr;

bool prepare_native_runtime() noexcept {
    // A runtime finalized and initialized again (embedding) runs module init
    // again: whatever the table held belonged to the finalized runtime and is
    // dropped, never released.
    g_table = Table{};
    g_runtime_ready = false;
    g_set_dummy = nullptr;
#if PY_VERSION_HEX >= 0x030A0000 && PY_VERSION_HEX < 0x030F0000 && !defined(Py_GIL_DISABLED)
    // The set walk's proof allocates (sets are tracked), so it runs here,
    // before any walk, as prepare_dumps_runtime's layout proof does. A
    // refused proof is not an import failure: sets keep their iterator.
    g_set_dummy = probe_set_dummy();
    if (g_set_dummy == nullptr)
        PyErr_Clear();
#endif
    for (Verdict& slot : g_verdicts)
        slot = Verdict{};
    g_tag_check = TagCheck::Off;
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
        intern(g_names.field_class, "Field") && intern(g_names.field_kind, "_field_type") &&
        intern(g_names.value, "value") && intern(g_names.value_slot, "_value_") &&
        intern(g_names.get, "__get__") && intern(g_names.fget, "fget") &&
        intern(g_names.utcoffset, "utcoffset") && intern(g_names.item, "item") &&
        intern(g_names.tolist, "tolist") && intern(g_names.dtype, "dtype") &&
        intern(g_names.kind, "kind") && intern(g_names.scalar_type, "type") &&
        intern(g_names.number, "num");
    if (g_runtime_ready)
        g_tag_check = prove_tag_check();
    return g_runtime_ready;
}

size_t format_pure_leaf(PyObject* object, char* out) noexcept {
    // An opt-out walk live anywhere: the mode lookup is classify's, latched.
    if (g_opt_outs.load(std::memory_order_relaxed) != 0)
        return 0;
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

namespace {

/**
 * `getattr(cls, name)` without running anything, where that is exact: the
 * metaclass is exactly `type`, which (like `object`) holds no attribute of
 * the names read here and cannot be given one, so the class's own MRO
 * decides -- an object found there with no `__get__` is the value (@p value,
 * borrowed; 1), nothing found is the AttributeError (0). -1 when only the
 * generic read can say: another metaclass, or a descriptor to call.
 */
int plain_class_attribute(PyTypeObject* cls, PyObject* name, PyObject*& value) noexcept {
#if defined(Py_GIL_DISABLED)
    (void)cls;
    (void)name;
    (void)value;
    return -1;
#else
    if (Py_TYPE(cls) != &PyType_Type)
        return -1;
    PyObject* const found = _PyType_Lookup(cls, name);
    if (found == nullptr)
        return 0;
    if (Py_TYPE(found)->tp_descr_get != nullptr)
        return -1;
    value = found;
    return 1;
#endif
}

/**
 * Whether a check of the precedence can run on the table as it stands.
 * Resolved groups can. Eagerly (after resolve()), an unresolved group cannot:
 * its module was not loaded, or its classes were not found. Lazily, an
 * unresolved group is probed in `sys.modules` -- one lookup in an exact dict,
 * which runs nothing: a module that is not loaded has no instances here, so the
 * check is skipped -- and a loaded one sets @p stop: resolving it runs code, so
 * the lazy evaluation gives up and the caller takes the eager order.
 */
bool usable(bool resolved, PyObject* module_name, bool lazy, bool& stop) {
    if (resolved)
        return true;
    if (!lazy)
        return false;
    PyObject* const module = loaded_module(module_name);
    if (module == nullptr)
        return false;
    Py_DECREF(module); // sys.modules still holds it
    stop = true;
    return false;
}

/**
 * The record's precedence over the type table as it stands, into @p kind.
 *
 * Lazily (@p lazy), a group is looked at only when every check ahead of it
 * has failed, so an Enum member never probes `sys.modules` for numpy -- a
 * lookup that missed on every conversion while numpy was not imported. It
 * returns false, deciding nothing, at the first group whose module is loaded
 * but not resolved; the caller then resolves every group and evaluates
 * eagerly, which is exactly the order classify had before: resolve(), then
 * the checks against the object's type as it is after resolution. Eagerly it
 * always decides. The two orders agree on every result: a lazy pass decides
 * only when each group ahead of the match is resolved or not loaded, which is
 * the state resolve() would leave them in. What the lazy pass leaves out is
 * the probes, and a loaded module's resolution, for groups behind the match --
 * resolution the next object of such a group performs (docs/decisions.md).
 */
bool evaluate(PyObject* object, bool lazy, Kind& kind) {
    bool stop = false;
    PyTypeObject* const type = Py_TYPE(object);
    // Held to the end: the attribute reads below can run code that reassigns
    // `object.__class__`, and the type may then have no other owner.
    const PyRef held(Py_NewRef(reinterpret_cast<PyObject*>(type)));
    kind = Kind::None;
    // The exact types only: a subclass can carry state its C fields do not
    // (pandas' `NaT`, a `Timestamp`'s nanoseconds), so it is unsupported.
    if (usable(g_table.datetime_api != nullptr, g_names.datetime_module, lazy, stop)) {
        const PyDateTime_CAPI* const api = g_table.datetime_api;
        if (type == api->DateTimeType)
            kind = Kind::DateTime;
        else if (type == api->DateType)
            kind = Kind::Date;
        else if (type == api->TimeType)
            kind = Kind::Time;
        if (kind != Kind::None)
            return true;
    }
    if (stop)
        return false;
    if (usable(g_table.uuid_type != nullptr, g_names.uuid_module, lazy, stop) &&
        PyType_IsSubtype(type, g_table.uuid_type)) {
        kind = Kind::Uuid;
        return true;
    }
    if (stop)
        return false;
    if (usable(g_table.decimal_type != nullptr, g_names.decimal_module, lazy, stop) &&
        PyType_IsSubtype(type, g_table.decimal_type)) {
        kind = Kind::Decimal;
        return true;
    }
    if (stop)
        return false;
    if (usable(g_table.enum_type != nullptr, g_names.enum_module, lazy, stop) &&
        PyType_IsSubtype(type, g_table.enum_type)) {
        kind = Kind::Enum;
        return true;
    }
    if (stop)
        return false;
    if (usable(g_table.dataclasses_fields != nullptr, g_names.dataclasses_module, lazy, stop) &&
        !PyType_Check(object)) {
        PyObject* declared = nullptr; // borrowed, unused: presence is the test
        int found = plain_class_attribute(type, g_names.dataclass_fields, declared);
        if (found < 0)
            found = has_attribute(reinterpret_cast<PyObject*>(type), g_names.dataclass_fields);
        if (found != 0) {
            kind = found < 0 ? Kind::Error : Kind::Dataclass;
            return true;
        }
    }
    if (stop)
        return false;
    if (PyAnySet_Check(object)) {
        kind = Kind::Set;
        return true;
    }
    if (usable(g_table.numpy_generic != nullptr, g_names.numpy_module, lazy, stop) &&
        is_numpy(object)) {
        kind = numpy_kind(object, PyType_IsSubtype(type, g_table.numpy_ndarray) != 0);
        return true;
    }
    return !stop;
}

/**
 * Keep @p kind as @p type's verdict when nothing but the type decided it and
 * nothing the table can still learn could change it: every group ahead of the
 * verdict's in the precedence is resolved (resolution is permanent), and for a
 * dataclass or set the `__dataclass_fields__` read ran no code
 * (plain_class_attribute). Error, None and the numpy kinds are never kept.
 */
void remember(PyTypeObject* type, Kind kind) {
    if (g_tag_check == TagCheck::Off)
        return;
    const Table& table = g_table;
    const bool datetime = table.datetime_api != nullptr;
    const bool through_uuid = datetime && table.uuid_type != nullptr;
    const bool through_decimal = through_uuid && table.decimal_type != nullptr;
    const bool through_enum = through_decimal && table.enum_type != nullptr;
    const bool through_dataclass = through_enum && table.dataclasses_fields != nullptr;
    PyObject* declared = nullptr;
    switch (kind) {
    case Kind::DateTime:
    case Kind::Date:
    case Kind::Time:
        break;
    case Kind::Uuid:
        if (!datetime)
            return;
        break;
    case Kind::Decimal:
        if (!through_uuid)
            return;
        break;
    case Kind::Enum:
        if (!through_decimal)
            return;
        break;
    case Kind::Dataclass:
        if (!through_enum || plain_class_attribute(type, g_names.dataclass_fields, declared) != 1)
            return;
        break;
    case Kind::Set:
        if (!through_dataclass ||
            plain_class_attribute(type, g_names.dataclass_fields, declared) != 0)
            return;
        break;
    default:
        return;
    }
    const unsigned int tag = assign_tag(type);
    if (tag == 0)
        return;
    verdict_slot(type) = Verdict{type, tag, kind, kind == Kind::Enum && stock_enum_type(type)};
}

} // namespace

Kind classify(PyObject* object) {
    if (g_opt_outs.load(std::memory_order_relaxed) != 0) {
        const int off = natives_off();
        if (off < 0)
            return Kind::Error;
        if (off != 0)
            return Kind::None;
    }
    PyTypeObject* const type = Py_TYPE(object);
    if (const Verdict* const verdict = cached_verdict(type))
        return verdict->kind;
    Kind kind = Kind::None;
    if (!evaluate(object, /*lazy=*/true, kind)) {
        resolve();
        (void)evaluate(object, /*lazy=*/false, kind);
    }
    // Code the evaluation ran may have reassigned `object.__class__`: the
    // verdict is then the new type's, and neither is remembered.
    if (Py_TYPE(object) == type)
        remember(type, kind);
    return kind;
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

PyObject* enum_value(PyObject* member) {
    // `getattr(member, "value")`, read as the stock descriptor would read it
    // when it is provably the one in place (stock_enum_value).
    return PyObject_GetAttr(member,
                            stock_enum_value(Py_TYPE(member)) ? g_names.value_slot : g_names.value);
}

PyObject* dataclass_field_names(PyObject* object, PyObject*& keys) {
    keys = nullptr;
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
    // What `dataclasses.fields` reads: it lists this dict's values, in order.
    // A cached entry stands while an exact dict holds the same keys and
    // fields, by identity and in order; anything else is a miss.
    PyObject* plain = nullptr;
    const PyRef declared(plain_class_attribute(reinterpret_cast<PyTypeObject*>(type),
                                               g_names.dataclass_fields, plain) == 1
                             ? Py_NewRef(plain)
                             : PyObject_GetAttr(type, g_names.dataclass_fields));
    if (!declared)
        return nullptr;
    const bool exact = PyDict_CheckExact(declared.get());
    if (exact) {
        PyObject* const cached = PyDict_GetItemWithError(cache, type);
        if (cached != nullptr && fields_match(declared.get(), PyTuple_GET_ITEM(cached, 0)) &&
            field_states_match(PyTuple_GET_ITEM(cached, 0), PyTuple_GET_ITEM(cached, 3))) {
            PyObject* const encoded = PyTuple_GET_ITEM(cached, 2);
            keys = encoded == Py_None ? nullptr : Py_NewRef(encoded);
            return Py_NewRef(PyTuple_GET_ITEM(cached, 1));
        }
        if (cached == nullptr && PyErr_Occurred())
            return nullptr;
    }
    // Taken before the calls below, which run code that can change the dict.
    const PyRef pairs(exact ? field_pairs(declared.get()) : nullptr);
    if (exact && !pairs)
        return nullptr;
    // Each field's name and kind, taken with the pairs: an entry is kept only
    // if they too are unchanged once the names are listed.
    const PyRef states(exact ? field_states(pairs.get()) : nullptr);
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
    // Not an exact dict, or changed while listed: nothing stable to check an entry against.
    if (!exact || !states || !fields_match(declared.get(), pairs.get()) ||
        !field_states_match(pairs.get(), states.get()))
        return names.release();
    const PyRef encoded(encode_keys(names.get()));
    if (!encoded)
        return nullptr;
    const PyRef entry(PyTuple_Pack(4, pairs.get(), names.get(), encoded.get(), states.get()));
    if (!entry)
        return nullptr;
    if (PyDict_GET_SIZE(cache) >= kFieldCacheLimit)
        PyDict_Clear(cache);
    if (PyDict_SetItem(cache, type, entry.get()) < 0)
        return nullptr;
    keys = encoded.get() == Py_None ? nullptr : Py_NewRef(encoded.get());
    return names.release();
}

PyObject* field_value(PyObject* object, PyObject* name) { return PyObject_GetAttr(object, name); }

PyObject* numpy_plain(PyObject* object, Kind kind) {
    switch (kind) {
    case Kind::NumpyBool: {
        const int truth = PyObject_IsTrue(object);
        return truth < 0 ? nullptr : Py_NewRef(truth != 0 ? Py_True : Py_False);
    }
    case Kind::NumpyInteger:
        return PyNumber_Index(object);
    case Kind::NumpyFloat:
        return PyNumber_Float(object);
    case Kind::NumpyArray:
        return PyObject_CallMethodNoArgs(object, g_names.tolist);
    default:
        return PyObject_CallMethodNoArgs(object, g_names.item);
    }
}

} // namespace strata::bindings::native

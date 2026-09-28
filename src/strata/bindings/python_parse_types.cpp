/**
 * @file python_parse_types.cpp
 * @brief The `parse_types` option, its revival walk, its lazy iterator and the
 * entry points that use them (docs/architecture/native_types.md, "Parse side").
 *
 * The walk runs over a tree the builder has just returned, before the caller
 * holds it, and replaces in place: list slots, and the values of keys a dict
 * already has. It is post-order, so a container's contents are revived before
 * the container is handed to a registered type. A registered type runs user
 * code (an `Enum` lookup, a dataclass's `__init__` and `__post_init__`), and
 * anything that allocates can run a finalizer, so the walk holds a strong
 * reference to every entry it converts, re-reads a list's size at every step,
 * and writes a result back only while the slot or key still holds the object
 * it read.
 *
 * The parse side's one translation unit that includes `datetime.h`: its
 * constructors come from the `datetime_CAPI` capsule, which the first call
 * imports. The serializer's type table (python_native_types.cpp) only reads
 * `sys.modules` and has no constructor to share.
 */

#include "python_parse_types.h"

#include "strata/util/folder.hpp"
#include "strata/util/temporal.hpp"

#include <cstdint>
#include <cstring>
#include <datetime.h>
#include <new>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace strata::bindings::parse_types {

namespace {

/// The widest offset the grammar admits, `±23:59`, in minutes.
constexpr int32_t kMaxOffsetMinutes = 23 * 60 + 59;

/// The shortest recognizable string (`HH:MM:SS`) and the longest (a UUID).
constexpr Py_ssize_t kShortestText = 8;
constexpr Py_ssize_t kLongestText = 36;

/// What the first call with `parse_types` set resolves, held for the process.
struct Runtime {
    PyDateTime_CAPI* api = nullptr;
    PyObject* uuid_class = nullptr;
    PyObject* int_keyword = nullptr; ///< the kwnames tuple `("int",)`
    PyObject* sixty_four = nullptr;
    /// Fixed-offset `timezone` objects by offset, created on first use.
    PyObject* zones[2 * kMaxOffsetMinutes + 1] = {};
};

Runtime g_runtime{};

/// Import `datetime`'s C API and `uuid.UUID` if this is the first call.
[[nodiscard]] bool ensure_runtime() {
    if (g_runtime.api != nullptr)
        return true;
    auto* const api = static_cast<PyDateTime_CAPI*>(PyCapsule_Import(PyDateTime_CAPSULE_NAME, 0));
    if (api == nullptr)
        return false;
    PyRef uuid_module(PyImport_ImportModule("uuid"));
    if (!uuid_module)
        return false;
    PyRef uuid_class(PyObject_GetAttrString(uuid_module.get(), "UUID"));
    if (!uuid_class)
        return false;
    PyRef keyword(PyUnicode_InternFromString("int"));
    if (!keyword)
        return false;
    PyRef kwnames(PyTuple_Pack(1, keyword.get()));
    PyRef sixty_four(PyLong_FromLong(64));
    if (!kwnames || !sixty_four)
        return false;
    PyDateTimeAPI = api;
    g_runtime.uuid_class = uuid_class.release();
    g_runtime.int_keyword = kwnames.release();
    g_runtime.sixty_four = sixty_four.release();
    g_runtime.api = api;
    return true;
}

/// `sys.modules[name]` as a new reference, or nullptr. Never raises.
[[nodiscard]] PyObject* loaded_module(const char* name) {
    PyObject* const modules = PyImport_GetModuleDict();
    if (modules == nullptr || !PyDict_Check(modules))
        return nullptr;
    PyObject* const module = PyDict_GetItemString(modules, name);
    return module == nullptr ? nullptr : Py_NewRef(module);
}

// ---------------------------------------------------------------------------
// Recognition
// ---------------------------------------------------------------------------

/// `timezone.utc` for 0, otherwise a fixed-offset `timezone`. A new reference.
[[nodiscard]] PyObject* zone(int32_t minutes) {
    PyDateTime_CAPI* const api = g_runtime.api;
    if (minutes == 0)
        return Py_NewRef(api->TimeZone_UTC);
    PyObject** const slot = minutes >= -kMaxOffsetMinutes && minutes <= kMaxOffsetMinutes
                                ? &g_runtime.zones[minutes + kMaxOffsetMinutes]
                                : nullptr;
    if (slot != nullptr && *slot != nullptr)
        return Py_NewRef(*slot);
    PyRef delta(api->Delta_FromDelta(0, minutes * 60, 0, 1, api->DeltaType));
    if (!delta)
        return nullptr;
    PyObject* const made = api->TimeZone_FromTimeZone(delta.get(), nullptr);
    if (made != nullptr && slot != nullptr)
        *slot = Py_NewRef(made);
    return made;
}

/// `uuid.UUID(int=hi << 64 | lo)`, a new reference.
[[nodiscard]] PyObject* make_uuid(uint64_t hi, uint64_t lo) {
    PyRef high(PyLong_FromUnsignedLongLong(hi));
    if (!high)
        return nullptr;
    PyRef shifted(PyNumber_Lshift(high.get(), g_runtime.sixty_four));
    PyRef low(PyLong_FromUnsignedLongLong(lo));
    if (!shifted || !low)
        return nullptr;
    PyRef value(PyNumber_Or(shifted.get(), low.get()));
    if (!value)
        return nullptr;
    PyObject* arguments[2] = {nullptr, value.get()};
    return PyObject_Vectorcall(g_runtime.uuid_class, arguments + 1, PY_VECTORCALL_ARGUMENTS_OFFSET,
                               g_runtime.int_keyword);
}

/**
 * The object @p text names when it is a date, time, date-time or UUID
 * (strata::util::scan_temporal), else @p text itself: a new reference, or
 * nullptr with an error set. A value the constructors refuse stays a `str`.
 */
[[nodiscard]] PyObject* recognize(PyObject* text) {
#if PY_VERSION_HEX < 0x030C0000
    if (PyUnicode_READY(text) < 0)
        return nullptr;
#endif
    const Py_ssize_t length = PyUnicode_GET_LENGTH(text);
    if (length < kShortestText || length > kLongestText || !PyUnicode_IS_ASCII(text))
        return Py_NewRef(text);
    const util::TemporalFields fields = util::scan_temporal(std::string_view(
        static_cast<const char*>(PyUnicode_DATA(text)), static_cast<size_t>(length)));

    PyDateTime_CAPI* const api = g_runtime.api;
    PyObject* made = nullptr;
    switch (fields.kind) {
    case util::TemporalKind::None:
        return Py_NewRef(text);
    case util::TemporalKind::Date:
        made = api->Date_FromDate(fields.year, fields.month, fields.day, api->DateType);
        break;
    case util::TemporalKind::Time:
    case util::TemporalKind::DateTime: {
        PyRef tzinfo(fields.offset == util::TemporalOffset::Minutes ? zone(fields.offset_minutes)
                                                                    : Py_NewRef(Py_None));
        if (!tzinfo)
            return nullptr;
        made = fields.kind == util::TemporalKind::Time
                   ? api->Time_FromTime(fields.hour, fields.minute, fields.second,
                                        fields.microsecond, tzinfo.get(), api->TimeType)
                   : api->DateTime_FromDateAndTime(
                         fields.year, fields.month, fields.day, fields.hour, fields.minute,
                         fields.second, fields.microsecond, tzinfo.get(), api->DateTimeType);
        break;
    }
    case util::TemporalKind::Uuid:
        made = make_uuid(fields.uuid_hi, fields.uuid_lo);
        break;
    }
    if (made == nullptr && PyErr_ExceptionMatches(PyExc_ValueError)) {
        PyErr_Clear();
        return Py_NewRef(text);
    }
    return made;
}

// ---------------------------------------------------------------------------
// The registry
// ---------------------------------------------------------------------------

/// 1 when @p value is an `Enum` subclass, 0 when it is not. Never raises.
[[nodiscard]] int is_enum_type(PyObject* value) {
    if (!PyType_Check(value))
        return 0;
    PyRef module(loaded_module("enum"));
    if (!module)
        return 0;
    PyRef base(PyObject_GetAttrString(module.get(), "Enum"));
    if (!base) {
        PyErr_Clear();
        return 0;
    }
    if (!PyType_Check(base.get()))
        return 0;
    return PyType_IsSubtype(reinterpret_cast<PyTypeObject*>(value),
                            reinterpret_cast<PyTypeObject*>(base.get()));
}

/// Add `field.name` to @p init_names when `field.init`, and to @p required
/// when it has neither a default nor a default factory.
[[nodiscard]] bool add_field(PyObject* field, PyObject* missing, PyObject* init_names,
                             PyObject* required) {
    PyRef init(PyObject_GetAttrString(field, "init"));
    if (!init)
        return false;
    const int is_init = PyObject_IsTrue(init.get());
    if (is_init <= 0)
        return is_init == 0;
    PyRef name(PyObject_GetAttrString(field, "name"));
    PyRef default_value(PyObject_GetAttrString(field, "default"));
    PyRef default_factory(PyObject_GetAttrString(field, "default_factory"));
    if (!name || !default_value || !default_factory)
        return false;
    if (PySet_Add(init_names, name.get()) != 0)
        return false;
    if (default_value.get() == missing && default_factory.get() == missing)
        return PySet_Add(required, name.get()) == 0;
    return true;
}

/**
 * For a dataclass type, `(type, init field names, init fields without a
 * default)` with the two sets frozen: the fields `dataclasses.fields()` lists
 * whose `init` is true. nullptr with no error set when @p value is not a
 * dataclass type; nullptr with an error set when reading it raised.
 */
[[nodiscard]] PyObject* dataclass_entry(PyObject* value) {
    if (!PyType_Check(value) || !PyObject_HasAttrString(value, "__dataclass_fields__"))
        return nullptr;
    PyRef module(loaded_module("dataclasses"));
    if (!module)
        return nullptr;
    PyRef missing(PyObject_GetAttrString(module.get(), "MISSING"));
    if (!missing)
        return nullptr;
    PyRef fields(PyObject_CallMethod(module.get(), "fields", "O", value));
    if (!fields)
        return nullptr;
    PyRef iterator(PyObject_GetIter(fields.get()));
    PyRef init_names(PySet_New(nullptr));
    PyRef required(PySet_New(nullptr));
    if (!iterator || !init_names || !required)
        return nullptr;
    while (PyRef field{PyIter_Next(iterator.get())}) {
        if (!add_field(field.get(), missing.get(), init_names.get(), required.get()))
            return nullptr;
    }
    if (PyErr_Occurred())
        return nullptr;
    PyRef frozen_init(PyFrozenSet_New(init_names.get()));
    PyRef frozen_required(PyFrozenSet_New(required.get()));
    if (!frozen_init || !frozen_required)
        return nullptr;
    return PyTuple_Pack(3, value, frozen_init.get(), frozen_required.get());
}

/**
 * The validated option: `True`, or a registry. The registry is a private
 * snapshot, `member name -> (type, init names, required names)`, with the two
 * sets `None` for an `Enum`; no caller code can reach it, so a mutation of the
 * caller's dict during the walk changes nothing.
 */
class Option {
  public:
    /// Validate @p value (anything but `False`) with the record's messages.
    [[nodiscard]] bool init(PyObject* value) {
        if (value != Py_True && !PyDict_Check(value)) {
            PyErr_Format(PyExc_TypeError, "parse_types must be a bool or a dict, not %s",
                         Py_TYPE(value)->tp_name);
            return false;
        }
        if (PyDict_Check(value) && !build_registry(value))
            return false;
        return ensure_runtime();
    }

    /// The registry, or nullptr when there is none (`True`, or an empty dict).
    [[nodiscard]] PyObject* registry() const noexcept { return registry_.get(); }

  private:
    [[nodiscard]] bool build_registry(PyObject* value) {
        PyRef snapshot(PyDict_Copy(value));
        PyRef entries(PyDict_New());
        if (!snapshot || !entries)
            return false;
        Py_ssize_t position = 0;
        PyObject* name = nullptr;
        PyObject* type = nullptr;
        while (PyDict_Next(snapshot.get(), &position, &name, &type)) {
            if (!PyUnicode_Check(name)) {
                PyErr_Format(PyExc_TypeError, "parse_types keys must be str, not %s",
                             Py_TYPE(name)->tp_name);
                return false;
            }
            PyRef entry(is_enum_type(type) ? PyTuple_Pack(3, type, Py_None, Py_None)
                                           : dataclass_entry(type));
            if (!entry) {
                if (!PyErr_Occurred())
                    PyErr_Format(PyExc_TypeError,
                                 "parse_types values must be Enum subclasses or dataclass "
                                 "types, not %R",
                                 type);
                return false;
            }
            if (PyDict_SetItem(entries.get(), name, entry.get()) != 0)
                return false;
        }
        if (PyDict_GET_SIZE(entries.get()) > 0)
            registry_ = std::move(entries);
        return true;
    }

    PyRef registry_;
};

// ---------------------------------------------------------------------------
// The walk
// ---------------------------------------------------------------------------

/// 1 when every key of @p dict is in @p init_names and every name in
/// @p required is a key of @p dict; 0 when not; -1 with an error set.
[[nodiscard]] int fits(PyObject* dict, PyObject* init_names, PyObject* required) {
    Py_ssize_t position = 0;
    PyObject* key = nullptr;
    PyObject* unused = nullptr;
    while (PyDict_Next(dict, &position, &key, &unused)) {
        const int known = PySet_Contains(init_names, key);
        if (known <= 0)
            return known;
    }
    PyRef names(PyObject_GetIter(required));
    if (!names)
        return -1;
    while (PyRef name{PyIter_Next(names.get())}) {
        const int present = PyDict_Contains(dict, name.get());
        if (present <= 0)
            return present;
    }
    return PyErr_Occurred() ? -1 : 1;
}

/**
 * @p object, already walked, as the registry @p entry makes it: `E(object)`
 * for an `Enum` (a `ValueError` leaves @p object), `D(**object)` for a
 * dataclass when @p object is a dict that fits, else @p object itself. A new
 * reference, or nullptr. The caller holds both arguments: this runs user code.
 */
[[nodiscard]] PyObject* convert(PyObject* object, PyObject* entry) {
    PyObject* const type = PyTuple_GET_ITEM(entry, 0);
    PyObject* const init_names = PyTuple_GET_ITEM(entry, 1);
    if (init_names == Py_None) {
        PyObject* const member = PyObject_CallOneArg(type, object);
        if (member == nullptr && PyErr_ExceptionMatches(PyExc_ValueError)) {
            PyErr_Clear(); // no member has that value
            return Py_NewRef(object);
        }
        return member;
    }
    if (!PyDict_CheckExact(object))
        return Py_NewRef(object);
    const int fit = fits(object, init_names, PyTuple_GET_ITEM(entry, 2));
    if (fit <= 0)
        return fit == 0 ? Py_NewRef(object) : nullptr;
    PyRef no_arguments(PyTuple_New(0));
    if (!no_arguments)
        return nullptr;
    return PyObject_Call(type, no_arguments.get(), object);
}

/**
 * The revival walk, iterative over an explicit stack of the containers it has
 * open: a level costs a stack entry, not C stack, so a document at the parse
 * cap revives on any thread that can parse it.
 *
 * Post-order: a container under a registered name is converted, and written
 * back into its parent, only once its last child is done. Every open
 * container, the child being revived and the registry entry being applied are
 * held by strong references across each conversion, which runs user code that
 * can clear the containers or the private registry (reachable through
 * `gc.get_objects()`).
 */
class Walk {
  public:
    explicit Walk(PyObject* registry) noexcept : registry_(registry) {}

    /// Revive @p object, which is not under a member name: a new reference to
    /// what replaces it (@p object itself when nothing does), or nullptr.
    [[nodiscard]] PyObject* value(PyObject* object) {
        if (PyUnicode_CheckExact(object))
            return recognize(object);
        if (!is_container(object))
            return Py_NewRef(object);
        open(PyRef(Py_NewRef(object)), PyRef(), PyRef(), PyRef(), 0);
        return run() ? Py_NewRef(object) : nullptr;
    }

  private:
    /// One open container, and where its result goes in its parent.
    struct Frame {
        PyRef container;     ///< the dict or list being walked
        PyRef element_entry; ///< a registered name's list: the entry each element is revived by
        PyRef own_entry;     ///< the entry the container itself is converted by once walked
        PyRef key;           ///< under a dict: the member name it is the value of
        Py_ssize_t index;    ///< under a list: its slot
        Py_ssize_t position; ///< the next child: PyDict_Next's position, or a list index
    };

    [[nodiscard]] static bool is_container(PyObject* object) noexcept {
        return PyDict_CheckExact(object) || PyList_CheckExact(object);
    }

    void open(PyRef container, PyRef element_entry, PyRef own_entry, PyRef key, Py_ssize_t index) {
        stack_.push_back(Frame{std::move(container), std::move(element_entry), std::move(own_entry),
                               std::move(key), index, 0});
    }

    /// Walk until the root closes. False with an error set.
    [[nodiscard]] bool run() {
        while (!stack_.empty()) {
            Frame& top = stack_.back();
            PyObject* const container = top.container.get();
            PyRef key;
            PyRef item;
            Py_ssize_t index = 0;
            if (PyDict_CheckExact(container)) {
                PyObject* borrowed_key = nullptr;
                PyObject* borrowed_value = nullptr;
                if (!PyDict_Next(container, &top.position, &borrowed_key, &borrowed_value)) {
                    if (!close())
                        return false;
                    continue;
                }
                key = PyRef(Py_NewRef(borrowed_key));
                item = PyRef(Py_NewRef(borrowed_value));
            } else {
                // Re-read at every step: user code may have shrunk the list.
                if (top.position >= PyList_GET_SIZE(container)) {
                    if (!close())
                        return false;
                    continue;
                }
                index = top.position++;
                item = PyRef(Py_NewRef(PyList_GET_ITEM(container, index)));
            }
            if (!child(std::move(key), index, std::move(item)))
                return false;
        }
        return true;
    }

    /**
     * One child of the top container: a dict member (@p key) by its name's
     * rule, or a list element (@p index) by the list's entry when the list is
     * a registered name's value (one level), else by the ordinary rule. A
     * container child is opened, and converted when it closes; its contents
     * always follow the ordinary rule.
     */
    [[nodiscard]] bool child(PyRef key, Py_ssize_t index, PyRef item) {
        PyRef entry;
        if (key) {
            if (registry_ != nullptr) {
                PyObject* const found = PyDict_GetItemWithError(registry_, key.get());
                if (found == nullptr && PyErr_Occurred())
                    return false;
                entry = PyRef(Py_XNewRef(found));
            }
            if (entry && PyList_CheckExact(item.get())) {
                open(std::move(item), std::move(entry), PyRef(), std::move(key), index);
                return true;
            }
        } else {
            entry = PyRef(Py_XNewRef(stack_.back().element_entry.get()));
        }
        if (is_container(item.get())) {
            open(std::move(item), PyRef(), std::move(entry), std::move(key), index);
            return true;
        }
        if (!entry && !PyUnicode_CheckExact(item.get()))
            return true;
        PyRef revived(entry ? convert(item.get(), entry.get()) : recognize(item.get()));
        if (!revived)
            return false;
        return store(key, index, item.get(), std::move(revived));
    }

    /// The top container is walked: pop it, and when it is under a registered
    /// name, convert it and write the result back into its parent.
    [[nodiscard]] bool close() {
        const Frame done = std::move(stack_.back());
        stack_.pop_back();
        if (!done.own_entry)
            return true; // the root, or a container nothing replaces
        PyRef result(convert(done.container.get(), done.own_entry.get()));
        if (!result)
            return false;
        return store(done.key, done.index, done.container.get(), std::move(result));
    }

    /// Write @p revived into the top container's slot -- @p key when set,
    /// else @p index -- only while that slot still holds @p item.
    [[nodiscard]] bool store(const PyRef& key, Py_ssize_t index, PyObject* item, PyRef revived) {
        if (revived.get() == item)
            return true;
        PyObject* const container = stack_.back().container.get();
        if (key) {
            PyObject* const current = PyDict_GetItemWithError(container, key.get());
            if (current == nullptr && PyErr_Occurred())
                return false;
            return current != item || PyDict_SetItem(container, key.get(), revived.get()) == 0;
        }
        if (index < PyList_GET_SIZE(container) && PyList_GET_ITEM(container, index) == item)
            PyList_SetItem(container, index, revived.release()); // steals; item still held
        return true;
    }

    PyObject* registry_;
    std::vector<Frame> stack_;
};

/// Revive a freshly parsed root. Consumes @p root; a new reference or nullptr.
[[nodiscard]] PyObject* revive(PyObject* root, PyObject* registry) {
    if (root == nullptr)
        return nullptr;
    PyRef owned(root);
    return Walk(registry).value(owned.get());
}

// ---------------------------------------------------------------------------
// Search
// ---------------------------------------------------------------------------

/**
 * `query_object(root, compiled)`, extended to a scalar root the way the
 * default `search` evaluates one: a scalar has no children, so only a path
 * that selects the root itself (`$`) matches it. Whether @p compiled is such a
 * path is read off an empty dict, which it matches exactly when it does.
 */
[[nodiscard]] PyObject* evaluate(PyObject* root, PyObject* compiled) {
    if (PyDict_Check(root) || PyList_Check(root) || PyTuple_Check(root))
        return query_object(root, compiled);
    PyRef probe(PyDict_New());
    if (!probe)
        return nullptr;
    PyRef found(query_object(probe.get(), compiled));
    if (!found)
        return nullptr;
    const bool root_only =
        PyList_GET_SIZE(found.get()) == 1 && PyList_GET_ITEM(found.get(), 0) == probe.get();
    PyObject* const matches = PyList_New(root_only ? 1 : 0);
    if (matches != nullptr && root_only)
        PyList_SET_ITEM(matches, 0, Py_NewRef(root));
    return matches;
}

/// One file's matches: load it whole, revive it, evaluate. @p is_directory
/// as in load_from_file.
[[nodiscard]] PyObject* search_one(const char* path, PyObject* compiled, PyObject* registry,
                                   bool* is_directory) {
    PyRef root(revive(
        load_from_file(path, "dict", /*iterator=*/false, /*skip_errors=*/false, is_directory),
        registry));
    if (!root)
        return nullptr;
    return evaluate(root.get(), compiled);
}

/// Discover, or set the folder readers' exception and return false.
[[nodiscard]] bool discover(const char* directory, std::vector<std::string>& files) {
    auto found = util::discover_json_files(directory);
    switch (found.status) {
    case util::DiscoveryStatus::Ok:
        files = std::move(found.files);
        return true;
    case util::DiscoveryStatus::NotADirectory:
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, directory);
        return false;
    case util::DiscoveryStatus::WalkFailed:
        PyErr_Format(PyExc_OSError, "cannot read %s: %s", directory, found.message.c_str());
        return false;
    }
    return false;
}

// ---------------------------------------------------------------------------
// The lazy iterator
// ---------------------------------------------------------------------------

/**
 * Two lazy shapes, one type. Revive mode wraps the NDJSON or folder iterator
 * `load` would return and revives each record as it is yielded. Search mode
 * walks a folder's discovered files one at a time, as the folder iterator
 * does, searching each with parse_types.
 */
struct RevivingIteratorObject {
    PyObject_HEAD PyObject* inner;   ///< revive mode: the wrapped iterator
    PyObject* registry;              ///< may be null
    std::vector<std::string>* files; ///< search mode: the discovered files
    size_t next_file;
    PyObject* compiled; ///< search mode: the compiled path
    PyObject* buffer;   ///< search mode: the current file's matches
    Py_ssize_t position;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
PyTypeObject kRevivingIteratorType = {PyVarObject_HEAD_INIT(nullptr, 0)};
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

void reviving_iterator_dealloc(PyObject* self) {
    auto* const iterator = reinterpret_cast<RevivingIteratorObject*>(self);
    Py_XDECREF(iterator->inner);
    Py_XDECREF(iterator->registry);
    delete iterator->files;
    Py_XDECREF(iterator->compiled);
    Py_XDECREF(iterator->buffer);
    PyObject_Free(self);
}

PyObject* reviving_iterator_self(PyObject* self) { return Py_NewRef(self); }

PyObject* reviving_iterator_next(PyObject* self) {
    STRATA_CPP_TRY
    auto* const iterator = reinterpret_cast<RevivingIteratorObject*>(self);
    if (iterator->inner != nullptr) {
        PyObject* const record = PyIter_Next(iterator->inner);
        return record == nullptr ? nullptr : revive(record, iterator->registry);
    }
    for (;;) {
        if (iterator->buffer != nullptr && iterator->position < PyList_GET_SIZE(iterator->buffer)) {
            PyObject* const match = PyList_GET_ITEM(iterator->buffer, iterator->position);
            ++iterator->position;
            return Py_NewRef(match);
        }
        Py_CLEAR(iterator->buffer);
        iterator->position = 0;
        if (iterator->next_file >= iterator->files->size())
            return nullptr; // exhausted
        const std::string& path = (*iterator->files)[iterator->next_file++];
        iterator->buffer =
            search_one(path.c_str(), iterator->compiled, iterator->registry, nullptr);
        if (iterator->buffer == nullptr)
            return nullptr;
    }
    STRATA_CPP_CATCH
}

[[nodiscard]] bool ready_iterator_type() {
    if ((kRevivingIteratorType.tp_flags & Py_TPFLAGS_READY) != 0)
        return true;
    kRevivingIteratorType.tp_name = "strata._strata.RevivingIterator";
    kRevivingIteratorType.tp_basicsize = sizeof(RevivingIteratorObject);
    kRevivingIteratorType.tp_dealloc = reviving_iterator_dealloc;
    kRevivingIteratorType.tp_flags = Py_TPFLAGS_DEFAULT;
    kRevivingIteratorType.tp_doc =
        PyDoc_STR("Lazy iterator that revives each record or searches each file (parse_types).");
    kRevivingIteratorType.tp_iter = reviving_iterator_self;
    kRevivingIteratorType.tp_iternext = reviving_iterator_next;
    return PyType_Ready(&kRevivingIteratorType) == 0;
}

[[nodiscard]] RevivingIteratorObject* new_iterator(PyObject* registry) {
    if (!ready_iterator_type())
        return nullptr;
    auto* const self = PyObject_New(RevivingIteratorObject, &kRevivingIteratorType);
    if (self == nullptr)
        return nullptr;
    self->inner = nullptr;
    self->registry = Py_XNewRef(registry);
    self->files = nullptr;
    self->next_file = 0;
    self->compiled = nullptr;
    self->buffer = nullptr;
    self->position = 0;
    return self;
}

/// Revive each item of @p inner as it is yielded. Consumes @p inner.
[[nodiscard]] PyObject* reviving_iterator(PyObject* inner, PyObject* registry) {
    PyRef owned(inner);
    if (!owned)
        return nullptr;
    RevivingIteratorObject* const self = new_iterator(registry);
    if (self == nullptr)
        return nullptr;
    self->inner = owned.release();
    return reinterpret_cast<PyObject*>(self);
}

/// Search @p files one at a time as the iterator is consumed.
[[nodiscard]] PyObject* searching_iterator(std::vector<std::string>&& files, PyObject* compiled,
                                           PyObject* registry) {
    RevivingIteratorObject* const self = new_iterator(registry);
    if (self == nullptr)
        return nullptr;
    self->compiled = Py_NewRef(compiled);
    self->files = new (std::nothrow) std::vector<std::string>(std::move(files));
    if (self->files == nullptr) {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }
    return reinterpret_cast<PyObject*>(self);
}

} // namespace

// ---------------------------------------------------------------------------
// Entry points
// ---------------------------------------------------------------------------

PyObject* loads(PyObject* source, bool want_cursor, bool iterator, PyObject* option) {
    Option parsed;
    if (!parsed.init(option))
        return nullptr;
    if (want_cursor) {
        PyErr_SetString(PyExc_ValueError, "parse_types needs return_type='dict'");
        return nullptr;
    }

    const char* text = nullptr;
    Py_ssize_t size = 0;
    if (PyUnicode_Check(source)) {
        text = PyUnicode_AsUTF8AndSize(source, &size);
        if (text == nullptr) {
            // Lone surrogates: not valid JSON text either way (strata_loads).
            PyErr_Clear();
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return nullptr;
        }
    } else if (PyBytes_Check(source)) {
        char* data = nullptr;
        if (PyBytes_AsStringAndSize(source, &data, &size) != 0)
            return nullptr;
        text = data;
    } else {
        PyErr_Format(PyExc_TypeError, "loads() expects str or bytes, not %s",
                     Py_TYPE(source)->tp_name);
        return nullptr;
    }

    PyRef value(revive(
        loads_to_python(std::string_view(text, static_cast<size_t>(size)), /*validate_utf8=*/false),
        parsed.registry()));
    if (!value || !iterator)
        return value.release();
    return make_root_iterator(value.get());
}

PyObject* load(const char* path, const char* return_type, bool iterator, bool skip_errors,
               PyObject* option) {
    Option parsed;
    if (!parsed.init(option))
        return nullptr;
    if (std::strcmp(return_type, "cursor") == 0) {
        PyErr_SetString(PyExc_ValueError, "parse_types needs return_type='dict'");
        return nullptr;
    }

    // A lazy NDJSON read stays lazy and is wrapped; a .json document is read
    // whole, revived, and only then handed to the root iterator.
    const bool lazy = iterator && file_is_ndjson(path);
    bool directory = false;
    PyObject* const loaded = load_from_file(path, return_type, lazy, skip_errors, &directory);
    if (directory) {
        if (std::strcmp(return_type, "dict") != 0) {
            PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
            return nullptr;
        }
        PyObject* const records = load_from_folder(path, iterator, skip_errors);
        return iterator ? reviving_iterator(records, parsed.registry())
                        : revive(records, parsed.registry());
    }
    if (lazy)
        return reviving_iterator(loaded, parsed.registry());
    PyRef value(revive(loaded, parsed.registry()));
    if (!value || !iterator)
        return value.release();
    return make_root_iterator(value.get());
}

PyObject* search(const char* path, PyObject* expression, bool iterator, PyObject* option) {
    Option parsed;
    if (!parsed.init(option))
        return nullptr;
    PyRef compiled(compile_expression(expression));
    if (!compiled)
        return nullptr;

    bool directory = false;
    PyRef matches(search_one(path, compiled.get(), parsed.registry(), &directory));
    if (directory) {
        std::vector<std::string> files;
        if (!discover(path, files))
            return nullptr;
        if (iterator)
            return searching_iterator(std::move(files), compiled.get(), parsed.registry());
        matches = PyRef(PyList_New(0));
        if (!matches)
            return nullptr;
        for (const std::string& file : files) {
            // Exactly search() on each file, in discovery order.
            PyRef found(search_one(file.c_str(), compiled.get(), parsed.registry(), nullptr));
            if (!found ||
                PyList_SetSlice(matches.get(), PY_SSIZE_T_MAX, PY_SSIZE_T_MAX, found.get()) != 0)
                return nullptr;
        }
    }
    if (!matches)
        return nullptr;
    return iterator ? PyObject_GetIter(matches.get()) : matches.release();
}

PyObject* query(PyObject* data, PyObject* expression, bool iterator, PyObject* option) {
    if (!PyBool_Check(option)) {
        PyErr_Format(PyExc_TypeError, "query() parse_types must be a bool, not %s",
                     Py_TYPE(option)->tp_name);
        return nullptr;
    }
    const bool recognizing = option == Py_True;
    if (recognizing && !ensure_runtime())
        return nullptr;
    PyRef matches(query_object(data, expression));
    if (!matches)
        return nullptr;
    // The result list is ours; the matches in it are the caller's objects and
    // are never mutated -- a str match is replaced in the list, not changed.
    for (Py_ssize_t index = 0; recognizing && index < PyList_GET_SIZE(matches.get()); ++index) {
        PyRef match(Py_NewRef(PyList_GET_ITEM(matches.get(), index)));
        if (!PyUnicode_Check(match.get()))
            continue;
        PyRef revived(recognize(match.get()));
        if (!revived)
            return nullptr;
        if (revived.get() != match.get() && index < PyList_GET_SIZE(matches.get()) &&
            PyList_GET_ITEM(matches.get(), index) == match.get())
            PyList_SetItem(matches.get(), index, revived.release());
    }
    return iterator ? PyObject_GetIter(matches.get()) : matches.release();
}

void reset_runtime() noexcept { g_runtime = Runtime{}; }

} // namespace strata::bindings::parse_types

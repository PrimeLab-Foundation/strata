/**
 * @file python_parse_types_walk.cpp
 * @brief The revival walk, the recognition it applies and the process-global
 * runtime both resolve (docs/architecture/native_types.md, "Parse side").
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
 * `sys.modules` and has no constructor to share. The option, the registry, the
 * entry points and the lazy iterator live in python_parse_types.cpp and reach
 * this file only through python_parse_types_walk.h.
 */

#include "python_parse_types_walk.h"

#include "python_parse_types.h"
#include "strata/util/temporal.hpp"

#include <cstdint>
#include <datetime.h>
#include <string_view>
#include <utility>
#include <vector>

namespace strata::bindings::parse_types {

/// The widest offset the grammar admits, `±23:59`, in minutes.
constexpr int32_t kMaxOffsetMinutes = 23 * 60 + 59;

/// The shortest recognizable string (`HH:MM:SS`) and the longest (a UUID).
constexpr Py_ssize_t kShortestText = 8;
constexpr Py_ssize_t kLongestText = 36;

namespace {

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

} // namespace

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
// The walk
// ---------------------------------------------------------------------------

namespace {

/// 1 when every key of @p dict is in @p init_names and every name in
/// @p required is a key of @p dict; 0 when not; -1 with an error set.
[[nodiscard]] int fits(PyObject* dict, PyObject* init_names, PyObject* required) {
    Py_ssize_t position = 0;
    PyObject* key = nullptr;
    PyObject* unused = nullptr;
    while (PyDict_Next(dict, &position, &key, &unused)) {
        // Hold the borrowed key across PySet_Contains: looking it up runs its
        // __hash__/__eq__, which for a str subclass is user code that could
        // drop the dict's only reference to it (the walk's invariant, above).
        PyRef held(Py_NewRef(key));
        const int known = PySet_Contains(init_names, held.get());
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

} // namespace

/// Revive a freshly parsed root. Consumes @p root; a new reference or nullptr.
[[nodiscard]] PyObject* revive(PyObject* root, PyObject* registry) {
    if (root == nullptr)
        return nullptr;
    PyRef owned(root);
    return Walk(registry).value(owned.get());
}

void reset_runtime() noexcept { g_runtime = Runtime{}; }

} // namespace strata::bindings::parse_types

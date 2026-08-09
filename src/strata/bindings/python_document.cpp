/**
 * @file python_document.cpp
 * @brief The `JsonCursor` Python type.
 *
 * A cursor is a borrowed pointer into a parsed tree plus a **share of that
 * tree's ownership**. Holding the share is what makes the C++
 * document-outlives-cursor invariant automatic here: a cursor cannot be left
 * pointing into freed memory, whatever the caller drops, and the user never
 * has to know a document object exists (docs/context/api.md, § Cursor).
 *
 * Navigating produces new cursors that share the same tree, so a chain like
 * `root.field("a").at(0)` keeps exactly one copy of the data alive.
 */

#include "python_types.h"
#include "strata/json/json_core.hpp"
#include "strata/json/json_cursor.hpp"

#include <memory>
#include <new>
#include <stdexcept>
#include <string>

namespace strata::bindings {

namespace {

struct CursorObject {
    PyObject_HEAD
        /// Share of the tree's ownership. Heap-allocated so the POD-ish PyObject
        /// can hold a type with a destructor.
        std::shared_ptr<const JsonValue>* owner;
    JsonCursor cursor;
};

PyTypeObject* cursor_type();

/// Build a cursor object over @p value, sharing ownership of @p owner.
[[nodiscard]] PyObject* make_cursor(const std::shared_ptr<const JsonValue>& owner,
                                    const JsonCursor& cursor) {
    auto* self = PyObject_New(CursorObject, cursor_type());
    if (self == nullptr)
        return nullptr;
    self->owner = new (std::nothrow) std::shared_ptr<const JsonValue>(owner);
    if (self->owner == nullptr) {
        PyObject_Free(self);
        return PyErr_NoMemory();
    }
    self->cursor = cursor;
    return reinterpret_cast<PyObject*>(self);
}

void cursor_dealloc(PyObject* self) {
    auto* cursor = reinterpret_cast<CursorObject*>(self);
    delete cursor->owner;
    PyObject_Free(self);
}

#define STRATA_PREDICATE(name, method)                                                             \
    PyObject* name(PyObject* self, PyObject*) {                                                    \
        const auto* object = reinterpret_cast<CursorObject*>(self);                                \
        if (object->cursor.method())                                                               \
            Py_RETURN_TRUE;                                                                        \
        Py_RETURN_FALSE;                                                                           \
    }

STRATA_PREDICATE(cursor_is_null, is_null)
STRATA_PREDICATE(cursor_is_bool, is_bool)
STRATA_PREDICATE(cursor_is_number, is_number)
STRATA_PREDICATE(cursor_is_string, is_string)
STRATA_PREDICATE(cursor_is_array, is_array)
STRATA_PREDICATE(cursor_is_object, is_object)

#undef STRATA_PREDICATE

PyObject* cursor_get_bool(PyObject* self, PyObject*) {
    STRATA_CPP_TRY
    const auto* object = reinterpret_cast<CursorObject*>(self);
    return PyBool_FromLong(object->cursor.get_bool_or_throw() ? 1 : 0);
    STRATA_CPP_CATCH
}

PyObject* cursor_get_int(PyObject* self, PyObject*) {
    STRATA_CPP_TRY
    const auto* object = reinterpret_cast<CursorObject*>(self);
    return PyLong_FromLongLong(object->cursor.get_int());
    STRATA_CPP_CATCH
}

PyObject* cursor_get_float(PyObject* self, PyObject*) {
    STRATA_CPP_TRY
    const auto* object = reinterpret_cast<CursorObject*>(self);
    return PyFloat_FromDouble(object->cursor.get_float());
    STRATA_CPP_CATCH
}

PyObject* cursor_get_str(PyObject* self, PyObject*) {
    STRATA_CPP_TRY
    const auto* object = reinterpret_cast<CursorObject*>(self);
    const std::string text = object->cursor.get_str();
    return PyUnicode_FromStringAndSize(text.data(), static_cast<Py_ssize_t>(text.size()));
    STRATA_CPP_CATCH
}

PyObject* cursor_field(PyObject* self, PyObject* args) {
    STRATA_CPP_TRY
    const char* key = nullptr;
    Py_ssize_t length = 0;
    if (!PyArg_ParseTuple(args, "s#", &key, &length))
        return nullptr;

    auto* object = reinterpret_cast<CursorObject*>(self);
    const JsonCursor child =
        object->cursor.field(std::string_view(key, static_cast<size_t>(length)));
    return make_cursor(*object->owner, child);
    STRATA_CPP_CATCH
}

PyObject* cursor_at(PyObject* self, PyObject* args) {
    STRATA_CPP_TRY
    Py_ssize_t index = 0;
    if (!PyArg_ParseTuple(args, "n", &index))
        return nullptr;

    auto* object = reinterpret_cast<CursorObject*>(self);
    if (index < 0) {
        // Negative indices count from the end, as they do everywhere in Python.
        index += static_cast<Py_ssize_t>(object->cursor.array_size());
        if (index < 0)
            throw std::out_of_range("index out of range");
    }
    const JsonCursor child = object->cursor.at(static_cast<size_t>(index));
    return make_cursor(*object->owner, child);
    STRATA_CPP_CATCH
}

PyMethodDef kCursorMethods[] = {
    {"is_null", cursor_is_null, METH_NOARGS, "is_null() -> bool"},
    {"is_bool", cursor_is_bool, METH_NOARGS, "is_bool() -> bool"},
    {"is_number", cursor_is_number, METH_NOARGS, "is_number() -> bool"},
    {"is_string", cursor_is_string, METH_NOARGS, "is_string() -> bool"},
    {"is_array", cursor_is_array, METH_NOARGS, "is_array() -> bool"},
    {"is_object", cursor_is_object, METH_NOARGS, "is_object() -> bool"},
    {"get_bool", cursor_get_bool, METH_NOARGS, "get_bool() -> bool"},
    {"get_int", cursor_get_int, METH_NOARGS, "get_int() -> int"},
    {"get_float", cursor_get_float, METH_NOARGS, "get_float() -> float"},
    {"get_str", cursor_get_str, METH_NOARGS, "get_str() -> str"},
    {"field", cursor_field, METH_VARARGS, "field(key) -> JsonCursor"},
    {"at", cursor_at, METH_VARARGS, "at(index) -> JsonCursor"},
    {nullptr, nullptr, 0, nullptr},
};

/// Filled in by register_cursor_type(): the HEAD_INIT macro cannot be mixed
/// with designated initialisers, and naming every slot positionally is worse.
// Leaving the tail of PyTypeObject zeroed is exactly what CPython prescribes
// for a static type, but the compiler cannot know that, so the warning is
// silenced here and nowhere wider.
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
PyTypeObject kCursorType = {PyVarObject_HEAD_INIT(nullptr, 0)};
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

PyTypeObject* cursor_type() { return &kCursorType; }

} // namespace

bool register_cursor_type(PyObject* module) {
    kCursorType.tp_name = "strata._strata.JsonCursor";
    kCursorType.tp_basicsize = sizeof(CursorObject);
    kCursorType.tp_dealloc = cursor_dealloc;
    kCursorType.tp_flags = Py_TPFLAGS_DEFAULT;
    kCursorType.tp_doc = PyDoc_STR("Lazy navigator over a parsed JSON document.");
    kCursorType.tp_methods = kCursorMethods;

    if (PyType_Ready(&kCursorType) < 0)
        return false;
    Py_INCREF(&kCursorType);
    if (PyModule_AddObject(module, "JsonCursor", reinterpret_cast<PyObject*>(&kCursorType)) < 0) {
        Py_DECREF(&kCursorType);
        return false;
    }
    return true;
}

PyObject* make_root_cursor(const std::shared_ptr<const JsonValue>& owner) {
    return make_cursor(owner, JsonCursor(owner.get()));
}

} // namespace strata::bindings

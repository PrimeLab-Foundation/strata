/**
 * @file python_parse_types.cpp
 * @brief The `parse_types` option and its private registry, the reviving
 * iterator and the four cold entry points that use them
 * (docs/architecture/native_types.md, "Parse side").
 *
 * Each entry point parses exactly as its default path does -- through the same
 * functions -- and then hands the tree to the revival walk in
 * python_parse_types_walk.cpp, reached through python_parse_types_walk.h. The
 * walk replaces, in place, the strings that name a date, time, date-time or
 * UUID, and the members a caller's registry names.
 *
 * The registry is a private snapshot built from the caller's dict, so a
 * mutation of the caller's dict during the walk changes nothing. `_strata`
 * only.
 */

#include "python_parse_types.h"

#include "python_parse_types_walk.h"
#include "strata/util/folder.hpp"

#include <cstring>
#include <new>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace strata::bindings::parse_types {

namespace {

/// `sys.modules[name]` as a new reference, or nullptr. Never raises.
[[nodiscard]] PyObject* loaded_module(const char* name) {
    PyObject* const modules = PyImport_GetModuleDict();
    if (modules == nullptr || !PyDict_Check(modules))
        return nullptr;
    PyObject* const module = PyDict_GetItemString(modules, name);
    return module == nullptr ? nullptr : Py_NewRef(module);
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

} // namespace strata::bindings::parse_types

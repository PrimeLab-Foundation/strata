/**
 * @file python_folder.cpp
 * @brief Folder mode: `load`, `dump(split_by=...)` and `search` over a directory.
 *
 * Folder mode is new in the target API, so every rule here comes from
 * docs/context/api.md rather than from prior code.
 *
 * Two laws shape the design:
 *
 * - `search(dir, e)` is the concatenation of `search(f, e)` over the discovered
 *   files, in discovery order.
 * - `dump(records, d, split_by=ks)` followed by `load(d)` returns the same
 *   records, grouped in bytewise key-path order with intra-group order kept.
 *
 * Both hold because a single discovery routine defines the order, and because
 * dump names its files from the same strings load will later sort by.
 */

#include "python_types.h"
#include "strata/util/folder.hpp"

#include <cerrno>
#include <cstdio>
#include <map>
#include <new>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#ifndef _WIN32
#include <sys/stat.h>
#endif

namespace strata::bindings {

namespace {

/// Discover, or set a Python exception and return false.
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

/// Extend @p out with one file's contribution to a folder load.
[[nodiscard]] bool extend_with_file(PyObject* out, const std::string& path, bool skip_errors) {
    PyRef loaded(load_from_file(path.c_str(), "dict", /*iterator=*/false, skip_errors));
    if (!loaded) {
        if (!skip_errors)
            return false;
        PyErr_Clear(); // the caller opted in to losing this file
        return true;
    }

    // A .json file holding a list contributes its elements; any other root
    // contributes itself. NDJSON already arrives as a list of its lines.
    if (PyList_Check(loaded.get()))
        return PyList_SetSlice(out, PY_SSIZE_T_MAX, PY_SSIZE_T_MAX, loaded.get()) == 0;
    return PyList_Append(out, loaded.get()) == 0;
}

/**
 * Walks the discovered files one at a time, yielding their records.
 *
 * Folder mode is the one place where laziness changes behaviour rather than
 * just timing: a directory can hold far more than fits in memory, and a
 * malformed file part-way through should surface only once iteration reaches
 * it. Discovery itself is eager, so a missing directory still raises at the
 * call rather than on the first `next()`.
 */
struct FolderIteratorObject {
    PyObject_HEAD std::vector<std::string>* files;
    size_t next_file;
    PyObject* buffer; ///< records or matches from the current file
    Py_ssize_t position;
    PyObject* expression; ///< null for load, a CompiledPath for search
    bool skip_errors;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
PyTypeObject kFolderIteratorType = {PyVarObject_HEAD_INIT(nullptr, 0)};
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

void folder_iterator_dealloc(PyObject* self) {
    auto* iterator = reinterpret_cast<FolderIteratorObject*>(self);
    delete iterator->files;
    Py_XDECREF(iterator->buffer);
    Py_XDECREF(iterator->expression);
    PyObject_Free(self);
}

PyObject* folder_iterator_self(PyObject* self) { return Py_NewRef(self); }

PyObject* folder_iterator_next(PyObject* self) {
    STRATA_CPP_TRY
    auto* iterator = reinterpret_cast<FolderIteratorObject*>(self);
    for (;;) {
        if (iterator->buffer != nullptr && iterator->position < PyList_GET_SIZE(iterator->buffer)) {
            PyObject* item = PyList_GET_ITEM(iterator->buffer, iterator->position);
            ++iterator->position;
            return Py_NewRef(item);
        }

        Py_CLEAR(iterator->buffer);
        iterator->position = 0;
        if (iterator->next_file >= iterator->files->size())
            return nullptr; // exhausted

        const std::string& path = (*iterator->files)[iterator->next_file++];
        PyObject* produced =
            iterator->expression != nullptr
                ? search_file(path.c_str(), iterator->expression)
                : load_from_file(path.c_str(), "dict", /*iterator=*/false, iterator->skip_errors);

        if (produced == nullptr) {
            // Folder search has no skip_errors, so a bad file always stops it.
            if (iterator->expression != nullptr || !iterator->skip_errors)
                return nullptr;
            PyErr_Clear();
            continue;
        }

        if (PyList_Check(produced)) {
            iterator->buffer = produced;
        } else {
            // A non-list document contributes itself as one record.
            PyObject* wrapper = PyList_New(1);
            if (wrapper == nullptr) {
                Py_DECREF(produced);
                return nullptr;
            }
            PyList_SET_ITEM(wrapper, 0, produced); // steals
            iterator->buffer = wrapper;
        }
    }
    STRATA_CPP_CATCH
}

/// @param expression null for load mode, a compiled path for search mode.
[[nodiscard]] PyObject* make_folder_iterator(std::vector<std::string>&& files, PyObject* expression,
                                             bool skip_errors) {
    auto* self = PyObject_New(FolderIteratorObject, &kFolderIteratorType);
    if (self == nullptr)
        return nullptr;
    self->files = new (std::nothrow) std::vector<std::string>(std::move(files));
    if (self->files == nullptr) {
        PyObject_Free(self);
        return PyErr_NoMemory();
    }
    self->next_file = 0;
    self->buffer = nullptr;
    self->position = 0;
    self->expression = expression == nullptr ? nullptr : Py_NewRef(expression);
    self->skip_errors = skip_errors;
    return reinterpret_cast<PyObject*>(self);
}

// ---------------------------------------------------------------------------
// dump(records, dir, split_by=...)
// ---------------------------------------------------------------------------

/**
 * The JSON string form of a split value.
 *
 * Only `str`, `int` and `bool` are groupable, and each has exactly one
 * spelling: a string as itself, an integer in decimal, a boolean as `true` or
 * `false`. That is what makes two different raw values able to collide, which
 * is an error rather than a silent merge.
 */
[[nodiscard]] bool split_value_to_string(PyObject* value, std::string& out) {
    if (PyBool_Check(value)) {
        out = (value == Py_True) ? "true" : "false";
        return true;
    }
    if (PyLong_Check(value)) {
        PyRef text(PyObject_Str(value));
        if (!text)
            return false;
        Py_ssize_t size = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(text.get(), &size);
        if (utf8 == nullptr)
            return false;
        out.assign(utf8, static_cast<size_t>(size));
        return true;
    }
    if (PyUnicode_Check(value)) {
        Py_ssize_t size = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(value, &size);
        if (utf8 == nullptr)
            return false;
        out.assign(utf8, static_cast<size_t>(size));
        return true;
    }
    // The error contract says a wrong value type is a TypeError; api.md's
    // ValueError list covers missing keys and unsafe names, not wrong types.
    PyErr_Format(PyExc_TypeError, "split_by values must be str, int or bool, not %s",
                 Py_TYPE(value)->tp_name);
    return false;
}

/// The split keys, as a list of UTF-8 names.
[[nodiscard]] bool read_split_keys(PyObject* split_by, std::vector<std::string>& out) {
    PyRef owned;
    PyObject* sequence = split_by;
    if (PyUnicode_Check(split_by)) {
        owned = PyRef(PyList_New(0));
        if (!owned || PyList_Append(owned.get(), split_by) != 0)
            return false;
        sequence = owned.get();
    } else if (!PyList_Check(split_by) && !PyTuple_Check(split_by)) {
        PyErr_Format(PyExc_TypeError, "split_by must be str or a sequence of str, not %s",
                     Py_TYPE(split_by)->tp_name);
        return false;
    }

    const Py_ssize_t count = PySequence_Fast_GET_SIZE(sequence);
    if (count == 0) {
        PyErr_SetString(PyExc_ValueError, "split_by must name at least one key");
        return false;
    }
    for (Py_ssize_t index = 0; index < count; ++index) {
        PyObject* key = PySequence_Fast_GET_ITEM(sequence, index);
        if (!PyUnicode_Check(key)) {
            PyErr_Format(PyExc_TypeError, "split_by keys must be str, not %s",
                         Py_TYPE(key)->tp_name);
            return false;
        }
        Py_ssize_t size = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(key, &size);
        if (utf8 == nullptr)
            return false;
        out.emplace_back(utf8, static_cast<size_t>(size));
    }
    return true;
}

/// One output file: its path components, and the records that belong in it.
struct Group {
    std::vector<std::string> components;
    PyRef records;
};

/// Group @p records by the split keys, in first-seen order per group.
[[nodiscard]] bool build_groups(PyObject* records, const std::vector<std::string>& keys,
                                std::vector<Group>& groups) {
    // Keyed by the joined string form, which is exactly what a collision is.
    std::map<std::string, size_t> by_path;
    // Lowercased path -> the path that claimed it, for case-only collisions.
    std::map<std::string, std::string> by_folded;
    // Per level: the raw value behind a component, to catch 1 vs "1".
    // Scoped per level because the same text at two different levels names
    // two different path components and cannot collide on disk.
    std::vector<std::map<std::string, PyObject*>> raw_for_component(keys.size());

    const Py_ssize_t count = PyList_GET_SIZE(records);
    for (Py_ssize_t index = 0; index < count; ++index) {
        PyObject* record = PyList_GET_ITEM(records, index);
        if (!PyDict_Check(record)) {
            PyErr_Format(PyExc_TypeError, "every record must be a dict, not %s",
                         Py_TYPE(record)->tp_name);
            return false;
        }

        std::vector<std::string> components;
        std::string joined;
        for (size_t level = 0; level < keys.size(); ++level) {
            const std::string& key = keys[level];
            PyRef key_object(
                PyUnicode_FromStringAndSize(key.data(), static_cast<Py_ssize_t>(key.size())));
            if (!key_object)
                return false;
            PyObject* value = PyDict_GetItemWithError(record, key_object.get());
            if (value == nullptr) {
                if (PyErr_Occurred())
                    return false;
                PyErr_Format(PyExc_ValueError, "record is missing split key %R", key_object.get());
                return false;
            }

            std::string component;
            if (!split_value_to_string(value, component))
                return false;
            if (!util::is_safe_path_component(component)) {
                PyErr_Format(PyExc_ValueError, "split value %R is not usable as a path component",
                             value);
                return false;
            }

            // Two different raw values with the same string form would merge
            // silently; api.md makes that an error.
            auto& level_map = raw_for_component[level];
            auto seen = level_map.find(component);
            if (seen == level_map.end()) {
                level_map.emplace(component, value);
            } else {
                const int same = PyObject_RichCompareBool(seen->second, value, Py_EQ);
                if (same < 0)
                    return false;
                const bool same_type = Py_TYPE(seen->second) == Py_TYPE(value);
                if (same == 0 || !same_type) {
                    PyErr_Format(PyExc_ValueError, "split values %R and %R both name %s",
                                 seen->second, value, component.c_str());
                    return false;
                }
            }

            joined += component;
            joined += '/';
            components.push_back(std::move(component));
        }

        // Names differing only by case would collide on a case-insensitive
        // filesystem, so they are rejected everywhere for consistency.
        const std::string folded = util::ascii_lowercase(joined);
        auto claimed = by_folded.find(folded);
        if (claimed == by_folded.end()) {
            by_folded.emplace(folded, joined);
        } else if (claimed->second != joined) {
            PyErr_Format(PyExc_ValueError, "group names %s and %s differ only by case",
                         claimed->second.c_str(), joined.c_str());
            return false;
        }

        auto slot = by_path.find(joined);
        if (slot == by_path.end()) {
            Group group;
            group.components = std::move(components);
            group.records = PyRef(PyList_New(0));
            if (!group.records)
                return false;
            by_path.emplace(joined, groups.size());
            groups.push_back(std::move(group));
            slot = by_path.find(joined);
        }
        if (PyList_Append(groups[slot->second].records.get(), record) != 0)
            return false;
    }
    return true;
}

/// Write one group's file, creating parent directories as needed.
[[nodiscard]] bool write_group(const std::string& directory, const Group& group) {
    std::string path = directory;
    for (size_t index = 0; index + 1 < group.components.size(); ++index) {
        path += '/';
        path += group.components[index];
        if (!util::make_directories(path)) {
            PyErr_SetFromErrnoWithFilename(PyExc_OSError, path.c_str());
            return false;
        }
    }
    path += '/';
    path += group.components.back();
    path += ".json";

    return dump_to_file(group.records.get(), path.c_str()) != nullptr;
}

} // namespace

PyObject* load_from_folder(const char* directory, bool iterator, bool skip_errors) {
    std::vector<std::string> files;
    if (!discover(directory, files))
        return nullptr;

    if (iterator)
        return make_folder_iterator(std::move(files), nullptr, skip_errors);

    PyRef records(PyList_New(0));
    if (!records)
        return nullptr;
    for (const std::string& path : files) {
        if (!extend_with_file(records.get(), path, skip_errors))
            return nullptr;
    }
    return records.release();
}

PyObject* dump_to_folder(PyObject* records, const char* directory, PyObject* split_by) {
    if (!PyList_Check(records)) {
        PyErr_Format(PyExc_TypeError, "folder dump expects a list of dicts, not %s",
                     Py_TYPE(records)->tp_name);
        return nullptr;
    }

    std::vector<std::string> keys;
    if (!read_split_keys(split_by, keys))
        return nullptr;

    std::vector<Group> groups;
    if (!build_groups(records, keys, groups))
        return nullptr;

    // An empty input still creates the directory, so a later load() finds an
    // empty folder rather than a missing one.
    if (!util::make_directories(directory)) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, directory);
        return nullptr;
    }
    for (const Group& group : groups) {
        if (!write_group(directory, group))
            return nullptr;
    }
    Py_RETURN_NONE;
}

PyObject* search_folder(const char* directory, PyObject* expression, bool iterator) {
    std::vector<std::string> files;
    if (!discover(directory, files))
        return nullptr;

    // Compiled once, reused for every file.
    PyRef compiled_once(compile_expression(expression));
    if (!compiled_once)
        return nullptr;
    if (iterator)
        return make_folder_iterator(std::move(files), compiled_once.get(), /*skip_errors=*/false);

    PyRef matches(PyList_New(0));
    if (!matches)
        return nullptr;
    for (const std::string& path : files) {
        // Exactly search() on each file, in discovery order: that is the law.
        PyRef found(search_file(path.c_str(), compiled_once.get()));
        if (!found)
            return nullptr;
        if (PyList_SetSlice(matches.get(), PY_SSIZE_T_MAX, PY_SSIZE_T_MAX, found.get()) != 0)
            return nullptr;
    }
    return matches.release();
}

bool register_folder_iterator_type(PyObject* module) {
    kFolderIteratorType.tp_name = "strata._strata.FolderIterator";
    kFolderIteratorType.tp_basicsize = sizeof(FolderIteratorObject);
    kFolderIteratorType.tp_dealloc = folder_iterator_dealloc;
    kFolderIteratorType.tp_flags = Py_TPFLAGS_DEFAULT;
    kFolderIteratorType.tp_doc = PyDoc_STR("Lazy iterator over the files of a directory.");
    kFolderIteratorType.tp_iter = folder_iterator_self;
    kFolderIteratorType.tp_iternext = folder_iterator_next;

    if (PyType_Ready(&kFolderIteratorType) < 0)
        return false;
    (void)module; // instances only ever come from load()/search()
    return true;
}

} // namespace strata::bindings

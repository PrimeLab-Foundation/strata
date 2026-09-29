/**
 * @file python_parse_types.cpp
 * @brief The `parse_types` option and its private registry, the reviving
 * iterator and the four cold entry points that use them
 * (docs/architecture/native_types.md, "Parse side", "Flag shape (M15b)").
 *
 * `strata._dumps_hook` only. Each entry point parses through `_strata`'s own
 * public `loads`/`load`/`query`/`compile` -- resolved once by
 * prepare_runtime(), from this image's module init -- and then hands the tree
 * to the revival walk in python_parse_types_walk.cpp, reached through
 * python_parse_types_walk.h. The walk replaces, in place, the strings that
 * name a date, time, date-time or UUID, and the members a caller's registry
 * names. This translation unit links no parser of its own: `_strata` is what
 * parses, so its `duplicate_key_policy` thread-local is honoured.
 *
 * The registry is a private snapshot built from the caller's dict, so a
 * mutation of the caller's dict during the walk changes nothing.
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

/// `_strata`'s public `loads`/`load`/`query`/`compile`, captured once at hook
/// module init and held for the process (module statics are never released).
struct StrataApi {
    PyObject* loads = nullptr;
    PyObject* load = nullptr;
    PyObject* query = nullptr;
    PyObject* compile = nullptr;
};

StrataApi g_strata{};

/// False, with an error set, when `prepare_runtime` could not resolve
/// `_strata`'s entries (only against a stand-in `_strata` missing one of
/// them; a real `_strata` always has all four).
[[nodiscard]] bool runtime_ready() {
    if (g_strata.loads != nullptr)
        return true;
    PyErr_SetString(PyExc_RuntimeError, "strata._dumps_hook: strata._strata does not provide the "
                                        "loads/load/query/compile entries parse_types needs");
    return false;
}

/// `_strata.loads(source)` -- default `return_type="dict"`, `iterator=False`.
[[nodiscard]] PyRef call_loads(PyObject* source) {
    return PyRef(PyObject_CallOneArg(g_strata.loads, source));
}

/// `_strata.load(path, return_type=.., iterator=.., skip_errors=..)`.
[[nodiscard]] PyRef call_load(PyObject* path, const char* return_type, bool iterator,
                              bool skip_errors) {
    PyRef args(PyTuple_Pack(1, path));
    PyRef kwargs(PyDict_New());
    PyRef return_type_obj(PyUnicode_FromString(return_type));
    if (!args || !kwargs || !return_type_obj)
        return PyRef();
    if (PyDict_SetItemString(kwargs.get(), "return_type", return_type_obj.get()) != 0 ||
        PyDict_SetItemString(kwargs.get(), "iterator", iterator ? Py_True : Py_False) != 0 ||
        PyDict_SetItemString(kwargs.get(), "skip_errors", skip_errors ? Py_True : Py_False) != 0)
        return PyRef();
    return PyRef(PyObject_Call(g_strata.load, args.get(), kwargs.get()));
}

/// `_strata.query(data, expression)` -- positional, default `iterator=False`.
[[nodiscard]] PyRef call_query(PyObject* data, PyObject* expression) {
    return PyRef(PyObject_CallFunctionObjArgs(g_strata.query, data, expression, nullptr));
}

/// `_strata.compile(expression)`.
[[nodiscard]] PyRef call_compile(PyObject* expression) {
    return PyRef(PyObject_CallOneArg(g_strata.compile, expression));
}

/// Keyword-only string option from a FASTCALL kwnames tuple, by exact name --
/// the same shape as `_strata`'s `fastcall_str_option` (python_module.cpp),
/// duplicated here since that translation unit is not linked into this image.
[[nodiscard]] const char* fastcall_str_option(PyObject* name, PyObject* value) {
    if (!PyUnicode_Check(value)) {
        PyErr_Format(PyExc_TypeError, "%U must be str, not %s", name, Py_TYPE(value)->tp_name);
        return nullptr;
    }
    return PyUnicode_AsUTF8(value);
}

/// An iterator over a parsed root: dict yields (key, value), list yields
/// elements, and a scalar is returned unchanged (docs/context/api.md). The
/// same rule as `_strata`'s `make_root_iterator` (python_loads.cpp), which is
/// not linked into this image.
[[nodiscard]] PyObject* root_iterator(PyObject* value) {
    if (PyDict_Check(value)) {
        PyRef items(PyObject_CallMethod(value, "items", nullptr));
        if (!items)
            return nullptr;
        return PyObject_GetIter(items.get());
    }
    if (PyList_Check(value))
        return PyObject_GetIter(value);
    return Py_NewRef(value);
}

// ---------------------------------------------------------------------------
// The registry
// ---------------------------------------------------------------------------

/// `sys.modules[name]` as a new reference, or nullptr. Never raises.
[[nodiscard]] PyObject* loaded_module(const char* name) {
    PyObject* const modules = PyImport_GetModuleDict();
    if (modules == nullptr || !PyDict_Check(modules))
        return nullptr;
    PyObject* const module = PyDict_GetItemString(modules, name);
    return module == nullptr ? nullptr : Py_NewRef(module);
}

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
 * `_strata.query(root, compiled)`, extended to a scalar root the way the
 * default `search` evaluates one: a scalar has no children, so only a path
 * that selects the root itself (`$`) matches it. Whether @p compiled is such a
 * path is read off an empty dict, which it matches exactly when it does --
 * `_strata.query()` itself refuses a scalar root, so it is never asked to
 * evaluate one.
 */
[[nodiscard]] PyObject* evaluate(PyObject* root, PyObject* compiled) {
    if (PyDict_Check(root) || PyList_Check(root) || PyTuple_Check(root))
        return call_query(root, compiled).release();
    PyRef probe(PyDict_New());
    if (!probe)
        return nullptr;
    PyRef found(call_query(probe.get(), compiled));
    if (!found)
        return nullptr;
    const bool root_only =
        PyList_GET_SIZE(found.get()) == 1 && PyList_GET_ITEM(found.get(), 0) == probe.get();
    PyObject* const matches = PyList_New(root_only ? 1 : 0);
    if (matches != nullptr && root_only)
        PyList_SET_ITEM(matches, 0, Py_NewRef(root));
    return matches;
}

/// One file's matches: load it whole (through `_strata.load`), revive it,
/// evaluate. @p path always names a file -- the caller resolves directories.
[[nodiscard]] PyObject* search_one(const char* path, PyObject* compiled, PyObject* registry) {
    PyRef path_obj(PyUnicode_FromString(path));
    if (!path_obj)
        return nullptr;
    PyRef root(revive(
        call_load(path_obj.get(), "dict", /*iterator=*/false, /*skip_errors=*/false).release(),
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
 * `_strata.load` would return and revives each record as it is yielded.
 * Search mode walks a folder's discovered files one at a time, searching each
 * with parse_types.
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
        iterator->buffer = search_one(path.c_str(), iterator->compiled, iterator->registry);
        if (iterator->buffer == nullptr)
            return nullptr;
    }
    STRATA_CPP_CATCH
}

[[nodiscard]] bool ready_iterator_type() {
    if ((kRevivingIteratorType.tp_flags & Py_TPFLAGS_READY) != 0)
        return true;
    kRevivingIteratorType.tp_name = "strata._dumps_hook.RevivingIterator";
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
// Runtime
// ---------------------------------------------------------------------------

bool prepare_runtime(PyObject* strata_module) {
    PyRef loads_fn(PyObject_GetAttrString(strata_module, "loads"));
    PyRef load_fn(PyObject_GetAttrString(strata_module, "load"));
    PyRef query_fn(PyObject_GetAttrString(strata_module, "query"));
    PyRef compile_fn(PyObject_GetAttrString(strata_module, "compile"));
    if (!loads_fn || !load_fn || !query_fn || !compile_fn)
        return false;
    g_strata.loads = loads_fn.release();
    g_strata.load = load_fn.release();
    g_strata.query = query_fn.release();
    g_strata.compile = compile_fn.release();
    return true;
}

// ---------------------------------------------------------------------------
// Entry points
// ---------------------------------------------------------------------------

PyObject* loads_typed(PyObject* /*self*/, PyObject* const* args, Py_ssize_t nargs,
                      PyObject* kwnames) {
    STRATA_CPP_TRY
    if (!runtime_ready())
        return nullptr;
    if (nargs != 1) {
        PyErr_Format(PyExc_TypeError,
                     "loads_typed() takes exactly 1 positional argument (%zd given)", nargs);
        return nullptr;
    }
    PyObject* const source = args[0];
    const char* return_type = "dict";
    int iterator = 0;
    PyObject* option = nullptr;
    if (kwnames != nullptr) {
        for (Py_ssize_t index = 0; index < PyTuple_GET_SIZE(kwnames); ++index) {
            PyObject* const name = PyTuple_GET_ITEM(kwnames, index);
            PyObject* const value = args[nargs + index];
            if (PyUnicode_CompareWithASCIIString(name, "return_type") == 0) {
                return_type = fastcall_str_option(name, value);
                if (return_type == nullptr)
                    return nullptr;
            } else if (PyUnicode_CompareWithASCIIString(name, "iterator") == 0) {
                iterator = PyObject_IsTrue(value);
                if (iterator < 0)
                    return nullptr;
            } else if (PyUnicode_CompareWithASCIIString(name, "parse_types") == 0) {
                option = value;
            } else {
                PyErr_Format(PyExc_TypeError,
                             "loads_typed() got an unexpected keyword argument '%U'", name);
                return nullptr;
            }
        }
    }
    if (option == nullptr) {
        PyErr_SetString(PyExc_TypeError, "loads_typed() missing required argument 'parse_types'");
        return nullptr;
    }

    const bool want_cursor = std::strcmp(return_type, "cursor") == 0;
    if (!want_cursor && std::strcmp(return_type, "dict") != 0) {
        PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
        return nullptr;
    }

    Option parsed;
    if (!parsed.init(option))
        return nullptr;
    if (want_cursor) {
        PyErr_SetString(PyExc_ValueError, "parse_types needs return_type='dict'");
        return nullptr;
    }

    PyRef value(revive(call_loads(source).release(), parsed.registry()));
    if (!value || !iterator)
        return value.release();
    return root_iterator(value.get());
    STRATA_CPP_CATCH
}

PyObject* load_typed(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    if (!runtime_ready())
        return nullptr;
    static const char* keywords[] = {
        "", "return_type", "iterator", "skip_errors", "parse_types", nullptr};
    const char* path = nullptr;
    const char* return_type = "dict";
    int iterator = 0;
    int skip_errors = 0;
    PyObject* option = nullptr;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|$sppO", const_cast<char**>(keywords), &path,
                                     &return_type, &iterator, &skip_errors, &option))
        return nullptr;
    if (option == nullptr) {
        PyErr_SetString(PyExc_TypeError, "load_typed() missing required argument 'parse_types'");
        return nullptr;
    }

    Option parsed;
    if (!parsed.init(option))
        return nullptr;
    if (std::strcmp(return_type, "cursor") == 0) {
        PyErr_SetString(PyExc_ValueError, "parse_types needs return_type='dict'");
        return nullptr;
    }

    PyRef path_obj(PyUnicode_FromString(path));
    if (!path_obj)
        return nullptr;
    // Not the open-first dispatch _strata.load uses (E26-P27): parse_types is
    // a cold, opt-in path, and this stat tells us -- before delegating -- when
    // to expect a lazy folder iterator back, which only iterator does.
    const bool is_dir = strata::util::is_directory(path);
    const bool lazy = iterator != 0 && (is_dir || file_is_ndjson(path));
    PyRef loaded(call_load(path_obj.get(), return_type, lazy, skip_errors != 0));
    if (!loaded)
        return nullptr;
    if (lazy)
        return reviving_iterator(loaded.release(), parsed.registry());
    PyRef value(revive(loaded.release(), parsed.registry()));
    if (!value || !iterator)
        return value.release();
    return root_iterator(value.get());
    STRATA_CPP_CATCH
}

PyObject* search_typed(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    if (!runtime_ready())
        return nullptr;
    static const char* keywords[] = {"", "", "iterator", "parse_types", nullptr};
    const char* path = nullptr;
    PyObject* expression = nullptr;
    int iterator = 0;
    PyObject* option = nullptr;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sO|$pO", const_cast<char**>(keywords), &path,
                                     &expression, &iterator, &option))
        return nullptr;
    if (option == nullptr) {
        PyErr_SetString(PyExc_TypeError, "search_typed() missing required argument 'parse_types'");
        return nullptr;
    }

    Option parsed;
    if (!parsed.init(option))
        return nullptr;
    PyRef compiled(call_compile(expression));
    if (!compiled)
        return nullptr;

    if (strata::util::is_directory(path)) {
        std::vector<std::string> files;
        if (!discover(path, files))
            return nullptr;
        if (iterator)
            return searching_iterator(std::move(files), compiled.get(), parsed.registry());
        PyRef matches(PyList_New(0));
        if (!matches)
            return nullptr;
        for (const std::string& file : files) {
            // Exactly search() on each file, in discovery order.
            PyRef found(search_one(file.c_str(), compiled.get(), parsed.registry()));
            if (!found ||
                PyList_SetSlice(matches.get(), PY_SSIZE_T_MAX, PY_SSIZE_T_MAX, found.get()) != 0)
                return nullptr;
        }
        return matches.release();
    }
    PyRef matches(search_one(path, compiled.get(), parsed.registry()));
    if (!matches)
        return nullptr;
    return iterator ? PyObject_GetIter(matches.get()) : matches.release();
    STRATA_CPP_CATCH
}

PyObject* query_typed(PyObject* /*self*/, PyObject* args, PyObject* kwargs) {
    STRATA_CPP_TRY
    if (!runtime_ready())
        return nullptr;
    static const char* keywords[] = {"", "", "iterator", "parse_types", nullptr};
    PyObject* data = nullptr;
    PyObject* expression = nullptr;
    int iterator = 0;
    PyObject* option = nullptr;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|$pO", const_cast<char**>(keywords), &data,
                                     &expression, &iterator, &option))
        return nullptr;
    if (option == nullptr) {
        PyErr_SetString(PyExc_TypeError, "query_typed() missing required argument 'parse_types'");
        return nullptr;
    }
    if (!PyBool_Check(option)) {
        PyErr_Format(PyExc_TypeError, "query() parse_types must be a bool, not %s",
                     Py_TYPE(option)->tp_name);
        return nullptr;
    }
    const bool recognizing = option == Py_True;
    if (recognizing && !ensure_runtime())
        return nullptr;
    PyRef matches(call_query(data, expression));
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
    STRATA_CPP_CATCH
}

} // namespace strata::bindings::parse_types

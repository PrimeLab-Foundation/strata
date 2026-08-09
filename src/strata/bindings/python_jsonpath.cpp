/**
 * @file python_jsonpath.cpp
 * @brief `query`, `search` and the compiled-path object.
 *
 * Two evaluators live behind this file and they must agree:
 *
 * - **PyObject-native** walks Python dicts and lists directly, so integers stay
 *   exact. It backs `query()` and, through it, `search()`.
 * - **C++ DOM** (`eval_jsonpath`) backs `CompiledPath.execute(cursor)`, where
 *   the data is already a parsed tree and numbers are doubles.
 *
 * `search(file, expr) == query(load(file), expr)` is a law the milestone
 * requires, so `search()` is defined *as* a load followed by a query rather
 * than as a second implementation that has to be kept in step
 * (docs/decisions.md).
 */

#include "python_types.h"
#include "strata/json/json_cursor.hpp"
#include "strata/search/jsonpath.hpp"

#include <cmath>
#include <cstddef>
#include <new>
#include <string>
#include <string_view>
#include <vector>

namespace strata::bindings {

namespace {

// ---------------------------------------------------------------------------
// The compiled-path object
// ---------------------------------------------------------------------------

struct CompiledPathObject {
    PyObject_HEAD CompiledPath* path;
    PyObject* expression; ///< kept for repr()
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
PyTypeObject kCompiledPathType = {PyVarObject_HEAD_INIT(nullptr, 0)};
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

void compiled_dealloc(PyObject* self) {
    auto* compiled = reinterpret_cast<CompiledPathObject*>(self);
    delete compiled->path;
    Py_XDECREF(compiled->expression);
    PyObject_Free(self);
}

PyObject* compiled_repr(PyObject* self) {
    auto* compiled = reinterpret_cast<CompiledPathObject*>(self);
    return PyUnicode_FromFormat("CompiledPath(%R)", compiled->expression);
}

// ---------------------------------------------------------------------------
// PyObject-native evaluation
// ---------------------------------------------------------------------------

using Matches = std::vector<PyObject*>; ///< borrowed references throughout

[[nodiscard]] bool is_sequence(PyObject* value) noexcept {
    return PyList_Check(value) || PyTuple_Check(value);
}

[[nodiscard]] Py_ssize_t sequence_size(PyObject* value) noexcept {
    return PyList_Check(value) ? PyList_GET_SIZE(value) : PyTuple_GET_SIZE(value);
}

[[nodiscard]] PyObject* sequence_item(PyObject* value, Py_ssize_t index) noexcept {
    return PyList_Check(value) ? PyList_GET_ITEM(value, index) : PyTuple_GET_ITEM(value, index);
}

/// Depth-first collection of every `name`, outermost first, descending into
/// matches as well -- the behaviour the C++ evaluator also implements.
void collect_recursive(PyObject* value, PyObject* name, Matches& out) {
    if (PyDict_Check(value)) {
        Py_ssize_t position = 0;
        PyObject* key = nullptr;
        PyObject* member = nullptr;
        while (PyDict_Next(value, &position, &key, &member)) {
            const int same = PyObject_RichCompareBool(key, name, Py_EQ);
            if (same < 0) {
                PyErr_Clear();
            } else if (same > 0) {
                out.push_back(member);
            }
            collect_recursive(member, name, out);
        }
        return;
    }
    if (is_sequence(value)) {
        const Py_ssize_t size = sequence_size(value);
        for (Py_ssize_t index = 0; index < size; ++index)
            collect_recursive(sequence_item(value, index), name, out);
    }
}

/// A number for comparison purposes. `bool` is not one, matching the DOM path.
[[nodiscard]] bool as_number(PyObject* value, double& out) noexcept {
    if (PyBool_Check(value))
        return false;
    if (PyFloat_Check(value)) {
        out = PyFloat_AS_DOUBLE(value);
        return true;
    }
    if (PyLong_Check(value)) {
        out = PyLong_AsDouble(value);
        if (out == -1.0 && PyErr_Occurred()) {
            PyErr_Clear();
            return false;
        }
        return true;
    }
    return false;
}

[[nodiscard]] bool compare(double left, FilterOp op, double right) noexcept {
    switch (op) {
    case FilterOp::Equal:
        return left == right;
    case FilterOp::NotEqual:
        return left != right;
    case FilterOp::Less:
        return left < right;
    case FilterOp::LessEqual:
        return left <= right;
    case FilterOp::Greater:
        return left > right;
    case FilterOp::GreaterEqual:
        return left >= right;
    }
    return false;
}

[[nodiscard]] bool passes_filter(PyObject* candidate, const PathStep& step, PyObject* key) {
    if (!PyDict_Check(candidate))
        return false;
    PyObject* field = PyDict_GetItemWithError(candidate, key);
    if (field == nullptr) {
        PyErr_Clear();
        return false;
    }

    if (step.filter_is_number) {
        double number = 0.0;
        if (!as_number(field, number))
            return false;
        return compare(number, step.filter_op, step.filter_number);
    }
    if (!PyUnicode_Check(field))
        return false;
    const int equal =
        PyUnicode_CompareWithASCIIString(field, std::string(step.filter_string).c_str());
    return step.filter_op == FilterOp::Equal ? equal == 0 : equal != 0;
}

[[nodiscard]] int64_t clamp_bound(int64_t value, Py_ssize_t size) noexcept {
    if (value < 0)
        value += size;
    if (value < 0)
        return 0;
    if (value > size)
        return size;
    return value;
}

/// Apply one step. False means a Python error was raised.
[[nodiscard]] bool apply(const PathStep& step, const Matches& input, Matches& output) {
    for (PyObject* value : input) {
        switch (step.op) {
        case PathOp::Root:
            output.push_back(value);
            break;

        case PathOp::Field: {
            if (!PyDict_Check(value))
                break;
            PyRef key(PyUnicode_FromStringAndSize(step.name.data(),
                                                  static_cast<Py_ssize_t>(step.name.size())));
            if (!key)
                return false;
            PyObject* member = PyDict_GetItemWithError(value, key.get());
            if (member != nullptr)
                output.push_back(member);
            else if (PyErr_Occurred())
                return false;
            break;
        }

        case PathOp::Wildcard:
            if (PyDict_Check(value)) {
                Py_ssize_t position = 0;
                PyObject* key = nullptr;
                PyObject* member = nullptr;
                while (PyDict_Next(value, &position, &key, &member))
                    output.push_back(member);
            } else if (is_sequence(value)) {
                const Py_ssize_t size = sequence_size(value);
                for (Py_ssize_t index = 0; index < size; ++index)
                    output.push_back(sequence_item(value, index));
            }
            break;

        case PathOp::Index: {
            if (!is_sequence(value))
                break;
            const Py_ssize_t size = sequence_size(value);
            int64_t index = step.index;
            if (index < 0)
                index += size;
            if (index >= 0 && index < size)
                output.push_back(sequence_item(value, static_cast<Py_ssize_t>(index)));
            break;
        }

        case PathOp::RecursiveDescent: {
            PyRef name(PyUnicode_FromStringAndSize(step.name.data(),
                                                   static_cast<Py_ssize_t>(step.name.size())));
            if (!name)
                return false;
            collect_recursive(value, name.get(), output);
            break;
        }

        case PathOp::Slice: {
            if (!is_sequence(value) || step.step <= 0)
                break; // a negative step selects nothing, by documented quirk
            const Py_ssize_t size = sequence_size(value);
            const int64_t start = step.has_start ? clamp_bound(step.start, size) : 0;
            const int64_t end = step.has_end ? clamp_bound(step.end, size) : size;
            for (int64_t index = start; index < end; index += step.step)
                output.push_back(sequence_item(value, static_cast<Py_ssize_t>(index)));
            break;
        }

        case PathOp::Filter: {
            if (!is_sequence(value))
                break; // filters select from arrays only
            PyRef key(PyUnicode_FromStringAndSize(step.name.data(),
                                                  static_cast<Py_ssize_t>(step.name.size())));
            if (!key)
                return false;
            const Py_ssize_t size = sequence_size(value);
            for (Py_ssize_t index = 0; index < size; ++index) {
                PyObject* element = sequence_item(value, index);
                if (passes_filter(element, step, key.get()))
                    output.push_back(element);
            }
            break;
        }
        }
    }
    return true;
}

/// The compiled path behind @p expression, whether it is text or already compiled.
[[nodiscard]] bool resolve_path(PyObject* expression, CompiledPath& out) {
    if (Py_IS_TYPE(expression, &kCompiledPathType)) {
        out = *reinterpret_cast<CompiledPathObject*>(expression)->path;
        return true;
    }
    if (!PyUnicode_Check(expression)) {
        PyErr_Format(PyExc_TypeError, "expression must be str or CompiledPath, not %s",
                     Py_TYPE(expression)->tp_name);
        return false;
    }
    Py_ssize_t size = 0;
    const char* text = PyUnicode_AsUTF8AndSize(expression, &size);
    if (text == nullptr)
        return false;

    auto compiled = compile_jsonpath(std::string_view(text, static_cast<size_t>(size)));
    if (!compiled.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
        return false;
    }
    out = std::move(compiled.value);
    return true;
}

PyObject* compiled_execute(PyObject* self, PyObject* args) {
    STRATA_CPP_TRY
    PyObject* cursor_object = nullptr;
    if (!PyArg_ParseTuple(args, "O", &cursor_object))
        return nullptr;

    const JsonValue* root = cursor_value(cursor_object);
    if (root == nullptr) {
        PyErr_SetString(PyExc_TypeError, "execute() expects a JsonCursor");
        return nullptr;
    }

    auto* compiled = reinterpret_cast<CompiledPathObject*>(self);
    const auto matches = eval_jsonpath(*compiled->path, *root);

    PyRef results(PyList_New(static_cast<Py_ssize_t>(matches.size())));
    if (!results)
        return nullptr;
    for (size_t index = 0; index < matches.size(); ++index) {
        PyObject* value = cursor_child(cursor_object, matches[index]);
        if (value == nullptr)
            return nullptr;
        PyList_SET_ITEM(results.get(), static_cast<Py_ssize_t>(index), value);
    }
    return results.release();
    STRATA_CPP_CATCH
}

PyMethodDef kCompiledPathMethods[] = {
    {"execute", compiled_execute, METH_VARARGS,
     "execute(cursor) -> list[JsonCursor]\n\nRun this path against a cursor."},
    {nullptr, nullptr, 0, nullptr},
};

} // namespace

bool register_jsonpath_types(PyObject* module) {
    kCompiledPathType.tp_name = "strata._strata.CompiledPath";
    kCompiledPathType.tp_basicsize = sizeof(CompiledPathObject);
    kCompiledPathType.tp_dealloc = compiled_dealloc;
    kCompiledPathType.tp_repr = compiled_repr;
    kCompiledPathType.tp_flags = Py_TPFLAGS_DEFAULT;
    kCompiledPathType.tp_doc = PyDoc_STR("A JSONPath expression, compiled once.");
    kCompiledPathType.tp_methods = kCompiledPathMethods;

    if (PyType_Ready(&kCompiledPathType) < 0)
        return false;
    Py_INCREF(&kCompiledPathType);
    if (PyModule_AddObject(module, "CompiledPath",
                           reinterpret_cast<PyObject*>(&kCompiledPathType)) < 0) {
        Py_DECREF(&kCompiledPathType);
        return false;
    }
    return true;
}

PyObject* compile_expression(PyObject* expression) {
    CompiledPath path;
    if (!resolve_path(expression, path))
        return nullptr;

    auto* self = PyObject_New(CompiledPathObject, &kCompiledPathType);
    if (self == nullptr)
        return nullptr;
    self->path = new (std::nothrow) CompiledPath(std::move(path));
    if (self->path == nullptr) {
        PyObject_Free(self);
        return PyErr_NoMemory();
    }
    self->expression = Py_NewRef(expression);
    return reinterpret_cast<PyObject*>(self);
}

PyObject* query_object(PyObject* data, PyObject* expression) {
    if (!PyDict_Check(data) && !PyList_Check(data) && !PyTuple_Check(data)) {
        PyErr_Format(PyExc_TypeError, "query() expects dict, list or tuple, not %s",
                     Py_TYPE(data)->tp_name);
        return nullptr;
    }

    CompiledPath path;
    if (!resolve_path(expression, path))
        return nullptr;

    Matches current{data};
    Matches next;
    for (const PathStep& step : path) {
        if (step.op == PathOp::Root)
            continue;
        next.clear();
        if (!apply(step, current, next))
            return nullptr;
        current.swap(next);
        if (current.empty())
            break;
    }

    PyObject* results = PyList_New(static_cast<Py_ssize_t>(current.size()));
    if (results == nullptr)
        return nullptr;
    for (size_t index = 0; index < current.size(); ++index)
        PyList_SET_ITEM(results, static_cast<Py_ssize_t>(index), Py_NewRef(current[index]));
    return results;
}

PyObject* search_file(const char* path, PyObject* expression) {
    // The law `search(f, e) == query(load(f), e)` is not a property to be
    // maintained by two implementations agreeing; it is how search is defined.
    PyRef data(load_from_file(path, "dict", /*iterator=*/false, /*skip_errors=*/false));
    if (!data)
        return nullptr;
    return query_object(data.get(), expression);
}

} // namespace strata::bindings

#include "python_convert.h"
#include "python_document.h"
#include "python_types.h"
#include "strata/json/json_mmap.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/search/jsonpath.hpp"
#include "strata/util/fast_parse.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <limits>
#include <string>
#include <vector>

// From python_module.cpp
extern bool strata_config_mem_eff();

//=============================================================================
// PyObject-native JSONPath evaluation
// Evaluates a compiled path directly on Python dicts/lists without building
// an intermediate C++ JsonValue tree. Results are borrowed refs from the
// input; the caller must Py_INCREF when placing them in an output list.
//=============================================================================

static bool eval_filter_pyobj(PyObject* val, const strata::FilterPredicate& filter) {
    if (filter.is_numeric) {
        double v;
        if (val == Py_True) {
            v = 1.0;
        } else if (val == Py_False) {
            v = 0.0;
        } else if (PyFloat_Check(val)) {
            v = PyFloat_AS_DOUBLE(val);
        } else if (PyLong_Check(val)) {
            v = PyLong_AsDouble(val);
            if (v == -1.0 && PyErr_Occurred()) {
                PyErr_Clear();
                return false;
            }
        } else {
            return false;
        }
        switch (filter.op) {
        case strata::FilterOp::Equal:
            return v == filter.numeric_value;
        case strata::FilterOp::NotEqual:
            return v != filter.numeric_value;
        case strata::FilterOp::GreaterThan:
            return v > filter.numeric_value;
        case strata::FilterOp::GreaterEqual:
            return v >= filter.numeric_value;
        case strata::FilterOp::LessThan:
            return v < filter.numeric_value;
        case strata::FilterOp::LessEqual:
            return v <= filter.numeric_value;
        default:
            return false;
        }
    } else {
        if (!PyUnicode_Check(val))
            return false;
        const char* s = PyUnicode_AsUTF8(val);
        if (!s) {
            PyErr_Clear();
            return false;
        }
        switch (filter.op) {
        case strata::FilterOp::Equal:
            return filter.string_value == s;
        case strata::FilterOp::NotEqual:
            return filter.string_value != s;
        default:
            return false;
        }
    }
}

// Collect values for recursive descent directly from C++ JsonValue tree,
// converting only the matched leaf values to Python (not the whole tree).
static void collect_recursive_jsonvalue(const strata::JsonValue* value,
                                        const std::string& field_name,
                                        std::vector<PyObject*>& collected) {
    if (!value)
        return;

    if (value->is_object()) {
        const auto& obj = value->as_object();
        for (const auto& pair : obj) {
            if (pair.first == field_name) {
                PyObject* py_val = json_value_to_python(pair.second);
                if (py_val)
                    collected.push_back(py_val);
            }
            collect_recursive_jsonvalue(&pair.second, field_name, collected);
        }
    } else if (value->is_array()) {
        const auto& arr = value->as_array();
        for (const auto& elem : arr) {
            collect_recursive_jsonvalue(&elem, field_name, collected);
        }
    }
}

// Recursive JSONPath evaluator on C++ JsonValue tree, producing Python results directly.
// Only the matched leaf values are converted to Python, not the whole tree.
static void eval_step_jsonvalue(const strata::JsonValue* value,
                                const std::vector<strata::PathStep>& steps, size_t step_idx,
                                std::vector<PyObject*>& results) {
    if (!value)
        return;

    if (step_idx >= steps.size()) {
        PyObject* py_val = json_value_to_python(*value);
        if (py_val)
            results.push_back(py_val);
        return;
    }

    const strata::PathStep& step = steps[step_idx];

    switch (step.op) {
    case strata::PathOp::Root:
        eval_step_jsonvalue(value, steps, step_idx + 1, results);
        break;

    case strata::PathOp::Field:
        if (value->is_object()) {
            const auto& obj = value->as_object();
            auto it = obj.find(step.field);
            if (it != obj.end()) {
                eval_step_jsonvalue(&it->second, steps, step_idx + 1, results);
            }
        }
        break;

    case strata::PathOp::Index:
        if (value->is_array()) {
            const auto& arr = value->as_array();
            int64_t idx = step.index;
            if (idx < 0)
                idx = static_cast<int64_t>(arr.size()) + idx;
            if (idx >= 0 && idx < static_cast<int64_t>(arr.size())) {
                eval_step_jsonvalue(&arr[static_cast<size_t>(idx)], steps, step_idx + 1, results);
            }
        }
        break;

    case strata::PathOp::Wildcard:
        if (value->is_array()) {
            const auto& arr = value->as_array();
            for (const auto& elem : arr) {
                eval_step_jsonvalue(&elem, steps, step_idx + 1, results);
            }
        } else if (value->is_object()) {
            const auto& obj = value->as_object();
            for (const auto& pair : obj) {
                eval_step_jsonvalue(&pair.second, steps, step_idx + 1, results);
            }
        }
        break;

    case strata::PathOp::RecursiveDescent: {
        // For recursive descent with remaining steps, collect matches then continue
        if (step_idx + 1 >= steps.size()) {
            // Terminal recursive: collect field values directly
            collect_recursive_jsonvalue(value, step.field, results);
        } else {
            // Non-terminal: collect cursors then evaluate remaining steps
            std::vector<const strata::JsonValue*> found;
            // Inline collection to avoid JsonCursor overhead
            std::function<void(const strata::JsonValue*)> collect;
            collect = [&](const strata::JsonValue* v) {
                if (!v)
                    return;
                if (v->is_object()) {
                    const auto& obj = v->as_object();
                    for (const auto& pair : obj) {
                        if (pair.first == step.field) {
                            found.push_back(&pair.second);
                        }
                        collect(&pair.second);
                    }
                } else if (v->is_array()) {
                    for (const auto& elem : v->as_array()) {
                        collect(&elem);
                    }
                }
            };
            collect(value);
            for (const auto* f : found) {
                eval_step_jsonvalue(f, steps, step_idx + 1, results);
            }
        }
        break;
    }

    case strata::PathOp::Slice:
        if (value->is_array()) {
            const auto& arr = value->as_array();
            int64_t start = step.slice_start;
            int64_t end = step.slice_end;
            int64_t stp = step.slice_step;
            int64_t sz = static_cast<int64_t>(arr.size());
            if (start < 0)
                start = sz + start;
            if (end < 0)
                end = sz + end;
            start = std::max(int64_t(0), std::min(start, sz));
            end = std::max(int64_t(0), std::min(end, sz));
            if (stp > 0) {
                for (int64_t i = start; i < end; i += stp) {
                    eval_step_jsonvalue(&arr[static_cast<size_t>(i)], steps, step_idx + 1, results);
                }
            }
        }
        break;

    case strata::PathOp::Filter:
        if (value->is_array()) {
            const auto& arr = value->as_array();
            for (const auto& elem : arr) {
                if (elem.is_object()) {
                    const auto& obj = elem.as_object();
                    auto it = obj.find(step.filter.field);
                    if (it != obj.end()) {
                        // Evaluate filter directly on JsonValue
                        bool match = false;
                        if (step.filter.is_numeric && it->second.is_number()) {
                            double v = it->second.as_number();
                            switch (step.filter.op) {
                            case strata::FilterOp::Equal:
                                match = v == step.filter.numeric_value;
                                break;
                            case strata::FilterOp::NotEqual:
                                match = v != step.filter.numeric_value;
                                break;
                            case strata::FilterOp::GreaterThan:
                                match = v > step.filter.numeric_value;
                                break;
                            case strata::FilterOp::GreaterEqual:
                                match = v >= step.filter.numeric_value;
                                break;
                            case strata::FilterOp::LessThan:
                                match = v < step.filter.numeric_value;
                                break;
                            case strata::FilterOp::LessEqual:
                                match = v <= step.filter.numeric_value;
                                break;
                            default:
                                break;
                            }
                        } else if (!step.filter.is_numeric && it->second.is_string()) {
                            const auto& s = it->second.as_string();
                            switch (step.filter.op) {
                            case strata::FilterOp::Equal:
                                match = s == step.filter.string_value;
                                break;
                            case strata::FilterOp::NotEqual:
                                match = s != step.filter.string_value;
                                break;
                            default:
                                break;
                            }
                        }
                        if (match) {
                            eval_step_jsonvalue(&elem, steps, step_idx + 1, results);
                        }
                    }
                }
            }
        }
        break;

    case strata::PathOp::End: {
        PyObject* py_val = json_value_to_python(*value);
        if (py_val)
            results.push_back(py_val);
    } break;
    }
}

// Collect all occurrences of field_name at any nesting level (mirrors C++ recursive descent).
static void collect_recursive_pyobj(PyObject* obj, const std::string& field_name,
                                    std::vector<PyObject*>& collected) {
    if (PyDict_Check(obj)) {
        // Check this node for the target field
        PyObject* found = PyDict_GetItemString(obj, field_name.c_str()); // borrowed
        if (found) {
            collected.push_back(found);
        }
        // Recurse into all values (including found, to detect nested matches)
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        while (PyDict_Next(obj, &pos, &key, &value)) {
            collect_recursive_pyobj(value, field_name, collected);
        }
    } else if (PyList_Check(obj)) {
        Py_ssize_t size = PyList_GET_SIZE(obj);
        for (Py_ssize_t i = 0; i < size; i++) {
            collect_recursive_pyobj(PyList_GET_ITEM(obj, i), field_name, collected);
        }
    } else if (PyTuple_Check(obj)) {
        Py_ssize_t size = PyTuple_GET_SIZE(obj);
        for (Py_ssize_t i = 0; i < size; i++) {
            collect_recursive_pyobj(PyTuple_GET_ITEM(obj, i), field_name, collected);
        }
    }
}

// Recursive JSONPath step evaluator on raw Python objects.
// All refs in `results` are borrowed from `obj` (or its descendants).
static void eval_step_pyobj(PyObject* obj, const std::vector<strata::PathStep>& steps,
                            size_t step_idx, std::vector<PyObject*>& results) {
    if (step_idx >= steps.size()) {
        results.push_back(obj);
        return;
    }

    const strata::PathStep& step = steps[step_idx];

    switch (step.op) {
    case strata::PathOp::Root:
        eval_step_pyobj(obj, steps, step_idx + 1, results);
        break;

    case strata::PathOp::Field:
        if (PyDict_Check(obj)) {
            PyObject* val = PyDict_GetItemString(obj, step.field.c_str()); // borrowed
            if (val) {
                eval_step_pyobj(val, steps, step_idx + 1, results);
            }
        }
        break;

    case strata::PathOp::Index:
        if (PyList_Check(obj)) {
            Py_ssize_t size = PyList_GET_SIZE(obj);
            int64_t idx = step.index;
            if (idx < 0)
                idx = (int64_t)size + idx;
            if (idx >= 0 && idx < (int64_t)size) {
                eval_step_pyobj(PyList_GET_ITEM(obj, (Py_ssize_t)idx), steps, step_idx + 1,
                                results);
            }
        } else if (PyTuple_Check(obj)) {
            Py_ssize_t size = PyTuple_GET_SIZE(obj);
            int64_t idx = step.index;
            if (idx < 0)
                idx = (int64_t)size + idx;
            if (idx >= 0 && idx < (int64_t)size) {
                eval_step_pyobj(PyTuple_GET_ITEM(obj, (Py_ssize_t)idx), steps, step_idx + 1,
                                results);
            }
        }
        break;

    case strata::PathOp::Wildcard:
        if (PyList_Check(obj)) {
            Py_ssize_t size = PyList_GET_SIZE(obj);
            for (Py_ssize_t i = 0; i < size; i++) {
                eval_step_pyobj(PyList_GET_ITEM(obj, i), steps, step_idx + 1, results);
            }
        } else if (PyTuple_Check(obj)) {
            Py_ssize_t size = PyTuple_GET_SIZE(obj);
            for (Py_ssize_t i = 0; i < size; i++) {
                eval_step_pyobj(PyTuple_GET_ITEM(obj, i), steps, step_idx + 1, results);
            }
        } else if (PyDict_Check(obj)) {
            PyObject *key, *value;
            Py_ssize_t pos = 0;
            while (PyDict_Next(obj, &pos, &key, &value)) {
                eval_step_pyobj(value, steps, step_idx + 1, results);
            }
        }
        break;

    case strata::PathOp::RecursiveDescent: {
        std::vector<PyObject*> collected;
        collect_recursive_pyobj(obj, step.field, collected);
        for (PyObject* item : collected) {
            eval_step_pyobj(item, steps, step_idx + 1, results);
        }
        break;
    }

    case strata::PathOp::Slice:
        if (PyList_Check(obj)) {
            Py_ssize_t size = PyList_GET_SIZE(obj);
            int64_t start = step.slice_start;
            int64_t end = step.slice_end;
            int64_t stp = step.slice_step;
            if (start < 0)
                start = (int64_t)size + start;
            if (end < 0)
                end = (int64_t)size + end;
            start = std::max(int64_t(0), std::min(start, (int64_t)size));
            end = std::max(int64_t(0), std::min(end, (int64_t)size));
            if (stp > 0) {
                for (int64_t i = start; i < end; i += stp) {
                    eval_step_pyobj(PyList_GET_ITEM(obj, (Py_ssize_t)i), steps, step_idx + 1,
                                    results);
                }
            }
        }
        break;

    case strata::PathOp::Filter:
        if (PyList_Check(obj)) {
            Py_ssize_t size = PyList_GET_SIZE(obj);
            for (Py_ssize_t i = 0; i < size; i++) {
                PyObject* item = PyList_GET_ITEM(obj, i);
                if (!PyDict_Check(item))
                    continue;
                PyObject* fval = PyDict_GetItemString(item, step.filter.field.c_str());
                if (fval && eval_filter_pyobj(fval, step.filter)) {
                    eval_step_pyobj(item, steps, step_idx + 1, results);
                }
            }
        } else if (PyTuple_Check(obj)) {
            Py_ssize_t size = PyTuple_GET_SIZE(obj);
            for (Py_ssize_t i = 0; i < size; i++) {
                PyObject* item = PyTuple_GET_ITEM(obj, i);
                if (!PyDict_Check(item))
                    continue;
                PyObject* fval = PyDict_GetItemString(item, step.filter.field.c_str());
                if (fval && eval_filter_pyobj(fval, step.filter)) {
                    eval_step_pyobj(item, steps, step_idx + 1, results);
                }
            }
        }
        break;

    case strata::PathOp::End:
        results.push_back(obj);
        break;
    }
}

// Build a Python list from a vector of owned PyObject refs (steal refs, no incref).
static PyObject* pyobj_results_to_list_steal(std::vector<PyObject*>& results) {
    PyObject* list = PyList_New((Py_ssize_t)results.size());
    if (!list) {
        for (auto* p : results)
            Py_XDECREF(p);
        return nullptr;
    }
    for (size_t i = 0; i < results.size(); i++) {
        PyList_SET_ITEM(list, (Py_ssize_t)i, results[i]); // steals ref
    }
    results.clear();
    return list;
}

// Build a Python list from a vector of borrowed PyObject refs (incref each).
static PyObject* pyobj_results_to_list(const std::vector<PyObject*>& results) {
    PyObject* list = PyList_New((Py_ssize_t)results.size());
    if (!list)
        return nullptr;
    for (size_t i = 0; i < results.size(); i++) {
        Py_INCREF(results[i]);
        PyList_SET_ITEM(list, (Py_ssize_t)i, results[i]);
    }
    return list;
}

static void emit_duplicate_key_warnings() {
    auto warnings = strata::consume_parse_warnings();
    for (const auto& msg : warnings) {
        PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
    }
}

extern int strata_get_cycle_policy();

static PyObject* make_cycle_memo() { return PyDict_New(); }

static bool memo_contains(PyObject* memo, PyObject* obj, bool& ok) {
    PyObject* key = PyLong_FromVoidPtr(obj);
    if (!key) {
        ok = false;
        return false;
    }
    int rc = PyDict_Contains(memo, key);
    Py_DECREF(key);
    if (rc < 0) {
        ok = false;
        return false;
    }
    return rc == 1;
}

static bool memo_add(PyObject* memo, PyObject* obj) {
    PyObject* key = PyLong_FromVoidPtr(obj);
    if (!key)
        return false;
    int rc = PyDict_SetItem(memo, key, Py_True);
    Py_DECREF(key);
    return rc == 0;
}

static void memo_remove(PyObject* memo, PyObject* obj) {
    PyObject* key = PyLong_FromVoidPtr(obj);
    if (!key) {
        PyErr_Clear();
        return;
    }
    if (PyDict_DelItem(memo, key) < 0) {
        PyErr_Clear();
    }
    Py_DECREF(key);
}

static bool is_container(PyObject* obj) {
    return PyDict_Check(obj) || PyList_Check(obj) || PyTuple_Check(obj);
}

static strata::JsonValue pyobject_to_json_value(PyObject* obj, PyObject* memo, bool& ok) {
    if (!ok) {
        return strata::JsonValue();
    }

    if (obj == Py_None) {
        return strata::JsonValue();
    }

    if (obj == Py_True) {
        return strata::JsonValue(strata::JsonValue::Variant(true));
    }
    if (obj == Py_False) {
        return strata::JsonValue(strata::JsonValue::Variant(false));
    }

    if (PyUnicode_Check(obj)) {
        Py_ssize_t len = 0;
        const char* data = PyUnicode_AsUTF8AndSize(obj, &len);
        if (!data) {
            ok = false;
            return strata::JsonValue();
        }
        return strata::JsonValue(strata::JsonValue::Variant(std::string(data, len)));
    }

    if (PyLong_Check(obj)) {
        int overflow = 0;
        long long int_val = PyLong_AsLongLongAndOverflow(obj, &overflow);
        if (overflow == 0 && !PyErr_Occurred()) {
            return strata::JsonValue(strata::JsonValue::Variant(static_cast<double>(int_val)));
        }
        PyErr_Clear();

        PyObject* str_obj = PyObject_Str(obj);
        if (!str_obj) {
            ok = false;
            return strata::JsonValue();
        }
        Py_ssize_t len = 0;
        const char* data = PyUnicode_AsUTF8AndSize(str_obj, &len);
        if (!data) {
            Py_DECREF(str_obj);
            ok = false;
            return strata::JsonValue();
        }
        double value = 0.0;
        size_t consumed = 0;
        bool parsed =
            strata::util::parse_double_fast(data, static_cast<size_t>(len), value, consumed);
        Py_DECREF(str_obj);
        if (!parsed || consumed != static_cast<size_t>(len)) {
            PyErr_SetString(PyExc_ValueError, "Failed to convert large integer");
            ok = false;
            return strata::JsonValue();
        }
        return strata::JsonValue(strata::JsonValue::Variant(value));
    }

    if (PyFloat_Check(obj)) {
        return strata::JsonValue(strata::JsonValue::Variant(PyFloat_AS_DOUBLE(obj)));
    }

    if (is_container(obj)) {
        if (memo_contains(memo, obj, ok)) {
            int policy = strata_get_cycle_policy();
            if (policy == 1) {
                PyErr_SetString(PyExc_ValueError, "Cycle detected during JSONPath conversion");
                ok = false;
                return strata::JsonValue();
            }
            if (policy == 0) {
                if (PyErr_WarnEx(PyExc_RuntimeWarning, "Cycle detected during JSONPath conversion",
                                 1) < 0) {
                    ok = false;
                    return strata::JsonValue();
                }
            }
            return strata::JsonValue();
        }

        if (!memo_add(memo, obj)) {
            ok = false;
            return strata::JsonValue();
        }

        if (PyDict_Check(obj)) {
            strata::JsonValue::Object out;
            PyObject* key = nullptr;
            PyObject* value = nullptr;
            Py_ssize_t pos = 0;
            while (PyDict_Next(obj, &pos, &key, &value)) {
                if (!PyUnicode_Check(key)) {
                    PyErr_SetString(PyExc_TypeError, "Dict keys must be strings");
                    ok = false;
                    break;
                }
                Py_ssize_t klen = 0;
                const char* kdata = PyUnicode_AsUTF8AndSize(key, &klen);
                if (!kdata) {
                    ok = false;
                    break;
                }
                std::string kstr(kdata, klen);
                strata::JsonValue v = pyobject_to_json_value(value, memo, ok);
                if (!ok) {
                    break;
                }
                out.emplace(std::move(kstr), std::move(v));
            }
            memo_remove(memo, obj);
            return ok ? strata::JsonValue(strata::JsonValue::Variant(std::move(out)))
                      : strata::JsonValue();
        }

        strata::JsonValue::Array arr;
        Py_ssize_t size = PySequence_Size(obj);
        if (size < 0) {
            ok = false;
            memo_remove(memo, obj);
            return strata::JsonValue();
        }
        arr.reserve(static_cast<size_t>(size));
        for (Py_ssize_t i = 0; i < size; ++i) {
            PyObject* item = PySequence_GetItem(obj, i);
            if (!item) {
                ok = false;
                break;
            }
            strata::JsonValue v = pyobject_to_json_value(item, memo, ok);
            Py_DECREF(item);
            if (!ok) {
                break;
            }
            arr.push_back(std::move(v));
        }
        memo_remove(memo, obj);
        return ok ? strata::JsonValue(strata::JsonValue::Variant(std::move(arr)))
                  : strata::JsonValue();
    }

    PyErr_SetString(PyExc_TypeError, "Unsupported type for JSONPath search");
    ok = false;
    return strata::JsonValue();
}

//=============================================================================
// CompiledPath Type
//=============================================================================

typedef struct {
    PyObject_HEAD strata::CompiledPath* path;
} PyCompiledPath;

static void PyCompiledPath_dealloc(PyCompiledPath* self) {
    delete self->path;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyCompiledPath_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyCompiledPath* self = (PyCompiledPath*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->path = nullptr;
    }
    return (PyObject*)self;
}

// Forward declarations
static PyObject* PyCompiledPath_from_string(PyObject* cls, PyObject* args);
static PyObject* PyCompiledPath_execute(PyCompiledPath* self, PyObject* args);

// Method table
static PyMethodDef PyCompiledPath_methods[] = {
    {"from_string", (PyCFunction)PyCompiledPath_from_string, METH_VARARGS | METH_CLASS,
     "Create CompiledPath from JSONPath string"},
    {"execute", (PyCFunction)PyCompiledPath_execute, METH_VARARGS, "Execute path on cursor"},
    {NULL, NULL, 0, NULL}};

// Type object
static PyTypeObject PyCompiledPathType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "strata._strata.CompiledPath",
    .tp_basicsize = sizeof(PyCompiledPath),
    .tp_dealloc = (destructor)PyCompiledPath_dealloc,
    .tp_new = PyCompiledPath_new,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Compiled JSONPath Query",
    .tp_methods = PyCompiledPath_methods,
};

//=============================================================================
// Method Implementations
//=============================================================================

static PyObject* PyCompiledPath_from_string(PyObject* cls, PyObject* args) {
    const char* path_str;

    if (!PyArg_ParseTuple(args, "s", &path_str)) {
        return NULL;
    }

    STRATA_CPP_TRY

    auto result = strata::compile_jsonpath(path_str);
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
        return NULL;
    }

    PyCompiledPath* self = (PyCompiledPath*)PyType_GenericAlloc(&PyCompiledPathType, 0);
    if (!self)
        return NULL;

    self->path = new strata::CompiledPath(std::move(result.value));

    return (PyObject*)self;

    STRATA_CPP_CATCH
}

static PyObject* PyCompiledPath_execute(PyCompiledPath* self, PyObject* args) {
    PyObject* cursor_obj;

    if (!PyArg_ParseTuple(args, "O", &cursor_obj)) {
        return NULL;
    }

    // Accept either C extension JsonCursor or Python JsonCursor wrapper (has _cursor)
    PyObject* cursor_to_use = cursor_obj;
    PyObject* borrowed = NULL;
    if (!is_py_json_cursor(cursor_obj) && PyObject_HasAttrString(cursor_obj, "_cursor")) {
        borrowed = PyObject_GetAttrString(cursor_obj, "_cursor");
        if (borrowed && is_py_json_cursor(borrowed)) {
            cursor_to_use = borrowed;
        } else if (borrowed) {
            Py_DECREF(borrowed);
            borrowed = NULL;
        }
    }

    if (!is_py_json_cursor(cursor_to_use)) {
        Py_XDECREF(borrowed);
        PyErr_SetString(PyExc_TypeError, "Expected JsonCursor object");
        return NULL;
    }

    strata::JsonCursor* cursor_ptr = get_py_json_cursor(cursor_to_use);
    Py_XDECREF(borrowed);
    borrowed = NULL;
    if (!cursor_ptr) {
        PyErr_SetString(PyExc_TypeError, "Invalid JsonCursor");
        return NULL;
    }

    STRATA_CPP_TRY

    auto result_values = strata::eval_jsonpath(*cursor_ptr, *self->path);
    return json_value_list_to_python(result_values);

    STRATA_CPP_CATCH
}

//=============================================================================
// search() and compile_path() Functions
//=============================================================================

PyObject* strata_compile_path(PyObject* self, PyObject* args) {
    const char* path_str;

    if (!PyArg_ParseTuple(args, "s", &path_str)) {
        return NULL;
    }

    STRATA_CPP_TRY

    auto result = strata::compile_jsonpath(path_str);
    if (!result.ok()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
        return NULL;
    }

    PyCompiledPath* self_obj = (PyCompiledPath*)PyType_GenericAlloc(&PyCompiledPathType, 0);
    if (!self_obj)
        return NULL;

    self_obj->path = new strata::CompiledPath(std::move(result.value));

    return (PyObject*)self_obj;

    STRATA_CPP_CATCH
}

// Helper: search a single JsonValue with compiled path, respecting mem_eff.
// Returns new ref (Python list).
static PyObject* search_jsonvalue(const strata::JsonValue* raw,
                                  const strata::CompiledPath& compiled_path, bool mem_eff) {
    if (!raw) {
        PyErr_SetString(PyExc_TypeError, "Invalid JSON value");
        return NULL;
    }

    if (mem_eff) {
        // Memory-efficient: search on C++ tree, only materialize matched results
        std::vector<PyObject*> py_results;
        eval_step_jsonvalue(raw, compiled_path.steps(), 0, py_results);
        if (PyErr_Occurred()) {
            for (auto* p : py_results)
                Py_XDECREF(p);
            return NULL;
        }
        return pyobj_results_to_list_steal(py_results);
    }

    // Check if path needs full walk (recursive descent / filter)
    bool needs_full_walk = false;
    for (const auto& step : compiled_path.steps()) {
        if (step.op == strata::PathOp::RecursiveDescent || step.op == strata::PathOp::Filter) {
            needs_full_walk = true;
            break;
        }
    }

    if (needs_full_walk) {
        // Convert to Python dict and use fast PyObject-native path
        PyObject* py_data = json_value_to_python(*raw);
        if (!py_data)
            return NULL;
        std::vector<PyObject*> py_results;
        eval_step_pyobj(py_data, compiled_path.steps(), 0, py_results);
        PyObject* result = PyErr_Occurred() ? NULL : pyobj_results_to_list(py_results);
        Py_DECREF(py_data);
        return result;
    }

    // Simple path: direct JsonValue evaluation (no full-tree convert)
    std::vector<PyObject*> py_results;
    eval_step_jsonvalue(raw, compiled_path.steps(), 0, py_results);
    if (PyErr_Occurred()) {
        for (auto* p : py_results)
            Py_XDECREF(p);
        return NULL;
    }
    return pyobj_results_to_list_steal(py_results);
}

// Helper: search a Python object with compiled path
static PyObject* search_pyobj(PyObject* py_data, const strata::CompiledPath& compiled_path) {
    std::vector<PyObject*> py_results;
    eval_step_pyobj(py_data, compiled_path.steps(), 0, py_results);
    if (PyErr_Occurred())
        return NULL;
    return pyobj_results_to_list(py_results);
}

// Check if string looks like a file path (ends with .json/.ndjson/.jsonl)
static bool is_file_path(const char* str, size_t len) {
    if (len > 5 && strcmp(str + len - 5, ".json") == 0)
        return true;
    if (len > 7 && strcmp(str + len - 7, ".ndjson") == 0)
        return true;
    if (len > 6 && strcmp(str + len - 6, ".jsonl") == 0)
        return true;
    return false;
}

static bool is_ndjson_path(const char* str, size_t len) {
    if (len > 7 && strcmp(str + len - 7, ".ndjson") == 0)
        return true;
    if (len > 6 && strcmp(str + len - 6, ".jsonl") == 0)
        return true;
    return false;
}

PyObject* strata_search(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* data_obj;
    PyObject* path_obj;
    PyObject* mem_eff_obj = nullptr;

    static const char* kwlist[] = {"data", "path", "mem_eff", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|O", const_cast<char**>(kwlist), &data_obj,
                                     &path_obj, &mem_eff_obj)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Resolve mem_eff: per-call kwarg > global config
    bool mem_eff;
    if (mem_eff_obj && mem_eff_obj != Py_None) {
        mem_eff = PyObject_IsTrue(mem_eff_obj);
    } else {
        mem_eff = strata_config_mem_eff();
    }

    // Compile the path (if it's a string)
    strata::CompiledPath compiled_path;

    if (PyUnicode_Check(path_obj)) {
        const char* path_str = PyUnicode_AsUTF8(path_obj);
        if (!path_str)
            return NULL;

        auto compile_result = strata::compile_jsonpath(path_str);
        if (!compile_result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
            return NULL;
        }
        compiled_path = std::move(compile_result.value);
    } else if (Py_TYPE(path_obj) == &PyCompiledPathType) {
        PyCompiledPath* compiled_obj = (PyCompiledPath*)path_obj;
        compiled_path = *compiled_obj->path;
    } else {
        PyErr_SetString(PyExc_TypeError, "path must be a string or CompiledPath");
        return NULL;
    }

    // Handle JsonDocument inputs
    if (is_py_json_document(data_obj)) {
        auto* doc = get_py_json_document(data_obj);
        if (!doc) {
            PyErr_SetString(PyExc_TypeError, "Invalid JsonDocument");
            return NULL;
        }
        return search_jsonvalue(&doc->root_value(), compiled_path, mem_eff);
    }

    // Handle JsonCursor inputs
    if (is_py_json_cursor(data_obj)) {
        auto* cursor_ptr = get_py_json_cursor(data_obj);
        if (!cursor_ptr) {
            PyErr_SetString(PyExc_TypeError, "Invalid JsonCursor");
            return NULL;
        }
        const strata::JsonValue* raw = cursor_ptr->raw();
        return search_jsonvalue(raw, compiled_path, mem_eff);
    }

    // Handle string input — could be file path or JSON text
    if (PyUnicode_Check(data_obj)) {
        Py_ssize_t json_len;
        const char* json_data = PyUnicode_AsUTF8AndSize(data_obj, &json_len);
        if (!json_data)
            return NULL;

        // Check if it's a file path
        if (is_file_path(json_data, static_cast<size_t>(json_len))) {
            if (is_ndjson_path(json_data, static_cast<size_t>(json_len))) {
                // NDJSON/JSONL file: search each line, return list of result lists
                std::ifstream file(std::string(json_data, json_len),
                                   std::ios::binary | std::ios::ate);
                if (!file.is_open()) {
                    PyErr_Format(PyExc_FileNotFoundError, "Cannot open file: %s", json_data);
                    return NULL;
                }
                auto size = file.tellg();
                file.seekg(0);
                std::string content(static_cast<size_t>(size), '\0');
                file.read(content.data(), size);
                file.close();

                strata::NdjsonStream stream(content);
                PyObject* outer_list = PyList_New(0);
                if (!outer_list)
                    return NULL;

                while (stream.has_next()) {
                    auto line_result = stream.next();
                    if (!line_result.ok())
                        continue;

                    PyObject* line_results;
                    if (mem_eff) {
                        // Search directly on C++ JsonValue
                        line_results = search_jsonvalue(&line_result.value, compiled_path, true);
                    } else {
                        // Convert line to Python, search on Python objects
                        PyObject* py_line = json_value_to_python(line_result.value);
                        if (!py_line) {
                            Py_DECREF(outer_list);
                            return NULL;
                        }
                        line_results = search_pyobj(py_line, compiled_path);
                        Py_DECREF(py_line);
                    }

                    if (!line_results) {
                        Py_DECREF(outer_list);
                        return NULL;
                    }

                    // Flatten: extend outer list with line results
                    Py_ssize_t n = PyList_GET_SIZE(line_results);
                    for (Py_ssize_t i = 0; i < n; i++) {
                        PyObject* item = PyList_GET_ITEM(line_results, i);
                        Py_INCREF(item);
                        if (PyList_Append(outer_list, item) < 0) {
                            Py_DECREF(item);
                            Py_DECREF(line_results);
                            Py_DECREF(outer_list);
                            return NULL;
                        }
                        Py_DECREF(item);
                    }
                    Py_DECREF(line_results);
                }
                return outer_list;
            }

            // JSON file: mmap + parse
            if (mem_eff) {
                // Parse to C++ tree, search on C++ tree
                auto result = strata::parse_json_file(json_data);
                if (!result.ok()) {
                    PyErr_Format(PyExc_ValueError, "Failed to parse JSON file: %s", json_data);
                    return NULL;
                }
                emit_duplicate_key_warnings();
                return search_jsonvalue(&result.value.root_value(), compiled_path, true);
            } else {
                // Read file and parse to Python via SAX
                auto result = strata::parse_json_file(json_data);
                if (!result.ok()) {
                    PyErr_Format(PyExc_ValueError, "Failed to parse JSON file: %s", json_data);
                    return NULL;
                }
                emit_duplicate_key_warnings();
                return search_jsonvalue(&result.value.root_value(), compiled_path, false);
            }
        }

        // Regular JSON text
        PyObject* py_data = parse_json_to_python(std::string_view(json_data, json_len));
        if (!py_data) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return NULL;
        }
        emit_duplicate_key_warnings();
        PyObject* result = search_pyobj(py_data, compiled_path);
        Py_DECREF(py_data);
        return result;
    }

    if (PyBytes_Check(data_obj)) {
        char* json_data = nullptr;
        Py_ssize_t json_len = 0;
        if (PyBytes_AsStringAndSize(data_obj, &json_data, &json_len) < 0) {
            return NULL;
        }

        PyObject* py_data = parse_json_to_python(std::string_view(json_data, json_len));
        if (!py_data) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return NULL;
        }
        emit_duplicate_key_warnings();
        PyObject* result = search_pyobj(py_data, compiled_path);
        Py_DECREF(py_data);
        return result;
    }

    // Handle Python object (dict/list/scalar)
    return search_pyobj(data_obj, compiled_path);

    STRATA_CPP_CATCH
}

//=============================================================================
// Module Registration
//=============================================================================

int register_jsonpath_types(PyObject* module) {
    // Register CompiledPath type
    if (PyType_Ready(&PyCompiledPathType) < 0) {
        return -1;
    }
    Py_INCREF(&PyCompiledPathType);
    if (PyModule_AddObject(module, "CompiledPath", (PyObject*)&PyCompiledPathType) < 0) {
        Py_DECREF(&PyCompiledPathType);
        return -1;
    }

    return 0;
}

// Methods are added to main module via python_module.cpp
// No need for get_jsonpath_methods() here

#include "python_convert.h"
#include "python_document.h"
#include "python_types.h"
#include "strata/json/json_mmap.hpp"
#include "strata/json/json_parser_inline.hpp"
#include "strata/json/json_sax_handler.hpp"
#include "strata/json/ndjson_stream.hpp"
#include "strata/search/jsonpath.hpp"
#include "strata/util/fast_parse.hpp"

extern "C" {
int _PyDict_SetItem_KnownHash(PyObject* mp, PyObject* key, PyObject* item, Py_hash_t hash);
}

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <limits>
#include <string>
#include <vector>

// POSIX I/O for fast file read (avoids iostream overhead)
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

// Forward declaration: SAX-based parse to Python dict (defined in python_loads.cpp)
extern PyObject* parse_json_to_python(std::string_view text, bool validate_utf8);

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

//=============================================================================
// SAX-based JSONPath search: evaluates JSONPath during JSON parsing.
// Never builds a full tree (C++ or Python). Only creates Python objects for
// matched values. Fastest path + lowest memory.
//=============================================================================

class SaxSearchHandler {
  public:
    explicit SaxSearchHandler(const std::vector<strata::PathStep>& steps) {
        // Skip Root step
        size_t start = 0;
        if (!steps.empty() && steps[0].op == strata::PathOp::Root)
            start = 1;
        steps_ = steps.data() + start;
        nsteps_ = steps.size() - start;

        stack_.reserve(32);
        results_.reserve(256);

        // Initial state: the root value is at step 0
        pending_step_ = 0;
        pending_skip_ = false;
    }

    // Check if path can be handled by SAX search (no Filter, no Slice)
    static bool is_sax_compatible(const std::vector<strata::PathStep>& steps) {
        for (const auto& s : steps) {
            if (s.op == strata::PathOp::Filter || s.op == strata::PathOp::Slice)
                return false;
        }
        return true;
    }

    bool on_null() {
        if (capture_depth_ > 0)
            return cap_.on_null();
        if (skip_depth_ > 0)
            return true;
        if (pending_skip_) {
            pending_skip_ = false;
            return true;
        }
        if (pending_step_ >= nsteps_) {
            Py_INCREF(Py_None);
            results_.push_back(Py_None);
        }
        return true;
    }

    bool on_bool(bool v) {
        if (capture_depth_ > 0)
            return cap_.on_bool(v);
        if (skip_depth_ > 0)
            return true;
        if (pending_skip_) {
            pending_skip_ = false;
            return true;
        }
        if (pending_step_ >= nsteps_) {
            PyObject* obj = v ? Py_True : Py_False;
            Py_INCREF(obj);
            results_.push_back(obj);
        }
        return true;
    }

    bool on_int(int64_t v) {
        if (capture_depth_ > 0)
            return cap_.on_int(v);
        if (skip_depth_ > 0)
            return true;
        if (pending_skip_) {
            pending_skip_ = false;
            return true;
        }
        if (pending_step_ >= nsteps_) {
            results_.push_back(PyLong_FromLongLong(v));
        }
        return true;
    }

    bool on_uint(uint64_t v) {
        if (capture_depth_ > 0)
            return cap_.on_uint(v);
        if (skip_depth_ > 0)
            return true;
        if (pending_skip_) {
            pending_skip_ = false;
            return true;
        }
        if (pending_step_ >= nsteps_) {
            results_.push_back(PyLong_FromUnsignedLongLong(v));
        }
        return true;
    }

    bool on_double(double v) {
        if (capture_depth_ > 0)
            return cap_.on_double(v);
        if (skip_depth_ > 0)
            return true;
        if (pending_skip_) {
            pending_skip_ = false;
            return true;
        }
        if (pending_step_ >= nsteps_) {
            results_.push_back(PyFloat_FromDouble(v));
        }
        return true;
    }

    bool on_string(std::string_view v) {
        if (capture_depth_ > 0)
            return cap_.on_string(v);
        if (skip_depth_ > 0)
            return true;
        if (pending_skip_) {
            pending_skip_ = false;
            return true;
        }
        if (pending_step_ >= nsteps_) {
            results_.push_back(PyUnicode_FromStringAndSize(v.data(), v.size()));
        }
        return true;
    }

    bool on_start_object(size_t hint = 0) {
        if (capture_depth_ > 0) {
            capture_depth_++;
            return cap_.on_start_object(hint);
        }
        if (skip_depth_ > 0) {
            skip_depth_++;
            return true;
        }
        if (pending_skip_) {
            pending_skip_ = false;
            skip_depth_ = 1;
            return true;
        }

        size_t step = pending_step_;
        if (step >= nsteps_) {
            // Entire object is a match — capture it
            capture_depth_ = 1;
            cap_.reset();
            return cap_.on_start_object(hint);
        }

        // Push frame: entering an object at this step
        stack_.push_back({Frame::Object, step, -1});
        return true;
    }

    bool on_key(std::string_view k) {
        if (capture_depth_ > 0)
            return cap_.on_key(k);
        if (skip_depth_ > 0)
            return true;

        auto& frame = stack_.back();
        size_t s = frame.step;

        if (s >= nsteps_) {
            // All steps matched, but we're inside a non-captured object? Shouldn't happen.
            pending_skip_ = true;
            return true;
        }

        const auto& step = steps_[s];

        if (step.op == strata::PathOp::Field) {
            if (step.field.size() == k.size() && step.field == k) {
                pending_step_ = s + 1;
                pending_skip_ = false;
            } else {
                pending_skip_ = true;
            }
        } else if (step.op == strata::PathOp::Wildcard) {
            pending_step_ = s + 1;
            pending_skip_ = false;
        } else if (step.op == strata::PathOp::RecursiveDescent) {
            // RecursiveDescent: the field name is stored in step.field
            // (compiled as a single step, e.g. $..price → RecursiveDescent(field="price"))
            if (!step.field.empty() && step.field == k) {
                // Key matches the recursive descent field — advance past this step
                pending_step_ = s + 1;
                pending_skip_ = false;
            } else {
                // Stay at RecursiveDescent step — value will continue searching
                pending_step_ = s;
                pending_skip_ = false;
            }
        } else {
            // Other ops (Index, etc.) don't match object keys
            pending_skip_ = true;
        }
        return true;
    }

    bool on_end_object() {
        if (capture_depth_ > 0) {
            capture_depth_--;
            cap_.on_end_object();
            if (capture_depth_ == 0) {
                PyObject* val = cap_.take_root();
                if (val)
                    results_.push_back(val);
            }
            return true;
        }
        if (skip_depth_ > 0) {
            skip_depth_--;
            return true;
        }
        if (!stack_.empty())
            stack_.pop_back();
        return true;
    }

    bool on_start_array(size_t hint = 0) {
        if (capture_depth_ > 0) {
            capture_depth_++;
            return cap_.on_start_array(hint);
        }
        if (skip_depth_ > 0) {
            skip_depth_++;
            return true;
        }
        if (pending_skip_) {
            pending_skip_ = false;
            skip_depth_ = 1;
            return true;
        }

        size_t step = pending_step_;
        if (step >= nsteps_) {
            // Entire array is a match — capture it
            capture_depth_ = 1;
            cap_.reset();
            return cap_.on_start_array(hint);
        }

        // Push frame: entering an array at this step
        stack_.push_back({Frame::Array, step, -1});
        return true;
    }

    bool on_end_array() {
        if (capture_depth_ > 0) {
            capture_depth_--;
            cap_.on_end_array();
            if (capture_depth_ == 0) {
                PyObject* val = cap_.take_root();
                if (val)
                    results_.push_back(val);
            }
            return true;
        }
        if (skip_depth_ > 0) {
            skip_depth_--;
            return true;
        }
        if (!stack_.empty())
            stack_.pop_back();
        return true;
    }

    // Called between array elements (before each element value).
    // The parser calls: on_start_array → [on_* for elem0] → [on_* for elem1] → ... → on_end_array
    // We need to set pending_step_ for each element. This is done by hooking into the
    // value callbacks: when a value completes at the array level, the next value in the
    // array gets its pending_step_ set.
    //
    // Actually, the SAX parser doesn't call a separate "next element" callback.
    // We set pending_step_ for the first element in on_start_array, and for subsequent
    // elements after each value completes.
    //
    // Let's handle this: after on_start_array, the very next value callback is the first element.
    // After each element finishes (scalar or end_object/end_array), the next value callback
    // is the next element. So we need to set pending_step_ after on_start_array and after
    // each element completes.

    // Called before each value callback from the wrapper.
    // If the top frame is an Array (not in capture/skip mode), set up pending_step_ for the
    // element.
    void setup_array_element_if_needed() {
        if (capture_depth_ > 0 || skip_depth_ > 0 || pending_skip_)
            return;
        if (stack_.empty())
            return;
        auto& frame = stack_.back();
        if (frame.type != Frame::Array)
            return;
        setup_array_element();
    }

    // Helper: called when we're about to process the next array element
    void setup_array_element() {
        if (stack_.empty())
            return;
        auto& frame = stack_.back();
        if (frame.type != Frame::Array)
            return;

        frame.array_idx++;
        size_t s = frame.step;

        if (s >= nsteps_) {
            pending_step_ = s; // shouldn't normally happen
            return;
        }

        const auto& step = steps_[s];
        if (step.op == strata::PathOp::Wildcard) {
            pending_step_ = s + 1;
            pending_skip_ = false;
        } else if (step.op == strata::PathOp::Index) {
            if (frame.array_idx == static_cast<int>(step.index)) {
                pending_step_ = s + 1;
                pending_skip_ = false;
            } else {
                pending_skip_ = true;
            }
        } else if (step.op == strata::PathOp::RecursiveDescent) {
            // Array elements continue recursive descent search
            pending_step_ = s;
            pending_skip_ = false;
        } else {
            // Field or other op at array level — skip elements
            pending_skip_ = true;
        }
    }

    PyObject* take_results() {
        PyObject* list = PyList_New(static_cast<Py_ssize_t>(results_.size()));
        if (!list) {
            for (auto* p : results_)
                Py_XDECREF(p);
            results_.clear();
            return nullptr;
        }
        for (size_t i = 0; i < results_.size(); i++) {
            PyList_SET_ITEM(list, static_cast<Py_ssize_t>(i), results_[i]);
        }
        results_.clear();
        return list;
    }

  private:
    struct Frame {
        enum Type : uint8_t { Object, Array };
        Type type;
        size_t step;
        int array_idx;
    };

    const strata::PathStep* steps_;
    size_t nsteps_;

    std::vector<Frame> stack_;
    size_t pending_step_ = 0;
    bool pending_skip_ = false;

    int capture_depth_ = 0;
    int skip_depth_ = 0;

    // PythonObjectBuilder for capturing matched complex values.
    // Defined inline here since we need access to its SAX methods.
    // We reuse the same one from python_loads.cpp — but it's in an anonymous namespace there.
    // So we duplicate a minimal capture builder here.
    struct CaptureBuilder {
        PyObject* root_ = nullptr;
        std::vector<PyObject*> stack_;
        struct KeyEntry {
            PyObject* key;
            Py_hash_t hash;
        };
        std::vector<KeyEntry> keys_;
        std::vector<PyObject*> array_items_;
        std::vector<size_t> array_starts_;

        void reset() {
            if (root_) {
                Py_DECREF(root_);
                root_ = nullptr;
            }
            for (auto* p : stack_) {
                if (p)
                    Py_DECREF(p);
            }
            stack_.clear();
            for (auto& ke : keys_)
                Py_DECREF(ke.key);
            keys_.clear();
            for (auto* p : array_items_)
                Py_DECREF(p);
            array_items_.clear();
            array_starts_.clear();
        }

        ~CaptureBuilder() { reset(); }

        PyObject* take_root() {
            PyObject* r = root_;
            root_ = nullptr;
            return r;
        }

        bool push_value(PyObject* val) {
            if (!val)
                return false;
            if (stack_.empty()) {
                if (root_)
                    Py_DECREF(root_);
                root_ = val;
                return true;
            }
            PyObject* top = stack_.back();
            if (top == nullptr) {
                array_items_.push_back(val);
                return true;
            }
            if (keys_.empty()) {
                Py_DECREF(val);
                return false;
            }
            auto [key, hash] = keys_.back();
            keys_.pop_back();
            if (_PyDict_SetItem_KnownHash(top, key, val, hash) < 0) {
                Py_DECREF(key);
                Py_DECREF(val);
                return false;
            }
            Py_DECREF(key);
            Py_DECREF(val);
            return true;
        }

        bool on_null() {
            Py_INCREF(Py_None);
            return push_value(Py_None);
        }
        bool on_bool(bool v) {
            PyObject* obj = v ? Py_True : Py_False;
            Py_INCREF(obj);
            return push_value(obj);
        }
        bool on_int(int64_t v) { return push_value(PyLong_FromLongLong(v)); }
        bool on_uint(uint64_t v) { return push_value(PyLong_FromUnsignedLongLong(v)); }
        bool on_double(double v) { return push_value(PyFloat_FromDouble(v)); }
        bool on_string(std::string_view v) {
            return push_value(PyUnicode_FromStringAndSize(v.data(), v.size()));
        }
        bool on_start_object(size_t = 0) {
            PyObject* d = PyDict_New();
            if (!d)
                return false;
            stack_.push_back(d);
            return true;
        }
        bool on_key(std::string_view v) {
            PyObject* key = PyUnicode_FromStringAndSize(v.data(), v.size());
            if (!key)
                return false;
            PyUnicode_InternInPlace(&key);
            Py_hash_t h = PyObject_Hash(key);
            keys_.push_back({key, h});
            return true;
        }
        bool on_end_object() {
            if (stack_.empty())
                return false;
            PyObject* d = stack_.back();
            stack_.pop_back();
            return push_value(d);
        }
        bool on_start_array(size_t = 0) {
            stack_.push_back(nullptr);
            array_starts_.push_back(array_items_.size());
            return true;
        }
        bool on_end_array() {
            if (stack_.empty() || stack_.back() != nullptr)
                return false;
            stack_.pop_back();
            size_t start = array_starts_.back();
            array_starts_.pop_back();
            size_t count = array_items_.size() - start;
            PyObject* list = PyList_New(static_cast<Py_ssize_t>(count));
            if (!list) {
                for (size_t i = start; i < array_items_.size(); i++)
                    Py_DECREF(array_items_[i]);
                array_items_.resize(start);
                return false;
            }
            for (size_t i = 0; i < count; i++)
                PyList_SET_ITEM(list, static_cast<Py_ssize_t>(i), array_items_[start + i]);
            array_items_.resize(start);
            return push_value(list);
        }
    } cap_;

    std::vector<PyObject*> results_;
};

// SAX-based search: parse JSON text and evaluate JSONPath in a single pass.
// Returns PyList of matched values, or nullptr if path not SAX-compatible
// (caller should fallback to tree-based search).
static PyObject* sax_search_json(std::string_view text, const strata::CompiledPath& path) {
    if (!SaxSearchHandler::is_sax_compatible(path.steps()))
        return nullptr; // signal fallback

    SaxSearchHandler handler(path.steps());

    // The SAX parser calls value callbacks directly. But for arrays, we need to
    // set pending_step_ before each element. We do this by wrapping the parser:
    // after on_start_array and after each element's value callbacks complete,
    // setup_array_element() is called.
    //
    // Unfortunately parse_sax_inline doesn't have an "array element separator" callback.
    // We need a wrapper that intercepts calls to inject array element setup.

    // Wrapper handler that injects array element tracking
    struct SaxWrapper {
        SaxSearchHandler& h;

        bool on_null() {
            h.setup_array_element_if_needed();
            return h.on_null();
        }
        bool on_bool(bool v) {
            h.setup_array_element_if_needed();
            return h.on_bool(v);
        }
        bool on_int(int64_t v) {
            h.setup_array_element_if_needed();
            return h.on_int(v);
        }
        bool on_uint(uint64_t v) {
            h.setup_array_element_if_needed();
            return h.on_uint(v);
        }
        bool on_double(double v) {
            h.setup_array_element_if_needed();
            return h.on_double(v);
        }
        bool on_string(std::string_view v) {
            h.setup_array_element_if_needed();
            return h.on_string(v);
        }
        bool on_start_object(size_t hint = 0) {
            h.setup_array_element_if_needed();
            return h.on_start_object(hint);
        }
        bool on_key(std::string_view v) { return h.on_key(v); }
        bool on_end_object() { return h.on_end_object(); }
        bool on_start_array(size_t hint = 0) {
            h.setup_array_element_if_needed();
            return h.on_start_array(hint);
        }
        bool on_end_array() { return h.on_end_array(); }
    };

    SaxWrapper wrapper{handler};
    PyGcPause gc_pause;
    auto status = strata::parse_sax_inline(text, wrapper, /*validate_utf8=*/false);
    if (status != strata::Status::Ok) {
        // Parse error — clean up any partial results
        PyObject* partial = handler.take_results();
        Py_XDECREF(partial);
        return nullptr;
    }
    return handler.take_results();
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

// Forward declaration for iterator support
extern PyObject* create_list_iterator(PyObject* list);

// Helper: compile path from string or CompiledPath object
static bool compile_path_from_arg(PyObject* path_obj, strata::CompiledPath& out) {
    if (PyUnicode_Check(path_obj)) {
        const char* path_str = PyUnicode_AsUTF8(path_obj);
        if (!path_str)
            return false;
        auto compile_result = strata::compile_jsonpath(path_str);
        if (!compile_result.ok()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSONPath expression");
            return false;
        }
        out = std::move(compile_result.value);
        return true;
    }
    if (Py_TYPE(path_obj) == &PyCompiledPathType) {
        PyCompiledPath* compiled_obj = (PyCompiledPath*)path_obj;
        out = *compiled_obj->path;
        return true;
    }
    PyErr_SetString(PyExc_TypeError, "path must be a string or CompiledPath");
    return false;
}

// search(): file paths only (.json/.ndjson/.jsonl)
PyObject* strata_search(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* filepath_obj;
    PyObject* path_obj;
    int iterator = 0;

    static const char* kwlist[] = {"filepath", "path", "iterator", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|p", const_cast<char**>(kwlist),
                                     &filepath_obj, &path_obj, &iterator)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Validate filepath is a string
    if (!PyUnicode_Check(filepath_obj)) {
        PyErr_SetString(PyExc_TypeError,
                        "search() requires a file path (str). Use query() for dict/list.");
        return NULL;
    }

    Py_ssize_t fp_len;
    const char* filepath = PyUnicode_AsUTF8AndSize(filepath_obj, &fp_len);
    if (!filepath)
        return NULL;

    // Validate file extension
    if (!is_file_path(filepath, static_cast<size_t>(fp_len))) {
        PyErr_SetString(PyExc_TypeError,
                        "search() requires a file path ending in .json, .ndjson, or .jsonl");
        return NULL;
    }

    // Compile path
    strata::CompiledPath compiled_path;
    if (!compile_path_from_arg(path_obj, compiled_path))
        return NULL;

    PyObject* result_list = nullptr;

    if (is_ndjson_path(filepath, static_cast<size_t>(fp_len))) {
        // NDJSON/JSONL file: search each line, flatten results
        std::ifstream file(std::string(filepath, fp_len), std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            PyErr_Format(PyExc_FileNotFoundError, "Cannot open file: %s", filepath);
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

            PyObject* py_line = json_value_to_python(line_result.value);
            if (!py_line) {
                Py_DECREF(outer_list);
                return NULL;
            }
            PyObject* line_results = search_pyobj(py_line, compiled_path);
            Py_DECREF(py_line);

            if (!line_results) {
                Py_DECREF(outer_list);
                return NULL;
            }

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
        result_list = outer_list;
    } else {
        // JSON file: read into buffer, then try SAX search (fastest, no tree at all).
        // Falls back to SAX→Python dict→query for Filter/Slice paths.
        int fd = ::open(filepath, O_RDONLY);
        if (fd < 0) {
            PyErr_Format(PyExc_FileNotFoundError, "Cannot open file: %s", filepath);
            return NULL;
        }
        struct stat sb;
        if (fstat(fd, &sb) < 0) {
            ::close(fd);
            PyErr_Format(PyExc_IOError, "Cannot stat file: %s", filepath);
            return NULL;
        }
        size_t file_size = static_cast<size_t>(sb.st_size);
        if (file_size == 0) {
            ::close(fd);
            PyErr_SetString(PyExc_ValueError, "Empty file");
            return NULL;
        }

        static thread_local char* file_buf = nullptr;
        static thread_local size_t file_buf_cap = 0;
        if (file_buf_cap < file_size) {
            char* new_buf = static_cast<char*>(std::realloc(file_buf, file_size));
            if (!new_buf) {
                ::close(fd);
                return PyErr_NoMemory();
            }
            file_buf = new_buf;
            file_buf_cap = file_size;
        }
        ssize_t bytes_read = ::read(fd, file_buf, file_size);
        ::close(fd);
        if (bytes_read < 0 || static_cast<size_t>(bytes_read) != file_size) {
            PyErr_Format(PyExc_IOError, "Failed to read file: %s", filepath);
            return NULL;
        }

        std::string_view file_text(file_buf, file_size);

        // Try SAX-based search first (no tree, lowest memory)
        result_list = sax_search_json(file_text, compiled_path);

        if (!result_list && !PyErr_Occurred()) {
            // Fallback: SAX search not supported (Filter/Slice path).
            // SAX→Python dict → query
            PyObject* py_data;
            {
                PyGcPause gc_pause;
                py_data = parse_json_to_python(file_text, /*validate_utf8=*/false);
            }
            if (!py_data) {
                if (!PyErr_Occurred())
                    PyErr_Format(PyExc_ValueError, "Failed to parse JSON file: %s", filepath);
                return NULL;
            }
            result_list = search_pyobj(py_data, compiled_path);
            Py_DECREF(py_data);
        }
    }

    if (!result_list)
        return NULL;

    if (iterator) {
        PyObject* it = create_list_iterator(result_list);
        Py_DECREF(result_list);
        return it;
    }

    return result_list;

    STRATA_CPP_CATCH
}

// query(): dict/list only, no file paths
PyObject* strata_query(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* data_obj;
    PyObject* path_obj;
    int iterator = 0;

    static const char* kwlist[] = {"data", "path", "iterator", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|p", const_cast<char**>(kwlist), &data_obj,
                                     &path_obj, &iterator)) {
        return NULL;
    }

    STRATA_CPP_TRY

    // Validate input is dict/list/tuple
    if (!PyDict_Check(data_obj) && !PyList_Check(data_obj) && !PyTuple_Check(data_obj)) {
        PyErr_SetString(PyExc_TypeError,
                        "query() requires a dict or list. Use search() for file paths.");
        return NULL;
    }

    // Compile path
    strata::CompiledPath compiled_path;
    if (!compile_path_from_arg(path_obj, compiled_path))
        return NULL;

    PyObject* result_list = search_pyobj(data_obj, compiled_path);
    if (!result_list)
        return NULL;

    if (iterator) {
        PyObject* it = create_list_iterator(result_list);
        Py_DECREF(result_list);
        return it;
    }

    return result_list;

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

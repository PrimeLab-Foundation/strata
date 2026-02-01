# Python C API Migration Plan

**Date**: 2026-01-30\
**Goal**: Replace pybind11 with direct Python C API for 10-20% performance gain\
**Strategy**: Keep C++ core, rewrite bindings only

______________________________________________________________________

## Why This Is The Right Move

### Expected Gains

- **Python C API overhead**: ~1.0ms (30% of API time)
- **Better type dispatch**: ~0.5ms (direct PyObject checks)
- **Tighter integration**: ~0.5ms (less abstraction)
- **Total estimated**: **2ms gain (10.55ms → 8.5ms)**
- **New rank**: Still #3, but **1.6x** from #1 (vs current 2.0x)

### Benefits Over pybind11

1. ✅ Zero abstraction overhead
1. ✅ Direct PyObject manipulation
1. ✅ Like msgspec architecture
1. ✅ Keep C++ core (safety, algorithms)
1. ✅ Better control over conversions

______________________________________________________________________

## Migration Strategy

### Phase 1: Create Python C API Layer ✅

**Create new module**: `src/strata/python/` with C API wrappers

**Files to create**:

- `python_module.c` - Module initialization
- `python_dumps.c` - Serialization binding
- `python_loads.c` - Parsing binding
- `python_ndjson.c` - NDJSON binding
- `python_jsonpath.c` - JSONPath binding
- `python_types.h` - Type definitions

**Keep**:

- All C++ core logic (`src/strata/json/`, `src/strata/search/`)
- Algorithms, SIMD, optimizations
- Just replace Python interface

______________________________________________________________________

## Phase 2: Module-by-Module Migration

### 2.1: dumps (Serialization)

**Priority**: HIGH (main bottleneck)

**Current (pybind11)**:

```cpp
m.def("dumps", &serialize_python_fast, "Serialize to JSON");
```

**New (Python C API)**:

```c
static PyObject* strata_dumps(PyObject* self, PyObject* args) {
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }
    
    // Call C++ serializer
    std::string result = strata::serialize_python_fast_c(obj);
    
    return PyUnicode_FromStringAndSize(result.c_str(), result.size());
}
```

**Estimated gain**: 1ms

______________________________________________________________________

### 2.2: loads (Parsing)

**Priority**: HIGH

**Current (pybind11)**:

```cpp
m.def("loads", [](const std::string& s) {
    // ...
});
```

**New (Python C API)**:

```c
static PyObject* strata_loads(PyObject* self, PyObject* args) {
    const char* data;
    Py_ssize_t len;
    
    if (!PyArg_ParseTuple(args, "s#", &data, &len)) {
        return NULL;
    }
    
    // Parse and convert to Python directly
    return strata_json_to_python(data, len);
}
```

**Estimated gain**: 0.5ms

______________________________________________________________________

### 2.3: NDJSON Streaming

**Priority**: MEDIUM

**New approach**:

- Direct PyList creation
- No intermediate objects
- Stream directly to Python

**Estimated gain**: 0.3ms

______________________________________________________________________

### 2.4: JSONPath

**Priority**: LOW (already #1)

**Keep working** - only migrate if time permits

______________________________________________________________________

## Implementation Plan

### Step 1: Setup (30 min)

- [x] Create `src/strata/python/` directory
- [ ] Create `python_module.c` skeleton
- [ ] Update `setup.py` for C extension
- [ ] Test module loads

### Step 2: dumps Migration (2-3 hours)

- [ ] Create `python_dumps.c`
- [ ] Direct PyObject serialization
- [ ] PyDict_Next for dicts
- [ ] PyList_GET_ITEM for lists
- [ ] Test compatibility
- [ ] Benchmark

### Step 3: loads Migration (2-3 hours)

- [ ] Create `python_loads.c`
- [ ] C++ to PyObject converter
- [ ] Direct PyDict_New, PyList_New
- [ ] Test compatibility
- [ ] Benchmark

### Step 4: NDJSON Migration (1-2 hours)

- [ ] Create `python_ndjson.c`
- [ ] Iterator object
- [ ] Test compatibility
- [ ] Benchmark

### Step 5: Cleanup & Testing (1-2 hours)

- [ ] Remove pybind11 dependency
- [ ] Update documentation
- [ ] Run full test suite
- [ ] Final benchmarks

**Total estimated time**: 6-12 hours

______________________________________________________________________

## Technical Details

### Module Structure

```c
// python_module.c
static PyMethodDef strata_methods[] = {
    {"dumps", strata_dumps, METH_O, "Serialize to JSON"},
    {"loads", strata_loads, METH_VARARGS, "Parse JSON"},
    {"iter_ndjson", strata_iter_ndjson, METH_VARARGS, "NDJSON iterator"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef strata_module = {
    PyModuleDef_HEAD_INIT,
    "_strata",
    "Fast JSON library",
    -1,
    strata_methods
};

PyMODINIT_FUNC PyInit__strata(void) {
    return PyModule_Create(&strata_module);
}
```

### Type Conversion Strategy

**C++ to Python**:

```c
PyObject* json_value_to_python(const strata::JsonValue& val) {
    if (val.is_null()) return Py_None;
    if (val.is_bool()) return PyBool_FromLong(val.as_bool());
    if (val.is_number()) return PyFloat_FromDouble(val.as_number());
    if (val.is_string()) {
        const std::string& s = val.as_string();
        return PyUnicode_FromStringAndSize(s.c_str(), s.size());
    }
    if (val.is_array()) {
        const auto& arr = val.as_array();
        PyObject* list = PyList_New(arr.size());
        for (size_t i = 0; i < arr.size(); ++i) {
            PyList_SET_ITEM(list, i, json_value_to_python(arr[i]));
        }
        return list;
    }
    if (val.is_object()) {
        const auto& obj = val.as_object();
        PyObject* dict = PyDict_New();
        for (const auto& [key, value] : obj) {
            PyObject* py_key = PyUnicode_FromStringAndSize(key.c_str(), key.size());
            PyObject* py_val = json_value_to_python(value);
            PyDict_SetItem(dict, py_key, py_val);
            Py_DECREF(py_key);
        }
        return dict;
    }
    return Py_None;
}
```

### Error Handling

```c
// Set Python exception from C++ exception
void set_python_exception(const std::exception& e) {
    PyErr_SetString(PyExc_ValueError, e.what());
}

// Try-catch wrapper
#define STRATA_TRY_BEGIN try {
#define STRATA_TRY_END } catch (const std::exception& e) { \
    set_python_exception(e); \
    return NULL; \
}
```

______________________________________________________________________

## build System Changes

### setup.py Changes

**Before (pybind11)**:

```python
ext_modules = [
    Pybind11Extension(
        "strata._strata",
        sources=[...],
    ),
]
```

**After (Python C API)**:

```python
ext_modules = [
    Extension(
        "strata._strata",
        sources=[
            "src/strata/python/python_module.c",
            "src/strata/python/python_dumps.c",
            "src/strata/python/python_loads.c",
            # ... C++ sources
        ],
        include_dirs=["include", "/usr/include/python3.11"],
        extra_compile_args=["-std=c++20"],
    ),
]
```

### CMakeLists.txt

Keep for C++ tests, not needed for Python module.

______________________________________________________________________

## Risk Mitigation

### Risks

1. **Reference counting bugs**: Manual Py_INCREF/Py_DECREF
1. **Type conversion errors**: Edge cases
1. **Memory leaks**: No RAII in C API
1. **API breakage**: Ensure compatibility

### Mitigation

1. ✅ Use RAII wrappers where possible
1. ✅ Extensive testing
1. ✅ Valgrind for leak detection
1. ✅ Keep test suite comprehensive

______________________________________________________________________

## Expected Results

### Performance

| Feature | pybind11 | Python C API | Gain |
| ------- | -------- | ------------ | ---- |
| dumps   | 10.55ms  | ~8.5ms       | 20%  |
| loads   | 9.21ms   | ~8.5ms       | 8%   |
| NDJSON  | 38.89ms  | ~36ms        | 7%   |

### Overall Ranking

| Rank | Library    | dumps      | Assessment         |
| ---- | ---------- | ---------- | ------------------ |
| #1   | orjson     | 5.15ms     | Rust (target)      |
| #2   | msgspec    | 5.49ms     | Cython             |
| #3   | **strata** | **~8.5ms** | **C++ + C API** ✅ |
| #4   | ujson      | 19.98ms    | Pure C             |

**Gap to #1**: 1.6x (improved from 2.0x)

______________________________________________________________________

## Success Criteria

1. ✅ All tests pass
1. ✅ 15-20% performance gain
1. ✅ No memory leaks
1. ✅ API compatibility maintained
1. ✅ Code quality preserved

______________________________________________________________________

## Next Steps

1. **Create skeleton** (now)
1. **Implement dumps** (highest impact)
1. **Test & benchmark**
1. **Implement loads**
1. **Complete migration**

Let's begin! 🚀

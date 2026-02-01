#ifndef STRATA_PYTHON_TYPES_H
#define STRATA_PYTHON_TYPES_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

// RAII wrapper for PyObject (when used from C++)
#ifdef __cplusplus
struct PyObjectPtr {
    PyObject* ptr;

    explicit PyObjectPtr(PyObject* p = nullptr) : ptr(p) {}
    ~PyObjectPtr() { Py_XDECREF(ptr); }

    PyObjectPtr(const PyObjectPtr&) = delete;
    PyObjectPtr& operator=(const PyObjectPtr&) = delete;

    PyObject* release() {
        PyObject* p = ptr;
        ptr = nullptr;
        return p;
    }

    operator PyObject*() const { return ptr; }
    PyObject* operator->() const { return ptr; }
};

// Temporarily disable GC to reduce collection overhead during bulk object creation.
struct PyGcPause {
    int was_enabled;
    PyGcPause() : was_enabled(PyGC_IsEnabled()) {
        if (was_enabled)
            PyGC_Disable();
    }
    ~PyGcPause() {
        if (was_enabled)
            PyGC_Enable();
    }
    PyGcPause(const PyGcPause&) = delete;
    PyGcPause& operator=(const PyGcPause&) = delete;
};
#endif

// Branch prediction hints for hot paths (e.g. dumps)
#ifndef LIKELY
#define LIKELY(x) __builtin_expect(!!(x), 1)
#endif
#ifndef UNLIKELY
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif

// Error handling macros
#define STRATA_RETURN_IF_NULL(expr)                                                                \
    do {                                                                                           \
        if ((expr) == NULL)                                                                        \
            return NULL;                                                                           \
    } while (0)

#define STRATA_RETURN_IF_ERROR(expr)                                                               \
    do {                                                                                           \
        if ((expr) < 0)                                                                            \
            return NULL;                                                                           \
    } while (0)

// C++ exception to Python exception converter
#ifdef __cplusplus
#define STRATA_CPP_TRY try {
#define STRATA_CPP_CATCH                                                                           \
    }                                                                                              \
    catch (const std::exception& e) {                                                              \
        if (!PyErr_Occurred()) {                                                                   \
            PyErr_SetString(PyExc_RuntimeError, e.what());                                         \
        }                                                                                          \
        return NULL;                                                                               \
    }                                                                                              \
    catch (...) {                                                                                  \
        if (!PyErr_Occurred()) {                                                                   \
            PyErr_SetString(PyExc_RuntimeError, "Unknown C++ exception");                          \
        }                                                                                          \
        return NULL;                                                                               \
    }
#endif

#ifdef __cplusplus
}
#endif

#endif // STRATA_PYTHON_TYPES_H

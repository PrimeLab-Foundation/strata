/**
 * @file python_ndjson.cpp
 * @brief NDJSON to Python, eagerly or lazily.
 *
 * Each line is parsed straight into Python objects with the same builder
 * `loads` uses, so integers stay exact and no intermediate C++ tree is built.
 *
 * The invalid-line contract is identical in both modes: a malformed line
 * raises `ValueError` unless the caller passed `skip_errors=True`. What
 * differs is *when* — the eager form reports at load time, the lazy form at
 * the failing line, because that is when it reaches it (docs/context/api.md).
 */

#include "python_types.h"
#include "strata/json/ndjson_stream.hpp"

#include <new>
#include <string>
#include <string_view>

namespace strata::bindings {

namespace {

/// What a failing line is called. A line nested past kMaxNestingDepth is a
/// refusal, not malformed text, and says so -- with the line number, because
/// that is what makes an NDJSON error actionable (docs/context/api.md).
[[nodiscard]] const char* line_message(Status status) noexcept {
    return status == Status::DepthExceeded ? kDepthExceededMessage : "Invalid JSON";
}

/// Owns the file text and walks it one line at a time.
struct NdjsonIteratorObject {
    PyObject_HEAD std::string* text;
    NdjsonStream* stream;
    bool skip_errors;
};

// See python_document.cpp: zeroing the tail of a static PyTypeObject is what
// CPython prescribes, but the compiler cannot know that.
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
PyTypeObject kNdjsonIteratorType = {PyVarObject_HEAD_INIT(nullptr, 0)};
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

void iterator_dealloc(PyObject* self) {
    auto* iterator = reinterpret_cast<NdjsonIteratorObject*>(self);
    delete iterator->stream; // the stream borrows the text, so it goes first
    delete iterator->text;
    PyObject_Free(self);
}

PyObject* iterator_self(PyObject* self) { return Py_NewRef(self); }

PyObject* iterator_next(PyObject* self) {
    STRATA_CPP_TRY
    auto* iterator = reinterpret_cast<NdjsonIteratorObject*>(self);
    for (;;) {
        std::string_view line;
        if (!iterator->stream->next_line(line))
            return nullptr; // exhausted: StopIteration, no exception set

        Status status = Status::Ok;
        PyObject* value = loads_to_python(line, /*validate_utf8=*/false, &status);
        if (value != nullptr)
            return value;

        if (!iterator->skip_errors) {
            PyErr_Clear();
            PyErr_Format(PyExc_ValueError, "%s on line %zu", line_message(status),
                         iterator->stream->line_number());
            return nullptr;
        }
        PyErr_Clear(); // the caller opted in to losing this line
    }
    STRATA_CPP_CATCH
}

} // namespace

bool register_ndjson_iterator_type(PyObject* module) {
    kNdjsonIteratorType.tp_name = "strata._strata.NdjsonIterator";
    kNdjsonIteratorType.tp_basicsize = sizeof(NdjsonIteratorObject);
    kNdjsonIteratorType.tp_dealloc = iterator_dealloc;
    kNdjsonIteratorType.tp_flags = Py_TPFLAGS_DEFAULT;
    kNdjsonIteratorType.tp_doc = PyDoc_STR("Lazy iterator over the records of an NDJSON file.");
    kNdjsonIteratorType.tp_iter = iterator_self;
    kNdjsonIteratorType.tp_iternext = iterator_next;

    if (PyType_Ready(&kNdjsonIteratorType) < 0)
        return false;
    // Not exported by name: instances are only ever obtained from load().
    (void)module;
    return true;
}

PyObject* ndjson_to_list(std::string_view text, bool skip_errors) {
    NdjsonStream stream(text);
    PyRef records(PyList_New(0));
    if (!records)
        return nullptr;

    for (;;) {
        std::string_view line;
        if (!stream.next_line(line))
            break;

        Status status = Status::Ok;
        PyRef value(loads_to_python(line, /*validate_utf8=*/false, &status));
        if (!value) {
            if (skip_errors) {
                PyErr_Clear();
                continue;
            }
            PyErr_Clear();
            PyErr_Format(PyExc_ValueError, "%s on line %zu", line_message(status),
                         stream.line_number());
            return nullptr;
        }
        if (PyList_Append(records.get(), value.get()) != 0)
            return nullptr;
    }
    return records.release();
}

PyObject* make_ndjson_iterator(std::string&& text, bool skip_errors) {
    auto* self = PyObject_New(NdjsonIteratorObject, &kNdjsonIteratorType);
    if (self == nullptr)
        return nullptr;

    self->text = new (std::nothrow) std::string(std::move(text));
    if (self->text == nullptr) {
        PyObject_Free(self);
        return PyErr_NoMemory();
    }
    self->stream = new (std::nothrow) NdjsonStream(*self->text);
    if (self->stream == nullptr) {
        delete self->text;
        PyObject_Free(self);
        return PyErr_NoMemory();
    }
    self->skip_errors = skip_errors;
    return reinterpret_cast<PyObject*>(self);
}

} // namespace strata::bindings

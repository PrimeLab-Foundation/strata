/**
 * @file python_numpy_twins.cpp
 * @brief numpy_twins_hold(): the type numbers `numpy_kind` admits, proven
 * against the running numpy (python_numpy_twins.h).
 */

#include "python_numpy_twins.h"

#include "python_native_types.h"

namespace strata::bindings::native {

namespace {

/// One type number the twins admit: the type character `numpy.dtype` takes
/// for it, and the dtype kind and item size that number must carry.
struct NumpyTwinRow {
    const char* code;
    long number;
    char kind;
    Py_ssize_t itemsize;
};

inline constexpr NumpyTwinRow kNumpyTwinRows[] = {
    {"?", kNumpyBool, 'b', 1},
    {"b", kNumpyFirstInteger, 'i', sizeof(signed char)},
    {"B", 2, 'u', sizeof(unsigned char)},
    {"h", 3, 'i', sizeof(short)},
    {"H", 4, 'u', sizeof(unsigned short)},
    {"i", 5, 'i', sizeof(int)},
    {"I", 6, 'u', sizeof(unsigned int)},
    {"l", 7, 'i', sizeof(long)},
    {"L", 8, 'u', sizeof(unsigned long)},
    {"q", 9, 'i', sizeof(long long)},
    {"Q", kNumpyLastInteger, 'u', sizeof(unsigned long long)},
    {"f", kNumpyFloat32, 'f', 4},
    {"e", kNumpyFloat16, 'f', 2},
};

/// The value @p row's probe scalar is built from: `True`, `0.1`, or the integer
/// type's extreme -- its minimum when signed -- which reaches the sign and every byte.
PyObject* twin_probe(const NumpyTwinRow& row) {
    if (row.kind == 'b')
        return Py_NewRef(Py_True);
    if (row.kind == 'f')
        return PyFloat_FromDouble(0.1);
    const unsigned long long ones = row.itemsize >= 8 ? ~0ULL : (1ULL << (8 * row.itemsize)) - 1;
    if (row.kind == 'u')
        return PyLong_FromUnsignedLongLong(ones);
    return PyLong_FromLongLong(-static_cast<long long>(ones >> 1) - 1);
}

/// Whether `@p owner.@p name` is an exact `int` equal to @p expected (>= 0).
/// False, possibly with an error set, otherwise.
bool attribute_is(PyObject* owner, const char* name, long expected) {
    const PyRef value(PyObject_GetAttrString(owner, name));
    return value && PyLong_CheckExact(value.get()) && PyLong_AsLong(value.get()) == expected;
}

/// One row of numpy_twins_hold(). False, possibly with an error set, when it does not hold.
bool twin_row_holds(PyObject* dtype_factory, const NumpyTwinRow& row) {
    const PyRef code(PyUnicode_FromString(row.code));
    if (!code)
        return false;
    const PyRef named(PyObject_CallOneArg(dtype_factory, code.get()));
    if (!named)
        return false;
    const PyRef type(PyObject_GetAttrString(named.get(), "type"));
    if (!type || !PyType_Check(type.get()))
        return false;
    const PyRef probe(twin_probe(row));
    if (!probe)
        return false;
    const PyRef scalar(PyObject_CallOneArg(type.get(), probe.get()));
    if (!scalar || reinterpret_cast<PyObject*>(Py_TYPE(scalar.get())) != type.get())
        return false;
    // The scalar's own dtype: what numpy_kind() reads.
    const PyRef dtype(PyObject_GetAttrString(scalar.get(), "dtype"));
    if (!dtype || !attribute_is(dtype.get(), "num", row.number) ||
        !attribute_is(dtype.get(), "itemsize", static_cast<long>(row.itemsize)))
        return false;
    const PyRef own_type(PyObject_GetAttrString(dtype.get(), "type"));
    if (!own_type || own_type.get() != type.get())
        return false;
    const PyRef kind(PyObject_GetAttrString(dtype.get(), "kind"));
    if (!kind || !PyUnicode_Check(kind.get()) || PyUnicode_GET_LENGTH(kind.get()) != 1 ||
        PyUnicode_READ_CHAR(kind.get(), 0) != static_cast<Py_UCS4>(row.kind))
        return false;
    const Kind twin_kind = row.kind == 'b'   ? Kind::NumpyBool
                           : row.kind == 'f' ? Kind::NumpyFloat
                                             : Kind::NumpyInteger;
    const PyRef twin(numpy_plain(scalar.get(), twin_kind));
    if (!twin)
        return false;
    const PyRef item(PyObject_CallMethod(scalar.get(), "item", nullptr));
    if (!item || Py_TYPE(twin.get()) != Py_TYPE(item.get()))
        return false;
    if (row.kind == 'b')
        return twin.get() == item.get();
    if (row.kind == 'f')
        return PyFloat_CheckExact(twin.get()) &&
               PyFloat_AS_DOUBLE(twin.get()) == PyFloat_AS_DOUBLE(item.get());
    return PyLong_CheckExact(twin.get()) &&
           PyObject_RichCompareBool(twin.get(), item.get(), Py_EQ) == 1;
}

} // namespace

bool numpy_twins_hold(PyObject* numpy) {
    const PyRef factory(PyObject_GetAttrString(numpy, "dtype"));
    if (!factory)
        return false;
    for (const NumpyTwinRow& row : kNumpyTwinRows)
        if (!twin_row_holds(factory.get(), row))
            return false;
    return true;
}

} // namespace strata::bindings::native

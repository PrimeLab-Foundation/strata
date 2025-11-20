#include "dummy.hpp"

#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;

// This defines the Python extension module named "_strata"
PYBIND11_MODULE(_strata, m) {
    m.doc() = "Minimal Strata C++ core module (temporary stub)";

    m.def("add", &add, py::arg("a"), py::arg("b"), "Add two integers and return the result.");

    m.def("hello", &hello, py::arg("name") = "world", "Return a friendly greeting.");
}

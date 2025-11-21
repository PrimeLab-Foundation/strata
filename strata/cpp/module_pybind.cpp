#include "dummy/dummy.hpp"
#include "json_core.hpp"
#include "json_cursor.hpp"
#include "json_document.hpp"
#include "json_parse.hpp"

#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;
using namespace strata;

// This defines the Python extension module named "_strata"
PYBIND11_MODULE(_strata, m) {
    m.doc() = "Strata minimal C++ core: dummy + JSON model/cursor (no simdjson)";

    // ------------------------------------------------------------------
    // dummy bindings
    // ------------------------------------------------------------------
    m.def("add", &add, py::arg("a"), py::arg("b"), "Add two integers and return the result.");
    m.def("hello", &hello, py::arg("name") = "world", "Return a friendly greeting.");

    // ------------------------------------------------------------------
    // JSON bindings
    // ------------------------------------------------------------------
    py::class_<JsonDocument>(m, "JsonDocument")
        .def_static(
            "from_string",
            [](const std::string& text) {
                auto res = parse_json(text);
                if (!res.ok())
                    throw py::value_error("failed to parse JSON");
                return JsonDocument(std::move(res.value));
            },
            py::arg("text"))
        .def(
            "root", [](const JsonDocument& doc) { return JsonCursor(doc.root_ptr()); },
            "Return a cursor at the document root");

    py::class_<JsonCursor>(m, "JsonCursor")
        .def("is_null", &JsonCursor::is_null)
        .def("is_bool", &JsonCursor::is_bool)
        .def("is_number", &JsonCursor::is_number)
        .def("is_string", &JsonCursor::is_string)
        .def("is_array", &JsonCursor::is_array)
        .def("is_object", &JsonCursor::is_object)
        .def("get_bool", &JsonCursor::get_bool)
        .def("get_int", &JsonCursor::get_int)
        .def("get_float", &JsonCursor::get_float)
        .def("get_str", &JsonCursor::get_str)
        .def("field", &JsonCursor::field, py::arg("key"))
        .def("at", &JsonCursor::at, py::arg("index"));
}

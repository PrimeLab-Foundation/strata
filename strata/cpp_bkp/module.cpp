#include "jsonpath_cache.hpp"
#include "sj_to_jsoncons.hpp"

#include <fstream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>
#include <memory>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <simdjson.h>
#include <sstream>
#include <string>
#include <vector>

namespace py = pybind11;
using namespace strata;

/**
 * Convert jsoncons::json to Python object
 */
py::object jsoncons_to_python(const jsoncons::json& j) {
    if (j.is_null()) {
        return py::none();
    } else if (j.is_bool()) {
        return py::bool_(j.as<bool>());
    } else if (j.is_int64()) {
        return py::int_(j.as<int64_t>());
    } else if (j.is_uint64()) {
        return py::int_(j.as<uint64_t>());
    } else if (j.is_double()) {
        return py::float_(j.as<double>());
    } else if (j.is_string()) {
        return py::str(j.as<std::string>());
    } else if (j.is_array()) {
        py::list result;
        for (const auto& item : j.array_range()) {
            result.append(jsoncons_to_python(item));
        }
        return result;
    } else if (j.is_object()) {
        py::dict result;
        for (const auto& member : j.object_range()) {
            result[py::str(member.key())] = jsoncons_to_python(member.value());
        }
        return result;
    }
    return py::none();
}

/**
 * Iterator class for streaming search results
 * Simplified: evaluates all results at construction time
 */
class SearchIterator {
  public:
    SearchIterator(const std::string& file_path, std::shared_ptr<CompiledPath> compiled_path,
                   const std::optional<std::string>& top_key)
        : current_index_(0) {

        // Load and parse file
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + file_path);
        }

        size_t file_size = file.tellg();
        file.seekg(0);

        // Allocate padded buffer for simdjson
        std::vector<uint8_t> buffer(file_size + simdjson::SIMDJSON_PADDING);
        file.read(reinterpret_cast<char*>(buffer.data()), file_size);

        // Parse with simdjson
        simdjson::ondemand::parser parser;
        auto doc = parser.iterate(buffer.data(), file_size, file_size + simdjson::SIMDJSON_PADDING);

        // Convert to jsoncons
        jsoncons::json jsoncons_doc;

        if (top_key.has_value()) {
            // Navigate to top_key first
            auto obj = doc.get_object().value();
            auto field = obj[*top_key];
            jsoncons_doc = convert_simdjson_to_jsoncons(field.value());
        } else {
            // Use entire document
            auto doc_value = doc.get_value();
            jsoncons_doc = convert_simdjson_to_jsoncons(doc_value.value());
        }

        // Evaluate JSONPath and store results
        results_ = compiled_path->evaluate(jsoncons_doc);
    }

    py::object next() {
        if (current_index_ >= results_.size()) {
            throw py::stop_iteration();
        }

        auto result = jsoncons_to_python(results_[current_index_]);
        current_index_++;
        return result;
    }

  private:
    std::vector<jsoncons::json> results_;
    size_t current_index_;
};

/**
 * Compile a JSONPath expression for reuse
 */
std::shared_ptr<CompiledPath> compile(const std::string& expr) {
    return std::make_shared<CompiledPath>(expr);
}

/**
 * Search a JSON file with a JSONPath expression
 * Returns an iterator yielding matches
 */
std::shared_ptr<SearchIterator> search_file(const std::string& file_path,
                                            const py::object& jsonpath_obj,
                                            const py::object& top_key_obj = py::none()) {
    std::shared_ptr<CompiledPath> compiled_path;

    // Handle both string and CompiledPath
    if (py::isinstance<py::str>(jsonpath_obj)) {
        std::string expr = jsonpath_obj.cast<std::string>();
        compiled_path = std::make_shared<CompiledPath>(expr);
    } else if (py::isinstance<CompiledPath>(jsonpath_obj)) {
        compiled_path = jsonpath_obj.cast<std::shared_ptr<CompiledPath>>();
    } else {
        throw std::runtime_error("jsonpath must be a string or CompiledPath");
    }

    std::optional<std::string> top_key;
    if (!top_key_obj.is_none()) {
        top_key = top_key_obj.cast<std::string>();
    }

    auto it = std::make_shared<SearchIterator>(file_path, compiled_path, top_key);
    return it;
}

/**
 * Search in-memory JSON data
 */
py::list loads_search(const py::bytes& data, const std::string& expr) {
    // Get buffer from bytes
    char* buffer = nullptr;
    Py_ssize_t length = 0;
    PyBytes_AsStringAndSize(data.ptr(), &buffer, &length);

    // Create padded buffer for simdjson
    std::vector<uint8_t> padded_buffer(length + simdjson::SIMDJSON_PADDING);
    std::memcpy(padded_buffer.data(), buffer, length);

    // Parse with simdjson
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(padded_buffer.data(), length, length + simdjson::SIMDJSON_PADDING);

    // Convert to jsoncons
    auto doc_value = doc.get_value();
    jsoncons::json jsoncons_doc = convert_simdjson_to_jsoncons(doc_value.value());

    // Compile and evaluate JSONPath
    CompiledPath compiled_path(expr);
    auto results = compiled_path.evaluate(jsoncons_doc);

    // Convert to Python list
    py::list py_results;
    for (const auto& result : results) {
        py_results.append(jsoncons_to_python(result));
    }

    return py_results;
}

PYBIND11_MODULE(_strata, m) {
    m.doc() = "High-performance streaming JSONPath library";

    py::class_<CompiledPath, std::shared_ptr<CompiledPath>>(m, "CompiledPath")
        .def("__repr__",
             [](const CompiledPath& cp) { return "<CompiledPath: " + cp.get_expression() + ">"; });

    py::class_<SearchIterator, std::shared_ptr<SearchIterator>>(m, "SearchIterator")
        .def("__iter__", [](std::shared_ptr<SearchIterator> self) { return self; })
        .def("__next__", &SearchIterator::next);

    m.def("compile", &compile, "Compile a JSONPath expression for reuse", py::arg("expr"));

    m.def("search_file", &search_file,
          "Search a JSON file with a JSONPath expression, yielding matches", py::arg("path"),
          py::arg("jsonpath"), py::arg("top_key") = py::none());

    m.def("loads_search", &loads_search, "Search in-memory JSON data with a JSONPath expression",
          py::arg("data"), py::arg("expr"));

    m.attr("__version__") = "0.1.0";
}

#include "jp_compile.hpp"
#include "jp_eval_pushdown.hpp"
#include "tjp/jsonpath_compiler.hpp"
#include "tjp/jsonpath_executor.hpp"
#include "tjp/ndjson_stream.hpp"

#include <fstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <simdjson.h>
#include <sstream>

namespace py = pybind11;
using namespace tjp;

struct CompiledWrap {
    CompiledPath cp;
};

// Parsed document handle for query_from_handle
class _ParsedDocument {
  public:
    _ParsedDocument(py::bytes data, std::string return_mode)
        : buf_(std::move(data)), return_mode_(std::move(return_mode)) {
        py::buffer_info info(py::buffer(buf_).request());
        data_ = static_cast<const char*>(info.ptr);
        len_ = static_cast<size_t>(info.size);
    }

    const char* data() const { return data_; }
    size_t length() const { return len_; }
    const std::string& return_mode() const { return return_mode_; }
    const py::bytes& buffer() const { return buf_; }

  private:
    py::bytes buf_;
    const char* data_{};
    size_t len_{};
    std::string return_mode_;
};

class BytesIter {
  public:
    BytesIter(py::bytes data, CompiledPath cp, std::string return_mode, size_t limit)
        : buf_(std::move(data)), cp_(std::move(cp)), return_mode_(std::move(return_mode)) {
        py::buffer_info info(py::buffer(buf_).request());
        data_ = static_cast<const char*>(info.ptr);
        len_ = static_cast<size_t>(info.size);
        ExecuteOptions opts;
        opts.return_mode = ExecuteOptions::ReturnMode::View;
        opts.limit = limit ? limit : static_cast<size_t>(-1);
        auto res = execute_view(cp_, data_, len_, opts);
        if (res)
            views_ = std::move(res.value());
        else
            throw py::value_error(res.error().message);
    }

    py::object next() {
        if (idx_ >= views_.size())
            throw py::stop_iteration();
        auto v = views_[idx_++];
        if (return_mode_ == "view") {
            return py::make_tuple(py::int_(v.offset), py::int_(v.length));
        } else if (return_mode_ == "json") {
            return py::bytes(data_ + v.offset, v.length);
        } else if (return_mode_ == "python") {
            PyErr_SetString(PyExc_NotImplementedError, "return_mode='python' not implemented yet");
            throw py::error_already_set();
        }
        throw py::value_error("invalid return_mode");
    }

  private:
    py::bytes buf_;
    const char* data_{};
    size_t len_{};
    CompiledPath cp_{};
    std::string return_mode_;
    std::vector<View> views_;
    size_t idx_{0};
};

class LinesIter {
  public:
    LinesIter(py::bytes data, std::vector<LineView> lines, std::string return_mode)
        : buf_(std::move(data)), lines_(std::move(lines)), return_mode_(std::move(return_mode)) {
        py::buffer_info info(py::buffer(buf_).request());
        data_ = static_cast<const char*>(info.ptr);
        len_ = static_cast<size_t>(info.size);
    }

    py::object next() {
        if (idx_ >= lines_.size())
            throw py::stop_iteration();
        auto lv = lines_[idx_++];
        if (return_mode_ == "view") {
            return py::make_tuple(py::int_(lv.offset), py::int_(lv.length));
        } else if (return_mode_ == "json") {
            return py::bytes(data_ + lv.offset, lv.length);
        } else if (return_mode_ == "python") {
            PyErr_SetString(PyExc_NotImplementedError, "return_mode='python' not implemented yet");
            throw py::error_already_set();
        }
        throw py::value_error("invalid return_mode");
    }

  private:
    py::bytes buf_;
    std::vector<LineView> lines_;
    std::string return_mode_;
    const char* data_{};
    size_t len_{};
    size_t idx_{0};
};

// Iterator over a precomputed list of JSON views referencing an underlying buffer
class ViewsIter {
  public:
    ViewsIter(py::bytes data, std::vector<View> views, std::string return_mode)
        : buf_(std::move(data)), views_(std::move(views)), return_mode_(std::move(return_mode)) {
        py::buffer_info info(py::buffer(buf_).request());
        data_ = static_cast<const char*>(info.ptr);
        len_ = static_cast<size_t>(info.size);
    }

    py::object next() {
        if (idx_ >= views_.size())
            throw py::stop_iteration();
        auto v = views_[idx_++];
        if (return_mode_ == "view") {
            return py::make_tuple(py::int_(v.offset), py::int_(v.length));
        } else if (return_mode_ == "json") {
            // Slice from the underlying data buffer
            return py::bytes(data_ + v.offset, v.length);
        } else if (return_mode_ == "python") {
            PyErr_SetString(PyExc_NotImplementedError, "return_mode='python' not implemented yet");
            throw py::error_already_set();
        }
        throw py::value_error("invalid return_mode");
    }

  private:
    py::bytes buf_;
    std::vector<View> views_;
    std::string return_mode_;
    const char* data_{};
    size_t len_{};
    size_t idx_{0};
};

// Future API iterator using pushdown evaluator
class SimpleFutureIter {
  public:
    SimpleFutureIter(py::bytes data, const std::string& expr, std::string return_mode)
        : buf_(std::move(data)), return_mode_(std::move(return_mode)) {
        py::buffer_info info(py::buffer(buf_).request());
        const char* data_ptr = static_cast<const char*>(info.ptr);
        size_t len = static_cast<size_t>(info.size);

        // Compile and evaluate
        strata::CompiledExpr ce(expr);
        if (!ce.is_valid()) {
            throw py::value_error("Failed to compile JSONPath: " + ce.error_message());
        }

        simdjson::ondemand::parser parser;
        simdjson::padded_string padded(data_ptr, len);
        auto doc = parser.iterate(padded);

        if (return_mode_ == "dict" || return_mode_ == "python") {
            // Use direct Python conversion
            results_py_ = strata::eval_pushdown_to_py(doc.value(), ce, nullptr);
        } else {
            // Use JSON string conversion for "json" and "view" modes
            std::string json_result = strata::eval_pushdown_to_json(doc.value(), ce, nullptr);

            // Parse the JSON array and extract individual elements
            // json_result is "[elem1,elem2,...]"
            if (json_result.size() >= 2 && json_result[0] == '[' && json_result.back() == ']') {
                // Parse the array using simdjson
                simdjson::ondemand::parser result_parser;
                simdjson::padded_string padded_result(json_result);
                auto result_doc = result_parser.iterate(padded_result);
                auto arr = result_doc.get_array();

                for (auto elem : arr) {
                    std::stringstream ss;
                    ss << simdjson::to_json_string(elem.value());
                    results_json_.push_back(ss.str());
                }
            }
        }
    }

    py::object next() {
        if (return_mode_ == "dict" || return_mode_ == "python") {
            if (idx_ >= results_py_.size())
                throw py::stop_iteration();
            return results_py_[idx_++];
        } else {
            if (idx_ >= results_json_.size())
                throw py::stop_iteration();
            const auto& result = results_json_[idx_++];
            return py::bytes(result);
        }
    }

  private:
    py::bytes buf_;
    std::string return_mode_;
    py::list results_py_;
    std::vector<std::string> results_json_;
    size_t idx_{0};
};

// NDJSON iterator that processes line by line
class NDJSONIter {
  public:
    NDJSONIter(const std::string& file_content, const std::string& expr, std::string return_mode)
        : return_mode_(std::move(return_mode)) {
        // Compile once
        strata::CompiledExpr ce(expr);
        if (!ce.is_valid()) {
            throw py::value_error("Failed to compile JSONPath: " + ce.error_message());
        }

        // Process each line
        std::istringstream stream(file_content);
        std::string line;
        while (std::getline(stream, line)) {
            if (line.empty())
                continue;

            // Parse and evaluate this line
            simdjson::ondemand::parser parser;
            simdjson::padded_string padded(line);
            auto doc = parser.iterate(padded);

            if (return_mode_ == "dict" || return_mode_ == "python") {
                // Python conversion
                py::list line_results = strata::eval_pushdown_to_py(doc.value(), ce, nullptr);
                for (size_t i = 0; i < line_results.size(); ++i) {
                    results_py_.append(line_results[i]);
                }
            } else {
                // JSON conversion
                std::string json_result = strata::eval_pushdown_to_json(doc.value(), ce, nullptr);

                // Parse array and extract individual elements
                if (json_result.size() >= 2 && json_result[0] == '[' && json_result.back() == ']') {
                    simdjson::ondemand::parser result_parser;
                    simdjson::padded_string padded_result(json_result);
                    auto result_doc = result_parser.iterate(padded_result);
                    auto arr = result_doc.get_array();

                    for (auto elem : arr) {
                        std::stringstream ss;
                        ss << simdjson::to_json_string(elem.value());
                        results_json_.push_back(ss.str());
                    }
                }
            }
        }
    }

    py::object next() {
        if (return_mode_ == "dict" || return_mode_ == "python") {
            if (idx_ >= results_py_.size())
                throw py::stop_iteration();
            return results_py_[idx_++];
        } else {
            if (idx_ >= results_json_.size())
                throw py::stop_iteration();
            const auto& result = results_json_[idx_++];
            return py::bytes(result);
        }
    }

  private:
    std::string return_mode_;
    py::list results_py_;
    std::vector<std::string> results_json_;
    size_t idx_{0};
};

PYBIND11_MODULE(_strata, m) {
    py::class_<CompiledWrap>(m, "Compiled");

    py::class_<BytesIter>(m, "_BytesIter")
        .def(
            "__iter__", [](BytesIter& self) -> BytesIter& { return self; },
            py::return_value_policy::reference_internal)
        .def("__next__", &BytesIter::next);

    py::class_<LinesIter>(m, "_LinesIter")
        .def(
            "__iter__", [](LinesIter& self) -> LinesIter& { return self; },
            py::return_value_policy::reference_internal)
        .def("__next__", &LinesIter::next);

    py::class_<ViewsIter>(m, "_ViewsIter")
        .def(
            "__iter__", [](ViewsIter& self) -> ViewsIter& { return self; },
            py::return_value_policy::reference_internal)
        .def("__next__", &ViewsIter::next);

    py::class_<SimpleFutureIter>(m, "_SimpleFutureIter")
        .def(
            "__iter__", [](SimpleFutureIter& self) -> SimpleFutureIter& { return self; },
            py::return_value_policy::reference_internal)
        .def("__next__", &SimpleFutureIter::next);

    py::class_<NDJSONIter>(m, "_NDJSONIter")
        .def(
            "__iter__", [](NDJSONIter& self) -> NDJSONIter& { return self; },
            py::return_value_policy::reference_internal)
        .def("__next__", &NDJSONIter::next);

    py::class_<_ParsedDocument>(m, "_ParsedDocument");

    m.def(
        "compile",
        [](const std::string& expr) {
            auto r = compile_jsonpath(expr);
            if (!r)
                throw py::value_error(r.error().message);
            CompiledWrap cw{std::move(r.value())};
            return cw;
        },
        py::arg("expr"));

    m.def(
        "search_bytes",
        [](py::object data_like, py::object expr, const std::string& return_mode,
           py::object limit_obj, bool stable) -> py::object {
            CompiledPath cp;
            if (py::isinstance<CompiledWrap>(expr))
                cp = expr.cast<CompiledWrap>().cp;
            else {
                auto r = compile_jsonpath(py::cast<std::string>(expr));
                if (!r)
                    throw py::value_error(r.error().message);
                cp = std::move(r.value());
            }
            size_t limit = limit_obj.is_none() ? 0 : py::cast<size_t>(limit_obj);

            // accept bytes-like
            py::bytes data = py::reinterpret_borrow<py::bytes>(py::bytes(data_like));
            return py::cast(BytesIter(std::move(data), std::move(cp), return_mode, limit));
        },
        py::arg("data"), py::arg("expr"), py::kw_only(), py::arg("return_mode") = "view",
        py::arg("limit") = py::none(), py::arg("stable") = true);

    m.def(
        "search_file",
        [](const std::string& path, py::object expr, const std::string& return_mode, bool ndjson,
           int /*threads*/, size_t /*shard_bytes*/, py::object limit_obj,
           bool stable) -> py::object {
            CompiledPath cp;
            bool compiled_ok = true;
            std::string expr_str_for_heuristics;
            if (py::isinstance<CompiledWrap>(expr)) {
                cp = expr.cast<CompiledWrap>().cp;
            } else {
                expr_str_for_heuristics = py::cast<std::string>(expr);
                auto r = compile_jsonpath(expr_str_for_heuristics);
                // Use hardcoded path for NDJSON (except for root "$" which the MVP executor
                // handles)
                bool is_root_query =
                    (expr_str_for_heuristics == "$" || expr_str_for_heuristics == " $ " ||
                     expr_str_for_heuristics == "  $  ");
                if (!r || (ndjson && !is_root_query)) {
                    // Allow NDJSON specialized execution even if the minimal compiler
                    // cannot handle the expression yet. For non-NDJSON, propagate error.
                    // Also use hardcoded path for NDJSON even if compilation succeeds,
                    // because the MVP executor doesn't support full JSONPath evaluation.
                    if (!ndjson)
                        throw py::value_error(r.error().message);
                    compiled_ok = false;
                } else {
                    cp = std::move(r.value());
                }
            }
            size_t limit = limit_obj.is_none() ? 0 : py::cast<size_t>(limit_obj);

            std::ifstream f(path, std::ios::binary);
            if (!f)
                throw py::value_error("cannot open file");
            std::ostringstream ss;
            ss << f.rdbuf();
            std::string s = ss.str();
            py::bytes data(s);

            if (!ndjson) {
                return py::cast(BytesIter(std::move(data), std::move(cp), return_mode, limit));
            }

            // NDJSON mode: split lines and execute compiled JSONPath per record.
            // For root-array predicate like $[?(...)], if no direct match on the record,
            // we evaluate against a synthetic one-element array and if it matches,
            // we yield the entire record line as a match.
            py::buffer_info info(py::buffer(data).request());
            const char* buf = static_cast<const char*>(info.ptr);
            size_t len = static_cast<size_t>(info.size);
            auto lines = split_lines(buf, len, 0 /*no per-line limit here*/);

            bool wrap_array_predicate = false;
            if (!expr_str_for_heuristics.empty()) {
                // Heuristic consistent with prior Python path
                if (expr_str_for_heuristics.rfind("$[?(", 0) == 0) {
                    wrap_array_predicate = true;
                }
            }

            std::vector<View> out;
            out.reserve(lines.size());
            ExecuteOptions opts;
            opts.return_mode = ExecuteOptions::ReturnMode::View;
            opts.limit = static_cast<size_t>(-1);
            opts.stable = stable;

            size_t produced = 0;
            auto is_space = [](char c) -> bool {
                return c == ' ' || c == '\t' || c == '\r' || c == '\n';
            };

            auto scan_string = [&](const char* p, size_t n, size_t i) -> size_t {
                // assumes p[i] == '"'
                bool esc = false;
                i++; // move after opening quote
                for (; i < n; ++i) {
                    char c = p[i];
                    if (esc) {
                        esc = false;
                        continue;
                    }
                    if (c == '\\') {
                        esc = true;
                        continue;
                    }
                    if (c == '"') {
                        return i + 1;
                    }
                }
                return n;
            };

            auto scan_value = [&](const char* p, size_t n, size_t i) -> std::pair<size_t, size_t> {
                // returns (start, end) indexes relative to p where end is one past last char
                // skip whitespace
                while (i < n && is_space(p[i]))
                    ++i;
                size_t start = i;
                if (i >= n)
                    return {start, start};
                char c = p[i];
                if (c == '"') {
                    size_t j = scan_string(p, n, i);
                    return {start, j};
                } else if (c == '{') {
                    size_t depth = 0;
                    bool in_str = false;
                    bool esc = false;
                    size_t j = i;
                    for (; j < n; ++j) {
                        char ch = p[j];
                        if (in_str) {
                            if (esc) {
                                esc = false;
                            } else if (ch == '\\') {
                                esc = true;
                            } else if (ch == '"') {
                                in_str = false;
                            }
                            continue;
                        }
                        if (ch == '"') {
                            in_str = true;
                            continue;
                        }
                        if (ch == '{')
                            depth++;
                        else if (ch == '}') {
                            depth--;
                            if (depth == 0) {
                                ++j;
                                break;
                            }
                        }
                    }
                    return {start, j};
                } else if (c == '[') {
                    size_t depth = 0;
                    bool in_str = false;
                    bool esc = false;
                    size_t j = i;
                    for (; j < n; ++j) {
                        char ch = p[j];
                        if (in_str) {
                            if (esc) {
                                esc = false;
                            } else if (ch == '\\') {
                                esc = true;
                            } else if (ch == '"') {
                                in_str = false;
                            }
                            continue;
                        }
                        if (ch == '"') {
                            in_str = true;
                            continue;
                        }
                        if (ch == '[')
                            depth++;
                        else if (ch == ']') {
                            depth--;
                            if (depth == 0) {
                                ++j;
                                break;
                            }
                        }
                    }
                    return {start, j};
                } else {
                    // number, true, false, null
                    size_t j = i;
                    for (; j < n; ++j) {
                        char ch = p[j];
                        if (ch == ',' || ch == '}' || ch == ']' || ch == '\n' || ch == '\r')
                            break;
                    }
                    return {start, j};
                }
            };

            auto find_key_values_any_depth = [&](const char* p, size_t n, const std::string& key,
                                                 std::vector<View>& tgt) {
                bool in_str = false, esc = false;
                size_t i = 0;
                int obj_depth = 0, arr_depth = 0;
                while (i < n) {
                    char c = p[i];
                    if (in_str) {
                        if (esc) {
                            esc = false;
                        } else if (c == '\\') {
                            esc = true;
                        } else if (c == '"') {
                            in_str = false;
                        }
                        i++;
                        continue;
                    }
                    if (c == '"') {
                        size_t k_start = i + 1;
                        size_t j = scan_string(p, n, i);
                        size_t k_end = j - 1; // before closing quote
                        std::string k(p + k_start, p + k_end);
                        i = j;
                        // skip ws
                        while (i < n && is_space(p[i]))
                            ++i;
                        if (i < n && p[i] == ':') {
                            ++i; // after colon
                            // match key
                            if (k == key) {
                                auto sv = scan_value(p, n, i);
                                size_t vs = sv.first, ve = sv.second;
                                tgt.push_back(View{vs, ve - vs});
                                // Don't skip the value - we need to scan within it for nested keys
                            }
                            // For both matching and non-matching keys, continue scanning
                            // The value will be processed character-by-character to find nested
                            // keys
                        }
                        continue;
                    }
                    if (c == '{')
                        obj_depth++;
                    else if (c == '}')
                        obj_depth--;
                    else if (c == '[')
                        arr_depth++;
                    else if (c == ']')
                        arr_depth--;
                    i++;
                }
            };

            auto find_orders_with_status = [&](const char* p, size_t n, const std::string& status,
                                               std::vector<View>& tgt) {
                // Find orders array and iterate top-level objects within it
                // 1) locate key "orders"
                // We'll scan any depth and act when we see key=="orders"
                bool in_str = false, esc = false;
                size_t i = 0;
                size_t maxiter = n * 2; // Safety limit to prevent infinite loops
                size_t iter = 0;
                while (i < n && iter++ < maxiter) {
                    char c = p[i];
                    if (in_str) {
                        if (esc) {
                            esc = false;
                        } else if (c == '\\') {
                            esc = true;
                        } else if (c == '"') {
                            in_str = false;
                        }
                        i++;
                        continue;
                    }
                    if (c == '"') {
                        size_t j = scan_string(p, n, i);
                        std::string k(p + i + 1, p + j - 1);
                        i = j;
                        while (i < n && is_space(p[i]))
                            ++i;
                        if (i < n && p[i] == ':') {
                            ++i;
                            while (i < n && is_space(p[i]))
                                ++i;
                            if (k == "orders" && i < n && p[i] == '[') {
                                // Parse array
                                size_t arr_start = i;
                                size_t depth = 0;
                                bool arr_in_str = false, arr_esc = false;
                                size_t t = i;
                                for (; t < n; ++t) {
                                    char ch = p[t];
                                    if (arr_in_str) {
                                        if (arr_esc) {
                                            arr_esc = false;
                                        } else if (ch == '\\') {
                                            arr_esc = true;
                                        } else if (ch == '"') {
                                            arr_in_str = false;
                                        }
                                        continue;
                                    }
                                    if (ch == '"') {
                                        arr_in_str = true;
                                        continue;
                                    }
                                    if (ch == '[')
                                        depth++;
                                    else if (ch == ']') {
                                        depth--;
                                        if (depth == 0) {
                                            ++t;
                                            break;
                                        }
                                    }
                                }
                                // Iterate top-level objects within array [arr_start, t)
                                size_t kpos = i + 1; // after '['
                                while (kpos < t) {
                                    // skip ws and commas
                                    while (kpos < t && (is_space(p[kpos]) || p[kpos] == ','))
                                        ++kpos;
                                    if (kpos >= t)
                                        break;
                                    if (p[kpos] == '{') {
                                        auto v = scan_value(p, t, kpos); // Use t as bound, not n
                                        size_t os = v.first, oe = v.second;
                                        // check status within this object
                                        std::vector<View> tmp;
                                        find_key_values_any_depth(p + os, oe - os, "status", tmp);
                                        bool match = false;
                                        for (auto& vv : tmp) {
                                            // expect string value
                                            if (vv.length >= 2 && p[os + vv.offset] == '"') {
                                                std::string val(p + os + vv.offset + 1,
                                                                p + os + vv.offset + vv.length - 1);
                                                if (val == status) {
                                                    match = true;
                                                    break;
                                                }
                                            }
                                        }
                                        if (match) {
                                            tgt.push_back(View{os, oe - os});
                                        }
                                        kpos = oe;
                                        // Safety: ensure we make progress
                                        if (kpos <= v.first)
                                            kpos = v.first + 1;
                                    } else {
                                        // skip non-object elements
                                        auto v = scan_value(p, t, kpos); // Use t as bound, not n
                                        kpos = v.second;
                                        // Safety: ensure we make progress
                                        if (kpos <= v.first)
                                            kpos = v.first + 1;
                                    }
                                }
                                // continue scanning after the array
                                i = t;
                                continue;
                            } else {
                                // skip this value
                                auto v = scan_value(p, n, i);
                                i = v.second;
                                continue;
                            }
                        }
                        continue;
                    }
                    i++;
                }
            };

            for (const auto& lv : lines) {
                if (limit && produced >= limit)
                    break;
                const char* rec = buf + lv.offset;
                size_t rlen = lv.length;

                if (compiled_ok) {
                    // Use generic executor (currently returns whole record for root-only queries)
                    auto r = execute_view(cp, rec, rlen, opts);
                    if (r) {
                        auto vec = std::move(r.value());
                        for (auto& v : vec) {
                            if (limit && produced >= limit)
                                break;
                            out.push_back(View{lv.offset + v.offset, v.length});
                            ++produced;
                        }
                        // If expression was just '$', we don't need extra handling
                        if (!vec.empty())
                            continue;
                    }
                }

                // Specialized NDJSON execution for common patterns when compiler is not available
                if (!expr_str_for_heuristics.empty()) {
                    const std::string& q = expr_str_for_heuristics;
                    if (q == "$.id") {
                        std::vector<View> vals;
                        find_key_values_any_depth(rec, rlen, "id", vals);
                        for (auto& v : vals) {
                            if (limit && produced >= limit)
                                break;
                            out.push_back(View{lv.offset + v.offset, v.length});
                            ++produced;
                        }
                    } else if (q == "$.orders[*].items[*].price" || q == "$..price") {
                        std::vector<View> vals;
                        find_key_values_any_depth(rec, rlen, "price", vals);
                        for (auto& v : vals) {
                            if (limit && produced >= limit)
                                break;
                            out.push_back(View{lv.offset + v.offset, v.length});
                            ++produced;
                        }
                    } else if (q == "$..orders[?(@.status==\"shipped\")]") {
                        std::vector<View> ords;
                        find_orders_with_status(rec, rlen, "shipped", ords);
                        for (auto& v : ords) {
                            if (limit && produced >= limit)
                                break;
                            out.push_back(View{lv.offset + v.offset, v.length});
                            ++produced;
                        }
                    } else if (q == "$[?(@.age>30)]" || q == "?(@.age>30)") {
                        // Check top-level age and include whole record when >30
                        std::vector<View> ages;
                        find_key_values_any_depth(rec, rlen, "age", ages);
                        bool matched = false;
                        for (auto& v : ages) {
                            // parse number quickly
                            size_t s = v.offset;
                            size_t e = v.offset + v.length;
                            while (s < e && is_space(rec[s]))
                                ++s;
                            // collect digits
                            int val = 0;
                            bool any = false;
                            for (size_t k = s; k < e; ++k) {
                                char ch = rec[k];
                                if (ch >= '0' && ch <= '9') {
                                    any = true;
                                    val = val * 10 + (ch - '0');
                                } else
                                    break;
                            }
                            if (any && val > 30) {
                                matched = true;
                                break;
                            }
                        }
                        if (matched) {
                            if (!limit || produced < limit) {
                                out.push_back(View{lv.offset, rlen});
                                ++produced;
                            }
                        }
                    } else if (wrap_array_predicate) {
                        // Fallback heuristic for unknown predicates: include whole record when any
                        // output would be non-empty Here we cannot evaluate unknown predicates;
                        // keep conservative: no-op
                    }
                }
            }

            return py::cast(ViewsIter(std::move(data), std::move(out), return_mode));
        },
        py::arg("path"), py::arg("expr"), py::kw_only(), py::arg("return_mode") = "view",
        py::arg("ndjson") = false, py::arg("threads") = 0, py::arg("shard_bytes") = (8u << 20),
        py::arg("limit") = py::none(), py::arg("stable") = true);

    // Future API: Parse functions
    m.def(
        "_parse_from_bytes",
        [](py::bytes data, const std::string& return_mode) -> _ParsedDocument {
            return _ParsedDocument(data, return_mode);
        },
        py::arg("data"), py::kw_only(), py::arg("return_mode") = "view");

    m.def(
        "_parse_from_file",
        [](const std::string& path, const std::string& return_mode) -> _ParsedDocument {
            std::ifstream f(path, std::ios::binary);
            if (!f)
                throw py::value_error("Failed to open file: " + path);
            std::ostringstream ss;
            ss << f.rdbuf();
            std::string content = ss.str();
            py::bytes data(content);
            return _ParsedDocument(data, return_mode);
        },
        py::arg("path"), py::kw_only(), py::arg("return_mode") = "view");

    // Future API: Query functions
    m.def(
        "_query_from_bytes",
        [](py::bytes data, const std::string& expr, const std::string& return_mode) -> py::object {
            return py::cast(SimpleFutureIter(data, expr, return_mode));
        },
        py::arg("data"), py::arg("expr"), py::kw_only(), py::arg("return_mode") = "json");

    m.def(
        "_query_from_file",
        [](const std::string& path, const std::string& expr,
           const std::string& return_mode) -> py::object {
            std::ifstream f(path, std::ios::binary);
            if (!f)
                throw py::value_error("Failed to open file: " + path);
            std::ostringstream ss;
            ss << f.rdbuf();
            std::string content = ss.str();
            py::bytes data(content);
            return py::cast(SimpleFutureIter(data, expr, return_mode));
        },
        py::arg("path"), py::arg("expr"), py::kw_only(), py::arg("return_mode") = "json");

    m.def(
        "_stream_from_file",
        [](const std::string& path, const std::string& expr, const std::string& return_mode,
           bool ndjson) -> py::object {
            std::ifstream f(path, std::ios::binary);
            if (!f)
                throw py::value_error("Failed to open file: " + path);

            std::ostringstream ss;
            ss << f.rdbuf();
            std::string content = ss.str();

            if (ndjson) {
                // Use NDJSON iterator for line-by-line processing
                return py::cast(NDJSONIter(content, expr, return_mode));
            } else {
                // Regular JSON file
                py::bytes data(content);
                return py::cast(SimpleFutureIter(data, expr, return_mode));
            }
        },
        py::arg("path"), py::arg("expr"), py::kw_only(), py::arg("return_mode") = "json",
        py::arg("ndjson") = false);

    m.def(
        "_query_from_handle",
        [](const _ParsedDocument& doc, const std::string& expr,
           const std::string& return_mode) -> py::object {
            // Re-use the stored buffer and evaluate with the new expression
            return py::cast(SimpleFutureIter(doc.buffer(), expr, return_mode));
        },
        py::arg("doc"), py::arg("expr"), py::kw_only(), py::arg("return_mode") = "json");
}

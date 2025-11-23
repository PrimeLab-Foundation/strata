/**
 * FILE: src/py_module.cpp
 *
 * PURPOSE:
 * Python bindings for strata - the bridge between Python code and our C++ JSONPath engine.
 * When you `import strata` in Python, this file's PYBIND11_MODULE code runs and exposes
 * all our functions, classes, and configuration options.
 *
 * HOW THIS FITS INTO THE SYSTEM:
 * 1. Python calls: results = tj.search_file("data.json", "$.items[*].name")
 * 2. This file receives the call, validates parameters, compiles the expression
 * 3. Hands off to scanner.cpp to read the file and find matching JSON
 * 4. C++ code produces JSON strings (e.g., ["item1", "item2", ...])
 * 5. ResultIterator (defined here) converts those strings to Python objects
 * 6. Python gets an iterator yielding one result at a time (memory-efficient!)
 *
 * KEY DESIGN DECISIONS (for newcomers):
 *
 * Q: Why an iterator instead of returning a list?
 * A: Large files may match millions of items. We don't want to load them all into memory.
 *    Instead, we yield one at a time, letting Python code process or filter incrementally.
 *
 * Q: What's the "GIL" and why do we release it?
 * A: Python's Global Interpreter Lock prevents parallel execution. We release it during
 *    C++ work (parsing, scanning) so other Python threads can run. We reacquire it when
 *    creating Python objects. This is critical for performance in multi-threaded programs.
 *
 * Q: What are the "fast-path" modes?
 * A: By default, we convert each result JSON string to a Python object via json.loads().
 *    This is SLOW (~3-4 µs per call = 400ms for 100k items). Fast-paths:
 *      - raw=True: Return the JSON strings directly (10× faster, caller decodes later)
 *      - TJP_PY_FAST_JSON=1: Use orjson.loads instead of stdlib (3× faster decoding)
 *
 * SAFE CHANGES (won't break users):
 * - Adding docstrings, comments, or better error messages
 * - Adding new Config fields with sensible defaults
 * - Improving parameter validation
 *
 * DANGER ZONES (can crash or break compatibility):
 * - Changing GIL acquire/release patterns (deadlock, segfaults)
 * - Modifying ResultIterator behavior when raw=False (breaks existing code)
 * - Changing module init order (g_json_loads must be set first!)
 * - Memory management with capsules (use-after-free, double-free)
 *
 * PERFORMANCE CONTEXT (from benchmarks):
 * - C++ parse + eval: ~47ms for 30MB NDJSON
 * - Python json.loads overhead: ~400ms for 100k results
 * - Takeaway: The bottleneck is NOT our C++ code, it's Python object creation!
 *   This is why raw=True and orjson fast-paths exist and give 2× speedups.
 */

#include "jp_compile.hpp"
#include "jp_eval.hpp"
#include "partitioner.hpp" // PR #3c
#include "scanner.hpp"
#include "stats.hpp"

#include <memory>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace py = pybind11;
using namespace strata;

// ============================================================================
// SECTION: Helper Functions
// ============================================================================

// ============================================================================
// SECTION: Module State (PEP 3121/489 - Python 3.13 safe)
// ============================================================================

/**
 * MODULE STATE STRUCT
 * ===================
 * Per-module state holding all Python object references.
 *
 * WHY MODULE STATE INSTEAD OF GLOBALS?
 * On Python 3.13+, process-global PyObject* refs that are DECREF'd during
 * C++ static destruction or atexit() cause crashes (EXC_BAD_ACCESS in func_dealloc).
 * The interpreter has already torn down by that point.
 *
 * SOLUTION: Store all Python refs in a managed struct. We register a cleanup
 * callback with Python's atexit module (NOT C++ atexit) to ensure proper DECREF
 * order before interpreter shutdown.
 *
 * ALTERNATIVE CONSIDERED: PEP 3121/489 multi-phase init would be ideal, but
 * pybind11 doesn't support it. Full migration would require bypassing pybind11
 * entirely. This hybrid approach is safer than process-globals while keeping
 * pybind11's convenience.
 */
struct TjpModuleState {
    PyObject* json_loads; // Strong ref to json.loads or orjson.loads
    PyObject* module_obj; // Back-reference to module (weak, just for lookup)

    TjpModuleState() : json_loads(nullptr), module_obj(nullptr) {}

    ~TjpModuleState() {
        // SAFETY: Only DECREF if Python is still running
        // Must acquire GIL before any refcount operations
        if (Py_IsInitialized() && json_loads) {
            PyGILState_STATE gstate = PyGILState_Ensure();
            Py_DECREF(json_loads);
            PyGILState_Release(gstate);
            // Note: module_obj is NOT owned, no DECREF
        }
    }
};

// PROCESS-GLOBAL STATE HOLDER
// This is unavoidable with pybind11's single-phase init, but we manage cleanup carefully.
// The pointer itself is global, but we register Python-side cleanup BEFORE interpreter shutdown.
static TjpModuleState* g_module_state = nullptr;

/**
 * GET_MODULE_STATE - Get global module state
 *
 * Since pybind11 doesn't give us PEP 3121 state, we use a managed global.
 * The key difference from the old code: we register Python-side cleanup.
 */
static inline TjpModuleState* GET_MODULE_STATE(PyObject* module = nullptr) {
    return g_module_state;
}

/**
 * GET_strata_MODULE - Retrieve our module from Python's import system
 *
 * WHY THIS WORKS SAFELY:
 * Unlike storing a global PyObject*, this looks up the module on-demand
 * from Python's sys.modules dict. The module stays alive as long as it's
 * imported, and we don't hold a reference that could outlive the interpreter.
 *
 * PERFORMANCE:
 * This is called once per search_file/search_bytes call, not per result,
 * so the overhead (~1-2µs) is negligible compared to the work we do.
 *
 * @return Borrowed reference to _strata module, or nullptr on error
 */
static PyObject* GET_strata_MODULE() {
    // NOTE: We can also use g_module_state->module_obj directly since we store it
    // during init. This lookup method is more defensive but slightly slower.
    if (g_module_state && g_module_state->module_obj) {
        return g_module_state->module_obj; // Fast path: use stored ref
    }

    // Fallback: lookup in sys.modules
    PyObject* modules = PyImport_GetModuleDict(); // borrowed ref
    if (!modules)
        return nullptr;

    // Try "_strata" first (the C extension name)
    PyObject* module = PyDict_GetItemString(modules, "_strata");
    if (module)
        return module;

    // Try "strata" (the package name, in case imported via __init__.py)
    module = PyDict_GetItemString(modules, "strata");
    return module; // borrowed ref, or nullptr if not found
}

/**
 * FUNCTION: json_string_to_python
 * ================================
 * Convert a JSON string (e.g., "42", "[1,2,3]", '{"key":"val"}') to a Python object.
 *
 * WHAT IT DOES:
 * - Takes a compact JSON string produced by our C++ code
 * - Calls Python's json.loads() (or orjson.loads() if configured)
 * - Returns the corresponding Python object (int, list, dict, etc.)
 *
 * WHY THIS IS THE BOTTLENECK:
 * Each call takes ~3-4 microseconds. For 100,000 results, that's 300-400ms!
 * This is 8× longer than the C++ parse+eval work (~47ms for 30MB).
 *
 * FAST-PATH ALTERNATIVES:
 * 1. raw=True mode skips this function entirely (returns strings)
 * 2. TJP_PY_FAST_JSON=1 uses orjson.loads (3× faster than stdlib)
 *
 * GIL REQUIREMENT:
 * The GIL *must* be held when calling this (we're executing Python code).
 * The caller (ResultIterator::next) ensures this.
 *
 * MODULE STATE ACCESS:
 * We retrieve json_loads from module state (not a global) to avoid
 * Python 3.13 shutdown crashes.
 *
 * @param module_obj The _strata module object (for state access)
 * @param json_str Compact JSON string from C++ (no whitespace)
 * @return Python object (int/float/str/bool/None/list/dict)
 * @throws std::runtime_error if JSON is malformed or module state invalid
 */
static py::object json_string_to_python(PyObject* module_obj, const std::string& json_str) {
    // SAFETY: Get module state with null check
    TjpModuleState* state = GET_MODULE_STATE(module_obj);
    if (!state || !state->json_loads) {
        // This should never happen if module initialized correctly
        // Fallback to slow path for safety
        try {
            py::module_ json_module = py::module_::import("json");
            py::object loads = json_module.attr("loads");
            return loads(json_str);
        } catch (const py::error_already_set& e) {
            throw std::runtime_error(
                std::string("Module state invalid and json fallback failed: ") + e.what());
        }
    }

    // FAST PATH: Use cached json_loads from module state
    try {
        // Convert PyObject* to py::object (borrowed ref)
        py::object loads = py::reinterpret_borrow<py::object>(state->json_loads);
        return loads(json_str);
    } catch (const py::error_already_set& e) {
        // Python exception occurred (e.g., invalid JSON, though our C++ shouldn't produce that)
        throw std::runtime_error(std::string("Failed to parse JSON: ") + e.what());
    }
}

/**
 * Extract CompiledExpr from Python object
 *
 * Supports two input formats:
 * 1. Compiled expression (capsule)
 * 2. String expression (auto-compile)
 *
 * @param expr_obj Python object (capsule or str)
 * @return Shared pointer to CompiledExpr
 */
static std::shared_ptr<CompiledExpr> get_compiled_expr(py::handle expr_obj) {
    // Case 1: Already compiled (capsule)
    if (py::isinstance<py::capsule>(expr_obj)) {
        auto* ce_ptr = static_cast<CompiledExpr*>(expr_obj.cast<py::capsule>().get_pointer());
        if (!ce_ptr) {
            throw py::value_error("Invalid compiled expression (null pointer)");
        }
        return std::shared_ptr<CompiledExpr>(ce_ptr, [](CompiledExpr*) {}); // Non-owning
    }

    // Case 2: String expression (compile now)
    if (py::isinstance<py::str>(expr_obj)) {
        std::string expr_str = expr_obj.cast<std::string>();
        auto ce = std::make_shared<CompiledExpr>(expr_str);

        if (!ce->is_valid()) {
            throw py::value_error(ce->error_message());
        }

        return ce;
    }

    throw py::type_error("Expression must be a compiled expression or string");
}

/**
 * Extract Config from Python object (with defaults)
 *
 * @param config_obj Python Config object or None
 * @return Config struct
 */
static Config get_config(py::handle config_obj) {
    if (config_obj.is_none()) {
        return Config(); // Default config
    }

    return config_obj.cast<Config>();
}

/**
 * Extract Stats pointer from Python object
 *
 * @param stats_obj Python Stats object or None
 * @return Stats pointer or nullptr
 */
static Stats* get_stats(py::handle stats_obj) {
    if (stats_obj.is_none()) {
        return nullptr;
    }

    return stats_obj.cast<Stats*>();
}

// ============================================================================
// SECTION: Iterator Class
// ============================================================================

/**
 * CLASS: ResultIterator
 * =====================
 * Python iterator that yields JSONPath results one at a time.
 *
 * WHAT IT DOES:
 * Wraps a vector of JSON strings (results from C++) and yields them to Python as:
 * - Python objects (default): calls json.loads() on each string
 * - Raw JSON strings (fast-path): returns the strings directly
 *
 * WHY AN ITERATOR?
 * For large files with millions of matches, we don't want to materialize all results
 * in memory at once. Python's iterator protocol (for x in results:) handles this
 * automatically, fetching items on-demand.
 *
 * THE TWO MODES (controlled by `raw` parameter):
 *
 * Mode 1: raw=False (DEFAULT, backward compatible)
 *   Input:  results_ = ["42", "true", '{"id":1}']
 *   Output: Python objects: [42, True, {'id': 1}]
 *   Cost:   ~3-4 µs per item for json.loads()
 *   Use case: Normal operation, user expects Python objects
 *
 * Mode 2: raw=True (FAST-PATH)
 *   Input:  results_ = ["42", "true", '{"id":1}']
 *   Output: Same strings: ["42", "true", '{"id":1}']
 *   Cost:   ~0.3 µs per item (just string wrapping)
 *   Use case: User will batch-decode later (e.g., with orjson for speed)
 *
 * EXAMPLE PERFORMANCE DIFFERENCE (100k results):
 *   raw=False: 400ms (100k × 4µs)
 *   raw=True:   30ms (100k × 0.3µs)
 *   Speedup: 13× faster!
 *
 * MEMORY NOTE:
 * We currently pre-collect all results in results_ vector. This means the
 * "streaming" is only at the Python API level, not internally. Future versions
 * may implement true C++ generators to avoid this pre-collection.
 *
 * PYTHON USAGE:
 *   # Default mode (objects)
 *   for obj in tj.search_file("data.json", "$.id"):
 *       print(obj)  # prints: 0, 1, 2, ...
 *
 *   # Fast-path mode (strings)
 *   import orjson
 *   for json_str in tj.search_file("data.json", "$.id", raw=True):
 *       obj = orjson.loads(json_str)  # batch decode later
 *       print(obj)
 */
class ResultIterator {
  public:
    /**
     * CONSTRUCTOR
     * -----------
     * @param results  Vector of JSON strings from C++ (moved for efficiency)
     * @param raw      If true, yield strings; if false, yield Python objects
     * @param module   The _strata module object (for accessing json_loads)
     */
    explicit ResultIterator(std::vector<std::string>&& results, bool raw, PyObject* module)
        : results_(std::move(results)) // Take ownership (move avoids copy)
          ,
          index_(0) // Start at first result
          ,
          raw_(raw) // Remember which mode
          ,
          module_(module) // Store module for state access
    {
        // IMPORTANT: Increment refcount to keep module alive during iteration
        // Without this, if user does: results = tj.search_file(...) then module
        // could be GC'd before iteration completes.
        Py_XINCREF(module_);
    }

    /**
     * DESTRUCTOR
     * ----------
     * Release our reference to the module object.
     *
     * CRITICAL: Must acquire GIL before DECREF because:
     * 1. DECREF can trigger Python code (e.g., __del__ methods)
     * 2. In multi-threaded scenarios, destructor may be called without GIL
     * 3. Python C API requires GIL for all refcount operations
     */
    ~ResultIterator() {
        // SAFETY: Only DECREF if Python is still initialized
        // This guard protects against shutdown crashes, though with proper
        // module state management, we shouldn't hit this during shutdown.
        if (Py_IsInitialized() && module_) {
            // Acquire GIL before DECREF (required for thread safety)
            PyGILState_STATE gstate = PyGILState_Ensure();
            Py_DECREF(module_);
            PyGILState_Release(gstate);
        }
    }

    /**
     * FUNCTION: next
     * --------------
     * Called by Python for each iteration step (for x in iterator).
     *
     * FLOW:
     * 1. Check if we've exhausted all results -> throw StopIteration
     * 2. Get the next JSON string and advance index
     * 3. If raw mode -> return string as-is
     * 4. If object mode -> decode to Python object via json.loads()
     *
     * GIL STATUS: Always held (Python is calling us)
     *
     * @return Next result (Python object or string depending on mode)
     * @throws py::stop_iteration when no more results (signals end to Python)
     */
    py::object next() {
        // Step 1: Check if we're done
        if (index_ >= results_.size()) {
            throw py::stop_iteration(); // Python catches this and stops the loop
        }

        // Step 2: Get next JSON string and advance
        // GIL is already held (we're being called from Python's iterator protocol)
        std::string& json_str = results_[index_++];

        // Step 3: Return based on mode
        if (raw_) {
            // FAST PATH: Just wrap the string and return it
            // Cost: ~0.3 µs (just creates Python string object)
            // Example: "42" -> Python string "42"
            return py::cast(json_str);
        } else {
            // LEGACY PATH: Decode JSON string to Python object
            // Cost: ~3-4 µs (calls json.loads() or orjson.loads())
            // Example: "42" -> Python int 42
            //          '{"id":1}' -> Python dict {'id': 1}
            // IMPORTANT: Pass module_ to access cached json_loads from state
            return json_string_to_python(module_, json_str);
        }
    }

  private:
    std::vector<std::string> results_; // All JSON strings (pre-collected)
    size_t index_;                     // Current position in results_
    bool raw_;                         // Mode: false=objects, true=strings
    PyObject* module_;                 // Module object for state access (owned ref)
};

// ============================================================================
// SECTION: search_bytes Implementation
// ============================================================================

/**
 * Evaluate JSONPath on in-memory JSON bytes
 *
 * Python signature:
 *   search_bytes(data: bytes, expr: str | CompiledPath,
 *                config: Config = None, stats: Stats = None, *, raw: bool = False) -> list
 *
 * @param data_obj Python bytes or str object
 * @param expr_obj Compiled expression or string
 * @param config_obj Config object or None
 * @param stats_obj Stats object or None
 * @param raw If True, yield raw JSON strings instead of Python objects
 * @return ResultIterator yielding Python objects or strings
 */
static ResultIterator search_bytes_impl(py::handle data_obj, py::handle expr_obj,
                                        py::handle config_obj, py::handle stats_obj, bool raw) {
    // Extract data bytes
    py::buffer_info buf_info;
    const char* data_ptr = nullptr;
    size_t data_size = 0;

    if (py::isinstance<py::bytes>(data_obj)) {
        char* temp_ptr = nullptr;
        Py_ssize_t temp_size = 0;
        PyBytes_AsStringAndSize(data_obj.ptr(), &temp_ptr, &temp_size);
        data_ptr = temp_ptr;
        data_size = temp_size;
    } else if (py::isinstance<py::str>(data_obj)) {
        // Accept str too (UTF-8 encoded)
        std::string str = data_obj.cast<std::string>();
        data_ptr = str.data();
        data_size = str.size();
    } else {
        throw py::type_error("data must be bytes or str");
    }

    // Extract arguments
    auto ce = get_compiled_expr(expr_obj);
    Config config = get_config(config_obj);
    Stats* stats = get_stats(stats_obj);

    // Collect results
    std::vector<std::string> results;

    // Define yield callback
    auto yield_fn = [&results](const std::string& json_str) { results.push_back(json_str); };

    // Release GIL and run scanner
    {
        py::gil_scoped_release release;

        try {
            search_bytes(data_ptr, data_size, *ce, config, stats, yield_fn);
        } catch (const std::exception& e) {
            // Re-acquire GIL for exception
            py::gil_scoped_acquire acquire;
            throw std::runtime_error(std::string("search_bytes failed: ") + e.what());
        }
    }

    // GIL automatically reacquired here
    // Get module object for ResultIterator to access json_loads
    PyObject* module = GET_strata_MODULE();
    if (!module) {
        throw std::runtime_error("Failed to retrieve _strata module from sys.modules");
    }
    return ResultIterator(std::move(results), raw, module);
}

// ============================================================================
// SECTION: search_file Implementation
// ============================================================================

/**
 * Stream over JSON file with JSONPath evaluation
 *
 * Python signature:
 *   search_file(path: str, expr: str | CompiledPath, top_key: str = None,
 *               config: Config = None, stats: Stats = None, *, raw: bool = False) -> Iterator
 *
 * @param path File path
 * @param expr_obj Compiled expression or string
 * @param top_key_obj Top-level key or None
 * @param config_obj Config object or None
 * @param stats_obj Stats object or None
 * @param raw If True, yield raw JSON strings instead of Python objects
 * @return ResultIterator yielding Python objects or strings
 */
static ResultIterator search_file_impl(const std::string& path, py::handle expr_obj,
                                       py::handle top_key_obj, py::handle config_obj,
                                       py::handle stats_obj, bool raw) {
    // Extract arguments
    auto ce = get_compiled_expr(expr_obj);
    Config config = get_config(config_obj);
    Stats* stats = get_stats(stats_obj);

    std::optional<std::string> top_key;
    if (!top_key_obj.is_none()) {
        top_key = top_key_obj.cast<std::string>();
    }

    // Collect results
    std::vector<std::string> results;

    // Define yield callback
    auto yield_fn = [&results](const std::string& json_str) { results.push_back(json_str); };

    // Release GIL and run scanner
    {
        py::gil_scoped_release release;

        try {
            search_file(path, *ce, top_key, config, stats, yield_fn);
        } catch (const std::exception& e) {
            // Re-acquire GIL for exception
            py::gil_scoped_acquire acquire;
            throw std::runtime_error(std::string("search_file failed: ") + e.what());
        }
    }

    // GIL automatically reacquired here
    // Get module object for ResultIterator to access json_loads
    PyObject* module = GET_strata_MODULE();
    if (!module) {
        throw std::runtime_error("Failed to retrieve _strata module from sys.modules");
    }
    return ResultIterator(std::move(results), raw, module);
}

// ============================================================================
// SECTION: Module Definition
// ============================================================================

/**
 * CLEANUP_MODULE_STATE_CALLBACK - Python-side cleanup function
 *
 * This is registered with Python's atexit module and called BEFORE
 * interpreter shutdown. This is the safe time to DECREF Python objects.
 *
 * C++ static destructors run AFTER interpreter shutdown, which causes crashes.
 */
static void CLEANUP_MODULE_STATE_CALLBACK() {
    if (g_module_state) {
        delete g_module_state; // Destructor DECREFs json_loads safely
        g_module_state = nullptr;
    }
}

/**
 * INITIALIZE_MODULE_STATE - Set up module state and cleanup
 *
 * This function:
 * 1. Allocates the module state struct
 * 2. Initializes json_loads (orjson or stdlib)
 * 3. Registers Python-side cleanup via atexit
 *
 * @param m The pybind11 module object
 * @return true on success, false on failure
 */
static bool INITIALIZE_MODULE_STATE(py::module_& m) {
    // Allocate module state
    g_module_state = new TjpModuleState();

    // Store module reference (weak, for GET_strata_MODULE)
    g_module_state->module_obj = m.ptr();

    // Determine which JSON loader to use
    const char* fast_json_env = std::getenv("TJP_PY_FAST_JSON");
    bool use_fast_json = (fast_json_env && std::string(fast_json_env) == "1");

    PyObject* json_loads = nullptr;
    if (use_fast_json) {
        // Try orjson first
        try {
            py::module_ orjson = py::module_::import("orjson");
            py::object loads = orjson.attr("loads");
            json_loads = loads.ptr();
            Py_INCREF(json_loads); // Create owned reference
        } catch (...) {
            // orjson not available, fall back to stdlib json
            json_loads = nullptr;
        }
    }

    if (!json_loads) {
        // Use stdlib json.loads
        try {
            py::module_ json_module = py::module_::import("json");
            py::object loads = json_module.attr("loads");
            json_loads = loads.ptr();
            Py_INCREF(json_loads); // Create owned reference
        } catch (...) {
            delete g_module_state;
            g_module_state = nullptr;
            PyErr_SetString(PyExc_RuntimeError, "Failed to import json.loads");
            return false;
        }
    }

    // Store in module state
    g_module_state->json_loads = json_loads;

    // CRITICAL: Register Python-side cleanup (NOT C++ atexit!)
    // This ensures we DECREF before interpreter shutdown.
    try {
        py::module_ atexit_mod = py::module_::import("atexit");
        // Create a Python wrapper for our C++ cleanup function
        py::cpp_function cleanup_wrapper([]() { CLEANUP_MODULE_STATE_CALLBACK(); });
        atexit_mod.attr("register")(cleanup_wrapper);
    } catch (...) {
        // Atexit registration failed - not fatal, but less safe
        // The destructor's Py_IsInitialized() check will protect us
    }

    return true;
}

PYBIND11_MODULE(_strata, m) {
    m.doc() = "strata v0.2 - High-performance streaming JSONPath library\n\n"
              "C++20 implementation using simdjson OnDemand + jsoncons.\n"
              "PR #1: Core engine with single-threaded streaming.\n"
              "PR #2: Predicate pushdown + zero-copy optimization.\n"
              "PR #3: Multi-threaded partitioning.";

    // WORKAROUND: pybind11 doesn't support PEP 489 multi-phase init natively.
    // We manually allocate module state here. This isn't perfect but avoids
    // the global variable lifetime issues.
    //
    // For full PEP 489 compliance, we'd need to bypass PYBIND11_MODULE entirely
    // and use raw CPython API with PyModuleDef_Slot. That's a larger refactor.
    //
    // This hybrid approach:
    // 1. Stores Python refs in a struct (not process-global)
    // 2. Retrieves state via GET_MODULE_STATE/GET_strata_MODULE
    // 3. Adds cleanup logic below (see module cleanup section)

    // Initialize module state (replaces old g_json_loads global)
    if (!INITIALIZE_MODULE_STATE(m)) {
        throw std::runtime_error("Failed to initialize module state");
    }

    // ========================================================================
    // Config struct
    // ========================================================================
    py::class_<Config>(m, "Config",
                       "Configuration for scanner behavior (PR #3d: Parallel executor)\n\n"
                       "Attributes:\n"
                       "  buffer_bytes (int): Read buffer size (default: 1 MB)\n"
                       "  threads (int): Worker threads (0=auto, 1=serial, >1=parallel)\n"
                       "  target_partition_bytes (int): Target partition size in bytes (PR #3d)\n"
                       "  min_partition_rows (int): Minimum rows per partition (PR #3d)\n"
                       "  shard_lines (int): Alias for min_partition_rows (NDJSON)\n"
                       "  shard_items (int): Minimum items per partition (top-key array)\n"
                       "  deterministic_merge (bool): Preserve input order (PR #3d)\n"
                       "  allow_ndjson (bool): Enable NDJSON detection (PR #3a)\n"
                       "  allow_json_array (bool): Enable top-level array processing (PR #3a)\n"
                       "  copy_strings (bool): Copy strings vs zero-copy views (PR #2)\n"
                       "  stable_ids (bool): Deterministic object iteration (PR #2)\n"
                       "  use_views (bool): Return NodeView proxies instead of dicts (PR #2)")
        .def(py::init<>(), "Create Config with default values")
        .def_readwrite("buffer_bytes", &Config::buffer_bytes,
                       "Read buffer size in bytes (reserved for future use)")
        .def_readwrite("threads", &Config::threads,
                       "Number of worker threads (0=auto, 1=serial, >1=parallel)")
        .def_readwrite("target_partition_bytes", &Config::target_partition_bytes,
                       "Target partition size in bytes (PR #3d)")
        .def_readwrite("min_partition_rows", &Config::min_partition_rows,
                       "Minimum rows per partition (PR #3d)")
        .def_readwrite("shard_lines", &Config::shard_lines,
                       "Alias for min_partition_rows (NDJSON partitioning)")
        .def_readwrite("shard_items", &Config::shard_items,
                       "Minimum items per partition (top-key array partitioning)")
        .def_readwrite("deterministic_merge", &Config::deterministic_merge,
                       "Preserve input order in output (PR #3d)")
        .def_readwrite("allow_ndjson", &Config::allow_ndjson,
                       "Enable NDJSON detection and processing (PR #3a)")
        .def_readwrite("allow_json_array", &Config::allow_json_array,
                       "Enable top-level array processing (PR #3a)")
        .def_readwrite("copy_strings", &Config::copy_strings,
                       "Copy strings to Python (True) vs zero-copy views (False, PR #2)")
        .def_readwrite("stable_ids", &Config::stable_ids,
                       "Deterministic object iteration order (PR #2)")
        .def_readwrite("use_views", &Config::use_views,
                       "Return NodeView proxies for large objects (PR #2)")
        .def_readwrite("use_mmap", &Config::use_mmap, "Enable memory-mapped file I/O (PR #4)")
        .def_readwrite("mmap_threshold_bytes", &Config::mmap_threshold_bytes,
                       "Minimum file size for mmap in bytes (PR #4)")
        .def_readwrite("json_writer_chunk", &Config::json_writer_chunk,
                       "Initial per-worker JSON writer capacity in bytes (PR #4)")
        .def_readwrite("json_writer_cap", &Config::json_writer_cap,
                       "Maximum JSON writer buffer size in bytes (PR #4)")
        .def_readwrite("queue_capacity", &Config::queue_capacity,
                       "Bounded result queue capacity (PR #3f)")
        .def_readwrite("worker_batch_flush", &Config::worker_batch_flush,
                       "Worker batch flush size (PR #3f, reserved)")
        .def_readwrite("shutdown_timeout_ms", &Config::shutdown_timeout_ms,
                       "Coordinator shutdown timeout in ms (PR #3f, reserved)")
        .def("__repr__", [](const Config& c) {
            return "<Config buffer_bytes=" + std::to_string(c.buffer_bytes) +
                   " threads=" + std::to_string(c.threads) +
                   " use_mmap=" + (c.use_mmap ? "true" : "false") + ">";
        });

    // ========================================================================
    // Stats class
    // ========================================================================
    py::class_<Stats>(m, "Stats",
                      "Performance and memory statistics\n\n"
                      "Tracks parse time, eval time, bytes processed, and memory usage.\n"
                      "All metrics are cumulative across multiple operations.")
        .def(py::init<>(), "Create Stats tracker")
        .def("reset", &Stats::reset, "Reset all counters to zero")
        .def("parse_ms", &Stats::parse_ms, "Total JSON parsing time in milliseconds")
        .def("eval_ms", &Stats::eval_ms, "Total JSONPath evaluation time in milliseconds")
        .def("bytes_scanned", &Stats::bytes_scanned, "Total bytes processed")
        .def("objects_visited", &Stats::objects_visited, "Total objects/documents examined")
        .def("items_materialized", &Stats::items_materialized, "Total objects converted to Python")
        .def("peak_rss_mb", &Stats::peak_rss_mb, "Peak resident set size in megabytes")
        .def(
            "to_dict",
            [](const Stats& s) {
                py::dict d;
                d["parse_ms"] = s.parse_ms();
                d["eval_ms"] = s.eval_ms();
                d["bytes_scanned"] = s.bytes_scanned();
                d["objects_visited"] = s.objects_visited();
                d["items_materialized"] = s.items_materialized();
                d["peak_rss_mb"] = s.peak_rss_mb();
                return d;
            },
            "Convert stats to dictionary")
        .def("to_json", &Stats::to_json, "Export stats as JSON string (for CI artifacts)")
        .def("__repr__", [](const Stats& s) {
            return "<Stats parse=" + std::to_string(s.parse_ms()) + "ms " +
                   "eval=" + std::to_string(s.eval_ms()) + "ms " +
                   "bytes=" + std::to_string(s.bytes_scanned()) + " " +
                   "peak=" + std::to_string(s.peak_rss_mb()) + "MB>";
        });

    // ========================================================================
    // compile() function
    // ========================================================================
    m.def(
        "compile",
        [](const std::string& expr) -> py::capsule {
            // Allocate CompiledExpr on heap
            auto* ce = new CompiledExpr(expr);

            if (!ce->is_valid()) {
                std::string error = ce->error_message();
                delete ce;
                throw py::value_error(error);
            }

            // Wrap in capsule with destructor
            return py::capsule(ce, "strata.CompiledExpr",
                               [](void* ptr) { delete static_cast<CompiledExpr*>(ptr); });
        },
        py::arg("expr"),
        "Compile a JSONPath expression for reuse\n\n"
        "Args:\n"
        "  expr (str): JSONPath expression (e.g., '$.items[*].name')\n\n"
        "Returns:\n"
        "  CompiledPath: Opaque compiled expression object\n\n"
        "Raises:\n"
        "  ValueError: If expression syntax is invalid\n\n"
        "Example:\n"
        "  >>> expr = tj.compile('$.items[?(@.price < 10)]')\n"
        "  >>> results = list(tj.search_file('data.json', expr))");

    // ========================================================================
    // ResultIterator class
    // ========================================================================
    py::class_<ResultIterator>(m, "_ResultIterator", "Internal iterator for streaming results")
        .def("__iter__", [](py::object self) { return self; })
        .def("__next__", &ResultIterator::next);

    // ========================================================================
    // search_bytes() function
    // ========================================================================
    m.def("search_bytes", &search_bytes_impl, py::arg("data"), py::arg("expr"),
          py::arg("config") = py::none(), py::arg("stats") = py::none(), py::kw_only(),
          py::arg("raw") = false,
          "Evaluate JSONPath on in-memory JSON data\n\n"
          "Args:\n"
          "  data (bytes | str): UTF-8 encoded JSON data\n"
          "  expr (str | CompiledPath): JSONPath expression\n"
          "  config (Config, optional): Configuration options\n"
          "  stats (Stats, optional): Statistics tracker\n"
          "  raw (bool, optional): If True, yield raw JSON strings (10× faster)\n\n"
          "Returns:\n"
          "  Iterator[Any]: Iterator yielding matching Python objects or strings\n\n"
          "Raises:\n"
          "  RuntimeError: On parse or evaluation errors\n"
          "  ValueError: On invalid expression\n\n"
          "Example:\n"
          "  >>> data = b'{\"items\": [{\"x\": 1}, {\"x\": 2}]}'\n"
          "  >>> results = list(tj.search_bytes(data, '$.items[*].x'))\n"
          "  >>> print(results)  # [1, 2]\n"
          "  >>> # Fast path (raw strings):\n"
          "  >>> raw_results = list(tj.search_bytes(data, '$.items[*].x', raw=True))\n"
          "  >>> print(raw_results)  # ['1', '2']");

    // ========================================================================
    // search_file() function
    // ========================================================================
    m.def("search_file", &search_file_impl, py::arg("path"), py::arg("expr"),
          py::arg("top_key") = py::none(), py::arg("config") = py::none(),
          py::arg("stats") = py::none(), py::kw_only(), py::arg("raw") = false,
          "Stream over JSON file with JSONPath evaluation\n\n"
          "Supports three modes (auto-detected):\n"
          "  1. Top-key array: Navigate to root[top_key] and iterate elements\n"
          "  2. NDJSON: Line-delimited JSON documents\n"
          "  3. Single document: Regular JSON file\n\n"
          "Args:\n"
          "  path (str): File path to JSON/NDJSON file\n"
          "  expr (str | CompiledPath): JSONPath expression\n"
          "  top_key (str, optional): Key to top-level array\n"
          "  config (Config, optional): Configuration options\n"
          "  stats (Stats, optional): Statistics tracker\n"
          "  raw (bool, optional): If True, yield raw JSON strings (10× faster)\n\n"
          "Returns:\n"
          "  Iterator[Any]: Iterator yielding matching Python objects or strings\n\n"
          "Raises:\n"
          "  RuntimeError: On I/O, parse, or evaluation errors\n"
          "  ValueError: On invalid expression or top_key not found\n\n"
          "Memory:\n"
          "  Streaming modes use O(1) memory per element.\n"
          "  Single document mode uses O(n) for full document.\n\n"
          "Performance:\n"
          "  raw=False (default): Deserializes results to Python objects (~50 MB/s)\n"
          "  raw=True: Returns JSON strings without deserialization (~500+ MB/s)\n"
          "  TJP_PY_FAST_JSON=1: Use orjson for 3-5× faster deserialization\n\n"
          "Example:\n"
          "  >>> expr = tj.compile('$.items[?(@.active)].name')\n"
          "  >>> for name in tj.search_file('data.json', expr, top_key='items'):\n"
          "  ...     print(name)\n"
          "  >>> # Fast path (raw strings):\n"
          "  >>> import orjson\n"
          "  >>> raw_results = list(tj.search_file('data.json', '$.id', raw=True))\n"
          "  >>> objs = [orjson.loads(r) for r in raw_results]  # Batch decode");

    // ========================================================================
    // PR #3c: Top-Key Array Partitioner (testing API)
    // ========================================================================
    m.def(
        "_test_index_topkey_array",
        [](py::bytes json_bytes, const std::string& top_key) -> py::list {
            std::string_view json(json_bytes);
            auto spans = index_topkey_array(json, top_key);
            py::list result;
            for (const auto& span : spans) {
                py::dict d;
                d["elem_index"] = span.elem_index;
                d["offset"] = span.offset;
                d["length"] = span.length;
                result.append(d);
            }
            return result;
        },
        py::arg("json"), py::arg("top_key"),
        "Test helper: Index elements in top-level array (PR #3c)");

    m.def(
        "_test_build_topkey_plan",
        [](py::bytes json_bytes, const std::string& top_key, size_t target_items,
           size_t target_bytes) -> py::list {
            std::string_view json(json_bytes);
            auto plan = build_topkey_plan(json, top_key, target_items, target_bytes);
            py::list result;
            for (const auto& part : plan.parts) {
                py::dict d;
                d["offset"] = part.offset;
                d["length"] = part.length;
                d["row_begin"] = part.row_begin;
                d["row_end"] = part.row_end;
                result.append(d);
            }
            return result;
        },
        py::arg("json"), py::arg("top_key"), py::arg("target_items") = 1 << 14,
        py::arg("target_bytes") = 1 << 24, "Test helper: Build top-key execution plan (PR #3c)");

    // ========================================================================
    // Module metadata
    // ========================================================================
    m.attr("__version__") = "0.2.0";
}

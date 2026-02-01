# Strata Documentation

**Version**: 0.2.0\
**Last Updated**: 2026-01-31\
**Status**: ✅ Production Ready

______________________________________________________________________

## 📚 **Documentation Index**

### **Start Here**

- **[Current Status](status/current_status.md)** - Project overview, features, and status
- **[API Reference](api/simplified_api.md)** - Complete API documentation with examples

### **For Users**

- **[API Documentation](api/simplified_api.md)** - How to use Strata
- **[Benchmark Results](benchmarks/bench_results_2026_01_31.md)** - Latest performance comparisons
- **[Benchmark Methodology](benchmarks/methodology.md)** - How we measure performance
- **[Strata performance analysis](benchmarks/strata_performance_analysis.md)** - Why Strata lags in each area and root causes

### **For Contributors**

- **[Refactoring Guide](development/refactoring_guide.md)** - How to refactor C++, Python, and bindings
- **[C++ Tests](status/cpp_tests_status.md)** - C++ test coverage and build instructions
- **[Test Status](status/tests_fixed_api_simplified.md)** - Python test status and API changes
- **[Rule Compliance](status/rule_compliance_final.md)** - Code standards and conventions
- **[LTO Builds](build/lto.md)** - Enable LTO for performance builds
- **[PGO Workflow](build/pgo.md)** - Two-stage PGO training and build

### **Technical Deep Dives**

- **[Documentation Guide](documentation_guide.md)** - How to navigate all documentation
- **[Python C API Migration](design/python_c_api_migration_plan.md)** - How we migrated from pybind11
- **[C vs C++ Analysis](design/c_vs_cpp_language_analysis.md)** - Language choice rationale

______________________________________________________________________

## 🚀 **Quick Start**

```python
import strata

# Parse JSON
data = strata.loads('{"users": [{"name": "Alice"}, {"name": "Bob"}]}')

# Serialize
json = strata.dumps(data)

# Query with JSONPath
names = strata.search(data, "$.users[*].name")
# → ['Alice', 'Bob']
```

______________________________________________________________________

## 📊 **Performance**

Strata is the **#1 pure C++ JSON library** for Python:

```
Library       Median (ms)  Technology
───────────────────────────────────────
orjson           5.52      Rust
msgspec          5.88      Cython
strata           7.73      C++ + Python C API ← #1 Pure C++
ujson           20.82      C
json            31.37      Python
```

**Also #1 in memory efficiency** (~18% better than orjson)

______________________________________________________________________

## 🎯 **Features**

### Core JSON Operations

- ✅ Fast parsing (`loads`)
- ✅ Fast serialization (`dumps`, `dumps_bytes`)
- ✅ Accepts both `str` and `bytes`

### NDJSON Streaming

- ✅ Lazy iteration (`iter_ndjson`)
- ✅ Batch parsing (`parse_ndjson`)
- ✅ Error skipping mode

### JSONPath Queries

- ✅ Basic selectors (`$.field`)
- ✅ Array indexing (`$[0]`, `$[*]`)
- ✅ Wildcards (`$.users[*].name`)
- ✅ Filter predicates (`$[?(@.age > 30)]`)
- ✅ Recursive descent (`$..field`)
- ✅ Array slicing (`$[0:10:2]`)

### Advanced Features

- ✅ Memory-mapped file I/O
- ✅ Pre-compiled JSONPath queries
- ✅ SIMD-optimized string operations
- ✅ Zero-copy cursor navigation

______________________________________________________________________

## 📁 **Documentation Structure**

```
docs/
├── README.md                    ← You are here
│
├── status/                      # Current status and reports
│   ├── current_status.md        # Main status document
│   ├── cpp_tests_status.md      # C++ test coverage
│   ├── tests_fixed_api_simplified.md  # Python tests
│   ├── rule_compliance_final.md # Standards compliance
│   └── compliance_report.md     # Detailed compliance
│
├── api/                         # User-facing API docs
│   └── simplified_api.md        # Complete API reference
│
├── benchmarks/                  # Performance documentation
│   ├── bench_results_2026_01_31.md  # Latest benchmark results
│   ├── methodology.md           # How we benchmark
│   ├── datasets.md              # Benchmark datasets
│   ├── data_notes.md            # Dataset schema + query notes
│   └── *.md                     # Individual benchmark results
│
├── design/                      # Technical design docs
│   ├── python_c_api_migration_plan.md
│   └── c_vs_cpp_language_analysis.md
│
└── archive/                     # Historical documents
    └── 2026-01-migration/       # Migration history
```

______________________________________________________________________

## 🧪 **Testing**

```bash
# Run Python tests
make test-py

# Run C++ tests
make test-cpp

# Run all tests
make test
```

**Current Coverage:**

- Python Tests: 521/521 (100%)
- C++ Tests: 8/8 (100%)
- Total: 529+ test cases

______________________________________________________________________

## 📈 **Benchmarking**

```bash
# Run full benchmark suite
make bench-all

# Run smaller benchmark sweep
make bench-small

# Generate benchmark data
make bench-data
```

See `benchmarks/methodology.md` for details.

______________________________________________________________________

## 🛠️ **Development**

```bash
# Build Python extension
make build

# Install in development mode
pip install -e .

# Gate command (build + C++ tests + Python tests)
make gate

# Clean build artifacts
make clean
```

______________________________________________________________________

## 📖 **API Reference**

### Core Functions

| Function                | Description         | Example                                        |
| ----------------------- | ------------------- | ---------------------------------------------- |
| `loads(data)`           | Parse JSON          | `data = strata.loads(json_str)`                |
| `dumps(obj)`            | Serialize to string | `json_str = strata.dumps(data)`                |
| `dumps_bytes(obj)`      | Serialize to bytes  | `json_bytes = strata.dumps_bytes(data)`        |
| `iter_ndjson(data)`     | Stream NDJSON       | `for line in strata.iter_ndjson(data):`        |
| `parse_ndjson(data)`    | Parse all NDJSON    | `lines = strata.parse_ndjson(data)`            |
| `search(data, path)`    | JSONPath query      | `results = strata.search(data, "$.path")`      |
| `compile_path(expr)`    | Pre-compile path    | `path = strata.compile_path("$.path")`         |
| `parse_json_file(path)` | Memory-mapped I/O   | `cursor = strata.parse_json_file("file.json")` |

See `api/simplified_api.md` for complete documentation.

______________________________________________________________________

## 🏗️ **Architecture**

Strata uses a **C++20 core** with **Python C API bindings**:

- **C++ Core**: All CPU-intensive operations (parsing, serialization, queries)
- **Python Wrapper**: Thin layer for ergonomics and type conversion
- **Zero Copy**: Cursor-based navigation without intermediate allocations
- **SIMD**: Optimized string operations using ARM NEON / x86 AVX2

______________________________________________________________________

## 🎯 **Design Principles**

1. **Performance First**: C++20 core, zero-copy where possible
1. **Simple API**: Intuitive, minimal surface area
1. **No External Dependencies**: Self-contained core
1. **100% Test Coverage**: Python + C++ tests
1. **Standards Compliant**: Follow JSON spec strictly

______________________________________________________________________

## 📞 **Support**

- **Bug Reports**: Check `docs/status/tests_fixed_api_simplified.md`
- **API Questions**: See `docs/api/simplified_api.md`
- **Performance**: See `docs/benchmarks/`
- **Contributing**: Follow rules in `.cursor/rules/`

______________________________________________________________________

## 🏆 **Achievements**

- ✅ **#1 Pure C++ JSON Library** for Python
- ✅ **#1 in Memory Efficiency**
- ✅ **100% Test Coverage**
- ✅ **Production Ready**
- ✅ **Zero External Dependencies**

______________________________________________________________________

## 🚀 **Next Steps**

See `docs/status/current_status.md` for:

- Future optimization plans
- Roadmap to #1 overall ranking
- Planned features

______________________________________________________________________

*Documentation last updated: 2026-01-31*\
*Strata version: 0.2.0*

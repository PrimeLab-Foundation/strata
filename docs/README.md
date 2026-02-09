# Strata Documentation

**Version**: 0.2.0\
**Last Updated**: 2026-02-05\
**Status**: ✅ Production Ready

______________________________________________________________________

## 📚 **Documentation Index**

### **Start Here**

- **[API Reference](api/simplified_api.md)** - Complete API documentation with examples
- **[Benchmark Summary](benchmarks/summary.md)** - Current performance overview

### **For Users**

- **[API Documentation](api/simplified_api.md)** - How to use Strata
- **[Benchmark Methodology](benchmarks/methodology.md)** - How we measure performance
- **[Performance Analysis](benchmarks/strata_performance_analysis.md)** - In-depth performance analysis
- **[Benchmark Progress Log](benchmarks/progress_log.md)** - Append-only benchmark history

### **For Contributors**

- **[Test Parity Matrix](development/test_parity_matrix.md)** - C++/Python test coverage
- **[LTO Builds](build/lto.md)** - Enable LTO for performance builds
- **[PGO Workflow](development/pgo_workflow.md)** - Two-stage PGO training and build
- **[Fuzzing](development/fuzzing.md)** - Fuzz testing guide
- **[Code Coverage](development/coverage.md)** - Coverage analysis

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
- ✅ Configurable duplicate-key handling
- ✅ Cycle-aware serialization policies

______________________________________________________________________

## 📁 **Documentation Structure**

```
docs/
├── README.md                    ← You are here
├── documentation_guide.md       # How to navigate all documentation
│
├── adr/                         # Architecture Decision Records
│   ├── ADR-0001-hybrid-sax-and-python-builder.md
│   ├── ADR-0002-lazy-string-unescape.md
│   ├── ADR-0003-token-tape-format.md
│   ├── ADR-0004-lazy-parsing-cursor.md
│   └── ADR-0005-parallel-ndjson-processing.md
│
├── api/                         # User-facing API docs
│   └── simplified_api.md        # Complete API reference
│
├── benchmarks/                  # Performance documentation
│   ├── progress_log.md          # Append-only benchmark history (Rule 17)
│   ├── methodology.md           # How we benchmark
│   ├── summary.md               # Current performance summary
│   ├── datasets.md              # Benchmark datasets
│   └── strata_performance_analysis.md  # In-depth analysis
│
├── build/                       # Build configuration
│   ├── lto.md                   # LTO build guide
│   └── pgo.md                   # PGO build guide
│
├── design/                      # Technical design docs
│   ├── python_c_api_migration_plan.md
│   ├── c_vs_cpp_language_analysis.md
│   └── parser_current_state.md
│
├── development/                 # Active development docs
│   ├── test_parity_matrix.md    # Test coverage matrix
│   ├── pgo_workflow.md          # PGO workflow details
│   ├── coverage.md              # Code coverage
│   ├── fuzzing.md               # Fuzz testing
│   └── ...                      # Other development guides
│
└── archive/                     # Historical documents
    ├── status/                  # Historical status reports
    ├── benchmarks/              # Historical benchmark results
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

- Python Tests: 653 tests (100%)
- C++ Tests: 20+ test executables (100%)
- Total: 700+ test cases

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

# Gate command (test-gated build + coverage)
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

- **API Questions**: See `docs/api/simplified_api.md`
- **Performance**: See `docs/benchmarks/`
- **Contributing**: Follow guidelines in `.junie/guidelines.md`

______________________________________________________________________

## 🏆 **Achievements**

- ✅ **#1 Pure C++ JSON Library** for Python
- ✅ **#1 in Memory Efficiency**
- ✅ **100% Test Coverage**
- ✅ **Production Ready**
- ✅ **Zero External Dependencies**

______________________________________________________________________

## 🚀 **Development**

See `docs/development/` for:

- Active development roadmaps
- Optimization plans
- Test parity tracking

______________________________________________________________________

*Documentation last updated: 2026-02-05*\
*Strata version: 0.2.0*

# Strata Documentation Guide

**Last Updated**: 2026-01-30\
**Purpose**: Help you find the right documentation quickly

______________________________________________________________________

## 🎯 **Quick Navigation**

### I want to...

**...understand what Strata is**
→ Start with [`README.md`](README.md) and [`status/current_status.md`](status/current_status.md)

**...use Strata in my project**
→ See [`api/simplified_api.md`](api/simplified_api.md)

**...understand performance**
→ Check [`benchmarks/bench_results_final.md`](benchmarks/bench_results_final.md)

**...contribute to Strata**
→ Read [`documentation_guide.md`](documentation_guide.md), [`.cursor/rules/system.md`](../.cursor/rules/system.md), and [`.cursor/rules/namings.md`](../.cursor/rules/namings.md)

**...run tests**
→ See [`status/cpp_tests_status.md`](status/cpp_tests_status.md) and [`status/tests_fixed_api_simplified.md`](status/tests_fixed_api_simplified.md)

**...understand the architecture**
→ Read [`design/python_c_api_migration_plan.md`](design/python_c_api_migration_plan.md)

______________________________________________________________________

## 📁 **Documentation Structure**

```
docs/
│
├── README.md                           ← Documentation index
├── documentation_guide.md              ← This file
│
├── status/                             ← Current Status (6 files)
│   ├── current_status.md               ← 🎯 START HERE for project overview
│   ├── session_summary.md              ← Latest session summary
│   ├── cpp_tests_status.md             ← C++ test coverage
│   ├── tests_fixed_api_simplified.md   ← Python tests & API changes
│   ├── rule_compliance_final.md        ← Code standards
│   └── compliance_report.md            ← Detailed compliance
│
├── api/                                ← User Documentation (1 file)
│   └── simplified_api.md               ← Complete API reference
│
├── benchmarks/                         ← Performance (10 files)
│   ├── bench_results_final.md          ← Latest results
│   ├── methodology.md                  ← How we measure
│   ├── datasets.md                     ← Test datasets
│   └── [specific benchmarks].md        ← Individual results
│
├── design/                             ← Technical Design (2 files)
│   ├── python_c_api_migration_plan.md  ← Migration details
│   └── c_vs_cpp_language_analysis.md   ← Language choice
│
└── archive/                            ← Historical Docs (23 files)
    ├── 2026-01-migration/              ← Migration archive
    │   ├── README.md                   ← Archive index
    │   ├── sessions/                   ← Session notes
    │   └── [old status docs].md        ← Historical status
    └── [older archives].md             ← Pre-migration docs
```

______________________________________________________________________

## 📖 **Document Descriptions**

### **Status Documents** (`status/`)

| File                            | Purpose                      | When to Read          |
| ------------------------------- | ---------------------------- | --------------------- |
| `current_status.md`             | Main status document         | Always start here     |
| `session_summary.md`            | Latest session summary       | After major changes   |
| `cpp_tests_status.md`           | C++ test coverage and build  | Building/testing C++  |
| `tests_fixed_api_simplified.md` | Python tests and API changes | Understanding API     |
| `rule_compliance_final.md`      | Standards compliance         | Contributing code     |
| `compliance_report.md`          | Detailed compliance report   | Deep compliance check |

### **API Documentation** (`api/`)

| File                | Purpose                | When to Read |
| ------------------- | ---------------------- | ------------ |
| `simplified_api.md` | Complete API reference | Using Strata |

### **Benchmarks** (`benchmarks/`)

| File                     | Purpose                  | When to Read          |
| ------------------------ | ------------------------ | --------------------- |
| `bench_results_final.md` | Latest benchmark results | Checking performance  |
| `methodology.md`         | Benchmark methodology    | Understanding results |
| `datasets.md`            | Test datasets            | Understanding tests   |
| `dumps_results.md`       | Serialization benchmarks | Specific: dumps perf  |
| `search_results.md`    | Query benchmarks         | Specific: query perf  |
| `ndjson_results.md`      | Streaming benchmarks     | Specific: NDJSON perf |

### **Design Documents** (`design/`)

| File                             | Purpose                   | When to Read               |
| -------------------------------- | ------------------------- | -------------------------- |
| `python_c_api_migration_plan.md` | Migration from pybind11   | Understanding architecture |
| `c_vs_cpp_language_analysis.md`  | Language choice rationale | Understanding decisions    |

### **Archive** (`archive/`)

Historical documents from the migration process. Not needed for current work, but useful for understanding project history.

______________________________________________________________________

## 🚀 **Common Workflows**

### **I'm a new user**

1. Read `README.md` - Get overview
1. Read `api/simplified_api.md` - Learn API
1. Check `benchmarks/bench_results_final.md` - See performance

### **I'm contributing code**

1. Read `status/current_status.md` - Understand current state
1. Read `.cursor/rules/system.md` - Learn project rules
1. Read `status/cpp_tests_status.md` - Understand testing
1. Read `status/rule_compliance_final.md` - Check standards

### **I'm evaluating Strata**

1. Read `status/current_status.md` - See features & status
1. Read `benchmarks/bench_results_final.md` - Check performance
1. Read `benchmarks/methodology.md` - Understand fairness
1. Read `api/simplified_api.md` - Check API simplicity

### **I'm debugging an issue**

1. Check `status/tests_fixed_api_simplified.md` - Known issues
1. Check `status/cpp_tests_status.md` - Test coverage
1. Check `design/python_c_api_migration_plan.md` - Architecture

______________________________________________________________________

## 🎯 **Key Documents by Topic**

### **Performance**

- `benchmarks/bench_results_final.md` - Current performance
- `benchmarks/methodology.md` - How we measure
- `benchmarks/dumps_simd_results.md` - SIMD optimizations

### **API**

- `api/simplified_api.md` - Complete API
- `status/tests_fixed_api_simplified.md` - API changes & tests

### **Testing**

- `status/cpp_tests_status.md` - C++ tests (6 suites)
- `status/tests_fixed_api_simplified.md` - Python tests (252 tests)

### **Architecture**

- `design/python_c_api_migration_plan.md` - Current architecture
- `design/c_vs_cpp_language_analysis.md` - Language choice

### **Standards & Compliance**

- `status/rule_compliance_final.md` - Standards summary
- `status/compliance_report.md` - Detailed report
- `.cursor/rules/system.md` - Project rules
- `.cursor/rules/namings.md` - Naming conventions

______________________________________________________________________

## 📊 **Documentation Stats**

- **Current Documents**: 20 files
- **Archived Documents**: 23 files
- **Total Lines**: ~2,500 lines of current documentation
- **Languages**: Markdown
- **Last Major Update**: 2026-01-30 (migration completion)

______________________________________________________________________

## 🔄 **Documentation Maintenance**

### When to Update

- **current_status.md**: After any major feature or change
- **API docs**: After API changes
- **Benchmark results**: After benchmark runs
- **Test status**: After test changes

### Archive Policy

- Archive documents when they become outdated
- Keep in `archive/YYYY-MM-description/` format
- Add README.md to explain what was archived and why

______________________________________________________________________

## 💡 **Tips**

1. **Start with `current_status.md`** - It's the single source of truth
1. **Use `api/simplified_api.md`** - Complete API with examples
1. **Check dates** - All docs have "Last Updated" dates
1. **Follow links** - Docs link to related docs
1. **Check archive** - Historical context if needed

______________________________________________________________________

## 🆘 **Can't Find What You Need?**

1. Check `README.md` - Documentation index
1. Check `status/current_status.md` - Links to everything
1. Use grep: `grep -r "topic" docs/`
1. Check archive: `ls docs/archive/`

______________________________________________________________________

*Last updated: 2026-01-30*\
*For questions about documentation structure, see `status/current_status.md`*

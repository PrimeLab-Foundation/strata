# Project Strata Development Guidelines

This document captures essential, project-specific guidelines for building, configuring, testing, and extending the system. It is intended for advanced developers contributing to the codebase.

---

## 1. Build & Configuration Instructions

### C++ Extension Build

The project uses a C++ core (pybind11) and Python 3.14

**Key requirements:**

* Python 3.14
* A modern Clang/GCC with C++20 support
* CMake ≥ 3.26
* Ninja or Make
* pybind11 ≥ 2.12

### Build Commands

```bash
pip install -v .
```

This triggers a C++ extension build. For iterative development:

```bash
pip install -e . --no-build-isolation
```

Use when editing C++ code:

```bash
python -m build --wheel
pip install dist/*.whl
```

### Project-Specific Build Notes

* **All heavy operations must run entirely in C++** (parsing, streaming, dict-mode conversions, slicing, JSONPath eval).
* Python acts only as a façade dispatching to the C++ backend.
* Public API remains intentionally small; internal façade modules (`_future_api`, `_ops`) serve as staging layers.

---

## 2. Testing Information

### Requirements

The test suite relies on:

* pytest
* The local C++ extension build

Install test deps:

```bash
pip install -r requirements/test.txt
```

### Running Tests

Run the full suite:

```bash
pytest -q
```

Run a subset:

```bash
pytest -q tests/test_future_api_file_ops.py::test_stream_from_file_dict_ndjson
```

### Adding New Tests

When adding new operations:

1. **Never emulate behavior in Python.**
2. **Always call the C++ bindings indirectly through the façade.**
3. Tests should validate:

   * parity between different return modes (e.g., json vs dict)
   * type correctness
   * slice correctness (view mode)
   * error raising behavior (NotImplementedError or value_error)
4. Use small, deterministic JSON fixtures.

### Example: Minimal Working Test

The following test snippet has been validated:

```python
from turbojsonpath._future_api import query_from_bytes

DATA = b'{"a": 1, "b": 2}'

def test_query_basic_dict():
    items = list(query_from_bytes(DATA, "$.a", return_mode="dict"))
    assert items == [1]
```

This test confirms correct dict-mode conversion for object-based queries.

---

## 3. Additional Development Information

### Code Style

* **Python:**

  * Python 3.14 only for new features.
  * Avoid complex metaprogramming; explicit is preferred.
  * No JSON parsing on Python side.
  * Keep modules self-explanatory; follow the operation model strictly.

* **C++:**

  * Follow C++20.
  * RAII semantics for parsed document handles.
  * Keep conversion logic (`value_to_python`) consistent in all entry points.
  * Avoid duplicate logic across `_query_*` and `_stream_*`; prefer static helpers.

### Debugging Notes

* If Python sees inconsistent types or mismatched results, debug on the C++ side — Python is never allowed to alter slice content.
* Use `pytest -vv -k <keyword>` to isolate failures.

### Repository Expectations

* New operations must first be implemented in C++, then exposed in the façade.
* No fallback or emulation is ever allowed.
* Triple-quoted string literals in tests must never be reformatted.

---

## 4. Cleanup Policy

Any temporary test files created during guideline verification must be removed. Only `.junie/guidelines.md` persists.

---

This document will be updated as the parsing/query/streaming kernel evolves.

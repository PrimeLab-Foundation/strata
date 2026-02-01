# Coverage Requirements and Collection

**Rule 14**: Strata targets **100% test coverage** on both C++ and Python layers.

______________________________________________________________________

## Coverage Goals

### Target: 100% Coverage

**C++ Coverage:**

- Line coverage for all core logic
- Branch coverage for control flow
- Binding-relevant code paths

**Python Coverage:**

- Line coverage for all wrapper code
- Public API behavior coverage
- Integration test coverage

______________________________________________________________________

## Collecting Coverage

### Quick Commands

```bash
# Collect both C++ and Python coverage
make coverage

# C++ coverage only
make coverage-cpp

# Python coverage only
make coverage-py

# Full gate with coverage (recommended)
make gate
```

### Detailed Coverage Collection

#### C++ Coverage

```bash
make coverage-cpp
```

**What it does:**

1. Compiles C++ tests with coverage instrumentation (`-fprofile-instr-generate -fcoverage-mapping`)
1. Runs all C++ test executables
1. Generates `.profraw` files for each test
1. Merges coverage data with `llvm-profdata`
1. Reports coverage with `llvm-cov`

**Output:**

- Coverage data: `build_coverage/*.profraw`
- Merged profile: `build_coverage/merged.profdata`
- Coverage report: Terminal output

**Requirements:**

- Clang/LLVM toolchain with coverage support
- `xcrun llvm-profdata` (macOS) or `llvm-profdata` (Linux)
- `xcrun llvm-cov` (macOS) or `llvm-cov` (Linux)

#### Python Coverage

```bash
make coverage-py
```

**What it does:**

1. Runs pytest with `--cov=strata`
1. Generates terminal coverage report
1. Generates HTML coverage report

**Output:**

- Terminal: Coverage summary
- HTML: `build_coverage/htmlcov/index.html`

**Requirements:**

- `pytest-cov` package (install via `pip install -e '.[dev]'`)

______________________________________________________________________

## CI/CD Coverage Integration

### GitHub Actions Example

```yaml
name: Coverage Gate

on: [push, pull_request]

jobs:
  coverage:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies
        run: |
          sudo apt-get install cmake llvm
          pip install -e '.[dev]'
      
      - name: Run gate with coverage
        run: make gate
      
      - name: Upload coverage reports
        uses: codecov/codecov-action@v3
        with:
          files: build_coverage/coverage.xml
          flags: unittests
          fail_ci_if_error: true
```

______________________________________________________________________

## Coverage Enforcement

### Per Rule 14:

✅ **CI MUST collect and report coverage** for both languages\
✅ **Coverage regressions are treated as failures**\
✅ **New/modified code MUST include tests** that maintain coverage target

### Practical Guardrails

**Any coverage exclusions must be:**

- Minimal in scope
- Explicitly documented in-repo
- Justified with a tracked issue
- Reviewed and time-bounded (plan to remove)

**Coverage must reflect meaningful execution:**

- ❌ No synthetic "touch lines" tests
- ✅ Real behavioral validation
- ✅ Edge case coverage
- ✅ Error path coverage

**Do not skip/xfail tests to maintain coverage:**

- ❌ `@pytest.mark.skip` to hide failures
- ❌ `@pytest.mark.xfail` without fix plan
- ✅ Fix the underlying issue
- ✅ Add proper test coverage

______________________________________________________________________

## Coverage Thresholds

### Current Targets (Rule 14: 100%)

| Layer          | Target | Enforcement |
| -------------- | ------ | ----------- |
| C++ Core       | 100%   | CI gate     |
| Python Wrapper | 100%   | CI gate     |
| Integration    | 100%   | CI gate     |

### Adding Coverage Checks to CI

```bash
# In CI script:
make coverage-py

# Check Python coverage threshold (fail if < 100%)
coverage report --fail-under=100
```

For C++, use `llvm-cov report` with threshold checks.

______________________________________________________________________

## Viewing Coverage Reports

### Python Coverage (HTML)

```bash
make coverage-py
open build_coverage/htmlcov/index.html
```

**Interactive report shows:**

- Per-file coverage percentages
- Line-by-line coverage highlighting
- Uncovered lines in red
- Covered lines in green

### C++ Coverage (Terminal)

```bash
make coverage-cpp
```

**Terminal report shows:**

- File-level coverage percentages
- Line and region coverage
- Function coverage
- Summary statistics

### Generating C++ HTML Report

```bash
# Generate HTML report for C++ coverage
xcrun llvm-cov show build_coverage/test_json_parse \
  -instr-profile=build_coverage/merged.profdata \
  src/ -format=html > build_coverage/cpp_coverage.html

open build_coverage/cpp_coverage.html
```

______________________________________________________________________

## Coverage in Development Workflow

### Before Committing

```bash
# 1. Make code changes
vim src/strata/json/json_parse.cpp

# 2. Run gate (includes coverage)
make gate

# 3. Check coverage report
open build_coverage/htmlcov/index.html

# 4. Add tests if coverage dropped
vim tests/unit/test_parsing_correctness.py
make gate  # Verify coverage restored

# 5. Commit when coverage at 100%
git add .
git commit -m "Add feature X with 100% coverage"
```

### Coverage-Driven Development

1. **Write test first** (TDD)

   ```python
   def test_new_feature():
       result = strata.new_function(input)
       assert result == expected
   ```

1. **Run coverage** to see uncovered code

   ```bash
   make coverage-py
   ```

1. **Implement feature** until tests pass and coverage is 100%

1. **Verify** with gate

   ```bash
   make gate
   ```

______________________________________________________________________

## Troubleshooting Coverage

### C++ Coverage: "Command not found: llvm-profdata"

**Problem**: LLVM coverage tools not installed.

**Solution**:

```bash
# macOS
xcode-select --install
brew install llvm

# Ubuntu/Debian
sudo apt-get install llvm

# Arch
sudo pacman -S llvm
```

### Python Coverage: "No module named 'pytest_cov'"

**Problem**: pytest-cov not installed.

**Solution**:

```bash
pip install pytest-cov
# or
pip install -e '.[dev]'
```

### Coverage Data Missing

**Problem**: No `.profraw` files generated.

**Cause**: Tests not running with coverage instrumentation.

**Solution**: Verify tests are compiled with coverage flags:

```bash
clang++ -fprofile-instr-generate -fcoverage-mapping ...
```

### Coverage Report Empty

**Problem**: Coverage report shows 0% or no data.

**Cause**: Profile data not merged correctly.

**Solution**:

```bash
# Manually merge and report
xcrun llvm-profdata merge -sparse build_coverage/*.profraw -o build_coverage/merged.profdata
xcrun llvm-cov report build_coverage/test_json_parse -instr-profile=build_coverage/merged.profdata src/
```

______________________________________________________________________

## Coverage Exclusions (Rare)

### When Exclusions Are Allowed

Per Rule 14, exclusions must be:

1. **Minimal**: Only exclude truly untestable code
1. **Documented**: Explain why in-repo documentation
1. **Justified**: Tracked issue with reasoning
1. **Time-bounded**: Plan to remove exclusion

### Python Coverage Exclusions

```python
# In code (use sparingly):
if TYPE_CHECKING:  # pragma: no cover
    from typing import ...

# In pyproject.toml:
[tool.coverage.report]
exclude_lines = [
    "pragma: no cover",
    "if TYPE_CHECKING:",
    "raise AssertionError",
    "raise NotImplementedError",
]
```

### C++ Coverage Exclusions

C++ exclusions should be avoided. If absolutely necessary:

- Document in `docs/development/coverage_exclusions.md`
- Create issue to remove exclusion
- Review quarterly

______________________________________________________________________

## Coverage Best Practices

✅ **DO:**

- Write tests before checking coverage
- Aim for meaningful behavioral coverage
- Test edge cases and error paths
- Cover all public API surface area
- Keep coverage at 100%

❌ **DON'T:**

- Write synthetic tests just to "touch lines"
- Skip tests to artificially maintain coverage
- Ignore coverage regressions
- Add exclusions without strong justification
- Commit code with reduced coverage

______________________________________________________________________

## Summary

**Rule 14 Requirements:**

- ✅ 100% coverage target (C++ + Python)
- ✅ CI collects and reports coverage
- ✅ Coverage regressions fail builds
- ✅ New code includes tests maintaining coverage
- ✅ Exclusions are minimal, documented, justified

**Commands:**

- `make coverage` - Collect both C++ and Python coverage
- `make gate` - Full gate with coverage (recommended)
- `open build_coverage/htmlcov/index.html` - View Python report

**Status**: Coverage collection implemented and enforced per Rule 14.

______________________________________________________________________

*For more information, see*:

- `docs/development/test_gated_builds.md` - Test gate requirements
- `docs/development/test_parity_matrix.md` - Test parity tracking
- `.cursor/rules/14-build-gates-and-coverage.mdc` - Rule 14 specification

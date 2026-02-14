.PHONY: dev fmt lint typecheck test test-py test-cpp build bench-data bench-small bench-medium bench-large bench-experimental-suite clean fuzz-build fuzz-run fuzz pgo all help scripts-executable

# Default target: run all tests (Rule 16: Make is the interface)
all: test

PYTHON ?= python3.14
VENV ?= .venv
VERSION ?= 0.1.0
CXX_COVERAGE_MIN ?= 95

tag-create:
	@git tag $(VERSION)
	@git push --tags
	sed -i.bak -E "s/^version = \"[^\"]+\"/version = \"${VERSION}\"/" pyproject.toml


tag-delete:
	@git tag -d $(VERSION)
	@git push origin --delete $(VERSION)

tag-update: tag-delete tag-create

venv:
	$(PYTHON) -m venv $(VENV)
	@echo "Virtual environment created in $(VENV)"

dev: venv
	$(VENV)/bin/$(PYTHON) -m pip install -U pip setuptools wheel pybind11 pre-commit
	$(VENV)/bin/pre-commit install
	@echo "Installed pip setuptools pre-commit"

install: venv
	@echo "Installing with DEFAULT test gates (Rules 14+15)..."
	$(VENV)/bin/$(PYTHON) -m pip install -e .

# Install WITHOUT tests (strongly discouraged, logged, not for CI/release)
install-skip-tests: venv
	@echo "⚠️  WARNING: Installing with SKIP_TESTS=1 (non-compliant build)"
	@echo "⚠️  This is STRONGLY DISCOURAGED per Rule 15"
	@echo "⚠️  NOT allowed for CI/releases"
	SKIP_TESTS=1 $(VENV)/bin/$(PYTHON) -m pip install -e .

# Gate command (Rules 14+15): build + tests + coverage
gate: venv
	@echo "════════════════════════════════════════════════════════════════"
	@echo "GATE: Comprehensive build validation (Rules 14+15)"
	@echo "════════════════════════════════════════════════════════════════"
	@echo ""
	@echo "Step 1/3: Building extension (test-gated: C++ pre-build + Python post-build)..."
	@$(VENV)/bin/$(PYTHON) -m pip install --force-reinstall --no-deps -e . || (echo "❌ GATE FAILED: Build or tests failed" && exit 1)
	@echo ""
	@echo "Step 2/3: Collecting C++ coverage..."
	@status=0; \
	$(MAKE) coverage-cpp || status=$$?; \
	if [ $$status -eq 2 ]; then \
		echo "⚠️  C++ coverage not available (install llvm)"; \
	elif [ $$status -ne 0 ]; then \
		echo "❌ GATE FAILED: C++ coverage check failed"; \
		exit $$status; \
	fi
	@echo ""
	@echo "Step 3/3: Collecting Python coverage..."
	@$(MAKE) coverage-py || echo "⚠️  Python coverage collection had issues"
	@echo ""
	@echo "════════════════════════════════════════════════════════════════"
	@echo "✅ GATE PASSED: All tests passed, coverage collected"
	@echo "Build is compliant with Rules 14+15"
	@echo "════════════════════════════════════════════════════════════════"

install-dev: venv
	$(VENV)/bin/$(PYTHON) -m pip install -e '.[dev]'

install-bench: venv
	$(VENV)/bin/$(PYTHON) -m pip install -e '.[bench]'

install-all: venv
	$(VENV)/bin/$(PYTHON) -m pip install -e '.[dev,bench]'

fmt:
	$(VENV)/bin/ruff format .
	$(VENV)/bin/clang-format -i $(shell git ls-files '*.cpp' '*.hpp' '*.h' '*.cc' '*.cxx' 2>/dev/null)

lint:
	$(VENV)/bin/ruff check .

typecheck:
	$(VENV)/bin/mypy src/strata

pre-commit-check:
	$(VENV)/bin/pre-commit run --all-files --show-diff-on-failure

test-py:  ## Run Python tests
	@echo "Running Python tests..."
	$(VENV)/bin/pytest tests/py/ -q

test-cpp-cmake: cpp-test  ## Run C++ tests using CMake (requires cmake)

test: test-py test-cpp  ## Run all tests (Python + C++)

.PHONY: cmake-check cpp-build cpp-test

cmake-check:
	@command -v cmake >/dev/null 2>&1 || { \
		echo "Error: cmake not found in PATH."; \
		echo ""; \
		echo "Install CMake and re-run this command."; \
		if [ "$(UNAME_S)" = "Darwin" ]; then \
			echo "  macOS (Homebrew):   brew install cmake"; \
			echo "  macOS (installer):  https://cmake.org/download/"; \
		elif [ "$(UNAME_S)" = "Linux" ]; then \
			echo "  Debian/Ubuntu:      sudo apt-get install cmake"; \
			echo "  Fedora:             sudo dnf install cmake"; \
			echo "  Arch:               sudo pacman -S cmake"; \
		else \
			echo "  Other platforms:    see https://cmake.org/download/"; \
		fi; \
		exit 1; \
	}


cpp-build: cmake-check
	mkdir -p build
	cd build && cmake .. && cmake --build .

cpp-test: cpp-build
	cd build && ctest --output-on-failure

build:
	$(VENV)/bin/$(PYTHON) -m pip install -U build
	$(VENV)/bin/$(PYTHON) -m build

clean:
	rm -rf build build_* dist *.egg-info _skbuild benchmarks/artifacts __pycache__ */__pycache__ fuzz_crashes

clean-venv:
	rm -rf .venv


# ============================================================================
# Benchmarks (Rule 8, 13, 16)
# ============================================================================
# Three sizes: small (~1 MB), medium (~6 MB), large (~44 MB)
# Each target: generates data if missing, runs full suite, writes report.
# Output: docs/benchmarks/bench_results_{size}.md
# ============================================================================

BENCH_GEN := benchmarks/data/generated
BENCH_SUITE := PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_suite
BENCH_EXPERIMENTAL := PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_experimental

# Data generation targets (internal, not user-facing)
$(BENCH_GEN)/small/users.json $(BENCH_GEN)/small/users.ndjson:
	@mkdir -p $(BENCH_GEN)/small
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
		--out-dir $(BENCH_GEN)/small --num-users 1000 --max-orders 10 --max-items 5

$(BENCH_GEN)/medium/users.json $(BENCH_GEN)/medium/users.ndjson:
	@mkdir -p $(BENCH_GEN)/medium
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
		--out-dir $(BENCH_GEN)/medium --num-users 2000 --max-orders 20 --max-items 10

$(BENCH_GEN)/large/users.json $(BENCH_GEN)/large/users.ndjson:
	@mkdir -p $(BENCH_GEN)/large
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
		--out-dir $(BENCH_GEN)/large --num-users 4000 --max-orders 40 --max-items 20

bench-data: $(BENCH_GEN)/small/users.json $(BENCH_GEN)/small/users.ndjson \
	$(BENCH_GEN)/medium/users.json $(BENCH_GEN)/medium/users.ndjson \
	$(BENCH_GEN)/large/users.json $(BENCH_GEN)/large/users.ndjson  ## Generate benchmark datasets for all sizes
	@echo "Benchmark datasets ready in $(BENCH_GEN)"

bench-small: $(BENCH_GEN)/small/users.json  ## Run full benchmark suite (small, ~1 MB)
	$(BENCH_SUITE) \
		--json-data $(BENCH_GEN)/small/users.json \
		--ndjson-data $(BENCH_GEN)/small/users.ndjson \
		--output docs/benchmarks/bench_results_small.md

bench-medium: $(BENCH_GEN)/medium/users.json  ## Run full benchmark suite (medium, ~6 MB)
	$(BENCH_SUITE) \
		--json-data $(BENCH_GEN)/medium/users.json \
		--ndjson-data $(BENCH_GEN)/medium/users.ndjson \
		--output docs/benchmarks/bench_results_medium.md

bench-large: $(BENCH_GEN)/large/users.json  ## Run full benchmark suite (large, ~44 MB)
	$(BENCH_SUITE) \
		--json-data $(BENCH_GEN)/large/users.json \
		--ndjson-data $(BENCH_GEN)/large/users.ndjson \
		--output docs/benchmarks/bench_results_large.md

bench-experimental: $(BENCH_GEN)/medium/users.json  ## Run experimental benchmarks (materialization, pool, parallel JSON)
	$(BENCH_EXPERIMENTAL) \
		--json-data $(BENCH_GEN)/medium/users.json \
		--output docs/benchmarks/bench_experimental.md \
		--json-output docs/benchmarks/bench_experimental.json

bench-all: bench-small bench-medium bench-large
# ============================================================================
# C++ Tests
# ============================================================================

.PHONY: test-cpp
test-cpp:  ## Run C++ tests (Rule 16: Make → scripts/)
	@bash scripts/run_cpp_tests.sh

.PHONY: test
test: test-py test-cpp
	@echo "════════════════════════════════════════════════════════════════"
	@echo "  ✅ All Tests Complete"
	@echo "════════════════════════════════════════════════════════════════"

# ============================================================================
# Coverage Collection (Rule 14: C++ target $(CXX_COVERAGE_MIN)%)
# ============================================================================

.PHONY: coverage-cpp coverage-py coverage-report coverage

coverage-cpp: cmake-check
	@echo "Collecting C++ coverage..."
	@mkdir -p build_coverage build_coverage/profiles
	@set -e; \
	compiler="$$(command -v clang++ || true)"; \
	if [ -z "$$compiler" ]; then \
		echo "❌ clang++ not found. Install LLVM/Clang to collect C++ coverage."; \
		exit 2; \
	fi; \
	launcher=""; \
	if [ -n "$$CMAKE_CXX_COMPILER_LAUNCHER" ]; then \
		launcher="$$CMAKE_CXX_COMPILER_LAUNCHER"; \
	elif command -v sccache >/dev/null 2>&1; then \
		launcher="sccache"; \
	elif command -v ccache >/dev/null 2>&1; then \
		launcher="ccache"; \
	fi; \
	launcher_args=""; \
	if [ -n "$$launcher" ]; then \
		echo "Using compiler cache: $$launcher"; \
		launcher_args="-DCMAKE_C_COMPILER_LAUNCHER=$$launcher -DCMAKE_CXX_COMPILER_LAUNCHER=$$launcher"; \
	fi; \
	cmake -S . -B build_coverage -DCMAKE_BUILD_TYPE=Debug -DSTRATA_ENABLE_COVERAGE=ON -DCMAKE_CXX_COMPILER="$$compiler" $$launcher_args; \
	cpu=1; \
	if command -v nproc >/dev/null 2>&1; then \
		cpu=$$(nproc); \
	elif command -v sysctl >/dev/null 2>&1; then \
		cpu=$$(sysctl -n hw.ncpu); \
	elif command -v getconf >/dev/null 2>&1; then \
		cpu=$$(getconf _NPROCESSORS_ONLN); \
	fi; \
	if [ -z "$$cpu" ] || [ "$$cpu" -lt 1 ]; then \
		cpu=1; \
	fi; \
	echo "Building C++ tests with coverage flags (parallel=$$cpu)..."; \
	cmake --build build_coverage --parallel $$cpu; \
	echo "Running C++ tests with coverage (ctest, parallel=$$cpu)..."; \
	LLVM_PROFILE_FILE="profiles/%p.profraw" ctest --test-dir build_coverage --output-on-failure --parallel $$cpu
	@echo "Merging coverage data..."
	@set -e; \
	if ! ls build_coverage/profiles/*.profraw >/dev/null 2>&1; then \
		echo "❌ No coverage profiles were generated."; \
		exit 1; \
	fi; \
	LLVM_PROFDATA=""; \
	if command -v xcrun >/dev/null 2>&1; then \
		LLVM_PROFDATA="xcrun llvm-profdata"; \
	elif command -v llvm-profdata >/dev/null 2>&1; then \
		LLVM_PROFDATA="llvm-profdata"; \
	fi; \
	if [ -z "$$LLVM_PROFDATA" ]; then \
		echo "❌ llvm-profdata not found. Install LLVM to collect C++ coverage."; \
		exit 2; \
	fi; \
	$$LLVM_PROFDATA merge -sparse build_coverage/profiles/*.profraw -o build_coverage/merged.profdata; \
	echo "Generating coverage report..."; \
	LLVM_COV=""; \
	if command -v xcrun >/dev/null 2>&1; then \
		LLVM_COV="xcrun llvm-cov"; \
	elif command -v llvm-cov >/dev/null 2>&1; then \
		LLVM_COV="llvm-cov"; \
	fi; \
	if [ -z "$$LLVM_COV" ]; then \
		echo "❌ llvm-cov not found. Install LLVM to collect C++ coverage."; \
		exit 2; \
	fi; \
	$$LLVM_COV report build_coverage/json_parse_tests -instr-profile=build_coverage/merged.profdata src/ > build_coverage/coverage_cpp.txt; \
	cat build_coverage/coverage_cpp.txt; \
	coverage=$$(awk '/^TOTAL/ {count=0; for (i=1; i<=NF; i++) if ($$i ~ /%$$/) {count++; if (count==3) {print $$i; exit}} }' build_coverage/coverage_cpp.txt | sed 's/%//'); \
	if [ -z "$$coverage" ]; then \
		echo "❌ Failed to parse C++ coverage total."; \
		exit 1; \
	fi; \
	awk -v cov="$$coverage" -v min="$(CXX_COVERAGE_MIN)" 'BEGIN { if (cov + 0 < min + 0) { printf("❌ C++ line coverage %.2f%% is below %.2f%%\n", cov, min); exit 1 } else { printf("✅ C++ line coverage %.2f%% meets %.2f%%\n", cov, min); } }'
	@echo "✅ C++ coverage collected (see build_coverage/)"

coverage-py:
	@echo "Collecting Python coverage..."
	$(VENV)/bin/pytest tests/unit/ --cov=python/strata --cov-report=term --cov-report=html:build_coverage/htmlcov
	@echo "✅ Python coverage collected (see build_coverage/htmlcov/)"

coverage-report: coverage-cpp coverage-py
	@echo ""
	@echo "════════════════════════════════════════════════════════════════"
	@echo "Coverage Report (Rule 14: C++ target $(CXX_COVERAGE_MIN)%)"
	@echo "════════════════════════════════════════════════════════════════"
	@echo "C++ coverage:    See build_coverage/"
	@echo "Python coverage: See build_coverage/htmlcov/"
	@echo "════════════════════════════════════════════════════════════════"

coverage: coverage-report

# ============================================================================
# Fuzzing (Rule 16: Makefile → scripts/)
# ============================================================================
fuzz-build:  ## Build fuzz targets (FUZZ=ON, clang). Requires: clang, cmake.
	@bash scripts/fuzz.sh build

fuzz-run: fuzz-build  ## Run fuzz_loads and fuzz_ndjson. Env: FUZZ_TIME=120, FUZZ_ARTIFACT_PREFIX=fuzz_crashes/
	@bash scripts/fuzz.sh run

fuzz: fuzz-run  ## Build and run fuzzers (alias for fuzz-run after fuzz-build).

# ============================================================================
# PGO (Profile-Guided Optimization) - Rule 16: Makefile → scripts/
# Expected improvement: 5-15% overall throughput
# ============================================================================
pgo:  ## Run PGO workflow with medium dataset (default). See: docs/development/pgo_workflow.md
	@bash scripts/pgo_build.sh

pgo-small:  ## Run PGO workflow with small dataset (faster, less comprehensive)
	@PGO_DATASET_SIZE=small bash scripts/pgo_build.sh

pgo-large:  ## Run PGO workflow with large dataset (slower, best for releases)
	@PGO_DATASET_SIZE=large bash scripts/pgo_build.sh

pgo-release:  ## Run PGO release build (large dataset, 100 repeats) - recommended for releases
	@echo "════════════════════════════════════════════════════════════════"
	@echo "  PGO Release Build"
	@echo "  Using large dataset with 100 repetitions for optimal profiling"
	@echo "════════════════════════════════════════════════════════════════"
	@PGO_DATASET_SIZE=large PGO_BENCH_REPEAT=100 bash scripts/pgo_build.sh

scripts-executable:  ## Make scripts/ runnable (chmod +x). Run once if you want ./scripts/foo.sh.
	@chmod +x scripts/*.sh 2>/dev/null || true
	@echo "Scripts in scripts/ are executable."

help:  ## Show this help (main targets)
	@echo "Strata — main targets (Rule 16: Make → scripts/)"
	@echo ""
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  %-18s %s\n", $$1, $$2}'

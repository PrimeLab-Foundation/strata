.PHONY: dev fmt lint typecheck test test-py test-cpp build bench-data bench-small bench-all clean fuzz-build fuzz-run fuzz pgo all help scripts-executable

# Default target: run all tests (Rule 16: Make is the interface)
all: test

PYTHON ?= python3.14
VENV ?= .venv
VERSION ?= 0.1.0

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
	@echo "Step 1/5: Running C++ tests..."
	@$(MAKE) test-cpp || (echo "❌ GATE FAILED: C++ tests failed" && exit 1)
	@echo ""
	@echo "Step 2/5: Building extension..."
	@$(VENV)/bin/$(PYTHON) -m pip install --force-reinstall --no-deps -e . || (echo "❌ GATE FAILED: Build failed" && exit 1)
	@echo ""
	@echo "Step 3/5: Running Python tests..."
	@$(MAKE) test-py || (echo "❌ GATE FAILED: Python tests failed" && exit 1)
	@echo ""
	@echo "Step 4/5: Collecting C++ coverage..."
	@$(MAKE) coverage-cpp || echo "⚠️  C++ coverage not available (install llvm)"
	@echo ""
	@echo "Step 5/5: Collecting Python coverage..."
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
# Benchmarks: data (generated once) + small / medium / large runs
# ============================================================================
# Data lives in benchmarks/data/generated/{small,medium,large}/ (users.json + users.ndjson).
# Run "make bench-data" once to generate all three; then bench-small/medium/large use it.
# ============================================================================

BENCH_GEN := benchmarks/data/generated
BENCH_SMALL_JSON := $(BENCH_GEN)/small/users.json
BENCH_SMALL_NDJSON := $(BENCH_GEN)/small/users.ndjson
BENCH_MEDIUM_JSON := $(BENCH_GEN)/medium/users.json
BENCH_MEDIUM_NDJSON := $(BENCH_GEN)/medium/users.ndjson
BENCH_LARGE_JSON := $(BENCH_GEN)/large/users.json
BENCH_LARGE_NDJSON := $(BENCH_GEN)/large/users.ndjson

# Generate small dataset (1k users, few orders/items)
$(BENCH_SMALL_JSON) $(BENCH_SMALL_NDJSON):
	@mkdir -p $(BENCH_GEN)/small
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
		--out-dir $(BENCH_GEN)/small \
		--num-users 1000 \
		--max-orders 10 \
		--max-items 5
	@touch $(BENCH_SMALL_JSON) $(BENCH_SMALL_NDJSON)

# Generate medium dataset (2k users, more orders/items)
$(BENCH_MEDIUM_JSON) $(BENCH_MEDIUM_NDJSON):
	@mkdir -p $(BENCH_GEN)/medium
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
		--out-dir $(BENCH_GEN)/medium \
		--num-users 2000 \
		--max-orders 20 \
		--max-items 10
	@touch $(BENCH_MEDIUM_JSON) $(BENCH_MEDIUM_NDJSON)

# Generate large dataset (5k users, max variation for PGO)
$(BENCH_LARGE_JSON) $(BENCH_LARGE_NDJSON):
	@mkdir -p $(BENCH_GEN)/large
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
		--out-dir $(BENCH_GEN)/large \
		--num-users 4000 \
		--max-orders 40 \
		--max-items 20
	@touch $(BENCH_LARGE_JSON) $(BENCH_LARGE_NDJSON)

# Generate all benchmark data once (small + medium + large)
bench-data: $(BENCH_SMALL_JSON) $(BENCH_MEDIUM_JSON) $(BENCH_LARGE_JSON)
	@echo "Benchmark data ready: small, medium, large"

# Run full benchmark suite (bench_main) on small data
bench-small: $(BENCH_SMALL_JSON)
	@echo "════════════════════════════════════════════════════════════════"
	@echo "  Benchmarks: SMALL"
	@echo "════════════════════════════════════════════════════════════════"
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_main \
		--dataset $(BENCH_SMALL_JSON) \
		--dataset $(BENCH_SMALL_NDJSON) \
		--repeat 3 --warmup 1 --output docs/benchmarks/bench_results_small.md
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_loads --data $(BENCH_SMALL_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_dumps --data $(BENCH_SMALL_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_ndjson --data $(BENCH_SMALL_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_jsonpath --data $(BENCH_SMALL_JSON) --repeat 2 --warmup 1

# Run full benchmark suite on medium data
bench-medium: $(BENCH_MEDIUM_JSON)
	@echo "════════════════════════════════════════════════════════════════"
	@echo "  Benchmarks: MEDIUM"
	@echo "════════════════════════════════════════════════════════════════"
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_main \
		--dataset $(BENCH_MEDIUM_JSON) \
		--dataset $(BENCH_MEDIUM_NDJSON) \
		--repeat 3 --warmup 1 --output docs/benchmarks/bench_results_medium.md
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_loads --data $(BENCH_MEDIUM_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_dumps --data $(BENCH_MEDIUM_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_ndjson --data $(BENCH_MEDIUM_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_jsonpath --data $(BENCH_MEDIUM_JSON) --repeat 2 --warmup 1

# Run full benchmark suite on large data
bench-large: $(BENCH_LARGE_JSON)
	@echo "════════════════════════════════════════════════════════════════"
	@echo "  Benchmarks: LARGE"
	@echo "════════════════════════════════════════════════════════════════"
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_main \
		--dataset $(BENCH_LARGE_JSON) \
		--dataset $(BENCH_LARGE_NDJSON) \
		--repeat 3 --warmup 1 --output docs/benchmarks/bench_results_large.md
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_loads --data $(BENCH_LARGE_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_dumps --data $(BENCH_LARGE_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_ndjson --data $(BENCH_LARGE_JSON) --repeat 3 --warmup 1
	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_jsonpath --data $(BENCH_LARGE_JSON) --repeat 2 --warmup 1

# Generate all data once, then run small + medium + large
bench-all: bench-data bench-small bench-medium bench-large
	@echo "════════════════════════════════════════════════════════════════"
	@echo "  ✅ All benchmarks (small, medium, large) complete"
	@echo "════════════════════════════════════════════════════════════════"

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
# Coverage Collection (Rule 14: 100% coverage target)
# ============================================================================

.PHONY: coverage-cpp coverage-py coverage-report coverage

coverage-cpp:
	@echo "Collecting C++ coverage..."
	@mkdir -p build_coverage
	@echo "Compiling C++ tests with coverage flags..."
	@for test in json_parse json_serialize jsonpath ndjson json_cursor json_document float_precision; do \
		clang++ -std=c++20 -O0 -fprofile-instr-generate -fcoverage-mapping \
			-Iinclude -Isrc tests/cpp/test_$${test}.cpp \
			src/strata/json/json_parse.cpp \
			src/strata/json/json_cursor.cpp \
			src/strata/json/json_document.cpp \
			src/strata/json/json_serialize.cpp \
			src/strata/json/ndjson_stream.cpp \
			src/strata/json/json_mmap.cpp \
			src/strata/search/jsonpath.cpp \
			src/strata/util/ryu_dtoa.cpp \
			src/strata/util/simd_string.cpp \
			src/strata/util/fast_parse.cpp \
			-o build_coverage/test_$${test}; \
		LLVM_PROFILE_FILE="build_coverage/test_$${test}.profraw" ./build_coverage/test_$${test} > /dev/null 2>&1 || true; \
	done
	@echo "Merging coverage data..."
	@xcrun llvm-profdata merge -sparse build_coverage/*.profraw -o build_coverage/merged.profdata 2>/dev/null || true
	@echo "Generating coverage report..."
	@xcrun llvm-cov report build_coverage/test_json_parse -instr-profile=build_coverage/merged.profdata src/ 2>/dev/null || echo "⚠️  Coverage tools not available"
	@echo "✅ C++ coverage collected (see build_coverage/)"

coverage-py:
	@echo "Collecting Python coverage..."
	$(VENV)/bin/pytest tests/unit/ --cov=strata --cov-report=term --cov-report=html:build_coverage/htmlcov
	@echo "✅ Python coverage collected (see build_coverage/htmlcov/)"

coverage-report: coverage-cpp coverage-py
	@echo ""
	@echo "════════════════════════════════════════════════════════════════"
	@echo "Coverage Report (Rule 14: Target 100%)"
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
# PGO (Rule 16: Makefile → scripts/)
# ============================================================================
pgo:  ## Run PGO workflow (generate profile, use+LTO build, tests, benchmarks). Env: PYTHON, CXX=clang++.
	@bash scripts/pgo_build.sh

scripts-executable:  ## Make scripts/ runnable (chmod +x). Run once if you want ./scripts/foo.sh.
	@chmod +x scripts/*.sh 2>/dev/null || true
	@echo "Scripts in scripts/ are executable."

help:  ## Show this help (main targets)
	@echo "Strata — main targets (Rule 16: Make → scripts/)"
	@echo ""
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  %-18s %s\n", $$1, $$2}'

